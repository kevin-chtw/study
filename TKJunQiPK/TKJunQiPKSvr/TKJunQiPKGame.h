#pragma once
#include "TKDllGame.h"
#include "TKJunQiPKData.h"

class CTKJunQiPKGame : public CTKGame
{
public:
    CTKJunQiPKGame(void);
    ~CTKJunQiPKGame(void);

    //实现接口
public:
    virtual BOOL OnGameBegin();
    virtual BOOL OnPlayerMsg(DWORD userID, int seatOrder, PTKHEADER pMsg);
    virtual void OnGameTimer(DWORD dwID, ULONG_PTR ulUserData);
    virtual int GetLastHandSaveInfoLen();  //必须重载，返回上局信息保存的长度，即m_pShareGameData的使用字节数
    virtual void OnPlayerNetBreak(CTKGamePlayer* pGamePlayer);
    virtual void OnPlayerNetResume(CTKGamePlayer* pGamePlayer);
    virtual void OnPlayerExitMatch(CTKGamePlayer* pGamePlayer);

public:
    void SendMsg(TKMobileAckMsg& rMsg, int nSeat = JUNQIPK_UNKNOWN);

private:
    std::shared_ptr<CTKJunQiPKData> m_pJunQiPKData;
};
