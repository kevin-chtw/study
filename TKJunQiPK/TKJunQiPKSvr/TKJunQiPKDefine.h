#pragma once
#include <Shlwapi.h>
#include <TKDllServer.h>
#include <limits.h>
#include <map>
#include <vector>

#define SIDE_CHESS_COUNT 25  // 每方棋子数
#define SIDE_CROSS_COUNT 30  // 每方交叉点数
#define QUICK2_POS_START 5   // 极速玩法2开始布子位置
#define JUNQIPK_UNKNOWN -1   // 未知

#define MAX_TRACESTRINGSIZE (1024 * 100)  // 每次写日志得长度
#define MAX_TRACESTRINGTIME (5 * 60)  // 上次写入的时间与下次日志存储距离间隔大于该时间就写入文件（单位：second）

#define LOG_ERROR(fmt, ...) \
    TKWriteLog("%s(%d)<%s>[ERROR]:" fmt, PathFindFileName(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__)

// 交叉点类型
enum ECrossType
{
    ECT_BZ,   // 兵站
    ECT_XY,   // 行营
    ECT_DBY,  // 大本营
};

enum EChessColor
{
    EC_Blue,  // 蓝色
    EC_Red,   // 红色
    EC_All,   // 所有颜色
};

// 棋子类型
enum EChessType
{
    ECT_ZhaDan,     // 炸弹
    ECT_DiLei,      // 地雷
    ECT_JunQi,      // 军旗
    ECT_GongBing,   // 工兵
    ECT_PaiZhang,   // 排长
    ECT_LianZhang,  // 连长
    ECT_YingZhang,  // 营长
    ECT_TuanZhang,  // 团长
    ECT_LvZhang,    // 旅长
    ECT_ShiZhang,   // 师长
    ECT_JunZhang,   // 军长
    ECT_SiLing,     // 司令

    ECT_ChessTypeCount,  // 种类数
};

enum EFightResult
{
    EFR_PerishTogether,  // 同归于尽
    EFR_Win,             // 战胜
    EFR_Loss,            // 战败
};

// 战斗事件
enum EFightEvent
{
    EFE_SiLingDead,  // 有司令阵亡
    EFE_JunQiDead,   // 有军旗被吃
    EFE_Timeout,     // 有军旗被吃
};

// 服务器状态
enum EStage
{
    ES_Distributing,           // 正在布局
    ES_WaitingMoveChess,       // 正在等待走棋
    ES_WaitingActionComplete,  // 正在等待走棋完毕
    ES_WaitingPeace,           // 正在求和阶段，游戏暂停
    ES_SetRule,                // 设置规则
    ES_Finished,               // 游戏已结束
};

enum EGrowId
{
    EG_DayWinCount = 50540006,        // 每日胜利次数
    EG_DayPlayCount = 50540007,       // 每日游戏局数
    EG_DayDesDiLeiCount = 50540010,   // 每日消灭地雷个数
    EG_DayDesSiLingCount = 50540011,  // 每日消灭司令个数
    EG_DayDesChessCount = 50540012,   // 每日消灭棋子个数
    EG_HundredStepWin = 50540013,     // 在100步内取胜局数
    EG_PlayFFQRankGame = 50540015,    // 玩翻翻棋排位赛       --废弃
    EG_PlayAQRankGame = 50540016,     // 玩暗棋排位赛         --废弃
    EG_PlayPKGame = 50540017,         // 参加一场PK赛
    EG_PlayPassGame = 50540018,       // 参加一场闯关赛

    EG_FFQRankCount = 50550006,  // 翻翻棋定级赛局数
    EG_AQRankCount = 50550007,   // 暗棋定级赛局数
    EG_SARankCount = 50550008,   // 四暗定级赛局数
    EG_SMRankCount = 50550009,   // 双明定级赛局数

    EG_Experience = 50560001,  // 军棋PK总经验积分

    EG_JunQiJunGong = 50560005,        // 军棋军功              - 废弃
    EG_AQZongJuShu = 50560006,         // 暗棋总局数
    EG_AQShengJuShu = 50560007,        // 暗棋胜局数
    EG_AQSaiJiShengJuShu = 50553028,   // 暗棋赛季胜局数  用于记录玩家赛季暗棋胜场数
    EG_AQFuJuShu = 50560008,           // 暗棋负局数
    EG_AQHeJuShu = 50560009,           // 暗棋和局数
    EG_AQDuanXianJuShu = 50560010,     // 暗棋断线局数
    EG_FFQZongJuShu = 50560011,        // 翻翻棋总局数
    EG_FFQShengJuShu = 50560012,       // 翻翻棋胜局数
    EG_FFQSaiJiShengJuShu = 50553027,  // 翻翻棋赛季胜场数  用于记录玩家翻翻棋单个赛季的胜场数
    EG_FFQFuJuShu = 50560013,          // 翻翻棋负局数
    EG_FFQHeJuShu = 50560014,          // 翻翻棋和局数
    EG_FFQDuanXianJuShu = 50560015,    // 翻翻棋断线局数
    EG_Season = 50560016,              // 赛季积分              - 废弃
    EG_ThroughPassGame = 50560027,     // 通关闯关赛             - 废弃
    EG_FFQSeasonRank = 50560033,       // 翻翻棋排位积分
    EG_AQSeasonRank = 50560034,        // 暗棋排位积分

    EG_DoubleCard = 50560048,             // 翻倍卡
    EG_KeepScoreCard = 50560049,          // 保分卡
    EG_DayDoubleCardUseCount = 50553039,  // 翻倍卡每日使用次数

    EG_SASeasonRank = 50580105,       // 四暗排位积分
    EG_SMSeasonRank = 50580106,       // 双明排位积分
    EG_SAZongJuShu = 50580107,        // 四暗总局数
    EG_SAShengJuShu = 50580108,       // 四暗胜局数
    EG_SASaiJiShengJuShu = 50553029,  // 四国四暗赛季胜场数  用于记录玩家四国四暗赛季胜场数
    EG_SAHeJuShu = 50580109,          // 四暗平局数
    EG_SAFuJuShu = 50580110,          // 四暗负局数
    EG_SADuanXianJuShu = 50580111,    // 四暗断线局数
    EG_SMZongJuShu = 50580112,        // 双明总局数
    EG_SMShengJuShu = 50580113,       // 双明胜局数
    EG_SMSaiJiShengJuShu = 50553030,  // 四国双明赛季胜场数  用于记录玩家四国双明赛季胜场数
    EG_SMHeJuShu = 50580114,          // 双明平局数
    EG_SMFuJuShu = 50580115,          // 双明负局数
    EG_SMDuanXianJuShu = 50580116,    // 双明断线局数
    EG_S_Experience = 50580117,       // 军棋PK四国四暗双明经验值(会关联修改50560001)
};

enum
{
    EVT_GameResult = 10000236,  // 军棋对局结果(0:平局  1:胜利 2:失败)
};

enum ETipsType
{
    ETS_TimeoutLoss = 1,       // 超时判负
    ETS_RepeatMoveLoss = 2,    // 重复走子判负
    ETS_UnFightDraw = 3,       // 不吃子和棋
    ETS_TotalStepDraw = 4,     // 总步数和棋
    ETS_UnFightTips = 5,       // 不吃子提示
    ETS_TotalStepTips = 6,     // 总步数提示
    ETS_UnMoveAbleLoss = 7,    // 不能走子判负
    ETS_ConcedeLoss = 8,       // 投降判负
    ETS_CarryJunQi = 9,        // 军棋被扛
    ETS_ChessPowerOver = 10,   // 棋力降到下限
    ETS_DelayTimePulish = 11,  // 延时惩罚
};

enum EGameType
{
    GT_FANFANQI = 1,    // 翻翻棋
    GT_ERRENANQI = 2,   // 二人暗棋
    GT_SIGUOSIAN = 3,   // 四国四棋
    GT_SHUANGMING = 4,  // 四国双明
};

enum EEndType
{
    EET_Normal = 0,   // 正常结束
    EET_ErrInit = 1,  // 初始化异常
    EET_Concede = 2,  // 投降
    EET_Peace = 3,    // 求和
    EET_UnFight = 4,  // 未交战
    EET_MaxStep = 5,  // 总步数限制
};

// 消极比赛类型
enum ENegativeType
{
    ENT_NOT_NEGATIVE = 0,     // 没有消极比赛
    ENT_TIMEOUT_AFK = 1,      // 超时挂机
    ENT_INITIATIVE_LOSE = 2,  // 主动认输
};

enum ENegativeAtomEvent
{
    ENA_EVENT = 10007301,  // 军棋-消极比赛次数
};

// Debug日志开关 1打印日志
#define OPEN_DEBUG_LOG 0

#define GongBingCleanDiLei 0  // 工兵排雷
#define AnyOneCarryJunQi 0    // 任意子扛棋
#define RandomFirst 0         // 随机先手
#define TwoConfirmColor 0     // 连续两次相同颜色确定阵营

#define SETRULETIME 6
#define INITWAITTIME 7
#define MOVECHESSWAITTIME 8
#define MOVEACTIONWAITTIME 9
#define PEACEWAITTIME 10
const int Max_Round_Timmer = 11;
const int DELAY_TIPS_TIME = 5;

const int PK_Player_Count = 2;
const int Int_True = 1;
const int Max_Round_Time = 3600;

const int SWAP_CHESS_NUM = 2;    // 相差多少个棋子交换
const int SWAP_SHOW_COUNT = 20;  // 翻开多少时交换
const int UNSWAP_PERCENT1 = 10;  // 不替换的概率
const int UNSWAP_PERCENT2 = 40;  // 不替换的概率
const int ONE_HUNDRED = 100;

const int RANK_MATCH = 1;
const int PK_MATCH = 2;

const int CHASE_REPORT_COUNT = 2;
const int DELAY_TIME = 5;

const int g_nLossScore = 1500;  // 棋力下限，极速翻翻棋
const int g_GradingCount = 1;   // 定积赛局数
const int g_GradingWin = 50;    // 定积赛胜利得分
const int g_GradingDraw = 20;   // 定积赛平局得分
const int g_GradingLoss = 10;   // 定积赛失败得分

const int g_RankScoreCfgCol = 7;  // 排位赛得分配置行数
const int g_RankScoreCfgRow = 4;  // 排位赛得分配置列数
const int g_RankScoreArray[][g_RankScoreCfgRow] = {
    {0, 30, 0, 7},      {501, 30, -5, 5},   {1001, 30, -10, 3}, {2001, 30, -15, 2},
    {3001, 30, -20, 1}, {4001, 30, -25, 0}, {6001, 30, -30, 0},
};  // 翻翻棋和暗棋，排位对局得分(1排位积分,2胜利得分,3失败得分,4平局得分)
const int g_SiGuoRankScoreArray[][g_RankScoreCfgRow] = {
    {0, 50, 0, 10},     {501, 50, -10, 7},  {1001, 50, -20, 5}, {2001, 50, -30, 3},
    {3001, 50, -40, 2}, {4001, 50, -50, 1}, {6001, 50, -50, 0},
};  // 四国四暗和四国双明，排位对局得分(1排位积分,2胜利得分,3失败得分,4平局得分)

const int g_RankDiffRow = 2;
const int g_UnMinusValue[] = {0, 50, 150, 300, 500, 800, 1200, 1600, 2000, 2500, 3000, 3500, 4000};  // 不扣分分数
const int g_RankDiff[] = {200,  400,  600,  800,  1000, 1200, 1500,  2000,
                          2500, 3000, 4000, 5000, 6000, 8000, 10000, INT_MAX};  // 排位赛分差，下标用于分数变化值
const int g_SiRankDiff[][g_RankDiffRow] = {{200, 0},   {400, 1},   {600, 2},   {800, 3},   {1000, 4},   {1200, 5},
                                           {1400, 6},  {1600, 7},  {2000, 8},  {2500, 9},  {3000, 10},  {3500, 12},
                                           {4000, 14}, {5000, 16}, {6000, 18}, {8000, 20}, {10000, 22}, {INT_MAX, 25}};

const int g_nChessCount[ECT_ChessTypeCount] = {2, 3, 1, 3, 3, 3, 2, 2, 2, 2, 1, 1};   // 棋子个数
const int g_nQuickChess[ECT_ChessTypeCount] = {5, 0, 1, 3, 3, 3, 2, 2, 2, 2, 1, 1};   // 极速翻翻棋棋子
const int g_nQuick2Chess[ECT_ChessTypeCount] = {2, 2, 1, 2, 0, 3, 2, 2, 2, 2, 1, 1};  // 极速翻翻棋棋子
const int g_nChessScore[ECT_ChessTypeCount] = {0,  0,   1000, 10,  20,  50,
                                               80, 100, 200,  300, 400, 500};  // 极速翻翻棋棋子分数
const int g_nChessPos[SIDE_CHESS_COUNT] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 12, 14,
                                           15, 16, 18, 19, 20, 22, 24, 25, 26, 27, 28, 29};  // 棋子的布局位置
