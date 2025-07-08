#pragma once
#include "framework.h"
#include "TKDllGame.h"
#include "TKDllObject.h"
#include "TKClassFunctor.h"
#include "TKBuffer.h"
#include <vector>
#include <cstdint>

class CFuncObjCallback
{
public:
    virtual ~CFuncObjCallback(void)
    {
    }

public:
    virtual BOOL Execute(void* pParam)
    {
        return FALSE;
    }
};

////////////////////////

#define TK_OBJECTCLASS_MAXLEN 64

typedef void (*UNREGISTERINPFACTORY)(void);

class CTKDllServer
{
    //public:
    //    CTKDllServer();
    //    virtual ~CTKDllServer();
    //
    //public:
    //    virtual BOOL OnInitialUpdate();
    //    virtual BOOL OnRegisterTKObject();
    //    virtual CTKGame* OnCreateGame();
    //    virtual CTKGamePlayer* OnCreateTKGamePlayer();
    //
    //public:
    //    BOOL PostMsg2Pivot_GSBase(PTKHEADER in_pMsgReq, DWORD in_dwUserID, DWORD in_dwMatchID, WORD in_wStageID, WORD in_wRoundID,
    //        CFuncObjCallback* in_pFuncObjCallback  // 堆对象指针，调用前分配堆空间
    //    );
    //
    //    void RegisterInpFactoryUnRegFunction(UNREGISTERINPFACTORY pFunc);

public:
    int m_nGameID = 0;      // 游戏ID
    int m_nPort = 0;        //Listen Port
    int m_nBotSrvPort = 0;  //Bot服务端口

public:
    CTKDllServer(void);
    virtual ~CTKDllServer(void);

public:
    //BOOL Init(HDLLGAMESERVER hExeGameServer, FUNC_DLLGAME_CALLENTRY* ppCallEntry);
    //BOOL DoFuncCall(PTKDLLGAMECALLPARAM lpCallParam, const char* pszFile, int nLine, char* Func);
    //BOOL DoEventCall(PTKDLLGAMECALLPARAM lpCallParam);
    //static BOOL WINAPI OnEventCall(PTKDLLGAMECALLPARAM lpCallParam);

public:
    virtual BOOL OnInitialUpdate();
    virtual BOOL OnRegisterTKObject();
    virtual CTKGamePlayer* OnCreateTKGamePlayer();
    virtual CTKGame* OnCreateGame();

public:
    virtual BOOL OnTimeout(struct tm* pTime);
    //virtual BOOL OnSetProp(PTKDLLSERVERPROP pProp);

public:
    void RegisterVersion(const char* pszVersionName, const char* pszVersionValue) {};

public:
    BOOL PostMsg2Pivot_GSBase(PTKHEADER in_pMsgReq, DWORD in_dwUserID, DWORD in_dwMatchID, WORD in_wStageID, WORD in_wRoundID,
        CFuncObjCallback* in_pFuncObjCallback  // 堆对象指针，调用前分配堆空间
    );
    BOOL OnCallback(DWORD dwCallback, void* pParam);

    BOOL PostMsg2MS(PTKHEADER pMsg, int nType, DWORD dwParam, WORD wParam, DWORD dwSerialGroup);

public:
    BOOL PostMsg2HttpBrokerSvr(const LPVOID pData, DWORD dwLength);
    //nEKLLogLevel日志级别参考枚举eELKLogLevel，如果nProductID、dwTourneyID、dwMatchID、wStageID、wRoundID取不到，就填0
    /*enum eELKLogLevel{
		TK_ELK_LOG_LEVEL_OFF = 0,
		TK_ELK_LOG_LEVEL_FATAL = 1,
		TK_ELK_LOG_LEVEL_ERROR = 2,
		TK_ELK_LOG_LEVEL_WARN = 3,
		TK_ELK_LOG_LEVEL_INFO = 4,
		TK_ELK_LOG_LEVEL_DEBUG = 5,
		TK_ELK_LOG_LEVEL_TRACE = 6,
		TK_ELK_LOG_LEVEL_ALL = 7
	};*/
    BOOL TKWriteELKLogFile(int nProductID, DWORD dwTourneyID, DWORD dwMatchID, WORD wStageID, WORD wRoundID, int nEKLLogLevel, LPCTSTR pszFmt, ...);
    // CMA 加/减统计指标
    BOOL CMAIncreaseCnt(const std::string& strName, DWORD dwMPID, DWORD dwGameID, int nIncrease = 1);
    // CMA Set 统计指标
    BOOL CMASetCnt(const std::string& strName, DWORD dwMPID, DWORD dwGameID, int value = 0);

public:
    //// For 捕鱼
    //BOOL GenericPostMsg(GENERICPOSTMSGPARAMS* pParams, PTKHEADER pMsg);
    ////返回值表示是否继续广播到已加载其他DLL实例，TRUE：继续广播;FALSE:停止广播
    //virtual BOOL OnGenericPostMsg(GENERICPOSTMSGPARAMS* pParams, PTKHEADER pMsg, PTKHEADER pAck);
    ////dwSrvID == 0;//表示未找到dwUserIP、wEnterPort对应的dwSrvID
    ////dwSrvID != 0;//表示dwUserIP、wEnterPort与dwSrvID对应
    ////返回值表示是否继续广播到已加载其他DLL实例，TRUE：继续广播;FALSE:停止广播
    //virtual BOOL OnAnyUserMsg(DWORD dwSrvID, DWORD dwUserIP, WORD wEnterPort, WORD wOrigine, PTKHEADER pMsg);

