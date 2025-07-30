#include "StdAfx.h"
#include "TKJunQiCore.h"

CTKJunQiCore::CTKJunQiCore(std::shared_ptr<CTKJunQiPKCfg> pCfg, int nPlayerCount, int nCrossCount)
    : m_pJunQiPKCfg(pCfg.get()), m_nPlayerCount(nPlayerCount), m_nCrossCount(nCrossCount)
{
    m_nSideCrossCount = m_nCrossCount / m_nPlayerCount;
    m_chess.resize(m_nCrossCount);
    m_chessBoard.resize(m_nCrossCount);
    _initChessBoard();
}

CTKJunQiCore::~CTKJunQiCore()
{
    m_pJunQiPKCfg = NULL;
}

BOOL CTKJunQiCore::InitLayout(int nSeat, const string& szLayOut)
{
    boost::tokenizer<boost::char_separator<char>> token(szLayOut, boost::char_separator<char>(","));
    vector<int> vChess;
    for (auto iter = token.begin(); iter != token.end(); ++iter)
    {
        int nChess = boost::lexical_cast<int>(*iter);
        vChess.push_back(nChess);
    }
    if (!_checkLayout(vChess))
    {
        LOG_ERROR("CheckLayout layout = %s", szLayOut.c_str());
        return FALSE;
    }
    for (int i = 0; i < SIDE_CHESS_COUNT; ++i)
    {
        int nPos = g_nChessPos[i] + m_nSideCrossCount * nSeat;
        std::shared_ptr<SChess> pChess = std::make_shared<SChess>();
        pChess->type = (EChessType)vChess[i];
        pChess->seat = nSeat;
        pChess->show = true;
        m_chess[nPos] = pChess;
    }
    return TRUE;
}

BOOL CTKJunQiCore::CanFightTo(int nAtPos, int nToPos)
{
    //不能和行营中的任何棋子交战
    if (ECT_XY == m_chessBoard[nToPos].type)
    {
        return FALSE;
    }
    if (NULL == m_chess[nAtPos].get() || NULL == m_chess[nToPos].get() || false == m_chess[nToPos]->show)
    {
        return FALSE;
    }
    if (_isAlly(m_chess[nAtPos]->seat, m_chess[nToPos]->seat))
    {
        return FALSE;
    }

    return TRUE;
}

bool CTKJunQiCore::ShowChess(int nPos, std::shared_ptr<SChess>& pChess)
{
    return true;
}

BOOL CTKJunQiCore::CheckDBYSelect(int nPos)
{
    return ECT_DBY != m_chessBoard[nPos].type;
}

bool CTKJunQiCore::IsOver(int& nTips, int nSeat)
{
    if (JUNQIPK_UNKNOWN == GetChessPos(nSeat, ECT_JunQi))
    {
        nTips = ETS_CarryJunQi;
        return true;
    }

    return false;
}

void CTKJunQiCore::CodeChess()
{
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL != m_chess[i].get())
        {
            m_chess[i]->index = i;
        }
    }
}

void CTKJunQiCore::GetCanMovePos(int nPos, set<int>& sPos)
{
    EnumerIteratorContainerT cont(m_chessBoard, nPos, this);
    auto iterEnd = cont.end();
    for (auto iter = cont.begin(); iter != iterEnd; ++iter)
    {
        const vector<int>& vPath = iter->FromTo();
        for (int i = 0; i < vPath.size(); ++i)
        {
            if (nPos != vPath[i])
            {
                sPos.insert(vPath[i]);
            }
        }
    }
}

void CTKJunQiCore::MoveChess(int nAtPos, int nToPos, int nResult)
{
    if (NULL == m_chess[nToPos].get() || EFR_Win == nResult)
    {
        std::swap(m_chess[nAtPos], m_chess[nToPos]);
    }
    else if (EFR_PerishTogether == nResult)
    {
        m_chess[nToPos].reset();
    }
    m_chess[nAtPos].reset();
}

void CTKJunQiCore::SetPlayerColor(int nSeat, int nColor)
{
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL == m_chess[i].get())
        {
            continue;
        }
        if (m_chess[i]->color == nColor)
        {
            m_chess[i]->seat = nSeat;
        }
    }
}

void CTKJunQiCore::GetInitPos(InitChessAck* pAck)
{
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL != m_chess[i].get())
        {
            pAck->add_pos(i);
        }
    }
}

void CTKJunQiCore::ClearChess(int nSeat)
{
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL != m_chess[i].get() && m_chess[i]->seat == nSeat)
        {
            m_chess[i].reset();
        }
    }
}

BOOL CTKJunQiCore::CanSelect(int nPos, int nSeat)
{
    if (nPos < 0 || nPos >= m_nCrossCount)
    {
        return FALSE;
    }
    if (NULL == m_chess[nPos].get())
    {
        return FALSE;
    }
    if (nSeat != m_chess[nPos]->seat)
    {
        return FALSE;
    }
    if (ECT_JunQi == m_chess[nPos]->type)
    {
        return FALSE;
    }

    return CheckDBYSelect(nPos);
}

BOOL CTKJunQiCore::CheckPath(const CJunQiPath& path, int turnedCount)
{
    if (NULL == m_chess[path.From()].get())
    {
        return FALSE;
    }

    //未亮明的棋子不能动
    if (false == m_chess[path.From()]->show)
    {
        return FALSE;
    }

    //地雷和军旗不能动
    if (ECT_JunQi == m_chess[path.From()]->type || ECT_DiLei == m_chess[path.From()]->type)
    {
        return FALSE;
    }

    //路途中不能有棋子
    const vector<int>& vPath = path.FromTo();
    int nWayCount = vPath.size() - 1;
    for (int i = 1; i < nWayCount; ++i)
    {
        if (NULL != m_chess[vPath[i]].get())
        {
            return FALSE;
        }
    }

    //不能走到无法交战的棋子
    if (NULL != m_chess[path.To()].get() && (!CanFightTo(path.From(), path.To())))
    {
        return FALSE;
    }

    //只有工兵才能拐弯
    if (ECT_GongBing != m_chess[path.From()]->type && turnedCount)
    {
        return FALSE;
    }
    return TRUE;
}

