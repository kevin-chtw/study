#pragma once

#include <stdint.h>
#include "TKProtocol.h"
//#include "ms_mvs_gs_mc_share_define.h"
#include "TKMatchS2GameSProtocolSrv.h"
//#include "TKSvrShareDef.h"

//-----------------------------------------------------------------------------------------
//   功    能：取消当前局 重新开始
//   说    明：MGR -> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RESETCURGAME (TK_MSG_GAME + 22)
typedef struct tagTKREQGAMERESETCURGAME
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
} TKREQGAMERESETCURGAME, *PTKREQGAMERESETCURGAME;
//ACK:TKHEADER

//-----------------------------------------------------------------------------------------
//   功    能：请求BotServer创建Bot
//   说    明：GameServer --> BotServer
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2BOTS_CREATEBOT64 (TK_MSG_GAME + 29)
typedef struct tagTKREQGAMECREATEBOT64
{
    TKHEADER header;

    char szGameServerIP[TK_BUFLEN_IP];
    int nGameServerPort;

    DWORD dwUserID;
    int nAILevel;  //机器人只能水平,0最低，越大AI越高

    DWORD dwMatchID;
    DWORD dwCheck;

    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;
    int64_t nScore;  //比赛积分

    DWORD dwFlags;
    int nGameID;
    DWORD dwProductID;
    BOOL bPacerBot;     // 陪打机器人
    DWORD dwAIVersion;  //AI版本号
    BYTE byAIType;      //陪打机器人类型
    BYTE byReserve[3];
    DWORD dwReserve[4];
} TKREQGAMECREATEBOT64, *PTKREQGAMECREATEBOT64;
//ACK:TKHEADER
#define TKID_GS2BOTS_CREATEBOT (TK_MSG_GAME + 30)
typedef struct tagTKREQGAMECREATEBOT
{
    TKHEADER header;

    char szGameServerIP[TK_BUFLEN_IP];
    int nGameServerPort;

    DWORD dwUserID;
    int nAILevel;  //机器人只能水平,0最低，越大AI越高

    DWORD dwMatchID;
    DWORD dwCheck;

    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;
    int nScore;  //比赛积分

    DWORD dwFlags;
    int nGameID;
    DWORD dwProductID;
    BOOL bPacerBot;     // 陪打机器人
    DWORD dwAIVersion;  //AI版本号
    BYTE byAIType;      //陪打机器人类型
    BYTE byReserve[3];
    DWORD dwReserve[4];
} TKREQGAMECREATEBOT, *PTKREQGAMECREATEBOT;
//ACK:TKHEADER

//-----------------------------------------------------------------------------------------
//   功    能：通知GS AI算法调用异常
//   说    明：BotServer --> GameServer
//
//-----------------------------------------------------------------------------------------
#define TKID_BOTS2GS_AIDISPATCH_ERROR (TK_MSG_GAME + 31)
typedef struct tagTKREQAIDISPATCHERROR
{
    TKHEADER header;
    DWORD dwMatchID;
} TKREQAIDISPATCHERROR, *PTKREQAIDISPATCHERROR;
//ACK:TKHEADER

/*////////////////////////////

游戏机制消息流程-Player进入：
Game <- REQ Enter <------------------- Client
Game -> Ack Enter -------------------> Client
Game -> Ack PlayerInfo(Send多条) ----> Client
Game -> Ack PlayerArrived(Broast) ---> Client
Game -> Ack BeginHand(Broast) -------> Client

游戏机制消息流程-Player断线：
Game -> Ack PlayerNetBreak(Broast) --> Client

游戏机制消息流程-Player重连：
Game <- REQ Enter <------------------- Client
Game -> Ack Enter -------------------> Client
Game -> Ack PlayerInfo(Send多条) ----> Client
Game -> Ack PlayerArrived(Broast) ---> Client
Game -> Ack PlayerNetResume(Broast) -> Client

/*////////////////////////////

//-----------------------------------------------------------------------------------------
//   功    能：比赛客户端启动进入比赛游戏服务器
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_ENTERMATCH (TK_MSG_GAME + 70)
typedef struct tagTKREQCLIENTENTENTERMATCH
{
    TKHEADER header;
    DWORD dwVersionMatchClient;  //比赛客户端版本
    int lenEnterParam;
    char szEnterParam[sizeof(ENTERROUNDPARAM) + 8];
    DWORD dwVersionGameDll;  //游戏动态库版本
} TKREQCLIENTENTENTERMATCH, *PTKREQCLIENTENTENTERMATCH;
typedef struct tagTKACKCLIENTENTENTERMATCH
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    char szMatchName[TK_BUFLEN_TOURNEYFULLNAME];  // 比赛名称

    DWORD dwUserID;  //用户帐号对应的唯一数字编号ID
    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;   //用户头像ID
    int nMatchRank;  //游戏服务器根据比赛积分计算出来的

    int nTotalMatchPlayer;  //总参数人数
    time_t timeMatchStart;  //比赛开始时间
    int nMatchLapseSecond;  //比赛已经进行的秒数

    int nTitleStyle;  //比赛标题类型，0锦标赛显示为“比赛名称+阶段/轮次名称”，1自由桌显示为“第%d局”
    int nProductID;
    DWORD dwTeamID;  //用户加入一场比赛的TeamID,自由桌换桌和结果班车报名下一场会是不同的TeamID,旁观为0
    DWORD dwFormID;  //用户形象ID

    time_t timeStamp;  // 游戏服务器时间戳
} TKACKCLIENTENTENTERMATCH, *PTKACKCLIENTENTENTERMATCH;

//-----------------------------------------------------------------------------------------
//   功    能：比赛客户端直接退出比赛游戏服务器
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_EXITMATCH (TK_MSG_GAME + 71)
typedef struct tagTKREQCLIENTEXITMATCH : public tagTKREQCLIENTENTENTERMATCH
{
} TKREQCLIENTEXITMATCH, *PTKREQCLIENTEXITMATCH;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送当前比赛进行的阶段轮次和名称
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHSTAGEPOSITION (TK_MSG_GAME + 72)
typedef struct tagTKACKGSPUSHMATCHSTAGEPOSITION
{
    TKHEADER header;
    WORD wCurStageID;
    int nCurBoutID;
    char szStageBoutName[TK_BUFLEN_TOURNEYFULLNAME];
    char szStageName[TK_BUFLEN_STAGENAME];
    char szBoutName[TK_BUFLEN_STAGENAME];
} TKACKGSPUSHMATCHSTAGEPOSITION, *PTKACKGSPUSHMATCHSTAGEPOSITION;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送比赛状况文字播报
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHTEXTPLAY (TK_MSG_GAME + 73)
typedef struct tagTKACKGSPUSHMATCHTEXTPLAY
{
    TKHEADER header;
    int nTextLength;  //播报文字长度(包含结束符\0)
} TKACKGSPUSHMATCHTEXTPLAY, *PTKACKGSPUSHMATCHTEXTPLAY;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送用户当前比赛排名位置
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHUSERPLACEORDERPOSITION (TK_MSG_GAME + 76)
typedef struct tagTKACKGSPUSHUSERPLACEORDERPOSITION
{
    TKHEADER header;
    int nPlayerCurPlaceOrder;  //用户当前排名
    int nTotalPlayerCount;     //当前总选手数
} TKACKGSPUSHUSERPLACEORDERPOSITION, *PTKACKGSPUSHUSERPLACEORDERPOSITION;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送当前局制规则信息
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHROUNDRULERINFO (TK_MSG_GAME + 77)
typedef struct tagTKACKGSPUSHROUNDRULERINFO
{
    TKHEADER header;
    int lenText;  //文本长度 包含结束符\0
} TKACKGSPUSHROUNDRULERINFO, *PTKACKGSPUSHROUNDRULERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送用户颁奖信息
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHAWARD (TK_MSG_GAME + 78)
typedef struct tagTKACKGSPUSHMATCHAWARD
{
    TKHEADER header;
    int nTextLength;  //颁奖文字长度(包含结束符\0)
} TKACKGSPUSHMATCHAWARD, *PTKACKGSPUSHMATCHAWARD;
//header.dwParam==0 给出奖状界面(点确定按钮后奖状收放到左下方，不关闭客户端)
//header.dwParam==1 给出奖状界面(点确定按钮后关闭客户端)

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送比赛选手信息
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHPLAYERINFO (TK_MSG_GAME + 79)
typedef struct tagTKACKGSPUSHMATCHPLAYERINFO
{
    TKHEADER header;
    int nTextLength;  //选手信息xml文本长度(包含结束符\0)
} TKACKGSPUSHMATCHPLAYERINFO, *PTKACKGSPUSHMATCHPLAYERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送游戏盘数累进奖励配置信息
//   说    明：GS --> GC
/*   xml格式如下
<GameCountAward Note="根据游戏盘数的奖励或者积分加倍等设置信息">
	<!--盘数进度style说明：style为1表示满足max就回到min。2表示满足max就应用下一条ruler，如果没有下一条就应用最后一条-->
	<Item id="1" name="天降喜金" style="1" Note="每完成8副牌就有机会获得天降喜金">
		<Ruler min="0" max="8"/>
	</Item>
	<Item id="2" name="铁戒指" style="1" Note="每完成8副牌就有机会获得1个铁戒指">
		<Ruler min="0" max="10"/>
	</Item>
	<Item id="3" name="参赛积分" style="1" Note="每完成8副牌就有机会获得100个参赛积分">
		<Ruler min="0" max="10"/>
	</Item>
	<Item id="4" name="积分加倍" style="2" Note="每完成10副牌积分加倍,最高4倍">
		<Ruler name="1倍积分" min="0" max="10"/>
		<Ruler name="2倍积分" min="10" max="20"/>
		<Ruler name="3倍积分" min="20" max="30"/>
		<Ruler name="4倍积分" min="30" max="40"/>
	</Item>
</GameCountAward>
*/
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHGAMECOUNTAWARDINFO (TK_MSG_GAME + 80)
typedef struct tagTKACKGSPUSHGAMECOUNTAWARDINFO
{
    TKHEADER header;
    int lenText;  //xml文本长度 包含结束符\0
} TKACKGSPUSHGAMECOUNTAWARDINFO, *PTKACKGSPUSHGAMECOUNTAWARDINFO;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送游戏数据信息
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHPLAYERGAMEDATA (TK_MSG_GAME + 81)
typedef struct tagTKACKGSPUSHPLAYERGAMEDATA
{
    TKHEADER header;

    DWORD dwUserID;

    int nExchangeRate;  // 兑换率
    int nLeftChip;      // 剩余筹码或者当前生命值

    int iCurGameHand;             // 用户当前进行的局制编号：第%d局（开局时累计。为0是表示没有打过，不显示）
    int nTotalFinishedGameCount;  // 已经完成的总有效局数（局制结束时累加。用于局数奖励显示）
    int nCurFinishedGameCount;    // 当前阶段完成的有效局数（局制结束时累加。用于积分加倍显示）
} TKACKGSPUSHPLAYERGAMEDATA, *PTKACKGSPUSHPLAYERGAMEDATA;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器下发给比赛客户端的动画xml文本
//   说    明：GS --> GC
/*
<action bmp="1">
	<stage name="预赛" bmp="10" />
	<ruler name="打立出局" bmp="100"/>
	<note>
		<line>
			<Item str="24" cr="11,22,33"/>
			<Item str=" 人晋级" cr="11,22,33"/>
			<Item str="    " cr="11,22,33"/>
			<Item str="30" cr="11,22,33"/>
			<Item str=" 人截止" cr="11,22,33"/>
		</line>
		<line>
			<Item str="前3名有机会获得天降秋卡" cr="11,22,33"/>
		</line>
	</note>
</action>
*/
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHACTION (TK_MSG_GAME + 82)
typedef struct tagTKACKGSPUSHMATCHACTION
{
    TKHEADER header;
    int nActionID;
    int lenText;  //xml文本长度 包含结束符\0
} TKACKGSPUSHMATCHACTION, *PTKACKGSPUSHMATCHACTION;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送阶段排名结果信息
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHSTAGEBOUTRESULT (TK_MSG_GAME + 83)
typedef struct tagTKACKGSPUSHSTAGEBOUTRESULT
{
    TKHEADER header;
    int lenText;  //文本长度 包含结束符\0
} TKACKGSPUSHSTAGEBOUTRESULT, *PTKACKGSPUSHSTAGEBOUTRESULT;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送局制分信息
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHROUNDSCOREINFO (TK_MSG_GAME + 84)
typedef struct tagTKACKGSPUSHROUNDSCOREINFO
{
    TKHEADER header;
    int lenText;  //文本长度 包含结束符\0
} TKACKGSPUSHROUNDSCOREINFO, *PTKACKGSPUSHROUNDSCOREINFO;

