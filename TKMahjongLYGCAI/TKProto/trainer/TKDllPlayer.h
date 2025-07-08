#pragma once
#include <string>
#include "TKProtocol.h"
#include "TKFixList.h"

//#include "TKDllObjectLock.h"
//#include "TKGameDllBaseType.h"
#include "TKBuffer.h"
//#include "TKDllGameInterface.h"
#include "TKDllObject.h"
#include "TKStatTypeDefine.h"

#define TK_PLAYERNETSTATUS_NORMAL 0
#define TK_PLAYERNETSTATUS_BREAK 1

class CTKGamePlayer : public CTKObject
{
public:
    CTKGamePlayer(void);
    virtual ~CTKGamePlayer(void);

public:
    //BOOL Init(HEXEGAMEPLAYER hExePlayer, PTKDLLPLAYERCTREATEINFO pInfo);
    //BOOL Init(HEXEGAMEPLAYER hExePlayer, PTKDLLPLAYERCTREATEINFOV2 pInfo);
    //HEXEGAMEPLAYER GetPlayerShell()
    //{
    //    return m_hExePlayer;
    //};
    //BOOL DoFuncCall(PTKDLLGAMECALLPARAM lpCallParam, const char* pszFile, int nLine, char* Func);
    //BOOL DoEventCall(PTKDLLGAMECALLPARAM lpCallParam);
    //static BOOL WINAPI OnEventCall(PTKDLLGAMECALLPARAM lpCallParam);

public:
    LONG AddRef();
    LONG Release();

protected:
    //CTKObjectLock m_clsLock;

private:
    LONG m_cnRef = 1;

public:
    virtual BOOL OnInitialUpdate();
    virtual BOOL OnTickCount();
    virtual void OnDataBalance();  //用户数据结算（注意要保证不发生重复结算！）
    virtual BOOL EnableEmotion(DWORD dwUserIDFrom, DWORD dwUserIDTo)
    {
        return TRUE;
    };

public:
    //
    void SetGameStart(DWORD dwCardPower, DWORD dwReserve1, DWORD dwReserve2, DWORD dwReserve3, DWORD dwReserve4,
        DWORD dwReserve5, DWORD dwReserve6, DWORD dwReserve7, DWORD dwReserve8);
    void SetGameResult(int nWinPlus, int nLossPlus, int nDrawPlus, int& nScorePlus, int nScoreType,
        PTKSTAGERULER pStageRuler, BOOL bValideHand = TRUE, BOOL bLevyTax = TRUE, WORD wGameResult = TK_UNKNOWN);
    void SetGameResult(int nWinPlus, int nLossPlus, int nDrawPlus, int64_t& i64ScorePlus, int nScoreType,
        PTKSTAGERULER pStageRuler, BOOL bValideHand = TRUE, BOOL bLevyTax = TRUE, WORD wGameResult = TK_UNKNOWN);
    void SetGameActor(WORD wGameActor);
    void SetTaxMultiDiv(int nMulti, int nDiv);

    void SendTipMessage(int nTipID, BYTE byShowType, const char* pszTipMessage, BOOL bSend2Watcher = TRUE);
    void SendTipMessage(BYTE byShowType, const char* pszTipMessage, BOOL bSend2Watcher = TRUE);

    void InvokeAddBreakUser();  //如果用户游戏超时了，可以调用一下此函数（有可能用户断线了没有进入游戏）
    void InvokeMasterRound(BOOL bInvoke);  //TRUE表示新增, FALSE表示删除

    int GetTax(TKSTAGERULER* pStageRuler);

