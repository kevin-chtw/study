#pragma once

#include "TKProtocol.h"

//////////////////////////////////////////////////////////////////////////////////////////结构：
//事件发生地属属性组
typedef struct tagTKOriEventPlaceProperty
{
    DWORD dwPlatform;  // 平台：PC、Flash、Mobile等 定义参见 eDicOEPPlatformType
    DWORD dwGID;       // 游戏ID
    DWORD dwMPType;    // 产品类型：岛屿、锦标赛等 定义参见 eDicOEPMPType
    DWORD dwMPID;      // 产品ID
    DWORD dwStage;     // 阶段
} TKORIEVENTPLACEPROPERTY, *PTKORIEVENTPLACEPROPERTY;

//事件发生人属属性组
typedef struct tagTKOriEventPersonProperty
{
    DWORD dwPP1;  // CMSTID
    DWORD dwPP2;
    DWORD dwPP3;
    DWORD dwPP4;
    DWORD dwPP5;
} TKORIEVENTPERSONPROPERTY, *PTKORIEVENTPERSONPROPERTY;

//--------------------------------------------------------------------------------------------------------
//   功    能： 获得某一种Grow信息
//   说    明： 1、ComitService->GrowService
//              2、Data 2.0 新增
//--------------------------------------------------------------------------------------------------------
#define TKID_COMITS2MATCHS_GETUSERSINGLEGROW (TKID_MATCHS2COMITSBASE + 0x011)
// 结构：
typedef struct tagTkMatchSTranGrow
{
    DWORD dwGrowID;  // 积分ID
    DWORD dwValue;   // 积分值

} TKMATCHSTRANGROW, *PTKMATCHSTRANGROW;

//--------------------------------------------------------------------------------------------------------
//   功    能： 获得某一种Grow信息 64位
//--------------------------------------------------------------------------------------------------------
#define TKID_COMITS2MATCHS_GETUSERSINGLEGROW_64 (TKID_MATCHS2COMITSBASE + 0x04F)
// 结构：
typedef struct tagTkMatchSTranGrow64
{
    DWORD dwGrowID;   // 积分ID
    __int64 llValue;  //64位积分值

} TKMATCHSTRANGROW64, *PTKMATCHSTRANGROW64;

//--------------------------------------------------------------------------------------------------------
//   功    能： 获取一条USR数据
//   说    明： 1、MS -> DS
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
//   功    能： 增加比赛筹码扣费[扣除用户虚拟货币](系统收入)
//   说    明： 1、GS -> PIVOT
//					2、GS在发消息时需记住这个人扣除货币数，在返回成功后增加相应量的筹码
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GAIN_CHIP (TK_MSG_GAME_PIVOT + 0x01)
//请求：
struct msg_gs2pivot_req_gain_chip : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // 比赛产品ID
    DWORD dwMatchID;

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11
    DWORD
    dwCMTSID;  // 说    明：组委会方案ID，集合包含了颁奖，扣费，回兑……各类操作所需的“动态分类”，“静态条件”……各类属性。
    // 应用举例：苹果手机用户颁奖，指名苹果用户这个动态分类，及此分类用户的各类属性。

    DWORD dwUserID;
    DWORD dwGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//回馈：
#define TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN 1
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN  表示未知原因失败
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_SRCLACK  表示金币不足
//header.dwParam = TK_DSGAINMATCHLIFEVALUEBONUS_SRCLACK 表示奖券不足
//header.dwParam = TK_DSGAINMATCHLIFEVALUECERT_SRCLACK  表示代金券(门票)不足
struct msg_pivot2gs_ack_gain_chip : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGold;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 减少比赛筹码退费[增加用户虚拟货币](系统支出)
//   说    明： 1、 PIVOT -> GS
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_EXPEND_CHIP (TK_MSG_GAME_PIVOT + 0x02)
//请求：
struct msg_gs2pivot_req_expend_chip : TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // 比赛产品ID
    DWORD dwMatchID;

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    DWORD
    dwCMTSID;  // 说    明：组委会方案ID，集合包含了颁奖，扣费，回兑……各类操作所需的“动态分类”，“静态条件”……各类属性。
    // 应用举例：苹果手机用户颁奖，指名苹果用户这个动态分类，及此分类用户的各类属性。

    DWORD dwUserID;
    DWORD dwGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;

    //
    int nDomain;  // add @ 2022/8/22
};

