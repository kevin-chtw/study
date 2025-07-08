#pragma once
#include <string>
#include "TKProtocol.h"
#include "TKFixList.h"

//#include "TKDllObjectLock.h"
//#include "TKGameDllBaseType.h"
#include "TKBuffer.h"
//#include "TKDllGameInterface.h"
#include "TKDllObject.h"
#include "TKStatTypeDefine.h"

#define TK_PLAYERNETSTATUS_NORMAL 0
#define TK_PLAYERNETSTATUS_BREAK 1

class CTKGamePlayer : public CTKObject
{
public:
    CTKGamePlayer(void);
    virtual ~CTKGamePlayer(void);

public:
    //BOOL Init(HEXEGAMEPLAYER hExePlayer, PTKDLLPLAYERCTREATEINFO pInfo);
    //BOOL Init(HEXEGAMEPLAYER hExePlayer, PTKDLLPLAYERCTREATEINFOV2 pInfo);
    //HEXEGAMEPLAYER GetPlayerShell()
    //{
    //    return m_hExePlayer;
    //};
    //BOOL DoFuncCall(PTKDLLGAMECALLPARAM lpCallParam, const char* pszFile, int nLine, char* Func);
    //BOOL DoEventCall(PTKDLLGAMECALLPARAM lpCallParam);
    //static BOOL WINAPI OnEventCall(PTKDLLGAMECALLPARAM lpCallParam);

public:
    LONG AddRef();
    LONG Release();

protected:
    //CTKObjectLock m_clsLock;

private:
    LONG m_cnRef = 1;

public:
    virtual BOOL OnInitialUpdate();
    virtual BOOL OnTickCount();
    virtual void OnDataBalance();  //�û����ݽ��㣨ע��Ҫ��֤�������ظ����㣡��
    virtual BOOL EnableEmotion(DWORD dwUserIDFrom, DWORD dwUserIDTo)
    {
        return TRUE;
    };

public:
    //
    void SetGameStart(DWORD dwCardPower, DWORD dwReserve1, DWORD dwReserve2, DWORD dwReserve3, DWORD dwReserve4,
        DWORD dwReserve5, DWORD dwReserve6, DWORD dwReserve7, DWORD dwReserve8);
    void SetGameResult(int nWinPlus, int nLossPlus, int nDrawPlus, int& nScorePlus, int nScoreType,
        PTKSTAGERULER pStageRuler, BOOL bValideHand = TRUE, BOOL bLevyTax = TRUE, WORD wGameResult = TK_UNKNOWN);
    void SetGameResult(int nWinPlus, int nLossPlus, int nDrawPlus, int64_t& i64ScorePlus, int nScoreType,
        PTKSTAGERULER pStageRuler, BOOL bValideHand = TRUE, BOOL bLevyTax = TRUE, WORD wGameResult = TK_UNKNOWN);
    void SetGameActor(WORD wGameActor);
    void SetTaxMultiDiv(int nMulti, int nDiv);

    void SendTipMessage(int nTipID, BYTE byShowType, const char* pszTipMessage, BOOL bSend2Watcher = TRUE);
    void SendTipMessage(BYTE byShowType, const char* pszTipMessage, BOOL bSend2Watcher = TRUE);

    void InvokeAddBreakUser();  //����û���Ϸ��ʱ�ˣ����Ե���һ�´˺������п����û�������û�н�����Ϸ��
    void InvokeMasterRound(BOOL bInvoke);  //TRUE��ʾ����, FALSE��ʾɾ��

    int GetTax(TKSTAGERULER* pStageRuler);

