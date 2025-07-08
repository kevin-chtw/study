#pragma once
#include "MJDealer.h"
#include "MJPlay.h"
#include "MJPlayDataEx.h"
#include "MJHu.h"
#include "MJDealer.h"

struct SPlayConf  //功能配置
{
    bool m_HasChow{};                                                 // 是否吃
    bool m_HasPon{};                                                  //是否允许碰
    bool m_PonPass{};                                                 //是否过碰不碰
    bool m_HuPass{};                                                  //过胡不胡
    bool m_MustHu{};                                                  //有胡必胡
    bool m_OnlyZimo{};                                                //只能自摸
    bool m_CanotOnlyLaiAfterPon{};                                    //不允许碰后全是赖子
    bool m_MustHuIfOnlyLai{};                                         //全赖子必胡
    bool m_CanotDiscardLai{};                                         //赖子不可打出
    bool m_TianTing{};                                                //有天听玩法
    bool m_CanDeclareCall{};                                          //有报叫玩法
    bool m_BuKonPass{};                                               //补杠区分过手杠
    bool m_ZhiKonAfterPon{};                                          //碰后补杠算直杠
    int64_t m_MinMultipleLimit{0};                                    //起胡倍数
    int64_t m_MaxMultipleLimit{std::numeric_limits<int64_t>::max()};  //封顶倍数

    int64_t GetRealMultiple(int64_t mult) const;  //获取倍数
    bool IsTopMultiple(int64_t mult) const;       //是否达到最大倍数
};

// 只负责游戏的玩法，不处理消息协议、外围数据等
class MJPlayEx : public MJPlay
{
public:
    MJPlayEx(MJGame* game);

    void Initialize() override;

    virtual ~MJPlayEx(){};

    virtual std::shared_ptr<MJPlayDataEx> OnCreatePlayData(ISeatID seat);
    virtual std::shared_ptr<MJ::Dealer> OnCreateDealer();  // default is nullptr

public:
    void SetTipsHandler(std::function<void(MJTips::Type, ISeatID)> handler);

    template <typename T = MJ::Dealer>
    std::shared_ptr<T> GetDealer() const
    {
        return std::dynamic_pointer_cast<T>(m_Dealer);
    }

    template <typename T = MJPlayDataEx>
    std::shared_ptr<T> GetPlayData(ISeatID seat) const
    {
        return std::dynamic_pointer_cast<T>(m_PlayDatas.at(seat));
    }

public:
    void Initialize(int banker);  // 初始化

    void DoDeal();  // 发牌

    void DoSwitchSeat(ISeatID seat = SEAT_NULL);  // 切换至seat，若SEAT_NULL 切换下一家

    ITileID DoDraw();  //摸牌

    bool DoTryChow(ISeatID seat, ITileID leftTile);  //吃

    void DoPon(ISeatID seat);  //碰

    void DoZhiKon(ISeatID seat);  //直杠

    bool DoTryKon(ITileID tile, KonType type);  //补杠或暗杠

    virtual bool DoTryCall(ITileID tile, ISeatID seat);  //听牌

    virtual void DoDiscard(ITileID tile);  //出牌，TILE_NULL或tile不合法时自动出一张，返回打出的牌

    virtual void DoDianPao(std::vector<ISeatID> const& huSeats);  // 点炮

    virtual void DoQiangGang(std::vector<ISeatID> const& huSeats);  //抢杠

    std::vector<int64_t> DoCheckSelfHuMultiple(std::vector<ISeatID>& cappedSeats);                                   //自摸，返回本轮结算[倍数]
    std::vector<int64_t> DoCheckPaoHuMultiple(ISeatID winSeat, ISeatID paoSeat, std::vector<ISeatID>& cappedSeats);  //点炮，返回本轮结算[倍数]

    virtual void Pass(ISeatID seat, MJOperates operates);  //过牌
    bool IsPonThisTileCurHand(ITileID tile, ISeatID curSeat);
    void SetNextBanker(ISeatID seat);  //指定下局庄
    ISeatID GetNextBanker() const;
    ISeatID GetLastDisTileSeat() const;

    void SetPlayersAtomEvents();

