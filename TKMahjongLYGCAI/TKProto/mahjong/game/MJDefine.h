/*
	摘要: 服务器内部一些基础的配置宏
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

// 测试模式
#if defined(_DEBUG) && !defined(NDEBUG)
#define MJ_DEBUG 1
#else
#define MJ_DEBUG 0
#endif

#define out__     //Tip: 注意这个变量会在调用中被修改
#define in_out__  //Tip: 注意这个变最应传入有效值，且会被修改

using ITileID = int;
using ISeatID = int;

constexpr ITileID TILE_NULL = (0);   //前端对0牌显示空牌面，无效的牌
constexpr ITileID TILE_BACK = (-1);  //前端用-1表示牌的的背面，即有效但自己不可见的牌
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

// 行牌下家, 按 3,2,1,0的顺序
inline ISeatID MJGetNextSeat(ISeatID seat, int step = 1, int SeatCount = NP4)
{
    return (seat + (SeatCount - step % SeatCount)) % SeatCount;
}

enum class EMJColor
{
    Undefined = -1,
    Character = 0,  //万
    Bamboo = 1,     // 条
    Dot = 2,        // 筒
    Wind = 3,       // 风,东南西北
    Dragon = 4,     // 字,中发白
    Flower = 5,     // 梅兰竹菊
    Season = 6,     // 春夏秋冬
    Hun = 7,        // 懒子（wild）
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
constexpr ITileID TILE_INF = MJMakeTile(EMJColor::_End, 0);  //无效

//东南西北中发白
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

// 取后4位值
inline int MJTileFlag(ITileID tile)
{
    return (tile & 0x0F);
}

// notice: loosen check!!
inline bool MJIsValidTile(ITileID tile)
{
    return (tile > 0 && tile < TILE_INF);
}

// 套牌
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

// 字牌
inline bool MJIsHonorColor(EMJColor color)
{
    return color == EMJColor::Wind || color == EMJColor::Dragon;
}

// 中发白
inline bool MJIsDragonTile(ITileID tile)
{
    return MJTileColor(tile) == EMJColor::Dragon;
}

inline bool MJIsHonorTile(ITileID tile)
{
    return MJIsValidTile(tile) && MJIsHonorColor(MJTileColor(tile));
}

// 花牌
inline bool MJIsExtraColor(EMJColor color)
{
    return color == EMJColor::Season || color == EMJColor::Flower;
}

inline bool MJIsExtraTile(ITileID tile)
{
    return MJIsValidTile(tile) && MJIsExtraColor(MJTileColor(tile));
}

// 同组牌向后第step张(内循环)
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

//积分最小化
enum class EScoreType
{
    Natural,        //不限带入分
    MinScore,       //积分最小化
    PositiveScore,  //超出玩家带入的输分由系统支出
    JustWin,        //只赢不输
};

//起手好牌类型
enum class EHandStyle
{
    None = -1,
    TianHu = 0,  // 庄家天胡
    TianTing,    // 天听
    YSYT,        // 闲家一上一听
};

// 托管标记，前端使用特定值（若修改请注意）
enum class ETrustType
{
    Untrust = 0,       //解除托管
    TimeOut = 2,       //超时托管
    FDTNetBreak = 5,   //自建桌断线
    FDTNetResume = 6,  //自建桌重连
};

// 对应 CByte CTKGamePlayer::m_byUserTypeTag
enum class EPlayerType
{
    None,
    Newbie,  //新手
    Unusual,
    Normal,    //正常
    Needhelp,  //帮助
};

// 决策阶段，大多数的共同决策都可分为这三个阶段，放在此公共区
// 例：宜宾麻将开局定飘、定缺等
enum class EDecisionStage
{
    Start = 1,   // 开始决策，通知所有玩法
    Ack = 2,     // 收到某个玩家的结果，通知该玩法
    Result = 3,  // 决策结束，通知最终结果
};

enum class EGameOverStatus
{
    Normal = 0,
    Timeout = 1,
    Exception = 2,
};

//杠类型
enum class KonType
{
    None = -1,
    Zhi,
    An,
    Bu,
    BuDelay,  //此值仅服务端内部使用
};

//附露类型
enum class EGroupType
{
    None = 0,
    Chow = 1,    //吃
    Pon = 2,     //碰
    ZhiKon = 3,  //直杠
    AnKon = 4,   //暗杠
    BuKon = 5,   //补杠
};

// 游戏服使用3210的行牌顺序，而AI使用0123的顺序
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
    int countConcealedKon{};  //暗杠数

    bool isCall{false};

    bool canCall{false};  //用于专门的报听阶段报听
    std::vector<ITileID> tilesLai;
    std::set<int> removeHuType;
    std::vector<int> extraHuTypes;  //额外添加胡
    int extraInfo{0};               //额外信息
};

namespace MJHuExFan
{
using Type = int;
enum
{
    Custom = 0,  //各游戏自行定义
};
}  // namespace MJHuExFan
struct HuResult
{
    std::vector<int> huTypes;
    std::map<MJHuExFan::Type, int> extras{};  //其它附加数据
    int totalFan{};                           //总番（特定游戏生效）
    int64_t totalMultiple{};                  //总倍数

    int GetExtra(MJHuExFan::Type type) const
    {
        auto iter = extras.find(type);
        return (iter == extras.end() ? 0 : iter->second);
    }
};

// 玩家游戏状态
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

// 结算帐单分数类型
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

// 结算时玩家分数标签
namespace MJScoreResult
{
using Type = int;
enum
{
    None = 0,
    PoChan = 1,    //破产
    FengDing = 2,  //封顶
};
};  // namespace MJScoreResult