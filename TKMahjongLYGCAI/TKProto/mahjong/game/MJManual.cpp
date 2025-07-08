#include "MJManual.h"
#include "MJLog.h"
#include "MJTile.h"
#include "MJHelper.h"
#include "MJString.h"
#include <TKDllServer.h>

namespace MJ
{
///配牌
// name eg: MahjongZJ
ManualCards::ManualCards(std::string const& name, int nth)
{
    if (nth >= 1)
    {
        m_InitCardFile = ".\\InitCard_" + name + "_" + std::to_string(nth) + ".ini";
    }
    else
    {
        m_InitCardFile = ".\\InitCard_" + name + ".ini";
    }
}

bool ManualCards::LoadManualCards(std::deque<ITileID>& out__ target, std::map<ITileID, int> const& tilesCount, int playerCount, int bankerTileCount)
{
    target.clear();
    auto tilesMap = tilesCount;  // mutable copy

    const int GROUP_COUNT = playerCount + 2;
    const int OTHER_INDEX = playerCount;
    const int BACK_INDEX = playerCount + 1;

    std::vector<std::string> configNames;
    for (int i = 0; i < playerCount; ++i)
    {
        configNames.push_back("player"s + std::to_string(i));
    }
    configNames.push_back("other");
    configNames.push_back("back");
    std::vector<std::vector<ITileID>> groups(GROUP_COUNT);
    for (int i = 0; i < GROUP_COUNT; ++i)
    {
        groups[i] = LoadTiles(configNames[i]);
        for (auto tile : groups[i])
        {
            --tilesMap[tile];
            if (tilesMap[tile] < 0)
            {
                MJ_LOG_ERROR(nullptr, "load manual cards failed:%s", _S(configNames[i]));
                return false;
            }
        }
    }

    std::vector<ITileID> rests;
    std::vector<ITileID> restHuas;  //花牌
    for (auto p : tilesMap)
    {
        if (MJIsExtraTile(p.first))
        {
            restHuas.insert(restHuas.end(), p.second, p.first);
        }
        else
        {
            rests.insert(rests.end(), p.second, p.first);
        }
    }
    std::shuffle(rests.begin(), rests.end(), MJ::DefaultRandomEngine);

    // 补充玩家手牌，不发花牌
    int MIN_COUNT = 0;
    for (int i = 0; i < playerCount; ++i)
    {
        target.insert(target.end(), groups[i].begin(), groups[i].end());
        size_t total = (i == 0 ? bankerTileCount : bankerTileCount - 1);
        MIN_COUNT += total;
        int more = int(groups[i].size() < total ? total - (int)groups[i].size() : 0);
        while (more-- && rests.size())
        {
            target.push_back(rests.back());
            rests.pop_back();
        }
    }

    // 处理剩余花牌
    rests.insert(rests.end(), restHuas.begin(), restHuas.end());
    std::shuffle(rests.begin(), rests.end(), MJ::DefaultRandomEngine);

    target.insert(target.end(), groups[OTHER_INDEX].begin(), groups[OTHER_INDEX].end());
    target.insert(target.end(), rests.begin(), rests.end());
    target.insert(target.end(), groups[BACK_INDEX].begin(), groups[BACK_INDEX].end());

    return true;
}

bool ManualCards::IsEnabled() const
{
    return GetPrivateProfileInt("InitCard", "EnableInitCard", 0, m_InitCardFile.c_str());
}

int ManualCards::GetConfigValue(std::string const& name, int defaultValue) const
{
    return GetPrivateProfileInt("InitCard", name.c_str(), defaultValue, m_InitCardFile.c_str());
}

std::string ManualCards::GetConfigString(std::string const& name) const
{
    constexpr int buf_size = 1024;
    char buffer[buf_size]{};
    TKGetIniString("InitCard", name.c_str(), "", buffer, buf_size, m_InitCardFile.c_str());
    return buffer;
}
std::string ManualCards::LoadIniCards(std::string const& name) const
{
    return GetConfigString(name);
}
std::vector<ITileID> ManualCards::LoadTiles(std::string const& name)
{
    return MJ::GetTileNameCvt()->CommaNamesToIds(GetConfigString(name));
}
}  // namespace MJ
