#include "StdAfx.h"
#include "TKJunQiPlayer.h"
#include "TKDllServer.h"
#include "TKJunQiPKGame.h"
#include "tkgame2pivotprotocol.h"

enum EComGrow
{
    EG_RankCount = 0,
    EG_SeasonRank = 1,
    EG_ZongJuShu = 2,
    EG_ShengJuShu = 3,
    EG_HeJuShu = 4,
    EG_FuJuShu = 5,
    EG_DuanXianJuShu = 6,
    EG_SaiJiShengJuShu = 7,
};

const int g_nGameCount = 4;
const int g_nGrowCount = 8;
const int g_nDayUseDouleCardLimit = 5;   //每日翻倍卡限用次数
const int g_nKeepScoreRankLimit = 4000;  //保分卡使用赛季积分限制

CCallback::CCallback()
{
    m_pFuncCallBack = NULL;
    memset(&m_sInvalid, 0, sizeof(SInvalid));
}

CCallback::CCallback(PFUNC_CALLBACK in_pfuncCallback, SInvalid& in_ReqGameSimpleAction)
{
    m_pFuncCallBack = in_pfuncCallback;
    m_sInvalid = in_ReqGameSimpleAction;
}

CCallback::CCallback(CCallback& in_funcObjCallback)
{
    *this = in_funcObjCallback;
}

CCallback::~CCallback()
{
    delete m_pFuncCallBack;
    m_pFuncCallBack = NULL;

    memset(&m_sInvalid, 0, sizeof(SInvalid));
}

BOOL CCallback::Execute(void* in_pAckMsg)
{
    if (m_pFuncCallBack == NULL)
    {
        return FALSE;
    }

    return m_pFuncCallBack->operator()((PTKHEADER)in_pAckMsg, (DWORD)&m_sInvalid);
}

CTKJunQiPlayer::CTKJunQiPlayer()
{
    p_nRankScoreArray = new int[g_RankScoreCfgRow];
    memset(p_nRankScoreArray, 0, sizeof(int) * g_RankScoreCfgRow);
}

CTKJunQiPlayer::~CTKJunQiPlayer()
{
    m_pJunQiPKCfg = NULL;
    if (NULL != p_nRankScoreArray)
    {
        delete[] p_nRankScoreArray;
        p_nRankScoreArray = NULL;
    }
}

int CTKJunQiPlayer::GetCurMoveTime()
{
    return m_nCurMoveTime;
}

void CTKJunQiPlayer::_resetData()
{
    m_nResult = JUNQIPK_UNKNOWN;
    m_bDisConnect = false;
    m_nPassCount = 0;
    m_nPeaceCount = 0;
    m_nLastColor = JUNQIPK_UNKNOWN;
    m_nLastRepeat = 0;
    m_nLastToPos = 0;
    m_nLastIndex = JUNQIPK_UNKNOWN;
    m_nLastCount = 0;
    m_nChaseIndex = JUNQIPK_UNKNOWN;
    m_nChaseCount = 0;
    m_nRankChange = 0;
    m_nUserWare = 0;
    m_nHeadImgId = 0;
    m_nBigAlly = 0;
    m_nAddScore = 0;
    m_nStartTime = 0;
    m_nCurMoveTime = 0;
    m_nDelayCount = 0;
    m_bShowJunQi = false;
    m_bInitChess = false;
    m_bChase = false;
    m_bPeace = false;

    m_szLayout = "";
    m_qMoveChess.clear();
    m_astGrowInfo.clear();
    m_vChasePos.clear();
    m_mTimeFeel.clear();

    memset(p_nRankScoreArray, 0, sizeof(int) * g_RankScoreCfgRow);

    m_nNegativeState = ENT_NOT_NEGATIVE;
    m_nTeamNegativeState = ENT_NOT_NEGATIVE;
    m_pGame = nullptr;
}

