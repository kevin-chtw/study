#include "MJHu.h"
#include "MJDefine.h"
#include "MJHelper.h"

static const int SEQ_BEGIN_BY_COLOR[(size_t)EMJColor::_End] = {0, 9, 18, 27, 31, 34, 38, 42};
static const int MAX_TOTAL_TYPE = 42;

static constexpr int MAX_HAND_NUM = 14;
static constexpr int MAX_HAND_NUM_17 = 17;

std::shared_ptr<MJ::HuCore> MJ::HuCore::GetInstance()
{
    static std::shared_ptr<MJ::HuCore> self = std::make_shared<MJ::HuCore>(MAX_HAND_NUM);
    return self;
}

std::shared_ptr<MJ::HuCore> MJ::HuCore::GetInstance17()
{
    static std::shared_ptr<MJ::HuCore> self = std::make_shared<MJ::HuCore>(MAX_HAND_NUM_17);
    return self;
}

MJ::HuCore::HuCore(int maxHandCount)
{
    mMaxHandCount = maxHandCount;
    mMapHuAll.resize(maxHandCount + 1, {});
    mMapHuAllFZ.resize(maxHandCount + 1, {});
    this->_Initialize();
}

void MJ::HuCore::_Initialize()
{
    std::map<int, int> single;         //单个顺子+刻子  50个
    std::map<int, int> singleFZ;       //单个顺子+刻子  22个
    std::map<int, int> singleJiang;    //单个将   19个
    std::map<int, int> singleJiangFZ;  //单个将   15个

    _TrainSingleKe(single, singleFZ, singleJiang, singleJiangFZ);
    _TrainAllComb(single, mMapHuAll);
    _TrainAllCombJiang(singleJiang, mMapHuAll);
    _TrainAllComb(singleFZ, mMapHuAllFZ);
    _TrainAllCombJiang(singleJiangFZ, mMapHuAllFZ);

    uint8_t byMax = 0;
    int numAll = 0;
    for (auto& m : mMapHuAll)
    {
        numAll += m.size();
    }
    for (auto& m : mMapHuAllFZ)
    {
        numAll += m.size();
    }

    for (int i = 0; i < (mMaxHandCount + 1); ++i)
    {
        auto iter = mMapHuAll[i].begin();
        for (; iter != mMapHuAll[i].end(); ++iter)
        {
            uint8_t byMax = 0;
            int nArrayIndex = _GetArrayIndex(iter->first, byMax);
            if (byMax < 4)
            {
                mByArray[nArrayIndex] = iter->second;
            }
            else
            {
                mMapHu4All[iter->first] = iter->second;
            }
        }
        iter = mMapHuAllFZ[i].begin();
        for (; iter != mMapHuAllFZ[i].end(); ++iter)
        {
            uint8_t byMax = 0;
            int nArrayIndex = _GetArrayIndex(iter->first, byMax);
            if (byMax < 4)
            {
                mByArrayFZ[nArrayIndex] = iter->second;
            }
            else
            {
                mMapHu4AllFZ[iter->first] = iter->second;
            }
        }
    }
    m_Initialized = true;
}

bool MJ::HuCore::CheckBasicHu(std::vector<ITileID> const& cards, int countLaiZi)
{
    if (!m_Initialized)
    {
        return false;
    }

    std::vector<uint8_t> byTiles(MAX_TOTAL_TYPE, 0);
    for (auto card : cards)
    {
        int color = (int)MJTileColor(card);
        int point = MJTilePoint(card);
        int pos = SEQ_BEGIN_BY_COLOR[color] + point;
        if (++byTiles.at(pos) > 4)
        {
            return Hu::CheckAnyHu(cards, countLaiZi);
        }
    }
    //查表算平胡
    if (_CheckBasicHu(byTiles.data(), cards.size(), countLaiZi))
    {
        return true;
    }
    return false;
}

