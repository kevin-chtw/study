#include "Stdafx.h"
#include "TKJunQiPKBotAI.h"
#include "json.h"
#include <boost/lexical_cast.hpp>

#define DEFAULT_AI_SERVICE_URL "http://test.ml.jjweb.cn:80/api/v2/junqi/move"

CTKJunQiPKBotAI::CTKJunQiPKBotAI()
    : m_MySeat(JUNQIPK_UNKNOWN),
      m_SerialNum(0),
      m_ProductID(0),
      m_nRequestIndex(0),
      m_LeftDiceNum(0),
      m_LastActionSeat(-1),
      m_DrawFlag(0)
{
    Init();
}

CTKJunQiPKBotAI::~CTKJunQiPKBotAI()
{
}

void CTKJunQiPKBotAI::SetDiceNum(const int diceNum)
{
    m_LeftDiceNum = diceNum;
}

void CTKJunQiPKBotAI::OnInitialUpdate(int moveToPosition, int selectPosition, int repeatCount)
{
}

void CTKJunQiPKBotAI::Init()
{
    char buf[512];
    memset(buf, 0, sizeof(buf));
    int nCount = GetPrivateProfileString("AISpecific", "TKJunQiPKBotUrl", DEFAULT_AI_SERVICE_URL, buf, sizeof(buf),
                                         ".\\TKGeneralServer.ini");

    if (nCount >= 0)
    {
        m_SendUrl = buf;
    }

    //根据配置解析出对应的http访问信息
    size_t pos = m_SendUrl.find("://");
    if (pos == std::string::npos)
    {
        return;
    }
    size_t startIndex = pos + 3;
    size_t endIndex = m_SendUrl.find(":", startIndex);
    if (endIndex - startIndex <= 0)
    {
        return;
    }
    m_URLIP = m_SendUrl.substr(startIndex, endIndex - startIndex);
    startIndex = endIndex + 1;
    endIndex = m_SendUrl.find("/", startIndex);
    if (endIndex - startIndex <= 0)
    {
        return;
    }
    std::string strport = m_SendUrl.substr(startIndex, endIndex - startIndex);
    m_Port = boost::lexical_cast<int>(strport.c_str());
    if (endIndex < m_SendUrl.size())
    {
        m_URLPath = m_SendUrl.substr(endIndex);
    }
}
void CTKJunQiPKBotAI::ReSet()
{
    m_HistoryMoves.clear();
    m_HistoryMoves.resize(0);
    m_LastActionSeat = -1;
}

void CTKJunQiPKBotAI::SetRePeace(const int peace)
{
    m_DrawFlag = peace;
}

int CTKJunQiPKBotAI::GetPeace()
{
    return m_DrawFlag;
}

void CTKJunQiPKBotAI::SetLastActionSeat(const int seat)
{
    m_LastActionSeat = seat;
}

void CTKJunQiPKBotAI::ToJson(Json::Value& dest)
{
    int pos = m_LastActionSeat == 0 ? 1 : 0;
    if (m_LastActionSeat == -1)
    {
        pos = m_MySeat;
    }
    dest["pos"] = pos;
    dest["seqs"] = m_HistoryMoves;
    dest["masked"] = m_LeftDiceNum;
    dest["draw"] = m_DrawFlag;
}

int CTKJunQiPKBotAI::GetIpPort(std::string& ip, int& port)
{
    ip = m_URLIP;
    port = m_Port;
    return 0;
}
int CTKJunQiPKBotAI::GetSerialNum()
{
    return m_SerialNum;
}
void CTKJunQiPKBotAI::DecDiceNum()
{
    --m_LeftDiceNum;
}
void CTKJunQiPKBotAI::AddHistroy(const std::string& history)
{
    m_HistoryMoves.append(history);
}