void CTKJunQiPlayer::InitPlayer(std::shared_ptr<CTKJunQiPKCfg> pCfg, CTKJunQiPKGame* pGame)
{
    _resetData();
    m_pJunQiPKCfg = pCfg;
    m_pGame = pGame;
    m_nCurMoveTime = m_pJunQiPKCfg->GetCfg(ECI_MoveTime);
    if (RANK_MATCH == m_pJunQiPKCfg->GetCfg(ECI_MatchType))
    {
        _getPivotGrow(_getGrowID(EG_RankCount));
        _getPivotGrow(_getGrowID(EG_SeasonRank));
        _getPivotGrow(EG_DoubleCard);
        _getPivotGrow(EG_KeepScoreCard);
        _getPivotGrow(EG_DayDoubleCardUseCount);
    }
}

bool CTKJunQiPlayer::CanMoveTo(int nAtPos, int nAtIndex, int nToPos, int nRepeatStep)
{
    string szMoveChess = (boost::format("%||-%||-%||") % nAtPos % nAtIndex % nToPos).str();
    m_qMoveChess.push_back(szMoveChess);
    int nCount = _getRepeatCount(nRepeatStep);
    m_qMoveChess.pop_back();
    return nRepeatStep > nCount;
}

bool CTKJunQiPlayer::CanChaseMoveTo(int nAtIndex, int nToIndex, int nToPos)
{
    if (!m_bChase)
    {
        return true;
    }
    if (nAtIndex != m_nLastIndex || nToIndex != m_nChaseIndex)
    {
        return true;
    }
    int nCount = m_nChaseCount + 1;
    if (m_pJunQiPKCfg->GetCfg(ECI_ChanseCount) > nCount)
    {
        return true;
    }
    auto iter = find(m_vChasePos.begin(), m_vChasePos.end(), nToPos);
    if (iter != m_vChasePos.end())
    {
        return false;
    }
    return !_isCircleChase();
}

bool CTKJunQiPlayer::CheckChaseMove(int& nTips)
{
    if (!m_bChase)
    {
        return false;
    }
    if (m_pJunQiPKCfg->GetCfg(ECI_ChanseCount) > m_nChaseCount)
    {
        return false;
    }
    if (!_isCircleChase())
    {
        return false;
    }
    nTips = ETS_RepeatMoveLoss;
    return true;
}

void CTKJunQiPlayer::AddMoveStep(int nAtPos, int nAtIndex, int nToPos, int nToIndex, int nChaseIndex)
{
    m_nLastToPos = nToPos;
    _procChase(nAtIndex, nChaseIndex);
    if (JUNQIPK_UNKNOWN != nToIndex)  //有吃子行为，清除移动步数
    {
        m_qMoveChess.clear();
        return;
    }

    string szMoveChess = (boost::format("%||-%||-%||") % nAtPos % nAtIndex % nToPos).str();
    int nRange = 2 * m_pJunQiPKCfg->GetCfg(ECI_RepeatStep) - 1;
    if (m_qMoveChess.size() >= nRange)
    {
        m_qMoveChess.pop_front();
    }
    m_qMoveChess.push_back(szMoveChess);
}

bool CTKJunQiPlayer::CheckPassCount(int nMaxCount, int& nTips)
{
    if (m_nPassCount >= nMaxCount)
    {
        nTips = ETS_TimeoutLoss;
        return true;
    }
    return false;
}

bool CTKJunQiPlayer::CheckRepeatStep(int nMaxRepeat, int& nTips)
{
    int nRepeatCount = _getRepeatCount(nMaxRepeat);
    if (nRepeatCount >= nMaxRepeat)
    {
        nTips = ETS_RepeatMoveLoss;
        return true;
    }
    return false;
}

