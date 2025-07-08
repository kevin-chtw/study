#include <TKDllServer.h>
#include <TKMatchRuler.h>
#include <TKDllPlayer.h>
#include <TKHttpTransProtocol.h>
#include "MJException.h"
#include "MJGame.h"
#include "MJLog.h"
#include "MJService.h"
#include "MJJson2pb.h"

#define __TRY \
    try       \
    {
#define __CATCH                                                \
    }                                                          \
    catch (std::exception & e)                                 \
    {                                                          \
        const char* what = e.what();                           \
        MJ_LOG_ERROR(this, "FATAL:exception:!%s", what);       \
        throw e;                                               \
    }                                                          \
    catch (...)                                                \
    {                                                          \
        MJ_LOG_ERROR(this, "FATAL:exception:Unknown!");        \
        throw std::runtime_error("An unknown error occurred"); \
    }

void MJGame::_InitializeMJGame()
{
    m_Initialized = false;

    m_Timer = std::make_shared<MJTimer>(this);

    int lengthJson = 0;
    auto bufferJson = m_pMatchRuler->GameJsonRule(out__ lengthJson);

    m_Config = std::make_shared<MJConfig>();
    m_Config->InitDefault(OnGetDefaultProperties());

    m_Config->LoadRuler(m_pMatchRuler->GamePropertyEx());

    if (IsFDTable() && lengthJson > 0)
    {
        auto jsonString = std::string(bufferJson, lengthJson);
        MJ_LOG_DEBUG(this, "FDTable Json:%s", _S(jsonString));

        m_Timer->SetLongLive(true);
        m_Config->LoadJsonRule(jsonString, OnGetFDTableDescriptor());  //旧版，兼容（即将弃用）
        m_Config->LoadJsonRule(jsonString, OnGetFDTableConfigMap());   //新版
    }
    MJ_LOG_DEBUG(this, "ResultRule:%s", _S(m_Config->ToCommaString()));

    m_GrowSystem = std::make_shared<MJGrowSystem>(this);

    this->_CreatePlayers();

    m_MJRecord = this->OnCreateRecord();
    if (m_MJRecord)
    {
        m_MJRecord->Initialize();
    }

    m_NBSeat = this->_InitNewbieSeat();

    m_MJPlay = this->OnCreatePlay();
    if (m_MJPlay)
    {
        m_MJPlay->Initialize();
    }

    m_JsonData = Json::Value();
    m_RequestIDs.resize(GetIniPlayerCount());
    m_IncreasedID = 0;

    m_Initialized = true;
}

BOOL MJGame::OnGameBegin()
{
    __TRY

    MJ_LOG_INFO(this, "GamePropertyEx:%s", _S(m_pMatchRuler->GamePropertyEx()));
    this->_InitializeMJGame();

    this->OnStart();

    _EnterNextStates();

    this->SetGameTimerMilli(TIMER_ID_NORMAL, 0, MJ_TIMER_INTERVAL_MS);

    MJ_LOG_INFO(this, "GameBegin success");
    return TRUE;

    __CATCH
    return FALSE;
}

BOOL MJGame::OnPlayerMsg(DWORD userID, int seatOrder, PTKHEADER pMsg)
{
    if (!pMsg)
    {
        MJ_LOG_ERROR(this, "msg is null");
        return FALSE;
    }
    if (pMsg->dwType != (TK_REQ | m_MsgType))
    {
        MJ_LOG_ERROR(this, "error msgtype dwType=%lx, while required=%lx", pMsg->dwType, m_MsgType);
        return FALSE;
    }
    if (CTKGame::OnPlayerMsg(userID, seatOrder, pMsg))
    {
        return TRUE;
    }
    if (!IsValidSeat(seatOrder))
    {
        MJ_LOG_ERROR(this, "invalid seat:%d", seatOrder);
        return FALSE;
    }
    __TRY
    this->OnReqMsg(seatOrder, pMsg);
    _EnterNextStates();
    return TRUE;
    __CATCH
    return FALSE;
}

