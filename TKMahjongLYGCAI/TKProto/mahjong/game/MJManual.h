#pragma once

#include <vector>
#include <deque>
#include <map>
#include <string>
#include "MJDefine.h"

/// <summary>
/// 配牌文件ini格式：
/*
[InitCard]
EnableInitCard=1
CardCount=56
player0=1筒,3筒,5筒,7筒,9筒,1条,6条,7条,9万,5万,2万,7万,2筒,4筒
player1=1筒,3筒,5筒,7筒,9筒,1条,4条,6条,9条,9万,4万,2万,3万
player2=1筒,2筒,4筒,6筒,8筒,9条,7条,5条,2万,4万,6万,6万,9万
player3=1万,1万,1万,2条,2条,2条,3条,3条,3条,4条,4条,4条,5条
other=3万,5万,7万,8万,1万
back=
*/
/// </summary>
namespace MJ
{
class ManualCards
{
public:
    /// <summary>
    /// 处理配牌文件名
    /// </summary>
    /// <param name="name">麻将玩法名称,如:MahjongZJ</param>
    /// <param name="nth">第N个配牌文件,则在文件如:2</param>
    /// <returns>字符串形如 "InitCard_#name#_#nth#.ini",若nth==0,则忽略nth. 如"InitCard_MahjongZJ_2.ini"(nth==2),"InitCard_MahjongZJ.ini"(nth==0)</returns>
    ManualCards(std::string const& name, int nth);

    bool LoadManualCards(std::deque<ITileID>& out__ target, std::map<ITileID, int> const& tilesCount, int playerCount = 4, int bankerTileCount = 14);

    bool IsEnabled() const;

    // 获取配置项 name 的值
    int GetConfigValue(std::string const& name, int defaultValue) const;

    // 获取配置项 name 的字符串值
    std::string GetConfigString(std::string const& name) const;

    //@depreciation replaced by GetConfigString
    std::string LoadIniCards(std::string const& name) const;

    std::vector<ITileID> LoadTiles(std::string const& names);

private:
    std::string m_InitCardFile;
};

}  // namespace MJ
