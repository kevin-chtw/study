#pragma once
#include "TKProtocol.h"

#define TKID_MSG_AI_WEB_SERVER 100
typedef struct tagTKMSGAIWEBSERVER
{
    TKHEADER header;
    DWORD dwUserID;
    time_t timeStamp;       // 请求发起的时间戳，直接返回请求数据
    int minThinkTime;       // 最小思考时间，直接返回请求数据
    int msgType;            // 请求类型，直接返回请求数据
    WORD offset;            // 数据偏移量
    WORD dataLength;        // 数据长度
    char szServerName[32];  // 请求的服务名字 ，直接返回请求数据
    char szServerIP[64];
    int port;
    // 后接http请求或响应数据
} TKMSGAIWEBSERVER, *PTKMSGAIWEBSERVER;
