#include "MJDealer.h"
#include "MJManual.h"
#include "MJGame.h"

namespace MJ
{
ITileID Dealer::DrawTile(ISeatID drawSeat, MJPlayData* playerData)
{
    ITileID tile = _DrawSelfTiles(drawSeat);
    if (tile == TILE_NULL)
    {
        tile = _DrawStratety(playerData);
    }
    return tile;
}

void Dealer::Exchange(std::vector<ITileID>& in_out__ tiles)
{
    if (m_TileWall.empty() || tiles.empty())
    {
        return;
    }
    EMJColor color = MJTileColor(tiles.front());
    std::map<EMJColor, std::vector<ITileID*>> wallTiles;
    for (auto& tile : m_TileWall)
    {
        if (MJTileColor(tile) == color)
        {
            continue;
        }
        auto& v = wallTiles[MJTileColor(tile)];
        v.push_back(&tile);
        if (v.size() == tiles.size())
        {
            for (size_t i = 0; i < tiles.size(); ++i)
            {
                std::swap(*v.at(i), tiles.at(i));
            }
            return;
        }
    }
}

ITileID Dealer::_DrawTile(ITileID need)
{
    if (need == TILE_NULL)
    {
        return _PopTile();
    }
    auto iter = std::find(m_TileWall.begin(), m_TileWall.end(), need);
    if (iter != m_TileWall.end())
    {
        m_TileWall.erase(iter);
        return need;
    }
    return _PopTile();
}

bool Dealer::LoadManual(std::string const& name, int initCardsFileNo, int playerCount, int bankerTileCount)
{
    ManualCards manual(name, initCardsFileNo);
    if (!manual.IsEnabled())
    {
        return false;
    }

    m_TileWall.clear();
    if (!manual.LoadManualCards(m_TileWall, TilesMap(), playerCount, bankerTileCount))
    {
        return false;
    }
    auto count = manual.GetConfigValue("CardCount", 0);
    auto dealCount = playerCount * (bankerTileCount - 1) + 1;
    if (count > dealCount && count < int(m_TileWall.size()))
    {
        m_TileWall.resize(count);
    }
    m_IsManual = true;
    return true;
}

void Dealer::InitRandom()
{
    m_TileWall.clear();
    auto& map = TilesMap();
    for (auto& p : map)
    {
        m_TileWall.insert(m_TileWall.end(), p.second, p.first);
    }
    std::shuffle(m_TileWall.begin(), m_TileWall.end(), MJ::DefaultRandomEngine);
}

ITileID Dealer::_DrawSelfTiles(ISeatID drawSeat)
{
    if (m_Game->IsValidSeat(drawSeat))
    {
        auto& selfTiles = m_SelfTiles[drawSeat];
        if (!selfTiles.empty())
        {
            ITileID tile = selfTiles.front();
            selfTiles.pop_front();
            return tile;
        }
    }
    return TILE_NULL;
}

ITileID Dealer::_DrawStratety(MJPlayData* typer)
{
    if (m_IsManual || nullptr == typer)
    {
        return _PopTile();
    }

    int rate = typer->GetDrawRate();
    if (rate <= 0 || MJ::Random(100) >= rate)
    {
        return _PopTile();
    }

    auto configs = m_Game->GetDrawConfigs();
    if (configs.empty())
    {
        return _PopTile();
    }
    int config = typer->GetDrawConfig();
    if (config < 0 || config >= configs.size())
    {
        config = 0;
    }
    DrawStrategy<4> drawStrategy(m_Game);
    drawStrategy.Init(configs.at(config));

    ITileID tile = drawStrategy.Draw(this, typer, typer->GetMinTing());
    tile = _DrawTile(tile);
    typer->UpdateMinTing(tile);
    return tile;
}

ITileID MJ::Dealer::GetTile(size_t index) const
{
    if (m_TileWall.size() > index)
    {
        auto tile = m_TileWall.at(index);
        return tile;
    }
    return TILE_NULL;
}

ITileID MJ::Dealer::_PopTile(size_t index)
{
    if (m_TileWall.size() > index)
    {
        auto tile = m_TileWall.at(index);
        m_TileWall.erase(m_TileWall.begin() + index);
        return tile;
    }
    return TILE_NULL;
}

size_t MJ::Dealer::GetRestTileCount() const
{
    size_t count = m_TileWall.size();
    for (auto& self : m_SelfTiles)
    {
        count += self.second.size();
    }
    return count;
}
}  // namespace MJ
