/*
	Copyright (c) 2001, �������磨�ɶ���-������ĿȺ-�ط��齫��Ŀ��

	Version: 0.1
	Author: Guotj7400
	Date: 2022-04-19

	ժҪ:
	�齫����ҵ�������Ϊ�����, ������Ϸ��ֹ���ʱ, ��Ҫ����ֽ�����С������.
	ͬ��, �����������, Ϊ���ֹ�ƽ, �����Ӯ��ҲҪ��������.
	��ģ�����ĸ����(4���齫)�� ����� �� ��Ӯ�� ��Ϊ����, �������С�������Ľ��.

	��غ���:
	TryReduceScore - ���Ƴ�,ʹ��TryReduceScore���
	TryReduceScore
*/

#pragma once

#include <vector>
#include <array>
#include "MJHelper.h"

namespace MJ
{
/// ������������Сֵ
/// ����������value_typeĬ�Ϲ���ֵ
template <typename Type>
Type MinOf(std::initializer_list<Type> const& values)
{
    if (values.size() == 0)
        return Type{};
    auto iter = std::min_element(values.begin(), values.end());
    return *iter;
}

//  ��������double���鵽����
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

//  ��ȡ��double���鵽����
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
* ������С������������
* 1)��AӮBCD, A�ֱ��BCD�÷�, A-X�����Ϊ{A����,X����,X���}����С���Ǹ�.
* 2)��A��BCD, A�ֱ��BCD���, A-X�����Ϊ{A����,X����,XӮ��}����С���Ǹ�.
* 3)��ABӮCD, AӮ�ֲ�����{CD�ܴ���,A����,CD�����,A��Ӯ��},Bͬ��. 
  ʵ��:�ֱ����������������, A/B������C/D����Ľ��, Ȼ�������һ����������.
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
/// ���л�����С������
/// </summary>
/// <param name="takeScoresIn">�����</param>
/// <param name="winScoresIn">ԭʼӮ��</param>
/// <param name="error">�������ָ�룬����nullptr�ɺ��ԣ�*error { 0:�ɹ���0��, -1:�����Ϊ��, -2:Ӯ�ֺͷ�0 }</param>
/// <returns>������С�����Ӯ�֣���������Ϊȫ0����*errorΪ��0</returns>
template <typename Type = int64_t>
std::vector<Type> TryReduceScore(std::vector<Type> const& takeScoresIn, std::vector<Type> const& winScoresIn, int* const error = nullptr)
{
    if (!_TryReduceScore_CheckArgs(takeScoresIn, winScoresIn, error))
    {
        return {};
    }
    auto const Count = takeScoresIn.size();

    std::vector<size_t> winnerIndexes;                   // Ӯ�������
    std::vector<size_t> loserIndexes;                    // ��������
    std::vector<Type> tempWins(takeScoresIn.size(), 0);  //��ʱ���

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

    Type winAll = 0;  // ��������������Ӯ����Ӯ
    for (auto i : winnerIndexes)
    {
        auto take = takeScoresIn.at(i);
        auto win = winScoresIn.at(i);
        for (auto k : loserIndexes)
        {
            // temp Ϊ i ����Ӯ k
            auto temp = MinOf({take, win, takeScoresIn.at(k), -winScoresIn.at(k)});
            tempWins.at(i) += temp;
            tempWins.at(k) += temp;  //at(k)��ʱ��¼loser��������
        }
        // i ��������Ӯ
        tempWins.at(i) = MinOf({tempWins.at(i), win});
        winAll += tempWins.at(i);
    }
    Type loseAll = 0;  // ���������������������
    for (auto k : loserIndexes)
    {
        // ��k����������С��
        tempWins.at(k) = MinOf({tempWins.at(k), takeScoresIn.at(k), -winScoresIn.at(k)});
        loseAll += tempWins.at(k);
    }

    auto minAll = MinOf({loseAll, winAll});            //��Ӯȡ��С��ֵ
    auto winRate = (double)minAll / (double)winAll;    //Ӯ������
    auto loseRate = (double)minAll / (double)loseAll;  //�������

    for (auto i : winnerIndexes)
    {
        tempWins.at(i) = (Type)std::round(winRate * tempWins.at(i));
    }

    for (auto k : loserIndexes)
    {
        tempWins.at(k) = -(Type)std::round(loseRate * tempWins.at(k));  //ȡ��
    }
    return tempWins;
}

template <typename Type = int64_t, size_t Count>
[[deprecated]] std::array<Type, Count> TryReduceScore(std::array<Type, Count> const& takeScoresIn, std::array<Type, Count> const& winScoresIn, int* const error = nullptr)
{
    return ToArray<Count>(TryReduceScore(ToVector(takeScoresIn), ToVector(winScoresIn), error));
}

}  // namespace MJ
