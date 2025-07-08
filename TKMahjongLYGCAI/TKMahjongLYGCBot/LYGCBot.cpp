#include "LYGCBotDefine.h"

#include "LYGCBot.h"
#include "LYGCDefine.h"

#define AI_SERVER_MSG_TYPE_ACTION 1
#define BOT_AI_LEVEL_101 101

/// @brief For DLL Create Bot.
/// @param
IMPLEMENT_CREATEBOTPLAYER(LYGCBot);

#define __TRY \
    try       \
    {
#define __CATCH                                          \
    }                                                    \
    catch (std::exception & e)                           \
    {                                                    \
        const char* what = e.what();                     \
        MJ_LOG_ERROR(this, "FATAL:exception:!%s", what); \
        this->Done();                                    \
    }                                                    \
    catch (...)                                          \
    {                                                    \
        MJ_LOG_ERROR(this, "FATAL:unknow exception!");   \
        this->Done();                                    \
    }

using namespace std::chrono;

static std::default_random_engine m_RE((int)system_clock::now().time_since_epoch().count());

LYGCBot::LYGCBot()
{
    m_Config.InitDefault(g_DefaultConfig);

    m_BirthTime = std::chrono::system_clock::now();
    m_RobotAI = std::make_shared<LYGCAI>(this);
}

LYGCBot::~LYGCBot()
{
}

BOOL LYGCBot::OnInitialUpdate()
{
    if (!CTKBotPlayer::OnInitialUpdate())
    {
        return FALSE;
    }
    this->Init();

    return TRUE;
}

bool LYGCBot::UseAI()
{
    return m_bPacerBot && m_RobotAI && m_RobotAI->IsValid();
}

void LYGCBot::Init()
{
#if DEBUG_AI
    m_bPacerBot = true;
#endif
}

BOOL LYGCBot::OnTickCount()
{
    __TRY

    BOOL success = FALSE;
    if (!m_IsOver)
    {
        if (m_AIAction && m_AIAction->OnTick())
        {
            m_AIAction.reset();
            success = TRUE;
        }
        SetThinkTickCount(DefaultTimerInterval);
    }
    return success;

    __CATCH
    return FALSE;
}

std::string LYGCBot::__GetHands()
{
    return MJ::GetTileAINameCvt()->IdsToNames(m_TableData->GetSelfData().GetHandTiles());
}

LYGCTableData* LYGCBot::GetTable()
{
    return m_TableData.get();
}

BOOL LYGCBot::OnMsg(PTKHEADER pMsg)
{
    __TRY
    //基类先处理
    if (CTKBotPlayer::OnMsg(pMsg))
    {
        return true;
    }
    switch (pMsg->dwType)
    {
    case TK_ACK | TK_MSG_GAME_ENTERROUND: {
        OnAckEnterRound(pMsg);
    }
    break;

    case TK_ACK | TK_MSG_GAME_ADDGAMEPLAYERINFO: {
        OnAckAddPlayerInfo(pMsg);
    }
    break;

    case TK_ACK | TK_MSG_GAME_BEGINHAND: {
        OnAckBeginHand(pMsg);
    }
    break;

    case TK_ACK | TK_MSG_GAME_HISTORYMSGBEGIN: {
        OnAckHistoryMsgBegin(pMsg);
    }
    break;
    case TK_ACK | TK_MSG_GAME_HISTORYMSGEND: {
        OnAckHistoryMsgEnd(pMsg);
    }
    break;
    case TK_ACK | TK_MSG_GAME_MJLYGC: {
        // from Svr
        if (!m_IsOver)
        {
            OnPbMsg(pMsg);
        }
    }
    break;
    case TKID_MSG_AI_WEB_SERVER: {
        // from AI
        if (!m_IsOver)
        {
            OnAIServerAck(pMsg);
        }
    }
    break;
    default: {
        break;
    }
    }
    return TRUE;
    __CATCH;
    return TRUE;
}

void LYGCBot::Ready()
{
    if (!IsValidSeat(m_Seat))
    {
        MJ_LOG_ERROR(0, "invalid my seat %d", m_Seat);
        return;
    }
    // 处理每盘开始的时需要重置的数据
    m_AIAction.reset();
    m_IsOver = false;
    m_LastHttpRequest.clear();

    m_TableData = std::make_shared<LYGCTableData>();

    m_TurnCount = 0;
    m_SerialNum = 0;

    m_RobotAI->Reset(GetUserID(), m_nAILevel);
    m_TableData->Reset(m_Seat);
    m_RobotAI->InitRecordId(m_AllPlayersUserID, m_dwMatchID, GetSeat());
}

