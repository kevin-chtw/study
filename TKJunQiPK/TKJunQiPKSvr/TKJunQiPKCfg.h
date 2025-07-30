#pragma once
#include "TKDllGame.h"
#include "TKDllPlayer.h"
#include "TKJunQiPKDefine.h"

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
    ECI_HonorHalf = 18,            //大于军功下限，且低于此值时，扣分减半
    ECI_WinHonorPlus = 19,         //非岛屿赛的军功胜利时增加值
    ECI_LossHonorMinus = 20,       //非岛屿赛的军功失败时增加值 //21 - 表情开关
    ECI_FeelColdTime = 22,         //表情间隔时长
    ECI_CalcType = 23,             //计算军功方式（0-按公式计算，1-按配置计算）
    ECI_FriendTable = 24,          //是否为好友桌（0-否，1-是）
    ECI_FriendTime = 25,           //好友桌设置规则，布局时间
    ECI_WinExp = 26,               //胜利时添加经验值
    ECI_DrawExp = 27,              //平局时添加经验值
    ECI_bottomTime = 28,           //保底剩余操作时间
    ECI_DelayReduceTime = 29,      // 延迟后的扣除的操作时长
    ECI_ChanseCount = 30,          //连续追子次数限制
    ECI_MatchType = 31,            //比赛类型（1-排位赛，2-比赛场，3-活动比赛）
    ECT_PerMatchCount = 32,        //每日比赛局数（0-未配置，n-积分ID）
    ECT_PerMatchTime = 33,         //每日比赛时长（0-未配置，n-积分ID）
    ECI_LoseExp = 34,              //失败时添加经验值
    ECI_NegativeNoFightStep = 35,  //消极比赛未吃子行为步数
    ECI_NegativeMatchTime = 36,    //消极比赛对局时长
    ECI_End,                       //配置结束标识
};
const int g_nDefaultValue[ECI_End] = {1, 5,  15, 3,  50, 280, 3, 2,  30, 3, 9, 20, 40, 16, 0, 0, 0,  0, 120,
                                      1, -1, 1,  10, 0,  0,   0, 25, 10, 5, 3, 4,  1,  0,  0, 5, 50, 90};

class CTKJunQiPKCfg
{
public:
    CTKJunQiPKCfg();
    ~CTKJunQiPKCfg();

public:
    void InitCfg(const string& szCfg);
    int GetCfg(ECfgIndex nCfgId);
    void SetCfg(ECfgIndex nCfgId, int nCfgValue);

private:
    int m_nCfgValue[ECI_End];
};
