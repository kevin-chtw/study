#include "StateDiscard.h"
#include "StateWait.h"
#include "StateResult.h"
#include "MJException.h"

StateDiscard::StateDiscard()
{
}

void StateDiscard::OnEnter()
{
    m_Operates = GetPlay()->FetchSelfOperates();
    if (GetGame()->GetPlayer(GetPlay()->GetCurrentSeat())->IsTrusted())
    {
        this->AsyncTimer(seconds(1), [this]() { this->AutoOperate(false); });
    }
    else if (GetPlay()->GetPlayData(GetPlay()->GetCurrentSeat())->m_Call && m_Operates.value == 0)
    {
        AsyncTimer(milliseconds(700), [this]() { this->Discard(TILE_NULL); });
    }
    else
    {
        auto playData = GetPlay()->GetPlayData<LYGCPlayData>(GetPlay()->GetCurrentSeat());
        GetMessager()->SendOperatesAck(GetPlay()->GetCurrentSeat(), m_Operates, playData->GetGangTiles(), playData->GetCiTiles());
        auto timeout = GetPlay()->GetConfigValue(ECI_DiscardTime) + 1;
        this->AsyncMsgTimer(MSG_HANDLER(StateDiscard::OnReqDiscard), seconds(GetPlay()->GetConfigValue(ECI_DiscardTime)), [this]() { this->OnTimeout(); });
    }
}

void StateDiscard::OnReqDiscard(ISeatID seat, MahJongLYGCReqMsg const& req)
{
    if (seat != GetPlay()->GetCurrentSeat())
    {
        return;
    }
    if (req.has_tkmahjongrequst_req_msg())
    {
        auto msg = req.tkmahjongrequst_req_msg();
        if (!GetGame()->IsRequestID(seat, msg.requestid()))
        {
            return;
        }
        auto operate = (EOperate)msg.requesttype();
        if (!m_Operates.HasOperate(operate) && operate != EOperatePass)
        {
            return;
        }
        if (operate == EOperateHu || operate == EOperateCI)
        {
            this->Hu(operate, msg.tileid());
        }
        else if (operate == EOperateKon)
        {
            this->Kon(msg.tileid());
        }
        else if (operate == EOperateDiscard)
        {
            this->Discard(msg.tileid());
        }
        else if (operate == EOperatePass)
        {
            this->Discard(TILE_NULL);
        }
        else
        {
            this->AutoOperate(false);
        }
    }
}

void StateDiscard::AutoOperate(bool isTimeout)
{
    if (m_Operates.HasOperate(EOperateCI))
    {
        auto vCiTiles = GetLYGCPlayData(GetPlay()->GetCurrentSeat())->GetCiTiles();
        if (vCiTiles.empty())
        {
            throw MJ_MAKE_EXCEPTION("error citilecount");
        }
        ISeatID seat = SEAT_NULL;
        auto ciType = GetLYGCPlayData(GetPlay()->GetCurrentSeat())->GetCiType(vCiTiles.at(MJ::Random(vCiTiles.size())), seat);

        EnterState<StateResultSelfHu>(ciType, seat);
    }
    else if (m_Operates.HasOperate(EOperateHu))
    {
        EnterState<StateResultSelfHu>(EHT_ZiMo, SEAT_ALL);
    }
    else if (m_Operates.HasOperate(EOperateDiscard))
    {
        GetMessager()->SendPlayerTrustAck(GetPlay()->GetCurrentSeat(), ETrustType::TimeOut);
        GetGame()->GetPlayer(GetPlay()->GetCurrentSeat())->SetTrusted(true);

        this->Discard(TILE_NULL);
    }
    else
    {
        throw MJ_MAKE_EXCEPTION("error operates");
    }
}

void StateDiscard::Discard(ITileID tile)
{
    GetPlay()->DoDiscard(tile);
    GetMessager()->SendDiscardAck();
    EnterState<StateWait>();
}

void StateDiscard::Kon(ITileID tile)
{
    if (GetPlay()->DoTryKon(tile, KonType::Bu))
    {
        GetMessager()->SendKonAck(GetPlay()->GetCurrentSeat(), KonType::Bu);
        EnterState<StateDraw>();
        return;
    }
    else if (GetPlay()->DoTryKon(tile, KonType::An))
    {
        GetMessager()->SendKonAck(GetPlay()->GetCurrentSeat(), KonType::An);
        EnterState<StateDraw>();
    }
}

void StateDiscard::Hu(EOperate operate, ITileID tile)
{
    if (operate == EOperateHu)
    {
        EnterState<StateResultSelfHu>(EHT_ZiMo, SEAT_ALL);
    }
    else
    {
        ISeatID seat = SEAT_NULL;
        auto ciType = GetLYGCPlayData(GetPlay()->GetCurrentSeat())->GetCiType(tile, seat);
        if (ciType != EHT_NONE)
        {
            EnterState<StateResultSelfHu>(ciType, seat);
        }
    }
}

void StateDiscard::OnTimeout()
{
    if (m_Game->IsFDTable())
    {
        return;
    }
    this->AutoOperate(true);
}
