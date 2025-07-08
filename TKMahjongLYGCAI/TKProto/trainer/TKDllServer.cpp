#include "TKDllServer.h"
#include <cstdarg>

HMODULE tk_hDllModule = nullptr;
CTKDllServer* tk_pDllServer = TKCreateDllServer();
CTKStat* tk_pStatMgr = nullptr;

CTKDllServer::CTKDllServer()
{
}

CTKDllServer::~CTKDllServer()
{
}

BOOL CTKDllServer::OnInitialUpdate()
{
    return TRUE;
}

BOOL CTKDllServer::OnRegisterTKObject()
{
    return TRUE;
}

CTKGame* CTKDllServer::OnCreateGame()
{
    return nullptr;
}

BOOL CTKDllServer::OnTimeout(tm* pTime)
{
    return 0;
}

CTKGamePlayer* CTKDllServer::OnCreateTKGamePlayer()
{
    return nullptr;
}

BOOL CTKDllServer::PostMsg2Pivot_GSBase(PTKHEADER in_pMsgReq, DWORD in_dwUserID, DWORD in_dwMatchID, WORD in_wStageID,
                                        WORD in_wRoundID, CFuncObjCallback* in_pFuncObjCallback)
{
    in_pFuncObjCallback->Execute(nullptr);
    delete in_pFuncObjCallback;
    return TRUE;
}

BOOL CTKDllServer::OnCallback(DWORD dwCallback, void* pParam)
{
    return 0;
}

BOOL CTKDllServer::PostMsg2MS(PTKHEADER pMsg, int nType, DWORD dwParam, WORD wParam, DWORD dwSerialGroup)
{
    return 0;
}

void CTKDllServer::RegisterInpFactoryUnRegFunction(UNREGISTERINPFACTORY pFunc)
{
}

CTKStat::CTKStat()
{
}

CTKStat::~CTKStat()
{
}

void CTKStat::AddStat3(int nType1, int nType2, int nType3, LONGLONG ll0, LONGLONG ll1 /*= 0*/, LONGLONG ll2 /*= 0*/,
                       LONGLONG ll3 /*= 0*/, LONGLONG ll4 /*= 0*/, LONGLONG ll5 /*= 0*/, LONGLONG ll6 /*= 0*/,
                       LONGLONG ll7 /*= 0*/, LONGLONG ll8 /*= 0*/, LONGLONG ll9 /*= 0*/, int nDomain /*= 0*/)
{
}

BOOL TKWriteLog(const char* pszFmt, ...)
{
    va_list argList{};
    va_start(argList, pszFmt);
    vprintf(pszFmt, argList);
    va_end(argList);
    printf("\n");
    return TRUE;
}

DWORD TKGetIniString(const char* pszSection, const char* pszEntry, const char* pszDefault, char* pszStr, DWORD dwSize,
                     const char* pszIniFile)
{
#if _WIN32
    return GetPrivateProfileString(pszSection, pszEntry, pszDefault, pszStr, dwSize, pszIniFile);
#else
    return 1;
#endif
}

int TKGetIniInt(const char* pszSection, const char* pszEntry, int nDefault, const char* pszIniFile)
{
#if _WIN32
    return GetPrivateProfileInt(pszSection, pszEntry, nDefault, pszIniFile);
#else
    return nDefault;
#endif
}
