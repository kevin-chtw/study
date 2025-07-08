#pragma once

#include <map>
#include <cstring>
#include <stdint.h>
#include <unordered_map>
#include "MJDefine.h"

namespace MJ
{

struct TileStyle
{
    uint8_t nShunCount{};
    uint8_t nNaiZiCount{};
    bool bEnable{};
};

/// <summary>
/// Hu 直接算胡，用于查七对、碰碰胡、平胡，以及部分组合（支持任意多张赖子）
/// </summary>
class Hu
{
public:
    static std::map<ITileID, int> TilesToMap(std::vector<ITileID> const& cards);
    /// <summary>
    /// 查胡，7对
    /// </summary>
    /// <param name="cards">除赖子外的牌张</param>
    /// <param name="countLaiZi">赖子数</param>
    /// <returns>是否胡七对</returns>
    static bool CheckQiDui(std::vector<ITileID> const& cards, int countLaiZi, int* const pLuxury);

    /// <summary>
    /// 是否可全部成对
    /// </summary>
    /// <param name="cards">除赖子外的牌张</param>
    /// <param name="countLaiZi">赖子数</param>
    /// <param name="pLuxury">(输出指针，可空)组成4张相同牌组数</param>
    /// <returns></returns>
    static bool CheckAllPairs(std::vector<ITileID> const& cards, int countLaiZi, int* const pLuxury);

