#include "MJGrowSystem.h"
#include "MJLog.h"

#include "tkgame2pivotprotocol.h"
#include "TKClassFunctor.h"
#include "TKDllServer.h"
#include <TKMatchS2GameSProtocolSrv.h>

using namespace tkclsfunctor;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MJLevelSystem::MJLevelSystem(CTKGame* in_pGame) : m_pGame(in_pGame)
{
    m_pLevelSystemConfigManager = new MJLevelSystemConfigManager();
    if (m_pLevelSystemConfigManager == nullptr)
    {
        assert(false);
        return;
    }
}

MJLevelSystem::~MJLevelSystem(void)
{
    m_pGame = nullptr;

    if (m_pLevelSystemConfigManager != nullptr)
    {
        delete m_pLevelSystemConfigManager;
        m_pLevelSystemConfigManager = nullptr;
    }
}

void MJLevelSystem::Init(unsigned long in_ulPID)
{
    if (!m_pLevelSystemConfigManager->LoadConfig(in_ulPID))
    {
        return;
    }
}

void MJLevelSystem::DumpDebug()
{
    for (int i = 0; i < 100000; i *= 2)
    {
        MJ_LOG_INFO(m_pGame, "%d 0: %d", i, CalculateEXPUpdate(i, false));
        MJ_LOG_INFO(m_pGame, "%d 1: %d", i, CalculateEXPUpdate(i, true));
    }
}

