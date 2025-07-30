#include "StdAfx.h"
#include "TKJunQiPKData.h"
#include "TKDllPlayer.h"
#include "TKJunQiPKGame.h"
#include "TKJunQiPKService.h"

CTKJunQiPKData::CTKJunQiPKData()
    : m_pJunQiPKGame(NULL),
      m_nPlayerCount(JUNQIPK_UNKNOWN),
      m_nCurSeat(JUNQIPK_UNKNOWN),
      m_nCurSelectPos(0),
      m_nStage(ES_Distributing),
      m_nNoFightSteps(0),
      m_nStepsMoved(0),
      m_nPeaceSeat(JUNQIPK_UNKNOWN),
      m_bColorSet(true)
{
    InitTimeout();
    m_pJunQiPKCfg = std::make_shared<CTKJunQiPKCfg>();
    m_pJunQiCore = std::make_shared<CTKJunQiPKAQ>(m_pJunQiPKCfg);
}

CTKJunQiPKData::~CTKJunQiPKData()
{
}

void CTKJunQiPKData::OnGameBegin(CTKJunQiPKGame* pGame, const string& szCfg)
{
    m_pJunQiPKGame = pGame;
    m_pJunQiPKCfg->InitCfg(szCfg);
    _initPlayers();
    m_nPeaceStatus = 0;
    if (Int_True == m_pJunQiPKCfg->GetCfg(ECI_FriendTable))
    {
        SendInitRuleAck();
    }
    else
    {
        SendInitChessAck();
    }
}

bool CTKJunQiPKData::OnPlayerMsg(int seatOrder, const JunQiPKReqMsg& reqMsg)
{
    if (reqMsg.has_initrule_req_msg())
    {
        return OnInitRuleMsg(seatOrder, reqMsg.initrule_req_msg());
    }
    if (reqMsg.has_initlayout_req_msg())
    {
        return OnInitLayoutMsg(seatOrder, reqMsg.initlayout_req_msg());
    }
    if (reqMsg.has_cancellayout_req_msg())
    {
        return OnCanCelLayoutMsg(seatOrder);
    }
    if (reqMsg.has_selectchess_req_msg())
    {
        return OnSelectChessMsg(seatOrder, reqMsg.selectchess_req_msg());
    }
    if (reqMsg.has_movechess_req_msg())
    {
        return OnMoveChessMsg(seatOrder, reqMsg.movechess_req_msg());
    }
    if (reqMsg.has_actionfinish_req_msg())
    {
        return OnActionFinishMsg(seatOrder, reqMsg.actionfinish_req_msg());
    }
    if (reqMsg.has_concede_req_msg())
    {
        return OnConcedeMsg(seatOrder, reqMsg.concede_req_msg());
    }
    if (reqMsg.has_peace_req_msg())
    {
        return OnPeaceMsg(seatOrder, reqMsg.peace_req_msg());
    }
    if (reqMsg.has_peaceres_req_msg())
    {
        return OnPeaceResMsg(seatOrder, reqMsg.peaceres_req_msg());
    }
    if (reqMsg.has_showchess_req_msg())
    {
        return OnShowChessMsg(seatOrder, reqMsg.showchess_req_msg());
    }
    if (reqMsg.has_feel_req_msg())
    {
        return OnFeelMsg(seatOrder, reqMsg.feel_req_msg());
    }
    return false;
}

void CTKJunQiPKData::OnTimeOut(DWORD dwID, ULONG_PTR ulUserData)
{
    auto iter = m_onTimeOut.find(dwID);
    if (iter != m_onTimeOut.end())
    {
        (this->*(iter->second))(ulUserData);
    }
}

void CTKJunQiPKData::OnPlayerNetBreak(int nSeat)
{
    CTKJunQiPlayer* pPlayer = _getPlayer(nSeat);
    if (NULL != pPlayer)
    {
        m_vPlayers[nSeat]->m_bDisConnect = true;
        if (IsLegalPeaceStatus())
        {
            // 判断是否在求和阶段，如果在求和阶段调用求和超时函数
            OnPeaceTimeout(0);
        }
    }
}

void CTKJunQiPKData::OnPlayerNetResume(int nSeat)
{
    CTKJunQiPlayer* pPlayer = _getPlayer(nSeat);
    if (NULL != pPlayer)
    {
        m_vPlayers[nSeat]->m_bDisConnect = false;
    }
}

void CTKJunQiPKData::SendInitRuleAck()
{
    m_nStage = ES_SetRule;
    m_pJunQiPKGame->SetGameTimer(SETRULETIME, 0, m_pJunQiPKCfg->GetCfg(ECI_FriendTime) + 2);
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    InitRuleAck* pAck = pAckMsg->mutable_initrule_ack_msg();
    pAck->set_reserve(1);
    m_pJunQiPKGame->SendMsg(mobilemsg);
}

void CTKJunQiPKData::InitTimeout()
{
    m_onTimeOut[SETRULETIME] = &CTKJunQiPKData::OnSetRuleTimeOut;
    m_onTimeOut[INITWAITTIME] = &CTKJunQiPKData::OnInitTimeOut;
    m_onTimeOut[MOVECHESSWAITTIME] = &CTKJunQiPKData::OnMoveChessTimeout;
    m_onTimeOut[MOVEACTIONWAITTIME] = &CTKJunQiPKData::OnActionTimeout;
    m_onTimeOut[PEACEWAITTIME] = &CTKJunQiPKData::OnPeaceTimeout;
    m_onTimeOut[Max_Round_Timmer] = &CTKJunQiPKData::OnRoundTimeout;
}

void CTKJunQiPKData::_initPlayers()
{
    m_nPlayerCount = m_pJunQiPKGame->GetGamePlayerCount();
    m_vPlayers.clear();
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        auto* pPlayer = dynamic_cast<CTKJunQiPlayer*>(m_pJunQiPKGame->GetGamePlayerBySeat(i));
        pPlayer->InitPlayer(m_pJunQiPKCfg, m_pJunQiPKGame);
        m_vPlayers.push_back(pPlayer);
    }
}

CTKJunQiPlayer* CTKJunQiPKData::_getPlayer(int nSeat)
{
    if (nSeat > 0 && nSeat < m_vPlayers.size())
    {
        return m_vPlayers[nSeat];
    }
    return NULL;
}

bool CTKJunQiPKData::OnInitRuleMsg(int seat, const InitRuleReq& rReq)
{
    if (ES_SetRule != m_nStage || Int_True != m_pJunQiPKCfg->GetCfg(ECI_FriendTable))
    {
        LOG_ERROR("m_nStage = %d", m_nStage);
        return false;
    }
    m_pJunQiPKCfg->SetCfg(ECI_GameType, rReq.gametype());
    m_pJunQiPKCfg->SetCfg(ECI_CleanDiLei, rReq.cleandilei());
    m_pJunQiPKCfg->SetCfg(ECI_CarryJunQi, rReq.carryjunqi());
    SendInitChessAck();
    return true;
}

bool CTKJunQiPKData::OnInitLayoutMsg(int seat, const InitLayoutReq& rReq)
{
    if (ES_Distributing != m_nStage || m_vPlayers[seat]->m_bInitChess)
    {
        LOG_ERROR("m_nStage = %d", m_nStage);
        return false;
    }

    if (m_pJunQiCore->InitLayout(seat, rReq.layout()))
    {
        m_vPlayers[seat]->m_szLayout = rReq.layout();
        m_vPlayers[seat]->m_bInitChess = true;
        m_vPlayers[seat]->m_nHeadImgId = rReq.headimg();
    }
    else
    {
        m_vPlayers[seat]->WriteLog("OnInitLayoutMsg Error");
        FinishGame(seat, EFR_PerishTogether, EFR_PerishTogether, EET_ErrInit, FALSE);
        return false;
    }

    if (IsAllReady())
    {
        _onAllReady();
    }
    else if (Int_True == m_pJunQiPKCfg->GetCfg(ECI_FriendTable))
    {
        sendInitLayoutAck();
    }
    return true;
}

