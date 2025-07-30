#pragma once
#include "TKJunQiCore.h"
#include "TKJunQiPlayer.h"

class CTKJunQiPKGame;
class CTKJunQiPKData
{
    typedef void (CTKJunQiPKData::*_onTimeout)(ULONG_PTR ulUserData);

public:
    CTKJunQiPKData();
    ~CTKJunQiPKData();

public:
    //初始化
    void OnGameBegin(CTKJunQiPKGame* pGame, const string& szCfg);
    //消息
    bool OnPlayerMsg(int seatOrder, const JunQiPKReqMsg& reqMsg);
    //超时
    void OnTimeOut(DWORD dwID, ULONG_PTR ulUserData);

    void OnPlayerNetBreak(int nSeat);
    void OnPlayerNetResume(int nSeat);

private:
    //初始化
    void InitTimeout();
    void _initPlayers();
    CTKJunQiPlayer* _getPlayer(int nSeat);

    //消息
    bool OnInitRuleMsg(int seat, const InitRuleReq& rReq);
    bool OnInitLayoutMsg(int seat, const InitLayoutReq& rReq);
    bool OnCanCelLayoutMsg(int seat);
    bool OnSelectChessMsg(int seat, const SelectChessReq& rReq);
    bool OnMoveChessMsg(int seat, const MoveChessReq& rReq);
    bool OnActionFinishMsg(int seat, const AcotionFinishReq& rReq);
    bool OnPeaceMsg(int seat, const PeaceReq& rReq);
    bool IsLegalPeaceStatus();
    bool OnPeaceResMsg(int seat, const PeaceResReq& rReq);
    bool OnShowChessMsg(int seat, const ShowChessReq& rReq);
    bool OnFeelMsg(int seat, const FeelReq& rReq);
    bool OnConcedeMsg(int seat, const ConcedeReq& rReq);

    //超时
    void OnSetRuleTimeOut(ULONG_PTR ulUserData);
    void OnInitTimeOut(ULONG_PTR ulUserData);
    void OnMoveChessTimeout(ULONG_PTR ulUserData);
    void OnActionTimeout(ULONG_PTR ulUserData);
    void OnPeaceTimeout(ULONG_PTR ulUserData);
    void OnRoundTimeout(ULONG_PTR ulUserData);

    //发送消息
    void SendInitRuleAck();
    void SendInitChessAck();
    void sendInitLayoutAck();
    void SendStartMoveChessAck();
    void SendSelectChessAck();
    void SendMoveChessAck(int nType, int nResult, int nEvent, vector<int>& path);
    void SendActionFinishAck(const vector<int>& vOverSeat);
    void SendPeaceAck();
    void SendFinishGameAck(int nEndType);
    void SendTips(int nTips, int nSeat = JUNQIPK_UNKNOWN, int reducedTime = 0);
    void SendRepeatCount(int nRepeatSeat, int nCount, int nSeat = JUNQIPK_UNKNOWN);
    void SendShowChessAck(int nPos, std::shared_ptr<SChess> pChess, int seat);
    void _sendRefusePeaceAck(int seat);

    //事件
    bool IsAllReady();
    void _onAllReady();
    void MoveChess(int nSeat, const MoveChessReq& rReq, vector<int>& path);
    bool IsNeedConfirm(int nSeat, const MoveChessReq& rReq, int nChessIndex);
    bool CheckPass(int seat, const MoveChessReq& rReq);
    void SendTimeOutAck(int nSeat);
    void Fight(std::shared_ptr<SChess> pFrom, std::shared_ptr<SChess> pTo, int& nResult, int& nEvent);
    void ChessDead(int nKillSeat, int nChessType, int& nEvent);
    void SetWaitAction();                          //设置等待执行动作
    void GetSLDeadSeat(vector<int>& vSLDeadSeat);  //获取刚刚死了司令的玩家座位号
    void _getOverSeat(vector<int>& vOverSeat);
    bool _isLoss(int nSeat, int& nTips);
    void CheckColorSet(int nSeat, int nColor, ShowChessAck* pAck);
    void ConfirmColor(int nSeat, int nColor, ShowChessAck* pAck);
    void _creatJunQiCore();
    void _getAllyLayout(int nSeat, ostringstream& ss);
    int _getNextSeat(int nSeat);
    void _procAgreePeace(int seat);

    //结束处理
    bool IsGameOver();
    bool _isAllyWin(int nAlly);

    bool IsPeace(int& nTips, int& nEndType);
    void FinishGame(int nSeat, int nCurResult, int nEnemyResult, int nEndType, BOOL bValidGame = TRUE);
    void _calcRankDiff();
    int _getAddScore(int nDiffScore);  //获取差额增减分数
    void _setEndResult(int nSeat, int nCurResult, int nEnemyResult);
    void SetGameResult(int nSeat, int nResult, BOOL bValidGame);
    void _statisticDataLog(int resultType);

    //记录事件
    void RecordLayout();
    void RecordShowChess(ShowChessAck* pShow);
    void RecordMoveChess(int nType, int nResult, int nEvent, const vector<int>& vPath);
    void _recordCleanChess(int nSeat, int nReason);
    void RecordResult();

private:
    CTKJunQiPKGame* m_pJunQiPKGame = nullptr;
    std::shared_ptr<CTKJunQiCore> m_pJunQiCore;
    std::shared_ptr<CTKJunQiPKCfg> m_pJunQiPKCfg;
    vector<CTKJunQiPlayer*> m_vPlayers;
    map<int, _onTimeout> m_onTimeOut;

    int m_nPlayerCount = JUNQIPK_UNKNOWN;
    int m_nCurSeat = JUNQIPK_UNKNOWN;    //当前走棋玩家
    int m_nCurSelectPos = 0;             //当前选中的位置
    int m_nStage = ES_Distributing;      //游戏阶段
    int m_nNoFightSteps = 0;             //未交战步数
    int m_nStepsMoved = 0;               //步数
    int m_nPeaceSeat = JUNQIPK_UNKNOWN;  //和棋玩家座位号

    int m_nPeaceStatus = 0;  // 求和状态，保存游戏状态 0代表没人求和
                             // 之外的数字代表有人求和，保存相应ES状态（在断线时使用，用于继续比赛计时）

    time_t m_tCurRemainTime = 0;  // 上一手出牌时间或距离上一手出牌经过的时间

    bool m_bColorSet = true;  //玩家的棋子颜色是否确定
    set<int> m_curCanMovePos;

private:
    /*消极比赛相关*/
    void _calcNegativeState(int nSeat, bool isInitiative);
    void _sendNegativeAtomEvent();
};
