#pragma once
#include <string>
#include <map>

// for MJOperates
using EOperate = int;
static constexpr int EOperateNone = -1;  // only for error.
static constexpr int EOperatePass = 0;

static constexpr int EOperateChow = 1;           //吃
static constexpr int EOperatePon = 1 << 1;       //碰
static constexpr int EOperateKon = 1 << 2;       //杠
static constexpr int EOperateTing = 1 << 3;      //听
static constexpr int EOperateHu = 1 << 4;        //胡
static constexpr int EOperateDiscard = 1 << 5;   //出牌
static constexpr int EOperateExchange = 1 << 6;  //换三张
static constexpr int EOperateDraw = 1 << 7;      //摸牌
static constexpr int EOperateTianTing = 1 << 8;  //天听
static constexpr int EOperateFlower = 1 << 9;    //换花
static constexpr int EOperatePass10 = 1 << 10;   //使用有效值表示Pass，EOperatePass弃用
static constexpr int EOperateInvalid = 0;

static const std::map<EOperate, std::string> g_MJOperateNames = {
    {EOperatePass, "Pass"},
    {EOperateChow, "Chow"},
    {EOperatePon, "Pon"},
    {EOperateKon, "Kon"},
    {EOperateTing, "Ting"},
    {EOperateHu, "Win"},
    {EOperateDiscard, "Discard"},
    {EOperateExchange, "Exchange"},
    {EOperateDraw, "Draw"},
    {EOperateTianTing, "TianTing"},
    {EOperateFlower, "Flower"},
    {EOperatePass10, "Pass"},
};

static const std::map<std::string, EOperate> g_MJOperateIDs = {
    {"Pass", EOperatePass},
    {"Chow", EOperateChow},
    {"Pon", EOperatePon},
    {"Kon", EOperateKon},
    {"Ting", EOperateTing},
    {"Win", EOperateHu},
    {"Discard", EOperateDiscard},
    {"Exchange", EOperateExchange},
    {"Draw", EOperateDraw},
    {"TianTing", EOperateTianTing},
    {"Flower", EOperateFlower},
    {"Pass", EOperatePass10},
};

struct MJOperates
{
    int value = 0;
    bool isMustHu = false;
    bool capped{};

    void AddOperate(int op)
    {
        value |= op;
    }
    void AddOperates(MJOperates const& ops)
    {
        value |= ops.value;
    }
    void RemoveOperate(int op)
    {
        value &= ~op;
    }
    bool HasOperate(int op) const
    {
        return (value & op);
    }
    void Reset()
    {
        value = 0;
    }
};

std::string GetOperateName(EOperate operate, std::map<EOperate, std::string> const& names = g_MJOperateNames);
EOperate GetOperateID(std::string const& name, std::map<std::string, EOperate> const& ids = g_MJOperateIDs);
