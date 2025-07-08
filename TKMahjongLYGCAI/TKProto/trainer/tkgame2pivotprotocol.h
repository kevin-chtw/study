#pragma once

#include "TKProtocol.h"

//////////////////////////////////////////////////////////////////////////////////////////�ṹ��
//�¼���������������
typedef struct tagTKOriEventPlaceProperty
{
    DWORD dwPlatform;  // ƽ̨��PC��Flash��Mobile�� ����μ� eDicOEPPlatformType
    DWORD dwGID;       // ��ϷID
    DWORD dwMPType;    // ��Ʒ���ͣ����졢�������� ����μ� eDicOEPMPType
    DWORD dwMPID;      // ��ƷID
    DWORD dwStage;     // �׶�
} TKORIEVENTPLACEPROPERTY, *PTKORIEVENTPLACEPROPERTY;

//�¼���������������
typedef struct tagTKOriEventPersonProperty
{
    DWORD dwPP1;  // CMSTID
    DWORD dwPP2;
    DWORD dwPP3;
    DWORD dwPP4;
    DWORD dwPP5;
} TKORIEVENTPERSONPROPERTY, *PTKORIEVENTPERSONPROPERTY;

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ĳһ��Grow��Ϣ
//   ˵    ���� 1��ComitService->GrowService
//              2��Data 2.0 ����
//--------------------------------------------------------------------------------------------------------
#define TKID_COMITS2MATCHS_GETUSERSINGLEGROW (TKID_MATCHS2COMITSBASE + 0x011)
// �ṹ��
typedef struct tagTkMatchSTranGrow
{
    DWORD dwGrowID;  // ����ID
    DWORD dwValue;   // ����ֵ

} TKMATCHSTRANGROW, *PTKMATCHSTRANGROW;

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ĳһ��Grow��Ϣ 64λ
//--------------------------------------------------------------------------------------------------------
#define TKID_COMITS2MATCHS_GETUSERSINGLEGROW_64 (TKID_MATCHS2COMITSBASE + 0x04F)
// �ṹ��
typedef struct tagTkMatchSTranGrow64
{
    DWORD dwGrowID;   // ����ID
    __int64 llValue;  //64λ����ֵ

} TKMATCHSTRANGROW64, *PTKMATCHSTRANGROW64;

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ��ȡһ��USR����
//   ˵    ���� 1��MS -> DS
//--------------------------------------------------------------------------------------------------------
#define TKID_MATCHS2COMITS_GETUSERINFO (TKID_MATCHS2COMITSBASE + 0x046)

typedef struct tagTKCMTS2MatchSUserData
{
    DWORD dwExpireTime;
    WORD wType;
    WORD wLen;
#pragma warning(disable : 4200)
    char val[0];
#pragma warning(default : 4200)
} TK_CMTS2MATCHS_USERDATA, *PTK_CMTS2MATCHS_USERDATA;

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ӱ�������۷�[�۳��û��������](ϵͳ����)
//   ˵    ���� 1��GS -> PIVOT
//					2��GS�ڷ���Ϣʱ���ס����˿۳����������ڷ��سɹ���������Ӧ���ĳ���
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GAIN_CHIP (TK_MSG_GAME_PIVOT + 0x01)
//����
struct msg_gs2pivot_req_gain_chip : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // ������ƷID
    DWORD dwMatchID;

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11
    DWORD
    dwCMTSID;  // ˵    ������ί�᷽��ID�����ϰ����˰佱���۷ѣ��ضҡ��������������ġ���̬���ࡱ������̬�����������������ԡ�
    // Ӧ�þ�����ƻ���ֻ��û��佱��ָ��ƻ���û������̬���࣬���˷����û��ĸ������ԡ�

    DWORD dwUserID;
    DWORD dwGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//������
#define TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN 1
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN  ��ʾδ֪ԭ��ʧ��
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_SRCLACK  ��ʾ��Ҳ���
//header.dwParam = TK_DSGAINMATCHLIFEVALUEBONUS_SRCLACK ��ʾ��ȯ����
//header.dwParam = TK_DSGAINMATCHLIFEVALUECERT_SRCLACK  ��ʾ����ȯ(��Ʊ)����
struct msg_pivot2gs_ack_gain_chip : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGold;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ٱ��������˷�[�����û��������](ϵͳ֧��)
//   ˵    ���� 1�� PIVOT -> GS
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_EXPEND_CHIP (TK_MSG_GAME_PIVOT + 0x02)
//����
struct msg_gs2pivot_req_expend_chip : TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // ������ƷID
    DWORD dwMatchID;

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    DWORD
    dwCMTSID;  // ˵    ������ί�᷽��ID�����ϰ����˰佱���۷ѣ��ضҡ��������������ġ���̬���ࡱ������̬�����������������ԡ�
    // Ӧ�þ�����ƻ���ֻ��û��佱��ָ��ƻ���û������̬���࣬���˷����û��ĸ������ԡ�

    DWORD dwUserID;
    DWORD dwGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;

    //
    int nDomain;  // add @ 2022/8/22
};

