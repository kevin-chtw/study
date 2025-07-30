#include "StdAfx.h"
#include "TKJunQiPKService.h"
#include <intrin.h>
#include "TKJunQiPKGame.h"

IMPLEMENT_CREATEDLLSERVER(CTKJunQiPKService);

CTKJunQiPKService::CTKJunQiPKService(void)
{
    __super::m_nGameID = TK_GAMEID_JUNQIPK;
    m_nPort = 21076;
}

CTKJunQiPKService::~CTKJunQiPKService(void)
{
}

BOOL CTKJunQiPKService::OnRegisterTKObject()
{
    if (!__super::OnRegisterTKObject())
    {
        return FALSE;
    }

    REGISTER_TKCLASS(CTKGame, CTKJunQiPKGame);
    REGISTER_TKCLASS(CTKGamePlayer, CTKJunQiPlayer);
    return TRUE;
}

CTKGame* CTKJunQiPKService::OnCreateGame()
{
    return CREATE_TKOBJECT(CTKGame, CTKJunQiPKGame);
}

CTKGamePlayer* CTKJunQiPKService::OnCreateTKGamePlayer()
{
    return (CTKGamePlayer*)(CREATE_TKOBJECT(CTKGamePlayer, CTKJunQiPlayer));
}

BOOL CTKJunQiPKService::OnInitialUpdate()
{
    if (!__super::OnInitialUpdate())
    {
        return FALSE;
    }

    m_szProductVersion = _initProductVersion();
    return TRUE;
}

const std::string& CTKJunQiPKService::GetProductVersion()
{
    return m_szProductVersion;
}

std::string CTKJunQiPKService::_initProductVersion()
{
    HMODULE hModule = NULL;
    void* callerAddress = _ReturnAddress();
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)callerAddress, &hModule);

    int nMaxPathName = 4096;
    char* pBuffer = new char[nMaxPathName];
    GetModuleFileName(hModule, pBuffer, nMaxPathName - 1);

    DWORD dwHandle;
    DWORD dwInfoSize = GetFileVersionInfoSize(pBuffer, &dwHandle);

    char sVersion[100];
    if (dwInfoSize > 0)
    {
        void* pData = new char[dwInfoSize];
        void* lpBuffer;
        UINT nItemLength;
        if (GetFileVersionInfo(pBuffer, dwHandle, dwInfoSize, pData) &&
            VerQueryValue(pData, "\\", &lpBuffer, &nItemLength))
        {
            VS_FIXEDFILEINFO* pFileInfo = (VS_FIXEDFILEINFO*)lpBuffer;
            sprintf_s(sVersion, "%d.%d.%d.%d", pFileInfo->dwProductVersionMS >> 16,
                      pFileInfo->dwProductVersionMS & 0xFFFF, pFileInfo->dwProductVersionLS >> 16,
                      pFileInfo->dwProductVersionLS & 0xFFFF);
        }
        delete[] pData;
    }
    delete[] pBuffer;
    return sVersion;
}