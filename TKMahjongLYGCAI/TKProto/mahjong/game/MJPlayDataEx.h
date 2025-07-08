#pragma once
#include <map>
#include <set>
#include "MJPlayData.h"
#include "MJConfig.h"
#include "MJModule.h"

//摸牌优化类型  0 有效进张，1 特殊牌 杠牌、花牌等，2 较优牌，3 无效
constexpr int TILE_USEFUL = 0;   //有效进张
constexpr int TILE_SPEACIL = 1;  //特殊进张
constexpr int TILE_RELATED = 2;  //关联进张
constexpr int TILE_USELESS = 3;  //无效进张

class MJPlayDataEx : public MJPlayData, public MJGameModule
{
public:
    using MJGameModule::MJGameModule;

    virtual ~MJPlayDataEx(){};

public:
    virtual bool CanPon(ITileID tile, bool CantOnlyLaiAfterPon) const;
    virtual bool CanChow(ITileID tile) const;
    //用于对方出牌时，查杠权限
    virtual bool CanZhiKon(ITileID tile, std::shared_ptr<MJConfig> cfg);
    //自己回合查杠权限
    virtual bool CanSelfKon(std::shared_ptr<MJConfig> cfg, std::vector<ITileID> const& ignoreTiles);  //任意暗或补

    bool CanDiscard(ITileID tile) const;
    virtual bool CanKon(ITileID tile, KonType type) const;
    bool CanChow(ITileID leftTile, ITileID curTile) const;

    bool IsTianDiHu() const;
    bool IsCurrentDrawTile(ITileID tile) const;

    ITileID FindAnyInHand(std::set<ITileID> const& tiles);
    void CloseTianDiHu();

    void PickTianHuTile();
    void ClearDraw();
    void SetRemoveHutypes(std::set<int>& types);

    virtual EHandStyle GetHandStyle();

    virtual HuPlayData MakeHuPlayData(ITileID extraTile = TILE_NULL, ITileID removeTile = TILE_NULL, int removeCount = 1) const;

    std::vector<ITileID> const& GetGangTiles() const;

    bool IsAllLai() const;

protected:
    //此方法仅用于 CanZhiKon、CanSelfKon，由于这两个调用是可能被重载，_CanKonAfterCall逻辑有变
    //so:不要在其它地方调用此方法
    bool _CanKonAfterCall(ITileID tile, KonType type, std::shared_ptr<MJConfig> cfg);
    std::vector<ITileID> m_GangTiles{};
    std::set<int> m_RemoveHutypes{};

public:  // for MoPai优化
    int CheckType(ITileID tile) override;
    void UpdateMinTing(ITileID tile) override;

protected:
    virtual bool _IsUsefulTile(ITileID tile);   //有效进张
    virtual bool _IsSpeacilTile(ITileID tile);  //特殊牌
    virtual bool _IsRelateTile(ITileID tile);   //关联牌
};