//������
#define TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN 1
//header.dwParam = TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN  ��ʾδ֪ԭ��ʧ��
struct msg_pivot2gs_ack_expend_chip : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGold;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� �����޸�ר������-- ��:"ר������"
//   ˵    ���� 1��GS -> PIVOT
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_DEAL_GROW (TK_MSG_GAME_PIVOT + 0x03)
#define TK_DSMATCHDEALRESULTGROW_UNKNOWN 1  // δ֪ԭ��ʧ��
// ����
struct msg_gs2pivot_req_deal_grow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;  // ������ƷID

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11
    DWORD dwReserve1;  // �����ֶ�1
    DWORD dwReserve2;  // �����ֶ�2
    DWORD dwReserve3;  // �����ֶ�3
    DWORD dwPort;      // ����˿�

    __time32_t tMatchBegin;  // ������ʼʱ��
    DWORD dwTourneyID;       // ����ID
    DWORD dwMatchID;         // ����ID
    WORD wGrowDomainID;      // ������ID
    DWORD dwRank;            // ��������

    DWORD dwGrowID;  // ����ID
    int nValue;      // ���ֵ��޸�ֵ����ֵ������

    char szNote[TK_BUFLEN_NOTE];  // ˵��������Ϊ��
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//������
struct msg_pivot2gs_ack_deal_grow : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGrowID;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ��ȡĳһ���������ȫ��Grow��Ϣ
//   ˵    ���� 1��ComitService->GrowService
//              2��Comit 2.0 ����
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_GROW (TK_MSG_GAME_PIVOT + 0x04)
// ����
struct msg_gs2pivot_req_get_grow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwDomainID;  // ������ID(������� TKGrowUniversalDicSrv.h �ļ�)

    DWORD dwMPID;  // ������ƷID
    DWORD dwMatchID;

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
// ������
#define TKID_GS2PIVOT_GET_GROW_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_get_grow : public TKHEADER
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwDomainID;  // ������ID
    DWORD dwMatchID;

    TKSUFFIXIDX stSuffixIdx;  // Grow�б�����ƫ����
    DWORD dwGrowCount;        // Grow�б����б������
    // ����������׺���ݣ�
    //  dwGrowCount�� TKMATCHSTRANGROW �ṹ
};

//------------------------------------------------------------------------------------------------------
//   ��    �ܣ� �������ҽ���(ϵͳ֧��)
//   ˵    ���� 1��GS -> PIVOT
//------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_AWARD_MONEY (TK_MSG_GAME_PIVOT + 0x05)
//����
struct msg_gs2pivot_req_award_money : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMatchID;
    DWORD dwMPID;      // ������ƷID
    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    DWORD
    dwCMTSID;  // ˵    ������ί�᷽��ID�����ϰ����˰佱���۷ѣ��ضҡ��������������ġ���̬���ࡱ������̬�����������������ԡ�
    // Ӧ�þ�����ƻ���ֻ��û��佱��ָ��ƻ���û������̬���࣬���˷����û��ĸ������ԡ�

    DWORD
    dwSrcTypeID;  // ���影����𣺾���μ� TKComitUniversalDicSrv.h��eDicComitGatherMetadataSrcTypeDef��TKCOMMSRV_ENUM_DATA_COMMONMONEY_��ͷ����
    DWORD dwGold;

    DWORD dwTourneyID;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;

    DWORD dwCoin;   //Ԫ����Coin��
                    //     DWORD dwGold;   //��ң�Gold��
    DWORD dwBonus;  //��ȯ��Bonus��
    DWORD dwCert;   //�������֣�Cert��

    int nDomain;
};
//������
#define TKID_GS2PIVOT_AWARD_MONEY_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_award_money : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGold;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ������Ʒ������ϵͳ֧����
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_AWARD_WARE (TK_MSG_GAME_PIVOT + 0x06)
//����
struct msg_gs2pivot_req_award_ware : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMatchID;
    DWORD dwMPID;      // ������ƷID
    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    DWORD
    dwSrcTypeID;  // ���影����𣺾���μ� TKComitUniversalDicSrv.h��eDicComitGatherMetadataSrcTypeDef��TKCOMMSRV_ENUM_DATA_COMMONWARE_��ͷ����
    DWORD dwWareTypeID;
    DWORD dwCount;

    DWORD dwTourneyID;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
    int nDomain;
};
//������
#define TKID_GS2PIVOT_AWARD_WARE_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_award_ware : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwWareTypeID;
    DWORD dwCount;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� �������ֽ�����ϵͳ֧����
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_AWARD_GROW (TK_MSG_GAME_PIVOT + 0x07)
//����
struct msg_gs2pivot_req_award_grow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;      // ������ƷID
    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    __time32_t tMatchBegin;  // ������ʼʱ��
    DWORD dwTourneyID;       // ����ID
    DWORD dwMatchID;         // ����ID
    WORD wGrowDomainID;      // ������ID
    DWORD dwRank;            // ��������

    DWORD
    dwSrcTypeID;  // ���影����𣺾���μ� TKComitUniversalDicSrv.h��eDicComitGatherMetadataSrcTypeDef��TKCOMMSRV_ENUM_DATA_COMMONGROW_��ͷ����
    DWORD dwGrowID;  // ����ID
    int nValue;      // ���ֵ��޸�ֵ����ֵ������
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
    int nDomain;
};
//������
#define TKID_GS2PIVOT_AWARD_GROW_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_award_grow : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGrowID;  // ����ID
    int nValue;      // ���ֵ��޸�ֵ����ֵ������
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���������ڵ������룩�۳���Ʒ��ϵͳ���룩
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_USE_WARE (TK_MSG_GAME_PIVOT + 0x08)
#define TK_CSCATCHFISHGAINSHELLFROMWARE_SRCLACK (10)  // ��Ʒ����
#define TK_CSCATCHFISHGAINSHELLFROMWARE_UNKNOWN (1)   // δ֪ԭ��
//����
struct msg_gs2pivot_req_use_ware : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMatchID;
    DWORD dwMPID;  // ������ƷID

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    DWORD dwUserID;
    DWORD dwWareTypeID;
    DWORD dwCount;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//������
