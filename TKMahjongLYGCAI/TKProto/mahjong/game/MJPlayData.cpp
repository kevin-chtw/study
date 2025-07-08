#include "MJPlay.h"
#include "MJLog.h"
#include "MJHelper.h"
#include "MJException.h"
#include "MJTing.h"
#include <sstream>

CallDataMap& MJPlayData::MutableCallDataMap()
{
    return m_CallDataMap;
}

CallDataMap const& MJPlayData::GetCallDataMap() const
{
    return m_CallDataMap;
}

void MJPlayData::Draw(ITileID tile)
{
    this->PutHandTile(tile);
    m_CurrentDrawTiles.insert(tile);
}

void MJPlayData::Discard(ITileID tile)
{
    this->RemoveHandTile(tile);
    this->PutOutTile(tile);
    CloseTianDiHu();
}

void MJPlayData::SetCall(ITileID tile, bool tianTing)
{
    if (tile != TILE_NULL)
    {
        this->RemoveHandTile(tile);
        this->PutOutTile(tile);
    }
    m_Call = true;
    m_TianTing = tianTing;
}

void MJPlayData::PutHandTile(ITileID tile)
{
    m_HandTiles.push_back(tile);
}

void MJPlayData::RemoveHandTile(ITileID tile, size_t count)
{
    while (count--)
    {
        this->RemoveHandTile(tile);
    }
}

void MJPlayData::RemoveHandTile(ITileID tile)
{
    if (!MJ::RemoveElement(m_HandTiles, tile))
    {
        throw MJ_MAKE_EXCEPTION("tile:%d", tile);
    }
}

void MJPlayData::PutOutTile(ITileID tile)
{
    m_OutTiles.push_back(tile);
}

void MJPlayData::RemoveOutTile()
{
    if (m_OutTiles.size())
    {
        m_OutTiles.pop_back();
    }
}

Group MJPlayData::RemovePon(ITileID tile)
{
    return RemoveGroup<Group>(tile, m_PonGroups);
}

KonGroup MJPlayData::RemoveKon(ITileID tile)
{
    return RemoveGroup<KonGroup>(tile, m_KonGroups);
}

void MJPlayData::RevertKon(ITileID tile)
{
    if (HasKon(tile))
    {
        auto from = RemoveKon(tile).from;
        m_PonGroups.push_back({tile, from});
    }
}

std::vector<Group> const& MJPlayData::GetPonGroups() const
{
    return m_PonGroups;
}

std::vector<KonGroup> const& MJPlayData::GetKonGroups() const
{
    return m_KonGroups;
}

bool MJPlayData::HasTile(ITileID tile) const
{
    return MJ::HasElement(m_HandTiles, tile);
}

std::vector<ITileID> const& MJPlayData::GetHandTiles() const
{
    return m_HandTiles;
}
std::vector<ITileID> const& MJPlayData::GetOutTiles() const
{
    return m_OutTiles;
}

ITileID MJPlayData::Chow(ITileID curTile, ITileID tile, ISeatID from)
{
    EMJColor color = MJTileColor(tile);
    int point = MJTilePoint(tile);

    for (int i = 0; i < 3; ++i)
    {
        ITileID tileId = MJMakeTile(color, point + i);
        if (tileId == curTile)
        {
            continue;
        }

        this->RemoveHandTile(tileId, 1);
    }
    m_ChowGroups.push_back({curTile, from, tile});

    CloseTianDiHu();
    IncEverChiCount();
    return tile;
}

const std::vector<ChowGroup>& MJPlayData::GetChowGroups() const
{
    return m_ChowGroups;
}

ITileID MJPlayData::Pon(ITileID tile, ISeatID from)
{
    this->RemoveHandTile(tile, 2);
    m_PonGroups.push_back({tile, from});

    CloseTianDiHu();
    IncEverPonCount();

    return tile;
}

bool MJPlayData::HasPon(ITileID tile) const
{
    return HasGroup<Group>(tile, m_PonGroups);
}

ITileID MJPlayData::KonS(ITileID tile, KonType type, ISeatID from, bool handPassBuKon, bool buKonAfterPon)
{
    if (type == KonType::Bu)
    {
        return _BuKon(tile, handPassBuKon, buKonAfterPon);
    }
    else
    {
        return _ZhiAnKon(tile, type, from);
    }
}

