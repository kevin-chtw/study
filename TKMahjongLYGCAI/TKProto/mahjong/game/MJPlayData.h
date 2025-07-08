#pragma once
#include "MJDefine.h"

struct MJAction
{
    int operate{};
    ISeatID seat{SEAT_NULL};
    ITileID tile{TILE_NULL};
    int extra{};
};

struct Group
{
    ITileID tile{TILE_NULL};
    ISeatID from{SEAT_NULL};
    int extra{};  //扩展，具体含义各游戏自定义
};
struct KonGroup
{
    ITileID tile{TILE_NULL};
    ISeatID from{SEAT_NULL};
    KonType type{};
    bool handPassBuKon = false;  //过手补杠
    ITileID extra{};             //额外的牌 （幺鸡麻将多癞子模式中表示飞杠使用的癞子牌ID）
};

struct ChowGroup
{
    ITileID chowTile{TILE_NULL};
    ISeatID from{SEAT_NULL};
    ITileID leftTile{TILE_NULL};
};

template <typename GroupType>
inline GroupType RemoveGroup(ITileID tile, std::vector<GroupType>& Groups)
{
    auto iter = std::find_if(Groups.begin(), Groups.end(), [&](const auto group) { return group.tile == tile; });
    GroupType Group{};
    if (iter != Groups.end())
    {
        Group = *iter;
        Groups.erase(iter);
    }
    return Group;
}
template <typename GroupType>
inline bool HasGroup(ITileID tile, const std::vector<GroupType>& Groups)
{
    auto iter = std::find_if(Groups.begin(), Groups.end(), [&](const auto group) { return group.tile == tile; });
    if (iter != Groups.end())
    {
        return true;
    }
    return false;
}

using PassHuType = std::map<ITileID, int>;

class MJPlayData
{
public:
    virtual ~MJPlayData(){};

public:
    CallDataMap& MutableCallDataMap();
    CallDataMap const& GetCallDataMap() const;

private:
    CallDataMap m_CallDataMap{};

public:
    void Draw(ITileID tile);

protected:
    std::set<ITileID> m_CurrentDrawTiles{};

public:
    void Discard(ITileID tile);
    void SetCall(ITileID tile, bool tianTing);

public:
    bool m_Call{false};      //是否报叫
    bool m_TianTing{false};  //是否天听

public:
    void PutHandTile(ITileID tile);
    void RemoveHandTile(ITileID tile, size_t count);  //移除手牌
    void RemoveHandTile(ITileID tile);                //移除手牌
    void PutOutTile(ITileID tile);                    //放到出牌区
    void RemoveOutTile();                             //移出出牌区1张
    bool HasTile(ITileID tile) const;
    std::vector<ITileID> const& GetHandTiles() const;
    std::vector<ITileID> const& GetOutTiles() const;

protected:
    std::vector<ITileID> m_HandTiles{};
    std::vector<ITileID> m_OutTiles{};

public:
    void CloseTianDiHu();
    bool TianDiHuState() const;

protected:
    bool m_TianDiHuState{true};

public:
    bool IsPassHuTile(ITileID tile, int fan) const;
    bool IsPassHuTile(ITileID tile) const;
    bool IsPassPonTile(ITileID tile) const;
    void ClearPass();
    void PassPon(ITileID tile);
    void PassHu(ITileID tile, int fan);
    void SetBanQiHuFanTip(bool flag);
    bool IsBanQiHuFanTip() const;

protected:
    std::set<ITileID> m_PassPon{};  //过碰的牌
    PassHuType m_PassHu{};          //过胡的牌
    bool m_QiHuFanLimitTip{};       //封顶番提示,与过胡过碰逻辑一致

public:
    ITileID Chow(ITileID curTile, ITileID tile, ISeatID from);
    const std::vector<ChowGroup>& GetChowGroups() const;

protected:
    std::vector<ChowGroup> m_ChowGroups{};  // {吃牌，来源，吃牌组左侧的牌}

public:
    ITileID Pon(ITileID tile, ISeatID from);
    bool HasPon(ITileID tile) const;
    ITileID KonS(ITileID tile, KonType type, ISeatID from, bool handPassBuKon = false, bool buKonAfterPon = false);
    bool HasKon(ITileID tile) const;
    void PushPon(Group group);
    void PushKon(KonGroup group);
    KonGroup const* GetKon(ITileID tile) const;
    Group const* GetPon(ITileID tile) const;

    Group RemovePon(ITileID tile);
    KonGroup RemoveKon(ITileID tile);
    void RevertKon(ITileID tile);
    std::vector<Group> const& GetPonGroups() const;
    std::vector<KonGroup> const& GetKonGroups() const;

protected:
    std::vector<Group> m_PonGroups{};
    std::vector<KonGroup> m_KonGroups{};

private:
    ITileID _ZhiAnKon(ITileID tile, KonType type, ISeatID from);
    ITileID _BuKon(ITileID tile, bool handPassBuKon = false, bool buKonAfterPon = false);

public:  //换三张
    std::vector<ITileID> GetExchangeRecommend() const;
    bool CanExchangeOut(std::vector<ITileID> const& tiles) const;
    void ExchangeOut(std::vector<ITileID> const& outs);
    void ExchangeIn(std::vector<ITileID> const& ines);
    void Exchange(std::vector<ITileID> const& outs, std::vector<ITileID> const& ines);

public:
    void IncEverPonCount();
    void IncEverKonCount();
    void IncEverChiCount();
    size_t GetEverPonCount() const;
    size_t GetEverKonCount() const;
    size_t GetEverChiCount() const;

protected:
    //操作即记数（非实际数量），补杠后碰数量不减
    size_t m_EverPonCount{};
    size_t m_EverKonCount{};
    size_t m_EverChiCount{};

public:  // for 摸牌优化
    // 0 有效进张，1 杠牌，2 较优牌，3 无效
    virtual int CheckType(ITileID card)
    {
        return 0;
    }
    virtual void UpdateMinTing(ITileID tile = TILE_NULL)
    {
    }
    int GetMinTing();
    void SetDrawConfig(int drawConfig, int drawRate);
    int GetDrawConfig();
    int GetDrawRate();

protected:
    int m_MinTingValue{17};

private:
    int m_DrawConfig{0};
    int m_DrawRate{0};
};