string GetLocalIpAddress()
{
    string strLocalIP = "00000000";
    WORD wVersionRequested = MAKEWORD(2, 2);

    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
        return strLocalIP;

    char szLocal[255] = {0};
    gethostname(szLocal, sizeof(szLocal));
    hostent* ph = gethostbyname(szLocal);
    if (ph == NULL)
        return strLocalIP;

    if (NULL == ph->h_addr_list[0])
        return strLocalIP;

    in_addr addr;
    memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));  // 这里仅获取第一个ip

    strLocalIP.clear();
    char szHex[10] = {0};
    sprintf_s(szHex, "%02x", addr.S_un.S_un_b.s_b1);
    strLocalIP += szHex;
    sprintf_s(szHex, "%02x", addr.S_un.S_un_b.s_b2);
    strLocalIP += szHex;
    sprintf_s(szHex, "%02x", addr.S_un.S_un_b.s_b3);
    strLocalIP += szHex;
    sprintf_s(szHex, "%02x", addr.S_un.S_un_b.s_b4);
    strLocalIP += szHex;

    WSACleanup();
    return strLocalIP;
}

unsigned long long GetCurrentTimeMsec()
{
#ifdef _WIN32
    struct timeval tv;
    time_t clock;
    struct tm tm;
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
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#endif
}

void CTKJunQiPKBotAI::SetTraceID(DWORD dwMyUserID)
{
    char szId[256] = {0};
    sprintf_s(szId, "%s%lld%05x%010u", GetLocalIpAddress().c_str(), GetCurrentTimeMsec(), GetCurrentProcessId(),
              dwMyUserID);
    m_TraceId = szId;
}

void CTKJunQiPKBotAI::GetTraceIDString(std::string& strRequest)
{
    //////////////////////////////////////////////////////////////////////////
    // traceid
    //例如:c0a83e01159229368304103dec00010099381002
    //|8位ip|13位时间戳|5位进程id|10位机器人uid|4位自增序列
    //共40位
    //////////////////////////////////////////////////////////////////////////
    strRequest = m_TraceId + std::to_string((long long)(m_nRequestIndex++));
}

void CTKJunQiPKBotAI::SetRecordID(DWORD dwPlayerId0, DWORD dwPlayerId1, DWORD dwMatchId, int nSeat)
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    char szId[256] = {0};
    sprintf_s(szId, "M%dG%dP%uP%uT%d%02d%02d%02d%02d%02d", dwMatchId, TK_GAMEID_JUNQI, dwPlayerId0, dwPlayerId1,
              st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    m_RecordId = szId;
}

void CTKJunQiPKBotAI::CreateWebServerMsg(CTKBuffer& buf, int aiType)
{
    std::stringstream stream;

    Json::Value json;
    ToJson(json);
    string post_content = json.toStyledString();

    stream << "POST " << m_URLPath;
    stream << " HTTP/1.1\r\n";
    stream << "Host: " << m_URLIP << "\r\n";
    stream << "Content-Type:application/json\r\n";
    stream << "Accept: */*,application/json\r\n";  // accept请求报头域，表示客户端接受哪些类型的信息
    stream << "Accept-Charset:UTF8\r\n";
    stream << "Accept-Language: zh-cn;q=0.8,en;q=0.6,ja;q=0.4\r\n";
    stream << "mpid:" << m_ProductID << "\r\n";
    stream << "traceid:" << m_TraceId << "\r\n";
    stream << "recordid:" << m_RecordId << "\r\n";
    stream << "gameid:" << TK_GAMEID_JUNQIPK << "\r\n";
    stream << "aitype:" << aiType << "\r\n";
    stream << "level:" << 101 << "\r\n";
    stream << "Content-Length:" << post_content.length() << "\r\n";
    stream << "connection:close\r\n\r\n";
    stream << post_content.c_str();

    DEBUG_LOG("SendToAI:%s", stream.str().c_str());

    buf.AppendString(stream.str().c_str());

    PTKHEADER header = (PTKHEADER)buf.GetBufPtr();
    header->dwSerial = ++m_SerialNum;
    header->dwLength = stream.str().length();

    m_AIActTypeReturn = "";     //返回类型
    m_AIActContentReturn = "";  //返回内容
}
