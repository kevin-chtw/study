#pragma once
#include <google/protobuf/message.h>
#include <memory>
#include <TKDllGame.h>
#include <functional>
#include "MJTimer.h"
#include "MJModule.h"
#include "MJState.h"
#include "MJRecord.h"
#include "MJPlayer.h"
#include "MJConfig.h"
#include "MJGrowSystem.h"
#include "MJPlayEx.h"
#include "MJChecker.h"

static constexpr DWORD TIMER_ID_NORMAL = 0;

//吃碰杠统计
namespace MJGameEventID
{
static constexpr int KonCount = 10000054;
static constexpr int ChowCount = 10000073;
static constexpr int PonCount = 10000074;
static constexpr int IsCall = 10000075;
static constexpr int TotalTimes = 10000177;  // 某玩家score / baseScore
static constexpr int HuDianPao = 10000052;   //非自摸胡牌次数
static constexpr int HuZiMo = 10000053;      //自摸胡牌次数

static constexpr int Over = 10000118;                  // 一局游戏结束
static constexpr int Coin = 10000130;                  //岛屿每局赢取金币数量
static constexpr int WinOrLose_FDTable = 10000142;     // 自建桌当局胜负情况（0-失败，1-胜利）
static constexpr int WinOrLose_NonFDTable = 10000143;  // 非自建桌当局胜负情况（0-失败，1-胜利）
};                                                     // namespace MJGameEventID

using MJDrawConfigs = std::vector<std::vector<MJ::DrawStrategy<4>::ConfigItem>>;

class MJGame : public CTKGame
{
public:
    /// <param name="msgType">eg: TK_MSG_GAME_MAHJONGZJ</param>
    MJGame(unsigned msgType);

    unsigned GetMsgType();

    virtual ~MJGame();

public:  // TODO: override these methods in the Target Game.
    //对局开始，创建牌桌，并进入初始状态
    virtual void OnStart() = 0;

    //创建MJRecord并返回
    virtual std::shared_ptr<MJRecord> OnCreateRecord();

    //创建MJPlay并返回
    virtual std::shared_ptr<MJPlay> OnCreatePlay()
    {
        return nullptr;
    }

    template <typename T = MJPlayEx>
    std::shared_ptr<T> GetPlay()
    {
        return std::dynamic_pointer_cast<T>(m_MJPlay);
    }

    //返回默认配置串
    virtual std::vector<int> OnGetDefaultProperties() = 0;

    //返回好友房json格式描述
    // @过时方法，新版本请使用OnGetFDTableConfigMap()替代
    virtual MJConfigFDDescriptor OnGetFDTableDescriptor();

    //返回好友房json格式描述,<键-配置串下标>对应
    virtual MJFDConfigMap OnGetFDTableConfigMap();

    //返回摸牌优化的配置
    virtual MJDrawConfigs GetDrawConfigs();

    //对局结束，可统一处理对局结束时的事务
    virtual void OnEnd(EGameOverStatus status);

    //玩家状态变换后回调，实现后续操作（如：给Client发消息）
    virtual void OnPlayerTrustedChange(ISeatID seat, ETrustType type) = 0;

    //收到玩家消息后回调
    virtual void OnReqMsg(ISeatID seat, PTKHEADER pMsg) = 0;

    virtual void OnSendDebugMsg(std::string const& msg, ISeatID seat);

public:
    // 局间传递数据.
    // Notice: THandInfo必须为类POD数据类型，且 GetLastHandSaveInfoLen() == sizeof(THandInfo)

    //局间传递的数据长度：必须重载，返回上局保存信息的长度
    virtual int GetLastHandSaveInfoLen() override = 0;

    template <typename THandInfo>
    void LoadLastHandInfo(THandInfo out__& info);

    template <typename THandInfo>
    void SaveLastHandInfo(THandInfo out__& info);

public:  // GTS
    void SendMsgToGTS(std::string const& data);

    virtual BOOL OnGTSMsg(const LPVOID pData, DWORD dwLength) override;

public:  // Http Broker
    void SendHttpBrokerReq(BrokerReqData const& data);
    // override this to accept http ack.
    virtual void OnHttpBrokerAck(BrokerAckData const& data);

