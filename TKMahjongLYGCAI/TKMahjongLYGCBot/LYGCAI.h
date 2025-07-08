#pragma once
#include "LYGCBotDefine.h"

#include <string>
#include <tuple>
#include "TKBotPlayer.h"
#include "TKBuffer.h"

#include "LYGCTableData.h"

using namespace std;

class LYGCBot;

enum class AIAction
{
    None = 0,
    Draw = 1,
    Discard = 2,
    Win = 3,
};

struct AIReplyData
{
    bool Success{};
    std::string Action;
    std::string Content;
    int type;
};

/// @brief AI网络消息层
class LYGCAI
{
public:
    LYGCAI(LYGCBot* pBotPlayer);
    ~LYGCAI(void);

    void Reset(UserID uid, int level);

    bool IsValid() const;

    void InitRecordId(std::array<UserID, NP4> const& users, DWORD dwMatchId, int nSeat);

public:  // getter
    std::string GetTraceID() const;
    std::string GetRecordID() const;
    int GetAiLevel() const;

    int GetPort() const;
    std::string GetIPString() const;

public:  // web
    AIReplyData HandleWebSeverMsgAck(PTKMSGAIWEBSERVER msg);
    bool _ParseAckJsonData(std::string const& rawString, std::string& o_action, std::string& o_content) const;
    std::string CreateWebServerMsg(std::string const& content, int traceNo, int pid);

private:  // for Init.
    bool _InitUrl();
    void _InitTraceID(UserID dwMyUserID);
    void _UpdateTraceID(int traceNo);

private:
    UserID m_UserID{};
    int m_Level{};

private:
    bool m_IsValid = false;
    std::string m_AiUrl;        // http://a.b.c.d:1234/path/mahjongxx/
    std::string m_IPString;     // a.b.c.d
    std::string m_UrlBasePath;  // /path/mahjongxx/
    std::string m_UrlPathMove;  // /path/mahjongxx/move
    int m_Port{};               // 1234

    std::string m_RecordID;
    std::string m_TraceIDPrefix;  // 除 RequestIndex

    std::string m_TraceID;  // = m_TraceIDPrefix + s(m_RequestID)

    std::string m_LastHttpRequest;  //上次HTTP请求
    LYGCBot* m_BotPlayer = nullptr;
};
