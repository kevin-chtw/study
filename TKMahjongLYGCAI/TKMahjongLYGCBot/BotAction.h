#pragma once

#include "LYGCDefine.h"
#include "LYGCAI.h"

class LYGCBot;

// AI返回消息回调
class BotAction
{
public:
    BotAction(LYGCBot* bot, std::chrono::milliseconds delay, bool useAI);
    virtual ~BotAction() = default;
    // 返回true，表示已经触发执行动作
    bool OnTick();

    void OnAIReply(AIReplyData const& data);

    virtual bool RunAIAction() = 0;
    virtual void RunDefaultAction() = 0;

protected:
    /// @brief 思考时长是一个随机值，若AI在思考时长内返回结果，则在思考结束时回复结果给游戏服
    /// 若思考时长内AI未返回，则继续等至过期时长，在此期间AI返回将标记网络波动，并正常回复游戏服
    /// 过期时长后仍未收到回复，则标记Miss，并采取低级默认操作回复游戏服
    /// LongExpiredTime 应设置不小于(>=) ThinkExpireTime
    std::chrono::system_clock::time_point m_ThinkExpireTime;  //思考时长
    std::chrono::system_clock::time_point m_LongExpiredTime;  // 过期时长

    LYGCBot* m_Bot{};
    AIReplyData m_Reply{};
    bool m_HasReplied = false;
    bool m_UseAI = false;
};

class BotActionOperation : public BotAction
{
public:
    using BotAction::BotAction;
    // 通过 BotAction 继承
    virtual bool RunAIAction() override;
    virtual void RunDefaultAction() override;

    void DoRequestAction(EOperate operate, ITileID tile);

    void SetRequestID(int id);
    void SetOperates(MJOperates operates);
    BotActionOperation(LYGCBot* bot, std::chrono::milliseconds delay, bool useAI);
    void SetCiTiles(const std::vector<ITileID> tiles);

private:
    int m_ReqID{};
    MJOperates m_Operates{};
    std::map<std::string, EOperate> m_mOperateIDs{g_MJOperateIDs};
    std::vector<ITileID> m_CiTiles;
};