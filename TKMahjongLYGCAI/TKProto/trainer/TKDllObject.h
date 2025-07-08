#pragma once
#include "framework.h"
#include "TKMatchS2GameSProtocolSrv.h"
#include "TKGameProtocol.h"

class CTKObject
{
public:
    CTKObject(void);
    virtual ~CTKObject(void) = default;

public:
    virtual BOOL OnCreate();
    virtual void OnDestroy();

public:
    LONG m_nTKOjbectID{};
};
