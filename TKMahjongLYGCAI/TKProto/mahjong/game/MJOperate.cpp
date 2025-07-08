#include "MJOperate.h"

std::string GetOperateName(EOperate operate, std::map<EOperate, std::string> const& names)
{
    auto iter = names.find(operate);
    if (iter == names.end())
    {
        return "";
    }
    return iter->second;
}

EOperate GetOperateID(std::string const& name, std::map<std::string, EOperate> const& ids)
{
    auto iter = ids.find(name);
    if (iter == ids.end())
    {
        return EOperateNone;
    }
    return iter->second;
}
