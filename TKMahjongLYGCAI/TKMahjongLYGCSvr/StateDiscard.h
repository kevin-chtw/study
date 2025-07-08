#pragma once
#include "State.h"

class StateDiscard : public State
{
public:
    StateDiscard();

public:
    virtual void OnEnter() override;
    void OnReqDiscard(ISeatID seat, MahJongLYGCReqMsg const& req);

    void AutoOperate(bool isTimeout);
    void Discard(ITileID tile);
    void Kon(ITileID tile);
    void OnTimeout();
    void Hu(EOperate operate, ITileID tile);

private:
    MJOperates m_Operates{};
};