ITileID MJPlayData::_ZhiAnKon(ITileID tile, KonType type, ISeatID from)
{
    if (type == KonType::Zhi)
    {
        RemoveHandTile(tile, 3);
    }
    else if (type == KonType::An)
    {
        RemoveHandTile(tile, 4);
    }

    m_KonGroups.push_back(KonGroup{tile, from, type});
    CloseTianDiHu();
    IncEverKonCount();
    return tile;
}

ITileID MJPlayData::_BuKon(ITileID tile, bool handPassBuKon, bool buKonAfterPon)
{
    RemoveHandTile(tile);
    ISeatID from = RemovePon(tile).from;
    if (buKonAfterPon)
    {
        m_KonGroups.push_back(KonGroup{tile, from, KonType::Zhi});
    }
    else
    {
        m_KonGroups.push_back(KonGroup{tile, from, KonType::Bu, handPassBuKon});
    }

    CloseTianDiHu();
    IncEverKonCount();
    return tile;  //返回使用的手牌
}

bool MJPlayData::HasKon(ITileID tile) const
{
    return HasGroup<KonGroup>(tile, m_KonGroups);
}

void MJPlayData::PushPon(Group group)
{
    m_PonGroups.push_back(group);
}

void MJPlayData::PushKon(KonGroup group)
{
    m_KonGroups.push_back(group);
}

KonGroup const* MJPlayData::GetKon(ITileID tile) const
{
    for (auto& g : m_KonGroups)
    {
        if (g.tile == tile)
        {
            return &g;
        }
    }
    return nullptr;
}

Group const* MJPlayData::GetPon(ITileID tile) const
{
    for (auto& g : m_PonGroups)
    {
        if (g.tile == tile)
        {
            return &g;
        }
    }
    return nullptr;
}

void MJPlayData::CloseTianDiHu()
{
    m_TianDiHuState = false;
}

bool MJPlayData::TianDiHuState() const
{
    return m_TianDiHuState;
}

bool MJPlayData::IsPassHuTile(ITileID tile, int fan) const
{
    auto iter = m_PassHu.find(tile);
    return !(iter == m_PassHu.end() || iter->second < fan);
}

bool MJPlayData::IsPassHuTile(ITileID tile) const
{
    return m_PassHu.find(tile) != m_PassHu.end();
}

bool MJPlayData::IsPassPonTile(ITileID tile) const
{
    return m_PassPon.find(tile) != m_PassPon.end();
}
void MJPlayData::ClearPass()
{
    m_PassHu.clear();
    m_PassPon.clear();
    SetBanQiHuFanTip(false);
}

void MJPlayData::PassPon(ITileID tile)
{
    m_PassPon.insert(tile);
}

void MJPlayData::PassHu(ITileID tile, int fan)
{
    auto& v = m_PassHu[tile];
    if (v < fan)
    {
        v = fan;
    }
}

void MJPlayData::SetBanQiHuFanTip(bool flag)
{
    m_QiHuFanLimitTip = flag;
}

bool MJPlayData::IsBanQiHuFanTip() const
{
    return m_QiHuFanLimitTip;
}

////////////////
// 推荐换牌算法 //
////////////////
static ITileID _PickOneInColor(std::vector<ITileID>& tiles, int& pickStep)
{
    if (tiles.empty())
    {
        return TILE_NULL;
    }
    std::map<ITileID, int> counts;
    for (auto tile : tiles)
    {
        ++counts[tile];
    }

    // steps.at(i) 表示第i张牌与其最相邻牌的距离（ID差值）
    std::vector<int> steps(tiles.size(), 128);  //最大距离
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        auto step = tiles.at(i) - tiles.at(i - 1);
        steps.at(i) = std::min(steps.at(i), step);
        steps.at(i - 1) = std::min(steps.at(i - 1), step);
    }
    for (size_t i = 0; i < tiles.size(); ++i)
    {
        auto& step = steps.at(i);
        if (step == 0)
        {
            step = 4 - counts[tiles.at(i)];
        }

        // 放大10倍后，个位表示边缘度（越大越靠边）
        step = step * 10 + std::abs(4 - MJTilePoint(tiles.at(i)));
    }

    size_t pickIndex = 0;
    pickStep = steps.at(0);
    for (size_t i = 1; i < steps.size(); ++i)
    {
        if (steps.at(i) > pickStep)
        {
            pickIndex = i;
            pickStep = steps.at(i);
        }
    }
    auto tile = tiles.at(pickIndex);
    tiles.erase(tiles.begin() + pickIndex);
    return tile;
}
/// <param name="tiles">从tiles中选出三张</param>
/// <param name="pickSumStep">选出三张的“相邻距离”之和，原值上增加</param>
static std::vector<ITileID> _GetRecommendInColor(std::vector<ITileID> const& tiles, int& pickSumStep)
{
    if (tiles.size() < 3)
    {
        return {};
    }
    auto copyTiles = tiles;
    std::sort(copyTiles.begin(), copyTiles.end());

    std::vector<ITileID> res;
    for (int i = 0; i < 3; ++i)
    {
        int pickStep = 0;
        res.push_back(_PickOneInColor(copyTiles, pickStep));
        pickSumStep += pickStep;
    }
    return res;
}

