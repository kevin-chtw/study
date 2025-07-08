#pragma once

#include <stdint.h>
#include "TKProtocol.h"
//#include "ms_mvs_gs_mc_share_define.h"
#include "TKMatchS2GameSProtocolSrv.h"
//#include "TKSvrShareDef.h"

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�ȡ����ǰ�� ���¿�ʼ
//   ˵    ����MGR -> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RESETCURGAME (TK_MSG_GAME + 22)
typedef struct tagTKREQGAMERESETCURGAME
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
} TKREQGAMERESETCURGAME, *PTKREQGAMERESETCURGAME;
//ACK:TKHEADER

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�����BotServer����Bot
//   ˵    ����GameServer --> BotServer
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2BOTS_CREATEBOT64 (TK_MSG_GAME + 29)
typedef struct tagTKREQGAMECREATEBOT64
{
    TKHEADER header;

    char szGameServerIP[TK_BUFLEN_IP];
    int nGameServerPort;

    DWORD dwUserID;
    int nAILevel;  //������ֻ��ˮƽ,0��ͣ�Խ��AIԽ��

    DWORD dwMatchID;
    DWORD dwCheck;

    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;
    int64_t nScore;  //��������

    DWORD dwFlags;
    int nGameID;
    DWORD dwProductID;
    BOOL bPacerBot;     // ��������
    DWORD dwAIVersion;  //AI�汾��
    BYTE byAIType;      //������������
    BYTE byReserve[3];
    DWORD dwReserve[4];
} TKREQGAMECREATEBOT64, *PTKREQGAMECREATEBOT64;
//ACK:TKHEADER
#define TKID_GS2BOTS_CREATEBOT (TK_MSG_GAME + 30)
typedef struct tagTKREQGAMECREATEBOT
{
    TKHEADER header;

    char szGameServerIP[TK_BUFLEN_IP];
    int nGameServerPort;

    DWORD dwUserID;
    int nAILevel;  //������ֻ��ˮƽ,0��ͣ�Խ��AIԽ��

    DWORD dwMatchID;
    DWORD dwCheck;

    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;
    int nScore;  //��������

    DWORD dwFlags;
    int nGameID;
    DWORD dwProductID;
    BOOL bPacerBot;     // ��������
    DWORD dwAIVersion;  //AI�汾��
    BYTE byAIType;      //������������
    BYTE byReserve[3];
    DWORD dwReserve[4];
} TKREQGAMECREATEBOT, *PTKREQGAMECREATEBOT;
//ACK:TKHEADER

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨGS AI�㷨�����쳣
//   ˵    ����BotServer --> GameServer
//
//-----------------------------------------------------------------------------------------
#define TKID_BOTS2GS_AIDISPATCH_ERROR (TK_MSG_GAME + 31)
typedef struct tagTKREQAIDISPATCHERROR
{
    TKHEADER header;
    DWORD dwMatchID;
} TKREQAIDISPATCHERROR, *PTKREQAIDISPATCHERROR;
//ACK:TKHEADER

/*////////////////////////////

��Ϸ������Ϣ����-Player���룺
Game <- REQ Enter <------------------- Client
Game -> Ack Enter -------------------> Client
Game -> Ack PlayerInfo(Send����) ----> Client
Game -> Ack PlayerArrived(Broast) ---> Client
Game -> Ack BeginHand(Broast) -------> Client

��Ϸ������Ϣ����-Player���ߣ�
Game -> Ack PlayerNetBreak(Broast) --> Client

��Ϸ������Ϣ����-Player������
Game <- REQ Enter <------------------- Client
Game -> Ack Enter -------------------> Client
Game -> Ack PlayerInfo(Send����) ----> Client
Game -> Ack PlayerArrived(Broast) ---> Client
Game -> Ack PlayerNetResume(Broast) -> Client

/*////////////////////////////

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������ͻ����������������Ϸ������
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_ENTERMATCH (TK_MSG_GAME + 70)
typedef struct tagTKREQCLIENTENTENTERMATCH
{
    TKHEADER header;
    DWORD dwVersionMatchClient;  //�����ͻ��˰汾
    int lenEnterParam;
    char szEnterParam[sizeof(ENTERROUNDPARAM) + 8];
    DWORD dwVersionGameDll;  //��Ϸ��̬��汾
} TKREQCLIENTENTENTERMATCH, *PTKREQCLIENTENTENTERMATCH;
typedef struct tagTKACKCLIENTENTENTERMATCH
{
    TKHEADER header;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    char szMatchName[TK_BUFLEN_TOURNEYFULLNAME];  // ��������

    DWORD dwUserID;  //�û��ʺŶ�Ӧ��Ψһ���ֱ��ID
    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;   //�û�ͷ��ID
    int nMatchRank;  //��Ϸ���������ݱ������ּ��������

    int nTotalMatchPlayer;  //�ܲ�������
    time_t timeMatchStart;  //������ʼʱ��
    int nMatchLapseSecond;  //�����Ѿ����е�����

    int nTitleStyle;  //�����������ͣ�0��������ʾΪ����������+�׶�/�ִ����ơ���1��������ʾΪ����%d�֡�
    int nProductID;
    DWORD dwTeamID;  //�û�����һ��������TeamID,�����������ͽ���೵������һ�����ǲ�ͬ��TeamID,�Թ�Ϊ0
    DWORD dwFormID;  //�û�����ID

    time_t timeStamp;  // ��Ϸ������ʱ���
} TKACKCLIENTENTENTERMATCH, *PTKACKCLIENTENTENTERMATCH;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������ͻ���ֱ���˳�������Ϸ������
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_EXITMATCH (TK_MSG_GAME + 71)
typedef struct tagTKREQCLIENTEXITMATCH : public tagTKREQCLIENTENTENTERMATCH
{
} TKREQCLIENTEXITMATCH, *PTKREQCLIENTEXITMATCH;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ������͵�ǰ�������еĽ׶��ִκ�����
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHSTAGEPOSITION (TK_MSG_GAME + 72)
typedef struct tagTKACKGSPUSHMATCHSTAGEPOSITION
{
    TKHEADER header;
    WORD wCurStageID;
    int nCurBoutID;
    char szStageBoutName[TK_BUFLEN_TOURNEYFULLNAME];
    char szStageName[TK_BUFLEN_STAGENAME];
    char szBoutName[TK_BUFLEN_STAGENAME];
} TKACKGSPUSHMATCHSTAGEPOSITION, *PTKACKGSPUSHMATCHSTAGEPOSITION;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ������ͱ���״�����ֲ���
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHTEXTPLAY (TK_MSG_GAME + 73)
typedef struct tagTKACKGSPUSHMATCHTEXTPLAY
{
    TKHEADER header;
    int nTextLength;  //�������ֳ���(����������\0)
} TKACKGSPUSHMATCHTEXTPLAY, *PTKACKGSPUSHMATCHTEXTPLAY;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ��������û���ǰ��������λ��
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHUSERPLACEORDERPOSITION (TK_MSG_GAME + 76)
typedef struct tagTKACKGSPUSHUSERPLACEORDERPOSITION
{
    TKHEADER header;
    int nPlayerCurPlaceOrder;  //�û���ǰ����
    int nTotalPlayerCount;     //��ǰ��ѡ����
} TKACKGSPUSHUSERPLACEORDERPOSITION, *PTKACKGSPUSHUSERPLACEORDERPOSITION;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ������͵�ǰ���ƹ�����Ϣ
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHROUNDRULERINFO (TK_MSG_GAME + 77)
typedef struct tagTKACKGSPUSHROUNDRULERINFO
{
    TKHEADER header;
    int lenText;  //�ı����� ����������\0
} TKACKGSPUSHROUNDRULERINFO, *PTKACKGSPUSHROUNDRULERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ��������û��佱��Ϣ
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHAWARD (TK_MSG_GAME + 78)
typedef struct tagTKACKGSPUSHMATCHAWARD
{
    TKHEADER header;
    int nTextLength;  //�佱���ֳ���(����������\0)
} TKACKGSPUSHMATCHAWARD, *PTKACKGSPUSHMATCHAWARD;
//header.dwParam==0 ������״����(��ȷ����ť��״�շŵ����·������رտͻ���)
//header.dwParam==1 ������״����(��ȷ����ť��رտͻ���)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ������ͱ���ѡ����Ϣ
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHPLAYERINFO (TK_MSG_GAME + 79)
typedef struct tagTKACKGSPUSHMATCHPLAYERINFO
{
    TKHEADER header;
    int nTextLength;  //ѡ����Ϣxml�ı�����(����������\0)
} TKACKGSPUSHMATCHPLAYERINFO, *PTKACKGSPUSHMATCHPLAYERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ���������Ϸ�����۽�����������Ϣ
//   ˵    ����GS --> GC
/*   xml��ʽ����
<GameCountAward Note="������Ϸ�����Ľ������߻��ּӱ���������Ϣ">
	<!--��������style˵����styleΪ1��ʾ����max�ͻص�min��2��ʾ����max��Ӧ����һ��ruler�����û����һ����Ӧ�����һ��-->
	<Item id="1" name="�콵ϲ��" style="1" Note="ÿ���8���ƾ��л������콵ϲ��">
		<Ruler min="0" max="8"/>
	</Item>
	<Item id="2" name="����ָ" style="1" Note="ÿ���8���ƾ��л�����1������ָ">
		<Ruler min="0" max="10"/>
	</Item>
	<Item id="3" name="��������" style="1" Note="ÿ���8���ƾ��л�����100����������">
		<Ruler min="0" max="10"/>
	</Item>
	<Item id="4" name="���ּӱ�" style="2" Note="ÿ���10���ƻ��ּӱ�,���4��">
		<Ruler name="1������" min="0" max="10"/>
		<Ruler name="2������" min="10" max="20"/>
		<Ruler name="3������" min="20" max="30"/>
		<Ruler name="4������" min="30" max="40"/>
	</Item>
</GameCountAward>
*/
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHGAMECOUNTAWARDINFO (TK_MSG_GAME + 80)
typedef struct tagTKACKGSPUSHGAMECOUNTAWARDINFO
{
    TKHEADER header;
    int lenText;  //xml�ı����� ����������\0
} TKACKGSPUSHGAMECOUNTAWARDINFO, *PTKACKGSPUSHGAMECOUNTAWARDINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ���������Ϸ������Ϣ
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHPLAYERGAMEDATA (TK_MSG_GAME + 81)
typedef struct tagTKACKGSPUSHPLAYERGAMEDATA
{
    TKHEADER header;

    DWORD dwUserID;

    int nExchangeRate;  // �һ���
    int nLeftChip;      // ʣ�������ߵ�ǰ����ֵ

    int iCurGameHand;             // �û���ǰ���еľ��Ʊ�ţ���%d�֣�����ʱ�ۼơ�Ϊ0�Ǳ�ʾû�д��������ʾ��
    int nTotalFinishedGameCount;  // �Ѿ���ɵ�����Ч���������ƽ���ʱ�ۼӡ����ھ���������ʾ��
    int nCurFinishedGameCount;    // ��ǰ�׶���ɵ���Ч���������ƽ���ʱ�ۼӡ����ڻ��ּӱ���ʾ��
} TKACKGSPUSHPLAYERGAMEDATA, *PTKACKGSPUSHPLAYERGAMEDATA;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������·��������ͻ��˵Ķ���xml�ı�
//   ˵    ����GS --> GC
/*
<action bmp="1">
	<stage name="Ԥ��" bmp="10" />
	<ruler name="��������" bmp="100"/>
	<note>
		<line>
			<Item str="24" cr="11,22,33"/>
			<Item str=" �˽���" cr="11,22,33"/>
			<Item str="    " cr="11,22,33"/>
			<Item str="30" cr="11,22,33"/>
			<Item str=" �˽�ֹ" cr="11,22,33"/>
		</line>
		<line>
			<Item str="ǰ3���л������콵�￨" cr="11,22,33"/>
		</line>
	</note>
</action>
*/
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHMATCHACTION (TK_MSG_GAME + 82)
typedef struct tagTKACKGSPUSHMATCHACTION
{
    TKHEADER header;
    int nActionID;
    int lenText;  //xml�ı����� ����������\0
} TKACKGSPUSHMATCHACTION, *PTKACKGSPUSHMATCHACTION;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ������ͽ׶����������Ϣ
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHSTAGEBOUTRESULT (TK_MSG_GAME + 83)
typedef struct tagTKACKGSPUSHSTAGEBOUTRESULT
{
    TKHEADER header;
    int lenText;  //�ı����� ����������\0
} TKACKGSPUSHSTAGEBOUTRESULT, *PTKACKGSPUSHSTAGEBOUTRESULT;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ������;��Ʒ���Ϣ
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHROUNDSCOREINFO (TK_MSG_GAME + 84)
typedef struct tagTKACKGSPUSHROUNDSCOREINFO
{
    TKHEADER header;
    int lenText;  //�ı����� ����������\0
} TKACKGSPUSHROUNDSCOREINFO, *PTKACKGSPUSHROUNDSCOREINFO;