bool MJ::HuCore::_CheckBasicHu(uint8_t* byTiles, int cardsNum, int countLaiZi)
{
    int countShun = 0;
    uint8_t byJiangNum = 0;
    for (int cor = 0; cor <= (int)EMJColor::Wind; ++cor)
    {
        TileStyle* byArray = (cor == (int)EMJColor::Wind ? mByArrayFZ : mByArray);
        auto& hu4All = (cor == (int)EMJColor::Wind ? mMapHu4AllFZ : mMapHu4All);

        int nMax = (cor == (int)EMJColor::Wind) ? MAX_FENZI_NUM : MAX_VAL_NUM;
        int nVal = 0, nNum = 0;

        bool isArrayFlag = _ToNumVal(byTiles + (MAX_VAL_NUM * cor), nMax, nNum, nVal);

        if (nNum == 0)
        {
            continue;
        }

        //预判断
        if (isArrayFlag && !byArray[nVal].bEnable)
        {
            return false;
        }

        int nVal4 = _GetKeyByIndex(byTiles + MAX_VAL_NUM * cor, MAX_VAL_NUM);
        if (cor == (int)EMJColor::Wind)
        {
            nVal4 &= 0x1FFFFF;
        }

        uint8_t nNaiTry = 0xFF;
        if (isArrayFlag)
        {
            nNaiTry = byArray[nVal].nNaiZiCount;
            countShun += byArray[nVal].nShunCount;
        }
        else
        {
            auto iter = hu4All.find(nVal4);
            if (iter != hu4All.end())
            {
                nNaiTry = iter->second.nNaiZiCount;
                countShun += iter->second.nShunCount;
            }
        }

        if (nNaiTry == 0xFF)
        {
            return false;
        }
        byJiangNum += ((nNum + nNaiTry) % 3 == 2);

        countLaiZi -= nNaiTry;

        if (byJiangNum > countLaiZi + 1)
        {
            return false;
        }
    }

    return byJiangNum > 0 || countLaiZi >= 2;
}

bool MJ::HuCore::_ToNumVal(uint8_t* byTilesStart, int nMax, int& nNum, int& nVal)
{
    bool isArrayFlag = true;
    for (int i = 0; i < nMax; ++i)
    {
        auto byTemp = byTilesStart[i];
        nNum += byTemp;
        if (byTemp > 3)
        {
            isArrayFlag = false;
        }
        else
        {
            nVal |= (int)(byTemp) << (2 * i);
        }
    }
    return isArrayFlag;
}

int MJ::HuCore::_GetArrayIndex(uint8_t byIndex[], int byNum)
{
    int nKey = 0;
    for (int i = 0; i < byNum; ++i)
    {
        if ((byIndex[i] & BIT_VAL_FLAG) > 3)
            byIndex[i] -= 3;                          //为节约内存，最大支持3张
        nKey |= (int)(byIndex[i] & 0x03) << (2 * i);  //因为小于等于3，所以只需要2bit
    }
    return nKey;
}

int MJ::HuCore::_GetArrayIndex(int llVal, uint8_t& byMax)
{
    byMax = 0;
    uint8_t byIndex[MAX_VAL_NUM] = {};
    for (int i = 0; i < sizeof(byIndex); ++i)
    {
        byIndex[i] = (llVal >> (BIT_VAL_NUM * i)) & BIT_VAL_FLAG;
        byMax = std::max(byMax, byIndex[i]);
    }
    return _GetArrayIndex(byIndex, sizeof(byIndex));
}

bool MJ::HuCore::_IsValidKey(int llVal)
{
    uint8_t byIndex[MAX_KEY_NUM]{};
    for (int i = 0; i < sizeof(byIndex); ++i)
        byIndex[i] = (llVal >> (BIT_VAL_NUM * i)) & BIT_VAL_FLAG;

    if (byIndex[sizeof(byIndex) - 1] > MAX_NAI_NUM)
        return false;
    int nNum = 0;
    for (int i = 0; i < sizeof(byIndex); ++i)
    {
        nNum += byIndex[i];
        if (byIndex[i] > 4 || nNum > mMaxHandCount)  //
            return false;
    }
    return nNum > 0;
}

uint8_t MJ::HuCore::_GetNumByKey(int llVal, uint8_t byNum)
{
    uint8_t byIndex[MAX_KEY_NUM]{};
    for (int i = 0; i < sizeof(byIndex); ++i)
        byIndex[i] = (llVal >> (BIT_VAL_NUM * i)) & BIT_VAL_FLAG;

    uint8_t nNum = 0;
    for (int i = 0; i < byNum; ++i)
        nNum += byIndex[i];
    return nNum;
}

