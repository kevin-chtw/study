#pragma once
#include "MJDefine.h"
#include "MJString.h"
#include "MJHelper.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>

/**
usage: ʹ��MJ::TingCore::GetInstance()

1\��service�г�ʼ��:
MJ::TingCore::GetInstance()->Initialize();

2\����
MJ::TingCore::GetInstance()->CalcStep(...);
*/

using TingCode = uint32_t;

// �����ã��򿪿�ʹ���������ݱ�
// ��ʹ��Ԥ����꣬��Ҫ�޸Ĵ˴�����
//#define USE_QUICK_INIT 1

namespace MJ
{
struct TingStepRecord
{
    int8_t Double{};  // ���ӣ�����
    int8_t Single{};  // ����
    int Sum()
    {
        return Double + Single;
    }
};

// ������
// �߶� �� ��ͨ����
class TingNormal
{
public:
    // �ȳ��Լ��������ļ�����ʧ���� Initialize
    void Init(int tileCount);

    int CalcStepPingHu(std::map<ITileID, int> countsMap, int laiziCount, int otherCount) const;

    // �����
    // ��� Initialize ���������ɱ��ļ�
    void Output() const;

private:
    // ֱ�Ӽ�����г�ʼ��
    void _Initialize(int tileCount);

    int _CheckTingStep(std::map<ITileID, int> const& countsMap, bool hasJiang, int laiziCount, int otherCount) const;

    int _CalcStep(TingStepRecord count, bool hasJiang, int laiziCount) const;

    void _CombineLaiZiRef(TingStepRecord& count, int& laiziCount) const;

    /**
			�� cardCounts,cardSteps ��ʾһ���Ƶ�����
			�磺C1C1C2C4C5 �ɴ���Ϊ "2(0)1(1)1(0)1", �����ڱ�ʾ�����������ֵ��ʾ������
			cardCountsȡ������= "2111", cardStepsȡ�����="010"
			cardCountsÿλ��ȡֵ1234��������������λ��ʾ(1:00, 2:01, 3:10, 4:11)��cardStepsÿλȡ01����һ��������λ��ʾ
			C1C1C2C4C5: 2(0)1(1)1(0)1: TingCode = <0b> 01 0 00 1 00 0 00
		*/

    TingCode _ExpressionToCode(std::string const& cardCounts, std::string const& cardSteps) const;

    void _AddItemToMap(std::string const& cardCounts, std::string const& cardSteps);

    // ��ѯ���еĲ�����������������o_Res����: o_Res += result.
    void _CheckCount(std::string const& cardCounts, std::string const& cardSteps, TingStepRecord& o_Res) const;

private:
    std::map<TingCode, TingStepRecord> m_TingCountMap;
    bool m_IsInited = false;
};

}  // namespace MJ
