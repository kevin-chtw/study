#pragma once
#include <algorithm>
#include <cctype>
#include <ctime>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "base64"
#include <json/json.h>

/*
概述： 字符串转换扩展
主要方法：
	ToString(T) -> s
	SplitByAny(s) -> v<s>
	ToLower(s) -> s
	ToUpper(s) -> s
	ToQuoteString(T) -> s
	GetPureFileName(s) -> s
	ToNumber<T>(s) -> T
	ToNumbers<T>(v<s>) -> T
	SplitByAny(s) -> v<s>

PS:
s:string, v:vector, T:template type
*/

using namespace std::string_literals;

namespace MJ
{
#pragma region ToString
// @function ToString
// 扩展 std::to_string, 添加对数组，vector，map的支持
// 1. 内置类型同 std::to_string
// 2. 数组或vector ： 生成逗号分隔的字符串
// 3. std::map : 生成串形如 "abc:9988,hello:0,ok:true"

// bool 、 char 、 char8_t 、 char16_t 、 char32_t 、 wchar_t 、 short 、 int 、 long 、 long long
template <typename _Number, typename std::enable_if<std::is_integral<_Number>::value, void*>::type = nullptr>
std::string ToString(_Number value)
{
    return std::to_string(value);
}

// enum
template <typename _Number, typename std::enable_if<std::is_enum<_Number>::value, void*>::type = nullptr>
std::string ToString(_Number value)
{
    return std::to_string((int64_t)value);
}

// float 、 double 、 long double
template <typename _Float, typename std::enable_if<std::is_floating_point<_Float>::value, void*>::type = nullptr>
std::string ToString(_Float value)
{
    return std::to_string(value);  // .123456
}

// std::string
inline std::string ToString(std::string const& s)
{
    return s;
}

// 数组
template <typename _Type, size_t Count>
std::string ToString(_Type (&values)[Count])
{
    std::string res;
    for (size_t i = 0; i < Count; ++i)
    {
        if (i != 0)
        {
            res.push_back(',');
        }
        res += ToString(values[i]);
    }
    return res;
}
// c++ 数组
template <typename _Type, size_t Count>
std::string ToString(std::array<_Type, Count> const& data)
{
    std::string res;
    for (size_t i = 0; i < Count; ++i)
    {
        if (i != 0)
        {
            res.push_back(',');
        }
        res += ToString(data[i]);
    }
    return res;
}

// char 数组特殊处理
template <size_t Count>
std::string ToString(char (&c_array)[Count])
{
    std::string str;
    for (auto i : c_array)
    {
        if (i == 0)
        {
            break;
        }
        str.push_back(i);
    }
    return str;
}

// c std::string
inline std::string ToString(char const* c_str)
{
    return std::string(c_str);
}

template <typename _Cnt>
std::string ContainerToString(_Cnt const& v)
{
    std::string res;
    bool add{};
    for (auto const& i : v)
    {
        if (add)
        {
            res += ',';
        }
        res += ToString(i);
        add = true;
    }
    return res;
}

// std::map
// return linke "k:v,k:v,k:v"
template <typename _Key, typename _Value>
std::string ToString(std::map<_Key, _Value> const& m)
{
    return ContainerToString(m);
}

// std::vector
// return like "v,v,v,v,v"
template <typename _Value>
std::string ToString(std::vector<_Value> const& v)
{
    return ContainerToString(v);
}
template <typename _Value>
std::string ToString(std::initializer_list<_Value> const& v)
{
    return ContainerToString(v);
}

template <typename _Value>
std::string ToString(std::set<_Value> const& v)
{
    return ContainerToString(v);
}

template <typename _Key, typename _Value>
std::string ToString(std::pair<_Key, _Value> const& v)
{
    return ToString(v.first) + ":" + ToString(v.second);
}

template <typename _Type, typename _Iter = typename _Type::iterator>
inline std::string ToString(_Type const& cnt)
{
    return ContainerToString(cnt);
}

#pragma endregion

#pragma region "ToLower/ToUpper"
// 转换为小写串
inline std::string ToLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)::tolower(c); });
    return s;
}
// 转换为大写串
inline std::string ToUpper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)::toupper(c); });
    return s;
}
inline void ToLowerInplace(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)::tolower(c); });
}
// 转换为大写串
inline void ToUpperInplace(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)::toupper(c); });
}

#pragma endregion

#pragma region "ToQuoteString"
// 字符串需要加引号
template <typename Type>
inline std::string ToQuoteString(Type const& s)
{
    return "\"" + ToString(s) + "\"";
}
#pragma endregion

#pragma region "ToNumber"

/// std::string 转整数
template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value, void*>::type = nullptr>
Integer ToNumber(std::string const& s)
{
    return static_cast<Integer>(std::strtoll(s.c_str(), nullptr, 10));
}

/// std::string 转 浮点数
template <typename Float, typename std::enable_if<std::is_floating_point<Float>::value, void*>::type = nullptr>
Float ToNumber(std::string const& s)
{
    return static_cast<Float>(std::strtold(s.c_str(), nullptr));
}

/// 批量转数值
template <typename Type>
std::vector<Type> ToNumbers(std::vector<std::string> const& num_strings)
{
    std::vector<Type> ret;
    for (auto& s : num_strings)
    {
        ret.push_back(ToNumber<Type>(s));
    }
    return ret;
}
#pragma endregion

