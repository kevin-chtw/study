#pragma once
#include "MJPlayDataEx.h"
#include "MJModule.h"
#include "LYGCDefine.h"
#include "LYGCHu.h"
struct stCiData
{
    stCiData(EHuType t, int s)
    {
        type = t;
        seat = s;
    }
    stCiData()
    {
    }
    EHuType type = EHT_NONE;
    ISeatID seat = SEAT_NULL;
};

class LYGCPlayData : public MJPlayDataEx
{
public:
    using MJPlayDataEx::MJPlayDataEx;

public:
    bool CanSelfKon(std::shared_ptr<MJConfig> cfg, std::vector<ITileID> const& ignoreTiles) override;
    bool CanSelfCi(std::shared_ptr<MJConfig> cfg, ITileID ciTile);
    bool CanWaitCi(std::shared_ptr<MJConfig> cfg, ITileID ciTile, ITileID outTile);
    EHandStyle GetHandStyle();
    bool IsCanTips(MJTips::Type tips);
    void SetCanTips(MJTips::Type tips, bool bCanTips = false)
    {
        m_mapCanTips[tips] = bCanTips;
    }
    std::vector<ITileID> GetCiTiles() const;

    EHuType GetCiType(ITileID ciTile, ISeatID& nSeat);

    HuPlayData MakeHuPlayData(ITileID extraTile = TILE_NULL, ITileID removeTile = TILE_NULL, int removeCount = 1) const override;

    bool _IsUsefulTile(ITileID tile) override;  //有效进张

protected:
    std::map<MJTips::Type, bool> m_mapCanTips;  // 是否能提示
    std::map<ITileID, stCiData> m_mapCiTiles{};
};
