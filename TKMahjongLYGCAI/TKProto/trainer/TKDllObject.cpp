#include "TKDllObject.h"

static int g_ObjectMaxID = 0;
CTKObject::CTKObject(void)
{
    m_nTKOjbectID = ++g_ObjectMaxID;
}

BOOL CTKObject::OnCreate()
{
    return TRUE;
}

void CTKObject::OnDestroy()
{
}
