#pragma once
#include <chrono>

inline bool IsExpireTime(std::chrono::system_clock::time_point expire)
{
#ifdef _TRAINER
    return true;
#else
    auto now = std::chrono::system_clock::now();
    return expire <= now;
#endif
}

inline bool IsExpireTimeTrue(std::chrono::system_clock::time_point start, std::chrono::milliseconds delay)
{
    auto expire = start + delay;
    auto now = std::chrono::system_clock::now();
    return expire <= now;
}

inline bool IsExpireTime(std::chrono::system_clock::time_point start, std::chrono::milliseconds delay)
{
#ifdef _TRAINER
    return true;
#else
    return IsExpireTimeTrue(start, delay);
#endif
}
