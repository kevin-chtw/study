#pragma once
#include <map>
#include "TKGeneralProtocol.h"
#include "ms_mvs_mis_gs_mc_mic_share_define.h"
#include "TKMatchS2GameSProtocolSrv.h"
#include "TKGameProtocol.h"

#include "TKDllBotInterface.h"
#include "TKDllObject.h"

struct MatchWinnerCount
{
    MatchWinnerCount(int nPhase, BOOL bGrouping, int nBout, DWORD dwWinnerCount, DWORD dwTotalCount)
        : m_nPhase(nPhase), m_bGrouping(bGrouping), m_nBout(nBout), m_dwWinnerCount(dwWinnerCount), m_dwTotalCount(dwTotalCount)
    {
    }
    int m_nPhase;
    BOOL m_bGrouping;
    int m_nBout;
    DWORD m_dwWinnerCount;
    DWORD m_dwTotalCount;  // 总参赛人数
};

enum EMatchType
{
    MatchType_Unknown = 0,        // 未知
    MatchType_Championships = 1,  // 锦标赛
    MatchType_FreeTable = 2,      // 自由桌
    MatchType_Standard = 3,       // 达标赛
};

class CTKGame;
class CTKBotPlayer : public CTKObject
{
public:
    CTKBotPlayer();
    virtual ~CTKBotPlayer();

public:
    virtual BOOL OnInitialUpdate();
    virtual BOOL OnMsg(PTKHEADER pMsg);
    virtual BOOL OnTickCount();
    virtual void SetThinkTickCount(int nTickCount);
    virtual BOOL SendMsg(PTKHEADER pMsg);
    virtual void Terminate(BOOL bWhenSendOver = FALSE);

public:
    virtual std::string __GetHands();  //获取每个玩家的手牌

public:
    void __InitGame(CTKGame* game, int seat);
    bool __GetAiRequest(std::string& req);
    bool __HandAction(const std::string& res);

private:
    void _InitEnterRound();
    void _InitBeginHand();

public:
    BYTE m_byAIType{52};     //陪打机器人类型
    BOOL m_bPacerBot{TRUE};  // 高级AI仿真人
    int m_nAILevel{101};
    DWORD m_dwMatchID{};  //用户所在比赛的MatchID
    DWORD m_dwProductID{};
    DWORD m_dwBotUserID{};

public:
    int GetMatchType()
    {
        return m_nMatchType;
    }
    int m_nMatchType{};

private:
    CTKGame* m_Game{};

    int m_Seat{};
    std::string m_AIRequest;
    WORD m_wOrigine{};
    DWORD m_dwSerial{};
    int m_msgType{};

    std::vector<std::vector<BYTE>> m_Requests;

protected:
    mvs::attribute* GetMatchPrivateAttribute(DWORD dwAttributeID);
    mvs::attribute* GetPlayerPrivateAttribute(DWORD dwAttributeID);

public:                           // 赛制阶段相关属性
    int m_nTotlePhase;            // 总的阶段数
    int m_nCurPhase;              // 当前阶段
    int m_nTotleBout;             // 当前阶段总轮数
    int m_nCurBout;               // 当前阶段处于第几轮
    int m_nTotalHand;             // 当前局总副数
    int m_nCurHnad;               // 当前局当前副数
    int m_nTotalCnt;              // 当前轮总人数
    int m_nCurPromotCnt;          // 当前轮晋级人数
    int m_nOrder;                 // 当前排名
    BOOL m_bGrouping;             // 是否是分组
    int m_nCVWin;                 // 当前连胜盘数
    int m_nScore;                 // 当前分数
    int m_nStageRule;             // 赛制
    int m_nRoundRule;             // 局制
    std::string m_strGroupName;   // 分组名称，处于分组状态时有效
    std::string m_strPromotRule;  // 晋级规则描述

    std::vector<MatchWinnerCount> m_vecWinnerCount;    // 各轮晋级人数列表
    std::map<DWORD, mvs::attribute*> m_mapMatchAttr;   // 比赛属性表
    std::map<DWORD, mvs::attribute*> m_mapPlayerAttr;  // 玩家属性表
};

extern CTKBotPlayer* TKCreateBotPlayer();

#define IMPLEMENT_CREATEBOTPLAYER(type) \
    CTKBotPlayer* TKCreateBotPlayer()   \
    {                                   \
        return new type;                \
    }

////
BOOL TKWriteLog(const char* pszFmt, ...);
int TKGenRandom();
int TKGetMatchType(DWORD dwRule);
