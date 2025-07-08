#pragma once
#include <stdarg.h>
#include <sstream>
#include <exception>
#include <string>
#include "MJString.h"

struct MJException : public std::exception
{
    MJException(std::string fmt, ...)
    {
        std::stringstream ss;
        va_list args;
        va_start(args, fmt);

        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), fmt.c_str(), args);
        ss << buffer;

        va_end(args);

        m_What = ss.str();
    }

    const char* what() const throw() override
    {
        return m_What.c_str();
    }

    std::string m_What;
};

#define MJ_MAKE_EXCEPTION(fmt, ...) MJException("%s(%d)<%s>[Exception]:" fmt, MJ::GetPureFileName(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define ITS_TRUE(_true)                                     \
    do                                                      \
    {                                                       \
        if (!(_true))                                       \
        {                                                   \
            throw MJException("ERROR", __FILE__, __LINE__); \
        }                                                   \
    } while (0)
