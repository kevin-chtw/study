#include "TKDllGame.h"
#include "TKDllServer.h"

int TKGenRandom()
{
    return 0;
}

int TKGetMatchType(DWORD dwRule)
{
    return 0;
}

CTKGame::CTKGame()
{
    m_pRuler = new TKGAMERULER;
    m_pMatchRuler = new CTKMatchRuler;

    m_pRuler->nPlayersPerTable = 4;
    m_pRuler->szProperty[0] = 0;
    m_pRuler->szPropertyEx[0] = 0;

    m_pShareGameData = new char[1024];
}

CTKGame::~CTKGame()
{
    delete m_pRuler;
    delete m_pMatchRuler;
    delete[] m_pShareGameData;
}

BOOL CTKGame::OnInitialUpdate()
{
    return FALSE;
}

int CTKGame::GetLastHandSaveInfoLen()
{
    return 0;
}

BOOL CTKGame::OnGameBegin()
{
    return FALSE;
}

BOOL CTKGame::OnPlayerMsg(DWORD dwUserID, int nSeatOrder, PTKHEADER pMsg)
{
    return FALSE;
}

BOOL CTKGame::OnMatchMsg(PTKHEADER pMsg)
{
    return FALSE;
}

void CTKGame::OnPlayerNetBreak(CTKGamePlayer* pGamePlayer)
{
}

void CTKGame::OnPlayerNetResume(CTKGamePlayer* pGamePlayer)
{
}

void CTKGame::OnPlayerArrived(CTKGamePlayer* pGamePlayer)
{
}

BOOL CTKGame::OnResetDump()
{
    return FALSE;
}

void CTKGame::OnGameTimer(DWORD dwID, ULONG_PTR ulUserData)
{
}

void CTKGame::OnPlayerExitMatch(CTKGamePlayer* pGamePlayer)
{
}

BOOL CTKGame::PlayerCanLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason)
{
    return FALSE;
}

void CTKGame::OnPlayerLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason)
{
}

BOOL CTKGame::OnGTSMsg(const LPVOID pData, DWORD dwLength)
{
    return FALSE;
}

BOOL CTKGame::PostMsg2GTS(const LPVOID pData, DWORD dwLength)
{
    return FALSE;
}

BOOL CTKGame::SendAdditionalInfo2MS(PTKHEADER in_pAdditionalMsg)
{
    return FALSE;
}

void CTKGame::AddGameAction(char* szActionDescribe)
{
}

void CTKGame::AddGameResult(char* szResultDescribe)
{
}

CTKGamePlayer* CTKGame::GetGamePlayerBySeat(int nSeat)
{
    if (nSeat < 0 || nSeat >= m_GamePlayers.size())
    {
        return nullptr;
    }
    return m_GamePlayers.at(nSeat).get();
}

CTKGamePlayer* CTKGame::GetGamePlayerByUserID(DWORD dwUserID)
{
    for (auto player : m_GamePlayers)
    {
        if (player->m_dwUserID == dwUserID)
        {
            return player.get();
        }
    }
    return nullptr;
}

int CTKGame::GetGamePlayerCount()
{
    return m_pRuler->nPlayersPerTable;
}

BOOL CTKGame::RemoveWatcher(CTKGamePlayer* pWatcher)
{
    return TRUE;
}

void CTKGame::SetGameResult(int nSeat, int nWinPlus, int nLossPlus, int nDrawPlus, int& nScorePlus,
                            BOOL bValideHand /*= TRUE*/, BOOL bLevyTax /*= TRUE*/, WORD wGameResult /*= TK_UNKNOWN*/)
{
    m_GamePlayers.at(nSeat)->m_nScore += nScorePlus;
    TKWriteLog("result seat:%d score:%d", nSeat, nScorePlus);
}

void CTKGame::SetGameResult(int nSeat, int nWinPlus, int nLossPlus, int nDrawPlus, int64_t& i64ScorePlus,
                            BOOL bValideHand /*= TRUE*/, BOOL bLevyTax /*= TRUE*/, WORD wGameResult /*= TK_UNKNOWN*/)
{
    m_GamePlayers.at(nSeat)->m_nScore += i64ScorePlus;
    TKWriteLog("result seat:%d score:%lld", nSeat, i64ScorePlus);
}

BOOL CTKGame::GetMaxScorePlus(int nSeat, int& nScorePlus)
{
    return TRUE;
}

BOOL CTKGame::GetMaxScorePlus(int nSeat, double& dScorePlus)
{
    return TRUE;
}

BOOL CTKGame::PostGame2MatchMsg(const uint64_t in_i64uUserID, const uint32_t in_MsgType,
                                const char* const in_pchGame2MatchMsgJson)
{
    return FALSE;
}

