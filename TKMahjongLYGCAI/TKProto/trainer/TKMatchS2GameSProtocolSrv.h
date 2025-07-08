#pragma once
#define TK_SVR_MAX_RELAY_MSG (300)

#include "framework.h"
#include "TKMatchProtocolBase.h"
#include "TKGeneralProtocol.h"
#include "TKMatchUniversalDicSrv.h"

/*****************************************************************************************************\
*                                                                     
*	MatchService 与 GameService 之间的协议
*	消息号：TKID_MS2GSBASE + 消息号偏移
*
\*****************************************************************************************************/

//-------------------------------------------------------------------------------------------------------------------------
//
// 常量和结构体定义
//
//-------------------------------------------------------------------------------------------------------------------------

//岛屿血瓶定义
#define MAX_HEMATINIC_COUNT (4)  // 可选的血瓶种类
typedef struct tagHematinic
{
    int nBlood;  // 补血量
    int nCost;   // 价钱(金币)
} HEMATINIC, *PHEMATINIC;

//岛屿血瓶定义（64位）
typedef struct tagHematinic_64
{
    int64_t i64Blood;     // 补血量
    int64_t i64Cost;      // 价钱(金币)
    uint32_t dwDisCount;  //折扣
} HEMATINIC_64, *PHEMATINIC_64;

//岛屿积分累进计算分级定义
#define MAX_SCOREMULTI_LEVEL_COUNT (4)  // 积分倍数分级最大级数
typedef struct tagScoreMultiLevel
{
    int cnGames;  // 每级所需牌副数
    int nMulti;   // 每级积分倍数
} SCOREMULTILEVEL, *PSCOREMULTILEVEL;

typedef struct tagTKSTAGERULER
{
    char szStageName[TK_BUFLEN_STAGENAME];       //阶段名
    char szStageRulerName[TK_BUFLEN_STAGENAME];  //阶段赛制名称
    char szTitleName[TK_BUFLEN_CLIENTTITLE];     //阶段显示标题

    int nAILevel;    //机器人只能水平,0最低，越大AI越高
    int nSaveFile;   //1：比赛服务存盘，0：比赛服务不存盘
    int nBoutCount;  // 总轮数(瑞士移位)

    int nScoreMultType;     //游戏计分倍分方式(倍数)，1=原游戏计分，2=原游戏计分*基数(不管用户积分大于还是小于基数)，3=原游戏计分*min(基数，用户积分)
    char szScoreType[64];   //游戏计分方式文字表述，如果没有设置，则按规则显示
    int nScoreBase;         //初始游戏基数，比方400，只在混战中使用
    int nOutRoundBaseRate;  //出局基数比例，比如25％设置为2500。（用户积分小于基数的多少比例就算出局，如果用户积分介于比例值和基数之间，输赢为用户积分值乘游戏倍数，再考虑上下限和平衡）
    int nOutRoundWarningBaseMult;  //出局警告基数倍数。当新的HandGame开始时，如果用户积分小于基数的该倍数时提示滚动警告
    int nMinWinnerOrder;           //出线最小名次
    int nSecondScoreType;          //平分方案（小分方案），三人淘汰时有两个相等的高分处理模式

    int nRestTime;              // 每局结束后可休息时间
    int nTax;                   // 每局抽水
    BOOL bCanRebuy;             // 筹码打立是否支持买入
    BOOL bCanAutoContinue;      // 支持自动开始下一局功能
    int nEnableBreakHeartTask;  // 支持碎心任务 wuqy add 2016.5.18
    BOOL bEnableHandWinAward;   // 支持赢牌奖励 wuqy add 2016.11.10
    DWORD adwReserve1[3];
    BOOL bEnableGameKickOut;    // 启用游戏中提前淘汰功能
    BOOL bHidePlayerUserID;     // 是否隐藏选手UserID
    int nOrderScoreFrom;        // 排名积分来源
    int nExchangeRate;          //
    int nTracedUserReShChance;  // 异常用户识别概率
    int nStageType;             // 赛制类型
    DWORD adwReserve2[6];
    DWORD dwMatchPlayType;         //玩法类型0-保留,1-积分城堡赛，2-挑战赛，3-班车闯关撒，4-岛屿大奖赛
    DWORD dwSpecialOpreation;      // 特殊操作：0-默认，1-1v1竞技场，2-个人等级赛，3-团体等级赛
    BOOL bShowResultTextPlay;      //是否文字播报一局结果(默认1播报)
    int nAutoPopRulePan;           //自动拉出局制信息面板方式：0不自动拉出，1自动拉出(默认)
    BOOL bShowOtherPlayerOrder;    //是否显示其他选手的名次信息(默认1，转播需要设置为0)
    BOOL bCheckWaitReGroup;        // 是否判断等待重组选手(CTKGambleChaosStage拆并桌配置用)
    BOOL bUpdateGrowValuePerGame;  // 是否每盘游戏结束更新GrowValue(默认FALSE)
    BOOL bShowOutRoundState;       // 是否显示出局状态(1默认显示)
    int nPlayersPerTable;          // 每桌游戏人数
    BOOL bHidePlayer;              // 是否隐藏选手
    int nSetInheritPercent;        // 副间带分百分比(0表示全带)

    BOOL bSortPlayerScore;          // 是否需要积分排名
    int nEstimatedProgressRound;    // 剩多少个round时计算时度
    int nEstimatedProgressInitial;  // 百分比初值
    BOOL bEnableRush;               // 启用Rush功能
    BOOL bEnableBounty;             // 是否有杀人赏金
    BOOL bEnableZeroHandOver;       // 是否允许0副局制结束（用于开赛时直退或超时进入结束当局重组）
    int nWaitEnterMatchSec;         // 选手首次进入超时等待时间

    BOOL bRandAwardScheme;                // 是否幸运轮盘赛(游戏服务可能需要根据这个调整游戏规则，比如拼十会调整底注限制)
    int64_t i64StandardScore;             //达标分数
    uint64_t u64ExChangeRate;             // 回兑率分子（回换率10000表示1，放大10000倍）
    uint64_t u64ExChangeRateDenominator;  // 回兑率分母（回换率10000表示1，放大10000倍），回兑的金币=筹码*u64ExChangeRate/(10000*u64ExChangeRateDenominator)
} TKSTAGERULER, *PTKSTAGERULER;
#define TK_SECONDSCORETYPE_LASTWINNERANDRANDOM 0  //平分时，最后赢的胜出否则随机
#define TK_SECONDSCORETYPE_ENTERORDER 1           //平分时，进入名次先的（首轮的进入先后即报名先后）

#define TK_ORDERSCOREFROM_SCORE 0     // 用Score排名(默认)
#define TK_ORDERSCOREFROM_WINSCORE 1  // 用WinScore排名

//游戏计分倍分方式(倍数)
#define TK_GAMESCOREMULTTYPE_ORIGINAL 1    //原游戏计分
#define TK_GAMESCOREMULTTYPE_WITHBASE 2    //原游戏计分*基数(不管用户积分大于还是小于基数)
#define TK_GAMESCOREMULTTYPE_MINORGBASE 3  //原游戏计分*min(基数，用户积分)

// 客户端带付费任务类型定义，与eDicPaymentBusinessTypeDef独立开
enum ePaymentTaskTypeDef
{
    TK_ENUM_PAYMENTTASK_TYPE_RESERVE = 0,

    TK_ENUM_PAYMENTTASK_TYPE_OLD = 1,  // 老业务兼容

    // Match
    TK_ENUM_PAYMENTTASK_TYPE_RELIFE = 1000,  // 复活
    TK_ENUM_PAYMENTTASK_TYPE_BLOOD = 1001,   // 加血

    // Game
    TK_ENUM_PAYMENTTASK_TYPE_EXPRESSION = 2000,  // 表情

    // 以上部分兼容老的扣费方式，映射参考eDicPaymentBusinessTypeDef

    // 以下部分与组委会扣费业务解耦，新定义类型
    TK_ENUM_PAYMENTTASK_TYPE_NEW_RESERVE = 10000,
    TK_ENUM_PAYMENTTASK_TYPE_NEW_TASKRELIVE = 10001,  // 任务复活（海捞）
};

enum eUserTypeDef
{
    //真实用户类别
    TK_USER_TYPE_REALUSER_NEW = 1,     //新手用户
    TK_USER_TYPE_REALUSER_HIGH = 2,    //异常用户
    TK_USER_TYPE_REALUSER_NORMAL = 3,  //普通用户

    //AI 用户类别（比赛定义）
    TK_USER_TYPE_PACERBOT_NEW = 50,           //新手陪玩AI
    TK_USER_TYPE_PACERBOT_HIGH = 51,          //异常用户陪玩AI
    TK_USER_TYPE_PACERBOT_TIMEOUT = 52,       //超时陪玩AI
    TK_USER_TYPE_PACERBOT_DUPLEX = 53,        //复式赛AI
    TK_USER_TYPE_PACERBOT_ENDGAME = 54,       //残局AI
    TK_USER_TYPE_PACERBOT_CANNONFODDER = 55,  //炮灰AI

    //AI 用户类别（游戏定义）
    TK_USER_TYPE_PACERBOT_TRUST = 100,  //托管代打机器人
};

typedef struct tagTKROUNDRULER
{
    int nType;                                        //局制类型 参见eDicRoundType定义
    char szRoundRulerName[TK_BUFLEN_ROUNDRULERNAME];  //局制名称
    char szRoundRulerNote[TK_BUFLEN_ROUNDRULERNOTE];  //游戏局制规则说明：N局积分最高者获胜 / 低于****分将被淘汰
    int nGameCount;                                   //总盘数	混战填0
    int nMinWinGameCount;                             //结束盘数，混战填0，和GameCount结合即成为n局n/2+1胜局制
    int nOvertime;                                    //总时长限制,0为无限制
} TKROUNDRULER, *PTKROUNDRULER;

typedef struct tagTKGAMERULER
{
    int nPlayersPerTable;  //游戏每桌人数

    char szProperty[TK_MAX_GAMEPROPLEN];      //游戏相关附加属性，描述游戏规则，游戏服务器自行解释，对复杂游戏可能会有
    char szPropertyEx[TK_MAX_GAMEPROPEXLEN];  //游戏相关附加扩展属性，描述游戏规则，游戏服务器自行解释，对复杂游戏可能会有

} TKGAMERULER, *PTKGAMERULER;

typedef struct tagTKTABLERULER
{
    int nScoreBase;  //初始游戏基数，比方400，只在混战中使用
    int nTax;        // 每局抽水

    char szProperty[TK_MAX_GAMEPROPLEN];
    char szPropertyEx[TK_MAX_GAMEPROPEXLEN];

    //桌上局制规则
    int nRoundType;                                   //局制类型
    char szRoundRulerName[TK_BUFLEN_ROUNDRULERNAME];  //局制名称
    char szRoundRulerNote[TK_BUFLEN_ROUNDRULERNOTE];  //游戏局制规则说明：N局积分最高者获胜 / 低于****分将被淘汰
    int nGameCount;                                   //总盘数	混战填0
    int nMinWinGameCount;                             //结束盘数，混战填0，和GameCount结合即成为n局n/2+1胜局制
    int nOvertime;                                    //总时长限制,0为无限制

} TKTABLERULER, *PTKTABLERULER;

typedef struct tagTKWATCHINFO
{
    DWORD dwWatcherID;  // 旁观者UID

    DWORD dwWatcheeID;     // 被旁观者UID
    DWORD dwWatcheeFlags;  //
    int nWatcheeSeat;      //
} TKWATCHINFO, *PTKWATCHINFO;

////////////////////////////////////////

#define TK_MATCHPLAYER_FLAGS_EXITEDMATACH 0x00000001           //确认退出了比赛
#define TK_MATCHPLAYER_FLAGS_LOBBYOFFLINE 0x00000002           //比赛开始时，用户大厅不在线
#define TK_MATCHPLAYER_FLAGS_NETBREAK 0x00000004               //比赛中，用户断线了
#define TK_MATCHPLAYER_FLAGS_HIDEINFO 0x00000008               //隐藏用户信息
#define TK_MATCHPLAYER_FLAGS_PREONLINE 0x00000010              //用户预检测在线（开始前T1(90秒)检测在线）
#define TK_MATCHPLAYER_FLAGS_BEFORESTARTONLINE 0x00000020      //用户开赛前检测在线（开始前T2(30秒)检测在线）
#define TK_MATCHPLAYER_FLAGS_ENTERMATCH 0x00000040             //用户进入比赛
#define TK_MATCHPLAYER_FLAGS_KICKOUTMATCH 0x00000080           //踢出用户
#define TK_MATCHPLAYER_FLAGS_ENABLEWATCH 0x00000100            //用户是否允许被旁观
#define TK_MATCHPLAYER_FLAGS_IDLEMARKED 0x00000200             //下局是否参与游戏(TableStage mode#2)
#define TK_MATCHPLAYER_FLAGS_BREAKOVERTIME 0x00000400          //断线超时
#define TK_MATCHPLAYER_FLAGS_CHGTABLEMARKED 0x00000800         //标记请求换桌
#define TK_MATCHPLAYER_FLAGS_EXITDIRMARKED 0x00001000          //标记直接退出了比赛（移动端退出新开比赛）
#define TK_MATCHPLAYER_FLAGS_ENTEROVERTIME 0x00002000          //进入比赛超时
#define TK_MATCHPLAYER_FLAGS_FORCEGROUP 0x00004000             //拉郎配强制重组
#define TK_MATCHPLAYER_FLAGS_UNPAYED 0x00008000                //用户未付费
#define TK_MATCHPLAYER_FLAGS_RESTRICTCONTINUEROUND 0x00010000  // 限制再来一局
#define TK_MATCHPLAYER_FLAGS_MATCHING 0x01000000               //还在比赛中
#define TK_MATCHPLAYER_FLAGS_MATCHOUT 0x02000000               //已经出局
#define TK_MATCHPLAYER_FLAGS_PROMOTION 0x04000000              //比赛确认晋级了
#define TK_MATCHPLAYER_FLAGS_WAITRESULT 0x08000000             //比赛完了，但不确定出局还是晋级
#define TK_MATCHPLAYER_FLAGS_NOTROUND 0x10000000               //轮空
#define TK_MATCHPLAYER_FLAGS_SWITCHMATCHSERVICE 0x20000000     //玩家切换比赛服务
#define TK_MATCHPLAYER_FLAGS_WATCHEE 0x40000000                //旁观者角色
#define TK_MATCHPLAYER_FLAGS_MATCHOVER 0x80000000              //比赛结束

//用户比赛状态定义
#define TK_MATCHPLAYERSTATUS_NULL 0            //
#define TK_MATCHPLAYERSTATUS_MATCHING 1        //比赛中
#define TK_MATCHPLAYERSTATUS_MATCHOVER 2       //比赛结束了
#define TK_MATCHPLAYERSTATUS_WAITING 3         //等待中
#define TK_MATCHPLAYERSTATUS_RESETCONTINUE 4   //处于休息中可以点击继续按钮继续比赛
#define TK_MATCHPLAYERSTATUS_RELIVECONTINEU 5  //出局了可以点击复活按钮继续比赛

////////////////////////////////////////////////////////////////////////////////
//
// 消息协议定义
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------------------------
//
// 创建MatchAgent
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CREATEMATCHAGENT (TKID_MS2GSBASE + 0x001)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQCREATEMATCHAGENT
{
    enum RecomLookOnFlag : DWORD
    {
        LOOKON_FLAG_ORDINARY = 0,     // 普通延迟旁观
        LOOKON_FLAG_RECOMMENDED = 1,  // 推荐延迟旁观
        LOOKON_FLAG_NEW_OB = 2,       // 新旁观(好友房，2V2)
    };

    TKHEADER header;
    DWORD dwTourneyID;
    char szMatchName[TK_BUFLEN_TOURNEYFULLNAME];  // 赛事名称
    DWORD dwMatchID;
    time_t timeMatchStart;
    TKADDRINFO stAddrMatchSrv;

    WORD wTeamType;  // 类型（1 = 个人赛，2 = pair赛  团体赛）参见eTourneyTeamType定义
    WORD wReserve1;

    int nGameID;
    DWORD dwReserve2;  // RecomLookOnFlag
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD adwReserve[60];

    int nTotalMatchPlayerCount;
    DWORD dwIPGS;
    int nPortGS;

    int nTourneyType;
    int nProductID;

    BOOL bEnableGCChat;  // 是否允许游戏者聊天
    DWORD dwChatSvrIP;   // 聊天服务器IP，注意是机器字节序
    DWORD dwChatSvrPort;
    DWORD dwBCChatUserType;

    BOOL bEnableBCChat;  //是否允许旁观者聊天

    BOOL bVipMode;
    int nBroadcastDelay;

    int nTitleStyle;          //比赛标题类型，0锦标赛显示为“比赛名称+阶段/轮次名称”，1自由桌显示为“第%d局”
    DWORD dwSeriesID;         //系列赛ID
    int nPlayersPerTable;     //每桌游戏人数
    BOOL bNewEnterMode;       //新的进入比赛方式
    BOOL bNoEnterWelcomeTip;  //进入比赛游戏服务时欢迎提示 wuqy add 2016.9.18
} TKREQCREATEMATCHAGENT, *PTKREQCREATEMATCHAGENT;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 增加比赛选手
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_ADDMATCHPLAYER (TKID_MS2GSBASE + 0x002)
typedef struct tagTKREQADDMATACHPLAYER
{
    TKHEADER header;
    int nOffsetInfo;  //=sizeof(TKREQADDMATACHPLAYER)
    DWORD dwMatchID;

    int cnMatchPlayer;  //后跟cnMatchPlayer个TKADDGSMATCHPLAYERINFO
    int nSizeofInfo;    //=sizeof(TKADDGSMATCHPLAYERINFO)
} TKREQADDMATACHPLAYER, *PTKREQADDMATACHPLAYER;