bool CTKJunQiPlayer::CheckRepeat(int nMaxRepeat, int& nRepeatCount)
{
    bool bReport = false;
    for (int i = nMaxRepeat - 1; i > 1; --i)
    {
        nRepeatCount = _getRepeatCount(i);
        if (nRepeatCount >= i)
        {
            if (nRepeatCount > m_nLastRepeat)
            {
                bReport = true;
            }
            break;
        }
    }
    m_nLastRepeat = nRepeatCount;
    return bReport;
}

void CTKJunQiPlayer::SaveGrowValue(time_t nTime)
{
    if (Int_True == m_pJunQiPKCfg->GetCfg(ECI_FriendTable))
    {
        return;
    }

    int dwPerCount = m_pJunQiPKCfg->GetCfg(ECT_PerMatchCount);
    if (0 != dwPerCount)
    {
        AddGrowValue(dwPerCount);
    }

    int dwPerTime = m_pJunQiPKCfg->GetCfg(ECT_PerMatchTime);
    if (0 != dwPerTime)
    {
        AddGrowValue(dwPerTime, nTime);
    }

    _addExperience();
    _saveGameGrow();
    _saveMatchGrow();
    _saveDB();
    AddAtomGameEvent(EVT_GameResult, m_nResult, TRUE);
}

DWORD CTKJunQiPlayer::_getGrowID(int nIndex)
{
    static DWORD dwGrowIds[g_nGameCount][g_nGrowCount] = {
        {
            EG_FFQRankCount,
            EG_FFQSeasonRank,
            EG_FFQZongJuShu,
            EG_FFQShengJuShu,
            EG_FFQHeJuShu,
            EG_FFQFuJuShu,
            EG_FFQDuanXianJuShu,
            EG_FFQSaiJiShengJuShu,
        },
        {EG_AQRankCount, EG_AQSeasonRank, EG_AQZongJuShu, EG_AQShengJuShu, EG_AQHeJuShu, EG_AQFuJuShu,
         EG_AQDuanXianJuShu, EG_AQSaiJiShengJuShu},
        {EG_SARankCount, EG_SASeasonRank, EG_SAZongJuShu, EG_SAShengJuShu, EG_SAHeJuShu, EG_SAFuJuShu,
         EG_SADuanXianJuShu, EG_SASaiJiShengJuShu},
        {EG_SMRankCount, EG_SMSeasonRank, EG_SMZongJuShu, EG_SMShengJuShu, EG_SMHeJuShu, EG_SMFuJuShu,
         EG_SMDuanXianJuShu, EG_SMSaiJiShengJuShu}};

    int nGameIndex = m_pJunQiPKCfg->GetCfg(ECI_GameType) - 1;
    return dwGrowIds[nGameIndex][nIndex];
}

void CTKJunQiPlayer::_saveMatchGrow()
{
    int nMacthType = m_pJunQiPKCfg->GetCfg(ECI_MatchType);
    if (RANK_MATCH == nMacthType)
    {
        int nChangeValue = _getRankChange();
        AddGrowValue(_getGrowID(EG_SeasonRank), nChangeValue);
        AddGrowValue(_getGrowID(EG_RankCount));
    }
    else if (PK_MATCH == nMacthType)
    {
        AddGrowValue(EG_PlayPKGame);
    }
}

