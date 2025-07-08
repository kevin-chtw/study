#pragma once
#include <TKDllServer.h>
#include <mutex>
#include <atomic>
#include "MJHu.h"
#include "MJTing.h"
#include "MJTile.h"
#include "MJString.h"
#include "MJDealer.h"
#include <TKDllGame.h>
#include <TKDllPlayer.h>
#include "MJConfig.h"
#include "MJTing.h"

extern std::string LoadModuleVersion();
extern std::string LoadModuleName();

class GameCounter
{
public:
    static GameCounter* Instance();

    GameCounter();

    virtual ~GameCounter() = default;

    void AddGameCount()
    {
        ++m_GameCount;
    }
    void DecGameCount()
    {
        --m_GameCount;
    }
    int GetGameCount()
    {
        return m_GameCount;
    }

protected:
    std::atomic<int> m_GameCount{};
};

class MJHuBase
{  //建议以单例方式实现子类
public:
    virtual CallDataMap CheckCall(HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg)
    {
        return {};
    }
    virtual bool CheckHu(HuResult& result, HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg)
    {
        return false;
    }
};

class Service : public CTKDllServer
{
public:
    Service(){};
    virtual ~Service(){};

    BOOL OnInitialUpdate() override;
    virtual std::shared_ptr<MJTingBase> OnCreateTingTool();  //默认MJTingEx14
    virtual std::shared_ptr<MJHuBase> OnCreateHuTool();      //默认MJHuBase
public:
    template <typename T = MJTingBase>
    std::shared_ptr<T> GetTingTool()  //默认MJTingEx14
    {
        return std::dynamic_pointer_cast<T>(m_TingTool);
    }
    template <typename T = MJHuBase>
    std::shared_ptr<T> GetHuTool()  //默认MJHuBase
    {
        return std::dynamic_pointer_cast<T>(m_HuTool);
    }
    virtual int GetHandCount()
    {
        return TILE_COUNT_INIT_NORMAL;
    }

    std::string GetVersion()
    {
        return m_Version;
    }
    std::string GetModuleName()
    {
        return m_ModuleName;
    }

private:
    std::string m_Version;
    std::string m_ModuleName;

    std::shared_ptr<MJTingBase> m_TingTool;  //默认MJTingEx14
    std::shared_ptr<MJHuBase> m_HuTool;      //默认MJHuBase
};

template <typename TGame, typename TPlayer, int GameID, int Port>
class MJService : public Service, public GameCounter
{
public:
    MJService(MJService&&) = delete;
    MJService()
    {
        m_nGameID = GameID;
        m_nPort = Port;
    }
    // 特殊的初始化过程，这里继承处理
    virtual void InitService()
    {
    }

public:
    virtual BOOL OnRegisterTKObject() override
    {
        if (!CTKDllServer::OnRegisterTKObject())
        {
            return FALSE;
        }

        REGISTER_TKCLASS(CTKGame, TGame);
        REGISTER_TKCLASS(CTKGamePlayer, TPlayer);

        return TRUE;
    }
    virtual BOOL OnInitialUpdate() override
    {
        if (!Service::OnInitialUpdate())
        {
            return FALSE;
        }

        this->InitService();
        return TRUE;
    }
    virtual CTKGame* OnCreateGame() override
    {
        return CREATE_TKOBJECT(CTKGame, TGame);
    }
    virtual CTKGamePlayer* OnCreateTKGamePlayer() override
    {
        return CREATE_TKOBJECT(CTKGamePlayer, TPlayer);
    }
};

template <typename T = Service>
T* GetMJService()
{
    return dynamic_cast<T*>(tk_pDllServer);
}