typedef struct tagTKADDGSMATCHPLAYERINFO
{
    tagTKADDGSMATCHPLAYERINFO()
    {
        memset(this, 0, sizeof(*this));
    }

    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];

    DWORD dwCheck;
    WORD wCntTID;  //大厅客户端链接类型 eLobbyConnetType
    BYTE byOSType;
    BYTE byPacerBotLevel;  //  陪打员动态设置Level 注意是BYTE的（原来这个字段名字为：byReserve1 保留字段）
    ULONGLONG ullMac;

    DWORD dwTeamID;

    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwMoneyAccType;

    int nFigureID;       //用户头像ID
    DWORD dwFormID;      //用户形象ID
    DWORD dwLanguageID;  //客户端语言版本ID
    BOOL bPacerBot;      // 高级AI仿真人
    DWORD dwAIversion;   // 使用的AI版本号
    int nDomain;
    DWORD dwCrowFundOri;  //1表示众筹来源玩家，0表示非众筹来源玩家（默认值）
} TKADDGSMATCHPLAYERINFO, *PTKADDGSMATCHPLAYERINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// 删除MatchAgent
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_DELETEMATCHAGENT (TKID_MS2GSBASE + 0x003)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQDELETEMATCHAGENT
{
    TKHEADER header;
    DWORD dwMatchID;
    DWORD dwReason;  //结束原因:???
} TKREQDELETEMATCHAGENT, *PTKREQDELETEMATCHAGENT;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 检查MatchAgent是否有效
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CHECKMATCHAGENT (TKID_MS2GSBASE + 0x004)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQCHECKMATCHAGENT
{
    TKHEADER header;
    DWORD dwMatchID;
} TKREQCHECKMATCHAGENT, *PTKREQCHECKMATCHAGENT;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS比赛当前进行到的阶段和轮次
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHSTAGEPOSITION (TKID_MS2GSBASE + 0x006)
typedef struct tagTKREQMATCHSTAGEPOSITION
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int nCurBoutID;
    char szStageName[TK_BUFLEN_STAGENAME];
    char szBoutName[TK_BUFLEN_STAGENAME];
} TKREQMATCHSTAGEPOSITION, *PTKREQMATCHSTAGEPOSITION;

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户强行退出后又回来
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHPLAYERRETURN (TKID_MS2GSBASE + 0x007)
typedef struct tagTKREQMATCHPLAYERRETURN
{
    TKHEADER header;
    DWORD dwMatchID;
    DWORD dwUserID;
} TKREQMATCHPLAYERRETURN, *PTKREQMATCHPLAYERRETURN;

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS比赛当前激战人数
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CURMATCHPLAYERCOUNT (TKID_MS2GSBASE + 0x008)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQCURMATCHPLAYERCOUNT
{
    TKHEADER header;

    DWORD dwMatchID;
    WORD wStageID;

    int nCurMatchPlayerCount;
} TKREQCURMATCHPLAYERCOUNT, *PTKREQCURMATCHPLAYERCOUNT;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS比赛用户颁奖信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHAWARD (TKID_MS2GSBASE + 0x009)
typedef struct tagTKREQMATCHTAWARD
{
    TKHEADER header;
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwPushAwardFlags;
    int lenTextAward;
} TKREQMATCHTAWARD, *PTKREQMATCHTAWARD;
#define TK_PUSHAWARDFLAG_EXITCLIENT 0x00000001  //退出比赛客户端
#define TK_PUSHAWARDFLAG_NOTEXPAND 0x00010000   //不展开显示奖状

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS该阶段用户需要切换GS服务器IP，不要处理断线
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHTPLAYERCHANGEGS (TKID_MS2GSBASE + 0x00B)
typedef struct tagTKREQMATCHTPLAYERCHANGEGS
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
} TKREQMATCHTPLAYERCHANGEGS, *PTKREQMATCHTPLAYERCHANGEGS;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS用户进入比赛
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USERENTERMATCH (TKID_MS2GSBASE + 0x00C)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQUSERENTERMATCH
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    DWORD dwUserID;
    int nFigureID;
    int nOpertor;  //操作者,参见eDicGamePlayerOperator定义
} TKREQUSERENTERMATCH, *PTKREQUSERENTERMATCH;
//ACK : TKHEADER

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS用户断线
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_ADDROUNDBREAKUSER (TKID_MS2GSBASE + 0x00D)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQADDROUNDBREAKUSER
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;

    DWORD dwReserve;
    int nReserve;
    BOOL bInvoke;  //游戏超时，疑似断线，通知大厅重启一下GC
} TKREQADDROUNDBREAKUSER, *PTKREQADDROUNDBREAKUSER;
//ACK : TKHEADER

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户断线回来了，GS通知MS用户清除断线记录
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_CLEANROUNDBREAKUSER (TKID_MS2GSBASE + 0x00E)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQCLEANROUNDBREAKUSER
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;
} TKREQCLEANROUNDBREAKUSER, *PTKREQCLEANROUNDBREAKUSER;
//ACK : TKHEADER

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户退出比赛
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USEREXITMATCH (TKID_MS2GSBASE + 0x00F)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQUSEREXITMATCH
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
    BOOL bDirExit;
} TKREQUSEREXITMATCH, *PTKREQUSEREXITMATCH;
//ACK : TKHEADER

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS删掉用户
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_DELMATCHPLAYER (TKID_MS2GSBASE + 0x010)
typedef struct tagTKREQDELMATCHPLAYER
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwMatchID;
    WORD wStageID;
} TKREQDELMATCHPLAYER, *PTKREQDELMATCHPLAYER;

//-------------------------------------------------------------------------------------------------------------------------
//
// 每30秒MS检测GS是否故障
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_GSIDLE (TKID_MS2GSBASE + 0x011)

//-------------------------------------------------------------------------------------------------------------------------
//
// WebGuest取得成绩，通知注册
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_NOTIFY_GUESTREGISTER_MANUAL (TKID_MS2GSBASE + 0x012)
typedef struct tagTKMATCHNOTIFYGUESTREGISTERMANUAL
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwWebGuestID;

    DWORD dwSuffixOffSet;  // 后缀提示文字字符串偏移量，相对于消息缓冲区顶端
    DWORD dwSuffixSize;    // 后缀提示文字字符串大小，包含结束符\0
} TKMATCHNOTIFYGUESTREGISTERMANUAL, *PTKMATCHNOTIFYGUESTREGISTERMANUAL;
//后跟提示文字

//-------------------------------------------------------------------------------------------------------------------------
//
// WebGuest注册请求
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_WEBREGISTER_MANUAL (TKID_MS2GSBASE + 0x013)
typedef struct tagTKREQWEBGUESTMATCHREGISTERMANUAL
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwWebGuestID;

    char szLoginName[TK_BUFLEN_USERNAME];
    char szNickName[TK_BUFLEN_NICKNAME];
    char szPassWord[TK_BUFLEN_PASSWORD];
    char szIP[TK_BUFLEN_IP];
    WORD wFigureID;  // 头像ID

    DWORD dwLcVer;  // 客户端版本号

    // 增加注册代码
    WORD wAgent;  // 注册来源

    // 推广员
    DWORD dwPromoterID;                           // 推广员ID
    char szPromoterNickName[TK_BUFLEN_NICKNAME];  // 推广员昵称

    // 新手卡
    char szNewPlayerCard[TK_BUFLEN_NEWPLAYERCARD];  // 新手卡号

    DWORD dwSuffixOffSet;  // 后缀XML字符串数据偏移量，相对于消息缓冲区顶端，用于协议兼容
    DWORD dwSuffixSize;    // 后缀XML字符串数据大小，，包含结束符\0

    // 保留
    WORD wStageID;
    WORD wReserve;
    DWORD dwParam2;  // 保留参数2
    DWORD dwParam3;  // 保留参数3

    char szCltIdent[TK_BUFLEN_NOTE];  // 客户端唯一标识
    DWORD dwCustomID;                 // 推广客户ID

} TKREQWEBGUESTMATCHREGISTERMANUAL, *PTKREQWEBGUESTMATCHREGISTERMANUAL;
// 请求后缀数据：
//     Web传过来的XML字符串数据
typedef struct tagTKACKWEBGUESTMATCHREGISTERMANUAL
{
    TKHEADER header;
    DWORD dwUserID;  //注册成功后的UserID
    char szNickName[TK_BUFLEN_NICKNAME];

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwWebGuestID;
} TKACKWEBGUESTMATCHREGISTERMANUAL, *PTKACKWEBGUESTMATCHREGISTERMANUAL;

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS比赛用户的游戏养成数值
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_SETPLAYERGAMEGROWINFO (TKID_MS2GSBASE + 0x014)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQSETPLAYERGAMEGROWINFO
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    DWORD dwUserID;

    int cnGameGrowInfo;  //
} TKREQSETPLAYERGAMEGROWINFO, *PTKREQSETPLAYERGAMEGROWINFO;
//后跟cnGameGrowInfo个GameGrowInfo(DWORD GrowID + DWORD dwGrowValue)
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS比赛用户的游戏养成数值有变更需要保存
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_SAVEUSERGROWVALUE (TKID_MS2GSBASE + 0x015)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQSAVEUSERGROWVALUE
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    DWORD dwUserID;

    int cnGameGrowInfo;  //
} TKREQSAVEUSERGROWVALUE, *PTKREQSAVEUSERGROWVALUE;
//后跟cnGameGrowInfo个GameGrowInfo(DWORD GrowID + DWORD dwGrowValue) dwGrowValue为变更的差值
//ACK:MatchServer -->GameServer  (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS比赛变更用户在比赛中的游戏等级数值
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_UPDATEPLAYERMATCHGAMERANK (TKID_MS2GSBASE + 0x016)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQUPDATEPLAYERMATCHGAMERANK
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    DWORD dwUserID;
    int nRank;  //
} TKREQUPDATEPLAYERMATCHGAMERANK, *PTKREQUPDATEPLAYERMATCHGAMERANK;

//-------------------------------------------------------------------------------------------------------------------------
//
// 将指定用户踢开比赛
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_KICKOUTMATCH (TKID_MS2GSBASE + 0x017)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQMS2GSKICKOUTMATCH
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    DWORD dwUserID;
} TKREQMS2GSKICKOUTMATCH, *PTKREQMS2GSKICKOUTMATCH;

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS比赛变更用户在比赛中的养成数值
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_UPDATEPLAYERGROWVALUE (TKID_MS2GSBASE + 0x018)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQUPDATEPLAYERGROWVALUE
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    DWORD dwUserID;

    int cnGameGrowInfo;  //
} TKREQUPDATEPLAYERGROWVALUE, *PTKREQUPDATEPLAYERGROWVALUE;
//后跟cnGameGrowInfo个GameGrowInfo(DWORD GrowID + DWORD dwGrowValue) dwGrowValue为变更后的值
//ACK:MatchServer -->GameServer  (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS：用户在游戏中发生了原子游戏事件
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_SAVEGAMEATOMEVENT (TKID_MS2GSBASE + 0x019)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQSAVEGAMEATOMEVENT
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    DWORD dwUserID;

    int cnAtomEvent;
} TKREQSAVEGAMEATOMEVENT, *PTKREQSAVEGAMEATOMEVENT;
//后跟cnAtomEvent个TKGAMEATOMEVENT(DWORD dwEventID + DWORD dwEventValue)
//ACK:MatchServer -->GameServer  (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS：用户在游戏中发生了原子游戏事件
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_SAVEGAMEATOMEVENTEX (TKID_MS2GSBASE + 0x01A)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQSAVEGAMEATOMEVENTEX
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    DWORD dwUserID;
    DWORD dwAppendOffset;  //=sizeof(TKREQSAVEGAMEATOMEVENTEX)
    int cnAtomEvent;

    WORD wCntTID;
    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwGrowAccType;

} TKREQSAVEGAMEATOMEVENTEX, *PTKREQSAVEGAMEATOMEVENTEX;
//后跟cnAtomEvent个TKGAMEATOMEVENT(DWORD dwEventID + DWORD dwEventValue)
//ACK:MatchServer -->GameServer  (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS删掉Stage用户
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_DELSTAGEPLAYER (TKID_MS2GSBASE + 0x01B)
typedef struct tagTKREQDELSTAGEPLAYER
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
} TKREQDELSTAGEPLAYER, *PTKREQDELSTAGEPLAYER;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS用户加入游戏
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_PLAYERENTERGAME (TKID_MS2GSBASE + 0x01C)
typedef struct tagTKREQPLAYERENTERGAME
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];
    int nEnterOrder;
    int nPlaceOrder;

    int nMatchScore;     // 比赛积分
    int nMatchSecScore;  // 比赛小分
    int nRoundScore;     // 局制积分
    int nRoundSecScore;  // 局制小分

    int nGameScore;  // 游戏积分

    int cnGameCurFinished;  // 当前完成的总盘数
    DWORD dwFlags;          // 一些标志 0位表示是否确认退出了比赛
    int nSeatOrder;
    int cnGameTotFinished;  // 比赛完成的总盘数
    int iCurGameHand;       // 用户当前进行的局制编号

    BYTE byBreakHeartTask;  // 是否参与了碎心任务 wuqy 2016.6.3
    BYTE byHeartCount;      // 用户的HeartCount，即比赛中用户命的条数（cnMatchLive）wuqy add 2016.12.31
    BYTE byGameResult;      // 上局游戏结果
    BYTE byGameActor;       // 上局游戏角色
    DWORD dwTeamID;         // 玩家的队伍编号
    DWORD dwCaptionID;      // 玩家队伍的队长ID（萌球）
    DWORD dwReserve3;
    DWORD dwReserve4;
} TKREQPLAYERENTERGAME, *PTKREQPLAYERENTERGAME;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS用户加入游戏的结果
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_PLAYERENTERGAMERESULT (TKID_MS2GSBASE + 0x01D)
typedef struct tagTKREQPLAYERENTERGAMERESULT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;

} TKREQPLAYERENTERGAMERESULT, *PTKREQPLAYERENTERGAMERESULT;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS用户加入游戏
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_PLAYERLEAVEGAME (TKID_MS2GSBASE + 0x01E)
typedef struct tagTKREQPLAYERLEAVEGAME
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;
    DWORD dwReason;  //离开原因 参见 TKMatchUniversalDicSrv.h 文件 eDynLeaveGameReason定义

    DWORD dwWatcheeID;
    DWORD dwWatcheeFlags;
    int nWatcheeSeat;

} TKREQPLAYERLEAVEGAME, *PTKREQPLAYERLEAVEGAME;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS用户加入游戏的结果
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_PLAYERLEAVEGAMERESULT (TKID_MS2GSBASE + 0x01F)
typedef struct tagTKREQPLAYERLEAVEGAMERESULT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;

    int nResultScore;     // 结果分：积分或筹码
    int nMatchTax;        // 本局制比赛扣费
    int nLastHandScore;   // 本局最后一盘游戏得分
    int nRoundScoreBase;  // 局制结束时的基数

    int nGSMatchScore;  // 比赛积分(GS可能调整过的)wuqy add 2015.3.27

    DWORD dwReason;  // 离开原因 wuqy add 2015.7.7

    WORD wGameResult;  // 0 == 未知， 1 == 胜利， 2 == 和， 3 == 负
    WORD wGameActor;   // 游戏规则定义的本局游戏角色，数值由每个游戏自己定义，在特定的Stage重组时使用 wuqy 2016.12.31
} TKREQPLAYERLEAVEGAMERESULT, *PTKREQPLAYERLEAVEGAMERESULT;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS用户补充了多少血
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_PLAYERADDHEMATINIC (TKID_MS2GSBASE + 0x020)
typedef struct tagTKREQPLAYERADDHEMATINIC
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;

    int nHematinic;

} TKREQPLAYERADDHEMATINIC, *PTKREQPLAYERADDHEMATINIC;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS用户补血的结果
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_PLAYERADDHEMATINICRESULT (TKID_MS2GSBASE + 0x021)
typedef struct tagTKREQPLAYERADDHEMATINICRESULT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;

    int nHematinic;

} TKREQPLAYERADDHEMATINICRESULT, *PTKREQPLAYERADDHEMATINICRESULT;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS给MS发送更新Round积分
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_UPDATEROUNDSCORE (TKID_MS2GSBASE + 0x022)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQUPDATEROUNDSCORE
{
    TKHEADER header;

    WORD wResultOffset;  //=sizeof(TKREQUPDATEROUNDSCORE)
    WORD wResultCount;
    WORD wResultSize;  //=sizeof(ROUNDROUNDSCOREINFO)
    WORD wReserve;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    module_identify id;

    int nRoundScoreBase;
} TKREQUPDATEROUNDSCORE, *PTKREQUPDATEROUNDSCORE;
typedef struct tagUPDATEROUNDSCOREINFO
{
    DWORD dwUserID;
    DWORD dwFlags;

    int nResultScore;   // 结果分：积分或筹码
    int nGSMatchScore;  // 比赛积分(GS可能调整过的)
} UPDATEROUNDSCOREINFO, *PUPDATEROUNDSCOREINFO;
//ACK : TKHEADER

