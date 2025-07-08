#include "MJPlayEx.h"
#include "MJUtils.h"
#include "MJGame.h"
#include "MJService.h"

int64_t SPlayConf::GetRealMultiple(int64_t mult) const
{
    if (m_MaxMultipleLimit <= 0)
    {
        return mult;
    }
    return mult >= m_MaxMultipleLimit ? m_MaxMultipleLimit : mult;
}

bool SPlayConf::IsTopMultiple(int64_t mult) const
{
    if (m_MaxMultipleLimit <= 0)
    {
        return false;
    }
    return mult >= m_MaxMultipleLimit;
}

MJPlayEx::MJPlayEx(MJGame* game) : MJPlay(game)
{
}

void MJPlayEx::Initialize()
{
    m_HuResults.assign(m_Game->GetIniPlayerCount(), {});

    m_Dealer = OnCreateDealer();

    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        m_PlayDatas.push_back(OnCreatePlayData(i));
    }
}

std::shared_ptr<MJPlayDataEx> MJPlayEx::OnCreatePlayData(ISeatID seat)
{
    return std::make_shared<MJPlayDataEx>(m_Game);
}

std::shared_ptr<MJ::Dealer> MJPlayEx::OnCreateDealer()
{
    return nullptr;
}

void MJPlayEx::SetTipsHandler(std::function<void(MJTips::Type, ISeatID)> handler)
{
    m_TipsHandler = handler;
}

void MJPlayEx::Initialize(int banker)
{
    m_Banker = banker;
    m_CurrentSeat = banker;
    m_Dealer->Initialize();
    this->_Initialize();
}

void MJPlayEx::DoDeal()
{
    int handCount = GetMJService()->GetHandCount();
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        int takeCount = i == 0 ? handCount + 1 : handCount;
        auto seat = (i + m_Banker) % m_Game->GetIniPlayerCount();
        auto play = m_PlayDatas.at(seat);
        for (int n = 0; n < takeCount; ++n)
        {
            play->Draw(m_Dealer->DrawTile(seat));
        }
        FreshCallData(seat);
    }
}

void MJPlayEx::DoSwitchSeat(ISeatID seat)
{
    if (m_Game->IsValidSeat(m_CurrentSeat))
    {
        m_PlayDatas.at(m_CurrentSeat)->ClearDraw();
    }
    MJPlay::DoSwitchSeat(seat);  //m_CurrentSeat changed
    m_PlayDatas.at(m_CurrentSeat)->ClearPass();
}

ITileID MJPlayEx::DoDraw()
{
    auto curPlayerData = m_PlayDatas.at(m_CurrentSeat);
    m_CurrentTile = m_Dealer->DrawTile(m_CurrentSeat, curPlayerData.get());
    if (MJIsValidTile(m_CurrentTile))
    {
        curPlayerData->Draw(m_CurrentTile);

        AddHistory(m_CurrentSeat, m_CurrentTile, EOperateDraw);
        FreshCallData(m_CurrentSeat);
    }
    return m_CurrentTile;
}

bool MJPlayEx::DoTryChow(ISeatID seat, ITileID leftTile)
{
    auto playData = m_PlayDatas.at(seat);
    if (!playData->CanChow(leftTile, m_CurrentTile))
    {
        MJ_LOG_ERROR(m_Game, "client try chow:%d, curtile:%d", leftTile, m_CurrentTile);
        return false;
    }

    playData->Chow(m_CurrentTile, leftTile, m_CurrentSeat);
    m_PlayDatas.at(m_CurrentSeat)->RemoveOutTile();
    AddHistory(seat, m_CurrentTile, EOperateChow);
    FreshCallData(seat);

    return true;
}

void MJPlayEx::DoPon(ISeatID seat)
{
    auto playData = m_PlayDatas.at(seat);
    if (!playData->CanPon(m_CurrentTile, m_PlayConf.m_CanotOnlyLaiAfterPon))
    {
        throw MJ_MAKE_EXCEPTION("pon error");
    }
    m_CurrentTile = playData->Pon(m_CurrentTile, m_CurrentSeat);
    m_PlayDatas.at(m_CurrentSeat)->RemoveOutTile();
    AddHistory(seat, m_CurrentTile, EOperatePon);
    FreshCallData(seat);
}