#pragma region Split Trim
/// <summary>
/// 分割字符串
/// </summary>
/// <param name="s">由分隔符串接的字符串</param>
/// <param name="delims">以delims中任意字符作为分隔符</param>
/// <returns>分割后的字符串vector</returns>
inline std::vector<std::string> SplitByAny(const std::string& s, const std::string& delims)
{
    if (s.empty())
    {
        return {};
    }
    if (delims.empty())
    {
        return {s};
    }

    std::vector<std::string> ret;
    size_t last = 0;
    size_t index = s.find_first_of(delims, last);
    while (index != std::string::npos)
    {
        ret.push_back(s.substr(last, index - last));

        last = index + 1;
        index = s.find_first_of(delims, last);
    }
    // here index is s.npos
    ret.push_back(s.substr(last));

    return ret;
}

inline std::vector<std::string> SplitByString(const std::string& s, const std::string& seperator)
{
    if (s.empty())
    {
        return {};
    }
    if (seperator.empty())
    {
        return {s};
    }

    std::vector<std::string> ret;
    size_t last = 0;
    size_t index = s.find(seperator, last);
    while (index != std::string::npos)
    {
        ret.push_back(s.substr(last, index - last));
        last = index + seperator.size();
        index = s.find(seperator, last);
    }
    // here index is s.npos
    ret.push_back(s.substr(last));

    return ret;
}

// 去掉字符串前后的空白字符(由::isspace判定）
inline std::string TrimString(std::string const& s)
{
    int rcount = 0;
    for (auto iter = s.rbegin(); iter != s.rend(); ++iter)
    {
        if (std::isspace((uint8_t)*iter))
            ++rcount;
        else
            break;
    }
    if (rcount == s.size())
        return "";

    int count = 0;
    for (auto iter = s.begin(); iter != s.end(); ++iter)
    {
        if (std::isspace((uint8_t)*iter))
            ++count;
        else
            break;
    }
    return s.substr(count, s.size() - count - rcount);
}

#pragma endregion
}  // namespace MJ

namespace MJ
{
/// 去掉文件路径中的目录信息
inline const char* GetPureFileName(const char* path)
{
    auto ret = path;
    while (*path)
    {
        if (*path == '/' || *path == '\\')
        {
            ret = path + 1;
        }
        path++;
    }
    return ret;
}
}  // namespace MJ

// TimeString
namespace MJ
{
// str like "1999-11-12 18:02:02"
/// return -1
inline time_t TimeStringToTime(std::string const& str)
{
    int year = 1970;
    int month = 1;
    int day = 1;
    int hour = 8;
    int min = 0;
    int sec = 0;

#if defined(_WIN32)
    sscanf_s(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec);
#else
    sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec);
#endif

    tm info{};
    info.tm_year = year - 1900;
    info.tm_mon = month - 1;
    info.tm_mday = day;
    info.tm_hour = hour;
    info.tm_min = min;
    info.tm_sec = sec;
    info.tm_isdst = 0;

    return mktime(&info);
}
inline std::string TimeToString(time_t value, bool digitOnly = false)
{
    tm info{};
#if defined(_WIN32)
    localtime_s(&info, &value);
#else
    localtime_r(&value, &info);
#endif
    char buf[128]{};
    strftime(buf, 128, digitOnly ? "%Y%m%d%H%M%S" : "%Y-%m-%d %H:%M:%S", &info);
    return buf;
}

struct Formator
{
    std::string m_Msg;
    std::vector<std::pair<std::string, std::string>> m_Datas;

    explicit Formator(std::string msg)
    {
        m_Msg = msg;
    }
    Formator& Push(std::string name, int64_t value)
    {
        m_Datas.push_back({name, std::to_string(value)});
        return *this;
    }
    Formator& Push(std::string name, std::string str)
    {
        m_Datas.push_back({name, str});
        return *this;
    }
    template <typename Type>
    Formator& Push(std::string name, Type const& value)
    {
        m_Datas.push_back({name, MJ::ToString(value)});
        return *this;
    }

    template <typename T1, typename T2>
    Formator& operator()(T1 const& v1, T2 const& v2)
    {
        return this->Push(v1, v2);
    }
    void clear()
    {
        m_Datas.clear();
        m_Msg.clear();
    }
    std::string ToString() const
    {
        if (m_Datas.empty())
        {
            return m_Msg;
        }
        else
        {
            std::string datas;
            for (auto& v : m_Datas)
            {
                if (datas.size())
                {
                    datas.push_back(',');
                }
                datas += v.first + ":" + v.second;
            }
            return m_Msg + "[" + datas + "]";
        }
    }
};

inline std::string Base64Encode(std::string const& s)
{
    return boost_beast::base64_encode(s);
}
inline std::string Base64Decode(std::string const& s)
{
    return boost_beast::base64_decode(s);
}

inline std::string ToString(Json::Value v)
{
    Json::FastWriter jwrite;
    std::string str = jwrite.write(v);
    if (!str.empty() && str.back() == '\n')
    {
        str.pop_back();
    }
    return str;
}

}  // namespace MJ
