#pragma once

#include "TKProtocol.h"

//MatchService �� LobbyService Э�鶨�������
//��Ϣ�ţ�TK_MSG_MATCH + TKID_MS2LSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2LSBASE
#define TKID_MS2LSBASE (TK_MSG_MATCH + 0x000)
#endif

//MatchService �� SnsTourneyService Э�鶨�������
//��Ϣ�ţ�TK_MSG_MATCH + TKID_MS2SNSTSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2SNSTSBASE
#define TKID_MS2SNSTSBASE (0x080)
#endif

//MatchService �� GameService Э�鶨�������
//��Ϣ�ţ�TK_MSG_MATCH + TKID_MS2GSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2GSBASE
#define TKID_MS2GSBASE (TK_MSG_MATCH + 0x100)
#endif

//MatchService �� MatchDBService Э�鶨�������
//��Ϣ�ţ�TK_MSG_MATCH + TKID_MS2DBBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2DBBASE
#define TKID_MS2DBBASE (0x200)
#endif

//MatchService �� TimeTourneyService Э�鶨�������
//��Ϣ�ţ�TK_MSG_MATCH + TKID_MS2TTSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2TTSBASE
#define TKID_MS2TTSBASE (0x240)
#endif

//MatchService �� MarketExtendService Э�鶨�������
//��Ϣ�ţ�TK_MSG_MATCH + TKID_MS2MARKETEXTSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2MARKETEXTSBASE
#define TKID_MS2MARKETEXTSBASE (0x280)
#endif

//MatchService �� MatchCom Э�鶨�������
//��Ϣ�ţ�TKID_MS2MCOMBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2MCOMBASE
#define TKID_MS2MCOMBASE (TK_MSG_MATCH + 0x2C0)
#endif

//MatchService �� TourneyInfoService Э�鶨�������
//��Ϣ�ţ�TKID_MS2TISBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2TISBASE
#define TKID_MS2TISBASE (TK_MSG_MATCH + 0x300)  //������ע�������Ķ��岻ͬ
#endif

//MatchService �� WebTourneyService Э�鶨�������
//��Ϣ�ţ�TKID_MS2WTSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2WTSBASE
#define TKID_MS2WTSBASE (TK_MSG_MATCH + 0x340)
#endif

//MatchService �� TKUserMatchInfoService Э�鶨�������
//��Ϣ�ţ�TKID_MS2UMISBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2UMISBASE
#define TKID_MS2UMISBASE (TK_MSG_MATCH + 0x370)
#endif

//MatchService �� MatchInfoService Э�鶨�������
//��Ϣ�ţ�TKID_MS2MISBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2MISBASE
#define TKID_MS2MISBASE (TK_MSG_MATCH + 0x380)
#endif

//MatchService �� TableInfoService Э�鶨�������
//��Ϣ�ţ�TKID_MS2TBISBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2TBISBASE
#define TKID_MS2TBISBASE (TK_MSG_MATCH + 0x3A0)
#endif

//MatchService �� RoomInfoService Э�鶨�������
//��Ϣ�ţ�TKID_MS2RISBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2RISBASE
#define TKID_MS2RISBASE (TK_MSG_MATCH + 0x3C0)
#endif

//MatchService �� TKMobileAwardReceiverService Э�鶨�������
//��Ϣ�ţ�TKID_MS2MARSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2MARSBASE
#define TKID_MS2MARSBASE (TK_MSG_MATCH + 0x3E0)
#endif

//MatchService �� TKBetService Э�鶨�������
//��Ϣ�ţ�TKID_MS2BETSBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2BETSBASE
#define TKID_MS2BETSBASE (TK_MSG_MATCH + 0x400)
#endif

//MatchService �� TKBroadInfoService Э�鶨�������
//��Ϣ�ţ�TKID_MS2BRDISBASE + ��Ϣ��ƫ��
#ifndef TKID_MS2BRDISBASE
#define TKID_MS2BRDISBASE (TK_MSG_MATCH + 0x420)
#endif

//MatchService �� TKMatchAwardInfoService Э�鶨�������
//��Ϣ�ţ�TKID_MS2MSAWIS + ��Ϣ��ƫ��
#ifndef TKID_MS2MAISBASE
#define TKID_MS2MAISBASE (TK_MSG_MATCH + 0x440)
#endif

//MatchService �� TKMatchMomService Э�鶨�������
//��Ϣ�ţ�TKID_MS2MOMS + ��Ϣ��ƫ��
#ifndef TKID_MS2MOMS
#define TKID_MS2MOMS (TK_MSG_MATCH + 0x460)
#endif
