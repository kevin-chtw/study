#pragma once
#include "MJDefine.h"
#include "MJPlayData.h"
class MJGame;
class MJPlay
{
public:
    MJPlay(MJGame* game);
    virtual ~MJPlay() = default;

    virtual void Initialize();

    void AddHistory(ISeatID seat, ITileID tile, int operate, int extra = 0);
    virtual void DumpHistory() const;
    virtual void DoSwitchSeat(ISeatID seat = SEAT_NULL);

    ISeatID GetCurrentSeat() const;

    ITileID GetCurrentTile() const;

    ISeatID GetBanker() const;

protected:
    MJGame* m_Game;
    ISeatID m_CurrentSeat{SEAT_NULL};  //当前出牌玩家
    ITileID m_CurrentTile{TILE_NULL};  //当前动作操作的牌
    ISeatID m_Banker{SEAT_NULL};       //庄
    std::vector<MJAction> m_History;
};