    BOOL GetGrowValue(DWORD dwGrowID, int& nValue);
    BOOL SetGrowValue(DWORD dwGrowID, int nValue, BOOL bSave2DB = TRUE);
    BOOL SetGrowValueInit(DWORD dwGrowID, int nValue);
    BOOL AddGrowValue(DWORD dwGrowID, int nValue, BOOL bSave2DB = TRUE);
    void SaveGrowValue();
    void UpdateGrowValue();
    BOOL AddAtomGameEvent(DWORD dwEventID, DWORD dwEventValue, BOOL bImmediate = FALSE,
        int nOverride = 0);  //nOverride 0:�ۼ�, 1:ֱ�Ӹ���, 2:��󸲸�
    void SaveAtomGameEvent(BOOL bImmediate = FALSE);
    //nEKLLogLevel��־����ο�ö��eELKLogLevel
    /*enum eELKLogLevel{
		TK_ELK_LOG_LEVEL_OFF = 0,
		TK_ELK_LOG_LEVEL_FATAL = 1,
		TK_ELK_LOG_LEVEL_ERROR = 2,
		TK_ELK_LOG_LEVEL_WARN = 3,
		TK_ELK_LOG_LEVEL_INFO = 4,
		TK_ELK_LOG_LEVEL_DEBUG = 5,
		TK_ELK_LOG_LEVEL_TRACE = 6,
		TK_ELK_LOG_LEVEL_ALL = 7
	};*/
    BOOL TKWriteELKLogFile(int nEKLLogLevel, LPCTSTR pszFmt, ...);
    // �ϱ���Ϣ
    // dwType:1,ͨ����־��ʽ�ϱ�,��Ҫ������־�ȼ�(nEKLLogLevel)����־����
    // dwType:2,ͨ����Ϣ�ϱ�,ֻ��Ҫ������־����
    BOOL TKReportMsg(DWORD dwType, int nEKLLogLevel, LPCTSTR pszFmt, ...);
    // CMA ��/��ͳ��ָ��
    BOOL CMAIncreaseCnt(const std::string& strName, DWORD dwMPID, DWORD dwGameID, int nIncrease = 1);
    // CMA Set ͳ��ָ��
    BOOL CMASetCnt(const std::string& strName, DWORD dwMPID, DWORD dwGameID, int value = 0);

    // For ����
    void PostExitMatch(BOOL bDirExit);
    void SendMsg(void* pMsg);
    bool WriteShareData(const char* pData, int nLen);
    bool ReadShareData(char** pData, int& nLen);
    void DeleteShareData();

public:
    void Terminate(BOOL bWhenSendOver = FALSE);
    void EnableBotEnter(BOOL bEnable)
    {
        m_bEnableBotEnter = bEnable;
    }

public:
    BOOL IsBotUser();
    BOOL IsExitGame();
    BOOL IsLobbyOnLine();  //��ʼʱ�����Ƿ�����
    BOOL IsMsTraced();     //�Ƿ����������ɱ��

public:
    int64_t Chip()
    {
        return m_nScore;
    };

public:
    void __Initialize(int seat, int chip);

public:
    //
    DWORD m_dwUserID{};
    char m_szNickName[TK_BUFLEN_NICKNAME]{};
    int m_nFigureID{};   //�û�ͷ��ID
    DWORD m_dwFormID{};  //�û�����ID wuqy add 2017.4.5
    WORD m_wCntTID{};    //�����ͻ����������� eLobbyConnetType
    BYTE m_byOSType{};
    DWORD m_dwAppID{};
    DWORD m_dwSiteID{};
    DWORD m_dwMoneyAccType{};
    DWORD m_dwLanguageID{};  // �ͻ������԰汾ID

    //
    int m_nGameID{};
    int m_nProductID{};
    DWORD m_dwTourneyID{};  //�û����ڱ�����TourneyID
    DWORD m_dwMatchID{};    //�û����ڱ�����MatchID

    //
    CDword m_dwTeamID{};
    CDword m_dwCaptainID{};  //�������У��ӳ���UserID

    //
    CBool m_bEnterRound{};
    CBool m_bBaseTaxed{};
    CBool m_bCanExitGame{};   // ��TV��Ŀ�Ĳ��ж˿���ѡ���˳��ı���(�μ�LordTV����) wuqy 2014.9.30
    CBool m_bRoundWatcher{};  //�Ƿ����Թ��ߣ��˱������������Ϸ���Ϣ�Ŀͻ������
    CBool m_bEnableBotEnter{};

    CByte m_byBreakHeartTask{};  // �Ƿ�μ����������� wuqy 2016.6.3
    CByte m_byHeartCount{};  // �û���HeartCount�����������û�����������cnMatchLive��wuqy add 2016.12.31