////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�ͻ��˽���Round
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ENTERROUND (TK_MSG_GAME + 100)
typedef struct tagTKREQGAMEENTERROUND
{
    TKHEADER header;
    DWORD dwVersion;  //�ͻ��˰汾
    int lenEnterParam;
    char szEnterParam[sizeof(ENTERROUNDPARAM) + 8];
} TKREQGAMEENTERROUND, *PTKREQGAMEENTERROUND;
#define TK_NETOPERATOR_PLAYER 0      //�û�������Ϸ
#define TK_NETOPERATOR_BOT 1         //�����˲�����Ϸ
#define TK_ROUND_FLAG_HIDE_PLAYER 1  // ����
typedef struct tagTKACKGAMEPLAYERENTER
{
    TKHEADER header;
    DWORD dwUserID;           //�û��ʺŶ�Ӧ��Ψһ���ֱ��ID
    int nSeatOrder;           //��λ���
    int nUserType;            //�û����ͣ�0��Ϸ��Player��1����Wather��2�Թ���Spectator
    int nMaxPlayersPerTable;  //ÿ���������
    DWORD flag;
} TKACKGAMEPLAYERENTER, *PTKACKGAMEPLAYERENTER;
#define TK_GAMEPLAYERTYPE_PLAYER 0     //��Ϸ��
#define TK_GAMEPLAYERTYPE_WATCHER 1    //����
#define TK_GAMEPLAYERTYPE_SPECTATOR 2  //�Թ���

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·���Ϸ����
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RULERINFO (TK_MSG_GAME + 101)
typedef struct tagTKACKGAMERULERINFO
{
    TKHEADER header;
    int nGameTotalPlayer;                             //������Ϸ�ܵ������
    int nTableNumber;                                 //���ţ�����RoundID��
    int nScoreType;                                   //���ַ�ʽ��ԭ���֣����룩
    int nScoreBase;                                   //��Ϸ�������ȷ�400*1.60��ֻ�ڻ�ս��ʹ��
    char szRoundRulerName[TK_BUFLEN_ROUNDRULERNAME];  //��������

    int nReserve;                                //nHandTime;								//ÿ���Ƶ�ʱ�䣬�ȷ�15�룬���ڲ�֧����ʱ�䷽��
    char szProperty[TK_MAX_GAMEPROPLEN];         //��Ϸ��ظ������ԣ�������Ϸ������Ϸ���������н��ͣ��Ը�����Ϸ���ܻ���
    char szTitleName[TK_BUFLEN_CLIENTTITLE];     //�ͻ��˱����ַ���
    char szRulerNote[TK_BUFLEN_ROUNDRULERNOTE];  //��Ϸ���ƹ���˵����N�ֻ�������߻�ʤ / ����****�ֽ�����̭

    int nRoundRuler;  // ����
} TKACKGAMERULERINFO, *PTKACKGAMERULERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·�����Player��Ϣ���ͻ������յ��Լ��ģ����յ����˵ģ�
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDGAMEPLAYERINFO (TK_MSG_GAME + 102)
typedef struct tagTKGAMEPLAYERINFO
{
    int nSeatOrder;  //��λ���
    DWORD dwUserID;  //�û��ʺŶ�Ӧ��Ψһ���ֱ��ID
    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;   //�û�ͷ��ID
    int nMatchRank;  //��Ϸ���������ݱ������ּ��������

    int nScore;  //���л���

    BOOL bArrived;   //�Ƿ�ͻ����Ѿ����ӵ�������
    int nNetStatus;  //����״̬��0�������ӣ�1����
    DWORD dwFormID;  //�û�����ID
} TKGAMEPLAYERINFO, *PTKGAMEPLAYERINFO;
typedef struct tagTKACKGAMEADDPLAYERENTERINFO
{
    TKHEADER header;
    TKGAMEPLAYERINFO info;
} TKACKGAMEADDPLAYERENTERINFO, *PTKACKGAMEADDPLAYERENTERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·���Ϣ���������ָ��Player����Ϣ
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_DELGAMEPLAYERINFO (TK_MSG_GAME + 103)
typedef struct tagTKACKGAMEDELPLAYERENTERINFO
{
    TKHEADER header;
    DWORD dwUserID;
} TKACKGAMEDELPLAYERENTERINFO, *PTKACKGAMEDELPLAYERENTERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·���Ϣ���޸ı���ָ��Player����Ϣ
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CHGGAMEPLAYERINFO (TK_MSG_GAME + 104)
typedef struct tagTKACKGAMECHGPLAYERENTERINFO
{
    TKHEADER header;
    DWORD dwUserID;
    BOOL bArrived;   //�Ƿ�ͻ����Ѿ����ӵ�������
    int nNetStatus;  //����״̬��0�������ӣ�1����
} TKACKGAMECHGPLAYERENTERINFO, *PTKACKGAMECHGPLAYERENTERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�Player����û�������Ϸ��
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYERARRIVED (TK_MSG_GAME + 105)
typedef struct tagTKACKGAMEPLAYERARRIVED
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //��λ���
} TKACKGAMEPLAYERARRIVED, *PTKACKGAMEPLAYERARRIVED;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�Player�뿪���û��뿪��Ϸ��
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYERLEAVED (TK_MSG_GAME + 106)
typedef struct tagTKACKGAMEPLAYERLEAVED
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //��λ���
} TKACKGAMEPLAYERLEAVED, *PTKACKGAMEPLAYERLEAVED;

////////////////
//-----------------------------------------------------------------------------------------
//   ��    �ܣ�һ����Ϸ��ʼ����Ϣ
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_BEGINHAND (TK_MSG_GAME + 107)
//ACK:GameServer --> GameClient
typedef struct tagTKACKGAMEBEGINHAND
{
    TKHEADER header;
    int iGameHand;  //�ڼ���
    int nRound;     // �ڼ���
    int nTable;     // �ڼ���
} TKACKGAMEBEGINHAND, *PTKACKGAMEBEGINHAND;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�ͻ����������
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NETBREAK (TK_MSG_GAME + 108)
typedef struct tagTKACKGAMENETBREAK
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //��λ���
} TKACKGAMENETBREAK, *PTKACKGAMENETBREAK;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�ͻ�����������
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NETRESUME (TK_MSG_GAME + 109)
typedef struct tagTKACKGAMENETRESUME
{
    TKHEADER header;
    DWORD dwUserID;
    int nSeatOrder;  //��λ���
} TKACKGAMENETRESUME, *PTKACKGAMENETRESUME;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������ػ�ʱ����ʷ��Ϣ��ʼ
//   ˵    ����Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HISTORYMSGBEGIN (TK_MSG_GAME + 110)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������ػ�ʱ����ʷ��Ϣ����
//   ˵    ����Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HISTORYMSGEND (TK_MSG_GAME + 111)
typedef struct tagTKACKGAMEHISTORYMSGEND
{
    TKHEADER header;
    time_t timeCurServer;  //��������ǰʱ��
} TKACKGAMEHISTORYMSGEND, *PTKACKGAMEHISTORYMSGEND;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ƽ���(����������Ϸ����)
//   ˵    ����Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OVERGAME (TK_MSG_GAME + 112)
#define TK_MSG_GAME_ROUNDOVER (TK_MSG_GAME_OVERGAME)

////////////////
//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·��µ�ScoreBase
//   ˵    ����Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SCOREBASERAISE (TK_MSG_GAME + 113)
typedef struct tagTKACKGAMESCOREBASERAISE
{
    TKHEADER header;
    int nNewScoreBase;  //�µ���Ϸ����
} TKACKGAMESCOREBASERAISE, *PTKACKGAMESCOREBASERAISE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û�ǿ���˳���������������ϵͳ�й�
//   ˵    ����GC -> GS, REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_EXITGAME (TK_MSG_GAME + 114)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��޸���Ϸ���ƹ���˵��
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CHANGEGAMERULERNOTE (TK_MSG_GAME + 115)
typedef struct tagTKACKGAMECHANGERULERNOTE
{
    TKHEADER header;
    char szRulerNote[TK_BUFLEN_ROUNDRULERNOTE];  //��Ϸ���ƹ���˵����N�ֻ�������߻�ʤ / ����****�ֽ�����̭
} TKACKGAMECHANGERULERNOTE, *PTKACKGAMECHANGERULERNOTE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨGC����ֱ���˳�������
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NOQUERYEXITGAME (TK_MSG_GAME + 116)
typedef struct tagTKACKNOQUERYEXITGAME
{
    TKHEADER header;
    BOOL bNoQueryExitGame;
} TKACKNOQUERYEXITGAME, *PTKACKNOQUERYEXITGAME;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��ͻ�������������
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CHECKENTERROUND (TK_MSG_GAME + 117)
typedef struct tagTKREQGAMECHECKENTERROUND : public tagTKREQGAMEENTERROUND
{
} TKREQGAMECHECKENTERROUND, *PTKREQGAMECHECKENTERROUND;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·���Ϸ��չ����
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RULEREXINFO (TK_MSG_GAME + 118)
typedef struct tagTKACKGAMERULEREXINFO
{
    TKHEADER header;
    char szPropertyEx[TK_MAX_GAMEPROPEXLEN];  //��Ϸ��ظ������ԣ�������Ϸ������Ϸ���������н��ͣ��Ը�����Ϸ���ܻ���
} TKACKGAMERULEREXINFO, *PTKACKGAMERULEREXINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û����ÿ�������
//   ˵    ����GC -> GS, REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SHOWCARD (TK_MSG_GAME + 119)
typedef struct tagTkReqGamePlayerShowCard
{
    TKHEADER header;
    BOOL bShowCard;  //trueΪ���Ƹ��Թ���,��֮Ϊ������
} TKREQGAMEPLAYERSHOWCARD, *PTKREQGAMEPLAYERSHOWCARD;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�һ����Ϸ��������Ϣ
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HANDOVER (TK_MSG_GAME + 120)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�ڼ򵥻�����Ϣ��Ϣ
//   ˵    ����GC <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SIMPLEACTION (TK_MSG_GAME + 121)
typedef struct tagTkReqGameSimpleAction
{
    TKHEADER header;

    DWORD dwUserIDFrom;  // ������
    DWORD dwUserIDTo;    // �����ߣ�0��ʾ����������

    DWORD dwEmotionID;  // ����ID
    DWORD dwTextID;     // ����ID

    DWORD dwUserIDTarget;
    DWORD dwDonate;  // ����
} TKREQGAMESIMPLEACTION, *PTKREQGAMESIMPLEACTION;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��ͻ��˴�����Ϣ�ӳ�
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ONMSGDELAY (TK_MSG_GAME + 122)
typedef struct tagTkReqOnMsgDelay
{
    TKHEADER header;

    DWORD dwMsgType;
    DWORD dwLastMsgType;
    DWORD dwTimeRecv;
    DWORD dwTimeInvoke;
    DWORD dwTimeOnMsg;

} TKREQGAMEONMSGDELAY, *PTKREQGAMEONMSGDELAY;

