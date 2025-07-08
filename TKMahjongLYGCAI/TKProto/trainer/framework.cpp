#include "framework.h"

#if defined(__linux__)

void GetModuleFileName(HMODULE m, char* fileName, int size)
{
}

void localtime_s(tm* t, time_t* tt)
{
}

int GetCurrentProcessId()
{
    return 0;
}

UINT GetPrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nDefault, LPCSTR lpFileName)
{
    return 0;
}

DWORD GetPrivateProfileString(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName)
{
    memcpy(lpReturnedString, lpDefault, strlen(lpDefault));
    return strlen(lpDefault);
}

#endif
