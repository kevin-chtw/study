#include "LYGCPlayData.h"
#include "LYGCDefine.h"
#include "MJTing.h"
#include "MJService.h"
#include "MJGame.h"
#include "LYGCDealer.h"

bool LYGCPlayData::CanSelfKon(std::shared_ptr<MJConfig> cfg, std::vector<ITileID> const& ignoreTiles)
{
    if (m_Game->GetPlay()->GetDealer()->GetRestTileCount() == 0)
    {
        return false;
    }

    return MJPlayDataEx::CanSelfKon(cfg, ignoreTiles);
}

bool LYGCPlayData::CanSelfCi(std::shared_ptr<MJConfig> cfg, ITileID ciTile)
{
    m_mapCiTiles.clear();
    std::map<ITileID, int> counts;
    for (auto tile : m_HandTiles)
    {
        ++counts[tile];
    }
    HuResult result{};
    for (auto& p : counts)
    {
        if (p.second == 4 || HasPon(p.first))
        {
            auto input = MakeHuPlayData();
            MJ::RemoveAllElement(input.tilesInHand, p.first);
            input.tilesInHand.push_back(ciTile);
            if (GetMJService()->GetHuTool()->CheckHu(out__ result, input, m_Game->GetConfig()))
            {
                if (p.second == 4)
                {
                    //暗次
                    m_mapCiTiles[p.first] = stCiData(EHT_AnCi, SEAT_ALL);
                }
                else
                {
                    //明次
                    m_mapCiTiles[p.first] = stCiData(EHT_MingCi, this->GetPon(p.first)->from);
                }
            }
        }
    }
    if (counts[ciTile] == 3)
    {  //皮次
        m_mapCiTiles[ciTile] = stCiData(EHT_PiCi, SEAT_ALL);
    }

    return !m_mapCiTiles.empty();
}

bool LYGCPlayData::CanWaitCi(std::shared_ptr<MJConfig> cfg, ITileID ciTile, ITileID outTile)
{
    m_mapCiTiles.clear();

    auto count = MJ::CountElement(m_HandTiles, outTile);
    HuResult result{};

    if (count == 3)
    {
        auto input = MakeHuPlayData(ciTile);
        MJ::RemoveAllElement(input.tilesInHand, outTile);
        if (GetMJService()->GetHuTool()->CheckHu(out__ result, input, m_Game->GetConfig()))
        {
            if (m_Game->GetPlay()->GetDealer()->GetRestTileCount() > 10)
            {
                m_mapCiTiles[outTile] = stCiData(EHT_MingCi, m_Game->GetPlay()->GetCurrentSeat());
            }
            else
            {
                m_mapCiTiles[outTile] = stCiData(EHT_BaoCi, m_Game->GetPlay()->GetCurrentSeat());
            }
        }
    }

    return !m_mapCiTiles.empty();
}

EHandStyle LYGCPlayData::GetHandStyle()
{
    auto tiles = GetHandTiles();
    auto callStep = GetMJService()->GetTingTool()->CalcStep(tiles, m_Game->GetPlay()->LaiTiles(), {});
    if (callStep > 2 || callStep == 0)
    {
        return EHandStyle::None;
    }
    return callStep == 2 ? EHandStyle::YSYT : EHandStyle::TianTing;
}

bool LYGCPlayData::IsCanTips(MJTips::Type tips)
{
    auto it = m_mapCanTips.find(tips);
    if (it == m_mapCanTips.end() || it->second == true)
    {
        return true;
    }
    return false;
}

std::vector<ITileID> LYGCPlayData::GetCiTiles() const
{
    std::vector<ITileID> vCiTiles;
    for (auto& tiles : m_mapCiTiles)
    {
        vCiTiles.push_back(tiles.first);
    }
    return vCiTiles;
}

EHuType LYGCPlayData::GetCiType(ITileID ciTile, ISeatID& nSeat)
{
    if (m_mapCiTiles.find(ciTile) != m_mapCiTiles.end())
    {
        nSeat = m_mapCiTiles[ciTile].seat;
        return m_mapCiTiles[ciTile].type;
    }
    return EHT_NONE;
}

HuPlayData LYGCPlayData::MakeHuPlayData(ITileID extraTile, ITileID removeTile, int removeCount) const
{
    auto data = MJPlayDataEx::MakeHuPlayData(extraTile, removeTile, removeCount);
    data.tilesLai.clear();
    //没有癞子用癞子的位置放置次牌
    data.tilesLai.push_back(m_Game->GetPlay()->GetDealer<LYGCDealer>()->GetCiTile());
    data.extraInfo = m_Game->GetPlay()->GetDealer<LYGCDealer>()->GetRestTileCount();
    return data;
}

bool LYGCPlayData::_IsUsefulTile(ITileID tile)
{
    if (m_Game->GetConfig()->GetValue(ECI_CiPai) != 1)
    {
        return MJPlayDataEx::_IsUsefulTile(tile);
    }

    auto hands = GetHandTiles();
    ITileID ciTile = m_Game->GetPlay()->GetDealer<LYGCDealer>()->GetCiTile();
    auto ciCol = MJTileColor(ciTile);
    if (MJIsHonorColor(ciCol))
    {
        return (tile == ciTile && MJ::CountElement(hands, ciTile) == 1) || MJPlayDataEx::_IsUsefulTile(tile);
    }
    if (MJPlayDataEx::_IsUsefulTile(tile))
    {
        return true;
    }

    auto ciPot = MJTilePoint(ciTile);
    ITileID ciTileA1 = MJMakeTile(ciCol, ciPot + 1);  // 直接加减点数
    ITileID ciTileA2 = MJMakeTile(ciCol, ciPot + 2);
    ITileID ciTileD1 = MJMakeTile(ciCol, ciPot - 1);
    ITileID ciTileD2 = MJMakeTile(ciCol, ciPot - 2);

    std::vector<ITileID> useTile;
    std::vector<ITileID> s1 = {ciTileD1, ciTileA2};
    std::vector<ITileID> s2 = {ciTileA1, ciTileD2};
    std::vector<ITileID> s3 = {ciTileA1, ciTileA2, ciTileD1, ciTileD2};

    if (MJ::HasElement(hands, ciTileA1) && !MJ::HasAnyElement(hands, s1))
    {
        useTile.insert(useTile.end(), s1.begin(), s1.end());
    }
    else if (MJ::HasElement(hands, ciTileD1) && !MJ::HasAnyElement(hands, s2))
    {
        useTile.insert(useTile.end(), s2.begin(), s2.end());
    }
    else if (MJ::HasElement(hands, ciTileA2) && !MJ::HasElement(hands, ciTileA1))
    {
        useTile.push_back(ciTileA1);
    }
    else if (MJ::HasElement(hands, ciTileD2) && !MJ::HasElement(hands, ciTileD1))
    {
        useTile.push_back(ciTileD1);
    }
    else if (MJ::CountElement(hands, ciTile) == 1 && !MJ::HasAnyElement(hands, s3))
    {
        useTile.insert(useTile.end(), s3.begin(), s3.end());
    }
    return MJ::HasElement(useTile, tile);
}