//-----------------------------------------------------------------------------------------
//
// MS��GS���ͱ���������ʾ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PUSHUSERAWARDMESSAGE (TK_MSG_GAME + 123)
typedef struct tagTKACKGSPUSHPUSHUSERAWARDMESSAGE
{
    TKHEADER header;

    DWORD dwAwardType;  //���˵㽱�� \�콵ϲ��\�콵��Ʒ\��ʤ����\��ʤ����\���̽���\PK����\�ʽ���\������ʾ�ȣ��μ���TKMatchUniversalDicSrv.h�� eMatchAwardMessageType����

    DWORD dwXmlLength;  // ��׺XML�ĳ��ȣ������ַ���������'\0'
    DWORD dwXmlOffset;  // ��׺XML��ַƫ��������λ�ֽڣ����յ�Ϊ��Ϣ����ʼλ��
} TKACKGSPUSHPUSHUSERAWARDMESSAGE, *PTKACKGSPUSHPUSHUSERAWARDMESSAGE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�ͻ��������໥���ٵ���Ϣ
//   ˵    ����GC <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NETTEST (TK_MSG_GAME + 124)
typedef struct tagTkReqGameNetTest
{
    TKHEADER header;

    DWORD dwTestID;  // �������к�

    DWORD dwReqUserID;  // ���ٷ�����
    DWORD dwAckUserID;  // ������Ӧ��

    DWORD dwReqTick;  // ���ٷ���ʱTick
    DWORD dwAckTick;  // �յ���ӦʱTick

} TKREQGAMENETTEST, *PTKREQGAMENETTEST;
typedef struct tagTkAckGameNetTest
{
    TKHEADER header;

    DWORD dwTestID;  // �������к�

    DWORD dwReqUserID;  // ���ٷ�����
    DWORD dwAckUserID;  // ������Ӧ��

    DWORD dwReqTick;  // ���ٷ���ʱTick
    DWORD dwAckTick;  // �յ���ӦʱTick

} TKACKGAMENETTEST, *PTKACKGAMENETTEST;

//-----------------------------------------------------------------------------------------
//
// ������ʾ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_TIPMESSAGE (TK_MSG_GAME + 125)
//ACK:GameServer --> Client
typedef struct tagTKACKPUSHGAMETIPMESSAGE
{
    TKHEADER header;
    int nClass;  // ��𣬲μ� TKGameTipMessageDefine.h �� ePushGameTipClass ����
    int nTipID;  // ��ţ��μ� TKGameTipMessageDefine.h ����ض���

    WORD wXmlOffset;  // ��׺XML��ַƫ��������λ�ֽڣ����յ�Ϊ��Ϣ����ʼλ��
    WORD wXmlLength;  // ��׺XML�ĳ��ȣ������ַ���������'\0'

    BYTE byShowType;   // ������ʾ���ͣ��μ� TKGameTipMessageDefine.h �� ePushGameTipShowType ����
    BYTE byOperation;  // ��������������μ� TKGameTipMessageDefine.h �� ePushGameTipOperation ����
    WORD wShowSecond;  // ������ʾʱ��(��)

} TKACKPUSHGAMETIPMESSAGE, *PTKACKPUSHGAMETIPMESSAGE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ����ͨ����Ϣ
//   ˵    ����GC <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GAMECOMMONPROTOCOL (TK_MSG_GAME + 126)
typedef struct tagTKREQGAMECOMMONPROTOCOL
{
    TKHEADER header;
    WORD wMsgID;                 // Э��ID
    WORD wSubMsgID;              // ��Э��ID
    TKSUFFIXIDX suffixJsonData;  // ��׺ҵ������(Json�ַ�����UTF-8��ʽ)
} TKREQGAMECOMMONPROTOCOL, *PTKREQGAMECOMMONPROTOCOL;
typedef struct tagTKACKGAMECOMMONPROTOCOL
{
    TKHEADER header;
    WORD wMsgID;                 // Э��ID
    WORD wSubMsgID;              // ��Э��ID
    TKSUFFIXIDX suffixJsonData;  // ��׺ҵ������(Json�ַ�����UTF-8��ʽ)
} TKACKGAMECOMMONPROTOCOL, *PTKACKGAMECOMMONPROTOCOL;

// TK_MSG_GAME_GAMECOMMONPROTOCOL��wMsgID�ڱ����˵Ķ���
enum CommonGameMsgIDDef : unsigned short
{
    GameBase_Begin = 0,
    EHeartBestTest = 1,
    AddGamePlayerInfo = 2,  // �����Ϸ�û�
    PushGameDataInfo = 3,   // ������Ϸ����
    GameBase_End = 1000,    // =========
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·�����Player��Ϣ���ͻ������յ��Լ��ģ����յ����˵ģ�
//   ˵    ����64λ��
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDGAMEPLAYERINFO64 (TK_MSG_GAME + 127)
typedef struct tagTKGAMEPLAYERINFO64
{
    int nSeatOrder;   //��λ���
    UINT64 dwUserID;  //�û��ʺŶ�Ӧ��Ψһ���ֱ��ID
    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;   //�û�ͷ��ID
    int nMatchRank;  //��Ϸ���������ݱ������ּ��������

    int64_t nScore;  //���л���

    BOOL bArrived;   //�Ƿ�ͻ����Ѿ����ӵ�������
    int nNetStatus;  //����״̬��0�������ӣ�1����
    DWORD dwFormID;  //�û�����ID
} TKGAMEPLAYERINFO64, *PTKGAMEPLAYERINFO64;
typedef struct tagTKACKGAMEADDPLAYERENTERINFO64
{
    TKHEADER header;
    TKGAMEPLAYERINFO64 info;
} TKACKGAMEADDPLAYERENTERINFO64, *PTKACKGAMEADDPLAYERENTERINFO64;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ���������Ϸ������Ϣ
//   ˵    ����GS --> GC 64λ��
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHPLAYERGAMEDATA64 (TK_MSG_GAME + 128)
typedef struct tagTKACKGSPUSHPLAYERGAMEDATA64
{
    TKHEADER header;

    UINT64 dwUserID;

    int nExchangeRate;  // �һ���
    int64_t nLeftChip;  // ʣ�������ߵ�ǰ����ֵ

    int iCurGameHand;             // �û���ǰ���еľ��Ʊ�ţ���%d�֣�����ʱ�ۼơ�Ϊ0�Ǳ�ʾû�д��������ʾ��
    int nTotalFinishedGameCount;  // �Ѿ���ɵ�����Ч���������ƽ���ʱ�ۼӡ����ھ���������ʾ��
    int nCurFinishedGameCount;    // ��ǰ�׶���ɵ���Ч���������ƽ���ʱ�ۼӡ����ڻ��ּӱ���ʾ��
} TKACKGSPUSHPLAYERGAMEDATA64, *PTKACKGSPUSHPLAYERGAMEDATA64;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·�����ѡ��������Ϣ���ͻ����յ�����Ϣ�����ԭ�ȵ�Order��Ϣ��
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_STAGEPLAYERORDER (TK_MSG_GAME + 200)
typedef struct tagTKACKGAMESTAGEPLAYERORDER
{
    TKHEADER header;
    DWORD dwSerial;
    WORD wFlags;
    WORD wScoreType;
    int nMinWinnerOrder;  //�����������
    int cnOrder;          //�����cnOrder��TKGAMEPLAYERORDERINFO
} TKACKGAMESTAGEPLAYERORDER, *PTKACKGAMESTAGEPLAYERORDER;
#define TK_GAME_SCORETYPE_SCORE 0  //��ʾΪ����
#define TK_GAME_SCORETYPE_CHIP 1   //��ʾΪ����
#define TK_GAME_SCORETYPE_LIFE 2   //��ʾΪ����ֵ
typedef struct tagTKGAMEPLAYERORDERINFO
{
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];
    int nScore;  //���л��֣�����ʱ��ʾ�����֡���������ʾnScore��ֵ
    WORD wPlaceOrder;
    WORD wOutRoundState;  //�Ƿ����,0Ϊ���֣�1����
} TKGAMEPLAYERORDERINFO, *PTKGAMEPLAYERORDERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ͻ����·�ָ��Round��PlaceOrder��Ϣ
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROUNDPLAYERORDER (TK_MSG_GAME + 201)
typedef struct tagTKACKGAMEROUNDPLAYERORDER
{
    TKHEADER header;
    DWORD dwSerial;
    WORD wFlags;
    int cnOrder;  //�����cnOrder��TKGAMEPLAYERORDERINFO
} TKACKGAMEROUNDPLAYERORDER, *PTKACKGAMEROUNDPLAYERORDER;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���֪ͨ�ͻ���Stage�����б��и��±仯
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_STAGEPLAYERORDERCHANGED (TK_MSG_GAME + 202)
typedef struct tagTKACKGAMESTAGEPLAYERORDERCHANGED
{
    TKHEADER header;
    DWORD dwSerial;
    WORD wFlags;  //1��ʾ����������
    WORD wUpdateSecond;
} TKACKGAMESTAGEPLAYERORDERCHANGED, *PTKACKGAMESTAGEPLAYERORDERCHANGED;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��ͻ��˻�ȡ�µ�Stage�����б�
//   ˵    ����GC -> GS, REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GETSTAGEPLAYERORDER (TK_MSG_GAME + 203)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���һ��ֱ仯
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_UPDATE_ROUND_PLAYER_SCORE (TK_MSG_GAME + 204)
typedef struct tagTKUPDATEROUNDPLAYERSCORE
{
    TKHEADER header;
    int cnPlayer;
    // ���cnPlayer��( userid + score )
    // DWORD dwUserID;
    // int nScore;
    // ...
} TKUPDATEROUNDPLAYERSCORE, *PTKUPDATEROUNDPLAYERSCORE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��ˣ��������Ϣһ��
//   ˵    ����GS -> GC, Push REQ
//   ͬ���ܵ�32λЭ��TK_MSG_GAME_REST
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_REST_64 (TK_MSG_GAME + 205)
typedef struct tagTKREQPLAYERREST_64
{
    TKHEADER header;
    int32_t nRestTime;          // ����Ϣʱ��,����������Ҳ��������ť��Ϸ���Զ���ʼ
    int64_t i64Life;            // ��ǰ����ֵ
    int64_t i64Coin;            // �ɻضҽ����
    int32_t cnGames;            // ������Ƹ���
    int64_t i64Multi;           // ��һ�ֵĻ��ֱ���
    int32_t nNextLevelGames;    // ��һ�������Ƹ���
    int64_t i64NextLevelMulti;  // ��һ�����ֱ���
    uint64_t u64ExChangeRate;   // �ض��ʷ��ӣ��ػ���10000��ʾ1���Ŵ�10000����
    uint64_t u64ExChangeRateDenominator;  // �ض��ʷ�ĸ���ػ���10000��ʾ1���Ŵ�10000������ע�����������ضҵĽ�Ҽ��㹫ʽ�ǣ��ضҵĽ��=����*u64ExChangeRate/(10000*u64ExChangeRateDenominator)
    int32_t nAwardTimeSpan;        //�佱ʱ����(��),<=0��ʾ���佱
    int32_t nNextAwardLeftSecond;  //������һ�ΰ佱��ʣ��ʱ��(��)
    int64_t i64Score;              // ��������
    int64_t i64LastHandScore;      // ���һ�ֵ÷�
    uint16_t wGameResult;          // ��Ϸ�����0 == δ֪�� 1 == ʤ���� 2 == �ͣ� 3 == ��

    //
    uint32_t dwSufOffset;  //��׺����ƫ��
    uint32_t dwSufSize;    //��׺���ݴ�С
    int64_t i64Reserve1;   //����1
    uint32_t dwReserve2;   //����2
    uint32_t dwReserve3;   //����3
} TKREQPLAYERREST_64, *PTKREQPLAYERREST_64;
//��׺Ѫƿjson{"Plan":[{"Blod":4444,"Cost":4444,"Discnt":100},{"Blod":333,"Cost":333,"Discnt":100}]}

