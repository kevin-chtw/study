#include "LYGCMsgDispatcher.h"

void LYGCMsgDispatcher::Dispatch(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    using FuncType = bool (LYGCMsgDispatcher::*)(const MahJongLYGCAckMsg& ack, ISeatID seat);

    static constexpr FuncType funcs[] = {
        &LYGCMsgDispatcher::_TryHandleGameBeginAck,
        &LYGCMsgDispatcher::_TryHandlePlaceAck,
        &LYGCMsgDispatcher::_TryHandleOpenDoorAck,
        &LYGCMsgDispatcher::_TryHandleActionAck,
        &LYGCMsgDispatcher::_TryHandlePengAck,
        &LYGCMsgDispatcher::_TryHandleGangAck,
        &LYGCMsgDispatcher::_TryHandleWinAck,
        &LYGCMsgDispatcher::_TryHandleDiscardTileAck,
        &LYGCMsgDispatcher::_TryHandleDrawTileAck,
        &LYGCMsgDispatcher::_TryHandleTrustPlayAck,
        &LYGCMsgDispatcher::_TryHandleScoreChangeAck,
        &LYGCMsgDispatcher::_TryHandleResultAck,
        &LYGCMsgDispatcher::_TryHandleMessageAck,
        &LYGCMsgDispatcher::_TryHandleDissolutionAck,
        &LYGCMsgDispatcher::_TryHandleCallDataAck,
        &LYGCMsgDispatcher::_TryHandleBeginAnimalAck,
        &LYGCMsgDispatcher::_TryHandleHandTilesAck,
        &LYGCMsgDispatcher::_TryHandleGenZhuangAck,
        &LYGCMsgDispatcher::_TryHandleFanCiAck,
        &LYGCMsgDispatcher::_TryHandleDebugLogAck,
    };
    for (auto func : funcs)
    {
        if ((this->*func)(ack, seat))
        {
            return;
        }
    }
}

bool LYGCMsgDispatcher::_TryHandleGameBeginAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjonggamebegin_ack_msg())
    {
        OnGameBeginAck(ack.tkmahjonggamebegin_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandlePlaceAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongplace_ack_msg())
    {
        OnPlaceAck(ack.tkmahjongplace_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleOpenDoorAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongopendoor_ack_msg())
    {
        OnOpenDoorAck(ack.tkmahjongopendoor_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleActionAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongaction_ack_msg())
    {
        OnActionAck(ack.tkmahjongaction_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandlePengAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongpeng_ack_msg())
    {
        OnPengAck(ack.tkmahjongpeng_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleGangAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjonggang_ack_msg())
    {
        OnGangAck(ack.tkmahjonggang_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleWinAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongwin_ack_msg())
    {
        OnWinAck(ack.tkmahjongwin_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleDiscardTileAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongdiscardtile_ack_msg())
    {
        OnDiscardTileAck(ack.tkmahjongdiscardtile_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleDrawTileAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongdrawtile_ack_msg())
    {
        OnDrawTileAck(ack.tkmahjongdrawtile_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleTrustPlayAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongtrustplay_ack_msg())
    {
        OnTrustPlayAck(ack.tkmahjongtrustplay_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleScoreChangeAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongscorechange_ack_msg())
    {
        OnScoreChangeAck(ack.tkmahjongscorechange_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleResultAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongresult_ack_msg())
    {
        OnResultAck(ack.tkmahjongresult_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleMessageAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongmessage_ack_msg())
    {
        OnMessageAck(ack.tkmahjongmessage_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleDissolutionAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongdissolution_ack_msg())
    {
        OnDissolutionAck(ack.tkmahjongdissolution_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleCallDataAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongcalldata_ack_msg())
    {
        OnCallDataAck(ack.tkmahjongcalldata_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleBeginAnimalAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongbeginanimal_ack_msg())
    {
        OnBeginAnimalAck(ack.tkmahjongbeginanimal_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleHandTilesAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjonghandtiles_ack_msg())
    {
        OnHandTilesAck(ack.tkmahjonghandtiles_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleGenZhuangAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjonggenzhuang_ack_msg())
    {
        OnGenZhuangAck(ack.tkmahjonggenzhuang_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleFanCiAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongfanci_ack_msg())
    {
        OnFanCiAck(ack.tkmahjongfanci_ack_msg(), seat);
        return true;
    }
    return false;
}

bool LYGCMsgDispatcher::_TryHandleDebugLogAck(const MahJongLYGCAckMsg& ack, ISeatID seat)
{
    if (ack.has_tkmahjongdebuglog_ack_msg())
    {
        OnDebugLogAck(ack.tkmahjongdebuglog_ack_msg(), seat);
        return true;
    }
    return false;
}