//回馈：
#define TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN 1
//header.dwParam = TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN  表示未知原因失败
struct msg_pivot2gs_ack_expend_chip : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGold;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 比赛修改专属积分-- 如:"专属积分"
//   说    明： 1、GS -> PIVOT
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_DEAL_GROW (TK_MSG_GAME_PIVOT + 0x03)
#define TK_DSMATCHDEALRESULTGROW_UNKNOWN 1  // 未知原因失败
// 请求
struct msg_gs2pivot_req_deal_grow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;  // 比赛产品ID

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11
    DWORD dwReserve1;  // 保留字段1
    DWORD dwReserve2;  // 保留字段2
    DWORD dwReserve3;  // 保留字段3
    DWORD dwPort;      // 服务端口

    __time32_t tMatchBegin;  // 比赛开始时间
    DWORD dwTourneyID;       // 赛事ID
    DWORD dwMatchID;         // 比赛ID
    WORD wGrowDomainID;      // 积分域ID
    DWORD dwRank;            // 比赛名次

    DWORD dwGrowID;  // 积分ID
    int nValue;      // 积分的修改值，此值有正负

    char szNote[TK_BUFLEN_NOTE];  // 说明，不可为空
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//回馈：
struct msg_pivot2gs_ack_deal_grow : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGrowID;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 获取某一种作用域的全部Grow信息
//   说    明： 1、ComitService->GrowService
//              2、Comit 2.0 新增
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_GROW (TK_MSG_GAME_PIVOT + 0x04)
// 请求：
struct msg_gs2pivot_req_get_grow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwDomainID;  // 积分域ID(定义参阅 TKGrowUniversalDicSrv.h 文件)

    DWORD dwMPID;  // 比赛产品ID
    DWORD dwMatchID;

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
// 回馈：
#define TKID_GS2PIVOT_GET_GROW_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_get_grow : public TKHEADER
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwDomainID;  // 积分域ID
    DWORD dwMatchID;

    TKSUFFIXIDX stSuffixIdx;  // Grow列表缓冲区偏移量
    DWORD dwGrowCount;        // Grow列表中列表项个数
    // “回馈”后缀数据：
    //  dwGrowCount个 TKMATCHSTRANGROW 结构
};

//------------------------------------------------------------------------------------------------------
//   功    能： 比赛货币奖励(系统支出)
//   说    明： 1、GS -> PIVOT
//------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_AWARD_MONEY (TK_MSG_GAME_PIVOT + 0x05)
//请求：
struct msg_gs2pivot_req_award_money : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMatchID;
    DWORD dwMPID;      // 比赛产品ID
    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    DWORD
    dwCMTSID;  // 说    明：组委会方案ID，集合包含了颁奖，扣费，回兑……各类操作所需的“动态分类”，“静态条件”……各类属性。
    // 应用举例：苹果手机用户颁奖，指名苹果用户这个动态分类，及此分类用户的各类属性。

    DWORD
    dwSrcTypeID;  // 具体奖励类别：具体参见 TKComitUniversalDicSrv.h中eDicComitGatherMetadataSrcTypeDef的TKCOMMSRV_ENUM_DATA_COMMONMONEY_打头部分
    DWORD dwGold;

    DWORD dwTourneyID;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;

    DWORD dwCoin;   //元宝（Coin）
                    //     DWORD dwGold;   //金币（Gold）
    DWORD dwBonus;  //奖券（Bonus）
    DWORD dwCert;   //参赛积分（Cert）

    int nDomain;
};
//回馈：
#define TKID_GS2PIVOT_AWARD_MONEY_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_award_money : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGold;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 比赛物品奖励（系统支出）
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_AWARD_WARE (TK_MSG_GAME_PIVOT + 0x06)
//请求：
struct msg_gs2pivot_req_award_ware : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMatchID;
    DWORD dwMPID;      // 比赛产品ID
    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    DWORD
    dwSrcTypeID;  // 具体奖励类别：具体参见 TKComitUniversalDicSrv.h中eDicComitGatherMetadataSrcTypeDef的TKCOMMSRV_ENUM_DATA_COMMONWARE_打头部分
    DWORD dwWareTypeID;
    DWORD dwCount;

    DWORD dwTourneyID;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
    int nDomain;
};
//回馈：
#define TKID_GS2PIVOT_AWARD_WARE_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_award_ware : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwWareTypeID;
    DWORD dwCount;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 比赛积分奖励（系统支出）
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_AWARD_GROW (TK_MSG_GAME_PIVOT + 0x07)
//请求：
struct msg_gs2pivot_req_award_grow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;      // 比赛产品ID
    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    __time32_t tMatchBegin;  // 比赛开始时间
    DWORD dwTourneyID;       // 赛事ID
    DWORD dwMatchID;         // 比赛ID
    WORD wGrowDomainID;      // 积分域ID
    DWORD dwRank;            // 比赛名次

    DWORD
    dwSrcTypeID;  // 具体奖励类别：具体参见 TKComitUniversalDicSrv.h中eDicComitGatherMetadataSrcTypeDef的TKCOMMSRV_ENUM_DATA_COMMONGROW_打头部分
    DWORD dwGrowID;  // 积分ID
    int nValue;      // 积分的修改值，此值有正负
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
    int nDomain;
};
//回馈：
#define TKID_GS2PIVOT_AWARD_GROW_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_award_grow : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGrowID;  // 积分ID
    int nValue;      // 积分的修改值，此值有正负
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 捕鱼增加炮弹（筹码）扣除物品（系统收入）
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_USE_WARE (TK_MSG_GAME_PIVOT + 0x08)
#define TK_CSCATCHFISHGAINSHELLFROMWARE_SRCLACK (10)  // 物品不足
#define TK_CSCATCHFISHGAINSHELLFROMWARE_UNKNOWN (1)   // 未知原因
//请求：
struct msg_gs2pivot_req_use_ware : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMatchID;
    DWORD dwMPID;  // 比赛产品ID

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    DWORD dwUserID;
    DWORD dwWareTypeID;
    DWORD dwCount;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//回馈：