void CTKJunQiPlayer::_saveDB()
{
    if (m_astGrowInfo.empty())
    {
        return;
    }
    int cnSaveGrowInfo = 0;
    CTKBuffer buffer;
    buffer.SetHeaderLen(sizeof(TKREQSAVEUSERGROWVALUE));

    for (auto iter = m_astGrowInfo.begin(); iter != m_astGrowInfo.end(); ++iter)
    {
        if (iter->second < 0)
        {
            _saveNegGrow(iter->first, iter->second);
            continue;
        }
        cnSaveGrowInfo++;
        DWORD dwGrowID = iter->first;
        DWORD dwValue = (DWORD)iter->second;
        if (!buffer.Append(&dwGrowID, sizeof(DWORD)) || !buffer.Append(&dwValue, sizeof(DWORD)))
        {
            return;
        }
    }
    m_astGrowInfo.clear();
    if (cnSaveGrowInfo <= 0)
    {
        return;
    }

    PTKREQSAVEUSERGROWVALUE pSend = (PTKREQSAVEUSERGROWVALUE)buffer.GetBufPtr();
    pSend->header.dwType = TK_REQ | TKID_GS2MS_SAVEUSERGROWVALUE;
    pSend->header.dwLength = buffer.GetBufLen() - sizeof(TKHEADER);
    pSend->dwTourneyID = m_dwTourneyID;
    pSend->dwMatchID = m_dwMatchID;
    pSend->dwUserID = m_dwUserID;
    pSend->cnGameGrowInfo = cnSaveGrowInfo;

    tk_pDllServer->PostMsg2MS((PTKHEADER)pSend, TK_GS2MS_MSGTYPE_PLAYER, m_dwMsIP, m_wMsPort, m_dwUserID);
}

void CTKJunQiPlayer::_saveNegGrow(DWORD dwGrowID, int nValue)
{
    msg_gs2pivot_req_deal_grow rReqGrow;
    memset(&rReqGrow, 0, sizeof(rReqGrow));
    rReqGrow.dwType = TKID_GS2PIVOT_DEAL_GROW | TK_REQ;
    rReqGrow.dwLength = sizeof(rReqGrow) - sizeof(TKHEADER);
    rReqGrow.dwUserID = m_dwUserID;
    rReqGrow.dwGrowID = dwGrowID;
    rReqGrow.nValue = nValue;
    rReqGrow.dwMPID = m_nProductID;
    rReqGrow.dwGameID = __super::m_nGameID;
    rReqGrow.dwPlatType = m_wCntTID;
    rReqGrow.byOSType = m_byOSType;
    rReqGrow.dwAppID = m_dwAppID;
    rReqGrow.dwSiteID = m_dwSiteID;

    tk_pDllServer->PostMsg2Pivot_GSBase(&rReqGrow, m_dwUserID, m_dwMatchID, m_pGame->m_wStageID, m_pGame->m_wRoundID,
                                        nullptr);
}

void CTKJunQiPlayer::SetEndResult(int nStep, int nResult)
{
    m_nResult = nResult;
    AddGrowValue(EG_DayPlayCount);

    if (EFR_Win == m_nResult)
    {
        AddGrowValue(EG_DayWinCount);
        if (nStep <= ONE_HUNDRED)
        {
            AddGrowValue(EG_HundredStepWin);
        }
    }
}

void CTKJunQiPlayer::AddGrowValue(DWORD nGrowId, int nValue)
{
    if (Int_True != m_pJunQiPKCfg->GetCfg(ECI_FriendTable) && nValue != 0)
    {
        m_astGrowInfo[nGrowId] += nValue;

        if (!__super::AddGrowValue(nGrowId, nValue, FALSE))
        {
            LOG_ERROR("failed,uid=%d,nGrowId=%d,value=%d", m_dwUserID, nGrowId, nValue);
        }
    }
}

void CTKJunQiPlayer::SetStartTime()
{
    m_nStartTime = time(NULL);
    m_bChase = false;
}

bool CTKJunQiPlayer::SetMoveFinish(bool bTimeOut)
{
    int moveTime = m_pJunQiPKCfg->GetCfg(ECI_MoveTime);
    int reduceTime = m_pJunQiPKCfg->GetCfg(ECI_DelayReduceTime);
    int bottomTime = m_pJunQiPKCfg->GetCfg(ECI_bottomTime);
    int oldMoveTime = m_nCurMoveTime;

    if (bTimeOut)
    {
        ++m_nDelayCount;
        moveTime = moveTime - (m_nDelayCount - 1) * reduceTime;
        m_nCurMoveTime = moveTime > bottomTime ? moveTime : bottomTime;
        if (m_nCurMoveTime == oldMoveTime)
        {
            return false;
        }

        if (m_nPassCount <= 0)
        {
            return false;
        }

        return true;
    }

    time_t nNow = time(NULL);
    int nCheckTime = nNow - m_nStartTime + DELAY_TIPS_TIME;
    if (nCheckTime > m_nCurMoveTime)
    {
        ++m_nDelayCount;
        moveTime = moveTime - (m_nDelayCount - 1) * reduceTime;
        m_nCurMoveTime = moveTime > bottomTime ? moveTime : bottomTime;

        if (m_nCurMoveTime != oldMoveTime)
        {
            return true;
        }
    }

    return false;
}

