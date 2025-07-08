#include "LYGCHu.h"
#include "LYGCDefine.h"
#include "MJHu.h"
#include "MJTing.h"
#include "MJHelper.h"
#include "MJService.h"

std::set<EMJColor> _GetColors(std::set<ITileID> const& tiles)
{
    std::set<EMJColor> colors;
    for (auto tile : tiles)
    {
        colors.insert(MJTileColor(tile));
    }
    return colors;
}
bool LYGCHu::CheckHu(HuResult out__& result, HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg)
{
    // 普通胡
    if (!MJ::HuCore::GetInstance()->CheckBasicHu(playerInfo.tilesInHand, 0))
    {
        return false;
    }
    result.huTypes.push_back(EHT_ZiMo);
    _CalcTotalFan(in_out__ result, cfg);
    return true;
}

bool LYGCHu::CheckCi(HuResult out__& result, HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg)
{
    std::map<ITileID, int> counts;
    for (auto tile : playerInfo.tilesInHand)
    {
        ++counts[tile];
    }
    ITileID ciTile{TILE_NULL};
    if (!playerInfo.tilesLai.empty())
    {
        ciTile = playerInfo.tilesLai.front();
    }

    if (counts[ciTile] == 3)
    {  //皮次
        result.huTypes.push_back(EHT_PiCi);
        return true;
    }
    for (auto& p : counts)
    {
        if (p.second == 4 || MJ::HasElement(playerInfo.tilesForPon, p.first))
        {
            auto input = playerInfo;
            auto count = p.second == 4 ? 4 : 1;
            MJ::RemoveElements(input.tilesInHand, p.first, count);
            input.tilesInHand.push_back(ciTile);
            if (!MJ::HuCore::GetInstance()->CheckBasicHu(input.tilesInHand, 0))
            {
                continue;
            }

            if (p.second == 4)
            {
                //暗次
                result.huTypes.push_back(EHT_AnCi);
            }
            else
            {
                //明次
                result.huTypes.push_back(EHT_MingCi);
            }
        }
    }

    return !result.huTypes.empty();
}

CallDataMap LYGCHu::CheckCall(HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg)
{
    auto r = playerInfo.tilesInHand.size() % 3;
    CallDataMap callMap{};
    if (r == 2)
    {
        std::deque<ITileID> checktiles;
        for (auto tile : playerInfo.tilesInHand)
        {
            if (!MJ::HasElement(checktiles, tile))
            {
                checktiles.push_back(tile);
            }
        }
        auto info = playerInfo;
        for (auto tile : checktiles)
        {
            MJ::RemoveElement(info.tilesInHand, tile);  //移出
            auto fans = _CheckCallFan(info, cfg);
            if (!fans.empty())
            {
                callMap[tile] = fans;
            }
            info.tilesInHand.push_back(tile);  //插回
        }
    }
    else if (r == 1)
    {
        auto info = playerInfo;
        auto fans = _CheckCallFan(info, cfg);
        if (!fans.empty())
        {
            callMap[0] = fans;
        }
    }
    return callMap;
}

CallTileFan LYGCHu::_CheckCallFan(HuPlayData& playerInfo, std::shared_ptr<MJConfig> cfg)
{
    CallTileFan fans;
    std::set<ITileID> testTiles;
    bool bBuDaiFeng = cfg->GetValue(ECI_RuleFeng) == 2;
    for (int cor = (int)EMJColor::Character; cor <= (int)EMJColor::Dragon; ++cor)
    {
        if (cor >= (int)EMJColor::Wind && bBuDaiFeng)
        {
            continue;
        }
        for (int p = 0; p < MJPointCountByColor.at(cor); ++p)
        {
            auto tile = MJMakeTile(cor, p);
            testTiles.insert(tile);
        }
    }
    for (auto tile : testTiles)
    {
        playerInfo.tilesInHand.push_back(tile);
        HuResult result;

        if (CheckCi(result, playerInfo, cfg))
        {
            _CalcTotalFan(in_out__ result, cfg);
            fans[tile] = result.totalMultiple;
        }
        else if ((cfg->GetValue(ECI_CiPai) == 0 || playerInfo.extraInfo < cfg->GetValue(ECI_MoBomb)) && CheckHu(result, playerInfo, cfg))
        {
            fans[tile] = result.totalMultiple;
        }

        playerInfo.tilesInHand.pop_back();
    }
    return fans;
}

void LYGCHu::_CalcTotalFan(HuResult out__& result, std::shared_ptr<MJConfig> cfg)
{
    result.totalMultiple = 0;
    if (result.huTypes.empty())
    {
        return;
    }
    switch (result.huTypes.front())
    {
    case EHT_ZiMo:
        result.totalMultiple = 1;
        break;
    case EHT_AnCi:
    case EHT_MingCi:
    case EHT_BaoCi:
        result.totalMultiple = cfg->GetValue(ECI_CiHuBei);
        break;
    case EHT_PiCi:
        result.totalMultiple = cfg->GetValue(ECI_PiCiBei);
        break;
    default:
        break;
    }
}
