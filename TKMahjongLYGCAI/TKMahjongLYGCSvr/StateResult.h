#pragma once
#include "State.h"

class StateResult : public State
{
public:
    void OnReqRequest(ISeatID seat, MahJongLYGCReqMsg const& req);
    void OnGameEnd(bool isLiuju = false);

    void WaitClientAnimateDone();  // - PrepareNextState - EnterNextState -
    void PrepareNextState();

protected:
    void _OnPlayerHu(std::vector<int64_t>& multiples, ISeatID paoSeat, EHuType huType);
    std::vector<ISeatID> m_HuSeats;
    ISeatID m_PaoSeat = SEAT_NULL;
    ISeatID m_PaoCiSeat = SEAT_NULL;
    bool m_QiangGang = false;
};

class StateResultLiuJu : public StateResult
{
public:
    void OnEnter() override;

protected:
    void _OnPlayerLiuJu();
};

class StateResultPaoHu : public StateResult
{
public:
    StateResultPaoHu(std::vector<ISeatID> const& huSeats, bool isGrabKon = false);
    void OnEnter() override;
};

class StateResultSelfHu : public StateResult
{
public:
    void OnEnter() override;
    StateResultSelfHu(EHuType huType, ISeatID nSeat);

private:
    ISeatID m_DianCiSeat{};
    EHuType m_HuType{};
};
