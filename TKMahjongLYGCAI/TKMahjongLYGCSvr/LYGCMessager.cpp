#include "LYGCMessager.h"
#include "LYGCPlay.h"
#include "LYGCGame.h"

LYGCMessager::LYGCMessager(LYGCGame* game)
{
    m_Game = game;
}

void LYGCMessager::SendPlayerTrustAck(ISeatID seat, ETrustType type, ISeatID sendTo)
{
    auto pPlayer = m_Game->GetPlayer(seat);
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongTrustPlayAck* pAck = pAckMsg->mutable_tkmahjongtrustplay_ack_msg();
    pAck->set_userid(pPlayer->GetUserID());
    pAck->set_type((int)type);
    m_Game->SendMsg(mobilemsg);
}

void LYGCMessager::SendDebugString(std::string const& str, ISeatID seat)
{
    TKMobileAckMsg mobilemsg;
    auto* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    auto* pAck = pAckMsg->mutable_tkmahjongdebuglog_ack_msg();
    pAck->set_msg(str);
    if (str.empty())
    {
        pAck->set_msg(m_Game->GetDebugStr());
        m_Game->ClearDebugStr();
    }
    m_Game->SendMsg(mobilemsg, seat);
}

void LYGCMessager::SendGameBeginAck()
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongGameBeginAck* pAck = pAckMsg->mutable_tkmahjonggamebegin_ack_msg();
    pAck->set_tilecount(m_Game->GetPlay()->GetDealer()->GetRestTileCount());
    pAck->set_svrversion(GetMJService()->GetVersion());
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        pAck->add_scores((score_t)m_Game->GetPlayer(i)->GetScore());
    }

    m_Game->SendMsg(mobilemsg);
}

void LYGCMessager::SendPlaceAck()
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongPlaceAck* pAck = pAckMsg->mutable_tkmahjongplace_ack_msg();
    pAck->set_banker(m_Game->GetPlay()->GetBanker());
    pAck->set_roundwind(m_Game->GetRoundData().firstBanker);
    pAck->set_lianzhuangtime(m_Game->GetRoundData().lianZhuang);
    pAck->set_jsonruler(m_Game->GetConfig()->ToCommaString());
    auto roundData = m_Game->GetRoundData();
    for (size_t i = 0; i < NP4; i++)
    {
        pAck->add_micount(roundData.miChi > 0 ? roundData.miCount[i] : 0);
    }

    m_Game->SendMsg(mobilemsg);
}

void LYGCMessager::SendOpenDoorAck()
{
    auto mahjong = m_Game->GetPlay();
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        TKMobileAckMsg mobilemsg;
        MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
        TKMahJongOpenDoorAck* pAck = pAckMsg->mutable_tkmahjongopendoor_ack_msg();
        auto playerData = mahjong->GetPlayData(i);
        pAck->set_seat(i);
        for (auto tile : playerData->GetHandTiles())
        {
            pAck->add_tiles(tile);
        }
        pAck->set_handstyle((int)playerData->GetHandStyle());
        m_Game->SendMsg(mobilemsg, i);
    }
}

void LYGCMessager::SendOpenFanCiAck()
{
    auto mahjong = m_Game->GetPlay();
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        TKMobileAckMsg mobilemsg;
        MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
        TKMahjongFanCiAck* pAck = pAckMsg->mutable_tkmahjongfanci_ack_msg();
        auto playerData = mahjong->GetPlayData(i);
        _SetAckCallData(pAck->mutable_calldata(), m_Game->GetPlay()->GetPlayData(i)->GetCallDataMap());
        pAck->set_tile(m_Game->GetPlay()->GetDealer<LYGCDealer>()->GetCiTile());
        m_Game->SendMsg(mobilemsg, i);
    }
}
void LYGCMessager::SendGenZhuangAck()
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahjongGenZhuangAck* pAck = pAckMsg->mutable_tkmahjonggenzhuang_ack_msg();

    m_Game->SendMsg(mobilemsg);
}
void LYGCMessager::SendOperatesAck(ISeatID seat, MJOperates operates, std::vector<ITileID> const& gangtiles, std::vector<ITileID> const& citiles)
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongActionAck* pAck = pAckMsg->mutable_tkmahjongaction_ack_msg();
    pAck->set_seat(seat);
    pAck->set_requesttype(operates.value);
    pAck->set_bmusthu(operates.isMustHu);
    pAck->set_capped(operates.capped);
    pAck->set_requestid(m_Game->UpdateRequestID(seat));
    for (auto tile : gangtiles)
    {
        pAck->add_gangtiles(tile);
    }
    for (auto tile : citiles)
    {
        pAck->add_citiles(tile);
    }
    m_Game->SendMsg(mobilemsg, seat);

    MJ_LOG_DEBUG(m_Game, "Send Operate: %d %d", seat, operates.value);
}