//header.dwParam = TK_CSCATCHFISHGAINSHELLFROMWARE_SRCLACK  ��ʾ��Ʒ����
struct msg_pivot2gs_ack_use_ware : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwWareTypeID;
    DWORD dwCount;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ĳһ��Grow��Ϣ
//   ˵    ���� 1��GS -> PIVOT
//   ����ʱ�䣺 2016-10-27
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_SINGLEGROW (TK_MSG_GAME_PIVOT + 0x09)
//����
struct msg_gs2pivot_req_get_singleGrow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    TKHEADER header;
    DWORD dwUserID;
    DWORD dwGrowID;

    DWORD dwMPID;  // ������ƷID

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // ����˿�
};
// ������
#define TKID_GS2PIVOT_GET_SINGLEGROW_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_get_singleGrow : public TKHEADER
{
    DWORD dwReserve1;  //Ԥ���ֶ�
    DWORD dwReserve2;
    WORD wReserve3;
    WORD wReserve4;
    DWORD dwReserve5;
    DWORD dwReserve6;
    DWORD dwReserve7;

    DWORD dwUserID;
    TKSUFFIXIDX stSuffixIdx;
};
// ����������׺���ݣ�
//  һ�� TKMATCHSTRANGROW �ṹ

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� �޸ľ���ֵ
//   ˵    ���� 1��GS -> PIVOT
//   ����ʱ�䣺 2017-02-17
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_DEAL_RESULTSCORE (TK_MSG_GAME_PIVOT + 0x0A)
#define TK_DSMATCHDEALRESULTSCORE_UNKNOWN 1  // δ֪ԭ��ʧ��
//����
struct msg_gs2pivot_req_deal_resultScore : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;

    DWORD dwMPID;      // ������ƷID
    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // ����˿�

    __time32_t tMatchBegin;  // ������ʼʱ��
    DWORD dwTourneyID;       // ����ID
    DWORD dwMatchID;         // ����ID

    WORD wGrowDomainID;  // ������ID

    DWORD dwRank;  // ��������

    int nScore;  // ����ֵ����ֵ�ǲ���ֵ

    int nReserve1;  // ����1
    int nReserve2;  // ����2
    int nReserve3;  // ����3
    int nReserve4;  // ����4

    int nMasterScore;  // ��ʦ�֣���ֵ�ǲ���ֵ
    int nMatchCount;   // ��Ϸ��������ֵ�ǲ���ֵ(��λ����)
    int nMatchTime;    // ��Ϸʱ�䣬��ֵ�ǲ���ֵ(��λ����)

    char szNote[TK_BUFLEN_NOTE];  // ˵��������Ϊ��
};
//����
struct msg_pivot2gs_ack_deal_resultScore : public TKHEADER
{  // TKHEADER dwParam ��ʾ���鲻��ԭ�򣬲μ���eDicCommSrvScoreSRCLack
    DWORD dwUserID;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ԭ���¼���Ϣ
//   ˵    ���� 1��GS -> PIVOT
//              2����Ϣ�����¼���ί��
//   ����ʱ�䣺 2017-11-09
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_ORIGINALEVENT (TK_MSG_GAME_PIVOT + 0x0B)
struct msg_gs2pivot_req_originalevent : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;      // �û�ID(�¼���ί���ΪdwPID)
    DWORD dwGameID;      // ��ϷID
    DWORD dwOEID;        // ԭ���¼�ID
    __time32_t tOEvent;  // ԭ���¼�ʱ��
    DWORD dwOEValue;     // ԭ���¼�ֵ

    DWORD dwTourneyID;   // ����ID
    DWORD dwMatchID;     // ����ID
    WORD wGrowDomainID;  // ������ID
    DWORD dwMsPort;

    char szTourneySimpleName[TK_BUFLEN_TOURNEYSIMPLENAME];  // Tourney���
    char szNote[TK_BUFLEN_MATCHAWARDNOTE];  // ע�ͣ���ע���� szTourneySimpleName ����ʹ�ã����Ϊ�տͻ��˽�����ʾ
    // �����Ϊ����ͻ��� ����ʾ szTourneySimpleName ����ʾ szNote����������

    TKORIEVENTPLACEPROPERTY dsOEPlacePro;    // ���������� dwMPIDΪ��ƷID(������Ҫ)
    TKORIEVENTPERSONPROPERTY dsOEPersonPro;  // ����������

    TKSUFFIXIDX stSuffix;  // ��׺��������
};
//��� DSORIEVENTDATA + DSORIEVENTDATA + ... �ṹ���ݣ��������һЩ��ʶ���ݣ����ݸ���Ϊ��stSuffix.dwSuffixSize/sizeof(DSORIEVENTDATA)

