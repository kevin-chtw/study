/*
	Copyright (c) 2001, �������磨�ɶ���

	Version: 0.1
	Author: Guotj7400
	Date: 2022-11

	ժҪ:
	�齫�� CardID �� �ַ�����ʾ ���л�ת

	class TileAINameCvt תΪ˫�ַ�
	class TileNameCvt תΪ������
*/

/*
class TileAINameCvt
����ITileID ��AI��˫�ַ���ʾ�� ���໥ת��
˫�ַ���ʾ����
1��CBD ��ʾ����Ͳ�����1-9��ʾ������
2��WE,WS,WW,WN ��ʾ��������
3��AC,AF,AB ��ʾ�з���
4��"FP", "FO", "FB", "FC" ��
5��"SP", "SU", "AU", "WI" ��
*/

/*
class TileNameCvt
1��,...,9��,...���������з��״����ﶬ÷�����
*/
#pragma once

#include <vector>
#include <string>
#include "MJDefine.h"
#include "MJString.h"

namespace MJ
{
class TileExpression
{
public:
    virtual ~TileExpression() = default;

    // �����չӳ��
    // prefix="��", flag=4
    // ��ӣ���1��=5 ....
    void AddPrefix(std::string const& prefix, int flag);

    std::string IdToName(ITileID card) const;
    ITileID NameToId(std::string const& id) const;

    // name,name,name ... ת std::vector{ ID, ID, ID ...}
    std::vector<ITileID> CommaNamesToIds(std::string const& commaString) const;
    std::string IdsToCommaNames(std::vector<ITileID> const& tiles) const;

    void _AddPair(std::string const& key, ITileID tile);

protected:
    TileExpression();
    std::map<std::string, ITileID> m_S2I;
    std::map<ITileID, std::string> m_I2S;
};

// AI��ʾ
class TileAINameCvt : public TileExpression
{
public:
    TileAINameCvt();

    std::string IdsToNames(std::vector<ITileID> const& tiles) const;
    std::vector<ITileID> NamesToIds(std::string const& str) const;
};

// ���ı�ʾ
class TileNameCvt : public TileExpression
{
public:
    TileNameCvt();

    std::vector<ITileID> TilesFromNames(std::vector<std::string> names) const;
};

//���ƹ������������ж�
class TileAssociation
{
public:
    bool IsAssociationCards(ITileID tile, ITileID drawTile);
    std::set<ITileID> GetTileAssociationCards(ITileID drawTile);

private:
    static const std::vector<std::set<int>> m_vvCards;
};

extern std::shared_ptr<TileAINameCvt> GetTileAINameCvt();
extern std::shared_ptr<TileNameCvt> GetTileNameCvt();
extern std::shared_ptr<TileAssociation> GetTileAssociation();

std::string GetTileName(ITileID tile);
ITileID GetTileID(std::string const& name);

EMJColor GetColorID(std::string const& name);
std::string GetColorName(EMJColor color);

}  // namespace MJ
