#pragma once

#include "LYGCDefine.h"
#include "LYGCAI.h"

class LYGCBot;

// AI������Ϣ�ص�
class BotAction
{
public:
    BotAction(LYGCBot* bot, std::chrono::milliseconds delay, bool useAI);
    virtual ~BotAction() = default;
    // ����true����ʾ�Ѿ�����ִ�ж���
    bool OnTick();

    void OnAIReply(AIReplyData const& data);

    virtual bool RunAIAction() = 0;
    virtual void RunDefaultAction() = 0;

protected:
    /// @brief ˼��ʱ����һ�����ֵ����AI��˼��ʱ���ڷ��ؽ��������˼������ʱ�ظ��������Ϸ��
    /// ��˼��ʱ����AIδ���أ��������������ʱ�����ڴ��ڼ�AI���ؽ�������粨�����������ظ���Ϸ��
    /// ����ʱ������δ�յ��ظ�������Miss������ȡ�ͼ�Ĭ�ϲ����ظ���Ϸ��
    /// LongExpiredTime Ӧ���ò�С��(>=) ThinkExpireTime
    std::chrono::system_clock::time_point m_ThinkExpireTime;  //˼��ʱ��
    std::chrono::system_clock::time_point m_LongExpiredTime;  // ����ʱ��

    LYGCBot* m_Bot{};
    AIReplyData m_Reply{};
    bool m_HasReplied = false;
    bool m_UseAI = false;
};

class BotActionOperation : public BotAction
{
public:
    using BotAction::BotAction;
    // ͨ�� BotAction �̳�
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