//-------------------------------------------------------------------------------------------------------------------------
//
// 设置用户MsTrace标志
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_SETUSERMATCHTRACEFLAGS (TKID_MS2GSBASE + 0x023)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQMS2GSSETUSERMATCHTRACEFLAGS
{
    TKHEADER header;

    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwMsTraceFlags;
} TKREQMS2GSSETUSERMATCHTRACEFLAGS, *PTKREQMS2GSSETUSERMATCHTRACEFLAGS;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS让玩家处于休息（64位）
// 说明:同功能的32位协议TKID_MS2GS_GAME_REST
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_GAME_REST_64 (TKID_MS2GSBASE + 0x024)
typedef struct tagTKMSREQPLAYERREST_64
{
    TKHEADER header;
    uint32_t dwUserID;
    uint32_t dwMatchID;
    uint16_t wStageID;

    //
    int32_t nRestTime;          // 可休息时间,到点后如果玩家不点继续按钮游戏将自动开始
    int64_t i64Life;            // 当前生命值
    int64_t i64Coin;            // 可回兑金币数
    int32_t cnGames;            // 已完成牌副数
    int64_t i64Multi;           // 下一局的积分倍数
    int32_t nNextLevelGames;    // 下一级所需牌副数
    int64_t i64NextLevelMulti;  // 下一级积分倍数
    uint64_t u64ExChangeRate;
    uint64_t u64ExChangeRateDenominator;
    int32_t nAwardTimeSpan;        //颁奖时间间隔(秒),<=0表示不颁奖
    int32_t nNextAwardLeftSecond;  //距离下一次颁奖的剩余时间(秒)
    int64_t i64Score;              // 积分
    int64_t i64LastHandScore;      // 最后一局得分
    uint16_t wGameResult;          // 游戏结果，0 == 未知， 1 == 胜利， 2 == 和， 3 == 负

    //
    uint32_t dwSufOffset;  //后缀数据偏移
    uint32_t dwSufSize;    //后缀数据大小
    int64_t i64Reserve1;   //保留1
    uint32_t dwReserve2;   //保留2
    uint32_t dwReserve3;   //保留3
} TKMSREQPLAYERREST_64, *PTKMSREQPLAYERREST_64;
//后缀血瓶json，{"Plan":[{"Blod":4444,"Cost":4444,"Discnt":100},{"Blod":333,"Cost":333,"Discnt":100}]}

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知客户端加血方案（64位）
// 说明:同功能的32位协议TKID_MS2GS_EXCHANGE
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_EXCHANGE_64 (TKID_MS2GSBASE + 0x025)
typedef struct tagTKMSREQEXCHANGE_64
{
    TKHEADER header;
    uint32_t dwUserID;
    uint32_t dwMPID;
    uint32_t dwTourneyID;
    uint32_t dwMatchID;
    uint16_t wStageID;

    //
    uint64_t u64ExChangeRate;
    uint64_t u64ExChangeRateDenominator;
    uint32_t dwSufOffset;  //后缀数据偏移
    uint32_t dwSufSize;    //后缀数据大小
    int64_t i64Reserve1;   //保留1
    uint32_t dwReserve2;   //保留2
    uint32_t dwReserve3;   //保留3
} TKMSREQEXCHANGE_64, *PTKMSREQEXCHANGE_64;
//后缀血瓶json，{"Plan":[{"Blod":4444,"Cost":4444,"Discnt":100},{"Blod":333,"Cost":333,"Discnt":100}]}

//-------------------------------------------------------------------------------------------------------------------------
//
// 创建StageAgent
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CREATESTAGEAGENT (TKID_MS2GSBASE + 0x032)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQCREATESTAGEAGENT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    int nStageType;
    DWORD dwSeriesID;  //系列赛ID
    module_identify module_id;

} TKREQCREATESTAGEAGENT, *PTKREQCREATESTAGEAGENT;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 设置Stage规则
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_SETSTAGERULER (TKID_MS2GSBASE + 0x033)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQSETSTAGERULER
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    int nRulerType;
    int nRulerLength;  //结构体后跟规则数据长度
} TKREQSETSTAGERULER, *PTKREQSETSTAGERULER;
#define TK_STAGERULER_STAGE 0  // Stage规则
#define TK_STAGERULER_ROUND 1  // Round规则
#define TK_STAGERULER_GAME 2   // Game规则
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 增加Stage选手
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_ADDSTAGEPLAYER (TKID_MS2GSBASE + 0x034)
typedef struct tagTKREQADDSTAGEPLAYER
{
    TKHEADER header;
    int nOffsetInfo;  //=sizeof(TKREQADDSTAGEPLAYER)
    DWORD dwMatchID;
    WORD wStageID;

    int cnStagePlayer;  //后跟cnStagePlayer个TKADDGSSTAGEPLAYERINFO
    int nSizeofInfo;    //=sizeof(TKADDGSSTAGEPLAYERINFO)
} TKREQADDSTAGEPLAYER, *PTKREQADDSTAGEPLAYER;
typedef struct tagTKADDGSSTAGEPLAYERINFO
{
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];

    int nEnterOrder;
    int nPlaceOrder;

    int nMatchScore;     // 比赛积分
    int nMatchSecScore;  // 比赛小分
    int nRoundScore;     // 局制积分
    int nRoundSecScore;  // 局制小分

    int nGameScore;  // 游戏积分

    int cnGameCurFinished;  // 当前完成的总盘数
    DWORD dwFlags;          // 一些标志 0位表示是否确认退出了比赛
    int cnGameTotFinished;  // 比赛完成的总盘数
    int iCurGameHand;       // 用户当前进行的局制编号
    int nDomain;
} TKADDGSSTAGEPLAYERINFO, *PTKADDGSSTAGEPLAYERINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// 删除StageAgent
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_DELETESTAGEAGENT (TKID_MS2GSBASE + 0x035)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQDELETESTAGEAGENT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwReason;  //结束原因:???
} TKREQDELETESTAGEAGENT, *PTKREQDELETESTAGEAGENT;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 创建Round
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CREATEROUND (TKID_MS2GSBASE + 0x036)
#define ROUND_SPECIAL_RULES_ALLIN 0x00000001         // 强制自动Allin liusj 2017.6.21
#define ROUND_SPECIAL_RULES_LUAVIPSORT 0x00000002    // 启用Lua计算用户VIP权重值，用于明星桌展示 wuqy add 2017.7.7
#define ROUND_SPECIAL_RULES_BYPASS 0x00000004        // 越级挑战对局 hexu add 2017.8.14
#define ROUND_SPECIAL_RULES_CRS 0x00000008           // 牌库：收集标志added by wanghongtao 2017-09-04
#define ROUND_SPECIAL_RULES_CRS_USING 0x00000010     // 牌库：使用标志added by wanghongtao 2017-11-13
#define ROUND_SPECIAL_RULES_SUPER_BANKER 0x00000020  // 超级庄家桌added by liusj 2017-12-26
#define ROUND_SPECIAL_RULES_CRS_COMPOUND 0x00000040  // 牌库：复式牌库标志

//REQ:MatchServer --> GameServer
typedef struct tagTKREQCREATEROUND_Old
{
    TKHEADER header;

    int nOffsetAppendInfo;  // =sizeof(TKREQCREATEROUND)

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    int nBout;   // 第几轮( 瑞士移位 )
    int nTable;  // 第几桌( 瑞士移位 )

    int nDealMode;  // 发牌方式 0 == 随机发牌， 1 == 指定牌号，此时nDealParam为牌号, 2 == 按照牌组、牌ID取牌，此时nDealParam为牌组ID，iGameHandBegin+1为牌ID,3 == 按关系ID取牌，此时nDealParam为关系ID
    int nDealParam;

    TKMATCHACTION stActRoundBegin;  // Round开始的Action
    TKMATCHACTION stActRoundOver;   // Round结束的Action
    TKMATCHACTION stActGameBegin;   // Game开始的Action
    TKMATCHACTION stActGameOver;    // Game结束的Action

    int cnUserID;           // 结构体后跟cnUserID个 TKADDGSROUNDPLAYERINFO
    int nSizeofPlayerInfo;  // =sizeof(TKADDGSROUNDPLAYERINFO)

    int nFinishedHand;         // 已经完成的局数
    int iGameHandBegin;        // 从第几局开始打
    int nLastHandSaveInfoLen;  // 上局保存数据

    int nSizeofTableRuler;  // sizeof(TKTABLERULER),如果为0表示没有

    DWORD dwMatchRoundIdx;  // 该dwMatchID下的总Round索引号
    module_identify id;
    BOOL bNeedClearTable;  // 游戏结束时是否需要清桌 wuqy add 2013.8.12
    WORD wActionID;        // ActionID wuqy add 2015.9.23
    WORD wActionLength;    // Action的XML字符串长度，包含结束符'\0' wuqy add 2015.9.23

    int cnWatcher;
    int nSizeofWatchInfo;

    DWORD dwSpecialRules;  // 由比赛性质决定的一些特殊规则( 32bit )，如强制allin

} TKREQCREATEROUND_OLD, *PTKREQCREATEROUND_OLD;
typedef struct tagTKREQCREATEROUND : public tagTKREQCREATEROUND_Old
{
    DWORD dwLimitingTime;  // 限定时间（1V1复式赛，限定时间内玩家思考不限时）
    WORD wReserve1;        // 预留 hexu add 2017.8.8
    WORD wReserve2;
    WORD wReserve3;
    WORD wReserve4;
    DWORD dwMsIP;
    DWORD dwMsPort;
    DWORD dwReserve3;
    DWORD dwReserve4;
} TKREQCREATEROUND, *PTKREQCREATEROUND;
//后跟cnUserID*TKADDGSROUNDPLAYERINFO+nLastHandSaveInfoLen+TKTABLERULER+ActionXML+cnWatcher*TKWATCHINFO
//ACK:GameServer --> MatchServer (消息包TKHEADER)
typedef struct tagTKADDGSROUNDPLAYERINFO
{
    DWORD dwUserID;

    DWORD dwReserve1;
    DWORD dwTeamID;
    DWORD dwCaptainID;      //团体赛的队长id
    BYTE byBreakHeartTask;  // 是否参与了碎心任务 wuqy 2016.6.3
    BYTE byHeartCount;      // 用户的HeartCount，即比赛中用户命的条数（cnMatchLive）wuqy add 2016.12.31
    BYTE byGameResult;      // 上局游戏结果
    BYTE byGameActor;       // 上局游戏角色
    DWORD dwMsTraceFlags;

    int nEnterOrder;
    int nPlaceOrder;

    int nMatchScore;     // 比赛积分
    int nMatchSecScore;  // 比赛小分
    int nRoundScore;     // 局制积分
    int nRoundSecScore;  // 局制小分

    int nGameScore;  // 游戏积分

    int cnGameCurFinished;  // 当前完成的总盘数
    DWORD dwFlags;          // 一些标志 0位表示是否确认退出了比赛
    int nSeatOrder;
    int cnGameTotFinished;  // 比赛完成的总盘数
    int iCurGameHand;       // 用户当前进行的局制编号
    BYTE byUserTypeTag;     // 用户标签值
    BYTE wReserve1;
    WORD wDomain;     //BYTE	wReserve2; //BYTE	wReserve3;
    DWORD dwRLBRank;  //玩家排行榜排名
    DWORD dwRecomLookOner;
} TKADDGSROUNDPLAYERINFO, *PTKADDGSROUNDPLAYERINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// 删除Round
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_DELETEROUND (TKID_MS2GSBASE + 0x037)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQDELETEROUND
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwReason;  //结束原因:???
} TKREQDELETEROUND, *PTKREQDELETEROUND;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 基数增长
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_SCOREBASERAISE (TKID_MS2GSBASE + 0x038)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQSCOREBASERAISE
{
    TKHEADER header;

    DWORD dwMatchID;
    WORD wStageID;

    int nNewScoreBase;
} TKREQSCOREBASERAISE, *PTKREQSCOREBASERAISE;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 所有Round结束当前Game后直接结束Round
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_STOPROUNDGAMEFORREGROUP (TKID_MS2GSBASE + 0x039)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQSTOPROUNDGAMEFORREGROUP
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    char szTipMessage[TK_TIPMESSAGE_MAXLEN];
    int nTipID;
} TKREQSTOPROUNDGAMEFORREGROUP, *PTKREQSTOPROUNDGAMEFORREGROUP;

//-------------------------------------------------------------------------------------------------------------------------
//
// 调整异常用户识别概率
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_SETTRACEDUSERRESHCHANCE (TKID_MS2GSBASE + 0x03A)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQSETTRACEDUSERRESHCHANCE
{
    TKHEADER header;

    DWORD dwMatchID;
    WORD wStageID;

    int nTracedUserReShChance;
} TKREQSETTRACEDUSERRESHCHANCE, *PTKREQSETTRACEDUSERRESHCHANCE;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// GS给MS发送Round结果
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_ROUNDRESULT (TKID_MS2GSBASE + 0x03B)
enum Result
{
    TK_UNKNOWN,
    TK_WIN,
    TK_DRAW,
    TK_LOSS
};
//REQ:GameServer --> MatchServer
typedef struct tagTKREQROUNDRESULT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wOverType;
    WORD wOverFlags;
    int nRoundScoreBase;

    WORD wResultCount;
    WORD wResultSize;    //=sizeof(ROUNDRESULTINFO)
    WORD wResultOffset;  //=sizeof(TKREQROUNDRESULT)
    WORD wReserve;

    int iGameHandOver;         //从第几局开始打
    int nLastHandSaveInfoLen;  //上局保存数据
    int cnGameCreate;          //本局制局数
    int nMatchTax;             //本局制比赛扣费（生命值数值）
    module_identify id;
} TKREQROUNDRESULT, *PTKREQROUNDRESULT;
typedef struct tagROUNDRESULTINFO
{
    DWORD dwUserID;
    DWORD dwFlags;
    WORD wGameResult;     // 0 == 未知， 1 == 胜利， 2 == 和， 3 == 负
    WORD wGameActor;      // 游戏规则定义的本局游戏角色，数值由每个游戏自己定义，在特定的Stage重组时使用 wuqy 2016.12.31
    int nResultScore;     // 结果分：积分或筹码
    int nLastHandScore;   // 最后一盘得分
    int nRoundWinScore;   // 本局累计赢得的积分
    int nRoundGameCount;  // 本局累计游戏盘数
    int nGSMatchScore;    // 比赛积分(GS可能调整过的)wuqy add 2015.3.27
} ROUNDRESULTINFO, *PROUNDRESULTINFO;
//ACK : TKHEADER

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS返回结果处理后的排名信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_ROUNDRESULTORDER (TKID_MS2GSBASE + 0x03C)
//ACK:MatchServer --> GameServer
typedef struct tagROUNDRESULTORDER
{
    DWORD dwUserID;
    WORD wOrder;
    WORD wOutRoundState;

    int nMatchScore;     // 比赛积分
    int nMatchSecScore;  // 比赛小分
    int nRoundScore;     // 局制积分
    int nRoundSecScore;  // 局制小分
} ROUNDRESULTORDER, *PROUNDRESULTORDER;
typedef struct tagTKREQROUNDRESULTORDER
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    int cnUserOrder;
    ROUNDRESULTORDER astUserOrder[TK_MAX_ROUNDPLAYER];
} TKREQROUNDRESULTORDER, *PTKREQROUNDRESULTORDER;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS返回结果处理后的用户状态信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_PLAYERROUNDORDERSTATE (TKID_MS2GSBASE + 0x03D)
//ACK:MatchServer --> GameServer
typedef struct tagTKREQPLAYERROUNDORDERSTATE
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int cnPlayer;
} TKREQPLAYERROUNDORDERSTATE, *PTKREQPLAYERROUNDORDERSTATE;
//后缀cnPlayer个ROUNDRESULTORDER