void LYGCMessager::SendKonAck(ISeatID konSeat, KonType konType)
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongGangAck* pAck = pAckMsg->mutable_tkmahjonggang_ack_msg();
    pAck->set_seat(konSeat);
    pAck->set_discardseat(m_Game->GetPlay()->GetCurrentSeat());
    pAck->set_tileid(_CurTile());
    pAck->set_gangtype((int)konType);

    if (konType == KonType::Bu)
    {
        if (m_Game->GetPlay()->IsKonAfterPon())
        {  // 碰后杠，状态为2
            pAck->set_gangtype((int)KonType::Zhi);

            pAck->set_discardseat(m_Game->GetPlay()->GetLastDisTileSeat());
        }
    }

    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        m_Game->SendMsg(mobilemsg, i);
    }
}

void LYGCMessager::SendPonAck(ISeatID ponSeat)
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongPengAck* pAck = pAckMsg->mutable_tkmahjongpeng_ack_msg();
    pAck->set_seat(ponSeat);
    pAck->set_discardseat(m_Game->GetPlay()->GetCurrentSeat());
    pAck->set_tileid(_CurTile());
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        pAck->clear_calldata();
        if (i == ponSeat)
        {
            _SetAckCallData(pAck->mutable_calldata(), m_Game->GetPlay()->GetPlayData(i)->GetCallDataMap());
        }

        m_Game->SendMsg(mobilemsg, i);
    }
}

void LYGCMessager::SendDiscardAck()
{
    auto seat = m_Game->GetPlay()->GetCurrentSeat();
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongDiscardTileAck* pAck = pAckMsg->mutable_tkmahjongdiscardtile_ack_msg();
    pAck->set_seat(seat);
    pAck->set_tileid(_CurTile());

    m_Game->SendMsg(mobilemsg);

    auto hands = MJ::GetTileAINameCvt()->IdsToNames(m_Game->GetPlay()->GetPlayData(seat)->GetHandTiles());
    MJ_LOG_DEBUG(m_Game, "%d hand:%s", (int)m_Game->GetPlayer(seat)->GetUserID(), _S(hands));
}

void LYGCMessager::SendDrawAck(const std::map<ITileID, int> mShowTiles)
{
    auto seat = m_Game->GetPlay()->GetCurrentSeat();
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongDrawTileAck* pAck = pAckMsg->mutable_tkmahjongdrawtile_ack_msg();
    pAck->set_seat(seat);
    auto type = m_Game->GetPlay()->IsAfterKonDraw() ? 2 : 1;
    pAck->set_drawtiletype(type);

    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        pAck->clear_recommendtileid();
        pAck->clear_calldata();
        if (i == seat)
        {
            pAck->set_tileid(_CurTile());
            _SetAckCallData(pAck->mutable_calldata(), m_Game->GetPlay()->GetPlayData(i)->GetCallDataMap());
            auto touch = _CheckTouchCard(pAck->calldata(), _CurTile(), mShowTiles);
            pAck->set_uitype(touch);
        }
        else
        {
            pAck->set_tileid(TILE_NULL);
        }

        m_Game->SendMsg(mobilemsg, i);
    }
}

