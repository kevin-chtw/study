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
    bool CheckRepeat(int nMaxRepeat, int& nRepeatCount);  //检查重复，增加时上报，不同的重复时上报
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
    int _getGrowValue(int nIndex);  //获取积分

    DWORD _getGrowID(int nIndex);
    void _saveMatchGrow();
    void _saveDB();
    void _saveNegGrow(DWORD dwGrowID, int nValue);
    void _procChase(int nAtIndex, int nChaseIndex);
    bool _isCircleChase();                  //循环追子
    int _getRepeatCount(int nRepeatCount);  //计算重复行棋步数
    void _refreshRankScoreCfg(int nRankScore);
    int _getWinChange(int nGameCount);   //排位赛胜利得分
    int _getDrawChange(int nGameCount);  //排位赛平局得分
    int _getLossChange(int nGameCount);  //排位赛失败得分
    int _getLossScore(int nGrowValue);
    int _calcDiffScore(int nScore);

protected:
    int m_nResult = JUNQIPK_UNKNOWN;  //结果
    bool m_bDisConnect = false;       //是否断线

private:
    std::shared_ptr<CTKJunQiPKCfg> m_pJunQiPKCfg;

    int m_nPassCount = 0;                 //过次数
    int m_nPeaceCount = 0;                //求和次数
    int m_nLastColor = JUNQIPK_UNKNOWN;   //上次选中的颜色
    int m_nLastRepeat = 0;                //上次检测重复次数
    int m_nLastToPos = 0;                 //最后移到到的位置
    int m_nLastIndex = JUNQIPK_UNKNOWN;   //最后移动的棋子
    int m_nLastCount = 0;                 //移动相同棋子的次数
    int m_nChaseIndex = JUNQIPK_UNKNOWN;  //被追的棋子
    int m_nChaseCount = 0;                //被追次数
    int m_nRankChange = 0;                //排位赛积分变化
    int m_nUserWare = 0;                  //使用的道具

    int m_nHeadImgId = 0;  //头像ID
    int m_nBigAlly = 0;    //分数较大的联盟
    int m_nAddScore = 0;   //额外增加分

    time_t m_nStartTime = 0;  //每步开始行棋时间
    int m_nCurMoveTime = 0;   //当前行动时长
    int m_nDelayCount = 0;    //延迟次数

    bool m_bShowJunQi = false;  //军棋是否亮了
    bool m_bInitChess = false;  //初始化是否完成
    bool m_bChase = false;      //是否在追
    bool m_bPeace = false;      //是否同意和棋

    string m_szLayout = "";
    deque<string> m_qMoveChess;  //当玩家在8步之内有完全重复的走子操作时，会记为一次重复走子。
    map<DWORD, int> m_astGrowInfo;
    vector<int> m_vChasePos;
    map<UINT, time_t> m_mTimeFeel;

    int* p_nRankScoreArray = nullptr;  //排位赛得分配置

private:
    int m_nNegativeState = ENT_NOT_NEGATIVE;      //消极比赛状态
    int m_nTeamNegativeState = ENT_NOT_NEGATIVE;  //队伍消极比赛状态
    CTKJunQiPKGame* m_pGame = nullptr;
};