//����
//������(dwParam)
#define TKDEF_ORIGINALEVENT_UNKNOWN 1                // δ֪ԭ��
#define TKDEF_ORIGINALEVENT_DEPLOY_ERROR 100         // ��������
#define TKDEF_ORIGINALEVENT_GETUSERINFO_FAIL 101     // ��ȡ�û���Ϣʧ��
#define TKDEF_ORIGINALEVENT_AWARD_FAIL 102           // ��������ʧ��
#define TKDEF_ORIGINALEVENT_NO_AVAILABLE_SCHEME 103  // û�п��õĽ�������
struct msg_pivot2gs_ack_originalevent : public TKHEADER
{
    DWORD dwUserID;   //�û�ID
    DWORD dwOEID;     //ԭ���¼�ID
    DWORD dwOEValue;  //ԭ���¼�ֵ
    DWORD dwMPID;     //��Ʒid
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ��ȡһ��USR����
//   ˵    ���� 1��GS -> PIVOT
//   ����ʱ�䣺 2017-11-23
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GETUSERINFO (TK_MSG_GAME_PIVOT + 0x0C)
struct msg_gs2pivot_req_getuserinfo : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwGameID;
    DWORD dwUserID;
    DWORD dwOriID;
};
//������
#define TKID_GS2PIVOT_GETUSERINFO_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_getuserinfo : public TKHEADER
{
    DWORD dwUserID;
    DWORD dwOriID;
    TK_CMTS2MATCHS_USERDATA data;
};
//������׺TK_CMTS2MATCHS_USERDATA�ṹ����wlen���ȵ�����

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ����ҵ��[�۳��û��������](ϵͳ����)
//   ˵    ���� 1��GS -> PIVOT
//					2��GS�ڷ���Ϣʱ���ס����˿۳����������ڷ��سɹ���������Ӧ���ĳ���
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_PAYMENT_BUSINESS (TK_MSG_GAME_PIVOT + 0x0D)
//����
struct msg_gs2pivot_req_payment_business : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;      // ������ƷID
    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨����
    DWORD dwMatchID;
    DWORD dwTourneyID;
    __time32_t tMatchBegin;  // ������ʼʱ��
    DWORD dwRank;            // ��������

    WORD wDomain;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwPort;  // ����˿�

    DWORD dwPaymentBusinessTypeID;  // ҵ�����Ͷ��� �μ�: eDicPaymentBusinessTypeDef
    DWORD dwPaymentBusinessSubID;   // ҵ��������ID��������š�0������Ĭ��Ϊ1����1��ʼ��š�
    char szNote
        [TK_BUFLEN_NOTE];  // �˵�ע�ͣ����Ϊ�գ���Ĭ�����ҵ���������ơ�Money��ķ��ó��⣬���Ϲ���ֻ�и���Ͷһ���
    DWORD dwLanguageID;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;

    DWORD
    dwCMTSID;  // ˵    ������ί�᷽��ID�����ϰ����˰佱���۷ѣ��ضҡ��������������ġ���̬���ࡱ������̬�����������������ԡ�
    // Ӧ�þ�����ƻ���ֻ��û��佱��ָ��ƻ���û������̬���࣬���˷����û��ĸ������ԡ�
    DWORD dwMoneyAccType;

    // �۷Ѳ��֣�֧�ֶ�����úϲ�
    DWORD dwPaymentCount;             // ��������
    TKSUFFIXIDX dsPaymentInfoSuffix;  // dwPaymentCount��TK_PAYMENTINFO
};
//������
#define TK_DSGAINMATCHLIFEVALUEGOLD_PAYMENT_BUSINESS_UNKNOWN 1
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_PAYMENT_BUSINESS_UNKNOWN  ��ʾδ֪ԭ��ʧ��
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_PAYMENT_BUSINESS_SRCLACK  ��ʾ��Ҳ���
//header.dwParam = TK_DSGAINMATCHLIFEVALUEBONUS_PAYMENT_BUSINESS_SRCLACK ��ʾ��ȯ����
//header.dwParam = TK_DSGAINMATCHLIFEVALUECERT_PAYMENT_BUSINESS_SRCLACK  ��ʾ����ȯ(��Ʊ)����
struct msg_pivot2gs_ack_payment_business : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwPaymentBusinessTypeID;
    DWORD dwPaymentBusinessSubID;
};

//-----------------------------------------------------------------------------------------------------------------
//   ��    �ܣ� �����ǩ����
//   ˵    ���� 1��GS -> PIVOT
//   ����ʱ�䣺 2019-2-19
//-----------------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GETTAGDATA (TK_MSG_GAME_PIVOT + 0x0E)
struct msg_gs2pivot_req_gettagdata : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;     // ���󷽵�UserID
    DWORD dwTourneyID;  // ����ID
    DWORD dwMatchID;    // ����ID
    DWORD dwTagID;      // ��ǩID
    DWORD dwMPID;       // ��ƷID
    DWORD dwGameID;     // ��ϷID
};

// Ack
#define TKID_GS2PIVOT_GETTAGDATA_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_gettagdata : public TKHEADER
{
    DWORD dwUserID;     // ���󷽵�UserID
    DWORD dwTourneyID;  // ����ID
    DWORD dwMatchID;    // ����ID
    DWORD dwTagID;      // ��ǩID
    DWORD dwMPID;       // ��ƷID
    ULONG64 llValue;    // ��ǩֵ
};

