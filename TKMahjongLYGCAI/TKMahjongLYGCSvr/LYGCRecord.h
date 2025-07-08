#pragma once
#include "MJRecord.h"
#include "LYGCMsgDispatcher.h"
#include "TKMahjongLYGC.pb.h"
using namespace ::cn::jj::service::msg::protocol;
namespace proto = ::google::protobuf;

class LYGCRecord : public MJRecord, LYGCMsgDispatcher
{
public:
    using MJRecord::MJRecord;

    void RecordAction(const google::protobuf::Message* ack, ISeatID seat) override;

private:
    void OnGameBeginAck(const TKMahJongGameBeginAck& ack, ISeatID seat) override;
    void OnPlaceAck(const TKMahJongPlaceAck& ack, ISeatID seat) override;
    void OnOpenDoorAck(const TKMahJongOpenDoorAck& ack, ISeatID seat) override;
    void OnDrawTileAck(const TKMahJongDrawTileAck& ack, ISeatID seat) override;
    void OnGangAck(const TKMahJongGangAck& ack, ISeatID seat) override;
    void OnDiscardTileAck(const TKMahJongDiscardTileAck& ack, ISeatID seat) override;
    void OnPengAck(const TKMahJongPengAck& ack, ISeatID seat) override;
    void OnTrustPlayAck(const TKMahJongTrustPlayAck& ack, ISeatID seat) override;
    void OnActionAck(const TKMahJongActionAck& ack, ISeatID seat) override;
    void OnWinAck(const TKMahJongWinAck& ack, ISeatID seat) override;
    void OnResultAck(const TKMahJongResultAck& ack, ISeatID seat) override;
    void OnFanCiAck(const TKMahjongFanCiAck& ack, ISeatID seat) override;
    void OnGenZhuangAck(const TKMahjongGenZhuangAck& ack, ISeatID seat) override;
};
