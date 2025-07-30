#pragma once
#include "TKJunQiPKDefine.h"

//��·
class CRailway
{
public:
    enum EDirection  //˳���ܸı�
    {
        ED_Up,
        ED_Left,
        ED_Down,
        ED_Right,
    };

    CRailway(EDirection inOut) : m_in(inOut), m_out(inOut)
    {
    }
    CRailway(EDirection in, EDirection out) : m_in(in), m_out(out)
    {
    }

    //�Ƿ����ƽ�����ɵ� next ��·��ƽ��ָ����ͬ��������߷���
    bool CanTransformToSmoothly(const CRailway& next) const
    {
        return (this->m_out == next.m_in);
    }
    //��ʱ����תһ��
    void RotateCounterClockwise()
    {
        m_in = EDirection((int(m_in) + 1) % 4);
        m_out = EDirection((int(m_out) + 1) % 4);
    }
    void RotateCounterClockwise(UINT times)
    {
        for (UINT i = 0; i < times; ++i)
            RotateCounterClockwise();
    }

private:
    EDirection m_in;
    EDirection m_out;
    friend bool operator<(const CRailway&, const CRailway&);
};
inline bool operator<(const CRailway& l, const CRailway& r)
{
    if (l.m_in < r.m_in)
        return true;
    if (l.m_in > r.m_in)
        return false;
    if (l.m_out < r.m_out)
        return true;
    if (l.m_out > r.m_out)
        return false;
    return false;
}

const CRailway s(CRailway::ED_Up), x(CRailway::ED_Down), z(CRailway::ED_Left), y(CRailway::ED_Right);
const CRailway sz(CRailway::ED_Up, CRailway::ED_Left), sy(CRailway::ED_Up, CRailway::ED_Right);

//��ͼ�����
struct SCross
{
    ECrossType type;             //���������
    set<int> road;               //��·�ɴ�
    map<CRailway, int> railway;  //��·�ɴ�
};

//����
struct SChess
{
    EChessType type;  //��������
    int seat;         //�������
    int color;
    bool show;
    int index;  //���ӱ�ţ������ж��ظ�����

    SChess() : type(ECT_ZhaDan), seat(JUNQIPK_UNKNOWN), color(EC_Red), show(true), index(0)
    {
    }

    bool operator==(const SChess& sc) const
    {
        return type == sc.type && seat == sc.seat && color == sc.color && show == sc.show && index == sc.index;
    }
};

class CJunQiPath
{
public:
    CJunQiPath()
    {
    }

    bool IsEmpty() const
    {
        return m_path.empty();
    }

    int From() const
    {
        return m_path.front();
    }

    int To() const
    {
        return m_path.back();
    }

    const std::vector<int>& FromTo() const
    {
        return m_path;
    }

    void AddFrom(int cross)
    {
        m_path.insert(m_path.begin(), cross);
    }
    void AddTo(int cross)
    {
        m_path.push_back(cross);
    }

private:
    std::vector<int> m_path;
};

//��·ö����
template <class PathCheckerT>
class CRoadEnumer
{
public:
    CRoadEnumer(const vector<SCross>& map, int beginCross, PathCheckerT* pathChecker)
        : m_beginCross(beginCross),
          m_pathChecker(pathChecker),
          m_currentRoad(map[beginCross].road.begin()),
          m_endRoad(map[beginCross].road.end())
    {
    }
    CJunQiPath Enum()
    {
        while (!IsEnd())
        {
            CJunQiPath path = EnumImp();
            if (m_pathChecker->CheckPath(path, 0))
                return path;
        }
        return CJunQiPath();
    }

private:
    CJunQiPath EnumImp()
    {
        CJunQiPath path;
        path.AddFrom(m_beginCross);
        path.AddTo(*(m_currentRoad++));
        return path;
    }
    bool IsEnd() const
    {
        return (m_currentRoad == m_endRoad);
    }

private:
    int m_beginCross;
    PathCheckerT* m_pathChecker;