void LYGCBot::Done()
{
    m_IsOver = true;
}

bool LYGCBot::OnAckEnterRound(PTKHEADER pMsg)
{
    PTKACKGAMEPLAYERENTER pAck = (PTKACKGAMEPLAYERENTER)pMsg;
    if (pMsg->dwParam != TK_ACKRESULT_SUCCESS)
    {
        TKWriteLog("Bot(%d) Enter GameRound failed:", m_dwBotUserID);
        Terminate(TRUE);
        return true;
    }

    if (pAck->nUserType != 0)
    {
        MJ::Formator logInfo("Bot type not a Player");
        logInfo("type", pAck->nUserType)("uid", pAck->dwUserID);
        return true;
    }

    m_UserID = pAck->dwUserID;
    m_Seat = pAck->nSeatOrder;
    m_IsOver = false;

    return true;
}
bool LYGCBot::OnAckAddPlayerInfo(PTKHEADER pMsg)
{
    //BOTLOG();
    try
    {
        PTKACKGAMEADDPLAYERENTERINFO pAck = (PTKACKGAMEADDPLAYERENTERINFO)pMsg;

        if (pAck->info.nSeatOrder < 0 || pAck->info.nSeatOrder >= NP4)
        {
            TKWriteLog("OnAckAddPlayerInfo Error, pAck->info.nSeatOrder = %d", pAck->info.nSeatOrder);
            return FALSE;
        }

        m_AllPlayersUserID[pAck->info.nSeatOrder] = pAck->info.dwUserID;

        return TRUE;
    }
    catch (...)
    {
        return TRUE;
    }
    return TRUE;
}
bool LYGCBot::OnAckHistoryMsgBegin(PTKHEADER pMsg)
{
    m_IsHistoryMsg = true;
    return true;
}

bool LYGCBot::OnAckHistoryMsgEnd(PTKHEADER pMsg)
{
    m_IsHistoryMsg = false;
    return true;
}

bool LYGCBot::OnAckBeginHand(PTKHEADER pMsg)
{
    MJ_LOG_INFO(this, "pacer:%d lvl:%d", (int)m_bPacerBot, (int)m_nAILevel);

    m_IsOver = false;
    return true;
}

void LYGCBot::OnPbMsg(PTKHEADER pMsg)
{
    if (!IsValidSeat(m_Seat))
    {
        MJ_LOG_ERROR(this, "Invalid My Seat @OnPbMsg", true);
        this->Done();
        return;
    }

    size_t szLen = pMsg->dwLength + sizeof(TKHEADER);
    std::string szSerializeData((BYTE*)pMsg + sizeof(TKHEADER), (BYTE*)pMsg + szLen);
    TKMobileAckMsg mobileAckMsg;
    mobileAckMsg.ParseFromString(szSerializeData);
    if (!mobileAckMsg.has_mahjonglygc_ack_msg())
    {
        return;
    }
    const auto& ackMsg = mobileAckMsg.mahjonglygc_ack_msg();

    this->Dispatch(ackMsg, m_Seat);
}

void LYGCBot::StartAction(std::shared_ptr<BotAction> action)
{
    if (m_IsHistoryMsg)
    {
        return;
    }
    ++m_SerialNum;
    m_AIAction = action;
}

void LYGCBot::StartActionWithAI(std::shared_ptr<BotAction> action, std::string const& content)
{
    if (m_IsHistoryMsg)
    {
        return;
    }
    ++m_SerialNum;
    m_AIAction = action;

    auto httpReq = m_RobotAI->CreateWebServerMsg(content, m_SerialNum, m_dwProductID);
    SendHttpMsgToAI(httpReq);
}

void LYGCBot::SendReqMsgToSvr(TKMobileReqMsg& rMsg)
{
    std::string szSerializeMsg;
    rMsg.mutable_mahjonglygc_req_msg()->set_matchid(m_dwMatchID);

    if (!rMsg.SerializeToString(&szSerializeMsg))
    {
        assert(false);
    }
    std::vector<BYTE> buffer(sizeof(TKHEADER) + rMsg.ByteSize(), 0);
    BYTE* pszPack = buffer.data();
    ((PTKHEADER)pszPack)->dwType = TK_REQ | TK_MSG_GAME_MJLYGC;
    ((PTKHEADER)pszPack)->dwLength = rMsg.ByteSize();
    BYTE* pData = pszPack + sizeof(TKHEADER);
    memcpy(pData, szSerializeMsg.c_str(), rMsg.ByteSize());
    SendMsg((PTKHEADER)pszPack);
}