    //获取RoundID对应的Player实例
    CTKGamePlayer* GetRoundPlayer(DWORD dwUserID, DWORD dwMatchID, WORD wStageID, WORD wRoundID);
    //获取MatchAgent中对应的Player实例
    CTKGamePlayer* GetMatchPlayer(DWORD dwUserID, DWORD dwMatchID);
    // game dll -> shell -> gsbase->gameround->onRemoteAckMsg
    //BOOL OnRemoteAckMsg(PTKHEADER pMsg, DWORD dwUserID, DWORD dwContext, PFUNC_CALLBACK in_pFuncCallback, DWORD dwMatchID, WORD wStageID, WORD wRoundID);

public:
    //private:
    //    CTKDllStat m_clsStat;
    //
    //private:
    //    CTKFixList<UNREGISTERINPFACTORY> m_listInpFactoryUnRegFunc;

public:
    void RegisterInpFactoryUnRegFunction(UNREGISTERINPFACTORY pFunc);

private:
    //HDLLGAMESERVER m_hExeGameServer;
    //FUNC_DLLGAME_CALLENTRY m_pCallEntry;
};

class CTKStat
{
public:
    CTKStat();
    ~CTKStat();

    void AddStat3(int nType1, int nType2, int nType3, LONGLONG ll0, LONGLONG ll1 = 0, LONGLONG ll2 = 0, LONGLONG ll3 = 0, LONGLONG ll4 = 0, LONGLONG ll5 = 0, LONGLONG ll6 = 0, LONGLONG ll7 = 0,
        LONGLONG ll8 = 0, LONGLONG ll9 = 0, int nDomain = 0);
};

extern CTKDllServer* TKCreateDllServer();

#define IMPLEMENT_CREATEDLLSERVER(type) \
    CTKDllServer* TKCreateDllServer()   \
    {                                   \
        return new type;                \
    }

extern CTKDllServer* tk_pDllServer;
extern CTKStat* tk_pStatMgr;

#define TK_OBJECTCLASS_MAXLEN 64

//泛型工厂注册类
#define REGISTER_TKCLASS(BASE_CLASS, DERIVED_CLASS) RegisterInpFactory<BASE_CLASS, DERIVED_CLASS> Register##DERIVED_CLASS(#DERIVED_CLASS)

#define CREATE_TKOBJECT(BASE_CLASS, DERIVED_CLASS) GenericFactory<BASE_CLASS>::instance().Create(#DERIVED_CLASS)
#define DESTROY_TKOBJECT(pObject) \
    if (pObject)                  \
    {                             \
        pObject->OnDestroy();     \
        pObject = NULL;           \
    }
#define TKOBJECT_LIST_DESTORY(list, type) \
    {                                     \
        int nCount = list.GetCount();     \
        for (int i = 0; i < nCount; i++)  \
        {                                 \
            type* pObj = list.GetAt(i);   \
            DESTROY_TKOBJECT(pObj);       \
        }                                 \
        list.RemoveAll();                 \
    }

template <class ManufacturedType>
class GenericFactory
{
    typedef ManufacturedType* (*BASEOBJECTCREATEFUNC)(const char*);

    typedef struct tagTKOBJTYPEMAP
    {
        std::string szClassName;
        BASEOBJECTCREATEFUNC pfnBaseObjectCreate;
    } TKOBJTYPEMAP, *PTKOBJTYPEMAP;

    std::vector<TKOBJTYPEMAP> m_listRegistry;

    GenericFactory()
    {
    }

