#include "StdAfx.h"
#include "TKJunQiPKBot.h"
#include <boost/algorithm/string.hpp>
#include "json.h"
#include <boost/lexical_cast.hpp>

#define BOT_AI_LEVEL_1 1
#define BOT_AI_LEVEL_99 99
#define BOT_AI_LEVEL_98 98
#define BOT_AI_LEVEL_97 97

#define AI_SERVER_MSG_TYPE_ACTION 1
//#define DEFAULT_ACTION_TIME  "15,2|30,3|25,4|15,5|8,6|4,7|3,8"
#define DEFAULT_ACTION_TIME "45,1|45,2|10,3"
IMPLEMENT_CREATEBOTPLAYER(CTKJunQiPKBot);

CTKJunQiPKBot::CTKJunQiPKBot(void) : m_MySeat(JUNQIPK_UNKNOWN)
{
    ReSet();
}

CTKJunQiPKBot::~CTKJunQiPKBot(void)
{
}

void CTKJunQiPKBot::ReSet()
{
    m_pJunQiPKAI = boost::make_shared<CTKJunQiPKBotAI>();
    m_MyColor = -1;
    m_MySelectPosition = -1;
    m_MyShowChessPosition = -1;
    m_RequestPeaceSeat = -1;
    m_IsAgreePeace = 0;
    InitAiTime();
    m_DelayPlayChess = DEFAULT_ACK;
#if _DEBUG
    m_bPacerBot = true;
#endif
}
BOOL CTKJunQiPKBot::OnInitialUpdate()
{
    if (!__super::OnInitialUpdate())
        return FALSE;

    ReSet();
    return TRUE;
}

BOOL CTKJunQiPKBot::OnTickCount()
{
    switch (m_GameSchedule)
    {
    case TKJUNQI_SCHEDULE_INITLAYOUT: {
        SendMsgInitLayout();
    }
    break;
    case TKJUNQI_SCHEDULE_SELECTCHESS: {
        SendMsgSelectChess(m_MySelectPosition);
    }
    break;
    case TKJUNQI_SCHEDULE_MOVECHESS: {
        SendMsgMoveChess(m_MySelectPosition, m_MyMoveToPosition);
    }
    break;
    case TKJUNQI_SCHEDULE_ACTIONFINISH: {
        SendMsgAcotionFinish();
    }
    break;
    case TKJUNQI_SCHEDULE_CONCEDE: {
    }
    break;
    case TKJUNQI_SCHEDULE_PEACE: {
    }
    break;
    case TKJUNQI_SCHEDULE_PEACERES: {
        SendMsgPeaceRes();
    }
    break;
    case TKJUNQI_SCHEDULE_SHOWCHESS: {
        SendMsgShowChess(m_MyShowChessPosition);
    }
    break;
    case TKJUNQI_SCHEDULE_INITRULE: {
    }
    break;
    case TKJUNQI_SCHEDULE_CANCELLAYOUT: {
    }
    break;
    }
    return TRUE;
}