////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------
//   功    能：游戏客户端进入Round
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ENTERROUND (TK_MSG_GAME + 100)
typedef struct tagTKREQGAMEENTERROUND
{
    TKHEADER header;
    DWORD dwVersion;  //客户端版本
    int lenEnterParam;
    char szEnterParam[sizeof(ENTERROUNDPARAM) + 8];
} TKREQGAMEENTERROUND, *PTKREQGAMEENTERROUND;
#define TK_NETOPERATOR_PLAYER 0      //用户操作游戏
#define TK_NETOPERATOR_BOT 1         //机器人操作游戏
#define TK_ROUND_FLAG_HIDE_PLAYER 1  // 隐名
typedef struct tagTKACKGAMEPLAYERENTER
{
    TKHEADER header;
    DWORD dwUserID;           //用户帐号对应的唯一数字编号ID
    int nSeatOrder;           //座位编号
    int nUserType;            //用户类型，0游戏者Player，1观众Wather，2旁观者Spectator
    int nMaxPlayersPerTable;  //每桌最大人数
    DWORD flag;
} TKACKGAMEPLAYERENTER, *PTKACKGAMEPLAYERENTER;
#define TK_GAMEPLAYERTYPE_PLAYER 0     //游戏者
#define TK_GAMEPLAYERTYPE_WATCHER 1    //观众
#define TK_GAMEPLAYERTYPE_SPECTATOR 2  //旁观者

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发游戏规则
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RULERINFO (TK_MSG_GAME + 101)
typedef struct tagTKACKGAMERULERINFO
{
    TKHEADER header;
    int nGameTotalPlayer;                             //本桌游戏总的玩家数
    int nTableNumber;                                 //桌号（就是RoundID）
    int nScoreType;                                   //积分方式（原积分，筹码）
    int nScoreBase;                                   //游戏基数，比方400*1.60，只在混战中使用
    char szRoundRulerName[TK_BUFLEN_ROUNDRULERNAME];  //局制名称

    int nReserve;                                //nHandTime;								//每手牌的时间，比方15秒，现在不支持总时间方案
    char szProperty[TK_MAX_GAMEPROPLEN];         //游戏相关附加属性，描述游戏规则，游戏服务器自行解释，对复杂游戏可能会有
    char szTitleName[TK_BUFLEN_CLIENTTITLE];     //客户端标题字符串
    char szRulerNote[TK_BUFLEN_ROUNDRULERNOTE];  //游戏局制规则说明：N局积分最高者获胜 / 低于****分将被淘汰

    int nRoundRuler;  // 赛制
} TKACKGAMERULERINFO, *PTKACKGAMERULERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发本桌Player信息（客户端先收到自己的，再收到别人的）
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDGAMEPLAYERINFO (TK_MSG_GAME + 102)
typedef struct tagTKGAMEPLAYERINFO
{
    int nSeatOrder;  //座位编号
    DWORD dwUserID;  //用户帐号对应的唯一数字编号ID
    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;   //用户头像ID
    int nMatchRank;  //游戏服务器根据比赛积分计算出来的

    int nScore;  //赛中积分

    BOOL bArrived;   //是否客户端已经连接到服务器
    int nNetStatus;  //网络状态：0正常连接，1断线
    DWORD dwFormID;  //用户形象ID
} TKGAMEPLAYERINFO, *PTKGAMEPLAYERINFO;
typedef struct tagTKACKGAMEADDPLAYERENTERINFO
{
    TKHEADER header;
    TKGAMEPLAYERINFO info;
} TKACKGAMEADDPLAYERENTERINFO, *PTKACKGAMEADDPLAYERENTERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发消息，清除本桌指定Player的信息
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_DELGAMEPLAYERINFO (TK_MSG_GAME + 103)
typedef struct tagTKACKGAMEDELPLAYERENTERINFO
{
    TKHEADER header;
    DWORD dwUserID;
} TKACKGAMEDELPLAYERENTERINFO, *PTKACKGAMEDELPLAYERENTERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发消息，修改本桌指定Player的信息
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CHGGAMEPLAYERINFO (TK_MSG_GAME + 104)
typedef struct tagTKACKGAMECHGPLAYERENTERINFO
{
    TKHEADER header;
    DWORD dwUserID;
    BOOL bArrived;   //是否客户端已经连接到服务器
    int nNetStatus;  //网络状态：0正常连接，1断线
} TKACKGAMECHGPLAYERENTERINFO, *PTKACKGAMECHGPLAYERENTERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：Player到达（用户进入游戏）
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYERARRIVED (TK_MSG_GAME + 105)
typedef struct tagTKACKGAMEPLAYERARRIVED
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //座位编号
} TKACKGAMEPLAYERARRIVED, *PTKACKGAMEPLAYERARRIVED;

//-----------------------------------------------------------------------------------------
//   功    能：Player离开（用户离开游戏）
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYERLEAVED (TK_MSG_GAME + 106)
typedef struct tagTKACKGAMEPLAYERLEAVED
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //座位编号
} TKACKGAMEPLAYERLEAVED, *PTKACKGAMEPLAYERLEAVED;

////////////////
//-----------------------------------------------------------------------------------------
//   功    能：一盘游戏开始的消息
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_BEGINHAND (TK_MSG_GAME + 107)
//ACK:GameServer --> GameClient
typedef struct tagTKACKGAMEBEGINHAND
{
    TKHEADER header;
    int iGameHand;  //第几手
    int nRound;     // 第几轮
    int nTable;     // 第几桌
} TKACKGAMEBEGINHAND, *PTKACKGAMEBEGINHAND;

//-----------------------------------------------------------------------------------------
//   功    能：游戏客户端网络断线
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NETBREAK (TK_MSG_GAME + 108)
typedef struct tagTKACKGAMENETBREAK
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //座位编号
} TKACKGAMENETBREAK, *PTKACKGAMENETBREAK;

//-----------------------------------------------------------------------------------------
//   功    能：游戏客户端网络重连
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NETRESUME (TK_MSG_GAME + 109)
typedef struct tagTKACKGAMENETRESUME
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //座位编号
} TKACKGAMENETRESUME, *PTKACKGAMENETRESUME;

//-----------------------------------------------------------------------------------------
//   功    能：断线重回时，历史消息开始
//   说    明：Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HISTORYMSGBEGIN (TK_MSG_GAME + 110)

//-----------------------------------------------------------------------------------------
//   功    能：断线重回时，历史消息结束
//   说    明：Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HISTORYMSGEND (TK_MSG_GAME + 111)
typedef struct tagTKACKGAMEHISTORYMSGEND
{
    TKHEADER header;
    time_t timeCurServer;  //服务器当前时间
} TKACKGAMEHISTORYMSGEND, *PTKACKGAMEHISTORYMSGEND;

//-----------------------------------------------------------------------------------------
//   功    能：局制结束(本局所有游戏结束)
//   说    明：Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OVERGAME (TK_MSG_GAME + 112)
#define TK_MSG_GAME_ROUNDOVER (TK_MSG_GAME_OVERGAME)

////////////////
//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发新的ScoreBase
//   说    明：Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SCOREBASERAISE (TK_MSG_GAME + 113)
typedef struct tagTKACKGAMESCOREBASERAISE
{
    TKHEADER header;
    int nNewScoreBase;  //新的游戏基数
} TKACKGAMESCOREBASERAISE, *PTKACKGAMESCOREBASERAISE;

//-----------------------------------------------------------------------------------------
//   功    能：用户强行退出比赛，比赛将被系统托管
//   说    明：GC -> GS, REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_EXITGAME (TK_MSG_GAME + 114)

//-----------------------------------------------------------------------------------------
//   功    能：修改游戏局制规则说明
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CHANGEGAMERULERNOTE (TK_MSG_GAME + 115)
typedef struct tagTKACKGAMECHANGERULERNOTE
{
    TKHEADER header;
    char szRulerNote[TK_BUFLEN_ROUNDRULERNOTE];  //游戏局制规则说明：N局积分最高者获胜 / 低于****分将被淘汰
} TKACKGAMECHANGERULERNOTE, *PTKACKGAMECHANGERULERNOTE;

//-----------------------------------------------------------------------------------------
//   功    能：通知GC可以直接退出比赛了
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NOQUERYEXITGAME (TK_MSG_GAME + 116)
typedef struct tagTKACKNOQUERYEXITGAME
{
    TKHEADER header;
    BOOL bNoQueryExitGame;
} TKACKNOQUERYEXITGAME, *PTKACKNOQUERYEXITGAME;

//-----------------------------------------------------------------------------------------
//   功    能：客户端请求比赛检测
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CHECKENTERROUND (TK_MSG_GAME + 117)
typedef struct tagTKREQGAMECHECKENTERROUND : public tagTKREQGAMEENTERROUND
{
} TKREQGAMECHECKENTERROUND, *PTKREQGAMECHECKENTERROUND;

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发游戏扩展规则
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RULEREXINFO (TK_MSG_GAME + 118)
typedef struct tagTKACKGAMERULEREXINFO
{
    TKHEADER header;
    char szPropertyEx[TK_MAX_GAMEPROPEXLEN];  //游戏相关附加属性，描述游戏规则，游戏服务器自行解释，对复杂游戏可能会有
} TKACKGAMERULEREXINFO, *PTKACKGAMERULEREXINFO;

//-----------------------------------------------------------------------------------------
//   功    能：用户设置开牌属性
//   说    明：GC -> GS, REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SHOWCARD (TK_MSG_GAME + 119)
typedef struct tagTkReqGamePlayerShowCard
{
    TKHEADER header;
    BOOL bShowCard;  //true为开牌给旁观者,反之为不开牌
} TKREQGAMEPLAYERSHOWCARD, *PTKREQGAMEPLAYERSHOWCARD;

//-----------------------------------------------------------------------------------------
//   功    能：一盘游戏结束的消息
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HANDOVER (TK_MSG_GAME + 120)

//-----------------------------------------------------------------------------------------
//   功    能：游戏内简单互动消息消息
//   说    明：GC <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SIMPLEACTION (TK_MSG_GAME + 121)
typedef struct tagTkReqGameSimpleAction
{
    TKHEADER header;

    DWORD dwUserIDFrom;  // 发送者
    DWORD dwUserIDTo;    // 接收者，0表示桌上所有人

    DWORD dwEmotionID;  // 表情ID
    DWORD dwTextID;     // 文字ID

    DWORD dwUserIDTarget;
    DWORD dwDonate;  // 赠送
} TKREQGAMESIMPLEACTION, *PTKREQGAMESIMPLEACTION;