const int g_nJunqiUnpos[] = {5,  10, 15, 20, 9,  14, 19, 24, 25, 29, 35, 40, 45,
                             50, 55, 39, 44, 49, 54, 59, 26, 27, 28, 56, 57, 58};
const int g_nDileiUnpos[] = {25, 27, 29, 59, 57, 55};
const int g_nDileiUnPosLen = 2;
const int g_nSilingUnpos[] = {0, 1, 2, 3, 4, 30, 31, 32, 33, 34};
const int g_nKeyPos[] = {7, 12, 15, 16, 18, 19, 22, 27, 37, 42, 45, 46, 48, 49, 52, 57};
const int g_nKeyPosChessNum = 8;  // 关键位置棋子数
const int g_nSpecialPos[] = {0, 1, 2, 3, 4, 30, 31, 32, 33, 34};
const std::map<int, std::vector<int>> g_specialCtrlPos = {
    {6, {0, 1, 2, 5, 7, 10, 12}},       {7, {1, 2, 3, 6, 8, 12}},       {8, {2, 3, 4, 7, 9, 12, 14}},
    {36, {30, 31, 32, 35, 37, 40, 42}}, {37, {31, 32, 33, 36, 38, 42}}, {38, {32, 33, 34, 37, 39, 42, 44}}};