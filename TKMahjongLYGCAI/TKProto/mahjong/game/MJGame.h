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

//������ͳ��
namespace MJGameEventID
{
static constexpr int KonCount = 10000054;
static constexpr int ChowCount = 10000073;
static constexpr int PonCount = 10000074;
static constexpr int IsCall = 10000075;
static constexpr int TotalTimes = 10000177;  // ĳ���score / baseScore
static constexpr int HuDianPao = 10000052;   //���������ƴ���
static constexpr int HuZiMo = 10000053;      //�������ƴ���

static constexpr int Over = 10000118;                  // һ����Ϸ����
static constexpr int Coin = 10000130;                  //����ÿ��Ӯȡ�������
static constexpr int WinOrLose_FDTable = 10000142;     // �Խ�������ʤ�������0-ʧ�ܣ�1-ʤ����
static constexpr int WinOrLose_NonFDTable = 10000143;  // ���Խ�������ʤ�������0-ʧ�ܣ�1-ʤ����
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
    //�Ծֿ�ʼ�������������������ʼ״̬
    virtual void OnStart() = 0;

    //����MJRecord������
    virtual std::shared_ptr<MJRecord> OnCreateRecord();

    //����MJPlay������
    virtual std::shared_ptr<MJPlay> OnCreatePlay()
    {
        return nullptr;
    }

    template <typename T = MJPlayEx>
    std::shared_ptr<T> GetPlay()
    {
        return std::dynamic_pointer_cast<T>(m_MJPlay);
    }

    //����Ĭ�����ô�
    virtual std::vector<int> OnGetDefaultProperties() = 0;

    //���غ��ѷ�json��ʽ����
    // @��ʱ�������°汾��ʹ��OnGetFDTableConfigMap()���
    virtual MJConfigFDDescriptor OnGetFDTableDescriptor();

    //���غ��ѷ�json��ʽ����,<��-���ô��±�>��Ӧ
    virtual MJFDConfigMap OnGetFDTableConfigMap();

    //���������Ż�������
    virtual MJDrawConfigs GetDrawConfigs();

    //�Ծֽ�������ͳһ����Ծֽ���ʱ������
    virtual void OnEnd(EGameOverStatus status);

    //���״̬�任��ص���ʵ�ֺ����������磺��Client����Ϣ��
    virtual void OnPlayerTrustedChange(ISeatID seat, ETrustType type) = 0;

    //�յ������Ϣ��ص�
    virtual void OnReqMsg(ISeatID seat, PTKHEADER pMsg) = 0;

    virtual void OnSendDebugMsg(std::string const& msg, ISeatID seat);

public:
    // �ּ䴫������.
    // Notice: THandInfo����Ϊ��POD�������ͣ��� GetLastHandSaveInfoLen() == sizeof(THandInfo)

    //�ּ䴫�ݵ����ݳ��ȣ��������أ������Ͼֱ�����Ϣ�ĳ���
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
    //������һ��״̬
    template <typename TState, typename... Args>
    auto SetNextState(Args... args) -> std::shared_ptr<TState>
    {
        auto state = std::make_shared<TState>(args...);
        m_NextState = state;
        m_NextState->m_Name = typeid(TState).name();
        return state;
    }

public:
    //��MJGameά��
    std::shared_ptr<MJTimer> GetTimer();
    std::shared_ptr<MJPlayer> GetPlayer(ISeatID seat);
    std::shared_ptr<MJGrowSystem> GetGrowSystem() const;
    std::shared_ptr<MJConfig> GetConfig() const;

    CTKGamePlayer* GetGamePlayer(ISeatID seat, bool nullable = false);

    inline ISeatID GetNewbieSeat() const
    {
        return m_NBSeat;
    }  //��ȡ������λ��

public:
    bool IsFDTable() const;    //���ѷ�
    bool IsCoinMatch() const;  //��ҳ�
    int GetScoreBase() const;
    int GetIniPlayerCount() const;
    int GetPlayingCount() const;

public:
    // ������Ϸ
    void FinishGame(EGameOverStatus status = EGameOverStatus::Normal);

    bool IsValidSeat(ISeatID seat) const;

    void SyncResult();
    void LogGameResultRecord();  //��ӽ��¼��
    void PostGameProcessData();  //ͬ����������

    void CacheStateData(const google::protobuf::Message* ack);  //����״̬����

    int UpdateRequestID(ISeatID seat = SEAT_ALL);  //�����������
    bool IsRequestID(ISeatID seat, int request);

    bool HasClientOnline();  //�Ƿ��пͻ�������
    void AddDebugStr(std::string const& str);
    std::string const& GetDebugStr();
    void ClearDebugStr();

    //����е���Ϸ��Ҫ���������Ƶ���Ϣ����䵽strExternData��
    //vData.first��nCardID  vData.second��nFan
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
    void _HandleWinAwards();  //������������

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
    Json::Value m_JsonData{};         //���ڴ洢������������
    std::vector<int> m_RequestIDs{};  //��¼ÿ����ҵ��������
    int m_IncreasedID{};              //��ǰ�������
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
