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
    // ���header��Ϣ
    void PushHeader(std::string sKey, std::string sValue);
    void SetClose(bool bClose);

    // ���ɻ�ȡ���͵���������
    std::string ToRequestData();

private:
    std::string m_Method;
    std::string m_ServerHost;
    std::string m_ServerPortString;
    std::string m_RequestPath;

    std::string m_ContentString;
    std::string m_ContentDataType;  // content����
    std::string m_AcceptType;       // accept����ͷ�򣬱�ʾ�ͻ��˽�����Щ���͵���Ϣ

    bool m_bClose = false;  // ������ɺ��Ƿ�Ͽ�

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