bool CTKJunQiPlayer::CheckFeel(UINT nTo)
{
    time_t tLast = m_mTimeFeel[nTo];
    time_t tNow = time(NULL);
    if ((tNow - tLast) < m_pJunQiPKCfg->GetCfg(ECI_FeelColdTime))
    {
        return false;
    }
    m_mTimeFeel[nTo] = tNow;
    return true;
}

void CTKJunQiPlayer::WriteLog(LPCTSTR pszFmt, ...)
{
    CTKBuffer bufLog;
    va_list args;
    va_start(args, pszFmt);
    BOOL bRet = bufLog.AppendFormatStringV(pszFmt, args);
    va_end(args);

    ostringstream ss;
    ss << "userid=" << m_dwUserID << ",name=" << m_szNickName << ",seat=" << m_nSeatOrder;
    TKWriteLog("%s; %s", bufLog.GetBufPtr(), ss.str().c_str());
}

bool CTKJunQiPlayer::IsLoss(int& nTips)
{
    if (CheckPassCount(m_pJunQiPKCfg->GetCfg(ECI_PassCount), nTips))
    {
        return true;
    }

    if (CheckRepeatStep(m_pJunQiPKCfg->GetCfg(ECI_RepeatStep), nTips))
    {
        return true;
    }
    if (CheckChaseMove(nTips))
    {
        return true;
    }
    return false;
}

BOOL CTKJunQiPlayer::OnPivotMsg(PTKHEADER pMsg, DWORD in_dwContext)
{
    if (pMsg->dwParam != TK_ACKRESULT_SUCCESS)
    {
        return FALSE;
    }
    if ((TKID_GS2PIVOT_GET_SINGLEGROW | TK_ACK) != pMsg->dwType)
    {
        return FALSE;
    }

    msg_pivot2gs_ack_get_singleGrow* pAckMsg = (msg_pivot2gs_ack_get_singleGrow*)(pMsg);

    if (pMsg->dwLength + TKHEADERSIZE < pAckMsg->stSuffixIdx.dwSuffixOffSet + sizeof(TKMATCHSTRANGROW))
    {
        return FALSE;
    }

    PTKMATCHSTRANGROW pGrow = (PTKMATCHSTRANGROW)((BYTE*)pAckMsg + pAckMsg->stSuffixIdx.dwSuffixOffSet);
    SetGrowValue(pGrow->dwGrowID, pGrow->dwValue, FALSE);

    return FALSE;
}

void CTKJunQiPlayer::SetRankDiff(int nBigAlly, int nAddScore)
{
    m_nBigAlly = nBigAlly;
    m_nAddScore = nAddScore;
}

int CTKJunQiPlayer::GetRankValue()
{
    return _getGrowValue(EG_SeasonRank);
}

bool CTKJunQiPlayer::CanPeace(int nMaxPeace)
{
    if (m_nPeaceCount >= nMaxPeace)
    {
        return false;
    }
    ++m_nPeaceCount;
    return true;
}