bool CTKJunQiPKData::OnCanCelLayoutMsg(int seat)
{
    if (ES_Distributing != m_nStage || !m_vPlayers[seat]->m_bInitChess)
    {
        LOG_ERROR("m_nStage = %d", m_nStage);
        return false;
    }
    m_vPlayers[seat]->m_bInitChess = false;
    sendInitLayoutAck();
    return true;
}

bool CTKJunQiPKData::OnSelectChessMsg(int seat, const SelectChessReq& rReq)
{
    if (ES_WaitingMoveChess != m_nStage || seat != m_nCurSeat)
    {
        m_vPlayers[seat]->WriteLog("OnSelectChessMsg m_nStage=%d,curseat=%d", m_nStage, m_nCurSeat);
        return false;
    }
    int nAtPos = rReq.pos();
    if (!m_pJunQiCore->CanSelect(nAtPos, seat))
    {
        m_vPlayers[seat]->WriteLog("OnSelectChessMsg nAtPos=%d", nAtPos);
        return false;
    }

    m_nCurSelectPos = nAtPos;
    SendSelectChessAck();
    return true;
}

bool CTKJunQiPKData::OnMoveChessMsg(int seat, const MoveChessReq& rReq)
{
    if (ES_WaitingMoveChess != m_nStage || seat != m_nCurSeat)
    {
        m_vPlayers[seat]->WriteLog("OnMoveChessMsg:m_nStage=%d,curSeat=%d", m_nStage, m_nCurSeat);
        return false;
    }
    if (CheckPass(seat, rReq))
    {
        ++m_nNoFightSteps;
        return true;
    }

    if (rReq.atpos() != m_nCurSelectPos)
    {
        m_vPlayers[seat]->WriteLog("OnMoveChessMsg:SelectPos=%d,movePos=%d", m_nCurSelectPos, rReq.atpos());
        return false;
    }

    vector<int> path;
    if (!m_pJunQiCore->CanMoveTo(rReq.atpos(), rReq.topos(), path))
    {
        ostringstream ss;
        for (auto iter = m_curCanMovePos.begin(); iter != m_curCanMovePos.end(); ++iter)
        {
            ss << *iter << ",";
        }
        int nOtherSeat = (seat + 1) % m_nPlayerCount;
        int nPos = m_pJunQiCore->GetChessPos(nOtherSeat, ECT_DiLei);

        m_vPlayers[seat]->WriteLog("OnMoveChessMsg:atPos=%d,toPos=%d,dilei=%d,step=%d,path=%s", rReq.atpos(),
                                   rReq.topos(), nPos, m_nStepsMoved, ss.str().c_str());
        return false;
    }

    MoveChess(seat, rReq, path);
    return true;
}

void CTKJunQiPKData::MoveChess(int nSeat, const MoveChessReq& rReq, vector<int>& path)
{
    std::shared_ptr<SChess> pFrom = m_pJunQiCore->GetChess(rReq.atpos());
    if (NULL == pFrom.get())
    {
        LOG_ERROR("pFrom = NULL");
        return;
    }

    int nResult = JUNQIPK_UNKNOWN;
    int nEvent = JUNQIPK_UNKNOWN;
    std::shared_ptr<SChess> pTo = m_pJunQiCore->GetChess(rReq.topos());
    if (NULL == pTo.get())
    {
        if (IsNeedConfirm(nSeat, rReq, pFrom->index))
        {
            return;
        }
        ++m_nNoFightSteps;
        int nOtherSeat = (nSeat + 1) % m_nPlayerCount;
        m_vPlayers[nSeat]->AddMoveStep(rReq.atpos(), pFrom->index, rReq.topos(), JUNQIPK_UNKNOWN,
                                       m_vPlayers[nOtherSeat]->m_nLastIndex);
    }
    else
    {
        m_nNoFightSteps = 0;
        Fight(pFrom, pTo, nResult, nEvent);
        m_vPlayers[nSeat]->AddMoveStep(rReq.atpos(), pFrom->index, rReq.topos(), pTo->index, JUNQIPK_UNKNOWN);
    }

    m_pJunQiCore->MoveChess(rReq.atpos(), rReq.topos(), nResult);

    SetWaitAction();
    if (m_vPlayers[m_nCurSeat]->SetMoveFinish(false))
    {
        SendTips(ETS_DelayTimePulish, m_nCurSeat, m_vPlayers[m_nCurSeat]->GetCurMoveTime());
    }
    SendMoveChessAck(pFrom->type, nResult, nEvent, path);
}

bool CTKJunQiPKData::IsNeedConfirm(int nSeat, const MoveChessReq& rReq, int nChessIndex)
{
    int nOtherSeat = (nSeat + 1) % m_nPlayerCount;
    vector<int> vPath;
    int nResult = JUNQIPK_UNKNOWN;
    m_pJunQiCore->MoveChess(rReq.atpos(), rReq.topos(), nResult);
    m_vPlayers[nSeat]->m_bChase = m_pJunQiCore->CanMoveTo(rReq.topos(), m_vPlayers[nOtherSeat]->m_nLastToPos, vPath);
    m_pJunQiCore->MoveChess(rReq.topos(), rReq.atpos(), nResult);

    if (rReq.isconfirm())
    {
        return false;
    }

    if (!m_vPlayers[nSeat]->CanMoveTo(rReq.atpos(), nChessIndex, rReq.topos(), m_pJunQiPKCfg->GetCfg(ECI_RepeatStep)))
    {
        m_vPlayers[nSeat]->m_bChase = false;
        SendRepeatCount(nSeat, m_pJunQiPKCfg->GetCfg(ECI_RepeatStep), nSeat);
        return true;
    }

    if (!m_vPlayers[nSeat]->CanChaseMoveTo(nChessIndex, m_vPlayers[nOtherSeat]->m_nLastIndex, rReq.topos()))
    {
        m_vPlayers[nSeat]->m_bChase = false;
        SendRepeatCount(nSeat, m_pJunQiPKCfg->GetCfg(ECI_RepeatStep), nSeat);
        return true;
    }
    return false;
}

bool CTKJunQiPKData::CheckPass(int seat, const MoveChessReq& rReq)
{
    if (JUNQIPK_UNKNOWN != rReq.atpos())
    {
        return false;
    }

    SetWaitAction();
    if (m_vPlayers[m_nCurSeat]->SetMoveFinish(true))
    {
        SendTips(ETS_DelayTimePulish, m_nCurSeat, m_vPlayers[m_nCurSeat]->GetCurMoveTime());
    }
    SendTimeOutAck(seat);

    vector<int> path;
    SendMoveChessAck(JUNQIPK_UNKNOWN, JUNQIPK_UNKNOWN, EFE_Timeout, path);
    return true;
}

void CTKJunQiPKData::SendTimeOutAck(int nSeat)
{
    ++m_vPlayers[nSeat]->m_nPassCount;
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    TimeOutAck* pAck = pAckMsg->mutable_timeout_ack_msg();
    pAck->set_seat(nSeat);
    pAck->set_count(m_vPlayers[nSeat]->m_nPassCount);
    pAck->set_reducetime(m_vPlayers[nSeat]->m_nCurMoveTime);
    m_pJunQiPKGame->SendMsg(mobilemsg);
}

