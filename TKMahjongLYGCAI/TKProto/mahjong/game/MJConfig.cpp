#include <math.h>
#include "MJConfig.h"
#include "MJException.h"
#include "MJString.h"
#include "MJLog.h"

void MJConfig::LoadRuler(std::string const& commaRuler)
{
    auto strValues = MJ::SplitByAny(commaRuler, ",");
    auto size = std::min(strValues.size(), m_Values.size());
    for (size_t i = 0; i < size; ++i)
    {
        auto s = MJ::TrimString(strValues.at(i));
        if (s.size())
        {
            m_Values.at(i) = MJ::ToNumber<int>(s);
        }
    }
}

static std::vector<int> _GetJsonArrayValues(Json::Value* v)
{
    if (v->isIntegral())
    {
        return {v->asInt()};
    }
    if (v->isArray())
    {
        std::vector<int> res;
        auto vsz = v->size();
        for (size_t i = 0; i < vsz; ++i)
        {
            auto const& value = (*v)[(int)i];
            if (value.isIntegral())
            {
                res.push_back(value.asInt());
            }
        }
        return res;
    }
    return {};
}
void MJConfig::LoadJsonRule(std::string const& json_ruler, MJConfigFDDescriptor const& formats)
{
    if (formats.m_IndexReflection.empty() || formats.m_JsonInfo.empty())
    {
        return;
    }
    try
    {
        // 初始化配置串
        _InitJsonConfig(formats);

        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(json_ruler, root, false) || !root.isObject())
        {
            MJ_LOG_ERROR(nullptr, "Bad Json String:", _S(json_ruler));
            return;
        }
        for (auto& p : formats.m_JsonInfo)
        {
            auto json_keys = _ParseValueKey(p.second);
            auto back = json_keys.back();
            json_keys.pop_back();

            Json::Value* v = _PickJsonValue(&root, json_keys);

            if (back == "int" && v->isIntegral())
            {
                this->_SetJsonValue(p.first, v->asInt(), formats);
            }
            else if (back == "array")
            {
                auto values = _GetJsonArrayValues(v);
                for (auto value : values)
                {
                    this->_SetJsonValue(p.first + value, 1, formats);
                }
            }
            else
            {
                MJ_LOG_ERROR(nullptr, "Warning: JSON Value, no key:%s", _S(p.second));
            }
        }
    }
    catch (MJException const& e)
    {
        MJ_LOG_ERROR(nullptr, "load josn ruler failed:%s e:%s", _S(json_ruler), _S(e.what()));
    }
    catch (...)
    {
        MJ_LOG_ERROR(nullptr, "load josn ruler failed:%s", _S(json_ruler));
    }
}

void MJConfig::LoadJsonRule(std::string const& json_ruler, MJFDConfigMap const& configs)
{
    if (configs.empty())
    {
        return;
    }
    Json::Value root{};
    if (!Json::Reader().parse(json_ruler, root, false) || !root.isObject())
    {
        MJ_LOG_ERROR(nullptr, "invalid fdtable ruler:%s", _S(json_ruler));
        return;
    }
    for (auto& p : configs)
    {
        auto& data = root[p.first];
        if (!data.isInt())
        {
            MJ_LOG_ERROR(nullptr, "invalid fdtable ruler for key:%s", _S(p.first));
            continue;
        }
        this->_SetConfig(p.second, data.asInt());
        MJ_LOG_DEBUG(nullptr, "fdtable set config index:%d key:%s value:%d", p.second, _S(p.first), (int)data.asInt());
    }
}

std::vector<int> const& MJConfig::GetValues() const
{
    return m_Values;
}

void MJConfig::_InitJsonConfig(MJConfigFDDescriptor const& formats)
{
    for (auto& p : formats.m_IndexReflection)
    {
        _SetConfig(p.second, 0);  // 未出现在checkout array中的项，表示未选中；其它项都有具体值
    }
}

// str like "play.some.int" or "play.some.array"
std::vector<std::string> MJConfig::_ParseValueKey(std::string const& str)
{
    auto json_keys = MJ::SplitByAny(str, ".");
    if (json_keys.empty() || (json_keys.back() != "int" && json_keys.back() != "array"))
    {
        throw MJ_MAKE_EXCEPTION("Error JsonKey key=%s", str.c_str());
    }
    return std::move(json_keys);
}

Json::Value* MJConfig::_PickJsonValue(Json::Value* root, std::vector<std::string> const& path)
{
    Json::Value* v = root;
    for (auto& key : path)
    {
        if (key.empty())
        {
            throw MJ_MAKE_EXCEPTION("Error Json Key empty");
        }
        if (::isdigit(key.front()))
        {
            auto index = MJ::ToNumber<int>(key);
            if (v->isArray() && (int)v->size() > index && index >= 0)
            {
                v = &((*v)[index]);  // 取子项再赋给v
            }
            else
            {
                throw MJ_MAKE_EXCEPTION("Error Array Index");
            }
        }
        else if (v->isObject())
        {
            v = &(*v)[key];
        }
        else
        {
            throw MJ_MAKE_EXCEPTION("Error Json for Key");
        }
    }
    return v;
}

void MJConfig::_SetJsonValue(int json_index, int value, MJConfigFDDescriptor const& formats)
{
    auto iter = formats.m_IndexReflection.find(json_index);
    if (iter == formats.m_IndexReflection.end())
    {
        throw MJ_MAKE_EXCEPTION("no Json Index Reflection");
    }
    _SetConfig(iter->second, value);
}

void MJConfig::_SetConfig(int index, int value)
{
    if (index < 0 || index >= (int)m_Values.size())
    {
        auto size = m_Values.size();
        throw MJ_MAKE_EXCEPTION("Json Reflection Index Error");
    }
    m_Values.at(index) = value;
}

void MJConfig::InitDefault(std::vector<int> defaultValues)
{
    m_Values = defaultValues;
}

int MJConfig::GetValue(ConfigIndex index) const
{
    if (index >= m_Values.size())
    {
        return 0;
    }
    return m_Values.at(index);
}

std::string MJConfig::ToCommaString() const
{
    return MJ::ToString(m_Values);
}
