#pragma once

#include <vector>
#include <deque>
#include <map>
#include <string>
#include "MJDefine.h"

/// <summary>
/// �����ļ�ini��ʽ��
/*
[InitCard]
EnableInitCard=1
CardCount=56
player0=1Ͳ,3Ͳ,5Ͳ,7Ͳ,9Ͳ,1��,6��,7��,9��,5��,2��,7��,2Ͳ,4Ͳ
player1=1Ͳ,3Ͳ,5Ͳ,7Ͳ,9Ͳ,1��,4��,6��,9��,9��,4��,2��,3��
player2=1Ͳ,2Ͳ,4Ͳ,6Ͳ,8Ͳ,9��,7��,5��,2��,4��,6��,6��,9��
player3=1��,1��,1��,2��,2��,2��,3��,3��,3��,4��,4��,4��,5��
other=3��,5��,7��,8��,1��
back=
*/
/// </summary>
namespace MJ
{
class ManualCards
{
public:
    /// <summary>
    /// ���������ļ���
    /// </summary>
    /// <param name="name">�齫�淨����,��:MahjongZJ</param>
    /// <param name="nth">��N�������ļ�,�����ļ���:2</param>
    /// <returns>�ַ������� "InitCard_#name#_#nth#.ini",��nth==0,�����nth. ��"InitCard_MahjongZJ_2.ini"(nth==2),"InitCard_MahjongZJ.ini"(nth==0)</returns>
    ManualCards(std::string const& name, int nth);

    bool LoadManualCards(std::deque<ITileID>& out__ target, std::map<ITileID, int> const& tilesCount, int playerCount = 4, int bankerTileCount = 14);

    bool IsEnabled() const;

    // ��ȡ������ name ��ֵ
    int GetConfigValue(std::string const& name, int defaultValue) const;

    // ��ȡ������ name ���ַ���ֵ
    std::string GetConfigString(std::string const& name) const;

    //@depreciation replaced by GetConfigString
    std::string LoadIniCards(std::string const& name) const;

    std::vector<ITileID> LoadTiles(std::string const& names);

private:
    std::string m_InitCardFile;
};

}  // namespace MJ