//-----------------------------------------------------------------------------------------
//   功    能：客户端处理消息延迟
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ONMSGDELAY (TK_MSG_GAME + 122)
typedef struct tagTkReqOnMsgDelay
{
    TKHEADER header;

    DWORD dwMsgType;
    DWORD dwLastMsgType;
    DWORD dwTimeRecv;
    DWORD dwTimeInvoke;
    DWORD dwTimeOnMsg;

} TKREQGAMEONMSGDELAY, *PTKREQGAMEONMSGDELAY;

//-----------------------------------------------------------------------------------------
//
// MS给GS推送比赛奖励提示
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PUSHUSERAWARDMESSAGE (TK_MSG_GAME + 123)
typedef struct tagTKACKGSPUSHPUSHUSERAWARDMESSAGE
{
    TKHEADER header;

    DWORD dwAwardType;  //幸运点奖励 \天降喜金\天降物品\连胜奖励\累胜奖励\满盘奖励\PK奖励\彩金奖励\结束提示等，参见“TKMatchUniversalDicSrv.h” eMatchAwardMessageType定义

    DWORD dwXmlLength;  // 后缀XML的长度，包含字符串结束符'\0'
    DWORD dwXmlOffset;  // 后缀XML地址偏移量，单位字节，参照点为消息包起始位置
} TKACKGSPUSHPUSHUSERAWARDMESSAGE, *PTKACKGSPUSHPUSHUSERAWARDMESSAGE;

//-----------------------------------------------------------------------------------------
//   功    能：游戏客户端网络相互测速的消息
//   说    明：GC <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NETTEST (TK_MSG_GAME + 124)
typedef struct tagTkReqGameNetTest
{
    TKHEADER header;

    DWORD dwTestID;  // 测速序列号

    DWORD dwReqUserID;  // 测速发起者
    DWORD dwAckUserID;  // 测速响应者

    DWORD dwReqTick;  // 测速发起时Tick
    DWORD dwAckTick;  // 收到响应时Tick

} TKREQGAMENETTEST, *PTKREQGAMENETTEST;
typedef struct tagTkAckGameNetTest
{
    TKHEADER header;

    DWORD dwTestID;  // 测速序列号

    DWORD dwReqUserID;  // 测速发起者
    DWORD dwAckUserID;  // 测速响应者

    DWORD dwReqTick;  // 测速发起时Tick
    DWORD dwAckTick;  // 收到响应时Tick

} TKACKGAMENETTEST, *PTKACKGAMENETTEST;

//-----------------------------------------------------------------------------------------
//
// 推送提示文字
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_TIPMESSAGE (TK_MSG_GAME + 125)
//ACK:GameServer --> Client
typedef struct tagTKACKPUSHGAMETIPMESSAGE
{
    TKHEADER header;
    int nClass;  // 类别，参见 TKGameTipMessageDefine.h 的 ePushGameTipClass 定义
    int nTipID;  // 编号，参见 TKGameTipMessageDefine.h 的相关定义

    WORD wXmlOffset;  // 后缀XML地址偏移量，单位字节，参照点为消息包起始位置
    WORD wXmlLength;  // 后缀XML的长度，包含字符串结束符'\0'

    BYTE byShowType;   // 建议显示类型，参见 TKGameTipMessageDefine.h 的 ePushGameTipShowType 定义
    BYTE byOperation;  // 建议后续操作，参见 TKGameTipMessageDefine.h 的 ePushGameTipOperation 定义
    WORD wShowSecond;  // 建议显示时间(秒)

} TKACKPUSHGAMETIPMESSAGE, *PTKACKPUSHGAMETIPMESSAGE;

//-----------------------------------------------------------------------------------------
//   功    能：游戏服务通用消息
//   说    明：GC <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GAMECOMMONPROTOCOL (TK_MSG_GAME + 126)
typedef struct tagTKREQGAMECOMMONPROTOCOL
{
    TKHEADER header;
    WORD wMsgID;                 // 协议ID
    WORD wSubMsgID;              // 子协议ID
    TKSUFFIXIDX suffixJsonData;  // 后缀业务数据(Json字符串，UTF-8格式)
} TKREQGAMECOMMONPROTOCOL, *PTKREQGAMECOMMONPROTOCOL;
typedef struct tagTKACKGAMECOMMONPROTOCOL
{
    TKHEADER header;
    WORD wMsgID;                 // 协议ID
    WORD wSubMsgID;              // 子协议ID
    TKSUFFIXIDX suffixJsonData;  // 后缀业务数据(Json字符串，UTF-8格式)
} TKACKGAMECOMMONPROTOCOL, *PTKACKGAMECOMMONPROTOCOL;

// TK_MSG_GAME_GAMECOMMONPROTOCOL中wMsgID在比赛端的定义
enum CommonGameMsgIDDef : unsigned short
{
    GameBase_Begin = 0,
    EHeartBestTest = 1,
    AddGamePlayerInfo = 2,  // 添加游戏用户
    PushGameDataInfo = 3,   // 发送游戏数据
    GameBase_End = 1000,    // =========
};

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发本桌Player信息（客户端先收到自己的，再收到别人的）
//   说    明：64位版
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDGAMEPLAYERINFO64 (TK_MSG_GAME + 127)
typedef struct tagTKGAMEPLAYERINFO64
{
    int nSeatOrder;   //座位编号
    UINT64 dwUserID;  //用户帐号对应的唯一数字编号ID
    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;   //用户头像ID
    int nMatchRank;  //游戏服务器根据比赛积分计算出来的

    int64_t nScore;  //赛中积分

    BOOL bArrived;   //是否客户端已经连接到服务器
    int nNetStatus;  //网络状态：0正常连接，1断线
    DWORD dwFormID;  //用户形象ID
} TKGAMEPLAYERINFO64, *PTKGAMEPLAYERINFO64;
typedef struct tagTKACKGAMEADDPLAYERENTERINFO64
{
    TKHEADER header;
    TKGAMEPLAYERINFO64 info;
} TKACKGAMEADDPLAYERENTERINFO64, *PTKACKGAMEADDPLAYERENTERINFO64;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送游戏数据信息
//   说    明：GS --> GC 64位版
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHPLAYERGAMEDATA64 (TK_MSG_GAME + 128)
typedef struct tagTKACKGSPUSHPLAYERGAMEDATA64
{
    TKHEADER header;

    UINT64 dwUserID;

    int nExchangeRate;  // 兑换率
    int64_t nLeftChip;  // 剩余筹码或者当前生命值

    int iCurGameHand;             // 用户当前进行的局制编号：第%d局（开局时累计。为0是表示没有打过，不显示）
    int nTotalFinishedGameCount;  // 已经完成的总有效局数（局制结束时累加。用于局数奖励显示）
    int nCurFinishedGameCount;    // 当前阶段完成的有效局数（局制结束时累加。用于积分加倍显示）
} TKACKGSPUSHPLAYERGAMEDATA64, *PTKACKGSPUSHPLAYERGAMEDATA64;

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发比赛选手名次信息（客户端收到此信息后，清除原先的Order信息）
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_STAGEPLAYERORDER (TK_MSG_GAME + 200)
typedef struct tagTKACKGAMESTAGEPLAYERORDER
{
    TKHEADER header;
    DWORD dwSerial;
    WORD wFlags;
    WORD wScoreType;
    int nMinWinnerOrder;  //出线最低名次
    int cnOrder;          //后面跟cnOrder个TKGAMEPLAYERORDERINFO
} TKACKGAMESTAGEPLAYERORDER, *PTKACKGAMESTAGEPLAYERORDER;
#define TK_GAME_SCORETYPE_SCORE 0  //显示为积分
#define TK_GAME_SCORETYPE_CHIP 1   //显示为筹码
#define TK_GAME_SCORETYPE_LIFE 2   //显示为生命值
typedef struct tagTKGAMEPLAYERORDERINFO
{
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];
    int nScore;  //赛中积分，出局时显示“出局”，否则显示nScore数值
    WORD wPlaceOrder;
    WORD wOutRoundState;  //是否出局,0为出局，1出局
} TKGAMEPLAYERORDERINFO, *PTKGAMEPLAYERORDERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：给客户端下发指定Round的PlaceOrder信息
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROUNDPLAYERORDER (TK_MSG_GAME + 201)
typedef struct tagTKACKGAMEROUNDPLAYERORDER
{
    TKHEADER header;
    DWORD dwSerial;
    WORD wFlags;
    int cnOrder;  //后面跟cnOrder个TKGAMEPLAYERORDERINFO
} TKACKGAMEROUNDPLAYERORDER, *PTKACKGAMEROUNDPLAYERORDER;

//-----------------------------------------------------------------------------------------
//   功    能：给通知客户端Stage排名列表有更新变化
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_STAGEPLAYERORDERCHANGED (TK_MSG_GAME + 202)
typedef struct tagTKACKGAMESTAGEPLAYERORDERCHANGED
{
    TKHEADER header;
    DWORD dwSerial;
    WORD wFlags;  //1表示请立即更新
    WORD wUpdateSecond;
} TKACKGAMESTAGEPLAYERORDERCHANGED, *PTKACKGAMESTAGEPLAYERORDERCHANGED;

//-----------------------------------------------------------------------------------------
//   功    能：客户端获取新的Stage排名列表
//   说    明：GC -> GS, REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GETSTAGEPLAYERORDER (TK_MSG_GAME + 203)

//-----------------------------------------------------------------------------------------
//   功    能：玩家积分变化
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_UPDATE_ROUND_PLAYER_SCORE (TK_MSG_GAME + 204)
typedef struct tagTKUPDATEROUNDPLAYERSCORE
{
    TKHEADER header;
    int cnPlayer;
    // 后跟cnPlayer个( userid + score )
    // DWORD dwUserID;
    // int nScore;
    // ...
} TKUPDATEROUNDPLAYERSCORE, *PTKUPDATEROUNDPLAYERSCORE;

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端，让玩家休息一会
//   说    明：GS -> GC, Push REQ
//   同功能的32位协议TK_MSG_GAME_REST
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_REST_64 (TK_MSG_GAME + 205)
typedef struct tagTKREQPLAYERREST_64
{
    TKHEADER header;
    int32_t nRestTime;          // 可休息时间,到点后如果玩家不点继续按钮游戏将自动开始
    int64_t i64Life;            // 当前生命值
    int64_t i64Coin;            // 可回兑金币数
    int32_t cnGames;            // 已完成牌副数
    int64_t i64Multi;           // 下一局的积分倍数
    int32_t nNextLevelGames;    // 下一级所需牌副数
    int64_t i64NextLevelMulti;  // 下一级积分倍数
    uint64_t u64ExChangeRate;   // 回兑率分子（回换率10000表示1，放大10000倍）
    uint64_t u64ExChangeRateDenominator;  // 回兑率分母（回换率10000表示1，放大10000倍），注：比赛结束回兑的金币计算公式是：回兑的金币=筹码*u64ExChangeRate/(10000*u64ExChangeRateDenominator)
    int32_t nAwardTimeSpan;        //颁奖时间间隔(秒),<=0表示不颁奖
    int32_t nNextAwardLeftSecond;  //距离下一次颁奖的剩余时间(秒)
    int64_t i64Score;              // 比赛积分
    int64_t i64LastHandScore;      // 最后一局得分
    uint16_t wGameResult;          // 游戏结果，0 == 未知， 1 == 胜利， 2 == 和， 3 == 负

    //
    uint32_t dwSufOffset;  //后缀数据偏移
    uint32_t dwSufSize;    //后缀数据大小
    int64_t i64Reserve1;   //保留1
    uint32_t dwReserve2;   //保留2
    uint32_t dwReserve3;   //保留3
} TKREQPLAYERREST_64, *PTKREQPLAYERREST_64;
//后缀血瓶json{"Plan":[{"Blod":4444,"Cost":4444,"Discnt":100},{"Blod":333,"Cost":333,"Discnt":100}]}