bool CTKJunQiCore::CanMoveTo(int nAtPos, int nToPos, vector<int>& vPath)
{
    if (nAtPos < 0 || nAtPos >= m_nCrossCount || nToPos < 0 || nToPos >= m_nCrossCount)
    {
        return false;
    }
    if (NULL == m_chess[nAtPos].get())
    {
        return false;
    }
    EnumerIteratorContainerT cont(m_chessBoard, nAtPos, this);
    for (auto iter = cont.begin(); iter != cont.end(); ++iter)
    {
        if (nToPos == iter->To())
        {
            vPath = iter->FromTo();
            return true;
        }
    }
    return false;
}

BOOL CTKJunQiCore::HasCanMoveChess(int nSeat)
{
    set<int> vPos;
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL == m_chess[i].get())
        {
            continue;
        }
        if (!m_chess[i]->show)
        {
            return TRUE;
        }

        if (m_chess[i]->seat != nSeat)
        {
            continue;
        }
        GetCanMovePos(i, vPos);
        if (!vPos.empty())
        {
            return TRUE;
        }
    }
    return FALSE;
}

int CTKJunQiCore::GetFightResult(int nAtType, int nToType)
{
    //有炸弹的情况
    if ((ECT_ZhaDan == nAtType) || (ECT_ZhaDan == nToType))
    {
        return EFR_PerishTogether;
    }

    //有地雷的情况
    if (ECT_DiLei == nToType)
    {
        if (GT_FANFANQI == m_pJunQiPKCfg->GetCfg(ECI_GameType))
        {
            return EFR_Win;
        }
        return ECT_GongBing == nAtType ? EFR_Win : EFR_Loss;
    }

    if (nAtType == nToType)
    {
        return EFR_PerishTogether;
    }

    return nAtType > nToType ? EFR_Win : EFR_Loss;
}

int CTKJunQiCore::SwapChessWhenColorConfirm(vector<CTKJunQiPlayer*> players)
{
    for (size_t seat = 0; seat < players.size(); seat++)
    {
        CTKJunQiPlayer* player = players[seat];
        if (player)
        {
            for (size_t i = 0; i < m_chess.size(); i++)
            {
                SChess* pChess = m_chess[i].get();
                if (pChess && pChess->color == player->GetLastColor())
                {
                    pChess->seat = player->m_nSeatOrder;
                }
            }
        }
    }
    return 0;
}

int CTKJunQiCore::GetChessPos(int nSeat, int nType)
{
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL == m_chess[i].get())
        {
            continue;
        }
        if (m_chess[i]->seat == nSeat && nType == m_chess[i]->type)
        {
            return i;
        }
    }
    return JUNQIPK_UNKNOWN;
}

void CTKJunQiCore::_initChessBoard()
{
    _initCrossType();
    _initRoad();
    _initRailway();
}

void CTKJunQiCore::_initCrossType()
{
    //先全部置兵站
    for (int i = 0; i < m_chessBoard.size(); ++i)
    {
        m_chessBoard[i].type = ECT_BZ;
    }
    //行营
    for (int side = 0; side < m_nPlayerCount; ++side)
    {
        static const int crossList[] = {11, 13, 17, 21, 23};
        for (int cross = 0; cross < _countof(crossList); ++cross)
        {
            m_chessBoard[crossList[cross] + side * m_nSideCrossCount].type = ECT_XY;
        }
    }

    //大本营
    for (int side = 0; side < m_nPlayerCount; ++side)
    {
        static const int crossList[] = {1, 3};
        for (int cross = 0; cross < _countof(crossList); ++cross)
        {
            m_chessBoard[crossList[cross] + side * m_nSideCrossCount].type = ECT_DBY;
        }
    }
}

void CTKJunQiCore::_initRoad()
{
    AddRoad(0, "1,5");
    AddRoad(2, "1,7,3");
    AddRoad(4, "3,9");
    AddRoad(5, "0,11");
    AddRoad(6, "1,11");
    AddRoad(7, "11,12,13,2");
    AddRoad(8, "3,13");
    AddRoad(9, "13,4");
    AddRoad(10, "11");
    AddRoad(11, "5,6,7,10,12,15,16,17");
    AddRoad(12, "7,11,13,17");
    AddRoad(13, "7,8,9,12,14,17,18,19");
    AddRoad(14, "13");
    AddRoad(15, "21,16,11");
    AddRoad(16, "11,15,17,21");
    AddRoad(17, "11,12,13,16,18,21,22,23");
    AddRoad(18, "13,17,19,23");
    AddRoad(19, "23,18,13");
    AddRoad(20, "21");
    AddRoad(21, "15,16,17,20,22,25,26,27");
    AddRoad(22, "17,21,23,27");
    AddRoad(23, "17,18,19,22,24,27,28,29");
    AddRoad(24, "23");
    AddRoad(25, "21");
    AddRoad(26, "21");
    AddRoad(27, "21,22,23");
    AddRoad(28, "23");
    AddRoad(29, "23");

    for (int cross = 0; cross < m_nSideCrossCount; ++cross)
    {
        std::set<int>& road0 = m_chessBoard[cross].road;
        for (int side = 1; side < m_nPlayerCount; ++side)
        {
            std::set<int>& road = m_chessBoard[cross + m_nSideCrossCount * side].road;
            road.clear();
            for (auto iter = road0.begin(); iter != road0.end(); ++iter)
            {
                road.insert(*iter + m_nSideCrossCount * side);
            }
        }
    }
}

void CTKJunQiCore::AddRoad(int cross, const string& toCrossList)
{
    boost::tokenizer<boost::char_separator<char>> token(toCrossList, boost::char_separator<char>(","));
    for (auto iter = token.begin(); iter != token.end(); ++iter)
    {
        int toCross = boost::lexical_cast<int>(*iter);
        m_chessBoard[cross].road.insert(toCross);
    }
}