//header.dwParam = TK_CSCATCHFISHGAINSHELLFROMWARE_SRCLACK  表示物品不足
struct msg_pivot2gs_ack_use_ware : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwWareTypeID;
    DWORD dwCount;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 获得某一种Grow信息
//   说    明： 1、GS -> PIVOT
//   创建时间： 2016-10-27
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_SINGLEGROW (TK_MSG_GAME_PIVOT + 0x09)
//请求：
struct msg_gs2pivot_req_get_singleGrow : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    TKHEADER header;
    DWORD dwUserID;
    DWORD dwGrowID;

    DWORD dwMPID;  // 比赛产品ID

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // 服务端口
};
// 回馈：
#define TKID_GS2PIVOT_GET_SINGLEGROW_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_get_singleGrow : public TKHEADER
{
    DWORD dwReserve1;  //预留字段
    DWORD dwReserve2;
    WORD wReserve3;
    WORD wReserve4;
    DWORD dwReserve5;
    DWORD dwReserve6;
    DWORD dwReserve7;

    DWORD dwUserID;
    TKSUFFIXIDX stSuffixIdx;
};
// “回馈”后缀数据：
//  一个 TKMATCHSTRANGROW 结构

//--------------------------------------------------------------------------------------------------------
//   功    能： 修改经验值
//   说    明： 1、GS -> PIVOT
//   创建时间： 2017-02-17
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_DEAL_RESULTSCORE (TK_MSG_GAME_PIVOT + 0x0A)
#define TK_DSMATCHDEALRESULTSCORE_UNKNOWN 1  // 未知原因失败
//请求
struct msg_gs2pivot_req_deal_resultScore : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;

    DWORD dwMPID;      // 比赛产品ID
    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // 服务端口

    __time32_t tMatchBegin;  // 比赛开始时间
    DWORD dwTourneyID;       // 赛事ID
    DWORD dwMatchID;         // 比赛ID

    WORD wGrowDomainID;  // 积分域ID

    DWORD dwRank;  // 比赛名次

    int nScore;  // 经验值，此值是差异值

    int nReserve1;  // 保留1
    int nReserve2;  // 保留2
    int nReserve3;  // 保留3
    int nReserve4;  // 保留4

    int nMasterScore;  // 大师分，此值是差异值
    int nMatchCount;   // 游戏次数，此值是差异值(单位：次)
    int nMatchTime;    // 游戏时间，此值是差异值(单位：秒)

    char szNote[TK_BUFLEN_NOTE];  // 说明，不可为空
};
//回馈
struct msg_pivot2gs_ack_deal_resultScore : public TKHEADER
{  // TKHEADER dwParam 表示经验不足原因，参见：eDicCommSrvScoreSRCLack
    DWORD dwUserID;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 原生事件消息
//   说    明： 1、GS -> PIVOT
//              2、消息发往事件组委会
//   创建时间： 2017-11-09
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_ORIGINALEVENT (TK_MSG_GAME_PIVOT + 0x0B)
struct msg_gs2pivot_req_originalevent : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;      // 用户ID(事件组委会成为dwPID)
    DWORD dwGameID;      // 游戏ID
    DWORD dwOEID;        // 原生事件ID
    __time32_t tOEvent;  // 原生事件时间
    DWORD dwOEValue;     // 原生事件值

    DWORD dwTourneyID;   // 赛事ID
    DWORD dwMatchID;     // 比赛ID
    WORD wGrowDomainID;  // 积分域ID
    DWORD dwMsPort;

    char szTourneySimpleName[TK_BUFLEN_TOURNEYSIMPLENAME];  // Tourney简称
    char szNote[TK_BUFLEN_MATCHAWARDNOTE];  // 注释，该注释与 szTourneySimpleName 联合使用，如果为空客户端将不显示
    // 如果不为空则客户端 先显示 szTourneySimpleName 再显示 szNote，构成完整

    TKORIEVENTPLACEPROPERTY dsOEPlacePro;    // 地属属性组 dwMPID为产品ID(根据需要)
    TKORIEVENTPERSONPROPERTY dsOEPersonPro;  // 人属属性组

    TKSUFFIXIDX stSuffix;  // 后缀数据索引
};
//后跟 DSORIEVENTDATA + DSORIEVENTDATA + ... 结构数据，用于外带一些标识数据，数据个数为：stSuffix.dwSuffixSize/sizeof(DSORIEVENTDATA)

