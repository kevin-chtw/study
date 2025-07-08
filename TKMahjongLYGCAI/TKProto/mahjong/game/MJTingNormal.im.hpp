#pragma once
#include "MJTingNormal.h"
#include "MJHelper.h"
#include "MJException.h"

#include <bitset>
#include <fstream>

#define MAX_TING 99

/// @brief 生成特征牌张数序列
/// 每位取1234，总长度限制为9，每位和限制为14
/// 如：有效序列：111111111, 4442,44114,1,4,11
/// 不包含的序列：1111111111（超长），4443（和为15）
class CountSequenceGenerator
{
    std::string m_TempSeq;
    std::function<void(std::string const&)> m_OnGen;

public:
    // 生成的序列通过回调onGen获取
    CountSequenceGenerator(int maxHandCount, std::function<void(std::string const&)> onGen)
    {
        m_OnGen = onGen;
        Run(maxHandCount);
    }

private:
    void Run(int rest)
    {
        m_OnGen(m_TempSeq);
        if (rest == 0 || m_TempSeq.size() >= 9)
        {
            return;
        }
        for (int i = 1; i <= rest && i <= 4; ++i)
        {
            m_TempSeq.push_back('0' + i);
            Run(rest - i);
            m_TempSeq.pop_back();
        }
    }
};

/// @brief 生成步长序列
/// 每位取01，长度+（1的个数）限制为8
/// 如：有效序列：0,1,00,1111,00000000,0000001
/// 不包含的序列：11110，000000000
class StepSequenceGenerator
{
    std::string m_TempSeq;
    std::function<void(std::string const&)> m_OnGen;

public:
    StepSequenceGenerator(std::function<void(std::string const&)> onGen)
    {
        m_OnGen = onGen;
        Run(0);
    }

private:
    void Run(int curLen = 0)
    {
        m_OnGen(m_TempSeq);
        if (m_TempSeq.size() > 13)
        {
            return;
        }
        for (int i = 0; i < 2; ++i)
        {
            if (i + curLen + 1 > 8)
            {
                break;
            }
            m_TempSeq.push_back('0' + i);
            Run(curLen + 1 + i);
            m_TempSeq.pop_back();
        }
    }
};

/// @brief 拆分一组牌
/// 算出可拆的 刻、顺、对、搭、单张的数量
/// 简单拆解，非最优
class SimpleCombineCalc
{
public:
    MJ::TingStepRecord DoCheck(std::vector<int> const& counts)
    {
        m_Counts = counts;

        m_Counts.insert(m_Counts.end(), 2, 0);  // 插两个占位

        for (size_t i = 0; i < m_Counts.size() - 2; ++i)
        {
            CheckKe(i);
            CheckPartKe(i);
            CheckShun(i);
        }

        for (size_t i = 0; i < m_Counts.size() - 2; ++i)
        {
            CheckPartShun(i);
            if (m_Counts.at(i) < 0)
            {
                throw MJ_MAKE_EXCEPTION("%d", m_Counts.at(i));
            }
            m_PartCounts.Single += m_Counts.at(i);
        }
        return m_PartCounts;
    }

private:
    MJ::TingStepRecord m_PartCounts;
    void CheckKe(size_t index)
    {
        if (m_Counts.at(index) >= 3)
        {
            m_Counts.at(index) -= 3;
        }
    }
    void CheckShun(size_t index)
    {
        auto n = m_Counts.at(index);
        if (n)
        {
            n = std::min(m_Counts.at(index + 1), n);
            n = std::min(m_Counts.at(index + 2), n);
            if (n)
            {
                m_Counts.at(index) -= n;
                m_Counts.at(index + 1) -= n;
                m_Counts.at(index + 2) -= n;
            }
        }
    }
    void CheckPartKe(size_t index)
    {
        if (m_Counts.at(index) >= 2)
        {
            m_PartCounts.Double++;

            m_Counts.at(index) -= 2;
        }
    }
    void CheckPartShun(size_t index)
    {
        auto& n0 = m_Counts.at(index);
        for (int i = 1; i <= 2 && n0; ++i)
        {
            auto& n1 = m_Counts.at(index + i);
            int n = std::min(n0, n1);
            if (n)
            {
                n0 -= n;
                n1 -= n;
                m_PartCounts.Double += n;
            }
        }
    }

private:
    std::vector<int> m_Counts;
};

