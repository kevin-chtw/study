#pragma once
#include "MJDefine.h"

class MJGame;

// base class for Sub Modules in MJGame.
class MJGameModule
{
public:
    virtual ~MJGameModule() = default;

    MJGameModule(MJGame* game)
    {
        m_Game = game;
    }

protected:
    MJGameModule() = delete;
    MJGameModule(MJGameModule&&) = delete;
    void operator=(MJGameModule&&) = delete;

    MJGame* m_Game{};
    friend class MJGame;
};

struct BrokerReqData
{
    int serialsNo{};
    std::string url;
    std::string extra;
    std::string httpHeader;
    std::string httpBody;
};
struct BrokerAckData
{
    int serialsNo{};
    std::string extra;
    std::string httpHeader;
    std::string httpBody;
};
