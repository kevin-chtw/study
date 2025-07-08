/*
	Copyright (c) 2001, 竞技世界（成都）-九天项目群-地方麻将项目部

	Version: 0.1
	Author: Guotj7400
	Date: 2022-04-19

	摘要:
	麻将中玩家的输分最多为其带入, 当其游戏输分过大时, 需要对输分进行最小化处理.
	同理, 因其输分受限, 为保持公平, 其最大赢分也要进行限制.
	本模块以四个玩家(4人麻将)的 带入分 和 输赢分 做为输入, 计算出最小化处理后的结果.

	相关函数:
	TryReduceScore - 已移除,使用TryReduceScore替代
	TryReduceScore
*/

#pragma once

#include <vector>
#include <array>
#include "MJHelper.h"

namespace MJ
{
/// 返回容器中最小值
/// 空容器返回value_type默认构造值
template <typename Type>
Type MinOf(std::initializer_list<Type> const& values)
{
    if (values.size() == 0)
        return Type{};
    auto iter = std::min_element(values.begin(), values.end());
    return *iter;
}

//  四舍五入double数组到整型
template <typename Int, size_t Count>
std::array<Int, Count> RoundTo(std::array<double, Count> const& values)
{
    std::array<Int, Count> result;
    for (size_t i = 0; i < Count; ++i)
    {
        result.at(i) = static_cast<Int>(std::round(values.at(i)));
    }
    return result;
}

//  上取整double数组到整型
template <typename Int, size_t Count>
std::array<Int, Count> CeilTo(std::array<double, Count> const& values)
{
    std::array<Int, Count> result;
    for (size_t i = 0; i < Count; ++i)
    {
        result.at(i) = static_cast<Int>(std::ceil(values.at(i)));
    }
    return result;
}

template <typename T, size_t Count>
inline std::vector<T> ToVector(std::array<T, Count> const& a)
{
    return std::vector<int64_t>(a.begin(), a.end());
}

template <size_t Count, typename T>
inline std::array<T, Count> ToArray(std::vector<T> const& v)
{
    std::array<T, Count> r{};
    if (v.size() < Count)
    {
        std::copy(v.begin(), v.end(), r.begin());
    }
    else
    {
        std::copy(v.begin(), v.begin() + Count, r.begin());
    }
    return r;
}

static inline void __set_error(int* const error, int value)
{
    if (error)
    {
        *error = value;
    }
}

/**
* 积分最小化限制条件：
* 1)若A赢BCD, A分别从BCD得分, A-X结算分为{A带入,X带入,X输分}中最小的那个.
* 2)若A输BCD, A分别给BCD输分, A-X结算分为{A带入,X带入,X赢分}中最小的那个.
* 3)若AB赢CD, A赢分不超过{CD总带入,A带入,CD总输分,A总赢分},B同理. 
  实现:分别计算在限制条件下, A/B单独与C/D结算的结果, 然后对其中一方进行缩放.
*/

template <typename Type = int64_t>
bool _TryReduceScore_CheckArgs(std::vector<Type> const& takeScoresIn, std::vector<Type> const& winScoresIn, int* const error = nullptr)
{
    if (takeScoresIn.size() != winScoresIn.size())
    {
        __set_error(error, -3);
        return {};
    }
    auto const Count = takeScoresIn.size();
    Type sumWinZero = 0;
    for (size_t i = 0; i < Count; ++i)
    {
        if (takeScoresIn.at(i) < 0)
        {
            __set_error(error, -1);
            return {};  // "negative player scores"
        }
        sumWinZero += winScoresIn.at(i);
    }
    if (sumWinZero != 0)
    {
        __set_error(error, -2);
        return {};  // "sum of win-scores is not 0"
    }
    return true;
}
/// <summary>
/// 进行积分最小化处理
/// </summary>
/// <param name="takeScoresIn">带入分</param>
/// <param name="winScoresIn">原始赢分</param>
/// <param name="error">捕获错误指针，传入nullptr可忽略，*error { 0:成功（0）, -1:带入分为负, -2:赢分和非0 }</param>
/// <returns>积分最小化后的赢分，若出错则为全0，且*error为非0</returns>
template <typename Type = int64_t>
std::vector<Type> TryReduceScore(std::vector<Type> const& takeScoresIn, std::vector<Type> const& winScoresIn, int* const error = nullptr)
{
    if (!_TryReduceScore_CheckArgs(takeScoresIn, winScoresIn, error))
    {
        return {};
    }
    auto const Count = takeScoresIn.size();

    std::vector<size_t> winnerIndexes;                   // 赢分玩家组
    std::vector<size_t> loserIndexes;                    // 输分玩家组
    std::vector<Type> tempWins(takeScoresIn.size(), 0);  //临时结果

    for (size_t i = 0; i < Count; ++i)
    {
        auto take = takeScoresIn.at(i);
        auto win = winScoresIn.at(i);
        if (win == 0 || take == 0)
        {
            continue;
        }
        (win > 0 ? winnerIndexes : loserIndexes).push_back(i);
    }

    if (loserIndexes.empty() || winnerIndexes.empty())
    {
        return tempWins;
    }

    Type winAll = 0;  // 限制条件下所有赢家总赢
    for (auto i : winnerIndexes)
    {
        auto take = takeScoresIn.at(i);
        auto win = winScoresIn.at(i);
        for (auto k : loserIndexes)
        {
            // temp 为 i 最多可赢 k
            auto temp = MinOf({take, win, takeScoresIn.at(k), -winScoresIn.at(k)});
            tempWins.at(i) += temp;
            tempWins.at(k) += temp;  //at(k)暂时记录loser的最多输分
        }
        // i 最终最多可赢
        tempWins.at(i) = MinOf({tempWins.at(i), win});
        winAll += tempWins.at(i);
    }
    Type loseAll = 0;  // 限制条件下所有输家总输
    for (auto k : loserIndexes)
    {
        // 对k最多可输做最小化
        tempWins.at(k) = MinOf({tempWins.at(k), takeScoresIn.at(k), -winScoresIn.at(k)});
        loseAll += tempWins.at(k);
    }

    auto minAll = MinOf({loseAll, winAll});            //输赢取较小的值
    auto winRate = (double)minAll / (double)winAll;    //赢分缩放
    auto loseRate = (double)minAll / (double)loseAll;  //输分缩放

    for (auto i : winnerIndexes)
    {
        tempWins.at(i) = (Type)std::round(winRate * tempWins.at(i));
    }

    for (auto k : loserIndexes)
    {
        tempWins.at(k) = -(Type)std::round(loseRate * tempWins.at(k));  //取负
    }
    return tempWins;
}

template <typename Type = int64_t, size_t Count>
[[deprecated]] std::array<Type, Count> TryReduceScore(std::array<Type, Count> const& takeScoresIn, std::array<Type, Count> const& winScoresIn, int* const error = nullptr)
{
    return ToArray<Count>(TryReduceScore(ToVector(takeScoresIn), ToVector(winScoresIn), error));
}

}  // namespace MJ
