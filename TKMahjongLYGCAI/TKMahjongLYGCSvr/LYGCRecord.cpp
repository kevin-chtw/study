#include "LYGCRecord.h"
#include "LYGCGame.h"
#include "MJString.h"
#include "MJJson2pb.h"
//
enum ActionID
{
    EAID_GameBegin = 0,
    EAID_BankerPlace = 1,
    EAID_OpenDoor = 2,
    EAID_Draw = 4,
    EAID_Gang = 7,
    EAID_Discard = 8,

    EAID_Peng = 12,

    EAID_NetBreak = 18,
    EAID_NetResume = 19,
    EAID_ExitMatch = 20,
    EAID_PlayerTrust = 23,

    EAID_SendAction = 1001,

    //EAID_INIT_HAND = 100,
    EAID_Hu = 101,
    EAID_MahjongResult = 102,
    EAID_FanCi = 103,
    EAID_GenZhuang = 104,

};

void LYGCRecord::RecordAction(const google::protobuf::Message* ack, ISeatID seat)
{
    if (HasPBRecord())
    {
        this->RecordPBAction(ack, seat);
    }

    this->Dispatch(dynamic_cast<const TKMobileAckMsg*>(ack)->mahjonglygc_ack_msg(), seat);
}

void LYGCRecord::OnGameBeginAck(const TKMahJongGameBeginAck& ack, ISeatID seat)
{
    if (seat > 0)
    {
        return;
    }
    m_curAction.ResetAction(EAID_GameBegin);

    m_curAction.AddProperty("tc", ack.tilecount());
    m_curAction.AddProperty("sc", MJ::ContainerToString(ack.scores()));
}

void LYGCRecord::OnPlaceAck(const TKMahJongPlaceAck& ack, ISeatID seat)
{
    if (seat > 0)
    {
        return;
    }

    if (m_curAction.m_ActionID != EAID_GameBegin)
    {
        MJ_LOG_ERROR(m_Game, "actionId = %d", m_curAction.m_ActionID);
        return;
    }

    m_curAction.AddProperty("o", ack.banker());
    m_curAction.AddProperty("we", ack.roundwind());
    m_curAction.AddProperty("v", GetMJService()->GetVersion());
    m_curAction.AddProperty("lz", ack.lianzhuangtime());
    m_curAction.AddProperty("jr", ack.jsonruler());
    m_curAction.AddProperty("mc", MJ::ContainerToString(ack.micount()));

    this->AddItem(m_curAction);
}

void LYGCRecord::OnOpenDoorAck(const TKMahJongOpenDoorAck& ack, ISeatID seat)
{
    if (seat != ack.seat() && seat >= 0)
    {
        return;
    }

    if (m_curAction.m_ActionID != EAID_OpenDoor)
    {
        m_curAction.ResetAction(EAID_OpenDoor);
    }
    std::string key = "hti" + std::to_string(seat);
    m_curAction.AddProperty(key, MJ::ContainerToString(ack.tiles()));

    if (m_curAction.m_Properties.size() == m_Game->GetIniPlayerCount())
    {
        this->AddItem(m_curAction);
    }
}

void LYGCRecord::OnActionAck(const TKMahJongActionAck& ack, ISeatID seat)
{
    if (seat != ack.seat() && seat >= 0)
    {
        return;
    }

    m_curAction.ResetAction(EAID_SendAction);
    m_curAction.AddProperty("s", ack.seat());
    m_curAction.AddProperty("rtp", ack.requesttype());
    m_curAction.AddProperty("ri", ack.requestid());

    this->AddItem(m_curAction);
}

void LYGCRecord::OnPengAck(const TKMahJongPengAck& ack, ISeatID seat)
{
    if (seat != ack.seat() && seat >= 0)
    {
        return;
    }

    m_curAction.ResetAction(EAID_Peng);

    m_curAction.AddProperty("s", ack.seat());
    m_curAction.AddProperty("ds", ack.discardseat());
    m_curAction.AddProperty("pti", ack.tileid());
    m_curAction.AddProperty("ptl", std::vector<int>(3, ack.tileid()));

    this->AddItem(m_curAction);
}