//-------------------------------------------------------------------------------------------------------------------------
//
// 更新岛屿休息中玩家的积分
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_UPDATERESTPLAYERMATCHSCORE (TKID_MS2GSBASE + 0x040)
typedef struct tagRESTPLAYERWINSCORE
{
    DWORD dwUserID;
    int nMatchScore;
} RESTPLAYERWINSCORE, *PRESTPLAYERWINSCORE;
typedef struct tagTKREQUPDATERESTPLAYERWINSCORE
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int cnRestPlayer;
} TKREQUPDATERESTPLAYERWINSCORE, *PTKREQUPDATERESTPLAYERWINSCORE;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS让玩家处于休息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_GAME_REST (TKID_MS2GSBASE + 0x041)
typedef struct tagTKMSREQPLAYERREST
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwMatchID;
    WORD wStageID;
    int nRestTime;                               // 可休息时间,到点后如果玩家不点继续按钮游戏将自动开始
    int cnHematinic;                             // 可选的血瓶类型
    HEMATINIC asHematinic[MAX_HEMATINIC_COUNT];  // 每种血瓶的详细信息

    int nLife;            // 当前生命值
    int nCoin;            // 可回兑金币数
    int cnGames;          // 已完成牌副数
    int nMulti;           // 下一局的积分倍数
    int nNextLevelGames;  // 下一级所需牌副数
    int nNextLevelMulti;  // 下一级积分倍数
    int nExchangeRate;

    int nAwardTimeSpan;        //颁奖时间间隔(秒),<=0表示不颁奖
    int nNextAwardLeftSecond;  //距离下一次颁奖的剩余时间(秒)
    int nScore;                // 积分

    int nLastHandScore;  // 最后一局得分
    WORD wGameResult;    // 游戏结果，0 == 未知， 1 == 胜利， 2 == 和， 3 == 负
} TKMSREQPLAYERREST, *PTKMSREQPLAYERREST;

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户请求继续游戏
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USERREQCONTINUE (TKID_MS2GSBASE + 0x044)
typedef struct tagTKUSERREQCONTINUE
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
    int nExtraTask;  // 附加任务, 0=无, 1=碎心挑战任务, 其他未定义        wuqy add 2016.5.30
} TKUSERREQCONTINUE, *PTKUSERREQCONTINUE;
typedef struct tagTKUSERACKCONTINUE : public tagTKUSERREQCONTINUE
{
} TKUSERACKCONTINUE, *PTKUSERACKCONTINUE;

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户请求离岛
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USERREQLEAVE (TKID_MS2GSBASE + 0x045)
typedef struct tagTKUSERREQLEAVE
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
    DWORD dwNoExchange;   // 不兑换金币，0-表示兑换金币，1表示不兑换。默认值为0，兼容线上版本。
    DWORD dwSaveArchive;  // 数据存档，0-表示不存档，1表示存档。默认值为1
} TKUSERREQLEAVE, *PTKUSERREQLEAVE;
typedef struct tagTKUSERACKLEAVE : public tagTKUSERREQLEAVE
{
} TKUSERACKLEAVE, *PTKUSERACKLEAVE;

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户请求补血
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USERREQHEMATINIC (TKID_MS2GSBASE + 0x046)
typedef struct tagTKUSERREQHEMATINIC
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
    int nHematinic;  // 补血量
} TKUSERREQHEMATINIC, *PTKUSERREQHEMATINIC;
typedef struct tagTKUSERACKHEMATINIC : public tagTKUSERREQHEMATINIC
{
} TKUSERACKHEMATINIC, *PTKUSERACKHEMATINIC;

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户请求复活
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USERREQRELIVE (TKID_MS2GSBASE + 0x047)
typedef struct tagTKUSERREQRELIVE
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
    int nSecondUsed;
    BOOL bRelive;
} TKUSERREQRELIVE, *PTKUSERREQRELIVE;
typedef struct tagTKUSERACKRELIVE : public tagTKUSERREQRELIVE
{
} TKUSERACKRELIVE, *PTKUSERACKRELIVE;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS排名信息变化
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_STAGEPLACEORDERCHANGED (TKID_MS2GSBASE + 0x048)
typedef struct tagTKREQSTAGEPLACEORDERCHANGED
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
} TKREQSTAGEPLACEORDERCHANGED, *PTKREQSTAGEPLACEORDERCHANGED;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS当前等待人数
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_STAGEWAITPLAYERCOUNT (TKID_MS2GSBASE + 0x049)
typedef struct tagTKREQSTAGEWAITPLAYERCOUNT
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int nWaitPlayerCount;
} TKREQSTAGEWAITPLAYERCOUNT, *PTKREQSTAGEWAITPLAYERCOUNT;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS设置聊天通道
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_SETUSERCHATCHANNEL (TKID_MS2GSBASE + 0x04A)
typedef struct tagTKREQSETUSERCHATCHANNEL
{
    TKHEADER header;

    DWORD dwMatchID;
    DWORD dwUserID;

    BOOL bEnableGCChat;

    DWORD dwChatSvrIP;  // 聊天服务器IP，注意是机器字节序
    DWORD dwChatSvrPort;
    DWORD dwChatChannelID;
    DWORD dwChatChannelSubID;
    DWORD dwChatChannelType;
    DWORD dwChatUserType;

} TKREQSETUSERCHATCHANNEL, *PTKREQSETUSERCHATCHANNEL;

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户满足某种游戏规则时给予某种奖励
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_GAMERULSERAWARD (TKID_MS2GSBASE + 0x04B)
// REQ: GameServer --> MatchServer
typedef struct tagTKREQGAMERULSERAWARD
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];

    int nGameRuler;
    int nAwardID;
    int nAwardParamLen;

} TKREQGAMERULSERAWARD, *PTKREQGAMERULSERAWARD;
// ACK:MatchServer --> GameServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS游戏盘数累进奖励配置信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_GAMECOUNTAWARDINFO (TKID_MS2GSBASE + 0x04C)
typedef struct tagTKREQGAMECOUNTAWARDINFO
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int lenText;
} TKREQGAMECOUNTAWARDINFO, *PTKREQGAMECOUNTAWARDINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS执行比赛Action
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHACTION (TKID_MS2GSBASE + 0x04D)
//ACK:MatchServer --> GameServer
typedef struct tagTKREQMATCACTION
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;
    int nActionID;
    int lenText;
} TKREQMATCACTION, *PTKREQMATCACTION;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS返回指定阶段的某轮次排名结果信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_STAGEBOUTRESULT (TKID_MS2GSBASE + 0x04E)
typedef struct tagTKREQSTAGEBOUTRESULT
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int lenText;
} TKREQSTAGEBOUTRESULT, *PTKREQSTAGEBOUTRESULT;

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS，让客户端打开或关闭轮次排名结果信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_OPENSTAGEBOUTRESULT (TKID_MS2GSBASE + 0x04F)
typedef struct tagTKREQOPENSTAGEBOUTRESULT
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    int nOpenStyle;  //0关闭，1列表框方式打开，2面板框方式打开
    DWORD dwUserID;
    BOOL bInWaitPlayoff;  //是否处于加赛等待中 wuqy add 2015.4.28(没有加赛配置或已经完成加赛的，bInWaitPlayoff为FALSE)
} TKREQOPENSTAGEBOUTRESULT, *PTKREQOPENSTAGEBOUTRESULT;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS返回指定局制的局制分信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_ROUNDSCOREINFO (TKID_MS2GSBASE + 0x050)
typedef struct tagTKREQROUNDSCOREINFO
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    int lenText;
} TKREQROUNDSCOREINFO, *PTKREQROUNDSCOREINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS让玩家复活
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_GAME_RELIVECOST (TKID_MS2GSBASE + 0x051)
typedef struct tagTKMSREQPLAYERRELIVECOST
{
    TKHEADER header;

    DWORD dwUserID;

    DWORD dwMatchID;
    WORD wStageID;

    int nCostType;   // 复活费用类型
    DWORD dwCostID;  // 复活费用ID
    int nAmount;     // 复活费用的数量

    int nScore;  // 可获得的分数

    int cnLivePlayer;  // 当前存活人数
    int nTimeOut;      // 倒计时
} TKMSREQPLAYERRELIVECOST, *PTKMSREQPLAYERRELIVECOST;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS设置桌底池坐下限制
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_SETTABLEPOTLIMIT (TKID_MS2GSBASE + 0x052)
typedef struct tagTKREQSETTABLEPOTLIMIT
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    int nCurTablePot;
    int nMinSitDownTablePotPer;
} TKREQSETTABLEPOTLIMIT, *PTKREQSETTABLEPOTLIMIT;

//-------------------------------------------------------------------------------------------------------------------------
//
// Continue Round
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CONTINUEROUND (TKID_MS2GSBASE + 0x053)
//REQ:MatchServer --> GameServer
typedef TKREQCREATEROUND TKREQCONTINUEROUND;
typedef PTKREQCREATEROUND PTKREQCONTINUEROUND;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS检查MatchAgent的有效性
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_CHECKMATCHAGENTVALID (TKID_MS2GSBASE + 0x054)
typedef struct tagTKREQCHECKMATCHAGENTVALID
{
    TKHEADER header;

    int nGSPort;
    int nGameID;

    int nProductID;
    int nTourneyType;
    DWORD dwTourneyID;
    DWORD dwMatchID;

} TKREQCHECKMATCHAGENTVALID, *PTKREQCHECKMATCHAGENTVALID;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS新的彩金信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_UPDATEJACKPOTINFO (TKID_MS2GSBASE + 0x055)
typedef struct tagTKREQGS2MSUPDATEJACKPOTINFO
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    // 后跟xml彩金信息数据
} TKREQGS2MSUPDATEJACKPOTINFO, *PTKREQGS2MSUPDATEJACKPOTINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS推送提示信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_USERTIPMESSAGE (TKID_MS2GSBASE + 0x057)
typedef struct tagTKREQUSERTIPMESSAGE
{
    TKHEADER header;

    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;  // 用户ID

    int nStatus;  // 比赛选手状态

    int nClass;  // 类别，参见 TKGameTipMessageDefine.h 的 ePushGameTipClass 定义
    int nTipID;  // 编号，参见 TKGameTipMessageDefine.h 的相关定义

    WORD wXmlOffset;  // 后缀XML地址偏移量，单位字节，参照点为消息包起始位置
    WORD wXmlLength;  // 后缀XML的长度，包含字符串结束符'\0'

    BYTE byShowType;   // 建议显示类型，参见 TKGameTipMessageDefine.h 的 ePushGameTipShowType 定义
    BYTE byOperation;  // 建议后续操作，参见 TKGameTipMessageDefine.h 的 ePushGameTipOperation 定义
    WORD wShowSecond;  // 建议显示时间(秒)
} TKREQUSERTIPMESSAGE, *PTKREQUSERTIPMESSAGE;

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS比赛用户的游戏养成数值有变更需要保存
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_SAVEUSERGROWVALUEEX (TKID_MS2GSBASE + 0x058)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQSAVEUSERGROWVALUEEX
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;

    DWORD dwUserID;
    DWORD dwAppendOffset;  //=sizeof(TKREQSAVEUSERGROWVALUEEX)
    int cnGameGrowInfo;    //

    BYTE byOSType;
    DWORD dwAppID;
    DWORD dwSiteID;
    DWORD dwGrowAccType;

} TKREQSAVEUSERGROWVALUEEX, *PTKREQSAVEUSERGROWVALUEEX;
//后跟cnGameGrowInfo个GameGrowInfo(DWORD GrowID + DWORD dwGrowValue) dwGrowValue为变更的差值
//ACK:MatchServer -->GameServer  (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS比赛变更用户在比赛中的游戏分数和名次
// 通知GS变更用户在游戏服务中的游戏分数和名次
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_UPDATEPLAYERMATCHSCORE (TKID_MS2GSBASE + 0x059)
//GameServer <--> MatchServer
typedef struct tagTKREQUPDATEPLAYERMATCHSCORE
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
    DWORD dwOrder;
    int nScore;
    BOOL bDieout;
} TKREQUPDATEPLAYERMATCHSCORE, *PTKREQUPDATEPLAYERMATCHSCORE;

//-------------------------------------------------------------------------------------------------------------------------
//
// 创建MixRound
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CREATEMIXROUND (TKID_MS2GSBASE + 0x060)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQCREATEMIXROUND
{
    TKHEADER header;

    int nOffsetAppendInfo;  // =sizeof(TKREQCREATEMIXROUND)

    DWORD dwRoundSrvIP;
    int nRoundSrvPort;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    module_identify id;

    int nGameID;

    int nDealMode;  // 发牌方式 0 == 随机发牌， 1 == 指定牌号，此时nDealParam为牌号
    int nDealParam;

    //TKMATCHACTION	stActRoundBegin;		// Round开始的Action
    //TKMATCHACTION	stActRoundOver;			// Round结束的Action
    //TKMATCHACTION	stActGameBegin;			// Game开始的Action
    //TKMATCHACTION	stActGameOver;			// Game结束的Action

    int cnUserID;           // 结构体后跟cnUserID个 TKADDGSMIXROUNDPLAYERINFO
    int nSizeofPlayerInfo;  // =sizeof(TKADDGSMIXROUNDPLAYERINFO)

    int nFinishedHand;         // 已经完成的局数
    int iGameHandBegin;        // 从第几局开始打
    int nLastHandSaveInfoLen;  // 上局保存数据

    int nSizeofStageRuler;  // sizeof(TKSTAGERULER),如果为0表示没有
    int nSizeofRoundRuler;  // sizeof(TKROUNDRULER),如果为0表示没有
    int nSizeofGameRuler;   // sizeof(TKGAMERULER),如果为0表示没有
    int nSizeofTableRuler;  // sizeof(TKTABLERULER),如果为0表示没有

    DWORD dwMatchRoundIdx;  // 该dwMatchID下的总Round索引号
    BOOL bNeedClearTable;   // 游戏结束时是否需要清桌 wuqy add 2013.8.12
    WORD wActionID;         // ActionID wuqy add 2015.9.23
    WORD wActionLength;     // Action的XML字符串长度，包含结束符'\0' wuqy add 2015.9.23

    int cnWatcher;
    int nSizeofWatchInfo;

    DWORD dwSpecialRules;  // 由比赛性质决定的一些特殊规则( 32bit )，如强制allin

    DWORD dwLimitingTime;  // 限定时间（1V1复式赛，限定时间内玩家思考不限时）
    WORD wReserve1;        // 预留 hexu add 2017.8.8
    WORD wTeamType;
    WORD wReserve3;
    WORD wReserve4;
    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;

} TKREQCREATEMIXROUND, *PTKREQCREATEMIXROUND;
//后跟cnUserID*TKADDGSMIXROUNDPLAYERINFO+nLastHandSaveInfoLen+TKTABLERULER+ActionXML+cnWatcher*TKWATCHINFO
//ACK:GameServer --> MatchServer (消息包TKHEADER)
typedef struct tagTKADDGSMIXROUNDPLAYERINFO
{
    DWORD dwUserID;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wReserve;
    module_identify id;  //用户所在比赛的module_id

    int nBout;   // 第几轮( 瑞士移位 )
    int nTable;  // 第几桌( 瑞士移位 )

    DWORD dwTeamID;
    DWORD dwCaptainID;      //团体赛的队长id
    BYTE byBreakHeartTask;  // 是否参与了碎心任务 wuqy 2016.6.3
    BYTE byHeartCount;      // 用户的HeartCount，即比赛中用户命的条数（cnMatchLive）wuqy add 2016.12.31
    BYTE byGameResult;      // 上局游戏结果
    BYTE byGameActor;       // 上局游戏角色
    DWORD dwMsTraceFlags;

    int nEnterOrder;
    int nPlaceOrder;

    int nRoundScoreBase;

    int nGameScore;      // 游戏积分
    int nMatchScore;     // 比赛积分
    int nMatchSecScore;  // 比赛小分
    int nRoundScore;     // 局制积分
    int nRoundSecScore;  // 局制小分

    int cnGameCurFinished;  // 当前完成的总盘数
    DWORD dwFlags;          // 一些标志 0位表示是否确认退出了比赛
    int nSeatOrder;
    int cnGameTotFinished;  // 比赛完成的总盘数
    int iCurGameHand;       // 用户当前进行的局制编号
} TKADDGSMIXROUNDPLAYERINFO, *PTKADDGSMIXROUNDPLAYERINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// 删除MixRound
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_DELETEMIXROUND (TKID_MS2GSBASE + 0x061)
//REQ:MatchServer --> GameServer
typedef struct tagTKREQDELETEMIXROUND
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    module_identify id;

    DWORD dwReason;  //结束原因:???
} TKREQDELETEMIXROUND, *PTKREQDELETEMIXROUND;
//ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// GS给MS发送MixRound结果
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_MIXROUNDRESULT (TKID_MS2GSBASE + 0x062)
//REQ:GameServer --> MatchServer
typedef struct tagTKREQMIXROUNDRESULT
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    module_identify id;

    WORD wOverType;
    WORD wOverFlags;
    int nRoundScoreBase;

    WORD wResultCount;
    WORD wResultSize;    //=sizeof(ROUNDRESULTINFO)
    WORD wResultOffset;  //=sizeof(TKREQROUNDRESULT)
    WORD wReserve;

    int iGameHandOver;         //从第几局开始打
    int nLastHandSaveInfoLen;  //上局保存数据
    int cnGameCreate;          //本局制局数
    int nMatchTax;             //本局制比赛扣费（生命值数值）
} TKREQMIXROUNDRESULT, *PTKREQMIXROUNDRESULT;
typedef struct tagMIXROUNDRESULTINFO
{
    DWORD dwUserID;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    DWORD dwFlags;
    WORD wGameResult;  // 0 == 未知， 1 == 胜利， 2 == 和， 3 == 负
    WORD wGameActor;   // 游戏规则定义的本局游戏角色，数值由每个游戏自己定义，在特定的Stage重组时使用 wuqy 2016.12.31
    int nRoundScoreBase;
    int nResultScore;     // 结果分：积分或筹码
    int nLastHandScore;   // 最后一盘得分
    int nRoundWinScore;   // 本局累计赢得的积分
    int nRoundGameCount;  // 本局累计游戏盘数
    int nGSMatchScore;    // 比赛积分(GS可能调整过的)wuqy add 2015.3.27
} MIXROUNDRESULTINFO, *PMIXROUNDRESULTINFO;
//ACK : TKHEADER

