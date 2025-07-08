#pragma once
#include "LYGCBotDefine.h"
#include "LYGCDefine.h"
#include "MJPlayData.h"

#include <array>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include "MJUtils.h"
#include "MJTile.h"

struct HistoryItem
{
    ISeatID seat{};
    EOperate operate{};
    ITileID tile{};
};
/// @brief Bot≈∆◊¿π‹¿Ì¿‡
class LYGCTableData
{
private:
    ISeatID m_Seat = SEAT_NULL;
    ISeatID m_Banker = SEAT_NULL;
    ITileID m_CiTile = TILE_NULL;
    std::vector<HistoryItem> m_History;
    Json::Value m_HistroyJsonAI;

    bool m_IsOut = false;

    MJPlayData m_SelfData;
    std::map<EOperate, std::string> m_mOperateName{g_MJOperateNames};

public:
    void Reset(ISeatID mySeat);

    void SetBanker(ISeatID banker);
    void SetCiTile(ITileID ciTile);

    MJPlayData& GetSelfData();
    LYGCTableData();

public:
    std::string CreateJsonContent(MJOperates operates);
    std::string GetActionString(MJOperates operates, bool hasPass) const;
    // self cards
    void RecordHistory(ISeatID seat, EOperate operate, ITileID tile);
    void HandleWin(std::vector<ISeatID> winSeats, ITileID tile, ISeatID paoSeat);

public:  // get&set
    ISeatID GetSeat() const;
};
