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

static constexpr int EOperateCI = 1 << 11;  //特有的CI

namespace MJTips
{
enum
{
    PassHuLong = 12,  // 永久过胡
    Ting = 13,        // 已经处于听状态
    BaoCiState = 14,  // 包次阶段
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
    ECI_MannualCardsIndex,  //配牌文件
    ECI_DiscardTime,        //出牌时间
    ECI_WaitTime,           //碰杠胡等待时间

    ECI_ScoreType,          //算分方式（0-正常算分，1-积分最小化，2-输家不负分）
    ECI_Expression,         //表情（0、1）
    ECI_ShortCode,          //短语（0、1）
    ECI_RuleFeng,           //不带风 风牌规则  0 带风不带风次 1 带风带次  2 不带风
    ECI_CiPai,              //次牌 0软次  1硬次
    ECI_GenZhuang,          //跟庄 1是 0否
    ECI_CiHuBei,            //次胡倍数
    ECI_PiCiBei,            //皮次倍数
    ECI_XiaMi,              //下米 1是 0否
    ECI_XiaMiBei,           //下米倍数
    ECI_DrawCtrl,           //摸牌优化概率控制(0-100 0-关）
    ECI_DealerCtrl,         //发牌控制(0-关,1-开）
    ECI_DispatchXiangTing,  //优化发牌相听
    ECI_MoBomb,             //摸炸弹张数（仅硬次玩法下生效 表示玩家摸走第X张牌后进入摸炸弹阶段 0表示无该阶段）
    ECI_DealerAnKe,         //发牌优化 发暗刻牌型概率(%)
    ECI_DealerAnGang,       //发牌优化 发暗杠牌型概率(%)
    ECI_DealerDoubleAnKe,   //发牌优化 发双暗刻牌型概率(%)
    ECI_DealerDuiZi,        //发牌优化 发对子牌型概率(%)
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
    int lianZhuang{0};  // 连庄次数
    int lastXiaMiSeat{SEAT_NULL};
    int lastHuType{-1};
    int miChi{0};
    int miCount[NP4]{};
};