//回馈
//错误码(dwParam)
#define TKDEF_ORIGINALEVENT_UNKNOWN 1                // 未知原因
#define TKDEF_ORIGINALEVENT_DEPLOY_ERROR 100         // 配置有误
#define TKDEF_ORIGINALEVENT_GETUSERINFO_FAIL 101     // 获取用户信息失败
#define TKDEF_ORIGINALEVENT_AWARD_FAIL 102           // 奖励发放失败
#define TKDEF_ORIGINALEVENT_NO_AVAILABLE_SCHEME 103  // 没有可用的奖励方案
struct msg_pivot2gs_ack_originalevent : public TKHEADER
{
    DWORD dwUserID;   //用户ID
    DWORD dwOEID;     //原生事件ID
    DWORD dwOEValue;  //原生事件值
    DWORD dwMPID;     //产品id
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 获取一条USR数据
//   说    明： 1、GS -> PIVOT
//   创建时间： 2017-11-23
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
//回馈：
#define TKID_GS2PIVOT_GETUSERINFO_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_getuserinfo : public TKHEADER
{
    DWORD dwUserID;
    DWORD dwOriID;
    TK_CMTS2MATCHS_USERDATA data;
};
//回馈后缀TK_CMTS2MATCHS_USERDATA结构体中wlen长度的数据

//--------------------------------------------------------------------------------------------------------
//   功    能： 付费业务[扣除用户虚拟货币](系统收入)
//   说    明： 1、GS -> PIVOT
//					2、GS在发消息时需记住这个人扣除货币数，在返回成功后增加相应量的筹码
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_PAYMENT_BUSINESS (TK_MSG_GAME_PIVOT + 0x0D)
//请求：
struct msg_gs2pivot_req_payment_business : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;      // 比赛产品ID
    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型
    DWORD dwMatchID;
    DWORD dwTourneyID;
    __time32_t tMatchBegin;  // 比赛开始时间
    DWORD dwRank;            // 比赛名次

    WORD wDomain;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwPort;  // 服务端口

    DWORD dwPaymentBusinessTypeID;  // 业务类型定义 参见: eDicPaymentBusinessTypeDef
    DWORD dwPaymentBusinessSubID;   // 业务类型子ID，如表情编号。0保留，默认为1，从1开始编号。
    char szNote
        [TK_BUFLEN_NOTE];  // 账单注释，如果为空，则默认填充业务类型名称。Money类的费用除外，走老规则（只有复活和兑换）
    DWORD dwLanguageID;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;

    DWORD
    dwCMTSID;  // 说    明：组委会方案ID，集合包含了颁奖，扣费，回兑……各类操作所需的“动态分类”，“静态条件”……各类属性。
    // 应用举例：苹果手机用户颁奖，指名苹果用户这个动态分类，及此分类用户的各类属性。
    DWORD dwMoneyAccType;

    // 扣费部分，支持多项费用合并
    DWORD dwPaymentCount;             // 费用数量
    TKSUFFIXIDX dsPaymentInfoSuffix;  // dwPaymentCount个TK_PAYMENTINFO
};
//回馈：
#define TK_DSGAINMATCHLIFEVALUEGOLD_PAYMENT_BUSINESS_UNKNOWN 1
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_PAYMENT_BUSINESS_UNKNOWN  表示未知原因失败
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_PAYMENT_BUSINESS_SRCLACK  表示金币不足
//header.dwParam = TK_DSGAINMATCHLIFEVALUEBONUS_PAYMENT_BUSINESS_SRCLACK 表示奖券不足
//header.dwParam = TK_DSGAINMATCHLIFEVALUECERT_PAYMENT_BUSINESS_SRCLACK  表示代金券(门票)不足
struct msg_pivot2gs_ack_payment_business : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwPaymentBusinessTypeID;
    DWORD dwPaymentBusinessSubID;
};

//-----------------------------------------------------------------------------------------------------------------
//   功    能： 请求标签数据
//   说    明： 1、GS -> PIVOT
//   创建时间： 2019-2-19
//-----------------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GETTAGDATA (TK_MSG_GAME_PIVOT + 0x0E)
struct msg_gs2pivot_req_gettagdata : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;     // 请求方的UserID
    DWORD dwTourneyID;  // 赛事ID
    DWORD dwMatchID;    // 比赛ID
    DWORD dwTagID;      // 标签ID
    DWORD dwMPID;       // 产品ID
    DWORD dwGameID;     // 游戏ID
};

// Ack
#define TKID_GS2PIVOT_GETTAGDATA_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_gettagdata : public TKHEADER
{
    DWORD dwUserID;     // 请求方的UserID
    DWORD dwTourneyID;  // 赛事ID
    DWORD dwMatchID;    // 比赛ID
    DWORD dwTagID;      // 标签ID
    DWORD dwMPID;       // 产品ID
    ULONG64 llValue;    // 标签值
};

