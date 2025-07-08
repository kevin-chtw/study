#include "TKMatchRuler.h"

CTKMatchRuler::CTKMatchRuler(void)
{
}

CTKMatchRuler::~CTKMatchRuler(void)
{
}

void CTKMatchRuler::SetProperyEx(const std::string& propertyEx)
{
    m_PropertyEx = propertyEx;
}

char* CTKMatchRuler::GamePropertyEx()
{
    return (char*)m_PropertyEx.c_str();
}

char* CTKMatchRuler::GameJsonRule(int& nJsonRuleLen)
{
    nJsonRuleLen = 0;
    return nullptr;
}