//-----------------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ĳһ��Grow��Ϣ 64λ
//   ˵    ���� 1��GS -> PIVOT
//   ����ʱ�䣺 2019-3-17
//-----------------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_SINGLEGROW_64 (TK_MSG_GAME_PIVOT + 0x0F)
struct msg_gs2pivot_req_get_singlegrow_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;
    DWORD dwGrowID;

    DWORD dwMPID;      // ������ƷID
    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11
    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // ����˿�

    DWORD dwReserve1;
    DWORD dwReserve2;
};
//������
struct msg_pivot2gs_ack_get_singleGrow_64 : public TKHEADER
{
    DWORD dwReserve1;  //Ԥ���ֶ�
    DWORD dwReserve2;
    WORD wReserve3;
    WORD wReserve4;
    DWORD dwReserve5;
    DWORD dwReserve6;
    DWORD dwReserve7;

    DWORD dwUserID;
    TKSUFFIXIDX stSuffixIdx;
};
// ����������׺���ݣ�
//  һ�� TKMATCHSTRANGROW64 �ṹ

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� �����޸�ר������-- ��:"ר������" 64λ
//   ˵    ���� 1��GS -> PIVOT
//   ����ʱ�䣺 2019-3-17
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_DEAL_GROW_64 (TK_MSG_GAME_PIVOT + 0x10)

struct msg_gs2pivot_req_deal_grow_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;

    DWORD dwMPID;  // ������ƷID

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // ����˿�

    __time32_t tMatchBegin;  // ������ʼʱ��
    DWORD dwTourneyID;       // ����ID
    DWORD dwMatchID;         // ����ID
    DWORD dwGrowDomainID;    // ������ID
    DWORD dwRank;            // ��������

    DWORD dwGrowID;   // ����ID
    __int64 llValue;  // ���ֵ��޸�ֵ����ֵ��������Ŀǰֻ֧��32λ��Χ

    char szNote[TK_BUFLEN_NOTE];  // ˵��������Ϊ��

    DWORD dwLanguageID;

    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
};
//����
struct msg_pivot2gs_ack_deal_grow_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGrowID;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ����û�Score��Ϣ
//   ˵    ���� 1��MS -> DS
//              2��Data 1.0 ����
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GETUSERSCORE (TK_MSG_GAME_PIVOT + 0x11)
#define TKID_GS2PIVOT_GETUSERSCORE_UNKNOWN (1)   // δ֪ԭ��ʧ��
#define TKID_GS2PIVOT_GETUSERSCORE_NOEXIST (10)  // �û�û�и���Ϸ����
//����
struct msg_gs2pivot_req_getuserscore : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwMPID;  // ������ƷID

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // ����˿�
};

//������
struct msg_pivot2gs_ack_getuserscore : public TKHEADER
{
    DWORD dwUserID;
    DWORD dwScore;
    DWORD dwWeekScore;
    DWORD dwMonthScore;
    DWORD dwSeasonScore;
    DWORD dwYearScore;
    DWORD dwMasterScore;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ��ѯ�û���ĳ����������óɼ�
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GETMARKERINFO (TK_MSG_GAME_PIVOT + 0x12)
struct msg_gs2pivot_req_getmarkerinfo : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;
    DWORD dwProductId;  // ������ƷID
    DWORD dwGameID;     // ��ϷID
};