//-------------------------------------------------------------------------------------------------------------------------
//
// GS给MS发送一局游戏结果
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_GAME_RESULT (TKID_MS2GSBASE + 0x63)
// 请求：
typedef struct tagTKREQGAMERESULT
{
    TKHEADER header;

    WORD player_offset;
    WORD player_count;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    view_identify view_id;

    // 后跟player_count个player
    // players...
    struct player
    {
        DWORD id;
        int score;
    };

    inline player* player_list()
    {
        return (player*)((char*)this + player_offset);
    }
} TKREQGAMERESULT, *PTKREQGAMERESULT;

//-------------------------------------------------------------------------------------------------------------------------
//功能：MS通知GS让玩家选择支付业务扣费
//创建：王斌
//时间：2018-06-07
//说明：MatchSvr -> GameSvr -> MatchClient
//      该功能为比赛或游戏过程中产生的通用业务扣费通知，可以用于复活、加血、购买道具等业务
//		扣费可以自由组合钱物分，不限定个数
//		dwPayTypeID参数表示业务类型。
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_GAME_PAYMENTCOST (TKID_MS2GSBASE + 0x64)
typedef struct tagTkMSReqPlayerPaymentCost
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    int nTimeOut;      // 购买倒计时
    int cnLivePlayer;  // 当前存活人数

    DWORD dwPayTypeID;      // 扣费业务类型 参见: eDicPaymentBusinessTypeDef, 参见 TKComitUniversalDicSrv.h
    DWORD dwCostPlanCount;  // 扣费方案个数，TK_COSTPLAN，参见 TKMatchUniversalDicSrv.h
    TKSUFFIXIDX sufData;
    TKSUFFIXIDX sufTaskData;  // 后缀任务数据
} TKMSREQPLAYERPAYMENTCOST, *PTKMSREQPLAYERPAYMENTCOST;
// 后缀数据 dwCastPlanCount个TK_COSTPLAN

// 回馈：
typedef struct tagTKACKMSPAYMENTCOST
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwResult;     // 业务执行结果，0-失败，1-表示成功
    DWORD dwPayTypeID;  // 扣费业务类型 参见: eDicPaymentBusinessTypeDef, 参见 TKComitUniversalDicSrv.h
    DWORD dwPlanID;     // 复活的方案ID
} TKACKMSPAYMENTCOST, *PTKACKMSPAYMENTCOST;

//-------------------------------------------------------------------------------------------------------------------------
//功能：MS通知GS让玩家选择支付业务扣费 的结果
//创建：王斌
//时间：2018-06-12
//说明：MatchSvr -> GameSvr -> MatchClient
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_GAME_PAYMENTRESULT (TKID_MS2GSBASE + 0x65)
typedef struct tagTkMSReqPlayerPaymentResult
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    DWORD dwPayTypeID;  // 扣费业务类型 参见: eDicPaymentBusinessTypeDef, 参见 TKComitUniversalDicSrv.h
    DWORD dwResult;     // 支付业务扣费的结果，eDicTypeTKPaymentResult，参见 TKMatchUniversalDicSrv.h
} TKMSREQPLAYERPAYMENTRESULT, *PTKMSREQPLAYERPAYMENTRESULT;

//-------------------------------------------------------------------------------------------------------------------------
//
// 更新MatchAgent
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_UPDATE_MATCH_AGENT (TKID_MS2GSBASE + 0x66)
// ! ! ! ! !动态运维，用于通知GS， MS IP端口的变化! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
// ! ! ! ! !小心！小心！小心！本协议MsgType与其它协议MsgType冲突（麻将GS->MS的游戏数据），维护时需 ! 小心! ! ! ! ! ! ! ! ! !
// 请求：
typedef struct tagTKREQUPDATEMATCHAGENT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwMsIP;
    int nMsPort;
} TKREQUPDATEMATCHAGENT, *PTKREQUPDATEMATCHAGENT;

/************************************************************************/
/*                        海底捞                                        */
//功能：GS通知MS游戏数据
//创建：袁文学
//时间：2018-06-27
//说明：GameSvr -> MatchSvr
// ! ! ! ! !小心！小心！小心！本协议MsgType与其它协议MsgType冲突(MS动态运维IP更换MS->GS)，维护时需 ! 小心! ! ! ! ! ! ! ! ! !
/************************************************************************/
#define TKID_GS2MS_GAME_ADDITIONAL_INFO (TKID_MS2GSBASE + 0x66)

typedef struct tagTKReqAdditionalInfo
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;

    TKSUFFIXIDX sufi32Data;
    TKSUFFIXIDX sufchData;
} TKREQADDITIONALINFO, *PTKREQADDITIONALINFO;

// 游戏服务申请更新岛屿用户筹码		20191129 by wanghuan
#define TKID_GS2MS_GAME_UPDATE_USERCHIP (TKID_MS2GSBASE + 0x68)
typedef struct tagTKReqUpdateUserChip
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wReserve1;
    DWORD dwUserID;
    int nChip;
} TKREQUPDATEUSERCHIP, *PTKREQUPDATEUSERCHIP;

typedef struct tagTKAckUpdateUserChip
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wReserve1;
    DWORD dwUserID;
    int nChip;
} TKACKUPDATEUSERCHIP, *PTKACKUPDATEUSERCHIP;

// 该协议用于对局过程中对局数据的记录。如斗地主炸弹得多少分，有无王炸。麻将中有无清一色，各种地方玩法积分
#define TKID_GS2MS_GAME_PROCESS_DATA (TKID_MS2GSBASE + 0x69)
typedef struct tagTKReqGS2MSGameProcessData
{
    TKHEADER header;
    DWORD dwMPID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwReserve1;
    DWORD dwReserve2;
    WORD wStageID;
    WORD wRoundID;
    WORD wReserve1;
    WORD wReserve2;
    TKSUFFIXIDX stSuffixData;
    TKSUFFIXIDX stSuffixReserve;
} TKREQGS2MSGAMEPROCESSDATA, *PTKREQGS2MSGAMEPROCESSDATA;
// 后缀数据为json格式，比赛服务不做解析，用于客户端展示

// ACK:
// TKHEADER

#define TKID_MS2GS_GAME_PROCESS_DATA (TKID_MS2GSBASE + 0x6A)
typedef TKREQGS2MSGAMEPROCESSDATA TKREQMS2GSGAMEPROCESSDATA;
// 后缀数据为json格式，比赛服务不做解析，用于客户端展示
// ACK:
// TKHEADER

// 游戏服务通知比赛服务AI调用出问题了
#define TKID_GS2MS_AIDISPATCH_ERROR (TKID_MS2GSBASE + 0x6B)
typedef struct tagTKREQBOTAIDISPATCHERROR
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
} TKREQBOTAIDISPATCHERROR, *PTKREQBOTAIDISPATCHERROR;

