#pragma once
#include "TKProtocol.h"

/************************************************************************/
/* Game�����͵�ͨ��Http������Ҫ���matchID��StageID��RoundID��UserID 
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
δʹ�õ�SUFFDATA�����ֶ����ʼ��Ϊ0
sufReturnData Ϊ�ش����ݣ�����ʱ���͵����ݻ᲻���κ��޸ĵķ��أ����ڷ��Ͷ��첽����ʱ���ݱ���
��wMethodΪ0����Get����ʱ��ֻ��Ҫ����������URL��
	sufURI��sufHeader��sufMsgBody���ֶγ�ʼ��Ϊ0
��wMethodΪ1����Post����ʱ��sufURL����д������url����������(�˿�)��URI
	��sufMsgBody������Ϣ��
	�����Ϣ�����Զ����Http Header�������ݸ�ֵ��sufHeader
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
    int nSenderType;  // 1-Game, 2-Player��3-DllServer
    DWORD dwReserve1;
    DWORD dwReserve2;
    SUFFDATA sufURL;         // ���������ַ
    SUFFDATA sufReturnData;  // �ش�����
    SUFFDATA sufHeader;      // ��Ҫ��ӵ�http header�ֶ�
    SUFFDATA sufMsgBody;     // Post������Ҫ������
} TKHTTPMATCHMSGREQ, *PTKHTTPMATCHMSGREQ;

typedef struct tagTKHTTPMATCHMSGACK
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wGameID;
    DWORD dwUserID;
    int nSenderType;  //1-Game, 2-Player��3-DllServer
    DWORD dwReserve1;
    DWORD dwReserve2;
    SUFFDATA sufReturnData;  // �ش�����
    SUFFDATA sufHeader;      // ��Ҫ��ӵ�http header�ֶ�
    SUFFDATA sufMsgBody;     // Post������Ҫ������
} TKHTTPMATCHMSGACK, *PTKHTTPMATCHMSGACK;
