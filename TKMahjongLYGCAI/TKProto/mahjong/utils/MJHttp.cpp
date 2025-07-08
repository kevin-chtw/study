#include "MJHttp.h"
#include "MJString.h"
#include "MJLog.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// HttpRequestData
////////////////////////////////////////////////////////////////////////////////////////////////
HttpRequestData::HttpRequestData(std::string strMethod, std::string strHost, std::string strPath)
{
    m_Method = strMethod;
    m_ServerHost = strHost;
    m_RequestPath = strPath;
    m_bClose = false;
}

void HttpRequestData::SetAccept(std::string strAccept)
{
    m_AcceptType = strAccept;
}

void HttpRequestData::SetContent(std::string strConType, std::string strContent)
{
    m_ContentDataType = strConType;
    m_ContentString = strContent;
}

void HttpRequestData::PushHeader(std::string sKey, std::string sValue)
{
    m_HeaderMap[sKey] = sValue;
}

void HttpRequestData::SetClose(bool bClose)
{
    m_bClose = bClose;
}

std::string HttpRequestData::ToRequestData()
{
    std::stringstream stream;
    stream << m_Method << " " << m_RequestPath;
    stream << " HTTP/1.1\r\n";
    stream << "Host: " << m_ServerHost << "\r\n";
    stream << "Content-Type:" << m_ContentDataType << "\r\n";
    stream << "Accept:" << m_AcceptType << "\r\n";
    stream << "Content-Length:" << m_ContentString.length() << "\r\n";

    // 写入自定义header
    for (auto iter = m_HeaderMap.begin(); iter != m_HeaderMap.end(); iter++)
    {
        stream << iter->first << ":" << iter->second << "\r\n";
    }

    if (m_bClose)
    {
        stream << "connection:close\r\n";
    }

    stream << "\r\n";  // 内容分隔行

    // 写入具体内容
    stream << m_ContentString.c_str();
    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// HttpResponseData
////////////////////////////////////////////////////////////////////////////////////////////////
int HttpResponseData::GetCode()
{
    return m_HttpCode;
}

std::string HttpResponseData::GetContent()
{
    return m_ContentString;
}

std::string HttpResponseData::GetHeader(const std::string& key)
{
    std::map<std::string, std::string>::iterator iter;
    iter = m_HeaderMap.find(key);
    if (iter != m_HeaderMap.end())
    {
        return iter->second;
    }
    return std::string();
}

bool HttpResponseData::ParseData(std::string const& strData)
{
    //  按分割http头和content数据
    std::string header;
    auto pos = strData.find("\r\n\r\n");
    if (pos == std::string::npos)
    {
        m_ContentString = "";
        return ParseHeader(strData);
    }
    else
    {
        m_ContentString = strData.substr(pos + 4);
        return ParseHeader(strData.substr(0, pos));
    }
}

bool HttpResponseData::ParseHeader(std::string const& header)
{
    m_HttpCode = 0;
    m_HeaderMap.clear();

    // 处理http头数据
    std::vector<std::string> vLines = MJ::SplitByString(header, "\r\n");

    if (vLines.size() <= 0)
    {
        MJ_LOG_ERROR(0, "%s:%d parse error: %s", __FUNCTION__, __LINE__, _S(header));
        return false;
    }

    // 处理第一行
    std::vector<std::string> line0 = MJ::SplitByString(vLines.at(0), " ");

    if (line0.size() < 2)
    {
        MJ_LOG_ERROR(0, "%s:%d parse code error %s", __FUNCTION__, __LINE__, _S(header));
        return false;
    }

    m_HttpVersion = line0.at(0);
    m_HttpCode = std::strtol(line0.at(1).c_str(), nullptr, 10);
    m_HttpCodeName.clear();
    for (int i = 2; i < line0.size(); ++i)
    {
        m_HttpCodeName += (m_HttpCodeName.size() ? " " : "") + line0.at(i);
    }

    m_HeaderMap.clear();
    //解析后几个header
    for (unsigned int i = 1; i < vLines.size(); i++)
    {
        auto strs = MJ::SplitByString(vLines.at(i), ":");
        if (strs.size() >= 2)
        {
            m_HeaderMap.insert(std::make_pair(strs.at(0), strs.at(1)));
        }
    }
    return true;
}

bool HttpUrlUtil::Parse(std::string const& url)
{
    m_Url = url;

    //根据配置解析出对应的http访问信息
    std::string ipSign = "://";
    std::string portSign = ":";
    std::string pathSign = "/";
    auto ipStart = m_Url.find(ipSign);
    if (ipStart == m_Url.npos)
    {
        return false;
    }
    ipStart += ipSign.size();

    auto ipEnd = m_Url.find(portSign, ipStart);
    if (ipEnd != m_Url.npos)
    {
        m_IP = m_Url.substr(ipStart, ipEnd - ipStart);

        auto portStart = ipEnd + portSign.size();
        auto portEnd = m_Url.find(pathSign, portStart);
        if (portEnd == m_Url.npos)
        {
            std::string strport = m_Url.substr(portStart, portEnd - portStart);
            m_Port = strtol(strport.c_str(), nullptr, 10);
            m_Path = "";
        }
        else
        {
            std::string strport = m_Url.substr(portStart, portEnd - portStart);
            m_Port = strtol(strport.c_str(), nullptr, 10);
            m_Path = m_Url.substr(portEnd);
        }
        return true;
    }
    //默认端口
    ipEnd = m_Url.find(pathSign, ipStart);
    if (ipEnd != m_Url.npos)
    {
        m_IP = m_Url.substr(ipStart, ipEnd - ipStart);
        m_Port = 80;
        m_Path = m_Url.substr(ipEnd);
        return true;
    }
    m_IP = m_Url.substr(ipStart);
    m_Port = 80;
    m_Path = "";
    return true;
}
