#include "MJChecker.h"
#include "MJScore.h"
#include "MJException.h"
#include "MJGame.h"

#define CHECK_VECTOR_SIZE_AS_NP(v)                          \
    if (v.size() != m_Game->GetIniPlayerCount())            \
    {                                                       \
        throw MJ_MAKE_EXCEPTION("error vector data count"); \
    }

MJChecker::MJChecker(MJGame* game, EScoreType minScoreType)
{
    m_Game = game;
    m_ScoreType = minScoreType;
}

MJChecker::MJChecker(MJGame* game, int scoreType)
{
    m_Game = game;
    m_ScoreType = (EScoreType)scoreType;
}

std::vector<int64_t> MJCheckerMany::Check(MJScoreType::Type type, std::vector<int64_t> const& multiples, std::vector<int64_t>* pOrigins, std::vector<bool>* pLimited)
{
    CHECK_VECTOR_SIZE_AS_NP(multiples);

    int64_t base = m_Game->m_nScoreBase;
    std::vector<int64_t> takeScores(multiples.size(), 0);
    std::vector<int64_t> originScores{};
    for (int i = 0; i < multiples.size(); ++i)
    {
        auto player = m_Game->GetPlayer(i);
        auto multi = multiples.at(i);
        takeScores.at(i) = player->GetCurrentScore() + (multi > 0 ? player->GetTax() : 0);
        originScores.push_back(multi * base);
    }
    auto res = CalFinalScore(takeScores, originScores, pLimited);
    for (int i = 0; i < res.size(); ++i)
    {
        m_Game->GetPlayer(i)->AddScoreChange(res.at(i));
    }

    RecordScoreInfo(type, multiples, originScores, res, pLimited);

    if (pOrigins)
    {
        (*pOrigins).swap(originScores);
    }

    return res;
}

std::vector<int64_t> MJCheckerMany::CheckWithScores(MJScoreType::Type type, std::vector<int64_t> const& origins, std::vector<bool>* pLimited)
{
    CHECK_VECTOR_SIZE_AS_NP(origins);

    int64_t base = m_Game->m_nScoreBase;
    std::vector<int64_t> takeScores(origins.size(), 0);
    for (int i = 0; i < takeScores.size(); ++i)
    {
        auto player = m_Game->GetPlayer(i);
        takeScores.at(i) = player->GetCurrentScore() + (origins.at(i) > 0 ? player->GetTax() : 0);
    }
    auto res = CalFinalScore(takeScores, origins, pLimited);
    for (int i = 0; i < res.size(); ++i)
    {
        m_Game->GetPlayer(i)->AddScoreChange(res.at(i));
    }
    RecordScoreInfo(type, {}, origins, res, pLimited);
    return res;
}

std::vector<int64_t> MJChecker::CalFinalScore(std::vector<int64_t> const& takeScores, std::vector<int64_t> const& oriScores, std::vector<bool>* pLimited) const
{
    auto count = takeScores.size();

    if (m_ScoreType == EScoreType::MinScore)
    {
        return _CalcMinScore(takeScores, oriScores, pLimited);
    }
    else if (m_ScoreType == EScoreType::PositiveScore)
    {
        auto res = oriScores;
        for (int i = 0; i < res.size(); ++i)
        {
            if (takeScores.at(i) < 0)
            {
                res.at(i) = 0;
            }
            else if (oriScores.at(i) + takeScores.at(i) < 0)
            {
                res.at(i) = -takeScores.at(i);
            }
        }
        return res;
    }
    else if (m_ScoreType == EScoreType::JustWin)
    {
        auto res = oriScores;
        for (auto& v : res)
        {
            v = std::max(int64_t(0), v);
        }
        return res;
    }
    else
    {
        return oriScores;
    }
}

std::vector<ISeatID> MJChecker::CheckBankrupt() const
{
    if (m_Game->IsFDTable() || m_ScoreType != EScoreType::MinScore)
    {
        return {};
    }
    auto res = std::vector<ISeatID>{};
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        if (!m_Game->GetPlayer(i)->IsOut() && m_Game->GetPlayer(i)->GetCurrentScore() <= 0)
        {
            res.push_back(i);
        }
    }
    return res;
}

