#pragma once
#include "TKJunQiPK.pb.h"
#include "TKJunQiPKCfg.h"
#include "TKJunQiPlayer.h"
#include "TKJunQiUtils.h"

using namespace cn::jj::service::msg::protocol;

class CTKJunQiCore
{
    typedef CEnumerIteratorContainer<CPathEnumer<CTKJunQiCore>> EnumerIteratorContainerT;

public:
    CTKJunQiCore(std::shared_ptr<CTKJunQiPKCfg> pCfg, int nPlayerCount, int nCrossCount);
    virtual ~CTKJunQiCore();

public:
    virtual void OnInitialUpdate() = 0;
    virtual BOOL InitLayout(int nSeat, const string& szLayOut);  //��ʼ����Ҳ���
    virtual BOOL CanFightTo(int nAtPos, int nToPos);             //������������Ƿ�ɶ�ս
    virtual bool ShowChess(int nPos, std::shared_ptr<SChess>& pChess);
    virtual BOOL CheckDBYSelect(int nPos);
    virtual bool IsOver(int& nTips, int nSeat);
    virtual bool NeedShowJunQi()
    {
        return true;
    }
    virtual bool Recordlayout()
    {
        return true;
    }
    virtual bool InitContinue()
    {
        return false;
    }

public:
    void CodeChess();  //��ÿ�����ӱ��
    void GetCanMovePos(int nPos, set<int>& sPos);
    void MoveChess(int nAtPos, int nToPos, int nResult);
    void SetPlayerColor(int nSeat, int nColor);
    void GetInitPos(InitChessAck* pAck);
    void ClearChess(int nSeat);

    BOOL CanSelect(int nPos, int nSeat);                         //��������Ƿ��ѡ��
    BOOL CheckPath(const CJunQiPath& path, int turnedCount);     //���·���Ƿ���ƶ�,ö�ٵ����������
    bool CanMoveTo(int nAtPos, int nToPos, vector<int>& vPath);  //����ܷ��ƶ�
    BOOL HasCanMoveChess(int nSeat);                             //����Ƿ��п��ƶ�������

    int GetFightResult(int nAtType, int nToType);  //�������ӵ�ս�����
    int GetChessPos(int nSeat, int nType);
    int SwapChessWhenColorConfirm(vector<CTKJunQiPlayer*> players);

    std::shared_ptr<SChess> GetChess(int nPos)
    {
        return m_chess[nPos];
    }

protected:
    void AddRoad(int cross, const string& toCrossList);

private:
    void _initChessBoard();  //��ʼ������
    void _initCrossType();   //��ʼ���ڵ�����
    void _initRoad();        //��ʼ����·
    void _initRailway();     //��ʼ����·

    BOOL _checkLayout(const vector<int>& vChess);  //��鲼��
    bool _isAlly(int lSeat, int rSeat);

protected:
    vector<std::shared_ptr<SChess>> m_chess;  //����
    vector<SCross> m_chessBoard;              //����
    CTKJunQiPKCfg* m_pJunQiPKCfg;             //����

    int m_nPlayerCount;
    int m_nCrossCount;
    int m_nSideCrossCount;
};

class CTKJunQiPKAQ : public CTKJunQiCore
{
public:
    CTKJunQiPKAQ(std::shared_ptr<CTKJunQiPKCfg> pCfg);
    virtual ~CTKJunQiPKAQ();

public:
    virtual void OnInitialUpdate();
};

class CTKJunQiPKFFQ : public CTKJunQiPKAQ
{
public:
    CTKJunQiPKFFQ(std::shared_ptr<CTKJunQiPKCfg> pCfg);
    virtual ~CTKJunQiPKFFQ();

public:
    virtual void OnInitialUpdate();
    virtual BOOL InitLayout(int nSeat, const string& szLayOut);  //��ʼ����Ҳ���
    virtual BOOL CanFightTo(int nAtPos, int nToPos);
    virtual bool ShowChess(int nPos, std::shared_ptr<SChess>& pChess);
    virtual BOOL CheckDBYSelect(int nPos);
    virtual bool NeedShowJunQi()
    {
        return false;
    }
    virtual bool Recordlayout()
    {
        return false;
    }
    virtual bool InitContinue()
    {
        return true;
    }

protected:
    virtual void initChess(vector<std::shared_ptr<SChess>>& vChess);
    virtual void initCross(vector<int>& vCross);
    virtual BOOL canCarryJunQi(int nAtPos, int nToPos);

    void initChess(vector<std::shared_ptr<SChess>>& vChess, const int* nChess);

private:
    void _initChess(char* szIniFile, char* szKey, int nColor);
    void _initChessRandom();
    void _getChessByType(vector<std::shared_ptr<SChess>>& vChess, vector<std::shared_ptr<SChess>>& vUChess,
                         vector<std::shared_ptr<SChess>>& _vChess, EChessType eChessType, EChessColor eChessColor);
    void _getChessRandom(vector<std::shared_ptr<SChess>>& vChess, vector<std::shared_ptr<SChess>>& vUChess,
                         std::shared_ptr<SChess>& pChess, EChessColor eChessColor);
    int _getChessPos(vector<int>& vCross, vector<int>& vUCross, const vector<int>& vUnPos);
    void _getDileiChessPos(vector<int>& vCross, vector<int>& vUCross, vector<int>& vDileiPos, int nDileiCount);
    void _setKeyPos(vector<std::shared_ptr<SChess>>& vChess, vector<std::shared_ptr<SChess>>& vUChess,
                    vector<int>& vCross, vector<int>& vUCross);
    BOOL _checkChessRandom();
    bool _isUnpos(const int* nUnpos, int nLen, int nPos);
    bool _isUnposByType(int* nUnpos, int nPos, int nType);
    void _split(const string& s, const string& delim, vector<string>& ret);
    int _chessString2Int(const string& str);

    void _swapDismoveBigChess(int nPos);
    void _swapChess(int nPos, int nColor);
    bool _isNeedSwap(int nPos, int& nSwapPos);
    bool _getSwapPos(int nLeftCount, int nNeedColor, int& nSwapPos);

    void _setShowInfo(int nType, int nColor);
    BOOL _canCleanDiLei(int nSeat, int nType);
    BOOL _bLeastChess(int nSeat, int nType);

    BOOL _diLeiIsInSpecialPos(int nDiLeiPos);
    BOOL _isInSpecialPos(int type);

    int _getChessPos(int type, int color);

    bool _isNeedRetry();
    bool _checkChessNumInHalfLayout();
    bool _checkKeyPos();
    bool _checkSpecialCtrlPos();

private:
    int m_nlastColor;
    int m_nRedCount;   //��ɫ����������
    int m_nBlueCount;  //��ɫ����������
};

//�Ĺ�����
class CTKJunQiSiGuo : public CTKJunQiCore
{
public:
    CTKJunQiSiGuo(std::shared_ptr<CTKJunQiPKCfg> pCfg);
    virtual ~CTKJunQiSiGuo();

public:
    virtual void OnInitialUpdate();

private:
    void _setOtherRailway();
};