BOOL CTKGame::NotifyGameOver(WORD wOverType, BOOL bNeedOverRound /*= FALSE*/,
                             WORD wOverFlags /*= TK_GAMEOVERFLAG_NULL*/)
{
    this->__FinishHand();
    return TRUE;
}

BOOL CTKGame::Broadcast(PTKHEADER pMsg)
{
    for (int i = 0; i < m_pRuler->nPlayersPerTable; ++i)
    {
        __SendAck(i, pMsg);
    }
    return TRUE;
}

BOOL CTKGame::Send2SeatPlayer(int nSeat, PTKHEADER pMsg)
{
    __SendAck(nSeat, pMsg);
    return TRUE;
}

BOOL CTKGame::Send2Black(PTKHEADER pMsg)
{
    return TRUE;
}

BOOL CTKGame::Send2SeatBlack(int nSeat, PTKHEADER pMsg)
{
    return TRUE;
}

BOOL CTKGame::Send2Observer(PTKHEADER pMsg)
{
    return TRUE;
}

BOOL CTKGame::Send2Player(DWORD dwUserID, PTKHEADER pMsg)
{
    for (auto p : m_GamePlayers)
    {
        if (p->m_dwUserID == dwUserID)
        {
            __SendAck(p->m_nSeatOrder, pMsg);
            return TRUE;
        }
    }
    return FALSE;
}

void CTKGame::RequestFDTablePreGameEnd()
{
}

BOOL CTKGame::SetGameTimer(DWORD dwID, ULONG_PTR ulUserData, int nSecond)
{
    return SetGameTimerMilli(dwID, ulUserData, nSecond * 1000);
}

BOOL CTKGame::SetGameTimerMilli(DWORD dwID, ULONG_PTR ulUserData, DWORD milliseconds)
{
    auto trigerTime = m_CurTime + milliseconds;
    auto iter = m_Timers.begin();
    for (; iter != m_Timers.end(); ++iter)
    {
        if (trigerTime < iter->trigerTime)
        {
            break;
        }
    }
    m_Timers.insert(iter, TimerData{dwID, ulUserData, (int)trigerTime});
    return TRUE;
}

void CTKGame::KillGameTimer(DWORD dwID)
{
    for (auto iter = m_Timers.begin(); iter != m_Timers.end(); ++iter)
    {
        if (iter->dwID == dwID)
        {
            m_Timers.erase(iter);
            break;
        }
    }
}

BOOL CTKGame::ValidTimer(DWORD dwID)
{
    for (auto& timer : m_Timers)
    {
        if (timer.dwID == dwID)
        {
            return TRUE;
        }
    }
    return FALSE;
}

bool CTKGame::__TrigerTimer()
{
    if (m_Timers.empty() || !m_IsRunning)
    {
        return false;
    }
    auto timer = m_Timers.front();
    m_Timers.pop_front();
    m_CurTime = timer.trigerTime + 100;
    this->OnGameTimer(timer.dwID, timer.ulUserData);
    return true;
}

void CTKGame::__FinishHand()
{
    ++m_iGameHand;
    m_IsRunning = false;
}

bool CTKGame::__IsRunning() const
{
    return m_IsRunning;
}

bool CTKGame::__GetAction(std::string& action)
{
    action.clear();
    m_CurSeat = -1;
    for (int i = 0; i < m_pRuler->nPlayersPerTable; ++i)
    {
        if (m_BotPlayers.at(i)->__GetAiRequest(action))
        {
            m_CurSeat = i;
            return true;
        }
    }

    return false;
}

void CTKGame::__GetCommonData(Json::Value& act)
{
    for (int i = 0; i < m_pRuler->nPlayersPerTable; ++i)
    {
        act["scores"].append((int64_t)m_GamePlayers.at(i)->m_nScore);
        act["hands"].append(m_BotPlayers.at(i)->__GetHands());
    }
}

bool CTKGame::__HandAction(const std::string& res)
{
    if (m_CurSeat != -1)
    {
        return m_BotPlayers.at(m_CurSeat)->__HandAction(res);
    }
    return false;
}

void CTKGame::__Initialize(const GameConfig& conf)
{
    m_pRuler->nPlayersPerTable = conf.playerCount;
    m_nScoreBase = conf.scoreBase;
    _SetPropertyEx(conf.propertyEx);

    m_GamePlayers.resize(m_pRuler->nPlayersPerTable);
    m_BotPlayers.resize(m_pRuler->nPlayersPerTable);
    for (int i = 0; i < m_pRuler->nPlayersPerTable; ++i)
    {
        m_GamePlayers.at(i) = std::shared_ptr<CTKGamePlayer>(tk_pDllServer->OnCreateTKGamePlayer());
        m_GamePlayers.at(i)->__Initialize(i, conf.initialChip);

        m_BotPlayers.at(i) = std::shared_ptr<CTKBotPlayer>(TKCreateBotPlayer());
        m_BotPlayers.at(i)->__InitGame(this, i);
    }
    m_Timers.clear();
    m_IsRunning = true;
}