// 根据一组相临牌的牌张数，计算最优拆牌
class BestCombineCalc
{
    MJ::TingStepRecord m_PartCounts;

public:
    MJ::TingStepRecord DoCheck(std::vector<int> const& counts)
    {
        m_PartCounts.Single = int8_t(counts.size() * 4);
        DoPick(counts, MJ::TingStepRecord(), 0);
        return m_PartCounts;
    }

private:
    void DoPick(std::vector<int> const& counts, MJ::TingStepRecord const& stast, int index)
    {
        bool hasKe = PickKe(counts, stast, index);
        bool hasShun = PickShun(counts, stast, index);
        bool k2 = PickKe2(counts, stast, index);
        bool s2 = PickShun2(counts, stast, index);
        if (!hasKe && !hasShun && !s2 && !k2)
        {
            End(counts, stast);
        }
    }
    bool PickKe(std::vector<int> counts, MJ::TingStepRecord stast, int index)
    {
        for (int i = index; i < counts.size(); ++i)
        {
            auto& count = counts[i];
            if (count >= 3)
            {
                count -= 3;
                DoPick(counts, stast, i);
                return true;
            }
        }
        return false;
    }
    bool PickShun(std::vector<int> counts, MJ::TingStepRecord stast, int index)
    {
        if (counts.size() < 3)
            return false;
        for (; index < (int)counts.size() - 2; ++index)
        {
            if (counts.at(index) && counts.at(index + 1) && counts.at(index + 2))
            {
                counts.at(index) -= 1;
                counts.at(index + 1) -= 1;
                counts.at(index + 2) -= 1;
                DoPick(counts, stast, index);
                return true;
            }
        }
        return false;
    }
    bool PickKe2(std::vector<int> counts, MJ::TingStepRecord stast, int index)
    {
        for (int i = index; i < counts.size(); ++i)
        {
            auto& count = counts[i];
            if (count >= 2)
            {
                count -= 2;
                stast.Double++;
                DoPick(counts, stast, i);
                return true;
            }
        }
        return false;
    }
    bool PickShun2(std::vector<int> counts, MJ::TingStepRecord stast, int index)
    {
        if (counts.size() < 2)
            return false;
        for (; index < (int)counts.size() - 1; ++index)
        {
            auto& n0 = counts.at(index);
            for (int i = 1; i <= 2 && n0 > 0 && i + index < (int)counts.size(); ++i)
            {
                auto& n1 = counts.at(index + i);
                int n = std::min(n0, n1);
                if (n)
                {
                    n0 -= n;
                    n1 -= n;
                    stast.Double += n;
                    DoPick(counts, stast, index);
                    return true;
                }
            }
        }
        return false;
    }

private:
    void End(std::vector<int> counts, MJ::TingStepRecord stast)
    {  // DFS 终点
        for (auto n : counts)
        {  //余牌为单张
            stast.Single += n;
        }
        if (stast.Sum() < m_PartCounts.Sum())
        {  //取最优
            m_PartCounts = stast;
        }
    }
};

MJ::TingStepRecord CheckPartCounts(std::vector<int> const& cardsCount)
{
    BestCombineCalc tvc;
    return tvc.DoCheck(cardsCount);
}
void MJ::TingNormal::_Initialize(int tileCount)
{
    if (m_TingCountMap.size())
    {
        return;
    }
    std::map<size_t, std::vector<std::string>> stepSeqs;
    StepSequenceGenerator stepGen([&stepSeqs](std::string const& s) { stepSeqs[s.size()].push_back(s); });

    CountSequenceGenerator countGen(tileCount, [&stepSeqs, this](std::string const& cs) {
        if (cs.empty())
            return;
        for (auto ss : stepSeqs[cs.size() - 1])
        {
            this->_AddItemToMap(cs, ss);
        }
    });
}