#define TKID_MS2GSDLL_COMMON_PROTOCOL (TKID_MS2GSBASE + 0x6C)
// 比赛服务与游戏动态库之间的通用头TKREQDAGONPLAYERMSG
struct TK_COMMON_MATCH_REQ
{
    TKHEADER stHeader;
    uint32_t uiMsgType;
    uint64_t ui64UserID;
    // 后接JSON字符串
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// 游戏中复活开始协议ID   MS->GSDLL
#define MS2GSDLL_GAMING_RELIFE_BEGIN 1
// JSON格式
// {
//	"Duration":时长,单位:秒,32位整数
//	"Chip":筹码,64位整数
// }

// 游戏中复活结果协议ID   GSDLL->MS
#define MS2GSDLL_GAMING_RELIFE_RESULT 2
// JSON格式
// {
//  "Res":座位号,32位整数
//  "AddChip":筹码增加值,64位整数
//  "Chip":筹码最终结果,64位整数
// }
// "Res"返回值定义
enum eGamingRelifeResultType
{
    ENUM_GAMING_RELIFE_RESULT_SUCCESS = 0,            // 复活成功
    ENUM_GAMING_RELIFE_RESULT_FAILED = 1,             // 复活失败
    ENUM_GAMING_RELIFE_RESULT_TIMEOUT = 2,            // 复活超时
    ENUM_GAMING_RELIFE_RESULT_PAY_FAILED = 3,         // 复活支付兑换失败
    ENUM_GAMING_RELIFE_RESULT_RELIFECOUNT_LIMIT = 4,  // 复活次数限制
    ENUM_GAMING_RELIFE_RESULT_END
};

// 游戏中放弃复活协议ID   GSDLL->MS
#define MS2GSDLL_GAMING_RELIFE_GIVEUP 3
// 无后缀

// 传输游戏结果协议ID   GSDLL->MS
#define MS2GSDLL_GAMING_POST_CARDRESULT 4
// JSON格式
// {
//  "CardMult":胡牌倍率,64位整数
//  "OneHandScore":一盘得分,64位整数
// }

// 透传数据到排行榜
#define MS2GSDLL_PUSH_MSG_RANKING_LIST 5
// JSON格式
/* {
	"bal": 1111111111,          // int64_t 余额 balance
	"vt": 3,                    // int32_t 数据类型定义 Value Type (enum RLPlayerDataType)
	"val": 22222222             // int64_t 值 value
	}
 */
enum RLPlayerDataType
{
    RL_PLAYER_DATA_BEGIN = 0,
    RL_PLAYER_DATA_BALANCE_CHANGE = 1,  // 余额变化
    RL_PLAYER_DATA_TOP_COUNT = 2,       // 番数封顶次数
    RL_PLAYER_DATA_END
};

// 游戏中通知比赛，设置玩家筹码
#define MS2GSDLL_GAMING_SET_PLAYER_CHIP 6
// JSON格式
/* {
	"Chip":筹码最终结果,64位整数
	}
 */
// 透传复活计划到游戏 MS->GSDLL
#define MS2GSDLL_PUSH_RELIFE_PLAN 7

// 重回巅峰消息 DLL->MS
#define MS2GSDLL_GAMMING_BACK_TO_TOP 8
// JSON格式
/* {
"InitChip":11   ,        //初始带入筹码
"MaxChip":11,            //本局中最大筹码
"BaseChip":11,           //基数
"CapMult":11,            //封顶倍数
"RelifeSuccessRate"     //复活成功率
	}
 */

// 携带道具信息到游戏 MS->GSDLL
#define MS2GSDLL_PUSH_CARRY_PROPS 9
// JSON格式
/*
{
    "props":[
        {
            "id":60087346,
            "value":1,
            "name":"起手花"
        },
        {
            "id":60087358,
            "value":30,
            "name":"收益加成30%"
        }
    ]
}
*/

// 对局信息到游戏 MS->GSDLL
#define MS2GSDLL_PUSH_CHICKEN_ROUND_DATA 10
// JSON格式
/*
{
    "Card":[
        {
            "ID":1,
            "Value":2
        },
        {
            "ID":1,
            "Value":2
        }
    ],
    "TotalKills":1,
    "Rewards":1,
    "Chicken":1,
    "AirDrop":1
}
*/

// 出局玩家到比赛 GSDLL->MS
#define MS2GSDLL_PUSH_OUT_PLAYER_DATA 11
// JSON格式
/*
{
    "Kills":1,
    "PlayerRewards":[
        {
            "UID":123456,
            "Rewards":123
        },
        {
            "UID":123456,
            "Rewards":123
        }
    ]
}
*/

// 巅峰值消息 GSDLL->MS
#define MS2GSDLL_PUSH_TOPSCORE 12
// JSON格式
/*
{
	"MaxChip":1000 
}
*/

// 给游戏推送定制化的游戏规则（一般用于自建赛） MS->GSDLL
#define MS2GSDLL_PUSH_EXTRA_GAMERULE 13
// JSON格式
/*
{
	"isFanShan": true,
	"startLevel": 0,
	"endLevel": 12
}
*/

// 最大胡牌倍数消息 GSDLL->MS
#define MS2GSDLL_PUSH_MAX_WIN 14
// JSON格式：目前无后缀

// GSDLL->MS 玩家游戏中胡牌对boss造成的伤害
#define MS2GSDLL_PUSH_DAMAGE 15
// JSON格式
/*
{
	"chip" = 10000
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------------------------
//
// 自建桌透传消息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TK_MATCH_RELAY_MSG_BEGIN (TKID_MS2GSBASE + 0x096)  //+150
#define TK_MATCH_RELAY_MSG_END (TK_MATCH_RELAY_MSG_BEGIN + TK_SVR_MAX_RELAY_MSG)
typedef struct tagPlayerMsgRelayHdr
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;

    DWORD dwDataLength;
} TK_PLAYER_MSG_RELAY_HDR, *LPTK_PLAYER_MSG_RELAY_HDR;

// MS-->GS
#define TKID_MS2GS_INITTABLE (TK_MATCH_RELAY_MSG_BEGIN + 0)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_PLAYER_SITDOWN (TK_MATCH_RELAY_MSG_BEGIN + 1)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_PLAYER_READY (TK_MATCH_RELAY_MSG_BEGIN + 2)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_ROUNDBEGIN (TK_MATCH_RELAY_MSG_BEGIN + 3)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// GS<-->MS
#define TKID_GS2MS_PLAYERENABLEWATCH (TK_MATCH_RELAY_MSG_BEGIN + 4)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_INITGAMETABLE (TK_MATCH_RELAY_MSG_BEGIN + 5)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_MARKPLAYERIDLE (TK_MATCH_RELAY_MSG_BEGIN + 6)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_ADDHP (TK_MATCH_RELAY_MSG_BEGIN + 7)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_SAVEHP (TK_MATCH_RELAY_MSG_BEGIN + 8)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_MARKAUTOADDHP (TK_MATCH_RELAY_MSG_BEGIN + 9)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

#define TKID_MS2GS_MARKLOCKDOWN (TK_MATCH_RELAY_MSG_BEGIN + 10)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_VIPMODE (TK_MATCH_RELAY_MSG_BEGIN + 11)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_LUCKCARD (TK_MATCH_RELAY_MSG_BEGIN + 12)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_ADDLUCKY (TK_MATCH_RELAY_MSG_BEGIN + 13)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_CONSUMELUCKY (TK_MATCH_RELAY_MSG_BEGIN + 14)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_EXCHANGE (TK_MATCH_RELAY_MSG_BEGIN + 15)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int cnExchange;
// DWORD dwBlood1;
// DWORD dwCost1;
// DWORD dwBlood2;
// DWORD dwCost2;
// DWORD dwBlood3;
// DWORD dwCost3;
// DWORD dwBlood4;
// DWORD dwCost4;

// 一个攻擂选手的信息
typedef struct tagTKChallengerInfo
{
    DWORD dwUserID;
    DWORD dwMaxWin;                       // 最大连胜(当擂主)次数
    DWORD dwTotalWin;                     // 总的胜利次数
    char szNickName[TK_BUFLEN_NICKNAME];  // 昵称
} TKCHALLENGERINFO, *PTKCHALLENGERINFO;

// MS<-->GS 擂台信息
#define TKID_MS2GS_CHALLENGERLIST (TK_MATCH_RELAY_MSG_BEGIN + 16)
// ACK
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int nWinnerWinCount;		// 擂主当前守擂次数
// int nChallengerCount;	// 挑战者,第一个为擂主
// TKCHALLENGERINFO[]

// GS<-->MS	申请打擂
#define TKID_GS2MS_CHALLENGERAPPLY (TK_MATCH_RELAY_MSG_BEGIN + 17)
// REQ/ACK
// TK_PLAYER_MSG_RELAY_HDR hdr;
typedef struct tagTKReqChallengerApply
{
    DWORD dwUserID;
    BOOL bApply;  // 1打擂, 2退擂
                  //若是打擂则后跟一个选手擂台的信息
} TKREQCHALLENGERAPPLY, *PTKREQCHALLENGERAPPLY;

// 擂主改变原因
enum CHALLENGE_UPDATE_WINNERINFO_REASON
{
    REASON_WINNERLEAVE,      // 擂主离开
    REASON_WINNERLOSS,       // 擂主失败
    REASON_WINNERUPDATEINFO  // 擂主信息更新
};

// MS-->GS	擂主变化
#define TKID_MS2GS_CHALLENGE_UPDATE_WINNERINFO (TK_MATCH_RELAY_MSG_BEGIN + 18)
// ACK
// TK_PLAYER_MSG_RELAY_HDR hdr;
typedef struct tagTKAckUpdateWinnerInfo
{
    DWORD dwWinnerUserID;           // 擂主ID
    int nWinnerWinCount;            // 擂主已经连胜次数
    DWORD dwReason;                 // 信息更新原因
    TKCHALLENGERINFO stWinnerInfo;  // 新擂主信息
    int nOutPlayerCount;            // 打擂失败或其他导致退出的玩家数量
                                    // 后跟nOutPlayerCount个DWORD
} TKACKUPDATEWINNERINFO, *PTKACKUPDATEWINNERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：下发自由房间用户养成数值信息
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_ROOMPLAYER_GROWVALUE (TK_MATCH_RELAY_MSG_BEGIN + 19)
// ACK:
//  TK_PLAYER_MSG_RELAY_HDR hdr;
//	DWORD dwUserID
//  int cnGrowValue
//  cnGrowValue个GROWINFO(DWORD dwGrowID, DWORD dwValue)

//-----------------------------------------------------------------------------------------
//   功    能：自由房间用户标志变更（比如是否桌主标志）
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_ROOMPLAYER_CHANGEFLAGS (TK_MATCH_RELAY_MSG_BEGIN + 20)
// ACK:
//  TK_PLAYER_MSG_RELAY_HDR hdr;
//	DWORD dwUserID
//  WORD wFlags   ,参见TKMatchRoomUniversalDicSrv.h中TK_ROOMPLAYER_FLAG定义

//-----------------------------------------------------------------------------------------
//   功    能：自由房间用户桌主替人(将指定座位号上的人踢开)
//   说    明：GC -> GS -> MS
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MS_ROOMPLAYER_HOSTKICK (TK_MATCH_RELAY_MSG_BEGIN + 21)
// REQ
//  TK_PLAYER_MSG_RELAY_HDR hdr;
//  int nSeat;                          // 座位号(base 0)

//-----------------------------------------------------------------------------------------
//   功    能：服务器询问用户是否同意花费金币加血补充生命值
//   说    明：MS -> GS -> GC
//             如果用户同意则发送TKID_MS2GS_ADDHP消息
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_ROOMPLAYER_QUERYADDHP (TK_MATCH_RELAY_MSG_BEGIN + 22)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
//	int			nCurLife;			// 当前生命值
//	int			nExchangeRate;		// 兑换率
//	int			nAddLife;			// 需要补充的生命值（如果nAddLife为0，则按照血瓶类型选择加血）
//	int			nCostCoin;			// 不要花费的金币数
//	int			cnHematinic;		// 可选的血瓶类型
//	HEMATINIC	asHematinic[ MAX_HEMATINIC_COUNT ];	// 每种血瓶的详细信息
//  string      询问加血的文字提示字符串（包含结束符\0）

//-----------------------------------------------------------------------------------------
//   功    能：
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_STARTABLE_ACCEPT_INVITE (TK_MATCH_RELAY_MSG_BEGIN + 23)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

//-----------------------------------------------------------------------------------------
//   功    能：
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_TERMINATE_GC (TK_MATCH_RELAY_MSG_BEGIN + 24)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

//-----------------------------------------------------------------------------------------
//   功    能：比赛选手列表信息字段定义
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHPLAYERLIST_DEFINE (TK_MATCH_RELAY_MSG_BEGIN + 25)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
//	String (xml字符串，包含结束符\0)
//
//xml格式:
//			<PlayerInfo>
//				<F name="用户名" id="F1"/>
//				<F name="级别" id="F2" GrowID="1001201"/>   //(如果GrowID不为0，表示需要由大厅来翻译成字符串)
//				<F name="积分" id="F3" />
//				<F name="战斗力" id="F4" />
//			</PlayerInfo>

//-----------------------------------------------------------------------------------------
//   功    能：增加或更新比赛选手列表信息
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHPLAYERLIST_ADD (TK_MATCH_RELAY_MSG_BEGIN + 26)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
//	String (xml字符串，包含结束符\0)
//
//xml格式:
//		<PlayerInfo>
//			<P UID="100003086" F1="苗苗" F2="2100" F3="1000" F4="796"/>
//			<P UID="100001005" F1="唐勇" F2="2100" F3="1000" F4="796"/>
//		</PlayerInfo>

//-----------------------------------------------------------------------------------------
//   功    能：删除比赛选手列表信息
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHPLAYERLIST_DEL (TK_MATCH_RELAY_MSG_BEGIN + 27)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
//		header.dwParam: 需要删除的UerID

//-----------------------------------------------------------------------------------------
//   功    能：明星桌切换
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_STAR_TABLE_SEL (TK_MATCH_RELAY_MSG_BEGIN + 28)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

//-----------------------------------------------------------------------------------------
//   功    能：团体对抗赛队伍比分
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_DUALMEET_SCORE (TK_MATCH_RELAY_MSG_BEGIN + 29)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data
struct MSTeamScore
{
    char szTeamName[TK_BUFLEN_USERNAME * 2];
    int nTeamScore;
};

//-----------------------------------------------------------------------------------------
//   功    能：德州锦标赛是否可以随时补充筹码
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_GAMBLE_CAN_REBUY (TK_MATCH_RELAY_MSG_BEGIN + 30)
// ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int nJackpot 奖池大小
// int nGold;
// int nChip;
// int nNotShowButton; 如果有此参数，则不显示加血按钮

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知GS让玩家复活
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_RELIVE_SELECT (TK_MATCH_RELAY_MSG_BEGIN + 31)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int cnLivePlayer;	// 当前存活人数
// int nTimeOut;		// 倒计时
// <select><case id = "0" tip = "花费100金币获得1000筹码"><case id = "1" tip = "花费200金币获得2000筹码"><case id = "2" tip = "花费10奠基石获得1000筹码"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
// 功    能：用户请求复活
// 说    明：GC -> GS -> MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USER_REQRELIVE (TK_MATCH_RELAY_MSG_BEGIN + 32)
// REQ
// TK_PLAYER_MSG_RELAY_HDR hdr;
//	int nSecondUsed;	// 用时
//	int nReliveCase;		// 选择的复活方案

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知GS让玩家add on
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_ADDON_SELECT (TK_MATCH_RELAY_MSG_BEGIN + 33)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int nTimeOut;		// 倒计时
// <select><case id = "0" tip = "花费100金币获得1000筹码"><case id = "1" tip = "花费200金币获得2000筹码"><case id = "2" tip = "花费10奠基石获得1000筹码"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
// 功    能：用户请求add on
// 说    明：GC -> GS -> MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_USER_REQADDON (TK_MATCH_RELAY_MSG_BEGIN + 34)
// REQ
// TK_PLAYER_MSG_RELAY_HDR hdr;
//	int nAddonCase;		// 选择的addon方案

//-----------------------------------------------------------------------------------------
//   功    能：团体对抗赛队伍比分
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_DUALMEET_SCOREEX (TK_MATCH_RELAY_MSG_BEGIN + 35)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data
//struct MSTeamScore
//{
//	char szTeamName[ TK_BUFLEN_USERNAME * 2 ];
//	int 	nTeamScore;
//};

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知玩家连胜奖励信息
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CV_AWARD_INFO (TK_MATCH_RELAY_MSG_BEGIN + 36)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// xml字符串

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知玩家获得连胜奖励
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_CV_AWARD (TK_MATCH_RELAY_MSG_BEGIN + 37)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD cnWin	// 连胜次数

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知玩家休息
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_REST (TK_MATCH_RELAY_MSG_BEGIN + 38)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// 后跟
typedef struct tagISLANDPLAYERINFO
{
    int nRestTime;                               // 可休息时间,到点后如果玩家不点继续按钮游戏将自动开始
    int cnHematinic;                             // 可选的血瓶类型
    HEMATINIC asHematinic[MAX_HEMATINIC_COUNT];  // 每种血瓶的详细信息

    int nLife;    // 当前生命值
    int nCoin;    // 可回兑金币数
    int nScore;   // 积分
    int cnGames;  // 已完成牌副数

    int nAwardTimeSpan;        //颁奖时间间隔(秒),<=0表示不颁奖
    int nNextAwardLeftSecond;  //距离下一次颁奖的剩余时间(秒)
} ISLANDPLAYERINFO, *PISLANDPLAYERINFO;

//-----------------------------------------------------------------------------------------
//   功    能： MS 推送Team列表
//   说    明： 1、MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_TEAM_LIST (TK_MATCH_RELAY_MSG_BEGIN + 39)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int cnTeam;
// 后跟cnTeam个MS2GSTeamInfo
struct MS2GSTeamInfo
{
    char szTeamName[TK_BUFLEN_USERNAME];
    int nScore;
    int nOrder;  // 名次为负表示出局
};

//-----------------------------------------------------------------------------------------
//   功    能： MS 推送玩家列表
//   说    明： 1、MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_PLAYER_LIST (TK_MATCH_RELAY_MSG_BEGIN + 40)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int cnPlayer;
// 后跟cnPlayer个MS2GSPlayerInfo
struct MS2GSPlayerInfo
{
    DWORD dwUserID;
    char szUserName[TK_BUFLEN_USERNAME];
    int nScore;
    BOOL bOut;  // 是否出局
    DWORD dwGSIP;
    DWORD dwBSIP;
    int nGSPort;
};

//-----------------------------------------------------------------------------------------
//   功    能： MS 推送玩家等待信息
//   说    明： 1、MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_TEAM_WAIT (TK_MATCH_RELAY_MSG_BEGIN + 41)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// 后跟字符串

//-----------------------------------------------------------------------------------------
//   功    能： MS 推送阶段结束信息
//   说    明： 1、MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_TEAM_PHASEOVER (TK_MATCH_RELAY_MSG_BEGIN + 42)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD dwFTPUserID;
// DWORD dwRestSecond;
// DWORD cnScoreBase;
// 后跟盲注信息

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知GS让玩家复活
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_RELIVE_SELECTEX (TK_MATCH_RELAY_MSG_BEGIN + 43)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int nJackpot;			// 总奖金池
// int cnLivePlayer;	// 当前存活人数
// int nAverageChip;	// 平均筹码
// int nScoreBase;		// 当前盲注
// int nReliveLimitTime;// 复活时限
// int nElapsedTime;	// 已进行秒数
// int nTimeOut;		// 倒计时
// <select><case id = "0" tip = "花费100金币获得1000筹码"><case id = "1" tip = "花费200金币获得2000筹码"><case id = "2" tip = "花费10奠基石获得1000筹码"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知GS让玩家add on
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_ADDON_SELECTEX (TK_MATCH_RELAY_MSG_BEGIN + 44)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int nTimeOut;		// 倒计时
// int nJackpot;			// 总奖金池
// int cnLivePlayer;	// 当前存活人数
// int nAverageChip;	// 平均筹码
// int nScoreBase;		// 当前盲注
// int anScoreBase[ 7 ];// 下阶段盲注
// string <select><case id = "0" tip = "花费100金币获得1000筹码"><case id = "1" tip = "花费200金币获得2000筹码"><case id = "2" tip = "花费10奠基石获得1000筹码"></select>
// int nElapseSecond	// 已经等待的秒数

//-----------------------------------------------------------------------------------------
//   功    能：德州锦标赛奖金池变化
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_GAMBLE_JACKPOT_RAISE (TK_MATCH_RELAY_MSG_BEGIN + 45)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int nJackpot 奖池大小
// int nAverageChip;	 平均筹码

// MS<-->GS
#define TKID_MS2GS_PLAYER_CHGTABLE (TK_MATCH_RELAY_MSG_BEGIN + 46)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS<-->GS
#define TKID_MS2GS_PLAYER_WAITINLIST (TK_MATCH_RELAY_MSG_BEGIN + 47)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_MATCH_OPTION (TK_MATCH_RELAY_MSG_BEGIN + 48)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD dwFlags: Bit0:支持进入队列按钮    Bit1:支持换桌按钮 Bit2:自动rush
// DWORD nMaxAddToHP
// DWORD nMinAddToHP
// DWORD nExchangeRate
// DWROD nHPMode
// ----------------------------------------------------------------------------------------

// GS-->MS
#define TKID_GS2MS_AUTO_ADDHP_TOUCHED (TK_MATCH_RELAY_MSG_BEGIN + 49)
// REQ:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// data

// MS-->GS
#define TKID_MS2GS_LUCKCARDINFO (TK_MATCH_RELAY_MSG_BEGIN + 50)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// xml (参见LuckCardInfo.xml格式)

// MS<-->GS
#define TKID_MS2GS_LUCKCARD_USEROPTION (TK_MATCH_RELAY_MSG_BEGIN + 51)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// int nUseMin 单次使用下限（低于该值不应用）
// int nUseMax 单次使用上限（高于该值按该值应用）

// MS<-->GS
#define TKID_MS2GS_PLAYER_SCAN (TK_MATCH_RELAY_MSG_BEGIN + 52)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// double lat;
// double lng;

// MS<-->GS
#define TKID_MS2GS_ENTER_TABLE (TK_MATCH_RELAY_MSG_BEGIN + 53)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD dwTableID;

// MS<-->GS
#define TKID_MS2GS_LEAVE_TABLE (TK_MATCH_RELAY_MSG_BEGIN + 54)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;

// MS<-->GS
#define TKID_MS2GS_GET_BILL (TK_MATCH_RELAY_MSG_BEGIN + 55)
/////////// REQ
// TK_PLAYER_MSG_RELAY_HDR hdr;

/////////// ACK
// TK_PLAYER_MSG_RELAY_HDR hdr;
// TKLBSBill bill;		// 帐单明细
struct TKLBSBill
{
    struct PlayerBill
    {
        DWORD dwUserID;
        int nInitChip;   // 初始筹码
        int nRebuyChip;  // 补充筹码
        int nChip;       // 当前筹码
    };

    int cnPlayer;
    // 后跟cnPlayer个PlayerBill
};

//争先赛消息
//MS->GS 争先赛开始
#define TKID_MS2GS_FORWARDBEGIN (TK_MATCH_RELAY_MSG_BEGIN + 56)
typedef struct tagTKMSFORWARDBEGIN
{
    TK_PLAYER_MSG_RELAY_HDR hdr;
    size_t timelong;   //争先赛比赛时长，单位s
    UINT64 timebegin;  //比赛开始时间单位 0.1微s
    tagTKMSFORWARDBEGIN()
    {
        memset(this, 0, sizeof(*this));
    }
} TKMSFORWARDBEGIN, *PTKMSFORWARDBEGIN;

//MS<->GS 请求当前排名列表和回复
#define TKID_MS2GS_FORWARDORDER (TK_MATCH_RELAY_MSG_BEGIN + 57)
typedef struct tagTKMSFORWARDORDERREQ
{
    TK_PLAYER_MSG_RELAY_HDR hdr;
    tagTKMSFORWARDORDERREQ()
    {
        memset(this, 0, sizeof(*this));
    }
} TKMSFORWARDORDERREQ, *PTKMSFORWARDORDERREQ;
struct Forwardplayerinfo
{
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];
    int nGrade;      //争先赛成绩
    size_t nRank;    //争先赛排名
    size_t usetime;  //争先赛使用时长
    size_t hand;     //以完成局数
    Forwardplayerinfo()
    {
        memset(this, 0, sizeof(*this));
    }
};
typedef struct tagTKMSFORWARDORDERACK
{
    TK_PLAYER_MSG_RELAY_HDR hdr;
    size_t count;    //后缀Forwardplayerinfo数量
    bool iscurdata;  //false表示目前不是争先赛进行阶段，显示上次的数据，并且不包含自己。true第1个节点是自己
    tagTKMSFORWARDORDERACK()
    {
        memset(this, 0, sizeof(*this));
    }
} TKMSFORWARDORDERACK, *PTKMSFORWARDORDERACK;

//MS->GS 在争先赛中每次beginhand的通知消息
#define TKID_MS2GS_FORWARDBEGINHAND (TK_MATCH_RELAY_MSG_BEGIN + 58)
typedef struct tagTKMSFORWARDBEGINHAND
{
    TK_PLAYER_MSG_RELAY_HDR hdr;
    size_t hand;         //当前局数
    size_t totalhand;    //总局数
    size_t rank;         //当前名次
    size_t playercount;  //当前总人数
    tagTKMSFORWARDBEGINHAND()
    {
        memset(this, 0, sizeof(*this));
    }
} TKMSFORWARDBEGINHAND, *PTKMSFORWARDBEGINHAND;

//MS->GS争先赛结束消息
#define TKID_MS2GS_FORWARDEND (TK_MATCH_RELAY_MSG_BEGIN + 59)
typedef struct tagTKMSFORWARDEND
{
    TK_PLAYER_MSG_RELAY_HDR hdr;
    size_t timelong;  //距离下一场争先赛的时长，单位s
    UINT64 timeend;   //比赛结束时间单位 0.1微s
    tagTKMSFORWARDEND()
    {
        memset(this, 0, sizeof(*this));
    }
} TKMSFORWARDEND, *PTKMSFORWARDEND;

//MS->GS方位排名消息
#define TKID_MS2GS_SEAT_RANK (TK_MATCH_RELAY_MSG_BEGIN + 60)
/////////// REQ
// TK_PLAYER_MSG_RELAY_HDR hdr;

/////////// ACK
// TK_PLAYER_MSG_RELAY_HDR hdr;
// WORD wGameCount;
// WORD wXmlOffset;
// WORD wXmlLen;
// char * xml

//-----------------------------------------------------------------------------------------
//   功    能：自建桌结算账单(行牌过程中的得分汇总)
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_BILL (TK_MATCH_RELAY_MSG_BEGIN + 61)
/////////// REQ
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD dwTableID;

/////////// ACK
// TK_PLAYER_MSG_RELAY_HDR hdr;
// TKFdTableBill bill;		// 自建桌结算单
struct TKFdTableBill
{
    struct PlayerBill
    {
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // 玩家昵称
        int nBuyInChip;                       // 买入积分
        int nMatchScore;                      // 积分输赢
        int nTotalScore;                      // 总积分
        int nStatus;                          // 用户状态(1、就座    2、站起    3、离开    4、旁观)
    };

    int cnPlayer;  // 后跟cnPlayer个PlayerBill
    int nGrade;    // 级别
};

//-----------------------------------------------------------------------------------------
//   功    能：自建桌结算账单(最终结束账单)
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_FINAL_BILL (TK_MATCH_RELAY_MSG_BEGIN + 62)
/////////// REQ
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD dwTableID;

/////////// ACK
// TK_PLAYER_MSG_RELAY_HDR hdr;
// TKFdTableFinalBill bill;		// 自建桌结算单
struct TKFdTableFinalBill
{
    struct PlayerBill
    {
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // 玩家昵称
        int nBuyInChip;                       // 买入积分筹码
        int nMatchScore;                      // 积分输赢
        int nTotalScore;                      // 总积分
    };

    int cnPlayer;         // 后跟cnPlayer个PlayerBill
    int nGrade;           // 级别
    int nTableTime;       // 时长(分钟)
    int nTotalGameCount;  // 总局数
};

//-----------------------------------------------------------------------------------------
//   功    能：自建桌桌主审批
//   说    明：MS -> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_PERMITSET (TK_MATCH_RELAY_MSG_BEGIN + 63)
//ACK
//TK_PLAYER_MSG_RELAY_HDR hdr;
//char szNickName[TK_BUFLEN_NICKNAME]; // 玩家昵称
//DWORD dwUserID;
//int nSeat;
//int nWaitPermitSet

//REQ
//TK_PLAYER_MSG_RELAY_HDR hdr;      //header.dwParam: 0: 不同意 1: 同意
//DWORD dwUserID;
//int nSeat;
//int nCancel; //1:取消

//-----------------------------------------------------------------------------------------
//   功    能：自建桌局间数据
//   说    明：MS -> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_SCORE (TK_MATCH_RELAY_MSG_BEGIN + 64)
// TK_PLAYER_MSG_RELAY_HDR hdr;
// TKFdTableScore score;    //得分
struct TKFdTableScore
{
    struct PlayerScore
    {
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // 玩家昵称
        int nMatchScore;                      // 本局得分
        int nTotalScore;                      // 总积分
    };
    int cnPlayer;                             // 后跟cnPlayer个PlayerScore
    int nGrade;                               // 级别
    int nTotalGameCount;                      // 总局数
    char szTitle[TK_BUFLEN_TOURNEYFULLNAME];  //赛事名称
    DWORD dwTableID;
};

//-----------------------------------------------------------------------------------------
//   功    能：断线重回推送自建桌属性信息
//   说    明：MS -> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_INFO (TK_MATCH_RELAY_MSG_BEGIN + 65)
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwTableID;
//int nGrade;
//int nInitScore;
//int nBase;
//int nGameTime;
//int nPermitSet;
//time_t tCreateTime;
//DWORD dwTableHostUserID;
//int nBuyInCost;
//int nMinChip;

//-----------------------------------------------------------------------------------------
//   功    能：牌桌提前解散
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_PREGAMEEND (TK_MATCH_RELAY_MSG_BEGIN + 66)
//REQ/ACK
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwTableID;

//-----------------------------------------------------------------------------------------
//   功    能：房主移交(服务器推送)(game)
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDROOM_ROOMHOST_INHERIT (TK_MATCH_RELAY_MSG_BEGIN + 67)
//REQ
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwRoomID;
//DWORD dwRoomHostID;
//DWORD dwInheritRoomHostID;
//int nReason;

//-----------------------------------------------------------------------------------------
//   功    能：房间转换(game)
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDROOM_TRANSFER (TK_MATCH_RELAY_MSG_BEGIN + 68)
//REQ
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwRoomID;
//WORD wReserve;

//ACK
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwRoomID;
//WORD wReserve;

//-----------------------------------------------------------------------------------------
//   功    能：更新超级庄排队列表
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_PUSH_SUPERBANKER_LIST (TK_MATCH_RELAY_MSG_BEGIN + 69)
// REQ
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD dwWaitCount;
// 后跟dwWaitCount个MS2GSBankerInfo

struct MS2GSBankerInfo
{
    DWORD dwUserID;
    DWORD dwFormID;  // 用户形象ID
    int nFigureID;   // 头像ID
    char szNickName[TK_BUFLEN_NICKNAME];
};

//-----------------------------------------------------------------------------------------
//   功    能：自建桌局间数据扩展
//   说    明：MS -> GS
//   备    注：兼容TKID_MS2GS_FDTABLE_SCORE协议
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_SCOREEX (TK_MATCH_RELAY_MSG_BEGIN + 70)
// TK_PLAYER_MSG_RELAY_HDR hdr;
// TKFDTABLESCOREEX scoreex;    //得分
// 后面跟cnPlayer个PlayerScore

typedef struct tagTKFDTABLESCOREEX
{
    int nOffsetInfo;  // =sizeof(TKFDTABLESCOREEX)
    int cnPlayer;     // 后跟cnPlayer个PlayerScore
    tagTKFDTABLESCOREEX()
    {
        memset(this, 0, sizeof(*this));
    }

    struct PlayerScore
    {
        int nSizeofInfo;  // =sizeof(PlayerScore)
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // 玩家昵称
        int nMatchScore;                      // 本局得分
        int nTotalScore;                      // 总积分
        int nWinCount;                        // 获胜局数
        int nTotalCount;                      // 总局数
        PlayerScore()
        {
            memset(this, 0, sizeof(*this));
        }
    };
} TKFDTABLESCOREEX, *PTKFDTABLESCOREEX;

//-----------------------------------------------------------------------------------------
//   功    能：自建桌手动开赛
//   说    明：MS <-> GS <-> GC
//   备    注：
//-----------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_MANUALBEGIN (TK_MATCH_RELAY_MSG_BEGIN + 71)
//REQ/ACK
//TK_PLAYER_MSG_RELAY_HDR hdr;

/************************************************************************/
/*                                                               */
//功能：GC通知用户操作信息
//创建：董龙洋
//时间：2018-06-27
//说明：GC->GameSvr -> MatchSvr
/************************************************************************/
#define TKID_GS2MS_GAME_HDL_INFO (TK_MATCH_RELAY_MSG_BEGIN + 72)
//REQ/ACK
//TK_PLAYER_MSG_RELAY_HDR hdr;
// dwResult   0：表示任务结束；1：未结束
// sufData    用户操作过程数据

/************************************************************************/
/*                                                               */
//功能：MS通知客户端过程奖励内容
//创建：郝炳常
//时间：2018-11-15
//说明：MS <-> GS <-> GC
/************************************************************************/
#define TKID_MS2GS_GAME_PROCESSAWARD (TK_MATCH_RELAY_MSG_BEGIN + 73)
//REQ
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwCount;// 循环体个数
//---循环体内容----
//DWORD dwUserID;
//DWORD dwBizID
//DWORD dwAwardID
//DWORD dwLen
//json  award
//------end-------
//...

/************************************************************************/
/*                                                               */
//功能：GC通知MS拆红包
//创建：郝炳常
//时间：2018-11-15
//说明：GC <-> GS <-> MS
/************************************************************************/
#define TKID_GS2MS_GAME_OPENREDPACKET (TK_MATCH_RELAY_MSG_BEGIN + 74)
//REQ
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwRPID;
//DWORD dwBizID;
//DWORD dwPlanID;
//BOOL	bFinal;

/************************************************************************/
/*                                                               */
//功能：GC通知MS拆红包
//创建：郝炳常
//时间：2019-01-09
//说明：GC <-> GS <-> MS
/************************************************************************/
#define TKID_GS2MS_GAME_CONFIRMAWARDINFO (TK_MATCH_RELAY_MSG_BEGIN + 75)
//REQ
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwCount;
//后缀dwCount个（dwAwardID+dwBizID+dwPlanID）

//ACK
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwCount;
//后缀dwCount个dwAwardID

/************************************************************************/
/*                                                               */
//功能：GC与MS间的透传通用消息，新增透传消息只需新增定义dwProtocolType及后缀数据格式，无需再升级中转
//创建：郝炳常
//时间：2019-05-20
//说明：GC <-> GS <-> MS
/************************************************************************/
#define TKID_GS2MS_GAME_COMMONPROTOCOL (TK_MATCH_RELAY_MSG_BEGIN + 76)
//REQ/ACK
//TK_PLAYER_MSG_RELAY_HDR hdr;
//DWORD dwProtocolType;		//区分具体的消息
//TKSUFFIXIDX suffixJsonData; 后缀业务数据
//json

/************************************************************************/
/*
// 功能:游戏过程中，提前淘汰某个玩家
// MS<-->GS
*/
/************************************************************************/

#define TKID_MS2GS_PLAYER_GAME_KICKOUT (TK_MATCH_RELAY_MSG_BEGIN + 77)
// REQ/ACK:
// TK_PLAYER_MSG_RELAY_HDR hdr;
// DWORD dwOrder

//客户端与比赛服务间的透传通用协议
#define GC_MS_COMMONPROTOCOL_QUIT_MATCH 0                       // 客户端请求退赛
#define GC_MS_COMMONPROTOCOL_PAYFEECONTINUE_MATCH 1             // 用户付费对局
#define GS_MS_COMMONPROTOCOL_EXIT_GAME 2                        // 用户已退出游戏
#define GS_MS_COMMONPROTOCOL_CHAPTER_AWARD 3                    // 闯关赛进度奖励
#define GS_MS_COMMONPROTOCOL_PAUSE_CHAPTER 4                    // 暂停闯关
#define GS_MS_COMMONPROTOCOL_GROUP_INFO 5                       // 复式赛里，MS通知客户端牌组信息
#define GS_MS_COMMONPROTOCOL_CARD_INFO 6                        // 复式赛里，MS通知客户端单副牌结果
#define GS_MS_COMMONPROTOCOL_PK_INFO 7                          // 复式赛里，MS通知客户端PK结果
#define GS_MS_COMMONPROTOCOL_EXCHANGE_INFO 8                    // 城堡赛，玩家当前可回兑信息
#define GS_MS_COMMONPROTOCOL_BREAKING_INFO 9                    // 闯关赛里，比赛通知客户端展示继续闯关界面
#define GS_MS_COMMONPROTOCOL_AWARD_AND_QUIT 10                  // 领取当前奖励并退出比赛
#define GS_MS_COMMONPROTOCOL_CONTINUE 11                        // 继续挑战比赛
#define GS_MS_COMMONPROTOCOL_TEAM_STATUS 12                     // 更新队伍状态
#define GS_MS_COMMONPROTOCOL_ROUND_PLAYER_INFO 13               // create round后，给客户端同步当前round的玩家信息
#define GS_MS_COMMONPROTOCOL_REFRESHDATA 14                     // 告知客户端数据有更新，需要重新获取数据
#define GS_MS_COMMONPROTOCOL_REPLAY 15                          // 重打某一局
#define GS_MS_COMMONPROTOCOL_FDTABLE_PLAYER_DISSOLVE 17         // 自建桌玩家申请解散
#define GS_MS_COMMONPROTOCOL_FDTABLE_PLAYER_DISSOLVE_STATUS 18  // 自建桌玩家解散状态
#define GS_MS_COMMONPROTOCOL_FDTABLE_PLAYER_STANDUP 19          // 自建桌玩家站起
#define GS_MS_COMMONPROTOCOL_FDTABLE_ONE_MORE_GAME 20           // 自建桌玩家再来一局
#define GS_MS_COMMONPROTOCOL_FDTABLE_FINAL_BILL 21              // 自建桌结算清单
#define GS_MS_COMMONPROTOCOL_MODIFY_GROW 22                     // 客户端通知比赛服务需要修改积分值
#define GS_MS_COMMONPROTOCOL_FDTABLE_COMMIT_RULE 23             // 自建桌房主修改规则
#define GS_MS_COMMONPROTOCOL_FDTABLE_TRANS_LOCATION 24          // 自建桌传输玩家位置信息
#define GS_MS_COMMONPROTOCOL_FDTABLE_PUSH_ROUND_DATA 25         // 自建桌玩家重连推送最近一局数据
#define GS_MS_COMMONPROTOCOL_FDTABLE_ROUND_BILLS 26             // 自建桌返回房间每局对局流水
#define GS_MS_COMMONPROTOCOL_FDTABLE_TABLE_INFO 27              // 自建桌桌子信息
#define GS_MS_COMMONPROTOCOL_SECTION_WAIT 28                    // 节间等待
#define GS_MS_COMMONPROTOCOL_MPRANK_RESULT 29                   // MP排名结果，带奖励
#define GS_MS_COMMONPROTOCOL_GAMING_RELIFE 30                   // 游戏中复活请求 GC->MS
#define GS_MS_COMMONPROTOCOL_GAMING_RELIFE_RESULT 31            // 游戏中复活结果回复	MS->GC
#define GS_MS_COMMONPROTOCOL_GAMING_RELIFE_RECHARGE 32          // 游戏中复活充值延时	GC->MS
#define GS_MS_COMMONPROTOCOL_ADDHP_64 33                        // 游戏结束加血请求64位	GC->MS请求
#define GS_MS_COMMONPROTOCOL_ADDHP_RESULT_64 34                 // 游戏结束加血请求回复64位 MS->GC请求
#define GS_MS_COMMONPROTOCOL_DYN_LEAVE_GAME 35                  // 游戏中提前离开游戏请求  GC->MS请求
#define GS_MS_COMMONPROTOCOL_GAMING_RELIFE_RECHARGE_CANCEL 36   // 游戏中复活充值延时取消  GC->MS请求
#define GS_MS_COMMONPROTOCOL_GAMING_RELIFE_INFO 37              // 游戏中复活信息获取 GC->MS请求
#define GS_MS_COMMONPROTOCOL_GAMING_RELIFE_INFO_RESULT 38       // 游戏中复活信息获取回复 MS->GC请求
#define GS_MS_COMMONPROTOCOL_GAMING_RANK_INFO_RESULT 39         // 转发排名信息至客户端  MS->GC
#define GS_MS_COMMONPROTOCOL_GAMING_RELIFE_PLAN 40              // 游戏中复活方案  MS->GC 推送
#define GS_MS_COMMONPROTOCOL_PUSH_AWARDINFO 41                  // 推送奖励内容
#define GS_MS_COMMONPROTOCOL_INDIV_PUSH_BOUTS_CHANGE 42         // 锦标赛推送轮次变更 MS->GC 推送
#define GS_MS_COMMONPROTOCOL_TIMELIMIT_PUSH_RANKLIST 43         // 限时积分赛推送排名列表信息 MS->GC 推送
#define GS_MS_COMMONPROTOCOL_TIMELIMIT_PUSH_PLYAERINFO 44       // 限时积分赛推送玩家信息 MS->GC 推送   0:比赛中，1：淘汰
#define GS_MS_COMMONPROTOCOL_TIMELIMIT_GET_RANKLIST 45          // 限时积分赛客户端获取排名请求  GC->MS 请求
#define GS_MS_COMMONPROTOCOL_PUSH_BYE_PLAYERINFO 46             // 推送轮空玩家信息
#define GS_MS_COMMONPROTOCOL_BACKTOTOP_REQ 47                   // 重回巅峰请求
#define GS_MS_COMMONPROTOCOL_BACKTOTOP_INFO 48                  // 推送重回巅峰信息
#define GS_MS_COMMONPROTOCOL_REQCONTINUE 50                     // 再来一局消息,可以携带道具信息 GC->MS 请求
#define GS_MS_COMMONPROTOCOL_PROPSINFO 51                       // 携带使用道具信息 MS->GC 推送
#define GS_MS_COMMONPROTOCOL_PROPSUSE 52                        // 道具使用信息 GC->MS 使用请求， MS->GC 使用结果推送
#define GS_MS_COMMONPROTOCOL_CHANGE_TABLE 53                    // 新换桌消息  GC->MS 请求
#define GS_MS_COMMONPROTOCOL_PLAYER_ROUND_OVER 54               // 推送玩家结算后的数据
#define GS_MS_COMMONPROTOCOL_SEARCH_CARD 55                     // 搜寻物资请求
#define GS_MS_COMMONPROTOCOL_SEARCH_CARD_RESULT 56              // 搜寻物资结果
#define GS_MS_COMMONPROTOCOL_SELECT_CARD_RESULT 57              // 玩家最终选择道具卡结果
#define GS_MS_COMMONPROTOCOL_OPEN_TREASURE 58                   // 玩家请求开箱
#define GS_MS_COMMONPROTOCOL_OPEN_TREASURE_RESULT 59            // 推送开箱结果
#define GS_MS_COMMONPROTOCOL_DISPLAY_MATCHDATA 60               // 展示比赛数据
#define GS_MS_COMMONPROTOCOL_REQUIRE_AWARD 61                   // 请求比赛颁奖
#define GS_MS_COMMONPROTOCOL_BOSS_MATCH_INFO 62                 // boss战消息推送 MS->GC
#define GS_MS_COMMONPROTOCOL_RANK_INFO_REQ 63                   // boss战排名相关信息请求 GC->MS
#define GS_MS_COMMONPROTOCOL_RANK_INFO 64                       // boss战排名相关信息回复 MS->GC
#define GS_MS_COMMONPROTOCOL_REFRESH_LEFT_PLAYER 65             // 刷新剩余人数
#define GS_MS_COMMONPROTOCOL_BOARDCAST_AWARD_MSG 66             // 中奖消息广播 MS->GC
//-------------------------------------------------------------------------------------------------------------------------
//
// 内旁观用于进入
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_WATCHER_ENTER (TK_MATCH_RELAY_MSG_END + 1)
// REQ: MatchServer --> GameServer
typedef struct tagTKReqWatcherEnter
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwWatcherID;  //旁观者ID

