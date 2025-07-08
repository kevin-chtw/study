#pragma once
#include "MJPlayer.h"
#include <queue>

struct MJScoreNode
{
    MJScoreType::Type winScoreType{};  //赢分类型
    std::vector<int64_t> multiple{};   //若是直接算分，该值为empty
    std::vector<int64_t> scoresOrigin{};
    std::vector<int64_t> scoresFinal{};
};

class MJChecker
{
public:
    /// <param name="game"></param>
    /// <param name="minScoreType">积分最小化类型</param>
    MJChecker(MJGame* game, EScoreType minScoreType);
    [[deprecated]] MJChecker(MJGame* game, int minScoreType);

    virtual ~MJChecker() = default;

    std::vector<ISeatID> CheckBankrupt() const;

public:  // 辅助操作Player，特殊处理可直接调用Player的方法
    // 变种（针对剩于尚未Out的玩家进行）
    std::vector<ISeatID> SetRestLoseOut();
    std::vector<ISeatID> SetBankruptOut();

    void SyncAllPlayerScore();

public:  // 辅助函数
    /// <param name="takeScores">个数不大于NP4</param>
    /// <param name="oriScores">个数不大于NP4，与takeScores个数一致</param>
    std::vector<int64_t> CalFinalScore(std::vector<int64_t> const& takeScores, std::vector<int64_t> const& oriScores, std::vector<bool>* limited) const;

private:
    std::vector<int64_t> _CalcMinScore(std::vector<int64_t> const& takeScores, std::vector<int64_t> const& oriScores, std::vector<bool>* limited) const;

protected:
    MJGame* m_Game{};
    EScoreType m_ScoreType{};
};

//多次收分
class MJCheckerMany : public MJChecker
{
public:
    using MJChecker::MJChecker;

    /// <summary>
    /// 按倍数结算,结算结果会同步到 MJPlayer的currentscore
    /// </summary>
    /// <param name="multiples">各玩家的输赢倍数（下标与坐位对应），数量与玩家数一致，值应为0和</param>
    /// <param name="pOrigins">捕获倍数计算后的原始得分，传nullptr可忽略。（该值会被重置，无需设初始值）</param>
    /// <param name="pLimited">捕获 上限（因带入分导致无法满赢） 信息，传nullptr表示不关心。（该值会被重置，无需设初始值）</param>
    /// <returns>实际分数变动</returns>
    std::vector<int64_t> Check(MJScoreType::Type, std::vector<int64_t> const& multiples, std::vector<int64_t>* pOrigins = nullptr, std::vector<bool>* pLimited = nullptr);

    /// <summary>
    /// 处理不按倍计算的分数
    /// </summary>
    /// <param name="origins">各玩家理论输赢分，0和</param>
    /// <param name="pLimited">参Check</param>
    std::vector<int64_t> CheckWithScores(MJScoreType::Type, std::vector<int64_t> const& origins, std::vector<bool>* pLimited = nullptr);

public:
    //扩展
    std::vector<MJScoreNode> const& GetFullBills() const;

protected:
    void RecordScoreInfo(
        MJScoreType::Type type, std::vector<int64_t> const& multiples, std::vector<int64_t> const& origins, std::vector<int64_t> const& finals, std::vector<bool> const* const pLimited);

    std::vector<MJScoreNode> m_Bills;
};

//单次收分
class MJCheckerOnce : public MJChecker
{
public:
    using MJChecker::MJChecker;
    void AddMultiple(MJScoreType::Type type, std::vector<int64_t> const& multiple);

    std::vector<int64_t> Check(std::vector<int64_t>* pOrigins = nullptr, std::vector<bool>* pLimited = nullptr);

    std::vector<int64_t> GetMultiples(MJScoreType::Type type);

    int64_t GetMultiple(ISeatID seat, MJScoreType::Type type);
    int64_t GetOriginScore(ISeatID seat, MJScoreType::Type type);

    const std::vector<int64_t>& GetTotalMultiples();

private:
    std::map<MJScoreType::Type, std::vector<int64_t>> m_MultiplesMap;
    std::vector<int64_t> m_TotalMultiples;
};

/// 一些常用的独立操作
template <typename T>
void MJScoreValueChange(std::vector<T>& result, T base, ISeatID winSeat, std::vector<ISeatID> releatedSeats)
{
    for (auto seat : releatedSeats)
    {
        if (seat != winSeat)
        {
            result.at(seat) -= base;
            result.at(winSeat) += base;
        }
    }
}

template <typename T, typename I>
void MJScoreValueChange(std::vector<T>& result, T base, ISeatID winSeat, std::vector<ISeatID> releatedSeats, std::vector<I> const& multiple)
{
    auto m1 = multiple.at(winSeat);
    for (auto seat : releatedSeats)
    {
        if (seat != winSeat)
        {
            auto score = base * m1 * multiple.at(seat);
            result.at(seat) -= score;
            result.at(winSeat) += score;
        }
    }
}
