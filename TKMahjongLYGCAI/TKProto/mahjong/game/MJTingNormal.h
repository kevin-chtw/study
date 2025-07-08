#pragma once
#include "MJDefine.h"
#include "MJString.h"
#include "MJHelper.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>

/**
usage: 使用MJ::TingCore::GetInstance()

1\在service中初始化:
MJ::TingCore::GetInstance()->Initialize();

2\调用
MJ::TingCore::GetInstance()->CalcStep(...);
*/

using TingCode = uint32_t;

// 测试用，打开可使用内联数据表
// 请使用预定义宏，不要修改此处代码
//#define USE_QUICK_INIT 1

namespace MJ
{
struct TingStepRecord
{
    int8_t Double{};  // 搭子，对子
    int8_t Single{};  // 单张
    int Sum()
    {
        return Double + Single;
    }
};

// 查向听
// 七对 与 普通牌型
class TingNormal
{
public:
    // 先尝试加载数据文件，若失败再 Initialize
    void Init(int tileCount);

    int CalcStepPingHu(std::map<ITileID, int> countsMap, int laiziCount, int otherCount) const;

    // 输出表
    // 配合 Initialize 可用于生成表文件
    void Output() const;

private:
    // 直接计算进行初始化
    void _Initialize(int tileCount);

    int _CheckTingStep(std::map<ITileID, int> const& countsMap, bool hasJiang, int laiziCount, int otherCount) const;

    int _CalcStep(TingStepRecord count, bool hasJiang, int laiziCount) const;

    void _CombineLaiZiRef(TingStepRecord& count, int& laiziCount) const;

    /**
			用 cardCounts,cardSteps 表示一组牌的特征
			如：C1C1C2C4C5 可处理为 "2(0)1(1)1(0)1", 括号内表示间隔数，外面值表示牌张数
			cardCounts取牌张数= "2111", cardSteps取间隔数="010"
			cardCounts每位可取值1234，用两个二进制位表示(1:00, 2:01, 3:10, 4:11)；cardSteps每位取01，用一个二进制位表示
			C1C1C2C4C5: 2(0)1(1)1(0)1: TingCode = <0b> 01 0 00 1 00 0 00
		*/

    TingCode _ExpressionToCode(std::string const& cardCounts, std::string const& cardSteps) const;

    void _AddItemToMap(std::string const& cardCounts, std::string const& cardSteps);

    // 查询序列的拆解结果，并将结果加入o_Res，即: o_Res += result.
    void _CheckCount(std::string const& cardCounts, std::string const& cardSteps, TingStepRecord& o_Res) const;

private:
    std::map<TingCode, TingStepRecord> m_TingCountMap;
    bool m_IsInited = false;
};

}  // namespace MJ
