/*
	Copyright (c) 2001, 竞技世界（成都）

	Version: 0.1
	Author: Guotj7400
	Date: 2022-11

	摘要:
	麻将牌 CardID 与 字符串表示 进行互转

	class TileAINameCvt 转为双字符
	class TileNameCvt 转为中文名
*/

/*
class TileAINameCvt
用于ITileID 与AI的双字符表示法 的相互转换
双字符表示法：
1、CBD 表示万条筒，后接1-9表示点数；
2、WE,WS,WW,WN 表示东南西北
3、AC,AF,AB 表示中发白
4、"FP", "FO", "FB", "FC" 花
5、"SP", "SU", "AU", "WI" 季
*/

/*
class TileNameCvt
1万,...,9万,...东南西北中发白春夏秋冬梅兰竹菊
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

    // 添加扩展映射
    // prefix="宝", flag=4
    // 添加：宝1万=5 ....
    void AddPrefix(std::string const& prefix, int flag);

    std::string IdToName(ITileID card) const;
    ITileID NameToId(std::string const& id) const;

    // name,name,name ... 转 std::vector{ ID, ID, ID ...}
    std::vector<ITileID> CommaNamesToIds(std::string const& commaString) const;
    std::string IdsToCommaNames(std::vector<ITileID> const& tiles) const;

    void _AddPair(std::string const& key, ITileID tile);

protected:
    TileExpression();
    std::map<std::string, ITileID> m_S2I;
    std::map<ITileID, std::string> m_I2S;
};

// AI表示
class TileAINameCvt : public TileExpression
{
public:
    TileAINameCvt();

    std::string IdsToNames(std::vector<ITileID> const& tiles) const;
    std::vector<ITileID> NamesToIds(std::string const& str) const;
};

// 中文表示
class TileNameCvt : public TileExpression
{
public:
    TileNameCvt();

    std::vector<ITileID> TilesFromNames(std::vector<std::string> names) const;
};

//抿牌功能相似牌张判断
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
