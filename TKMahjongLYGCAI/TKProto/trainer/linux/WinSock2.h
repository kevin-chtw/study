#pragma once

struct in_addr
{
    struct
    {
        struct
        {
            unsigned char s_b1, s_b2, s_b3, s_b4;
        } S_un_b;
    } S_un;
};

struct sockaddr_in
{
    struct in_addr sin_addr;
};

#define AF_INET 2 /* internetwork: UDP, TCP, etc. */
inline void inet_pton(int type, const char* strIp, in_addr* addr)
{
}
