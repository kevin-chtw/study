#include "LYGCDealer.h"
#include "MJManual.h"
#include "MJHelper.h"
#include "MJString.h"
#include "MJTile.h"
#include "MJLog.h"
#include "LYGCGame.h"
#include "TKDllGame.h"
#include "LYGCHu.h"

void LYGCDealer::Initialize()
{
    int initCardsFileNo = m_Game->GetConfig()->GetValue(ECI_MannualCardsIndex);
    if (LoadManual("MahjongLYGC", initCardsFileNo))
    {
        m_CiTile = m_TileWall.back();
        m_TileWall.pop_back();
        return;
    }

    m_SelfTiles.clear();
    InitRandom();

    _InitCiTile();

    if (m_Game->GetConfig()->GetValue(ECI_DealerCtrl))
    {
        _CtrlDispatch();
    }
#ifdef _DEBUG
    assert(testctrl());
#endif  // DEBUG
}

bool LYGCDealer::testctrl()
{
    std::map<int, int> mtiles;
    for (auto tile : m_TileWall)
    {
        mtiles[tile]++;
    }

    for (ISeatID seat = 0; seat < m_Game->GetIniPlayerCount(); ++seat)
    {
        for (auto tile : m_SelfTiles[seat])
        {
            mtiles[tile]++;
        }
    }
    mtiles[m_CiTile]++;
    if (mtiles.size() != TilesMap().size())
    {
        return false;
    }

    for (auto p : mtiles)
    {
        if (p.second != 4)
        {
            return false;
        }
    }
    return true;
}

void LYGCDealer::_InitCiTile()
{
    auto eCiPoint = _getCiTilePoint();

    for (auto it = m_TileWall.rbegin(); it < m_TileWall.rend(); it++)
    {
        if (_checkFitCiPoint(*it, eCiPoint))
        {
            m_CiTile = *it;
            m_TileWall.erase((++it).base());
            break;
        }
    }
    if (m_CiTile == TILE_NULL)
    {
        m_CiTile = m_TileWall.back();
        m_TileWall.pop_back();
    }

    // 计算次牌关联张
    m_vCiAssociationCards.clear();
    m_vCiAssociationCards.push_back(m_CiTile);
    auto color = MJTileColor(m_CiTile);

    if (!MJIsSuitColor(color))
    {
        // 次牌为风牌时 关联张只有他本身
        return;
    }

    auto point = MJTilePoint(m_CiTile);
    switch (point)
    {
    case 0:
        m_vCiAssociationCards.push_back(MJMakeTile(color, 1));
        m_vCiAssociationCards.push_back(MJMakeTile(color, 2));
        break;
    case 8:
        m_vCiAssociationCards.push_back(MJMakeTile(color, 7));
        m_vCiAssociationCards.push_back(MJMakeTile(color, 6));
        break;
    case 1:
        m_vCiAssociationCards.push_back(MJMakeTile(color, 0));
        m_vCiAssociationCards.push_back(MJMakeTile(color, 2));
        m_vCiAssociationCards.push_back(MJMakeTile(color, 3));
        break;
    case 7:
        m_vCiAssociationCards.push_back(MJMakeTile(color, 5));
        m_vCiAssociationCards.push_back(MJMakeTile(color, 6));
        m_vCiAssociationCards.push_back(MJMakeTile(color, 8));
        break;
    default:
        if (point >= 2 && point <= 6)
        {
            m_vCiAssociationCards.push_back(MJMakeTile(color, point - 2));
            m_vCiAssociationCards.push_back(MJMakeTile(color, point - 1));
            m_vCiAssociationCards.push_back(MJMakeTile(color, point + 1));
            m_vCiAssociationCards.push_back(MJMakeTile(color, point + 2));
        }
        break;
    }
}

LYGCDealer::CI_TILE_POINT LYGCDealer::_getCiTilePoint()
{
    auto nRand = MJ::Random(100);
    if (nRand > 92)
    {
        return CI_TILE_POINT_1_9;
    }
    if (nRand > 80)
    {
        return CI_TILE_POINT_2_8;
    }

    if (m_Game->GetConfig()->GetValue(ECI_RuleFeng) == 1 && nRand > 70)
    {
        return CI_TILE_POINT_ZI;
    }
    return CI_TILE_POINT_3_7;
}