std::vector<ITileID> MJPlayData::GetExchangeRecommend() const
{
    std::map<EMJColor, std::vector<int>> colorMap{};

    for (auto card : m_HandTiles)
    {
        colorMap[MJTileColor(card)].push_back(card);
    }
    int mightColorCount = 0;
    size_t minMightColorCardCount = 99;  //max
    for (auto& p : colorMap)
    {
        if (p.second.size() >= 3)
        {
            mightColorCount++;
            if (p.second.size() < minMightColorCardCount)
            {
                minMightColorCardCount = p.second.size();
            }
        }
    }
    if (mightColorCount <= 1)
    {
        return {};
    }

    static const int ColorStepFix[] = {0, 1, 0};  // 条少一张，让它更容易被换

    std::vector<ITileID> selected;
    int maxPickSumStep = -1;

    for (auto& p : colorMap)
    {
        if (p.second.size() == minMightColorCardCount)
        {
            int pickSumStep = ColorStepFix[(int)p.first % std::size(ColorStepFix)];
            auto temp = _GetRecommendInColor(p.second, pickSumStep);
            if (pickSumStep > maxPickSumStep)
            {
                selected = temp;
                maxPickSumStep = pickSumStep;
            }
        }
    }
    return selected;
}

bool MJPlayData::CanExchangeOut(std::vector<ITileID> const& tiles) const
{
    if (tiles.empty())
    {
        return false;
    }

    std::map<ITileID, int> tileCount{};
    for (auto card : m_HandTiles)
    {
        ++tileCount[card];
    }
    auto col = MJTileColor(tiles.front());
    for (auto card : tiles)
    {
        if (MJTileColor(card) != col)
        {
            return false;
        }
        if (tileCount.find(card) == tileCount.end())
        {
            return false;
        }
        if ((--tileCount[card]) < 0)
        {
            return false;
        }
    }
    return true;
}

void MJPlayData::ExchangeOut(std::vector<ITileID> const& outs)
{
    for (auto card : outs)
    {
        RemoveHandTile(card);
    }
}

void MJPlayData::ExchangeIn(std::vector<ITileID> const& ines)
{
    for (auto card : ines)
    {
        this->PutHandTile(card);
    }
}

void MJPlayData::Exchange(std::vector<ITileID> const& outs, std::vector<ITileID> const& ines)
{
    ExchangeOut(outs);
    ExchangeIn(ines);
}

void MJPlayData::IncEverPonCount()
{
    ++m_EverPonCount;
}

void MJPlayData::IncEverKonCount()
{
    ++m_EverKonCount;
}
void MJPlayData::IncEverChiCount()
{
    ++m_EverChiCount;
}

size_t MJPlayData::GetEverPonCount() const
{
    return m_EverPonCount;
}

size_t MJPlayData::GetEverKonCount() const
{
    return m_EverKonCount;
}

size_t MJPlayData::GetEverChiCount() const
{
    return m_EverChiCount;
}

int MJPlayData::GetMinTing()
{
    UpdateMinTing();
    return m_MinTingValue;
}

void MJPlayData::SetDrawConfig(int drawConfig, int drawRate)
{
    m_DrawConfig = drawConfig;
    m_DrawRate = drawRate;
}

int MJPlayData::GetDrawConfig()
{
    return m_DrawConfig;
}

int MJPlayData::GetDrawRate()
{
    return m_DrawRate;
}