void MJ::TingNormal::Init(int tileCount)
{
    if (m_IsInited)
    {
        return;
    }

    m_IsInited = true;
    std::ifstream ifile("MahjongTingCodes" + std::to_string(tileCount));
    if (ifile.is_open())
    {
        int64_t v = 0;
        char c = ',';
        //std::vector<uint32_t> data;
        while (c == ',' && ifile >> v)
        {
            auto code = TingCode(v >> 5);
            auto db_n = (v >> 2) & 0x7;
            auto sg_n = v & 0x3;
            TingStepRecord pc{};
            pc.Double = int8_t(db_n);
            pc.Single = int8_t(sg_n);

            m_TingCountMap[code] = pc;

            ifile >> c;
        }
    }
    // 如果从文件中加载成功,则应该有140920/417491条记录
    if ((tileCount == 14 && m_TingCountMap.size() != 140920)     // 14
        || (tileCount == 17 && m_TingCountMap.size() != 417491)  // 17
        || (tileCount != 14 && tileCount != 17))                 //other
    {
        m_TingCountMap.clear();
        this->_Initialize(tileCount);
        return;
    }
}

void MJ::TingNormal::Output() const
{
    int n = 0;
    for (auto pair : m_TingCountMap)
    {
        uint32_t db = pair.second.Double;
        uint32_t sg = pair.second.Single;
        auto code = pair.first;
        if (sg > 3 || db > 7)
        {
            throw MJ_MAKE_EXCEPTION("sg=%d,db=%d", sg, db);
        }
        auto tail = (db << 2) | sg;
        std::cout << ((code << 5) | tail) << ",";
        if (++n % 10 == 0)
        {
            std::cout << "\n";
        }
    }
}
int MJ::TingNormal::CalcStepPingHu(std::map<ITileID, int> countsMap, int laiziCount, int otherCount) const
{
    int step = _CheckTingStep(countsMap, false, laiziCount, otherCount);
    for (auto& pair : countsMap)
    {
        if (pair.second >= 2)
        {
            pair.second -= 2;
            int step0 = _CheckTingStep(countsMap, true, laiziCount, otherCount);
            if (step0 < step)
            {
                step = step0;
            }
            pair.second += 2;
        }
    }
    return step;
}

int MJ::TingNormal::_CheckTingStep(std::map<ITileID, int> const& countsMap, bool hasJiang, int laiziCount, int otherCount) const
{
    std::string cardCounts;  // 牌数序列
    std::string cardSteps;   // 步长序列
    ITileID preCard = 0;
    TingStepRecord counts{};
    for (auto pair : countsMap)
    {
        if (pair.second < 0)
        {
            throw MJ_MAKE_EXCEPTION("%d", pair.second);
        }
        if (pair.second == 0)
        {
            continue;
        }
        if (!cardCounts.empty())
        {
            int step = (pair.first - preCard) / TILE_ID_STEP;
            if (!MJIsSuitTile(pair.first))
            {
                step = 3;
            }

            if (step > 2)
            {
                _CheckCount(cardCounts, cardSteps, out__ counts);
                cardCounts.clear();
                cardSteps.clear();
            }
            else if (step == 2)
            {
                cardSteps.push_back('1');
            }
            else
            {
                cardSteps.push_back('0');
            }
        }
        cardCounts.push_back(pair.second + '0');
        preCard = pair.first;
    }

    if (!cardCounts.empty())
    {
        _CheckCount(cardCounts, cardSteps, out__ counts);
    }
    counts.Single += otherCount;
    return _CalcStep(counts, hasJiang, laiziCount);
}

