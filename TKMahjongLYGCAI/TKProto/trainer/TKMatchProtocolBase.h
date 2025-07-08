#pragma once

#include "TKProtocol.h"

//MatchService 与 LobbyService 协议定义基础号
//消息号：TK_MSG_MATCH + TKID_MS2LSBASE + 消息号偏移
#ifndef TKID_MS2LSBASE
#define TKID_MS2LSBASE (TK_MSG_MATCH + 0x000)
#endif

//MatchService 与 SnsTourneyService 协议定义基础号
//消息号：TK_MSG_MATCH + TKID_MS2SNSTSBASE + 消息号偏移
#ifndef TKID_MS2SNSTSBASE
#define TKID_MS2SNSTSBASE (0x080)
#endif

//MatchService 与 GameService 协议定义基础号
//消息号：TK_MSG_MATCH + TKID_MS2GSBASE + 消息号偏移
#ifndef TKID_MS2GSBASE
#define TKID_MS2GSBASE (TK_MSG_MATCH + 0x100)
#endif

//MatchService 与 MatchDBService 协议定义基础号
//消息号：TK_MSG_MATCH + TKID_MS2DBBASE + 消息号偏移
#ifndef TKID_MS2DBBASE
#define TKID_MS2DBBASE (0x200)
#endif

//MatchService 与 TimeTourneyService 协议定义基础号
//消息号：TK_MSG_MATCH + TKID_MS2TTSBASE + 消息号偏移
#ifndef TKID_MS2TTSBASE
#define TKID_MS2TTSBASE (0x240)
#endif

//MatchService 与 MarketExtendService 协议定义基础号
//消息号：TK_MSG_MATCH + TKID_MS2MARKETEXTSBASE + 消息号偏移
#ifndef TKID_MS2MARKETEXTSBASE
#define TKID_MS2MARKETEXTSBASE (0x280)
#endif

//MatchService 与 MatchCom 协议定义基础号
//消息号：TKID_MS2MCOMBASE + 消息号偏移
#ifndef TKID_MS2MCOMBASE
#define TKID_MS2MCOMBASE (TK_MSG_MATCH + 0x2C0)
#endif

//MatchService 与 TourneyInfoService 协议定义基础号
//消息号：TKID_MS2TISBASE + 消息号偏移
#ifndef TKID_MS2TISBASE
#define TKID_MS2TISBASE (TK_MSG_MATCH + 0x300)  //！！！注意和上面的定义不同
#endif

//MatchService 与 WebTourneyService 协议定义基础号
//消息号：TKID_MS2WTSBASE + 消息号偏移
#ifndef TKID_MS2WTSBASE
#define TKID_MS2WTSBASE (TK_MSG_MATCH + 0x340)
#endif

//MatchService 与 TKUserMatchInfoService 协议定义基础号
//消息号：TKID_MS2UMISBASE + 消息号偏移
#ifndef TKID_MS2UMISBASE
#define TKID_MS2UMISBASE (TK_MSG_MATCH + 0x370)
#endif

//MatchService 与 MatchInfoService 协议定义基础号
//消息号：TKID_MS2MISBASE + 消息号偏移
#ifndef TKID_MS2MISBASE
#define TKID_MS2MISBASE (TK_MSG_MATCH + 0x380)
#endif

//MatchService 与 TableInfoService 协议定义基础号
//消息号：TKID_MS2TBISBASE + 消息号偏移
#ifndef TKID_MS2TBISBASE
#define TKID_MS2TBISBASE (TK_MSG_MATCH + 0x3A0)
#endif

//MatchService 与 RoomInfoService 协议定义基础号
//消息号：TKID_MS2RISBASE + 消息号偏移
#ifndef TKID_MS2RISBASE
#define TKID_MS2RISBASE (TK_MSG_MATCH + 0x3C0)
#endif

//MatchService 与 TKMobileAwardReceiverService 协议定义基础号
//消息号：TKID_MS2MARSBASE + 消息号偏移
#ifndef TKID_MS2MARSBASE
#define TKID_MS2MARSBASE (TK_MSG_MATCH + 0x3E0)
#endif

//MatchService 与 TKBetService 协议定义基础号
//消息号：TKID_MS2BETSBASE + 消息号偏移
#ifndef TKID_MS2BETSBASE
#define TKID_MS2BETSBASE (TK_MSG_MATCH + 0x400)
#endif

//MatchService 与 TKBroadInfoService 协议定义基础号
//消息号：TKID_MS2BRDISBASE + 消息号偏移
#ifndef TKID_MS2BRDISBASE
#define TKID_MS2BRDISBASE (TK_MSG_MATCH + 0x420)
#endif

//MatchService 与 TKMatchAwardInfoService 协议定义基础号
//消息号：TKID_MS2MSAWIS + 消息号偏移
#ifndef TKID_MS2MAISBASE
#define TKID_MS2MAISBASE (TK_MSG_MATCH + 0x440)
#endif

//MatchService 与 TKMatchMomService 协议定义基础号
//消息号：TKID_MS2MOMS + 消息号偏移
#ifndef TKID_MS2MOMS
#define TKID_MS2MOMS (TK_MSG_MATCH + 0x460)
#endif