    GenericFactory(const GenericFactory&)
    {
    }  // 没有实现

    GenericFactory& operator=(const GenericFactory&)
    {
    }  // 没有实现

public:
    static GenericFactory& instance()
    {
        static GenericFactory<ManufacturedType> bf;
        return bf;
    }

    void RegisterCreateFunc(const char* pszName, BASEOBJECTCREATEFUNC fn)
    {
        TKOBJTYPEMAP map{};
        map.szClassName = pszName;
        map.pfnBaseObjectCreate = fn;
        m_listRegistry.push_back(map);
    }

    void RemoveAllFunc()
    {
        m_listRegistry.clear();
    }

    ManufacturedType* Create(const char* pszName) const
    {
        int cnCount = m_listRegistry.size();
        for (int i = 0; i < cnCount; i++)
        {
            auto pMap = &m_listRegistry.at(i);
            if (pMap->szClassName == pszName)
            {
                return pMap->pfnBaseObjectCreate(pszName);
            }
        }
        return NULL;
    }
};

typedef void (*UNREGISTERINPFACTORY)(void);

template <class AncestorType, class ManufacturedType>
class RegisterInpFactory
{
public:
    static AncestorType* CreateInstance(const char* pszClassName)
    {
        AncestorType* pObj = new ManufacturedType;
        if (pObj)
        {
            pObj->OnCreate();
        }
        else
        {
            //TKTRACE("new: 0x%08X, %s(%d)\n",pObj,pszFile,nLine);
        }
        return pObj;
    }

    static void UnregisterInpFactory()
    {
        GenericFactory<AncestorType>::instance().RemoveAllFunc();
    }
    RegisterInpFactory(const char* pszName)
    {
        GenericFactory<AncestorType>::instance().RegisterCreateFunc(pszName, CreateInstance);
        tk_pDllServer->RegisterInpFactoryUnRegFunction(UnregisterInpFactory);
    }
};

////////////////////////
BOOL TKWriteLog(const char* pszFmt, ...);
BOOL TKWriteLogData(const void* pBuf, int len);
BOOL TKWriteOutOfMemory(const char* pszFile, int nLine, DWORD dwLength = -1);
BOOL TKWriteLogFile(const char* pszFile, LPCTSTR pszFmt, ...);
int TKWriteExceptionLog(const char* pszFile, int nLine, DWORD dwExceptionCode, LPEXCEPTION_POINTERS pep);

DWORD TKGetIniString(const char* pszSection, const char* pszEntry, const char* pszDefault, char* pszStr, DWORD dwSize, const char* pszIniFile = NULL);
BOOL TKWriteIniString(const char* pszSection, const char* pszEntry, const char* pszStr, const char* pszIniFile = NULL);
int TKGetIniInt(const char* pszSection, const char* pszEntry, int nDefault, const char* pszIniFile = NULL);
double TKGetIniPercent(const char* pszSection, const char* pszEntry, double dDefault, const char* pszIniFile = NULL);
double TKGetIniDouble(const char* pszSection, const char* pszEntry, double dDefault, const char* pszIniFile = NULL);
int TKGetIniIntCyc(const char* pszSection, const char* pszEntry, int nDefault, const char* pszIniFile = NULL, time_t tNow = 0);
BOOL TKWriteIniInt(const char* pszSection, const char* pszEntry, int n, const char* pszIniFile = NULL);
DWORD TKGetIniVersion(const char* pszSection, const char* pszEntry, DWORD dwDefault, const char* pszIniFile = NULL);
time_t TKGetIniLongTime(const char* pszSection, const char* pszEntry, DWORD dwDefault, const char* pszIniFile = NULL);
void TKDelIniKey(const char* pszSection, const char* pszEntry, const char* pszIniFile = NULL);

#define HGAMEOBJECT void*
void FreeExeBuffer(HGAMEOBJECT hObjBuf);

enum eGs2MsMsgType
{
    TK_GS2MS_MSGTYPE_NULL = 0,    //
    TK_GS2MS_MSGTYPE_PLAYER = 1,  //
    TK_GS2MS_MSGTYPE_GAME = 2,    //
    TK_GS2MS_MSGTYPE_ROUND = 3,   //
    TK_GS2MS_MSGTYPE_STAGE = 4,   //
    TK_GS2MS_MSGTYPE_MATCH = 5,   //
};

extern HMODULE tk_hDllModule;

typedef tkclsfunctor::function_base_ex<BOOL, PTKHEADER, DWORD>* PFUNC_CALLBACK;