void LYGCBot::SendHttpMsgToAI(std::string const& httpReq)
{
    CTKBuffer buffer;
    _InitAIMsg(buffer, 0, httpReq);

    m_LastHttpRequest = httpReq;
    MJ_LOG_DEBUG(this, "send Ai req=%s", httpReq.c_str());
    if (!SendMsg((PTKHEADER)buffer.GetBufPtr()))
    {
        TKWriteLog("fail send ai web msg");
    }
}

unsigned int LYGCBot::GetUserID() const
{
    return m_UserID;
}

ISeatID LYGCBot::GetSeat() const
{
    return m_Seat;
}

bool LYGCBot::OnAIServerAck(PTKHEADER pMsg)
{
    switch (pMsg->wOrigine)
    {
    case BOT_AI_LEVEL_101:
        return OnAIServerAckWithLevel(pMsg, 101);
        break;
    default:
        TKWriteLog("Warning Unknown Msg Origine: %d", pMsg->wOrigine);
    }
    return true;
    ;
}

bool LYGCBot::OnAIServerAckWithLevel(PTKHEADER pMsg, int level)
{
    PTKMSGAIWEBSERVER msg = (PTKMSGAIWEBSERVER)pMsg;

    if (msg->msgType == AI_SERVER_MSG_TYPE_ACTION)
    {
        if ((m_SerialNum != msg->header.dwSerial))
        {
            std::string retStr = (char*)(msg) + TKHEADERSIZE + msg->offset;

            MJ_LOG_ERROR(this, "serial=%d, ack serial=%d, %s", m_SerialNum, (int)msg->header.dwSerial, _S(retStr));

            return false;
        }
        if (m_AIAction)
        {
            auto reply = m_RobotAI->HandleWebSeverMsgAck(msg);
            MJ_LOG_DEBUG(this, "AIAction %d %s %s", (int)reply.Success, _S(reply.Action), _S(reply.Content));
            m_AIAction->OnAIReply(reply);
        }
    }
    return true;
}

/// @brief 构造AI的HTTP请求消息（封装到通用消息）
/// @param buffer 序列化后的消息数据
/// @param nMinThinkTime 设定的AI思考时长
/// @param httpReq HTTP请求协议包
void LYGCBot::_InitAIMsg(CTKBuffer& buffer, int nMinThinkTime, std::string const& httpReq)
{
    buffer.SetHeaderLen(sizeof(TKMSGAIWEBSERVER));
    buffer.AppendString(httpReq.c_str());

    PTKMSGAIWEBSERVER pMsg = (PTKMSGAIWEBSERVER)buffer.GetBufPtr();

    pMsg->header.wOrigine = BOT_AI_LEVEL_101;
    pMsg->msgType = AI_SERVER_MSG_TYPE_ACTION;

    pMsg->header.dwSerial = m_SerialNum;
    pMsg->header.dwType = TKID_MSG_AI_WEB_SERVER;
    pMsg->dwUserID = GetUserID();
    pMsg->timeStamp = GetTickCount();
    pMsg->minThinkTime = nMinThinkTime;
    pMsg->offset = sizeof(TKMSGAIWEBSERVER) - TKHEADERSIZE;
    pMsg->dataLength = (WORD)httpReq.length();
    pMsg->header.dwLength = pMsg->offset + pMsg->dataLength;

    auto strIP = m_RobotAI->GetIPString();
    auto port = m_RobotAI->GetPort();

    memset(pMsg->szServerName, 0, sizeof(pMsg->szServerName));
    // memcpy(pMsg->szServerName, strDomainName.c_str(), strDomainName.length());
    memset(pMsg->szServerIP, 0, sizeof(pMsg->szServerIP));
    memcpy(pMsg->szServerIP, strIP.c_str(), strIP.length());
    pMsg->port = port;
}

// PB MSG BEGIN ----------->

void LYGCBot::OnGameBeginAck(TKMahJongGameBeginAck const& ack, ISeatID)
{
    if (ack.has_svrversion())
    {
        auto ver = ack.svrversion();
        MJ_LOG_ERROR(this, "svr version:%s", ver.c_str());
    }

    this->Ready();

    SetThinkTickCount(DefaultTimerInterval);
}

void LYGCBot::OnPlaceAck(TKMahJongPlaceAck const& ack, ISeatID)
{
    m_TableData->SetBanker(ack.banker());
    MJ_LOG_DEBUG(this, "banker %d", ack.banker());
    std::string ruler;
    if (ack.has_jsonruler())
    {
        ruler = ack.jsonruler();
    }
    m_Config.LoadRuler(ruler);

    MJ_LOG_DEBUG(this, "ruler:%s", _S(m_Config.ToCommaString()));
}