void MJPlayEx::DoZhiKon(ISeatID seat)
{
    auto playData = m_PlayDatas.at(seat);
    if (!playData->CanKon(m_CurrentTile, KonType::Zhi))
    {
        throw MJ_MAKE_EXCEPTION("zhi kon error");
    }
    m_CurrentTile = playData->KonS(m_CurrentTile, KonType::Zhi, m_CurrentSeat);
    m_PlayDatas.at(m_CurrentSeat)->RemoveOutTile();

    AddHistory(seat, m_CurrentTile, EOperateKon, (int)KonType::Zhi);
    FreshCallData(seat);
}

bool MJPlayEx::DoTryKon(ITileID tile, KonType type)
{
    auto playData = m_PlayDatas.at(m_CurrentSeat);
    if (!playData->CanKon(tile, type))
    {
        MJ_LOG_ERROR(m_Game, "client try kon:%d[%d], hands:%s", tile, (int)type, MJ::ToString(playData->GetHandTiles()).c_str());
        return false;
    }
    bool buAfterPon = false;
    bool pass = false;
    if (type == KonType::Bu)
    {
        if (m_PlayConf.m_ZhiKonAfterPon && IsKonAfterPonBefor(tile))
        {
            buAfterPon = true;
        }
        else if (m_PlayConf.m_BuKonPass && (!IsPonThisTileCurHand(tile, m_CurrentSeat) && !playData->IsCurrentDrawTile(tile)))
        {
            pass = true;
        }
    }

    m_CurrentTile = playData->KonS(tile, type, m_CurrentSeat, pass, buAfterPon);
    AddHistory(m_CurrentSeat, m_CurrentTile, EOperateKon, (int)_FixKonTypeAfterPon(type, tile));

    FreshCallData(m_CurrentSeat);
    return true;
}

bool MJPlayEx::DoTryCall(ITileID tile, ISeatID seat)
{
    auto playData = m_PlayDatas.at(seat);

    if (playData->GetHandTiles().size() % 3 == 1)
    {
        tile = TILE_NULL;
    }
    else if (playData->GetHandTiles().size() % 3 == 2 && !playData->HasTile(tile))
    {
        return false;
    }

    auto input = playData->MakeHuPlayData(TILE_NULL, tile);
    auto callInfo = GetMJService()->GetHuTool()->CheckCall(input, m_Game->GetConfig());
    if (callInfo.empty())
    {
        return false;
    }
    bool tianTing = m_PlayConf.m_TianTing && _IsTianTing(seat);
    playData->SetCall(tile, tianTing);
    if (tile != TILE_NULL)
    {
        playData->CloseTianDiHu();
        m_CurrentTile = tile;
    }
    auto operate = tianTing ? EOperateTianTing : EOperateTing;
    AddHistory(seat, tile, operate);
    FreshCallData(seat);
    return true;
}

void MJPlayEx::DoDiscard(ITileID tile)
{
    auto playData = m_PlayDatas.at(m_CurrentSeat);
    if (playData->m_Call)
    {  //下叫后
        tile = playData->GetHandTiles().back();
    }
    else
    {
        _NBBotDisCard(tile);
        if (this->NoDiscardByLai(tile) || !playData->CanDiscard(tile))
        {
            tile = _CalcAutoDiscardTile();
        }
    }
    if (tile == TILE_NULL)
    {
        throw MJ_MAKE_EXCEPTION("all tingyong in hand, cannot discard");
    }

    playData->Discard(tile);
    m_CurrentTile = tile;
    AddHistory(m_CurrentSeat, m_CurrentTile, EOperateDiscard);
    FreshCallData(m_CurrentSeat);
}

void MJPlayEx::DoDianPao(std::vector<ISeatID> const& huSeats)
{
    m_PlayDatas.at(m_CurrentSeat)->RemoveOutTile();
}

void MJPlayEx::DoQiangGang(std::vector<ISeatID> const& huSeats)
{
    if (IsAfterKon())
    {
        m_PlayDatas.at(m_CurrentSeat)->RevertKon(m_CurrentTile);
    }
}

