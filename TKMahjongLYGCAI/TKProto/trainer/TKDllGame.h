#pragma once
#include <sstream>
#include "framework.h"
#include "TKDllPlayer.h"
#include "TKMatchS2GameSProtocolSrv.h"
#include "TKMatchUniversalDicSrv.h"
#include "TKMatchRuler.h"
#include "TKGameProtocol.h"
#include "TKBotPlayer.h"
#include "json/json.h"

int TKGenRandom();

struct GameConfig
{
    std::string propertyEx;
    size_t playerCount{};
    int scoreBase{};
    int initialChip{};
};

class CTKGame : public CTKObject
{
public:
    int m_iGameHand = 0;
    bool m_IsRunning = false;

public:
    CTKGame();
    virtual ~CTKGame();

public:
    virtual BOOL OnInitialUpdate();
    virtual int GetLastHandSaveInfoLen();
    virtual BOOL OnGameBegin();
    virtual BOOL OnPlayerMsg(DWORD dwUserID, int nSeatOrder, PTKHEADER pMsg);
    virtual BOOL OnMatchMsg(PTKHEADER pMsg);
    virtual void OnPlayerNetBreak(CTKGamePlayer* pGamePlayer);
    virtual void OnPlayerNetResume(CTKGamePlayer* pGamePlayer);
    virtual void OnPlayerArrived(CTKGamePlayer* pGamePlayer);
    virtual BOOL OnResetDump();
    virtual void OnGameTimer(DWORD dwID, ULONG_PTR ulUserData);
    virtual void OnPlayerExitMatch(CTKGamePlayer* pGamePlayer);
    virtual BOOL PlayerCanLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason);
    virtual void OnPlayerLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason);
    virtual BOOL OnGTSMsg(const LPVOID pData, DWORD dwLength);
    BOOL PostMsg2GTS(const LPVOID pData, DWORD dwLength);
    BOOL SendAdditionalInfo2MS(PTKHEADER in_pAdditionalMsg);

public:
    void AddGameAction(char* szActionDescribe);
    void AddGameResult(char* szResultDescribe);

    //取得指定座位的GamePlayer
    CTKGamePlayer* GetGamePlayerBySeat(int nSeat);

    //取得指定UID的GamePlayer
    CTKGamePlayer* GetGamePlayerByUserID(DWORD dwUserID);
    int GetGamePlayerCount();

    //设置用户的游戏结果
    void SetGameResult(int nSeat, int nWinPlus, int nLossPlus, int nDrawPlus, int& nScorePlus, BOOL bValideHand = TRUE,
                       BOOL bLevyTax = TRUE, WORD wGameResult = TK_UNKNOWN);
    void SetGameResult(int nSeat, int nWinPlus, int nLossPlus, int nDrawPlus, int64_t& i64ScorePlus,
                       BOOL bValideHand = TRUE, BOOL bLevyTax = TRUE, WORD wGameResult = TK_UNKNOWN);

    //取得积分变化上下限
    BOOL GetMaxScorePlus(int nSeat, int& nScorePlus);
    BOOL GetMaxScorePlus(int nSeat, double& dScorePlus);

    BOOL PostGame2MatchMsg(const uint64_t in_i64uUserID, const uint32_t in_MsgType,
                           const char* const in_pchGame2MatchMsgJson);

    //一个HandGame结束后的调用
    BOOL NotifyGameOver(WORD wOverType, BOOL bNeedOverRound = FALSE, WORD wOverFlags = TK_GAMEOVERFLAG_NULL);

    BOOL Broadcast(PTKHEADER pMsg);
    BOOL Send2SeatPlayer(int nSeat, PTKHEADER pMsg);
    BOOL Send2Black(PTKHEADER pMsg);
    BOOL Send2SeatBlack(int nSeat, PTKHEADER pMsg);
    BOOL Send2Observer(PTKHEADER pMsg);
    BOOL Send2Player(DWORD dwUserID, PTKHEADER pMsg);

    void RequestFDTablePreGameEnd();

public:  // call __SetXXX befor __Initialize
    void __Initialize(const GameConfig& conf);

    void __SendAck(int seat, PTKHEADER msg);
    void __OnPlayerMsg(int seat, PTKHEADER pMsg);
    bool __TrigerTimer();

    void __FinishHand();
    bool __IsRunning() const;

    bool __GetAction(std::string& action);
    void __GetCommonData(Json::Value& act);
    bool __HandAction(const std::string& res);

private:
    void _SetPropertyEx(std::string const& configPropertyEx);

