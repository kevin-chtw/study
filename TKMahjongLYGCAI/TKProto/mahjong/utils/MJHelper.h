/*
	Copyright (c) 2001, 竞技世界（成都）

	Version: 0.1
	Author: Guotj7400
	Date: 2022-04-19

	摘要: 一些常用操作
	Functions:

	-- 随机数
	GetRandomEngine
	Random
	RandomByRates

	-- 16进制字符
	HexValue
	HexChar

	-- 容器操作
	HasElement
	CountElement
	RemoveElement
	HasSameKey(map, map)
	HasSameElements(cnt, cnt)

*/
#pragma once

#include <cmath>
#include <map>
#include <functional>
#include <algorithm>
#include <ctime>
#include <vector>
#include <numeric>
#include <set>
#include <deque>
#include <array>
#include <string>
#include <cctype>
#include <random>

namespace MJ
{
// 注意：不同源文件引用会产生不同实例
static std::default_random_engine DefaultRandomEngine((unsigned int)time(nullptr));

// return random value in range [0, maxValue).
template <typename Int>
Int Random(Int maxValue)
{
    auto mv = (unsigned int)maxValue;
    return int(mv ? (DefaultRandomEngine() % mv) : 0);
}

/* get a random index, by rates.
 case rates = [3, 1]: we have rates of 1/4 return 1, 3/4 return 0.
 case raets = [3, 3, 3]: we have the same rates of 3/9 to return 0,1,2.
 template Vector can be std::vector<int>, std::array<int>, ....
*/
template <typename Vector>
inline int RandomByRates(Vector const& rates)
{
    int sum = std::accumulate(std::begin(rates), std::end(rates), 0);
    if (sum <= 0)
    {
        return 0;
    }
    int value = (int)Random(sum);
    for (int i = 0; i < (int)std::size(rates); ++i)
    {
        if (value < rates[i])
            return i;
        value -= rates[i];
    }
    return 0;
}

// 16进制字符转数值，ASCii“[0-9a-fA-F]”转为 int-[0,15]
inline int HexValue(char hexChar)
{
    if (::isdigit(hexChar))
        return hexChar - '0';
    if (hexChar >= 'a' && hexChar <= 'f')
        return 10 + (hexChar - 'a');
    if (hexChar >= 'A' && hexChar <= 'F')
        return 10 + (hexChar - 'A');
    return 0;
}

// int[0-15] 转为 char[0-9A-F]
inline char HexChar(int value)
{
    if (value >= 0 && value <= 9)
        return char('0' + value);
    if (value >= 10 && value <= 15)
        return char('A' + (value - 10));
    return '?';
}

}  // namespace MJ

