#pragma once
#include "TKProtocol.h"

/************************************************************************/
/* Game服务发送的通用Http请求，需要标记matchID，StageID，RoundID和UserID 
*/
/************************************************************************/
#define TKID_GS2BROKER_HTTPMSG (TK_MSG_HTTPBROKER + 2)
struct SUFFDATA
{
    WORD wOffSet;
    WORD wDataLen;
};

#define TK_BROKER_HTTP_METHOD_GET (0)
#define TK_BROKER_HTTP_METHOD_POST (1)

/************************************************************************/
/* 
未使用的SUFFDATA类型字段请初始化为0
sufReturnData 为回传数据，请求时发送的数据会不做任何修改的返回，用于发送端异步发送时数据保持
当wMethod为0，即Get请求时，只需要传输完整的URL，
	sufURI、sufHeader、sufMsgBody等字段初始化为0
当wMethod为1，即Post请求时，sufURL处填写完整的url，包括域名(端口)及URI
	在sufMsgBody附上消息体
	如果消息中有自定义的Http Header，将数据赋值到sufHeader
*/
/************************************************************************/
typedef struct tagTKHTTPMATCHMSG
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wGameID;
    WORD wMethod;     // 0 Get, 1 Post
    int nSenderType;  // 1-Game, 2-Player，3-DllServer
    DWORD dwReserve1;
    DWORD dwReserve2;
    SUFFDATA sufURL;         // 完整请求地址
    SUFFDATA sufReturnData;  // 回传数据
    SUFFDATA sufHeader;      // 需要添加的http header字段
    SUFFDATA sufMsgBody;     // Post请求需要的数据
} TKHTTPMATCHMSGREQ, *PTKHTTPMATCHMSGREQ;

typedef struct tagTKHTTPMATCHMSGACK
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wGameID;
    DWORD dwUserID;
    int nSenderType;  //1-Game, 2-Player，3-DllServer
    DWORD dwReserve1;
    DWORD dwReserve2;
    SUFFDATA sufReturnData;  // 回传数据
    SUFFDATA sufHeader;      // 需要添加的http header字段
    SUFFDATA sufMsgBody;     // Post请求需要的数据
} TKHTTPMATCHMSGACK, *PTKHTTPMATCHMSGACK;