void CTKGame::_SetPropertyEx(std::string const& configPropertyEx)
{
    if (configPropertyEx.size() > std::size(m_pRuler->szPropertyEx))
    {
        throw std::string("too long propertyex");
    }

    std::fill(std::begin(m_pRuler->szPropertyEx), std::end(m_pRuler->szPropertyEx), 0);
    std::copy(configPropertyEx.begin(), configPropertyEx.end(), std::begin(m_pRuler->szPropertyEx));
    m_pMatchRuler->SetProperyEx(configPropertyEx);
}

void CTKGame::__SendAck(int seat, PTKHEADER msg)
{
    m_BotPlayers.at(seat)->OnMsg(msg);
}

void CTKGame::__OnPlayerMsg(int seat, PTKHEADER pMsg)
{
    auto player = m_GamePlayers.at(seat);
    OnPlayerMsg(player->m_dwUserID, seat, pMsg);
}

BOOL CTKGame::GameRulerAward(CTKGamePlayer* pGamePlayer, int nGameRuler, int nAwardID)
{
    return TRUE;
}

BOOL CTKGame::GameRulerAwardByPlayer(DWORD dwUserID, int nGameRuler, int nAwardID)
{
    return TRUE;
}

BOOL CTKGame::GameRulerAwardBySeat(int nSeat, int nGameRuler, int nAwardID)
{
    return TRUE;
}

BOOL CTKGame::HunterAwardWare(CTKGamePlayer* pGamePlayer, DWORD dwWareID, int nAmount)
{
    return TRUE;
}

BOOL CTKGame::HunterAwardWareByPlayer(DWORD dwUserID, DWORD dwWareID, int nAmount)
{
    return TRUE;
}

BOOL CTKGame::HunterAwardWareBySeat(int nSeat, DWORD dwWareID, int nAmount)
{
    return TRUE;
}

BOOL CTKGame::BreakHeartAward(CTKGamePlayer* pGamePlayer, int nAddMatchScore, int nChgHeartCount)
{
    return TRUE;
}

BOOL CTKGame::BreakHeartAwardByPlayer(DWORD dwUserID, int nAddMatchScore, int nChgHeartCount)
{
    return TRUE;
}

BOOL CTKGame::BreakHeartAwardBySeat(int nSeat, int nAddMatchScore, int nChgHeartCount)
{
    return TRUE;
}

BOOL CTKGame::HandWinAward(CTKGamePlayer* pGamePlayer, DWORD dwSubType, DWORD dwParam, CTKFixList<DWORD>& lsShowUID)
{
    return 1;
}

BOOL CTKGame::HandWinAwardByPlayer(DWORD dwUserID, DWORD dwSubType, DWORD dwParam, CTKFixList<DWORD>& lsShowUID)
{
    return 1;
}

BOOL CTKGame::HandWinAwardBySeat(int nSeat, DWORD dwSubType, DWORD dwParam, CTKFixList<DWORD>& lsShowUID)
{
    return 1;
}

BOOL CTKGame::NotifyHttpBrokerAckMsg(const LPVOID pData, DWORD dwLength)
{
    return 0;
}

BOOL CTKGame::GameProcessData(const char* szData, DWORD dwDataLength, const char* szReserve, DWORD dwReserveLength)
{
    return 0;
}

BOOL CTKGame::PostHttpBrokerMsg(const LPVOID pData, DWORD dwLength)
{
    return 0;
}

BOOL CTKGame::PosGameProcessData2MS(char* szGameProcessData, char* szReserve, DWORD dwReserve1, DWORD dwReserve2,
                                    WORD wReserve1)
{
    return 0;
}

BOOL CTKGame::InitialPlayer(CTKGamePlayer* pGamePlayer)
{
    return TRUE;
}

BOOL CTKGame::InsertPlayer(CTKGamePlayer* pGamePlayer)
{
    return TRUE;
}

BOOL CTKGame::RemovePlayer(CTKGamePlayer* pGamePlayer)
{
    return TRUE;
}

BOOL CTKGame::InitialWatcher(CTKGamePlayer* pWatcher)
{
    return TRUE;
}

BOOL CTKGame::InsertWatcher(CTKGamePlayer* pWatcher)
{
    return TRUE;
}
