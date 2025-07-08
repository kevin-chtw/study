#pragma once
#include "LYGCDefine.h"
#include "TKMahjongLYGC.pb.h"

using namespace cn::jj::service::msg::protocol;
class LYGCMsgDispatcher
{
public:
    void Dispatch(const MahJongLYGCAckMsg& ack, ISeatID seat);

private:
    bool _TryHandleGameBeginAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandlePlaceAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleOpenDoorAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleActionAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandlePengAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleGangAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleWinAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleDiscardTileAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleDrawTileAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleTrustPlayAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleScoreChangeAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleResultAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleMessageAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleDissolutionAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleCallDataAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleBeginAnimalAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleHandTilesAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleGenZhuangAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleFanCiAck(const MahJongLYGCAckMsg& ack, ISeatID seat);
    bool _TryHandleDebugLogAck(const MahJongLYGCAckMsg& ack, ISeatID seat);

public:
    virtual void OnGameBeginAck(const TKMahJongGameBeginAck& ack, ISeatID seat){};
    virtual void OnPlaceAck(const TKMahJongPlaceAck& ack, ISeatID seat){};
    virtual void OnOpenDoorAck(const TKMahJongOpenDoorAck& ack, ISeatID seat){};
    virtual void OnActionAck(const TKMahJongActionAck& ack, ISeatID seat){};
    virtual void OnPengAck(const TKMahJongPengAck& ack, ISeatID seat){};
    virtual void OnGangAck(const TKMahJongGangAck& ack, ISeatID seat){};
    virtual void OnWinAck(const TKMahJongWinAck& ack, ISeatID seat){};
    virtual void OnDiscardTileAck(const TKMahJongDiscardTileAck& ack, ISeatID seat){};
    virtual void OnDrawTileAck(const TKMahJongDrawTileAck& ack, ISeatID seat){};
    virtual void OnTrustPlayAck(const TKMahJongTrustPlayAck& ack, ISeatID seat){};
    virtual void OnScoreChangeAck(const TKMahJongScoreChangeAck& ack, ISeatID seat){};
    virtual void OnResultAck(const TKMahJongResultAck& ack, ISeatID seat){};
    virtual void OnMessageAck(const TKMahJongMessageAck& ack, ISeatID seat){};
    virtual void OnDissolutionAck(const TKMahJongDissolutionAck& ack, ISeatID seat){};
    virtual void OnCallDataAck(const TKMahjongCallDataAck& ack, ISeatID seat){};
    virtual void OnBeginAnimalAck(const TKMahjongBeginAnimalAck& ack, ISeatID seat){};
    virtual void OnHandTilesAck(const TKMahjongHandTilesAck& ack, ISeatID seat){};
    virtual void OnGenZhuangAck(const TKMahjongGenZhuangAck& ack, ISeatID seat){};
    virtual void OnFanCiAck(const TKMahjongFanCiAck& ack, ISeatID seat){};
    virtual void OnDebugLogAck(const TKMahJongDebugLogAck& ack, ISeatID seat){};
};