//������
#define TKID_GS2PIVOT_GETMARKERINFO_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_getmarkerinfo : public TKHEADER
{
    DWORD dwUserID;       // �û�Id
    DWORD dwProductId;    // �����Ĳ�ƷID
    DWORD dwRank;         // �������
    DWORD dwQuantity;     // ������������
    DWORD dwMarkerStamp;  // ������ʼʱ�䣬ʱ���

    //// ������δ������ھ�������������δ������Ӽ�
    // if (1 == dwRank) {
    //    �ھ�����;
    // } else {
    //    ������δ���;
    // }
    DWORD dwBestRankCount;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ӱ�������۷�[�۳��û��������](ϵͳ����)
//   ˵    ���� 1��GS -> PIVOT
//					2��GS�ڷ���Ϣʱ���ס����˿۳����������ڷ��سɹ���������Ӧ���ĳ���
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GAIN_CHIP_64 (TK_MSG_GAME_PIVOT + 0x13)
//����
struct msg_gs2pivot_req_gain_chip_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // ������ƷID
    DWORD dwMatchID;

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11
    DWORD
    dwCMTSID;  // ˵    ������ί�᷽��ID�����ϰ����˰佱���۷ѣ��ضҡ��������������ġ���̬���ࡱ������̬�����������������ԡ�
    // Ӧ�þ�����ƻ���ֻ��û��佱��ָ��ƻ���û������̬���࣬���˷����û��ĸ������ԡ�

    DWORD dwUserID;
    ULONGLONG llGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//������
//#define TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN			1
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN  ��ʾδ֪ԭ��ʧ��
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_SRCLACK  ��ʾ��Ҳ���
//header.dwParam = TK_DSGAINMATCHLIFEVALUEBONUS_SRCLACK ��ʾ��ȯ����
//header.dwParam = TK_DSGAINMATCHLIFEVALUECERT_SRCLACK  ��ʾ����ȯ(��Ʊ)����
struct msg_pivot2gs_ack_gain_chip_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    ULONGLONG llGold;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ٱ��������˷�[�����û��������](ϵͳ֧��)
//   ˵    ���� 1�� PIVOT -> GS
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_EXPEND_CHIP_64 (TK_MSG_GAME_PIVOT + 0x14)
//����
struct msg_gs2pivot_req_expend_chip_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // ������ƷID
    DWORD dwMatchID;

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    DWORD
    dwCMTSID;  // ˵    ������ί�᷽��ID�����ϰ����˰佱���۷ѣ��ضҡ��������������ġ���̬���ࡱ������̬�����������������ԡ�
    // Ӧ�þ�����ƻ���ֻ��û��佱��ָ��ƻ���û������̬���࣬���˷����û��ĸ������ԡ�

    DWORD dwUserID;
    ULONGLONG llGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//������
//#define TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN		1
//header.dwParam = TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN  ��ʾδ֪ԭ��ʧ��
struct msg_pivot2gs_ack_expend_chip_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    ULONGLONG llGold;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ��ѯ�û���64λMoney��Ϣ
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_MONEY_64 (TK_MSG_GAME_PIVOT + 0x15)
// ����
struct msg_gs2pivot_req_get_money_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMatchID;
    DWORD dwUserID;        // �û�ID����Э����ʹ��DWORD���ڳ������ݿ���ʹ��int
    DWORD dwMoneyAccType;  // Moneyϵͳ�ʺ�����
};
// ������
#define TKID_GS2PIVOT_GET_MONEY_64_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_get_money_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;

    ULONGLONG llCoin;   //Ԫ����Coin��
    ULONGLONG llGold;   //��ң�Gold��
    ULONGLONG llBonus;  //��ȯ��Bonus��
    ULONGLONG llCert;   //�������֣�Cert��
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ��õ���Ware��Ϣ
//   ˵    ���� 1��GS <-> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_GETSINGLEWARE (TK_MSG_GAME_PIVOT + 0x16)
//����
struct msg_gs2pivot_req_get_singleware : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwGameID;  // ��ϷID(0λ��Ĭ����ί��)
    DWORD dwWATID;   // Wareϵͳ�ʺ�����
    DWORD dwWID;     // ��ƷID
};

// ������
#define TKID_GS2PIVOT_GET_GETSINGLEWARE_UNKNOWN 1  // δ֪ԭ��ʧ��
// ������ɸ���Ʒ�б������ṹ��TKDATASTRANWAREBASEINFO����
struct msg_gs2pivot_ack_get_singleware : public TKHEADER
{
    DWORD dwUserID;

    DWORD dwWareListOffset;  // Ware�б�����ƫ�������������Ϣ������������Ʒ�б�
    DWORD dwItemCount;       // Ware�б����б������
    DWORD dwWareItemSize;    // Ware�б����С(��׺�������ÿһ���С)
};
// ������׺���ݣ�
// dwItemCount * TKMATCHS_TRANWAREBASEINFO

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ���������ҵ��ͨ�ýӿ�[64λ]
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_PAYMENT_BUSINESS_64 (TK_MSG_GAME_PIVOT + 0x17)

#define TK_PAYMENT_BUSINESS_MONEY_SRCLACK 100  // ���Ҳ��㣨���˽�ң���ȯ������ȯ��������⣩
#define TK_PAYMENT_BUSINESS_GOLD_SRCLACK 101   // ��Ҳ���
#define TK_PAYMENT_BUSINESS_BONUS_SRCLACK 102  // ��ȯ����
#define TK_PAYMENT_BUSINESS_CERT_SRCLACK 103   // ����ȯ����Ʊ������
#define TK_PAYMENT_BUSINESS_WARE_SRCLACK 104   // ��Ʒ����
#define TK_PAYMENT_BUSINESS_GROW_SRCLACK 105   // ���ֲ���
#define TK_PAYMENT_BUSINESS_SRCLACK 106        // Դ���Ҳ���
#define TK_PAYMENT_BUSINESS_DSTACCNOEXIST 107  // Ŀ���˻�������

#define TK_PAYMENT_BUSINESS_NOT_SUPPORT 200                        // ��֧�ִ���۷�ҵ��
#define TK_PAYMENT_BUSINESS_RESTRICT (TK_COMMSRV_USER_RESTICTING)  // �û������Ʋ���

//����
struct msg_gs2pivot_req_payment_business_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;      // ������ƷID
    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨����
    DWORD dwMatchID;
    DWORD dwTourneyID;
    __time32_t tMatchBegin;  // ������ʼʱ��
    DWORD dwRank;            // ��������

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwPort;  // ����˿�

    DWORD dwPaymentBusinessTypeID;  // ҵ�����Ͷ��� �μ�: eDicPaymentBusinessTypeDef
    DWORD dwPaymentBusinessSubID;   // ҵ��������ID��������š�0������Ĭ��Ϊ1����1��ʼ��š�
    char szNote
        [TK_BUFLEN_NOTE];  // �˵�ע�ͣ����Ϊ�գ���Ĭ�����ҵ���������ơ�Money��ķ��ó��⣬���Ϲ���ֻ�и���Ͷһ���
    DWORD dwLanguageID;
    DWORD dwReserve2;  // �ڳ�ID
    DWORD dwReserve3;  // �ڳ���ܽ��
    DWORD dwReserve4;  // �ڳ﷢�����û�ID

    // �۷Ѳ��֣�֧�ֶ�����úϲ�
    DWORD dwPaymentCount;             // ��������
    TKSUFFIXIDX dsPaymentInfoSuffix;  // dwPaymentCount��TK_PAYMENTINFO_64
};