void CTKJunQiCore::_initRailway()
{
    m_chessBoard[5].railway[s] = 10;
    m_chessBoard[5].railway[y] = 6;

    m_chessBoard[6].railway[z] = 5;
    m_chessBoard[6].railway[y] = 7;

    m_chessBoard[7].railway[z] = 6;
    m_chessBoard[7].railway[y] = 8;

    m_chessBoard[8].railway[z] = 7;
    m_chessBoard[8].railway[y] = 9;

    m_chessBoard[9].railway[z] = 8;
    m_chessBoard[9].railway[s] = 14;

    m_chessBoard[10].railway[s] = 15;
    m_chessBoard[10].railway[x] = 5;

    m_chessBoard[14].railway[s] = 19;
    m_chessBoard[14].railway[x] = 9;

    m_chessBoard[15].railway[s] = 20;
    m_chessBoard[15].railway[x] = 10;

    m_chessBoard[19].railway[s] = 24;
    m_chessBoard[19].railway[x] = 14;

    m_chessBoard[20].railway[s] = 25;
    m_chessBoard[20].railway[x] = 15;

    m_chessBoard[24].railway[s] = 29;
    m_chessBoard[24].railway[x] = 19;

    m_chessBoard[25].railway[y] = 26;
    m_chessBoard[25].railway[x] = 20;

    m_chessBoard[26].railway[y] = 27;
    m_chessBoard[26].railway[z] = 25;

    m_chessBoard[27].railway[y] = 28;
    m_chessBoard[27].railway[z] = 26;

    m_chessBoard[28].railway[y] = 29;
    m_chessBoard[28].railway[z] = 27;

    m_chessBoard[29].railway[z] = 28;
    m_chessBoard[29].railway[x] = 24;
}

BOOL CTKJunQiCore::_checkLayout(const vector<int>& vChess)
{
    int nCount[ECT_ChessTypeCount] = {0};
    for (auto iter = vChess.begin(); iter != vChess.end(); ++iter)
    {
        if (*iter >= ECT_ZhaDan && *iter < ECT_ChessTypeCount)
        {
            nCount[*iter]++;
        }
    }

    //检查棋子个数
    for (int i = 0; i < ECT_ChessTypeCount; ++i)
    {
        if (g_nChessCount[i] != nCount[i])
        {
            return FALSE;
        }
    }

    //炸弹不能在第一排
    for (int i = SIDE_CHESS_COUNT - 5; i < SIDE_CHESS_COUNT; ++i)
    {
        if (ECT_ZhaDan == vChess[i])
        {
            return FALSE;
        }
    }

    //地雷只能放在最后二排
    int nDiLeiCount = 0;
    for (int i = 0; i < 10; ++i)
    {
        if (ECT_DiLei == vChess[i])
        {
            ++nDiLeiCount;
        }
    }
    if (3 != nDiLeiCount)
    {
        return FALSE;
    }

    //军旗只能在大本营
    return ECT_JunQi == vChess[1] || ECT_JunQi == vChess[3];
}

bool CTKJunQiCore::_isAlly(int lSeat, int rSeat)
{
    int nSeatDiff = abs(lSeat - rSeat);
    return 0 == nSeatDiff % 2;
}

CTKJunQiPKAQ::CTKJunQiPKAQ(std::shared_ptr<CTKJunQiPKCfg> pCfg) : CTKJunQiCore(pCfg, 2, 60)
{
}

CTKJunQiPKAQ::~CTKJunQiPKAQ()
{
}

void CTKJunQiPKAQ::OnInitialUpdate()
{
    for (int cross = 0; cross < m_nSideCrossCount; ++cross)
    {
        std::map<CRailway, int>& railway0 = m_chessBoard[cross].railway;
        std::map<CRailway, int>& railway = m_chessBoard[cross + m_nSideCrossCount].railway;
        for (auto iter = railway0.begin(); iter != railway0.end(); ++iter)
        {
            CRailway rRailWay = iter->first;
            rRailWay.RotateCounterClockwise(2);
            railway[rRailWay] = iter->second + m_nSideCrossCount;
        }
    }

    m_chessBoard[25].railway[s] = 59;
    m_chessBoard[27].railway[s] = 57;
    m_chessBoard[29].railway[s] = 55;

    m_chessBoard[55].railway[x] = 29;
    m_chessBoard[57].railway[x] = 27;
    m_chessBoard[59].railway[x] = 25;
}

CTKJunQiPKFFQ::CTKJunQiPKFFQ(std::shared_ptr<CTKJunQiPKCfg> pCfg)
    : CTKJunQiPKAQ(pCfg), m_nlastColor(JUNQIPK_UNKNOWN), m_nRedCount(0), m_nBlueCount(0)
{
}

CTKJunQiPKFFQ::~CTKJunQiPKFFQ()
{
}

void CTKJunQiPKFFQ::OnInitialUpdate()
{
    __super::OnInitialUpdate();

    //翻翻棋大本营可以走
    AddRoad(1, "0,6,2");
    AddRoad(3, "2,8,4");
    AddRoad(31, "30,36,32");
    AddRoad(33, "32,38,34");

    char szIniFile[MAX_PATH] = ".\\InitChess_JunQiPK.ini";
    if (1 == GetPrivateProfileInt("InitChess", "EnableInitChess", 0, szIniFile))
    {
        _initChess(szIniFile, "RedChess", EC_Red);
        _initChess(szIniFile, "BlueChess", EC_Blue);
    }
    else
    {
        for (int i = 0; i < 10; ++i)
        {
            m_chess.clear();
            m_chess.resize(m_nCrossCount);

            _initChessRandom();
            if (!_isNeedRetry())
            {
                break;
            }
        }
    }
}

BOOL CTKJunQiPKFFQ::InitLayout(int nSeat, const string& szLayOut)
{
    return TRUE;
}

