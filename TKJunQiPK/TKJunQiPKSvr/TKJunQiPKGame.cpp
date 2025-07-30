#include "StdAfx.h"
#include "TKJunQiPKGame.h"
#include "TKDllPlayer.h"
#include "TKGameTipMessageDefine.h"

CTKJunQiPKGame::CTKJunQiPKGame(void)
{
}

CTKJunQiPKGame::~CTKJunQiPKGame(void)
{
}

BOOL CTKJunQiPKGame::OnGameBegin()
{
    m_pJunQiPKData = std::make_shared<CTKJunQiPKData>();
    m_pJunQiPKData->OnGameBegin(this, m_pRuler->szPropertyEx);
    SetGameTimer(Max_Round_Timmer, 0, Max_Round_Time);
    return TRUE;
}

BOOL CTKJunQiPKGame::OnPlayerMsg(DWORD userID, int seatOrder, PTKHEADER pMsg)
{
    switch (pMsg->dwType)
    {
        // 过滤淘汰提示
    case (TK_REQ | TKID_MS2GS_USERTIPMESSAGE): {
        PTKREQUSERTIPMESSAGE pReq = (PTKREQUSERTIPMESSAGE)pMsg;
        if (TKID_MATCH_TIP_PKSTAGE_1 == pReq->nTipID)
        {
            pReq->byShowType = TK_TIPMESSAGETYPE_MATCHTEXTPLAY;
        }
        break;
    }
    default:
        break;
    }

    if (pMsg->dwType != (TK_REQ | TK_MSG_GAME_JUNQIPK))
    {
        return FALSE;
    }

    try
    {
        size_t szLen = pMsg->dwLength + sizeof(TKHEADER);
        std::string szSerializeData((BYTE*)pMsg + sizeof(TKHEADER), (BYTE*)pMsg + szLen);
        TKMobileReqMsg mobileReqMsg;
        mobileReqMsg.ParseFromString(szSerializeData);  // 反序列化
        const JunQiPKReqMsg& reqMsg = mobileReqMsg.junqipk_req_msg();

        return m_pJunQiPKData->OnPlayerMsg(seatOrder, reqMsg);
    }
    catch (std::exception& e)
    {
        LOG_ERROR("exception: %s", e.what());
    }
    catch (...)
    {
        LOG_ERROR("unknow exception");
    }
    return TRUE;
}

void CTKJunQiPKGame::OnGameTimer(DWORD dwID, ULONG_PTR ulUserData)
{
    m_pJunQiPKData->OnTimeOut(dwID, ulUserData);
}

int CTKJunQiPKGame::GetLastHandSaveInfoLen()
{
    return 0;
}

void CTKJunQiPKGame::OnPlayerNetBreak(CTKGamePlayer* pGamePlayer)
{
    m_pJunQiPKData->OnPlayerNetBreak(pGamePlayer->m_nSeatOrder);
}

void CTKJunQiPKGame::OnPlayerNetResume(CTKGamePlayer* pGamePlayer)
{
    m_pJunQiPKData->OnPlayerNetResume(pGamePlayer->m_nSeatOrder);
}

void CTKJunQiPKGame::OnPlayerExitMatch(CTKGamePlayer* pGamePlayer)
{
}

void CTKJunQiPKGame::SendMsg(TKMobileAckMsg& rMsg, int nSeat)
{
    JunQiPKAckMsg* pAckMsg = rMsg.mutable_junqipk_ack_msg();

    pAckMsg->set_matchid(m_dwMatchID);
    pAckMsg->set_severtime(time(NULL));
    // 序列化
    std::string szSerializeMsg;
    rMsg.SerializeToString(&szSerializeMsg);

    // 发送
    BYTE* pszPack = new BYTE[sizeof(TKHEADER) + rMsg.ByteSize()];  // 分配空间
    ZeroMemory(pszPack, sizeof(TKHEADER) + rMsg.ByteSize());

    ((PTKHEADER)pszPack)->dwType = TK_ACK | TK_MSG_GAME_JUNQIPK;
    ((PTKHEADER)pszPack)->dwLength = rMsg.ByteSize();  // size
    BYTE* pData = pszPack + sizeof(TKHEADER);
    memcpy(pData, szSerializeMsg.c_str(), rMsg.ByteSize());

    if (JUNQIPK_UNKNOWN != nSeat)
    {
        Send2SeatPlayer(nSeat, (PTKHEADER)pszPack);
    }
    else
    {
        Broadcast((PTKHEADER)pszPack);
    }
    delete[] pszPack;
    pszPack = NULL;
}
