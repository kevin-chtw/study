#include "LYGCTableData.h"
#include <memory>
#include "MJHelper.h"
#include "MJLog.h"

using namespace std::string_literals;

static EOperate ValidOperates[] = {
    EOperateHu,
    EOperateKon,
    EOperatePon,
    EOperateDiscard,
    EOperateTing,
    EOperateCI,
};

void LYGCTableData::Reset(ISeatID mySeat)
{
    m_HistroyJsonAI = Json::Value(Json::arrayValue);
    m_Seat = mySeat;
}
void LYGCTableData::SetBanker(ISeatID banker)
{
    m_Banker = banker;
}
void LYGCTableData::SetCiTile(ITileID ciTile)
{
    m_CiTile = ciTile;
}

MJPlayData& LYGCTableData::GetSelfData()
{
    return m_SelfData;
}

ISeatID LYGCTableData::GetSeat() const
{
    return m_Seat;
}

std::string LYGCTableData::CreateJsonContent(MJOperates operates)
{
    Json::Value root;

    root["seat"] = Json::Value(MJSwitchAISeat(m_Seat));
    root["dealer"] = Json::Value(MJSwitchAISeat(m_Banker));
    root["hand"] = Json::Value(MJ::GetTileAINameCvt()->IdsToNames(m_SelfData.GetHandTiles()));
    root["legal_action"] = Json::Value(GetActionString(operates, m_SelfData.GetHandTiles().size() % 3 != 2));
    root["Tsi_tile"] = Json::Value(MJ::GetTileName(m_CiTile));
    root["history"] = m_HistroyJsonAI;

    return root.toStyledString();
}

std::string LYGCTableData::GetActionString(MJOperates operates, bool hasPass) const
{
    std::string res;
    for (auto operate : ValidOperates)
    {
        if (operates.HasOperate(operate))
        {
            if (res.size())
            {
                res.push_back(',');
            }
            res += GetOperateName(operate, m_mOperateName);
        }
    }
    if (hasPass)
    {
        if (res.size())
        {
            res.push_back(',');
        }
        res += "Pass";
    }
    return res;
}

void LYGCTableData::RecordHistory(ISeatID seat, EOperate operate, ITileID tile)
{
    m_History.push_back({seat, operate, tile});
    m_HistroyJsonAI.append(std::to_string(MJSwitchAISeat(seat)) + "," + GetOperateName(operate) + "," + MJ::GetTileName(tile));
}

void LYGCTableData::HandleWin(std::vector<ISeatID> winSeats, ITileID tile, ISeatID paoSeat)
{
    if (paoSeat == m_Seat)
    {
        auto prev = m_History.back();
        if (prev.operate == EOperateKon)
        {
            m_SelfData.RevertKon(prev.tile);
        }
        else if (prev.operate == EOperateDiscard)
        {
            m_SelfData.RemoveOutTile();
        }
    }
    for (auto seat : winSeats)
    {
        RecordHistory(seat, EOperateHu, tile);
    }
}
LYGCTableData::LYGCTableData()
{
    m_mOperateName[EOperateCI] = "Tsi";
}