    BOOL GetGrowValue(DWORD dwGrowID, int& nValue);
    BOOL SetGrowValue(DWORD dwGrowID, int nValue, BOOL bSave2DB = TRUE);
    BOOL SetGrowValueInit(DWORD dwGrowID, int nValue);
    BOOL AddGrowValue(DWORD dwGrowID, int nValue, BOOL bSave2DB = TRUE);
    void SaveGrowValue();
    void UpdateGrowValue();
    BOOL AddAtomGameEvent(DWORD dwEventID, DWORD dwEventValue, BOOL bImmediate = FALSE,
        int nOverride = 0);  //nOverride 0:累加, 1:直接覆盖, 2:最大覆盖
    void SaveAtomGameEvent(BOOL bImmediate = FALSE);
    //nEKLLogLevel日志级别参考枚举eELKLogLevel
    /*enum eELKLogLevel{
		TK_ELK_LOG_LEVEL_OFF = 0,
		TK_ELK_LOG_LEVEL_FATAL = 1,
		TK_ELK_LOG_LEVEL_ERROR = 2,
		TK_ELK_LOG_LEVEL_WARN = 3,
		TK_ELK_LOG_LEVEL_INFO = 4,
		TK_ELK_LOG_LEVEL_DEBUG = 5,
		TK_ELK_LOG_LEVEL_TRACE = 6,
		TK_ELK_LOG_LEVEL_ALL = 7
	};*/
    BOOL TKWriteELKLogFile(int nEKLLogLevel, LPCTSTR pszFmt, ...);
    // 上报信息
    // dwType:1,通过日志方式上报,需要设置日志等级(nEKLLogLevel)和日志内容
    // dwType:2,通过消息上报,只需要设置日志内容
    BOOL TKReportMsg(DWORD dwType, int nEKLLogLevel, LPCTSTR pszFmt, ...);
    // CMA 加/减统计指标
    BOOL CMAIncreaseCnt(const std::string& strName, DWORD dwMPID, DWORD dwGameID, int nIncrease = 1);
    // CMA Set 统计指标
    BOOL CMASetCnt(const std::string& strName, DWORD dwMPID, DWORD dwGameID, int value = 0);

    // For 捕鱼
    void PostExitMatch(BOOL bDirExit);
    void SendMsg(void* pMsg);
    bool WriteShareData(const char* pData, int nLen);
    bool ReadShareData(char** pData, int& nLen);
    void DeleteShareData();

public:
    void Terminate(BOOL bWhenSendOver = FALSE);
    void EnableBotEnter(BOOL bEnable)
    {
        m_bEnableBotEnter = bEnable;
    }

public:
    BOOL IsBotUser();
    BOOL IsExitGame();
    BOOL IsLobbyOnLine();  //开始时大厅是否在线
    BOOL IsMsTraced();     //是否有作弊嫌疑标记

public:
    int64_t Chip()
    {
        return m_nScore;
    };

public:
    void __Initialize(int seat, int chip);

public:
    //
    DWORD m_dwUserID{};
    char m_szNickName[TK_BUFLEN_NICKNAME]{};
    int m_nFigureID{};   //用户头像ID
    DWORD m_dwFormID{};  //用户形象ID wuqy add 2017.4.5
    WORD m_wCntTID{};    //大厅客户端链接类型 eLobbyConnetType
    BYTE m_byOSType{};
    DWORD m_dwAppID{};
    DWORD m_dwSiteID{};
    DWORD m_dwMoneyAccType{};
    DWORD m_dwLanguageID{};  // 客户端语言版本ID

    //
    int m_nGameID{};
    int m_nProductID{};
    DWORD m_dwTourneyID{};  //用户所在比赛的TourneyID
    DWORD m_dwMatchID{};    //用户所在比赛的MatchID

    //
    CDword m_dwTeamID{};
    CDword m_dwCaptainID{};  //在团赛中，队长的UserID

    //
    CBool m_bEnterRound{};
    CBool m_bBaseTaxed{};
    CBool m_bCanExitGame{};   // 给TV项目的裁判端控制选手退出的变量(参见LordTV工程) wuqy 2014.9.30
    CBool m_bRoundWatcher{};  //是否是旁观者，此变量用来区分上发消息的客户端身份
    CBool m_bEnableBotEnter{};