void MJGame::OnGameTimer(DWORD dwID, ULONG_PTR ulUserData)
{
    __TRY
    if (dwID == TIMER_ID_NORMAL)
    {
        m_Timer->OnTick();
        _EnterNextStates();
        this->SetGameTimerMilli(TIMER_ID_NORMAL, 0, MJ_TIMER_INTERVAL_MS);
    }
    __CATCH
}

void MJGame::OnPlayerExitMatch(CTKGamePlayer* pGamePlayer)
{
    if (pGamePlayer == nullptr)
    {
        return;
    }

    __TRY
    auto seat = pGamePlayer->m_nSeatOrder;
    if (IsValidSeat(seat))
    {
        GetPlayer(seat)->SetOffline(true);
        GetPlayer(seat)->SetTrusted(true);
        if (m_MJRecord)
        {
            m_MJRecord->AddPlayerExitMatch(seat);
        }
    }
    __CATCH
}

void MJGame::OnPlayerNetBreak(CTKGamePlayer* pGamePlayer)
{
    if (pGamePlayer == nullptr)
    {
        return;
    }

    __TRY
    auto seat = pGamePlayer->m_nSeatOrder;
    if (IsValidSeat(seat))
    {
        GetPlayer(seat)->SetOffline(true);
        if (m_MJRecord)
        {
            m_MJRecord->AddPlayerNetBreak(seat);
        }
        if (IsFDTable())
        {
            this->OnPlayerTrustedChange(seat, ETrustType::FDTNetBreak);
        }
    }
    _EnterNextStates();
    __CATCH
}

void MJGame::OnPlayerNetResume(CTKGamePlayer* pGamePlayer)
{
    if (pGamePlayer == nullptr)
    {
        return;
    }

    __TRY
    auto seat = pGamePlayer->m_nSeatOrder;
    if (IsValidSeat(seat))
    {
        GetPlayer(seat)->SetOffline(false);
        if (m_MJRecord)
        {
            m_MJRecord->AddPlayerNetResume(seat);
        }
        if (IsFDTable())
        {
            this->OnPlayerTrustedChange(seat, ETrustType::FDTNetResume);
        }
    }
    _EnterNextStates();
    __CATCH
}

int MJGame::GetScoreBase() const
{
    return m_nScoreBase;
}
int MJGame::GetIniPlayerCount() const
{
    return m_Players.size();
}

int MJGame::GetPlayingCount() const
{
    int count = 0;
    for (ISeatID seat = 0; seat < GetIniPlayerCount(); ++seat)
    {
        if (!m_Players.at(seat)->IsOut())
        {
            ++count;
        }
    }
    return count;
}

void MJGame::_CreatePlayers()
{
    m_Players.clear();
    auto count = this->GetGamePlayerCount();
    for (ISeatID seat = 0; seat < count; ++seat)
    {
        auto player = std::make_shared<MJPlayer>(this);
        player->Initialize(seat);
        m_Players.push_back(player);
    }
}

std::shared_ptr<MJPlayer> MJGame::GetPlayer(ISeatID seat)
{
    if (!IsValidSeat(seat))
    {
        throw MJ_MAKE_EXCEPTION("invalid seat: %d", seat);
    }
    return m_Players.at(seat);
}

std::shared_ptr<MJGrowSystem> MJGame::GetGrowSystem() const
{
    return m_GrowSystem;
}

std::shared_ptr<MJConfig> MJGame::GetConfig() const
{
    return m_Config;
}

CTKGamePlayer* MJGame::GetGamePlayer(ISeatID seat, bool nullable)
{
    auto player = this->GetGamePlayerBySeat(seat);
    if (player == nullptr && !nullable)
    {
        throw MJ_MAKE_EXCEPTION("game player is null at seat:%d", seat);
    }
    return player;
}

bool MJGame::IsFDTable() const
{
    static constexpr int FD_STAGE_TYPE = 36;
    return m_stStageRuler.nStageType == FD_STAGE_TYPE;
}