std::vector<int64_t> MJPlayEx::DoCheckSelfHuMultiple(std::vector<ISeatID>& cappedSeats)
{
    m_HuSeats.push_back(m_CurrentSeat);
    AddHistory(m_CurrentSeat, m_CurrentTile, EOperateHu);

    std::vector<int64_t> multiples(m_Game->GetIniPlayerCount(), {});
    // 查胡权限时已经计算过结果，直接用
    auto& huResult = m_HuResults.at(m_CurrentSeat);
    auto multi = m_PlayConf.GetRealMultiple(huResult.totalMultiple);
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        if (m_Game->GetPlayer(i)->IsOut() || i == m_CurrentSeat)
        {
            continue;
        }
        multiples.at(i) = -multi;
        multiples.at(m_CurrentSeat) += multi;
    }

    if (m_PlayDatas.at(m_CurrentSeat)->IsTianDiHu() && m_Banker == m_CurrentSeat)
    {
        m_PlayDatas.at(m_CurrentSeat)->PickTianHuTile();
        m_CurrentTile = m_PlayDatas.at(m_CurrentSeat)->GetHandTiles().back();
    }
    if (m_PlayConf.IsTopMultiple(multi))
    {
        cappedSeats.push_back(m_CurrentSeat);
    }

    return multiples;
}

std::vector<int64_t> MJPlayEx::DoCheckPaoHuMultiple(ISeatID winSeat, ISeatID paoSeat, std::vector<ISeatID>& cappedSeats)
{
    m_HuSeats.push_back(winSeat);
    AddHistory(winSeat, m_CurrentTile, EOperateHu);

    std::vector<int64_t> multiples(m_Game->GetIniPlayerCount(), 0);
    auto& huResult = m_HuResults.at(winSeat);
    auto multi = m_PlayConf.GetRealMultiple(huResult.totalMultiple);

    multiples.at(paoSeat) -= multi;
    multiples.at(winSeat) += multi;

    if (m_PlayConf.IsTopMultiple(multi))
    {
        cappedSeats.push_back(winSeat);
    }
    return multiples;
}

void MJPlayEx::FreshCallData(ISeatID seat)
{
    auto playData = m_PlayDatas.at(seat);
    auto input = playData->MakeHuPlayData();
    input.extraHuTypes = GetExtraCallHuTypes(seat);
    playData->MutableCallDataMap() = GetMJService()->GetHuTool()->CheckCall(input, m_Game->GetConfig());
}

bool MJPlayEx::CheckXueZhanDaoDiTip(int restPlayerCount, int beforePlayerCount)
{
    if (restPlayerCount != 2)
    {
        return false;
    }
    if (beforePlayerCount <= 2)
    {
        return false;
    }
    //剩余牌只有一张不需要提示
    if (m_Dealer->GetRestTileCount() <= 1)
    {
        return false;
    }
    //只有四人模式需要提示
    if (m_Game->GetIniPlayerCount() != 4)
    {
        return false;
    }
    return true;
}

void MJPlayEx::_OnTips(MJTips::Type tip, ISeatID seat)
{
    if (m_TipsHandler)
    {
        if (tip == MJTips::QiHuFan)
        {
            m_PlayDatas.at(seat)->SetBanQiHuFanTip(true);
        }
        m_TipsHandler(tip, seat);
    }
}

bool MJPlayEx::_IsPonOperates(EOperate operate) const
{
    return (operate == EOperatePon);
}

bool MJPlayEx::_IsKonOperates(EOperate operate) const
{
    return (operate == EOperateKon);
}

void MJPlayEx::_AddHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, bool mustHu)
{
    opt.capped = m_PlayConf.IsTopMultiple(result.totalMultiple);
    m_HuResults[seat] = result;
    opt.AddOperate(EOperateHu);
    opt.isMustHu = mustHu;
}

