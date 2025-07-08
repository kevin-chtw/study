#pragma once

#include "framework.h"

class CTKBuffer
{
public:
    CTKBuffer();
    virtual ~CTKBuffer();

public:
    //BOOL SetExpandLen(int lenExpand);
    BOOL SetHeaderLen(int lenHeader);
    //BOOL AddDataLen(int lenData);
    void Clear(BOOL bFree = FALSE);

public:
    char* GetBufPtr()
    {
        return m_Buffer.data();
    }
    char* GetDataPtr()
    {
        return m_Buffer.data() + m_lenHeader;
    }

    BOOL Append(const void* pBuf, int len);
    BOOL AppendText(const char* pszTest);
    BOOL AppendString(const char* pszString);
    BOOL AppendFormatText(LPCTSTR pszFmt, ...);
    BOOL AppendFormatTextV(LPCTSTR pszFmt, va_list args);
    BOOL AppendFormatString(LPCTSTR pszFmt, ...);
    BOOL AppendFormatStringV(LPCTSTR pszFmt, va_list args);

    BOOL AppendByte(BYTE byValue)
    {
        return Append(&byValue, sizeof(BYTE));
    };
    BOOL AppendWord(WORD wValue)
    {
        return Append(&wValue, sizeof(WORD));
    };
    BOOL AppendDWord(DWORD dwValue)
    {
        return Append(&dwValue, sizeof(DWORD));
    };
    BOOL AppendInt(int nValue)
    {
        return Append(&nValue, sizeof(int));
    };
    BOOL AppendULL(ULONGLONG ullValue)
    {
        return Append(&ullValue, sizeof(ULONGLONG));
    };
    BOOL Append(CTKBuffer& buffer)
    {
        return Append(buffer.GetBufPtr(), buffer.GetBufLen());
    };

    char* Detach();
    BOOL Expand(int len);
    BOOL ExpandTo(int len);
    void Exchange(CTKBuffer& buffer);

    int GetCapability()
    {
        return m_Buffer.capacity();
    };
    int GetBufLen()
    {
        return m_lenData + m_lenHeader;
    }
    int GetDataLen()
    {
        return m_lenData;
    };
    int GetHeaderLen()
    {
        return m_lenHeader;
    };
    int GetExpandLen()
    {
        return GetCapability() - GetBufLen();
    };

private:
    std::vector<char> m_Buffer;
    int m_lenData;
    int m_lenHeader;
};
