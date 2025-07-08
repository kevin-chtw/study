#include "TKBuffer.h"

CTKBuffer::CTKBuffer()
{
}

CTKBuffer::~CTKBuffer()
{
}

BOOL CTKBuffer::SetHeaderLen(int lenHeader)
{
    m_lenHeader = lenHeader;
    m_Buffer.resize(m_lenHeader);
    return 1;
}

void CTKBuffer::Clear(BOOL bFree)
{
    m_Buffer.clear();
    m_lenData = 0;
    m_lenHeader = 0;
}

char* CTKBuffer::Detach()
{
    throw "";
}

BOOL CTKBuffer::Expand(int len)
{
    throw "";
}

BOOL CTKBuffer::ExpandTo(int len)
{
    throw "";
}

void CTKBuffer::Exchange(CTKBuffer& buffer)
{
    std::swap(m_Buffer, buffer.m_Buffer);
    std::swap(m_lenData, buffer.m_lenData);
    std::swap(m_lenHeader, buffer.m_lenHeader);
}

BOOL CTKBuffer::Append(const void* pBuf, int len)
{
    auto data = (const char*)pBuf;
    m_Buffer.insert(m_Buffer.end(), data, data + len);
    return TRUE;
}

BOOL CTKBuffer::AppendString(const char* pszString)
{
    m_Buffer.insert(m_Buffer.end(), pszString, pszString + strlen(pszString));
    return TRUE;
}
