#include "TKBotPlayer.h"
#include "TKDllGame.h"
#include <iostream>

CTKBotPlayer::CTKBotPlayer()
{
}

CTKBotPlayer::~CTKBotPlayer()
{
}

BOOL CTKBotPlayer::OnInitialUpdate()
{
    return TRUE;
}

BOOL CTKBotPlayer::OnMsg(PTKHEADER pMsg)
{
    return FALSE;
}

BOOL CTKBotPlayer::OnTickCount()
{
    return TRUE;
}

void CTKBotPlayer::SetThinkTickCount(int nTickCount)
{
}

BOOL CTKBotPlayer::SendMsg(PTKHEADER pMsg)
{
    if (TKID_MSG_AI_WEB_SERVER == pMsg->dwType)
    {
        PTKMSGAIWEBSERVER msg = (PTKMSGAIWEBSERVER)(pMsg);
        size_t szLen = pMsg->dwLength + sizeof(TKHEADER);
        std::string szSerializeData((BYTE*)msg + sizeof(TKMSGAIWEBSERVER), (BYTE*)msg + szLen);
        m_wOrigine = msg->header.wOrigine;
        m_dwSerial = msg->header.dwSerial;
        m_msgType = msg->msgType;
        m_AIRequest = szSerializeData;
    }
    else
    {
        std::vector<BYTE> buffer((BYTE*)pMsg, (BYTE*)pMsg + sizeof(TKHEADER) + pMsg->dwLength);
        m_Requests.push_back(buffer);
    }
    return TRUE;
}

void CTKBotPlayer::Terminate(BOOL bWhenSendOver /*= FALSE*/)
{
}

std::string CTKBotPlayer::__GetHands()
{
    return "";
}

void CTKBotPlayer::__InitGame(CTKGame* game, int seat)
{
    m_Game = game;
    m_Seat = seat;

    _InitEnterRound();
    _InitBeginHand();
}

void CTKBotPlayer::_InitEnterRound()
{
    TKACKGAMEPLAYERENTER ack{};
    ack.header.dwType = TK_ACK | TK_MSG_GAME_ENTERROUND;
    ack.dwUserID = m_dwBotUserID;
    ack.nSeatOrder = m_Seat;
    OnMsg((PTKHEADER)&ack);
}

void CTKBotPlayer::_InitBeginHand()
{
    TKACKGAMEBEGINHAND ack{};
    ack.header.dwType = TK_ACK | TK_MSG_GAME_BEGINHAND;
    OnMsg((PTKHEADER)&ack);
}

mvs::attribute* CTKBotPlayer::GetMatchPrivateAttribute(DWORD dwAttributeID)
{
    return nullptr;
}

mvs::attribute* CTKBotPlayer::GetPlayerPrivateAttribute(DWORD dwAttributeID)
{
    return nullptr;
}

bool CTKBotPlayer::__GetAiRequest(std::string& req)
{
    if (m_AIRequest.empty())
    {
        return false;
    }

    req = m_AIRequest;
    m_AIRequest.clear();
    return true;
}

bool CTKBotPlayer::__HandAction(const std::string& res)
{
    std::vector<BYTE> data(sizeof(TKMSGAIWEBSERVER) + res.size());
    auto msg = (PTKMSGAIWEBSERVER)data.data();

    msg->header.dwType = TKID_MSG_AI_WEB_SERVER;
    msg->header.wOrigine = m_wOrigine;
    msg->header.dwSerial = m_dwSerial;
    msg->header.dwLength = res.size() + sizeof(TKMSGAIWEBSERVER) - sizeof(TKHEADER);

    msg->msgType = m_msgType;
    msg->dataLength = res.size();
    msg->offset = sizeof(TKMSGAIWEBSERVER) - sizeof(TKHEADER);
    memcpy((BYTE*)msg + sizeof(TKMSGAIWEBSERVER), res.c_str(), res.size());

    if (!OnMsg((PTKHEADER)data.data()))
    {
        return false;
    }
    while (m_Requests.empty() && OnTickCount())
    {
    }
    if (m_Requests.empty()) // bot收到ai消息，却没发给游戏服消息，说明该ai消息有问题
    {
        return false;
    }
    std::vector<std::vector<BYTE>> reqs = m_Requests;
    m_Requests.clear();
    for (auto& r : reqs)
    {
        m_Game->OnPlayerMsg(m_dwBotUserID, m_Seat, (PTKHEADER)r.data());
    }
    return true;
}
