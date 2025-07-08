#pragma once
#include "LYGCDefine.h"
#include "LYGCGame.h"
#include "LYGCPlay.h"
#include "LYGCMessager.h"

using YBMsgHandler = std::function<void(ISeatID seat, MahJongLYGCReqMsg const& req)>;
#define MSG_HANDLER(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2)

// 负责协调事件（网络、计时器等）-玩法，以及各类外围数据处理
class State : public MJStateEx<MahJongLYGCReqMsg>
{
public:
    LYGCGame* GetGame();
    std::shared_ptr<LYGCPlay> GetPlay();
    std::shared_ptr<LYGCPlayData> GetLYGCPlayData(ISeatID seat);

    LYGCMessager* GetMessager();

protected:
    std::bitset<NP4> m_RequestStatus{};
};

inline LYGCGame* State::GetGame()
{
    return dynamic_cast<LYGCGame*>(m_Game);
}

inline std::shared_ptr<LYGCPlay> State::GetPlay()
{
    return GetGame()->GetPlay();
}

inline std::shared_ptr<LYGCPlayData> State::GetLYGCPlayData(ISeatID seat)
{
    return GetPlay()->GetPlayData<LYGCPlayData>(seat);
}

inline LYGCMessager* State::GetMessager()
{
    return GetGame()->GetMessager().get();
}

class StateInit : public State
{
public:
    virtual void OnEnter() override;
};

class StateDeal : public State
{
public:
    void OnReqRequest(ISeatID seat, MahJongLYGCReqMsg const& req);
    virtual void OnEnter() override;
};

class StateDraw : public State
{
public:
    virtual void OnEnter() override;
};