    std::set<int>::const_iterator m_currentRoad;
    std::set<int>::const_iterator m_endRoad;
};

//��·ö����
template <class PathCheckerT>
class CRailwayEnumer
{
public:
    CRailwayEnumer(const vector<SCross>& map, int beginCross, PathCheckerT* pathChecker)
        : m_map(map), m_beginCross(beginCross), m_pathChecker(pathChecker)
    {
        if (UINT(beginCross) < map.size())
        {
            m_buf.reserve(50);
            m_buf.push_back(SEnumNode(beginCross));
        }
        m_nextExpand = 0;
        m_nextExpandLen = m_buf.size();
        m_nextEnum = m_buf.size();
    }

    CJunQiPath Enum()
    {
        if (m_nextEnum >= m_buf.size())
            Expand();
        if (m_nextEnum >= m_buf.size())
            return CJunQiPath();
        else
            return GetPath(m_nextEnum++);
    }

private:
    struct SEnumNode
    {
        SEnumNode(int crossPara, int comeFromPara = -1,
                  CRailway comeFromRailwayPara = CRailway(CRailway::EDirection(-1)), UINT turnedCountPara = 0)
            : cross(crossPara),
              comeFrom(comeFromPara),
              comeFromRailway(comeFromRailwayPara),
              turnedCount(turnedCountPara)
        {
        }
        int cross;                 //�����ĸ������
        int comeFrom;              //�Ǵ��Ķ����ģ������������ţ���-1 ��ʾ��ʼ�ڵ�
        CRailway comeFromRailway;  //��ʱ����·
        UINT turnedCount;          //�ѹ�ת����
    };
    struct CrossIs
    {
        CrossIs(int crossPara) : cross(crossPara)
        {
        }
        bool operator()(const SEnumNode& enumNode) const
        {
            return (enumNode.cross == cross);
        }
        int cross;
    };

    typedef std::vector<SEnumNode> NodeBuf;

