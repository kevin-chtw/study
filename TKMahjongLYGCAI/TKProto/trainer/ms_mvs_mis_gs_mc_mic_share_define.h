#pragma once

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string>
#include "TKMatchUniversalDicSrv.h"

namespace mvs
{
struct attribute
{
    WORD id;
    WORD data_size;
    // ºó¸úÊý¾Ý

    inline attribute* next()
    {
        return (attribute*)((char*)this + data_size);
    }

    inline DWORD dword_data()
    {
        return *((DWORD*)(this + 1));
    }
    inline int int_data()
    {
        return *((int*)(this + 1));
    }
    inline int64_t int64_data()
    {
        return *((int64_t*)(this + 1));
    }
    inline WORD word_data()
    {
        return *((WORD*)(this + 1));
    }
    inline BYTE byte_data()
    {
        return *((BYTE*)(this + 1));
    }
    inline char* string_data()
    {
        return (char*)(this + 1);
    }
    inline time_t time_t_data()
    {
        return *((time_t*)(this + 1));
    }
    inline void* data()
    {
        return this + 1;
    }
    inline size_t data_len()
    {
        return data_size - sizeof(attribute);
    }
    inline void push_int(int data)
    {
        *((int*)(this + 1)) = data;
        data_size = sizeof(attribute) + sizeof(int);
    }
    inline void push_int64(int64_t data)
    {
        *((int64_t*)(this + 1)) = data;
        data_size = sizeof(attribute) + sizeof(int64_t);
    }
    inline void push_word(WORD data)
    {
        *((WORD*)(this + 1)) = data;
        data_size = sizeof(attribute) + sizeof(WORD);
    }
    inline void push_byte(BYTE data)
    {
        *((BYTE*)(this + 1)) = data;
        data_size = sizeof(attribute) + sizeof(BYTE);
    }
    inline void push_dword(DWORD data)
    {
        *((DWORD*)(this + 1)) = data;
        data_size = sizeof(attribute) + sizeof(DWORD);
    }
    inline void push_string(const char* s)
    {
#pragma warning(push)
#pragma warning(disable : 4996)
        strcpy((char*)(this + 1), s);
#pragma warning(pop)
        data_size = sizeof(attribute) + (WORD)strlen(s) + 1;
    }
    inline void push_time_t(time_t data)
    {
        *((time_t*)(this + 1)) = data;
        data_size = sizeof(attribute) + sizeof(time_t);
    }
    inline void push_data(void* data, size_t len)
    {
        memcpy(this + 1, data, len);
        data_size = sizeof(attribute) + (WORD)len;
    }
};
};  // namespace mvs