void CTKJunQiPKFFQ::initChess(vector<std::shared_ptr<SChess>>& vChess, const int* nChess)
{
    for (int i = 0; i < ECT_ChessTypeCount; ++i)
    {
        for (int nColor = EC_Blue; nColor <= EC_Red; ++nColor)
        {
            for (int j = 0; j < nChess[i]; ++j)
            {
                std::shared_ptr<SChess> pChess = std::make_shared<SChess>();
                pChess->type = (EChessType)i;
                pChess->color = nColor;
                pChess->show = false;
                pChess->seat = nColor;
                vChess.push_back(pChess);
            }
        }
    }
}

void CTKJunQiPKFFQ::_initChess(char* szIniFile, char* szKey, int nColor)
{
    char szBuffer[1024] = {0};
    TKGetIniString("InitChess", szKey, "", szBuffer, sizeof(szBuffer), szIniFile);

    vector<string> vStr;
    _split(szBuffer, ",", vStr);
    for (int i = 0; i < SIDE_CHESS_COUNT * 2 && i < vStr.size(); ++i)
    {
        vector<string> vChessInfo;
        _split(vStr[i], "-", vChessInfo);
        if (2 != vChessInfo.size())
        {
            return;
        }
        UINT nPos = boost::lexical_cast<UINT>(vChessInfo[0]);
        if (nPos > m_nCrossCount)
        {
            return;
        }
        int nType = _chessString2Int(vChessInfo[1]);
        if (nType > ECT_SiLing)
        {
            return;
        }
        m_chess[nPos] = std::make_shared<SChess>();
        m_chess[nPos]->color = nColor;
        m_chess[nPos]->type = (EChessType)nType;
        m_chess[nPos]->show = false;
    }
}

bool CTKJunQiPKFFQ::_isUnpos(const int* nUnpos, int nLen, int nPos)
{
    for (int i = 0; i < nLen; ++i)
    {
        if (nPos == nUnpos[i])
        {
            return true;
        }
    }
    return false;
}

bool CTKJunQiPKFFQ::_isUnposByType(int* nUnpos, int nPos, int nType)
{
    if (nType == ECT_DiLei)
    {
        return _isUnpos(nUnpos, g_nDileiUnPosLen, nPos);
    }
    else if (nType == ECT_JunQi)
    {
        return _isUnpos(g_nJunqiUnpos, ARRAYSIZE(g_nJunqiUnpos), nPos);
    }
    return false;
}

void CTKJunQiPKFFQ::_initChessRandom()
{
    vector<std::shared_ptr<SChess>> vChess;
    initChess(vChess);

    vector<int> vCross;
    initCross(vCross);

    vector<std::shared_ptr<SChess>> vUChess;
    vector<int> vUCross;

    vector<std::shared_ptr<SChess>> vFindChess;
    vFindChess.clear();
    vector<int> vUnPos;
    vUnPos.clear();

    //确定军旗位置
    vFindChess.clear();
    _getChessByType(vChess, vUChess, vFindChess, ECT_JunQi, EC_All);
    vUnPos.clear();
    vUnPos = vector<int>(g_nJunqiUnpos, g_nJunqiUnpos + ARRAYSIZE(g_nJunqiUnpos));
    m_chess[_getChessPos(vCross, vUCross, vUnPos)] = vFindChess[0];
    m_chess[_getChessPos(vCross, vUCross, vUnPos)] = vFindChess[1];

    //确定司令位置
    vFindChess.clear();
    _getChessByType(vChess, vUChess, vFindChess, ECT_SiLing, EC_All);
    vUnPos.clear();
    vUnPos = vector<int>(g_nSilingUnpos, g_nSilingUnpos + ARRAYSIZE(g_nSilingUnpos));
    m_chess[_getChessPos(vCross, vUCross, vUnPos)] = vFindChess[0];
    m_chess[_getChessPos(vCross, vUCross, vUnPos)] = vFindChess[1];

    //确定地雷位置
    vFindChess.clear();
    _getChessByType(vChess, vUChess, vFindChess, ECT_DiLei, EC_All);
    vector<int> vDileiPos;
    vDileiPos.clear();
    _getDileiChessPos(vCross, vUCross, vDileiPos, vFindChess.size());
    for (int i = 0; i < vFindChess.size(); i++)
    {
        m_chess[vDileiPos[i]] = vFindChess[i];
    }

    //确定16个关键位置
    _setKeyPos(vChess, vUChess, vCross, vUCross);

    //确定剩余位置
    vUnPos.clear();
    for (int i = 0; i < vChess.size(); i++)
    {
        m_chess[_getChessPos(vCross, vUCross, vUnPos)] = vChess[i];
    }

    for (int i = vChess.size() - 1; i >= 0; i--)
    {
        vUChess.push_back(vChess[i]);
        vChess.erase(vChess.begin() + i);
    }

#if _DEBUG
    assert(_checkChessRandom());
#endif
}

void CTKJunQiPKFFQ::_getChessByType(vector<std::shared_ptr<SChess>>& vChess, vector<std::shared_ptr<SChess>>& vUChess,
                                    vector<std::shared_ptr<SChess>>& _vChess, EChessType eChessType,
                                    EChessColor eChessColor)
{
    int nChessCount = g_nChessCount[eChessType];
    vector<int> vPopChess;
    if (EC_All == eChessColor)
    {
        nChessCount = nChessCount * 2;
    }
    int nCount = 0;
    for (int i = 0; i < vChess.size(); i++)
    {
        if (NULL == vChess[i].get())
        {
            continue;
        }
        BOOL bIsPush = FALSE;
        bIsPush = (EC_All == eChessColor && vChess[i]->type == eChessType) ||
                  (EC_All != eChessColor && vChess[i]->type == eChessType && vChess[i]->color == eChessColor);

        if (bIsPush)
        {
            _vChess.push_back(vChess[i]);
            vPopChess.push_back(i);
            vUChess.push_back(vChess[i]);
            nCount++;
            if (nCount >= nChessCount)
            {
                break;
            }
        }
    }

    for (int i = vPopChess.size() - 1; i >= 0; i--)
    {
        vChess.erase(vChess.begin() + vPopChess[i]);
    }
}