//-----------------------------------------------------------------------------------------------------------------
//   功    能： 获得某一种Grow信息 64位
//   说    明： 1、GS -> PIVOT
//   创建时间： 2019-3-17
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

    DWORD dwMPID;      // 比赛产品ID
    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11
    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // 服务端口

    DWORD dwReserve1;
    DWORD dwReserve2;
};
//回馈：
struct msg_pivot2gs_ack_get_singleGrow_64 : public TKHEADER
{
    DWORD dwReserve1;  //预留字段
    DWORD dwReserve2;
    WORD wReserve3;
    WORD wReserve4;
    DWORD dwReserve5;
    DWORD dwReserve6;
    DWORD dwReserve7;

    DWORD dwUserID;
    TKSUFFIXIDX stSuffixIdx;
};
// “回馈”后缀数据：
//  一个 TKMATCHSTRANGROW64 结构

//--------------------------------------------------------------------------------------------------------
//   功    能： 比赛修改专属积分-- 如:"专属积分" 64位
//   说    明： 1、GS -> PIVOT
//   创建时间： 2019-3-17
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_DEAL_GROW_64 (TK_MSG_GAME_PIVOT + 0x10)

struct msg_gs2pivot_req_deal_grow_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;

    DWORD dwMPID;  // 比赛产品ID

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // 服务端口

    __time32_t tMatchBegin;  // 比赛开始时间
    DWORD dwTourneyID;       // 赛事ID
    DWORD dwMatchID;         // 比赛ID
    DWORD dwGrowDomainID;    // 积分域ID
    DWORD dwRank;            // 比赛名次

    DWORD dwGrowID;   // 积分ID
    __int64 llValue;  // 积分的修改值，此值有正负，目前只支持32位范围

    char szNote[TK_BUFLEN_NOTE];  // 说明，不可为空

    DWORD dwLanguageID;

    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
};
//回馈
struct msg_pivot2gs_ack_deal_grow_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGrowID;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 获得用户Score信息
//   说    明： 1、MS -> DS
//              2、Data 1.0 新增
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GETUSERSCORE (TK_MSG_GAME_PIVOT + 0x11)
#define TKID_GS2PIVOT_GETUSERSCORE_UNKNOWN (1)   // 未知原因失败
#define TKID_GS2PIVOT_GETUSERSCORE_NOEXIST (10)  // 用户没有该游戏积分
//请求：
struct msg_gs2pivot_req_getuserscore : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwMPID;  // 比赛产品ID

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // 服务端口
};

//回馈：
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
//   功    能： 查询用户在某个比赛的最好成绩
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GETMARKERINFO (TK_MSG_GAME_PIVOT + 0x12)
struct msg_gs2pivot_req_getmarkerinfo : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    }

    DWORD dwUserID;
    DWORD dwProductId;  // 比赛产品ID
    DWORD dwGameID;     // 游戏ID
};

//回馈：
#define TKID_GS2PIVOT_GETMARKERINFO_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_getmarkerinfo : public TKHEADER
{
    DWORD dwUserID;       // 用户Id
    DWORD dwProductId;    // 比赛的产品ID
    DWORD dwRank;         // 最好名次
    DWORD dwQuantity;     // 比赛参赛人数
    DWORD dwMarkerStamp;  // 比赛开始时间，时间戳

    //// 最好名次次数，冠军次数是最好名次次数的子集
    // if (1 == dwRank) {
    //    冠军次数;
    // } else {
    //    最好名次次数;
    // }
    DWORD dwBestRankCount;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 增加比赛筹码扣费[扣除用户虚拟货币](系统收入)
//   说    明： 1、GS -> PIVOT
//					2、GS在发消息时需记住这个人扣除货币数，在返回成功后增加相应量的筹码
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GAIN_CHIP_64 (TK_MSG_GAME_PIVOT + 0x13)
//请求：
struct msg_gs2pivot_req_gain_chip_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // 比赛产品ID
    DWORD dwMatchID;

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11
    DWORD
    dwCMTSID;  // 说    明：组委会方案ID，集合包含了颁奖，扣费，回兑……各类操作所需的“动态分类”，“静态条件”……各类属性。
    // 应用举例：苹果手机用户颁奖，指名苹果用户这个动态分类，及此分类用户的各类属性。

    DWORD dwUserID;
    ULONGLONG llGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//回馈：
//#define TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN			1
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_UNKNOWN  表示未知原因失败
//header.dwParam = TK_DSGAINMATCHLIFEVALUEGOLD_SRCLACK  表示金币不足
//header.dwParam = TK_DSGAINMATCHLIFEVALUEBONUS_SRCLACK 表示奖券不足
//header.dwParam = TK_DSGAINMATCHLIFEVALUECERT_SRCLACK  表示代金券(门票)不足
struct msg_pivot2gs_ack_gain_chip_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    ULONGLONG llGold;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 减少比赛筹码退费[增加用户虚拟货币](系统支出)
//   说    明： 1、 PIVOT -> GS
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_EXPEND_CHIP_64 (TK_MSG_GAME_PIVOT + 0x14)
//请求：
struct msg_gs2pivot_req_expend_chip_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMPID;  // 比赛产品ID
    DWORD dwMatchID;

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    DWORD
    dwCMTSID;  // 说    明：组委会方案ID，集合包含了颁奖，扣费，回兑……各类操作所需的“动态分类”，“静态条件”……各类属性。
    // 应用举例：苹果手机用户颁奖，指名苹果用户这个动态分类，及此分类用户的各类属性。

    DWORD dwUserID;
    ULONGLONG llGold;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;
};
//回馈：
//#define TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN		1
//header.dwParam = TK_GS2PIVOT_EXPEND_CHIP_UNKNOWN  表示未知原因失败
struct msg_pivot2gs_ack_expend_chip_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    ULONGLONG llGold;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 查询用户的64位Money信息
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_MONEY_64 (TK_MSG_GAME_PIVOT + 0x15)
// 请求：
struct msg_gs2pivot_req_get_money_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwMatchID;
    DWORD dwUserID;        // 用户ID。在协议中使用DWORD，在程序及数据库中使用int
    DWORD dwMoneyAccType;  // Money系统帐号类型
};
// 回馈：
#define TKID_GS2PIVOT_GET_MONEY_64_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_get_money_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;

    ULONGLONG llCoin;   //元宝（Coin）
    ULONGLONG llGold;   //金币（Gold）
    ULONGLONG llBonus;  //奖券（Bonus）
    ULONGLONG llCert;   //参赛积分（Cert）
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 获得单个Ware信息
//   说    明： 1、GS <-> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_GETSINGLEWARE (TK_MSG_GAME_PIVOT + 0x16)
//请求
struct msg_gs2pivot_req_get_singleware : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwGameID;  // 游戏ID(0位走默认组委会)
    DWORD dwWATID;   // Ware系统帐号类型
    DWORD dwWID;     // 物品ID
};