    DWORD dwWatcheeID;  //被旁观者ID
    char szNoUse[TK_BUFLEN_NICKNAME];
    int nWatcheeSeat;
    DWORD dwWatcheeFlags;
} TKREQ_WATCHER_ENTER, *LPTKREQ_WATCHER_ENTER;
// ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS，游戏一手牌开始了
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_GAME_BEGIN (TK_MATCH_RELAY_MSG_END + 2)
// REQ: GameServer --> MatchServer
typedef struct tagTKReqGameBegin
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    int iGameHand;
    module_identify id;
} TKREQ_GAME_BEGIN, *LPTKREQ_GAME_BEGIN;
// ACK:MatchServer --> GameServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 注入彩金
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_INJECTION_JACKPOT (TK_MATCH_RELAY_MSG_END + 3)
// REQ: GameServer --> MatchServer
typedef struct tagTKReqInjectionJackpot
{
    TKHEADER header;
    DWORD dwTourneyID;
    int nTax;
} TKREQINJECTIONJACKPOT, *PTKREQINJECTIONJACKPOT;
// ACK:MatchServer --> GameServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 某个用户中奖了
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_WINNING_JACKPOT (TK_MATCH_RELAY_MSG_END + 4)
// REQ: GameServer --> MatchServer
typedef struct tagTKReqWinningJackpot
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];

    char szWinningReason[64];  // 中奖原因
    int nJackpotCalcType;      // 彩金计算方式(以百分比方式还是以固定金额方式，0直接数额，1百分比，2直接使用nWinning且不用再发放)
    int nWinning;              // 金额,如果是百分比方式需要除以100以得到实际比例
    BOOL bBroadcast;           // 是否需要全场广播
    WORD wCntTID;
} TKREQWINNINGJACKPOT, *PTKREQWINNINGJACKPOT;
// ACK:MatchServer --> GameServer (消息包TKHEADER)
#define JACKPOT_CALCTYPE_VALUE 0          // 直接发放
#define JACKPOT_CALCTYPE_PERCENT 1        // 百分比发放
#define JACKPOT_CALCTYPE_ONLYBROADCAST 2  // 只是广播

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知GS广播当前彩金数量
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_JACKPOT_COUNT (TK_MATCH_RELAY_MSG_END + 5)
// REQ: MatchServer --> GameServer
typedef struct tagTKReqJackpotCount
{
    TKHEADER header;
    DWORD dwMatchID;
    DWORD dwUserID;
    int nJackpotCount;
} TKREQJACKPOTCOUNT, *PTKREQJACKPOTCOUNT;
// ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户进入时，通知该用户当前彩金数量和彩金中奖规则
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_JACKPOT (TK_MATCH_RELAY_MSG_END + 6)
// REQ: MatchServer --> GameServer
typedef struct tagTKReqJackpot
{
    TKHEADER header;
    DWORD dwMatchID;
    DWORD dwUserID;
    int nJackpotCount;
    // 后跟彩金中奖规则描述字符串
} TKREQJACKPOT, *PTKREQJACKPOT;
// ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知用户当前彩金中奖记录
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_JACKPOT_WINNER (TK_MATCH_RELAY_MSG_END + 7)
// REQ: MatchServer --> GameServer
typedef struct tagTKReqJackpotWinner
{
    TKHEADER header;
    DWORD dwMatchID;
    DWORD dwUserID;
    int cnWinner;
    // 后跟cnWinner个中奖者信息,以逗号分隔
} TKREQJACKPOTWINNER, *PTKREQJACKPOTWINNER;
// ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户请求锁定选手（比如象棋岛屿）
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_LOCKDOWN (TK_MATCH_RELAY_MSG_END + 8)
// REQ: GameServer --> MatchServer
typedef struct tagTKReqLockDown
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    BOOL bUnlock;
} TKREQLOCKDOWN, *PTKREQLOCKDOWN;

