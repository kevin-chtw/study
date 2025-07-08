#include "StateWait.h"
#include "StateDiscard.h"
#include "StateResult.h"

static constexpr EOperate ValidOperates[] = {EOperateCI, EOperateHu, EOperateKon, EOperatePon, EOperatePass};

void StateWait::OnEnter()
{
    auto discardSeat = GetPlay()->GetCurrentSeat();
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        if (i == discardSeat)
        {
            continue;
        }
        std::vector<ITileID> vGangTiles{};
        bool trusted = GetGame()->GetPlayer(i)->IsTrusted();
        auto operates = GetPlay()->FetchWaitOperates(i);

        m_OperatesForSeats.at(i) = operates;

        if (operates.value != 0 && !trusted)
        {
            GetMessager()->SendOperatesAck(i, operates, GetPlay()->GetPlayData(i)->GetGangTiles(), GetPlay()->GetPlayData<LYGCPlayData>(i)->GetCiTiles());
        }
        else
        {
            SetRequestAction(i, _GetDefaultOperate(i));
        }
    }

    auto timeout = GetPlay()->GetConfigValue(ECI_WaitTime) + 1;
    auto onTimeout = [this]() { this->Timeout(); };
    this->AsyncMsgTimer(MSG_HANDLER(StateWait::OnReqRequest), seconds(timeout), onTimeout);

    TryHandleAction();
}

void StateWait::OnReqRequest(ISeatID seat, MahJongLYGCReqMsg const& req)
{
    if (m_RequestStatus.test(seat) || m_OperatesForSeats.at(seat).value == 0)
    {
        return;
    }
    if (!req.has_tkmahjongrequst_req_msg())
    {
        return;
    }
    auto& msg = req.tkmahjongrequst_req_msg();
    if (!msg.has_requestid() || !GetGame()->IsRequestID(seat, msg.requestid()))
    {
        return;
    }

    EOperate operate = _GetDefaultOperate(seat);
    if (msg.has_requesttype() && _IsValidOperate(seat, (EOperate)msg.requesttype()))
    {
        if ((EOperate)msg.requesttype() == EOperatePon && !GetLYGCPlayData(seat)->CanPon(GetPlay()->GetCurrentTile(), false))
        {
            return;
        }
        if ((EOperate)msg.requesttype() == EOperateKon && !GetLYGCPlayData(seat)->CanZhiKon(GetPlay()->GetCurrentTile(), GetGame()->GetConfig()))
        {
            return;
        }
        operate = (EOperate)msg.requesttype();
    }

    this->SetRequestAction(seat, operate);
    TryHandleAction();
}

void StateWait::ToDrawState(ISeatID seat)
{
    GetPlay()->DoSwitchSeat(seat);
    EnterState<StateDraw>();
}

void StateWait::ToDiscardState(ISeatID seat)
{
    GetPlay()->DoSwitchSeat(seat);
    EnterState<StateDiscard>();
}

void StateWait::TryHandleAction()
{
    auto curSeat = GetPlay()->GetCurrentSeat();

    // step1: handle Hu
    std::vector<ISeatID> huSeats;

    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        auto seat = MJGetNextSeat(curSeat, i, m_Game->GetIniPlayerCount());
        if (m_RequestStatus.test(seat))
        {  //seat 已请求
            if (m_ReqOperateForSeats.at(seat) == EOperateCI)
            {
                huSeats.push_back(seat);
            }
        }
        else
        {
            if (_GetMaxOperate(seat) == EOperateHu)
            {  //有胡牌玩家未请求，继续等
                return;
            }
        }
    }
    if (!huSeats.empty())
    {
        ExcuteActionHu(huSeats);
        return;
    }

    EOperate maxOper = EOperatePass;
    ISeatID maxOperSeat = SEAT_NULL;
    bool isMaxRequested = true;

    for (int i = 1; i < m_Game->GetIniPlayerCount(); ++i)
    {
        auto seat = MJGetNextSeat(curSeat, i, m_Game->GetIniPlayerCount());
        if (m_RequestStatus.test(seat))
        {  //seat 已请求
            if (maxOper < m_ReqOperateForSeats.at(seat))
            {
                maxOper = m_ReqOperateForSeats.at(seat);
                maxOperSeat = seat;
                isMaxRequested = true;
            }
        }
        else
        {  //seat 尚未请求
            auto mightMaxOper = _GetMaxOperate(seat);
            if (maxOper < mightMaxOper)
            {
                maxOper = mightMaxOper;
                maxOperSeat = seat;
                isMaxRequested = false;
            }
        }
    }
    if (isMaxRequested)
    {
        ExcuteAction(maxOperSeat, maxOper);
    }
}

void StateWait::ExcuteAction(ISeatID seat, EOperate operate)
{
    if (operate == EOperateKon)
    {
        GetPlay()->DoZhiKon(seat);
        GetMessager()->SendKonAck(seat, KonType::Zhi);

        ToDrawState(seat);
        return;
    }
    if (operate == EOperatePon)
    {
        GetPlay()->DoPon(seat);
        GetMessager()->SendPonAck(seat);
        ToDiscardState(seat);
        return;
    }
    // All Pass.
    ToDrawState();
}

void StateWait::ExcuteActionHu(std::vector<ISeatID> const& huSeats)
{
    GetPlay()->DoDianPao(huSeats);
    EnterState<StateResultPaoHu>(huSeats);
}

bool StateWait::_IsValidOperate(ISeatID seat, EOperate operate)
{
    if (operate == EOperatePass)
    {
        return true;
    }
    return m_OperatesForSeats.at(seat).HasOperate(operate) && MJ::HasElement(ValidOperates, operate);
}

int StateWait::_GetMaxOperate(ISeatID seat)
{
    auto ops = m_OperatesForSeats.at(seat);
    for (auto operate : ValidOperates)
    {
        if (ops.HasOperate(operate))
        {
            return operate;
        }
    }
    return EOperatePass;
}

int StateWait::_GetDefaultOperate(ISeatID seat)
{
    auto ops = m_OperatesForSeats.at(seat);
    return ops.HasOperate(EOperateCI) ? EOperateCI : EOperatePass;
}

void StateWait::Timeout()
{
    if (m_Game->IsFDTable())
    {
        return;
    }
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        if (!m_RequestStatus.test(i))
        {
            if (m_OperatesForSeats.at(i).value != 0)
            {
                this->SetRequestAction(i, _GetDefaultOperate(i));

                GetMessager()->SendPlayerTrustAck(i, ETrustType::TimeOut);
                GetGame()->GetPlayer(i)->SetTrusted(true);
            }
        }
    }
    TryHandleAction();
}

void StateWait::SetRequestAction(ISeatID seat, EOperate operate)
{
    m_RequestStatus.set(seat);
    if (operate == EOperatePass)
    {
        GetPlay()->Pass(seat, m_OperatesForSeats.at(seat));
    }
    m_ReqOperateForSeats.at(seat) = operate;
}
