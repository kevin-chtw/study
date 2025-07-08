/*
	ժҪ: �������ڲ�һЩ���������ú�
*/
#pragma once

#include <string>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <functional>
#include <exception>
#include <memory>
#include "MJOperate.h"

// ����ģʽ
#if defined(_DEBUG) && !defined(NDEBUG)
#define MJ_DEBUG 1
#else
#define MJ_DEBUG 0
#endif

#define out__     //Tip: ע������������ڵ����б��޸�
#define in_out__  //Tip: ע���������Ӧ������Чֵ���һᱻ�޸�

using ITileID = int;
using ISeatID = int;

constexpr ITileID TILE_NULL = (0);   //ǰ�˶�0����ʾ�����棬��Ч����
constexpr ITileID TILE_BACK = (-1);  //ǰ����-1��ʾ�Ƶĵı��棬����Ч���Լ����ɼ�����
constexpr ISeatID SEAT_NULL = (-1);
constexpr ISeatID SEAT_ALL = (-2);

//player count
#ifndef M_NP4
#define M_NP4
constexpr int NP4 = 4;
constexpr int NP3 = 3;
constexpr int NP2 = 2;
#endif

constexpr int TILE_COUNT_INIT_BANKER = 14;
constexpr int TILE_COUNT_INIT_NORMAL = 13;

// �����¼�, �� 3,2,1,0��˳��
inline ISeatID MJGetNextSeat(ISeatID seat, int step = 1, int SeatCount = NP4)
{
    return (seat + (SeatCount - step % SeatCount)) % SeatCount;
}

enum class EMJColor
{
    Undefined = -1,
    Character = 0,  //��
    Bamboo = 1,     // ��
    Dot = 2,        // Ͳ
    Wind = 3,       // ��,��������
    Dragon = 4,     // ��,�з���
    Flower = 5,     // ÷�����
    Season = 6,     // �����ﶬ
    Hun = 7,        // ���ӣ�wild��
    _End,
    _Begin = 0,

    _SuitFront = Character,
    _SuitBack = Dot,
};

inline int MJIsValidColor(EMJColor color)
{
    return color >= EMJColor::_Begin && color < EMJColor::_End;
}
inline constexpr ITileID MJMakeTile(int color, int point, int flag = 1)
{
    return (((color) << 8) | ((point) << 4) | flag);
}
constexpr int TILE_ID_STEP = MJMakeTile(0, 1) - MJMakeTile(0, 0);

inline constexpr ITileID MJMakeTile(EMJColor color, int point, int flag = 1)
{
    return MJMakeTile((int)color, point, flag);
}

constexpr std::array<const int, (int)EMJColor::_End> MJPointCountByColor = {9, 9, 9, 4, 3, 4, 4, 0};
constexpr std::array<const int, (int)EMJColor::_End> MJSameTileCountByColor = {4, 4, 4, 4, 4, 1, 1, 0};

constexpr ITileID TILE_HUN = MJMakeTile(EMJColor::Hun, 0);
constexpr ITileID TILE_INF = MJMakeTile(EMJColor::_End, 0);  //��Ч

//���������з���
constexpr ITileID TILE_ZHONG = MJMakeTile(EMJColor::Dragon, 0);
constexpr ITileID TILE_FA = MJMakeTile(EMJColor::Dragon, 1);
constexpr ITileID TILE_BAI = MJMakeTile(EMJColor::Dragon, 2);
constexpr ITileID TILE_DONG = MJMakeTile(EMJColor::Wind, 0);
constexpr ITileID TILE_NAN = MJMakeTile(EMJColor::Wind, 1);
constexpr ITileID TILE_XI = MJMakeTile(EMJColor::Wind, 2);
constexpr ITileID TILE_BEI = MJMakeTile(EMJColor::Wind, 3);

constexpr ITileID TILE_YAOJI = MJMakeTile(EMJColor::Bamboo, 0);

constexpr ITileID TILE_FLOWER = MJMakeTile(EMJColor::Flower, 0);
constexpr ITileID TILE_SPRING = MJMakeTile(EMJColor::Season, 0);

inline EMJColor MJTileColor(ITileID tile)
{
    return (EMJColor)(((tile) >> 8) & 0x0F);
}

inline int MJTilePoint(ITileID tile)
{
    return (((tile) >> 4) & 0x0F);
}

// ȡ��4λֵ
inline int MJTileFlag(ITileID tile)
{
    return (tile & 0x0F);
}

// notice: loosen check!!
inline bool MJIsValidTile(ITileID tile)
{
    return (tile > 0 && tile < TILE_INF);
}

// ����
inline bool MJIsSuitColor(EMJColor color)
{
    return (color >= EMJColor::_SuitFront && color <= EMJColor::_SuitBack);
}

inline bool MJIsSuitTile(ITileID tile)
{
    return MJIsValidTile(tile) && MJIsSuitColor(MJTileColor(tile));
}

inline bool MJIs258Tile(ITileID tile)
{
    return MJIsValidTile(tile) && MJIsSuitColor(MJTileColor(tile)) && (MJTilePoint(tile) % 3 == 1);
}

// ����
inline bool MJIsHonorColor(EMJColor color)
{
    return color == EMJColor::Wind || color == EMJColor::Dragon;
}

// �з���
inline bool MJIsDragonTile(ITileID tile)
{
    return MJTileColor(tile) == EMJColor::Dragon;
}

inline bool MJIsHonorTile(ITileID tile)
{
    return MJIsValidTile(tile) && MJIsHonorColor(MJTileColor(tile));
}

// ����
inline bool MJIsExtraColor(EMJColor color)
{
    return color == EMJColor::Season || color == EMJColor::Flower;
}

