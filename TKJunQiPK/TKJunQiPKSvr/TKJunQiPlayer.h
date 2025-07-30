#pragma once
#include "TKJunQiPK.pb.h"
#include "TKJunQiPKCfg.h"

using namespace cn::jj::service::msg::protocol;

struct SInvalid
{
};

class CCallback : public CFuncObjCallback
{
public:
    CCallback();
    CCallback(PFUNC_CALLBACK in_pfuncCallback, SInvalid& in_ReqGameSimpleAction);
    CCallback(CCallback& in_funcObjCallback);
    ~CCallback();

public:
    virtual BOOL Execute(void* in_pAckMsg);

private:
    PFUNC_CALLBACK m_pFuncCallBack;
    SInvalid m_sInvalid;
};

class CTKJunQiPKGame;

class CTKJunQiPlayer : public CTKGamePlayer
{
    friend class CTKJunQiPKData;

public:
    CTKJunQiPlayer();
    ~CTKJunQiPlayer();

    void InitPlayer(std::shared_ptr<CTKJunQiPKCfg> pCfg, CTKJunQiPKGame* pGame);
    bool CanMoveTo(int nAtPos, int nAtIndex, int nToPos, int nRepeatStep);
    bool CanChaseMoveTo(int nAtIndex, int nToIndex, int nToPos);
    bool CheckChaseMove(int& nTips);
    void AddMoveStep(int nAtPos, int nAtIndex, int nToPos, int nToIndex, int nChaseIndex);
    bool CheckPassCount(int nMaxCount, int& nTips);
    bool CheckRepeatStep(int nMaxRepeat, int& nTips);
    bool CanPeace(int nMaxPeace);
    bool CheckRepeat(int nMaxRepeat, int& nRepeatCount);  //����ظ�������ʱ�ϱ�����ͬ���ظ�ʱ�ϱ�
    void SaveGrowValue(time_t nTime);
    void SetEndResult(int nStep, int nResult);
    void AddGrowValue(DWORD nGrowId, int nValue = 1);
    void SetStartTime();
    bool SetMoveFinish(bool bTimeOut);
    bool CheckFeel(UINT nTo);
    void WriteLog(LPCTSTR pszFmt, ...);
    bool IsLoss(int& nTips);
    BOOL OnPivotMsg(PTKHEADER pMsg, DWORD in_dwContext);
    void SetRankDiff(int nBigAlly, int nAddScore);
    int GetRankValue();
    int GetLastColor()
    {
        return m_nLastColor;
    }
    int GetCurMoveTime();

private:
    void _resetData();
    void _addExperience();
    void _saveGameGrow();
    void _getPivotGrow(DWORD nGrowId);
    int _getRankChange();
    void _calcNegRankChange();
    void _calcUseWare();
    int _getGrowValue(int nIndex);  //��ȡ����

    DWORD _getGrowID(int nIndex);
    void _saveMatchGrow();
    void _saveDB();
    void _saveNegGrow(DWORD dwGrowID, int nValue);
    void _procChase(int nAtIndex, int nChaseIndex);
    bool _isCircleChase();                  //ѭ��׷��
    int _getRepeatCount(int nRepeatCount);  //�����ظ����岽��
    void _refreshRankScoreCfg(int nRankScore);
    int _getWinChange(int nGameCount);   //��λ��ʤ���÷�
    int _getDrawChange(int nGameCount);  //��λ��ƽ�ֵ÷�
    int _getLossChange(int nGameCount);  //��λ��ʧ�ܵ÷�
    int _getLossScore(int nGrowValue);
    int _calcDiffScore(int nScore);

protected:
    int m_nResult = JUNQIPK_UNKNOWN;  //���
    bool m_bDisConnect = false;       //�Ƿ����

private:
    std::shared_ptr<CTKJunQiPKCfg> m_pJunQiPKCfg;

    int m_nPassCount = 0;                 //������
    int m_nPeaceCount = 0;                //��ʹ���
    int m_nLastColor = JUNQIPK_UNKNOWN;   //�ϴ�ѡ�е���ɫ
    int m_nLastRepeat = 0;                //�ϴμ���ظ�����
    int m_nLastToPos = 0;                 //����Ƶ�����λ��
    int m_nLastIndex = JUNQIPK_UNKNOWN;   //����ƶ�������
    int m_nLastCount = 0;                 //�ƶ���ͬ���ӵĴ���
    int m_nChaseIndex = JUNQIPK_UNKNOWN;  //��׷������
    int m_nChaseCount = 0;                //��׷����
    int m_nRankChange = 0;                //��λ�����ֱ仯
    int m_nUserWare = 0;                  //ʹ�õĵ���

    int m_nHeadImgId = 0;  //ͷ��ID
    int m_nBigAlly = 0;    //�����ϴ������
    int m_nAddScore = 0;   //�������ӷ�

    time_t m_nStartTime = 0;  //ÿ����ʼ����ʱ��
    int m_nCurMoveTime = 0;   //��ǰ�ж�ʱ��
    int m_nDelayCount = 0;    //�ӳٴ���

    bool m_bShowJunQi = false;  //�����Ƿ�����
    bool m_bInitChess = false;  //��ʼ���Ƿ����
    bool m_bChase = false;      //�Ƿ���׷
    bool m_bPeace = false;      //�Ƿ�ͬ�����

    string m_szLayout = "";
    deque<string> m_qMoveChess;  //�������8��֮������ȫ�ظ������Ӳ���ʱ�����Ϊһ���ظ����ӡ�
    map<DWORD, int> m_astGrowInfo;
    vector<int> m_vChasePos;
    map<UINT, time_t> m_mTimeFeel;

    int* p_nRankScoreArray = nullptr;  //��λ���÷�����

private:
    int m_nNegativeState = ENT_NOT_NEGATIVE;      //��������״̬
    int m_nTeamNegativeState = ENT_NOT_NEGATIVE;  //������������״̬
    CTKJunQiPKGame* m_pGame = nullptr;
};
