#include "MJTimer.h"
#include "MJGame.h"
#include "ExpireTime.h"

void MJTimer::Schedule(std::chrono::milliseconds delay, std::function<void()> callback)
{
    m_TrigerTime = std::chrono::system_clock::now();
    m_Delay = delay;
    m_Callback = callback;
}

void MJTimer::Cancel()
{
    m_Callback = nullptr;
}

void MJTimer::SetLongLive(bool infinite)
{
    m_IsLongLive = infinite;
}

void MJTimer::OnTick()
{
    if (!m_IsLongLive && IsExpireTimeTrue(m_TrigerTime, std::chrono::milliseconds(MJ_GAME_TIMEOUT_MS)))
    {
        MJ_LOG_ERROR(m_Game, "GameTimeout");
        m_Game->FinishGame(EGameOverStatus::Timeout);
        return;
    }
    if (m_Callback && IsExpireTime(m_TrigerTime, m_Delay))
    {
        auto callback = m_Callback;
        m_Callback = nullptr;
        callback();
    }
}