void MJLevelSystem::PostMsg2MS_SetEXP(MJGamePlayer* in_pPlayer, bool in_bWin)
{
    if (in_pPlayer == nullptr)
    {
        return;
    }

    int nEXPCur = (int)in_pPlayer->GetHonorValue();  // 当前荣誉值
    if (nEXPCur < 0)
    {
        return;
    }

    if (nEXPCur <= 0 && !in_bWin)
    {
        return;
    }

    int nEXPUpdate = CalculateEXPUpdate(nEXPCur, in_bWin);

    MJ_LOG_DEBUG(m_pGame, "exp update, seat:%d value:%d->%d win:%d", (int)in_pPlayer->m_nSeatOrder, (int)nEXPCur, (int)nEXPUpdate, (int)in_bWin);

    if (nEXPCur <= 0 && nEXPUpdate == 0 || nEXPCur == nEXPUpdate)
    {
        return;
    }

    typedef struct tagTKPLAYERMATCHGROWITEM
    {
        DWORD dwGrowID;
        DWORD dwValule;
    } TKPLAYERMATCHGROWITEM, *PTKPLAYERMATCHGROWITEM;

    TKPLAYERMATCHGROWITEM item{};
    item.dwGrowID = EXP_GROW_ID_VALUE;
    item.dwValule = nEXPUpdate - nEXPCur;

    CTKBuffer buffer;
    buffer.SetHeaderLen(sizeof(TKREQMODIFYPLAYERGROWINFO));
    buffer.Append(&item, 1 * sizeof(TKPLAYERMATCHGROWITEM));
    PTKREQMODIFYPLAYERGROWINFO pPush = (PTKREQMODIFYPLAYERGROWINFO)buffer.GetBufPtr();
    pPush->header.dwType = TK_REQ | TKID_GS2MS_MODIFYPLAYERGROWINFO;
    pPush->header.dwLength = buffer.GetBufLen() - sizeof(TKHEADER);
    pPush->dwProductID = m_pGame->m_nProductID;
    pPush->dwTourneyID = m_pGame->m_dwTourneyID;
    pPush->dwMatchID = m_pGame->m_dwMatchID;
    pPush->dwUserID = in_pPlayer->m_dwUserID;
    pPush->wGrowDomainID = EXP_DOMAIN_ID;
    pPush->dwGrowInfoOffset = sizeof(TKREQMODIFYPLAYERGROWINFO);
    pPush->dwGrowInfoCount = 1;

    tk_pDllServer->PostMsg2MS((PTKHEADER)pPush, TK_GS2MS_MSGTYPE_PLAYER, in_pPlayer->m_dwMsIP, in_pPlayer->m_wMsPort, in_pPlayer->m_dwUserID);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int MJLevelSystem::CalculateEXPUpdate(int in_nEXPCur, bool in_bWin)
{
    int nEXPUpdateTemp = in_nEXPCur;

    //////////////////////////////////////////////////////////////////////////
    // 一般配置
    int nWinRatio = 0;
    int nLoseRatio = 0;
    int nEXPMin = 0;
    for (int i = 0; i < (int)m_pLevelSystemConfigManager->m_vecsLevelInfo.size(); ++i)
    {
        MJLevelInfoNode& sLevelInfoT = m_pLevelSystemConfigManager->m_vecsLevelInfo[i];
        if (i == m_pLevelSystemConfigManager->m_vecsLevelInfo.size() - 1)
        {
            nWinRatio = sLevelInfoT.nWinRatio;
            nLoseRatio = sLevelInfoT.nLoseRatio;
            nEXPMin = sLevelInfoT.nEXPMin;
            break;
        }

        //////////////////////////////////////////////////////////////////////////
        MJLevelInfoNode& sLevelInfoNext = m_pLevelSystemConfigManager->m_vecsLevelInfo[i + 1];
        if (in_nEXPCur >= sLevelInfoT.nEXPMin && in_nEXPCur < sLevelInfoNext.nEXPMin)
        {
            nWinRatio = sLevelInfoT.nWinRatio;
            nLoseRatio = sLevelInfoT.nLoseRatio;
            nEXPMin = sLevelInfoT.nEXPMin;
            break;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // 特殊配置 special
    for (int i = 0; i < (int)m_pLevelSystemConfigManager->m_vecLevelConfigSpecial.size(); ++i)
    {
        MJLevelConfigSpecial& sSpecial = m_pLevelSystemConfigManager->m_vecLevelConfigSpecial[i];
        if (m_pGame->m_nProductID == sSpecial.ulPID && GetLevel(in_nEXPCur) >= sSpecial.eLevelMin)
        {
            nWinRatio = sSpecial.nWinRatio;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // calculate
    if (in_bWin)
    {
        nEXPUpdateTemp += m_pLevelSystemConfigManager->m_nEXPBase * nWinRatio / 10;
    }
    else
    {
        nEXPUpdateTemp -= m_pLevelSystemConfigManager->m_nEXPBase * nLoseRatio / 10;
    }

    //////////////////////////////////////////////////////////////////////////
    // 晋级额外奖励（修正荣誉值）
    nEXPUpdateTemp += CalculateEXPAddition(in_nEXPCur, nEXPUpdateTemp);

    if (nEXPUpdateTemp < nEXPMin)
    {
        nEXPUpdateTemp = nEXPMin;  // 修正
    }

    return nEXPUpdateTemp;
}

EMJLevel MJLevelSystem::GetLevel(int in_nEXP)
{
    for (int i = 0; i < (int)m_pLevelSystemConfigManager->m_vecsLevelInfo.size(); ++i)
    {
        if (i == (int)m_pLevelSystemConfigManager->m_vecsLevelInfo.size() - 1)
        {
            return EMJLevel::eLevel_Max;
        }

        //////////////////////////////////////////////////////////////////////////
        MJLevelInfoNode& sLevelInfo = m_pLevelSystemConfigManager->m_vecsLevelInfo[i];
        MJLevelInfoNode& sLevelInfoNext = m_pLevelSystemConfigManager->m_vecsLevelInfo[i + 1];

        if (in_nEXP >= sLevelInfo.nEXPMin && in_nEXP < sLevelInfoNext.nEXPMin)
        {
            return sLevelInfo.eLevel;
        }
    }

    return EMJLevel::eLevel_Min;
}

int MJLevelSystem::GetEXPMin(EMJLevel in_eLevel)
{
    if (in_eLevel < EMJLevel::eLevel_Min)
    {
        in_eLevel = EMJLevel::eLevel_Min;
    }
    else if (in_eLevel > EMJLevel::eLevel_Max)
    {
        in_eLevel = EMJLevel::eLevel_Max;
    }

    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < (int)m_pLevelSystemConfigManager->m_vecsLevelInfo.size(); ++i)
    {
        MJLevelInfoNode& sLevelInfo = m_pLevelSystemConfigManager->m_vecsLevelInfo[i];
        if (in_eLevel == sLevelInfo.eLevel)
        {
            return sLevelInfo.nEXPMin;
        }
    }

    return 0;
}

int MJLevelSystem::GetAwardAddition(EMJLevel in_eLevel)
{
    int nValue = 0;
    for (int i = 0; i < (int)m_pLevelSystemConfigManager->m_vecsLevelInfo.size(); ++i)
    {
        MJLevelInfoNode& sLevelInfo = m_pLevelSystemConfigManager->m_vecsLevelInfo[i];
        if (in_eLevel == sLevelInfo.eLevel)
        {
            nValue = sLevelInfo.nAwardAddition;
            break;
        }
    }

    return nValue;
}

int MJLevelSystem::CalculateEXPAddition(int in_nEXPCur, int in_nEXPUpdate)
{
    EMJLevel&& eLevelCur = GetLevel(in_nEXPCur);
    EMJLevel&& eLevelUpdate = GetLevel(in_nEXPUpdate);
    if (eLevelCur >= eLevelUpdate)
    {
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////
    EMJLevel&& eLevelNextT = (EMJLevel)((int)eLevelCur + 1);
    int nEXPAddition = GetAwardAddition(eLevelNextT);  // 获取额外奖励配置

    // 递归
    int&& nEXPNextT = GetEXPMin(eLevelNextT);
    int&& nEXPUpdateT = in_nEXPUpdate + nEXPAddition;
    return nEXPAddition += CalculateEXPAddition(nEXPNextT, nEXPUpdateT);
}

/////*************************
/////*************************
/////*************************
//////////////////////////////////////////////////////////////////////////
MJLevelSystemConfigManager::MJLevelSystemConfigManager() : m_nEXPBase(0)
{
    m_vecsLevelInfo.clear();
    m_vecLevelConfigSpecial.clear();
}

MJLevelSystemConfigManager::~MJLevelSystemConfigManager()
{
    m_nEXPBase = 0;
    m_vecsLevelInfo.clear();
    m_vecLevelConfigSpecial.clear();
}

bool MJLevelSystemConfigManager::LoadConfig(unsigned long in_ulPID)
{
    // read xml file
    rapidxml::file<> fdoc("MJLevelSystem.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());
    rapidxml::xml_node<>* root = doc.first_node();
    if (root == nullptr)
    {
        return false;
    }
    for (auto* pElemConfig = root->first_node(); pElemConfig != nullptr; pElemConfig = pElemConfig->next_sibling())
    {
        auto* pchElemName = pElemConfig->name();
        if (pchElemName == nullptr)
        {
            continue;
        }

        if (strcmp(pchElemName, "PIDList") == 0)
        {
            LoadConfigPIDList(pElemConfig, in_ulPID);
        }
        else if (strcmp(pchElemName, "LevelList") == 0)
        {
            LoadConfigLevelList(pElemConfig);
        }
        else if (strcmp(pchElemName, "SpecialList") == 0)
        {
            LoadConfigSpecialList(pElemConfig);
        }
    }
    return true;
}
void MJLevelSystemConfigManager::LoadConfigPIDList(rapidxml::xml_node<>* pElemConfig, unsigned long in_ulPID)
{
    for (auto* pElemPIDList = pElemConfig->first_node(); pElemPIDList != nullptr; pElemPIDList = pElemPIDList->next_sibling())
    {  // pid info
        auto* attrPID = pElemPIDList->first_attribute("PID");
        if (attrPID == nullptr)
        {
            continue;
        }
        unsigned long ulPID = ::strtoul(attrPID->value(), nullptr, 10);
        if (in_ulPID == ulPID)
        {
            auto* attrEXPBase = pElemPIDList->first_attribute("EXPBase");
            if (attrEXPBase == nullptr)
            {
                break;
            }

            m_nEXPBase = ::strtol(attrEXPBase->value(), nullptr, 10);  // EXP base
            break;
        }
    }
}

void MJLevelSystemConfigManager::LoadConfigLevelList(rapidxml::xml_node<>* pElemConfig)
{
    for (auto* pElemLevelList = pElemConfig->first_node(); pElemLevelList != nullptr; pElemLevelList = pElemLevelList->next_sibling())
    {  // level info
        auto* attrName = pElemLevelList->first_attribute("name");
        auto* attrEXPMin = pElemLevelList->first_attribute("EXPMin");
        auto* attrWinRatio = pElemLevelList->first_attribute("win_ratio");
        auto* attrLoseRatio = pElemLevelList->first_attribute("lose_ratio");
        if (attrName == nullptr || attrEXPMin == nullptr || attrWinRatio == nullptr || attrLoseRatio == nullptr)
        {
            continue;
        }

        MJLevelInfoNode sLevelInfo;
        sLevelInfo.eLevel = ConvertLevelName2Enum(attrName->value());
        sLevelInfo.nEXPMin = strtol(attrEXPMin->value(), nullptr, 10);
        sLevelInfo.nWinRatio = strtol(attrWinRatio->value(), nullptr, 10);
        sLevelInfo.nLoseRatio = strtol(attrLoseRatio->value(), nullptr, 10);

        auto* attrAwardAddition = pElemLevelList->first_attribute("award_addition");
        if (attrAwardAddition != nullptr)
        {
            sLevelInfo.nAwardAddition = strtol(attrAwardAddition->value(), nullptr, 10);
        }

        m_vecsLevelInfo.push_back(sLevelInfo);  // level info
    }
}
void MJLevelSystemConfigManager::LoadConfigSpecialList(rapidxml::xml_node<>* pElemConfig)
{
    for (auto* pElemSpecialList = pElemConfig->first_node(); pElemSpecialList != nullptr; pElemSpecialList = pElemSpecialList->next_sibling())
    {  // level info
        auto* attrPID = pElemSpecialList->first_attribute("PID");
        auto* attrLevelMin = pElemSpecialList->first_attribute("level_min");
        auto* attrWinRatio = pElemSpecialList->first_attribute("win_ratio");
        if (attrPID == nullptr || attrLevelMin == nullptr || attrWinRatio == nullptr)
        {
            continue;
        }

        MJLevelConfigSpecial sLevelConfigSpecial;
        sLevelConfigSpecial.eLevelMin = ConvertLevelName2Enum(attrLevelMin->value());
        sLevelConfigSpecial.ulPID = strtoul(attrPID->value(), nullptr, 10);
        sLevelConfigSpecial.nWinRatio = strtol(attrWinRatio->value(), nullptr, 10);
        m_vecLevelConfigSpecial.push_back(sLevelConfigSpecial);  // level info special
    }
}

EMJLevel MJLevelSystemConfigManager::ConvertLevelName2Enum(const char* in_attrLevel)
{
    if (in_attrLevel == nullptr)
    {
        assert(false);
        return EMJLevel::eLevelInvalid;
    }

    //////////////////////////////////////////////////////////////////////////
    std::map<std::string, EMJLevel> name2levels = {
        //
        {
            "QingTong_1",
            EMJLevel::eLevelQingTong_1,
        },
        {
            "QingTong_2",
            EMJLevel::eLevelQingTong_2,
        },
        {
            "QingTong_3",
            EMJLevel::eLevelQingTong_3,
        },
        {
            "QingTong_4",
            EMJLevel::eLevelQingTong_4,
        },
        {
            "QingTong_5",
            EMJLevel::eLevelQingTong_5,
        },
        {
            "BaiYin_1",
            EMJLevel::eLevelBaiYin_1,
        },
        {
            "BaiYin_2",
            EMJLevel::eLevelBaiYin_2,
        },
        {
            "BaiYin_3",
            EMJLevel::eLevelBaiYin_3,
        },
        {
            "BaiYin_4",
            EMJLevel::eLevelBaiYin_4,
        },
        {
            "BaiYin_5",
            EMJLevel::eLevelBaiYin_5,
        },
        {
            "HuangJin_1",
            EMJLevel::eLevelHuangJin_1,
        },
        {
            "HuangJin_2",
            EMJLevel::eLevelHuangJin_2,
        },
        {
            "HuangJin_3",
            EMJLevel::eLevelHuangJin_3,
        },
        {
            "HuangJin_4",
            EMJLevel::eLevelHuangJin_4,
        },
        {
            "HuangJin_5",
            EMJLevel::eLevelHuangJin_5,
        },
        {
            "BoJin_1",
            EMJLevel::eLevelBoJin_1,
        },
        {
            "BoJin_2",
            EMJLevel::eLevelBoJin_2,
        },
        {
            "BoJin_3",
            EMJLevel::eLevelBoJin_3,
        },
        {
            "BoJin_4",
            EMJLevel::eLevelBoJin_4,
        },
        {
            "BoJin_5",
            EMJLevel::eLevelBoJin_5,
        },
        {
            "ZuanShi_1",
            EMJLevel::eLevelZuanShi_1,
        },
        {
            "ZuanShi_2",
            EMJLevel::eLevelZuanShi_2,
        },
        {
            "ZuanShi_3",
            EMJLevel::eLevelZuanShi_3,
        },
        {
            "ZuanShi_4",
            EMJLevel::eLevelZuanShi_4,
        },
        {
            "ZuanShi_5",
            EMJLevel::eLevelZuanShi_5,
        },
        {
            "WangZhe_1",
            EMJLevel::eLevelWangZhe_1,
        },
        {
            "WangZhe_2",
            EMJLevel::eLevelWangZhe_2,
        },
        {
            "WangZhe_3",
            EMJLevel::eLevelWangZhe_3,
        },
        {
            "WangZhe_4",
            EMJLevel::eLevelWangZhe_4,
        },
        {
            "WangZhe_5",
            EMJLevel::eLevelWangZhe_5,
        },
        {
            "QueSheng_1",
            EMJLevel::eLevelQueSheng_1,
        },
        {
            "QueSheng_2",
            EMJLevel::eLevelQueSheng_2,
        },
        {
            "QueSheng_3",
            EMJLevel::eLevelQueSheng_3,
        },
        {
            "QueSheng_4",
            EMJLevel::eLevelQueSheng_4,
        },
        {
            "QueSheng_5",
            EMJLevel::eLevelQueSheng_5,
        },
    };

    auto iter = name2levels.find(in_attrLevel);
    if (iter != name2levels.end())
    {
        return iter->second;
    }

    return EMJLevel::eLevelInvalid;
}

using LevelMsgCallFunc = std::function<BOOL(PTKHEADER)>;

class CFuncObjCallbackGrow : public CFuncObjCallback
{
public:
    CFuncObjCallbackGrow()
    {
    }

    CFuncObjCallbackGrow(LevelMsgCallFunc in_pfuncCallback)
    {
        m_pCallback = in_pfuncCallback;
    }

    CFuncObjCallbackGrow(CFuncObjCallbackGrow& in_funcObjCallback)
    {
        *this = in_funcObjCallback;
    }

    virtual ~CFuncObjCallbackGrow(void)
    {
    }

public:
    virtual BOOL Execute(void* in_pAckMsg)
    {
        if (m_pCallback == NULL)
        {
            return FALSE;
        }

        return m_pCallback.operator()((PTKHEADER)in_pAckMsg);
    }

private:
    LevelMsgCallFunc m_pCallback;
};

MJGrowSystem::MJGrowSystem(CTKGame* pGame) : m_LevelSystem(pGame)
{
    m_pGame = pGame;
}

void MJGrowSystem::LoadGrowValue(ISeatID seat, DWORD growID)
{
    this->PostMsg2MS_GetValue(seat, growID);
}

void MJGrowSystem::SaveGrowValue(ISeatID seat, DWORD growID, DWORD value)
{
    this->PostMsg2MS_SetValue(seat, growID, value);
}

void MJGrowSystem::SaveGrowValues(ISeatID seat, std::map<DWORD, DWORD> values)
{
    auto pPlayer = m_pGame->GetGamePlayerBySeat(seat);
    if (values.empty() || pPlayer == nullptr)
    {
        return;
    }

    CTKBuffer buffer;
    buffer.SetHeaderLen(sizeof(TKREQSAVEUSERGROWVALUE));

    for (auto& p : values)
    {
        buffer.Append(&p.first, sizeof(DWORD));
        buffer.Append(&p.second, sizeof(DWORD));
    }

    PTKREQSAVEUSERGROWVALUE pSend = (PTKREQSAVEUSERGROWVALUE)buffer.GetBufPtr();
    pSend->header.dwType = TK_REQ | TKID_GS2MS_SAVEUSERGROWVALUE;
    pSend->header.dwLength = buffer.GetBufLen() - sizeof(TKHEADER);
    pSend->dwTourneyID = m_pGame->m_dwTourneyID;
    pSend->dwMatchID = m_pGame->m_dwMatchID;
    pSend->dwUserID = pPlayer->m_dwUserID;
    pSend->cnGameGrowInfo = values.size();

    tk_pDllServer->PostMsg2MS((PTKHEADER)pSend, TK_GS2MS_MSGTYPE_PLAYER, pPlayer->m_dwMsIP, pPlayer->m_wMsPort, pPlayer->m_dwUserID);
}

void MJGrowSystem::LoadExp(ISeatID seat)
{
    this->PostMsg2MS_GetValue(seat, EXP_GROW_ID_VALUE);
}

void MJGrowSystem::UpdateExp(ISeatID seat, bool isWinner)
{
    auto pPlayer = dynamic_cast<MJGamePlayer*>(m_pGame->GetGamePlayerBySeat(seat));
    if (pPlayer)
    {
        m_LevelSystem.PostMsg2MS_SetEXP(pPlayer, isWinner);
    }
}
void MJGrowSystem::PostMsg2MS_GetValue(ISeatID seat, DWORD growID)
{
    auto pPlayer = dynamic_cast<MJGamePlayer*>(m_pGame->GetGamePlayerBySeat(seat));

    msg_gs2pivot_req_get_singleGrow req_msg_get_singleGrow;
    memset(&req_msg_get_singleGrow, 0, sizeof(req_msg_get_singleGrow));
    req_msg_get_singleGrow.dwType = TK_REQ | TKID_GS2PIVOT_GET_SINGLEGROW;
    req_msg_get_singleGrow.dwLength = sizeof(req_msg_get_singleGrow) - sizeof(TKHEADER);
    req_msg_get_singleGrow.dwUserID = pPlayer->m_dwUserID;
    req_msg_get_singleGrow.dwGrowID = growID;
    req_msg_get_singleGrow.dwMPID = m_pGame->m_nProductID;
    req_msg_get_singleGrow.dwGameID = m_pGame->m_nGameID;
    req_msg_get_singleGrow.dwPlatType = pPlayer->m_wCntTID;
    req_msg_get_singleGrow.byOSType = pPlayer->m_byOSType;
    req_msg_get_singleGrow.dwAppID = pPlayer->m_dwAppID;
    req_msg_get_singleGrow.dwSiteID = pPlayer->m_dwSiteID;
    req_msg_get_singleGrow.dwPort = PIVOT_PORT;

    CTKBuffer buf;
    buf.Append(&req_msg_get_singleGrow, sizeof(msg_gs2pivot_req_get_singleGrow));

    auto funcCallback = std::bind(&MJGamePlayer::OnPivotMsg, pPlayer, std::placeholders::_1, seat, growID);
    CFuncObjCallbackGrow* pFuncObjCallback = new CFuncObjCallbackGrow(funcCallback);
    // post
    tk_pDllServer->PostMsg2Pivot_GSBase((PTKHEADER)buf.GetBufPtr(), pPlayer->m_dwUserID, m_pGame->m_dwMatchID, m_pGame->m_wStageID, m_pGame->m_wRoundID,
        pFuncObjCallback);  //由PostMsg2Pivot_GSBase释放
}

void MJGrowSystem::PostMsg2MS_SetValue(ISeatID seat, DWORD growID, DWORD value)
{
    auto pPlayer = dynamic_cast<MJGamePlayer*>(m_pGame->GetGamePlayerBySeat(seat));
    msg_gs2pivot_req_deal_grow rReqGrow;
    memset(&rReqGrow, 0, sizeof(rReqGrow));
    rReqGrow.dwType = TKID_GS2PIVOT_DEAL_GROW | TK_REQ;
    rReqGrow.dwLength = sizeof(rReqGrow) - sizeof(TKHEADER);
    rReqGrow.dwUserID = pPlayer->m_dwUserID;
    rReqGrow.dwGrowID = growID;
    rReqGrow.nValue = value;
    rReqGrow.dwMPID = m_pGame->m_nProductID;
    rReqGrow.dwGameID = m_pGame->m_nGameID;
    rReqGrow.dwPlatType = pPlayer->m_wCntTID;
    rReqGrow.byOSType = pPlayer->m_byOSType;
    rReqGrow.dwAppID = pPlayer->m_dwAppID;
    rReqGrow.dwSiteID = pPlayer->m_dwSiteID;

    bool b = tk_pDllServer->PostMsg2Pivot_GSBase(&rReqGrow, pPlayer->m_dwUserID, m_pGame->m_dwMatchID, m_pGame->m_wStageID, m_pGame->m_wRoundID, nullptr);
}