void CTKJunQiPlayer::_addExperience()
{
    /*
        caosi 2021.02.23
        JunqiPK定义了多个积分域 5056, 5058, 5055等
        其中二人模式比赛配置的域是 5056, 四人模式的比赛配置的是 5058
        这样就导致了一个问题, 四人模式比赛中无法跨域使用 50560001 来记录总经验值
        因此在5058域中重新定义了原来预留给"极速翻翻棋"玩法的积分50580117, 用来来记录四人模式的经验值,
        同时关联修改50560001, 从而达到跨域记录总经验值的目的
    */

    int nExpGrowId = 0;
    int nValue = 0;

    // 确定使用的经验积分ID
    switch (m_pJunQiPKCfg->GetCfg(ECI_GameType))
    {
    case GT_SIGUOSIAN:   //四国四棋
    case GT_SHUANGMING:  //四国双明
        nExpGrowId = EG_S_Experience;
        break;
    default:
        nExpGrowId = EG_Experience;
        break;
    }

    // 给予的经验值
    if (EFR_Win == m_nResult)
    {
        nValue = m_pJunQiPKCfg->GetCfg(ECI_WinExp);
    }
    else if (EFR_PerishTogether == m_nResult)
    {
        nValue = m_pJunQiPKCfg->GetCfg(ECI_DrawExp);
    }
    else if (EFR_Loss == m_nResult)
    {
        nValue = m_pJunQiPKCfg->GetCfg(ECI_LoseExp);
    }

    if (nExpGrowId > 0 && nValue > 0)
    {
        AddGrowValue(nExpGrowId, nValue);
    }
    else
    {
        LOG_ERROR("expGrowId=%d, value=%d", nExpGrowId, nValue);
    }
}

void CTKJunQiPlayer::_saveGameGrow()
{
    AddGrowValue(_getGrowID(EG_ZongJuShu));
    if (EFR_Win == m_nResult)
    {
        AddGrowValue(_getGrowID(EG_ShengJuShu));
        // 2022/1/6号添加 赛季排位赛胜局数数据
        int nMacthType = m_pJunQiPKCfg->GetCfg(ECI_MatchType);
        if (RANK_MATCH == nMacthType)
        {
            AddGrowValue(_getGrowID(EG_SaiJiShengJuShu));
        }
    }
    else if (EFR_Loss == m_nResult)
    {
        AddGrowValue(_getGrowID(EG_FuJuShu));
    }
    else
    {
        AddGrowValue(_getGrowID(EG_HeJuShu));
    }

    if (m_bDisConnect)
    {
        AddGrowValue(_getGrowID(EG_DuanXianJuShu));
    }
}

void CTKJunQiPlayer::_getPivotGrow(DWORD nGrowId)
{
    msg_gs2pivot_req_get_singleGrow rReqGrow;
    memset(&rReqGrow, 0, sizeof(rReqGrow));
    rReqGrow.dwType = TKID_GS2PIVOT_GET_SINGLEGROW | TK_REQ;
    rReqGrow.dwLength = sizeof(rReqGrow) - sizeof(TKHEADER);
    rReqGrow.dwUserID = m_dwUserID;
    rReqGrow.dwGrowID = nGrowId;
    rReqGrow.dwMPID = m_nProductID;
    rReqGrow.dwGameID = 1076;
    rReqGrow.dwPlatType = m_wCntTID;
    rReqGrow.byOSType = m_byOSType;
    rReqGrow.dwAppID = m_dwAppID;
    rReqGrow.dwSiteID = m_dwSiteID;

    PFUNC_CALLBACK pFuncCallback = tkclsfunctor::bind_ex(&CTKJunQiPlayer::OnPivotMsg, (CTKJunQiPlayer*)this);

    SInvalid sInvalid;
    CCallback* pCallBack = new CCallback(pFuncCallback, sInvalid);
    tk_pDllServer->PostMsg2Pivot_GSBase(&rReqGrow, m_dwUserID, m_dwMatchID, m_pGame->m_wStageID, m_pGame->m_wRoundID,
                                        pCallBack);
}

