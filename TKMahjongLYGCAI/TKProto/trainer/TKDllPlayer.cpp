#include "TKDllPlayer.h"

CTKGamePlayer::CTKGamePlayer()
{
}

CTKGamePlayer::~CTKGamePlayer()
{
}

LONG CTKGamePlayer::AddRef()
{
    return ++m_cnRef;
}

LONG CTKGamePlayer::Release()
{
    return --m_cnRef;
}

BOOL CTKGamePlayer::OnInitialUpdate()
{
    return TRUE;
}

BOOL CTKGamePlayer::OnTickCount()
{
    return 0;
}

void CTKGamePlayer::OnDataBalance()
{
}

BOOL CTKGamePlayer::IsBotUser()
{
    return 0;
}

BOOL CTKGamePlayer::IsExitGame()
{
    return FALSE;
}

void CTKGamePlayer::__Initialize(int seat, int chip)
{
    m_dwUserID = seat;
    m_nSeatOrder = seat;
    m_nScore = chip;
}

int CTKGamePlayer::GetTax(TKSTAGERULER* pStageRuler)
{
    return 0;
}

BOOL CTKGamePlayer::AddAtomGameEvent(DWORD dwEventID, DWORD dwEventValue, BOOL bImmediate, int nOverride)
{
    return 0;
}