bool MJGame::IsCoinMatch() const
{
    return this->m_nScoreType == TK_GAME_SCORETYPE_LIFE;
};

MJGame::MJGame(unsigned msgType)
{
    m_MsgType = msgType;
    GameCounter::Instance()->AddGameCount();
}

unsigned MJGame::GetMsgType()
{
    return m_MsgType;
}

MJGame::~MJGame()
{
    GameCounter::Instance()->DecGameCount();
}

std::shared_ptr<MJRecord> MJGame::OnCreateRecord()
{
    return std::make_shared<MJRecord>(this);
}

MJConfigFDDescriptor MJGame::OnGetFDTableDescriptor()
{
    return MJConfigFDDescriptor();
}

MJFDConfigMap MJGame::OnGetFDTableConfigMap()
{
    return MJFDConfigMap();
}

MJDrawConfigs MJGame::GetDrawConfigs()
{
    static MJDrawConfigs configs = {
#include "DrawConfig.inc"
    };
    return configs;
}

void MJGame::OnEnd(EGameOverStatus status)
{
    if (status != EGameOverStatus::Normal)
    {
        if (nullptr != m_MJPlay)
        {
            m_MJPlay->DumpHistory();
        }
    }
}

void MJGame::OnSendDebugMsg(std::string const& msg, ISeatID seat)
{
    MJ_LOG_DEBUG(this, "%d:%s", seat, _S(msg));
}

std::shared_ptr<MJTimer> MJGame::GetTimer()
{
    return m_Timer;
}

void MJGame::_HandleWinAwards()
{
    if (!this->m_stStageRuler.bEnableHandWinAward)
    {
        return;
    }
    std::vector<MJPlayer*> players;

    for (int i = 0; i < GetIniPlayerCount(); i++)
    {
        players.push_back(GetPlayer(i).get());
    }
    auto comp = [](MJPlayer* a, MJPlayer* b) { return a->GetScoreChange() > b->GetScoreChange(); };
    std::sort(players.begin(), players.end(), comp);

    int nWinAwardRank = 1;
    auto nMaxScore = players.at(0)->GetScoreChange();
    for (int i = 0; i < GetIniPlayerCount(); i++)
    {
        auto curScore = players.at(i)->GetScoreChange();
        if ((nMaxScore < 0) || (curScore < nMaxScore && nWinAwardRank >= 2))
        {  //2名之后名次不发奖
            break;
        }

        if (curScore < nMaxScore)
        {
            nWinAwardRank++;
            nMaxScore = curScore;
        }

        if (nMaxScore > 0 && nWinAwardRank <= 2)
        {
            DWORD uid = players.at(i)->GetUserID();
            CTKFixList<DWORD> fixlist;
            fixlist.AddTail(&uid);
            //subtype:与比赛配置有关，一般会配置bout0（第一名）和bout1（第二名）；
            this->HandWinAwardByPlayer(uid, nWinAwardRank - 1, 0, fixlist);
        }
    }
}

void MJGame::FinishGame(EGameOverStatus status)
{
    MJ_LOG_INFO(this, "status=%d", _D(status));
    if (m_MJRecord)
    {
        m_MJRecord->Finish();
    }

    if (EGameOverStatus::Normal != status)
    {
        for (ISeatID seat = 0; seat < GetIniPlayerCount(); ++seat)
        {
            int scoreChange = 0;
            SetGameResult(seat, 0, 0, 0, scoreChange, TRUE, FALSE);
        }
    }
    this->OnEnd(status);
    this->NotifyGameOver(TK_GAMEOVERTYPE_NORNAL);
}

bool MJGame::IsValidSeat(ISeatID seat) const
{
    return seat >= 0 && seat < (int)m_Players.size();
}

void MJGame::SyncResult()
{
    LogGameResultRecord();
    PostGameProcessData();
    _HandleWinAwards();
}

void MJGame::CacheStateData(const google::protobuf::Message* ack)
{
    auto& stateData = m_JsonData["StatementData"];
    const std::string& name = ack->GetDescriptor()->name();
    stateData[name] = MJ::pb2json(*ack);
}

