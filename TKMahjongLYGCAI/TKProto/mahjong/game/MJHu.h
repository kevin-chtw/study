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
/// Hu ֱ����������ڲ��߶ԡ���������ƽ�����Լ�������ϣ�֧������������ӣ�
/// </summary>
class Hu
{
public:
    static std::map<ITileID, int> TilesToMap(std::vector<ITileID> const& cards);
    /// <summary>
    /// �����7��
    /// </summary>
    /// <param name="cards">�������������</param>
    /// <param name="countLaiZi">������</param>
    /// <returns>�Ƿ���߶�</returns>
    static bool CheckQiDui(std::vector<ITileID> const& cards, int countLaiZi, int* const pLuxury);

    /// <summary>
    /// �Ƿ��ȫ���ɶ�
    /// </summary>
    /// <param name="cards">�������������</param>
    /// <param name="countLaiZi">������</param>
    /// <param name="pLuxury">(���ָ�룬�ɿ�)���4����ͬ������</param>
    /// <returns></returns>
    static bool CheckAllPairs(std::vector<ITileID> const& cards, int countLaiZi, int* const pLuxury);

    /// <summary>
    /// �����ֱ�Ӽ��㣬֧������������4��
    /// </summary>
    /// <param name="cards"></param>
    /// <param name="countLaiZi"></param>
    /// <param name="hasShun">������ڷֱ�ƽ����������</param>
    /// <returns></returns>
    static bool CheckBasicHu(std::vector<ITileID> const& cards, int countLaiZi, bool* allPon = nullptr, bool* allShun = nullptr);
    //ȫ���Ӻ�
    static bool CheckPonPonHu(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckPonPonHu(std::map<ITileID, int> cards, int countLaiZi);
    //ȫ˳�Ӻ������������ܣ�
    static bool CheckStraightHu(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckStraightHu(std::map<ITileID, int> cards, int countLaiZi);
    //�����
    static bool CheckAnyHu(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckAnyHu(std::map<ITileID, int> cards, int countLaiZi);

    /// <summary>
    /// �Ƿ��ȫ������
    /// </summary>
    /// <param name="cards"></param>
    /// <param name="countLaiZi"></param>
    /// <returns></returns>
    static bool CheckAllTriplet(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckAllTriplet(std::map<ITileID, int> cards, int countLaiZi);

    /// <summary>
    /// �Ƿ�ȫ��˳
    /// </summary>
    /// <param name="cards"></param>
    /// <param name="countLaiZi"></param>
    /// <returns></returns>
    static bool CheckAllStraight(std::vector<ITileID> const& cards, int countLaiZi);
    static bool _CheckAllStraight(std::map<ITileID, int> cards, int countLaiZi);

    /// <summary>
    /// �Ƿ��ȫ��������˳���̣�
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
    /// �õ�ǰ�������뵱ǰ�����ٺ������Ƶ�map�����������Ϊһ����λֵ�����ں�������
    ///     �磺1��2�ţ���2��(next1 == end)��3��3��(next2->second == 3). �򷵻�202
    ///   ������203��curCount������2������λȡֵ������curCount��
    /// </summary>
    /// <param name="curCount">count of card=1,2</param>
    /// <param name="next1">count-iter of card+1</param>
    /// <param name="next2">count-iter of card+2</param>
    /// <param name="end">the end-iter of cards map</param>
    /// <returns>a hundred num by 3 cards count.</returns>
    static int _GetFollowCode(int curCount, Iter& next1, Iter& next2, Iter end);

    /// <summary>
    /// ����ǰ��ȫ����ϣ�����Ҫ��������
    ///     �����˳�ӣ�next1��next2ָ���ֵ�������޸�
    /// </summary>
    /// <param name="followCode">_GetFollowCode�ķ���ֵ</param>
    /// <param name="next1">count-iter of card+1</param>
    /// <param name="next2">count-iter of card+2</param>
    /// <param name="end">the end-iter of cards map</param>
    /// <returns>��Ҫ��������</returns>
    static int _HandleMeld(int followCode, Iter& next1, Iter& next2, Iter end);
};

/// ������
class HuCore
{
public:
    static std::shared_ptr<MJ::HuCore> GetInstance();
    static std::shared_ptr<MJ::HuCore> GetInstance17();

    HuCore(int maxHandCount);

    /// <summary>
    /// �������+N*��
    /// </summary>
    /// <param name="cards">�������������</param>
    /// <param name="countLaiZi">������</param>
    /// <param name="hasDiffer">Ke����ʱ����ʶ�Ƿ���˳��˳����ʱ����ʶ�Ƿ��п�</param>
    /// <returns>�Ƿ��</returns>
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
    static constexpr int MAX_KEY_NUM = (MAX_VAL_NUM + 1);  //9+����
    static constexpr int MAX_ONE_CARD_NUM = 4;             //ͬһ���Ƶ���
    static constexpr int MAX_NAI_NUM = 4;                  //����������
    static constexpr int BIT_VAL_NUM = 3;                  //һ��ֵռ��bit��
    static constexpr int BIT_VAL_FLAG = 0x07;              //
    static constexpr int MAX_FENZI_NUM = 7;                //�硢������

    TileStyle mByArray[1 << (MAX_VAL_NUM * 2)];
    TileStyle mByArrayFZ[1 << (MAX_VAL_NUM * 2)];
    std::unordered_map<int, TileStyle> mMapHu4All;  //���鲻��֧�ֵĲ���map(������4����ͬ����)
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

//�Ƽ�����
class DiscardRecommender
{
public:
    void SetIgnoredTiles(std::set<ITileID> const& ignoredTiles);

    ITileID Recommend(std::vector<ITileID> const& handTiles, std::vector<ITileID> const& pengTiles) const;

private:
    std::set<ITileID> m_Ignored;
};

}  // namespace MJ