//   功    能：加血方案（64位）
//   说    明：GS-> GC
//   同功能的32位协议TKID_MSG_GAME_EXCHANGE
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_EXCHANGE_64 (TK_MSG_GAME + 206)
typedef struct tagTKREQGAMEEXCHANGE_64
{
    TKHEADER header;
    uint64_t u64ExChangeRate;  // 回兑率分子（回换率10000表示1，放大10000倍）
    uint64_t u64ExChangeRateDenominator;  // 回兑率分母（回换率10000表示1，放大10000倍），注：比赛结束回兑的金币计算公式是：回兑的金币=筹码*u64ExChangeRate/(10000*u64ExChangeRateDenominator)
    uint32_t dwMPID;       //产品ID
    uint32_t dwTourneyID;  //TourneyID
    uint32_t dwStageID;    //阶段ID
    uint32_t dwSufOffSet;  //后缀数据偏移
    uint32_t dwSufSize;    //后缀数据大小
    int64_t i64Reserve1;   //保留字段1
    uint32_t dwReserve2;   // 保留字段2
    uint32_t dwReserve3;   // 保留字段3
} TKREQGAMEEXCHANGE_64, *PTKREQGAMEEXCHANGE_64;
//后缀血瓶json，{"Plan":[{"Blod":4444,"Cost":4444,"Discnt":100},{"Blod":333,"Cost":333,"Discnt":100}]}

//-----------------------------------------------------------------------------------------
//   功    能：玩家请求补血（64位）
//   说    明：GC -> GS -> GC
//   同功能的32位协议TK_MSG_GAME_HEMATINIC
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HEMATINIC_64 (TK_MSG_GAME + 207)
typedef struct tagTKPLAYERREQHEMATINIC_64
{
    TKHEADER header;
    uint32_t dwUserID;     // 用户ID
    int64_t i64Hematinic;  // 补血量
    int64_t i64Reserve1;   //保留字段1
    uint32_t dwDisCount;   //折扣
    uint32_t dwReserve2;   //保留字段2
    uint32_t dwReserve3;   //保留字段3
} TKPLAYERREQHEMATINIC_64, *PTKPLAYERREQHEMATINIC_64;

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送游戏数据信息
//   说    明：GS --> GC( 64位扩展版）
//   同功能的64位和32位协议TKID_GS2MC_PUSHPLAYERGAMEDATA64和TKID_GS2MC_PUSHPLAYERGAMEDATA
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHPLAYERGAMEDATA64EX (TK_MSG_GAME + 208)
typedef struct tagTKACKGSPUSHPLAYERGAMEDATA64EX
{
    TKHEADER header;
    uint32_t dwUserID;
    int64_t i64LeftChip;              // 剩余筹码或者当前生命值
    int32_t iCurGameHand;             // 用户当前进行的局制编号：第%d局（开局时累计。为0是表示没有打过，不显示）
    int32_t nTotalFinishedGameCount;  // 已经完成的总有效局数（局制结束时累加。用于局数奖励显示）
    int32_t nCurFinishedGameCount;    // 当前阶段完成的有效局数（局制结束时累加。用于积分加倍显示）
    uint64_t u64ExChangeRate;         // 回兑率分子（回换率10000表示1，放大10000倍）
    uint64_t u64ExChangeRateDenominator;  // 回兑率分母（回换率10000表示1，放大10000倍），注：比赛结束回兑的金币计算公式是：回兑的金币=筹码*u64ExChangeRate/(10000*u64ExChangeRateDenominator)
    uint64_t u64Reserve1;  // 保留字段1
    uint32_t dwReserve2;   // 保留字段2
    uint32_t dwReserve3;   // 保留字段3
} TKACKGSPUSHPLAYERGAMEDATA64EX, *PTKACKGSPUSHPLAYERGAMEDATA64EX;

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端，让玩家休息一会
//   说    明：GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_REST (TK_MSG_GAME + 305)
typedef struct tagTKREQPLAYERREST
{
    TKHEADER header;
    int nRestTime;                               // 可休息时间,到点后如果玩家不点继续按钮游戏将自动开始
    int cnHematinic;                             // 可选的血瓶类型
    HEMATINIC asHematinic[MAX_HEMATINIC_COUNT];  // 每种血瓶的详细信息
    int nLife;                                   // 当前生命值
    int nCoin;                                   // 可回兑金币数
    int cnGames;                                 // 已完成牌副数
    int nMulti;                                  // 下一局的积分倍数
    int nNextLevelGames;                         // 下一级所需牌副数
    int nNextLevelMulti;                         // 下一级积分倍数
    int nExchangeRate;                           // 回兑比例

    int nAwardTimeSpan;        //颁奖时间间隔(秒),<=0表示不颁奖
    int nNextAwardLeftSecond;  //距离下一次颁奖的剩余时间(秒)

    //2011.11.16 新增
    int nScore;  // 比赛积分

    int nLastHandScore;  // 最后一局得分
    WORD wGameResult;    // 游戏结果，0 == 未知， 1 == 胜利， 2 == 和， 3 == 负
} TKREQPLAYERREST, *PTKREQPLAYERREST;

//-----------------------------------------------------------------------------------------
//   功    能：玩家请求继续游戏
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CONTINUE (TK_MSG_GAME + 306)
typedef struct tagTKPLAYERREQCONTINUE
{
    TKHEADER header;
    DWORD dwUserID;
    int nExtraTask;  // 附加任务, 0=无, 1=碎心挑战任务, 其他未定义        wuqy add 2016.5.30
} TKPLAYERREQCONTINUE, *PTKPLAYERREQCONTINUE;

//-----------------------------------------------------------------------------------------
//   功    能：玩家请求退出游戏
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LEAVE (TK_MSG_GAME + 307)
typedef struct tagTKPLAYERREQLEAVE
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwNoExchange;   // 不兑换金币，0-表示兑换金币，1表示不兑换。默认值为0，兼容线上版本。
    DWORD dwSaveArchive;  // 数据存档，0-表示不存档，1表示存档。默认值为1
} TKPLAYERREQLEAVE, *PTKPLAYERREQLEAVE;

//-----------------------------------------------------------------------------------------
//   功    能：玩家请求补血
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HEMATINIC (TK_MSG_GAME + 308)
typedef struct tagTKPLAYERREQHEMATINIC
{
    TKHEADER header;
    DWORD dwUserID;
    int nHematinic;  // 补血量
} TKPLAYERREQHEMATINIC, *PTKPLAYERREQHEMATINIC;

//-----------------------------------------------------------------------------------------
//   功    能：玩家请求复活
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_PLAYER_RELIVE (TK_MSG_GAME + 310)
typedef struct tagTKPLAYERRELIVE
{
    TKHEADER header;
    DWORD dwUserID;
} TKPLAYERRELIVE, *PTKPLAYERRELIVE;

//-----------------------------------------------------------------------------------------
//   功    能：玩家确认收到奖状消息
//   说    明：GC -> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_PLAYER_RECEIVEDMATCHAWARD (TK_MSG_GAME + 311)
// ACK:TKHEADER header

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端当前奖池大小
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_JACKPOT_COUNT (TK_MSG_GAME + 312)
typedef struct tagJACKPOTCOUNT
{
    TKHEADER header;
    int nJackpotCount;
} TKJACKPOTCOUNT, *PTKJACKPOTCOUNT;

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端当前奖池描述
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_JACKPOT (TK_MSG_GAME + 313)
typedef struct tagJACKPOT
{
    TKHEADER header;
    int nJackpotCount;
    // 后跟描述字符串
} TKJACKPOT, *PTKJACKPOT;

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端当前中奖玩家列表
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_JACKPOT_WINNER (TK_MSG_GAME + 314)
typedef struct tagJACKPOTWINNER
{
    TKHEADER header;
    int cnWinner;
    // 后跟描述字符串
} TKJACKPOTWINNER, *PTKJACKPOTWINNER;

//-----------------------------------------------------------------------------------------
//   功    能：通知比赛客户端开放聊天模式
//   说    明：GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ENABLECHAT (TK_MSG_GAME + 315)
// ACK:
// BOOL bEnableGCChat
// DWORD dwChatSvrIP			// 聊天服务器IP，注意是机器字节序
// DWORD dwChatSvrPort
// DWORD dwChatChannelID
// DWORD dwChatChannelSubID		wuqy add 2010.05.12
// DWORD dwChatChannelType
// DWORD dwChatUserType

//-----------------------------------------------------------------------------------------
//   功    能：通知网页客户端注册
//   说    明：GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NOTIFY_GUESTREGISTER (TK_MSG_GAME + 316)
typedef struct tagTKGAMENOTIFYGUESTREGISTER
{
    TKHEADER header;
    DWORD dwSuffixOffSet;  // 后缀提示文字字符串偏移量，相对于消息缓冲区顶端
    DWORD dwSuffixSize;    // 后缀提示文字字符串大小，包含结束符\0
} TKGAMENOTIFYGUESTREGISTER, *PTKGAMENOTIFYGUESTREGISTER;
//后跟提示文字

//-----------------------------------------------------------------------------------------
//   功    能：网页客户端请求注册
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_WEBREGISTER_MANUAL (TK_MSG_GAME + 317)
#define TK_MSG_GAME_WEBREGISTER (TK_MSG_GAME_WEBREGISTER_MANUAL)
typedef struct tagTKREQWEBGUESTGAMEREGISTERBASE
{
    TKHEADER header;
    char szLoginName[TK_BUFLEN_USERNAME];
    char szNickName[TK_BUFLEN_NICKNAME];
    char szPassWord[TK_BUFLEN_PASSWORD];
    char szIP[TK_BUFLEN_IP];
    WORD wFigureID;  // 头像ID
    WORD wReserve1;

    DWORD dwLcVer;  // 客户端版本号

    // 增加注册代码
    WORD wAgent;  // 注册来源
    WORD wReserve2;

    // 推广员
    DWORD dwPromoterID;                           // 推广员ID
    char szPromoterNickName[TK_BUFLEN_NICKNAME];  // 推广员昵称

    // 新手卡
    char szNewPlayerCard[TK_BUFLEN_NEWPLAYERCARD];  // 新手卡号

    DWORD dwSuffixOffSet;  // 后缀XML字符串数据偏移量，相对于消息缓冲区顶端，用于协议兼容
    DWORD dwSuffixSize;    // 后缀XML字符串数据大小，，包含结束符\0
    // 保留
    DWORD dwParam1;  // 保留参数1
    DWORD dwParam2;  // 保留参数2
    DWORD dwParam3;  // 保留参数3

    //！！！新增协议在tagTKREQWEBGUESTGAMEREGISTER里添加 (wuqy 2013.9.6)
} TKREQWEBGUESTGAMEREGISTERBASE;
typedef struct tagTKREQWEBGUESTGAMEREGISTER : public tagTKREQWEBGUESTGAMEREGISTERBASE
{
    char szCltIdent[TK_BUFLEN_NOTE];  // 客户端唯一标识(wuqy add 2011.8.1)
    DWORD dwCustomID;                 // 安装员 推广ID(wuqy add 2013.9.4)
} TKREQWEBGUESTGAMEREGISTER, *PTKREQWEBGUESTGAMEREGISTER;
// 请求后缀数据：
//     Web传过来的XML字符串数据
typedef struct tagTKACKWEBGUESTGAMEREGISTER
{
    TKHEADER header;
    DWORD dwUserID;  //注册成功后的UserID
    char szNickName[TK_BUFLEN_NICKNAME];
} TKACKWEBGUESTGAMEREGISTER, *PTKACKWEBGUESTGAMEREGISTER;
//注册消息返回值：
#define TK_WEBGAMEREGISTER_LOGINNAMEEXIST (10)    // 用户名已经被使用
#define TK_WEBGAMEREGISTER_NICKNAMEEXIST (11)     // 用户昵称已经被使用
#define TK_WEBGAMEREGISTER_NICKNAMELAWLESS (12)   // 登录昵称非法
#define TK_WEBGAMEREGISTER_LOGINNAMELAWLESS (13)  // 登录名非法
#define TK_WEBGAMEREGISTER_PWDLAWLESS (14)        // 密码格式非法
#define TK_WEBGAMEREGISTER_IDENTLIMIT (15)        // 设备注册侧次数限制
#define TK_WEBGAMEREGISTER_LOGINNAMEERR (100)     // 登陆的用户名不符合有效规则
#define TK_WEBGAMEREGISTER_NICKNAMEERR (101)      // 昵称不符合有效规则
#define TK_WEBGAMEREGISTER_PSWERR (102)           // 密码不符合有效规则 或者密码不能包括或等于登陆名

