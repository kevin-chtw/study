#pragma once
#include <map>
#include <vector>
#include <deque>
#include <functional>
#include <memory>
#include <chrono>
#include <algorithm>

#include "MJDefine.h"
#include "MJTile.h"
#include "MJConfig.h"
#include "MJLog.h"
#include "MJPlay.h"
#include "MJUtils.h"
#define USE_OLD_DRAW 1
#define MAX_PLAYER_COUNT 4
class LYGCGame;
class MJPlayEx;
class State;
class StateInit;
class LYGCMessager;

static constexpr int EOperateCI = 1 << 11;  //���е�CI

namespace MJTips
{
enum
{
    PassHuLong = 12,  // ���ù���
    Ting = 13,        // �Ѿ�������״̬
    BaoCiState = 14,  // ���ν׶�
};
};  // namespace MJTips

namespace MJScoreType
{
enum
{
    Gang = 3,
    GenZhuang = 4,
    Mi = 5
};
};  // namespace MJScoreType
enum class ECallType
{
    None,
    Call,
    FirstCall,
};

using score_t = int32_t;

enum RuleItem
{
    ECI_MannualCardsIndex,  //�����ļ�
    ECI_DiscardTime,        //����ʱ��
    ECI_WaitTime,           //���ܺ��ȴ�ʱ��

    ECI_ScoreType,          //��ַ�ʽ��0-������֣�1-������С����2-��Ҳ����֣�
    ECI_Expression,         //���飨0��1��
    ECI_ShortCode,          //���0��1��
    ECI_RuleFeng,           //������ ���ƹ���  0 ���粻����� 1 �������  2 ������
    ECI_CiPai,              //���� 0���  1Ӳ��
    ECI_GenZhuang,          //��ׯ 1�� 0��
    ECI_CiHuBei,            //�κ�����
    ECI_PiCiBei,            //Ƥ�α���
    ECI_XiaMi,              //���� 1�� 0��
    ECI_XiaMiBei,           //���ױ���
    ECI_DrawCtrl,           //�����Ż����ʿ���(0-100 0-�أ�
    ECI_DealerCtrl,         //���ƿ���(0-��,1-����
    ECI_DispatchXiangTing,  //�Ż���������
    ECI_MoBomb,             //��ը����������Ӳ���淨����Ч ��ʾ������ߵ�X���ƺ������ը���׶� 0��ʾ�޸ý׶Σ�
    ECI_DealerAnKe,         //�����Ż� ���������͸���(%)
    ECI_DealerAnGang,       //�����Ż� ���������͸���(%)
    ECI_DealerDoubleAnKe,   //�����Ż� ��˫�������͸���(%)
    ECI_DealerDuiZi,        //�����Ż� ���������͸���(%)
    ECI_End,
};

constexpr int DEFAULT_CONFIG_VALUES[] = {0, 10, 8, 1, 1, 1, 0, 0, 0, 3, 3, 0, 3, 0, 0, 3, 0, 22, 10, 8, 10};
static_assert(std::size(DEFAULT_CONFIG_VALUES) == ECI_End, "config size error");

const std::vector<int> g_DefaultConfig(std::begin(DEFAULT_CONFIG_VALUES), std::end(DEFAULT_CONFIG_VALUES));

const MJFDConfigMap g_FDTableConfigMap = {
    {"DuanYu", ECI_ShortCode},
    {"BiaoQing", ECI_Expression},
    {"RuleFeng", ECI_RuleFeng},
    {"CiPai", ECI_CiPai},
    {"CiHuBei", ECI_CiHuBei},
    {"PiCiBei", ECI_PiCiBei},
    {"XiaMi", ECI_XiaMi},
    {"XiaMiBei", ECI_XiaMiBei},
    {"GenZhuang", ECI_GenZhuang},
    {"YingCiZhaDan", ECI_MoBomb},
};

struct LYGCLastHandData
{
    int banker{SEAT_NULL};
    int firstBanker{SEAT_NULL};
    int lianZhuang{0};  // ��ׯ����
    int lastXiaMiSeat{SEAT_NULL};
    int lastHuType{-1};
    int miChi{0};
    int miCount[NP4]{};
};
