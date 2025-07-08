#pragma once
#include "MJDealer.h"
#include "MJPlay.h"
#include "MJPlayDataEx.h"
#include "MJHu.h"
#include "MJDealer.h"

struct SPlayConf  //��������
{
    bool m_HasChow{};                                                 // �Ƿ��
    bool m_HasPon{};                                                  //�Ƿ�������
    bool m_PonPass{};                                                 //�Ƿ��������
    bool m_HuPass{};                                                  //��������
    bool m_MustHu{};                                                  //�к��غ�
    bool m_OnlyZimo{};                                                //ֻ������
    bool m_CanotOnlyLaiAfterPon{};                                    //����������ȫ������
    bool m_MustHuIfOnlyLai{};                                         //ȫ���ӱغ�
    bool m_CanotDiscardLai{};                                         //���Ӳ��ɴ��
    bool m_TianTing{};                                                //�������淨
    bool m_CanDeclareCall{};                                          //�б����淨
    bool m_BuKonPass{};                                               //�������ֹ��ָ�
    bool m_ZhiKonAfterPon{};                                          //���󲹸���ֱ��
    int64_t m_MinMultipleLimit{0};                                    //�������
    int64_t m_MaxMultipleLimit{std::numeric_limits<int64_t>::max()};  //�ⶥ����

    int64_t GetRealMultiple(int64_t mult) const;  //��ȡ����
    bool IsTopMultiple(int64_t mult) const;       //�Ƿ�ﵽ�����
};

// ֻ������Ϸ���淨����������ϢЭ�顢��Χ���ݵ�
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
    void Initialize(int banker);  // ��ʼ��

    void DoDeal();  // ����

    void DoSwitchSeat(ISeatID seat = SEAT_NULL);  // �л���seat����SEAT_NULL �л���һ��

    ITileID DoDraw();  //����

    bool DoTryChow(ISeatID seat, ITileID leftTile);  //��

    void DoPon(ISeatID seat);  //��

    void DoZhiKon(ISeatID seat);  //ֱ��

    bool DoTryKon(ITileID tile, KonType type);  //���ܻ򰵸�

    virtual bool DoTryCall(ITileID tile, ISeatID seat);  //����

    virtual void DoDiscard(ITileID tile);  //���ƣ�TILE_NULL��tile���Ϸ�ʱ�Զ���һ�ţ����ش������

    virtual void DoDianPao(std::vector<ISeatID> const& huSeats);  // ����

    virtual void DoQiangGang(std::vector<ISeatID> const& huSeats);  //����

    std::vector<int64_t> DoCheckSelfHuMultiple(std::vector<ISeatID>& cappedSeats);                                   //���������ر��ֽ���[����]
    std::vector<int64_t> DoCheckPaoHuMultiple(ISeatID winSeat, ISeatID paoSeat, std::vector<ISeatID>& cappedSeats);  //���ڣ����ر��ֽ���[����]

    virtual void Pass(ISeatID seat, MJOperates operates);  //����
    bool IsPonThisTileCurHand(ITileID tile, ISeatID curSeat);
    void SetNextBanker(ISeatID seat);  //ָ���¾�ׯ
    ISeatID GetNextBanker() const;
    ISeatID GetLastDisTileSeat() const;

    void SetPlayersAtomEvents();

    std::vector<ITileID> const& LaiTiles() const;

    bool IsLai(ITileID tile) const;

    bool NoDiscardByLai(ITileID tile) const;  //�������Ӳ��ܴ��

    void GetShowTilesInfo(std::map<ITileID, int>& mTiles, ISeatID seat);  //����ܿ��������� Ŀǰ���������ж�

public:  //Ȩ��
    MJOperates FetchWaitCallOperates(ISeatID seat);
    virtual MJOperates FetchSelfOperates();
    virtual MJOperates FetchWaitOperates(ISeatID seat);
    MJOperates FetchGrabKonOperates(ISeatID seat);

    ITileID GetCurrentWinTile();

public:
    int GetConfigValue(ConfigIndex index) const;
    bool IsAfterPon();      //���󣨴�ʱ���ɺ�����ֱ�ܣ�
    bool IsAfterKon();      //�ܺ󣨴�ʱ�����ܣ�
    bool IsGangThisHand();  //�Ƿ񱾻غϸ�
    bool IsGangPao();       //�Ƿ���ڣ���ʱ����ת�ƣ�
    bool IsAfterKonDraw();  //��������ʱ�ɸܿ���
    bool IsHaiDiLao();
    bool IsAfterKonDrawDiscard();  //�����򣨴�ʱ�ɸ��ڣ�
    virtual void FreshCallData(ISeatID seat);
    bool CheckXueZhanDaoDiTip(int restPlayerCount, int beforePlayerCount);

public:
    ISeatID GetLastDisSeat() const;
    HuResult const& GetHuResult(ISeatID seat) const;
    std::vector<ISeatID> const& GetHuSeats() const;
    bool IsCurrentPassBuKon() const;
    bool IsKonAfterPon() const;
    bool IsKonAfterPonBefor(ITileID tile) const;  //���һ���ܻ�δ������ʷ��¼ʱ�ж���Ҫ���⴦��
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

protected:  // ����
    void _OnTips(MJTips::Type tip, ISeatID seat);

    bool _IsPonOperates(EOperate operate) const;
    bool _IsKonOperates(EOperate operate) const;
    void _AddHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, bool mustHu = false);

    virtual void _CheckPaoHuOperate(MJOperates& opt, ISeatID seat, HuResult const& result, std::vector<MJTips::Type>& tips);

    virtual void _NBBotDisCard(ITileID& tile);  //���ָ����֣�BOT���������ƣ�����ӡ�˳�ӣ������ܱ�Ϊ1����

protected:  //��Ҫ�����Լ�ʵ�ֵĲ���
    virtual void _CheckCall(MJOperates& opt, ISeatID seat);
    virtual std::vector<int> GetExtraCallHuTypes(ISeatID);

    virtual bool _IsTianTing(ISeatID seat);

    virtual void _Initialize() = 0;
    virtual std::vector<int> _ZiMoExtraFan(ISeatID seat) = 0;                 //�������ӷ�
    virtual bool _PaoExtraFan(ISeatID seat, std::vector<int>& extraFan) = 0;  //���ڸ��ӷ�
    virtual std::vector<int> _QiangGangExtraFan(ISeatID seat) = 0;            //���ܸ��ӷ�
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