void CTKJunQiPKFFQ::_getChessRandom(vector<std::shared_ptr<SChess>>& vChess, vector<std::shared_ptr<SChess>>& vUChess,
                                    std::shared_ptr<SChess>& pChess, EChessColor eChessColor)
{
    int nCount = vChess.size();
    int nRandNum = TKGenRandom() % nCount;
    if (eChessColor == EC_All)
    {
        pChess = vChess[nRandNum];
        vChess.erase(vChess.begin() + nRandNum);
        vUChess.push_back(pChess);
    }
    else
    {
        vector<std::shared_ptr<SChess>> vRedChess;
        vector<std::shared_ptr<SChess>> vBlueChess;
        for (int i = 0; i < vChess.size(); i++)
        {
            if (vChess[i]->color == EC_Red)
            {
                vRedChess.push_back(vChess[i]);
            }
            else
            {
                vBlueChess.push_back(vChess[i]);
            }
        }

        if (eChessColor == EC_Red)
        {
            nCount = vRedChess.size();
            nRandNum = TKGenRandom() % nCount;
            pChess = vRedChess[nRandNum];
        }
        else
        {
            nCount = vBlueChess.size();
            nRandNum = TKGenRandom() % nCount;
            pChess = vBlueChess[nRandNum];
        }

        for (int i = 0; i < vChess.size(); i++)
        {
            if (&(*vChess[i]) == &(*pChess))
            {
                vChess.erase(vChess.begin() + i);
                vUChess.push_back(pChess);
                break;
            }
        }
    }
}

int CTKJunQiPKFFQ::_getChessPos(vector<int>& vCross, vector<int>& vUCross, const vector<int>& vUnPos)
{
    vector<int> vUPos;
    for (int i = 0; i < vCross.size(); i++)
    {
        BOOL bIsIn = FALSE;
        for (int j = 0; j < vUnPos.size(); j++)
        {
            if (vCross[i] == vUnPos[j])
            {
                bIsIn = TRUE;
                break;
            }
        }
        if (!bIsIn)
        {
            vUPos.push_back(vCross[i]);
        }
    }
    int nCount = vUPos.size();
    int nNum = TKGenRandom() % nCount;
    int nPos = vUPos[nNum];
    for (int i = 0; i < vCross.size(); i++)
    {
        if (vCross[i] == nPos)
        {
            vCross.erase(vCross.begin() + i);
            vUCross.push_back(nPos);
            break;
        }
    }
    return nPos;
}

void CTKJunQiPKFFQ::_getDileiChessPos(vector<int>& vCross, vector<int>& vUCross, vector<int>& vDileiPos,
                                      int nDileiCount)
{
    int nDileiNum = 0;
    int nDileiKeyPosCount = 0;
    for (int i = 0; i < nDileiCount; i++)
    {
        bool bContinueFindPos = true;

        do
        {
            int nCount = vCross.size();
            int nNum = TKGenRandom() % nCount;
            int nPos = vCross[nNum];
            for (int j = 0; j < ARRAYSIZE(g_nDileiUnpos); j++)
            {
                if (nPos == g_nDileiUnpos[j])
                {
                    break;
                }
            }
            if (nDileiKeyPosCount < g_nDileiUnPosLen)
            {
                //
                BOOL isIn = _diLeiIsInSpecialPos(nPos);

                if (isIn)
                {
                    continue;
                }

                nDileiKeyPosCount++;

                vCross.erase(vCross.begin() + nNum);
                vUCross.push_back(nPos);
                vDileiPos.push_back(nPos);
                nDileiNum++;

                break;
            }
            else
            {
                bContinueFindPos = false;
                break;
            }
        } while (true);

        if (!bContinueFindPos)
        {
            break;
        }
    }

    int nNeedDilei = nDileiCount - nDileiNum;
    if (nNeedDilei <= 0)
    {
        return;
    }
    vector<int> vUnPos = vector<int>(g_nDileiUnpos, g_nDileiUnpos + ARRAYSIZE(g_nDileiUnpos));
    for (int i = 0; i < nNeedDilei; i++)
    {
        do
        {
            int nPos = _getChessPos(vCross, vUCross, vUnPos);
            BOOL isIn = _diLeiIsInSpecialPos(nPos);

            if (isIn)
            {
                vCross.push_back(nPos);
                auto iter = std::find(vUCross.begin(), vUCross.end(), nPos);
                vUCross.erase(iter);

                continue;
            }

            vDileiPos.push_back(nPos);
            break;

        } while (true);
    }
}