void MJ::TingNormal::_CombineLaiZiRef(TingStepRecord& record, int& laiziCount) const
{
    // every Double need 1 laizi
    // every Single need 2 laizi
    if (laiziCount < record.Double)
    {
        record.Double -= laiziCount;
        laiziCount = 0;
    }
    else
    {
        laiziCount -= record.Double;
        record.Double = 0;
        if (laiziCount >= record.Single * 2)
        {
            laiziCount -= record.Single * 2;
            record.Single = 0;
        }
        else
        {
            record.Single -= (laiziCount / 2);
            laiziCount = laiziCount % 2;
        }
    }
}

int MJ::TingNormal::_CalcStep(TingStepRecord record, bool hasJiang, int laiziCount) const
{
    _CombineLaiZiRef(record, in_out__ laiziCount);

    int step = 0;
    if (record.Double <= record.Single)
    {
        step += record.Double;
        int restSingle = record.Single - record.Double;
        if (hasJiang)
        {
            step += ((restSingle + 2) / 3) * 2;
        }
        else
        {
            if (restSingle > 2)
            {
                restSingle -= 2;  // 两单张，换1成将
                step += 1;
                step += ((restSingle + 2) / 3) * 2;  // 每3个，向听+2
            }
            else if (restSingle > 0)
            {
                step += 1;
            }
            else
            {
                step += 2;
            }
        }
    }
    else
    {
        int restKS2 = record.Double - record.Single;
        step += record.Single;
        if (hasJiang)
        {
            step += (restKS2 / 3) * 2;  // 每3个，向听+2
            step += (restKS2 % 3);
        }
        else
        {  // 全是Shun2
            step += (restKS2 / 3) * 2;
            constexpr int n[] = {2, 1, 2};
            step += n[restKS2 % 3];
        }
    }
    return step >= laiziCount ? step - laiziCount : 0;
}

void MJ::TingNormal::_AddItemToMap(std::string const& cardCounts, std::string const& cardSteps)
{
    if (cardSteps.size() + 1 != cardCounts.size())
    {
        throw MJ_MAKE_EXCEPTION("%d %d", cardSteps.size(), cardCounts.size());
    }
    BestCombineCalc tvc;
    std::vector<int> cardsCount;
    cardsCount.push_back(cardCounts.front() - '0');
    for (size_t i = 0; i < cardSteps.size(); ++i)
    {
        if (cardSteps.at(i) != '0')
        {
            cardsCount.push_back(0);
        }
        cardsCount.push_back(cardCounts.at(i + 1) - '0');
    }

    auto code = _ExpressionToCode(cardCounts, cardSteps);

    m_TingCountMap[code] = CheckPartCounts(cardsCount);
}

TingCode MJ::TingNormal::_ExpressionToCode(std::string const& cardCounts, std::string const& cardSteps) const
{
    if (cardSteps.size() + 1 != cardCounts.size())
    {
        throw MJ_MAKE_EXCEPTION("%d %d", cardSteps.size(), cardCounts.size());
    }
    // count 1:00, 2:01, 3:10, 4:11
    // step 0:0, 1:1

    TingCode code = (1 << 2) | TingCode((TingCode)cardCounts.front() - '1');
    for (size_t i = 0; i < cardSteps.size(); ++i)
    {
        code <<= 1;
        code |= (cardSteps.at(i) == '0' ? 0 : 1);
        code <<= 2;
        code |= TingCode((TingCode)cardCounts.at(i + 1) - '1');
    }
    if (code > 0xffffffff)
    {
        throw MJ_MAKE_EXCEPTION("code=%d", code);
    }
    return code;
}

void MJ::TingNormal::_CheckCount(std::string const& cardCounts, std::string const& cardSteps, TingStepRecord& o_Res) const
{
    auto code = _ExpressionToCode(cardCounts, cardSteps);
    auto iter = m_TingCountMap.find(code);

    if (iter == m_TingCountMap.end())
    {
        throw MJ_MAKE_EXCEPTION("%d %s %s", m_TingCountMap.size(), cardCounts.c_str(), cardSteps.c_str());
    }

    o_Res.Double += iter->second.Double;
    o_Res.Single += iter->second.Single;
}