    virtual BOOL NotifyHttpBrokerAckMsg(const LPVOID pData, DWORD dwLength) override;

public:
    //设置下一个状态
    template <typename TState, typename... Args>
    auto SetNextState(Args... args) -> std::shared_ptr<TState>
    {
        auto state = std::make_shared<TState>(args...);
        m_NextState = state;
        m_NextState->m_Name = typeid(TState).name();
        return state;
    }

public:
    //由MJGame维护
    std::shared_ptr<MJTimer> GetTimer();
    std::shared_ptr<MJPlayer> GetPlayer(ISeatID seat);
    std::shared_ptr<MJGrowSystem> GetGrowSystem() const;
    std::shared_ptr<MJConfig> GetConfig() const;

    CTKGamePlayer* GetGamePlayer(ISeatID seat, bool nullable = false);

    inline ISeatID GetNewbieSeat() const
    {
        return m_NBSeat;
    }  //获取新手座位号

public:
    bool IsFDTable() const;    //好友房
    bool IsCoinMatch() const;  //金币场
    int GetScoreBase() const;
    int GetIniPlayerCount() const;
    int GetPlayingCount() const;

public:
    // 结束游戏
    void FinishGame(EGameOverStatus status = EGameOverStatus::Normal);

    bool IsValidSeat(ISeatID seat) const;

    void SyncResult();
    void LogGameResultRecord();  //添加结果录像
    void PostGameProcessData();  //同步过程数据

    void CacheStateData(const google::protobuf::Message* ack);  //缓存状态数据

    int UpdateRequestID(ISeatID seat = SEAT_ALL);  //更新请求序号
    bool IsRequestID(ISeatID seat, int request);

    bool HasClientOnline();  //是否有客户端在线
    void AddDebugStr(std::string const& str);
    std::string const& GetDebugStr();
    void ClearDebugStr();

    //如果有的游戏需要填充玩家手牌等信息，填充到strExternData中
    //vData.first：nCardID  vData.second：nFan
    virtual void FillAndSendHaiDiLaoMsg(int nFan, DWORD uId, const std::vector<std::pair<int, int>>& vData, const std::string& strExternData = "");

protected:
    void SendAckMsg(const google::protobuf::Message* ack, ISeatID seat = SEAT_ALL);
    std::vector<BYTE> SerializeAck(const google::protobuf::Message* ack);

    static bool ConvertReqMsg(google::protobuf::Message& msg, PTKHEADER pMsg);

public:  // override from CTKGame.
    virtual BOOL OnGameBegin() override final;
    virtual BOOL OnPlayerMsg(DWORD userID, int seatOrder, PTKHEADER pMsg) override;
    virtual void OnGameTimer(DWORD dwID, ULONG_PTR ulUserData) override;
    virtual void OnPlayerExitMatch(CTKGamePlayer* pGamePlayer) override;
    virtual void OnPlayerNetBreak(CTKGamePlayer* pGamePlayer) override;
    virtual void OnPlayerNetResume(CTKGamePlayer* pGamePlayer) override;

protected:
    void _InitializeMJGame();
    void _CreatePlayers();
    void _EnterNextStates();

private:
    ISeatID _InitNewbieSeat();
    void _HandleWinAwards();  //处理排名奖励

protected:
    unsigned m_MsgType{};
    bool m_Initialized{};
    std::shared_ptr<MJTimer> m_Timer;
    std::shared_ptr<MJState> m_CurrentState{};
    std::shared_ptr<MJState> m_NextState{};

    std::shared_ptr<MJGrowSystem> m_GrowSystem;
    std::shared_ptr<MJConfig> m_Config;

    std::shared_ptr<MJRecord> m_MJRecord;
    std::shared_ptr<MJPlay> m_MJPlay;

    using SharedPlayer = std::shared_ptr<MJPlayer>;
    std::vector<SharedPlayer> m_Players;
    ISeatID m_NBSeat{SEAT_NULL};
    std::string m_Debugstr{};

protected:
    Json::Value m_JsonData{};         //用于存储断线重链数据
    std::vector<int> m_RequestIDs{};  //记录每个玩家的请求序号
    int m_IncreasedID{};              //当前请求序号
};

template <typename THandInfo>
inline void MJGame::LoadLastHandInfo(THandInfo out__& info)
{
    ::memcpy(&info, m_pShareGameData, GetLastHandSaveInfoLen());
}

template <typename THandInfo>
inline void MJGame::SaveLastHandInfo(THandInfo out__& info)
{
    ::memcpy(m_pShareGameData, &info, GetLastHandSaveInfoLen());
}