bool LYGCDealer::_checkFitCiPoint(ITileID tile, CI_TILE_POINT ciPoint)
{
    auto point = MJTilePoint(tile);
    switch (ciPoint)
    {
    case LYGCDealer::CI_TILE_POINT_1_9:
        if (MJIsSuitTile(tile) && (point == 0 || point == 8))
        {
            return true;
        }
        break;
    case LYGCDealer::CI_TILE_POINT_2_8:
        if (MJIsSuitTile(tile) && (point == 1 || point == 7))
        {
            return true;
        }
        break;
    case LYGCDealer::CI_TILE_POINT_3_7:
        if (MJIsSuitTile(tile) && (point > 1 && point < 7))
        {
            return true;
        }
        break;
    case LYGCDealer::CI_TILE_POINT_ZI:
        if (MJIsHonorTile(tile))
        {
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

bool LYGCDealer::_CtrlDispatch()
{
    std::vector<int> vcFanRates{m_Game->GetConfig()->GetValue(ECI_DealerAnKe), m_Game->GetConfig()->GetValue(ECI_DealerAnGang), m_Game->GetConfig()->GetValue(ECI_DealerDoubleAnKe),
        m_Game->GetConfig()->GetValue(ECI_DealerDuiZi)};
    int nSum = std::accumulate(std::begin(vcFanRates), std::end(vcFanRates), 0);
    vcFanRates.push_back(std::max(0, 100 - nSum));
    ESDealType type = (ESDealType)MJ::RandomByRates(vcFanRates);
    int fanSeat = MJ::Random(m_Game->GetIniPlayerCount());

    for (ISeatID seat = 0; seat < m_Game->GetIniPlayerCount(); ++seat)
    {
        bool bSu = false;
        int handCnt = GetGame()->GetPlay()->GetBanker() == seat ? TILE_COUNT_INIT_BANKER : TILE_COUNT_INIT_NORMAL;
        _takeCardsRandom(seat, handCnt);
        ESDealType ntype = seat == fanSeat ? type : ESDT_PingHu;
        for (int n = 0; n < 300; ++n)
        {
            if (!_checkRandomDealCard(ntype, m_Game->GetConfig()->GetValue(ECI_DispatchXiangTing), seat))
            {
                _swapCards(seat, 0);
            }
            else
            {
                bSu = true;
                break;
            }
        }
        if (!bSu)
        {
            GetGame()->GetMessager().get()->SendDebugString("random fa pai fail");
            _clearHandCards(seat);
            _takeFanXingCards(seat, ntype);
            int p = m_SelfTiles[seat].size();
            _takeCardsRandom(seat, handCnt);
            for (int n = 0; n < 300; ++n)
            {
                if (!_checkRandomDealCard(ntype, m_Game->GetConfig()->GetValue(ECI_DispatchXiangTing), seat))
                {
                    _swapCards(seat, p);
                }
                else
                {
                    bSu = true;
                    break;
                }
            }
            if (!bSu)
            {
                GetGame()->GetMessager().get()->SendDebugString("yuzhi fa pai fail");
            }
        }
    }

    // 次牌不在最后20张
    _checkCiCardPos();
    return true;
}

void LYGCDealer::_checkCiCardPos()
{
    if (m_TileWall.size() > 20)
    {
        int np20 = m_TileWall.size() - 20;
        for (int i = np20; i < m_TileWall.size(); ++i)
        {
            if (m_TileWall.at(i) == m_CiTile)
            {
                int nc = 0;
                while (nc < 10)
                {
                    auto ic = MJ::Random(np20);
                    if (m_TileWall.at(ic) != m_CiTile)
                    {
                        std::swap(m_TileWall.at(ic), m_TileWall.at(i));
                        break;
                    }
                    nc++;
                }
            }
        }
    }
}

void LYGCDealer::_takeCardsRandom(int seat, int count)
{
    for (int i = m_SelfTiles[seat].size(); i < count && m_TileWall.size() > 0; ++i)
    {
        m_SelfTiles[seat].push_back(m_TileWall.front());
        m_TileWall.pop_front();
    }
}

bool LYGCDealer::_checkRandomDealCard(ESDealType type, int callStep, int seat)
{
    if (callStep <= 0)
    {
        callStep = 4;
    }
    auto& hands = m_SelfTiles[seat];
    std::map<ITileID, int> mTiles;
    for (auto it : hands)
    {
        mTiles[it]++;
    }

    // 对子+刻子大于等于2(杠牌算两对)和番型
    if (!_checkFanXing(mTiles, type))
    {
        return false;
    }
    // 至少发一张次牌关联张（次牌为风牌时不验证该条件）
    if (!MJIsHonorTile(m_CiTile) && !MJ::HasAnyElement(hands, m_vCiAssociationCards))
    {
        return false;
    }
    // 向听数
    std::vector<ITileID> vhand{hands.begin(), hands.end()};
    if (GetMJService()->GetTingTool()->CalcTing(vhand, {}, {}) > callStep)
    {
        return false;
    }
    return true;
}

bool LYGCDealer::_checkFanXing(std::map<ITileID, int> const& mTileCounts, ESDealType type)
{
    std::vector<int> vn{0, 0, 0, 0, 0};
    for (auto it : mTileCounts)
    {
        if (it.second >= 0 && it.second <= 4)
        {
            vn[it.second]++;
        }
    }
    if (vn[2] + vn[3] + 2 * vn[4] < 2)
    {
        return false;
    }
    if (type == ESDT_PingHu)
    {
        return true;
    }
    else if (type == ESDT_AnKe && vn[3] > 0)
    {
        return true;
    }
    else if (type == ESDT_AnGang && vn[4] > 0)
    {
        return true;
    }
    else if (type == ESDT_DoubleAnKe && vn[3] > 1)
    {
        return true;
    }
    else if (type == ESDT_DuiZi && vn[2] + vn[3] + 2 * vn[4] > 4)
    {
        return true;
    }
    return false;
}

void LYGCDealer::_swapCards(int seat, int pos)
{
    auto& hands = m_SelfTiles[seat];
    for (int i = pos; i < hands.size(); ++i)
    {
        int r = MJ::Random<int>(m_TileWall.size());
        std::swap(hands.at(i), m_TileWall.at(r));
    }
}

void LYGCDealer::_clearHandCards(int seat)
{
    auto& hands = m_SelfTiles[seat];
    m_TileWall.insert(m_TileWall.end(), hands.begin(), hands.end());
    hands.clear();
}

void LYGCDealer::_takeFanXingCards(int seat, ESDealType type)
{
    switch (type)
    {
    case ESDT_AnKe:
        _getKeCard(seat, 1);
        _getDuiZiCard(seat, 1);
        break;
    case ESDT_AnGang:
        _getGangCard(seat, 1);
        break;
    case ESDT_DoubleAnKe:
        _getKeCard(seat, 2);
        break;
    case ESDT_DuiZi:
        _getDuiZiCard(seat, 4);
        break;
    default:
        break;
    }

    if (!MJ::HasAnyElement(m_SelfTiles[seat], m_vCiAssociationCards))
    {
        _takeCiAssociationCards(seat);
    }
    std::shuffle(m_TileWall.begin(), m_TileWall.end(), MJ::DefaultRandomEngine);
}

void LYGCDealer::_takeCiAssociationCards(int seat, int count)
{
    for (auto it = m_TileWall.begin(); it != m_TileWall.end(); ++it)
    {
        if (MJ::HasElement(m_vCiAssociationCards, *it))
        {
            m_SelfTiles[seat].push_back(*it);
            m_TileWall.erase(it);
            break;
        }
    }
}

void LYGCDealer::_getDuiZiCard(int seat, int nCount)
{
    std::map<int, int> mapCard;
    std::vector<int> vcDuiZi;
    for (auto it : m_TileWall)
    {
        if (MJIsExtraTile(it))
        {
            continue;
        }
        mapCard[it]++;
    }
    for (auto it : mapCard)
    {
        if (it.second >= 2)
        {
            vcDuiZi.push_back(it.first);
        }
    }
    for (int i = 0; i < nCount && vcDuiZi.size() > 0; ++i)
    {
        int r = MJ::Random<int>(vcDuiZi.size());
        int card = vcDuiZi[r];
        _putCardIntoHand(seat, card, 2);
        vcDuiZi.erase(vcDuiZi.begin() + r);
    }
}

void LYGCDealer::_getShunCard(int seat)
{
    for (auto card : m_TileWall)
    {
        if (MJTileColor(card) > EMJColor::Dot || MJTilePoint(card) >= 7)
        {
            continue;
        }
        int card2 = MJMakeTile(MJTileColor(card), MJTilePoint(card) + 1);
        int card3 = MJMakeTile(MJTileColor(card), MJTilePoint(card) + 2);
        if (MJ::HasElement(m_TileWall, card2) && MJ::HasElement(m_TileWall, card3))
        {
            _putCardIntoHand(seat, card, 1);
            _putCardIntoHand(seat, card2, 1);
            _putCardIntoHand(seat, card3, 1);
            return;
        }
    }
}

void LYGCDealer::_getKeCard(int seat, int nCount)
{
    std::map<int, int> mapCard;
    std::vector<int> vcKeZi;
    for (auto it : m_TileWall)
    {
        if (MJIsExtraTile(it))
        {
            continue;
        }
        mapCard[it]++;
    }
    for (auto it : mapCard)
    {
        if (it.second >= 3)
        {
            vcKeZi.push_back(it.first);
        }
    }
    for (int i = 0; i < nCount && vcKeZi.size() > 0; ++i)
    {
        int r = MJ::Random<int>(vcKeZi.size());
        int card = vcKeZi[r];
        _putCardIntoHand(seat, card, 3);
        vcKeZi.erase(vcKeZi.begin() + r);
    }
}
void LYGCDealer::_getGangCard(int seat, int nCount)
{
    std::map<int, int> mapCard;
    std::vector<int> vcGangZi;
    for (auto it : m_TileWall)
    {
        mapCard[it]++;
    }
    for (auto it : mapCard)
    {
        if (it.second == 4)
        {
            vcGangZi.push_back(it.first);
        }
    }
    for (int i = 0; i < nCount && vcGangZi.size() > 0; ++i)
    {
        int r = MJ::Random<int>(vcGangZi.size());
        int card = vcGangZi[r];
        _putCardIntoHand(seat, card, 4);
        vcGangZi.erase(vcGangZi.begin() + r);
    }
}

void LYGCDealer::_putCardIntoHand(int seat, ITileID card, int nCount)
{
    for (int i = 0; i < nCount; ++i)
    {
        if (MJ::RemoveElement(m_TileWall, card))
        {
            m_SelfTiles[seat].push_back(card);
        }
    }
}

LYGCGame* LYGCDealer::GetGame()
{
    return dynamic_cast<LYGCGame*>(m_Game);
}

const std::map<ITileID, int>& LYGCDealer::TilesMap()
{
    return GetMJService<LYGCService>()->GetTilesMap(m_Game->GetConfig());
}

ITileID LYGCDealer::GetCiTile()
{
    return m_CiTile;
}

ITileID LYGCDealer::_DrawStratety(MJPlayData* typer)
{
    if (m_IsManual || nullptr == typer)
    {
        return _PopTile();
    }

    int rate = typer->GetDrawRate();
    if (rate <= 0 || MJ::Random(100) >= rate)
    {
        return _PopTile();
    }

    auto configs = m_Game->GetDrawConfigs();
    if (configs.empty())
    {
        return _PopTile();
    }
    int config = typer->GetDrawConfig();
    if (config < 0 || config >= configs.size())
    {
        config = 0;
    }
    LYGCDrawStrategy drawStrategy(m_Game);
    drawStrategy.Init(configs.at(config));

    ITileID tile = drawStrategy.Draw(this, typer, typer->GetMinTing());
    tile = _DrawTile(tile);
    typer->UpdateMinTing(tile);
    return tile;
}

LYGCNBDealer::LYGCNBDealer(MJGame* game, ISeatID seat) : LYGCDealer(game), m_NBSeat(seat)
{
}

void LYGCNBDealer::Initialize()
{
    InitRandom();
    _InitCiTile();
    m_SelfTiles.clear();

    // 基本不会失败 不做失败时预制发牌
    _getNBGangCiCard(m_NBSeat, 2);
    for (int seat = 0; seat < m_Game->GetIniPlayerCount(); ++seat)
    {
        if (seat == m_NBSeat)
        {
            continue;
        }
        _baseHuNB(seat, 4);
    }

#ifdef _DEBUG
    assert(testctrl());
#endif  // DEBUG
}

void LYGCNBDealer::_baseHuNB(int seat, int callStep)
{
    int tryTime = 500;
    int handCnt = GetGame()->GetPlay()->GetBanker() == seat ? TILE_COUNT_INIT_BANKER : TILE_COUNT_INIT_NORMAL;

    _takeCardsRandom(seat, handCnt);
    for (int i = 0; i < tryTime; ++i)
    {
        std::vector<ITileID> vhand{m_SelfTiles[seat].begin(), m_SelfTiles[seat].end()};
        if (GetMJService()->GetTingTool()->CalcTing(vhand, {}, {}) >= 4)
        {
            break;
        }
        else
        {
            _swapCards(seat, 0);
        }
        if (i == tryTime - 1)
        {
            GetGame()->GetMessager().get()->SendDebugString("random nb ai fa pai fail");
        }
    }
}

void LYGCNBDealer::_getNBGangCiCard(ISeatID seat, int callStep)
{
    if (MJ::Random(100) < 50)
    {
        _getKeCard(seat, 2);
    }
    else
    {
        _getGangCard(seat, 1);
    }

    _getNBCiCardShun(seat);
    int pos = m_SelfTiles[seat].size();
    int handCnt = GetGame()->GetPlay()->GetBanker() == seat ? TILE_COUNT_INIT_BANKER : TILE_COUNT_INIT_NORMAL;
    _takeCardsRandom(seat, handCnt);
    bool bSu = false;

    for (int n = 0; n < 500; ++n)
    {
        std::vector<ITileID> vhand{m_SelfTiles[seat].begin(), m_SelfTiles[seat].end()};
        if (GetMJService()->GetTingTool()->CalcTing(vhand, {}, {}) <= 2)
        {
            break;
        }
        else
        {
            _swapCards(seat, pos);
        }
        if (n == 499)
        {
            GetGame()->GetMessager().get()->SendDebugString("random nb fa pai fail");
        }
    }
}

void LYGCNBDealer::_getNBCiCardShun(ISeatID seat)
{
    auto color = MJTileColor(m_CiTile);
    if (!MJIsSuitColor(color))
    {
        return;
    }

    auto point = MJTilePoint(m_CiTile);
    if (point == 0 || point == 8)
    {
        int np = point == 0 ? 1 : -1;
        _putCardIntoHand(seat, MJMakeTile(color, point + np), 1);
        _putCardIntoHand(seat, MJMakeTile(color, point + np + np), 1);
    }
    else if (point == 1 || point == 7)
    {
        if (MJ::Random(100) < 50)
        {
            _putCardIntoHand(seat, MJMakeTile(color, point - 1), 1);
            _putCardIntoHand(seat, MJMakeTile(color, point + 1), 1);
        }
        else
        {
            int np = point == 1 ? 1 : -1;
            _putCardIntoHand(seat, MJMakeTile(color, point + np), 1);
            _putCardIntoHand(seat, MJMakeTile(color, point + np + np), 1);
        }
    }
    else if (point >= 2 && point <= 6)
    {
        int t = MJ::Random(100);
        if (t < 33)
        {
            _putCardIntoHand(seat, MJMakeTile(color, point - 1), 1);
            _putCardIntoHand(seat, MJMakeTile(color, point + 1), 1);
        }
        else if (t < 66)
        {
            _putCardIntoHand(seat, MJMakeTile(color, point - 1), 1);
            _putCardIntoHand(seat, MJMakeTile(color, point - 2), 1);
        }
        else
        {
            _putCardIntoHand(seat, MJMakeTile(color, point + 1), 1);
            _putCardIntoHand(seat, MJMakeTile(color, point + 2), 1);
        }
    }
}

bool LYGCDrawStrategy::_checkUsefulIsKe(MJPlayData* typer, ITileID tile)
{
    std::vector<ITileID> hands = typer->GetHandTiles();
    std::vector<ITileID> thands = hands;
    if (MJ::CountElement(hands, tile) == 2)
    {
        MJ::RemoveAllElement(thands, tile);

        auto ting0 = GetMJService()->GetTingTool()->CalcTing(hands, m_Game->GetPlay()->LaiTiles(), {});
        hands.push_back(tile);
        auto ting1 = GetMJService()->GetTingTool()->CalcTing(thands, m_Game->GetPlay()->LaiTiles(), {});
        return (ting1 < ting0);
    }
    return false;
}

ITileID LYGCDrawStrategy::_checkChangeUsefulTile(MJPlayData* typer, std::vector<MJ::DrawTileType>& types, MJ::Dealer const* dealer, ITileID nowTile, size_t configIndex)
{
    const auto& ciass = m_Game->GetPlay()->GetDealer<LYGCDealer>()->GetCiAssociationCards();
    if (MJ::HasElement(ciass, nowTile) || _checkUsefulIsKe(typer, nowTile))
    {
        return nowTile;
    }

    std::vector<int> vUseful;
    for (auto s = 0; s < types.size(); ++s)
    {
        if (types[s] == TILE_USEFUL)
        {
            vUseful.push_back(s);
        }
    }
    if (vUseful.empty())
    {
        return nowTile;
    }
    for (auto it : vUseful)
    {
        if (MJ::HasElement(ciass, dealer->GetTile(it)))
        {
            if (MJ::Random(100) < 50)
            {
                return dealer->GetTile(it);
            }
            break;
        }
    }

    if (configIndex != 2)
    {
        return nowTile;
    }
    for (auto it : vUseful)
    {
        if (_checkUsefulIsKe(typer, dealer->GetTile(it)))
        {
            if (MJ::Random(100) < 40)
            {
                return dealer->GetTile(it);
            }
            break;
        }
    }
    return nowTile;
}