//   ��    �ܣ���Ѫ������64λ��
//   ˵    ����GS-> GC
//   ͬ���ܵ�32λЭ��TKID_MSG_GAME_EXCHANGE
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_EXCHANGE_64 (TK_MSG_GAME + 206)
typedef struct tagTKREQGAMEEXCHANGE_64
{
    TKHEADER header;
    uint64_t u64ExChangeRate;  // �ض��ʷ��ӣ��ػ���10000��ʾ1���Ŵ�10000����
    uint64_t u64ExChangeRateDenominator;  // �ض��ʷ�ĸ���ػ���10000��ʾ1���Ŵ�10000������ע�����������ضҵĽ�Ҽ��㹫ʽ�ǣ��ضҵĽ��=����*u64ExChangeRate/(10000*u64ExChangeRateDenominator)
    uint32_t dwMPID;       //��ƷID
    uint32_t dwTourneyID;  //TourneyID
    uint32_t dwStageID;    //�׶�ID
    uint32_t dwSufOffSet;  //��׺����ƫ��
    uint32_t dwSufSize;    //��׺���ݴ�С
    int64_t i64Reserve1;   //�����ֶ�1
    uint32_t dwReserve2;   // �����ֶ�2
    uint32_t dwReserve3;   // �����ֶ�3
} TKREQGAMEEXCHANGE_64, *PTKREQGAMEEXCHANGE_64;
//��׺Ѫƿjson��{"Plan":[{"Blod":4444,"Cost":4444,"Discnt":100},{"Blod":333,"Cost":333,"Discnt":100}]}

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��������Ѫ��64λ��
//   ˵    ����GC -> GS -> GC
//   ͬ���ܵ�32λЭ��TK_MSG_GAME_HEMATINIC
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HEMATINIC_64 (TK_MSG_GAME + 207)
typedef struct tagTKPLAYERREQHEMATINIC_64
{
    TKHEADER header;
    uint32_t dwUserID;     // �û�ID
    int64_t i64Hematinic;  // ��Ѫ��
    int64_t i64Reserve1;   //�����ֶ�1
    uint32_t dwDisCount;   //�ۿ�
    uint32_t dwReserve2;   //�����ֶ�2
    uint32_t dwReserve3;   //�����ֶ�3
} TKPLAYERREQHEMATINIC_64, *PTKPLAYERREQHEMATINIC_64;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ���������Ϸ������Ϣ
//   ˵    ����GS --> GC( 64λ��չ�棩
//   ͬ���ܵ�64λ��32λЭ��TKID_GS2MC_PUSHPLAYERGAMEDATA64��TKID_GS2MC_PUSHPLAYERGAMEDATA
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHPLAYERGAMEDATA64EX (TK_MSG_GAME + 208)
typedef struct tagTKACKGSPUSHPLAYERGAMEDATA64EX
{
    TKHEADER header;
    uint32_t dwUserID;
    int64_t i64LeftChip;              // ʣ�������ߵ�ǰ����ֵ
    int32_t iCurGameHand;             // �û���ǰ���еľ��Ʊ�ţ���%d�֣�����ʱ�ۼơ�Ϊ0�Ǳ�ʾû�д��������ʾ��
    int32_t nTotalFinishedGameCount;  // �Ѿ���ɵ�����Ч���������ƽ���ʱ�ۼӡ����ھ���������ʾ��
    int32_t nCurFinishedGameCount;    // ��ǰ�׶���ɵ���Ч���������ƽ���ʱ�ۼӡ����ڻ��ּӱ���ʾ��
    uint64_t u64ExChangeRate;         // �ض��ʷ��ӣ��ػ���10000��ʾ1���Ŵ�10000����
    uint64_t u64ExChangeRateDenominator;  // �ض��ʷ�ĸ���ػ���10000��ʾ1���Ŵ�10000������ע�����������ضҵĽ�Ҽ��㹫ʽ�ǣ��ضҵĽ��=����*u64ExChangeRate/(10000*u64ExChangeRateDenominator)
    uint64_t u64Reserve1;  // �����ֶ�1
    uint32_t dwReserve2;   // �����ֶ�2
    uint32_t dwReserve3;   // �����ֶ�3
} TKACKGSPUSHPLAYERGAMEDATA64EX, *PTKACKGSPUSHPLAYERGAMEDATA64EX;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��ˣ��������Ϣһ��
//   ˵    ����GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_REST (TK_MSG_GAME + 305)
typedef struct tagTKREQPLAYERREST
{
    TKHEADER header;
    int nRestTime;                               // ����Ϣʱ��,����������Ҳ��������ť��Ϸ���Զ���ʼ
    int cnHematinic;                             // ��ѡ��Ѫƿ����
    HEMATINIC asHematinic[MAX_HEMATINIC_COUNT];  // ÿ��Ѫƿ����ϸ��Ϣ
    int nLife;                                   // ��ǰ����ֵ
    int nCoin;                                   // �ɻضҽ����
    int cnGames;                                 // ������Ƹ���
    int nMulti;                                  // ��һ�ֵĻ��ֱ���
    int nNextLevelGames;                         // ��һ�������Ƹ���
    int nNextLevelMulti;                         // ��һ�����ֱ���
    int nExchangeRate;                           // �ضұ���

    int nAwardTimeSpan;        //�佱ʱ����(��),<=0��ʾ���佱
    int nNextAwardLeftSecond;  //������һ�ΰ佱��ʣ��ʱ��(��)

    //2011.11.16 ����
    int nScore;  // ��������

    int nLastHandScore;  // ���һ�ֵ÷�
    WORD wGameResult;    // ��Ϸ�����0 == δ֪�� 1 == ʤ���� 2 == �ͣ� 3 == ��
} TKREQPLAYERREST, *PTKREQPLAYERREST;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������������Ϸ
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CONTINUE (TK_MSG_GAME + 306)
typedef struct tagTKPLAYERREQCONTINUE
{
    TKHEADER header;
    DWORD dwUserID;
    int nExtraTask;  // ��������, 0=��, 1=������ս����, ����δ����        wuqy add 2016.5.30
} TKPLAYERREQCONTINUE, *PTKPLAYERREQCONTINUE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���������˳���Ϸ
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LEAVE (TK_MSG_GAME + 307)
typedef struct tagTKPLAYERREQLEAVE
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwNoExchange;   // ���һ���ң�0-��ʾ�һ���ң�1��ʾ���һ���Ĭ��ֵΪ0���������ϰ汾��
    DWORD dwSaveArchive;  // ���ݴ浵��0-��ʾ���浵��1��ʾ�浵��Ĭ��ֵΪ1
} TKPLAYERREQLEAVE, *PTKPLAYERREQLEAVE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��������Ѫ
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_HEMATINIC (TK_MSG_GAME + 308)
typedef struct tagTKPLAYERREQHEMATINIC
{
    TKHEADER header;
    DWORD dwUserID;
    int nHematinic;  // ��Ѫ��
} TKPLAYERREQHEMATINIC, *PTKPLAYERREQHEMATINIC;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������󸴻�
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_PLAYER_RELIVE (TK_MSG_GAME + 310)
typedef struct tagTKPLAYERRELIVE
{
    TKHEADER header;
    DWORD dwUserID;
} TKPLAYERRELIVE, *PTKPLAYERRELIVE;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ȷ���յ���״��Ϣ
//   ˵    ����GC -> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_PLAYER_RECEIVEDMATCHAWARD (TK_MSG_GAME + 311)
// ACK:TKHEADER header

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��˵�ǰ���ش�С
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_JACKPOT_COUNT (TK_MSG_GAME + 312)
typedef struct tagJACKPOTCOUNT
{
    TKHEADER header;
    int nJackpotCount;
} TKJACKPOTCOUNT, *PTKJACKPOTCOUNT;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��˵�ǰ��������
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_JACKPOT (TK_MSG_GAME + 313)
typedef struct tagJACKPOT
{
    TKHEADER header;
    int nJackpotCount;
    // ��������ַ���
} TKJACKPOT, *PTKJACKPOT;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��˵�ǰ�н�����б�
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_JACKPOT_WINNER (TK_MSG_GAME + 314)
typedef struct tagJACKPOTWINNER
{
    TKHEADER header;
    int cnWinner;
    // ��������ַ���
} TKJACKPOTWINNER, *PTKJACKPOTWINNER;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�����ͻ��˿�������ģʽ
//   ˵    ����GS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ENABLECHAT (TK_MSG_GAME + 315)
// ACK:
// BOOL bEnableGCChat
// DWORD dwChatSvrIP			// ���������IP��ע���ǻ����ֽ���
// DWORD dwChatSvrPort
// DWORD dwChatChannelID
// DWORD dwChatChannelSubID		wuqy add 2010.05.12
// DWORD dwChatChannelType
// DWORD dwChatUserType

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ��ҳ�ͻ���ע��
//   ˵    ����GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NOTIFY_GUESTREGISTER (TK_MSG_GAME + 316)
typedef struct tagTKGAMENOTIFYGUESTREGISTER
{
    TKHEADER header;
    DWORD dwSuffixOffSet;  // ��׺��ʾ�����ַ���ƫ�������������Ϣ����������
    DWORD dwSuffixSize;    // ��׺��ʾ�����ַ�����С������������\0
} TKGAMENOTIFYGUESTREGISTER, *PTKGAMENOTIFYGUESTREGISTER;
//�����ʾ����

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���ҳ�ͻ�������ע��
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_WEBREGISTER_MANUAL (TK_MSG_GAME + 317)
#define TK_MSG_GAME_WEBREGISTER (TK_MSG_GAME_WEBREGISTER_MANUAL)
typedef struct tagTKREQWEBGUESTGAMEREGISTERBASE
{
    TKHEADER header;
    char szLoginName[TK_BUFLEN_USERNAME];
    char szNickName[TK_BUFLEN_NICKNAME];
    char szPassWord[TK_BUFLEN_PASSWORD];
    char szIP[TK_BUFLEN_IP];
    WORD wFigureID;  // ͷ��ID
    WORD wReserve1;

    DWORD dwLcVer;  // �ͻ��˰汾��

    // ����ע�����
    WORD wAgent;  // ע����Դ
    WORD wReserve2;

    // �ƹ�Ա
    DWORD dwPromoterID;                           // �ƹ�ԱID
    char szPromoterNickName[TK_BUFLEN_NICKNAME];  // �ƹ�Ա�ǳ�

    // ���ֿ�
    char szNewPlayerCard[TK_BUFLEN_NEWPLAYERCARD];  // ���ֿ���

    DWORD dwSuffixOffSet;  // ��׺XML�ַ�������ƫ�������������Ϣ���������ˣ�����Э�����
    DWORD dwSuffixSize;    // ��׺XML�ַ������ݴ�С��������������\0
    // ����
    DWORD dwParam1;  // ��������1
    DWORD dwParam2;  // ��������2
    DWORD dwParam3;  // ��������3

    //����������Э����tagTKREQWEBGUESTGAMEREGISTER����� (wuqy 2013.9.6)
} TKREQWEBGUESTGAMEREGISTERBASE;
typedef struct tagTKREQWEBGUESTGAMEREGISTER : public tagTKREQWEBGUESTGAMEREGISTERBASE
{
    char szCltIdent[TK_BUFLEN_NOTE];  // �ͻ���Ψһ��ʶ(wuqy add 2011.8.1)
    DWORD dwCustomID;                 // ��װԱ �ƹ�ID(wuqy add 2013.9.4)
} TKREQWEBGUESTGAMEREGISTER, *PTKREQWEBGUESTGAMEREGISTER;
// �����׺���ݣ�
//     Web��������XML�ַ�������
typedef struct tagTKACKWEBGUESTGAMEREGISTER
{
    TKHEADER header;
    DWORD dwUserID;  //ע��ɹ����UserID
    char szNickName[TK_BUFLEN_NICKNAME];
} TKACKWEBGUESTGAMEREGISTER, *PTKACKWEBGUESTGAMEREGISTER;
//ע����Ϣ����ֵ��
#define TK_WEBGAMEREGISTER_LOGINNAMEEXIST (10)    // �û����Ѿ���ʹ��
#define TK_WEBGAMEREGISTER_NICKNAMEEXIST (11)     // �û��ǳ��Ѿ���ʹ��
#define TK_WEBGAMEREGISTER_NICKNAMELAWLESS (12)   // ��¼�ǳƷǷ�
#define TK_WEBGAMEREGISTER_LOGINNAMELAWLESS (13)  // ��¼���Ƿ�
#define TK_WEBGAMEREGISTER_PWDLAWLESS (14)        // �����ʽ�Ƿ�
#define TK_WEBGAMEREGISTER_IDENTLIMIT (15)        // �豸ע����������
#define TK_WEBGAMEREGISTER_LOGINNAMEERR (100)     // ��½���û�����������Ч����
#define TK_WEBGAMEREGISTER_NICKNAMEERR (101)      // �ǳƲ�������Ч����
#define TK_WEBGAMEREGISTER_PSWERR (102)           // ���벻������Ч���� �������벻�ܰ�������ڵ�½��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Թۿͻ�������������Ϣ
//   ˵    ����BS -> GC, Push ACK
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELAYSRV_CHAT_CFG (TK_MSG_GAME + 318)
typedef struct tagRelaySvrChatCfgAck
{
    TKHEADER hdr;
    DWORD dwChatSvrIP;  // ���������IP��ע���ǻ����ֽ���
    DWORD dwChatSvrPort;
    DWORD dwChatChannelID;
    BOOL bListTablePlayerOnly;

    DWORD dwChatChannelSubID;  //wuqy add 2010.05.12
    DWORD dwChatChannelType;
    DWORD dwChatUserType;

} TKACK_RELAYSVR_CHAT_CFG, *LPTKACK_RELAYSVR_CHAT_CFG;
// ACK:
// TKACK_RELAYSVR_CHAT_CFG

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Թۿͻ��������Թ�����
//   ˵    ����GC <-> BS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELAYSRV_GET_ROUND_WATCHER_COUNT (TK_MSG_GAME + 319)
typedef struct tagGetRoundWatcherCntReq
{
    TKHEADER hdr;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwRoundID;
} GET_ROUND_WATCHER_CNT_REQ, *LPGET_ROUND_WATCHER_CNT_REQ;
// ACK:
// TKHEADER header
// header.dwParam == ����

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������������
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LOCKDOWN (TK_MSG_GAME + 320)
typedef struct tagTKReqLockdown
{
    TKHEADER header;
    int nSeat;
    BOOL bUnlock;  // �Ƿ�ȡ������
} TKREQLOCKDOWNGAME, *PTKREQLOCKDOWNGAME;
typedef struct tagTKAckLockdown
{
    TKHEADER header;
    BOOL bAllow;  // �Ƿ�����
} TKACKLOCKDOWNGAME, *PTKACKLOCKDOWNGAME;

