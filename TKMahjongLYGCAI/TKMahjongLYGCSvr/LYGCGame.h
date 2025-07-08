#pragma once
#include "LYGCDefine.h"
#include "LYGCPlay.h"
#include "LYGCMessager.h"
#include "LYGCRecord.h"
#include "MJService.h"

// 仅提供基础建设，不实现具体游戏逻辑
class LYGCGame : public MJGame
{
public:
    LYGCGame();
    // sub-modules
    std::shared_ptr<LYGCMessager> GetMessager() const;
    std::shared_ptr<MJCheckerOnce> GetChecker() const;
    std::shared_ptr<LYGCPlay> GetPlay() const;
    void SendMsg(TKMobileAckMsg& ack, ISeatID seat = SEAT_ALL);

public:  // RoundData 局间数据 (LastHandInfo)
    LYGCLastHandData& GetRoundData();
    void UpdateRoundData();
    virtual int GetLastHandSaveInfoLen() override;

protected:
    std::shared_ptr<LYGCMessager> m_Messager;
    std::shared_ptr<MJCheckerOnce> m_Checker;
    LYGCLastHandData m_RoundData;

protected:
    // 通过 MJGame 继承
    void OnStart() override;
    void _CheckCiBeiShuParam();
    virtual void OnPlayerTrustedChange(ISeatID seat, ETrustType type) override;
    virtual void OnReqMsg(ISeatID seat, PTKHEADER pMsg) override;
    virtual void OnHttpBrokerAck(BrokerAckData const& data) override;
    virtual std::shared_ptr<MJRecord> OnCreateRecord() override;
    virtual std::shared_ptr<MJPlay> OnCreatePlay() override;
    virtual std::vector<int> OnGetDefaultProperties() override;
    virtual MJFDConfigMap OnGetFDTableConfigMap() override;
    MJDrawConfigs GetDrawConfigs() override;

protected:
    // 通过 CTKGame 继承
    virtual BOOL PlayerCanLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason) override;
    virtual void OnPlayerLeaveDyn(CTKGamePlayer* pGamePlayer, DWORD dwReason) override;
};

class LYGCService : public MJService<LYGCGame, MJGamePlayer, TK_GAMEID_MJLYGC, 21900>
{
public:
    LYGCService();
    void InitService() override;
    std::shared_ptr<MJHuBase> OnCreateHuTool() override;

public:
    const std::map<ITileID, int>& GetTilesMap(std::shared_ptr<MJConfig> conf);

private:
    void _InitTilesMap();

private:
    std::map<ITileID, int> m_TilesMap;
    std::map<ITileID, int> m_TilesMapFeng;
};
