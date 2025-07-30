#pragma once

#define PLAYER_COUNT 2       //玩家数
#define CROSS_COUNT 60       //交叉点数
#define SIDE_CROSS_COUNT 30  //每方交叉点数
#define SIDE_CHESS_COUNT 25  //每方棋子数

#define JUNQIPK_UNKNOWN -1  //未知

enum ECfgIndex
{
    ECI_GameType = 0,        //游戏类型
    ECI_InitTime = 1,        //初始化时间
    ECI_MoveTime = 2,        //移动棋子时间
    ECI_MoveActionTime = 3,  //移动棋子的动画时间
    ECI_NoFightStep = 4,     //连续50步没有吃子行为。强制和局。
    ECI_TotalStep = 5,  //总步数超过280（玩家因超时没有操作的回合不会增加步数）。强制和局。
    ECI_RepeatStep = 6,    //八步内重复达4次。强制判负。
    ECI_PassCount = 7,     //超时三次。强制判负。
    ECI_PeaceStep = 8,     //未满20步，不能求和。
    ECI_PeaceCount = 9,    //求和次数已满三次，不能求和
    ECI_PeaceTime = 10,    //求和超时时间
    ECI_ConcedeStep = 11,  //未满10步，不能投降
    ECI_HonorLower = 12,   //军功下限，失败时低于此值时不扣分
    ECI_KFactor = 13,      //结算K值
    ECI_CleanDiLei = 14,   //排雷限制（0-工兵排雷,1-最小子排雷）
    ECI_CarryJunQi = 15,   //扛旗限制（0-任意子扛棋,1-最小子扛棋）
    ECI_FirstWay = 16,     //先手方式（0-随机先手，1-军功较低方先手）
    ECI_ConfirmColor = 17,  //确定颜色（0-连续两次相同颜色确定阵营，1-先手方翻到的为对手的棋子）
    ECI_HonorHalf = 18,       //大于军功下限，且低于此值时，扣分减半
    ECI_WinHonorPlus = 19,    //非岛屿赛的军功胜利时增加值
    ECI_LossHonorMinus = 20,  //非岛屿赛的军功失败时增加值
    // 21 - 表情开关，22 - 表情间隔时长
    ECI_CalcType = 23,       //计算军功方式（0-按公式计算，1-按配置计算）
    ECI_FriendTable = 24,    //是否为好友桌（0-否，1-是）
    ECI_FriendTime = 25,     //好友桌设置规则，布局时间
    ECI_WinExp = 26,         //胜利时添加经验值
    ECI_DrawExp = 27,        //平局时添加经验值
    ECI_DelayCount = 28,     //连续延迟次数
    ECI_DelayMoveTime = 29,  //延迟后的行动时长
    ECI_ChanseCount = 30,    //连续追子次数限制
};
//交叉点类型
enum ECrossType
{
    ECT_BZ,   //兵站
    ECT_XY,   //行营
    ECT_DBY,  //大本营
};

enum EChessColor
{
    EC_Blue = 0,  //蓝色
    EC_Red = 1    //红色
};

//棋子类型
enum EChessType
{
    ECT_Unknow = -1,  //未知
    ECT_ZhaDan = 0,   //炸弹
    ECT_DiLei,        //地雷
    ECT_JunQi,        //军旗
    ECT_GongBing,     //工兵
    ECT_PaiZhang,     //排长
    ECT_LianZhang,    //连长
    ECT_YingZhang,    //营长
    ECT_TuanZhang,    //团长
    ECT_LvZhang,      //旅长
    ECT_ShiZhang,     //师长
    ECT_JunZhang,     //军长
    ECT_SiLing,       //司令

    ECT_ChessTypeCount,  //种类数
};

enum EFightResult
{
    EFR_PerishTogether,  //同归于尽
    EFR_Win,             //战胜
    EFR_Loss,            //战败
};

//战斗事件
enum EFightEvent
{
    EFE_SiLingDead,  //有司令阵亡
    EFE_JunQiDead,   //有军旗被吃
    EFE_Timeout,     //有军旗被吃
};

enum GAMESCHEDULE
{
    TKJUNQI_SCHEDULE_INITLAYOUT,
    TKJUNQI_SCHEDULE_SELECTCHESS,
    TKJUNQI_SCHEDULE_MOVECHESS,
    TKJUNQI_SCHEDULE_ACTIONFINISH,
    TKJUNQI_SCHEDULE_CONCEDE,
    TKJUNQI_SCHEDULE_PEACE,
    TKJUNQI_SCHEDULE_PEACERES,
    TKJUNQI_SCHEDULE_SHOWCHESS,
    TKJUNQI_SCHEDULE_INITRULE,
    TKJUNQI_SCHEDULE_CANCELLAYOUT,
};

enum EGameType
{
    GT_None = 0,
    GT_FANFANQI = 1,    // 翻翻棋
    GT_ERRENANQI = 2,   // 二人暗棋
    GT_SIGUOSIAN = 3,   // 四国四棋
    GT_SHUANGMING = 4,  // 四国双明
};

enum ENTRY_TYPE
{
    TKJUNQI_EXACT,
    TKJUNQI_LOWER,
    TKJUNQI_UPPER,
};

#define GongBingCleanDiLei 0  //工兵排雷
#define AnyOneCarryJunQi 0    //任意子扛棋
#define RandomFirst 0         //随机先手
#define TwoConfirmColor 0     //连续两次相同颜色确定阵营

#define FANFANQI 1

#define SETRULETIME 6
#define INITWAITTIME 7
#define MOVECHESSWAITTIME 8
#define MOVEACTIONWAITTIME 9
#define PEACEWAITTIME 10

const int SWAP_CHESS_NUM = 2;
const double INIT_ALPHA_VALUE = -200000;
const double INIT_BETA_VALUE = 200000;
const double SEARCH_MAX_VALUE = 100000;
const double SEARCH_MIN_VALUE = -15000;
const double DISTANCE_WEIGHT = 0.00000001;
const double CAMP_WEIGHT = 0.01;
const double MIN_DOUBLE_VALUE = 0.000000001;
const double NO_FIND_IN_TTABLE = 666666;