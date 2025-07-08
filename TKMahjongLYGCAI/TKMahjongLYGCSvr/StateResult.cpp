#include "StateResult.h"
#include "MJScore.h"

void StateResult::OnReqRequest(ISeatID seat, MahJongLYGCReqMsg const& req)
{
    if (!req.has_tkmahjongendanimal_req_msg())
    {
        return;
    }

    auto msg = req.tkmahjongendanimal_req_msg();

    if (!GetGame()->IsRequestID(seat, msg.requestid()) || msg.seat() != seat)
    {
        return;
    }
    this->PrepareNextState();
}

void StateResult::PrepareNextState()
{
    GetGame()->FinishGame();
}

void StateResult::OnGameEnd(bool isLiuJu)
{
    GetGame()->UpdateRoundData();
    GetMessager()->SendMahjongResult(isLiuJu, m_PaoSeat, m_PaoCiSeat);
    GetGame()->SyncResult();

    GetGame()->GetChecker()->SyncAllPlayerScore();
    GetPlay()->SetPlayersAtomEvents();
}

void StateResult::WaitClientAnimateDone()
{
    GetMessager()->SendBeginAnimalAck();
    static constexpr int longTimeout = 10;
    this->AsyncMsgTimer(MSG_HANDLER(StateResult::OnReqRequest), seconds(longTimeout), [this]() { this->PrepareNextState(); });
}

void StateResult::_OnPlayerHu(std::vector<int64_t>& multiples, ISeatID paoSeat, EHuType huType)
{
    GetGame()->GetChecker()->AddMultiple(MJScoreType::Hu, multiples);
    GetGame()->GetChecker()->AddMultiple(MJScoreType::Gang, GetPlay()->DoCheckGangMultiple());
    GetGame()->GetChecker()->AddMultiple(MJScoreType::GenZhuang, GetPlay()->DoCheckGenZhuangMultiple());

    GetGame()->GetChecker()->AddMultiple(MJScoreType::Mi, GetPlay()->DoCheckMiMultiple(GetGame()->GetRoundData(), huType));

    GetGame()->GetChecker()->Check();

    for (auto seat : m_HuSeats)
    {
        GetGame()->GetPlayer(seat)->SetOut(MJPlayerStatus::Hu, (paoSeat == SEAT_NULL ? EHuResultMode::ZiMo : EHuResultMode::PaoHu));
    }
    GetGame()->GetChecker()->SetRestLoseOut();

    WinAckData data;
    data.paoSeat = paoSeat;
    data.winMode = huType;
    data.winSeats = m_HuSeats;
    data.isGrabKon = m_QiangGang;
    GetMessager()->SendWinAck(data);
    this->OnGameEnd();
    this->WaitClientAnimateDone();
}

void StateResultLiuJu::_OnPlayerLiuJu()
{
    GetGame()->GetChecker()->AddMultiple(MJScoreType::GenZhuang, GetPlay()->DoCheckGenZhuangMultiple());

    GetGame()->GetChecker()->AddMultiple(MJScoreType::Mi, GetPlay()->DoCheckMiMultiple(GetGame()->GetRoundData(), EHT_NONE));

    GetGame()->GetChecker()->Check();

    // GetMessager()->SendHandTiles();
    this->OnGameEnd(true);
    this->WaitClientAnimateDone();
}

void StateResultLiuJu::OnEnter()
{
    GetGame()->GetChecker()->SetRestLoseOut();
    GetPlay()->SetNextBanker(MJGetNextSeat(GetPlay()->GetBanker()));
    _OnPlayerLiuJu();
}

StateResultPaoHu::StateResultPaoHu(std::vector<ISeatID> const& huSeats, bool isGrabKon)
{
    m_HuSeats = huSeats;
    m_QiangGang = isGrabKon;
}

void StateResultPaoHu::OnEnter()
{
    auto paoSeat = GetPlay()->GetCurrentSeat();
    std::vector<int64_t> sumTimes(m_Game->GetIniPlayerCount(), 0);
    auto huType = EHT_MingCi;
    if (GetPlay()->GetDealer()->GetRestTileCount() <= 10)
    {
        huType = EHT_BaoCi;
    }
    for (auto seat : m_HuSeats)
    {
        auto multiples = GetPlay()->GetHuMultiple(huType, paoSeat, seat);
        sumTimes = multiples;
    }
    if (m_HuSeats.size() > 0)
    {
        GetPlay()->SetNextBanker(m_HuSeats.front());
    }
    m_PaoSeat = paoSeat;
    m_PaoCiSeat = paoSeat;
    _OnPlayerHu(sumTimes, paoSeat, huType);
}

void StateResultSelfHu::OnEnter()
{
    auto seat = GetPlay()->GetCurrentSeat();

    auto multiples = GetPlay()->GetHuMultiple(m_HuType, m_DianCiSeat, seat);

    m_HuSeats = {seat};
    GetPlay()->SetNextBanker(seat);
    if (m_HuType == EHT_MingCi)
    {
        m_PaoCiSeat = m_DianCiSeat;
    }

    _OnPlayerHu(multiples, SEAT_NULL, m_HuType);
}

StateResultSelfHu::StateResultSelfHu(EHuType huType, ISeatID nSeat) : m_HuType(huType), m_DianCiSeat(nSeat)
{
}
