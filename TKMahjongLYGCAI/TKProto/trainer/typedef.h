// --------------------------------------------------------------------------------------------
// Copyright:     (C) Openersoft Corporation. All Rights Reserved
// File name:     typedef.h
// Description:   平台数据基本类型定义
// Author:        黄永涛
// Version:       1.0
// Date:          2013-08-25
// History:
// --------------------------------------------------------------------------------------------
#ifndef DEF_TYPEDEF_H
#define DEF_TYPEDEF_H

#if defined(_WINDOWS_)
// WIN 平台
typedef __int8 int8;
typedef unsigned __int8 uint8;
typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef signed __int64 int64_t;
typedef unsigned __int32 uint32_t;

#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
// linux 平台
#include <cstdint>
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef int SOCKET;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET ~0
#endif

#define SOCKET_ERROR (-1)
#define closesocket(s) close(s)

#define MAKEWORD(a, b) ((WORD)(((BYTE)((DWORD)(a)&0xff)) | ((WORD)((BYTE)((DWORD)(b)&0xff))) << 8))
#define MAKELONG(a, b) ((LONG)(((WORD)((DWORD)(a)&0xffff)) | ((DWORD)((WORD)((DWORD)(b)&0xffff))) << 16))
#define LOWORD(l) ((WORD)((DWORD)(l)&0xffff))
#define HIWORD(l) ((WORD)((DWORD)(l) >> 16))
#define LOBYTE(w) ((BYTE)((DWORD)(w)&0xff))
#define HIBYTE(w) ((BYTE)((DWORD)(w) >> 8))

#else
// 未支持的系统类型，ANSI C 标准
#include <stdint.h>
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
#endif

typedef unsigned char BYTE;
typedef uint16 WORD;
typedef uint32 UINT;
typedef unsigned long DWORD;
typedef int32 BOOL;
typedef int64 INT64;
typedef uint64 UINT64;

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define CP_GBK 936  //简体中文gb2312的编码

#endif  // DEF_TYPEDEF_H