int CTKJunQiPlayer::_getRankChange()
{
    m_nRankChange = 0;
    int nRankCount = _getGrowValue(EG_RankCount);
    if (EFR_Win == m_nResult)
    {
        m_nRankChange = _getWinChange(nRankCount);
    }
    else if (EFR_Loss == m_nResult)
    {
        //军棋7期需要积分修正
        m_nRankChange = _getLossChange(nRankCount);
        if (m_nRankChange > 0)
        {
            m_nRankChange = 0;
        }
    }
    else
    {
        m_nRankChange = _getDrawChange(nRankCount);
    }

    _calcNegRankChange();
    _calcUseWare();
    return m_nRankChange;
}

//四国军棋需要单独计算消极比赛积分增加
void CTKJunQiPlayer::_calcNegRankChange()
{
    int nGameType = m_pJunQiPKCfg->GetCfg(ECI_GameType);
    if (GT_SIGUOSIAN != nGameType && GT_SHUANGMING != nGameType)
    {
        return;
    }

    int nDoubleRank = m_nRankChange * 2;
    if (ENT_NOT_NEGATIVE != m_nNegativeState)  //自己消极比赛
    {
        m_nRankChange = m_nRankChange < 0 ? nDoubleRank : 0;
    }
    else if (ENT_NOT_NEGATIVE != m_nTeamNegativeState)  //队友消极比赛
    {
        m_nRankChange = m_nRankChange < 0 ? 0 : nDoubleRank;
    }
}

void CTKJunQiPlayer::_calcUseWare()
{
    if (m_nRankChange > 0 && EFR_Win == m_nResult)
    {
        int nDoubleCard = 0;
        GetGrowValue(EG_DoubleCard, nDoubleCard);
        int nUseCount = 0;
        GetGrowValue(EG_DayDoubleCardUseCount, nUseCount);
        if (nDoubleCard > 0 && nUseCount < g_nDayUseDouleCardLimit)
        {
            m_nRankChange *= 2;
            m_nUserWare = EG_DoubleCard;
            AddGrowValue(EG_DoubleCard, -1);
            AddGrowValue(EG_DayDoubleCardUseCount);
        }
        return;
    }

    if (m_nRankChange < 0)
    {
        int nKeepScoreCard = 0;
        GetGrowValue(EG_KeepScoreCard, nKeepScoreCard);
        if (nKeepScoreCard > 0 && GetRankValue() < g_nKeepScoreRankLimit)
        {
            m_nRankChange = 0;
            m_nUserWare = EG_KeepScoreCard;
            AddGrowValue(EG_KeepScoreCard, -1);
        }
    }
}

int CTKJunQiPlayer::_getGrowValue(int nIndex)
{
    int nGrowValue = 0;
    DWORD dwGrowID = _getGrowID(nIndex);
    GetGrowValue(dwGrowID, nGrowValue);
    return nGrowValue;
}

void CTKJunQiPlayer::_procChase(int nAtIndex, int nChaseIndex)
{
    if (GT_FANFANQI != m_pJunQiPKCfg->GetCfg(ECI_GameType))
    {
        return;
    }
    if (m_bChase)
    {
        if (nAtIndex == m_nLastIndex && nChaseIndex == m_nChaseIndex)
        {
            m_vChasePos.push_back(m_nLastToPos);
            ++m_nChaseCount;
        }
        else
        {
            m_nLastIndex = nAtIndex;
            m_nChaseIndex = nChaseIndex;
            m_nChaseCount = 1;
            m_vChasePos.clear();
            m_vChasePos.push_back(m_nLastToPos);
        }
    }
    else
    {
        m_nLastIndex = nAtIndex;
        m_nChaseIndex = JUNQIPK_UNKNOWN;
        m_nChaseCount = 0;
        m_vChasePos.clear();
    }
}

bool CTKJunQiPlayer::_isCircleChase()
{
    map<int, int> mapCount;
    for (auto iter = m_vChasePos.rbegin(); iter != m_vChasePos.rend(); ++iter)
    {
        mapCount[*iter] += 1;
        if (mapCount[*iter] > 1)
        {
            return true;
        }
    }
    return false;
}