//-----------------------------------------------------------------------------------------
//   功    能：旁观客户端聊天配置信息
//   说    明：BS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELAYSRV_CHAT_CFG (TK_MSG_GAME + 318)
typedef struct tagRelaySvrChatCfgAck
{
    TKHEADER hdr;
    DWORD dwChatSvrIP;  // 聊天服务器IP，注意是机器字节序
    DWORD dwChatSvrPort;
    DWORD dwChatChannelID;
    BOOL bListTablePlayerOnly;

    DWORD dwChatChannelSubID;  //wuqy add 2010.05.12
    DWORD dwChatChannelType;
    DWORD dwChatUserType;

} TKACK_RELAYSVR_CHAT_CFG, *LPTKACK_RELAYSVR_CHAT_CFG;
// ACK:
// TKACK_RELAYSVR_CHAT_CFG

//-----------------------------------------------------------------------------------------
//   功    能：旁观客户端请求旁观人数
//   说    明：GC <-> BS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELAYSRV_GET_ROUND_WATCHER_COUNT (TK_MSG_GAME + 319)
typedef struct tagGetRoundWatcherCntReq
{
    TKHEADER hdr;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwRoundID;
} GET_ROUND_WATCHER_CNT_REQ, *LPGET_ROUND_WATCHER_CNT_REQ;
// ACK:
// TKHEADER header
// header.dwParam == 人数

//-----------------------------------------------------------------------------------------
//   功    能：岛屿锁定对手
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LOCKDOWN (TK_MSG_GAME + 320)
typedef struct tagTKReqLockdown
{
    TKHEADER header;
    int nSeat;
    BOOL bUnlock;  // 是否取消锁定
} TKREQLOCKDOWNGAME, *PTKREQLOCKDOWNGAME;
typedef struct tagTKAckLockdown
{
    TKHEADER header;
    BOOL bAllow;  // 是否允许
} TKACKLOCKDOWNGAME, *PTKACKLOCKDOWNGAME;

//-----------------------------------------------------------------------------------------
//
// WebGuest取得成绩，通知注册
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NOTIFY_GUESTREGISTEREX (TK_MSG_GAME + 321)
typedef struct tagTKGAMENOTIFYGUESTREGISTEREX
{
    TKHEADER header;
    DWORD dwOrder;       // 获得名次
    BOOL bAutoRegister;  // 是否由服务器自动注册
} TKGAMENOTIFYGUESTREGISTEREX, *PTKGAMENOTIFYGUESTREGISTEREX;

//-----------------------------------------------------------------------------------------
//
// WebGuest隐式注册请求
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_WEBREGISTER_AUTO (TK_MSG_GAME + 322)
typedef struct tagTKREQWEBGUESTGAMEREGISTERAUTO
{
    TKHEADER header;

    DWORD dwPartnerID;  // 合作厂商ID

    // 推广站点市场编号
    DWORD dwAgent;                     // 注册来源
    char szAgentMark[TK_BUFLEN_NOTE];  // 代理商注释
    BOOL bRegister;                    // 用户是否选择注册

    //如果填写了以下信息，就用以下信息注册
    char szNickName[TK_BUFLEN_NICKNAME];
    char szUserName[TK_BUFLEN_USERNAME];
    char szPassword[TK_BUFLEN_PASSWORD];
    DWORD dwFigureID;  // 头像ID

    char szCltIdent[TK_BUFLEN_NOTE];  // 客户端唯一标识
    char szIP[TK_BUFLEN_IP];          // 由Agent服务器传过来的用户真实IP
    DWORD dwCustomID;                 // 推广客户ID

} TKREQWEBGUESTGAMEREGISTERAUTO, *PTKREQWEBGUESTGAMEREGISTERAUTO;
typedef struct tagTKACKWEBGUESTGAMEREGISTERAUTO
{
    TKHEADER header;
    DWORD dwUserID;  //注册成功后的UserID
    char szNickName[TK_BUFLEN_NICKNAME];
    char szUserName[TK_BUFLEN_USERNAME];
    char szPassword[TK_BUFLEN_PASSWORD];
    DWORD dwFigureID;
    DWORD dwBankAccID;  // 系统账单
} TKACKWEBGUESTGAMEREGISTERAUTO, *PTKACKWEBGUESTGAMEREGISTERAUTO;

//-----------------------------------------------------------------------------------------
//
//   功    能：打开或关闭阶段排名结果信息实时更新
//   说    明：GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OPENSTAGEBOUTRESULT (TK_MSG_GAME + 323)
typedef struct tagTKREQGAMEOPENSTAGEBOUTRESULT
{
    TKHEADER header;
    BOOL bOpen;
} TKREQGAMEOPENSTAGEBOUTRESULT, *PTKREQGAMEOPENSTAGEBOUTRESULT;
typedef struct tagTKACKGAMEOPENSTAGEBOUTRESULT
{
    TKHEADER header;
    int nOpenStyle;       //0关闭，1列表框方式打开，2面板框方式打开
    BOOL bInWaitPlayoff;  //是否处于加赛等待中 wuqy add 2015.4.28(没有加赛配置或已经完成加赛的，bInWaitPlayoff为FALSE)
} TKACKGAMEOPENSTAGEBOUTRESULT, *PTKACKGAMEOPENSTAGEBOUTRESULT;

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端，让玩家复活
//   说    明：GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELIVECOST (TK_MSG_GAME + 324)
typedef struct tagTKGAMERELIVECOST
{
    TKHEADER header;

    int nCostType;   // 复活费用类型
    DWORD dwCostID;  // 复活费用ID
    int nAmount;     // 复活费用的数量

    int nScore;        // 可获得积分
    int cnLivePlayer;  // 当前存活人数
    int nTimeOut;      // 倒计时
} TKGAMERELIVECOST, *PTKGAMERELIVECOST;

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端，让玩家选择支付业务扣费
//   说    明：GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PAYMENTCOST (TK_MSG_GAME + 325)

// 请求：
typedef struct tagTKREQGAMEPAYMENTCOST
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    int nTimeOut;      // 购买倒计时
    int cnLivePlayer;  // 当前存活人数

    DWORD dwPayTypeID;        // 扣费业务类型 参见: eDicPaymentBusinessTypeDef, 参见 TKComitUniversalDicSrv.h
    DWORD dwCostPlanCount;    // 扣费方案个数
    TKSUFFIXIDX sufData;      // 后缀数据，TK_COSTPLAN，参见 TKMatchUniversalDicSrv.h
    TKSUFFIXIDX sufTaskData;  // 后缀任务数据
} TKREQGAMEPAYMENTCOST, *PTKREQGAMEPAYMENTCOST;
// 后缀数据 dwCastPlanCount个TK_COSTPLAN

// 回馈：
typedef struct tagTKACKGAMEPAYMENTCOST
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwResult;     // 业务执行结果，0-玩家不购买，1-玩家购买
    DWORD dwPayTypeID;  // 扣费业务类型 参见: eDicPaymentBusinessTypeDef, 参见 TKComitUniversalDicSrv.h
    DWORD dwPlanID;     // 复活的方案ID
} TKACKGAMEPAYMENTCOST, *PTKACKGAMEPAYMENTCOST;

//-----------------------------------------------------------------------------------------
//   功    能：通知客户端，玩家选择支付业务扣费 的结果
//   说    明：GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PAYMENTRESULT (TK_MSG_GAME + 326)

// 请求：
typedef struct tagTKREQGAMEPAYMENTRESULT
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    DWORD dwPayTypeID;  // 扣费业务类型 参见: eDicPaymentBusinessTypeDef, 参见 TKComitUniversalDicSrv.h
    DWORD dwResult;     // 支付业务扣费的结果，eDicTypeTKPaymentResult，参见 TKMatchUniversalDicSrv.h
} TKREQGAMEPAYMENTRESULT, *PTKREQGAMEPAYMENTRESULT;

//-----------------------------------------------------------------------------------------
//   功    能：MS <-> GS <-> GC 消息透传
//   说    明：
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELAY_BEGIN (TK_MSG_GAME + 330)
#define TK_MSG_GAME_RELAY_END (TK_MSG_GAME_RELAY_BEGIN + TK_SVR_MAX_RELAY_MSG)

//-----------------------------------------------------------------------------------------
//   功    能：约战桌初始化消息
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_INITTABLE (TK_MSG_GAME_RELAY_BEGIN + 0)
// ACK:
//  TKHEADER header;
//	BOOL bGaming;
//  int nSeatCount;                      //
//  nSeatCount个TK_SEAT_PLAYER_INFO
//  DWORD dwChatSvrIP					// 聊天服务器IP，注意是机器字节序
//  int nChatSvrPort
//  DWORD dwChatChannelID
//  BOOL bShowTable
//  DWORD dwChatChannelSubID		wuqy add 2010.05.12
//  DWORD dwChatChannelType
//  DWORD dwChatUserType

//-----------------------------------------------------------------------------------------
//   功    能：游戏桌坐下和站起
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_SITDOWN (TK_MSG_GAME_RELAY_BEGIN + 1)
// REQ
//  TKHEADER header;
//  int nSeat;                          // 座位号, base 0. -1表示站起
// ACK
//  TKHEADER header;
//  TK_SEAT_PLAYER_INFO stPlayer;       //
//  int nSeat;                          // 座位号, base 0. -1表示站起
//  nPlayerInfoExItemSize               // =sizeof(TK_SEAT_PLAYER_INFOEX) wuqy add 2017.1.18
//  TK_SEAT_PLAYER_INFOEX stPlayerEx    // wuqy add 2017.1.18

//-----------------------------------------------------------------------------------------
//   功    能：约战桌用户准备开始游戏
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_READY (TK_MSG_GAME_RELAY_BEGIN + 2)
// REQ
//  TKHEADER header;
//  int nReady;                        // 1准备 0取消
// ACK
//  TKHEADER header;
//  DWORD dwUserID;
//  int nReady;                        // 1准备 0取消