void CTKJunQiPKData::SendMoveChessAck(int nType, int nResult, int nEvent, vector<int>& path)
{
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    MoveChessAck* pAck = pAckMsg->mutable_movechess_ack_msg();
    pAck->set_seat(m_nCurSeat);
    pAck->set_result(nResult);
    pAck->set_event(nEvent);
    for (auto iter = path.begin(); iter != path.end(); ++iter)
    {
        pAck->add_path(*iter);
    }
    pAck->set_step(m_nStepsMoved);
    pAck->set_chesstype(nType);
    m_pJunQiPKGame->SendMsg(mobilemsg);

    RecordMoveChess(nType, nResult, nEvent, path);
}

void CTKJunQiPKData::Fight(std::shared_ptr<SChess> pFrom, std::shared_ptr<SChess> pTo, int& nResult, int& nEvent)
{
    nResult = m_pJunQiCore->GetFightResult(pFrom->type, pTo->type);
    map<int, int> mapChess;
    if (EFR_Loss != nResult)
    {
        mapChess[pFrom->seat] = pTo->type;
    }
    if (EFR_Win != nResult)
    {
        mapChess[pTo->seat] = pFrom->type;
    }
    for (auto iter = mapChess.begin(); iter != mapChess.end(); ++iter)
    {
        ChessDead(iter->first, iter->second, nEvent);
    }
}

void CTKJunQiPKData::ChessDead(int nKillSeat, int nChessType, int& nEvent)
{
    m_vPlayers[nKillSeat]->AddGrowValue(EG_DayDesChessCount);
    if (ECT_SiLing == nChessType)
    {
        nEvent = EFE_SiLingDead;
        m_vPlayers[nKillSeat]->AddGrowValue(EG_DayDesSiLingCount);
    }
    if (ECT_DiLei == nChessType)
    {
        m_vPlayers[nKillSeat]->AddGrowValue(EG_DayDesDiLeiCount);
    }
    if (ECT_JunQi == nChessType)
    {
        nEvent = EFE_JunQiDead;
    }
}

bool CTKJunQiPKData::OnActionFinishMsg(int seat, const AcotionFinishReq& rReq)
{
    // 有一家走完即可进入下一步
    if (m_nStage != ES_WaitingActionComplete)
    {
        return true;
    }
    vector<int> vOverSeat;
    _getOverSeat(vOverSeat);
    if (!IsGameOver())
    {
        int nRepeatCount = 0;
        if (m_vPlayers[m_nCurSeat]->CheckRepeat(m_pJunQiPKCfg->GetCfg(ECI_RepeatStep), nRepeatCount))
        {
            SendRepeatCount(m_nCurSeat, nRepeatCount);
        }
        SendActionFinishAck(vOverSeat);
    }
    return true;
}

void CTKJunQiPKData::GetSLDeadSeat(vector<int>& vSLDeadSeat)
{
    if (m_pJunQiCore->NeedShowJunQi())
    {
        for (int i = 0; i < m_nPlayerCount; ++i)
        {
            if (m_vPlayers[i]->m_bShowJunQi)
            {
                continue;
            }
            if (JUNQIPK_UNKNOWN == m_pJunQiCore->GetChessPos(i, ECT_SiLing))
            {
                vSLDeadSeat.push_back(i);
            }
        }
    }
}

void CTKJunQiPKData::_getOverSeat(vector<int>& vOverSeat)
{
    int nextSeat = _getNextSeat(m_nCurSeat);
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (EFR_Loss == m_vPlayers[i]->m_nResult)
        {
            continue;
        }
        int nTips = JUNQIPK_UNKNOWN;
        if (!_isLoss(i, nTips))
        {
            if (i != nextSeat)
            {
                continue;
            }

            if (m_pJunQiCore->HasCanMoveChess(nextSeat))
            {
                continue;
            }
            nTips = ETS_UnMoveAbleLoss;
        }

        SendTips(nTips, i);
        vOverSeat.push_back(i);
        m_vPlayers[i]->m_nResult = EFR_Loss;
        m_pJunQiCore->ClearChess(i);
        _recordCleanChess(i, nTips);
        _calcNegativeState(i, false);
    }
}

bool CTKJunQiPKData::_isLoss(int nSeat, int& nTips)
{
    if (m_vPlayers[nSeat]->IsLoss(nTips))
    {
        return true;
    }
    if (!m_bColorSet)
    {
        return false;
    }
    return m_pJunQiCore->IsOver(nTips, nSeat);
}

void CTKJunQiPKData::SendActionFinishAck(const vector<int>& vOverSeat)
{
    m_nCurSeat = _getNextSeat(m_nCurSeat);
    m_vPlayers[m_nCurSeat]->SetStartTime();

    m_nStage = ES_WaitingMoveChess;
    m_pJunQiPKGame->KillGameTimer(MOVECHESSWAITTIME);
    m_pJunQiPKGame->KillGameTimer(MOVEACTIONWAITTIME);
    m_pJunQiPKGame->SetGameTimer(MOVECHESSWAITTIME, m_nCurSeat, m_vPlayers[m_nCurSeat]->m_nCurMoveTime + 2);
    // 更新上一手动作时间
    time_t temp = time(NULL);
    m_tCurRemainTime = temp;
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    ActionFinishAck* pAck = pAckMsg->mutable_actionfinish_ack_msg();
    pAck->set_nextseat(m_nCurSeat);
    pAck->set_movetime(m_vPlayers[m_nCurSeat]->m_nCurMoveTime);

    vector<int> vSLDeadSeat;
    GetSLDeadSeat(vSLDeadSeat);
    for (auto iter = vSLDeadSeat.begin(); iter != vSLDeadSeat.end(); ++iter)
    {
        m_vPlayers[*iter]->m_bShowJunQi = true;
        Chess* pChess = pAck->add_chess();
        pChess->set_pos(m_pJunQiCore->GetChessPos(*iter, ECT_JunQi));
        pChess->set_type(ECT_JunQi);
        pChess->set_color(*iter);  // 暗棋时，客户端根据座位号设置颜色
    }

    for (auto iter = vOverSeat.begin(); iter != vOverSeat.end(); ++iter)
    {
        pAck->add_overseat(*iter);
    }
    m_pJunQiPKGame->SendMsg(mobilemsg);
}

bool CTKJunQiPKData::IsGameOver()
{
    int nCurAlly = m_nCurSeat % 2;
    if (_isAllyWin(nCurAlly))
    {
        FinishGame(m_nCurSeat, EFR_Win, EFR_Loss, EET_Normal);
        return true;
    }
    int nOtherAlly = (nCurAlly + 1) % 2;
    if (_isAllyWin(nOtherAlly))
    {
        FinishGame(m_nCurSeat, EFR_Loss, EFR_Win, EET_Normal);
        return true;
    }

    int nTips = 0;
    int nEndType = EET_Normal;
    if (IsPeace(nTips, nEndType))
    {
        SendTips(nTips);
        FinishGame(m_nCurSeat, EFR_PerishTogether, EFR_PerishTogether, nEndType);
        return true;
    }
    return false;
}

bool CTKJunQiPKData::_isAllyWin(int nAlly)
{
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (i % 2 != nAlly)
        {
            if (EFR_Loss != m_vPlayers[i]->m_nResult)
            {
                return false;
            }
        }
    }
    return true;
}

bool CTKJunQiPKData::IsPeace(int& nTips, int& nEndType)
{
    if (m_nNoFightSteps >= m_pJunQiPKCfg->GetCfg(ECI_NoFightStep))
    {
        nTips = ETS_UnFightDraw;
        nEndType = EET_UnFight;
        return true;
    }

    if (m_nStepsMoved >= m_pJunQiPKCfg->GetCfg(ECI_TotalStep))
    {
        nTips = ETS_TotalStepDraw;
        nEndType = EET_MaxStep;
        return true;
    }

    if (m_nNoFightSteps == m_pJunQiPKCfg->GetCfg(ECI_NoFightStep) - 10)
    {
        SendTips(ETS_UnFightTips);
    }
    else if (m_nStepsMoved == m_pJunQiPKCfg->GetCfg(ECI_TotalStep) - 30)
    {
        SendTips(ETS_TotalStepTips);
    }
    return false;
}

