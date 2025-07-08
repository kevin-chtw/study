#pragma once

typedef int(__cdecl* TKFIXLISTCMPFUNC)(const void* pElement1, const void* pElement2);

template <class TYPE>
class CTKFixList
{
public:
    CTKFixList(int cnExpand = 10, int lenElement = sizeof(TYPE), int lenHeader = 0);
    virtual ~CTKFixList();

public:
    //set expanding count of element buffer
    void SetExpandCount(int cnExpand, int cnTotal = 0);

    //set element length
    BOOL SetElementLength(int lenElement);

    //set header length
    BOOL SetElementHeaderLength(int lenHeader);

    //Get element buffer list pointer and element count
    TYPE* GetElementPtr(int& cnElement) const;

    //Get element header pointer and element count
    char* GetElementHeader(int& cnElement);

    //Get element buffer list pointer
    TYPE* GetElementPtr() const;

    //Get element header pointer
    char* GetElementHeader();

    //Detach Buf
    char* DetachElementBuf(int& nTotalLen);

    //reduce capability
    BOOL ReduceCapability();

public:
    // count of elements
    int GetCount() const;
    int GetSize() const;
    BOOL IsEmpty() const;

    // peek at head or tail
    BOOL GetHead(TYPE* pElement);
    TYPE* GetHead() const;
    BOOL GetTail(TYPE* pElement);
    TYPE* GetTail() const;

    // remove at head or tail
    BOOL RemoveHead();
    BOOL RemoveHead(int cnElement);
    BOOL RemoveTail();
    BOOL RemoveTail(int cnElement);

    // add before head or after tail
    BOOL AddHead(TYPE* pElement);
    BOOL AddTail(TYPE* pElement);

    // remove all elements
    void RemoveAll(BOOL bFreeList = FALSE);

    // adding/getting/modifying an element at a given position
    BOOL AddAt(int nPosition, TYPE* pElement);
    BOOL GetAt(int nPosition, TYPE* pElement);
    TYPE* GetAt(int nPosition) const;
    BOOL RemoveAt(int nPosition);
    BOOL ReplaceAt(int nPosition, TYPE* pElement);

    //finding/removing element by order
    TYPE* FindElement(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc = NULL) const;
    BOOL RemoveElement(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc = NULL);

    //finding element pos by order
    int FindElementPos(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc = NULL) const;
    //finding element pos on sorted element list
    int FindPos(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc) const;

    //finding/removing on sorted element list
    void Sort(TKFIXLISTCMPFUNC pCompFunc);
    BOOL Insert(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc);
    TYPE* Find(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc) const;
    BOOL Remove(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc);

private:
    //realloc capability
    BOOL ReallocCapability(BOOL bExpand = TRUE);

private:
    //length of element
    int m_lenElement;

    //capability of element buffer list
    int m_nTotalCap;

    //count of expanding element buffer
    int m_cnExpand;

    //length of header
    int m_lenHeader;

    //count of element
    int m_nCount;

    //element buffer list
    char* m_pElementList;

    //element header
    char* m_pElementHeader;
};

/////////////////////////////////////////////////////////////////////////////
// CTKFixList<TYPE> inline functions

template <class TYPE>
inline int CTKFixList<TYPE>::GetCount() const
{
    return m_nCount;
}

template <class TYPE>
inline int CTKFixList<TYPE>::GetSize() const
{
    return m_nCount;
}

template <class TYPE>
inline BOOL CTKFixList<TYPE>::IsEmpty() const
{
    return (m_nCount == 0);
}

template <class TYPE>
inline BOOL CTKFixList<TYPE>::GetHead(TYPE* pElement)
{
    if (m_nCount <= 0)
    {
        return FALSE;
    }
    memmove(pElement, m_pElementList, m_lenElement);
    return TRUE;
}

template <class TYPE>
inline TYPE* CTKFixList<TYPE>::GetHead() const
{
    if (m_nCount <= 0)
    {
        return NULL;
    }
    return (TYPE*)m_pElementList;
}

template <class TYPE>
inline BOOL CTKFixList<TYPE>::GetTail(TYPE* pElement)
{
    if (m_nCount <= 0)
    {
        return FALSE;
    }
    memmove(pElement, m_pElementList + (m_nCount - 1) * m_lenElement, m_lenElement);
    return TRUE;
}

template <class TYPE>
inline TYPE* CTKFixList<TYPE>::GetTail() const
{
    if (m_nCount <= 0)
    {
        return NULL;
    }
    return (TYPE*)(m_pElementList + (m_nCount - 1) * m_lenElement);
}

