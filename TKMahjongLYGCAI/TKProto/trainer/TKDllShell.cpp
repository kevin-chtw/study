#include "TKDllShell.h"

TKDllShell::TKDllShell()
{
}

TKDllShell::~TKDllShell()
{
}

void TKDllShell::InitializeServer()
{
    tk_pDllServer->OnInitialUpdate();
    tk_pDllServer->OnRegisterTKObject();
}

void TKDllShell::StartGame(GameConfig const& conf)
{
    if (m_CurrentGame)
    {
        delete m_CurrentGame;
    }
    m_CurrentGame = tk_pDllServer->OnCreateGame();
    m_CurrentGame->__Initialize(conf);
    m_CurrentGame->OnInitialUpdate();
    m_CurrentGame->OnGameBegin();
}

bool TKDllShell::TimeForward()
{
    return m_CurrentGame->__TrigerTimer();
}

CTKGame* TKDllShell::GetCurrentGame()
{
    return m_CurrentGame;
}

CTKDllServer* TKDllShell::GetServer()
{
    return tk_pDllServer;
}

void TKDllShell::GetAction(Json::Value& action)
{
    std::string act;
    while (m_CurrentGame->m_IsRunning)
    {
        if (m_CurrentGame->__GetAction(act))
        {
            break;
        }
        m_CurrentGame->__TrigerTimer();
    }
    action["action"] = act;
    m_CurrentGame->__GetCommonData(action);
}

bool TKDllShell::DoAction(const std::string& res)
{
    return m_CurrentGame->__HandAction(res);
}