public:
    int m_nGameID = 0;
    int m_nProductID = 0;
    WORD m_wStageID = 0;
    WORD m_wRoundID = 0;
    WORD m_wTableID = 0;
    DWORD m_dwTourneyID = 0;
    DWORD m_dwMatchID = 0;

    int m_nScoreBase = 1;  // 游戏基数，比方400

    int m_nScoreType = TK_GAME_SCORETYPE_LIFE;

    PTKGAMERULER m_pRuler = nullptr;  //游戏规则
    TKSTAGERULER m_stStageRuler{};

    CTKMatchRuler* m_pMatchRuler{};

    char* m_pShareGameData{};  // 游戏对象共享数据区
private:
    std::vector<std::shared_ptr<CTKGamePlayer>> m_GamePlayers;
    std::vector<std::shared_ptr<CTKBotPlayer>> m_BotPlayers;
    std::function<void(int seat, PTKHEADER msg)> m_Sender;

public:
    // 当用户满足某种游戏规则时给予某种奖励(nGameRuler游戏规则定义，nAwardID奖励方案配置)
    BOOL GameRulerAward(CTKGamePlayer* pGamePlayer, int nGameRuler, int nAwardID);
    BOOL GameRulerAwardByPlayer(DWORD dwUserID, int nGameRuler, int nAwardID);
    BOOL GameRulerAwardBySeat(int nSeat, int nGameRuler, int nAwardID);

    BOOL HunterAwardWare(CTKGamePlayer* pGamePlayer, DWORD dwWareID, int nAmount);  //猎杀物品奖励 FryUpgrade
    BOOL HunterAwardWareByPlayer(DWORD dwUserID, DWORD dwWareID, int nAmount);
    BOOL HunterAwardWareBySeat(int nSeat, DWORD dwWareID, int nAmount);

    BOOL BreakHeartAward(CTKGamePlayer* pGamePlayer, int nAddMatchScore,
                         int nChgHeartCount);  //nAddMatchScore需要奖励的比赛积分，nChgHeartCount需要变化的“心”
    BOOL BreakHeartAwardByPlayer(DWORD dwUserID, int nAddMatchScore,
                                 int nChgHeartCount);  //nAddMatchScore需要奖励的比赛积分，nChgHeartCount需要变化的“心”
    BOOL BreakHeartAwardBySeat(int nSeat, int nAddMatchScore,
                               int nChgHeartCount);  //nAddMatchScore需要奖励的比赛积分，nChgHeartCount需要变化的“心”

    BOOL HandWinAward(
        CTKGamePlayer* pGamePlayer, DWORD dwSubType, DWORD dwParam,
        CTKFixList<DWORD>& lsShowUID);  //赢牌奖励，dwSubType为子类型（地主赢或农民赢等）,dwParam为扩展定义参数
    BOOL HandWinAwardByPlayer(DWORD dwUserID, DWORD dwSubType, DWORD dwParam, CTKFixList<DWORD>& lsShowUID);
    BOOL HandWinAwardBySeat(int nSeat, DWORD dwSubType, DWORD dwParam, CTKFixList<DWORD>& lsShowUID);

    virtual BOOL NotifyHttpBrokerAckMsg(const LPVOID pData, DWORD dwLength);  // 由派生类完成
    virtual BOOL GameProcessData(const char* szData, DWORD dwDataLength, const char* szReserve,
                                 DWORD dwReserveLength);  // 由派生类完成
    BOOL PostHttpBrokerMsg(const LPVOID pData, DWORD dwLength);
    BOOL PosGameProcessData2MS(char* szGameProcessData, char* szReserve = 0, DWORD dwReserve1 = 0, DWORD dwReserve2 = 0,
                               WORD wReserve1 = 0);

public:
    BOOL InitialPlayer(CTKGamePlayer* pGamePlayer);
    BOOL InsertPlayer(CTKGamePlayer* pGamePlayer);
    BOOL RemovePlayer(CTKGamePlayer* pGamePlayer);

    BOOL InitialWatcher(CTKGamePlayer* pWatcher);
    BOOL InsertWatcher(CTKGamePlayer* pWatcher);
    BOOL RemoveWatcher(CTKGamePlayer* pWatcher);

    //
    BOOL SetGameTimer(DWORD dwID, ULONG_PTR ulUserData, int nSecond);
    BOOL SetGameTimerMilli(DWORD dwID, ULONG_PTR ulUserData, DWORD dwMilliseconds);
    void KillGameTimer(DWORD dwID);
    BOOL ValidTimer(DWORD dwID);

private:
    struct TimerData
    {
        DWORD dwID{};
        ULONG_PTR ulUserData{};
        int64_t trigerTime{};
    };
    std::deque<TimerData> m_Timers;
    int64_t m_CurTime{};  //for virtual Timer.
    int m_CurSeat{-1};
};
