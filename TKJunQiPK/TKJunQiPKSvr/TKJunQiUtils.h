#pragma once
#include "TKJunQiPKDefine.h"

//铁路
class CRailway
{
public:
    enum EDirection  //顺序不能改变
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

    //是否可以平滑过渡到 next 铁路。平滑指：相同方向或切线方向
    bool CanTransformToSmoothly(const CRailway& next) const
    {
        return (this->m_out == next.m_in);
    }
    //逆时针旋转一下
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

//地图交叉点
struct SCross
{
    ECrossType type;             //交叉点类型
    set<int> road;               //公路可达
    map<CRailway, int> railway;  //铁路可达
};

//棋子
struct SChess
{
    EChessType type;  //棋子类型
    int seat;         //所属玩家
    int color;
    bool show;
    int index;  //棋子编号，用于判断重复走棋

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

//公路枚举器
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

//铁路枚举器
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
        int cross;                 //这是哪个交叉点
        int comeFrom;              //是从哪儿来的（缓冲区索引号），-1 表示起始节点
        CRailway comeFromRailway;  //来时的铁路
        UINT turnedCount;          //已拐转次数
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
        //展开
        for (UINT i = m_nextExpand; i < m_nextExpand + m_nextExpandLen; ++i)
            Expand(i);
        //排序
        std::sort(m_buf.begin() + m_nextExpand + m_nextExpandLen, m_buf.end(), LessTurn());
        //删除重复的和不合格的
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
        //更新 3 个指示器位置
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

    NodeBuf m_buf;         //搜索缓冲区
    UINT m_nextExpand;     //下一次要展开的起始位置
    UINT m_nextExpandLen;  //下一次要展开的个数
    UINT m_nextEnum;       //下一次要枚举出的位置
};

//枚举器包装成迭代器（支持有限）
template <class EnumerT>
class CEnumerIterator
{
public:
    typedef std::shared_ptr<EnumerT> EnumerPtr;

    // iterator 协议
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

    //前置
    CEnumerIterator& operator++()
    {
        Enum();
        return *this;
    }
    //后置
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

//容器样式（支持有限）
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

//路径枚举器
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