void MJPlayEx::_CheckPaoHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, std::vector<MJTips::Type>& tips)
{
    if (m_PlayConf.m_OnlyZimo)
    {
        tips.push_back(MJTips::OnlyZiMo);
        return;
    }
    auto playData = GetPlayData(seat);
    if (m_PlayConf.m_MustHu)
    {
        _AddHuOperate(opt, seat, result, true);
    }
    else if (playData->IsPassHuTile(m_CurrentTile) && m_PlayConf.m_HuPass)
    {
        tips.push_back(MJTips::PassHu);
    }
    else if (result.totalMultiple < m_PlayConf.m_MinMultipleLimit)
    {
        tips.push_back(MJTips::QiHuFan);
    }
    else
    {
        _AddHuOperate(opt, seat, result);
    }
}

void MJPlayEx::Pass(ISeatID seat, MJOperates operates)
{
    auto playData = m_PlayDatas.at(seat);
    if (operates.HasOperate(EOperateHu))
    {
        playData->PassHu(m_CurrentTile, 0);
    }
    playData->PassPon(m_CurrentTile);
}

bool MJPlayEx::IsPonThisTileCurHand(ITileID tile, ISeatID curSeat)
{
    for (auto iter = m_History.rbegin(); iter != m_History.rend(); ++iter)
    {
        if (iter->seat != curSeat)
        {
            return false;
        }
        if (iter->operate == EOperatePon && iter->tile == tile)
        {
            return true;
        }
    }
    return false;
}

void MJPlayEx::SetNextBanker(ISeatID seat)
{
    if (m_Game->IsValidSeat(m_NextBanker))
    {
        return;
    }
    m_NextBanker = seat;
}

ISeatID MJPlayEx::GetNextBanker() const
{
    return m_NextBanker;
}

ISeatID MJPlayEx::GetLastDisTileSeat() const
{
    for (auto iter = m_History.rbegin(); iter != m_History.rend(); ++iter)
    {
        if (iter->operate == EOperateDiscard)
        {
            return iter->seat;
        }
    }
    return m_CurrentSeat;
}

void MJPlayEx::SetPlayersAtomEvents()
{
    for (ISeatID seat = 0; seat < m_Game->GetIniPlayerCount(); ++seat)
    {
        auto data = m_PlayDatas.at(seat);
        auto player = m_Game->GetPlayer(seat);

        player->AddOperateEvent(data->GetEverChiCount(), data->GetEverPonCount(), data->GetEverKonCount(), data->m_Call);
        player->AddHuEvent();
    }
}

std::vector<ITileID> const& MJPlayEx::LaiTiles() const
{
    return m_LaiTiles;
}

bool MJPlayEx::IsLai(ITileID tile) const
{
    return MJ::HasElement(m_LaiTiles, tile);
}

bool MJPlayEx::NoDiscardByLai(ITileID tile) const
{
    return m_PlayConf.m_CanotDiscardLai && IsLai(tile);
}

MJOperates MJPlayEx::FetchWaitCallOperates(ISeatID seat)
{
    MJOperates operates;
    _CheckCall(operates, seat);
    return operates;
}

MJOperates MJPlayEx::FetchSelfOperates()
{
    MJOperates operates;
    std::vector<MJTips::Type> tips;
    if (!IsAfterPon())
    {
        _CheckSelfHu(operates, m_CurrentSeat, tips);
    }

    if (!operates.isMustHu)
    {
        if (m_Dealer->GetRestTileCount() > 0)
        {
            _CheckSelfKon(operates, m_CurrentSeat);
        }
        if (!m_PlayDatas.at(m_CurrentSeat)->m_Call)
        {
            _CheckCall(operates, m_CurrentSeat);
            operates.AddOperate(EOperateDiscard);
        }
    }
    if (tips.size())
    {
        _OnTips(tips.front(), m_CurrentSeat);
    }

    return operates;
}

MJOperates MJPlayEx::FetchWaitOperates(ISeatID seat)
{
    MJOperates operates;
    if (m_Game->GetPlayer(seat)->IsOut())
    {
        return operates;
    }

    std::vector<MJTips::Type> tips;
    _CheckPaoHu(operates, seat, tips);

    if (!operates.isMustHu)
    {
        _CheckChow(operates, seat, tips);
        _CheckPon(operates, seat, tips);
        if (m_Dealer->GetRestTileCount() > 0)
        {
            _CheckZhiKon(operates, seat);
        }
    }
    if (!tips.empty())
    {
        _OnTips(tips.front(), seat);
    }
    return operates;
}