void CTKJunQiPKData::FinishGame(int nSeat, int nCurResult, int nEnemyResult, int nEndType, BOOL bValidGame)
{
    _calcRankDiff();
    _setEndResult(nSeat, nCurResult, nEnemyResult);
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (bValidGame)
        {
            time_t nTime = time(NULL) - m_pJunQiPKGame->m_timeGameBegin;
            m_vPlayers[i]->SaveGrowValue(nTime);
        }
        SetGameResult(i, m_vPlayers[i]->m_nResult, bValidGame);
    }
    SendFinishGameAck(nEndType);
    m_pJunQiPKGame->KillGameTimer(Max_Round_Timmer);
    m_pJunQiPKGame->NotifyGameOver(TK_GAMEOVERTYPE_NORNAL);
    _sendNegativeAtomEvent();
    _statisticDataLog(nEndType);
}

void CTKJunQiPKData::_calcRankDiff()
{
    int nScore[] = {0, 0};  // 保存联盟积分和
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        nScore[i % 2] += m_vPlayers[i]->GetRankValue();
    }

    int nDiffScore = nScore[0] - nScore[1];
    int nBigAlly = 0;
    if (nScore[0] < nScore[1])
    {
        nBigAlly = 1;
        nDiffScore = nScore[1] - nScore[0];
    }
    int nAddScore = _getAddScore(nDiffScore);

    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_vPlayers[i]->SetRankDiff(nBigAlly, nAddScore);
    }
}

int CTKJunQiPKData::_getAddScore(int nDiffScore)
{
    // g_SiRankDiff
    int nGameType = m_pJunQiPKCfg->GetCfg(ECI_GameType);

    if (GT_SIGUOSIAN == nGameType || GT_SHUANGMING == nGameType)
    {
        for (int i = 0; i < _ARRAYSIZE(g_SiRankDiff); ++i)
        {
            if (nDiffScore < g_SiRankDiff[i][0])
            {
                return g_SiRankDiff[i][1];
            }
        }
    }
    else
    {
        for (int i = 0; i < ARRAYSIZE(g_RankDiff); ++i)
        {
            if (nDiffScore < g_RankDiff[i])
            {
                return i;
            }
        }
    }

    return 0;
}

void CTKJunQiPKData::_setEndResult(int nSeat, int nCurResult, int nEnemyResult)
{
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (i % 2 == nSeat % 2)
        {
            m_vPlayers[i]->SetEndResult(m_nStepsMoved, nCurResult);
        }
        else
        {
            m_vPlayers[i]->SetEndResult(m_nStepsMoved, nEnemyResult);
        }
    }
}

void CTKJunQiPKData::SetGameResult(int nSeat, int nResult, BOOL bValidGame)
{
    int nWin = EFR_Win == nResult ? 1 : 0;
    int nLoss = EFR_Loss == nResult ? 1 : 0;
    int nDraw = EFR_PerishTogether == nResult ? 1 : 0;
    // PK赛，闯关赛是通过积分来判断玩家胜负平的
    int nScorePlus = 0;
    WORD wGameResult = TK_DRAW;
    if (EFR_Win == nResult)
    {
        wGameResult = TK_WIN;
        nScorePlus = 1;
    }
    else if (EFR_Loss == nResult)
    {
        wGameResult = TK_LOSS;
        nScorePlus = -1;
    }

    m_pJunQiPKGame->SetGameResult(nSeat, nWin, nLoss, nDraw, nScorePlus, bValidGame, FALSE, wGameResult);
}

void CTKJunQiPKData::_statisticDataLog(int resultType)
{
    if (GT_FANFANQI != m_pJunQiPKCfg->GetCfg(ECI_GameType) && GT_ERRENANQI != m_pJunQiPKCfg->GetCfg(ECI_GameType))
    {
        return;
    }

    std::ostringstream os;
    os << "{\"gameID\":" << TK_GAMEID_JUNQIPK << ",\"PID\":" << m_pJunQiPKGame->m_nProductID;

    switch (resultType)
    {
    case EET_MaxStep: {
        os << ",\"MaxStep\":" << 1;
    }
    break;
    case EET_UnFight: {
        os << ",\"UnFight\":" << 1;
    }
    break;
    case EET_Peace: {
        os << ",\"Peace\":" << 1;
        if (JUNQIPK_UNKNOWN != m_nPeaceSeat)
        {
            os << ",\"ReqPeaceUID\":" << m_vPlayers[m_nPeaceSeat]->m_dwUserID;
        }
    }
    break;
    case EET_Concede: {
        for (int i = 0; i < m_nPlayerCount; ++i)
        {
            if (EFR_Loss == m_vPlayers[i]->m_nResult)
            {
                os << ",\"ReqConcedeUID\":" << m_vPlayers[i]->m_dwUserID;
                break;
            }
        }
    }
    break;
    case EET_Normal: {
        int maxPassCount = m_pJunQiPKCfg->GetCfg(ECI_PassCount);
        for (int i = 0; i < m_nPlayerCount; ++i)
        {
            if (EFR_Loss == m_vPlayers[i]->m_nResult && m_vPlayers[i]->m_nPassCount >= maxPassCount)
            {
                os << ",\"PassCountUID\":" << m_vPlayers[i]->m_dwUserID;
                break;
            }
        }
    }
    break;
    default:
        break;
    }

    int delayPlayerNum = 0;
    os << ",\"DelayTimeUID\":[";
    for (int seat = 0; seat < m_nPlayerCount; ++seat)
    {
        if (m_vPlayers[seat]->m_nDelayCount > 0)
        {
            if (delayPlayerNum > 0)
            {
                os << ",";
            }
            os << m_vPlayers[seat]->m_dwUserID;
            delayPlayerNum++;
        }
    }
    os << "]}";

    time_t now = time(NULL);
    tm* tm_t = localtime(&now);

    char strTime[512] = {0};
    sprintf_s(strTime, 512, "%d%02d%02d", tm_t->tm_year + 1900, tm_t->tm_mon + 1, tm_t->tm_mday);

    std::string strFileName = "";
    strFileName += "./logagent//gameserver_" + std::to_string(TK_GAMEID_JUNQIPK) + "_" + std::string(strTime) + ".log";

    TKWriteLogFile(strFileName.c_str(), "%s", os.str().c_str());
}

void CTKJunQiPKData::SendFinishGameAck(int nEndType)
{
    RecordResult();

    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    FinishGameAck* pAck = pAckMsg->mutable_finishgame_ack_msg();
    pAck->add_urlparas(m_pJunQiPKGame->m_dwTourneyID);
    pAck->add_urlparas(m_pJunQiPKGame->m_dwMatchID);
    pAck->add_urlparas(m_pJunQiPKGame->m_wStageID);
    pAck->add_urlparas(m_pJunQiPKGame->m_wRoundID);
    pAck->add_urlparas(m_pJunQiPKGame->m_timeGameBegin);
    pAck->set_endtype(nEndType);

    int nLoseNegative[] = {0, 0, 0, 0};
    if (m_pJunQiPKCfg->GetCfg(ECI_GameType) == GT_SIGUOSIAN || m_pJunQiPKCfg->GetCfg(ECI_GameType) == GT_SHUANGMING)
    {
        for (int i = 0; i < m_nPlayerCount; ++i)
        {
            if (ENT_NOT_NEGATIVE != m_vPlayers[i]->m_nNegativeState)
            {
                nLoseNegative[i] = 1;  // 自己消极比赛
            }
            else if (ENT_NOT_NEGATIVE != m_vPlayers[i]->m_nTeamNegativeState)
            {
                nLoseNegative[i] = 2;  // 队友消极比赛
            }
        }
    }

    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        pAck->add_result(m_vPlayers[i]->m_nResult);
        pAck->add_layout(m_vPlayers[i]->m_szLayout);
        pAck->add_honor(0);
        pAck->add_grade(m_vPlayers[i]->m_nRankChange);
        pAck->add_usedware(m_vPlayers[i]->m_nUserWare);
        pAck->add_timeoutcost(nLoseNegative[i]);
    }

    m_pJunQiPKGame->SendMsg(mobilemsg);
}

