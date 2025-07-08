#include "MJPlayer.h"
#include "MJGame.h"
#include "MJException.h"
#include "MJString.h"
#include "MJGrowSystem.h"
#include "TKMatchRuler.h"
#include "tkgame2pivotprotocol.h"

void MJPlayer::Initialize(ISeatID seat)
{
    m_Seat = seat;

    auto gamePlayer = m_Game->GetGamePlayer(m_Seat);
    //统一开局扣税，不同步到前端
    PayTax();
    m_Score = gamePlayer->m_nScore;
    m_UserID = gamePlayer->m_dwUserID;
    m_IsBot = gamePlayer->IsBotUser();

    m_Game->GetGrowSystem()->LoadExp(m_Seat);
}

ISeatID MJPlayer::GetSeat() const
{
    return m_Seat;
}

int64_t MJPlayer::GetScore() const
{
    return m_Score + m_Tax;
}

uint32_t MJPlayer::GetUserID() const
{
    return m_UserID;
}

void MJPlayer::SetTrusted(bool trusted)
{
    m_IsTrusted = trusted;
}

bool MJPlayer::IsTrusted() const
{
    return m_IsTrusted;
}

void MJPlayer::SetOffline(bool offline)
{
    m_IsOffline = offline;
}

bool MJPlayer::IsOffline()
{
    return m_IsOffline;
}

void MJPlayer::SetOut(MJPlayerStatus::Type status, EHuResultMode huMode)
{
    m_Status = status;
    m_HuMode = huMode;
    m_IsOut = true;
}

bool MJPlayer::IsOut()
{
    return m_IsOut;
}

void MJPlayer::PayTax()
{
    int64_t score = 0;
    m_Game->SetGameResult(m_Seat, 0, 0, 0, out__ score, FALSE, TRUE, TK_UNKNOWN);
    m_Tax += -score;
}

void MJPlayer::RefundTax()
{
    m_Game->SetGameResult(m_Seat, 0, 0, 0, m_Tax, FALSE, FALSE, TK_UNKNOWN);
    m_Tax = 0;
}

int64_t MJPlayer::GetTax() const
{
    return m_Tax;
}

void MJPlayer::SyncGameResult()
{
    auto gamePlayer = m_Game->GetGamePlayer(m_Seat);
    WORD wGameResult = (m_ScoreChange > 0) ? TK_WIN : TK_LOSS;
    m_Game->SetGameResult(m_Seat, 0, 0, 0, m_ScoreChange, TRUE, FALSE, wGameResult);

    auto gotScore = m_ScoreChange - m_Tax;
    if (m_Game->IsCoinMatch() && gotScore > 0)
    {  //需要写入赢取金币的event
        auto coin = gotScore * m_Game->m_pMatchRuler->GetExChangeRate32_StageRuler();
        if (coin > 0)
        {
            gamePlayer->AddAtomGameEvent(MJGameEventID::Coin, DWORD(coin), TRUE);
        }
    }

    gamePlayer->AddAtomGameEvent(MJGameEventID::Over, 1, TRUE);

    DWORD winState = GetScoreChange() > 0 ? 1 : 0;
    auto specID = m_Game->IsFDTable() ? MJGameEventID::WinOrLose_FDTable : MJGameEventID::WinOrLose_NonFDTable;
    gamePlayer->AddAtomGameEvent(specID, winState, TRUE);

    if (tk_pStatMgr)
    {
        tk_pStatMgr->AddStat3(TK_STAT_TYPE_GAME_TAX, m_Game->m_nGameID, m_Game->m_nProductID, 1, std::abs(m_ScoreChange), GetTax(), 0, 0, 0, 0, 0, 0, 0, gamePlayer->m_nDomain);
    }
    if (tk_pStatMgr && gamePlayer->m_bPacerBot)
    {
        tk_pStatMgr->AddStat3(TK_STAT_TYPE_LORD_BOTAI, m_Game->m_nGameID, m_Game->m_nProductID, 1, wGameResult == TK_WIN);
    }

    m_Game->GetGrowSystem()->UpdateExp(m_Seat, GetScoreChange() > 0);
}

void MJPlayer::AddOperateEvent(int chowCount, int ponCount, int konCount, bool isCall)
{
    auto gamePlayer = m_Game->GetGamePlayer(m_Seat);
    gamePlayer->AddAtomGameEvent(MJGameEventID::ChowCount, chowCount, TRUE);
    gamePlayer->AddAtomGameEvent(MJGameEventID::PonCount, ponCount, TRUE);
    gamePlayer->AddAtomGameEvent(MJGameEventID::KonCount, konCount, TRUE);
    if (isCall)
    {
        gamePlayer->AddAtomGameEvent(MJGameEventID::IsCall, 1, TRUE);
    }
}

