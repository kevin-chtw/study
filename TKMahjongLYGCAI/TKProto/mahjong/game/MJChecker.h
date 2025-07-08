#pragma once
#include "MJPlayer.h"
#include <queue>

struct MJScoreNode
{
    MJScoreType::Type winScoreType{};  //Ӯ������
    std::vector<int64_t> multiple{};   //����ֱ����֣���ֵΪempty
    std::vector<int64_t> scoresOrigin{};
    std::vector<int64_t> scoresFinal{};
};

class MJChecker
{
public:
    /// <param name="game"></param>
    /// <param name="minScoreType">������С������</param>
    MJChecker(MJGame* game, EScoreType minScoreType);
    [[deprecated]] MJChecker(MJGame* game, int minScoreType);

    virtual ~MJChecker() = default;

    std::vector<ISeatID> CheckBankrupt() const;

public:  // ��������Player�����⴦���ֱ�ӵ���Player�ķ���
    // ���֣����ʣ����δOut����ҽ��У�
    std::vector<ISeatID> SetRestLoseOut();
    std::vector<ISeatID> SetBankruptOut();

    void SyncAllPlayerScore();

public:  // ��������
    /// <param name="takeScores">����������NP4</param>
    /// <param name="oriScores">����������NP4����takeScores����һ��</param>
    std::vector<int64_t> CalFinalScore(std::vector<int64_t> const& takeScores, std::vector<int64_t> const& oriScores, std::vector<bool>* limited) const;

private:
    std::vector<int64_t> _CalcMinScore(std::vector<int64_t> const& takeScores, std::vector<int64_t> const& oriScores, std::vector<bool>* limited) const;

protected:
    MJGame* m_Game{};
    EScoreType m_ScoreType{};
};

//����շ�
class MJCheckerMany : public MJChecker
{
public:
    using MJChecker::MJChecker;

    /// <summary>
    /// ����������,��������ͬ���� MJPlayer��currentscore
    /// </summary>
    /// <param name="multiples">����ҵ���Ӯ�������±�����λ��Ӧ���������������һ�£�ֵӦΪ0��</param>
    /// <param name="pOrigins">������������ԭʼ�÷֣���nullptr�ɺ��ԡ�����ֵ�ᱻ���ã��������ʼֵ��</param>
    /// <param name="pLimited">���� ���ޣ������ֵ����޷���Ӯ�� ��Ϣ����nullptr��ʾ�����ġ�����ֵ�ᱻ���ã��������ʼֵ��</param>
    /// <returns>ʵ�ʷ����䶯</returns>
    std::vector<int64_t> Check(MJScoreType::Type, std::vector<int64_t> const& multiples, std::vector<int64_t>* pOrigins = nullptr, std::vector<bool>* pLimited = nullptr);

    /// <summary>
    /// ������������ķ���
    /// </summary>
    /// <param name="origins">�����������Ӯ�֣�0��</param>
    /// <param name="pLimited">��Check</param>
    std::vector<int64_t> CheckWithScores(MJScoreType::Type, std::vector<int64_t> const& origins, std::vector<bool>* pLimited = nullptr);

public:
    //��չ
    std::vector<MJScoreNode> const& GetFullBills() const;

protected:
    void RecordScoreInfo(
        MJScoreType::Type type, std::vector<int64_t> const& multiples, std::vector<int64_t> const& origins, std::vector<int64_t> const& finals, std::vector<bool> const* const pLimited);

    std::vector<MJScoreNode> m_Bills;
};

//�����շ�
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

/// һЩ���õĶ�������
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