bool CTKJunQiPKData::OnConcedeMsg(int seat, const ConcedeReq& rReq)
{
    if (m_nStage == ES_SetRule)
    {
        m_pJunQiPKGame->NotifyGameOver(TK_GAMEOVERTYPE_NORNAL);
        return true;
    }
    if (m_nStepsMoved < m_pJunQiPKCfg->GetCfg(ECI_ConcedeStep))
    {
        return false;
    }
    if (EFR_Loss == m_vPlayers[seat]->m_nResult)
    {
        return false;
    }
    // 若在求和阶段，则不允许认输，需结束求和阶段才可认输
    if (m_nStage == ES_WaitingPeace)
    {
        return false;
    }

    m_vPlayers[seat]->WriteLog("OnConcedeMsg");
    SendTips(ETS_ConcedeLoss, seat);
    m_vPlayers[seat]->m_nResult = EFR_Loss;
    m_pJunQiCore->ClearChess(seat);
    _recordCleanChess(seat, ETS_ConcedeLoss);

    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    ConcedeAck* pAck = pAckMsg->mutable_concede_ack_msg();
    pAck->set_seat(seat);
    m_pJunQiPKGame->SendMsg(mobilemsg);

    _calcNegativeState(seat, true);

    int nOtherAlly = (seat + 1) % 2;
    if (_isAllyWin(nOtherAlly))
    {
        FinishGame(seat, EFR_Loss, EFR_Win, EET_Concede);
    }
    else if (seat == m_nCurSeat && m_nStage == ES_WaitingMoveChess)
    {
        vector<int> vOverSeat;
        SendActionFinishAck(vOverSeat);
    }
    return true;
}

bool CTKJunQiPKData::OnPeaceMsg(int seat, const PeaceReq& rReq)
{
    if (ES_SetRule == m_nStage)
    {
        return false;
    }

    if (EFR_Loss == m_vPlayers[seat]->m_nResult)
    {
        return false;
    }
    if (JUNQIPK_UNKNOWN != m_nPeaceSeat)
    {
        LOG_ERROR("PeaceSeat = %d", m_nPeaceSeat);
        return false;
    }
    if (m_nStepsMoved < m_pJunQiPKCfg->GetCfg(ECI_PeaceStep))
    {
        return false;
    }

    if (!m_vPlayers[seat]->CanPeace(m_pJunQiPKCfg->GetCfg(ECI_PeaceCount)))
    {
        return false;
    }

    m_vPlayers[seat]->WriteLog("PeaceReq");
    m_nPeaceSeat = seat;
    SendPeaceAck();
    return true;
}

void CTKJunQiPKData::SendPeaceAck()
{
    m_pJunQiPKGame->SetGameTimer(PEACEWAITTIME, 0, m_pJunQiPKCfg->GetCfg(ECI_PeaceTime) + 2);
    // TODO:保存现场 回合暂停 将目前所有的计时器停止并保存计时
    m_nPeaceStatus = m_nStage;
    m_nStage = ES_WaitingPeace;
    m_pJunQiPKGame->KillGameTimer(MOVECHESSWAITTIME);
    m_pJunQiPKGame->KillGameTimer(MOVEACTIONWAITTIME);
    // 更新上一手动作时间为从倒计时开始到求和所经过的时间
    time_t temp = time(NULL);
    m_tCurRemainTime = temp - m_tCurRemainTime;

    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    PeaceAck* pAck = pAckMsg->mutable_peace_ack_msg();
    pAck->set_seat(m_nPeaceSeat);

    m_pJunQiPKGame->SendMsg(mobilemsg);
}

void CTKJunQiPKData::SendTips(int nTips, int nSeat, int reducedTime)
{
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    ShowTipsAck* pAck = pAckMsg->mutable_showtips_ack_msg();
    pAck->set_seat(nSeat);
    pAck->set_tips(nTips);
    if (ETS_ConcedeLoss == nTips)
    {
        for (int i = 0; i < m_nPlayerCount; ++i)
        {
            if (nSeat != i)
            {
                m_pJunQiPKGame->SendMsg(mobilemsg, i);
            }
        }
    }
    else if (ETS_DelayTimePulish == nTips)
    {
        pAck->set_reducetime(reducedTime);
        m_pJunQiPKGame->SendMsg(mobilemsg);
    }
    else
    {
        m_pJunQiPKGame->SendMsg(mobilemsg);
    }
}

void CTKJunQiPKData::SendRepeatCount(int nRepeatSeat, int nCount, int nSeat)
{
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    RepeatCountAck* pAck = pAckMsg->mutable_repeatcount_ack_msg();
    pAck->set_seat(nRepeatSeat);
    pAck->set_count(nCount);
    m_pJunQiPKGame->SendMsg(mobilemsg, nSeat);
}

bool CTKJunQiPKData::IsLegalPeaceStatus()
{
    return m_nPeaceStatus > ES_Distributing && m_nPeaceStatus < ES_SetRule;
}

bool CTKJunQiPKData::OnPeaceResMsg(int seat, const PeaceResReq& rReq)
{
    if (JUNQIPK_UNKNOWN == m_nPeaceSeat || seat == m_nPeaceSeat)
    {
        return false;
    }
    if (!IsLegalPeaceStatus())
    {
        LOG_ERROR("m_nPeaceStatus = %d", m_nPeaceStatus);
        return false;
    }
    if (EFR_Loss == m_vPlayers[seat]->m_nResult || m_nStage != ES_WaitingPeace)
    {
        LOG_ERROR("illegal seat or illegal m_nStage");
        return false;
    }
    if (!rReq.agree())
    {
        int second = 0;
        time_t temp = time(NULL);
        switch (m_nPeaceStatus)
        {
        case ES_WaitingMoveChess:
            m_nStage = ES_WaitingMoveChess;
            second = m_vPlayers[m_nCurSeat]->m_nCurMoveTime + 2;
            if (second <= m_tCurRemainTime)
            {
                second = 0;
            }
            else
            {
                second -= m_tCurRemainTime;
            }
            // 更新时间戳，同时减少从计时开始到求和的时间，使得下次求和时保留的时长正常
            m_tCurRemainTime = temp - m_tCurRemainTime;

            m_pJunQiPKGame->SetGameTimer(MOVECHESSWAITTIME, m_nCurSeat, second);
            break;
        case ES_WaitingActionComplete:
            m_nStage = ES_WaitingActionComplete;
            second = m_pJunQiPKCfg->GetCfg(ECI_MoveActionTime) + 2;
            if (second <= m_tCurRemainTime)
            {
                second = 0;
            }
            else
            {
                second -= m_tCurRemainTime;
            }
            // 更新时间戳，同时减少从计时开始到求和的时间，使得下次求和时保留的时长正常
            m_tCurRemainTime = temp - m_tCurRemainTime;
            m_pJunQiPKGame->SetGameTimer(MOVEACTIONWAITTIME, m_nCurSeat, second);
            break;
        default:
            LOG_ERROR("m_nPeaceStatus = %d", m_nPeaceStatus);
            return false;
        }
        m_nPeaceStatus = 0;  // 求和状态初始化
        _sendRefusePeaceAck(seat);
    }
    else
    {
        _procAgreePeace(seat);
    }
    return true;
}