void LYGCRecord::OnGangAck(const TKMahJongGangAck& ack, ISeatID seat)
{
    if (seat != ack.seat() && seat >= 0)
    {
        return;
    }

    m_curAction.ResetAction(EAID_Gang);
    m_curAction.AddProperty("s", ack.seat());
    m_curAction.AddProperty("ds", ack.discardseat());
    m_curAction.AddProperty("gti", ack.tileid());
    m_curAction.AddProperty("gt", ack.gangtype());
    m_curAction.AddProperty("gtl", std::vector<int>(4, ack.tileid()));

    this->AddItem(m_curAction);
}

void LYGCRecord::OnWinAck(const TKMahJongWinAck& ack, ISeatID seat)
{
    if (seat > 0)
    {
        return;
    }
    m_curAction.ResetAction(EAID_Hu);

    m_curAction.AddProperty("wc", ack.winseatcount());
    m_curAction.AddProperty("ps", ack.paoseat());
    m_curAction.AddProperty("wti", ack.wintileid());
    m_curAction.AddProperty("wm", ack.winmode());
    m_curAction.AddProperty("ws", MJ::ContainerToString(ack.winseats()));

    this->AddItem(m_curAction);
}

void LYGCRecord::OnDiscardTileAck(const TKMahJongDiscardTileAck& ack, ISeatID seat)
{
    if (seat != ack.seat() && seat >= 0)
    {
        return;
    }
    m_curAction.ResetAction(EAID_Discard);

    m_curAction.AddProperty("s", ack.seat());
    m_curAction.AddProperty("dti", ack.tileid());

    this->AddItem(m_curAction);
}

void LYGCRecord::OnDrawTileAck(const TKMahJongDrawTileAck& ack, ISeatID seat)
{
    if (seat != ack.seat() && seat >= 0)
    {
        return;
    }

    m_curAction.ResetAction(EAID_Draw);
    m_curAction.AddProperty("s", ack.seat());
    m_curAction.AddProperty("dti", ack.tileid());
    m_curAction.AddProperty("dtt", ack.drawtiletype());

    this->AddItem(m_curAction);
}

void LYGCRecord::OnTrustPlayAck(const TKMahJongTrustPlayAck& ack, ISeatID seat)
{
    if (seat != ack.seat() && seat >= 0)
    {
        return;
    }
    m_curAction.ResetAction(EAID_PlayerTrust);

    m_curAction.AddProperty("ui", ack.userid());
    m_curAction.AddProperty("tp", ack.type());
    m_curAction.AddProperty("seat", seat);
    this->AddItem(m_curAction);
}

void LYGCRecord::OnResultAck(const TKMahJongResultAck& ack, ISeatID seat)
{
    m_curAction.ResetAction(EAID_MahjongResult);
    m_curAction.AddProperty("totalscore", MJ::ContainerToString(ack.totalscore()));
    m_curAction.AddProperty("showtotalscore", MJ::ContainerToString(ack.showtotalscore()));
    m_curAction.AddProperty("konscore", MJ::ContainerToString(ack.konscore()));
    m_curAction.AddProperty("huscore", MJ::ContainerToString(ack.huscore()));

    const std::string& name = ack.GetDescriptor()->name();
    m_curAction.AddProperty(name, MJ::Base64Encode(MJ::pb2json(ack).toStyledString()));

    this->AddItem(m_curAction);

    if (HasPBRecord())
    {
        std::vector<ISeatID> winners;
        for (int i = 0; i < ack.huscore_size(); ++i)
        {
            if (ack.huscore(i) > 0)
            {
                winners.push_back(i);
            }
        }
        this->RecordPBResult(winners);
    }
}

void LYGCRecord::OnFanCiAck(const TKMahjongFanCiAck& ack, ISeatID seat)
{
    if (seat > 0)
    {
        return;
    }
    m_curAction.ResetAction(EAID_FanCi);
    m_curAction.AddProperty("ct", ack.tile());
    this->AddItem(m_curAction);
}

void LYGCRecord::OnGenZhuangAck(const TKMahjongGenZhuangAck& ack, ISeatID seat)
{
    if (seat > 0)
    {
        return;
    }
    m_curAction.ResetAction(EAID_GenZhuang);

    this->AddItem(m_curAction);
}