//-----------------------------------------------------------------------------------------
//   功    能：游戏桌局制开始
//   说    明：GS-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROUNDBEGIN (TK_MSG_GAME_RELAY_BEGIN + 3)
// ACK:
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   功    能：游戏桌允许旁观自己的牌
//   说    明：GC<-->GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_ENABLE_WATCH (TK_MSG_GAME_RELAY_BEGIN + 4)
// REQ:
//  TKHEADER header;
// ACK:
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   功    能：自由桌初始化消息
//   说    明：GS-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_INITGAMETABLE (TK_MSG_GAME_RELAY_BEGIN + 5)
// ACK:
//  TKHEADER header;
//  int nSeatCount;                      //
//  nSeatCount个TK_SEAT_PLAYER_INFO
//  DWORD dwChatSvrIP					// 聊天服务器IP，注意是机器字节序
//  int nChatSvrPort
//  DWORD dwChatChannelID
//  int nMaxAddToHP;                     // HP上限(最多加到/最大),视nHPMode的值而定
//  int nMinAddToHP;                     // HP下限(最小加到/最小),视nHPMode的值而定
//  int nExchangeRate;                   // 兑换率
//  int nHPMode;                         // 加血模式 1: 相对模式 2: 绝对模式
//  DWORD dwChatChannelSubID				wuqy add 2010.05.12
//  DWORD dwChatChannelType
//  DWORD dwChatUserType
//	int nLastTableDataLength			 // wuqy add 2011.1.25
//  nLastTableDataLength字节的数据
//  nPlayerInfoExItemSize                // =sizeof(TK_SEAT_PLAYER_INFOEX) wuqy add 2017.1.18
//  nSeatCount个TK_SEAT_PLAYER_INFOEX    // wuqy add 2017.1.18

//-----------------------------------------------------------------------------------------
//   功    能：自由桌 设置下一局不再继续
//   说    明：GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MARKPLAYERIDLE (TK_MSG_GAME_RELAY_BEGIN + 6)
// REQ
//  TKHEADER header;
//    header.dwParam:  1设置 0取消
// ACK
//  TKHEADER header;
//    header.dwParam:  1设置 0取消

//-----------------------------------------------------------------------------------------
//   功    能：自由桌 请求加血（补充筹码）
//   说    明：GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDHP (TK_MSG_GAME_RELAY_BEGIN + 7)
// REQ
//  TKHEADER header;
//    header.dwParam:  加血量
// ACK
//  TKHEADER header;
//  DWORD dwUserID
//  int nHPAdded
//  DWORD dwCost

//-----------------------------------------------------------------------------------------
//   功    能：自由桌 请求保存筹码（回兑部分筹码）
//   说    明：GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SAVEHP (TK_MSG_GAME_RELAY_BEGIN + 8)
// REQ
//  TKHEADER header;
//    header.dwParam: 保存到的buy-in倍数

// ACK
// TKHEADER header;
// DWORD dwUserID
// int nHPSaved
// int nGoldAdded;

//-----------------------------------------------------------------------------------------
//   功    能：自由桌 请求一局结束后自己加血
//   说    明：GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MARKAUTOADDHP (TK_MSG_GAME_RELAY_BEGIN + 9)
// REQ
//  TKHEADER header;
//    header.dwParam:  非0标志设置自动加血到dwParam； 0取消
// ACK
//  TKHEADER header;
//    header.dwParam:  非0标志设置自动加血到dwParam； 0取消

//-----------------------------------------------------------------------------------------
//   功    能：通知游戏Dll锁定了本局选手
//   说    明：MS -> GS -> GameDll
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MARKLOCKDOWN (TK_MSG_GAME_RELAY_BEGIN + 10)

//-----------------------------------------------------------------------------------------
//   功    能：通知游戏Dll采用VIP模式界面
//   说    明：MS -> GS -> GameDll
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_VIPMODE (TK_MSG_GAME_RELAY_BEGIN + 11)
// ACK
//  TKHEADER header;
//    header.dwParam:  1设置 0取消

//-----------------------------------------------------------------------------------------
//   功    能：通知GC本场游戏有幸运点功能
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LUCKCARD (TK_MSG_GAME_RELAY_BEGIN + 12)
// REQ
//  TKHEADER header;
// DWORD dwBalance 剩余金额

//-----------------------------------------------------------------------------------------
//   功    能：请求金币兑换幸运点
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDLUCKY (TK_MSG_GAME_RELAY_BEGIN + 13)
// REQ
//  TKHEADER header;
//    header.dwParam: 要补充的幸运点数
// ACK
//  TKHEADER header;
//    header.dwParam: 成功补充的幸运点数(0为失败)
//    char *szReason; 失败原因

//-----------------------------------------------------------------------------------------
//   功    能：告知幸运使用结果
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CONSUMELUCKY (TK_MSG_GAME_RELAY_BEGIN + 14)
// REQ
//  TKHEADER header;
//    header.dwParam: 消耗的幸运点数

//-----------------------------------------------------------------------------------------
//   功    能：通知游戏Dll，岛屿的加血设置
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_EXCHANGE (TK_MSG_GAME_RELAY_BEGIN + 15)
// REQ
//  TKHEADER header;
//    header.dwParam:
// int cnExchange;
// DWORD dwBlood1;
// DWORD dwCost1;
// DWORD dwBlood2;
// DWORD dwCost2;
// DWORD dwBlood3;
// DWORD dwCost3;
// DWORD dwBlood4;
// DWORD dwCost4;

//-----------------------------------------------------------------------------------------
//   功    能：擂台信息
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_CHALLENGERLIST (TK_MSG_GAME_RELAY_BEGIN + 16)
// ACK
// TKHEADER hdr;
// int nWinnerWinCount;		// 擂主当前守擂次数
// int nChallengerCount;	// 挑战者,第一个为擂主
// TKCHALLENGERINFO[]

//-----------------------------------------------------------------------------------------
//   功    能：申请打擂
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_CHALLENGERAPPLY (TK_MSG_GAME_RELAY_BEGIN + 17)
// REQ/ACK
// TKHEADER hdr;
//typedef struct tagTKReqChallengerApply
//{
//	BOOL		bApply;	// 1打擂, 2退擂
//	//若是打擂则后跟一个选手擂台的信息
//}TKREQCHALLENGERAPPLY, *PTKREQCHALLENGERAPPLY;

//-----------------------------------------------------------------------------------------
//   功    能：擂主变化
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_CHALLENGE_UPDATE_WINNERINFO (TK_MSG_GAME_RELAY_BEGIN + 18)
// ACK
// TKHEADER hdr;
//typedef struct tagTKAckUpdateWinnerInfo
//{
//	DWORD		dwWinnerUserID;				// 擂主ID
//	int			nWinnerWinCount;			// 擂主已经连胜次数
//	DWORD		dwReason;					// 信息更新原因
//	TKCHALLENGERINFO	stWinnerInfo;		// 新擂主信息
//	int			nOutPlayerCount;			// 打擂失败或其他导致退出的玩家数量
//	// 后跟nOutPlayerCount个DWORD
//}TKACKUPDATEWINNERINFO, *PTKACKUPDATEWINNERINFO;

//-----------------------------------------------------------------------------------------
//   功    能：下发自由房间用户养成数值信息
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_GROWVALUE (TK_MSG_GAME_RELAY_BEGIN + 19)
// ACK:
//  TKHEADER header;
//	DWORD dwUserID
//  int cnGrowValue
//  cnGrowValue个GROWINFO(DWORD dwGrowID, DWORD dwValue)

//-----------------------------------------------------------------------------------------
//   功    能：自由房间用户标志变更（比如是否桌主标志）
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_CHANGEFLAGS (TK_MSG_GAME_RELAY_BEGIN + 20)
// ACK:
//  TKHEADER header;
//	DWORD dwUserID
//  WORD wFlags   ,参见TKMatchRoomUniversalDicSrv.h中TK_ROOMPLAYER_FLAG定义

//-----------------------------------------------------------------------------------------
//   功    能：自由房间用户桌主替人(将指定座位号上的人踢开)
//   说    明：GC -> GS -> MS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_HOSTKICK (TK_MSG_GAME_RELAY_BEGIN + 21)
// REQ
//  TKHEADER header;
//  int nSeat;                          // 座位号(base 0)

// ACK
//  TKHEADER hdr;
//  int nNotAgainEnter;        //1=禁止再次进入
//  int nErrorCode;            //

//-----------------------------------------------------------------------------------------
//   功    能：服务器询问用户是否同意花费金币加血补充生命值
//   说    明：MS -> GS -> GC
//             如果用户同意则发送TK_MSG_GAME_ADDHP消息
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_QUERYADDHP (TK_MSG_GAME_RELAY_BEGIN + 22)
// ACK:
//  TKHEADER header;
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
#define TK_MSG_GAME_STARTABLE_ACCEPT_INVITE (TK_MSG_GAME_RELAY_BEGIN + 23)
// ACK
//  TKHEADER header;
// REQ
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   功    能：
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_TERMINATE_GC (TK_MSG_GAME_RELAY_BEGIN + 24)
// ACK
//  TKHEADER header;
// REQ
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   功    能：比赛选手列表信息字段定义
//   说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCHPLAYERLIST_DEFINE (TK_MSG_GAME_RELAY_BEGIN + 25)
// ACK:
//  TKHEADER header;
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
#define TK_MSG_GAME_MATCHPLAYERLIST_ADD (TK_MSG_GAME_RELAY_BEGIN + 26)
// ACK:
//  TKHEADER header;
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
#define TK_MSG_GAME_MATCHPLAYERLIST_DEL (TK_MSG_GAME_RELAY_BEGIN + 27)
// ACK:
//  TKHEADER header;
//		header.dwParam: 需要删除的UerID

//-----------------------------------------------------------------------------------------
//   功    能：明星桌切换
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_STAR_TABLE_SEL (TK_MSG_GAME_RELAY_BEGIN + 28)
// REQ:
// TKHEADER hdr;
//   hdr.dwParam: target star-table ID
// ACK:
// TKHEADER hdr;
//   int nCount;
//   TK_STAR_TABLE_SEL_REC[nCount];

//-----------------------------------------------------------------------------------------
//   功    能：团体对抗赛队伍比分
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_DUALMEET_SCORE (TK_MSG_GAME_RELAY_BEGIN + 29)
// REQ/ACK:
// TKHEADER hdr;
// data
struct GSTeamScore
{
    char szTeamName[TK_BUFLEN_USERNAME * 2];
    int nTeamScore;
};

//-----------------------------------------------------------------------------------------
//   功    能：德州锦标赛是否可以随时补充筹码
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_GAMBLE_CAN_REBUY (TK_MSG_GAME_RELAY_BEGIN + 30)
// REQ/ACK:
// TKHEADER hdr;
// int nJackpot 奖池大小
// int nGold;
// int nChip;

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知GS让玩家复活
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_RELIVE_SELECT (TK_MSG_GAME_RELAY_BEGIN + 31)
// REQ:
// TKHEADER hdr;
// int cnLivePlayer;	// 当前存活人数
// int nTimeOut;		// 倒计时
// <select><case id = "1" tip = "花费100金币获得1000筹码"><case id = "2" tip = "花费200金币获得2000筹码"><case id = "3" tip = "花费10奠基石获得1000筹码"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
// 功    能：用户请求复活
// 说    明：GC -> GS -> MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_USER_REQRELIVE (TK_MSG_GAME_RELAY_BEGIN + 32)
// REQ
// TKHEADER hdr;
//	int nSecondUsed;	// 用时
//	int nReliveCase;		// 选择的复活方案

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知GS让玩家add on
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_ADDON_SELECT (TK_MSG_GAME_RELAY_BEGIN + 33)
// REQ:
// TKHEADER hdr;
// int nTimeOut;		// 倒计时
// <select><case id = "0" tip = "花费100金币获得1000筹码"><case id = "1" tip = "花费200金币获得2000筹码"><case id = "2" tip = "花费10奠基石获得1000筹码"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
// 功    能：用户请求add on
// 说    明：GC -> GS -> MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_USER_REQADDON (TK_MSG_GAME_RELAY_BEGIN + 34)
// REQ
// TKHEADER hdr;
//	int nAddonCase;		// 选择的addon方案

