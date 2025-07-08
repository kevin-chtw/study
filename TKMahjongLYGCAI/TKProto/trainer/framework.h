// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include <cstdint>
#include <chrono>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <functional>
#include <deque>
#include <stdarg.h>

#if _WIN32
#define WIN32_LEAN_AND_MEAN  // 从 Windows 头文件中排除极少使用的内容
#define NOMINMAX
#include <Windows.h>

#elif defined(__linux__)

typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef uint32_t UINT;
typedef unsigned long DWORD;
typedef int32_t BOOL;
typedef int64_t INT64;
typedef uint64_t UINT64;

typedef int64_t LONGLONG;
typedef uint64_t ULONGLONG;
typedef LONGLONG* PLONGLONG;
typedef ULONGLONG* PULONGLONG;
typedef uint64_t ULONG64;

typedef struct
{
} MODULE, *HMODULE;

using __time32_t = time_t;
using ULONG_PTR = unsigned long;
using PULONG_PTR = unsigned long*;
using LONG = long;
#define TRUE 1
#define FALSE 0
#define __cdecl

using LPCSTR = const char*;
using PCSTR = const char*;
using PSTR = char*;
using LPSTR = char*;
using LPCTSTR = const char*;
using LPVOID = void*;
using LPEXCEPTION_POINTERS = void*;  // temp

using __int64 = int64_t;
using __int32 = int32_t;
using __uint64 = uint64_t;
using __uint32 = uint32_t;

#define _stricmp strcasecmp
#define wsprintf sprintf

inline void ZeroMemory(void* dst, size_t bytes)
{
    ::memset(dst, 0, bytes);
}

inline int sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int result = vsnprintf(buffer, sizeOfBuffer, format, ap);
    va_end(ap);
    return result;
}

template <size_t sizeOfBuffer>
inline int sprintf_s(char (&buffer)[sizeOfBuffer], const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int result = vsnprintf(buffer, sizeOfBuffer, format, ap);
    va_end(ap);
    return result;
}

inline DWORD GetTickCount()
{
    return (DWORD)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void GetModuleFileName(HMODULE m, char* fileName, int size);

void localtime_s(tm* t, time_t* tt);
int GetCurrentProcessId();

UINT GetPrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nDefault, LPCSTR lpFileName);
DWORD GetPrivateProfileString(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName);

#else
#error "unknow system"
#endif

using CInt = int;
using CInt64 = int64_t;
using CWord = WORD;
using CDword = DWORD;
using CBool = bool;
using CByte = char;
using CTime_t = time_t;
