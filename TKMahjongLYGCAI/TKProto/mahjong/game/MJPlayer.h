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
    int64_t GetScore() const;  //原始带入手分
    uint32_t GetUserID() const;

    int64_t AddScoreChange(int64_t value);
    int64_t GetIncremental(int64_t value);  //发送scroechange时获取是否展示税分

    int64_t GetScoreChange() const;         //变动值
    int64_t GetScoreChangeWithTax() const;  //变动值(算上扣税）
    int64_t GetCurrentScore() const;        //变动后的当前分（算税）

    void SetTrusted(bool trusted);
    bool IsTrusted() const;

    void SetOffline(bool offline);
    bool IsOffline();

    void SetOut(MJPlayerStatus::Type status, EHuResultMode mode = {});
    bool IsOut();

    void PayTax();           //缴税
    void RefundTax();        //退税
    int64_t GetTax() const;  //税费

    void SyncGameResult();  // 统计，经验
    void AddOperateEvent(int chowCount, int ponCount, int konCount, bool isCall);
    void AddHuEvent();

    MJPlayerStatus::Type GetStatus() const;

    void ResetScore(int64_t score);  //重置带入分（测试）

protected:
    ISeatID m_Seat{SEAT_NULL};

    MJPlayerStatus::Type m_Status{MJPlayerStatus::Playing};
    EHuResultMode m_HuMode{};

    int64_t m_Tax{};
    int64_t m_Score{};  //税后初始分
    uint32_t m_UserID{};
    bool m_IsTrusted{};
    bool m_IsOffline{};
    bool m_IsBot{};
    bool m_IsOut{};  //是否出局

    int64_t m_ScoreChange{};  //不包含税分
};

using GrowValueCallback = std::function<void(ISeatID, DWORD growID, int value)>;

class MJGamePlayer : public CTKGamePlayer
{
public:
    void SetValueByID(DWORD growID, int64_t value);
    int64_t GetValueByID(DWORD growID, int64_t notFoundValue) const;

    int64_t GetHonorValue() const;

    /// <param name="onGrowValueLoad">异步若取成长值成功时的回调，若不关心，可设为nullptr</param>
    void SetCallback(GrowValueCallback onGrowValueLoad);
    BOOL OnPivotMsg(PTKHEADER in_pAckMsg, int seatBind, DWORD growIDBind);

private:
    std::map<DWORD, int64_t> m_GrowValues{};
    GrowValueCallback m_Callback{};
};
