#include "MJPlay.h"
#include "MJGame.h"

MJPlay::MJPlay(MJGame* game) : m_Game(game)
{
}

void MJPlay::Initialize()
{
}

void MJPlay::AddHistory(ISeatID seat, ITileID tile, int operate, int extra)
{
    m_History.push_back(MJAction{operate, seat, tile, extra});
}

void MJPlay::DumpHistory() const
{
    std::stringstream ss;
    for (auto& action : m_History)
    {
        ss << action.seat << "," << action.operate << "," << action.tile << "," << action.extra << ";";
    }
    MJ_LOG_INFO(m_Game, "History;%s", _S(ss.str()));
}

ISeatID MJPlay::GetCurrentSeat() const
{
    return m_CurrentSeat;
}
ITileID MJPlay::GetCurrentTile() const
{
    return m_CurrentTile;
}

ISeatID MJPlay::GetBanker() const
{
    return m_Banker;
}

void MJPlay::DoSwitchSeat(ISeatID seat)
{
    if (m_Game->IsValidSeat(seat))
    {
        m_CurrentSeat = seat;
    }
    else
    {
        for (int i = 0; i < m_Game->GetIniPlayerCount(); ++i)
        {
            m_CurrentSeat = MJGetNextSeat(m_CurrentSeat, 1, m_Game->GetIniPlayerCount());
            if (!m_Game->GetPlayer(m_CurrentSeat)->IsOut())
            {
                break;
            }
        }
    }
}