std::vector<int64_t> MJChecker::_CalcMinScore(std::vector<int64_t> const& takeScores, std::vector<int64_t> const& oriScores, std::vector<bool>* pLimited) const
{
    int error = 0;
    int count = takeScores.size();
    auto finalScores = MJ::TryReduceScore(takeScores, oriScores, &error);
    if (error)
    {
        throw MJ_MAKE_EXCEPTION("Min Score Error: %d %s %s", error, MJ::ToString(takeScores).c_str(), MJ::ToString(oriScores).c_str());
    }
    if (pLimited)
    {  // 非空，则返回上限信息
        // 查是否上限限制：增加赢分者的带入分，其结果会增加
        auto tempTake4 = takeScores;
        for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
        {
            if (oriScores.at(i) > 0)
            {
                tempTake4.at(i) *= 2;
                tempTake4.at(i) += 10000;
            }
        }
        auto tempFinal = MJ::TryReduceScore(tempTake4, oriScores, &error);
        pLimited->assign(count, false);
        for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
        {
            if (oriScores.at(i) > 0 && tempFinal.at(i) > finalScores.at(i))
            {
                pLimited->at(i) = true;
            }
        }
    }
    return finalScores;
}

void MJChecker::SyncAllPlayerScore()
{
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        m_Game->GetPlayer(i)->SyncGameResult();
    }
}

std::vector<ISeatID> MJChecker::SetRestLoseOut()
{
    std::vector<ISeatID> seats;
    for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
    {
        auto player = m_Game->GetPlayer(i);
        if (player->IsOut())
        {
            continue;
        }
        seats.push_back(i);
        player->SetOut(MJPlayerStatus::End);
    }
    return seats;
}

std::vector<ISeatID> MJChecker::SetBankruptOut()
{
    auto bankruptSeats = this->CheckBankrupt();
    for (auto seat : bankruptSeats)
    {
        m_Game->GetPlayer(seat)->SetOut(MJPlayerStatus::Bankrupt);
    }
    return bankruptSeats;
}

std::vector<MJScoreNode> const& MJCheckerMany::GetFullBills() const
{
    return m_Bills;
}

void MJCheckerMany::RecordScoreInfo(
    MJScoreType::Type type, std::vector<int64_t> const& multiples, std::vector<int64_t> const& origins, std::vector<int64_t> const& finals, std::vector<bool> const* const pLimited)
{
    m_Bills.push_back({});
    auto& back = m_Bills.back();
    back.winScoreType = type;
    back.multiple = multiples;
    back.scoresOrigin = origins;
    back.scoresFinal = finals;
}

void MJCheckerOnce::AddMultiple(MJScoreType::Type type, std::vector<int64_t> const& multiple)
{
    CHECK_VECTOR_SIZE_AS_NP(multiple);
    auto& ref = m_MultiplesMap[type];

    if (ref.empty())
    {
        ref = multiple;
    }
    else
    {
        for (size_t i = 0; i < ref.size(); ++i)
        {
            ref.at(i) += multiple.at(i);
        }
    }

    if (m_TotalMultiples.empty())
    {
        m_TotalMultiples = multiple;
    }
    else
    {
        for (size_t i = 0; i < ref.size(); ++i)
        {
            m_TotalMultiples.at(i) += multiple.at(i);
        }
    }
}

std::vector<int64_t> MJCheckerOnce::Check(std::vector<int64_t>* pOrigins, std::vector<bool>* pLimited)
{
    int64_t base = m_Game->m_nScoreBase;
    std::vector<int64_t> takeScores(m_TotalMultiples.size(), 0);
    std::vector<int64_t> originScores{};
    for (int i = 0; i < m_TotalMultiples.size(); ++i)
    {
        auto player = m_Game->GetPlayer(i);
        auto time = m_TotalMultiples.at(i);
        takeScores.at(i) = player->GetCurrentScore() + (time > 0 ? player->GetTax() : 0);
        originScores.push_back(time * base);
    }

    auto res = CalFinalScore(takeScores, originScores, pLimited);
    for (int i = 0; i < res.size(); ++i)
    {
        m_Game->GetPlayer(i)->AddScoreChange(res.at(i));
    }

    if (pOrigins)
    {
        (*pOrigins).swap(originScores);
    }

    return res;
}

std::vector<int64_t> MJCheckerOnce::GetMultiples(MJScoreType::Type type)
{
    auto iter = m_MultiplesMap.find(type);
    if (iter == m_MultiplesMap.end())
    {
        return std::vector<int64_t>(m_Game->GetIniPlayerCount(), 0);
    }
    return iter->second;
}

int64_t MJCheckerOnce::GetMultiple(ISeatID seat, MJScoreType::Type type)
{
    auto iter = m_MultiplesMap.find(type);
    if (iter == m_MultiplesMap.end())
    {
        return 0;
    }
    return iter->second.at(seat);
}

int64_t MJCheckerOnce::GetOriginScore(ISeatID seat, MJScoreType::Type type)
{
    return GetMultiple(seat, type) * m_Game->GetScoreBase();
}

const std::vector<int64_t>& MJCheckerOnce::GetTotalMultiples()
{
    return m_TotalMultiples;
}
