#pragma once
#include "LYGCDefine.h"
#include "MJHu.h"
#include "MJConfig.h"
#include "MJService.h"

enum EHuType
{
    EHT_ZiMo = 1,
    EHT_MingCi = 2,
    EHT_AnCi = 3,
    EHT_PiCi = 4,
    EHT_BaoCi = 5,
    EHT_END,
    EHT_NONE = -1,
};

class LYGCHu : public MJHuBase
{
public:
    bool CheckHu(HuResult out__& result, HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg) override;
    CallDataMap CheckCall(HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg) override;

private:
    bool CheckCi(HuResult out__& result, HuPlayData const& playerInfo, std::shared_ptr<MJConfig> cfg);

    CallTileFan _CheckCallFan(HuPlayData& playerInfo, std::shared_ptr<MJConfig> cfg);
    void _CalcTotalFan(HuResult out__& result, std::shared_ptr<MJConfig> cfg);
};