    std::vector<ITileID> const& LaiTiles() const;

    bool IsLai(ITileID tile) const;

    bool NoDiscardByLai(ITileID tile) const;  //因是赖子不能打出

    void GetShowTilesInfo(std::map<ITileID, int>& mTiles, ISeatID seat);  //玩家能看到的牌张 目前用于抿牌判断

public:  //权限
    MJOperates FetchWaitCallOperates(ISeatID seat);
    virtual MJOperates FetchSelfOperates();
    virtual MJOperates FetchWaitOperates(ISeatID seat);
    MJOperates FetchGrabKonOperates(ISeatID seat);

    ITileID GetCurrentWinTile();

public:
    int GetConfigValue(ConfigIndex index) const;
    bool IsAfterPon();      //碰后（此时不可胡，可直杠）
    bool IsAfterKon();      //杠后（此时可抢杠）
    bool IsGangThisHand();  //是否本回合杠
    bool IsGangPao();       //是否杠炮（此时呼叫转移）
    bool IsAfterKonDraw();  //杠摸（此时可杠开）
    bool IsHaiDiLao();
    bool IsAfterKonDrawDiscard();  //杠摸打（此时可杠炮）
    virtual void FreshCallData(ISeatID seat);
    bool CheckXueZhanDaoDiTip(int restPlayerCount, int beforePlayerCount);

public:
    ISeatID GetLastDisSeat() const;
    HuResult const& GetHuResult(ISeatID seat) const;
    std::vector<ISeatID> const& GetHuSeats() const;
    bool IsCurrentPassBuKon() const;
    bool IsKonAfterPon() const;
    bool IsKonAfterPonBefor(ITileID tile) const;  //最后一个杠还未放入历史记录时判断需要特殊处理
    EOperate GetLastOperate();
    bool HasNoAction(ISeatID seat);

protected:
    virtual ITileID _CalcAutoDiscardTile() const;

    void _CheckSelfHu(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips);
    void _CheckPaoHu(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips);

    void _CheckChow(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips);
    void _CheckPon(MJOperates& opt, ISeatID seat, std::vector<MJTips::Type>& tips);

    void _CheckZhiKon(MJOperates& opt, ISeatID seat);
    void _CheckSelfKon(MJOperates& opt, ISeatID seat);

    KonType _FixKonTypeAfterPon(KonType type, ITileID tile) const;

    bool _CheckMustHuByLai(ISeatID seat);

protected:  // 过程
    void _OnTips(MJTips::Type tip, ISeatID seat);

    bool _IsPonOperates(EOperate operate) const;
    bool _IsKonOperates(EOperate operate) const;
    void _AddHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, bool mustHu = false);

    virtual void _CheckPaoHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, std::vector<MJTips::Type>& tips);

    virtual void _NBBotDisCard(ITileID& tile);  //新手福利局，BOT需打出成组牌（如刻子、顺子），不能变为1向听

protected:  //需要子类自己实现的部分
    virtual void _CheckCall(MJOperates& opt, ISeatID seat);
    virtual std::vector<int> GetExtraCallHuTypes(ISeatID);

    virtual bool _IsTianTing(ISeatID seat);

    virtual void _Initialize() = 0;
    virtual std::vector<int> _ZiMoExtraFan(ISeatID seat) = 0;                 //自摸附加番
    virtual bool _PaoExtraFan(ISeatID seat, std::vector<int>& extraFan) = 0;  //点炮附加番
    virtual std::vector<int> _QiangGangExtraFan(ISeatID seat) = 0;            //抢杠附加番
protected:
    MJ::DiscardRecommender m_Recommender;
    std::vector<HuResult> m_HuResults{};

    SPlayConf m_PlayConf;

    std::function<void(MJTips::Type, ISeatID)> m_TipsHandler;
    ISeatID m_NextBanker = SEAT_NULL;
    std::vector<ISeatID> m_HuSeats;
    ITileID m_WinTile{TILE_NULL};

    std::vector<ITileID> m_LaiTiles{};

private:
    std::shared_ptr<MJ::Dealer> m_Dealer;
    std::vector<std::shared_ptr<MJPlayDataEx>> m_PlayDatas;
};