void MJ::HuCore::_AddMap(std::vector<std::unordered_map<int, TileStyle>>& out__ mapTemp, int llVal, int nShun)
{
    uint8_t nNum = _GetNumByKey(llVal, MAX_VAL_NUM);
    uint8_t byNum = (llVal >> (BIT_VAL_NUM * 9)) & BIT_VAL_FLAG;
    int val = (llVal & 0x7FFFFFF);
    std::unordered_map<int, TileStyle>::iterator iter = mapTemp[nNum].find(val);
    if (iter != mapTemp[nNum].end())
    {
        //优先对对胡
        if (byNum < iter->second.nNaiZiCount || (byNum == iter->second.nNaiZiCount && iter->second.nShunCount > nShun))
        {
            iter->second.nNaiZiCount = byNum;
            iter->second.nShunCount = nShun;
        }
    }
    else
    {
        mapTemp[nNum][val].nNaiZiCount = byNum;
        mapTemp[nNum][val].nShunCount = nShun;
        mapTemp[nNum][val].bEnable = true;
    }
}

void MJ::HuCore::_TrainSingleKe(std::map<int, int>& single, std::map<int, int>& singleFZ, std::map<int, int>& singleJiang, std::map<int, int>& singleJiangFZ)
{
    uint8_t byTempKe[MAX_KEY_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 3};
    single[_GetKeyByIndex(byTempKe)] = 0;  //刻优先，顺子计1，刻计0
    singleFZ[_GetKeyByIndex(byTempKe)] = 0;
    // 1.1 刻子
    for (int i = 0; i < MAX_VAL_NUM; ++i)
    {
        memset(byTempKe, 0, MAX_KEY_NUM);
        for (int n = 0; n < 3; ++n)
        {
            byTempKe[i] = 3 - n;
            byTempKe[9] = n;
            single[_GetKeyByIndex(byTempKe)] = 0;
            if (i < 7)  //风、字牌最多7张
                singleFZ[_GetKeyByIndex(byTempKe)] = 0;
        }
    }
    // 1.2 顺子 没赖子
    for (int i = 0; i < MAX_VAL_NUM - 2; ++i)
    {
        memset(byTempKe, 0, MAX_KEY_NUM);
        byTempKe[i] = 1;
        byTempKe[i + 1] = 1;
        byTempKe[i + 2] = 1;
        single[_GetKeyByIndex(byTempKe)] = 1;
    }
    // 1.3 顺子 1个赖子 (2个赖子时也就是刻子)
    for (int i = 0; i < MAX_VAL_NUM - 2; ++i)
    {
        for (int n = 0; n < 3; ++n)
        {
            memset(byTempKe, 0, MAX_KEY_NUM);
            byTempKe[i] = 1;
            byTempKe[i + 1] = 1;
            byTempKe[i + 2] = 1;
            byTempKe[i + n] = 0;
            byTempKe[9] = 1;
            single[_GetKeyByIndex(byTempKe)] = 1;
        }
    }
    // 2.1 将牌
    memset(byTempKe, 0, MAX_KEY_NUM);
    byTempKe[9] = 2;
    singleJiang[_GetKeyByIndex(byTempKe)] = 0;
    singleJiangFZ[_GetKeyByIndex(byTempKe)] = 0;
    for (int i = 0; i < MAX_VAL_NUM; ++i)
    {
        memset(byTempKe, 0, MAX_KEY_NUM);
        for (int n = 0; n < 2; ++n)
        {
            byTempKe[i] = 2 - n;
            byTempKe[9] = n;
            singleJiang[_GetKeyByIndex(byTempKe)] = 0;
            if (i < 7)  //风、字牌最多7张
                singleJiangFZ[_GetKeyByIndex(byTempKe)] = 0;
        }
    }
}