//-----------------------------------------------------------------------------------------
//
// WebGuestȡ�óɼ���֪ͨע��
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_NOTIFY_GUESTREGISTEREX (TK_MSG_GAME + 321)
typedef struct tagTKGAMENOTIFYGUESTREGISTEREX
{
    TKHEADER header;
    DWORD dwOrder;       // �������
    BOOL bAutoRegister;  // �Ƿ��ɷ������Զ�ע��
} TKGAMENOTIFYGUESTREGISTEREX, *PTKGAMENOTIFYGUESTREGISTEREX;

//-----------------------------------------------------------------------------------------
//
// WebGuest��ʽע������
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_WEBREGISTER_AUTO (TK_MSG_GAME + 322)
typedef struct tagTKREQWEBGUESTGAMEREGISTERAUTO
{
    TKHEADER header;

    DWORD dwPartnerID;  // ��������ID

    // �ƹ�վ���г����
    DWORD dwAgent;                     // ע����Դ
    char szAgentMark[TK_BUFLEN_NOTE];  // ������ע��
    BOOL bRegister;                    // �û��Ƿ�ѡ��ע��

    //�����д��������Ϣ������������Ϣע��
    char szNickName[TK_BUFLEN_NICKNAME];
    char szUserName[TK_BUFLEN_USERNAME];
    char szPassword[TK_BUFLEN_PASSWORD];
    DWORD dwFigureID;  // ͷ��ID

    char szCltIdent[TK_BUFLEN_NOTE];  // �ͻ���Ψһ��ʶ
    char szIP[TK_BUFLEN_IP];          // ��Agent���������������û���ʵIP
    DWORD dwCustomID;                 // �ƹ�ͻ�ID

} TKREQWEBGUESTGAMEREGISTERAUTO, *PTKREQWEBGUESTGAMEREGISTERAUTO;
typedef struct tagTKACKWEBGUESTGAMEREGISTERAUTO
{
    TKHEADER header;
    DWORD dwUserID;  //ע��ɹ����UserID
    char szNickName[TK_BUFLEN_NICKNAME];
    char szUserName[TK_BUFLEN_USERNAME];
    char szPassword[TK_BUFLEN_PASSWORD];
    DWORD dwFigureID;
    DWORD dwBankAccID;  // ϵͳ�˵�
} TKACKWEBGUESTGAMEREGISTERAUTO, *PTKACKWEBGUESTGAMEREGISTERAUTO;

//-----------------------------------------------------------------------------------------
//
//   ��    �ܣ��򿪻�رս׶����������Ϣʵʱ����
//   ˵    ����GC <-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OPENSTAGEBOUTRESULT (TK_MSG_GAME + 323)
typedef struct tagTKREQGAMEOPENSTAGEBOUTRESULT
{
    TKHEADER header;
    BOOL bOpen;
} TKREQGAMEOPENSTAGEBOUTRESULT, *PTKREQGAMEOPENSTAGEBOUTRESULT;
typedef struct tagTKACKGAMEOPENSTAGEBOUTRESULT
{
    TKHEADER header;
    int nOpenStyle;       //0�رգ�1�б��ʽ�򿪣�2����ʽ��
    BOOL bInWaitPlayoff;  //�Ƿ��ڼ����ȴ��� wuqy add 2015.4.28(û�м������û��Ѿ���ɼ����ģ�bInWaitPlayoffΪFALSE)
} TKACKGAMEOPENSTAGEBOUTRESULT, *PTKACKGAMEOPENSTAGEBOUTRESULT;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��ˣ�����Ҹ���
//   ˵    ����GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELIVECOST (TK_MSG_GAME + 324)
typedef struct tagTKGAMERELIVECOST
{
    TKHEADER header;

    int nCostType;   // �����������
    DWORD dwCostID;  // �������ID
    int nAmount;     // ������õ�����

    int nScore;        // �ɻ�û���
    int cnLivePlayer;  // ��ǰ�������
    int nTimeOut;      // ����ʱ
} TKGAMERELIVECOST, *PTKGAMERELIVECOST;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��ˣ������ѡ��֧��ҵ��۷�
//   ˵    ����GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PAYMENTCOST (TK_MSG_GAME + 325)

// ����
typedef struct tagTKREQGAMEPAYMENTCOST
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    int nTimeOut;      // ���򵹼�ʱ
    int cnLivePlayer;  // ��ǰ�������

    DWORD dwPayTypeID;        // �۷�ҵ������ �μ�: eDicPaymentBusinessTypeDef, �μ� TKComitUniversalDicSrv.h
    DWORD dwCostPlanCount;    // �۷ѷ�������
    TKSUFFIXIDX sufData;      // ��׺���ݣ�TK_COSTPLAN���μ� TKMatchUniversalDicSrv.h
    TKSUFFIXIDX sufTaskData;  // ��׺��������
} TKREQGAMEPAYMENTCOST, *PTKREQGAMEPAYMENTCOST;
// ��׺���� dwCastPlanCount��TK_COSTPLAN

// ������
typedef struct tagTKACKGAMEPAYMENTCOST
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    DWORD dwResult;     // ҵ��ִ�н����0-��Ҳ�����1-��ҹ���
    DWORD dwPayTypeID;  // �۷�ҵ������ �μ�: eDicPaymentBusinessTypeDef, �μ� TKComitUniversalDicSrv.h
    DWORD dwPlanID;     // ����ķ���ID
} TKACKGAMEPAYMENTCOST, *PTKACKGAMEPAYMENTCOST;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ�ͻ��ˣ����ѡ��֧��ҵ��۷� �Ľ��
//   ˵    ����GS -> GC, Push REQ
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PAYMENTRESULT (TK_MSG_GAME + 326)

// ����
typedef struct tagTKREQGAMEPAYMENTRESULT
{
    TKHEADER header;

    DWORD dwUserID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;

    DWORD dwPayTypeID;  // �۷�ҵ������ �μ�: eDicPaymentBusinessTypeDef, �μ� TKComitUniversalDicSrv.h
    DWORD dwResult;     // ֧��ҵ��۷ѵĽ����eDicTypeTKPaymentResult���μ� TKMatchUniversalDicSrv.h
} TKREQGAMEPAYMENTRESULT, *PTKREQGAMEPAYMENTRESULT;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�MS <-> GS <-> GC ��Ϣ͸��
//   ˵    ����
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_RELAY_BEGIN (TK_MSG_GAME + 330)
#define TK_MSG_GAME_RELAY_END (TK_MSG_GAME_RELAY_BEGIN + TK_SVR_MAX_RELAY_MSG)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�Լս����ʼ����Ϣ
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_INITTABLE (TK_MSG_GAME_RELAY_BEGIN + 0)
// ACK:
//  TKHEADER header;
//	BOOL bGaming;
//  int nSeatCount;                      //
//  nSeatCount��TK_SEAT_PLAYER_INFO
//  DWORD dwChatSvrIP					// ���������IP��ע���ǻ����ֽ���
//  int nChatSvrPort
//  DWORD dwChatChannelID
//  BOOL bShowTable
//  DWORD dwChatChannelSubID		wuqy add 2010.05.12
//  DWORD dwChatChannelType
//  DWORD dwChatUserType

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�����º�վ��
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_SITDOWN (TK_MSG_GAME_RELAY_BEGIN + 1)
// REQ
//  TKHEADER header;
//  int nSeat;                          // ��λ��, base 0. -1��ʾվ��
// ACK
//  TKHEADER header;
//  TK_SEAT_PLAYER_INFO stPlayer;       //
//  int nSeat;                          // ��λ��, base 0. -1��ʾվ��
//  nPlayerInfoExItemSize               // =sizeof(TK_SEAT_PLAYER_INFOEX) wuqy add 2017.1.18
//  TK_SEAT_PLAYER_INFOEX stPlayerEx    // wuqy add 2017.1.18

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�Լս���û�׼����ʼ��Ϸ
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_READY (TK_MSG_GAME_RELAY_BEGIN + 2)
// REQ
//  TKHEADER header;
//  int nReady;                        // 1׼�� 0ȡ��
// ACK
//  TKHEADER header;
//  DWORD dwUserID;
//  int nReady;                        // 1׼�� 0ȡ��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�����ƿ�ʼ
//   ˵    ����GS-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROUNDBEGIN (TK_MSG_GAME_RELAY_BEGIN + 3)
// ACK:
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ�������Թ��Լ�����
//   ˵    ����GC<-->GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_ENABLE_WATCH (TK_MSG_GAME_RELAY_BEGIN + 4)
// REQ:
//  TKHEADER header;
// ACK:
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���������ʼ����Ϣ
//   ˵    ����GS-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_INITGAMETABLE (TK_MSG_GAME_RELAY_BEGIN + 5)
// ACK:
//  TKHEADER header;
//  int nSeatCount;                      //
//  nSeatCount��TK_SEAT_PLAYER_INFO
//  DWORD dwChatSvrIP					// ���������IP��ע���ǻ����ֽ���
//  int nChatSvrPort
//  DWORD dwChatChannelID
//  int nMaxAddToHP;                     // HP����(���ӵ�/���),��nHPMode��ֵ����
//  int nMinAddToHP;                     // HP����(��С�ӵ�/��С),��nHPMode��ֵ����
//  int nExchangeRate;                   // �һ���
//  int nHPMode;                         // ��Ѫģʽ 1: ���ģʽ 2: ����ģʽ
//  DWORD dwChatChannelSubID				wuqy add 2010.05.12
//  DWORD dwChatChannelType
//  DWORD dwChatUserType
//	int nLastTableDataLength			 // wuqy add 2011.1.25
//  nLastTableDataLength�ֽڵ�����
//  nPlayerInfoExItemSize                // =sizeof(TK_SEAT_PLAYER_INFOEX) wuqy add 2017.1.18
//  nSeatCount��TK_SEAT_PLAYER_INFOEX    // wuqy add 2017.1.18

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������� ������һ�ֲ��ټ���
//   ˵    ����GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MARKPLAYERIDLE (TK_MSG_GAME_RELAY_BEGIN + 6)
// REQ
//  TKHEADER header;
//    header.dwParam:  1���� 0ȡ��
// ACK
//  TKHEADER header;
//    header.dwParam:  1���� 0ȡ��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������� �����Ѫ��������룩
//   ˵    ����GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDHP (TK_MSG_GAME_RELAY_BEGIN + 7)
// REQ
//  TKHEADER header;
//    header.dwParam:  ��Ѫ��
// ACK
//  TKHEADER header;
//  DWORD dwUserID
//  int nHPAdded
//  DWORD dwCost

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������� ���󱣴���루�ضҲ��ֳ��룩
//   ˵    ����GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SAVEHP (TK_MSG_GAME_RELAY_BEGIN + 8)
// REQ
//  TKHEADER header;
//    header.dwParam: ���浽��buy-in����

