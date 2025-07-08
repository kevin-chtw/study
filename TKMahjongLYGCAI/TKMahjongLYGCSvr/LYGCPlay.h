#pragma once

#include "MJPlayEx.h"
#include "LYGCPlayData.h"
#include "LYGCDealer.h"

class LYGCPlay : public MJPlayEx
{
public:
    LYGCPlay(LYGCGame* game);

    std::shared_ptr<MJPlayDataEx> OnCreatePlayData(ISeatID seat) override;
    std::shared_ptr<MJ::Dealer> OnCreateDealer() override;

public:
    std::vector<int64_t> DoCheckGangMultiple();
    std::vector<int64_t> DoCheckLiuJuMultiple();
    std::vector<int64_t> DoCheckGenZhuangMultiple();
    std::vector<int64_t> DoCheckMiMultiple(LYGCLastHandData& data, EHuType huType);
    MJOperates FetchSelfOperates() override;
    MJOperates FetchWaitOperates(ISeatID seat) override;
    bool CheckCurGenZhuang();
    bool IsGenZhuang()
    {
        return m_bGenZhuang;
    }

    int GetCurSeatTouchTimes();
    void SetCurSeatTouchTimes(int Times);

protected:
    void _CheckPaoHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, std::vector<MJTips::Type>& tips);

protected:  //需要子类自己实现的部分
    void _Initialize() override;
    std::vector<int> _ZiMoExtraFan(ISeatID seat) override;
    bool _PaoExtraFan(ISeatID seat, std::vector<int>& extraFan) override;
    std::vector<int> _QiangGangExtraFan(ISeatID seat) override;
    void _CheckSelfCi(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips);
    bool m_bGenZhuang{};
    std::map<ISeatID, int> m_mTouchTileTimes;

public:
    int GetWinMode(ISeatID seat) const;

    std::vector<int64_t> GetHuMultiple(EHuType huType, ISeatID nDianCiSeat, ISeatID huSeat);
    void _handTianHu(const ISeatID& huSeat);
};