void MJ::HuCore::_TrainAllComb(const std::map<int, int>& single, std::vector<std::unordered_map<int, TileStyle>>& out__ mapOut)
{
    for (auto i1 = single.begin(); i1 != single.end(); ++i1)
    {
        _AddMap(mapOut, i1->first, i1->second);
        for (auto i2 = i1; i2 != single.end(); ++i2)
        {
            int key = i1->first + i2->first;
            if (!_IsValidKey(key))
                continue;
            _AddMap(mapOut, key, i1->second + i2->second);
            for (auto i3 = i2; i3 != single.end(); ++i3)
            {
                int key = i1->first + i2->first + i3->first;
                if (!_IsValidKey(key))
                    continue;
                _AddMap(mapOut, key, i1->second + i2->second + i3->second);
                for (auto i4 = i3; i4 != single.end(); ++i4)
                {
                    int key = i1->first + i2->first + i3->first + i4->first;
                    if (!_IsValidKey(key))
                        continue;
                    _AddMap(mapOut, key, i1->second + i2->second + i3->second + i4->second);
                    for (auto i5 = i4; i5 != single.end(); ++i5)
                    {
                        int key = i1->first + i2->first + i3->first + i4->first + i5->first;
                        if (!_IsValidKey(key))
                            continue;
                        _AddMap(mapOut, key, i1->second + i2->second + i3->second + i4->second + i5->second);
                    }
                }
            }
        }
    }
}

void MJ::HuCore::_TrainAllCombJiang(const std::map<int, int>& setSingle, std::vector<std::unordered_map<int, TileStyle>>& out__ mapOut)
{
    auto mapTemp = mapOut;  // copy

    for (auto iter = setSingle.begin(); iter != setSingle.end(); ++iter)
    {
        _AddMap(mapOut, iter->first, iter->second);
        for (int j = 0; j < (mMaxHandCount + 1); ++j)
        {
            //加上之前的顺、刻
            for (auto iterTemp = mapTemp[j].begin(); iterTemp != mapTemp[j].end(); ++iterTemp)
            {
                int key = iter->first + iterTemp->first + (int(iterTemp->second.nNaiZiCount & BIT_VAL_FLAG) << 27);
                if (_IsValidKey(key))
                    _AddMap(mapOut, key, iter->second + iterTemp->second.nShunCount);
            }
        }
    }
}

/// MJ::Hu ///

bool MJ::Hu::CheckBasicHu(std::vector<ITileID> const& cards, int countLaiZi, bool* allPon, bool* allShun)
{
    auto cardsMap = TilesToMap(cards);
    // 碰碰胡
    if (allPon && _CheckPonPonHu(cardsMap, countLaiZi))
    {
        *allPon = true;
        return true;
    }
    if (allShun && _CheckStraightHu(cardsMap, countLaiZi))
    {
        *allShun = true;
        return true;
    }
    if (_CheckAnyHu(cardsMap, countLaiZi))
    {
        return true;
    }
    return false;
}

bool MJ::Hu::CheckPonPonHu(std::vector<ITileID> const& cards, int countLaiZi)
{
    return _CheckPonPonHu(TilesToMap(cards), countLaiZi);
}

bool MJ::Hu::_CheckPonPonHu(std::map<ITileID, int> cards, int countLaiZi)
{
    bool hasJiang = false;
    int needLaiZi = 0;
    for (auto p : cards)
    {
        int rest = (p.second % 3);
        if (rest == 0)
        {
            continue;
        }
        if (hasJiang)
        {
            needLaiZi += 3 - rest;
        }
        else
        {
            hasJiang = true;
            needLaiZi += 2 - rest;
        }
    }
    if (!hasJiang)
    {
        needLaiZi += 2;
    }
    return (needLaiZi <= countLaiZi);
}

bool MJ::Hu::CheckStraightHu(std::vector<ITileID> const& cards, int countLaiZi)
{
    return _CheckStraightHu(TilesToMap(cards), countLaiZi);
}

bool MJ::Hu::_CheckStraightHu(std::map<ITileID, int> cards, int countLaiZi)
{
    return _TestJiang(cards, countLaiZi, &_CheckAllStraight);
}

bool MJ::Hu::CheckAnyHu(std::vector<ITileID> const& cards, int countLaiZi)
{
    return _CheckAnyHu(TilesToMap(cards), countLaiZi);
}

bool MJ::Hu::_CheckAnyHu(std::map<ITileID, int> cards, int countLaiZi)
{
    return _TestJiang(cards, countLaiZi, &Hu::_CheckAllMeld);
}