// ACK
// TKHEADER header;
// DWORD dwUserID
// int nHPSaved
// int nGoldAdded;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������� ����һ�ֽ������Լ���Ѫ
//   ˵    ����GS<-->GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MARKAUTOADDHP (TK_MSG_GAME_RELAY_BEGIN + 9)
// REQ
//  TKHEADER header;
//    header.dwParam:  ��0��־�����Զ���Ѫ��dwParam�� 0ȡ��
// ACK
//  TKHEADER header;
//    header.dwParam:  ��0��־�����Զ���Ѫ��dwParam�� 0ȡ��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ��ϷDll�����˱���ѡ��
//   ˵    ����MS -> GS -> GameDll
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MARKLOCKDOWN (TK_MSG_GAME_RELAY_BEGIN + 10)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ��ϷDll����VIPģʽ����
//   ˵    ����MS -> GS -> GameDll
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_VIPMODE (TK_MSG_GAME_RELAY_BEGIN + 11)
// ACK
//  TKHEADER header;
//    header.dwParam:  1���� 0ȡ��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨGC������Ϸ�����˵㹦��
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LUCKCARD (TK_MSG_GAME_RELAY_BEGIN + 12)
// REQ
//  TKHEADER header;
// DWORD dwBalance ʣ����

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������Ҷһ����˵�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ADDLUCKY (TK_MSG_GAME_RELAY_BEGIN + 13)
// REQ
//  TKHEADER header;
//    header.dwParam: Ҫ��������˵���
// ACK
//  TKHEADER header;
//    header.dwParam: �ɹ���������˵���(0Ϊʧ��)
//    char *szReason; ʧ��ԭ��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���֪����ʹ�ý��
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CONSUMELUCKY (TK_MSG_GAME_RELAY_BEGIN + 14)
// REQ
//  TKHEADER header;
//    header.dwParam: ���ĵ����˵���

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�֪ͨ��ϷDll������ļ�Ѫ����
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_EXCHANGE (TK_MSG_GAME_RELAY_BEGIN + 15)
// REQ
//  TKHEADER header;
//    header.dwParam:
// int cnExchange;
// DWORD dwBlood1;
// DWORD dwCost1;
// DWORD dwBlood2;
// DWORD dwCost2;
// DWORD dwBlood3;
// DWORD dwCost3;
// DWORD dwBlood4;
// DWORD dwCost4;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���̨��Ϣ
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_CHALLENGERLIST (TK_MSG_GAME_RELAY_BEGIN + 16)
// ACK
// TKHEADER hdr;
// int nWinnerWinCount;		// ������ǰ���޴���
// int nChallengerCount;	// ��ս��,��һ��Ϊ����
// TKCHALLENGERINFO[]

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��������
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_CHALLENGERAPPLY (TK_MSG_GAME_RELAY_BEGIN + 17)
// REQ/ACK
// TKHEADER hdr;
//typedef struct tagTKReqChallengerApply
//{
//	BOOL		bApply;	// 1����, 2����
//	//���Ǵ�������һ��ѡ����̨����Ϣ
//}TKREQCHALLENGERAPPLY, *PTKREQCHALLENGERAPPLY;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������仯
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_CHALLENGE_UPDATE_WINNERINFO (TK_MSG_GAME_RELAY_BEGIN + 18)
// ACK
// TKHEADER hdr;
//typedef struct tagTKAckUpdateWinnerInfo
//{
//	DWORD		dwWinnerUserID;				// ����ID
//	int			nWinnerWinCount;			// �����Ѿ���ʤ����
//	DWORD		dwReason;					// ��Ϣ����ԭ��
//	TKCHALLENGERINFO	stWinnerInfo;		// ��������Ϣ
//	int			nOutPlayerCount;			// ����ʧ�ܻ����������˳����������
//	// ���nOutPlayerCount��DWORD
//}TKACKUPDATEWINNERINFO, *PTKACKUPDATEWINNERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��·����ɷ����û�������ֵ��Ϣ
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_GROWVALUE (TK_MSG_GAME_RELAY_BEGIN + 19)
// ACK:
//  TKHEADER header;
//	DWORD dwUserID
//  int cnGrowValue
//  cnGrowValue��GROWINFO(DWORD dwGrowID, DWORD dwValue)

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ɷ����û���־����������Ƿ�������־��
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_CHANGEFLAGS (TK_MSG_GAME_RELAY_BEGIN + 20)
// ACK:
//  TKHEADER header;
//	DWORD dwUserID
//  WORD wFlags   ,�μ�TKMatchRoomUniversalDicSrv.h��TK_ROOMPLAYER_FLAG����

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ɷ����û���������(��ָ����λ���ϵ����߿�)
//   ˵    ����GC -> GS -> MS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_HOSTKICK (TK_MSG_GAME_RELAY_BEGIN + 21)
// REQ
//  TKHEADER header;
//  int nSeat;                          // ��λ��(base 0)

// ACK
//  TKHEADER hdr;
//  int nNotAgainEnter;        //1=��ֹ�ٴν���
//  int nErrorCode;            //

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������ѯ���û��Ƿ�ͬ�⻨�ѽ�Ҽ�Ѫ��������ֵ
//   ˵    ����MS -> GS -> GC
//             ����û�ͬ������TK_MSG_GAME_ADDHP��Ϣ
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ROOMPLAYER_QUERYADDHP (TK_MSG_GAME_RELAY_BEGIN + 22)
// ACK:
//  TKHEADER header;
//	int			nCurLife;			// ��ǰ����ֵ
//	int			nExchangeRate;		// �һ���
//	int			nAddLife;			// ��Ҫ���������ֵ�����nAddLifeΪ0������Ѫƿ����ѡ���Ѫ��
//	int			nCostCoin;			// ��Ҫ���ѵĽ����
//	int			cnHematinic;		// ��ѡ��Ѫƿ����
//	HEMATINIC	asHematinic[ MAX_HEMATINIC_COUNT ];	// ÿ��Ѫƿ����ϸ��Ϣ
//  string      ѯ�ʼ�Ѫ��������ʾ�ַ���������������\0��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_STARTABLE_ACCEPT_INVITE (TK_MSG_GAME_RELAY_BEGIN + 23)
// ACK
//  TKHEADER header;
// REQ
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_TERMINATE_GC (TK_MSG_GAME_RELAY_BEGIN + 24)
// ACK
//  TKHEADER header;
// REQ
//  TKHEADER header;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�����ѡ���б���Ϣ�ֶζ���
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCHPLAYERLIST_DEFINE (TK_MSG_GAME_RELAY_BEGIN + 25)
// ACK:
//  TKHEADER header;
//	String (xml�ַ���������������\0)
//
//xml��ʽ:
//			<PlayerInfo>
//				<F name="�û���" id="F1"/>
//				<F name="����" id="F2" GrowID="1001201"/>   //(���GrowID��Ϊ0����ʾ��Ҫ�ɴ�����������ַ���)
//				<F name="����" id="F3" />
//				<F name="ս����" id="F4" />
//			</PlayerInfo>

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ӻ���±���ѡ���б���Ϣ
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCHPLAYERLIST_ADD (TK_MSG_GAME_RELAY_BEGIN + 26)
// ACK:
//  TKHEADER header;
//	String (xml�ַ���������������\0)
//
//xml��ʽ:
//		<PlayerInfo>
//			<P UID="100003086" F1="����" F2="2100" F3="1000" F4="796"/>
//			<P UID="100001005" F1="����" F2="2100" F3="1000" F4="796"/>
//		</PlayerInfo>

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�ɾ������ѡ���б���Ϣ
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCHPLAYERLIST_DEL (TK_MSG_GAME_RELAY_BEGIN + 27)
// ACK:
//  TKHEADER header;
//		header.dwParam: ��Ҫɾ����UerID

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��������л�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_STAR_TABLE_SEL (TK_MSG_GAME_RELAY_BEGIN + 28)
// REQ:
// TKHEADER hdr;
//   hdr.dwParam: target star-table ID
// ACK:
// TKHEADER hdr;
//   int nCount;
//   TK_STAR_TABLE_SEL_REC[nCount];

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�����Կ�������ȷ�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_DUALMEET_SCORE (TK_MSG_GAME_RELAY_BEGIN + 29)
// REQ/ACK:
// TKHEADER hdr;
// data
struct GSTeamScore
{
    char szTeamName[TK_BUFLEN_USERNAME * 2];
    int nTeamScore;
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ݽ������Ƿ������ʱ�������
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_GAMBLE_CAN_REBUY (TK_MSG_GAME_RELAY_BEGIN + 30)
// REQ/ACK:
// TKHEADER hdr;
// int nJackpot ���ش�С
// int nGold;
// int nChip;

//-------------------------------------------------------------------------------------------------------------------------
//
//	��    �ܣ�MS֪ͨGS����Ҹ���
// ˵    ����MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_RELIVE_SELECT (TK_MSG_GAME_RELAY_BEGIN + 31)
// REQ:
// TKHEADER hdr;
// int cnLivePlayer;	// ��ǰ�������
// int nTimeOut;		// ����ʱ
// <select><case id = "1" tip = "����100��һ��1000����"><case id = "2" tip = "����200��һ��2000����"><case id = "3" tip = "����10���ʯ���1000����"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
// ��    �ܣ��û����󸴻�
// ˵    ����GC -> GS -> MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_USER_REQRELIVE (TK_MSG_GAME_RELAY_BEGIN + 32)
// REQ
// TKHEADER hdr;
//	int nSecondUsed;	// ��ʱ
//	int nReliveCase;		// ѡ��ĸ����

//-------------------------------------------------------------------------------------------------------------------------
//
//	��    �ܣ�MS֪ͨGS�����add on
// ˵    ����MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_ADDON_SELECT (TK_MSG_GAME_RELAY_BEGIN + 33)
// REQ:
// TKHEADER hdr;
// int nTimeOut;		// ����ʱ
// <select><case id = "0" tip = "����100��һ��1000����"><case id = "1" tip = "����200��һ��2000����"><case id = "2" tip = "����10���ʯ���1000����"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
// ��    �ܣ��û�����add on
// ˵    ����GC -> GS -> MS
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_USER_REQADDON (TK_MSG_GAME_RELAY_BEGIN + 34)
// REQ
// TKHEADER hdr;
//	int nAddonCase;		// ѡ���addon����

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�����Կ�������ȷ�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_DUALMEET_SCOREEX (TK_MSG_GAME_RELAY_BEGIN + 35)
// REQ/ACK:
// TKHEADER hdr;
// data
//struct GSTeamScore
//{
//	char szTeamName[ TK_BUFLEN_USERNAME * 2 ];
//	int 	nTeamScore;
//};

//-------------------------------------------------------------------------------------------------------------------------
//
//	��    �ܣ�MS֪ͨ�����ʤ������Ϣ
// ˵    ����MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_CV_AWARD_INFO (TK_MSG_GAME_RELAY_BEGIN + 36)
// REQ:
// TKHEADER hdr;
// xml�ַ���

//-------------------------------------------------------------------------------------------------------------------------
//
//	��    �ܣ�MS֪ͨ��һ����ʤ����
// ˵    ����MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_CV_AWARD (TK_MSG_GAME_RELAY_BEGIN + 37)
// REQ:
// TKHEADER hdr;
// DWORD cnWin	// ��ʤ����

//-------------------------------------------------------------------------------------------------------------------------
//
//	��    �ܣ�MS֪ͨ��һ����ʤ����
// ˵    ����MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_REST (TK_MSG_GAME_RELAY_BEGIN + 38)
// REQ:
// TKHEADER hdr;
// ���
typedef struct tagGAMEPLAYERINFO
{
    int nRestTime;                               // ����Ϣʱ��,����������Ҳ��������ť��Ϸ���Զ���ʼ
    int cnHematinic;                             // ��ѡ��Ѫƿ����
    HEMATINIC asHematinic[MAX_HEMATINIC_COUNT];  // ÿ��Ѫƿ����ϸ��Ϣ

    int nLife;    // ��ǰ����ֵ
    int nCoin;    // �ɻضҽ����
    int nScore;   // ����
    int cnGames;  // ������Ƹ���

    int nAwardTimeSpan;        //�佱ʱ����(��),<=0��ʾ���佱
    int nNextAwardLeftSecond;  //������һ�ΰ佱��ʣ��ʱ��(��)
} GAMEPLAYERINFO, *PGAMEPLAYERINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ� MS ����Team�б�
//   ˵    ���� 1��MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MSG_TEAM_LIST (TK_MSG_GAME_RELAY_BEGIN + 39)
// REQ
// TKHEADER hdr;
// int cnTeam;
// ���cnTeam��GS2GCTeamInfo
struct GS2GCTeamInfo
{
    char szTeamName[TK_BUFLEN_USERNAME];
    int nScore;
    int nOrder;  // ����Ϊ����ʾ����
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ� MS ��������б�
//   ˵    ���� 1��MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MSG_PLAYER_LIST (TK_MSG_GAME_RELAY_BEGIN + 40)
// REQ
// TKHEADER hdr;
// int cnPlayer;
// ���cnPlayer��GS2GCPlayerInfo
struct GS2GCPlayerInfo
{
    DWORD dwUserID;
    char szUserName[TK_BUFLEN_USERNAME];
    int nScore;
    BOOL bOut;  // �Ƿ����
    DWORD dwGSIP;
    DWORD dwBSIP;
    int nGSPort;
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ� MS ������ҵȴ���Ϣ
//   ˵    ���� 1��MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MSG_TEAM_WAIT (TK_MSG_GAME_RELAY_BEGIN + 41)
// REQ/ACK:
// TKHEADER hdr;
// ����ַ���

//-----------------------------------------------------------------------------------------
//   ��    �ܣ� MS ���ͽ׶ν�����Ϣ
//   ˵    ���� 1��MS -> GS -> GC
//-----------------------------------------------------------------------------------------
#define TKID_MSG_TEAM_PHASEOVER (TK_MSG_GAME_RELAY_BEGIN + 42)
// REQ/ACK:
// TKHEADER hdr;
// DWORD dwFTPUserID;
// DWORD dwRestSecond;
// DWORD cnScoreBase;
// ���äע��Ϣ

//-------------------------------------------------------------------------------------------------------------------------
//
//	��    �ܣ�MS֪ͨGS����Ҹ���
// ˵    ����MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_RELIVE_SELECTEX (TK_MSG_GAME_RELAY_BEGIN + 43)
// REQ:
// TKHEADER hdr;
// int nJackpot;			//�ܽ����
// int cnLivePlayer;	// ��ǰ�������
// int nAverageChip;	// ƽ������
// int nScoreBase;		// ��ǰäע
// int nReliveLimitTime;// ����ʱ��
// int nElapsedTime;	// �ѽ�������
// int nTimeOut;		// ����ʱ
// <select><case id = "0" tip = "����100��һ��1000����"><case id = "1" tip = "����200��һ��2000����"><case id = "2" tip = "����10���ʯ���1000����"></select>

//-------------------------------------------------------------------------------------------------------------------------
//
//	��    �ܣ�MS֪ͨGS�����add on
// ˵    ����MS -> GS -> GC
//
//-------------------------------------------------------------------------------------------------------------------------
#define TKID_MSG_GAME_ADDON_SELECTEX (TK_MSG_GAME_RELAY_BEGIN + 44)
// REQ:
// TKHEADER hdr;
// int nTimeOut;		// ����ʱ
// int nJackpot;			// �ܽ����
// int cnLivePlayer;	// ��ǰ�������
// int nAverageChip;	// ƽ������
// int nScoreBase;		// ��ǰäע
// int anScoreBase[ 7 ];// �½׶�äע
// string <select><case id = "0" tip = "����100��һ��1000����"><case id = "1" tip = "����200��һ��2000����"><case id = "2" tip = "����10���ʯ���1000����"></select>
// int nElapseSecond	// �Ѿ��ȴ�������

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����ݽ���������ر仯
//   ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_JACKPOT_RAISE (TK_MSG_GAME_RELAY_BEGIN + 45)
// REQ:
// TKHEADER hdr;
// int nJackpot ���ش�С
// int nAverageChip;	 ƽ������

//-----------------------------------------------------------------------------------------
//   ��    �ܣ���Ϸ���л�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_CHGTABLE (TK_MSG_GAME_RELAY_BEGIN + 46)
// REQ
//  TKHEADER header;
// ACK
//  TKHEADER header;
//  hdr.dwParam: target table ID

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�����/�˳�/���µȴ�����
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_WAITINLIST (TK_MSG_GAME_RELAY_BEGIN + 47)
// REQ
//  TKHEADER header;
// hdr.dwParam: 0�˳��ȴ����У�1����ȴ�����
// ACK
//  TKHEADER header;
//  hdr.dwParam: 0���ڵȴ����У�n(>0)�����е�λ��
//  DWORD dwWaitCount: �ȴ�����������

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�����ѡ��
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCH_OPTION (TK_MSG_GAME_RELAY_BEGIN + 48)
// REQ
//  TKHEADER header;
//  DWORD dwFlags: Bit0:֧�ֽ�����а�ť    Bit1:֧�ֻ�����ť Bit2:�Զ�rush

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��ϴ��û��Ƿ���"�Զ��������"checkbox
//   ˵    ����GC-->GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_AUTO_ADDHP_TOUCHED (TK_MSG_GAME_RELAY_BEGIN + 49)
// REQ:
//  TKHEADER header;
//  header.dwParam: 0: û��� 1: �����

//-----------------------------------------------------------------------------------------
//
//	��    �ܣ�֪ͨ�����LuckCard������Ϣ
// ˵    ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LUCKCARDINFO (TK_MSG_GAME_RELAY_BEGIN + 50)
// REQ
// TKHEADER hdr;
// ���xml����ʽ����:
// <LuckCard>
// 	<!--AddCfg���˵㲹�����ã�ItemΪ��ѡ����DefΪĬ�ϵ�-->
// 	<AddCfg Item="10000,20000,50000,100000,200000" Def="100000"/>
//
// 	<!--UseMin���˵�Ӧ����С���ƣ����ڴ���ֵ��Ӧ�����˵㣩��ItemΪ��ѡ����DefΪĬ�ϵ�-->
// 	<UseMin Item="0,10000" Def="10000"/>
//
// 	<!--UseMin���˵�Ӧ��������ƣ����ڴ���ֵʱ�޶�Ϊ����ֵ����ItemΪ��ѡ����DefΪĬ�ϵ�-->
// 	<UseMax Item="20000,50000" Def="20000"/>
// </LuckCard>

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û�LuckCardѡ��
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LUCKCARD_USEROPTION (TK_MSG_GAME_RELAY_BEGIN + 51)
// REQ
//  TKHEADER header;
//  int nUseMin ����ʹ�����ޣ����ڸ�ֵ��Ӧ�ã�
//  int nUseMax ����ʹ�����ޣ����ڸ�ֵ����ֵӦ�ã�

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û��ϴ�λ����Ϣ�����������
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PLAYER_SCAN (TK_MSG_GAME_RELAY_BEGIN + 52)
// REQ/ACK:
// TKHEADER hdr;
// double lat;
// double lng;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û��������ָ����
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_ENTER_TABLE (TK_MSG_GAME_RELAY_BEGIN + 53)
// REQ/ACK:
// TKHEADER hdr;
// DWORD dwTableID;
// ����ACK header.dwParam : 1 == û���������		2 == ָ������������    3 == ������������Ϸ

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û������뿪����
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_LEAVE_TABLE (TK_MSG_GAME_RELAY_BEGIN + 54)
// REQ/ACK:
// TKHEADER hdr;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û������ʵ�
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_BILL (TK_MSG_GAME_RELAY_BEGIN + 55)
////////// REQ:
// TKHEADER hdr;

/////////ACK
// TKHEADER hdr;
// TKGameLBSBill bill;		// �ʵ���ϸ
struct TKGameLBSBill
{
    struct PlayerBill
    {
        DWORD dwUserID;
        int nInitChip;   // ��ʼ����
        int nRebuyChip;  // �������
        int nChip;       // ��ǰ����
    };