//-----------------------------------------------------------------------------------------
//   功    能：团体对抗赛队伍比分
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_DUALMEET_SCOREEX (TK_MSG_GAME_RELAY_BEGIN + 35)
// REQ/ACK:
// TKHEADER hdr;
// data
//struct GSTeamScore
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
#define TKID_MSG_GAME_CV_AWARD_INFO (TK_MSG_GAME_RELAY_BEGIN + 36)
// REQ:
// TKHEADER hdr;
// xml字符串

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知玩家获得连胜奖励
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_CV_AWARD (TK_MSG_GAME_RELAY_BEGIN + 37)
// REQ:
// TKHEADER hdr;
// DWORD cnWin	// 连胜次数

//-------------------------------------------------------------------------------------------------------------------------
//
//	功    能：MS通知玩家获得连胜奖励
// 说    明：MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_REST (TK_MSG_GAME_RELAY_BEGIN + 38)
// REQ:
// TKHEADER hdr;
// 后跟
typedef struct tagGAMEPLAYERINFO
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
} GAMEPLAYERINFO, *PGAMEPLAYERINFO;

//-----------------------------------------------------------------------------------------
//   功    能： MS 推送Team列表
//   说    明： 1、MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MSG_TEAM_LIST (TK_MSG_GAME_RELAY_BEGIN + 39)
// REQ
// TKHEADER hdr;
// int cnTeam;
// 后跟cnTeam个GS2GCTeamInfo
struct GS2GCTeamInfo
{
    char szTeamName[TK_BUFLEN_USERNAME];
    int nScore;
    int nOrder;  // 名次为负表示出局
};

//-----------------------------------------------------------------------------------------
//   功    能： MS 推送玩家列表
//   说    明： 1、MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MSG_PLAYER_LIST (TK_MSG_GAME_RELAY_BEGIN + 40)
// REQ
// TKHEADER hdr;
// int cnPlayer;
// 后跟cnPlayer个GS2GCPlayerInfo
struct GS2GCPlayerInfo
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
#define TKID_MSG_TEAM_WAIT (TK_MSG_GAME_RELAY_BEGIN + 41)
// REQ/ACK:
// TKHEADER hdr;
// 后跟字符串

//-----------------------------------------------------------------------------------------
//   功    能： MS 推送阶段结束信息
//   说    明： 1、MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MSG_TEAM_PHASEOVER (TK_MSG_GAME_RELAY_BEGIN + 42)
// REQ/ACK:
// TKHEADER hdr;
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
#define TKID_MSG_GAME_RELIVE_SELECTEX (TK_MSG_GAME_RELAY_BEGIN + 43)
// REQ:
// TKHEADER hdr;
// int nJackpot;			//总奖金池
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
#define TKID_MSG_GAME_ADDON_SELECTEX (TK_MSG_GAME_RELAY_BEGIN + 44)
// REQ:
// TKHEADER hdr;
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
#define TKID_MSG_GAME_JACKPOT_RAISE (TK_MSG_GAME_RELAY_BEGIN + 45)
// REQ:
// TKHEADER hdr;
// int nJackpot 奖池大小
// int nAverageChip;	 平均筹码

//-----------------------------------------------------------------------------------------
//   功    能：游戏桌切换
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_CHGTABLE (TK_MSG_GAME_RELAY_BEGIN + 46)
// REQ
//  TKHEADER header;
// ACK
//  TKHEADER header;
//  hdr.dwParam: target table ID

//-----------------------------------------------------------------------------------------
//   功    能：进入/退出/更新等待队列
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_WAITINLIST (TK_MSG_GAME_RELAY_BEGIN + 47)
// REQ
//  TKHEADER header;
// hdr.dwParam: 0退出等待队列，1进入等待队列
// ACK
//  TKHEADER header;
//  hdr.dwParam: 0不在等待队列，n(>0)队列中的位置
//  DWORD dwWaitCount: 等待队列中人数

//-----------------------------------------------------------------------------------------
//   功    能：比赛选项
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCH_OPTION (TK_MSG_GAME_RELAY_BEGIN + 48)
// REQ
//  TKHEADER header;
//  DWORD dwFlags: Bit0:支持进入队列按钮    Bit1:支持换桌按钮 Bit2:自动rush

//-----------------------------------------------------------------------------------------
//   功    能：上传用户是否点过"自动补充筹码"checkbox
//   说    明：GC-->GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_AUTO_ADDHP_TOUCHED (TK_MSG_GAME_RELAY_BEGIN + 49)
// REQ:
//  TKHEADER header;
//  header.dwParam: 0: 没点过 1: 点过了

//-----------------------------------------------------------------------------------------
//
//	功    能：通知让玩家LuckCard配置信息
// 说    明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LUCKCARDINFO (TK_MSG_GAME_RELAY_BEGIN + 50)
// REQ
// TKHEADER hdr;
// 后跟xml，格式如下:
// <LuckCard>
// 	<!--AddCfg幸运点补充配置，Item为可选档，Def为默认档-->
// 	<AddCfg Item="10000,20000,50000,100000,200000" Def="100000"/>
//
// 	<!--UseMin幸运点应用最小限制（低于此数值不应用幸运点），Item为可选档，Def为默认档-->
// 	<UseMin Item="0,10000" Def="10000"/>
//
// 	<!--UseMin幸运点应用最大限制（大于此数值时限定为此数值），Item为可选档，Def为默认档-->
// 	<UseMax Item="20000,50000" Def="20000"/>
// </LuckCard>

//-----------------------------------------------------------------------------------------
//   功    能：用户LuckCard选项
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LUCKCARD_USEROPTION (TK_MSG_GAME_RELAY_BEGIN + 51)
// REQ
//  TKHEADER header;
//  int nUseMin 单次使用下限（低于该值不应用）
//  int nUseMax 单次使用上限（高于该值按该值应用）

//-----------------------------------------------------------------------------------------
//   功    能：用户上传位置信息搜索附近玩家
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_SCAN (TK_MSG_GAME_RELAY_BEGIN + 52)
// REQ/ACK:
// TKHEADER hdr;
// double lat;
// double lng;

//-----------------------------------------------------------------------------------------
//   功    能：用户请求进入指定桌
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ENTER_TABLE (TK_MSG_GAME_RELAY_BEGIN + 53)
// REQ/ACK:
// TKHEADER hdr;
// DWORD dwTableID;
// 返回ACK header.dwParam : 1 == 没有这个桌号		2 == 指定的桌已坐满    3 == 正在其它桌游戏

//-----------------------------------------------------------------------------------------
//   功    能：用户请求离开桌子
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LEAVE_TABLE (TK_MSG_GAME_RELAY_BEGIN + 54)
// REQ/ACK:
// TKHEADER hdr;

//-----------------------------------------------------------------------------------------
//   功    能：用户请求帐单
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_BILL (TK_MSG_GAME_RELAY_BEGIN + 55)
////////// REQ:
// TKHEADER hdr;