// ������
struct msg_gs2pivot_ack_payment_business_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwPaymentBusinessTypeID;
    DWORD dwPaymentBusinessSubID;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ��������ҽ���64λ(ϵͳ֧��)
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_COMMONMONEYREWARD_64 (TK_MSG_GAME_PIVOT + 0x18)
//����
struct msg_gs2pivot_req_commonmoneyreward_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwMPID;        // ������ƷID
    DWORD dwGameID;      // ��ϷID
    DWORD dwPlatType;    // ƽ̨���� pangs added 2013-07-11
    DWORD dwTouerneyID;  // �����ֶ�1 ----����TouerneyID pangs added 2014-11-04
    DWORD dwMatchID;     // �����ֶ�2 ----����MatchID pangs added 2014-11-04

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;  //������ʽ��0--��ͨ��1--������� modify by hfc 2021-9-26

    DWORD dwPort;  // ����˿�

    DWORD dwMoneyAccType;

    DWORD
    dwSrcTypeID;  // ���影����𣺾���μ� TKComitUniversalDicSrv.h��eDicComitGatherMetadataSrcTypeDef��TKCOMMSRV_ENUM_DATA_COMMONMONEY_��ͷ����
    ULONGLONG llCoin;
    ULONGLONG llBonus;
    ULONGLONG llGold;
    ULONGLONG llCert;

    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwLanguageID;
    char szNote[TK_BUFLEN_NOTE];  // ��׼ע��
};

//������
struct msg_gs2pivot_ack_commonmoneyreward_64 : public TKHEADER
{
    DWORD dwUserID;
    ULONGLONG llCoin;
    ULONGLONG llBonus;
    ULONGLONG llGold;
    ULONGLONG llCert;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ��ȡĳһ���������ȫ��64λGrow��Ϣ
//   ˵    ���� 1��ComitService->GrowService
//              2��Comit 2.0 ����
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_DOMAINGROW_64 (TK_MSG_GAME_PIVOT + 0x19)
// ����
struct msg_gs2pivot_req_get_domaingrow_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwDomainID;  // ������ID(������� TKGrowUniversalDicSrv.h �ļ�)

    DWORD dwMPID;  // ������ƷID
    DWORD dwMatchID;

    DWORD dwGameID;    // ��ϷID
    DWORD dwPlatType;  // ƽ̨���� pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // ����˿�

    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
};
// ������
#define TKID_GS2PIVOT_GET_GROW_UNKNOWN 1  // δ֪ԭ��ʧ��
struct msg_pivot2gs_ack_get_domaingrow_64 : public TKHEADER
{
    DWORD dwUserID;
    DWORD dwDomainID;  // ������ID

    TKSUFFIXIDX stSuffixIdx;  // Grow�б�����ƫ����
    DWORD dwGrowCount;        // Grow�б����б������
    // ����������׺���ݣ�
    //  dwGrowCount�� TKMATCHSTRANGROW64 �ṹ
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ����������ϵͳ֧����[ͨ��64λ]
//   ˵    ���� 1��GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_REWARD_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1A)
//����
struct msg_gs2pivot_req_reward_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // ��Ʒ��Ϣ
    DWORD dwGameID;          // ��ϷID
    DWORD dwMPID;            // ������ƷID
    DWORD dwTourneyID;       // ����ID
    DWORD dwMatchID;         // ����ID
    __time32_t tMatchBegin;  // ������ʼʱ��

    // ������Ϣ
    DWORD dwPlatType;  // ƽ̨����
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat����ͳ�����ݷ���ʹ��
    DWORD dwLanguageID;

    //ҵ�����
    DWORD dwGrowDomainID;  // ������ID��ֻ�л������ͻ�ʹ�õ�
    DWORD dwRank;          // ��������
    DWORD
    dwSrcTypeID;  // ���影����𣺾���μ� TKComitUniversalDicSrv.h��eDicComitGatherMetadataSrcTypeDef��TKCOMMSRV_ENUM_DATA_COMMONGROW_��ͷ����
    DWORD dwDCID;        // ��������ID��1-Money��2-��Ʒ��3-����
    DWORD dwAccType;     // �˻����ͣ�һ��ֻ��Moneyʹ�ã�0-Ĭ�ϣ�3000-�齫�����˻�
    DWORD dwDTID;        // ����ID�����磺�������ͣ���Ӧ�ľ��ǻ���ID
    DWORD dwSignupMode;  // ������ʽ��0--��ͨ��1--�������
    LONGLONG llValue;    // �޸�ֵ����ֵ������

    //��չ����
    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD dwReserve5;

    //
    int nDomain;
};
//������
struct msg_pivot2gs_ack_reward_common_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;

    //
    DWORD dwDCID;
    DWORD dwAccType;
    DWORD dwDTID;
    LONGLONG llValue;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ӱ�������۷�[�۳��û��������](ϵͳ����)[ͨ��64λ]
