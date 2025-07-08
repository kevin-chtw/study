#include "LYGCPlay.h"
#include "LYGCGame.h"
#include "LYGCPlayData.h"
#include "LYGCHu.h"

LYGCPlay::LYGCPlay(LYGCGame* game) : MJPlayEx(game)
{
}

std::shared_ptr<MJPlayDataEx> LYGCPlay::OnCreatePlayData(ISeatID seat)
{
    return std::make_shared<LYGCPlayData>(m_Game);
}

std::shared_ptr<MJ::Dealer> LYGCPlay::OnCreateDealer()
{
    ISeatID newbieSeat = m_Game->GetNewbieSeat();
    if (newbieSeat == SEAT_NULL)
    {
        return std::make_shared<LYGCDealer>(m_Game);
    }

    return std::make_shared<LYGCNBDealer>(m_Game, newbieSeat);
}

void LYGCPlay::_Initialize()
{
    m_PlayConf.m_HasPon = true;
    m_PlayConf.m_PonPass = false;
    m_PlayConf.m_HuPass = true;
    m_PlayConf.m_MinMultipleLimit = 0;
    m_PlayConf.m_BuKonPass = true;
    m_PlayConf.m_ZhiKonAfterPon = true;

    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        auto curPlayData = GetPlayData(i);
        curPlayData->SetDrawConfig(0, GetConfigValue(ECI_DrawCtrl));
    }
    m_bGenZhuang = false;
}

std::vector<int> LYGCPlay::_ZiMoExtraFan(ISeatID seat)
{
    std::vector<int> extraFan;

    extraFan.push_back(EHT_ZiMo);

    return extraFan;
}

bool LYGCPlay::_PaoExtraFan(ISeatID seat, std::vector<int>& extraFan)
{
    return false;
}
std::vector<int> LYGCPlay::_QiangGangExtraFan(ISeatID seat)
{
    return std::vector<int>{};
}

int LYGCPlay::GetWinMode(ISeatID seat) const
{
    auto huinFo = m_HuResults.at(seat);
    if (huinFo.huTypes.empty())
    {
        return EHT_NONE;
    }
    return huinFo.huTypes.front();
}

void LYGCPlay::_CheckPaoHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, std::vector<MJTips::Type>& tips)
{
    auto playData = GetPlayData<LYGCPlayData>(seat);
    MJTips::Type tip = MJTips::None;
    if (playData->IsPassHuTile(m_CurrentTile, tip) && m_PlayConf.m_HuPass)
    {
        if (playData->IsCanTips(tip))
        {
            tips.push_back(tip);
            playData->SetCanTips(tip);
        }
        return;
    }
    _AddHuOperate(opt, seat, result);
}

std::vector<int64_t> LYGCPlay::DoCheckGangMultiple()
{
    int nPlayerCount = m_Game->GetIniPlayerCount();
    std::vector<int64_t> multiples(nPlayerCount, {});
    for (int i = 0; i < nPlayerCount; ++i)
    {
        const auto& konGroup = GetPlayData(i)->GetKonGroups();
        for (auto kon : konGroup)
        {
            int64_t multi = 1;
            std::vector<ISeatID> vReleateSeat{};
            if (kon.type == KonType::An)
            {
                vReleateSeat = {0, 1, 2, 3};  // 三家出
            }
            else
            {
                vReleateSeat = {kon.from};  // 一家出
            }
            MJScoreValueChange(multiples, multi, i, vReleateSeat);  //
        }
    }
    return multiples;
}

std::vector<int64_t> LYGCPlay::DoCheckLiuJuMultiple()
{
    std::vector<int64_t> multiples = DoCheckGangMultiple();
    for (auto& it : multiples)
    {
        it = -it;
    }

    return multiples;
}

std::vector<int64_t> LYGCPlay::DoCheckGenZhuangMultiple()
{
    int nPlayerCount = m_Game->GetIniPlayerCount();
    std::vector<int64_t> multiples(nPlayerCount, 0);
    if (m_bGenZhuang)
    {
        for (size_t i = 0; i < nPlayerCount; i++)
        {
            if (i == m_Banker)
            {
                multiples.at(i) = -3;
            }
            else
            {
                multiples.at(i) = 1;
            }
        }
    }

    return multiples;
}

std::vector<int64_t> LYGCPlay::DoCheckMiMultiple(LYGCLastHandData& data, EHuType huType)
{
    int nPlayerCount = m_Game->GetIniPlayerCount();
    std::vector<int64_t> multiples(nPlayerCount, 0);
    if (m_Game->GetConfig()->GetValue(ECI_XiaMi) == 0)
    {
        return multiples;
    }
    auto huSeat = SEAT_NULL;
    if (!m_HuSeats.empty())
    {
        huSeat = m_HuSeats.front();
    }
    if (huType > EHT_ZiMo && huType < EHT_END)
    {  //次胡
        if (!m_Game->IsValidSeat(huSeat))
        {
            return multiples;
        }

        if (data.miChi > 0)
        {
            for (size_t i = 0; i < nPlayerCount; i++)
            {
                multiples[huSeat] += data.miCount[i];
                multiples[i] -= data.miCount[i];
            }

            data.miChi = 0;
        }
        else if (data.miCount[huSeat] > 0)
        {
            multiples[huSeat] += data.miCount[huSeat];
            multiples[data.lastXiaMiSeat] -= data.miCount[huSeat];
        }
    }

    if (data.lastHuType > EHT_ZiMo && data.lastHuType < EHT_END)
    {  //上局次 本局清理之前下米信息
        memset(data.miCount, 0, sizeof(data.miCount));
    }
    if (m_HuSeats.empty())
    {
        if (m_Game->IsValidSeat(data.lastXiaMiSeat))
        {  //下米
            data.miCount[data.lastXiaMiSeat] += 1;
            data.miChi += 1;
        }
    }
    else
    {
        //谁胡牌谁下米了
        data.lastXiaMiSeat = m_HuSeats.front();
    }

    data.lastHuType = huType;

    for (auto& mifen : multiples)
    {
        mifen *= m_Game->GetConfig()->GetValue(ECI_XiaMiBei);
    }
    return multiples;
}

