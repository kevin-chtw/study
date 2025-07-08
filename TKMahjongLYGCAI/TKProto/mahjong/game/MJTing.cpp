#include "MJTing.h"
#include "MJTingNormal.im.hpp"
#include <sstream>

//≤π≥‰≈∆’≈Œ™3N+1
int _FixHandCount(int cardCount)
{
    return (122 - cardCount) % 3;
}

std::map<ITileID, int> MJ::TileListToMap(std::vector<ITileID> const& tiles, std::vector<ITileID> laizi, int& laiziCount, int& otherCount)
{
    laiziCount = 0;
    otherCount = 0;
    std::map<ITileID, int> countsMap;
    for (auto card : tiles)
    {
        if (MJ::HasElement(laizi, card))
        {
            ++laiziCount;
        }
        else if (::MJIsExtraTile(card))
        {
            ++otherCount;
        }
        else
        {
            countsMap[card]++;
        }
    }
    return countsMap;
}

static int _CheckQiDuiStep(std::map<ITileID, int> const& countsMap, int laiziCount)
{
    int pairCount = laiziCount;
    for (auto& p : countsMap)
    {
        if (!MJIsExtraTile(p.first))
        {
            pairCount += (p.second / 2);
        }
    }
    if (pairCount >= 7)
    {
        return 0;
    }
    return (7 - pairCount);
}

int MJ::CalcStepTo13Yao(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards)
{
    if (cards.size() < 13)
    {
        return MAX_TING;
    }
    int laiziCount = 0;

    std::vector<ITileID> targets = {
        MJMakeTile(EMJColor::Character, 0),
        MJMakeTile(EMJColor::Character, 8),
        MJMakeTile(EMJColor::Bamboo, 0),
        MJMakeTile(EMJColor::Bamboo, 8),
        MJMakeTile(EMJColor::Dot, 0),
        MJMakeTile(EMJColor::Dot, 8),
        MJMakeTile(EMJColor::Wind, 0),
        MJMakeTile(EMJColor::Wind, 1),
        MJMakeTile(EMJColor::Wind, 2),
        MJMakeTile(EMJColor::Wind, 3),
        MJMakeTile(EMJColor::Dragon, 0),
        MJMakeTile(EMJColor::Dragon, 1),
        MJMakeTile(EMJColor::Dragon, 2),
    };

    std::vector<ITileID> selected;
    int extraCount = 0;
    for (auto card : cards)
    {
        if (MJ::HasElement(laiziCards, card))
        {
            laiziCount++;
        }
        else if (MJ::HasElement(targets, card))
        {
            if (MJ::HasElement(selected, card))
            {
                ++extraCount;
            }
            else
            {
                selected.push_back(card);
            }
        }
    }
    int usedCount = (int)selected.size() + (extraCount > 0 ? 1 : 0) + laiziCount;
    if (usedCount >= 14)
    {
        return 0;
    }
    else
    {
        return (14 - usedCount);
    }
}

int MJ::CalcStepQiDui(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards)
{
    int laiZiCount{}, otherCount{};
    std::map<ITileID, int> countsMap = TileListToMap(cards, laiziCards, laiZiCount, otherCount);
    return _CheckQiDuiStep(countsMap, laiZiCount);
}
/// <summary>
/// new
/// </summary>

MJTingEx11::MJTingEx11()
{
    m_NormalTool.Init(11);
}

MJTingEx14::MJTingEx14()
{
    m_NormalTool.Init(14);
}

MJTingEx17::MJTingEx17()
{
    m_NormalTool.Init(17);
}

std::shared_ptr<MJTingEx11> MJTingEx11::GetInstance()
{
    static auto self = std::make_shared<MJTingEx11>();
    return self;
}

std::shared_ptr<MJTingEx14> MJTingEx14::GetInstance()
{
    static auto self = std::make_shared<MJTingEx14>();
    return self;
}

std::shared_ptr<MJTingEx17> MJTingEx17::GetInstance()
{
    static auto self = std::make_shared<MJTingEx17>();
    return self;
}

int MJTingBase::CalcStep(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes, int* type)
{
    try
    {
        std::map<ITileID, int> tileMap;
        int laiZiCount = 0;
        int otherCount = 0;
        std::map<ITileID, int> countsMap = MJ::TileListToMap(cards, laiziCards, laiZiCount, otherCount);
        otherCount += _FixHandCount(cards.size());
        return m_NormalTool.CalcStepPingHu(countsMap, laiZiCount, otherCount);
    }
    catch (MJException& e)
    {
        std::ostringstream ss;
        ss << e.what() << " cards=";
        for (auto c : cards)
        {
            ss << c << ",";
        }
        throw MJ_MAKE_EXCEPTION("%s", ss.str().c_str());
    }
}

int MJTingBase::CalcTing(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes, int* type)
{
    int step = CalcStep(cards, laiziCards, extraTypes, type) - 1;
    return step >= 0 ? step : 0;
}

int MJTingEx14::CalcStep(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes, int* type)
{
    std::map<ITileID, int> tileMap;
    int laiZiCount = 0;
    int otherCount = 0;
    std::map<ITileID, int> countsMap = MJ::TileListToMap(cards, laiziCards, laiZiCount, otherCount);
    otherCount += _FixHandCount(cards.size());
    auto step = m_NormalTool.CalcStepPingHu(countsMap, laiZiCount, otherCount);
    MJ::SetValueIfNotNull(type, (int)Type::Normal);
    if (MJ::HasElement(extraTypes, (int)Type::QiDui) && cards.size() > 12)
    {
        auto step1 = _CheckQiDuiStep(countsMap, laiZiCount);
        if (step1 < step)
        {
            step = step1;
            MJ::SetValueIfNotNull(type, (int)Type::QiDui);
        }
    }
    if (MJ::HasElement(extraTypes, (int)Type::N13Yao) && cards.size() > 12)
    {
        auto step1 = MJ::CalcStepTo13Yao(cards, laiziCards);
        if (step1 < step)
        {
            step = step1;
            MJ::SetValueIfNotNull(type, (int)Type::N13Yao);
        }
    }
    return step;
}
