#pragma once

#define module_identify int
#define view_identify int
#define TK_TIPMESSAGE_MAXLEN 256

#ifndef REQ_TYPE
#define REQ_TYPE(type) (TK_REQ | (type))
#define ACK_TYPE(type) (TK_ACK | (type))
#define MSG_LENGTH(type) (sizeof(type) - sizeof(TKHEADER))
#endif