MJOperates LYGCPlay::FetchSelfOperates()
{
    MJOperates operates;
    std::vector<MJTips::Type> tips;
    operates.AddOperate(EOperateDiscard);
    if (IsAfterPon())
    {  //碰后只能出牌

        return operates;
    }

    if (GetDealer()->GetRestTileCount() > 0)
    {
        _CheckSelfKon(operates, m_CurrentSeat);
    }

    _CheckSelfCi(operates, m_CurrentSeat, tips);

    if (!operates.HasOperate(EOperateCI) && (m_Game->GetConfig()->GetValue(ECI_CiPai) == 0 || GetDealer()->GetRestTileCount() < m_Game->GetConfig()->GetValue(ECI_MoBomb)))
    {  //可以胡
        _CheckSelfHu(operates, m_CurrentSeat, tips);
    }
    if (tips.size())
    {
        _OnTips(tips.front(), m_CurrentSeat);
    }

    return operates;
}

MJOperates LYGCPlay::FetchWaitOperates(ISeatID seat)
{
    auto operates = MJPlayEx::FetchWaitOperates(seat);

    if (GetPlayData<LYGCPlayData>(seat)->CanWaitCi(m_Game->GetConfig(), GetDealer<LYGCDealer>()->GetCiTile(), m_CurrentTile))
    {
        operates.AddOperate(EOperateCI);
    }
    return operates;
}

void LYGCPlay::_CheckSelfCi(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips)
{
    if (GetPlayData<LYGCPlayData>(seat)->CanSelfCi(m_Game->GetConfig(), GetDealer<LYGCDealer>()->GetCiTile()))
    {
        opt.AddOperate(EOperateCI);
    }
}
std::vector<int64_t> LYGCPlay::GetHuMultiple(EHuType huType, ISeatID nDianCiSeat, ISeatID huSeat)
{
    if (huType == EHT_NONE || huType >= EHT_END)
    {
        return {};
    }
    m_HuSeats.push_back(huSeat);
    AddHistory(huSeat, m_CurrentTile, huType);

    std::vector<int64_t> multiples(m_Game->GetIniPlayerCount(), {});

    auto& huResult = m_HuResults.at(huSeat);
    huResult.huTypes.push_back(huType);
    auto multi = 0;
    switch (huType)
    {
    case EHT_ZiMo:
        multi = 1;
        break;
    case EHT_BaoCi:
    case EHT_MingCi:
        multi = m_Game->GetConfig()->GetValue(ECI_CiHuBei) - 1;
        multiples.at(nDianCiSeat) -= 1;
        multiples.at(huSeat) += 1;
        break;
    case EHT_AnCi:
        multi = m_Game->GetConfig()->GetValue(ECI_CiHuBei);
        break;
    case EHT_PiCi:
        multi = m_Game->GetConfig()->GetValue(ECI_PiCiBei);
        break;
    default:
        break;
    }

    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        if (m_Game->GetPlayer(i)->IsOut() || i == huSeat)
        {
            continue;
        }
        auto nFan = multi;
        if (i == m_Banker || huSeat == m_Banker)
        {  //庄
            nFan += 1;
        }
        auto nPaySeat = i;
        if (huType == EHT_BaoCi)
        {  //包
            nPaySeat = nDianCiSeat;
        }

        multiples.at(nPaySeat) -= nFan;
        multiples.at(huSeat) += nFan;
    }
    _handTianHu(huSeat);

    return multiples;
}

void LYGCPlay::_handTianHu(const ISeatID& huSeat)
{
    if (GetPlayData(huSeat)->IsTianDiHu() && m_Banker == huSeat)
    {
        GetPlayData(huSeat)->PickTianHuTile();
        m_CurrentTile = GetPlayData(huSeat)->GetHandTiles().back();
    }
}

bool LYGCPlay::CheckCurGenZhuang()
{
    if (m_bGenZhuang)
    {
        return false;
    }
    int nPlayerCount = m_Game->GetIniPlayerCount();
    ITileID outTile{TILE_NULL};
    for (size_t i = 0; i < nPlayerCount; i++)
    {
        auto playeData = GetPlayData<LYGCPlayData>(i);
        if (playeData->GetOutTiles().size() != 1)
        {
            return false;
        }
        if (playeData->GetEverChiCount() + playeData->GetEverPonCount() + playeData->GetEverKonCount() > 0)
        {
            return false;
        }
        if (outTile == TILE_NULL)
        {
            outTile = playeData->GetOutTiles().front();
        }
        if (outTile != playeData->GetOutTiles().front())
        {
            return false;
        }
    }

    m_bGenZhuang = true;
    return true;
}

int LYGCPlay::GetCurSeatTouchTimes()
{
    return m_mTouchTileTimes[m_CurrentSeat];
}
void LYGCPlay::SetCurSeatTouchTimes(int Times)
{
    m_mTouchTileTimes[m_CurrentSeat] = Times;
}