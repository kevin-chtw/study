#include "TKTrainer.h"
#include "TKDllShell.h"

static std::shared_ptr<TKDllShell> shell = nullptr;

DLLEXPORT void InitService()
{
    shell = std::make_shared<TKDllShell>();
    shell->InitializeServer();
}

DLLEXPORT void StartGame(const char* conf)
{
    GameConfig config{"", 4, 1, 1000};
    if (nullptr != conf)
    {
        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(conf, root, false) || !root.isObject())
        {
            return;
        }
        config.propertyEx = root["property"].asString();
        config.playerCount = root["playercount"].asInt();
        config.initialChip = root["chip"].asInt();
        config.scoreBase = root["scorebase"].asInt();
    }
    shell->StartGame(config);
}

DLLEXPORT const char* GetAction()
{
    Json::Value act;
    shell->GetAction(act);

    static std::string action;
    Json::FastWriter fw;
    action = fw.write(act);
    return action.c_str();
}

DLLEXPORT bool DoAction(char const* action)
{
    return shell->DoAction(action);
}
