#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

const std::string HTTP_GET = "GET";
const std::string HTTP_POST = "POST";

class HttpRequestData
{
public:
    HttpRequestData(std::string sMethod, std::string sHost, std::string sPath);

public:
    void SetAccept(std::string strAccept);
    void SetContent(std::string strConType, std::string strContent);
    // 添加header信息
    void PushHeader(std::string sKey, std::string sValue);
    void SetClose(bool bClose);

    // 生成获取发送的完整数据
    std::string ToRequestData();

private:
    std::string m_Method;
    std::string m_ServerHost;
    std::string m_ServerPortString;
    std::string m_RequestPath;

    std::string m_ContentString;
    std::string m_ContentDataType;  // content类型
    std::string m_AcceptType;       // accept请求报头域，表示客户端接受哪些类型的信息

    bool m_bClose = false;  // 连接完成后是否断开

    std::map<std::string, std::string> m_HeaderMap;
};

class HttpResponseData
{
public:
    HttpResponseData() = default;

public:
    bool ParseData(std::string const& strData);
    bool ParseHeader(std::string const& header);

    int GetCode();
    std::string GetContent();
    std::string GetHeader(const std::string& key);

private:
    int m_HttpCode;
    std::string m_HttpCodeName;
    std::string m_HttpVersion;
    std::string m_ContentString;
    std::map<std::string, std::string> m_HeaderMap;
};

class HttpUrlUtil
{
public:
    bool Parse(std::string const& url);

    std::string m_Url;
    std::string m_IP;
    int m_Port{80};
    std::string m_Path;
};
