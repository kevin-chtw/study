#pragma once
#include "TKDllServer.h"
#include "TKDllGame.h"

class TKDllShell
{
public:
    TKDllShell();
    ~TKDllShell();

public:
    // call this once.
    void InitializeServer();

    // start a new HAND.
    void StartGame(GameConfig const& conf);

    bool TimeForward();

    CTKGame* GetCurrentGame();

    CTKDllServer* GetServer();

    void GetAction(Json::Value& action);

    bool DoAction(const std::string& res);

protected:
    CTKGame* m_CurrentGame{};
};