void LYGCMessager::SendWinAck(WinAckData const& data)
{
    for (auto seat : data.winSeats)
    {
        _SetUntrustOnGameEnd(seat);
    }

    auto tile = _CurTile();
    //庄家直接胡牌
    if (data.winSeats.size() == 1 && data.winSeats.front() == m_Game->GetPlay()->GetBanker() && m_Game->GetPlay()->GetPlayData(data.winSeats.front())->IsTianDiHu())
    {
        tile = TILE_NULL;
    }

    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongWinAck* pAck = pAckMsg->mutable_tkmahjongwin_ack_msg();

    pAck->set_paoseat(data.paoSeat);
    pAck->set_wintileid(tile);
    pAck->set_winmode(data.winMode);

    for (auto winSeat : data.winSeats)
    {
        pAck->add_winseats(winSeat);
    }

    pAck->set_winseatcount(data.winSeats.size());

    m_Game->SendMsg(mobilemsg);  // 发送win信息
}

void LYGCMessager::SendTips(MJTips::Type nType, ISeatID seat)
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongMessageAck* pAck = pAckMsg->mutable_tkmahjongmessage_ack_msg();
    pAck->set_messagetype(nType);
    m_Game->SendMsg(mobilemsg, seat);
}

void LYGCMessager::SendHandTiles()
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahjongHandTilesAck* pAck = pAckMsg->mutable_tkmahjonghandtiles_ack_msg();
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        auto pMsg = pAck->add_playerhandtiles();
        _SetHandTIles(pMsg, i);
    }
    m_Game->SendMsg(mobilemsg);
}

void LYGCMessager::SendMahjongResult(bool isLiuJu, ISeatID paoSeat, ISeatID paoCiSeat)
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahJongResultAck* pAck = pAckMsg->mutable_tkmahjongresult_ack_msg();

    auto huSeats = m_Game->GetPlay()->GetHuSeats();
    ITileID winTile = _CurTile();
    if (huSeats.size() == 1 && huSeats.front() == m_Game->GetPlay()->GetBanker() && m_Game->GetPlay()->GetPlayData(huSeats.front())->IsTianDiHu())
    {
        winTile = TILE_NULL;
    }
    pAck->set_winmode(-1);
    pAck->set_zhuangseat(m_Game->GetPlay()->GetBanker());
    pAck->set_wintileid(winTile);
    pAck->set_paoseat(paoSeat);
    pAck->set_paociseat(paoCiSeat);
    pAck->set_scorebase(m_Game->GetScoreBase());
    pAck->set_curround(m_Game->m_iGameHand);
    pAck->set_citileid(m_Game->GetPlay()->GetDealer<LYGCDealer>()->GetCiTile());
    pAck->set_genzhuang(m_Game->GetPlay()->IsGenZhuang());
    auto miBei = m_Game->GetConfig()->GetValue(ECI_XiaMiBei) > 0 ? m_Game->GetConfig()->GetValue(ECI_XiaMiBei) : 1;
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        auto const& playData = m_Game->GetPlay()->GetPlayData(i);
        pAck->add_totalscore((int)m_Game->GetPlayer(i)->GetScoreChangeWithTax());
        pAck->add_showtotalscore((int)m_Game->GetPlayer(i)->GetScoreChange());
        pAck->add_konscore((int)m_Game->GetChecker()->GetOriginScore(i, MJScoreType::Gang));
        pAck->add_huscore((int)m_Game->GetChecker()->GetOriginScore(i, MJScoreType::Hu));

        pAck->add_micount((int)m_Game->GetChecker()->GetMultiple(i, MJScoreType::Mi) / miBei);
        auto pMsg = pAck->add_playerhandtiles();
        _SetHandTIles(pMsg, i);
    }
    for (auto winSeat : huSeats)
    {
        pAck->set_winmode(m_Game->GetPlay()->GetWinMode(winSeat));

        pAck->add_winseats(winSeat);
    }

    m_Game->SendMsg(mobilemsg);
    m_Game->CacheStateData(pAck);
}

enum FDCardType
{
    CardType_None = 0,
    CardType_Fei = 8,
    CardType_QiangAnGang = 7,
    CardType_Chi = 6,
    CardType_HandCard = 5,
    CardType_Peng = 4,
    CardType_BuGang = 3,
    CardType_MingGang = 2,
    CardType_AnGang = 1,
};

