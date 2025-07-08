#pragma once
#include "MJDefine.h"
#include "MJString.h"
#include "MJHelper.h"
#include "MJTingNormal.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace MJ
{
int CalcStepTo13Yao(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards);
int CalcStepQiDui(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards);
std::map<ITileID, int> TileListToMap(std::vector<ITileID> const& tiles, std::vector<ITileID> laizi, int& laiziCount, int& otherCount);
}  // namespace MJ

class MJTingBase
{
public:
    enum class Type
    {
        Normal,
        QiDui,
        N13Yao,
    };
    /// <summary>
    /// 查向胡数
    /// </summary>
    /// <param name="cards">手牌</param>
    /// <param name="laiziCards">指定哪些是赖子</param>
    /// <param name="extraTypes">指定查七对、十三幺等类型（不指定默认平胡型Normal）</param>
    /// <param name="type">返回类型</param>
    /// <returns>向胡数</returns>
    virtual int CalcStep(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes = {}, int* type = nullptr);

    ///查向听数
    /// <param name="cards">手牌</param>
    /// <param name="laiziCards">指定哪些是赖子</param>
    /// <param name="extraTypes">指定查七对、十三幺等类型（不指定默认平胡型Normal）</param>
    /// <param name="type">返回类型</param>
    /// <returns>向听数 = 向胡数 - 1</returns>
    int CalcTing(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes = {}, int* type = nullptr);

protected:
    MJ::TingNormal m_NormalTool;
};

class MJTingEx11 : public MJTingBase
{
public:
    MJTingEx11();
    static std::shared_ptr<MJTingEx11> GetInstance();
};

class MJTingEx14 : public MJTingBase
{
public:
    static std::shared_ptr<MJTingEx14> GetInstance();
    MJTingEx14();
    int CalcStep(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes = {}, int* type = nullptr) override;
};

class MJTingEx17 : public MJTingBase
{
public:
    MJTingEx17();
    static std::shared_ptr<MJTingEx17> GetInstance();
};