int MJGame::UpdateRequestID(ISeatID seat /*= SEAT_ALL*/)
{
    ++m_IncreasedID;
    if (IsValidSeat(seat))
    {
        m_RequestIDs.at(seat) = m_IncreasedID;
    }
    else
    {
        std::fill(m_RequestIDs.begin(), m_RequestIDs.end(), m_IncreasedID);
    }
    return m_IncreasedID;
}

bool MJGame::IsRequestID(ISeatID seat, int request)
{
    if (IsValidSeat(seat))
    {
        return m_RequestIDs.at(seat) == request;
    }
    return false;
}

bool MJGame::HasClientOnline()
{
    for (int i = 0; i < GetGamePlayerCount(); ++i)
    {
        CTKGamePlayer* player = GetGamePlayerBySeat(i);
        if (nullptr == player)
        {
            continue;
        }

        if (!player->IsBotUser() && !player->IsExitGame() && player->m_nNetStatus == TK_PLAYERNETSTATUS_NORMAL && player->m_nOpertorType == TK_GAMEPLAYEROPERATORTYPE_PLAYER)
        {
            return true;
        }
    }
    return false;
}

void MJGame::AddDebugStr(std::string const& str)
{
    m_Debugstr += str;
}

std::string const& MJGame::GetDebugStr()
{
    return m_Debugstr;
}

void MJGame::ClearDebugStr()
{
    m_Debugstr.clear();
}

void MJGame::SendAckMsg(const google::protobuf::Message* ack, ISeatID seat)
{
    if (m_MJRecord)
    {
        m_MJRecord->RecordAction(ack, seat);
    }

    auto msg = SerializeAck(ack);
    if (IsValidSeat(seat))
    {
        Send2SeatPlayer(seat, (PTKHEADER)msg.data());
    }
    else
    {
        Broadcast((PTKHEADER)msg.data());
    }
}

std::vector<BYTE> MJGame::SerializeAck(const google::protobuf::Message* ack)
{
    std::vector<BYTE> data(sizeof(TKHEADER) + ack->ByteSize());
    auto pHeader = (PTKHEADER)data.data();

    pHeader->dwType = TK_ACK | m_MsgType;
    pHeader->dwLength = ack->ByteSize();
    ack->SerializeToArray((BYTE*)pHeader + sizeof(TKHEADER), ack->ByteSize());
    return data;
}

bool MJGame::ConvertReqMsg(google::protobuf::Message& msg, PTKHEADER pMsg)
{
    size_t szLen = pMsg->dwLength + sizeof(TKHEADER);
    std::string szSerializeData((BYTE*)pMsg + sizeof(TKHEADER), (BYTE*)pMsg + szLen);
    return msg.ParseFromString(szSerializeData);  // 反序列化
}

void MJGame::FillAndSendHaiDiLaoMsg(int nFan, DWORD uId, const std::vector<std::pair<int, int>>& vData, const std::string& strExternData)
{
    TKREQADDITIONALINFO reqHDLInfo;
    reqHDLInfo.header.dwType = REQ_TYPE(TKID_GS2MS_GAME_ADDITIONAL_INFO);
    reqHDLInfo.dwUserID = uId;
    // 生成xml
    std::string strHDL;
    {
        strHDL += strExternData;
        for (auto it : vData)
        {
            char szAction[256] = {0};
            sprintf_s(szAction, "<bottom id=\"%d\" fan=\"%d\"/>", it.first, it.second);

            strHDL += szAction;
        }
        // 奖励
        char szReward[256] = {0};
        sprintf_s(szReward, "<reward value=\"%d\"/>", nFan * m_nScoreBase);
        strHDL += szReward;
    }

    //////////////////////////////////////////////////////////////////////////
    reqHDLInfo.header.dwLength = MSG_LENGTH(TKREQADDITIONALINFO) + 1 * sizeof(int) + strHDL.length();

    reqHDLInfo.sufi32Data.dwSuffixOffSet = sizeof(TKREQADDITIONALINFO);
    reqHDLInfo.sufi32Data.dwSuffixSize = 1 * sizeof(int);

    reqHDLInfo.sufchData.dwSuffixOffSet = reqHDLInfo.sufi32Data.dwSuffixOffSet + reqHDLInfo.sufi32Data.dwSuffixSize;
    reqHDLInfo.sufchData.dwSuffixSize = strHDL.length();

    CTKBuffer buf;
    buf.Append(&reqHDLInfo, sizeof(TKREQADDITIONALINFO));
    buf.Append(&nFan, sizeof(nFan));              // max fan
    buf.Append(strHDL.c_str(), strHDL.length());  // data

    //////////////////////////////////////////////////////////////////////////
    // call interface
    if (!SendAdditionalInfo2MS((PTKHEADER)buf.GetBufPtr()))
    {
        TKWriteLog("this=%p\t发送海底捞消息失败！UID=%d\\tDATA=%s", this, uId, strHDL.c_str());
    }

    MJ_LOG_INFO(this, "SendHaiDiLaoMsg %s", strHDL.c_str());
}

