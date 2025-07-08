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
    ISeatID m_CurrentSeat{SEAT_NULL};  //��ǰ�������
    ITileID m_CurrentTile{TILE_NULL};  //��ǰ������������
    ISeatID m_Banker{SEAT_NULL};       //ׯ
    std::vector<MJAction> m_History;
};
