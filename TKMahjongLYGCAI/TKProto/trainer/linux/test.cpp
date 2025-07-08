// TKLongFengCardAI.cpp : 定义应用程序的入口点。
//
#pragma once

#include <iostream>
#include "TKTrainer.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "json/json.h"

std::string GetAiResp(const std::string& request)
{
    const std::string host = "10.30.119.29";
    int port = 80;

    int sockfd;
    struct sockaddr_in address;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(host.c_str());  //直接填写地址;

    if (-1 == connect(sockfd, (sockaddr*)&address, sizeof(address)))
    {
        std::cout << host << ":" << port << " connection failed!" << std::endl;
        return "";
    }

    send(sockfd, request.c_str(), request.size(), 0);
    char buf[1024 * 2] = {0};  //不能设置过大，否则会栈溢出，如果必须设置较大，用堆new；
    int offset = 0;
    int rc;

    while (rc = recv(sockfd, buf + offset, 1024, 0))
    {
        offset += rc;
    }

    buf[offset] = '\0';  //最后结束符;
    close(sockfd);       //断开;
    return std::string(buf);
}

int main()
{
    InitService();
    StartGame("{\"property\":\"\",\"playercount\":4,\"chip\":1000,\"scorebase\":1}");
    Json::Value root;
    Json::Reader reader;
    while (true)
    {
        std::string action = GetAction();
        std::cout << action << std::endl;
        if (!reader.parse(action, root))
        {
            break;
        }
        auto act = root["action"].asString();
        if (act.empty())
        {
            break;
        }
        std::string res = GetAiResp(act);  //请求AI
        std::cout << res << std::endl;
        DoAction(res.c_str());
    }
    return 0;
}