//   ˵    ���� 1��GS -> PIVOT
//					2��GS�ڷ���Ϣʱ���ס����˿۳����������ڷ��سɹ���������Ӧ���ĳ���
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GAIN_CHIP_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1B)
//����
struct msg_gs2pivot_req_gain_chip_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // ��Ʒ��Ϣ
    DWORD dwGameID;  // ��ϷID
    DWORD dwMPID;    // ������ƷID

    // ������Ϣ
    DWORD dwPlatType;  // ƽ̨����
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat����ͳ�����ݷ���ʹ��
    DWORD dwLanguageID;

    // ҵ�����
    DWORD dwDCID;      // ��������ID��1-Money��2-��Ʒ��3-����
    DWORD dwAccType;   // �˻����ͣ�һ��ֻ��Moneyʹ�ã�0-Ĭ�ϣ�3000-�齫�����˻�
    DWORD dwDTID;      // ����ID�����磺�������ͣ���Ӧ�ľ��ǻ���ID
    LONGLONG llValue;  // �һ���ֵ

    // ��չ����
    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD dwReserve5;
};
//������
#define TK_GS2PIVOT_GAIN_CHIP_COMMON_64_UNKNOWN 1
struct msg_pivot2gs_ack_gain_chip_common_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;

    //
    DWORD dwDCID;
    DWORD dwAccType;
    DWORD dwDTID;
    LONGLONG llValue;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ���ٱ��������˷�[�����û��������](ϵͳ֧��)[ͨ��64λ]
//   ˵    ���� 1�� PIVOT -> GS
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_EXPEND_CHIP_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1C)
//����
struct msg_gs2pivot_req_expend_chip_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // ��Ʒ��Ϣ
    DWORD dwGameID;  // ��ϷID
    DWORD dwMPID;    // ������ƷID

    // ������Ϣ
    DWORD dwPlatType;  // ƽ̨����
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat����ͳ�����ݷ���ʹ��
    DWORD dwLanguageID;

    // ҵ�����
    DWORD dwDCID;      // ��������ID��1-Money��2-��Ʒ��3-����
    DWORD dwAccType;   // �˻����ͣ�һ��ֻ��Moneyʹ�ã�0-Ĭ�ϣ�3000-�齫�����˻�
    DWORD dwDTID;      // ����ID�����磺�������ͣ���Ӧ�ľ��ǻ���ID
    LONGLONG llValue;  // �һ���ֵ

    // ��չ����
    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD dwReserve5;
};
//������
struct msg_pivot2gs_ack_expend_chip_common_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;

    //
    DWORD dwDCID;
    DWORD dwAccType;
    DWORD dwDTID;
    LONGLONG llValue;
};

//--------------------------------------------------------------------------------------------------------
//   ��    �ܣ� ����ҵ��[�۳��û��������](ϵͳ����)[ͨ��64λ]
//   ˵    ���� 1��GS -> PIVOT
//					2��GS�ڷ���Ϣʱ���ס����˿۳����������ڷ��سɹ���������Ӧ���ĳ���
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_PAYMENT_BUSINESS_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1D)
//����
struct msg_gs2pivot_req_payment_business_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // ��Ʒ��Ϣ
    DWORD dwGameID;          // ��ϷID
    DWORD dwMPID;            // ������ƷID
    DWORD dwTourneyID;       // ����ID
    DWORD dwMatchID;         // ����ID
    __time32_t tMatchBegin;  // ������ʼʱ��

    // ������Ϣ
    DWORD dwPlatType;  // ƽ̨����
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat����ͳ�����ݷ���ʹ��
    DWORD dwLanguageID;

    // ҵ�����
    DWORD dwPaymentBusinessTypeID;  // ҵ�����Ͷ��� �μ�: eDicPaymentBusinessTypeDef
    DWORD dwPaymentBusinessSubID;   // ҵ��������ID��������š�0������Ĭ��Ϊ1����1��ʼ��š�
                                   // ��dwPaymentBusinessTypeID=1020���ڳ,dwPaymentBusinessSubID���ڳ﷢��ʱ��
    char szNote
        [TK_BUFLEN_NOTE];  // �˵�ע�ͣ����Ϊ�գ���Ĭ�����ҵ���������ơ�Money��ķ��ó��⣬���Ϲ���ֻ�и���Ͷһ���
    DWORD dwRank;          // ��������
    DWORD dwParam1;        // ������ڳ�ҵ�񣬴˲�����Ӧ�����ڳ����
    DWORD dwParam2;        // ������ڳ�ҵ�񣬴˲�����Ӧ�����ڳ�ID
    DWORD dwParam3;        // ������ڳ�ҵ�񣬴˲�����Ӧ�����ڳ���ܽ��
    DWORD dwParam4;        // ������ڳ�ҵ�񣬴˲�����Ӧ�����ڳ﷢�����û�ID
    DWORD dwParam5;

    // �۷Ѳ��֣�֧�ֶ�����úϲ�
    DWORD dwPaymentCount;             // ��������
    TKSUFFIXIDX dsPaymentInfoSuffix;  // dwPaymentCount��TK_PAYMENTINFO_64
};
//������
#define TK_GS2PIVOT_PAYMENT_BUSINESS_COMMON_64_UNKNOWN 1
struct msg_pivot2gs_ack_payment_business_common_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;

    //
    DWORD dwPaymentBusinessTypeID;
    DWORD dwPaymentBusinessSubID;
};