void MJPlayer::AddHuEvent()
{
    auto gamePlayer = m_Game->GetGamePlayer(m_Seat);
    if (m_HuMode == EHuResultMode::PaoHu)
    {
        gamePlayer->AddAtomGameEvent(MJGameEventID::HuDianPao, 1, TRUE);
    }
    else if (m_HuMode == EHuResultMode::ZiMo)
    {
        gamePlayer->AddAtomGameEvent(MJGameEventID::HuZiMo, 1, TRUE);
    }
    if (m_Game->GetScoreBase() > 0)
    {
        auto totalMutiple = GetScoreChange() / m_Game->GetScoreBase();
        gamePlayer->AddAtomGameEvent(MJGameEventID::TotalTimes, (DWORD)totalMutiple, TRUE);
    }
}

MJPlayerStatus::Type MJPlayer::GetStatus() const
{
    return m_Status;
}

void MJPlayer::ResetScore(int64_t score)
{
    m_Score = score;
}

int64_t MJPlayer::AddScoreChange(int64_t value)
{
    m_ScoreChange += value;
    return m_ScoreChange;
}

int64_t MJPlayer::GetIncremental(int64_t value)
{
    if (value != 0 && m_IsOut)
    {
        value -= m_Tax;
    }
    return value;
}

int64_t MJPlayer::GetScoreChange() const
{
    return m_ScoreChange;
}

int64_t MJPlayer::GetScoreChangeWithTax() const
{
    return m_ScoreChange - m_Tax;
}

int64_t MJPlayer::GetCurrentScore() const
{
    return m_Score + m_ScoreChange;
}

void MJGamePlayer::SetValueByID(DWORD growID, int64_t value)
{
    m_GrowValues[growID] = value;
}

int64_t MJGamePlayer::GetValueByID(DWORD growID, int64_t notFoundValue) const
{
    auto iter = m_GrowValues.find(growID);
    if (iter == m_GrowValues.end())
    {
        return notFoundValue;
    }
    return iter->second;
}

int64_t MJGamePlayer::GetHonorValue() const
{
    return GetValueByID(EXP_GROW_ID_VALUE, -1);
}

void MJGamePlayer::SetCallback(GrowValueCallback onGrowValueLoad)
{
    m_Callback = onGrowValueLoad;
}

BOOL MJGamePlayer::OnPivotMsg(PTKHEADER in_pAckMsg, int seatBind, DWORD growIDBind)
{
    if (in_pAckMsg == nullptr)
    {
        MJ_LOG_ERROR(nullptr, "Error:LevelSystem Msg null, seat:%d growID:%d", (int)seatBind, (int)growIDBind);
        return FALSE;
    }

    if (in_pAckMsg->dwParam != TK_ACKRESULT_SUCCESS)
    {
        MJ_LOG_ERROR(nullptr, "Error:LevelSystem Failed, seat:%d growID:%d", (int)seatBind, (int)growIDBind);
        return FALSE;
    }

    if (in_pAckMsg->dwType != (TK_ACK | TKID_GS2PIVOT_GET_SINGLEGROW))
    {
        MJ_LOG_ERROR(nullptr, "GetSingleGrowAck Error Type. type:%d", (int)in_pAckMsg->dwType);
        return FALSE;
    }

    //////////////////////////////////////////////////////////////////////////
    msg_pivot2gs_ack_get_singleGrow* pAck = (msg_pivot2gs_ack_get_singleGrow*)in_pAckMsg;

    if (in_pAckMsg->dwLength + TKHEADERSIZE < pAck->stSuffixIdx.dwSuffixOffSet + sizeof(TKMATCHSTRANGROW))
    {
        MJ_LOG_ERROR(nullptr, "GetSingleGrowAck Error Size. dwSuffixOffSet:%d", (int)pAck->stSuffixIdx.dwSuffixOffSet);
        return FALSE;
    }

    PTKMATCHSTRANGROW pGrowData = (PTKMATCHSTRANGROW)((BYTE*)pAck + pAck->stSuffixIdx.dwSuffixOffSet);
    if (pGrowData->dwGrowID != growIDBind)
    {
        MJ_LOG_ERROR(nullptr, "Error:Diff GrowID, seat:%d growID:%d,%d", (int)seatBind, (int)growIDBind, (int)pGrowData->dwGrowID);
        return FALSE;
    }

    SetValueByID(pGrowData->dwGrowID, pGrowData->dwValue);

    // 如果有回调，则调用之
    if (m_Callback)
    {
        m_Callback(m_nSeatOrder, pGrowData->dwGrowID, pGrowData->dwValue);
    }
    return TRUE;
}