MJOperates MJPlayEx::FetchGrabKonOperates(ISeatID seat)
{
    auto playData = m_PlayDatas.at(seat);
    MJOperates operates{};
    if (m_Game->GetPlayer(seat)->IsOut())
    {
        return operates;
    }
    auto realTile = m_CurrentTile;
    auto input = playData->MakeHuPlayData(realTile);
    HuResult result{};
    std::vector<MJTips::Type> tips;
    input.extraHuTypes = _QiangGangExtraFan(seat);
    if (GetMJService()->GetHuTool()->CheckHu(out__ result, input, m_Game->GetConfig()))
    {
        _CheckPaoHuOperate(operates, seat, result, tips);
    }

    if (tips.size())
    {
        _OnTips(tips.front(), seat);
    }
    return operates;
}

ITileID MJPlayEx::GetCurrentWinTile()
{
    return m_WinTile;
}

bool MJPlayEx::_CheckMustHuByLai(ISeatID seat)
{
    if (!m_PlayConf.m_MustHuIfOnlyLai)
    {
        return false;
    }
    auto data = GetPlayData(seat);
    if (data->IsAllLai() || (data->m_Call && IsLai(data->GetHandTiles().back())))
    {
        return true;
    }
    return false;
}

int MJPlayEx::GetConfigValue(ConfigIndex index) const
{
    return m_Game->GetConfig()->GetValue(index);
}

bool MJPlayEx::IsAfterPon()
{
    if (m_History.empty())
    {
        return false;
    }
    auto op = m_History.back().operate;
    return op == EOperatePon;
}

bool MJPlayEx::IsAfterKon()
{
    if (m_History.empty())
    {
        return false;
    }
    auto op = m_History.back().operate;
    return (op == EOperateKon);
}

bool MJPlayEx::IsGangThisHand()
{
    for (auto iter = m_History.rbegin(); iter != m_History.rend(); ++iter)
    {
        if (iter->seat != m_CurrentSeat)
        {
            return false;
        }
        if ((*iter).operate == EOperateKon)
        {
            return true;
        }
    }
    return false;
}

bool MJPlayEx::IsGangPao()
{
    auto hisInfo = m_History.back();
    if (hisInfo.operate != EOperateDiscard)
    {
        return false;
    }
    for (auto iter = m_History.rbegin(); iter != m_History.rend(); ++iter)
    {
        if (iter->seat != m_CurrentSeat)
        {
            return false;
        }
        if ((*iter).operate == EOperateKon)
        {
            return true;
        }
    }
    return false;
}

bool MJPlayEx::IsAfterKonDraw()
{
    if (m_History.size() < 2)
    {
        return false;
    }
    auto draw = m_History.back().operate;
    auto kon = m_History.at(m_History.size() - 2).operate;
    return (draw == EOperateDraw && kon == EOperateKon);
}

bool MJPlayEx::IsHaiDiLao()
{
    return m_Dealer->GetRestTileCount() == 0;
}

bool MJPlayEx::IsAfterKonDrawDiscard()
{
    if (m_History.size() < 3)
    {
        return false;
    }
    auto i = m_History.size();
    auto kon = (m_History.at(i - 3).operate == EOperateKon);
    auto draw = (m_History.at(i - 2).operate == EOperateDraw);
    auto back = m_History.back().operate;
    auto discard = (back == EOperateDiscard || back == EOperateTing);
    return kon && draw && discard;
}

ISeatID MJPlayEx::GetLastDisSeat() const
{
    auto it = std::find_if(m_History.rbegin(), m_History.rend(), [](const MJAction& act) { return act.operate == EOperateDiscard || act.operate == EOperateTing; });
    return it != m_History.rend() ? it->seat : m_CurrentSeat;
}

HuResult const& MJPlayEx::GetHuResult(ISeatID seat) const
{
    return m_HuResults.at(seat);
}