//-------------------------------------------------------------------------------------------------------------------------
//
// 游戏者进入成功（用于TableStage）
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_PLAYER_ENTERSUCCESS (TK_MATCH_RELAY_MSG_END + 9)
// REQ: MatchServer --> GameServer
typedef struct tagTKReqPlayerEnterSuccess
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwUserID;
} TKREQ_PLAYER_ENTERSUCCESS, *LPTKREQ_PLAYER_ENTERSUCCESS;
// ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 内旁观用于离开
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_WATCHER_LEAVE (TK_MATCH_RELAY_MSG_END + 10)
// REQ: MatchServer --> GameServer
typedef struct tagTKReqWatcherLeave
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    DWORD dwWatcherID;  //旁观者ID
} TKREQ_WATCHER_LEAVE, *LPTKREQ_WATCHER_LEAVE;
// ACK:GameServer --> MatchServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// WebGuest取得成绩，通知注册
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_NOTIFY_GUESTREGISTER_AUTO (TK_MATCH_RELAY_MSG_END + 11)
typedef struct tagTKMATCHNOTIFYGUESTREGISTERAUTO
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwWebGuestID;

    DWORD dwOrder;       // 获得名次
    BOOL bAutoRegister;  // 是否由服务器自动注册
} TKMATCHNOTIFYGUESTREGISTERAUTO, *PTKMATCHNOTIFYGUESTREGISTERAUTO;

//-------------------------------------------------------------------------------------------------------------------------
//
// WebGuest隐式注册请求
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_WEBREGISTER_AUTO (TK_MATCH_RELAY_MSG_END + 12)
typedef struct tagTKREQWEBGUESTMATCHREGISTERAUTO
{
    TKHEADER header;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwWebGuestID;

    DWORD dwPartnerID;  // 合作厂商ID

    // 推广站点市场编号
    DWORD dwAgent;                     // 注册来源
    char szAgentMark[TK_BUFLEN_NOTE];  // 代理商注释
    BOOL bRegister;                    // 是否同意用自动注册

    //如果填写了以下信息，就用以下信息注册
    char szNickName[TK_BUFLEN_NICKNAME];
    char szUserName[TK_BUFLEN_USERNAME];
    char szPassword[TK_BUFLEN_PASSWORD];
    DWORD dwFigureID;

    //
    char szIP[TK_BUFLEN_IP];

    char szCltIdent[TK_BUFLEN_NOTE];  // 客户端唯一标识
    DWORD dwCustomID;                 // 推广客户ID
    WORD wStageID;

} TKREQWEBGUESTMATCHREGISTERAUTO, *PTKREQWEBGUESTMATCHREGISTERAUTO;
typedef struct tagTKACKWEBGUESTMATCHREGISTERAUTO
{
    TKHEADER header;
    DWORD dwUserID;  //注册成功后的UserID
    char szNickName[TK_BUFLEN_NICKNAME];
    char szUserName[TK_BUFLEN_USERNAME];
    char szPassword[TK_BUFLEN_PASSWORD];
    DWORD dwFigureID;
    DWORD dwBankAccID;  // 系统账单

    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwWebGuestID;
} TKACKWEBGUESTMATCHREGISTERAUTO, *PTKACKWEBGUESTMATCHREGISTERAUTO;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS推送复式赛游戏结果
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_DUPLEX_RESULT (TK_MATCH_RELAY_MSG_END + 13)
typedef struct tagTKREQDUPLEXRESULT
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD awRoundID[2];
    int lenText;
} TKTKREQDUPLEXRESULT, *PTKREQDUPLEXRESULT;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS推送玩家信息字段定义
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_PLAYERINFO_FIELD_DEFINE (TK_MATCH_RELAY_MSG_END + 14)
typedef struct tagTKREQPLAYERINFOFIELDDEFINE
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int lenText;
} TKREQPLAYERINFOFIELDDEFINE, *PTKREQPLAYERINFOFIELDDEFINE;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS推送更新玩家信息
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_UPDATE_PLAYERINFO (TK_MATCH_RELAY_MSG_END + 15)
typedef struct tagTKTKREQUPDATEPLAYERINFO
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    int lenText;
} TKTKREQUPDATEPLAYERINFO, *PTKTKREQUPDATEPLAYERINFO;

//-------------------------------------------------------------------------------------------------------------------------
//
// GS通知MS mvs重启
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_MVS_RESTART (TK_MATCH_RELAY_MSG_END + 16)
typedef struct tagTKREQMVSRESTART
{
    TKHEADER header;
    DWORD dwMatchID;
} TKREQMVSRESTART, *PTKREQMVSRESTART;

//-------------------------------------------------------------------------------------------------------------------------
//
// 用户请求比赛进度存档
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_MATCHARCHIVE (TK_MATCH_RELAY_MSG_END + 17)
typedef struct tagTKREQMATCHARCHIVE
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;
    BYTE byArchive;
} TKREQMATCHARCHIVE, *PTKREQMATCHARCHIVE;
typedef struct tagTKACKMATCHARCHIVE
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;
} TKACKMATCHARCHIVE, *PTKACKMATCHARCHIVE;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS给GS推送更新比赛配置
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_UPDATE_MATCHCONFIG (TK_MATCH_RELAY_MSG_END + 18)
typedef struct tagTKREQUPDATEMATCHCONFIG
{
    TKHEADER header;
    DWORD dwMatchID;
    BOOL bEnableDataCollect;  // 是否开启数据采集
    DWORD adwReserve[10];
} TKREQUPDATEMATCHCONFIG, *PTKREQUPDATEMATCHCONFIG;

//-------------------------------------------------------------------------------------------------------------------------
//
// 功能：游戏服务通知比赛服务器获取牌权
// 流程：GS->MS
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_GETCARDRIGHT (TK_MATCH_RELAY_MSG_END + 19)
typedef struct tagTKREQGETCARDRIGHT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wGetCardRightMode;  // 获取牌权方式，0-玩家主动请求切换牌权，1-按副请求切换牌权
    int nGameHand;
    view_identify view_id;
    WORD wReserve;

    //后缀数据
    WORD wPlayerCount;
    TKSUFFIXIDX sufData;
    struct switchPlayer
    {
        DWORD dwUserId;
        WORD wReason;
        DWORD dwWatcheeID;
        DWORD dwWatcheeFlags;
        int nWatcheeSeat;
    };

    inline switchPlayer* player_list()
    {
        return (switchPlayer*)((char*)this + sufData.dwSuffixOffSet);
    }

} TKREQGETCARDRIGHT, *PTKREQGETCARDRIGHT;

//-------------------------------------------------------------------------------------------------------------------------
//
//  功能：比赛服务通知游戏服务器切换牌权
//  流程：MS->GS
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_SWITCHCARDRIGHT (TK_MATCH_RELAY_MSG_END + 20)
typedef struct tagTKREQSWITCHCARDRIGHT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wSwitchCardRightMode;  // 切换牌权方式，0-玩家主动切换牌权，1-按副切换牌权，2-玩家退出比赛切换牌权
    WORD wReserve;

    //后缀数据
    WORD wPlayerCount;
    TKSUFFIXIDX sufData;
    struct switchPlayer
    {
        DWORD dwUserId;
        WORD wReason;
        DWORD dwWatcheeID;
        DWORD dwWatcheeFlags;
        int nWatcheeSeat;
    };

    inline switchPlayer* player_list()
    {
        return (switchPlayer*)((char*)this + sufData.dwSuffixOffSet);
    }

} TKREQSWITCHCARDRIGHT, *PTKREQSWITCHCARDRIGHT;

//-------------------------------------------------------------------------------------------------------------------------
//
//  功能：游戏服务器通知比赛服务游戏服务出现故障
//  流程：GS->MS
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_NOTIFY_GSERROR (TK_MATCH_RELAY_MSG_END + 21)
typedef struct tagTKREQNOTIFYGSERROR
{
    TKHEADER header;
    DWORD dwProductID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
} TKREQNOTIFYGSERROR, *PTKREQNOTIFYGSERROR;

//-------------------------------------------------------------------------------------------------------------------------
//
// 通知MS修改用户的积分数值
// 流向：GS->MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_MODIFYPLAYERGROWINFO (TK_MATCH_RELAY_MSG_END + 22)
typedef struct tagTKREQMODIFYPLAYERGROWINFO
{
    TKHEADER header;

    DWORD dwProductID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwGrowInfoOffset;  // =sizeof(TKREQMODIFYPLAYERGROWINFO)
    DWORD dwGrowInfoCount;   // 积分个数
    WORD wGrowDomainID;      //积分域ID，要修改的GrowID需要在积分域ID下才能成功

    DWORD dwReserve1;  // 保留1
    DWORD dwReserve2;  // 保留2
} TKREQMODIFYPLAYERGROWINFO, *PTKREQMODIFYPLAYERGROWINFO;
//后跟dwGrowInfoCount个GrowInfo(DWORD GrowID + DWORD dwGrowValue)
//ACK:MatchServer-->GameServer (消息包TKHEADER)

//-------------------------------------------------------------------------------------------------------------------------
//
// 保存牌局
// 流向：GS->MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_GS2MS_GAMEROUND_PAUSE (TK_MATCH_RELAY_MSG_END + 23)
typedef struct tagTKREQGAMEROUNDPAUSE
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wOverType;
    WORD wOverFlags;
    DWORD dwReserve1;  // 保留字段1
    DWORD dwReserve2;  // 保留字段2
    DWORD dwReserve3;  // 保留字段3
    DWORD dwReserve4;  // 保留字段4

    DWORD dwMatchGameDataOffSet;  // 比赛游戏牌局信息
    DWORD dwMatchGameDataLenght;

} TKREQGAMEROUNDPAUSE, *PTKREQGAMEROUNDPAUSE;

//-------------------------------------------------------------------------------------------------------------------------
//
// 继续牌局
// 流向：MS->GS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_ROUND_RESUME (TK_MATCH_RELAY_MSG_END + 24)
typedef TKREQCREATEROUND TKREQROUNDRESUME;

//-------------------------------------------------------------------------------------------------------------------------
//
// MS通知GS比赛时间到了
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_MATCHTIMEISOUT (TK_MATCH_RELAY_MSG_END + 25)
typedef struct tagTKREQMATCHTIMEISOUT
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwUserID;

    DWORD dwReserve1;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    DWORD dwReserve5;

} TKREQMATCHTIMEISOUT, *PTKREQMATCHTIMEISOUT;

//-------------------------------------------------------------------------------------------------------------------------
//
// 自建桌桌子时间到
// 流向：MS->GS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MS2GS_FDTABLE_TIMEOUT (TK_MATCH_RELAY_MSG_END + 26)
typedef struct tagTKReqFdTableTimeOut
{
    TKHEADER header;
    int nGameID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    DWORD dwTableID;
} TKREQ_FDTABLE_TIMEOUT, *LPTKREQ_FDTABLE_TIMEOUT;
