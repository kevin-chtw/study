#pragma once
#include "Stdafx.h"
#include <json.h>
#include "TKBotPlayer.h"
#include "TKJunQiPKBotDefine.h"
#include <ctime>
#include <queue>
#if _DEBUG
#define DEBUG_LOG(...) TKWriteLog(__VA_ARGS__)
#else
#define DEBUG_LOG(...) void(0)
#endif

typedef pair<int, double> PAIR;

template <typename T>
struct CompareByChessValue
{
    bool operator()(const T& lhs, const T& rhs)
    {
        return lhs.second > rhs.second;
    }
};

class CTKJunQiPKBotAI
{
public:
    CTKJunQiPKBotAI();
    ~CTKJunQiPKBotAI();

public:
    void OnInitialUpdate(int moveToPosition, int selectPosition, int repeatCount);
    void SetDiceNum(const int diceNum);
    void CreateWebServerMsg(CTKBuffer& buf, int aiType);
    void SetTraceID(DWORD dwMyUserID);
    void SetRecordID(DWORD dwPlayerId0, DWORD dwPlayerId1, DWORD dwMatchId, int nSeat);
    void AddHistroy(const std::string& history);
    void DecDiceNum();
    int GetSerialNum();
    int GetIpPort(std::string& ip, int& port);
    void SetLastActionSeat(const int seat);
    void ReSet();
    void SetRePeace(const int peace);
    int GetPeace();

    void SetProductId(const DWORD pid)
    {
        m_ProductID = pid;
    };

private:
    void ToJson(Json::Value& dest);
    void GetTraceIDString(std::string& strRequest);
    void Init();

private:
    int m_MySeat;
    int m_LeftDiceNum;

    Json::Value m_HistoryMoves;
    std::string m_AIActTypeReturn;
    std::string m_AIActContentReturn;
    std::string m_RecordId;
    std::string m_TraceId;
    std::string m_SendUrl;
    std::string m_URLPath;
    std::string m_URLIP;
    int m_Port;
    int m_LastActionSeat;
    int m_nRequestIndex;
    DWORD m_ProductID;
    int m_SerialNum;
    int m_DrawFlag;
};
