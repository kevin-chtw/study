#pragma once

#include <memory>
#include <string>
#include <chrono>
#include <bitset>
#include <google/protobuf/message.h>

#include "MJModule.h"
#include "MJLog.h"

class MJGame;

using namespace std::chrono;

class MJState
{
public:
    MJState() = default;
    virtual void OnEnter() = 0;
    virtual ~MJState() = default;

    virtual void HandleHttpBrokerMsg(BrokerAckData const& ack)
    {
    }
    virtual void HandlePlayerMsg(ISeatID seat, ::google::protobuf::Message const* message)
    {
    }

    template <typename TState, typename... Args>
    void EnterState(Args... args)
    {
        m_Game->template SetNextState<TState>(args...);
    }

protected:
    std::string m_Name;
    MJGame* m_Game{};
    friend class MJGame;
};

template <typename MahJongReqMsg>
class MJStateEx : public MJState
{
public:
    using MsgHandler = std::function<void(ISeatID seat, MahJongReqMsg const& req)>;

    void AsyncMsgTimer(MsgHandler onMsg, milliseconds timeout, std::function<void()> onTimeout)
    {
        m_MsgHandler = onMsg;
        m_Game->GetTimer()->Schedule(timeout, onTimeout);
    }
    void AsyncTimer(milliseconds timeout, std::function<void()> onTimeout)
    {
        m_MsgHandler = nullptr;
        m_Game->GetTimer()->Schedule(timeout, onTimeout);
    }

    virtual void HandlePlayerMsg(ISeatID seat, ::google::protobuf::Message const* message)
    {
        this->HandleMahJongReqMsg(seat, dynamic_cast<MahJongReqMsg const*>(message));
    }

    void HandleMahJongReqMsg(ISeatID seat, MahJongReqMsg const* req)
    {
        if (m_MsgHandler && req)
        {
            m_MsgHandler(seat, *req);
        }
    }

protected:
    MsgHandler m_MsgHandler;

    //[[deprecated]] std::bitset<NP4> m_RequestStatus{};  // ÊÇ·ñÒÑ²Ù×÷
};
