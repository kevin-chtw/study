#pragma once
#include <TKDllServer.h>
#include <TKDllGame.h>
#include "MJModule.h"

class MJGame;

using IGrowID = DWORD;

enum class EHuResultMode
{
    None,
    PaoHu,
    ZiMo,
};

class MJPlayer : public MJGameModule
{
public:
    using MJGameModule::MJGameModule;

    void Initialize(ISeatID seat);
    ISeatID GetSeat() const;
    int64_t GetScore() const;  //ԭʼ�����ַ�
    uint32_t GetUserID() const;

    int64_t AddScoreChange(int64_t value);
    int64_t GetIncremental(int64_t value);  //����scroechangeʱ��ȡ�Ƿ�չʾ˰��

    int64_t GetScoreChange() const;         //�䶯ֵ
    int64_t GetScoreChangeWithTax() const;  //�䶯ֵ(���Ͽ�˰��
    int64_t GetCurrentScore() const;        //�䶯��ĵ�ǰ�֣���˰��

    void SetTrusted(bool trusted);
    bool IsTrusted() const;

    void SetOffline(bool offline);
    bool IsOffline();

    void SetOut(MJPlayerStatus::Type status, EHuResultMode mode = {});
    bool IsOut();

    void PayTax();           //��˰
    void RefundTax();        //��˰
    int64_t GetTax() const;  //˰��

    void SyncGameResult();  // ͳ�ƣ�����
    void AddOperateEvent(int chowCount, int ponCount, int konCount, bool isCall);
    void AddHuEvent();

    MJPlayerStatus::Type GetStatus() const;

    void ResetScore(int64_t score);  //���ô���֣����ԣ�

protected:
    ISeatID m_Seat{SEAT_NULL};

    MJPlayerStatus::Type m_Status{MJPlayerStatus::Playing};
    EHuResultMode m_HuMode{};

    int64_t m_Tax{};
    int64_t m_Score{};  //˰���ʼ��
    uint32_t m_UserID{};
    bool m_IsTrusted{};
    bool m_IsOffline{};
    bool m_IsBot{};
    bool m_IsOut{};  //�Ƿ����

    int64_t m_ScoreChange{};  //������˰��
};

using GrowValueCallback = std::function<void(ISeatID, DWORD growID, int value)>;

class MJGamePlayer : public CTKGamePlayer
{
public:
    void SetValueByID(DWORD growID, int64_t value);
    int64_t GetValueByID(DWORD growID, int64_t notFoundValue) const;

    int64_t GetHonorValue() const;

    /// <param name="onGrowValueLoad">�첽��ȡ�ɳ�ֵ�ɹ�ʱ�Ļص����������ģ�����Ϊnullptr</param>
    void SetCallback(GrowValueCallback onGrowValueLoad);
    BOOL OnPivotMsg(PTKHEADER in_pAckMsg, int seatBind, DWORD growIDBind);

private:
    std::map<DWORD, int64_t> m_GrowValues{};
    GrowValueCallback m_Callback{};
};
