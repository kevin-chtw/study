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
    //��ʼ��
    void OnGameBegin(CTKJunQiPKGame* pGame, const string& szCfg);
    //��Ϣ
    bool OnPlayerMsg(int seatOrder, const JunQiPKReqMsg& reqMsg);
    //��ʱ
    void OnTimeOut(DWORD dwID, ULONG_PTR ulUserData);

    void OnPlayerNetBreak(int nSeat);
    void OnPlayerNetResume(int nSeat);

private:
    //��ʼ��
    void InitTimeout();
    void _initPlayers();
    CTKJunQiPlayer* _getPlayer(int nSeat);

    //��Ϣ
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

    //��ʱ
    void OnSetRuleTimeOut(ULONG_PTR ulUserData);
    void OnInitTimeOut(ULONG_PTR ulUserData);
    void OnMoveChessTimeout(ULONG_PTR ulUserData);
    void OnActionTimeout(ULONG_PTR ulUserData);
    void OnPeaceTimeout(ULONG_PTR ulUserData);
    void OnRoundTimeout(ULONG_PTR ulUserData);

    //������Ϣ
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

    //�¼�
    bool IsAllReady();
    void _onAllReady();
    void MoveChess(int nSeat, const MoveChessReq& rReq, vector<int>& path);
    bool IsNeedConfirm(int nSeat, const MoveChessReq& rReq, int nChessIndex);
    bool CheckPass(int seat, const MoveChessReq& rReq);
    void SendTimeOutAck(int nSeat);
    void Fight(std::shared_ptr<SChess> pFrom, std::shared_ptr<SChess> pTo, int& nResult, int& nEvent);
    void ChessDead(int nKillSeat, int nChessType, int& nEvent);
    void SetWaitAction();                          //���õȴ�ִ�ж���
    void GetSLDeadSeat(vector<int>& vSLDeadSeat);  //��ȡ�ո�����˾��������λ��
    void _getOverSeat(vector<int>& vOverSeat);
    bool _isLoss(int nSeat, int& nTips);
    void CheckColorSet(int nSeat, int nColor, ShowChessAck* pAck);
    void ConfirmColor(int nSeat, int nColor, ShowChessAck* pAck);
    void _creatJunQiCore();
    void _getAllyLayout(int nSeat, ostringstream& ss);
    int _getNextSeat(int nSeat);
    void _procAgreePeace(int seat);

    //��������
    bool IsGameOver();
    bool _isAllyWin(int nAlly);

    bool IsPeace(int& nTips, int& nEndType);
    void FinishGame(int nSeat, int nCurResult, int nEnemyResult, int nEndType, BOOL bValidGame = TRUE);
    void _calcRankDiff();
    int _getAddScore(int nDiffScore);  //��ȡ�����������
    void _setEndResult(int nSeat, int nCurResult, int nEnemyResult);
    void SetGameResult(int nSeat, int nResult, BOOL bValidGame);
    void _statisticDataLog(int resultType);

    //��¼�¼�
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
    int m_nCurSeat = JUNQIPK_UNKNOWN;    //��ǰ�������
    int m_nCurSelectPos = 0;             //��ǰѡ�е�λ��
    int m_nStage = ES_Distributing;      //��Ϸ�׶�
    int m_nNoFightSteps = 0;             //δ��ս����
    int m_nStepsMoved = 0;               //����
    int m_nPeaceSeat = JUNQIPK_UNKNOWN;  //���������λ��

    int m_nPeaceStatus = 0;  // ���״̬��������Ϸ״̬ 0����û�����
                             // ֮������ִ���������ͣ�������ӦES״̬���ڶ���ʱʹ�ã����ڼ���������ʱ��

    time_t m_tCurRemainTime = 0;  // ��һ�ֳ���ʱ��������һ�ֳ��ƾ�����ʱ��

    bool m_bColorSet = true;  //��ҵ�������ɫ�Ƿ�ȷ��
    set<int> m_curCanMovePos;

private:
    /*�����������*/
    void _calcNegativeState(int nSeat, bool isInitiative);
    void _sendNegativeAtomEvent();
};
