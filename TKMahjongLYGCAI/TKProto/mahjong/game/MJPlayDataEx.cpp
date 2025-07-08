#include "MJPlayDataEx.h"
#include "MJHelper.h"
#include "MJService.h"
#include "MJGame.h"

bool MJPlayDataEx::CanZhiKon(ITileID tile, std::shared_ptr<MJConfig> cfg)
{
    m_GangTiles.clear();
    int count = MJ::CountElement(m_HandTiles, tile);
    if (count == 3)
    {
        if (m_Call && !_CanKonAfterCall(tile, KonType::Zhi, cfg))
        {
            return false;
        }
        m_GangTiles.push_back(tile);
        return true;
    }
    return false;
}

bool MJPlayDataEx::CanSelfKon(std::shared_ptr<MJConfig> cfg, std::vector<ITileID> const& ignoreTiles)
{
    m_GangTiles.clear();

    std::map<ITileID, int> counts;
    for (auto tile : m_HandTiles)
    {
        if (std::find(ignoreTiles.begin(), ignoreTiles.end(), tile) == ignoreTiles.end())
        {
            ++counts[tile];
        }
    }
    if (!m_Call)
    {
        for (auto pon : m_PonGroups)
        {
            if (HasTile(pon.tile))
            {
                m_GangTiles.push_back(pon.tile);
            }
        }
        for (auto& p : counts)
        {
            if (p.second == 4)
            {
                m_GangTiles.push_back(p.first);
            }
        }
        return !m_GangTiles.empty();
    }
    //об╫п╨С
    for (auto pon : m_PonGroups)
    {
        if (pon.tile == m_HandTiles.back())
        {
            m_GangTiles.push_back(pon.tile);
            return true;
        }
    }
    if (counts[m_HandTiles.back()] == 4 && _CanKonAfterCall(m_HandTiles.back(), KonType::An, cfg))
    {
        m_GangTiles.push_back(m_HandTiles.back());
        return true;
    }
    return false;
}

bool MJPlayDataEx::CanKon(ITileID tile, KonType type) const
{
    int count = MJ::CountElement(m_HandTiles, tile);
    switch (type)
    {
    case KonType::Zhi:
        return count == 3;
    case KonType::An:
        return count == 4;
    case KonType::Bu:
        return count && HasPon(tile);
    default:
        return false;
    }
}

bool MJPlayDataEx::CanChow(ITileID leftTile, ITileID curTile) const
{
    EMJColor color = MJTileColor(leftTile);
    int leftPoint = MJTilePoint(leftTile);

    if (color != MJTileColor(curTile))
    {
        return false;
    }

    if (MJTilePoint(curTile) - leftPoint >= 3)
    {
        return false;
    }

    for (int i = 0; i < 3; ++i)
    {
        ITileID tileId = MJMakeTile(color, leftPoint + i);
        if (tileId == curTile)
        {
            continue;
        }

        if (std::find(m_HandTiles.begin(), m_HandTiles.end(), tileId) == m_HandTiles.end())
        {
            return false;
        }
    }

    return true;
}

bool MJPlayDataEx::CanPon(ITileID tile, bool CantOnlyLaiAfterPon) const
{
    if (MJ::CountElement(m_HandTiles, tile) < 2)
    {
        return false;
    }
    if (CantOnlyLaiAfterPon)
    {
        auto tiles = m_HandTiles;
        int noLaiTilesCnt = 0;
        MJ::RemoveElements(tiles, tile, 2);
        for (auto tile : tiles)
        {
            if (!MJ::HasElement(m_Game->GetPlay()->LaiTiles(), tile))
            {
                ++noLaiTilesCnt;
            }
        }
        return noLaiTilesCnt > 0;
    }
    return true;
}

bool MJPlayDataEx::CanChow(ITileID tile) const
{
    EMJColor color = MJTileColor(tile);
    int point = MJTilePoint(tile);
    std::array<int, MJPointCountByColor[(int)EMJColor::Character]> points = {0};

    for (int i = 0; i < m_HandTiles.size(); ++i)
    {
        if (MJTileColor(m_HandTiles.at(i)) == color)
        {
            ++points[MJTilePoint(m_HandTiles.at(i))];
        }
    }
    ++points[point];

    int leftPoint = std::max((point - 2), 0);
    int maxLeftPoint = std::min(6, point);
    for (int i = leftPoint; i <= maxLeftPoint; ++i)
    {
        if (0 != points[i] && 0 != points[i + 1] && 0 != points[i + 2])
        {
            return true;
        }
    }

    return false;
}

bool MJPlayDataEx::IsTianDiHu() const
{
    return TianDiHuState();
}

bool MJPlayDataEx::IsCurrentDrawTile(ITileID tile) const
{
    return MJ::HasElement(m_CurrentDrawTiles, tile);
}

bool MJPlayDataEx::CanDiscard(ITileID tile) const
{
    return HasTile(tile);
}

void MJPlayDataEx::ClearDraw()
{
    m_CurrentDrawTiles.clear();
}

void MJPlayDataEx::SetRemoveHutypes(std::set<int>& types)
{
    m_RemoveHutypes = types;
}

EHandStyle MJPlayDataEx::GetHandStyle()
{
    auto tiles = GetHandTiles();
    auto callStep = GetMJService()->GetTingTool()->CalcStep(tiles, m_Game->GetPlay()->LaiTiles(), {});
    if (tiles.size() % 3 == 2 && callStep == 0)
    {
        return EHandStyle::TianHu;
    }
    if (callStep == 1)
    {
        return EHandStyle::TianTing;
    }
    if (tiles.size() % 3 != 2 && callStep == 2)
    {
        return EHandStyle::YSYT;
    }
    return EHandStyle::None;
}

