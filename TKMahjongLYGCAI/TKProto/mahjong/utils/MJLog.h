#pragma once
#include <string>
#include "MJString.h"

#if _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>
#endif

//for declare of __WriteLog
#ifdef TK_BOT
#include <TKBotPlayer.h>
#define __WriteLog TKWriteLog
#elif defined TK_TRUST
extern BOOL TKDllWriteLog(LPCTSTR pszFmt, ...);
#define __WriteLog TKDllWriteLog
#else
#include <TKDllServer.h>
#define __WriteLog TKWriteLog
#endif

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#ifndef M_LOGS
#define M_LOGS
inline const char* _S(char* s)
{
    return s;
}
inline const char* _S(const char* s)
{
    return s;
}
inline const char* _S(std::string const& s)
{
    return s.c_str();
}
template <typename T>
void _S(T t)
{  // other type not allowed for %s
}

template <typename T>
inline int _D(T const& v)
{
    return int(v);
}
#endif

#ifdef TK_BOT
inline int MJ_LOG_OBJ_ID(CTKBotPlayer* obj)
{
    return (obj ? int(obj->m_dwBotUserID) : -1);
}
#elif defined TK_TRUST
inline int MJ_LOG_OBJ_ID(void* obj)
{
    return int(obj);
}
#else
inline int MJ_LOG_OBJ_ID(CTKObject* obj)
{
    return (obj ? int(obj->m_nTKOjbectID) : -1);
}
#endif

#if defined(NO_LOG)
#define MJ_LOG_ERROR(...)
#define MJ_LOG_DEBUG(...)
#define MJ_LOG_INFO(...)
#else
#define MJ_LOG_ERROR(obj, fmt, ...) __WriteLog("%s(%d)<%s>[ERROR:objID=%d]:" fmt, MJ::GetPureFileName(__FILE__), __LINE__, __FUNCTION__, MJ_LOG_OBJ_ID(obj), ##__VA_ARGS__)

#if _DEBUG
#define MJ_LOG_DEBUG(obj, fmt, ...) __WriteLog("%s(%d)<%s>[DEBUG:objID=%d]:" fmt, MJ::GetPureFileName(__FILE__), __LINE__, __FUNCTION__, MJ_LOG_OBJ_ID(obj), ##__VA_ARGS__)
#else
#define MJ_LOG_DEBUG(...) void(0)
#endif

#define MJ_LOG_INFO(obj, fmt, ...) __WriteLog("%s(%d)<%s>[DLLLOG:objID=%d]:" fmt, MJ::GetPureFileName(__FILE__), __LINE__, __FUNCTION__, MJ_LOG_OBJ_ID(obj), ##__VA_ARGS__)

#define MJ_LOG_STR(obj, str) __WriteLog("%s(%d)<%s>[DLLLOG:objID=%d]:%s", MJ::GetPureFileName(__FILE__), __LINE__, __FUNCTION__, MJ_LOG_OBJ_ID(obj), _S(str))
#endif
