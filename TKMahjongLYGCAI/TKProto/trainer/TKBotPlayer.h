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
    DWORD m_dwTotalCount;  // �ܲ�������
};

enum EMatchType
{
    MatchType_Unknown = 0,        // δ֪
    MatchType_Championships = 1,  // ������
    MatchType_FreeTable = 2,      // ������
    MatchType_Standard = 3,       // �����
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
    virtual std::string __GetHands();  //��ȡÿ����ҵ�����

public:
    void __InitGame(CTKGame* game, int seat);
    bool __GetAiRequest(std::string& req);
    bool __HandAction(const std::string& res);

private:
    void _InitEnterRound();
    void _InitBeginHand();

public:
    BYTE m_byAIType{52};     //������������
    BOOL m_bPacerBot{TRUE};  // �߼�AI������
    int m_nAILevel{101};
    DWORD m_dwMatchID{};  //�û����ڱ�����MatchID
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

public:                           // ���ƽ׶��������
    int m_nTotlePhase;            // �ܵĽ׶���
    int m_nCurPhase;              // ��ǰ�׶�
    int m_nTotleBout;             // ��ǰ�׶�������
    int m_nCurBout;               // ��ǰ�׶δ��ڵڼ���
    int m_nTotalHand;             // ��ǰ���ܸ���
    int m_nCurHnad;               // ��ǰ�ֵ�ǰ����
    int m_nTotalCnt;              // ��ǰ��������
    int m_nCurPromotCnt;          // ��ǰ�ֽ�������
    int m_nOrder;                 // ��ǰ����
    BOOL m_bGrouping;             // �Ƿ��Ƿ���
    int m_nCVWin;                 // ��ǰ��ʤ����
    int m_nScore;                 // ��ǰ����
    int m_nStageRule;             // ����
    int m_nRoundRule;             // ����
    std::string m_strGroupName;   // �������ƣ����ڷ���״̬ʱ��Ч
    std::string m_strPromotRule;  // ������������

    std::vector<MatchWinnerCount> m_vecWinnerCount;    // ���ֽ��������б�
    std::map<DWORD, mvs::attribute*> m_mapMatchAttr;   // �������Ա�
    std::map<DWORD, mvs::attribute*> m_mapPlayerAttr;  // ������Ա�
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
