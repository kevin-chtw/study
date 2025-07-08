#pragma once
#include <chrono>
#include <functional>
#include "MJModule.h"

constexpr int MJ_TIMER_INTERVAL_MS = 250;
constexpr int MJ_GAME_TIMEOUT_MS = 120 * 1000;

class MJGame;

class MJTimer : public MJGameModule
{
public:
    using MJGameModule::MJGameModule;

    void Schedule(std::chrono::milliseconds delay, std::function<void()> callback);
    void Cancel();
    void SetLongLive(bool infinite);

protected:
    friend class MJGame;

    void OnTick();

    std::chrono::time_point<std::chrono::system_clock> m_TrigerTime{};
    std::chrono::milliseconds m_Delay{0};
    std::function<void()> m_Callback;
    bool m_IsLongLive{};
};