BOOL CTKJunQiPKBot::OnMsg(PTKHEADER pMsg)
{
    if (__super::OnMsg(pMsg))
        return TRUE;
    switch (pMsg->dwType)
    {
    case TK_ACK | TK_MSG_GAME_ENTERROUND: {
        OnAckEnterRound(pMsg);
    }
    break;
    case TK_ACK | TK_MSG_GAME_RULERINFO: {
        OnAckGameRulerInfo(pMsg);
    }
    break;
    case TK_ACK | TK_MSG_GAME_RULEREXINFO: {
        OnAckGameRulerExInfo(pMsg);
    }
    break;
    case TK_ACK | TK_MSG_GAME_ADDGAMEPLAYERINFO: {
        OnAckAddPlayerInfo(pMsg);
    }
    break;
    case TK_ACK | TK_MSG_GAME_JUNQIPK: {
        OnAckPbMsg(pMsg);
        break;
    }
    case TKID_MSG_AI_WEB_SERVER: {
        OnAckAIServerMsg(pMsg);
    }
    default:
        break;
    }
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckEnterRound(PTKHEADER pMsg)
{
    PTKACKGAMEPLAYERENTER pAck = (PTKACKGAMEPLAYERENTER)pMsg;
    if (m_pJunQiPKAI)
    {
        m_pJunQiPKAI->ReSet();
    }

    m_MyColor = -1;
    m_MySeat = -1;
    m_seatLastColor.clear();
    m_seatColor.clear();

    m_MySeat = pAck->nSeatOrder;

    if (m_pJunQiPKAI)
    {
        m_pJunQiPKAI->SetTraceID(m_dwBotUserID);
    }
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckGameRulerInfo(PTKHEADER pMsg)
{
    PTKACKGAMERULERINFO pAck = (PTKACKGAMERULERINFO)pMsg;
    m_TotalPlayerCount = pAck->nGameTotalPlayer;
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckGameRulerExInfo(PTKHEADER pMsg)
{
    PTKACKGAMERULEREXINFO pAck = (PTKACKGAMERULEREXINFO)pMsg;
    m_GameType = (EGameType)strtoll(pAck->szPropertyEx, nullptr, 10);
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckAddPlayerInfo(PTKHEADER pMsg)
{
    PTKACKGAMEADDPLAYERENTERINFO pAck = (PTKACKGAMEADDPLAYERENTERINFO)pMsg;
    //#ifdef _DEBUG
    TKWriteLog("Bot(%d) Recv : AddGamePlayer, UserID=%d, SeatOrder=%d, NickName=%s, Arrived=%d, NetStatus=%d",
               m_dwBotUserID, pAck->info.dwUserID, pAck->info.nSeatOrder, pAck->info.szNickName, pAck->info.bArrived,
               pAck->info.nNetStatus);
    //#endif

    if (pAck->info.nSeatOrder < 0 || pAck->info.nSeatOrder >= MAX_PLAYER_COUNT)
    {
        TKWriteLog("OnAckAddPlayerInfo Error, pAck->info.nSeatOrder = %d", pAck->info.nSeatOrder);
        return FALSE;
    }

    m_AllUserID[pAck->info.nSeatOrder] = pAck->info.dwUserID;

    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckAIServerMsg(PTKHEADER pMsg)
{
    PTKMSGAIWEBSERVER msg = (PTKMSGAIWEBSERVER)pMsg;

    if (msg->msgType == AI_SERVER_MSG_TYPE_ACTION && m_pJunQiPKAI)
    {
        WebSeverMsgAck(pMsg);
    }
    return true;
}

BOOL CTKJunQiPKBot::OnAckPbMsg(PTKHEADER pMsg)
{
    if (!m_bPacerBot)
    {  // 玩家id机器人不再处理game msg，转由游戏服务超时处理。
        return TRUE;
    }

    if (EGameType::GT_FANFANQI != m_GameType)
    {
        return FALSE;
    }

    size_t szLen = pMsg->dwLength + sizeof(TKHEADER);
    std::string szSerializeData((BYTE*)pMsg + sizeof(TKHEADER), (BYTE*)pMsg + szLen);
    TKMobileAckMsg mobileAckMsg;
    mobileAckMsg.ParseFromString(szSerializeData);
    const JunQiPKAckMsg& ackMsg = mobileAckMsg.junqipk_ack_msg();
    // TKWriteLog("OnAckPbMsg:%s", ackMsg.DebugString().c_str());
    if (ackMsg.has_initchess_ack_msg())
    {
        OnAckJunQiPKInitChess(ackMsg.initchess_ack_msg());
    }
    if (ackMsg.has_startmovechess_ack_msg())
    {
        OnAckJunQiPKStartMoveChess(ackMsg.startmovechess_ack_msg());
    }
    if (ackMsg.has_selectchess_ack_msg())
    {
        OnAckJunQiPKSelectChess(ackMsg.selectchess_ack_msg());
    }
    if (ackMsg.has_movechess_ack_msg())
    {
        OnAckJunQiPKMoveChess(ackMsg.movechess_ack_msg());
    }
    if (ackMsg.has_actionfinish_ack_msg())
    {
        OnAckJunQiPKActionFinish(ackMsg.actionfinish_ack_msg());
    }
    if (ackMsg.has_peace_ack_msg())
    {
        OnAckPeace(ackMsg.peace_ack_msg());
    }
    if (ackMsg.has_finishgame_ack_msg())
    {
        OnAckFinishGame(ackMsg.finishgame_ack_msg());
    }
    if (ackMsg.has_showchess_ack_msg())
    {
        OnAckJunQiPKShowChess(ackMsg.showchess_ack_msg());
    }
    if (ackMsg.has_refusepeace_ack_msg())
    {
        OnAckJunQiPKRefusePeace(ackMsg.refusepeace_ack_msg());
    }
    if (ackMsg.has_repeatcount_ack_msg())
    {
        OnAckJunQiPKRepeatCount(ackMsg.repeatcount_ack_msg());
    }
    if (ackMsg.has_timeout_ack_msg())
    {
    }
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckJunQiPKInitChess(const InitChessAck& rAck)
{
    for (int i = 0; i < rAck.pos_size(); ++i)
    {
        int dice = rAck.pos(i);
    }
    if (m_pJunQiPKAI)
    {
        m_pJunQiPKAI->SetDiceNum(rAck.pos_size());
        m_pJunQiPKAI->ReSet();
    }
    SendMsgInitLayout();

    if (m_pJunQiPKAI)
    {
        m_pJunQiPKAI->SetRecordID(m_AllUserID[0], m_AllUserID[1], m_dwMatchID, m_MySeat);
        m_pJunQiPKAI->SetTraceID(m_dwBotUserID);
        m_pJunQiPKAI->SetProductId(m_dwProductID);
    }

    return TRUE;
}
//开始走棋
BOOL CTKJunQiPKBot::OnAckJunQiPKStartMoveChess(const StartMoveChessAck& rAck)
{
    if (rAck.seat() == m_MySeat)
    {
        PlayChess();
    }
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckJunQiPKSelectChess(const SelectChessAck& rAck)
{
    if (1 == m_pJunQiPKAI->GetPeace())
    {
        m_DelayPlayChess = MOVECHESS_ACK;
        // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
        return TRUE;
    }

    m_GameSchedule = TKJUNQI_SCHEDULE_MOVECHESS;
    // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
    SetThinkTickCount(GetRandomTime());
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckJunQiPKMoveChess(const MoveChessAck& rAck)
{
    if (!m_pJunQiPKAI)
    {
        return FALSE;
    }
    if (1 == m_pJunQiPKAI->GetPeace())
    {
        m_moveChessAck = rAck;
        m_DelayPlayChess = MOVECHESS_ACK;
        //TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
        return TRUE;
    }

    for (int i = 0; i < rAck.path_size(); ++i)
    {
        int pathIndex = rAck.path(i);
    }

    std::stringstream history;
    history << "1|" << rAck.chesstype() << "|";
    for (size_t i = 0; i < rAck.path_size(); i++)
    {
        if (i > 0 && i < rAck.path_size())
        {
            history << ",";
        }
        history << rAck.path(i);
    }
    if (rAck.chesstype() == -1)
    {
        history << "|" << rAck.seat() << "|" << rAck.result() << "|" << -1;
    }
    else
    {
        history << "|" << rAck.seat() << "|" << rAck.result() << "|" << m_seatColor[rAck.seat()];
    }
    //TKWriteLog("AddHistroy %s", history.str().c_str());
    m_pJunQiPKAI->AddHistroy(history.str());
    m_pJunQiPKAI->SetLastActionSeat(rAck.seat());

    m_GameSchedule = TKJUNQI_SCHEDULE_ACTIONFINISH;
    SetThinkTickCount(GetRandomTime());
    return TRUE;
}
void CTKJunQiPKBot::InitAiTime()
{
    int nInitCount = 0;
    char szKey[32] = {0};
    char szBuffer[1024] = {0};
    memset(szBuffer, 0, sizeof(char) * 1024);
    GetPrivateProfileString(AiIniCh, "ActionTime", DEFAULT_ACTION_TIME, szBuffer, sizeof(char) * 1024, AiIniFile);
    vector<string> vStr;
    vector<string> vStr_s;
    int nSumPro = 0;
    boost::split(vStr, szBuffer, boost::is_any_of("|"));
    for (int i = 0; i < vStr.size(); ++i)
    {
        vStr_s.clear();
        boost::split(vStr_s, vStr[i], boost::is_any_of(","));
        if (vStr_s.size() == 2)
        {
            STime _sTime;
            _sTime.nProb = atof(vStr_s.front().c_str()) * 100;
            _sTime.nTime = atof(vStr_s.back().c_str()) * 1000;
            m_vActTime.push_back(_sTime);
            nSumPro += _sTime.nProb;
        }
    }
    if (m_vActTime.size() == 0 || nSumPro == 0)
    {
        m_vActTime.clear();
        memset(szBuffer, 0, sizeof(char) * 1024);
        memcpy(szBuffer, DEFAULT_AI_ACTTIME, std::string(DEFAULT_AI_ACTTIME).length() * sizeof(char));
        vector<string> vStr;
        vector<string> vStr_s;
        boost::split(vStr, szBuffer, boost::is_any_of("|"));
        for (int i = 0; i < vStr.size(); ++i)
        {
            vStr_s.clear();
            boost::split(vStr_s, vStr[i], boost::is_any_of(","));
            if (vStr_s.size() == 2)
            {
                STime _sTime;
                _sTime.nProb = atof(vStr_s.front().c_str()) * 100;
                _sTime.nTime = atof(vStr_s.back().c_str()) * 1000;
                m_vActTime.push_back(_sTime);
            }
        }
    }
}
int CTKJunQiPKBot::GetRandomTime()
{
    if (m_vActTime.size() != 0)
    {
        int nActProbSum = 0;
        int nCurNum = 0;
        for each (auto iter in m_vActTime)
        {
            nActProbSum += iter.nProb;
        }

        int nRandAct = TKGenRandom() % nActProbSum;
        for (int i = 0; i < m_vActTime.size(); ++i)
        {
            nCurNum += m_vActTime[i].nProb;
            if (nRandAct < nCurNum)
            {
                return m_vActTime[i].nTime;
            }
        }
    }
    return 0;
}
BOOL CTKJunQiPKBot::OnAckJunQiPKShowChess(const ShowChessAck& rAck)
{
    if (!m_pJunQiPKAI)
    {
        return FALSE;
    }

    if (1 == m_pJunQiPKAI->GetPeace())
    {
        m_showChessAck = rAck;
        m_DelayPlayChess = SHOWCHESS_ACK;
        // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
        return TRUE;
    }

    Chess chess = rAck.chess();
    int pos = chess.pos();
    EChessType chessType = EChessType(chess.type());
    int chessColor = chess.color();

    if (m_MyColor == -1)
    {
        std::map<int, int>::iterator it = m_seatLastColor.find(chess.seat());
        if (it == m_seatLastColor.end())
        {
            m_seatLastColor.insert(std::make_pair(chess.seat(), chessColor));
        }
        else
        {
            if (chess.seat() == m_MySeat && chessColor == it->second)
            {
                m_MyColor = chessColor;
                m_seatColor.insert(std::make_pair(m_MySeat, m_MyColor));
                int otherSeat = (m_MySeat == 0 ? 1 : 0);
                int otherColor = (m_MyColor == EC_Red ? EC_Blue : EC_Red);
                m_seatColor.insert(std::make_pair(otherSeat, otherColor));
            }
            else if (chess.seat() == m_MySeat)
            {
                it->second = chessColor;
            }
            else if (chessColor == it->second && it->first == chess.seat())
            {
                m_MyColor = (chessColor == EC_Red ? EC_Blue : EC_Red);
                int otherSeat = (m_MySeat == 0 ? 1 : 0);
                int otherColor = (m_MyColor == EC_Red ? EC_Blue : EC_Red);
                m_seatColor.insert(std::make_pair(m_MySeat, m_MyColor));
                m_seatColor.insert(std::make_pair(otherSeat, otherColor));
            }
            else
            {
                it->second = chessColor;
            }
        }
    }

    m_pJunQiPKAI->DecDiceNum();
    int color = -1;
    if (m_seatColor.find(chess.seat()) != m_seatColor.end())
    {
        color = m_seatColor[chess.seat()];
    }
    std::stringstream history;
    history << "10"
            << "|" << chessColor << "|" << (int)chessType << "|" << pos << "|" << chess.seat() << "|" << color;
    //TKWriteLog("AddHistroy %s", history.str().c_str());
    m_pJunQiPKAI->AddHistroy(history.str());
    m_pJunQiPKAI->SetLastActionSeat(chess.seat());

    m_GameSchedule = TKJUNQI_SCHEDULE_ACTIONFINISH;
    SetThinkTickCount(GetRandomTime());
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckJunQiPKActionFinish(const ActionFinishAck& rAck)
{
    int nextSeat = rAck.nextseat();
    for (int i = 0; i < rAck.chess_size(); ++i)
    {
        int chessPos = rAck.chess(i).pos();
        int chessType = rAck.chess(i).type();
        int color = rAck.chess(i).color();
    }
    if (nextSeat == m_MySeat)
    {
        if (1 == m_pJunQiPKAI->GetPeace())
        {
            // m_bDelayPlayChess = true;
            // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
        }
        else
        {
            PlayChess();
            // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
        }
    }
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckJunQiPKRefusePeace(const RefusePeaceAck& rAck)
{
    // TKWriteLog("OnAckJunQiPKRefusePeace %d", rAck.seat());
    //自己求和被拒绝，出牌
    m_pJunQiPKAI->SetRePeace(0);
    if (rAck.seat() == m_MySeat)
    {
        PlayChess();
    }
    // m_enGameSchedule = TKJUNQI_SCHEDULE_ACTIONFINISH;
    // SetThinkTickCount(1);
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckJunQiPKRepeatCount(const RepeatCountAck& rAck)
{
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckFinishGame(const FinishGameAck& rAck)
{
    TKWriteLog("OnAckFinishGame:");
    for (int i = 0; i < rAck.result_size(); i++)
    {
        TKWriteLog("seat = %d result =  %d", i, rAck.result(i));
    }
    m_MyColor = JUNQIPK_UNKNOWN;
    m_MySelectPosition = JUNQIPK_UNKNOWN;
    m_MyShowChessPosition = JUNQIPK_UNKNOWN;
    m_RequestPeaceSeat = JUNQIPK_UNKNOWN;
    return TRUE;
}

BOOL CTKJunQiPKBot::OnAckPeace(const PeaceAck& rAck)
{
    m_RequestPeaceSeat = rAck.seat();
    if (m_RequestPeaceSeat != m_MySeat && m_pJunQiPKAI)
    {
        m_pJunQiPKAI->SetRePeace(1);
        SendMsgToAi();

        // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
    }
    return TRUE;
}

void CTKJunQiPKBot::SendReqMsg(const TKMobileReqMsg& rMsg)
{
    // TKWriteLog("SendReqMsg:%s", rMsg.DebugString().c_str());
    std::string szSerializeMsg;
    if (!rMsg.SerializeToString(&szSerializeMsg))
    {
        return;
        // assert(false);
    }
    BYTE* pszPack = new BYTE[sizeof(TKHEADER) + rMsg.ByteSize()];
    ZeroMemory(pszPack, sizeof(TKHEADER) + rMsg.ByteSize());
    ((PTKHEADER)pszPack)->dwType = TK_MSG_GAME_JUNQIPK;
    ((PTKHEADER)pszPack)->dwLength = rMsg.ByteSize();
    BYTE* pData = pszPack + sizeof(TKHEADER);
    memcpy(pData, szSerializeMsg.c_str(), rMsg.ByteSize());
    SendMsg((PTKHEADER)pszPack);
    delete[] pszPack;
    pszPack = NULL;
}

BOOL CTKJunQiPKBot::SendMsgInitLayout()
{
    TKMobileReqMsg mobilemsg;
    JunQiPKReqMsg* pMsg = mobilemsg.mutable_junqipk_req_msg();
    pMsg->set_matchid(m_dwMatchID);
    InitLayoutReq* pReq = pMsg->mutable_initlayout_req_msg();
    pReq->set_seat(m_MySeat);
    pReq->set_headimg(1);
    SendReqMsg(mobilemsg);
    return TRUE;
}

BOOL CTKJunQiPKBot::SendMsgSelectChess(UINT pos)
{
    TKMobileReqMsg mobilemsg;
    JunQiPKReqMsg* pMsg = mobilemsg.mutable_junqipk_req_msg();
    pMsg->set_matchid(m_dwMatchID);
    SelectChessReq* pReq = pMsg->mutable_selectchess_req_msg();
    pReq->set_seat(m_MySeat);
    pReq->set_pos(pos);
    m_MySelectPosition = pos;
    SendReqMsg(mobilemsg);
    return TRUE;
}

BOOL CTKJunQiPKBot::SendMsgShowChess(UINT pos)
{
    TKMobileReqMsg mobilemsg;
    JunQiPKReqMsg* pMsg = mobilemsg.mutable_junqipk_req_msg();
    pMsg->set_matchid(m_dwMatchID);
    ShowChessReq* pReq = pMsg->mutable_showchess_req_msg();
    pReq->set_atpos(pos);
    SendReqMsg(mobilemsg);
    return TRUE;
}

BOOL CTKJunQiPKBot::SendMsgAcotionFinish()
{
    TKMobileReqMsg mobilemsg;
    JunQiPKReqMsg* pMsg = mobilemsg.mutable_junqipk_req_msg();
    pMsg->set_matchid(m_dwMatchID);
    AcotionFinishReq* pReq = pMsg->mutable_actionfinish_req_msg();
    pReq->set_seat(m_MySeat);
    SendReqMsg(mobilemsg);
    return TRUE;
}

BOOL CTKJunQiPKBot::SendMsgMoveChess(UINT startPos, UINT endPos)
{
    TKMobileReqMsg mobilemsg;
    JunQiPKReqMsg* pMsg = mobilemsg.mutable_junqipk_req_msg();
    pMsg->set_matchid(m_dwMatchID);
    MoveChessReq* pReq = pMsg->mutable_movechess_req_msg();
    pReq->set_seat(m_MySeat);
    pReq->set_atpos(startPos);
    pReq->set_topos(endPos);
    pReq->set_isconfirm(1);
    m_MySelectPosition = JUNQIPK_UNKNOWN;
    SendReqMsg(mobilemsg);
    return TRUE;
}

BOOL CTKJunQiPKBot::SendMsgPeaceRes()
{
    TKMobileReqMsg mobilemsg;
    JunQiPKReqMsg* pMsg = mobilemsg.mutable_junqipk_req_msg();
    pMsg->set_matchid(m_dwMatchID);
    PeaceResReq* pReq = pMsg->mutable_peaceres_req_msg();
    pReq->set_seat(m_RequestPeaceSeat);
    pReq->set_agree(m_IsAgreePeace);
    m_RequestPeaceSeat = JUNQIPK_UNKNOWN;
    SendReqMsg(mobilemsg);
    if (m_pJunQiPKAI)
    {
        m_pJunQiPKAI->SetRePeace(0);
    }
    // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);

    if (m_DelayPlayChess != DEFAULT_ACK)
    {
        if (MOVECHESS_ACK == m_DelayPlayChess)
        {
            SelectChessAck rAck;
            OnAckJunQiPKSelectChess(rAck);
            OnAckJunQiPKMoveChess(m_moveChessAck);
        }
        else if (SHOWCHESS_ACK == m_DelayPlayChess)
        {
            OnAckJunQiPKShowChess(m_showChessAck);
        }
        m_DelayPlayChess = DEFAULT_ACK;
        PlayChess();
        // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
    }
    return TRUE;
}
void CTKJunQiPKBot::InitAISvrMsg(PTKHEADER msg, int nMinThinkTime)
{
    PTKMSGAIWEBSERVER pMsg = (PTKMSGAIWEBSERVER)msg;
    pMsg->header.dwType = TKID_MSG_AI_WEB_SERVER;
    pMsg->dwUserID = m_dwBotUserID;
    pMsg->timeStamp = GetTickCount();
    pMsg->minThinkTime = nMinThinkTime;
    pMsg->offset = sizeof(TKMSGAIWEBSERVER) - TKHEADERSIZE;
    pMsg->dataLength = pMsg->header.dwLength;
    pMsg->header.dwLength += pMsg->offset;

    int nPort = 0;
    string strIP;
    if (m_pJunQiPKAI)
    {
        m_pJunQiPKAI->GetIpPort(strIP, nPort);
    }

    memset(pMsg->szServerName, 0, sizeof(pMsg->szServerName));
    memset(pMsg->szServerIP, 0, sizeof(pMsg->szServerIP));
    memcpy(pMsg->szServerIP, strIP.c_str(), strIP.length());
    pMsg->port = nPort;
}
BOOL CTKJunQiPKBot::SendMsgToAi()
{
    if (!m_pJunQiPKAI)
    {
        return false;
    }
    CTKBuffer buf;
    buf.SetHeaderLen(sizeof(TKMSGAIWEBSERVER));
    m_pJunQiPKAI->CreateWebServerMsg(buf, m_byAIType);
    PTKMSGAIWEBSERVER pMsg = (PTKMSGAIWEBSERVER)buf.GetBufPtr();

    InitAISvrMsg((PTKHEADER)pMsg, GetTickCount() % 2000);

    pMsg->header.wOrigine = BOT_AI_LEVEL_99;
    pMsg->msgType = AI_SERVER_MSG_TYPE_ACTION;
    // TKWriteLog("send to ai  %s", buf.GetDataPtr());

    if (!SendMsg((PTKHEADER)buf.GetBufPtr()))
    {
        TKWriteLog("fail send ai web msg");
        return false;
    }
    return true;
}
BOOL CTKJunQiPKBot::PlayChess()
{
    return SendMsgToAi();
}
int GetHttpAckCode(std::string& data)
{
    std::string flag = " ";
    size_t pos = data.find(flag);
    if (pos != std::string::npos)
    {
        std::string scoreStr = data.substr(pos + flag.length());
        size_t length = scoreStr.find(" ");
        if (pos != std::string::npos)
        {
            scoreStr = scoreStr.substr(0, length);
            int s = boost::lexical_cast<int>(scoreStr.c_str());
            return s;
        }
    }
    return 0;
}

int GetHttpAckBody(std::string& data, std::string& body)
{
    char headOverFlag[256] = {"\r\n\r\n"};
    std::string str = data;
    std::string::size_type pos = str.find(headOverFlag);
    if (pos != str.npos)
    {
        body = str.substr(pos + std::string(headOverFlag).length());
        pos = body.rfind("\n");
        if (pos != body.npos)
        {
            body.erase(pos);
        }
    }
    return body.size();
}

bool CTKJunQiPKBot::WebSeverMsgAck(PTKHEADER pMsg)
{
    PTKMSGAIWEBSERVER msg = (PTKMSGAIWEBSERVER)pMsg;
    if ((m_pJunQiPKAI->GetSerialNum() != pMsg->dwSerial))
    {
        std::string retStr = (char*)(msg) + TKHEADERSIZE + msg->offset;
        TKWriteLog("CTKJunQiPKBotAI::WebSeverMsgAck error, m_nSerialNum:%d, pMsg->dwSerial: %d, recv:%s, \n",
                   m_pJunQiPKAI->GetSerialNum(), pMsg->dwSerial, retStr.c_str());
        return false;
    }

    std::string retStr = (char*)(msg) + TKHEADERSIZE + msg->offset;

    DEBUG_LOG("ONAIRETURN:%s", retStr.c_str());

    int nAckCode = GetHttpAckCode(retStr);
    if (nAckCode == 200)
    {
        std::string body;
        GetHttpAckBody(retStr, body);
        Json::Reader reader;
        Json::Value responseJson;
        if (!reader.parse(body, responseJson))
        {
            TKWriteLog("Http Post Failed: parse response failed.\n%s", retStr.c_str());
            return false;
        }
        if (responseJson["type"].isInt())
        {
            int type = responseJson["type"].asInt();
            if (type == 10)  //翻棋
            {
                if (responseJson["move"].isString())
                {
                    std::string pos = responseJson["move"].asString();
                    m_MyShowChessPosition = boost::lexical_cast<int>(pos.c_str());
                    m_GameSchedule = TKJUNQI_SCHEDULE_SHOWCHESS;
                    SetThinkTickCount(GetRandomTime());
                    // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
                }
            }
            else  //出棋
                if (type == 1)
                {
                    if (responseJson["move"].isString())
                    {
                        std::string moveString = responseJson["move"].asString();
                        std::string::size_type pos = moveString.find(",");
                        int startPos = -1, endPos = -1;
                        if (pos != moveString.npos)
                        {
                            startPos = boost::lexical_cast<int>(moveString.substr(0, pos).c_str());
                            endPos = boost::lexical_cast<int>(moveString.substr(pos + 1).c_str());
                        }
                        if (startPos != -1 && endPos != -1)
                        {
                            m_MySelectPosition = startPos;
                            m_MyMoveToPosition = endPos;
                            m_GameSchedule = TKJUNQI_SCHEDULE_SELECTCHESS;
                            SetThinkTickCount(GetRandomTime());
                            // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
                        }
                    }
                }
                else if (type == 0)
                {
                    if (responseJson["result"].isInt())
                    {
                        m_IsAgreePeace = responseJson["result"].asInt();
                    }
                    m_IsAgreePeace = (m_IsAgreePeace == 4 ? 0 : 1);
                    m_GameSchedule = TKJUNQI_SCHEDULE_PEACERES;
                    SetThinkTickCount(GetRandomTime());
                    // TKWriteLog("%s:%d, seat:%d", __FUNCTION__, __LINE__, m_nMySeat);
                }
        }
    }
    else
    {
        TKWriteLog("error, send to ai return %s", retStr.c_str());
    }
    return true;
}