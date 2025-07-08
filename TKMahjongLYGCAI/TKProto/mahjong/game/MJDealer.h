#pragma once

#include <map>
#include <vector>
#include <string>
#include "MJTing.h"
#include "MJDefine.h"
#include "MJPlay.h"
#include "MJTile.h"
class MJGame;

namespace MJ
{
/// <summary>
/// �������ӿ�
/// </summary>
class Dealer
{
public:
    Dealer(MJGame* game) : m_Game(game){};
    virtual ~Dealer(){};

public:
    virtual void Initialize(){};
    virtual ITileID DrawTile(ISeatID drawSeat, MJPlayData* playerData = nullptr);

    ITileID GetTile(size_t index) const;
    size_t GetRestTileCount() const;
    void Exchange(std::vector<ITileID>& in_out__ tiles);  //���� ���ڻ�����ʱֻ��һ����һ���

protected:
    virtual const std::map<ITileID, int>& TilesMap() = 0;

    bool LoadManual(std::string const& name, int initCardsFileNo, int playerCount = 4, int bankerTileCount = 14);
    void InitRandom();

    ITileID _PopTile(size_t index = 0);
    ITileID _DrawSelfTiles(ISeatID drawSeat);
    ITileID _DrawTile(ITileID need = TILE_NULL);
    virtual ITileID _DrawStratety(MJPlayData* typer);

protected:
    std::deque<ITileID> m_TileWall;
    std::map<ISeatID, std::deque<ITileID>> m_SelfTiles;  //˽���ƿ�
    MJGame* m_Game{};
    bool m_IsManual{false};
};

using DrawTileType = int;

/// ���ƾ��ߣ������ӷ�����������������
/// ��������'configIndex' ��ʾ��������򣬵�ǰʹ��������С��������Ӧ���ֵ�����ʹ������ֵ��ȷ����Ӧ��ϵ��
template <size_t TypeCount>
class DrawStrategy
{
public:
    DrawStrategy(MJGame* game) : m_Game(game){};
    static const size_t MaxLevel = 3;
    struct ConfigItem
    {
        int drawCount{1};
        std::array<int, TypeCount> weights{};
    };

    // ʹ���Զ�������
    void Init(std::vector<ConfigItem> configs);

    // ��ǰ�������Ƿ���Ч
    bool IsValid() const;

    //����configIndex��Ӧ�����У�Ԥ�����ź�ѡ�ƣ�
    size_t GetDrawCount(size_t configIndex);

    // ���ߴ�dealer���������ƣ�������dealer��
    virtual ITileID Draw(Dealer const* dealer, MJPlayData* typer, size_t configIndex);

    //����types��ѡ������һ�ţ�����ֵ��Ӧtypes�±꣩
    virtual size_t DrawIndex(std::vector<DrawTileType> const& types, size_t configIndex);

protected:
    //����index�ķ�Χ
    size_t _FixConfigIndex(size_t configIndex);
    MJGame* m_Game{};
    std::vector<ConfigItem> m_Configs;
};

template <size_t TypeCount>
size_t MJ::DrawStrategy<TypeCount>::_FixConfigIndex(size_t configIndex)
{
    if (configIndex >= m_Configs.size())
    {
        if (m_Configs.size() == 0)
        {
            return 0;
        }
        return m_Configs.size() - 1;
    }
    return configIndex;
}

template <size_t TypeCount>
void MJ::DrawStrategy<TypeCount>::Init(std::vector<ConfigItem> configs)
{
    m_Configs = configs;
}

template <size_t TypeCount>
bool MJ::DrawStrategy<TypeCount>::IsValid() const
{
    return m_Configs.size() == TypeCount;
}

template <size_t TypeCount>
inline ITileID DrawStrategy<TypeCount>::Draw(Dealer const* dealer, MJPlayData* typer, size_t configIndex)
{
    auto count = dealer->GetRestTileCount();
    if (count == 0)
    {
        return TILE_NULL;
    }

    auto ci = _FixConfigIndex(configIndex);
    count = (std::min)(GetDrawCount(ci), count);
    if (count <= 1)
    {
        return dealer->GetTile(0);
    }
    std::vector<DrawTileType> types;
    for (size_t i = 0; i < count; ++i)
    {
        auto tile = dealer->GetTile(i);
        types.push_back(typer->CheckType(tile));
    }

#if _DEBUG
    std::vector<ITileID> tiles;
    for (size_t i = 0; i < count; ++i)
    {
        tiles.push_back(dealer->GetTile(i));
    }

    std::string debugstr;
    debugstr = "types : " + MJ::ContainerToString(types);
    debugstr += " tiles : " + MJ::GetTileAINameCvt()->IdsToNames(tiles);
    debugstr += " minTingStep : " + MJ::ToString(configIndex);
    m_Game->AddDebugStr(debugstr);
#endif
    return dealer->GetTile(DrawIndex(types, configIndex));
}

template <size_t TypeCount>
size_t MJ::DrawStrategy<TypeCount>::DrawIndex(std::vector<DrawTileType> const& types, size_t configIndex)
{
    if (types.size() < 2 || m_Configs.empty())
    {
        return 0;
    }
    configIndex = _FixConfigIndex(configIndex);
    std::vector<int> weights;
    for (auto type : types)
    {
        if (type < 0 || type >= (int)m_Configs.size())
        {
            weights.push_back(0);
        }
        else
        {
            weights.push_back(m_Configs.at(configIndex).weights.at(int(type)));
        }
    }
    return MJ::RandomByRates(weights);
}

template <size_t TypeCount>
size_t MJ::DrawStrategy<TypeCount>::GetDrawCount(size_t configIndex)
{
    if (m_Configs.empty())
    {
        return 0;
    }
    return m_Configs.at(_FixConfigIndex(configIndex)).drawCount;
}

}  // namespace MJ