inline bool MJIsExtraTile(ITileID tile)
{
    return MJIsValidTile(tile) && MJIsExtraColor(MJTileColor(tile));
}

// ͬ��������step��(��ѭ��)
inline ITileID MJNextTileInSameColor(ITileID tile, int step = 1)
{
    if (!MJIsValidTile(tile))
    {
        return tile;
    }
    auto color = MJTileColor(tile);
    auto count = MJPointCountByColor.at((int)color);
    step %= count;
    auto point = (MJTilePoint(tile) + step + count) % count;
    return MJMakeTile(color, point);
}

using CallTileFan = std::map<ITileID /*call*/, int /*fan*/>;
using CallDataMap = std::map<ITileID /*discard*/, CallTileFan>;

//������С��
enum class EScoreType
{
    Natural,        //���޴����
    MinScore,       //������С��
    PositiveScore,  //������Ҵ���������ϵͳ֧��
    JustWin,        //ֻӮ����
};

//���ֺ�������
enum class EHandStyle
{
    None = -1,
    TianHu = 0,  // ׯ�����
    TianTing,    // ����
    YSYT,        // �м�һ��һ��
};

// �йܱ�ǣ�ǰ��ʹ���ض�ֵ�����޸���ע�⣩
enum class ETrustType
{
    Untrust = 0,       //����й�
    TimeOut = 2,       //��ʱ�й�
    FDTNetBreak = 5,   //�Խ�������
    FDTNetResume = 6,  //�Խ�������
};

// ��Ӧ CByte CTKGamePlayer::m_byUserTypeTag
enum class EPlayerType
{
    None,
    Newbie,  //����
    Unusual,
    Normal,    //����
    Needhelp,  //����
};

// ���߽׶Σ�������Ĺ�ͬ���߶��ɷ�Ϊ�������׶Σ����ڴ˹�����
// �����˱��齫���ֶ�Ʈ����ȱ��
enum class EDecisionStage
{
    Start = 1,   // ��ʼ���ߣ�֪ͨ�����淨
    Ack = 2,     // �յ�ĳ����ҵĽ����֪ͨ���淨
    Result = 3,  // ���߽�����֪ͨ���ս��
};

enum class EGameOverStatus
{
    Normal = 0,
    Timeout = 1,
    Exception = 2,
};

//������
enum class KonType
{
    None = -1,
    Zhi,
    An,
    Bu,
    BuDelay,  //��ֵ��������ڲ�ʹ��
};

//��¶����
enum class EGroupType
{
    None = 0,
    Chow = 1,    //��
    Pon = 2,     //��
    ZhiKon = 3,  //ֱ��
    AnKon = 4,   //����
    BuKon = 5,   //����
};

// ��Ϸ��ʹ��3210������˳�򣬶�AIʹ��0123��˳��
inline ISeatID MJSwitchAISeat(ISeatID seat)
{
    if (seat % 2 == 0)
    {
        return seat == 0 ? 2 : 0;
    }
    return seat;
}

inline std::vector<ITileID> operator+(std::vector<ITileID> const& left, std::vector<ITileID> const& right)
{
    auto r = left;
    r.insert(r.end(), right.begin(), right.end());
    return r;
}

inline std::vector<ITileID>& operator+=(std::vector<ITileID>& left, std::vector<ITileID> const& right)
{
    left.insert(left.end(), right.begin(), right.end());
    return left;
}

struct HuPlayData
{
    std::vector<ITileID> tilesInHand;
    std::vector<ITileID> tilesForChowLeft;
    std::vector<ITileID> tilesForPon;
    std::vector<ITileID> tilesForKon;

    ITileID paoTile{TILE_NULL};
    int countConcealedKon{};  //������

    bool isCall{false};

    bool canCall{false};  //����ר�ŵı����׶α���
    std::vector<ITileID> tilesLai;
    std::set<int> removeHuType;
    std::vector<int> extraHuTypes;  //������Ӻ�
    int extraInfo{0};               //������Ϣ
};

namespace MJHuExFan
{
using Type = int;
enum
{
    Custom = 0,  //����Ϸ���ж���
};
}  // namespace MJHuExFan
struct HuResult
{
    std::vector<int> huTypes;
    std::map<MJHuExFan::Type, int> extras{};  //������������
    int totalFan{};                           //�ܷ����ض���Ϸ��Ч��
    int64_t totalMultiple{};                  //�ܱ���

    int GetExtra(MJHuExFan::Type type) const
    {
        auto iter = extras.find(type);
        return (iter == extras.end() ? 0 : iter->second);
    }
};

// �����Ϸ״̬
namespace MJPlayerStatus
{
using Type = int;
enum
{
    Playing = 0,
    Hu = 1,
    Bankrupt = 2,
    CheckCall = 3,
    End = 4,
    CustomBegin = 5,
};
};  // namespace MJPlayerStatus

// �����ʵ���������
namespace MJScoreType
{
using Type = int;
enum
{
    Hu = 1,
    Jiao = 2,
    CustomBegin = 3,
};
};  // namespace MJScoreType

namespace MJTips
{
using Type = int;
enum
{
    None = -1,
    PassHu = 0,
    PassPon = 1,
    QiHuFan = 2,
    OnlyZiMo = 3,
    CustomBegin = 4,
};
};  // namespace MJTips

// ����ʱ��ҷ�����ǩ
namespace MJScoreResult
{
using Type = int;
enum
{
    None = 0,
    PoChan = 1,    //�Ʋ�
    FengDing = 2,  //�ⶥ
};
};  // namespace MJScoreResult