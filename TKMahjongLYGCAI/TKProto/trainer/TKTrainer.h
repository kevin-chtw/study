#pragma once

#include <stdio.h>
#include <string>
#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __attribute__((visibility("default")))
#endif

extern "C"
{
    DLLEXPORT void InitService();
    DLLEXPORT void StartGame(char const* conf);
    DLLEXPORT const char* GetAction();
    DLLEXPORT bool DoAction(char const* action);
}