/// 以下是一些对STL的快速操作扩展，合理使用可以使代码更易于阅读
namespace MJ
{
/// 容器中是否存在元素 v
///     eg: HasElement(std::set<int>{1, 2, ,3}, v)
///        等价于 (v == 1 || v == 2 || v == 3)
template <typename Cnt, typename Type>
bool HasElement(Cnt const& cnt, Type const& v)
{
    return std::find(std::begin(cnt), std::end(cnt), v) != std::end(cnt);
}

//template <typename Type>
//bool HasElement(std::deque<Type> const& cnt, Type const& v)
//{
//    return std::find(cnt.begin(), cnt.end(), v) != cnt.end();
//}
template <typename Cnt, typename Type>
bool HasKey(Cnt const& cnt, Type const& v)
{
    return cnt.find(v) != cnt.end();
}

// cnt 中包含
template <class container, typename Type>
size_t CountElement(container const& cnt, Type const& v)
{
    size_t count = 0;
    for (auto& e : cnt)
    {
        if (e == v)
            ++count;
    }
    return count;
}

/// <summary>
/// 从 vector 中移除第一个元素 v
/// </summary>
/// <typeparam name="Type">应可做相等比较</typeparam>
/// <param name="mutableContainer"></param>
/// <param name="v"></param>
/// <returns>移除成功返回true</returns>
template <class container, typename Type>
bool RemoveElement(container& mutableContainer, Type const& v)
{
    auto iter = std::find(mutableContainer.begin(), mutableContainer.end(), v);
    if (iter == mutableContainer.end())
    {
        return false;
    }
    mutableContainer.erase(iter);
    return true;
}

template <class container, typename Type>
size_t RemoveElements(container& mutableContainer, Type const& v, size_t count)
{
    size_t removed = 0;
    for (auto iter = mutableContainer.begin(); removed < count && iter != mutableContainer.end();)
    {
        if (*iter == v)
        {
            ++removed;
            iter = mutableContainer.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
    return removed;
}

/// <summary>
/// 删除所有值为 v 的元素
/// </summary>
/// <returns>移除元素个数</returns>
template <class container, typename Type>
size_t RemoveAllElement(container& mutableContainer, Type const& v)
{
    size_t count = 0;
    for (auto iter = mutableContainer.begin(); iter < mutableContainer.end();)
    {
        if (*iter == v)
        {
            iter = mutableContainer.erase(iter);
            ++count;
        }
        else
        {
            ++iter;
        }
    }
    return count;
}

/// <summary>
/// map m1 和 m2 是否有相同的 键（Key），注：忽略值（Value）是否相同
/// </summary>
template <typename Key, typename Value>
bool HasSameKeys(std::map<Key, Value> const& m1, std::map<Key, Value> const& m2)
{
    if (m1.size() != m2.size())
    {
        return false;
    }
    for (auto iter1 = m1.begin(), iter2 = m2.begin(); iter1 != m1.end(); ++iter1, ++iter2)
    {
        if (iter1->first != iter2->first)
        {
            return false;
        }
    }
    return true;
}

/// <summary>
/// 序列容器 v1 和 v2 是否相同
/// </summary>
/// <param name="reorder">比较时是否忽略顺序，即当reorder为true时 {1，2} == {2，1} 成立</param>
template <typename Type>
bool HasSameElements(std::vector<Type> const& v1, std::vector<Type> const& v2, bool reorder)
{
    if (v1.size() != v2.size())
    {
        return false;
    }

    if (reorder)
    {
        std::vector<Type> ov1 = v1;
        std::vector<Type> ov2 = v2;
        std::sort(ov1.begin(), ov1.end());
        std::sort(ov2.begin(), ov2.end());
        return HasSameElements(ov1, ov2, false);
    }

    // assert v1.size == v2.size
    for (auto iter1 = v1.begin(), iter2 = v2.begin(); iter1 != v1.end(); ++iter1, ++iter2)
    {
        if (*iter1 != *iter2)
        {
            return false;
        }
    }
    return true;
}

// 若Cnt中包含set中的任何一个元素即返回true
// set为空时，返回false
template <typename Cnt, typename Set>
bool HasAnyElement(Cnt const& cnt, Set const& targets)
{
    for (auto v : targets)
    {
        if (HasElement(cnt, v))
            return true;
    }
    return false;
}

// 若Cnt中存在set中的所有元素则返回true
// set为空时，返回true
template <typename Cnt, typename Set>
bool HasAllElement(Cnt const& cnt, Set const& targets)
{
    for (auto v : targets)
    {
        if (!HasElement(cnt, v))
            return false;
    }
    return true;
}

// 若Cnt中包含set中的任何一个key即返回true
// set为空时，返回false
template <typename Cnt, typename Type>
bool HasAnyKey(Cnt const& cnt, std::set<Type> const& targets)
{
    for (auto v : targets)
    {
        if (HasKey(cnt, v))
            return true;
    }
    return false;
}

// 若Cnt中存在set中的所有key则返回true
// set为空时，返回true
template <typename Cnt, typename Type>
bool HasAllKey(Cnt const& cnt, std::set<Type> const& targets)
{
    for (auto v : targets)
    {
        if (!HasKey(cnt, v))
            return false;
    }
    return true;
}

template <typename Cnt>
std::vector<typename Cnt::value_type> ToVector(Cnt const& cnt)
{
    return std::vector<typename Cnt::value_type>(std::begin(cnt), std::end(cnt));
}

// p不为空时，置为value
template <typename Type>
void SetValueIfNotNull(Type* p, Type const& value)
{
    if (p != nullptr)
    {
        *p = value;
    }
}

}  // namespace MJ