void MJPlayDataEx::PickTianHuTile()
{
    std::map<EMJColor, std::vector<ITileID>> tileCounts;
    for (auto tile : m_HandTiles)
    {
        tileCounts[MJTileColor(tile)].push_back(tile);
    }
    auto target = TILE_NULL;
    if (tileCounts.size() == 1)
    {
        auto& tiles = tileCounts.begin()->second;
        target = *std::max_element(tiles.begin(), tiles.end());
    }
    else if (tileCounts.size() == 2)
    {
        EMJColor color = EMJColor::Undefined;
        size_t count = m_HandTiles.size();
        for (auto& p : tileCounts)
        {
            if (p.second.size() <= count)
            {
                color = p.first;
                count = p.second.size();
            }
        }
        auto& tiles = tileCounts[color];
        target = *std::max_element(tiles.begin(), tiles.end());
    }
    if (target != TILE_NULL)
    {
        if (MJ::RemoveElement(m_HandTiles, target))
        {
            m_HandTiles.push_back(target);
        }
    }
}

ITileID MJPlayDataEx::FindAnyInHand(std::set<ITileID> const& tiles)
{
    for (auto tile : m_HandTiles)
    {
        if (tiles.find(tile) != tiles.end())
        {
            return tile;
        }
    }
    return TILE_NULL;
}

void MJPlayDataEx::CloseTianDiHu()
{
    m_TianDiHuState = false;
}

bool MJPlayDataEx::_CanKonAfterCall(ITileID tile, KonType type, std::shared_ptr<MJConfig> cfg)
{
    if (KonType::Zhi != type && tile != m_HandTiles.back())
    {
        return false;
    }
    auto info = MakeHuPlayData();
    info.tilesInHand = GetHandTiles();
    if (KonType::Zhi != type)
    {
        info.tilesInHand.pop_back();
    }
    auto call0 = GetMJService()->GetHuTool()->CheckCall(info, cfg);
    MJ::RemoveAllElement(info.tilesInHand, tile);
    auto call1 = GetMJService()->GetHuTool()->CheckCall(info, cfg);
    return call0.size() == 1 && call1.size() == 1 && MJ::HasSameKeys(call0[0], call1[0]);
}

HuPlayData MJPlayDataEx::MakeHuPlayData(ITileID extraTile, ITileID removeTile, int removeCount) const
{
    HuPlayData data;
    data.removeHuType = m_RemoveHutypes;
    data.tilesInHand = GetHandTiles();
    data.tilesLai = m_Game->GetPlay()->LaiTiles();
    for (auto pon : GetPonGroups())
    {
        data.tilesForPon.push_back(pon.tile);
    }
    for (auto kon : GetKonGroups())
    {
        data.tilesForKon.push_back(kon.tile);
        if (kon.type == KonType::An)
        {
            ++data.countConcealedKon;
        }
    }

    for (auto chow : GetChowGroups())
    {
        data.tilesForChowLeft.push_back(chow.leftTile);
    }

    if (MJIsValidTile(extraTile))
    {
        data.tilesInHand.push_back(extraTile);
        data.paoTile = extraTile;
    }
    MJ::RemoveElements(data.tilesInHand, removeTile, removeCount);
    data.isCall = m_Call;
    return data;
}

std::vector<ITileID> const& MJPlayDataEx::GetGangTiles() const
{
    return m_GangTiles;
}

bool MJPlayDataEx::IsAllLai() const
{
    for (auto tile : m_HandTiles)
    {
        if (!m_Game->GetPlay()->IsLai(tile))
        {
            return false;
        }
    }
    return true;
}

int MJPlayDataEx::CheckType(ITileID tile)
{
    if (_IsUsefulTile(tile))
    {
        return TILE_USEFUL;
    }
    if (_IsSpeacilTile(tile))
    {
        return TILE_SPEACIL;
    }
    return _IsRelateTile(tile) ? TILE_RELATED : TILE_USELESS;
}

void MJPlayDataEx::UpdateMinTing(ITileID tile)
{
    auto tiles = GetHandTiles();
    if (tile != TILE_NULL)
    {
        tiles.push_back(tile);
    }

    auto value = GetMJService()->GetTingTool()->CalcTing(tiles, m_Game->GetPlay()->LaiTiles(), {});
    if (value < m_MinTingValue)
    {
        m_MinTingValue = value;
    }
}

bool MJPlayDataEx::_IsUsefulTile(ITileID tile)
{
    auto hands = GetHandTiles();
    auto ting0 = GetMJService()->GetTingTool()->CalcTing(hands, m_Game->GetPlay()->LaiTiles(), {});
    hands.push_back(tile);
    auto ting1 = GetMJService()->GetTingTool()->CalcTing(hands, m_Game->GetPlay()->LaiTiles(), {});
    return (ting1 < ting0);
}

bool MJPlayDataEx::_IsSpeacilTile(ITileID tile)
{
    auto color = MJTileColor(tile);
    if (color == EMJColor::Flower || color == EMJColor::Season)
    {
        return true;
    }

    if (MJ::CountElement(GetHandTiles(), tile) == 3)
    {
        return true;
    }

    for (auto& group : GetPonGroups())
    {
        if (group.tile == tile)
        {
            return true;
        }
    }
    return false;
}

bool MJPlayDataEx::_IsRelateTile(ITileID tile)
{
    std::set<ITileID> tiles{tile};
    if (MJIsSuitTile(tile))
    {
        auto color = MJTileColor(tile);
        auto point = MJTilePoint(tile);
        if (point > 0)
        {
            tiles.insert(MJMakeTile(color, point - 1));
        }
        if (point < 8)
        {
            tiles.insert(MJMakeTile(color, point + 1));
        }
    }

    return MJ::HasAnyElement(GetHandTiles(), tiles);
}