void LYGCBot::OnFanCiAck(const TKMahjongFanCiAck& ack, ISeatID seat)
{
    m_TableData->SetCiTile(ack.tile());
}

void LYGCBot::OnOpenDoorAck(TKMahJongOpenDoorAck const& ack, ISeatID)
{
    for (auto tile : ack.tiles())
    {
        m_TableData->GetSelfData().Draw(tile);
    }
    auto tiles = std::vector<int>(ack.tiles().begin(), ack.tiles().end());
    MJ_LOG_DEBUG(this, "init hand: %s seat:%d", _S(MJ::GetTileAINameCvt()->IdsToNames(tiles)), m_Seat);
}

void LYGCBot::OnActionAck(TKMahJongActionAck const& ack, ISeatID)
{
    MJOperates operates;
    operates.value = ack.requesttype();
    auto action = std::make_shared<BotActionOperation>(this, std::chrono::milliseconds(MJ::Random(NormaltAiExpiredTime - AiExpiredTimRage) + AiExpiredTimRage), UseAI());
    action->SetRequestID(ack.requestid());
    action->SetOperates(operates);
    std::vector<ITileID> tiles;
    tiles.reserve(ack.citiles().size());
    std::copy(ack.citiles().begin(), ack.citiles().end(), std::back_inserter(tiles));
    action->SetCiTiles(tiles);
    if (UseAI())
    {
        StartActionWithAI(action, m_TableData->CreateJsonContent(operates));
    }
    else
    {
        StartAction(action);
    }
}

void LYGCBot::OnDiscardTileAck(TKMahJongDiscardTileAck const& ack, ISeatID)
{
    auto tile = ack.tileid();
    auto seat = ack.seat();
    if (seat == m_Seat)
    {
        m_TableData->GetSelfData().Discard(tile);
        MJ_LOG_DEBUG(this, "Action discard %s", _S(MJ::GetTileAINameCvt()->IdToName(tile)));
        MJ_LOG_DEBUG(this, "hand:%s", _S(MJ::GetTileAINameCvt()->IdsToNames(m_TableData->GetSelfData().GetHandTiles())));
    }
    m_TableData->RecordHistory(seat, EOperateDiscard, tile);
}

void LYGCBot::OnDrawTileAck(TKMahJongDrawTileAck const& ack, ISeatID)
{
    auto tile = ack.tileid();
    auto seat = ack.seat();
    if (seat == m_Seat)
    {
        m_TableData->GetSelfData().Draw(tile);
        MJ_LOG_DEBUG(this, "Action draw %s", _S(MJ::GetTileAINameCvt()->IdToName(tile)));
    }
    m_TableData->RecordHistory(seat, EOperateDraw, tile);
}

void LYGCBot::OnPengAck(TKMahJongPengAck const& ack, ISeatID)
{
    auto tile = ack.tileid();
    auto seat = ack.seat();
    if (seat == m_Seat)
    {
        m_TableData->GetSelfData().Pon(tile, ack.discardseat());
        MJ_LOG_DEBUG(this, "Action pon %s", _S(MJ::GetTileAINameCvt()->IdToName(tile)));
    }
    m_TableData->RecordHistory(seat, EOperatePon, tile);
}

void LYGCBot::OnGangAck(TKMahJongGangAck const& ack, ISeatID)
{
    auto tile = ack.tileid();
    auto seat = ack.seat();
    if (seat == m_Seat)
    {
        m_TableData->GetSelfData().KonS(tile, (KonType)ack.gangtype(), ack.discardseat());
        MJ_LOG_DEBUG(this, "Action kon %s", _S(MJ::GetTileAINameCvt()->IdToName(tile)));
    }
    m_TableData->RecordHistory(seat, EOperateKon, tile);
}

void LYGCBot::OnWinAck(TKMahJongWinAck const& ack, ISeatID)
{
    auto winSeats = std::vector<ISeatID>(ack.winseats().begin(), ack.winseats().end());
    m_TableData->HandleWin(winSeats, ack.wintileid(), ack.paoseat());
    if (MJ::HasElement(winSeats, m_Seat))
    {
        this->Done();
    }
}

void LYGCBot::OnTrustPlayAck(TKMahJongTrustPlayAck const& ack, ISeatID)
{
    if (ack.type() != 0 && ack.seat() == m_Seat)
    {
        TKMobileReqMsg mobileReq;
        auto pReqMsg = mobileReq.mutable_mahjonglygc_req_msg();
        auto pReq = pReqMsg->mutable_tkmahjongtrustplay_req_msg();
        pReq->set_type(0);
        pReq->set_userid(m_dwBotUserID);
        this->SendReqMsgToSvr(mobileReq);
    }
}