FDCardType ConvertKonType(KonType type)
{
    switch (type)
    {
    case KonType::Zhi:
        return CardType_MingGang;
    case KonType::An:
        return CardType_AnGang;
    case KonType::Bu:
        return CardType_BuGang;
    default:
        return CardType_None;
    }
}

void LYGCMessager::SendBeginAnimalAck()
{
    TKMobileAckMsg mobilemsg;
    MahJongLYGCAckMsg* pAckMsg = mobilemsg.mutable_mahjonglygc_ack_msg();
    TKMahjongBeginAnimalAck* pAck = pAckMsg->mutable_tkmahjongbeginanimal_ack_msg();
    pAck->set_requestid(m_Game->UpdateRequestID());
    m_Game->SendMsg(mobilemsg);
}

void LYGCMessager::_SetAckCallData(::proto::RepeatedPtrField<TKMahJongCallData>* pAckCallDatas, CallDataMap const& callData)
{
    for (auto const& calldata : callData)
    {
        auto pAckItem = pAckCallDatas->Add();
        pAckItem->set_tile(calldata.first);
        _SetAckCallTiles(pAckItem, calldata.second);
    }
}

void LYGCMessager::_SetAckCallTiles(TKMahJongCallData* pAckCallData, CallTileFan const& tileFans)
{
    for (auto const& huTile : tileFans)
    {
        auto pAckHuTile = pAckCallData->add_hutilesdetail();
        pAckHuTile->set_tile(huTile.first);
        pAckHuTile->set_fan(huTile.second);
    }
}

void LYGCMessager::_SetUntrustOnGameEnd(ISeatID seat)
{
    auto player = m_Game->GetPlayer(seat);
    if (player->IsTrusted())
    {
        player->SetTrusted(false);
    }
}

void LYGCMessager::_SetHandTIles(TKMahJongHandTiles* pAck, ISeatID seat)
{
    pAck->set_seat(seat);
    auto handCards = m_Game->GetPlay()->GetPlayData(seat)->GetHandTiles();
    auto ponTiles = m_Game->GetPlay()->GetPlayData(seat)->GetPonGroups();
    auto konTiles = m_Game->GetPlay()->GetPlayData(seat)->GetKonGroups();
    for (auto card : handCards)
    {
        pAck->add_anhandtile(card);
    }
    for (auto group : ponTiles)
    {
        auto pshowTiles = pAck->add_tilegroup();
        pshowTiles->set_grouptileid(group.tile);
        pshowTiles->set_groupfrom(group.from);
        pshowTiles->set_grouptype((int)EGroupType::Pon);
    }
    for (auto group : konTiles)
    {
        auto pshowTiles = pAck->add_tilegroup();
        pshowTiles->set_grouptileid(group.tile);
        pshowTiles->set_groupfrom(group.from);
        pshowTiles->set_grouptype((int)group.type + 3);
    }
}

ISeatID LYGCMessager::_CurSeat()
{
    return m_Game->GetPlay()->GetCurrentSeat();
}

ITileID LYGCMessager::_CurTile()
{
    return m_Game->GetPlay()->GetCurrentTile();
}

bool LYGCMessager::_CheckTouchCard(const proto::RepeatedPtrField<TKMahJongCallData>& pb, ITileID tile, const std::map<ITileID, int> mShowTiles)
{
    auto count = m_Game->GetPlay()->GetCurSeatTouchTimes();
    if (count >= 3)
    {
        return false;
    }

    for (auto& callData : pb)
    {
        if (callData.tile() == tile)
        {
            auto sTiles = MJ::GetTileAssociation()->GetTileAssociationCards(tile);
            for (auto& hutile : callData.hutilesdetail())
            {
                auto iter = mShowTiles.find(hutile.tile());
                if (iter != mShowTiles.end() && iter->second >= 4)
                {
                    return false;
                }
                if (hutile.tile() == tile && count < 3)
                {
                    m_Game->GetPlay()->SetCurSeatTouchTimes(++count);
                    return true;
                }
                if (MJ::HasElement(sTiles, hutile.tile()) && count < 2)
                {
                    m_Game->GetPlay()->SetCurSeatTouchTimes(++count);
                    return true;
                }
            }
        }
    }

    return false;
}