// 回馈：
#define TKID_GS2PIVOT_GET_GETSINGLEWARE_UNKNOWN 1  // 未知原因失败
// 后跟若干个物品列表项，表项结构见TKDATASTRANWAREBASEINFO定义
struct msg_gs2pivot_ack_get_singleware : public TKHEADER
{
    DWORD dwUserID;

    DWORD dwWareListOffset;  // Ware列表缓冲区偏移量，相对于消息缓冲区顶端物品列表
    DWORD dwItemCount;       // Ware列表中列表项个数
    DWORD dwWareItemSize;    // Ware列表项大小(后缀数据项的每一项大小)
};
// 回馈后缀数据：
// dwItemCount * TKMATCHS_TRANWAREBASEINFO

//--------------------------------------------------------------------------------------------------------
//   功    能：比赛付费业务通用接口[64位]
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_PAYMENT_BUSINESS_64 (TK_MSG_GAME_PIVOT + 0x17)

#define TK_PAYMENT_BUSINESS_MONEY_SRCLACK 100  // 货币不足（除了金币，奖券，代金券以外的问题）
#define TK_PAYMENT_BUSINESS_GOLD_SRCLACK 101   // 金币不足
#define TK_PAYMENT_BUSINESS_BONUS_SRCLACK 102  // 奖券不足
#define TK_PAYMENT_BUSINESS_CERT_SRCLACK 103   // 代金券（门票）不足
#define TK_PAYMENT_BUSINESS_WARE_SRCLACK 104   // 物品不足
#define TK_PAYMENT_BUSINESS_GROW_SRCLACK 105   // 积分不足
#define TK_PAYMENT_BUSINESS_SRCLACK 106        // 源货币不足
#define TK_PAYMENT_BUSINESS_DSTACCNOEXIST 107  // 目标账户不存在

#define TK_PAYMENT_BUSINESS_NOT_SUPPORT 200                        // 不支持此类扣费业务
#define TK_PAYMENT_BUSINESS_RESTRICT (TK_COMMSRV_USER_RESTICTING)  // 用户被限制操作

//请求
struct msg_gs2pivot_req_payment_business_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;

    DWORD dwMPID;      // 比赛产品ID
    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型
    DWORD dwMatchID;
    DWORD dwTourneyID;
    __time32_t tMatchBegin;  // 比赛开始时间
    DWORD dwRank;            // 比赛名次

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwPort;  // 服务端口

    DWORD dwPaymentBusinessTypeID;  // 业务类型定义 参见: eDicPaymentBusinessTypeDef
    DWORD dwPaymentBusinessSubID;   // 业务类型子ID，如表情编号。0保留，默认为1，从1开始编号。
    char szNote
        [TK_BUFLEN_NOTE];  // 账单注释，如果为空，则默认填充业务类型名称。Money类的费用除外，走老规则（只有复活和兑换）
    DWORD dwLanguageID;
    DWORD dwReserve2;  // 众筹ID
    DWORD dwReserve3;  // 众筹的总金额
    DWORD dwReserve4;  // 众筹发起人用户ID

    // 扣费部分，支持多项费用合并
    DWORD dwPaymentCount;             // 费用数量
    TKSUFFIXIDX dsPaymentInfoSuffix;  // dwPaymentCount个TK_PAYMENTINFO_64
};

