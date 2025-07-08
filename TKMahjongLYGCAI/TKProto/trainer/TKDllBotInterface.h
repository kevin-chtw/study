#pragma once
#include "TKProtocol.h"

#define TKID_MSG_AI_WEB_SERVER 100
typedef struct tagTKMSGAIWEBSERVER
{
    TKHEADER header;
    DWORD dwUserID;
    time_t timeStamp;       // �������ʱ�����ֱ�ӷ�����������
    int minThinkTime;       // ��С˼��ʱ�䣬ֱ�ӷ�����������
    int msgType;            // �������ͣ�ֱ�ӷ�����������
    WORD offset;            // ����ƫ����
    WORD dataLength;        // ���ݳ���
    char szServerName[32];  // ����ķ������� ��ֱ�ӷ�����������
    char szServerIP[64];
    int port;
    // ���http�������Ӧ����
} TKMSGAIWEBSERVER, *PTKMSGAIWEBSERVER;