    /// <summary>
    /// 查胡（直接计算，支持赖子数大于4）
    /// </summary>
    /// <param name="cards"></param>
    /// <param name="countLaiZi"></param>
    /// <param name="hasShun">输出用于分辨平胡、碰碰胡</param>
    /// <returns></returns>
    static bool CheckBasicHu(std::vector<ITileID> const& cards, int countLaiZi, bool* allPon = nullptr, bool* allShun = nullptr);
    //全刻子胡
    static bool CheckPonPonHu(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckPonPonHu(std::map<ITileID, int> cards, int countLaiZi);
    //全顺子胡（不考虑碰杠）
    static bool CheckStraightHu(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckStraightHu(std::map<ITileID, int> cards, int countLaiZi);
    //任意胡
    static bool CheckAnyHu(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckAnyHu(std::map<ITileID, int> cards, int countLaiZi);

    /// <summary>
    /// 是否可全成三刻
    /// </summary>
    /// <param name="cards"></param>
    /// <param name="countLaiZi"></param>
    /// <returns></returns>
    static bool CheckAllTriplet(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckAllTriplet(std::map<ITileID, int> cards, int countLaiZi);

    /// <summary>
    /// 是否全成顺
    /// </summary>
    /// <param name="cards"></param>
    /// <param name="countLaiZi"></param>
    /// <returns></returns>
    static bool CheckAllStraight(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckAllStraight(std::map<ITileID, int> cards, int countLaiZi);

    /// <summary>
    /// 是否可全成三坎（顺、刻）
    /// </summary>
    /// <param name="cards"></param>
    /// <param name="countLaiZi"></param>
    /// <returns></returns>
    static bool CheckAllMeld(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckAllMeld(std::map<ITileID, int> cards, int countLaiZi);

private:
    static bool _TestJiang(std::map<ITileID, int> cards, int countLaiZi, std::function<bool(std::map<ITileID, int> cards, int countLaiZi)> checker);

    // for CheckAllMeld
    using Iter = std::map<ITileID, int>::iterator;

    /// <summary>
    /// 用当前牌数量与当前牌相临后两张牌的map迭代量，组合为一个百位值，用于后续处理
    ///     如：1万2张，无2万(next1 == end)，3万3张(next2->second == 3). 则返回202
    ///   （不是203，curCount不大于2，后两位取值不大于curCount）
    /// </summary>
    /// <param name="curCount">count of card=1,2</param>
    /// <param name="next1">count-iter of card+1</param>
    /// <param name="next2">count-iter of card+2</param>
    /// <param name="end">the end-iter of cards map</param>
    /// <returns>a hundred num by 3 cards count.</returns>
    static int _GetFollowCode(int curCount, Iter& next1, Iter& next2, Iter end);

    /// <summary>
    /// 将当前牌全部组合，所需要的赖子数
    ///     若组成顺子，next1和next2指向的值，将被修改
    /// </summary>
    /// <param name="followCode">_GetFollowCode的返回值</param>
    /// <param name="next1">count-iter of card+1</param>
    /// <param name="next2">count-iter of card+2</param>
    /// <param name="end">the end-iter of cards map</param>
    /// <returns>需要的赖子数</returns>
    static int _HandleMeld(int followCode, Iter& next1, Iter& next2, Iter end);
};

/// 查表算胡
class HuCore
{
public:
    static std::shared_ptr<MJ::HuCore> GetInstance();
    static std::shared_ptr<MJ::HuCore> GetInstance17();

    HuCore(int maxHandCount);

    /// <summary>
    /// 查胡，将+N*坎
    /// </summary>
    /// <param name="cards">除赖子外的牌张</param>
    /// <param name="countLaiZi">赖子数</param>
    /// <param name="hasDiffer">Ke优先时，标识是否有顺；顺优先时，标识是否有刻</param>
    /// <returns>是否胡</returns>
    bool CheckBasicHu(std::vector<ITileID> const& cards, int countLaiZi);

private:
    bool _CheckBasicHu(uint8_t* byTiles, int cardsNum, int countLaiZi);
    bool _ToNumVal(uint8_t* byTilesStart, int nMax, int& nNum, int& nVal);

    int _GetArrayIndex(uint8_t byIndex[], int byNum);
    int _GetArrayIndex(int llVal, uint8_t& byMax);
    bool _IsValidKey(int llVal);
    uint8_t _GetNumByKey(int llVal, uint8_t byNum = MAX_KEY_NUM);
    void _AddMap(std::vector<std::unordered_map<int, TileStyle>>& out__ mapTemp, int llVal, int nShun);

    void _TrainSingleKe(std::map<int, int>& single, std::map<int, int>& singleFZ, std::map<int, int>& singleJiang, std::map<int, int>& singleJiangFZ);
    void _TrainAllComb(const std::map<int, int>& single, std::vector<std::unordered_map<int, TileStyle>>& out__ mapOut);
    void _TrainAllCombJiang(const std::map<int, int>& setSingle, std::vector<std::unordered_map<int, TileStyle>>& out__ mapOut);

private:
    void _Initialize();

    static constexpr int MAX_VAL_NUM = 9;
    static constexpr int MAX_KEY_NUM = (MAX_VAL_NUM + 1);  //9+赖子
    static constexpr int MAX_ONE_CARD_NUM = 4;             //同一张牌的数
    static constexpr int MAX_NAI_NUM = 4;                  //赖子最大个数
    static constexpr int BIT_VAL_NUM = 3;                  //一个值占的bit数
    static constexpr int BIT_VAL_FLAG = 0x07;              //
    static constexpr int MAX_FENZI_NUM = 7;                //风、字数量

    TileStyle mByArray[1 << (MAX_VAL_NUM * 2)];
    TileStyle mByArrayFZ[1 << (MAX_VAL_NUM * 2)];
    std::unordered_map<int, TileStyle> mMapHu4All;  //数组不能支持的才用map(即存在4张相同的牌)
    std::unordered_map<int, TileStyle> mMapHu4AllFZ;

    std::vector<std::unordered_map<int, TileStyle>> mMapHuAll;
    std::vector<std::unordered_map<int, TileStyle>> mMapHuAllFZ;

    int mMaxHandCount{};

private:
    template <std::size_t N>
    inline int _GetKeyByIndex(const uint8_t (&byIndex)[N])
    {
        return _GetKeyByIndex(&byIndex[0], N);
    }

    inline int _GetKeyByIndex(const uint8_t* byIndex, uint32_t byNum)
    {
        int nKey = 0;
        for (uint32_t i = 0; i < byNum; ++i)
            nKey |= (int)(byIndex[i] & BIT_VAL_FLAG) << (BIT_VAL_NUM * i);
        return nKey;
    }

private:
    bool m_Initialized{};
};

//推荐出牌
class DiscardRecommender
{
public:
    void SetIgnoredTiles(std::set<ITileID> const& ignoredTiles);

    ITileID Recommend(std::vector<ITileID> const& handTiles, std::vector<ITileID> const& pengTiles) const;

private:
    std::set<ITileID> m_Ignored;
};

}  // namespace MJ