// 回馈：
struct msg_gs2pivot_ack_payment_business_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwPaymentBusinessTypeID;
    DWORD dwPaymentBusinessSubID;
};

//--------------------------------------------------------------------------------------------------------
//   功    能：比赛货币奖励64位(系统支出)
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_COMMONMONEYREWARD_64 (TK_MSG_GAME_PIVOT + 0x18)
//请求
struct msg_gs2pivot_req_commonmoneyreward_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwMPID;        // 比赛产品ID
    DWORD dwGameID;      // 游戏ID
    DWORD dwPlatType;    // 平台类型 pangs added 2013-07-11
    DWORD dwTouerneyID;  // 保留字段1 ----用作TouerneyID pangs added 2014-11-04
    DWORD dwMatchID;     // 保留字段2 ----用作MatchID pangs added 2014-11-04

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;  //报名方式：0--普通；1--二人组队 modify by hfc 2021-9-26

    DWORD dwPort;  // 服务端口

    DWORD dwMoneyAccType;

    DWORD
    dwSrcTypeID;  // 具体奖励类别：具体参见 TKComitUniversalDicSrv.h中eDicComitGatherMetadataSrcTypeDef的TKCOMMSRV_ENUM_DATA_COMMONMONEY_打头部分
    ULONGLONG llCoin;
    ULONGLONG llBonus;
    ULONGLONG llGold;
    ULONGLONG llCert;

    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwLanguageID;
    char szNote[TK_BUFLEN_NOTE];  // 标准注释
};

//回馈：
struct msg_gs2pivot_ack_commonmoneyreward_64 : public TKHEADER
{
    DWORD dwUserID;
    ULONGLONG llCoin;
    ULONGLONG llBonus;
    ULONGLONG llGold;
    ULONGLONG llCert;
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 获取某一种作用域的全部64位Grow信息
//   说    明： 1、ComitService->GrowService
//              2、Comit 2.0 新增
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GET_DOMAINGROW_64 (TK_MSG_GAME_PIVOT + 0x19)
// 请求：
struct msg_gs2pivot_req_get_domaingrow_64 : public TKHEADER
{
    inline DWORD GetMsgGroup()
    {
        return dwUserID;
    };

    DWORD dwUserID;
    DWORD dwDomainID;  // 积分域ID(定义参阅 TKGrowUniversalDicSrv.h 文件)

    DWORD dwMPID;  // 比赛产品ID
    DWORD dwMatchID;

    DWORD dwGameID;    // 游戏ID
    DWORD dwPlatType;  // 平台类型 pangs added 2013-07-11

    WORD wReserve1;
    BYTE byOSType;
    BYTE byReserve1;
    DWORD dwAppID;
    DWORD dwSiteID;

    DWORD dwPort;  // 服务端口

    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
};
// 回馈：
#define TKID_GS2PIVOT_GET_GROW_UNKNOWN 1  // 未知原因失败
struct msg_pivot2gs_ack_get_domaingrow_64 : public TKHEADER
{
    DWORD dwUserID;
    DWORD dwDomainID;  // 积分域ID