int CTKJunQiPlayer::_getRepeatCount(int nRepeatCount)
{
    map<string, int> mStepCount;
    int nRange = nRepeatCount * 2 - 1;
    for (auto iter = m_qMoveChess.rbegin(); iter != m_qMoveChess.rend() && nRange > 0; ++iter)
    {
        ++mStepCount[*iter];
        --nRange;  //统计步数-1
    }
    int iCount = 0;
    for (auto iter = mStepCount.begin(); iter != mStepCount.end(); ++iter)
    {
        iCount = iCount > iter->second ? iCount : iter->second;
    }
    return iCount;
}

void CTKJunQiPlayer::_refreshRankScoreCfg(int nRankScore)
{
    memset(p_nRankScoreArray, 0, sizeof(int) * g_RankScoreCfgRow);
    int nRankCfgArray[g_RankScoreCfgCol][g_RankScoreCfgRow];
    memset(nRankCfgArray, 0, sizeof(int) * g_RankScoreCfgCol * g_RankScoreCfgRow);
    if (m_pJunQiPKCfg->GetCfg(ECI_GameType) == GT_FANFANQI || m_pJunQiPKCfg->GetCfg(ECI_GameType) == GT_ERRENANQI)
    {
        memcpy(nRankCfgArray, g_RankScoreArray, sizeof(int) * g_RankScoreCfgCol * g_RankScoreCfgRow);
    }
    else
    {
        memcpy(nRankCfgArray, g_SiGuoRankScoreArray, sizeof(int) * g_RankScoreCfgCol * g_RankScoreCfgRow);
    }

    if (nRankScore < nRankCfgArray[0][0])
    {
        memcpy(p_nRankScoreArray, nRankCfgArray[0], sizeof(int) * g_RankScoreCfgRow);
    }
    else if (nRankScore >= nRankCfgArray[g_RankScoreCfgCol - 1][0])
    {
        memcpy(p_nRankScoreArray, nRankCfgArray[g_RankScoreCfgCol - 1], sizeof(int) * g_RankScoreCfgRow);
    }
    else
    {
        for (int i = 0; i < g_RankScoreCfgCol - 1; i++)
        {
            if (nRankScore >= nRankCfgArray[i][0] && nRankScore < nRankCfgArray[i + 1][0])
            {
                memcpy(p_nRankScoreArray, nRankCfgArray[i], sizeof(int) * g_RankScoreCfgRow);
                break;
            }
        }
    }
}

int CTKJunQiPlayer::_getWinChange(int nGameCount)
{
    _refreshRankScoreCfg(GetRankValue());
    int nScore = nGameCount < g_GradingCount ? g_GradingWin : p_nRankScoreArray[1];
    return _calcDiffScore(nScore);
}

int CTKJunQiPlayer::_getDrawChange(int nGameCount)
{
    _refreshRankScoreCfg(GetRankValue());
    int nScore = nGameCount < g_GradingCount ? g_GradingDraw : p_nRankScoreArray[3];
    return _calcDiffScore(nScore);
}

int CTKJunQiPlayer::_getLossChange(int nGameCount)
{
    if (nGameCount < g_GradingCount)
    {
        int nScore = g_GradingLoss;
        return _calcDiffScore(nScore);
    }

    int nGrowValue = GetRankValue();
    int nScore = _getLossScore(nGrowValue);
    return nScore;
}

int CTKJunQiPlayer::_getLossScore(int nGrowValue)
{
    _refreshRankScoreCfg(GetRankValue());
    int nScore = p_nRankScoreArray[2];

    return _calcDiffScore(nScore);
}

int CTKJunQiPlayer::_calcDiffScore(int nScore)
{
    if (m_nBigAlly == m_nSeatOrder % 2)
    {
        nScore -= m_nAddScore;
    }
    else
    {
        nScore += m_nAddScore;
    }
    return nScore;
}