    CByte m_byBreakHeartTask{};  // 是否参加了碎心任务 wuqy 2016.6.3
    CByte m_byHeartCount{};  // 用户的HeartCount，即比赛中用户命的条数（cnMatchLive）wuqy add 2016.12.31

    CInt m_nGameExpScore{};   // 游戏经验值（来自UserData Server）
    CInt m_nMatchRank{};      //游戏服务器根据比赛积分计算出来的
    CInt m_nEnterOrder{};     //进入时的排名
    CInt m_nPlaceOrder{};     //
    CInt m_nOutRoundState{};  //出局状态，0没有出局，>0出局
    CInt m_nSeatOrder{};

    //
    CInt64 m_nScore{};        //比赛积分
    CInt m_nMatchScore{};     // 比赛积分
    CInt m_nMatchSecScore{};  // 比赛小分
    CInt m_nRoundScore{};     // 局制积分
    CInt m_nRoundSecScore{};  // 局制小分

    //Round累计赢输和
    CInt m_nRoundWins{};
    CInt m_nRoundLosses{};
    CInt m_nRoundDraws{};

    //Round累计赢得的分数、副数、扣费
    CInt m_nRoundWinScore{};
    CInt m_nRoundGameCount{};
    CInt m_nRoundTax{};

    //Round最后盘游戏得分赢输和
    CInt m_nLastScore{};  //最后一盘的税前得分(如果扣税非基类操作而是游戏服务自己操作，则该分也变成了税后分)
    CInt m_nLastScoreTax{};  //最后一盘的税后得分（目前只用于客户端结果播报）wuqy 2014.10.23
    CInt m_nLastWins{};
    CInt m_nLastLosses{};
    CInt m_nLastDraws{};
    CInt m_iCurGameHand{};  // 用户当前进行的局制编号：第%d局（开局时累计。为0是表示没有打过，不显示）
    CInt m_cnGameCurFinished{};  // 当前完成的总盘数
    CInt m_cnGameTotFinished{};  // 比赛完成的总盘数
    //
    CInt m_nBroadcastDelay{};  //转播延迟时间(秒)
    CInt m_nVipSortValue{};    //明星对局VIP权重
    CInt m_nMatchStatus{};     //用户当前的比赛状态
    CInt m_nNetStatus{};       //网络状态：0正常连接，1断线，2重连
    CInt m_nOpertorType{};     //游戏操作者类型

    CWord m_wGameResult{};  // 游戏结果
    CWord
        m_wGameActor{};  // 游戏规则定义的本局游戏角色，数值由每个游戏自己定义，在特定的Stage重组时使用 wuqy 2016.12.31

    CDword m_dwChatChannelID{};
    CDword m_dwChatChannelSubID{};
    CDword m_dwFlags{};  //一些标志 0位表示是否确认退出了比赛
    CDword m_dwMsTraceFlags{};
    CDword m_dwVersionMatchClient{};
    CDword m_dwVersionGameDll{};

    CInt m_nWatcheeSeat{};
    CDword m_dwWatcheeID{};
    CDword m_dwWatcheeFlags{};

    CTime_t m_timeMatchBreak{};
    CTime_t m_timeBeginMatch{};  //开始比赛的时间
    CTime_t m_timeExitMatch{};   //退出比赛的时间

    CBool m_bPacerBot{};  // 高级AI仿真人

    DWORD m_dwMsIP{};
    WORD m_wMsPort{};
    CByte m_byPacerBotLevel{};

    CInt m_nGameOrder{};          // 桌排名，从1开始
    CByte m_byUserTypeTag{};      //玩家标签
    CInt m_nEndGameCurMission{};  //残局当前关卡序号
    CInt m_nDomain{};

private:
    //HDLLGAMEPLAYER m_hExePlayer;
    //FUNC_DLLGAME_CALLENTRY m_pCallEntry;
};
//extern LONG tk_cnTKDllPlayer;
//extern LONG tk_alDPEventCall[TK_DP_EVENTCALL_MAX];
