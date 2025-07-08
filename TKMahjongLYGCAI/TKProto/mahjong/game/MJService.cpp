#include "MJService.h"
#include "MJLog.h"

#if _WIN32
#include <winsock2.h>
#include <Windows.h>
#include <intrin.h>

#pragma comment(lib, "version.lib")
#endif

static std::string ToVersionString(DWORD high, DWORD low)
{
    return std::to_string(high >> 16) + '.' + std::to_string(high & 0xFFFF) + '.'  //--
           + std::to_string(low >> 16) + '.' + std::to_string(low & 0xFFFF);
}
std::string LoadModuleVersion()
{
#if _WIN32
    constexpr int nMaxPathName = 4096;  // Max length of file name/path
    // Get the file path and name
    std::vector<char> pathBuffer(nMaxPathName, 0);
    auto pBuffer = pathBuffer.data();  // new char[nMaxPathName];

    HMODULE hModule = NULL;

    void* callerAddress = _ReturnAddress();

    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCSTR)callerAddress,  //这是函数名，强转
        &hModule);
    GetModuleFileName(hModule, pBuffer, nMaxPathName - 1);
    // Get File Version Info size
    DWORD dwHandle;
    auto dwInfoSize = GetFileVersionInfoSize(pBuffer, &dwHandle);
    if (dwInfoSize > 0)
    {
        std::vector<char> dataBuffer(dwInfoSize, 0);
        void* pData = dataBuffer.data();  //new char[dwInfoSize];
        if (GetFileVersionInfo(pBuffer, dwHandle, dwInfoSize, pData))
        {
            void* lpBuffer = 0;
            UINT nItemLength = 0;
            if (VerQueryValue(pData, "\\", &lpBuffer, &nItemLength))
            {
                auto info = (VS_FIXEDFILEINFO*)lpBuffer;
                return ToVersionString(info->dwProductVersionMS, info->dwProductVersionLS);
            }
        }
    }
#endif
    return "";
}

std::string LoadModuleName()
{
    constexpr int NameSize = 512;
    char lpFileName[NameSize]{};
    ::GetModuleFileName(tk_hDllModule, lpFileName, NameSize);

    std::string moduleName = MJ::GetPureFileName(lpFileName);
    auto pos = moduleName.find_first_of("svr");
    if (pos != 0 && pos != std::string::npos)
    {
        return moduleName.substr(2, pos - 3);
    }
    return moduleName;
}

static GameCounter* s_GameCounter = nullptr;

GameCounter* GameCounter::Instance()
{
    return s_GameCounter;
}

GameCounter::GameCounter()
{
    s_GameCounter = this;
}

BOOL Service::OnInitialUpdate()
{
    if (!CTKDllServer::OnInitialUpdate())
    {
        return FALSE;
    }
    m_Version = ::LoadModuleVersion();
    m_ModuleName = ::LoadModuleName();

    m_HuTool = OnCreateHuTool();
    m_TingTool = OnCreateTingTool();
    return TRUE;
}

std::shared_ptr<MJTingBase> Service::OnCreateTingTool()
{
    return MJTingEx14::GetInstance();
}

std::shared_ptr<MJHuBase> Service::OnCreateHuTool()
{
    return std::make_shared<MJHuBase>();
}