void CTKJunQiPKData::_procAgreePeace(int seat)
{
    m_vPlayers[seat]->WriteLog("OnPeaceResMsg agree");
    m_vPlayers[seat]->m_bPeace = true;
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (i == m_nPeaceSeat || EFR_Loss == m_vPlayers[i]->m_nResult)
        {
            continue;
        }
        if (!m_vPlayers[i]->m_bPeace)
        {
            return;
        }
    }
    FinishGame(m_nPeaceSeat, EFR_PerishTogether, EFR_PerishTogether, EET_Peace);
}

void CTKJunQiPKData::_sendRefusePeaceAck(int seat)
{
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        m_vPlayers[i]->m_bPeace = false;
    }

    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    RefusePeaceAck* pAck = pAckMsg->mutable_refusepeace_ack_msg();
    pAck->set_count(m_vPlayers[m_nPeaceSeat]->m_nPeaceCount);
    pAck->set_disseat(seat);
    pAck->set_seat(m_nPeaceSeat);

    m_pJunQiPKGame->SendMsg(mobilemsg);
    m_pJunQiPKGame->KillGameTimer(PEACEWAITTIME);
    m_nPeaceSeat = JUNQIPK_UNKNOWN;
}

bool CTKJunQiPKData::OnShowChessMsg(int seat, const ShowChessReq& rReq)
{
    if (ES_WaitingMoveChess != m_nStage || seat != m_nCurSeat)
    {
        m_vPlayers[seat]->WriteLog("OnShowChessMsg:m_nStage=%d,curseat=%d", m_nStage, m_nCurSeat);
        return false;
    }
    std::shared_ptr<SChess> pChess;
    if (!m_pJunQiCore->ShowChess(rReq.atpos(), pChess))
    {
        m_vPlayers[seat]->WriteLog("OnShowChessMsg:pos=%d", rReq.atpos());
        return false;
    }

    m_vPlayers[seat]->AddMoveStep(rReq.atpos(), pChess->index, JUNQIPK_UNKNOWN, JUNQIPK_UNKNOWN, JUNQIPK_UNKNOWN);
    ++m_nNoFightSteps;
    SetWaitAction();
    if (m_vPlayers[m_nCurSeat]->SetMoveFinish(false))
    {
        SendTips(ETS_DelayTimePulish, m_nCurSeat, m_vPlayers[m_nCurSeat]->GetCurMoveTime());
    }
    SendShowChessAck(rReq.atpos(), pChess, seat);
    return true;
}

bool CTKJunQiPKData::OnFeelMsg(int seat, const FeelReq& rReq)
{
    if (!m_vPlayers[seat]->CheckFeel(rReq.to()))
    {
        return false;
    }
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    FeelAck* pAck = pAckMsg->mutable_feel_ack_msg();
    pAck->set_from(rReq.from());
    pAck->set_to(rReq.to());
    pAck->set_feel(rReq.feel());
    pAck->set_str(rReq.str());
    m_pJunQiPKGame->SendMsg(mobilemsg);
    return true;
}

void CTKJunQiPKData::SendShowChessAck(int nPos, std::shared_ptr<SChess> pChess, int seat)
{
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    ShowChessAck* pAck = pAckMsg->mutable_showchess_ack_msg();
    Chess* pAckChess = pAck->mutable_chess();
    pAckChess->set_pos(nPos);
    pAckChess->set_type(pChess->type);
    pAckChess->set_color(pChess->color);
    pAckChess->set_seat(seat);
    pAck->set_step(m_nStepsMoved);
    CheckColorSet(m_nCurSeat, pChess->color, pAck);
    RecordShowChess(pAck);
    m_pJunQiPKGame->SendMsg(mobilemsg);
}

void CTKJunQiPKData::OnSetRuleTimeOut(ULONG_PTR ulUserData)
{
    if (ES_SetRule != m_nStage)
    {
        return;
    }
    LOG_ERROR("FinishGame by OnSetRuleTimeOut");
    m_pJunQiPKGame->NotifyGameOver(TK_GAMEOVERTYPE_NORNAL);
}

void CTKJunQiPKData::SetWaitAction()
{
    ++m_nStepsMoved;
    m_nCurSelectPos = JUNQIPK_UNKNOWN;
    m_nStage = ES_WaitingActionComplete;
    m_pJunQiPKGame->KillGameTimer(MOVECHESSWAITTIME);
    m_pJunQiPKGame->SetGameTimer(MOVEACTIONWAITTIME, m_nCurSeat, m_pJunQiPKCfg->GetCfg(ECI_MoveActionTime) + 2);
    // 更新上一手动作时间戳
    time_t temp = time(NULL);
    m_tCurRemainTime = temp;
}

void CTKJunQiPKData::CheckColorSet(int nSeat, int nColor, ShowChessAck* pAck)
{
    if (m_bColorSet)
    {
        return;
    }
    if (TwoConfirmColor != m_pJunQiPKCfg->GetCfg(ECI_ConfirmColor))
    {
        ConfirmColor(nSeat, nColor, pAck);
    }
    else if (m_vPlayers[nSeat]->m_nLastColor != nColor)
    {
        m_vPlayers[nSeat]->m_nLastColor = nColor;
    }
    else
    {
        ConfirmColor(nSeat, nColor, pAck);
    }
}

void CTKJunQiPKData::ConfirmColor(int nSeat, int nColor, ShowChessAck* pAck)
{
    m_bColorSet = true;

    int nOtherSeat = (nSeat + 1) % m_nPlayerCount;
    int nOtherColor = (nColor + 1) % m_nPlayerCount;
    if (TwoConfirmColor == m_pJunQiPKCfg->GetCfg(ECI_ConfirmColor))
    {
        m_vPlayers[nOtherSeat]->m_nLastColor = nOtherColor;
    }
    else
    {
        m_vPlayers[nOtherSeat]->m_nLastColor = nColor;
        m_vPlayers[nSeat]->m_nLastColor = nOtherColor;
    }
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        pAck->add_color(m_vPlayers[i]->m_nLastColor);
        if (m_pJunQiCore)
        {
            m_pJunQiCore->SetPlayerColor(i, m_vPlayers[i]->m_nLastColor);
        }
    }
    if (m_pJunQiCore)
    {
        m_pJunQiCore->SwapChessWhenColorConfirm(m_vPlayers);
    }
}

void CTKJunQiPKData::_creatJunQiCore()
{
    int nGameType = m_pJunQiPKCfg->GetCfg(ECI_GameType);
    switch (nGameType)
    {
    case GT_FANFANQI:  // 二人翻翻棋
    {
        m_bColorSet = false;
        m_pJunQiCore = std::make_shared<CTKJunQiPKFFQ>(m_pJunQiPKCfg);
        break;
    }
    case GT_SIGUOSIAN:   // 四国四暗
    case GT_SHUANGMING:  // 四国双明
    {
        m_pJunQiCore = std::make_shared<CTKJunQiSiGuo>(m_pJunQiPKCfg);
        break;
    }
    default:  // 二人暗棋
    {
        m_pJunQiCore = std::make_shared<CTKJunQiPKAQ>(m_pJunQiPKCfg);
        break;
    }
    }
    m_pJunQiCore->OnInitialUpdate();
}