    int cnPlayer;
    // ���cnPlayer��PlayerBill
};

//��������Ϣ
//MS->GS ��������ʼ
#define TKID_MSG_GAME_FORWARDBEGIN (TK_MSG_GAME_RELAY_BEGIN + 56)
typedef struct tagTKGSFORWARDBEGIN
{
    TKHEADER header;
    size_t timelong;   //����������ʱ������λs
    UINT64 timebegin;  //������ʼʱ�䵥λ 0.1΢s
    tagTKGSFORWARDBEGIN()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDBEGIN, *PTKGSFORWARDBEGIN;

//MS<->GS ����ǰ�����б�ͻظ�
#define TKID_MSG_GAME_FORWARDORDER (TK_MSG_GAME_RELAY_BEGIN + 57)
typedef struct tagTKGSFORWARDORDERREQ
{
    TKHEADER header;
    tagTKGSFORWARDORDERREQ()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDORDERREQ, *PTKGSFORWARDORDERREQ;
struct gsForwardplayerinfo
{
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];
    int nGrade;      //�������ɼ�
    size_t nRank;    //����������
    size_t usetime;  //������ʹ��ʱ��
    size_t hand;     //����ɾ���
    gsForwardplayerinfo()
    {
        memset(this, 0, sizeof(*this));
    }
};
typedef struct tagTKGSFORWARDORDERACK
{
    TKHEADER header;
    size_t count;    //��׺gsForwardplayerinfo����
    bool iscurdata;  //false��ʾĿǰ�������������н׶Σ���ʾ�ϴε����ݣ����Ҳ������Լ���true��1���ڵ����Լ�
    tagTKGSFORWARDORDERACK()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDORDERACK, *PTKGSFORWARDORDERACK;

//MS->GS ����������ÿ��beginhand��֪ͨ��Ϣ
#define TKID_MSG_GAME_FORWARDBEGINHAND (TK_MSG_GAME_RELAY_BEGIN + 58)
typedef struct tagTKGSFORWARDBEGINHAND
{
    TKHEADER header;
    size_t hand;         //��ǰ����
    size_t totalhand;    //�ܾ���
    size_t rank;         //��ǰ����
    size_t playercount;  //��ǰ������
    tagTKGSFORWARDBEGINHAND()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDBEGINHAND, *PTKGSFORWARDBEGINHAND;

//MS->GS������������Ϣ
#define TKID_MSG_GAME_FORWARDEND (TK_MSG_GAME_RELAY_BEGIN + 59)
typedef struct tagTKGSFORWARDEND
{
    TKHEADER header;
    size_t timelong;  //������һ����������ʱ������λs
    UINT64 timeend;   //��������ʱ�䵥λ 0.1΢s
    tagTKGSFORWARDEND()
    {
        memset(this, 0, sizeof(*this));
    }
} TKGSFORWARDEND, *PTKGSFORWARDEND;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û�����������Ϣ
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_SEAT_RANK_INFO (TK_MSG_GAME_RELAY_BEGIN + 60)
////////// REQ:
struct TKGameReqGetSeatRankInfo
{
    TKHEADER header;
};

////////// ACK:
struct TKGameAckGetSeatRankInfo
{
    TKHEADER header;
    WORD wGameCount;
    WORD wXmlOffset;
    WORD wXmlLen;
    // char * xml

    inline const char* RankInfo()
    {
        return (const char*)((char*)this + wXmlOffset);
    };
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Խ��������˵�(���ƹ����еĵ÷ֻ���)
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_BILL (TK_MSG_GAME_RELAY_BEGIN + 61)
/////////// REQ
// TKHEADER hdr;
// DWORD dwTableID;

/////////// ACK
// TKHEADER hdr;
// TKGameFdTableBill bill;		// �Խ������㵥
struct TKGameFdTableBill
{
    struct PlayerBill
    {
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // ����ǳ�
        int nBuyInChip;                       // �������
        int nMatchScore;                      // ������Ӯ
        int nTotalScore;                      // �ܻ���
        int nStatus;                          // �û�״̬(1������    2��վ��    3���뿪    4���Թ�)
    };

    int cnPlayer;  // ���cnPlayer��PlayerBill
    int nGrade;    // ����
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Խ��������˵�(���ս����˵�)
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_FINAL_BILL (TK_MSG_GAME_RELAY_BEGIN + 62)
/////////// REQ
// TKHEADER hdr;
// DWORD dwTableID;

/////////// ACK
// TKHEADER hdr;
// TKGameFdTableFinalBill bill; // �Խ������㵥
struct TKGameFdTableFinalBill
{
    struct PlayerBill
    {
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // ����ǳ�
        int nBuyInChip;                       // ������ֳ���
        int nMatchScore;                      // ������Ӯ
        int nTotalScore;                      // �ܻ���
    };

    int cnPlayer;         // ���cnPlayer��PlayerBill
    int nGrade;           // ����
    int nTableTime;       // ʱ��(����)
    int nTotalGameCount;  // �ܾ���
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Խ�����������
//   ˵    ����MS -> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_PERMITSET (TK_MSG_GAME_RELAY_BEGIN + 63)
// ACK
// TKHEADER hdr;
// char		szNickName[TK_BUFLEN_NICKNAME];		// ����ǳ�
// DWORD dwUserID;
// int nSeat;
//int nWaitPermitSet;

// REQ
// TKHEADER hdr;//hdr.dwParam: 0: ��ͬ�� 1: ͬ��
// DWORD dwUserID;
// int nSeat;
// int nCancel; //1:ȡ��

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Խ����ּ�����
//   ˵    ����MS -> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_SCORE (TK_MSG_GAME_RELAY_BEGIN + 64)
// TKHEADER hdr;
// TKGameFdTableScore score;    //�÷�
struct TKGameFdTableScore
{
    struct PlayerScore
    {
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // ����ǳ�
        int nMatchScore;                      // ���ֵ÷�
        int nTotalScore;                      // �ܻ���
    };
    int cnPlayer;                             // ���cnPlayer��PlayerScore
    int nGrade;                               // ����
    int nTotalGameCount;                      // �ܾ���
    char szTitle[TK_BUFLEN_TOURNEYFULLNAME];  //��������
    DWORD dwTableID;
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������ػ������Խ���������Ϣ
//   ˵    ����MS -> GS
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_INFO (TK_MSG_GAME_RELAY_BEGIN + 65)
//TKHEADER hdr;
//DWORD dwTableID;
//int nGrade;
//int nInitScore;
//int nBase;
//int nGameTime;
//int nPermitSet;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������ǰ��ɢ
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_PREGAMEEND (TK_MSG_GAME_RELAY_BEGIN + 66)
//REQ/ACK
//TKHEADER hdr;
//DWORD dwTableID;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ������ƽ�(����������)(game)
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDROOM_ROOMHOST_INHERIT (TK_MSG_GAME_RELAY_BEGIN + 67)
//REQ
//TKHEADER hdr;
//DWORD dwRoomID;
//DWORD dwRoomHostID;
//DWORD dwInheritRoomHostID;
//int nReason;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�����ת��(game)
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDROOM_TRANSFER (TK_MSG_GAME_RELAY_BEGIN + 68)
//REQ
//TKHEADER hdr;
//DWORD dwRoomID;
//WORD wReserve;

//ACK
//TKHEADER hdr;
//DWORD dwRoomID;
//WORD wReserve;

//-----------------------------------------------------------------------------------------
// �� �ܣ����³���ׯ�Ŷ��б�
// ˵ ����MS -> GS -> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_PUSH_SUPERBANKER_LIST (TK_MSG_GAME_RELAY_BEGIN + 69)
//REQ
//TKHEADER hdr;
// DWORD dwWaitCount
// ���dwWaitCount��GS2MCBankerInfo
struct GS2MCBankerInfo
{
    DWORD dwUserID;
    DWORD dwFormID;  // �û�����ID
    int nFigureID;   // ͷ��ID
    char szNickName[TK_BUFLEN_NICKNAME];
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Խ����ּ�������չ
//   ˵    ����GS -> MC
//   ��    ע������TKID_MS2GS_FDTABLE_SCOREЭ��
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_SCOREEX (TK_MSG_GAME_RELAY_BEGIN + 70)
// TK_PLAYER_MSG_RELAY_HDR hdr;
// TKFDTABLESCOREEX scoreex;    //�÷�
// �����cnPlayer��PlayerScore

typedef struct tagTKGAMEFDTABLESCOREEX
{
    int nOffsetInfo;  // =sizeof(TKGAMEFDTABLESCOREEX)
    int cnPlayer;     // ���cnPlayer��PlayerScore
    tagTKGAMEFDTABLESCOREEX()
    {
        memset(this, 0, sizeof(*this));
    }

    struct PlayerScore
    {
        int nSizeofInfo;  // =sizeof(PlayerScore)
        DWORD dwUserID;
        char szNickName[TK_BUFLEN_NICKNAME];  // ����ǳ�
        int nMatchScore;                      // ���ֵ÷�
        int nTotalScore;                      // �ܻ���
        int nWinCount;                        // ��ʤ����
        int nTotalCount;                      // �ܾ���
        PlayerScore()
        {
            memset(this, 0, sizeof(*this));
        }
    };
} TKGAMEFDTABLESCOREEX, *PTKGAMEFDTABLESCOREEX;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��Խ����ֶ�����
//   ˵    ����MS <-> GS <-> GC
//   ��    ע��
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_FDTABLE_MANUALBEGIN (TK_MSG_GAME_RELAY_BEGIN + 71)
//REQ/ACK
//TKHEADER hdr;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û����͸�����Ϣ
//   ˵    ����GC->GameSvr -> MatchSvr
//   ��    ע��
//-----------------------------------------------------------------------------------------
#define TKID_MSG_GAME_HDL_INFO (TK_MSG_GAME_RELAY_BEGIN + 72)
// REQ
// dwResult
// sufData

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�MS֪ͨ�ͻ��˽���
//   ˵    ����MS -> GS -> GC
//   ��    ע��
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_PROCESSAWARD (TK_MSG_GAME_RELAY_BEGIN + 73)
//TKHEADER hdr;
//DWORD dwCount; //(ѭ�����ݰ���dwUserID+dwBizID+dwAwardID+dwLen+award)
//DWORD dwUserID; //
//DWORD dwBizID //���̽���ҵ��ID
//DWORD dwAwardID //���̽���ID
//DWORD dwLen //����json������
//json  award //���̽���json��
//...

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�GC֪ͨMS����
//   ˵    ����GC -> GS -> MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OPENREDPACKET (TK_MSG_GAME_RELAY_BEGIN + 74)
//TKHEADER hdr;
//DWORD dwRPID; //���ID����Ӧ�ϸ���Ϣ�е�dwAwardID
//DWORD dwBizID; //���̽���ҵ��ID
//DWORD dwPlanID; //���̽�������ID
//BOOL  bFinal; //�Ƿ����һ�����

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�GC֪ͨMS����
//   ˵    ����GC <-> GS <-> MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_CONFIRMAWARDINFO (TK_MSG_GAME_RELAY_BEGIN + 75)
//REQ:
//TKHEADER hdr;
//DWORD dwCount;
//��׺dwCount����dwAwardID+dwBizID+dwPlanID��

//ACK:
//TKHEADER hdr;
//DWORD dwCount;
//��׺dwCount��dwAwardID

//-----------------------------------------------------------------------------------------
//���ܣ�GC��MS���͸��ͨ����Ϣ������͸����Ϣֻ����������dwProtocolType����׺���ݸ�ʽ��������������ת
//˵����GC <-> GS <-> MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_COMMONPROTOCOL (TK_MSG_GAME_RELAY_BEGIN + 76)
//TKHEADER hdr;
//DWORD dwProtocolType; //���־������Ϣ
//TKSUFFIXIDX suffixJsonData; ��׺ҵ������
//json

//�ͻ��������������͸��ͨ��Э��
#define GC_MS_COMMONPROTOCOL_QUIT_MATCH 0  //�ͻ�����������

//-----------------------------------------------------------------------------------------
//   ��    �ܣ�������Ϸ�������������ͻ������͸�ʽ�������Ϣ
//   ˵    ����GS --> GC
//
//-----------------------------------------------------------------------------------------
#define TKID_GS2MC_PUSHDUPLEXRESULT (TK_MSG_GAME_RELAY_END + 1)
typedef struct tagTKACKGSPUSHDUPLEXRESULT
{
    TKHEADER header;
    int lenText;  //�ı����� ����������\0
} TKACKGSPUSHDUPLEXRESULT, *PTKACKGSPUSHDUPLEXRESULT;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û���������Ϣ
//   ˵    ����GC<-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_ROUND_RESULT (TK_MSG_GAME_RELAY_END + 2)
////////// REQ:
struct TKGameReqGetRoundResult
{
    TKHEADER header;
    WORD wCardSetNo;
};

////////// ACK:
struct TKGameAckGetRoundResult
{
    TKHEADER header;
    WORD wResultCount;
    WORD wResultOffset;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wCardSetNo;

    // ���wResultCount��SdgrRoundResult
    // SdgrRoundResult array

    //	inline SdgrRoundResult * ResultList(){ return ( SdgrRoundResult * )( ( char * )this + wResultOffset ); };
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û�����¼����Ϣ
//   ˵    ����GC<-> GS
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GET_ROUND_RECORD (TK_MSG_GAME_RELAY_END + 3)
////////// REQ:
struct TKGameReqGetRoundRecord
{
    TKHEADER header;
    WORD wRoundID;
};

////////// ACK:
struct TKGameAckGetRoundRecord
{
    TKHEADER header;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wRecordLen;
    WORD wRecordOffset;

    // ���xml�ַ���
    // char * xml;

    inline const char* RoundRecord()
    {
        return (const char*)((char*)this + wRecordOffset);
    };
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û�����������ȴ浵
//   ˵    ����MS <-> GS <-> GC
//
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_MATCH_ARCHIVE (TK_MSG_GAME_RELAY_END + 4)
////////// REQ:
struct TKGameReqMatchArchive
{
    TKHEADER header;
    BYTE byArchive;  // �浵��ǣ�0��ʾȡ���浵��1��ʾ�浵
};

////////// ACK:
struct TKGameAckMatchArchive
{
    TKHEADER header;
};

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û����󽻻���Ȩ
//   ˵    ����GC->GS->MS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_GETCARDRIGHT (TK_MSG_GAME_RELAY_END + 5)
typedef struct tagTKGameReqGetCardRight
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwWatcheeID;
    int nOperationMode;  // 0-�Թ�������1-Playerͬ�⣬2-Player�ܾ�

} TKGAMEREQGETCARDRIGHT, *PTKGAMEREQGETCARDRIGHT;

typedef struct tagTKGameAckGetCardRight
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwWatcheeID;
    int nOperationMode;  // 0-�Թ�������1-Playerͬ�⣬2-Player�ܾ�
    int nFigureID;
    int nWatcherFigureID;
    char szNickName[TK_BUFLEN_NICKNAME];
    char szWatcherNickName[TK_BUFLEN_NICKNAME];

} TKGAMEACKGETCARDRIGHT, *PTKGAMEACKGETCARDRIGHT;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��û����󱣴��ƾ���Ϣ
//   ˵    ����GC->GS
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_SAVECARDINFO (TK_MSG_GAME_RELAY_END + 6)
typedef struct tagTKGameReqSaveCardInfo
{
    TKHEADER header;
    DWORD dwUserID;
    DWORD dwTourneyID;

    DWORD dwReserve1;  // ����1
    DWORD dwReserve2;  // ����2

} TKGAMEREQSAVECARDINFO, *PTKGAMEREQSAVECARDINFO;

//-----------------------------------------------------------------------------------------
//   ��    �ܣ��򿪽������
//   ˵    ����GS->GC
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_OPEN_ACCOUTINTERFACE (TK_MSG_GAME_RELAY_END + 7)
typedef struct tagTKGameReqOpenAccountInterface
{
    TKHEADER header;
    DWORD dwMPID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwUserID;
    DWORD dwReserve1;     //�����ֶ�1
    DWORD dwReserve2;     //�����ֶ�2
    TKSUFFIXIDX sufData;  //��׺����
} TKGAMEREQOPENACCOUNTINTERFACE, *PTKGAMEREQOPENACCOUNTINTERFACE;
//��׺����"<open name=%s order=%d formid=%u figid=%u memname=%s memorder=%u memformid=%u,memfigid=%u memstate=%u total=%u />"

//-----------------------------------------------------------------------------------------
//   ��    �ܣ����½�����Ϣ
//   ˵    ����GS->GC
//-----------------------------------------------------------------------------------------
#define TK_MSG_GAME_UPDATE_ACCOUTINFOR (TK_MSG_GAME_RELAY_END + 8)
typedef TKGAMEREQOPENACCOUNTINTERFACE TKGAMEREQUPDATEACCOUNTINFOR;
// ��׺����"<update memname=%s memorder=%u memformid=%u,memfigid=%u memstate=%u total=%u />"

//-----------------------------------------------------------------------------------------
//   ��    �ܣ� GS��GTS��������ݽ���
//   ˵    ���� GS <-> GTS
//-----------------------------------------------------------------------------------------
#define TKID_GS2GTS_GAMEMSG (TK_MSG_GAME_RELAY_END + 651)
typedef struct tagTKREQGS2GTSGAMEMSG
{
    TKHEADER header;

    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;
    WORD wGameIndex;

    WORD wGameID;
    DWORD dwDataOffset;  // ��Ϸ����ƫ��
    DWORD dwDataLength;  // ��Ϸ���ݳ���

} TKREQGS2GTSGAMEMSG, *PTKREQGS2GTSGAMEMSG;

// ----------------------------------------------------------------------------------------