void MJGame::SendMsgToGTS(std::string const& data)
{
    this->PostMsg2GTS((LPVOID)data.data(), data.length());
}

BOOL MJGame::OnGTSMsg(const LPVOID pData, DWORD dwLength)
{
    auto* begin = (const char*)pData;
    auto* end = begin + dwLength;
    std::string msg(begin, end);
    MJ_LOG_DEBUG(this, "ignore gts msg:%s", msg.c_str());
    return 0;
}

BOOL MJGame::NotifyHttpBrokerAckMsg(const LPVOID pData, DWORD dwLength)
{
    __TRY
    TKHTTPMATCHMSGACK* ack = (TKHTTPMATCHMSGACK*)pData;
    if (dwLength < sizeof(TKHTTPMATCHMSGACK))
    {
        MJ_LOG_ERROR(this, "invalid NotifyHttpBrokerAckMsg data length %d", int(dwLength));
        return FALSE;
    }
    if (ack->header.dwLength + TKHEADERSIZE != dwLength)
    {
        MJ_LOG_ERROR(this, "error TKHTTPMATCHMSGACK.header.dwLength:%d data length %d", ack->header.dwLength, int(dwLength));
        return FALSE;
    }

    auto buffer = (const char*)pData + TKHEADERSIZE;
    BrokerAckData ackData;
    ackData.serialsNo = ack->header.dwSerial;
    ackData.extra.assign(buffer + ack->sufReturnData.wOffSet, ack->sufReturnData.wDataLen);
    ackData.httpHeader.assign(buffer + ack->sufHeader.wOffSet, ack->sufHeader.wDataLen);
    ackData.httpBody.assign(buffer + ack->sufMsgBody.wOffSet, ack->sufMsgBody.wDataLen);

    this->OnHttpBrokerAck(ackData);

    _EnterNextStates();

    __CATCH
    return TRUE;
}