void CTKJunQiPKData::_getAllyLayout(int nSeat, ostringstream& ss)
{
    if (GT_SHUANGMING != m_pJunQiPKCfg->GetCfg(ECI_GameType))
    {
        return;
    }
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (i % 2 == nSeat % 2 && i != nSeat)
        {
            ss << "," << m_vPlayers[i]->m_szLayout;
        }
    }
}

int CTKJunQiPKData::_getNextSeat(int nSeat)
{
    nSeat = (nSeat + 1) % m_nPlayerCount;
    while (JUNQIPK_UNKNOWN != m_vPlayers[nSeat]->m_nResult)
    {
        nSeat = (nSeat + 1) % m_nPlayerCount;
    }
    return nSeat;
}

void CTKJunQiPKData::OnInitTimeOut(ULONG_PTR ulUserData)
{
    if (ES_Distributing != m_nStage)
    {
        return;
    }

    if (m_pJunQiCore->InitContinue())
    {
        _onAllReady();
        return;
    }
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (!m_vPlayers[i]->m_bInitChess)
        {
            m_vPlayers[i]->WriteLog("OnInitTimeOut");
            FinishGame(i, EFR_PerishTogether, EFR_PerishTogether, EET_ErrInit, FALSE);
            break;
        }
    }
}

void CTKJunQiPKData::OnMoveChessTimeout(ULONG_PTR ulUserData)
{
    if (ES_WaitingMoveChess != m_nStage)
    {
        return;
    }
    MoveChessReq rReq;
    rReq.set_seat((int)ulUserData);
    rReq.set_atpos(JUNQIPK_UNKNOWN);
    rReq.set_topos(JUNQIPK_UNKNOWN);

    OnMoveChessMsg((int)ulUserData, rReq);
}

void CTKJunQiPKData::OnActionTimeout(ULONG_PTR ulUserData)
{
    if (ES_WaitingActionComplete != m_nStage)
    {
        return;
    }
    AcotionFinishReq rReq;
    OnActionFinishMsg((int)ulUserData, rReq);
}

void CTKJunQiPKData::OnPeaceTimeout(ULONG_PTR ulUserData)
{
    if (JUNQIPK_UNKNOWN == m_nPeaceSeat)
    {
        return;
    }
    int nSeat = JUNQIPK_UNKNOWN;
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (i == m_nPeaceSeat)
        {
            continue;
        }
        if (!m_vPlayers[i]->m_bPeace)
        {
            nSeat = i;
            break;
        }
    }
    PeaceResReq rReq;
    rReq.set_seat(m_nPeaceSeat);
    OnPeaceResMsg(nSeat, rReq);
}

void CTKJunQiPKData::OnRoundTimeout(ULONG_PTR ulUserData)
{
    LOG_ERROR("FinishGame by OnRoundTimeout");
    for (int i = 0; i < m_nPlayerCount; i++)
    {
        m_vPlayers[i]->WriteLog("OnRoundTimeout");
    }
    m_pJunQiPKGame->NotifyGameOver(TK_GAMEOVERTYPE_FORCEREGROUP);
}

void CTKJunQiPKData::SendInitChessAck()
{
    _creatJunQiCore();

    m_nStage = ES_Distributing;
    m_pJunQiPKGame->KillGameTimer(SETRULETIME);
    int nTimeLeigth = Int_True == m_pJunQiPKCfg->GetCfg(ECI_FriendTable) ? m_pJunQiPKCfg->GetCfg(ECI_FriendTime)
                                                                         : m_pJunQiPKCfg->GetCfg(ECI_InitTime);
    m_pJunQiPKGame->SetGameTimer(INITWAITTIME, 0, nTimeLeigth + 2);

    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    InitChessAck* pAck = pAckMsg->mutable_initchess_ack_msg();
    pAck->set_gametype(m_pJunQiPKCfg->GetCfg(ECI_GameType));
    pAck->set_cleandilei(m_pJunQiPKCfg->GetCfg(ECI_CleanDiLei));
    pAck->set_carryjunqi(m_pJunQiPKCfg->GetCfg(ECI_CarryJunQi));

    m_pJunQiCore->GetInitPos(pAck);
    auto* pService = dynamic_cast<CTKJunQiPKService*>(tk_pDllServer);
    pAck->set_serverversion(pService->GetProductVersion());
    m_pJunQiPKGame->SendMsg(mobilemsg);
}

void CTKJunQiPKData::sendInitLayoutAck()
{
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    InitLayoutAck* pAck = pAckMsg->mutable_initlayout_ack_msg();
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        pAck->add_layoutstate(m_vPlayers[i]->m_bInitChess);
    }
    m_pJunQiPKGame->SendMsg(mobilemsg);
}

void CTKJunQiPKData::SendStartMoveChessAck()
{
    m_nCurSeat = TKGenRandom() % m_nPlayerCount;
    m_nStage = ES_WaitingMoveChess;
    m_pJunQiPKGame->KillGameTimer(INITWAITTIME);
    m_pJunQiPKGame->SetGameTimer(MOVECHESSWAITTIME, m_nCurSeat, m_pJunQiPKCfg->GetCfg(ECI_MoveTime) + 2);
    // 更新上一手动作时间
    time_t temp = time(NULL);
    m_tCurRemainTime = temp;
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    StartMoveChessAck* pAck = pAckMsg->mutable_startmovechess_ack_msg();
    pAck->set_seat(m_nCurSeat);
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        pAck->add_headimg(m_vPlayers[i]->m_nHeadImgId);
        if (m_vPlayers[i]->m_nResult == EFR_Loss)
        {
            pAck->add_overseat(i);
        }
    }
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        ostringstream ss;
        ss << m_vPlayers[i]->m_szLayout;
        _getAllyLayout(i, ss);
        pAck->set_layout(ss.str());
        m_pJunQiPKGame->SendMsg(mobilemsg, i);
    }
    m_vPlayers[m_nCurSeat]->SetStartTime();
}

void CTKJunQiPKData::SendSelectChessAck()
{
    TKMobileAckMsg mobilemsg;
    JunQiPKAckMsg* pAckMsg = mobilemsg.mutable_junqipk_ack_msg();
    SelectChessAck* pAck = pAckMsg->mutable_selectchess_ack_msg();
    pAck->set_seat(m_nCurSeat);
    pAck->set_pos(m_nCurSelectPos);
    m_curCanMovePos.clear();
    m_pJunQiCore->GetCanMovePos(m_nCurSelectPos, m_curCanMovePos);
    for (auto iter = m_curCanMovePos.begin(); iter != m_curCanMovePos.end(); ++iter)
    {
        pAck->add_postions(*iter);
    }
    m_pJunQiPKGame->SendMsg(mobilemsg);
}

void CTKJunQiPKData::RecordLayout()
{
    int rt = (time(NULL) - m_pJunQiPKGame->m_timeGameBegin);
    ostringstream ss;
    ss << "<a id=\"0\" rt=\"" << rt << "\" v=\"1.1.0\" gt=\"" << m_pJunQiPKCfg->GetCfg(ECI_GameType) << "\" p=\""
       << m_vPlayers[0]->m_nHeadImgId << "," << m_vPlayers[1]->m_nHeadImgId;
    if (m_pJunQiCore->Recordlayout())
    {
        ss << "\" o=\"" << m_vPlayers[0]->m_szLayout;
        for (int i = 1; i < m_vPlayers.size(); ++i)
        {
            ss << "," << m_vPlayers[i]->m_szLayout;
        }
    }
    ss << "\"/>";
    m_pJunQiPKGame->AddGameAction((char*)(ss.str().c_str()));
}

