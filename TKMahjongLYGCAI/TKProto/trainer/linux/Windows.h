#pragma once
#include <chrono>
#include <iostream>
#include <cstring>
#include "WS2tcpip.h"

using UINT = uint32_t;
using TCHAR = char;
using UINT = uint32_t;
using UINT64 = uint64_t;
using UINT32 = uint32_t;

#define TEXT(...) __VA_ARGS__

#define MAX_PATH 260

inline uint64_t GetTickCount64()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
using errno_t = int;
inline errno_t strcat_s(char* strDestination, size_t numberOfElements, const char* strSource)
{
    if (strlen(strDestination) + strlen(strSource) + 1 > numberOfElements)
        return -1;
    strcat(strDestination, strSource);
    return 0;
}
template <size_t size>
errno_t strcat_s(char (&strDestination)[size], const char* strSource)
{
    if (strlen(strDestination) + strlen(strSource) + 1 > size)
        return -1;
    strcat(strDestination, strSource);
    return 0;
}
