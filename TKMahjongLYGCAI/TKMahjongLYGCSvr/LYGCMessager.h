#pragma once
#include "LYGCDefine.h"
#include "LYGCPlay.h"
#include "MJGame.h"
#include "MJHttp.h"
#include "LYGCHu.h"

#include "TKMahjongLYGC.pb.h"
using namespace ::cn::jj::service::msg::protocol;
namespace proto = ::google::protobuf;

struct WinAckData
{
    int winMode{EHT_ZiMo};
    ISeatID paoSeat{-1};
    std::vector<ISeatID> winSeats;
    bool isGrabKon = false;
};

// 统一处理发送给前端的消息
class LYGCMessager
{
public:
    LYGCMessager(LYGCGame* game);

    void SendPlayerTrustAck(ISeatID seat, ETrustType trust, ISeatID sendTo = SEAT_ALL);
    void SendDebugString(std::string const& str, ISeatID seat = SEAT_ALL);

    void SendGameBeginAck();
    void SendPlaceAck();
    void SendOpenDoorAck();
    void SendOpenFanCiAck();
    void SendGenZhuangAck();
    void SendOperatesAck(ISeatID seat, MJOperates operates, std::vector<ITileID> const& gangtiles, std::vector<ITileID> const& citiles = {});
    void SendKonAck(ISeatID konSeat, KonType konType);
    void SendPonAck(ISeatID seat);
    void SendDiscardAck();
    void SendDrawAck(const std::map<ITileID, int> mShowTiles);
    void SendWinAck(WinAckData const& data);
    void SendTips(MJTips::Type nType, ISeatID seat = -1);
    void SendHandTiles();
    void SendMahjongResult(bool isLiuJu, ISeatID paoSeat, ISeatID paoCiSeat);
    void SendBeginAnimalAck();

protected:
    void _SetAckCallData(proto::RepeatedPtrField<TKMahJongCallData>* pAckCallDatas, CallDataMap const& callData);
    void _SetAckCallTiles(TKMahJongCallData* pAckCallData, CallTileFan const& tileFans);
    void _SetUntrustOnGameEnd(ISeatID seat);  // 结束时取消托管
    void _SetHandTIles(TKMahJongHandTiles* pAck, ISeatID seat);
    bool _CheckTouchCard(const proto::RepeatedPtrField<TKMahJongCallData>& pb, ITileID tile, const std::map<ITileID, int> mShowTiles);
    ISeatID _CurSeat();
    ITileID _CurTile();

protected:
    LYGCGame* m_Game{};
};