    TKSUFFIXIDX stSuffixIdx;  // Grow列表缓冲区偏移量
    DWORD dwGrowCount;        // Grow列表中列表项个数
    // “回馈”后缀数据：
    //  dwGrowCount个 TKMATCHSTRANGROW64 结构
};

//--------------------------------------------------------------------------------------------------------
//   功    能： 比赛奖励（系统支出）[通用64位]
//   说    明： 1、GS -> PIVOT
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_REWARD_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1A)
//请求：
struct msg_gs2pivot_req_reward_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // 产品信息
    DWORD dwGameID;          // 游戏ID
    DWORD dwMPID;            // 比赛产品ID
    DWORD dwTourneyID;       // 赛事ID
    DWORD dwMatchID;         // 比赛ID
    __time32_t tMatchBegin;  // 比赛开始时间

    // 环境信息
    DWORD dwPlatType;  // 平台类型
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat金融统计数据分类使用
    DWORD dwLanguageID;

    //业务参数
    DWORD dwGrowDomainID;  // 积分域ID，只有积分类型会使用到
    DWORD dwRank;          // 比赛名次
    DWORD
    dwSrcTypeID;  // 具体奖励类别：具体参见 TKComitUniversalDicSrv.h中eDicComitGatherMetadataSrcTypeDef的TKCOMMSRV_ENUM_DATA_COMMONGROW_打头部分
    DWORD dwDCID;        // 数据类型ID：1-Money；2-物品；3-积分
    DWORD dwAccType;     // 账户类型，一般只有Money使用：0-默认；3000-麻将单包账户
    DWORD dwDTID;        // 数据ID，例如：积分类型，对应的就是积分ID
    DWORD dwSignupMode;  // 报名方式：0--普通；1--二人组队
    LONGLONG llValue;    // 修改值，此值有正负

    //扩展参数
    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD dwReserve5;

    //
    int nDomain;
};
//回馈：
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
//   功    能： 增加比赛筹码扣费[扣除用户虚拟货币](系统收入)[通用64位]
//   说    明： 1、GS -> PIVOT
//					2、GS在发消息时需记住这个人扣除货币数，在返回成功后增加相应量的筹码
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_GAIN_CHIP_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1B)
//请求：
struct msg_gs2pivot_req_gain_chip_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // 产品信息
    DWORD dwGameID;  // 游戏ID
    DWORD dwMPID;    // 比赛产品ID

    // 环境信息
    DWORD dwPlatType;  // 平台类型
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat金融统计数据分类使用
    DWORD dwLanguageID;

    // 业务参数
    DWORD dwDCID;      // 数据类型ID：1-Money；2-物品；3-积分
    DWORD dwAccType;   // 账户类型，一般只有Money使用：0-默认；3000-麻将单包账户
    DWORD dwDTID;      // 数据ID，例如：积分类型，对应的就是积分ID
    LONGLONG llValue;  // 兑换数值

    // 扩展参数
    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD dwReserve5;
};
//回馈：
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
//   功    能： 减少比赛筹码退费[增加用户虚拟货币](系统支出)[通用64位]
//   说    明： 1、 PIVOT -> GS
//
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_EXPEND_CHIP_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1C)
//请求：
struct msg_gs2pivot_req_expend_chip_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // 产品信息
    DWORD dwGameID;  // 游戏ID
    DWORD dwMPID;    // 比赛产品ID

    // 环境信息
    DWORD dwPlatType;  // 平台类型
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat金融统计数据分类使用
    DWORD dwLanguageID;

    // 业务参数
    DWORD dwDCID;      // 数据类型ID：1-Money；2-物品；3-积分
    DWORD dwAccType;   // 账户类型，一般只有Money使用：0-默认；3000-麻将单包账户
    DWORD dwDTID;      // 数据ID，例如：积分类型，对应的就是积分ID
    LONGLONG llValue;  // 兑换数值

    // 扩展参数
    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD dwReserve5;
};
//回馈：
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
//   功    能： 付费业务[扣除用户虚拟货币](系统收入)[通用64位]
//   说    明： 1、GS -> PIVOT
//					2、GS在发消息时需记住这个人扣除货币数，在返回成功后增加相应量的筹码
//--------------------------------------------------------------------------------------------------------
#define TKID_GS2PIVOT_PAYMENT_BUSINESS_COMMON_64 (TK_MSG_GAME_PIVOT + 0x1D)
//请求：
struct msg_gs2pivot_req_payment_business_common_64 : public TKHEADER
{
    inline UINT64 GetMsgGroup()
    {
        return u64UserID;
    }

    UINT64 u64UserID;

    // 产品信息
    DWORD dwGameID;          // 游戏ID
    DWORD dwMPID;            // 比赛产品ID
    DWORD dwTourneyID;       // 赛事ID
    DWORD dwMatchID;         // 比赛ID
    __time32_t tMatchBegin;  // 比赛开始时间

    // 环境信息
    DWORD dwPlatType;  // 平台类型
    DWORD dwOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwStatDomainID;  // TkStat金融统计数据分类使用
    DWORD dwLanguageID;

    // 业务参数
    DWORD dwPaymentBusinessTypeID;  // 业务类型定义 参见: eDicPaymentBusinessTypeDef
    DWORD dwPaymentBusinessSubID;   // 业务类型子ID，如表情编号。0保留，默认为1，从1开始编号。
                                   // 当dwPaymentBusinessTypeID=1020【众筹】,dwPaymentBusinessSubID传众筹发起时间
    char szNote
        [TK_BUFLEN_NOTE];  // 账单注释，如果为空，则默认填充业务类型名称。Money类的费用除外，走老规则（只有复活和兑换）
    DWORD dwRank;          // 比赛名次
    DWORD dwParam1;        // 如果是众筹业务，此参数对应的是众筹份数
    DWORD dwParam2;        // 如果是众筹业务，此参数对应的是众筹ID
    DWORD dwParam3;        // 如果是众筹业务，此参数对应的是众筹的总金额
    DWORD dwParam4;        // 如果是众筹业务，此参数对应的是众筹发起人用户ID
    DWORD dwParam5;

    // 扣费部分，支持多项费用合并
    DWORD dwPaymentCount;             // 费用数量
    TKSUFFIXIDX dsPaymentInfoSuffix;  // dwPaymentCount个TK_PAYMENTINFO_64
};
//回馈：
#define TK_GS2PIVOT_PAYMENT_BUSINESS_COMMON_64_UNKNOWN 1
struct msg_pivot2gs_ack_payment_business_common_64 : public TKHEADER
{
    DWORD dwMatchID;
    DWORD dwUserID;

    //
    DWORD dwPaymentBusinessTypeID;
    DWORD dwPaymentBusinessSubID;
};