BOOL CTKJunQiPKFFQ::_diLeiIsInSpecialPos(int nDiLeiPos)
{
    BOOL junQiIsIn = _isInSpecialPos(ECT_JunQi);

    if (FALSE == junQiIsIn)
    {
        return FALSE;
    }

    for (int i = 0; i < EC_All; ++i)
    {
        int nPos = _getChessPos(ECT_JunQi, i);

        for (int i = 0; i < sizeof(g_nSpecialPos) / sizeof(g_nSpecialPos[0]); ++i)
        {
            if (nPos == g_nSpecialPos[i])
            {
                int liberty = 0;
                bool isHasDileiPos = false;

                std::set<int>& road = m_chessBoard[nPos].road;
                for (auto iter = road.begin(); iter != road.end(); ++iter)
                {
                    if (NULL == m_chess[*iter].get() || m_chess[*iter]->type == ECT_SiLing)
                    {
                        liberty++;
                        if (*iter == nDiLeiPos)
                        {
                            isHasDileiPos = true;
                        }
                    }
                }

                if (true == isHasDileiPos && liberty <= 1)
                {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

int CTKJunQiPKFFQ::_getChessPos(int type, int color)
{
    for (int i = 0; i < m_chess.size(); ++i)
    {
        if (NULL != m_chess[i].get())
        {
            if (m_chess[i]->color == color && m_chess[i]->type == type)
            {
                return i;
            }
        }
    }
    return JUNQIPK_UNKNOWN;
}

bool CTKJunQiPKFFQ::_isNeedRetry()
{
    if (!_checkChessNumInHalfLayout() || !_checkKeyPos() || !_checkSpecialCtrlPos())
    {
        return true;
    }

    return false;
}

bool CTKJunQiPKFFQ::_checkChessNumInHalfLayout()
{
    enum layout
    {
        layout_down = 0,
        layout_up = 1,
        layout_all = 2,
    };
    std::array<std::array<int, EC_All>, layout_all> colorStatis{};

    for (int i = 0; i < m_chess.size(); ++i)
    {
        if (nullptr == m_chess[i])
        {
            continue;
        }

        if (i < m_chess.size() / layout_all)
        {
            if (EC_Blue == m_chess[i]->color || EC_Red == m_chess[i]->color)
            {
                colorStatis[layout_down][m_chess[i]->color]++;
            }
        }
        else
        {
            if (EC_Blue == m_chess[i]->color || EC_Red == m_chess[i]->color)
            {
                colorStatis[layout_up][m_chess[i]->color]++;
            }
        }
    }

    for (int layoutIdx = layout_down; layoutIdx < layout_all; ++layoutIdx)
    {
        for (int color = EC_Blue; color < EC_All; ++color)
        {
            if (colorStatis[layoutIdx][color] <= 5)
            {
                return false;
            }
        }
    }
    return true;
}

bool CTKJunQiPKFFQ::_checkKeyPos()
{
    int blueNum = 0;
    int redNum = 0;
    for (int i = 0; i < ARRAYSIZE(g_nKeyPos); i++)
    {
        if (EC_Blue == m_chess[g_nKeyPos[i]]->color)
        {
            blueNum++;
        }
        else if (EC_Red == m_chess[g_nKeyPos[i]]->color)
        {
            redNum++;
        }
    }

    if (blueNum <= 4 || redNum <= 4)
    {
        return false;
    }
    return true;
}

bool CTKJunQiPKFFQ::_checkSpecialCtrlPos()
{
    for (auto it = g_specialCtrlPos.begin(); it != g_specialCtrlPos.end(); ++it)
    {
        int mainPos = it->first;
        if (ECT_DiLei == m_chess[mainPos]->type || ECT_JunQi == m_chess[mainPos]->type)
        {
            continue;
        }

        int notMoveChessNum = 0;
        std::vector<int> vecPos = it->second;
        for (int i = 0; i < vecPos.size(); ++i)
        {
            int pos = vecPos[i];
            if (ECT_DiLei == m_chess[pos]->type || ECT_JunQi == m_chess[pos]->type)
            {
                notMoveChessNum++;
            }
        }

        if (notMoveChessNum >= 3)
        {
            return false;
        }
    }

    return true;
}

BOOL CTKJunQiPKFFQ::_isInSpecialPos(int type)
{
    for (int i = 0; i < sizeof(g_nSpecialPos) / sizeof(g_nSpecialPos[0]); ++i)
    {
        int nPos = g_nSpecialPos[i];
        if (m_chess.size() > nPos)
        {
            if (NULL != m_chess[nPos].get() && type == m_chess[nPos]->type)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

void CTKJunQiPKFFQ::_setKeyPos(vector<std::shared_ptr<SChess>>& vChess, vector<std::shared_ptr<SChess>>& vUChess,
                               vector<int>& vCross, vector<int>& vUCross)
{
    vector<int> vUsedPos;
    int nRedCount = 0;
    int nBlueCount = 0;
    for (int i = 0; i < ARRAYSIZE(g_nKeyPos); ++i)
    {
        int nPos = g_nKeyPos[i];
        if (NULL == m_chess[nPos].get())
        {
            continue;
        }
        if (EC_Red == m_chess[nPos]->color)
        {
            ++nRedCount;
        }
        else
        {
            ++nBlueCount;
        }
        vUsedPos.push_back(nPos);
    }
    vector<int> vUnusePos;
    for (int i = 0; i < ARRAYSIZE(g_nKeyPos); i++)
    {
        BOOL isIn = FALSE;
        for (int j = 0; j < vUsedPos.size(); j++)
        {
            if (g_nKeyPos[i] == vUsedPos[j])
            {
                isIn = TRUE;
                break;
            }
        }
        if (!isIn)
        {
            vUnusePos.push_back(g_nKeyPos[i]);
        }
    }
    int redDiff = g_nKeyPosChessNum - nRedCount;
    int blueDiff = g_nKeyPosChessNum - nBlueCount;
    vector<std::shared_ptr<SChess>> _vChess;
    _vChess.clear();
    for (int i = 0; i < redDiff; i++)
    {
        std::shared_ptr<SChess> pChess;
        _getChessRandom(vChess, vUChess, pChess, EC_Red);
        _vChess.push_back(pChess);
    }
    for (int i = 0; i < blueDiff; i++)
    {
        std::shared_ptr<SChess> pChess;
        _getChessRandom(vChess, vUChess, pChess, EC_Blue);
        _vChess.push_back(pChess);
    }

    vector<int> vUsedPosIndex;
    vUsedPosIndex.clear();
    for (int i = 0; i < _vChess.size(); i++)
    {
        int nCount = vUnusePos.size();
        int nIdx = TKGenRandom() % nCount;
        int nPos = vUnusePos[nIdx];
        vUnusePos.erase(vUnusePos.begin() + nIdx);
        m_chess[nPos] = _vChess[i];
        for (int j = 0; j < vCross.size(); j++)
        {
            if (vCross[j] == nPos)
            {
                vUsedPosIndex.push_back(j);
                vUCross.push_back(nPos);
                break;
            }
        }
    }

    std::sort(vUsedPosIndex.begin(), vUsedPosIndex.end());
    for (int i = vUsedPosIndex.size() - 1; i >= 0; i--)
    {
        vCross.erase(vCross.begin() + vUsedPosIndex[i]);
    }
}

BOOL CTKJunQiPKFFQ::_checkChessRandom()
{
    for (int i = 0; i < ARRAYSIZE(g_nJunqiUnpos); i++)
    {
        if (ECT_JunQi == m_chess[g_nJunqiUnpos[i]]->type)
        {
            LOG_ERROR("unexpected junqi position");
            return FALSE;
        }
    }

    for (int i = 0; i < ARRAYSIZE(g_nSilingUnpos); i++)
    {
        if (ECT_SiLing == m_chess[g_nSilingUnpos[i]]->type)
        {
            LOG_ERROR("unexpected siling position");
            return FALSE;
        }
    }

    int nDileiNum = 0;
    for (int i = 0; i < ARRAYSIZE(g_nDileiUnpos); i++)
    {
        if (ECT_DiLei == m_chess[g_nDileiUnpos[i]]->type)
        {
            if (++nDileiNum > g_nDileiUnPosLen)
            {
                LOG_ERROR("key position dilei(%d) more than %d", nDileiNum, g_nDileiUnPosLen);
                return FALSE;
            }
        }
    }

    int nRedChessNum = 0;
    int nBlueChessNum = 0;
    for (int i = 0; i < ARRAYSIZE(g_nKeyPos); i++)
    {
        if (EC_Red == m_chess[g_nKeyPos[i]]->color)
        {
            nRedChessNum++;
        }
        else
        {
            nBlueChessNum++;
        }
    }
    if (nRedChessNum != g_nKeyPosChessNum || nBlueChessNum != g_nKeyPosChessNum)
    {
        LOG_ERROR("red or blue chess is not equal to %d in sixteen key position chess", g_nKeyPosChessNum);
        return FALSE;
    }

    // check chess color
    int nRChessNumArray[ECT_ChessTypeCount] = {0};
    int nBChessNumArray[ECT_ChessTypeCount] = {0};
    for (int i = 0; i < m_chess.size(); i++)
    {
        if (m_chess[i].get() == NULL)
        {
            continue;
        }
        if (m_chess[i]->color == EC_Red)
        {
            nRChessNumArray[m_chess[i]->type]++;
        }
        else if (m_chess[i]->color == EC_Blue)
        {
            nBChessNumArray[m_chess[i]->type]++;
        }
    }
    for (int i = 0; i < ECT_ChessTypeCount; i++)
    {
        if (nRChessNumArray[i] != g_nChessCount[i])
        {
            LOG_ERROR("chess type %d red count is not right %d %d", i, nRChessNumArray[i], g_nChessCount[i]);
            return FALSE;
        }
        if (nBChessNumArray[i] != g_nChessCount[i])
        {
            TKWriteLog("chess type %d blue count is not right %d %d", i, nBChessNumArray[i], g_nChessCount[i]);
            return FALSE;
        }
    }

    return TRUE;
}

void CTKJunQiPKFFQ::_split(const string& s, const string& delim, vector<string>& ret)
{
    boost::tokenizer<boost::char_separator<char>> token(s, boost::char_separator<char>(delim.c_str()));
    for (auto iter = token.begin(); iter != token.end(); ++iter)
    {
        ret.push_back(*iter);
    }
}

int CTKJunQiPKFFQ::_chessString2Int(const string& str)
{
    static string szChess[ECT_ChessTypeCount] = {"炸", "雷", "旗", "兵", "排", "连",
                                                 "营", "团", "旅", "师", "军", "司"};
    for (int i = 0; i < ECT_ChessTypeCount; ++i)
    {
        if (szChess[i] == str)
        {
            return i;
        }
    }
    return ECT_ChessTypeCount;
}

void CTKJunQiPKFFQ::initChess(vector<std::shared_ptr<SChess>>& vChess)
{
    initChess(vChess, g_nChessCount);
}

void CTKJunQiPKFFQ::initCross(vector<int>& vCross)
{
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (ECT_XY != m_chessBoard[i].type)
        {
            vCross.push_back(i);
        }
    }
}

BOOL CTKJunQiPKFFQ::canCarryJunQi(int nAtPos, int nToPos)
{
    if (JUNQIPK_UNKNOWN != GetChessPos(m_chess[nToPos]->seat, ECT_DiLei))
    {
        return FALSE;
    }
    if (AnyOneCarryJunQi == m_pJunQiPKCfg->GetCfg(ECI_CarryJunQi))
    {
        return TRUE;
    }
    return _bLeastChess(m_chess[nAtPos]->seat, m_chess[nAtPos]->type);
}

bool CTKJunQiPKFFQ::ShowChess(int nPos, std::shared_ptr<SChess>& pChess)
{
    if (NULL == m_chess[nPos].get() || m_chess[nPos]->show)
    {
        return false;
    }
    int nSwapPos = JUNQIPK_UNKNOWN;
    if (_isNeedSwap(nPos, nSwapPos))
    {
        std::swap(m_chess[nPos], m_chess[nSwapPos]);
    }
    _swapDismoveBigChess(nPos);
    pChess = m_chess[nPos];
    pChess->show = true;
    _setShowInfo(pChess->type, pChess->color);
    return true;
}

void CTKJunQiPKFFQ::_swapDismoveBigChess(int nPos)
{
    if (m_chess[nPos]->type < ECT_ShiZhang)
    {
        return;
    }

    const SCross& rCross = m_chessBoard.at(nPos);
    if (!rCross.railway.empty())
    {
        return;
    }

    for (auto iter = rCross.road.begin(); iter != rCross.road.end(); ++iter)
    {
        if (m_chessBoard[*iter].type == ECT_XY)
        {
            return;
        }
        if (NULL == m_chess[*iter].get())
        {
            return;
        }
        if (m_chess[*iter]->type != ECT_DiLei && m_chess[*iter]->type != ECT_JunQi)
        {
            return;
        }
    }
    _swapChess(nPos, m_chess[nPos]->color);
}

void CTKJunQiPKFFQ::_swapChess(int nPos, int nColor)
{
    int nCount = nColor == EC_Red ? m_nRedCount : m_nBlueCount;
    int nLeftCount = SIDE_CHESS_COUNT - nCount;
    int nSwapIndex = TKGenRandom() % nLeftCount;
    int nIndex = 0;
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL != m_chess[i].get() && m_chess[i]->color == nColor && (!m_chess[i]->show))
        {
            if (nIndex >= nSwapIndex && m_chess[i]->type < ECT_ShiZhang)
            {
                std::swap(m_chess[nPos], m_chess[i]);
                return;
            }
            ++nIndex;
        }
    }
}

bool CTKJunQiPKFFQ::_isNeedSwap(int nPos, int& nSwapPos)
{
    int nShowCount = m_nRedCount + m_nBlueCount;
    if (nShowCount > SWAP_SHOW_COUNT)
    {
        return false;
    }
    int nDiff = abs(m_nRedCount - m_nBlueCount);
    if (nDiff < SWAP_CHESS_NUM)
    {
        return false;
    }

    int nUnswap = nDiff == SWAP_CHESS_NUM ? UNSWAP_PERCENT2 : UNSWAP_PERCENT1;
    if ((TKGenRandom() % 100) < nUnswap)
    {
        return false;
    }

    int nNeedColor = EC_Red;
    int nLeftCount = SIDE_CHESS_COUNT - m_nRedCount;
    if (m_nRedCount > m_nBlueCount)
    {
        nNeedColor = EC_Blue;
        nLeftCount = SIDE_CHESS_COUNT - m_nBlueCount;
    }
    if (m_chess[nPos]->color == nNeedColor)
    {
        return false;
    }

    return _getSwapPos(nLeftCount, nNeedColor, nSwapPos);
}

bool CTKJunQiPKFFQ::_getSwapPos(int nLeftCount, int nNeedColor, int& nSwapPos)
{
    int nSwapIndex = TKGenRandom() % (nLeftCount - 1);
    int nIndex = 0;
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL != m_chess[i].get() && nNeedColor == m_chess[i]->color && (!m_chess[i]->show))
        {
            if (nIndex >= nSwapIndex && m_chess[i]->type != ECT_JunQi)
            {
                nSwapPos = i;
                return true;
            }
            ++nIndex;
        }
    }
    return false;
}

BOOL CTKJunQiPKFFQ::CheckDBYSelect(int nPos)
{
    return TRUE;
}

void CTKJunQiPKFFQ::_setShowInfo(int nType, int nColor)
{
    if (EC_Red == nColor)
    {
        ++m_nRedCount;
    }
    else
    {
        ++m_nBlueCount;
    }
}

BOOL CTKJunQiPKFFQ::CanFightTo(int nAtPos, int nToPos)
{
    if (!__super::CanFightTo(nAtPos, nToPos))
    {
        return FALSE;
    }

    if (ECT_DiLei == m_chess[nToPos]->type)
    {
        return _canCleanDiLei(m_chess[nAtPos]->seat, m_chess[nAtPos]->type);
    }

    //必须把雷挖完才能扛军旗
    if (ECT_JunQi == m_chess[nToPos]->type)
    {
        return canCarryJunQi(nAtPos, nToPos);
    }

    //小子不能攻击大子
    if (m_chess[nAtPos]->type >= ECT_GongBing && m_chess[nToPos]->type >= ECT_GongBing)
    {
        return m_chess[nAtPos]->type >= m_chess[nToPos]->type;
    }
    return TRUE;
}

BOOL CTKJunQiPKFFQ::_bLeastChess(int nSeat, int nType)
{
    for (int i = 0; i < m_nCrossCount; ++i)
    {
        if (NULL == m_chess[i].get() || m_chess[i]->seat != nSeat)
        {
            continue;
        }
        if (m_chess[i]->type < nType && ECT_GongBing <= m_chess[i]->type)
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CTKJunQiPKFFQ::_canCleanDiLei(int nSeat, int nType)
{
    //只有工兵(或最小子)和炸弹能攻击地雷
    if (ECT_ZhaDan == nType)
    {
        return TRUE;
    }
    if (GongBingCleanDiLei == m_pJunQiPKCfg->GetCfg(ECI_CleanDiLei))
    {
        return ECT_GongBing == nType;
    }
    return _bLeastChess(nSeat, nType);
}

CTKJunQiSiGuo::CTKJunQiSiGuo(std::shared_ptr<CTKJunQiPKCfg> pCfg) : CTKJunQiCore(pCfg, 4, 129)
{
}

CTKJunQiSiGuo::~CTKJunQiSiGuo()
{
}

void CTKJunQiSiGuo::OnInitialUpdate()
{
    m_chessBoard[25].railway[sz] = 125;
    m_chessBoard[25].railway[s] = 30;

    m_chessBoard[27].railway[s] = 31;

    m_chessBoard[29].railway[sy] = 57;
    m_chessBoard[29].railway[s] = 62;

    m_chessBoard[30].railway[s] = 127;
    m_chessBoard[30].railway[x] = 25;
    m_chessBoard[30].railway[z] = 125;
    m_chessBoard[30].railway[y] = 31;

    m_chessBoard[31].railway[s] = 128;
    m_chessBoard[31].railway[x] = 27;
    m_chessBoard[31].railway[z] = 30;
    m_chessBoard[31].railway[y] = 62;

    _setOtherRailway();

    m_chessBoard[128].railway[s] = 95;
    m_chessBoard[128].railway[x] = 31;
    m_chessBoard[128].railway[z] = 127;
    m_chessBoard[128].railway[y] = 63;
}

void CTKJunQiSiGuo::_setOtherRailway()
{
    int nMidPoint = m_nCrossCount - 1;
    for (int side0Cross = 0; side0Cross < m_nSideCrossCount; ++side0Cross)
    {
        std::map<CRailway, int>& railway0 = m_chessBoard[side0Cross].railway;
        for (int side = 1; side < m_nPlayerCount; ++side)
        {
            std::map<CRailway, int>& railway = m_chessBoard[side0Cross + side * m_nSideCrossCount].railway;
            for (auto iter = railway0.begin(); iter != railway0.end(); ++iter)
            {
                CRailway rRailWay = iter->first;
                rRailWay.RotateCounterClockwise(side);
                int nPos = iter->second;
                if (iter->second < nMidPoint)
                {
                    nPos = (nPos + m_nSideCrossCount * side) % (nMidPoint);
                }
                railway[rRailWay] = nPos;
            }
        }
    }
}