void CTKJunQiPKData::RecordShowChess(ShowChessAck* pShow)
{
    int rt = (time(NULL) - m_pJunQiPKGame->m_timeGameBegin);
    const Chess& rChess = pShow->chess();
    ostringstream ss;
    ss << "<a id=\"10\" rt=\"" << rt << "\" s=\"" << m_nCurSeat << "\" ps=\"" << rChess.pos() << "\" ct=\""
       << rChess.type() << "\" cc=\"" << rChess.color() << "\" ";

    if (pShow->color_size() == m_nPlayerCount)
    {
        ss << "pc=\"" << pShow->color(0) << "," << pShow->color(1) << "\"";
    }
    ss << "/>";
    m_pJunQiPKGame->AddGameAction((char*)(ss.str().c_str()));
}

void CTKJunQiPKData::RecordMoveChess(int nType, int nResult, int nEvent, const vector<int>& vPath)
{
    int rt = (time(NULL) - m_pJunQiPKGame->m_timeGameBegin);
    ostringstream ss;
    ss << "<a id=\"1\" rt=\"" << rt << "\" s=\"" << m_nCurSeat << "\" ct=\"" << nType << "\" fr=\"" << nResult
       << "\" et=\"" << nEvent << "\" ph=\"";
    int nLast = vPath.size() - 1;
    for (int i = 0; i < vPath.size(); ++i)
    {
        if (i == nLast)
        {
            ss << vPath[i];
        }
        else
        {
            ss << vPath[i] << ",";
        }
    }
    ss << "\"/>";
    m_pJunQiPKGame->AddGameAction((char*)(ss.str().c_str()));
}

void CTKJunQiPKData::_recordCleanChess(int nSeat, int nReason)
{
    int rt = (time(NULL) - m_pJunQiPKGame->m_timeGameBegin);
    ostringstream ss;
    ss << "<a id=\"11\" rt=\"" << rt << "\" s=\"" << nSeat << "\" r=\"" << nReason << "\"/>";
    m_pJunQiPKGame->AddGameAction((char*)(ss.str().c_str()));
}

void CTKJunQiPKData::RecordResult()
{
    vector<int> vLossSeat;
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (EFR_Loss == m_vPlayers[i]->m_nResult)
        {
            vLossSeat.push_back(i);
        }
        ostringstream ss;
        ss << "<result seat=\"" << i << "\" result=\"" << m_vPlayers[i]->m_nResult << "\" honor=\""
           << m_vPlayers[i]->m_nRankChange << "\"/>";
        m_pJunQiPKGame->AddGameResult((char*)(ss.str().c_str()));
    }
    if (vLossSeat.empty())
    {
        vLossSeat.push_back(-1);
    }
    int rt = (time(NULL) - m_pJunQiPKGame->m_timeGameBegin);
    ostringstream ssr;
    ssr << "<a id=\"5\" rt=\"" << rt << "\" ls=\"" << vLossSeat[0];
    for (int i = 1; i < vLossSeat.size(); ++i)
    {
        ssr << "," << vLossSeat[i];
    }
    ssr << "\" t=\"0\" />";
    m_pJunQiPKGame->AddGameAction((char*)(ssr.str().c_str()));
}

bool CTKJunQiPKData::IsAllReady()
{
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (!m_vPlayers[i]->m_bInitChess)
        {
            return false;
        }
    }
    return true;
}

void CTKJunQiPKData::_onAllReady()
{
    RecordLayout();
    m_pJunQiCore->CodeChess();
    SendStartMoveChessAck();
}

void CTKJunQiPKData::_calcNegativeState(int nSeat, bool isInitiative)
{
    if (nSeat < 0 || nSeat >= m_nPlayerCount)
    {
        return;
    }

    bool isComfirm = false;  // 是否判断完成

    if (isInitiative || m_vPlayers[nSeat]->m_nPassCount >= m_pJunQiPKCfg->GetCfg(ECI_PassCount))
    {
        int nNegativeType = isInitiative ? ENT_INITIATIVE_LOSE : ENT_TIMEOUT_AFK;

        // 如果是四国军棋，还要计算是否挂机退赛
        if (GT_SIGUOSIAN == m_pJunQiPKCfg->GetCfg(ECI_GameType) || GT_SHUANGMING == m_pJunQiPKCfg->GetCfg(ECI_GameType))
        {
            for (int i = 0; i < m_nPlayerCount; ++i)
            {
                if (i % 2 == nSeat % 2 && i != nSeat)
                {
                    // 队友消极比赛自己不算消极
                    if (ENT_NOT_NEGATIVE != m_vPlayers[i]->m_nNegativeState)
                    {
                        m_vPlayers[nSeat]->m_nNegativeState = ENT_NOT_NEGATIVE;
                        isComfirm = true;
                    }
                    else
                    {
                        m_vPlayers[nSeat]->m_nNegativeState = nNegativeType;
                    }
                }
            }

            // 队伍消极比赛状态设置
            for (int i = 0; i < m_nPlayerCount; ++i)
            {
                if (i % 2 == nSeat % 2 && i != nSeat)
                {
                    // 队友或者我消极比赛，队伍都是消极比赛
                    if (ENT_NOT_NEGATIVE != m_vPlayers[nSeat]->m_nNegativeState)
                    {
                        m_vPlayers[i]->m_nTeamNegativeState = m_vPlayers[nSeat]->m_nNegativeState;
                        m_vPlayers[nSeat]->m_nTeamNegativeState = m_vPlayers[nSeat]->m_nNegativeState;
                    }
                    if (ENT_NOT_NEGATIVE != m_vPlayers[i]->m_nNegativeState)
                    {
                        m_vPlayers[nSeat]->m_nTeamNegativeState = m_vPlayers[i]->m_nNegativeState;
                        m_vPlayers[i]->m_nTeamNegativeState = m_vPlayers[i]->m_nNegativeState;
                    }
                }
            }
        }

        if (isComfirm)
        {
            return;
        }
        // 主动认输和超时判断
        int rt = (time(NULL) - m_pJunQiPKGame->m_timeGameBegin);
        // 对局时长判断
        if (rt < ECI_NegativeMatchTime)
        {
            m_vPlayers[nSeat]->m_nNegativeState = nNegativeType;
        }

        // 总步数判断
        if (m_nStepsMoved < ECI_NegativeNoFightStep)
        {
            // 司令或者军长还在场的情况
            int posSiLing = m_pJunQiCore->GetChessPos(nSeat, ECT_SiLing);
            int posJunZhang = m_pJunQiCore->GetChessPos(nSeat, ECT_JunZhang);
            if (JUNQIPK_UNKNOWN != posJunZhang || JUNQIPK_UNKNOWN != posSiLing)
            {
                m_vPlayers[nSeat]->m_nNegativeState = nNegativeType;
            }

            if (GT_FANFANQI == m_pJunQiPKCfg->GetCfg(ECI_GameType))
            {
                // 翻翻棋再加入地雷在场判断
                int posDiLei = m_pJunQiCore->GetChessPos(nSeat, ECT_DiLei);
                if (JUNQIPK_UNKNOWN != posJunZhang)
                {
                    m_vPlayers[nSeat]->m_nNegativeState = nNegativeType;
                }
            }
        }

        if (m_nNoFightSteps == m_nStepsMoved)
        {
            // 未进行过交战的情况
            m_vPlayers[nSeat]->m_nNegativeState = nNegativeType;
        }
    }
}

void CTKJunQiPKData::_sendNegativeAtomEvent()
{
    // 队伍消极比赛状态设置
    for (int i = 0; i < m_nPlayerCount; ++i)
    {
        if (ENT_NOT_NEGATIVE != m_vPlayers[i]->m_nNegativeState)
        {
            m_vPlayers[i]->AddAtomGameEvent(ENA_EVENT, 1, TRUE);
        }
    }
}