template <class TYPE>
CTKFixList<TYPE>::CTKFixList(int cnExpand, int lenElement, int lenHeader)
{
    if (cnExpand <= 0)
    {
        m_cnExpand = 10;
    }
    else
    {
        m_cnExpand = cnExpand;
    }
    if (lenElement <= 0)
    {
        m_lenElement = sizeof(TYPE);
    }
    else
    {
        m_lenElement = lenElement;
    }
    if (lenHeader >= 0)
    {
        m_lenHeader = lenHeader;
    }
    else
    {
        m_lenHeader = 0;
    }
    m_nTotalCap = 0;
    m_nCount = 0;
    m_pElementHeader = NULL;
    m_pElementList = NULL;
}

template <class TYPE>
CTKFixList<TYPE>::~CTKFixList()
{
    if (m_pElementHeader)
    {
        free(m_pElementHeader);
    }
    m_pElementList = NULL;
}

template <class TYPE>
inline void CTKFixList<TYPE>::SetExpandCount(int cnExpand, int cnTotal)
{
    if (cnExpand > 0)
    {
        m_cnExpand = cnExpand;
    }
    else if (cnTotal > 0)
    {
        m_cnExpand = 0x00002000;           //8192
        DWORD dwCount = 0x00000010;        //16
        while ((int)dwCount < m_cnExpand)  //
        {
            if (cnTotal <= (int)dwCount)
            {
                m_cnExpand = dwCount;
                break;
            }
            dwCount <<= 1;
        }
    }
}

template <class TYPE>
inline BOOL CTKFixList<TYPE>::SetElementLength(int lenElement)
{
    if (m_nTotalCap || lenElement <= 0)
    {
        return FALSE;
    }
    m_lenElement = lenElement;
    return TRUE;
}

