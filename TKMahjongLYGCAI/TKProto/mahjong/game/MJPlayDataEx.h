#pragma once
#include <map>
#include <set>
#include "MJPlayData.h"
#include "MJConfig.h"
#include "MJModule.h"

//�����Ż�����  0 ��Ч���ţ�1 ������ ���ơ����Ƶȣ�2 �����ƣ�3 ��Ч
constexpr int TILE_USEFUL = 0;   //��Ч����
constexpr int TILE_SPEACIL = 1;  //�������
constexpr int TILE_RELATED = 2;  //��������
constexpr int TILE_USELESS = 3;  //��Ч����

class MJPlayDataEx : public MJPlayData, public MJGameModule
{
public:
    using MJGameModule::MJGameModule;

    virtual ~MJPlayDataEx(){};

public:
    virtual bool CanPon(ITileID tile, bool CantOnlyLaiAfterPon) const;
    virtual bool CanChow(ITileID tile) const;
    //���ڶԷ�����ʱ�����Ȩ��
    virtual bool CanZhiKon(ITileID tile, std::shared_ptr<MJConfig> cfg);
    //�Լ��غϲ��Ȩ��
    virtual bool CanSelfKon(std::shared_ptr<MJConfig> cfg, std::vector<ITileID> const& ignoreTiles);  //���ⰵ��

    bool CanDiscard(ITileID tile) const;
    virtual bool CanKon(ITileID tile, KonType type) const;
    bool CanChow(ITileID leftTile, ITileID curTile) const;

    bool IsTianDiHu() const;
    bool IsCurrentDrawTile(ITileID tile) const;

    ITileID FindAnyInHand(std::set<ITileID> const& tiles);
    void CloseTianDiHu();

    void PickTianHuTile();
    void ClearDraw();
    void SetRemoveHutypes(std::set<int>& types);

    virtual EHandStyle GetHandStyle();

    virtual HuPlayData MakeHuPlayData(ITileID extraTile = TILE_NULL, ITileID removeTile = TILE_NULL, int removeCount = 1) const;

    std::vector<ITileID> const& GetGangTiles() const;

    bool IsAllLai() const;

protected:
    //�˷��������� CanZhiKon��CanSelfKon�����������������ǿ��ܱ����أ�_CanKonAfterCall�߼��б�
    //so:��Ҫ�������ط����ô˷���
    bool _CanKonAfterCall(ITileID tile, KonType type, std::shared_ptr<MJConfig> cfg);
    std::vector<ITileID> m_GangTiles{};
    std::set<int> m_RemoveHutypes{};

public:  // for MoPai�Ż�
    int CheckType(ITileID tile) override;
    void UpdateMinTing(ITileID tile) override;

protected:
    virtual bool _IsUsefulTile(ITileID tile);   //��Ч����
    virtual bool _IsSpeacilTile(ITileID tile);  //������
    virtual bool _IsRelateTile(ITileID tile);   //������
};
