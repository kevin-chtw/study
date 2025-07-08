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
ʹ��MJGrowSystem
    void LoadGrowValue(ISeatID seat, DWORD growID); 
    void SaveGrowValue(ISeatID seat, DWORD growID, DWORD value);
    void LoadExp(ISeatID seat);
    void UpdateExp(ISeatID seat, bool isWinner);
*/

#define PIVOT_PORT 30070
#define EXP_DOMAIN_ID 60013         // �齫�ȼ�ϵͳ��������_��ID
#define EXP_GROW_ID_VALUE 60013038  // �齫�ȼ�ϵͳ��������_�û���������
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
    int nAwardAddition{};  // ���⽱��
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
    bool LoadConfig(unsigned long in_ulPID);  // ��ʼ�������б�

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
    // ������Ϸ������ľ���ֵ�����ݵ�ǰ����ֵ��
    int CalculateEXPUpdate(int in_nEXPCur, bool in_bWin);

protected:
    EMJLevel GetLevel(int in_nEXP);            // ��ȡ����ֵ��Ӧ�ĵȼ�
    int GetEXPMin(EMJLevel in_eLevel);         // ��ȡ�ȼ���Ӧ����С����ֵ
    int GetAwardAddition(EMJLevel in_eLevel);  // ��ȡ�ȼ���Ӧ�Ķ��⽱��

    // ������⾭��ֵ������ʱ��
    int CalculateEXPAddition(int in_nEXPCur, int in_nEXPUpdate);

protected:
    CTKGame* m_pGame{};

    MJLevelSystemConfigManager* m_pLevelSystemConfigManager{};
};

/// <summary>
/// MJGrowSystem ʹ���첽���ã�������Game��ʹ��MJGrowSystem�ĳ�Ա���Ա�֤����������
/// </summary>
class MJGrowSystem
{
public:
    /// <param name="pGame">��ǰGame���󣬲���Ϊ�գ�Ӧ�ܻ�ȡ��player����</param>
    MJGrowSystem(CTKGame* pGame);

public:
    // ����growID
    void LoadGrowValue(ISeatID seat, DWORD growID);  //�ڻص���ȡֵ

    // ����seat��growID��Ӧֵ����growID��ֵδ���룬�򷵻�false
    void SaveGrowValue(ISeatID seat, DWORD growID, DWORD value);
    void SaveGrowValues(ISeatID seat, std::map<DWORD, DWORD> values);

    void LoadExp(ISeatID seat);
    void UpdateExp(ISeatID seat, bool isWinner);

protected:
    CTKGame* m_pGame{};

    GrowValueCallback m_Callback{};

    MJLevelSystem m_LevelSystem;

public:
    //��ȡ�ɳ�ֵ
    void PostMsg2MS_GetValue(ISeatID seat, DWORD growID);
    void PostMsg2MS_SetValue(ISeatID seat, DWORD growID, DWORD value);
};
