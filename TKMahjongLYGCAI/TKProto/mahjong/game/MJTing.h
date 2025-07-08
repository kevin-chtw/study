#pragma once
#include "MJDefine.h"
#include "MJString.h"
#include "MJHelper.h"
#include "MJTingNormal.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace MJ
{
int CalcStepTo13Yao(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards);
int CalcStepQiDui(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards);
std::map<ITileID, int> TileListToMap(std::vector<ITileID> const& tiles, std::vector<ITileID> laizi, int& laiziCount, int& otherCount);
}  // namespace MJ

class MJTingBase
{
public:
    enum class Type
    {
        Normal,
        QiDui,
        N13Yao,
    };
    /// <summary>
    /// �������
    /// </summary>
    /// <param name="cards">����</param>
    /// <param name="laiziCards">ָ����Щ������</param>
    /// <param name="extraTypes">ָ�����߶ԡ�ʮ���۵����ͣ���ָ��Ĭ��ƽ����Normal��</param>
    /// <param name="type">��������</param>
    /// <returns>�����</returns>
    virtual int CalcStep(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes = {}, int* type = nullptr);

    ///��������
    /// <param name="cards">����</param>
    /// <param name="laiziCards">ָ����Щ������</param>
    /// <param name="extraTypes">ָ�����߶ԡ�ʮ���۵����ͣ���ָ��Ĭ��ƽ����Normal��</param>
    /// <param name="type">��������</param>
    /// <returns>������ = ����� - 1</returns>
    int CalcTing(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes = {}, int* type = nullptr);

protected:
    MJ::TingNormal m_NormalTool;
};

class MJTingEx11 : public MJTingBase
{
public:
    MJTingEx11();
    static std::shared_ptr<MJTingEx11> GetInstance();
};

class MJTingEx14 : public MJTingBase
{
public:
    static std::shared_ptr<MJTingEx14> GetInstance();
    MJTingEx14();
    int CalcStep(std::vector<ITileID> const& cards, std::vector<ITileID> const& laiziCards, std::vector<int> const& extraTypes = {}, int* type = nullptr) override;
};

class MJTingEx17 : public MJTingBase
{
public:
    MJTingEx17();
    static std::shared_ptr<MJTingEx17> GetInstance();
};
