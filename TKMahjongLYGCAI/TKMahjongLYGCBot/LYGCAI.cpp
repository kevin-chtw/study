#include "LYGCBotDefine.h"

#include "LYGCAI.h"
#include <WS2tcpip.h>
#include <WinSock2.h>
#include "TKDllBotInterface.h"
#include "MJHttp.h"
#include "MJUtils.h"
#include "LYGCBot.h"

class HttpRequestData;
class HttpResponseData;

unsigned long long GetCurrentTimeMsec()
{
    timeval tv{};
    time_t clock;
    tm tm{};
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tv.tv_sec = clock;
    tv.tv_usec = wtm.wMilliseconds * 1000;
    return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
}

LYGCAI::LYGCAI(LYGCBot* pBotPlayer)
{
    m_BotPlayer = pBotPlayer;
}

LYGCAI::~LYGCAI(void)
{
}

void LYGCAI::Reset(UserID uid, int level)
{
    m_UserID = uid;
    m_Level = level;

    m_IsValid = _InitUrl();
    _InitTraceID(uid);
}

bool LYGCAI::_InitUrl()
{
    auto defaultURL = DEFAULT_AI_SERVICE_URL;
    auto initKey = DEFAULT_AI_URL_KEY;

    char buf[512];
    int nCount = GetPrivateProfileString("AISpecific", initKey, defaultURL, buf, sizeof(buf), ".\\TKGeneralServer.ini");
    if (nCount <= 0)
    {
        MJ_LOG_ERROR(m_BotPlayer, "invalid ai url config");
        return false;
    }
    m_AiUrl = buf;

    //根据配置解析出对应的http访问信息
    std::string ipSign = "://";
    std::string portSign = ":";
    std::string pathSign = "/";
    auto ipStart = m_AiUrl.find(ipSign);
    if (ipStart == m_AiUrl.npos)
    {
        MJ_LOG_ERROR(m_BotPlayer, "invalid url ip(://) :%s", _S(m_AiUrl));
        return false;
    }
    ipStart += ipSign.size();
    auto ipEnd = m_AiUrl.find(portSign, ipStart);
    if (ipEnd == m_AiUrl.npos)
    {
        MJ_LOG_ERROR(m_BotPlayer, "invalid url no port:%s", _S(m_AiUrl));
        return false;
    }
    m_IPString = m_AiUrl.substr(ipStart, ipEnd - ipStart);

    auto portStart = ipEnd + portSign.size();
    auto portEnd = m_AiUrl.find("/", portStart);
    if (portEnd == m_AiUrl.npos)
    {
        MJ_LOG_ERROR(m_BotPlayer, "invalid url port:%s", _S(m_AiUrl));
        return false;
    }
    std::string strport = m_AiUrl.substr(portStart, portEnd - portStart);
    m_Port = strtol(strport.c_str(), nullptr, 10);

    m_UrlBasePath = m_AiUrl.substr(portEnd);
    m_UrlPathMove = m_UrlBasePath + "move";
    return true;
}

void LYGCAI::_InitTraceID(UserID dwMyUserID)
{
    char szId[256] = {0};

    sockaddr_in sa{};

    // store this IP address in sa:
    inet_pton(AF_INET, m_IPString.c_str(), &(sa.sin_addr));

    std::string strLocalIP;
    char szHex[10] = {0};
    sprintf_s(szHex, "%02x", sa.sin_addr.S_un.S_un_b.s_b1);
    strLocalIP += szHex;
    sprintf_s(szHex, "%02x", sa.sin_addr.S_un.S_un_b.s_b2);
    strLocalIP += szHex;
    sprintf_s(szHex, "%02x", sa.sin_addr.S_un.S_un_b.s_b3);
    strLocalIP += szHex;
    sprintf_s(szHex, "%02x", sa.sin_addr.S_un.S_un_b.s_b4);
    strLocalIP += szHex;
    sprintf_s(szId, "%s%lld%05x%010u", strLocalIP.c_str(), GetCurrentTimeMsec(), GetCurrentProcessId(), dwMyUserID);

    m_TraceIDPrefix = szId;
}

void LYGCAI::_UpdateTraceID(int traceNo)
{
    //////////////////////////////////////////////////////////////////////////
    // traceid
    //例如:c0a83e01159229368304103dec00010099381002
    //|8位ip|13位时间戳|5位进程id|10位机器人uid|4位自增序列
    //共40位
    //////////////////////////////////////////////////////////////////////////
    auto ss = std::to_string(10000 + traceNo);
    m_TraceID = m_TraceIDPrefix + ss.substr(ss.size() - 4);
}

