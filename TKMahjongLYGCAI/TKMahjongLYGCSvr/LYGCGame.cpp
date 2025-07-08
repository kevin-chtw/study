#include "LYGCGame.h"
#include "State.h"
#include <TKHttpTransProtocol.h>

IMPLEMENT_CREATEDLLSERVER(LYGCService);

LYGCService::LYGCService()
{
}

void LYGCService::InitService()
{
    _InitTilesMap();
}

std::shared_ptr<MJHuBase> LYGCService::OnCreateHuTool()
{
    return std::make_shared<LYGCHu>();
}

const std::map<ITileID, int>& LYGCService::GetTilesMap(std::shared_ptr<MJConfig> conf)
{
    if (conf->GetValue(ECI_RuleFeng) != 2)
    {
        return m_TilesMapFeng;
    }
    return m_TilesMap;
}

void LYGCService::_InitTilesMap()
{
    for (int color = (int)EMJColor::Character; color <= (int)EMJColor::Dragon; ++color)
    {
        auto pointCount = MJPointCountByColor.at(color);
        for (int point = 0; point < pointCount; ++point)
        {
            ITileID tile = MJMakeTile(color, point);

            if (color < (int)EMJColor::Wind)
            {
                m_TilesMap[tile] = 4;
            }
            m_TilesMapFeng[tile] = 4;
        }
    }
}

LYGCGame::LYGCGame() : MJGame(TK_MSG_GAME_MJLYGC)
{
}

void LYGCGame::OnStart()
{
    if (m_iGameHand == 0)
    {
        auto banker = MJ::Random(GetIniPlayerCount());
        m_RoundData.banker = banker;
        m_RoundData.firstBanker = banker;
    }
    else
    {
        LoadLastHandInfo(m_RoundData);
    }

    _CheckCiBeiShuParam();

    m_Messager = std::make_shared<LYGCMessager>(this);
    m_Checker = std::make_shared<MJCheckerOnce>(this, m_Config->GetValue(ECI_ScoreType));
    SetNextState<StateInit>();
}

void LYGCGame::_CheckCiBeiShuParam()
{
    if (m_Config->GetValue(ECI_CiHuBei) < 3 || m_Config->GetValue(ECI_CiHuBei) > 10)
    {
        MJ_LOG_ERROR(this, "ECI_CiHuBei value err =%d ", m_Config->GetValue(ECI_CiHuBei));
    }
    if (m_Config->GetValue(ECI_PiCiBei) < 3 || m_Config->GetValue(ECI_PiCiBei) > 10)
    {
        MJ_LOG_ERROR(this, "ECI_PiCiBei value err =%d ", m_Config->GetValue(ECI_PiCiBei));
    }
    if (m_Config->GetValue(ECI_XiaMiBei) < 3 || m_Config->GetValue(ECI_XiaMiBei) > 10)
    {
        MJ_LOG_ERROR(this, "ECI_XiaMiBei value err =%d ", m_Config->GetValue(ECI_XiaMiBei));
    }
}

std::shared_ptr<LYGCMessager> LYGCGame::GetMessager() const
{
    return (m_Messager);
}
std::shared_ptr<MJCheckerOnce> LYGCGame::GetChecker() const
{
    return (m_Checker);
}

std::shared_ptr<LYGCPlay> LYGCGame::GetPlay() const
{
    return std::dynamic_pointer_cast<LYGCPlay>(m_MJPlay);
}

LYGCLastHandData& LYGCGame::GetRoundData()
{
    return m_RoundData;
}

void LYGCGame::UpdateRoundData()
{
    if (IsValidSeat(GetPlay()->GetNextBanker()))
    {
        m_RoundData.banker = GetPlay()->GetNextBanker();
    }
    else if (GetPlay()->GetHuSeats().size())
    {
        m_RoundData.banker = GetPlay()->GetHuSeats().front();
    }
    if (GetPlay()->GetBanker() == m_RoundData.banker)
    {
        m_RoundData.lianZhuang++;
    }
    else
    {
        m_RoundData.lianZhuang = 0;
    }

    this->SaveLastHandInfo(m_RoundData);
}

void LYGCGame::SendMsg(TKMobileAckMsg& ack, ISeatID seat)
{
    auto ackMsg = ack.mutable_mahjonglygc_ack_msg();
    ackMsg->set_matchid(m_dwMatchID);
    this->SendAckMsg(&ack, seat);
}

void LYGCGame::OnPlayerTrustedChange(ISeatID seat, ETrustType type)
{
    GetMessager()->SendPlayerTrustAck(seat, type);
}

void LYGCGame::OnReqMsg(ISeatID seat, PTKHEADER pMsg)
{
    TKMobileReqMsg reqMsg;
    if (!this->ConvertReqMsg(reqMsg, pMsg))
    {
        MJ_LOG_ERROR(this, "wrong mobile req msg");
        return;
    }
    if (!reqMsg.has_mahjonglygc_req_msg())
    {
        MJ_LOG_ERROR(this, "wrong LYGC req msg");
        return;
    }
    auto& LYGCMsg = reqMsg.mahjonglygc_req_msg();

    if (LYGCMsg.has_tkmahjongtrustplay_req_msg())
    {
        auto& msg = LYGCMsg.tkmahjongtrustplay_req_msg();
        if (msg.type() == (int)ETrustType::Untrust)
        {
            this->GetPlayer(seat)->SetOffline(false);
            this->GetPlayer(seat)->SetTrusted(false);
            GetMessager()->SendPlayerTrustAck(seat, ETrustType::Untrust);
        }
        return;
    }
    m_CurrentState->HandlePlayerMsg(seat, &LYGCMsg);
}

void LYGCGame::OnHttpBrokerAck(BrokerAckData const& data)
{
    m_CurrentState->HandleHttpBrokerMsg(data);
}

int LYGCGame::GetLastHandSaveInfoLen()
{
    return sizeof(LYGCLastHandData);
}

BOOL LYGCGame::PlayerCanLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason)
{
    return pGamePlayer && GetPlayer(pGamePlayer->m_nSeatOrder)->IsOut();
}

void LYGCGame::OnPlayerLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason)
{
    RemoveWatcher(pGamePlayer);
}

std::shared_ptr<MJRecord> LYGCGame::OnCreateRecord()
{
    return std::make_shared<LYGCRecord>(this);
}

std::shared_ptr<MJPlay> LYGCGame::OnCreatePlay()
{
    return std::make_shared<LYGCPlay>(this);
}

std::vector<int> LYGCGame::OnGetDefaultProperties()
{
    return g_DefaultConfig;
}

MJFDConfigMap LYGCGame::OnGetFDTableConfigMap()
{
    return g_FDTableConfigMap;
}

MJDrawConfigs LYGCGame::GetDrawConfigs()
{
    static MJDrawConfigs configs = {
#include "DrawConfigLYGC.inc"
    };
    return configs;
}