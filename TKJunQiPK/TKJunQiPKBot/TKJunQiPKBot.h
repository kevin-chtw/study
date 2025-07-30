#pragma once
#include <ctime>
#include "TKBotPlayer.h"
#include "TKJunQiPK.pb.h"
#include "TKJunQiPKBotAI.h"
#include "TKJunQiPKBotDefine.h"

struct STime
{
    int nProb;
    int nTime;
};
#define DEFAULT_AI_ACTTIME "20,1|20,1.5|20,1.8|20,2|10,2.2|10,3"

const static char AiIniFile[MAX_PATH] = ".\\TKAIActionTime.ini";
const static char AiIniCh[MAX_PATH] = "MahjongJunQi";

enum
{
    DEFAULT_ACK = 0,
    MOVECHESS_ACK,
    SHOWCHESS_ACK
};

using namespace cn::jj::service::msg::protocol;
#define MAX_PLAYER_COUNT 2
class CTKJunQiPKBot : public CTKBotPlayer
{
public:
    CTKJunQiPKBot(void);
    ~CTKJunQiPKBot(void);

public:
    virtual BOOL OnInitialUpdate();
    virtual BOOL OnTickCount();
    virtual BOOL OnMsg(PTKHEADER pMsg);

private:
    BOOL OnAckEnterRound(PTKHEADER pMsg);
    BOOL OnAckGameRulerInfo(PTKHEADER pMsg);
    BOOL OnAckGameRulerExInfo(PTKHEADER pMsg);
    BOOL OnAckAddPlayerInfo(PTKHEADER pMsg);

    BOOL OnAckAIServerMsg(PTKHEADER pMsg);
    BOOL OnAckPbMsg(PTKHEADER pMsg);
    BOOL OnAckJunQiPKInitChess(const InitChessAck& rAck);
    BOOL OnAckJunQiPKStartMoveChess(const StartMoveChessAck& rAck);
    BOOL OnAckJunQiPKSelectChess(const SelectChessAck& rAck);
    BOOL OnAckJunQiPKMoveChess(const MoveChessAck& rAck);
    BOOL OnAckJunQiPKShowChess(const ShowChessAck& rAck);
    BOOL OnAckJunQiPKActionFinish(const ActionFinishAck& rAck);
    BOOL OnAckJunQiPKRefusePeace(const RefusePeaceAck& rAck);
    BOOL OnAckJunQiPKRepeatCount(const RepeatCountAck& rAck);
    BOOL OnAckFinishGame(const FinishGameAck& rAck);
    BOOL OnAckPeace(const PeaceAck& rAck);

    void SendReqMsg(const TKMobileReqMsg& rMsg);
    BOOL SendMsgInitLayout();
    BOOL SendMsgSelectChess(UINT pos);
    BOOL SendMsgShowChess(UINT pos);
    BOOL SendMsgAcotionFinish();
    BOOL SendMsgMoveChess(UINT startPos, UINT endPos);
    BOOL SendMsgPeaceRes();

    BOOL PlayChess();
    BOOL SendMsgToAi();
    void InitAISvrMsg(PTKHEADER msg, int nMinThinkTime);
    bool WebSeverMsgAck(PTKHEADER pMsg);
    int GetRandomTime();
    void InitAiTime();
    void ReSet();

private:
    int m_MySeat;
    int m_MyColor;
    int m_MySelectPosition;
    int m_MyMoveToPosition;
    int m_MyShowChessPosition;
    int m_RequestPeaceSeat;
    int m_IsAgreePeace;
    DWORD m_AllUserID[MAX_PLAYER_COUNT]{};
    GAMESCHEDULE m_GameSchedule;
    boost::shared_ptr<CTKJunQiPKBotAI> m_pJunQiPKAI;
    std::vector<STime> m_vActTime;
    std::map<int, int> m_seatLastColor;
    std::map<int, int> m_seatColor;
    int m_DelayPlayChess;
    MoveChessAck m_moveChessAck;
    ShowChessAck m_showChessAck;
    int m_TotalPlayerCount{};
    EGameType m_GameType{GT_None};
};
