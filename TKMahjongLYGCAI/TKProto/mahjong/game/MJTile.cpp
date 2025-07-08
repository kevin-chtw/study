#include "MJTile.h"
#include "MJString.h"

/**
Tile转字符串
	CBD 表示万条筒
*/
namespace MJ
{
constexpr int SUIT_COLOR_COUNT = 3;
constexpr int SUIT_POINT_COUNT = 9;

void TileExpression::AddPrefix(std::string const& prefix, int flag)
{
    auto i2s = m_I2S;
    for (auto& p : i2s)
    {
        _AddPair(prefix + p.second, p.first | flag);
    }
}

std::string TileExpression::IdToName(ITileID card) const
{
    auto iter = m_I2S.find(card);
    if (iter != m_I2S.end())
    {
        return iter->second;
    }
    return "?";
}

ITileID TileExpression::NameToId(std::string const& id) const
{
    auto iter = m_S2I.find(id);
    if (iter != m_S2I.end())
    {
        return iter->second;
    }
    return TILE_NULL;
}

std::vector<ITileID> TileExpression::CommaNamesToIds(std::string const& commaString) const
{
    auto names = MJ::SplitByString(commaString, ",");
    std::vector<ITileID> cards;

    for (auto& name : names)
    {
        cards.push_back(NameToId(MJ::TrimString(name)));
    }
    return cards;
}

std::string TileExpression::IdsToCommaNames(std::vector<ITileID> const& tiles) const
{
    std::vector<std::string> names;
    for (auto c : tiles)
    {
        names.push_back(IdToName(c));
    }
    return MJ::ToString(names);
}

TileExpression::TileExpression()
{
}

void TileExpression::_AddPair(std::string const& key, ITileID tile)
{
    m_I2S[tile] = key;
    m_S2I[key] = tile;
}

TileAINameCvt::TileAINameCvt()
{
    static const std::string SuitColorNames[SUIT_COLOR_COUNT] = {"C", "B", "D"};

    for (int i = 0; i < SUIT_COLOR_COUNT; ++i)
    {
        for (int j = 0; j < SUIT_POINT_COUNT; ++j)
        {
            std::string key = SuitColorNames[i] + std::to_string(j + 1);
            int value = MJMakeTile(i, j);

            this->_AddPair(key, value);
        }
    }

    const char* flowers[] = {"FP", "FO", "FB", "FC"};
    const char* seasons[] = {"SP", "SU", "AU", "WI"};
    const char* winds[] = {"WE", "WS", "WW", "WN"};
    const char* arrows[] = {"AC", "AF", "AB"};

    for (int i = 0; i < 4; ++i)
    {
        _AddPair(flowers[i], MJMakeTile(EMJColor::Flower, i));
        _AddPair(seasons[i], MJMakeTile(EMJColor::Season, i));
        _AddPair(winds[i], MJMakeTile(EMJColor::Wind, i));
    }
    for (int i = 0; i < (int)std::size(arrows); ++i)
    {
        _AddPair(arrows[i], MJMakeTile(EMJColor::Dragon, i));
    }
}

std::string TileAINameCvt::IdsToNames(std::vector<ITileID> const& tiles) const
{
    std::string res;
    for (auto tile : tiles)
    {
        res += IdToName(tile);
    }
    return res;
}

std::vector<ITileID> TileAINameCvt::NamesToIds(std::string const& str) const
{
    if (str.size() % 2 != 0)
    {
        return {};
    }
    std::vector<ITileID> res;
    for (size_t i = 1; i < str.size(); i += 2)
    {
        res.push_back(NameToId(str.substr(i - 1, 2)));
    }
    return res;
}

TileNameCvt::TileNameCvt()
{
    std::pair<EMJColor, std::string> SuitNames[SUIT_COLOR_COUNT] = {
        {EMJColor::Character, "万"},
        {EMJColor::Bamboo, "条"},
        {EMJColor::Dot, "筒"},
    };

    for (auto& name : SuitNames)
    {
        for (int i = 0; i < SUIT_POINT_COUNT; ++i)
        {
            _AddPair(char(i + '1') + name.second, MJMakeTile(name.first, i));
        }
    }

    _AddPair("东", MJMakeTile(EMJColor::Wind, 0));
    _AddPair("南", MJMakeTile(EMJColor::Wind, 1));
    _AddPair("西", MJMakeTile(EMJColor::Wind, 2));
    _AddPair("北", MJMakeTile(EMJColor::Wind, 3));
    _AddPair("中", MJMakeTile(EMJColor::Dragon, 0));
    _AddPair("发", MJMakeTile(EMJColor::Dragon, 1));
    _AddPair("白", MJMakeTile(EMJColor::Dragon, 2));

    _AddPair("春", MJMakeTile(EMJColor::Season, 0));
    _AddPair("夏", MJMakeTile(EMJColor::Season, 1));
    _AddPair("秋", MJMakeTile(EMJColor::Season, 2));
    _AddPair("冬", MJMakeTile(EMJColor::Season, 3));

    _AddPair("梅", MJMakeTile(EMJColor::Flower, 0));
    _AddPair("兰", MJMakeTile(EMJColor::Flower, 1));
    _AddPair("竹", MJMakeTile(EMJColor::Flower, 2));
    _AddPair("菊", MJMakeTile(EMJColor::Flower, 3));
}

std::vector<ITileID> TileNameCvt::TilesFromNames(std::vector<std::string> names) const
{
    std::vector<ITileID> res;
    for (auto name : names)
    {
        res.push_back(NameToId(name));
    }
    return res;
}

std::shared_ptr<TileAINameCvt> GetTileAINameCvt()
{
    static auto s = std::make_shared<TileAINameCvt>();
    return s;
}

std::shared_ptr<TileNameCvt> GetTileNameCvt()
{
    static auto s = std::make_shared<TileNameCvt>();
    return s;
}
std::shared_ptr<TileAssociation> GetTileAssociation()
{
    static auto s = std::make_shared<TileAssociation>();
    return s;
}

const std::vector<std::set<int>> TileAssociation::m_vvCards = {
    {MJMakeTile(EMJColor::Bamboo, 2), MJMakeTile(EMJColor::Bamboo, 3), MJMakeTile(EMJColor::Bamboo, 4)},
    {MJMakeTile(EMJColor::Bamboo, 5), MJMakeTile(EMJColor::Bamboo, 6)},
    {MJMakeTile(EMJColor::Dot, 3), MJMakeTile(EMJColor::Dot, 4)},
    {MJMakeTile(EMJColor::Dot, 5), MJMakeTile(EMJColor::Dot, 6)},
    {MJMakeTile(EMJColor::Wind, 0), MJMakeTile(EMJColor::Wind, 1), MJMakeTile(EMJColor::Wind, 2), MJMakeTile(EMJColor::Wind, 3)},
};

bool TileAssociation::IsAssociationCards(ITileID card, ITileID drawCard)
{
    if (drawCard == card)
    {
        return true;
    }

    EMJColor drawColor = MJTileColor(drawCard);
    if (drawColor != MJTileColor(card))
    {
        return false;
    }

    if (EMJColor::Wind == drawColor)
    {
        return true;
    }

    if (EMJColor::Character == drawColor)
    {
        auto point = MJTilePoint(card);
        auto drawPoint = MJTilePoint(drawCard);
        if ((point == 0 || point == 8) && std::abs(point - drawPoint) <= 2)
        {
            return true;
        }
        return std::abs(point - drawPoint) <= 1;
    }

    for (auto it = m_vvCards.begin(); it != m_vvCards.end(); it++)
    {
        if (std::find(it->begin(), it->end(), card) != it->end() && std::find(it->begin(), it->end(), drawCard) != it->end())
        {
            return true;
        }
    }
    return false;
}

std::set<ITileID> TileAssociation::GetTileAssociationCards(ITileID drawTile)
{
    std::set<ITileID> asTiles{};

    if (MJTileColor(drawTile) == EMJColor::Character)
    {
        asTiles.insert(drawTile);
        auto point = MJTilePoint(drawTile);
        if (point == 0)
        {
            asTiles.insert(MJMakeTile(EMJColor::Character, 1));
        }
        else if (point == 8)
        {
            asTiles.insert(MJMakeTile(EMJColor::Character, 7));
        }

        else
        {
            if (point == 2)
            {
                asTiles.insert(MJMakeTile(EMJColor::Character, 0));
            }
            else if (point == 6)
            {
                asTiles.insert(MJMakeTile(EMJColor::Character, 8));
            }
            asTiles.insert(MJMakeTile(EMJColor::Character, point - 1));
            asTiles.insert(MJMakeTile(EMJColor::Character, point + 1));
        }
    }
    else
    {
        for (auto group : m_vvCards)
        {
            if (std::find(group.begin(), group.end(), drawTile) != group.end())
            {
                asTiles = group;
                return asTiles;
            }
        }
        asTiles = {drawTile};
    }

    return asTiles;
}

std::string GetTileName(ITileID tile)
{
    if (MJIsValidTile(tile))
    {
        return MJ::GetTileAINameCvt()->IdToName(tile);
    }
    return "Unknow";
}

ITileID GetTileID(std::string const& name)
{
    return MJ::GetTileAINameCvt()->NameToId(name);
}

EMJColor GetColorID(std::string const& name)
{
    std::map<std::string, EMJColor> namesMap = {
        {"C", EMJColor::Character},
        {"B", EMJColor::Bamboo},
        {"D", EMJColor::Dot},
    };
    auto iter = namesMap.find(name);
    if (iter != namesMap.end())
    {
        return iter->second;
    }
    return EMJColor::Undefined;
}

std::string GetColorName(EMJColor color)
{
    static const std::string names = "CBD";
    if (color >= EMJColor::Character && color <= EMJColor::Dot)
    {
        return std::string(1, names.at((int)color));
    }
    return std::string();
}

}  // namespace MJ