/////////ACK
// TKHEADER hdr;
// TKGameLBSBill bill;		// 帐单明细
struct TKGameLBSBill
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
#define TKID_MSG_GAME_FORWARDBEGIN (TK_MSG_GAME_RELAY_BEGIN + 56)
typedef struct tagTKGSFORWARDBEGIN
{
    TKHEADER header;
    size_t timelong;   //争先赛比赛时长，单位s
    UINT64 timebegin;  //比赛开始时间单位 0.1微s
    tagTKGSFORWARDBEGIN()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDBEGIN, *PTKGSFORWARDBEGIN;

//MS<->GS 请求当前排名列表和回复
#define TKID_MSG_GAME_FORWARDORDER (TK_MSG_GAME_RELAY_BEGIN + 57)
typedef struct tagTKGSFORWARDORDERREQ
{
    TKHEADER header;
    tagTKGSFORWARDORDERREQ()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDORDERREQ, *PTKGSFORWARDORDERREQ;
struct gsForwardplayerinfo
{
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];
    int nGrade;      //争先赛成绩
    size_t nRank;    //争先赛排名
    size_t usetime;  //争先赛使用时长
    size_t hand;     //以完成局数
    gsForwardplayerinfo()
    {
        memset(this, 0, sizeof(*this));
    }
};
typedef struct tagTKGSFORWARDORDERACK
{
    TKHEADER header;
    size_t count;    //后缀gsForwardplayerinfo数量
    bool iscurdata;  //false表示目前不是争先赛进行阶段，显示上次的数据，并且不包含自己。true第1个节点是自己
    tagTKGSFORWARDORDERACK()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDORDERACK, *PTKGSFORWARDORDERACK;

//MS->GS 在争先赛中每次beginhand的通知消息
#define TKID_MSG_GAME_FORWARDBEGINHAND (TK_MSG_GAME_RELAY_BEGIN + 58)
typedef struct tagTKGSFORWARDBEGINHAND
{
    TKHEADER header;
    size_t hand;         //当前局数
    size_t totalhand;    //总局数
    size_t rank;         //当前名次
    size_t playercount;  //当前总人数
    tagTKGSFORWARDBEGINHAND()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDBEGINHAND, *PTKGSFORWARDBEGINHAND;

//MS->GS争先赛结束消息
#define TKID_MSG_GAME_FORWARDEND (TK_MSG_GAME_RELAY_BEGIN + 59)
typedef struct tagTKGSFORWARDEND
{
    TKHEADER header;
    size_t timelong;  //距离下一场争先赛的时长，单位s
    UINT64 timeend;   //比赛结束时间单位 0.1微s
    tagTKGSFORWARDEND()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDEND, *PTKGSFORWARDEND;

//-----------------------------------------------------------------------------------------
//   功    能：用户请求排名信息
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_SEAT_RANK_INFO (TK_MSG_GAME_RELAY_BEGIN + 60)
////////// REQ:
struct TKGameReqGetSeatRankInfo
{
    TKHEADER header;
};

////////// ACK:
struct TKGameAckGetSeatRankInfo
{
    TKHEADER header;
    WORD wGameCount;
    WORD wXmlOffset;
    WORD wXmlLen;
    // char * xml

    inline const char* RankInfo()
    {
        return (const char*)((char*)this + wXmlOffset);
    };
};

//-----------------------------------------------------------------------------------------
//   功    能：自建桌结算账单(行牌过程中的得分汇总)
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_BILL (TK_MSG_GAME_RELAY_BEGIN + 61)
/////////// REQ
// TKHEADER hdr;
// DWORD dwTableID;

/////////// ACK
// TKHEADER hdr;
// TKGameFdTableBill bill;		// 自建桌结算单
struct TKGameFdTableBill
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
#define TKID_MSG_GAME_FDTABLE_FINAL_BILL (TK_MSG_GAME_RELAY_BEGIN + 62)
/////////// REQ
// TKHEADER hdr;
// DWORD dwTableID;

/////////// ACK
// TKHEADER hdr;
// TKGameFdTableFinalBill bill; // 自建桌结算单
struct TKGameFdTableFinalBill
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
#define TKID_MSG_GAME_FDTABLE_PERMITSET (TK_MSG_GAME_RELAY_BEGIN + 63)
// ACK
// TKHEADER hdr;
// char		szNickName[TK_BUFLEN_NICKNAME];		// 玩家昵称
// DWORD dwUserID;
// int nSeat;
//int nWaitPermitSet;

// REQ
// TKHEADER hdr;//hdr.dwParam: 0: 不同意 1: 同意
// DWORD dwUserID;
// int nSeat;
// int nCancel; //1:取消

//-----------------------------------------------------------------------------------------
//   功    能：自建桌局间数据
//   说    明：MS -> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_SCORE (TK_MSG_GAME_RELAY_BEGIN + 64)
// TKHEADER hdr;
// TKGameFdTableScore score;    //得分
struct TKGameFdTableScore
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
#define TKID_MSG_GAME_FDTABLE_INFO (TK_MSG_GAME_RELAY_BEGIN + 65)
//TKHEADER hdr;
//DWORD dwTableID;
//int nGrade;
//int nInitScore;
//int nBase;
//int nGameTime;
//int nPermitSet;

//-----------------------------------------------------------------------------------------
//   功    能：牌桌提前解散
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_PREGAMEEND (TK_MSG_GAME_RELAY_BEGIN + 66)
//REQ/ACK
//TKHEADER hdr;
//DWORD dwTableID;

//-----------------------------------------------------------------------------------------
//   功    能：房主移交(服务器推送)(game)
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDROOM_ROOMHOST_INHERIT (TK_MSG_GAME_RELAY_BEGIN + 67)
//REQ
//TKHEADER hdr;
//DWORD dwRoomID;
//DWORD dwRoomHostID;
//DWORD dwInheritRoomHostID;
//int nReason;

//-----------------------------------------------------------------------------------------
//   功    能：房间转换(game)
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDROOM_TRANSFER (TK_MSG_GAME_RELAY_BEGIN + 68)
//REQ
//TKHEADER hdr;
//DWORD dwRoomID;
//WORD wReserve;

//ACK
//TKHEADER hdr;
//DWORD dwRoomID;
//WORD wReserve;

//-----------------------------------------------------------------------------------------
// 功 能：更新超级庄排队列表
// 说 明：MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_PUSH_SUPERBANKER_LIST (TK_MSG_GAME_RELAY_BEGIN + 69)
//REQ
//TKHEADER hdr;
// DWORD dwWaitCount
// 后跟dwWaitCount个GS2MCBankerInfo
struct GS2MCBankerInfo
{
    DWORD dwUserID;
    DWORD dwFormID;  // 用户形象ID
    int nFigureID;   // 头像ID
    char szNickName[TK_BUFLEN_NICKNAME];
};

//-----------------------------------------------------------------------------------------
//   功    能：自建桌局间数据扩展
//   说    明：GS -> MC
//   备    注：兼容TKID_MS2GS_FDTABLE_SCORE协议
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_SCOREEX (TK_MSG_GAME_RELAY_BEGIN + 70)
// TK_PLAYER_MSG_RELAY_HDR hdr;
// TKFDTABLESCOREEX scoreex;    //得分
// 后面跟cnPlayer个PlayerScore

typedef struct tagTKGAMEFDTABLESCOREEX
{
    int nOffsetInfo;  // =sizeof(TKGAMEFDTABLESCOREEX)
    int cnPlayer;     // 后跟cnPlayer个PlayerScore
    tagTKGAMEFDTABLESCOREEX()
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
} TKGAMEFDTABLESCOREEX, *PTKGAMEFDTABLESCOREEX;

//-----------------------------------------------------------------------------------------
//   功    能：自建桌手动开赛
//   说    明：MS <-> GS <-> GC
//   备    注：
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_MANUALBEGIN (TK_MSG_GAME_RELAY_BEGIN + 71)
//REQ/ACK
//TKHEADER hdr;

//-----------------------------------------------------------------------------------------
//   功    能：用户发送附加信息
//   说    明：GC->GameSvr -> MatchSvr
//   备    注：
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_HDL_INFO (TK_MSG_GAME_RELAY_BEGIN + 72)
// REQ
// dwResult
// sufData

//-----------------------------------------------------------------------------------------
//   功    能：MS通知客户端奖励
//   说    明：MS -> GS -> GC
//   备    注：
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PROCESSAWARD (TK_MSG_GAME_RELAY_BEGIN + 73)
//TKHEADER hdr;
//DWORD dwCount; //(循环数据包括dwUserID+dwBizID+dwAwardID+dwLen+award)
//DWORD dwUserID; //
//DWORD dwBizID //过程奖励业务ID
//DWORD dwAwardID //过程奖励ID
//DWORD dwLen //后续json串长度
//json  award //过程奖励json串
//...

//-----------------------------------------------------------------------------------------
//   功    能：GC通知MS拆红包
//   说    明：GC -> GS -> MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OPENREDPACKET (TK_MSG_GAME_RELAY_BEGIN + 74)
//TKHEADER hdr;
//DWORD dwRPID; //红包ID，对应上个消息中的dwAwardID
//DWORD dwBizID; //过程奖励业务ID
//DWORD dwPlanID; //过程奖励方案ID
//BOOL  bFinal; //是否最后一个红包

//-----------------------------------------------------------------------------------------
//   功    能：GC通知MS拆红包
//   说    明：GC <-> GS <-> MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CONFIRMAWARDINFO (TK_MSG_GAME_RELAY_BEGIN + 75)
//REQ:
//TKHEADER hdr;
//DWORD dwCount;
//后缀dwCount个（dwAwardID+dwBizID+dwPlanID）

//ACK:
//TKHEADER hdr;
//DWORD dwCount;
//后缀dwCount个dwAwardID

//-----------------------------------------------------------------------------------------
//功能：GC与MS间的透传通用消息，新增透传消息只需新增定义dwProtocolType及后缀数据格式，无需再升级中转
//说明：GC <-> GS <-> MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_COMMONPROTOCOL (TK_MSG_GAME_RELAY_BEGIN + 76)
//TKHEADER hdr;
//DWORD dwProtocolType; //区分具体的消息
//TKSUFFIXIDX suffixJsonData; 后缀业务数据
//json

//客户端与比赛服务间的透传通用协议
#define GC_MS_COMMONPROTOCOL_QUIT_MATCH 0  //客户端请求退赛

//-----------------------------------------------------------------------------------------
//   功    能：比赛游戏服务器给比赛客户端推送复式赛结果信息
//   说    明：GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHDUPLEXRESULT (TK_MSG_GAME_RELAY_END + 1)
typedef struct tagTKACKGSPUSHDUPLEXRESULT
{
    TKHEADER header;
    int lenText;  //文本长度 包含结束符\0
} TKACKGSPUSHDUPLEXRESULT, *PTKACKGSPUSHDUPLEXRESULT;

//-----------------------------------------------------------------------------------------
//   功    能：用户请求结果信息
//   说    明：GC<-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_ROUND_RESULT (TK_MSG_GAME_RELAY_END + 2)
////////// REQ:
struct TKGameReqGetRoundResult
{
    TKHEADER header;
    WORD wCardSetNo;
};

////////// ACK:
struct TKGameAckGetRoundResult
{
    TKHEADER header;
    WORD wResultCount;
    WORD wResultOffset;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wCardSetNo;

    // 后跟wResultCount个SdgrRoundResult
    // SdgrRoundResult array

    //	inline SdgrRoundResult * ResultList(){ return ( SdgrRoundResult * )( ( char * )this + wResultOffset ); };
};

//-----------------------------------------------------------------------------------------
//   功    能：用户请求录像信息
//   说    明：GC<-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_ROUND_RECORD (TK_MSG_GAME_RELAY_END + 3)
////////// REQ:
struct TKGameReqGetRoundRecord
{
    TKHEADER header;
    WORD wRoundID;
};

////////// ACK:
struct TKGameAckGetRoundRecord
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wRecordLen;
    WORD wRecordOffset;

    // 后跟xml字符串
    // char * xml;

    inline const char* RoundRecord()
    {
        return (const char*)((char*)this + wRecordOffset);
    };
};

//-----------------------------------------------------------------------------------------
//   功    能：用户请求比赛进度存档
//   说    明：MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCH_ARCHIVE (TK_MSG_GAME_RELAY_END + 4)
////////// REQ:
struct TKGameReqMatchArchive
{
    TKHEADER header;
    BYTE byArchive;  // 存档标记，0表示取消存档，1表示存档
};

////////// ACK:
struct TKGameAckMatchArchive
{
    TKHEADER header;
};

//-----------------------------------------------------------------------------------------
//   功    能：用户请求交换牌权
//   说    明：GC->GS->MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GETCARDRIGHT (TK_MSG_GAME_RELAY_END + 5)
typedef struct tagTKGameReqGetCardRight
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwWatcheeID;
    int nOperationMode;  // 0-旁观者请求，1-Player同意，2-Player拒绝

} TKGAMEREQGETCARDRIGHT, *PTKGAMEREQGETCARDRIGHT;

typedef struct tagTKGameAckGetCardRight
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwWatcheeID;
    int nOperationMode;  // 0-旁观者请求，1-Player同意，2-Player拒绝
    int nFigureID;
    int nWatcherFigureID;
    char szNickName[TK_BUFLEN_NICKNAME];
    char szWatcherNickName[TK_BUFLEN_NICKNAME];

} TKGAMEACKGETCARDRIGHT, *PTKGAMEACKGETCARDRIGHT;

//-----------------------------------------------------------------------------------------
//   功    能：用户请求保存牌局信息
//   说    明：GC->GS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SAVECARDINFO (TK_MSG_GAME_RELAY_END + 6)
typedef struct tagTKGameReqSaveCardInfo
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwTourneyID;

    DWORD dwReserve1;  // 保留1
    DWORD dwReserve2;  // 保留2

} TKGAMEREQSAVECARDINFO, *PTKGAMEREQSAVECARDINFO;

//-----------------------------------------------------------------------------------------
//   功    能：打开结算界面
//   说    明：GS->GC
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OPEN_ACCOUTINTERFACE (TK_MSG_GAME_RELAY_END + 7)
typedef struct tagTKGameReqOpenAccountInterface
{
    TKHEADER header;
    DWORD dwMPID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwReserve1;     //保留字段1
    DWORD dwReserve2;     //保留字段2
    TKSUFFIXIDX sufData;  //后缀数据
} TKGAMEREQOPENACCOUNTINTERFACE, *PTKGAMEREQOPENACCOUNTINTERFACE;
//后缀数据"<open name=%s order=%d formid=%u figid=%u memname=%s memorder=%u memformid=%u,memfigid=%u memstate=%u total=%u />"

//-----------------------------------------------------------------------------------------
//   功    能：更新结算信息
//   说    明：GS->GC
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_UPDATE_ACCOUTINFOR (TK_MSG_GAME_RELAY_END + 8)
typedef TKGAMEREQOPENACCOUNTINTERFACE TKGAMEREQUPDATEACCOUNTINFOR;
// 后缀数据"<update memname=%s memorder=%u memformid=%u,memfigid=%u memstate=%u total=%u />"

//-----------------------------------------------------------------------------------------
//   功    能： GS和GTS服务间数据交互
//   说    明： GS <-> GTS
//-----------------------------------------------------------------------------------------
#define TKID_GS2GTS_GAMEMSG (TK_MSG_GAME_RELAY_END + 651)
typedef struct tagTKREQGS2GTSGAMEMSG
{
    TKHEADER header;

    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wGameIndex;

    WORD wGameID;
    DWORD dwDataOffset;  // 游戏数据偏移
    DWORD dwDataLength;  // 游戏数据长度

} TKREQGS2GTSGAMEMSG, *PTKREQGS2GTSGAMEMSG;

// ----------------------------------------------------------------------------------------