bool MJ::Hu::CheckAllTriplet(std::vector<ITileID> const& cards, int countLaiZi)
{
    return _CheckAllTriplet(TilesToMap(cards), countLaiZi);
}

bool MJ::Hu::_CheckAllTriplet(std::map<ITileID, int> cards, int countLaiZi)
{
    int needLaiZi = 0;
    for (auto& p : cards)
    {
        auto n = (p.second % 3);
        needLaiZi += (n == 0 ? 0 : (3 - n));
    }
    return (needLaiZi <= countLaiZi);
}

bool MJ::Hu::CheckAllStraight(std::vector<ITileID> const& cards, int countLaiZi)
{
    return _CheckAllStraight(TilesToMap(cards), countLaiZi);
}

inline int __CountTile(std::map<ITileID, int> const& cards, ITileID tile)
{
    auto iter = cards.find(tile);
    if (iter == cards.end() || iter->second <= 0)
    {
        return 0;
    }
    return iter->second;
}

bool MJ::Hu::_CheckAllStraight(std::map<ITileID, int> cards, int countLaiZi)
{
    for (auto& p : cards)
    {
        if (!MJIsSuitTile(p.first))
        {
            return false;
        }
        if (p.second <= 0)
        {
            continue;
        }
        auto count = p.second;
        auto tile1 = p.first + TILE_ID_STEP;
        auto tile2 = p.first + TILE_ID_STEP * 2;
        auto count1 = ::__CountTile(cards, tile1);
        auto count2 = ::__CountTile(cards, tile2);
        int needLai = (count1 < count ? count - count1 : 0) + (count2 < count ? count - count2 : 0);
        if (needLai > countLaiZi)
        {
            return false;
        }
        countLaiZi -= needLai;
        if (count1)
        {
            cards[tile1] -= std::min(count1, count);
        }
        if (count2)
        {
            cards[tile2] -= std::min(count2, count);
        }
    }
    return true;
}

bool MJ::Hu::CheckAllMeld(std::vector<ITileID> const& cards, int countLaiZi)
{
    return _CheckAllMeld(TilesToMap(cards), countLaiZi);
}

bool MJ::Hu::_CheckAllMeld(std::map<ITileID, int> cards, int countLaiZi)
{
    int needLaiZi = 0;
    for (auto& p : cards)
    {
        if (MJIsHonorTile(p.first))
        {
            auto n = (p.second % 3);
            needLaiZi += (n == 0 ? 0 : (3 - n));
            continue;
        }
        if (p.second >= 3)
        {
            p.second -= 3;
        }

        if (p.second > 0)
        {
            auto next1 = cards.find(p.first + TILE_ID_STEP);
            auto next2 = cards.find(p.first + TILE_ID_STEP * 2);
            int x = _GetFollowCode(p.second, next1, next2, cards.end());
            needLaiZi += _HandleMeld(x, next1, next2, cards.end());
            p.second = 0;
        }

        if (needLaiZi > countLaiZi)
        {
            break;
        }
    }
    return (needLaiZi <= countLaiZi);
}

bool MJ::Hu::_TestJiang(std::map<ITileID, int> cards, int countLaiZi, std::function<bool(std::map<ITileID, int> cards, int countLaiZi)> checker)
{
    for (auto j : cards)
    {
        // select jiang.
        auto tempMap = cards;
        int needLaiZi = 0;
        if (j.second > 2)
        {
            tempMap[j.first] -= 2;
        }
        else if (j.second == 2)
        {
            tempMap.erase(j.first);
        }
        else if (j.second == 1)
        {
            tempMap.erase(j.first);
            needLaiZi = 1;
        }
        else
        {
            continue;
        }

        if (countLaiZi - needLaiZi >= 0 && checker(std::move(tempMap), countLaiZi - needLaiZi))
        {
            return true;
        }
    }
    return false;
}

int MJ::Hu::_GetFollowCode(int curCount, Iter& next1, Iter& next2, Iter end)
{
    int x = curCount * 100;
    if (next1 != end)
    {
        if (curCount <= next1->second)
        {
            x += curCount * 10;
        }
        else
        {
            x += next1->second * 10;
        }
    }

    if (next2 != end)
    {
        if (curCount <= next2->second)
        {
            x += curCount;
        }
        else
        {
            x += next2->second;
        }
    }
    return x;
}

