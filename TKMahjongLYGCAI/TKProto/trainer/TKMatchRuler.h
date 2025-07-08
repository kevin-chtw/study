#pragma once
//#include <WinSock2.h>
#include "framework.h"
//#include "ms_mvs_mis_gs_mc_mic_share_define.h"
#include "TKMatchS2GameSProtocolSrv.h"
#include "TKBuffer.h"

class CTKMatchRuler
{
public:
    CTKMatchRuler(void);
    virtual ~CTKMatchRuler(void);

public:
    void SetStageRuler(PTKSTAGERULER pStageRuler);
    void SetRoundRuler(PTKROUNDRULER pRoundRuler);
    void SetGameRuler(PTKGAMERULER pGameRuler);
    void SetTableRuler(int nSizeofTableRuler, PTKTABLERULER pTableRuler);
    void SetJsonRuler(int nSizeofJsonRuler, char* pJsonRuler);

public:
    int ScoreBase();  //return m_pStageRuler->nScoreBase;
    int AILevel()
    {
        return m_pStageRuler->nAILevel;
    }
    int SaveType()
    {
        return m_pStageRuler->nSaveFile;
    }
    BOOL ShowResultTextPlay()
    {
        return m_pStageRuler->bShowResultTextPlay;
    }
    char* StageRulerName()
    {
        return m_pStageRuler->szStageRulerName;
    }
    int SecondScoreType()
    {
        return m_pStageRuler->nSecondScoreType;
    }
    BOOL CheckWaitReGroup()
    {
        return m_pStageRuler->bCheckWaitReGroup;
    }
    BOOL NeedUpdateGrowValuePerGame()
    {
        return m_pStageRuler->bUpdateGrowValuePerGame;
    }
    int OutRoundBaseRate()
    {
        return m_pStageRuler->nOutRoundBaseRate;
    }
    int ScoreMultType()
    {
        return m_pStageRuler->nScoreMultType;
    }
    int OutRoundWarningBaseMult()
    {
        return m_pStageRuler->nOutRoundWarningBaseMult;
    }
    char* TitleName()
    {
        return m_pStageRuler->szTitleName;
    }
    int TotalBoutCount()
    {
        return m_pStageRuler->nBoutCount;
    }
    int AutoPopRulePanStyle()
    {
        return m_pStageRuler->nAutoPopRulePan;
    }
    char* ScoreType()
    {
        return m_pStageRuler->szScoreType;
    }
    int ExchangeRate()
    {
        return m_pStageRuler->nExchangeRate;
    }
    BOOL CanRebuy()
    {
        return m_pStageRuler->bCanRebuy > 0;
    }
    BOOL ShowOtherPlayerOrder()
    {
        return m_pStageRuler->bShowOtherPlayerOrder;
    }

    int RoundType();             //{				return m_pRoundRuler->nType;}
    char* RoundRulerName();      //{		return m_pRoundRuler->szRoundRulerName;}
    char* RoundRulerNote();      //{		return m_pRoundRuler->szRoundRulerNote;}
    int RoundGameCount();        //{		return m_pRoundRuler->nGameCount;}
    int RoundMinWinGameCount();  //{	return m_pRoundRuler->nMinWinGameCount;}
    int Overtime();              //{				return m_pRoundRuler->nOvertime;}

    int PlayersPerTable()
    {
        return m_stGameRuler.nPlayersPerTable;
    }
    int GetExChangeRate32_StageRuler()
    {
        return 1;
    }

    char* GameProperty();  // return m_pGameRuler->szProperty
    void SetProperyEx(const std::string& propertyEx);
    char* GamePropertyEx();  // return m_pGameRuler->szPropertyEx
    char* GameJsonRule(int& nJsonRuleLen);

public:
    void GetStageRuler(PTKSTAGERULER pStageRuler);
    PTKSTAGERULER GetStageRuler()
    {
        return m_pStageRuler;
    };
    PTKGAMERULER GetGameRuler();

private:
    PTKSTAGERULER m_pStageRuler;
    PTKROUNDRULER m_pRoundRuler;

    //
    int m_nSizeofTableRuler;
    TKTABLERULER m_stTableRuler;

    TKGAMERULER m_stGameRuler;

    CTKBuffer m_bufferJsonRule;
    std::string m_PropertyEx;
};