void MJGame::SendHttpBrokerReq(BrokerReqData const& data)
{
    CTKBuffer buf{};
    buf.SetHeaderLen(sizeof(TKHTTPMATCHMSGREQ));
    buf.Append(data.url.data(), data.url.size());
    buf.Append(data.extra.data(), data.extra.size());
    buf.Append(data.httpHeader.data(), data.httpHeader.size());
    buf.Append(data.httpBody.data(), data.httpBody.size());

    static int x = 0;
    TKHTTPMATCHMSGREQ* pAiMsg = (TKHTTPMATCHMSGREQ*)buf.GetBufPtr();
    pAiMsg->header.dwLength = buf.GetBufLen() - TKHEADERSIZE;
    pAiMsg->dwUserID = 0;
    pAiMsg->dwMatchID = m_dwMatchID;
    pAiMsg->wStageID = m_wStageID;
    pAiMsg->wRoundID = m_wRoundID;
    pAiMsg->wGameID = m_nGameID;
    pAiMsg->wMethod = data.httpHeader.substr(0, 4) == "POST" ? 1 : 0;  // 0 Get, 1 Post
    pAiMsg->nSenderType = 1;                                           // 1-Game, 2-Player，3-DllServer
    pAiMsg->dwReserve1 = 0;
    pAiMsg->dwReserve2 = 0;

    pAiMsg->sufURL.wOffSet = MSG_LENGTH(TKHTTPMATCHMSGREQ);
    pAiMsg->sufURL.wDataLen = (WORD)data.url.size();
    pAiMsg->sufReturnData.wOffSet = pAiMsg->sufURL.wOffSet + pAiMsg->sufURL.wDataLen;
    pAiMsg->sufReturnData.wDataLen = (WORD)data.extra.size();
    pAiMsg->sufHeader.wOffSet = pAiMsg->sufReturnData.wOffSet + pAiMsg->sufReturnData.wDataLen;
    pAiMsg->sufHeader.wDataLen = (WORD)data.httpHeader.size();
    pAiMsg->sufMsgBody.wOffSet = pAiMsg->sufHeader.wOffSet + pAiMsg->sufHeader.wDataLen;
    pAiMsg->sufMsgBody.wDataLen = (WORD)data.httpBody.size();

    pAiMsg->header.dwMagic = 0;
    pAiMsg->header.wReserve = 0;
    pAiMsg->header.dwParam = 0;
    pAiMsg->header.dwType = TKID_GS2BROKER_HTTPMSG;
    pAiMsg->header.wOrigine = 0;
    pAiMsg->header.dwSerial = data.serialsNo;

    auto res = this->PostHttpBrokerMsg(buf.GetBufPtr(), buf.GetBufLen());
    MJ_LOG_DEBUG(this, "PostHttpBrokerMsg %d", res);
}

void MJGame::OnHttpBrokerAck(BrokerAckData const& data)
{
    MJ_LOG_ERROR(this, "ignore http serialsNo:%d header:%s body:%s", data.serialsNo, data.httpHeader.c_str(), data.httpBody.c_str());
}

void MJGame::_EnterNextStates()
{
    while (m_NextState)
    {
        m_CurrentState = m_NextState;
        m_NextState = nullptr;

        m_CurrentState->m_Game = this;
        m_Timer->Cancel();
        m_CurrentState->OnEnter();  // maybe call EnterState<> again.
    }
}

ISeatID MJGame::_InitNewbieSeat()
{
    int botCount = 0;
    ISeatID nbSeat = SEAT_NULL;
    for (int i = 0; i < GetIniPlayerCount(); ++i)
    {
        CTKGamePlayer* pPlayer = this->GetGamePlayer(i);
        if (pPlayer->IsBotUser())
        {
            ++botCount;
            continue;
        }
        if (pPlayer->m_byUserTypeTag == (int)EPlayerType::Newbie)
        {
            nbSeat = i;
        }
    }

    return botCount == GetIniPlayerCount() - 1 ? nbSeat : SEAT_NULL;
}

void MJGame::LogGameResultRecord()
{
    for (int i = 0; i < GetIniPlayerCount(); ++i)
    {
        auto pPlayer = this->GetPlayer(i);
        auto s = "<result seat=\"" + std::to_string(i) + "\" score=\"" + std::to_string(pPlayer->GetScoreChange()) + "\" />";
        this->AddGameResult((char*)s.c_str());
    }
}

void MJGame::PostGameProcessData()
{
    auto& stateData = m_JsonData["StatementData"];
    stateData["curRound"] = Json::Int(m_iGameHand);

    auto& resultData = m_JsonData["ResultData"];
    for (int i = 0; i < GetIniPlayerCount(); ++i)
    {
        auto player = GetPlayer(i);
        resultData["uidList"].append((double)player->GetUserID());
        resultData["scoreList"].append((double)player->GetCurrentScore());
        stateData["uidList"].append((double)player->GetUserID());
    }
    this->PosGameProcessData2MS((char*)m_JsonData.toStyledString().c_str());
}
