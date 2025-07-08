#pragma once

#include <string>
#include <array>
#include <vector>
#include "json/json.h"
#include "MJModule.h"

// ConfigIndex ���ô��±�
using ConfigIndex = unsigned;

/*
- m_JsonInfo #json_index# ��Ӧ�ļ������εļ��õ�ָ�����һ��ӦΪint��array���磺hupai.value.int
- m_IndexReflection #json_index# ��Ӧ��ֵ��ConfigIndex����ӳ��
*/
struct MJConfigFDDescriptor
{
    std::map<int /*json_index*/, std::string /*jkeys*/> m_JsonInfo;
    std::map<int /*json_index*/, ConfigIndex> m_IndexReflection;
};

// �°���ѷ����ã���� MJConfigFDDescriptor
using MJFDConfigMap = std::map<std::string, ConfigIndex>;

class MJConfig
{
public:
    void InitDefault(std::vector<int> defaultValues);

    ~MJConfig() = default;

    int GetValue(ConfigIndex index) const;

    std::string ToCommaString() const;

    void LoadRuler(std::string const& commaRuler);

    /// <summary>
    /// ����LoadJsonRuleǰ������ LoadRuler.
    /// </summary>
    /// <param name="s"></param>
    /// <param name="formats">Json�����ݸ�ʽ</param>
    void LoadJsonRule(std::string const& json_ruler, MJConfigFDDescriptor const& formats);

    void LoadJsonRule(std::string const& json_ruler, MJFDConfigMap const& configs);

    std::vector<int> const& GetValues() const;

protected:
    std::vector<int> m_Values;

private:
    void _InitJsonConfig(MJConfigFDDescriptor const& formats);
    std::vector<std::string> _ParseValueKey(std::string const& str);
    Json::Value* _PickJsonValue(Json::Value* root, std::vector<std::string> const& path);

    void _SetJsonValue(int json_index, int value, MJConfigFDDescriptor const& formats);

    void _SetConfig(int index, int value);
};