    CInt m_nGameExpScore{};   // ��Ϸ����ֵ������UserData Server��
    CInt m_nMatchRank{};      //��Ϸ���������ݱ������ּ��������
    CInt m_nEnterOrder{};     //����ʱ������
    CInt m_nPlaceOrder{};     //
    CInt m_nOutRoundState{};  //����״̬��0û�г��֣�>0����
    CInt m_nSeatOrder{};

    //
    CInt64 m_nScore{};        //��������
    CInt m_nMatchScore{};     // ��������
    CInt m_nMatchSecScore{};  // ����С��
    CInt m_nRoundScore{};     // ���ƻ���
    CInt m_nRoundSecScore{};  // ����С��

    //Round�ۼ�Ӯ���
    CInt m_nRoundWins{};
    CInt m_nRoundLosses{};
    CInt m_nRoundDraws{};

    //Round�ۼ�Ӯ�õķ������������۷�
    CInt m_nRoundWinScore{};
    CInt m_nRoundGameCount{};
    CInt m_nRoundTax{};

    //Round�������Ϸ�÷�Ӯ���
    CInt m_nLastScore{};  //���һ�̵�˰ǰ�÷�(�����˰�ǻ������������Ϸ�����Լ���������÷�Ҳ�����˰���)
    CInt m_nLastScoreTax{};  //���һ�̵�˰��÷֣�Ŀǰֻ���ڿͻ��˽��������wuqy 2014.10.23
    CInt m_nLastWins{};
    CInt m_nLastLosses{};
    CInt m_nLastDraws{};
    CInt m_iCurGameHand{};  // �û���ǰ���еľ��Ʊ�ţ���%d�֣�����ʱ�ۼơ�Ϊ0�Ǳ�ʾû�д��������ʾ��
    CInt m_cnGameCurFinished{};  // ��ǰ��ɵ�������
    CInt m_cnGameTotFinished{};  // ������ɵ�������
    //
    CInt m_nBroadcastDelay{};  //ת���ӳ�ʱ��(��)
    CInt m_nVipSortValue{};    //���ǶԾ�VIPȨ��
    CInt m_nMatchStatus{};     //�û���ǰ�ı���״̬
    CInt m_nNetStatus{};       //����״̬��0�������ӣ�1���ߣ�2����
    CInt m_nOpertorType{};     //��Ϸ����������

    CWord m_wGameResult{};  // ��Ϸ���
    CWord
        m_wGameActor{};  // ��Ϸ������ı�����Ϸ��ɫ����ֵ��ÿ����Ϸ�Լ����壬���ض���Stage����ʱʹ�� wuqy 2016.12.31

    CDword m_dwChatChannelID{};
    CDword m_dwChatChannelSubID{};
    CDword m_dwFlags{};  //һЩ��־ 0λ��ʾ�Ƿ�ȷ���˳��˱���
    CDword m_dwMsTraceFlags{};
    CDword m_dwVersionMatchClient{};
    CDword m_dwVersionGameDll{};

    CInt m_nWatcheeSeat{};
    CDword m_dwWatcheeID{};
    CDword m_dwWatcheeFlags{};

    CTime_t m_timeMatchBreak{};
    CTime_t m_timeBeginMatch{};  //��ʼ������ʱ��
    CTime_t m_timeExitMatch{};   //�˳�������ʱ��

    CBool m_bPacerBot{};  // �߼�AI������

    DWORD m_dwMsIP{};
    WORD m_wMsPort{};
    CByte m_byPacerBotLevel{};

    CInt m_nGameOrder{};          // ����������1��ʼ
    CByte m_byUserTypeTag{};      //��ұ�ǩ
    CInt m_nEndGameCurMission{};  //�оֵ�ǰ�ؿ����
    CInt m_nDomain{};

private:
    //HDLLGAMEPLAYER m_hExePlayer;
    //FUNC_DLLGAME_CALLENTRY m_pCallEntry;
};
//extern LONG tk_cnTKDllPlayer;
//extern LONG tk_alDPEventCall[TK_DP_EVENTCALL_MAX];
