#pragma once

#include <TKDllServer.h>
#include "TKDllGame.h"
#include "MJPlayer.h"

#include <vector>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <map>
#include <string>

/**
使用MJGrowSystem
    void LoadGrowValue(ISeatID seat, DWORD growID); 
    void SaveGrowValue(ISeatID seat, DWORD growID, DWORD value);
    void LoadExp(ISeatID seat);
    void UpdateExp(ISeatID seat, bool isWinner);
*/

#define PIVOT_PORT 30070
#define EXP_DOMAIN_ID 60013         // 麻将等级系统荣誉积分_域ID
#define EXP_GROW_ID_VALUE 60013038  // 麻将等级系统荣誉积分_用户荣誉积分
//////////////////////////////////////////////////////////////////////////
enum class EMJLevel
{
    eLevelInvalid = -1,

    eLevel_Min,
    eLevelQingTong_1 = eLevel_Min,
    eLevelQingTong_2,
    eLevelQingTong_3,
    eLevelQingTong_4,
    eLevelQingTong_5,

    eLevelBaiYin_1,
    eLevelBaiYin_2,
    eLevelBaiYin_3,
    eLevelBaiYin_4,
    eLevelBaiYin_5,

    eLevelHuangJin_1,
    eLevelHuangJin_2,
    eLevelHuangJin_3,
    eLevelHuangJin_4,
    eLevelHuangJin_5,

    eLevelBoJin_1,
    eLevelBoJin_2,
    eLevelBoJin_3,
    eLevelBoJin_4,
    eLevelBoJin_5,

    eLevelZuanShi_1,
    eLevelZuanShi_2,
    eLevelZuanShi_3,
    eLevelZuanShi_4,
    eLevelZuanShi_5,

    eLevelWangZhe_1,
    eLevelWangZhe_2,
    eLevelWangZhe_3,
    eLevelWangZhe_4,
    eLevelWangZhe_5,

    eLevelQueSheng_1,
    eLevelQueSheng_2,
    eLevelQueSheng_3,
    eLevelQueSheng_4,
    eLevelQueSheng_5,
    eLevel_Max = eLevelQueSheng_5,
    eLevelLast
};

struct MJLevelInfoNode
{
    EMJLevel eLevel{EMJLevel::eLevelInvalid};
    int nEXPMin{};
    int nWinRatio{};
    int nLoseRatio{};
    int nAwardAddition{};  // 额外奖励
};

struct MJLevelConfigSpecial
{
    unsigned long ulPID{};
    EMJLevel eLevelMin{EMJLevel::eLevelInvalid};
    int nWinRatio{};
};
//////////////////////////////////////////////////////////////////////////
class TiXmlElement;

class MJLevelSystemConfigManager
{
public:
    MJLevelSystemConfigManager();
    ~MJLevelSystemConfigManager();

public:
    bool LoadConfig(unsigned long in_ulPID);  // 初始化任务列表

    void LoadConfigPIDList(rapidxml::xml_node<>* pElemConfig, unsigned long in_ulPID);

    void LoadConfigLevelList(rapidxml::xml_node<>* pElemConfig);

    void LoadConfigSpecialList(rapidxml::xml_node<>* pElemConfig);

protected:
    EMJLevel ConvertLevelName2Enum(const char* in_attrLevel);

public:
    int m_nEXPBase{};
    std::vector<MJLevelInfoNode> m_vecsLevelInfo;
    std::vector<MJLevelConfigSpecial> m_vecLevelConfigSpecial;
};

//////////////////////////////////////////////////////////////////////////
//MJLevelSystem used by MJGrowSystem.
class MJLevelSystem
{
public:
    MJLevelSystem(CTKGame* in_pGame);
    ~MJLevelSystem(void);

public:
    void Init(unsigned long in_ulPID);
    void DumpDebug();

public:
    void PostMsg2MS_SetEXP(MJGamePlayer* in_pPlayer, bool in_bWin);

public:
    // 计算游戏结束后的经验值（根据当前经验值）
    int CalculateEXPUpdate(int in_nEXPCur, bool in_bWin);

protected:
    EMJLevel GetLevel(int in_nEXP);            // 获取荣誉值对应的等级
    int GetEXPMin(EMJLevel in_eLevel);         // 获取等级对应的最小荣誉值
    int GetAwardAddition(EMJLevel in_eLevel);  // 获取等级对应的额外奖励

    // 计算额外经验值（晋级时）
    int CalculateEXPAddition(int in_nEXPCur, int in_nEXPUpdate);

protected:
    CTKGame* m_pGame{};

    MJLevelSystemConfigManager* m_pLevelSystemConfigManager{};
};

/// <summary>
/// MJGrowSystem 使用异步调用，建议在Game中使用MJGrowSystem的成员，以保证其生命周期
/// </summary>
class MJGrowSystem
{
public:
    /// <param name="pGame">当前Game对象，不可为空，应能获取到player对象</param>
    MJGrowSystem(CTKGame* pGame);

public:
    // 载入growID
    void LoadGrowValue(ISeatID seat, DWORD growID);  //在回调中取值

    // 保存seat的growID对应值，若growID的值未载入，则返回false
    void SaveGrowValue(ISeatID seat, DWORD growID, DWORD value);
    void SaveGrowValues(ISeatID seat, std::map<DWORD, DWORD> values);

    void LoadExp(ISeatID seat);
    void UpdateExp(ISeatID seat, bool isWinner);

protected:
    CTKGame* m_pGame{};

    GrowValueCallback m_Callback{};

    MJLevelSystem m_LevelSystem;

public:
    //获取成长值
    void PostMsg2MS_GetValue(ISeatID seat, DWORD growID);
    void PostMsg2MS_SetValue(ISeatID seat, DWORD growID, DWORD value);
};
