#include "StdAfx.h"
#include "TKJunQiPKCfg.h"
#include <boost/algorithm/string.hpp>

CTKJunQiPKCfg::CTKJunQiPKCfg()
{
    memset(m_nCfgValue, 0, ECI_End * sizeof(int));
}

CTKJunQiPKCfg::~CTKJunQiPKCfg()
{
}

void CTKJunQiPKCfg::InitCfg(const string& szCfg)
{
    vector<string> vStrCfg;
    boost::split(vStrCfg, szCfg, boost::is_any_of(","));
    for (int i = 0; i < ECI_End; ++i)
    {
        if (i < vStrCfg.size() && !vStrCfg[i].empty())
        {
            m_nCfgValue[i] = boost::lexical_cast<int>(vStrCfg[i]);
        }
        else
        {
            m_nCfgValue[i] = g_nDefaultValue[i];
        }
    }
}

int CTKJunQiPKCfg::GetCfg(ECfgIndex nCfgId)
{
    int nValue = 0;
    if (nCfgId >= 0 && nCfgId < ECI_End)
    {
        nValue = m_nCfgValue[nCfgId];
    }
    return nValue;
}

void CTKJunQiPKCfg::SetCfg(ECfgIndex nCfgId, int nCfgValue)
{
    if (nCfgId >= 0 && nCfgId < ECI_End)
    {
        m_nCfgValue[nCfgId] = nCfgValue;
    }
}
