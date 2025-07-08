#include "State.h"
#include "StateDiscard.h"
#include "StateResult.h"

void StateInit::OnEnter()
{
    GetMessager()->SendDebugString(GetGame()->GetConfig()->ToCommaString());

    GetPlay()->SetTipsHandler(std::bind(&LYGCMessager::SendTips, GetMessager(), std::placeholders::_1, std::placeholders::_2));
    GetPlay()->Initialize(GetGame()->GetRoundData().banker);

    GetMessager()->SendGameBeginAck();
    GetMessager()->SendPlaceAck();

    AsyncTimer(seconds(1), [this]() { EnterState<StateDeal>(); });
}

void StateDeal::OnEnter()
{
    auto pYKXPlay = GetPlay();
    pYKXPlay->DoDeal();

    GetMessager()->SendOpenDoorAck();
    GetMessager()->SendOpenFanCiAck();
    GetMessager()->SendBeginAnimalAck();
    AsyncTimer(seconds(5), [this]() { EnterState<StateDiscard>(); });
}
void StateDeal::OnReqRequest(ISeatID seat, MahJongLYGCReqMsg const& req)
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
    EnterState<StateDiscard>();
}

void StateDraw::OnEnter()
{
    ITileID tile = GetPlay()->DoDraw();
    GetMessager()->SendDebugString({}, GetPlay()->GetCurrentSeat());
    if (m_Game->GetConfig()->GetValue(ECI_GenZhuang) && GetPlay()->CheckCurGenZhuang())
    {
        GetMessager()->SendGenZhuangAck();
    }

    if (tile == TILE_NULL /*|| GetPlay()->GetDealer()->GetRestTileCount() < 78*/)
    {
        EnterState<StateResultLiuJu>();
        return;
    }
    if (GetPlay()->GetDealer()->GetRestTileCount() == 10)
    {
        GetMessager()->SendTips(MJTips::BaoCiState);
    }
    std::map<ITileID, int> mShowTiles;
    GetPlay()->GetShowTilesInfo(mShowTiles, GetPlay()->GetCurrentSeat());
    mShowTiles[GetPlay()->GetDealer<LYGCDealer>()->GetCiTile()]++;
    //刚摸到牌张不再范围内
    --mShowTiles[tile];
    GetMessager()->SendDrawAck(mShowTiles);
    EnterState<StateDiscard>();
}
