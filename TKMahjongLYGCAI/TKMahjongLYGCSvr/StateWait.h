#pragma once
#include "State.h"

class StateWait : public State
{
public:
    virtual void OnEnter() override;
    void OnReqRequest(ISeatID seat, MahJongLYGCReqMsg const& req);
    void Timeout();
    void SetRequestAction(ISeatID seat, EOperate operate);
    void TryHandleAction();
    void ExcuteAction(ISeatID seat, EOperate operate);
    void ExcuteActionHu(std::vector<ISeatID> const& huSeats);
    void ToDrawState(ISeatID seat = SEAT_NULL);  // SEAT_NULL ˳���¼�
    void ToDiscardState(ISeatID seat);

private:
    bool _IsValidOperate(ISeatID seat, EOperate operate);
    int _GetMaxOperate(ISeatID seat);
    int _GetDefaultOperate(ISeatID seat);
    std::array<MJOperates, NP4> m_OperatesForSeats{};  // �ɽ��еĲ���
    std::array<EOperate, NP4> m_ReqOperateForSeats{};  // �Ѿ����еĲ���
};