std::vector<ISeatID> const& MJPlayEx::GetHuSeats() const
{
    return m_HuSeats;
}

bool MJPlayEx::IsCurrentPassBuKon() const
{
    return !m_PlayDatas.at(m_CurrentSeat)->IsCurrentDrawTile(m_CurrentTile);
}

bool MJPlayEx::IsKonAfterPon() const
{
    if (m_History.size() < 2)
    {
        return false;
    }
    auto& kon = m_History.back();
    auto& pon = m_History.at(m_History.size() - 2);
    return (kon.tile == pon.tile && _IsKonOperates(kon.operate) && _IsPonOperates(pon.operate));
}

bool MJPlayEx::IsKonAfterPonBefor(ITileID tile) const
{
    auto& pon = m_History.back();
    return pon.tile == tile && _IsPonOperates(pon.operate);
}

EOperate MJPlayEx::GetLastOperate()
{
    if (m_History.empty())
    {
        return EOperateNone;
    }
    return m_History.back().operate;
}

bool MJPlayEx::HasNoAction(ISeatID seat)
{
    return std::find_if(m_History.begin(), m_History.end(), [=](MJAction& a) { return a.seat == seat; }) == m_History.end();
}

ITileID MJPlayEx::_CalcAutoDiscardTile() const
{
    auto playData = m_PlayDatas.at(m_CurrentSeat);
    if (playData->m_Call)
    {
        return playData->GetHandTiles().back();
    }
    auto recommend = m_Recommender.Recommend(playData->GetHandTiles(), {});
    if (!playData->GetCallDataMap().empty())
    {
        recommend = (*playData->GetCallDataMap().begin()).first;
    }
    if (recommend == TILE_NULL)
    {
        recommend = playData->GetHandTiles().back();
    }
    if (NoDiscardByLai(recommend))
    {
        for (auto tile : playData->GetHandTiles())
        {
            if (!IsLai(tile))
            {
                return tile;
            }
        }
        throw MJ_MAKE_EXCEPTION("cannot find tile to discard");
    }
    return recommend;
}

void MJPlayEx::_CheckPaoHu(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips)
{
    std::vector<int> extraFan;
    if (!_PaoExtraFan(seat, extraFan))
    {
        return;
    }
    auto input = m_PlayDatas.at(seat)->MakeHuPlayData(m_CurrentTile);
    input.extraHuTypes = extraFan;
    HuResult result{};
    if (GetMJService()->GetHuTool()->CheckHu(out__ result, input, m_Game->GetConfig()))
    {
        _CheckPaoHuOperate(opt, seat, result, tips);
    }
}

void MJPlayEx::_CheckChow(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips)
{
    if (m_PlayDatas.at(seat)->m_Call || !m_PlayConf.m_HasChow)
    {
        return;
    }

    if (MJGetNextSeat(m_CurrentSeat, 1, m_Game->GetIniPlayerCount()) != seat)
    {
        return;
    }

    MJOperates tempOpr;
    if (m_PlayDatas.at(seat)->CanChow(m_CurrentTile))
    {
        opt.AddOperate(EOperateChow);
    }
}

void MJPlayEx::_CheckPon(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips)
{
    if (m_PlayDatas.at(seat)->m_Call || !m_PlayConf.m_HasPon)
    {
        return;
    }

    MJOperates tempOpr;
    if (m_PlayDatas.at(seat)->CanPon(m_CurrentTile, m_PlayConf.m_CanotOnlyLaiAfterPon))
    {
        tempOpr.AddOperate(EOperatePon);
    }

    if (!m_PlayDatas.at(seat)->IsPassPonTile(m_CurrentTile) || !m_PlayConf.m_PonPass)
    {
        opt.AddOperates(tempOpr);
    }
    else if (tempOpr.value != 0)
    {  //过碰
        tips.push_back(MJTips::PassPon);
    }
}

void MJPlayEx::_CheckZhiKon(MJOperates& opt, ISeatID seat)
{
    if (m_PlayDatas.at(seat)->CanZhiKon(m_CurrentTile, m_Game->GetConfig()))
    {
        opt.AddOperate(EOperateKon);
    }
}

