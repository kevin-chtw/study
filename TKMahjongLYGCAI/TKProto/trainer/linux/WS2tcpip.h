#pragma once

typedef struct SYSTEMTIME
{
    int wYear{};
    int wMonth{};
    int wDay{};
    int wHour{};
    int wMinute{};
    int wSecond{};
    int wMilliseconds{};
};

inline void GetLocalTime(SYSTEMTIME* st)
{
}