template <class TYPE>
inline BOOL CTKFixList<TYPE>::SetElementHeaderLength(int lenHeader)
{
    if (m_nTotalCap || lenHeader <= 0)
    {
        return FALSE;
    }
    m_lenHeader = lenHeader;
    if (!ReallocCapability())
    {
        return FALSE;
    }
    return TRUE;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::RemoveHead()
{
    if (m_nCount <= 0)
    {
        return FALSE;
    }
    memmove(m_pElementList, m_pElementList + 1 * m_lenElement, (m_nCount - 1) * m_lenElement);
    m_nCount--;
    return TRUE;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::RemoveHead(int cnElement)
{
    if (m_nCount <= 0 || cnElement <= 0 || cnElement > m_nCount)
    {
        return FALSE;
    }
    if (cnElement == m_nCount)
    {
        RemoveAll();
    }
    else
    {
        memmove(m_pElementList, m_pElementList + cnElement * m_lenElement, (m_nCount - cnElement) * m_lenElement);
        m_nCount -= cnElement;
    }
    return TRUE;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::RemoveTail()
{
    if (m_nCount <= 0)
    {
        return FALSE;
    }
    m_nCount--;
    return TRUE;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::RemoveTail(int cnElement)
{
    if (m_nCount <= 0 || cnElement <= 0 || cnElement > m_nCount)
    {
        return FALSE;
    }
    m_nCount -= cnElement;
    return TRUE;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::AddHead(TYPE* pElement)
{
    if (!ReallocCapability())
    {
        return FALSE;
    }
    memmove(m_pElementList + m_lenElement, m_pElementList, m_nCount * m_lenElement);
    memmove(m_pElementList, pElement, m_lenElement);
    m_nCount++;
    return TRUE;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::AddTail(TYPE* pElement)
{
    if (!ReallocCapability())
    {
        return FALSE;
    }
    memmove(m_pElementList + m_nCount * m_lenElement, pElement, m_lenElement);
    m_nCount++;
    return TRUE;
}

template <class TYPE>
inline void CTKFixList<TYPE>::RemoveAll(BOOL bFreeList)
{
    if (bFreeList)
    {
        if (m_pElementHeader)
        {
            free(m_pElementHeader);
        }
        m_pElementHeader = NULL;
        m_pElementList = NULL;
        m_nTotalCap = 0;
    }
    m_nCount = 0;
}

template <class TYPE>
char* CTKFixList<TYPE>::DetachElementBuf(int& nTotalLen)
{
    char* s = m_pElementHeader;
    nTotalLen = m_nTotalCap;
    m_nTotalCap = 0;
    m_nCount = 0;
    m_pElementHeader = NULL;
    m_pElementList = NULL;
    return s;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::AddAt(int nPosition, TYPE* pElement)
{
    if (nPosition <= 0)
    {
        return AddHead(pElement);
    }
    if (nPosition >= m_nCount)
    {
        return AddTail(pElement);
    }
    if (!ReallocCapability())
    {
        return FALSE;
    }
    memmove(m_pElementList + (nPosition + 1) * m_lenElement, m_pElementList + nPosition * m_lenElement, (m_nCount - nPosition) * m_lenElement);
    memmove(m_pElementList + nPosition * m_lenElement, pElement, m_lenElement);
    m_nCount++;
    return TRUE;
}

template <class TYPE>
inline BOOL CTKFixList<TYPE>::GetAt(int nPosition, TYPE* pElement)
{
    if (nPosition < 0 || nPosition >= m_nCount)
    {
        return FALSE;
    }
    memmove(pElement, m_pElementList + nPosition * m_lenElement, m_lenElement);
    return TRUE;
}

template <class TYPE>
inline TYPE* CTKFixList<TYPE>::GetAt(int nPosition) const
{
    if (nPosition < 0 || nPosition >= m_nCount)
    {
        return NULL;
    }
    return (TYPE*)(m_pElementList + nPosition * m_lenElement);
}

template <class TYPE>
BOOL CTKFixList<TYPE>::RemoveAt(int nPosition)
{
    if (nPosition < 0 || nPosition >= m_nCount)
    {
        return FALSE;
    }
    memmove(m_pElementList + nPosition * m_lenElement, m_pElementList + (nPosition + 1) * m_lenElement, (m_nCount - nPosition - 1) * m_lenElement);
    m_nCount--;
    return TRUE;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::ReplaceAt(int nPosition, TYPE* pElement)
{
    if (nPosition < 0 || nPosition >= m_nCount)
    {
        return FALSE;
    }
    memmove(m_pElementList + nPosition * m_lenElement, pElement, m_lenElement);
    return TRUE;
}

template <class TYPE>
int CTKFixList<TYPE>::FindElementPos(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc) const
{
    if (m_nCount <= 0)
    {
        return -1;
    }
    if ((char*)pElement >= m_pElementList && (char*)pElement < m_pElementList + m_nCount * m_lenElement)
    {
        return ((char*)pElement - m_pElementList) / m_lenElement;
    }
    for (int i = 0; i < m_nCount; i++)
    {
        BOOL bRet;
        if (pCompFunc == NULL)
        {
            bRet = memcmp(pElement, m_pElementList + i * m_lenElement, m_lenElement);
        }
        else
        {
            bRet = pCompFunc(m_pElementList + i * m_lenElement, pElement);
        }
        if (bRet == 0)
        {
            return i;
        }
    }
    return -1;
}

template <class TYPE>
TYPE* CTKFixList<TYPE>::FindElement(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc) const
{
    int nPosition = FindElementPos(pElement, pCompFunc);
    if (nPosition < 0)
    {
        return NULL;
    }
    return (TYPE*)(m_pElementList + nPosition * m_lenElement);
}

template <class TYPE>
BOOL CTKFixList<TYPE>::RemoveElement(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc)
{
    int nPosition = FindElementPos(pElement, pCompFunc);
    if (nPosition < 0)
    {
        return FALSE;
    }
    return RemoveAt(nPosition);
}

template <class TYPE>
void CTKFixList<TYPE>::Sort(TKFIXLISTCMPFUNC pCompFunc)
{
    if (m_nCount > 0)
    {
        qsort(m_pElementList, m_nCount, m_lenElement, pCompFunc);
    }
}

template <class TYPE>
BOOL CTKFixList<TYPE>::Insert(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc)
{
    if (m_nCount == 0)
    {
        return AddTail(pElement);
    }

    TYPE* pElementTmp;
    if (m_nCount == 1)
    {
        pElementTmp = GetHead();
        if (pCompFunc(pElement, pElementTmp) <= 0)
        {
            return AddHead(pElement);
        }
        return AddTail(pElement);
    }
    int n1 = 0;
    int n2 = m_nCount - 1;
    pElementTmp = GetAt(n1);
    if (pCompFunc(pElement, pElementTmp) <= 0)
    {
        return AddHead(pElement);
    }
    pElementTmp = GetAt(n2);
    if (pCompFunc(pElement, pElementTmp) >= 0)
    {
        return AddTail(pElement);
    }

    while (TRUE)
    {
        if (n1 + 1 == n2)
        {
            return AddAt(n2, pElement);
        }
        int n = n1 + (n2 - n1) / 2;
        pElementTmp = GetAt(n);
        int nRet = pCompFunc(pElement, pElementTmp);
        if (nRet == 0)
        {
            return AddAt(n, pElement);
        }
        else if (nRet <= 0)
        {
            n2 = n;
        }
        else
        {
            n1 = n;
        }
    }
    return FALSE;
}

template <class TYPE>
int CTKFixList<TYPE>::FindPos(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc) const
{
    if (m_nCount <= 0)
    {
        return -1;
    }
    if ((char*)pElement >= m_pElementList && (char*)pElement < m_pElementList + m_nCount * m_lenElement)
    {
        return ((char*)pElement - m_pElementList) / m_lenElement;
    }
    pElement = (TYPE*)bsearch(pElement, m_pElementList, m_nCount, m_lenElement, pCompFunc);
    if (pElement == NULL)
    {
        return -1;
    }
    return ((char*)pElement - m_pElementList) / m_lenElement;
}

template <class TYPE>
TYPE* CTKFixList<TYPE>::Find(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc) const
{
    int nPosition = FindPos(pElement, pCompFunc);
    if (nPosition < 0)
    {
        return NULL;
    }
    return (TYPE*)(m_pElementList + nPosition * m_lenElement);
}

template <class TYPE>
BOOL CTKFixList<TYPE>::Remove(TYPE* pElement, TKFIXLISTCMPFUNC pCompFunc)
{
    int nPosition = FindPos(pElement, pCompFunc);
    if (nPosition < 0)
    {
        return FALSE;
    }
    return RemoveAt(nPosition);
}

template <class TYPE>
TYPE* CTKFixList<TYPE>::GetElementPtr(int& cnElement) const
{
    if (m_nCount == 0)
    {
        return NULL;
    }
    cnElement = m_nCount;
    return (TYPE*)(m_pElementList);
}

template <class TYPE>
char* CTKFixList<TYPE>::GetElementHeader(int& cnElement)
{
    if (m_lenHeader == 0)
    {
        if (m_nCount == 0)
        {
            return NULL;
        }
        cnElement = m_nCount;
        return m_pElementHeader;
    }
    if (m_pElementHeader == NULL)
    {
        m_pElementHeader = (char*)malloc(m_lenHeader);
        if (m_pElementHeader != NULL)
        {
            m_pElementList = m_pElementHeader + m_lenHeader;
        }
    }
    cnElement = m_nCount;
    return m_pElementHeader;
}

template <class TYPE>
TYPE* CTKFixList<TYPE>::GetElementPtr() const
{
    if (m_nCount == 0)
    {
        return NULL;
    }
    return (TYPE*)(m_pElementList);
}

template <class TYPE>
char* CTKFixList<TYPE>::GetElementHeader()
{
    if (m_lenHeader == 0)
    {
        if (m_nCount == 0)
        {
            return NULL;
        }
        return m_pElementHeader;
    }
    if (m_pElementHeader == NULL)
    {
        m_pElementHeader = (char*)malloc(m_lenHeader);
        if (m_pElementHeader != NULL)
        {
            m_pElementList = m_pElementHeader + m_lenHeader;
        }
    }
    return m_pElementHeader;
}

template <class TYPE>
BOOL CTKFixList<TYPE>::ReduceCapability()
{
    return ReallocCapability(FALSE);
}

template <class TYPE>
BOOL CTKFixList<TYPE>::ReallocCapability(BOOL bExpand)
{
    if (bExpand)
    {
        if (m_nTotalCap > m_nCount)
        {
            return TRUE;
        }
        char* pList = (char*)realloc(m_pElementHeader, m_lenHeader + (m_nTotalCap + m_cnExpand) * m_lenElement);
        if (pList == NULL)
        {
            return FALSE;
        }

        m_pElementHeader = pList;
        m_pElementList = m_pElementHeader + m_lenHeader;
        m_nTotalCap += m_cnExpand;
    }
    else
    {
        if (m_nTotalCap < m_nCount + m_cnExpand)
        {
            return TRUE;
        }
        if (m_nCount <= 0)
        {
            if (m_pElementHeader != NULL)
            {
                free(m_pElementHeader);
                m_pElementHeader = NULL;
                m_pElementList = NULL;
            }
            m_nCount = 0;
            m_nTotalCap = 0;
        }
        else
        {
            int nTotalCap = m_nTotalCap - ((m_nTotalCap - m_nCount) / m_cnExpand) * m_cnExpand;
            if (nTotalCap < m_nTotalCap)
            {
                char* pList = (char*)realloc(m_pElementHeader, m_lenHeader + nTotalCap * m_lenElement);
                if (pList == NULL)
                {
                    return FALSE;
                }
                m_pElementHeader = pList;
                m_pElementList = m_pElementHeader + m_lenHeader;
                m_nTotalCap = nTotalCap;
            }
        }
    }
    return TRUE;
}