void LYGCAI::InitRecordId(std::array<UserID, NP4> const& users, DWORD dwMatchId, int nSeat)
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    char szId[256] = {0};
    sprintf_s(szId, "M%dG%dP%uP%uP%uP%uT%d%02d%02d%02d%02d%02d", dwMatchId, TK_GAMEID_MJLYGC, (unsigned)users[0], (unsigned)users[1], (unsigned)users[2], (unsigned)users[3], st.wYear, st.wMonth,
        st.wDay, st.wHour, st.wMinute, st.wSecond);
    m_RecordID = szId;
}

bool LYGCAI::IsValid() const
{
    return m_IsValid;
}

std::string LYGCAI::GetTraceID() const
{
    return m_TraceID;
}

std::string LYGCAI::GetRecordID() const
{
    return m_RecordID;
}

int LYGCAI::GetAiLevel() const
{
    return m_Level;
}

int LYGCAI::GetPort() const
{
    return m_Port;
}

std::string LYGCAI::GetIPString() const
{
    return m_IPString;
}

std::string LYGCAI::CreateWebServerMsg(std::string const& content, int traceNo, int pid)
{
    _UpdateTraceID(traceNo);

    std::stringstream stream;
    stream << "POST " << (m_UrlPathMove);
    stream << " HTTP/1.1\r\n";
    stream << "Host: " << m_IPString << "\r\n";
    stream << "Content-Type:application/json\r\n";
    stream << "Accept:application/json\r\n";  // accept请求报头域，表示客户端接受哪些类型的信息
    stream << "mpid:" << pid << "\r\n";
    stream << "traceid:" << GetTraceID() << "\r\n";
    stream << "recordid:" << GetRecordID() << "\r\n";
    stream << "gameid:" << TK_GAMEID_MJLYGC << "\r\n";
    stream << "aitype:" << (int)m_BotPlayer->m_byAIType << "\r\n";
    stream << "level:" << GetAiLevel() << "\r\n";
    stream << "time:" << time(nullptr) << "\r\n";
    stream << "Content-Length:" << content.length() << "\r\n";
    stream << "connection:close\r\n\r\n";
    stream << content.c_str();

    m_LastHttpRequest = stream.str();
    return stream.str();
}

AIReplyData LYGCAI::HandleWebSeverMsgAck(PTKMSGAIWEBSERVER msg)
{
    AIReplyData reply{};
    auto begin = (char*)(msg) + TKHEADERSIZE + msg->offset;
    auto end = begin + msg->dataLength;
    std::string retStr(begin, end);
    HttpResponseData httpResponsHandler{};
    if (!httpResponsHandler.ParseData(retStr))
    {
        MJ_LOG_ERROR(m_BotPlayer, "http parse error %s", _S(retStr));
        return reply;
    }
    MJ_LOG_DEBUG(m_BotPlayer, "recv AI msg=%s", retStr.c_str());
    auto ackCode = httpResponsHandler.GetCode();

    if (ackCode == 200)
    {
        auto httpContent = httpResponsHandler.GetContent();
        std::string action, content;
        if (!_ParseAckJsonData(httpContent, std::ref(action), std::ref(content)))
        {
            MJ_LOG_ERROR(m_BotPlayer, "parse content error %s", _S(httpContent));
            return reply;
        }

        reply.Success = true;
        reply.Action = action;
        reply.Content = content;

        return reply;
    }
    else
    {
        MJ_LOG_ERROR(m_BotPlayer, "WebServerAck Error ack:%d req:%s res:%s", ackCode, _S(m_LastHttpRequest), _S(retStr));
    }

    return reply;
}

bool LYGCAI::_ParseAckJsonData(std::string const& rawString, std::string& o_action, std::string& o_content) const
{
    try
    {
        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(rawString, root, false) || !root.isObject())
        {
            return false;
        }
        auto jsonAction = root["action"];
        if (!jsonAction.isString())
        {
            return false;
        }
        o_action = jsonAction.asString();

        auto jsonContent = root["content"];
        o_content = jsonContent.isString() ? jsonContent.asString() : ""s;

        return true;
    }
    catch (...)
    {
        MJ_LOG_ERROR(m_BotPlayer, "AI Error %s", _S(rawString));
        return false;
    }
}