    struct LessTurn
    {
        bool operator()(const SEnumNode node1, const SEnumNode node2)
        {
            return (node1.turnedCount < node2.turnedCount);
        }
    };
    void Expand()
    {
        //չ��
        for (UINT i = m_nextExpand; i < m_nextExpand + m_nextExpandLen; ++i)
            Expand(i);
        //����
        std::sort(m_buf.begin() + m_nextExpand + m_nextExpandLen, m_buf.end(), LessTurn());
        //ɾ���ظ��ĺͲ��ϸ��
        for (NodeBuf::iterator iter = m_buf.begin() + m_nextExpand + m_nextExpandLen; iter != m_buf.end();)
        {
            if ((std::find_if(m_buf.begin(), iter, boost::bind<bool>(CrossIs(iter->cross), _1)) != iter) ||
                (!m_pathChecker->CheckPath(GetPath(int(iter - m_buf.begin())), iter->turnedCount)))
            {
                iter = m_buf.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
        //���� 3 ��ָʾ��λ��
        m_nextExpand += m_nextExpandLen;
        m_nextExpandLen = m_buf.size() - m_nextExpand;
        m_nextEnum = m_nextExpand;
    }

    void Expand(UINT i)
    {
        SEnumNode node = m_buf[i];
        for (auto iter = m_map[node.cross].railway.begin(); iter != m_map[node.cross].railway.end(); ++iter)
        {
            const CRailway& toWay = iter->first;
            const int toCross = iter->second;

            UINT turnedCount = node.turnedCount;
            if ((node.comeFrom >= 0) && (!node.comeFromRailway.CanTransformToSmoothly(toWay)))
                ++turnedCount;

            m_buf.push_back(SEnumNode(toCross, i, toWay, turnedCount));
        }
    }
    CJunQiPath GetPath(int i)
    {
        CJunQiPath path;
        while (i >= 0)
        {
            path.AddFrom(m_buf[i].cross);
            i = m_buf[i].comeFrom;
        }
        return path;
    }

private:
    const vector<SCross>& m_map;
    int m_beginCross;
    PathCheckerT* m_pathChecker;

    NodeBuf m_buf;         //����������
    UINT m_nextExpand;     //��һ��Ҫչ������ʼλ��
    UINT m_nextExpandLen;  //��һ��Ҫչ���ĸ���
    UINT m_nextEnum;       //��һ��Ҫö�ٳ���λ��
};

//ö������װ�ɵ�������֧�����ޣ�
template <class EnumerT>
class CEnumerIterator
{
public:
    typedef std::shared_ptr<EnumerT> EnumerPtr;

    // iterator Э��
    typedef std::forward_iterator_tag iterator_category;
    typedef const CJunQiPath& value_type;
    typedef int difference_type;
    typedef const CJunQiPath* pointer;
    typedef const CJunQiPath& reference;

public:
    CEnumerIterator(EnumerPtr enumer) : m_enumer(enumer)
    {
        Enum();
    }
    CEnumerIterator() : m_isEnd(true)
    {
    }

    //ǰ��
    CEnumerIterator& operator++()
    {
        Enum();
        return *this;
    }
    //����
    CEnumerIterator operator++(int)
    {
        CEnumerIterator tmp = *this;
        ++*this;
        return (tmp);
    }

    const CJunQiPath& operator*() const
    {
        return m_path;
    }
    const CJunQiPath* operator->() const
    {
        return &m_path;
    }

    bool operator==(const CEnumerIterator<EnumerT>& other) const
    {
        if (m_isEnd && other.m_isEnd)
            return true;
        return false;
    }
    bool operator!=(const CEnumerIterator<EnumerT>& other) const
    {
        return !(*this == other);
    }

private:
    void Enum()
    {
        m_path = m_enumer->Enum();
        m_isEnd = m_path.IsEmpty();
    }

private:
    EnumerPtr m_enumer;
    CJunQiPath m_path;
    bool m_isEnd;
};

//������ʽ��֧�����ޣ�
template <class EnumerT, class PathCheckerT = CTKJunQiCore>
class CEnumerIteratorContainer
{
public:
    typedef CEnumerIterator<EnumerT> IterT;
    typedef IterT iterator;
    typedef IterT const_iterator;

public:
    CEnumerIteratorContainer(const vector<SCross>& map, int beginCross, PathCheckerT* pathChecker)
        : m_map(map), m_beginCross(beginCross), m_pathChecker(pathChecker)
    {
    }

    IterT begin()
    {
        return IterT(IterT::EnumerPtr(new EnumerT(m_map, m_beginCross, m_pathChecker)));
    }
    IterT end()
    {
        return IterT();
    }

private:
    const vector<SCross>& m_map;
    int m_beginCross;
    PathCheckerT* m_pathChecker;
};

//·��ö����
template <class PathCheckerT>
class CPathEnumer
{
    typedef CRoadEnumer<PathCheckerT> RoadEnumer;
    typedef CRailwayEnumer<PathCheckerT> RailwayEnumer;
    typedef CEnumerIterator<RoadEnumer> RoadIterator;
    typedef CEnumerIterator<RailwayEnumer> RailwayIterator;

public:
    CPathEnumer(const vector<SCross>& map, int beginCross, PathCheckerT* pathChecker)
        : m_roadIterator(RoadIterator::EnumerPtr(new RoadEnumer(map, beginCross, pathChecker))),
          m_railwayIterator(RailwayIterator::EnumerPtr(new RailwayEnumer(map, beginCross, pathChecker)))
    {
    }

    CJunQiPath Enum()
    {
        if (m_roadIterator != m_roadEnd)
            return *(m_roadIterator++);
        else if (m_railwayIterator != m_railwayEnd)
            return *(m_railwayIterator++);
        else
            return CJunQiPath();
    }

private:
    RoadIterator m_roadIterator;
    RoadIterator m_roadEnd;
    RailwayIterator m_railwayIterator;
    RailwayIterator m_railwayEnd;
};