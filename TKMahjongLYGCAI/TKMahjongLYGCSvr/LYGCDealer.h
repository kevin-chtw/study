#pragma once

#include "LYGCDefine.h"
#include "MJDealer.h"
#include "MJModule.h"
#include "MJPlayDataEx.h"
#include "MJGame.h"

enum ESDealType
{
    ESDT_AnKe,
    ESDT_AnGang,
    ESDT_DoubleAnKe,
    ESDT_DuiZi,
    ESDT_PingHu,
};

/// <summary>
/// 发牌器
/// </summary>
class LYGCGame;
class LYGCDealer : public MJ::Dealer
{
public:
    using MJ::Dealer::Dealer;
    virtual void Initialize();
    bool testctrl();
    ITileID GetCiTile();
    const std::vector<ITileID>& GetCiAssociationCards()
    {
        return m_vCiAssociationCards;
    }

protected:
    inline LYGCGame* GetGame();

protected:
    bool _CtrlDispatch();

    enum CI_TILE_POINT
    {
        CI_TILE_POINT_1_9,
        CI_TILE_POINT_2_8,
        CI_TILE_POINT_3_7,
        CI_TILE_POINT_ZI,
    };
    void _InitCiTile();
    CI_TILE_POINT _getCiTilePoint();
    bool _checkFitCiPoint(ITileID tile, CI_TILE_POINT ciPoint);

    void _getDuiZiCard(int seat, int nCount);
    void _getShunCard(int seat);
    void _getKeCard(int seat, int nCount);
    void _getGangCard(int seat, int nCount);

    void _putCardIntoHand(int seat, ITileID card, int nCount);
    const std::map<ITileID, int>& TilesMap() override;

    void _takeCardsRandom(int seat, int count);
    bool _checkRandomDealCard(ESDealType type, int callStep, int seat);
    bool _checkFanXing(std::map<ITileID, int> const& mTileCounts, ESDealType type);
    void _swapCards(int seat, int pos);
    void _clearHandCards(int seat);
    void _takeFanXingCards(int seat, ESDealType type);
    void _takeCiAssociationCards(int seat, int count = 1);
    void _checkCiCardPos();

    ITileID _DrawStratety(MJPlayData* typer);

protected:
    ITileID m_CiTile{};
    std::vector<ITileID> m_vCiAssociationCards{};  // 次牌关联张
};

//新手保护发牌
class LYGCNBDealer : public LYGCDealer
{
public:
    LYGCNBDealer(MJGame* game, ISeatID seat);

    void Initialize() override;

private:
    void _baseHuNB(int callStep, int nNeedHua);

    void _getNBGangCiCard(ISeatID seat, int callStep);
    void _getNBCiCardShun(ISeatID seat);

private:
    int m_NBSeat{};
};

// 摸牌优化
class LYGCDrawStrategy : public MJ::DrawStrategy<4>
{
public:
    LYGCDrawStrategy(MJGame* game) : DrawStrategy(game){};

public:
    bool _checkUsefulIsKe(MJPlayData* typer, ITileID tile);
    ITileID _checkChangeUsefulTile(MJPlayData* typer, std::vector<MJ::DrawTileType>& types, MJ::Dealer const* dealer, ITileID nowTile, size_t configIndex);

    ITileID Draw(MJ::Dealer const* dealer, MJPlayData* typer, size_t configIndex)
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
        std::vector<MJ::DrawTileType> types;
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
        auto tileIndex = DrawIndex(types, configIndex);
        if (types.at(tileIndex) == TILE_USEFUL)
        {
            ITileID nowTile = dealer->GetTile(tileIndex);
            return _checkChangeUsefulTile(typer, types, dealer, nowTile, configIndex);
        }
        return dealer->GetTile(tileIndex);
    }

    inline size_t DrawIndex(std::vector<MJ::DrawTileType> const& types, size_t configIndex) override
    {
        if (types.size() < 2 || m_Configs.empty())
        {
            return 0;
        }
        if (configIndex == 2 && types.at(0) == TILE_USEFUL && types.at(1) == TILE_USEFUL)
        {
            //
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
};