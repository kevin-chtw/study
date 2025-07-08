#include "BotAction.h"
#include "LYGCBot.h"
#include "ExpireTime.h"

BotAction::BotAction(LYGCBot* bot, std::chrono::milliseconds delay, bool useAI)
{
    m_Bot = bot;
    auto now = std::chrono::system_clock::now();
    m_ThinkExpireTime = now + delay;
    m_LongExpiredTime = now + std::chrono::milliseconds(std::max((long long)delay.count(), DefaultAiExpiredTime));
    m_UseAI = useAI;
}

bool BotAction::OnTick()
{
    if (!IsExpireTime(m_ThinkExpireTime))
    {
        return false;
    }

    if (!m_UseAI)
    {
        this->RunDefaultAction();
        return true;
    }

    if (!m_HasReplied && !IsExpireTime(m_LongExpiredTime))
    {
        return false;
    }

    if (m_Reply.Success && this->RunAIAction())
    {
        return true;
    }

#ifdef _TRAINER
    return false;
#else
    this->RunDefaultAction();
    return true;
#endif
}

void BotAction::OnAIReply(AIReplyData const& data)
{
    m_Reply = data;
    m_HasReplied = true;
}

bool BotActionOperation::RunAIAction()
{
    auto operate = GetOperateID(m_Reply.Action, m_mOperateIDs);

    if (operate == EOperateNone || (operate != EOperatePass && !m_Operates.HasOperate(operate)))
    {
        return false;
    }
    auto tile = MJ::GetTileAINameCvt()->NameToId(m_Reply.Content);
    DoRequestAction(operate, tile);
    return true;
}

void BotActionOperation::RunDefaultAction()
{
    if (m_Operates.HasOperate(EOperateCI))
    {
        auto tile = m_CiTiles.empty() ? 0 : m_CiTiles.front();
        DoRequestAction(EOperateCI, tile);
    }
    else if (m_Operates.HasOperate(EOperateHu))
    {
        DoRequestAction(EOperateHu, 0);
    }
    else if (m_Operates.HasOperate(EOperateDiscard))
    {
        DoRequestAction(EOperateDiscard, 0);
    }
    else
    {
        DoRequestAction(EOperatePass, 0);
    }
}

void BotActionOperation::DoRequestAction(EOperate operate, ITileID tile)
{
    TKMobileReqMsg mobileReq;
    auto pReqMsg = mobileReq.mutable_mahjonglygc_req_msg();
    auto pReq = pReqMsg->mutable_tkmahjongrequst_req_msg();
    pReq->set_requestid(m_ReqID);
    pReq->set_requesttype((int)operate);
    pReq->set_seat(m_Bot->GetSeat());
    pReq->set_tileid(tile);
    m_Bot->SendReqMsgToSvr(mobileReq);
}

void BotActionOperation::SetRequestID(int id)
{
    m_ReqID = id;
}

void BotActionOperation::SetOperates(MJOperates operates)
{
    m_Operates = operates;
}
BotActionOperation::BotActionOperation(LYGCBot* bot, std::chrono::milliseconds delay, bool useAI) : BotAction(bot, delay, useAI)
{
    m_mOperateIDs["Tsi"] = EOperateCI;
}

void BotActionOperation::SetCiTiles(const std::vector<ITileID> tiles)
{
    m_CiTiles = tiles;
}