int MJ::Hu::_HandleMeld(int x, Iter& next1, Iter& next2, Iter end)
{
    struct Item
    {
        int laiZi = 0;
        int use1 = 0;
        int use2 = 0;
    };
    std::map<int, Item> datas = {
        {100, {2, 0, 0}},
        {101, {1, 0, 1}},
        {110, {1, 1, 0}},
        {111, {0, 1, 1}},
        {200, {1, 0, 0}},
        {201, {1, 0, 0}},
        {210, {1, 0, 0}},
        {211, {1, 0, 0}},
        {202, {1, 0, 0}},
        {220, {1, 0, 0}},
        {221, {1, 2, 1}},
        {212, {1, 1, 2}},
        {222, {0, 2, 2}},
    };  // .....
    auto& p = datas[x];
    if (p.use1 && next1 != end)
    {
        next1->second -= p.use1;
    }
    if (p.use2 && next2 != end)
    {
        next2->second -= p.use2;
    }
    return p.laiZi;
}

std::map<ITileID, int> MJ::Hu::TilesToMap(std::vector<ITileID> const& cards)
{
    std::map<ITileID, int> cardsMap;
    for (auto card : cards)
    {
        cardsMap[card]++;
    }
    return cardsMap;
}

bool MJ::Hu::CheckQiDui(std::vector<ITileID> const& cards, int countLaiZi, int* const pLuxury)
{
    if (cards.size() + countLaiZi != 14)
    {
        return false;
    }
    return Hu::CheckAllPairs(cards, countLaiZi, pLuxury);
}

bool MJ::Hu::CheckAllPairs(std::vector<ITileID> const& cards, int countLaiZi, int* const pLuxury)
{
    std::map<ITileID, int> counts;
    for (auto card : cards)
    {
        ++counts[card];
    }
    int luxury = 0;
    int need = 0;
    for (auto pair : counts)
    {
        if (pair.second % 2)
        {
            ++need;
        }
        if (pair.second > 2)
        {
            ++luxury;
        }
    }
    if (need > countLaiZi)
    {
        return false;
    }
    if (pLuxury)
    {
        *pLuxury = luxury + (countLaiZi - need) / 2;
    }
    return true;
}

void MJ::DiscardRecommender::SetIgnoredTiles(std::set<ITileID> const& ignoredTiles)
{
    m_Ignored = ignoredTiles;
}

ITileID MJ::DiscardRecommender::Recommend(std::vector<ITileID> const& handTiles, std::vector<ITileID> const& keep) const
{
    // values: 卡搭A-?-C +5, 连搭A-B +10, 对AA +15, 刻AAA + 20, 杠AAAA + 30, 单+碰A(AAA) +25
    static constexpr int ValueForSames[5] = {0, 0, 20, 30, 40};
    static constexpr int ValueForLinks[3] = {0, 10, 8};
    static constexpr int ValueForKon = 25;

    std::map<ITileID, int> tilesCount;
    std::map<ITileID, int> tilesValue;
    for (auto tile : handTiles)
    {
        if (m_Ignored.find(tile) == m_Ignored.end())
        {
            ++tilesCount[tile];
        }
    }
    for (auto& p : tilesCount)
    {
        tilesValue[p.first] = ValueForSames[p.second];

        if (MJ::HasElement(keep, p.first))
        {
            tilesValue[p.first] += 25;
        }
        if (!MJIsSuitTile(p.first))
        {
            continue;
        }
        int diffs[4] = {-2, -1, 1, 2};
        for (int diff : diffs)
        {
            ITileID tile = p.first + TILE_ID_STEP * diff;
            if (tilesCount.find(tile) != tilesCount.end())
            {
                tilesValue[p.first] += ValueForLinks[std::abs(diff)];
            }
        }
    }
    int minValue = 100;
    ITileID target = TILE_NULL;
    for (auto& p : tilesValue)
    {
        if (p.second < minValue || (p.second == minValue && MJ::Random(2) == 0))
        {  // 相同值，取随机
            target = p.first;
            minValue = p.second;
        }
    }
    return target;
}
