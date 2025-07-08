#pragma once
#include "LYGCBotDefine.h"

#include <chrono>
#include <list>
#include <mutex>
#include <random>
#include "TKBotPlayer.h"

#include "LYGCDefine.h"
#include "LYGCAI.h"
#include "BotAction.h"
#include "LYGCTableData.h"
#include "LYGCMsgDispatcher.h"
#include "MJConfig.h"

#define DEBUG_AI (_DEBUG && 1)

/// @brief Bot逻辑类
class LYGCBot : public CTKBotPlayer, public LYGCMsgDispatcher
{
public:
    LYGCBot();
    virtual ~LYGCBot();

public:
    // 机器人连接初始化
    virtual BOOL OnInitialUpdate() override;
    // 消息回调
    virtual BOOL OnMsg(PTKHEADER pMsg) override;
    // TickCount时间回调，使用SetThinkTickCount来启用
    virtual BOOL OnTickCount() override;

public:
    virtual std::string __GetHands();

public:
    LYGCTableData* GetTable();

public:
    bool UseAI();

    void Init();   // 初始化时调用一次，
    void Ready();  // 游戏服开局消息时调用

    void Done();  //出错或破产

public:
    bool OnAckEnterRound(PTKHEADER pMsg);
    bool OnAckAddPlayerInfo(PTKHEADER pMsg);
    bool OnAckHistoryMsgBegin(PTKHEADER pMsg);
    bool OnAckHistoryMsgEnd(PTKHEADER pMsg);
    bool OnAckBeginHand(PTKHEADER pMsg);

public:  // getter
    UserID GetUserID() const;
    ISeatID GetSeat() const;

public:
    void StartAction(std::shared_ptr<BotAction> action);
    void StartActionWithAI(std::shared_ptr<BotAction> action, std::string const& content);
    void SendReqMsgToSvr(TKMobileReqMsg& rMsg);
    void SendHttpMsgToAI(std::string const& httpMsg);

private:
    void _InitAIMsg(CTKBuffer& buffer, int nMinThinkTime, std::string const& httpReq);

private:  // pb msg handle
    bool OnAIServerAck(PTKHEADER pMsg);
    bool OnAIServerAckWithLevel(PTKHEADER pMsg, int level);

    void OnPbMsg(PTKHEADER pMsg);

private:
    std::chrono::system_clock::time_point m_BirthTime;
    /// 开局前由比赛服数据同步

    UserID m_UserID{};
    ISeatID m_Seat{};
    std::array<UserID, NP4> m_AllPlayersUserID{};

    /// 初始化加载
    using TimePair = std::pair<std::chrono::milliseconds, std::chrono::milliseconds>;
    //std::map<int, TimePair> m_DelayConfigs;

    std::shared_ptr<LYGCAI> m_RobotAI;
    std::shared_ptr<LYGCTableData> m_TableData;

    std::shared_ptr<BotAction> m_AIAction;

    ISeatID m_Banker = SEAT_NULL;

    int m_TurnCount{};  //切换玩家次数

    int m_SerialNum{};  // for msg

    bool m_IsOver = true;  //已经结束
    std::string m_LastHttpRequest;

    MJConfig m_Config;

    /// @brief 当前是否为是历史消息
    bool m_IsHistoryMsg{};

    virtual void OnGameBeginAck(const TKMahJongGameBeginAck& ack, ISeatID seat) override;
    virtual void OnPlaceAck(const TKMahJongPlaceAck& ack, ISeatID seat) override;
    virtual void OnOpenDoorAck(const TKMahJongOpenDoorAck& ack, ISeatID seat) override;
    virtual void OnActionAck(const TKMahJongActionAck& ack, ISeatID seat) override;
    virtual void OnPengAck(const TKMahJongPengAck& ack, ISeatID seat) override;
    virtual void OnGangAck(const TKMahJongGangAck& ack, ISeatID seat) override;
    virtual void OnWinAck(const TKMahJongWinAck& ack, ISeatID seat) override;
    virtual void OnDiscardTileAck(const TKMahJongDiscardTileAck& ack, ISeatID seat) override;
    virtual void OnDrawTileAck(const TKMahJongDrawTileAck& ack, ISeatID seat) override;
    virtual void OnTrustPlayAck(const TKMahJongTrustPlayAck& ack, ISeatID seat) override;
    virtual void OnFanCiAck(const TKMahjongFanCiAck& ack, ISeatID seat) override;
};