void MJPlayEx::_CheckCall(MJOperates& opt, ISeatID seat)
{
    if (!m_PlayConf.m_CanDeclareCall)
    {
        return;
    }

    auto input = m_PlayDatas.at(seat)->MakeHuPlayData();
    auto callInfo = GetMJService()->GetHuTool()->CheckCall(input, m_Game->GetConfig());
    if (callInfo.empty())
    {
        return;
    }

    if (m_PlayConf.m_TianTing && _IsTianTing(seat))
    {
        opt.AddOperate(EOperateTianTing);
    }
    else
    {
        opt.AddOperate(EOperateTing);
    }
}

std::vector<int> MJPlayEx::GetExtraCallHuTypes(ISeatID)
{
    return std::vector<int>();
}

bool MJPlayEx::_IsTianTing(ISeatID seat)
{
    return HasNoAction(seat);
}

void MJPlayEx::_CheckSelfHu(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips)
{
    auto input = m_PlayDatas.at(seat)->MakeHuPlayData();
    input.extraHuTypes = _ZiMoExtraFan(seat);
    HuResult result{};
    if (GetMJService()->GetHuTool()->CheckHu(out__ result, input, m_Game->GetConfig()))
    {
        if (m_PlayConf.m_MustHu || _CheckMustHuByLai(m_CurrentSeat))
        {
            _AddHuOperate(opt, seat, result, true);
        }
        else if (result.totalMultiple < m_PlayConf.m_MinMultipleLimit)
        {
            tips.push_back(MJTips::QiHuFan);
        }
        else
        {
            _AddHuOperate(opt, seat, result);
        }
    }
}

void MJPlayEx::_CheckSelfKon(MJOperates& opt, ISeatID seat)
{
    if (m_PlayDatas.at(seat)->CanSelfKon(m_Game->GetConfig(), m_LaiTiles))
    {
        opt.AddOperate(EOperateKon);
    }
}

KonType MJPlayEx::_FixKonTypeAfterPon(KonType type, ITileID tile) const
{
    if (m_History.empty())
    {
        return type;
    }
    auto last = m_History.back();
    if (last.operate == EOperatePon && last.tile == tile)
    {
        return KonType::Zhi;
    }
    return type;
}

void MJPlayEx::_NBBotDisCard(ITileID& tile)
{
    ISeatID nbSeat = m_Game->GetNewbieSeat();
    if (nbSeat == SEAT_NULL || m_CurrentSeat == nbSeat)
    {
        return;
    }

    std::vector<ITileID> handTiles = m_PlayDatas.at(m_CurrentSeat)->GetHandTiles();
    if (tile == TILE_NULL)
    {
        tile = handTiles.back();
    }

    MJ::RemoveElement(handTiles, tile);
    if (GetMJService()->GetTingTool()->CalcTing(handTiles, m_LaiTiles, {}) > 0)
    {
        return;
    }

    auto maxCount = handTiles.size();
    for (auto i = 0; i < maxCount; ++i)
    {
        handTiles.push_back(tile);
        tile = handTiles.front();
        handTiles.erase(handTiles.begin());
        if (GetMJService()->GetTingTool()->CalcTing(handTiles, m_LaiTiles, {}) > 0)
        {
            return;
        }
    }
}
void MJPlayEx::GetShowTilesInfo(std::map<ITileID, int>& mTiles, ISeatID seat)
{
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        auto play = m_PlayDatas.at(i);
        for (auto& chi : play->GetChowGroups())
        {
            ++mTiles[chi.leftTile];
            ++mTiles[MJNextTileInSameColor(chi.leftTile, 1)];
            ++mTiles[MJNextTileInSameColor(chi.leftTile, 2)];
        }
        for (auto& pon : play->GetPonGroups())
        {
            mTiles[pon.tile] += 3;
        }
        for (auto& kon : play->GetKonGroups())
        {
            mTiles[kon.tile] += 4;
        }
        for (auto& out : play->GetOutTiles())
        {
            ++mTiles[out];
        }
        if (i == seat)
        {
            for (auto& hand : play->GetHandTiles())
            {
                ++mTiles[hand];
            }
        }
    }
}