#pragma once

#include "TKProtocol.h"

//#include "TKSTBillUniversalDic.h"
//#include "TKMoneyUniversalDicSrv.h"

enum eCommiteeType
{
    TK_MATCHCMT_TYPE_NULL = 0,   // ����ί��
    TK_MATCHCMT_TYPE_SIGN = 1,   // ������ί��
    TK_MATCHCMT_TYPE_DATA = 2,   // ������ί��
    TK_MATCHCMT_TYPE_AWARD = 3,  // �佱��ί��
    TK_MATCHCMT_TYPE_EVENT = 4,  // �¼���ί��
    TK_MATCHCMT_TYPE_PROXY = 5,  // ������������������
};

//========================================================================================
// ��������
//========================================================================================

// Tourney���Ͷ���
enum eTourneyType
{
    TK_TOURNEYTYPE_RESERVE = 0,  // 0��Ԥ��

    TK_TOURNEYTYPE_SITANDGO = 1,       // ���������������˼��̿�ʼ�ı������������͵�Tourney������������������ܶ���Զ������е�Match��
    TK_TOURNEYTYPE_TITLE = 2,          // �ǳ��������������͵�Tourneyֻ��һ���������е�Match����ȷ���Ŀ�ʼ������ʱ�䣩
    TK_TOURNEYTYPE_ISLAND = 3,         // ���浺�����������͵�Tourneyֻ��һ���������е�Match����ȷ���Ŀ�ʼ������ʱ�䣩
    TK_TOURNEYTYPE_SERIES = 4,         // ϵ�н�����
    TK_TOURNEYTYPE_USER = 5,           // �û��Խ�����
    TK_TOURNEYTYPE_AMTTBATCH = 6,      // �೵�ೣ���������ÿһ������ʱ���Ƕ�̬�仯�ģ�һ��ʱ��һ��������������һ����������������ʱ��೵��
    TK_TOURNEYTYPE_MINUTE = 7,         // ������Tourney��������Tourney������Ŀ�ʼʱ���
    TK_TOURNEYTYPE_TABLE = 8,          // ɢ����Tourney��������Tourney�кܶ�ͬʱ���еı�����ÿ����������һ�����û�������ʱ�����뿪
    TK_TOURNEYTYPE_BROADCAST_DEL = 9,  // ɾ��wuqy02017.7.28 (ת��Tourney����ת�����ƿͻ��������Ʊ�����״̬������)
    TK_TOURNEYTYPE_RANKBATCH = 10,     // �ȼ��೵tourney
    TK_TOURNEYTYPE_GROUPTITLE = 11,    // ���������(����ü����˵���������2000���飬û����԰佱)
    TK_TOURNEYTYPE_ROOM = 12,          // ���ɷ���
    TK_TOURNEYTYPE_DYNBATCH = 13,      // ��̬������İ೵������
    TK_TOURNEYTYPE_TIMEBATCH = 14,     // ��׼ʱ��೵��������ÿ���೵��ʱ���ǹ̶����õ�
    TK_TOURNEYTYPE_LONGRANGE = 15,     // ���������Զ�������һ�׶Σ�����Ҫ��������
    TK_TOURNEYTYPE_INTRO = 16,         // ���½������͵�Tourney wuqy add 2013.11.21
    TK_TOURNEYTYPE_LADDER = 17,        // ������ģ�������
    TK_TOURNEYTYPE_LBS = 18,           // LBS���������
    TK_TOURNEYTYPE_RANK = 19,          // ������
    TK_TOURNEYTYPE_FDTABLE = 20,       // ������/�Խ��� wuqy 2016.12.11
    TK_TOURNEYTYPE_CUSTOMIZED = 21,    // �Խ�����
    TK_TOURNEYTYPE_DUPLEX = 22,        // ��ʽ��
    TK_TOURNEYTYPE_BREAKING = 23,      // �´�����
    TK_TOURNEYTYPE_COMBATTEAM = 24,    // ս����
    TK_TOURNEYTYPE_ENDGAME = 25,       // �о�
    TK_TOURNEYTYPE_DOUBLELADDER = 26,  // ˫�˰೵

    TK_TOURNEYTYPE_FAMILYMATCH_BEGIN = 27,                                    // �������
    TK_TOURNEYTYPE_FAMILYMATCH_TABLE = TK_TOURNEYTYPE_FAMILYMATCH_BEGIN + 0,  // ��������
    TK_TOURNEYTYPE_FAMILYMATCH_END = 47,                                      // �������

    TK_TOURNEYTYPE_TIME_LIMIT = 48,  // ��ʱ������
    TK_TOURNEYTYPE_CHICKEN = 49,     // �Լ�ģʽ
    TK_TOURNEYTYPE_BOSS = 50,        // ������bossս
    TK_TOURNEYTYPE_STANDARD = 51,    // �����(�齫��)
    TK_TOURNEYTYPE_END
};

// ���û�������Touney״̬
enum eTourneyInterfixUserState
{
    TK_USER_TOURNEYRESERVE = 0,   // 0��Ԥ��
    TK_USER_TOURNEYSIGNUPED = 1,  // �û�������
    TK_USER_TOURNEYUNSIGNUP = 2   // �û�û�б���
};

// �û�����״̬
enum eOutRoundState
{
    TK_USER_INROUNDSTATE = 0,  // �ǳ���
    TK_USER_OUTROUNDSTATE = 1  // ����
};

// ����״̬
enum eMatchSignUpState
{
    TK_MATCHSIGNUP_SUCCESS = 0,  // �����ɹ�
    TK_MATCHSIGNUP_FAILED = 1,   // ����ʧ��

    TK_MATCHSIGNUP_MATCHUNEXIST = 2,  // ����������

    TK_MATCHSIGNUP_PHASENOBEGIN = 3,  // ����û��ʼ
    TK_MATCHSIGNUP_PHASEEND = 4,      // ����ʱ�����

    TK_MATCHSIGNUP_SIGNUPED = 5,  // �Ѿ�����
    TK_MATCHSIGNUP_PLAYERIN = 6,  // �û��Ѿ��ڱ�����
    TK_MATCHSIGNUP_TESTOK = 7,    // ���Ա����ɹ�

    TK_MATCHSIGNUP_GOLDLACK = 8,   // ���������
    TK_MATCHSIGNUP_BONUSLACK = 9,  // ��ȯ������
    TK_MATCHSIGNUP_CERTLACK = 10,  // ����ȯ(��Ʊ)����

    TK_MATCHSIGNUP_FULL = 11,  // �Ѿ�����

    TK_MATCHSIGNUP_MIN_MASTERSCORE = 12,  // �������ٴ�ʦ������
    TK_MATCHSIGNUP_MAX_MASTERSCORE = 13,  // ��������ʦ������
    TK_MATCHSIGNUP_MIN_MATCHSCORE = 14,   // �������ٱ�����������
    TK_MATCHSIGNUP_MAX_MATCHSCORE = 15,   // ������������������

    TK_MATCHSIGNUP_MIN_GAMEMATCHCOUNT = 16,         // ������Ϸ�������ƣ��μӸ���Ϸ����������
    TK_MATCHSIGNUP_MIN_MATCHCOUNT = 17,             // �����ܲ����������ƣ��μ�������Ϸ�ı����ܳ�����
    TK_MATCHSIGNUP_MIN_GAMETIME = 18,               // ������Ϸʱ�����ƣ��룩
    TK_MATCHSIGNUP_MIN_TOTALTIME = 19,              // ������ʱ�����ƣ��룩
    TK_MATCHSIGNUP_NOTINPERMITSIGNUPUSERLIST = 20,  //�������������û��б���

    TK_MATCHSIGNUP_LCVERSION_LOW = 21,  // �ͻ��˰汾�ͣ����ܱ���

    TK_MATCHSIGNUP_GROWLACK = 22,  // ���ֲ���
    TK_MATCHSIGNUP_WARELACK = 23,  // ��Ʒ����

    TK_MATCHSIGNUP_GROWCONDITIONLACK = 24,  // ���ֲ����ϱ�������
    TK_MATCHSIGNUP_WARECONDITIONLACK = 25,  // ��Ʒ�����ϱ�������

    TK_MATCHSIGNUP_MATCHPAUSESIGNUP = 26,    //ϵͳ��������ά����������ͣ����
    TK_MATCHSIGNUP_TABLEPAUSESIGNUP = 27,    //ϵͳ��������ά������ͣ��Ϸ
    TK_MATCHSIGNUP_TABLEUNEXIST = 28,        // ��Ϸ��������
    TK_MATCHSIGNUP_TABLEROOMFULL = 29,       // ��Ϸ�Ѿ���
    TK_MATCHSIGNUP_TABLESIGNUPED = 30,       // �Ѿ�����Ϸ����
    TK_MATCHSIGNUP_TABLEMULTGAMELIMIT = 31,  // ������������������

    TK_MATCHSIGNUP_MSGLEN_FAIL = 32,          // ��Ϣ���ȴ���
    TK_MATCHSIGNUP_PARAM_FAIL = 33,           // ��Ϣ��������
    TK_MATCHSIGNUP_WILDER_FAIL = 34,          // ������δͨ��
    TK_MATCHSIGNUP_SIGNUPSVR_ERR = 35,        // SignSvr�����쳣
    TK_MATCHSIGNUP_SIGNUPSVR_COND_FAIL = 36,  // SignSvr��֤�û���������δͨ��
    TK_MATCHSIGNUP_SIGNUPSVR_FEE_FAIL = 37,   // SignSvr�۳��û���������ʧ��

    TK_MATCHSIGNUP_SIGNUPDATA_LOADING = 38,  // ��������������
    TK_MATCHSIGNUP_RESTRICT = 39,            // �û������Ʋ���
    TK_MATCHSIGNUP_CROWDFUNDING = 40,        // �ڳ��У��޷�����
    TK_MATCHSIGNUP_SAMEIPLIMIT = 41,         // ��ͬIP����
    TK_MATCHSIGNUP_SAMEMACLIMIT = 42,        // ��ͬMac����
    TK_MATCHSIGNUP_HAS_ARCHIVE = 43,         // �����д浵��Ӧ����DAG˫�˰೵��
    TK_MATCHSIGNUP_HAS_CROWDFUND = 44,       // �������ڳӦ����DAG˫�˰೵��
    TK_MATCHSIGNUP_ONLYWEB = 45,             // ����Web���ڲ�������������ͻ��˱���
    TK_MATCHSIGNUP_LOGINUNSAFE = 46,         // ��¼��ȫ��֤�����������������Ʋ���ȫ�û�������Lobby���ƣ�

    TK_MATCHSIGNUP_RELATION_FAIL = 200,     // ���ѹ�ϵ��֤ʧ��
    TK_MATCHSIGNUP_TABLE_NOEXISTENT = 201,  // ���Ӳ�����

    TK_MATCHSIGNUP_WRONG_PWD = 301,             // �������
    TK_MATCHSIGNUP_MAX_SNS_PLAYER_LIMIT = 302,  // ����������������������
    TK_MATCHSIGNUP_TEAM_FULL = 303,             // �Խ�������������
    TK_MATCHSIGNUP_PWD_CNT_OUT = 304,           // �����������
    TK_MATCHSIGNUP_NOSURPORT_LARGERATE = 402,   // ��֧�ָ�ҡ�ڵĶһ���
};

// �ڳ���أ������ڳ�״̬
enum eBuyCrownState
{
    TK_BUYCROWN_SUCCESS = 0,          // ����ɹ�
    TK_BUYCROWN_FAILED = 1,           // ����ʧ��
    TK_BUYCROWN_BRUSH = 2,            // ˢ���û�
    TK_BUYCROWN_USERBUYCNTFULL = 3,   // �û������������
    TK_BUYCROWN_MONEY_SRCLACK = 100,  // ���Ҳ��㣨���˽�ң���ȯ������ȯ��������⣩
    TK_BUYCROWN_GOLD_SRCLACK = 101,   // ��Ҳ���
    TK_BUYCROWN_BONUS_SRCLACK = 102,  // ��ȯ����
    TK_BUYCROWN_CERT_SRCLACK = 103,   // ����ȯ����Ʊ������
    TK_BUYCROWN_WARE_SRCLACK = 104,   // ��Ʒ����
    TK_BUYCROWN_GROW_SRCLACK = 105,   // ���ֲ���
    TK_BUYCROWN_SRCLACK = 106,        // Դ���Ҳ���
    TK_BUYCROWN_DSTACCNOEXIST = 107,  // Ŀ���˻�������
};

// �ڳ���أ������ڳ�״̬
enum eAttachCrownState
{
    TK_ATTACHCROWN_SUCCESS = 0,  // �����ڳ�ɹ�
    TK_ATTACHCROWN_FAILED = 1,   // �����ڳ�ʧ��
};

// ���״̬
enum eMatchUnSignUpState
{
    TK_MATCHUNSIGNUP_SUCCESS = 0,  // ��������ɹ�
    TK_MATCHUNSIGNUP_FAILED = 1,   // �������ʧ��

    TK_MATCHUNSIGNUP_MATCHUNEXIST = 2,  // ����������

    TK_MATCHUNSIGNUP_PHASENOBEGIN = 3,  // ����û��ʼ
    TK_MATCHUNSIGNUP_PHASEEND = 4,      // ����ʱ�����

    TK_MATCHUNSIGNUP_NOSIGNUP = 5,     // ��û�б���
    TK_MATCHUNSIGNUP_PLAYERIN = 6,     // �û��Ѿ��ڱ�����
    TK_MATCHUNSIGNUP_TEAMUNEXIST = 7,  // ���鲻����
    TK_MATCHUNSIGNUP_CUSTOMFINAL = 8,  // �Խ�������ȡ��ʧ��
    TK_MATCHUNSIGNUP_ONLYWEB = 9,      // ����Web���ڲ�������������ͻ���ȡ������

};

// �Թ�״̬
enum eMatchWatchState
{
    TK_MATCHWATCH_SUCCESS = 0,              // �Թ۳ɹ�
    TK_MATCHWATCH_FAILED = 1,               // �Թ�ʧ�ܣ�����ԭ���£�
    TK_MATCHWATCH_TIMEOUT = 2,              // �Թ���Ϣ����ʱ
    TK_MATCHWATCH_MATCHUNEXIST = 3,         // ����������
    TK_MATCHWATCH_WATCHEEUNEXIST = 4,       // ���Թ��߲����ڣ�����ĳ��Round�У�
    TK_MATCHWATCH_ONESELF = 5,              // �Թ��Լ�
    TK_MATCHWATCH_WATCHED = 6,              // ��������Թ�
    TK_MATCHWATCH_WATCH_DROPOUT_ROUND = 7,  // �����Թ���;�˳���Round��Ŀǰ��������;�˳���Ϸ�������ȥ�Թ����˳����Ǹ�Round�е���ң�
    TK_MATCHWATCH_SIGNUP_PLAYERIN = 8,      // �û����Թ��ߣ�����ʱ���ڱ�����
    TK_MATCHWATCH_SIGNUP_FAILED = 9,        // �Թ۱���ʧ��
    TK_MATCHWATCH_SIGNUPED = 10,            // �Ѿ�����
    TK_MATCHWATCH_SIGNUP_SUCCESS = 11       // �Թ۱����ɹ�
};

// Լս״̬
enum eMatchInviteState
{
    TK_MATCH_INVITE_SUCCESS = 0,               // Լս�ɹ�������������ö�����ǰԼս�Ự�е�����ҷ������������Կ�����
    TK_MATCH_INVITE_FAILED = 1,                // Լսʧ��
    TK_MATCH_INVITE_SERVICE_BUSY = 2,          // ����æ
    TK_MATCH_INVITE_MATCHUNEXIST = 3,          // ����������
    TK_MATCH_INVITE_ONESELF = 4,               // ��Լս�����Լ���������Լս�Լ���
    TK_MATCH_INVITE_USERID_INVALID = 5,        // ��Ϣ�����ߣ��Ȳ��ǻỰ�е�Լս�����ߣ�Ҳ����Լս������
    TK_MATCH_INVITE_MISMATCH = 6,              // Լս��������Լս�����߲���ͬһ���Ự��
    TK_MATCH_INVITE_FIND_SESSION_FAILED = 7,   // �Ҳ����Ự
    TK_MATCH_INVITE_SESSION_TIMEOUT = 8,       // Լս�Ự��ʱ
    TK_MATCH_INVITE_SEND_INVITATION_DUP = 9,   // �ظ�����Լս
    TK_MATCH_INVITE_CREATE_TABLE_FAILED = 10,  // ����Tableʧ��
    TK_MATCH_INVITE_ENTER_TABLE_FAILED = 11,   // ����Tableʧ��

    TK_MATCH_INVITE_SESSION_CREATOR_INVALID = 12,  // ��Ч��Լս�Ự������
    TK_MATCH_INVITE_SESSION_JOINER_INVALID = 13,   // ��Ч��Լս�Ự������
    TK_MATCH_INVITE_SESSION_INVALID = 14,          // ��Ч��Լս�Ự
    TK_MATCH_INVITE_CREATE_SESSION_FAILED = 15,    // ����Լս�Ựʧ��
    TK_MATCH_INVITE_JOIN_SESSION_FAILED = 16,      // ����Լս�Ựʧ��
    TK_MATCH_INVITE_CANCEL_SESSION_FAILED = 17,    // ȡ��Լս�Ựʧ��
    TK_MATCH_INVITE_SESSION_CANCELED = 18,         // Լս�Ự�ѱ�ȡ��

};

// ��������
enum eMatchCreateTeamState
{
    TK_MATCH_CREATE_TEAM_SUCCESS = 0,  // �����ɹ�
    TK_MATCH_CREATE_TEAM_FAILED = 1,   // ����ʧ��

    TK_MATCH_CREATE_TEAM_MATCHUNEXIST = TK_MATCHSIGNUP_MATCHUNEXIST,  // ����������

    TK_MATCH_CREATE_TEAM_PHASENOBEGIN = TK_MATCHSIGNUP_PHASENOBEGIN,  // ����û��ʼ
    TK_MATCH_CREATE_TEAM_PHASEEND = TK_MATCHSIGNUP_PHASEEND,          // ����ʱ�����

    TK_MATCH_CREATE_TEAM_SIGNUPED = TK_MATCHSIGNUP_SIGNUPED,  // �Ѿ�����
    TK_MATCH_CREATE_TEAM_PLAYERIN = TK_MATCHSIGNUP_PLAYERIN,  // �û��Ѿ��ڱ�����
    TK_MATCH_CREATE_TEAM_TESTOK = TK_MATCHSIGNUP_TESTOK,      // ���Ա����ɹ�

    TK_MATCH_CREATE_TEAM_GOLDLACK = TK_MATCHSIGNUP_GOLDLACK,    // ���������
    TK_MATCH_CREATE_TEAM_BONUSLACK = TK_MATCHSIGNUP_BONUSLACK,  // ��ȯ������
    TK_MATCH_CREATE_TEAM_CERTLACK = TK_MATCHSIGNUP_CERTLACK,    // ����ȯ(��Ʊ)����

    TK_MATCH_CREATE_TEAM_FULL = TK_MATCHSIGNUP_FULL,  // ������������

    TK_MATCH_CREATE_TEAM_MIN_MASTERSCORE = TK_MATCHSIGNUP_MIN_MASTERSCORE,  // �������ٴ�ʦ������
    TK_MATCH_CREATE_TEAM_MAX_MASTERSCORE = TK_MATCHSIGNUP_MAX_MASTERSCORE,  // ��������ʦ������
    TK_MATCH_CREATE_TEAM_MIN_MATCHSCORE = TK_MATCHSIGNUP_MIN_MATCHSCORE,    // �������ٱ�����������
    TK_MATCH_CREATE_TEAM_MAX_MATCHSCORE = TK_MATCHSIGNUP_MAX_MATCHSCORE,    // ������������������

    TK_MATCH_CREATE_TEAM_MIN_GAMEMATCHCOUNT = TK_MATCHSIGNUP_MIN_GAMEMATCHCOUNT,                // ������Ϸ�������ƣ��μӸ���Ϸ����������
    TK_MATCH_CREATE_TEAM_MIN_MATCHCOUNT = TK_MATCHSIGNUP_MIN_MATCHCOUNT,                        // �����ܲ����������ƣ��μ�������Ϸ�ı����ܳ�����
    TK_MATCH_CREATE_TEAM_MIN_GAMETIME = TK_MATCHSIGNUP_MIN_GAMETIME,                            // ������Ϸʱ�����ƣ��룩
    TK_MATCH_CREATE_TEAM_MIN_TOTALTIME = TK_MATCHSIGNUP_MIN_TOTALTIME,                          // ������ʱ�����ƣ��룩
    TK_MATCH_CREATE_TEAM_NOTINPERMITSIGNUPUSERLIST = TK_MATCHSIGNUP_NOTINPERMITSIGNUPUSERLIST,  //�������������û��б���

    TK_MATCH_CREATE_TEAM_LCVERSION_LOW = TK_MATCHSIGNUP_LCVERSION_LOW,  // �ͻ��˰汾�ͣ����ܱ���

    TK_MATCH_CREATE_TEAM_GROWLACK = TK_MATCHSIGNUP_GROWLACK,  // ���ֲ���
    TK_MATCH_CREATE_TEAM_WARELACK = TK_MATCHSIGNUP_WARELACK,  // ��Ʒ����

    TK_MATCH_CREATE_TEAM_GROWCONDITIONLACK = TK_MATCHSIGNUP_GROWCONDITIONLACK,  // ���ֲ����ϱ�������
    TK_MATCH_CREATE_TEAM_WARECONDITIONLACK = TK_MATCHSIGNUP_WARECONDITIONLACK,  // ��Ʒ�����ϱ�������

    TK_MATCH_CREATE_TEAM_MATCHPAUSESIGNUP = TK_MATCHSIGNUP_MATCHPAUSESIGNUP,  //ϵͳ��������ά����������ͣ����

    TK_MATCH_CREATE_TEAM_NAME_CLASH,    // ��������ͻ
    TK_MATCH_CREATE_TEAM_HAS_ARCHIVE,   // �����д浵��Ӧ����DAG˫�˰೵��
    TK_MATCH_CREATE_TEAM_HAS_CROWDFUND  // �������ڳӦ����DAG˫�˰೵��
};

// �޸Ķ���ѡ��
enum eMatchAlterTeamState
{
    TK_MATCH_ALTER_TEAM_SUCCESS = 0,  // �޸ĳɹ�
    TK_MATCH_ALTER_TEAM_FAILED = 1,   // �޸�ʧ��

    TK_MATCH_ALTER_TEAM_TEAMUNEXIST,   // ���鲻����
    TK_MATCH_ALTER_TEAM_NO_PERMISSION  // û��Ȩ��
};

// �������
enum eMatchJoinTeamState
{
    TK_MATCH_JOIN_TEAM_SUCCESS = 0,  // ����ɹ�
    TK_MATCH_JOIN_TEAM_FAILED = 1,   // ����ʧ��

    TK_MATCH_JOIN_TEAM_MATCHUNEXIST = TK_MATCHSIGNUP_MATCHUNEXIST,  // ����������

    TK_MATCH_JOIN_TEAM_PHASENOBEGIN = TK_MATCHSIGNUP_PHASENOBEGIN,  // ����û��ʼ
    TK_MATCH_JOIN_TEAM_PHASEEND = TK_MATCHSIGNUP_PHASEEND,          // ����ʱ�����

    TK_MATCH_JOIN_TEAM_SIGNUPED = TK_MATCHSIGNUP_SIGNUPED,  // �Ѿ�����
    TK_MATCH_JOIN_TEAM_PLAYERIN = TK_MATCHSIGNUP_PLAYERIN,  // �û��Ѿ��ڱ�����
    TK_MATCH_JOIN_TEAM_TESTOK = TK_MATCHSIGNUP_TESTOK,      // ���Ա����ɹ�

    TK_MATCH_JOIN_TEAM_GOLDLACK = TK_MATCHSIGNUP_GOLDLACK,    // ���������
    TK_MATCH_JOIN_TEAM_BONUSLACK = TK_MATCHSIGNUP_BONUSLACK,  // ��ȯ������
    TK_MATCH_JOIN_TEAM_CERTLACK = TK_MATCHSIGNUP_CERTLACK,    // ����ȯ(��Ʊ)����

    TK_MATCH_JOIN_TEAM_FULL = TK_MATCHSIGNUP_FULL,  // ������������

    TK_MATCH_JOIN_TEAM_MIN_MASTERSCORE = TK_MATCHSIGNUP_MIN_MASTERSCORE,  // �������ٴ�ʦ������
    TK_MATCH_JOIN_TEAM_MAX_MASTERSCORE = TK_MATCHSIGNUP_MAX_MASTERSCORE,  // ��������ʦ������
    TK_MATCH_JOIN_TEAM_MIN_MATCHSCORE = TK_MATCHSIGNUP_MIN_MATCHSCORE,    // �������ٱ�����������
    TK_MATCH_JOIN_TEAM_MAX_MATCHSCORE = TK_MATCHSIGNUP_MAX_MATCHSCORE,    // ������������������

    TK_MATCH_JOIN_TEAM_MIN_GAMEMATCHCOUNT = TK_MATCHSIGNUP_MIN_GAMEMATCHCOUNT,                // ������Ϸ�������ƣ��μӸ���Ϸ����������
    TK_MATCH_JOIN_TEAM_MIN_MATCHCOUNT = TK_MATCHSIGNUP_MIN_MATCHCOUNT,                        // �����ܲ����������ƣ��μ�������Ϸ�ı����ܳ�����
    TK_MATCH_JOIN_TEAM_MIN_GAMETIME = TK_MATCHSIGNUP_MIN_GAMETIME,                            // ������Ϸʱ�����ƣ��룩
    TK_MATCH_JOIN_TEAM_MIN_TOTALTIME = TK_MATCHSIGNUP_MIN_TOTALTIME,                          // ������ʱ�����ƣ��룩
    TK_MATCH_JOIN_TEAM_NOTINPERMITSIGNUPUSERLIST = TK_MATCHSIGNUP_NOTINPERMITSIGNUPUSERLIST,  //�������������û��б���

    TK_MATCH_JOIN_TEAM_LCVERSION_LOW = TK_MATCHSIGNUP_LCVERSION_LOW,  // �ͻ��˰汾�ͣ����ܱ���

    TK_MATCH_JOIN_TEAM_GROWLACK = TK_MATCHSIGNUP_GROWLACK,  // ���ֲ���
    TK_MATCH_JOIN_TEAM_WARELACK = TK_MATCHSIGNUP_WARELACK,  // ��Ʒ����

    TK_MATCH_JOIN_TEAM_GROWCONDITIONLACK = TK_MATCHSIGNUP_GROWCONDITIONLACK,  // ���ֲ����ϱ�������
    TK_MATCH_JOIN_TEAM_WARECONDITIONLACK = TK_MATCHSIGNUP_WARECONDITIONLACK,  // ��Ʒ�����ϱ�������

    TK_MATCH_JOIN_TEAM_MATCHPAUSESIGNUP = TK_MATCHSIGNUP_MATCHPAUSESIGNUP,  //ϵͳ��������ά����������ͣ����

    TK_MATCH_JOIN_TEAM_NUMBER_INVALID,  // ��������ͻ
    TK_MATCH_JOIN_TEAM_TEAMUNEXIST,     // ���鲻����
    TK_MATCH_JOIN_TEAM_HAS_ARCHIVE,     // �����д浵��Ӧ����DAG˫�˰೵��
    TK_MATCH_JOIN_TEAM_HAS_CROWDFUND    // �������ڳӦ����DAG˫�˰೵��
};

// �뿪����״̬
enum eMatchLeaveTeamState
{
    TK_MATCH_LEAVE_TEAM_SUCCESS = 0,  // �뿪����ɹ�
    TK_MATCH_LEAVE_TEAM_FAILED = 1,   // �뿪����ʧ��

    TK_MATCH_LEAVE_TEAM_MATCHUNEXIST = TK_MATCHUNSIGNUP_MATCHUNEXIST,  // ����������

    TK_MATCH_LEAVE_TEAM_PHASENOBEGIN = TK_MATCHUNSIGNUP_PHASENOBEGIN,  // ����û��ʼ
    TK_MATCH_LEAVE_TEAM_PHASEEND = TK_MATCHUNSIGNUP_PHASEEND,          // ����ʱ�����

    TK_MATCH_LEAVE_TEAM_NOSIGNUP = TK_MATCHUNSIGNUP_NOSIGNUP,  // ��û�б���
    TK_MATCH_LEAVE_TEAM_PLAYERIN = TK_MATCHUNSIGNUP_PLAYERIN,  // �û��Ѿ��ڱ�����

    TK_MATCH_LEAVE_TEAM_TEAMUNEXIST = TK_MATCHUNSIGNUP_TEAMUNEXIST  // ���鲻����
};

// ɾ������״̬
enum eMatchDeleteTeamState
{
    TK_MATCH_DELETE_TEAM_SUCCESS = 0,  // �뿪����ɹ�
    TK_MATCH_DELETE_TEAM_FAILED = 1,   // �뿪����ʧ��

    TK_MATCH_DELETE_TEAM_MATCHUNEXIST = TK_MATCHUNSIGNUP_MATCHUNEXIST,  // ����������

    TK_MATCH_DELETE_TEAM_PHASENOBEGIN = TK_MATCHUNSIGNUP_PHASENOBEGIN,  // ����û��ʼ
    TK_MATCH_DELETE_TEAM_PHASEEND = TK_MATCHUNSIGNUP_PHASEEND,          // ����ʱ�����

    TK_MATCH_DELETE_TEAM_NOSIGNUP = TK_MATCHUNSIGNUP_NOSIGNUP,  // ��û�б���
    TK_MATCH_DELETE_TEAM_PLAYERIN = TK_MATCHUNSIGNUP_PLAYERIN,  // �û��Ѿ��ڱ�����

    TK_MATCH_DELETE_TEAM_TEAMUNEXIST = TK_MATCHUNSIGNUP_TEAMUNEXIST,  // ���鲻����
    TK_MATCH_DELETE_TEAM_NO_PERMISSION                                // û��Ȩ��
};

// ������Ա״̬
enum eMatchRegisterUserToTeamState
{
    TK_MATCH_REGISTER_USER_TO_TEAM_SUCCESS = 0,                                   // ������Ա�ɹ�
    TK_MATCH_REGISTER_USER_TO_TEAM_FAILED = 1,                                    // ������Աʧ��
    TK_MATCH_REGISTER_USER_TO_TEAM_MATCHUNEXIST = TK_MATCHUNSIGNUP_MATCHUNEXIST,  // ����������
    TK_MATCH_REGISTER_USER_TO_TEAM_TEAMUNEXIST                                    // ���鲻����
};

// ȷ�϶������״̬
enum eMatchConfirmTeamState
{
    TK_MATCH_CONFIRM_TEAM_SUCCESS = 0,  //ȷ�϶�����ɳɹ�
    TK_MATCH_CONFIRM_TEAM_FAILED = 1,   //ȷ�϶������ʧ��

    TK_MATCH_CONFIRM_TEAM_MATCHUNEXIST = TK_MATCHUNSIGNUP_MATCHUNEXIST,  // ����������
    TK_MATCH_CONFIRM_TEAM_PHASENOBEGIN = TK_MATCHUNSIGNUP_PHASENOBEGIN,  // ����û��ʼ
    TK_MATCH_CONFIRM_TEAM_PHASEEND = TK_MATCHUNSIGNUP_PHASEEND,          // ����ʱ�����
    TK_MATCH_CONFIRM_TEAM_NOSIGNUP = TK_MATCHUNSIGNUP_NOSIGNUP,          // ��û�б���
    TK_MATCH_CONFIRM_TEAM_PLAYERIN = TK_MATCHUNSIGNUP_PLAYERIN,          // �û��Ѿ��ڱ�����

    TK_MATCH_CONFIRM_TEAM_TEAMUNEXIT,    //��ȷ�϶��鲻����
    TK_MATCH_CONFIRM_TEAM_NO_PERMISSION  //û��Ȩ��
};

// �浵״̬
enum eMatchArchiveState
{
    TK_MATCH_ARCHIVE_SUCCESS = 0,           // �浵�ɹ�
    TK_MATCH_ARCHIVE_FAILED = 1,            // �浵ʧ��
    TK_MATCH_ARCHIVE_FAILED_LOCKTABLE = 2,  // �浵ʧ�ܣ��������ܴ浵
    TK_MATCH_ARCHIVE_FAILED_FINAL = 3,      // �浵ʧ�ܣ����һ�ֲ��ܴ浵
};

enum eMatchCreateSelfBuildPlaceState
{
    TK_MATCH_CREATE_SELFBUILD_PLACE_SUCCESS = TK_MATCHSIGNUP_SUCCESS,
    TK_MATCH_CREATE_SELFBUILD_PLACE_FAILED = TK_MATCHSIGNUP_FAILED,
    TK_MATCH_CREATE_SELFBUILD_PLACE_MATCHUNEXIST = TK_MATCHUNSIGNUP_MATCHUNEXIST,
    TK_MATCH_CREATE_SELFBUILD_PLACE_DROPOUT_LIMIT = 1002,
    TK_MATCH_CREATE_SELFBUILD_PLACE_SERVICE_BUSY = 1003,
    TK_MATCH_CREATE_SELFBUILD_PLACE_FORBID = 1012,          // ��ֹ��ǰ�ͻ�����
    TK_MATCH_CREATE_SELFBUILD_PLACE_ADMIN_IP_LIMIT = 1013,  // ��ǰ�ͻ�Ϊ����Ա����IP��������Χ��
};

enum eMatchEnterSelfBuildPlaceState
{
    TK_MATCH_ENTER_SELFBUILD_PLACE_SUCCESS = TK_MATCHSIGNUP_SUCCESS,
    TK_MATCH_ENTER_SELFBUILD_PLACE_FAILED = TK_MATCHSIGNUP_FAILED,
    TK_MATCH_ENTER_SELFBUILD_PLACE_MATCHUNEXIST = TK_MATCHUNSIGNUP_MATCHUNEXIST,
    TK_MATCH_ENTER_SELFBUILD_PLACE_DROPOUT_LIMIT = 1002,
    TK_MATCH_ENTER_SELFBUILD_PLACE_SERVICE_BUSY = 1003,
    TK_MATCH_ENTER_SELFBUILD_PLACE_FULL = 1004,
    TK_MATCH_ENTER_SELFBUILD_PLACE_MATCHING = 1005,
    TK_MATCH_ENTER_SELFBUILD_PLACE_UNKNOWN_TYPE = 1006,
    TK_MATCH_ENTER_SELFBUILD_PLACE_ALREADY_IN = 1007,
    TK_MATCH_ENTER_SELFBUILD_PLACE_ID_INVALID = 1008,
    TK_MATCH_ENTER_SELFBUILD_PLACE_IN_ENTERING = 1009,
    TK_MATCH_ENTER_SELFBUILD_PLACE_NOT_IN_ENTERING = 1010,
    TK_MATCH_ENTER_SELFBUILD_PLACE_MULTI_STATUS = 1011,
    TK_MATCH_ENTER_SELFBUILD_PLACE_FORBID = 1012,          // ��ֹ��ǰ�ͻ�����
    TK_MATCH_ENTER_SELFBUILD_PLACE_ADMIN_IP_LIMIT = 1013,  // ��ǰ�ͻ�Ϊ����Ա����IP��������Χ��
};

enum eMatchLeaveRoomState
{
    TK_MATCH_LEAVE_ROOM_SUCCESS = 0,
    TK_MATCH_LEAVE_ROOM_FAILED = 1,
    TK_MATCH_LEAVE_ROOM_NOT_IN_SEAT = 2,  // ��Ҳ����κ���λ��
    TK_MATCH_LEAVE_ROOM_NOT_IN_ROOM = 3,  // ��Ҳ��ڷ�����
};

// �����������
enum eDicReliveCostType
{
    RELIVECOSTTYPE_NULL = 0,  // ����

    RELIVECOSTTYPE_MONEY = 1,  // Money����
    RELIVECOSTTYPE_WARE = 2,   //��Ʒ����

};

// �������ID
enum eDicReliveCostID
{
    RELIVECOSTID_NULL = 0,
    RELIVECOSTID_MONEY_GOLD = 2,  //=2, ��ң�Gold��
    RELIVECOSTID_MONEY_CERT = 4   //=4, �������֣�Cert��
};

// ������ü��㹫ʽ
enum eDicReliveCostFormula
{
    RELIVECOSTFORMULA_NULL = 0,  // ����

    RELIVECOSTFORMULA_LIST = 1,      // �б�ָ��
    RELIVECOSTFORMULA_BUYIN = 2,     // ����
    RELIVECOSTFORMULA_BASEMULT = 3,  // ��������
    RELIVECOSTFORMULA_POWER = 4,     // ����ָ��
    RELIVECOSTFORMULA_LINE = 5,      // ��������
    RELIVECOSTFORMULA_TABLE = 6,     // ���
};

// ��ʤ���ģʽ
enum eDicClearWinningStreakMode
{
    TK_CLEARWINNINGSTREAK_MODE_NULL = 0,      // ����
    TK_CLEARWINNINGSTREAK_MODE_LOSS = 1,      // ���ʱ��ֹ��ʤ��ϵͳĬ�ϣ�
    TK_CLEARWINNINGSTREAK_MODE_LOSSDRAW = 2,  // ƽ�ֻ����ʱ����ֹ��ʤ
};

// �������ģʽ
enum eDicClearLossingStreakMode
{
    TK_CLEARLOSSINGSTREAK_MODE_NULL = 0,    // ����
    TK_CLEARLOSSINGSTREAK_MODE_WIN = 1,     // Ӯ��ʱ��ֹ����
    TK_CLEARLOSSINGSTREAK_MODE_WINDRAW = 2  // ƽ�ֻ�Ӯ��ʱ����ֹ����
};

// ������Ϣ���Ͷ���(Ŀǰ����MC������ʾ���ݶ���)
enum eDicMatchInfoType
{
    TK_MATCHINFO_ENUM_NULL = 0,  // ����

    TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN = 100,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN = 200,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN = 300,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN = 400,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN = 500,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN = 600,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN = 700,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN = 800,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN = 900,  // ������Ϣ��ʵ����

    TK_MATCHINFO_ENUM_DAG_UNIT_DEFINEBEGIN = 1000,  // DAG������Ԫ��Ϣ��ʼ����
};

enum
{
    TK_MATCHINFO_ENUM_TOURNEY_NAME = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 0,                     // ��������( string )
    TK_MATCHINFO_ENUM_TOURNEY_PRODUCTID = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 1,                // ��ƷID( DWORD )
    TK_MATCHINFO_ENUM_TOURNEY_GAMEID = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 2,                   // ��ϷID( WORD )
    TK_MATCHINFO_ENUM_TOURNEY_TYPE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 3,                     // ��������( BYTE )
    TK_MATCHINFO_ENUM_TOURNEY_TEAM_TYPE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 4,                // ������Ԫ����( BYTE )
    TK_MATCHINFO_ENUM_TOURNEY_START_TYPE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 5,               // ������������(�μ�eTourneyStartMatchType����)( BYTE )
    TK_MATCHINFO_ENUM_TOURNEY_STATE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 6,                    // ״̬( BYTE )
    TK_MATCHINFO_ENUM_TOURNEY_TEAM_MEMBER_COUNT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 7,        // ��Ա��( BYTE )
    TK_MATCHINFO_ENUM_TOURNEY_TEAM_SUBSTITUTE_COUNT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 8,    // �油��( BYTE )
    TK_MATCHINFO_ENUM_TOURNEY_SIGN_MODE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 9,                // ������ʽ( BYTE )
    TK_MATCHINFO_ENUM_TOURNEY_SIGN_PLAYER_COUNT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 10,       // ��������( DWORD )
    TK_MATCHINFO_ENUM_TOURNEY_MATCHING_PLAYER_COUNT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 11,   // ��ս����( DWORD )
    TK_MATCHINFO_ENUM_TOURNEY_UNIT_COUNT_LIMIT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 12,        // ��������( ����DWORD )
    TK_MATCHINFO_ENUM_TOURNEY_AVERAGE_MATCH_INTERVAL = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 13,  // ƽ������ʱ��( DWORD�� )

    TK_MATCHINFO_ENUM_TOURNEY_CHAT_SVRIP = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 14,         // DWORD
    TK_MATCHINFO_ENUM_TOURNEY_CHAT_SVRPORT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 15,       // DWORD
    TK_MATCHINFO_ENUM_TOURNEY_CHAT_CHANNELTYPE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 16,   // DWORD
    TK_MATCHINFO_ENUM_TOURNEY_CHAT_CHANNELID = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 17,     // DWORD
    TK_MATCHINFO_ENUM_TOURNEY_CHAT_CHANNELSUBID = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 18,  // DWORD
    TK_MATCHINFO_ENUM_TOURNEY_CHAT_USERTYPE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 19,      // DWORD
    TK_MATCHINFO_ENUM_TOURNEY_DNY_FIELD_DEF = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 20,      // ��̬�ֶζ���(player_dny_field_def_attribute)

    TK_MATCHINFO_ENUM_TOURNEY_AVERAGE_PLAYER_COUNT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 21,  // ƽ����������( DWORD�� )

    TK_MATCHINFO_ENUM_TOURNEY_LADDER_MATCH = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 22,        // �Ƿ��������( byte )
    TK_MATCHINFO_ENUM_TOURNEY_CROWD_FUND_TYPE = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 23,     // 1��֧���ڳ��뱨����2����֧���ڳ�( byte )
    TK_MATCHINFO_ENUM_TOURNEY_USER_BUY_TOTAL_CNT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 24,  // �û������ܴ�����DWORD��
    TK_MATCHINFO_ENUM_TOURNEY_USER_CODE_CNT = TK_MATCHINFO_ENUM_TOURNEY_DEFINEBEGIN + 25,       // �û���֤��λ����DWORD��
    TK_MATCHINFO_ENUM_TOURNEY_DEFINE_END
};
enum sign_mode
{
    sign_mode_normal,          // ��ͨ
    sign_mode_commune,         // ���籨��
    sign_mode_random,          // ������
    sign_mode_club,            // ս�ӱ���
    sign_mode_mix_random,      // �����ӡ������ӻ�ϱ�����ʽ
    sign_mode_mix_club,        // ��ͨ��ӡ�ս�ӱ�����ϱ�����ʽ
    sign_mode_mix_club_random  // ��ͨ��ӡ�ս�ӱ����������ӻ�ϱ�����ʽ
};
enum tourney_state
{
    tourney_state_null,
    tourney_state_memprepare,
    tourney_state_preshow,
    tourney_state_signing,
    tourney_state_waittingmatch,
    tourney_state_matching,
    tourney_state_canceled,
    tourney_state_resultshow,
    tourney_state_waittingdelete
};

enum
{
    TK_MATCHINFO_ENUM_COMMITTEE_START_TIME = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 0,              // ��ʼʱ��( timet )
    TK_MATCHINFO_ENUM_COMMITTEE_STATE = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 1,                   // ״̬( BYTE )
    TK_MATCHINFO_ENUM_COMMITTEE_PLAYER_COUNT = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 2,            // �ѱ�������( DWORD )
    TK_MATCHINFO_ENUM_COMMITTEE_UNIT_COUNT = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 3,              // �ѱ���������Ԫ��( DWORD )
    TK_MATCHINFO_ENUM_COMMITTEE_EANBLE_PUSH_UNIT_INFO = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 4,   // �Ƿ���������unit info
    TK_MATCHINFO_ENUM_COMMITTEE_UNIT_LIST_MAX_COUNT = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 5,     // T���ɵ�Ԫ�б�ʱ������( int )
    TK_MATCHINFO_ENUM_COMMITTEE_EANBLE_RANDOM_TEAM = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 6,      // �Ƿ�����������(BYTE)
    TK_MATCHINFO_ENUM_COMMITTEE_PUSH_ALL_SIGNUP_PLAYER = TK_MATCHINFO_ENUM_COMMITTEE_DEFINEBEGIN + 7,  // ǿ�����ͱ�������б�(BYTE)

    TK_MATCHINFO_ENUM_COMMITTEE_DEFINE_END
};

enum committee_state
{
    committee_state_prepare,
    committee_state_signing,
    committee_state_matching,
    committee_state_cancel,
    committee_state_over,

    committee_state_end
};

enum
{
    TK_MATCHINFO_ENUM_MATCH_NAME = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 0,                 // ��������	( string )
    TK_MATCHINFO_ENUM_MATCH_PHASE_COUNT = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 1,          // �׶���		( BYTE )
    TK_MATCHINFO_ENUM_MATCH_BEGIN_TIME = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 2,           // ��ʼʱ��	( time_t )
    TK_MATCHINFO_ENUM_MATCH_UNIT_COUNT = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 3,           // �ܲ�����Ԫ��	( DWORD )
    TK_MATCHINFO_ENUM_MATCH_UNIT_TYPE = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 4,            // ������ / ��� / ������ / ��ʽ�� / ����������..( BYTE ).
    TK_MATCHINFO_ENUM_MATCH_TOURNEY_TYPE = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 5,         // ������ / S&G / ���� / room / table...( BYTE )
    TK_MATCHINFO_ENUM_MATCH_TABLE_NAME = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 6,           // ��table string��
    TK_MATCHINFO_ENUM_MATCH_DELAY_BROADCAST = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 7,      // ��ʱת����int �룩
    TK_MATCHINFO_ENUM_MATCH_JACKPOT = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 8,              // �����( DWORD )
    TK_MATCHINFO_ENUM_MATCH_STATE = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 9,                // ����״̬( BYTE )
    TK_MATCHINFO_ENUM_MATCH_POKER_TEAM_MODE = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 10,     // ����������ģʽ( BYTE )
    TK_MATCHINFO_ENUM_MATCH_TOURNEY_ID = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 11,          // tourney id( DWORD )
    TK_MATCHINFO_ENUM_MATCH_MS_IP = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 12,               // match ip( DWORD )
    TK_MATCHINFO_ENUM_MATCH_MS_PORT = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 13,             // match port( int )
    TK_MATCHINFO_ENUM_MATCH_RANDOAWARDSCHEMEID = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 14,  // ��������佱����ID(DWORD)
    TK_MATCHINFO_ENUM_MATCH_COMMITTEE_ID = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 15,        // committee id(DWORD)
    // add by liy at 2016.11.23
    TK_MATCHINFO_ENUM_MATCH_ESTIMATE_ORDER = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 16,  // �Ƿ�Ԥ��ȫ������( BYTE )
    TK_MATCHINFO_ENUM_MATCH_PLAYING_COUNT = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 17,   // ��ǰ��ս����( DWORD )
    //  [3/15/2018 dongly]
    TK_MATCHINFO_ENUM_MATCH_PLAY_TYPE = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 18,  // �淨����( BYTE )

    TK_MATCHINFO_ENUM_MATCH_QUALIFYING_FLAG = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 19,           // �Ƿ�Ϊ��λ���ı�ʶ( BYTE )
    TK_MATCHINFO_ENUM_MATCH_QUALIFYING_OPENTIME_FLAG = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 20,  // �Ƿ�����λ������ʱ��ı�ʶ( BYTE )
    TK_MATCHINFO_ENUM_MATCH_GROUP_RANK_RULE = TK_MATCHINFO_ENUM_MATCH_DEFINEBEGIN + 21,           // Ⱥ����������(string��json��ʽ)

    TK_MATCHINFO_ENUM_MATCH_DEFINE_END
};

enum eMatchPlayType
{
    MATCH_PLAY_TYPE_RESERVE = 0,         // ����
    MATCH_PLAY_TYPE_INTEGRALCASTLE = 1,  // ���ֳǱ���
    MATCH_PLAY_TYPE_CHALLENGE = 2,       // ��ս����������ʤ��ս��
    MATCH_PLAY_TYPE_CHAPTER = 3,         // �೵������
    MATCH_PLAY_TYPE_BIGAWARD = 4,        // �������
    MATCH_PLAY_TYPE_CHESSCHALLENGE = 5,  // ����ÿ����ս��

    MATCH_PLAY_TYPE_END
};

enum match_state
{
    match_state_prepare,
    match_state_matching,
    match_state_over,
    match_state_close,
    match_state_end
};

enum
{
    TK_MATCHINFO_ENUM_PHASE_INDEX = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 0,                  // �ڼ��׶�( BYTE )
    TK_MATCHINFO_ENUM_PHASE_TYPE = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 1,                   // pahse / group phase / parallel phase / join phase( BYTE )
    TK_MATCHINFO_ENUM_PHASE_RULE = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 2,                   // ����( BYTE )
    TK_MATCHINFO_ENUM_PHASE_NAME = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 3,                   // ���׶����ƣ��������������( string )
    TK_MATCHINFO_ENUM_PHASE_BEGIN_TIME = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 4,             // ���׶ο�ʼʱ��( time_t )
    TK_MATCHINFO_ENUM_PHASE_OVER_TIME = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 5,              // ���׶ν���ʱ��( time_t )
    TK_MATCHINFO_ENUM_PHASE_WINNER_COUNT = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 6,           // ���׶ν���������Ԫ��( DWORD )
    TK_MATCHINFO_ENUM_PHASE_STAGE_COUNT = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 7,            // ������( BYTE )
    TK_MATCHINFO_ENUM_PHASE_UNIT_COUNT = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 8,             // ���׶��ܲ�����Ԫ��( DWORD )
    TK_MATCHINFO_ENUM_NEXT_PHASE_SCORE_BASE_TABLE = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 9,  // �½׶λ�����( int list )
    TK_MATCHINFO_ENUM_PHASE_LIMIT_TIME = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 10,            // �׶���ʱ( time_t )
    TK_MATCHINFO_ENUM_PHASE_STATE = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 11,                 // ����״̬( BYTE )
    TK_MATCHINFO_ENUM_PHASE_FLAG = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 12,                  // ����״̬( WORD )��msg_generate_view::enum
    TK_MATCHINFO_ENUM_PHASE_NAME_ENU = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 13,              // Ӣ�ģ����׶����ƣ��������������( string )
    TK_MATCHINFO_ENUM_PHASE_SHOW_GROUP_NAME = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 14,       // �Ƿ���ʾ��������Ӧ���ڷ�������BYTE��
    TK_MATCHINFO_ENUM_PHASE_RELIVE_TIMEOUT = TK_MATCHINFO_ENUM_PHASE_DEFINEBEGIN + 15,        // �����ʱ����λ�루WORD��
    TK_MATCHINFO_ENUM_PHASE_DEFINE_END
};
enum phase_type
{
    SINGLE_PHASE = 1,    // ��ͨ����
    GROUP_PHASE,         // ��ͨ����
    PARALLEL_PHASE,      // ��������������
    JOIN_PHASE,          // ��������������
    VIRTUAL_GROUP_PHASE  // �������
};
enum phase_state
{
    phase_state_prepare,
    phase_state_matching,
    phase_state_over,

    phase_state_end
};

enum
{
    TK_MATCHINFO_ENUM_STAGE_INDEX = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 0,                       // �ڼ�����( BYTE )
    TK_MATCHINFO_ENUM_STAGE_RULE = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 1,                        // ����( BYTE )
    TK_MATCHINFO_ENUM_STAGE_BOUT_COUNT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 2,                  // ����( BYTE )
    TK_MATCHINFO_ENUM_STAGE_WINNER_COUNT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 3,                // ���������������Ԫ��( DWORD )
    TK_MATCHINFO_ENUM_STAGE_OVER_WHEN_LEFT_UNIT_COUNT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 4,   // ��ʣ���δ��������ʱ��׼���������������飬Ĭ�ϵ���nWinnerCount( DWORD )
    TK_MATCHINFO_ENUM_STAGE_UNIT_COUNT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 5,                  // �������ܲ�����Ԫ��( DWORD )
    TK_MATCHINFO_ENUM_STAGE_GAMECOUNT_AWARD_INFO = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 6,        // ����������Ϣ( string ���� )
    TK_MATCHINFO_ENUM_STAGE_TASK_INFO = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 7,                   // ������Ϣ( string ���� )
    TK_MATCHINFO_ENUM_STAGE_EXCHANGE_RATE = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 8,               // �һ���( int ���� )
    TK_MATCHINFO_ENUM_STAGE_AWARD_TIME = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 9,                  // �´ΰ佱����ʱ( time_t ���� )
    TK_MATCHINFO_ENUM_STAGE_PLAYERDYNFIELD = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 10,             //  room��̬�ֶζ���( string )
    TK_MATCHINFO_ENUM_STAGE_RELIVE_ENABLE = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 11,              //  �Ƿ�������( BYTE )
    TK_MATCHINFO_ENUM_STAGE_RELIVE_TIME_LIMIT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 12,          //  ����ʱ������( DWORD �� )
    TK_MATCHINFO_ENUM_STAGE_RELIVE_PLAYER_COUNT_LIMIT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 13,  //  ������������( DWORD )
    TK_MATCHINFO_ENUM_STAGE_RELIVE_COUNT_LIMIT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 14,         //  �����������( DWORD )
    TK_MATCHINFO_ENUM_STAGE_BASE_RAISE_SECOND = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 15,          //  ��������ʱ����( WORD �� )
    TK_MATCHINFO_ENUM_STAGE_AVERAGE_CHIP = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 16,               //  ����ƽ������( DWORD )
    TK_MATCHINFO_ENUM_STAGE_BASE_RAISE_TIME = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 17,            //  ��������ʱ��( time_t )
    TK_MATCHINFO_ENUM_STAGE_STATE = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 18,                      //  ����״̬( BYTE )
    TK_MATCHINFO_ENUM_STAGE_GAMESRV_IP = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 19,                 //  game server ip( DWORD )
    TK_MATCHINFO_ENUM_STAGE_GAMESRV_PORT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 20,               //  game server port( DWORD )
    TK_MATCHINFO_ENUM_STAGE_FLAG = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 21,                       // ����״̬( WORD )��msg_generate_view::enum
    TK_MATCHINFO_ENUM_STAGE_PLAYOFF_FLAG = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 22,               // ������ʶ��BYTE bit0-4 �Ƿ��м��� bit5 �Ƿ������ )
    TK_MATCHINFO_ENUM_STAGE_MIN_REDEMPTION_CHIP = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 23,        // ���ٻضҳ�����( DWORD )
    TK_MATCHINFO_ENUM_STAGE_SET_INHERIT_PERCENT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 24,        // �ּ���ְٷֱ�( BYTE )
    TK_MATCHINFO_ENUM_STAGE_MIN_CHIP = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 25,                   // ���ٳ�����( DWORD )
    TK_MATCHINFO_ENUM_STAGE_SORT_PLAYER = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 26,                // �������Ƿ�����( BYTE )
    TK_MATCHINFO_ENUM_STAGE_AVG_TIME = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 27,                   // ƽ��ʱ��( int )
    TK_MATCHINFO_ENUM_STAGE_COMPLETE_PECENT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 28,            // ��ɰٷֱ�( int )
    TK_MATCHINFO_ENUM_STAGE_PROMOTE_SCORE = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 29,              // Ԥ�ƽ�������( int )
    TK_MATCHINFO_ENUM_STAGE_RUNNING_ROUND = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 30,              // ���ڽ��е�round��( int )
    TK_MATCHINFO_ENUM_STAGE_COUNTDOWN_PECENT_MAX = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 31,       // ����ʱ�ٷֱ����ֵ
    TK_MATCHINFO_ENUM_STAGE_NEXT_BASE = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 32,                  // ��һ��������С( DWORD)
    TK_MATCHINFO_ENUM_STAGE_PEAK_AWARD = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 33,                 // �۷�Ծֽ���XML(string)
    TK_MATCHINFO_ENUM_STAGE_LADDERMATCH = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 34,                // ����೵��־(byte)
    TK_MATCHINFO_ENUM_STAGE_SUPERBANKER_INIT_CHIP = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 35,      // ����ׯ�ҵĳ�ʼ������( DWORD )
    TK_MATCHINFO_ENUM_STAGE_INDIV_PROMOTE_AWARD_PLAN = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 36,   // �೵�������ִν�����������( json string )
    TK_MATCHINFO_ENUM_STAGE_MAX_UNIT_LIST_COUNT = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 37,        // ������չʾ�����ȫ��������( int )
    TK_MATCHINFO_ENUM_STAGE_MIN_CHIP_64_STR = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 38,            // ���ٳ�����( 64λstring )
    TK_MATCHINFO_ENUM_STAGE_EXCHANGE_RATE64 = TK_MATCHINFO_ENUM_STAGE_DEFINEBEGIN + 39,            // 64λ�ض���XML( string )

    TK_MATCHINFO_ENUM_STAGE_DEFINE_END
};

enum stage_state
{
    stage_state_prepare,
    stage_state_matching,
    stage_state_over,

    stage_state_end
};

enum
{
    TK_MATCHINFO_ENUM_BOUT_INDEX = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 0,                       // �ڼ���			( BYTE )
    TK_MATCHINFO_ENUM_BOUT_TYPE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 1,                        // BOUT����		( BYTE )
    TK_MATCHINFO_ENUM_BOUT_BEGIN_TIME = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 2,                  // ��ʼʱ��		( time_t )
    TK_MATCHINFO_ENUM_BOUT_OVER_TIME = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 3,                   // ����ʱ��		( time_t )
    TK_MATCHINFO_ENUM_BOUT_WINNER_COUNT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 4,                // ����������Ԫ��	( DWORD )
    TK_MATCHINFO_ENUM_BOUT_OVER_WHEN_LEFT_UNIT_COUNT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 5,   // ��ʣ���δ��������ʱ��׼���������������飬Ĭ�ϵ���nWinnerCount( DWORD )
    TK_MATCHINFO_ENUM_BOUT_UNIT_COUNT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 6,                  // �����ܲ�����Ԫ��( DWORD )
    TK_MATCHINFO_ENUM_BOUT_PROMOTE_RULE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 7,                // ��������( string )
    TK_MATCHINFO_ENUM_BOUT_RELIVE_ENABLE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 8,               //  �Ƿ�������( BYTE )
    TK_MATCHINFO_ENUM_BOUT_RELIVE_TIME_LIMIT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 9,           //  ����ʱ������( DWORD �� )
    TK_MATCHINFO_ENUM_BOUT_RELIVE_PLAYER_COUNT_LIMIT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 10,  //  ������������( DWORD )
    TK_MATCHINFO_ENUM_BOUT_RELIVE_COUNT_LIMIT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 11,         //  �����������( DWORD )
    TK_MATCHINFO_ENUM_BOUT_BASE_RAISE_SECOND = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 12,          //  ��������ʱ����( WORD �� )
    TK_MATCHINFO_ENUM_BOUT_AVERAGE_CHIP = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 13,               //  ����ƽ������( DWORD )
    TK_MATCHINFO_ENUM_BOUT_BASE_RAISE_TIME = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 14,            //  ��������ʱ��( time_t )
    TK_MATCHINFO_ENUM_BOUT_STATE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 15,                      //  ����״̬( BYTE )
    TK_MATCHINFO_ENUM_BOUT_AVG_TIME = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 16,                   // ƽ��ʱ��( int )
    TK_MATCHINFO_ENUM_BOUT_COMPLETE_PECENT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 17,            // ��ɰٷֱ�( int )
    TK_MATCHINFO_ENUM_BOUT_PROMOTE_SCORE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 18,              // Ԥ�ƽ�������( int )
    TK_MATCHINFO_ENUM_BOUT_RUNNING_ROUND = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 19,              // ���ڽ��е�round��( int )
    TK_MATCHINFO_ENUM_BOUT_COUNTDOWN_PECENT_MAX = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 20,       // ����ʱ�ٷֱ����ֵ
    TK_MATCHINFO_ENUM_BOUT_ALIVE_UNIT_COUNT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 21,           // ָ���ͻ�����ʾ����������( DWORD )
    TK_MATCHINFO_ENUM_BOUT_RESULT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 22,                     // BoutResult XML(string)
    TK_MATCHINFO_ENUM_BOUT_PROMOTEAWARD = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 23,               // ��̭���ִν����ͽ���XML(string)
    TK_MATCHINFO_ENUM_BOUT_FINALAWARD = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 24,                 // �������ͽ���XML(string)
    TK_MATCHINFO_ENUM_BOUT_RUNNING_UNIT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 25,               // ���ڽ��е�unit��( int )
    TK_MATCHINFO_ENUM_BOUT_TEAM_GAME_TYPE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 26,             // �������������ͣ��ܷ���0���ڷ���1
    TK_MATCHINFO_ENUM_BOUT_RUNNING_TABLE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 27,              // ���ڽ��е�table��( int )
    TK_MATCHINFO_ENUM_BOUT_PROMOTE_RULE_ENU = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 28,           // Ӣ�ģ���������( string )
    TK_MATCHINFO_ENUM_BOUT_INDIV_PROMOTE_AWARD_PLAN = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 29,   // �೵�������ִν�����������( json string )
    TK_MATCHINFO_ENUM_BOUT_MAX_UNIT_LIST_COUNT = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 30,        // ����չʾ�����ȫ��������( int )
    TK_MATCHINFO_ENUM_BOUT_IS_ADDITIONAL = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 31,              // �Ƿ�����Ϊ������������ĸ�����,Ĭ��false( Byte )
    TK_MATCHINFO_ENUM_BOUT_IS_FINAL_OF_PHASE = TK_MATCHINFO_ENUM_BOUT_DEFINEBEGIN + 32,          // �Ƿ��ǵ�ǰ�׶����һ��,Ĭ��false( Byte )
    TK_MATCHINFO_ENUM_BOUT_DEFINE_END
};
enum bout_state
{
    bout_state_prepare,
    bout_state_matching,
    bout_state_over,
    bout_state_switch,  //�л���������
    bout_state_end
};

enum
{
    TK_MATCHINFO_ENUM_ROUND_SCORE_BASE = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 0,             // ��ǰ����( int )
    TK_MATCHINFO_ENUM_ROUND_GAME_COUNT = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 1,             // �ܸ���( BYTE )
    TK_MATCHINFO_ENUM_ROUND_HAND_OVER = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 2,              // ����ɸ���( BYTE )
    TK_MATCHINFO_ENUM_ROUND_RULER = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 3,                  // ���ƣ���������ʤ���� ����������߻�ʤ�������������֡�....��
    TK_MATCHINFO_ENUM_ROUND_TABLE_COUNT = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 4,            // ��������������( BYTE )
    TK_MATCHINFO_ENUM_ROUND_UNIT_TYPE = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 5,              // player / pair / team / pair_team( BYTE )
    TK_MATCHINFO_ENUM_ROUND_OUT_SCORE = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 6,              // �������ֵ����̭( int )
    TK_MATCHINFO_ENUM_ROUND_STANDARD_SCORE = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 7,         // ������( int )
    TK_MATCHINFO_ENUM_ROUND_SECTION_COUNT = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 8,          // ����( BYTE )
    TK_MATCHINFO_ENUM_ROUND_SECTION_INDEX = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 9,          // ��ǰ�ڼ���( BYTE )
    TK_MATCHINFO_ENUM_ROUND_NEXT_SCORE_BASE = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 10,       // ��һ�εĻ���( DWORD )
    TK_MATCHINFO_ENUM_ROUND_MATCHING_TABLE_COUNT = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 11,  // ���ڱ�������������������( BYTE )
    TK_MATCHINFO_ENUM_ROUND_SCORE = TK_MATCHINFO_ENUM_ROUND_DEFINEBEGIN + 12,                 // ����ȷ֣�7 : 5 : 1��( string )

    TK_MATCHINFO_ENUM_ROUND_DEFINE_END
};

enum
{
    TK_MATCHINFO_ENUM_TABLE_INDEX = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 0,           // ����( BYTE )
    TK_MATCHINFO_ENUM_TABLE_SCORE_BASE = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 1,      // ��ǰ����( int )
    TK_MATCHINFO_ENUM_TABLE_GAME_COUNT = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 2,      // �ܸ���( BYTE )
    TK_MATCHINFO_ENUM_TABLE_HAND_OVER = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 3,       // ����ɸ���( BYTE )
    TK_MATCHINFO_ENUM_TABLE_RULER = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 4,           // ���ƣ����̶��������� ��n��n/2+1ʤ���ơ������������֡�"���"....��
    TK_MATCHINFO_ENUM_TABLE_UNIT_TYPE = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 5,       // player / pair( BYTE )
    TK_MATCHINFO_ENUM_TABLE_OUT_SCORE = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 6,       // �������ֵ����̭( int )
    TK_MATCHINFO_ENUM_TABLE_STANDARD_SCORE = TK_MATCHINFO_ENUM_TABLE_DEFINEBEGIN + 7,  // ������( int )

    TK_MATCHINFO_ENUM_TABLE_DEFINE_END
};

enum eDicRoundType
{
    TK_ROUNDTYPE_TOTALGAME = 1,   // �̶�����
    TK_ROUNDTYPE_MINWINGAME = 2,  // n��n/2+1ʤ����
    TK_ROUNDTYPE_TOZEROCHIP = 3,  // ����������
    //#define TK_ROUNDTYPE_???		=4, //
    TK_ROUNDTYPE_GAMBLE = 5,   // ���浺
    TK_ROUNDTYPE_SWISS = 6,    // ��ʿ��λ���ֻ���
    TK_ROUNDTYPE_STANDARD = 7  // ���������������
};

enum
{
    TK_MATCHINFO_ENUM_UNIT_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 0,                   // �����ܻ���( int )
    TK_MATCHINFO_ENUM_UNIT_ORDER = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 1,                   // ����( int )
    TK_MATCHINFO_ENUM_UNIT_OUT_STATE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 2,               // ����״̬( WORD )
    TK_MATCHINFO_ENUM_UNIT_CV = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 3,                      // ��ʤ����( int )
    TK_MATCHINFO_ENUM_UNIT_GAME_COUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 4,              // ��ǰ�׶���������( int )
    TK_MATCHINFO_ENUM_UNIT_LIFE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 5,                    // ����ֵ( int )
    TK_MATCHINFO_ENUM_UNIT_TOTAL_GAME_COUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 6,        // ������( int )
    TK_MATCHINFO_ENUM_UNIT_DYNFIELD = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 7,                // ��̬�ֶ�( string )
    TK_MATCHINFO_ENUM_UNIT_ROUND_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 8,             // ����( int )
    TK_MATCHINFO_ENUM_UNIT_GS_IP = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 9,                   // gs ip( DWORD )
    TK_MATCHINFO_ENUM_UNIT_GS_PORT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 10,                // gs port( DWORD )
    TK_MATCHINFO_ENUM_UNIT_SECOND_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 11,           // С��( int )
    TK_MATCHINFO_ENUM_UNIT_THIRD_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 12,            // �ڶ�С�֣���������Ϸ�֣�( int )
    TK_MATCHINFO_ENUM_UNIT_DISSCUSSID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 13,             // ������( DWORD )
    TK_MATCHINFO_ENUM_UNIT_NUMBER = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 14,                 // ��ұ�ţ��ڶ����ڵ�λ��( byte )
    TK_MATCHINFO_ENUM_UNIT_CAPTAIN = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 15,                // �ӳ�( string )
    TK_MATCHINFO_ENUM_UNIT_LEADER = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 16,                 // ����( string )
    TK_MATCHINFO_ENUM_UNIT_TEAM_FLAG = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 17,              // �����ʶ( DWORD )
    TK_MATCHINFO_ENUM_UNIT_TEAM_RESTRICT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 18,          // �油����( team_restrict_attribute )
    TK_MATCHINFO_ENUM_UNIT_TEAM_LEADING_PLAYER = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 19,    // ������Ա��(BYTE)
    TK_MATCHINFO_ENUM_UNIT_OPPOSITE_CAPTAIN = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 20,       // ָ�����ֶӳ���
    TK_MATCHINFO_ENUM_UNIT_TEAM_SNSID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 21,             // sns( team_sns_attribute )
    TK_MATCHINFO_ENUM_UNIT_KILLCOUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 22,              // �����������˵�ɱ����( DWORD 3000��ʾɱ��0.3���� )
    TK_MATCHINFO_ENUM_UNIT_TABLE_INDEX = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 23,            // ��������( WORD Ϊ0��ʾ������Ϸ�� )
    TK_MATCHINFO_ENUM_UNIT_ROUND_SECOND_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 24,     // ����С��( int )
    TK_MATCHINFO_ENUM_UNIT_PAIR_LIMITS = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 25,            // ���������( player_pair_limits_attribute )
    TK_MATCHINFO_ENUM_PLAYER_DNY_FIELD = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 26,            // ��̬�ֶ�(player_dny_field_attribute)
    TK_MATCHINFO_ENUM_UNIT_INFO = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 27,                   // ��ϸ����( string )
    TK_MATCHINFO_ENUM_PLAYER_SECTION_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 28,        // �ڷ�( int )
    TK_MATCHINFO_ENUM_UNIT_TEAM_RESERVE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 29,           // ������λ( team_reserve_attribute )
    TK_MATCHINFO_ENUM_PLAYER_FLAG = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 30,                 // ��ұ�ʶ( byte )
    TK_MATCHINFO_ENUM_UNIT_MATCH_STATE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 31,            // ��ұ���״̬( DWORD )
    TK_MATCHINFO_ENUM_UNIT_CARRYBOUNTY = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 32,            //������������ Я���ͽ���( DWORD )
    TK_MATCHINFO_ENUM_UNIT_MATCH_PROCESS_AWARD = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 33,    //�������̽�����player_match_process_award_attribute��
    TK_MATCHINFO_ENUM_UNIT_ALIVE_PLAYER_COUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 34,     //����೵���������int��
    TK_MATCHINFO_ENUM_UNIT_DISABLE_WINSTREAK = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 35,      //�Ƿ������ʤ����BYTE��
    TK_MATCHINFO_ENUM_UNIT_REMAINDER_HEART_COUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 36,  //���������ʣ���ĵĿ�����int��
    TK_MATCHINFO_ENUM_UNIT_PLAYER_MATCH_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 37,     //��������ҵķ�����int��
    TK_MATCHINFO_ENUM_UNIT_BREAKHEART_TASK_STATE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 38,  //����������״̬��BYTE��
    TK_MATCHINFO_ENUM_UNIT_FIGURE_ID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 39,              // �û�ͷ��ID��int��
    TK_MATCHINFO_ENUM_UNIT_TEAM_TEMP_GROUP_ID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 40,     // ����/�Խ�����ʱ������ID��DWORD��
    TK_MATCHINFO_ENUM_UNIT_GOLD_COST_TOTAL = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 41,        // ��һ����ܼƣ�DWORD��
    TK_MATCHINFO_ENUM_UNIT_KILL_BOUNTY_TOTAL = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 42,      // �����ͽ��ܼƣ�DWORD��
    TK_MATCHINFO_ENUM_UNIT_TEAM_TEMP_SNSCLASS_ID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 43,  // �����顰��ϵ���������DWORD��
    TK_MATCHINFO_ENUM_UNIT_TEAM_TEMP_SNSTYPE_ID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 44,   // �����顰��ϵ�����ͣ�DWORD��
    TK_MATCHINFO_ENUM_UNIT_ORGANIZATION_ID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 45,        // ����ID��DWORD��
    TK_MATCHINFO_ENUM_UNIT_MATCH_ARCHIVE_FLAG = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 46,     // �浵��־(BYTE 0��ʾ��֧�ִ浵��1��ʾδԤԼ�浵��2��ʾ��ԤԼ�浵)
    TK_MATCHINFO_ENUM_UNIT_REALTIME_STANDBY_ID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 47,    // ʵʱ�Թ����췿��ID(DWORD)
    TK_MATCHINFO_ENUM_UNIT_HUNT_COUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 48,             // ��������ɱ����(DWORD)
    TK_MATCHINFO_ENUM_UNIT_ROUND_ORDER = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 49,            // ���ֽ���ʱ������(DWORD)
    TK_MATCHINFO_ENUM_UNIT_EXCHANGE_COIN = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 50,          // �������ضҽ����(DWORD)
    TK_MATCHINFO_ENUM_UNIT_CNT_TID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 51,                // �����ͻ�����������(WORD)
    TK_MATCHINFO_ENUM_UNIT_CLT_TID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 52,                // �����ͻ��˳�������(WORD)
    TK_MATCHINFO_ENUM_UNIT_SIGN_UP_TYPE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 53,           // ����������(DWORD)
    TK_MATCHINFO_ENUM_UNIT_GAMEID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 54,                 // ��ϷID(WORD)
    TK_MATCHINFO_ENUM_UNIT_PRODUCTID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 55,              // ��ƷID(DWORD)
    TK_MATCHINFO_ENUM_UNIT_GROWDOMAINID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 56,           // ������ID(WORD)
    TK_MATCHINFO_ENUM_UNIT_RANKSCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 57,              // �ط��齫��λ��(DWORD)
    TK_MATCHINFO_ENUM_UNIT_MATCHREMAINUNITCOUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 58,   // ������ڱ�����ʣ�������Ԫ����(int)
    TK_MATCHINFO_ENUM_UNIT_LANGVERSION = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 59,            // ���԰汾(int)
    TK_MATCHINFO_ENUM_UNIT_PHASE_INDEX = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 60,            // ������ڵĽ׶�(BYTE)
    TK_MATCHINFO_ENUM_UNIT_STAGE_INDEX = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 61,            // ������ڵķ���(BYTE)
    TK_MATCHINFO_ENUM_UNIT_BOUT_INDEX = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 62,             // ������ڵ���(BYTE)
    TK_MATCHINFO_ENUM_UNIT_RESUMEARCHIVE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 63,          // ����Ƿ��д浵��Byte��
    TK_MATCHINFO_ENUM_UNIT_ARCHIVEDATA = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 64,            // ��Ҵ浵���� (LadderRecord)
    TK_MATCHINFO_ENUM_UNIT_TAGDATA = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 65,                // ��ұ�ǩ���ݣ�DWORD��
    TK_MATCHINFO_ENUM_UNIT_MONEYACCTYPE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 66,           // ��ұ����˻����ͣ�DWORD��
    TK_MATCHINFO_ENUM_UNIT_OSTYPE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 67,                 // ��ҿͻ��˲���ϵͳ��BYTE��
    TK_MATCHINFO_ENUM_UNIT_APPID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 68,                  // ��ҿͻ��˰�װ��ID (DWORD)
    TK_MATCHINFO_ENUM_UNIT_SITEID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 69,                 // ������ʶ��DWORD��
    TK_MATCHINFO_ENUM_UNIT_FORM_ID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 70,                // ����ID��int��
    TK_MATCHINFO_ENUM_UNIT_PROCESS_RESULT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 71,         // ���ƥ����Ƚ����BYTE��
    TK_MATCHINFO_ENUM_UNIT_ENTER_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 72,            // ��Ҵ��֣�int��
    TK_MATCHINFO_ENUM_UNIT_SIGN_ORDER = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 73,             // ��ұ���˳��DWORD��
    TK_MATCHINFO_ENUM_UNIT_PACERBOT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 74,               // �Ƿ���AI
    TK_MATCHINFO_ENUM_UNIT_LOCALBOUT_GAMECOUNT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 75,    // ���ִξ���
    TK_MATCHINFO_ENUM_UNIT_PACERBOT_MSPORT = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 76,        // AI����ʱ��Ms Port
    TK_MATCHINFO_ENUM_UNIT_BOUT_BYE_SCORE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 77,         // �ֿմ���
    TK_MATCHINFO_ENUM_UNIT_GROUPNAME = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 78,              // ���ڷ���������ʾ�������������,��A\B\C\A1\B1\C1��string��
    TK_MATCHINFO_ENUM_UNIT_ADDITIONALLIST = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 79,         // ���鸽����Ϣ��string��
    TK_MATCHINFO_ENUM_UNIT_MATCH_STATUS = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 80,           // ��ұ���״̬����չ(BYTE������ֵ�ο�ePlayerMatchStatus)
    TK_MATCHINFO_ENUM_UNIT_PAYERID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 81,                // ������ID (DWORD)
    TK_MATCHINFO_ENUM_UNIT_CROWDFUNDID = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 82,            // �ڳ�ID(DWORD)
    TK_MATCHINFO_ENUM_UNIT_DOUBLEPLAYER = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 83,           // �����ӱ�ʶ��Ӧ����˫�˰೵���ƣ�BYTE��
    TK_MATCHINFO_ENUM_UNIT_MATCH_LEVEL = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 84,            // ��������(BYTE)
    TK_MATCHINFO_ENUM_UNIT_DEDUCTIONFEETYPE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 85,       // �۷�����(DWORD)
    TK_MATCHINFO_ENUM_UNIT_TOP_CV_CALC_RULE = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 86,       // �ﵽ�����ʤ����ʤ�����������( byte )
    TK_MATCHINFO_ENUM_UNIT_DOMAIN = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 87,                 // �û�ͳ����
    TK_MATCHINFO_ENUM_UNIT_SCORE_64 = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 88,               // �����ܻ���( int64 )
    TK_MATCHINFO_ENUM_UNIT_LIFE_64 = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 89,                // ����ֵ( int64 )
    TK_MATCHINFO_ENUM_UNIT_SCORE_64_STR = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 90,           // �����ܻ���( String )
    TK_MATCHINFO_ENUM_UNIT_LIFE_64_STR = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 91,            // ����ֵ( String )
    TK_MATCHINFO_ENUM_UNIT_PLAYER_UNIQUE_FLAG = TK_MATCHINFO_ENUM_UNIT_DEFINEBEGIN + 92,     // �����һ�������е�Ψһ��ʶ( DWORD )

    TK_MATCHINFO_ENUM_UNIT_DEFINE_END
};

enum
{
    TEAM_FLAG_NEED_CAPTION_CONFIRM = 1,     // ���������Ҫ�ӳ�ȷ��
    TEAM_FLAG_CAPTION_PAY_ALL_FEES = 2,     // �Ƿ�ӳ�����
    TEAM_FLAG_MUST_FRIEND = 4,              // ֻ�Ӻ���
    TEAM_FLAG_NO_SUBSTITUTE = 8,            // ��Ҫ�油
    TEAM_FLAG_COMPLETED = 16,               // �Ƿ�������(��������)
    TEAM_FLAG_HIDE_ARRANGEMENT = 32,        // �����Ƿ�ɼ�
    TEAM_FLAG_MATCH_BEGIN = 64,             //�����Ѿ���ʼ
    TEAM_FLAG_CREATE_TEAM_BY_SYSTEM = 128,  // ϵͳ�����Ķ���
    TEAM_FLAG_TEAM_CONFIRMED = 256,         // �����Ƿ���ȷ��
    TEAM_FLAG_END
};

enum
{
    PLAYER_FLAG_PENDING = 1,  // �ҵ����

    PLAYER_FLAG_END
};

enum
{
    TK_UNIT_MATCH_STATE_MATCHING = 0x01000000,    //���ڱ�����
    TK_UNIT_MATCH_STATE_MATCHOUT = 0x02000000,    //�Ѿ�����
    TK_UNIT_MATCH_STATE_PROMOTION = 0x04000000,   //����ȷ�Ͻ�����
    TK_UNIT_MATCH_STATE_WAITRESULT = 0x08000000,  //�������ˣ�����ȷ�����ֻ��ǽ���
    TK_UNIT_MATCH_STATE_NOTROUND = 0x10000000,    //�ֿ�

    TK_UNIT_MATCH_STATE_END
};

//Tourney�Ĳ�Ʒ���Ա��
enum eTourneyFlags
{
    TK_TOURNEY_FLAG_SIGN_PC = 0x0001,  //pc�˿��Ա���
    TK_TOURNEY_FLAG_SIGN_WC = 0x0002,  //flash�˿��Ա���
    TK_TOURNEY_FLAG_SIGN_MB = 0x0004,  //�ƶ��˿��Ա���

    TK_TOURNEY_FLAG_END
};

//�ͻ�����������
enum eLcStartMcType
{
    TK_LCSTARTMC_TYPE_BEGIN = 0,     // ��������
    TK_LCSTARTMC_TYPE_BREAK = 1,     // ��������
    TK_LCSTARTMC_TYPE_INVOKE = 2,    // ���ƶ�������
    TK_LCSTARTMC_TYPE_GSCHANGE = 3,  // GS��ת����
    TK_LCSTARTMC_TYPE_SPECTATOR = 4  // �Թ�����(Agent��װ������Ϣ�·����ƶ���ʱ)
};

//========================================================================================
// ������޹ص����ݽṹ
//========================================================================================

//----------------------------------------------------------------------------------------
// ͨ�ýṹ����

// �û���Ϣ
typedef struct tagLsUserInfo
{
    DWORD dwUserID;
    char szNickName[TK_BUFLEN_NICKNAME];
    WORD wFigureID;

    DWORD dwCoin;   // Ԫ��
    DWORD dwBonus;  // ��ȯ
    DWORD dwGold;   // ���
    DWORD dwCert;   // ����ȯ����Ʊ��

    DWORD dwScore;  // �ۼƻ���
    //Gaoy, Add
    DWORD dwMasterScore;  // ��ʦ��

} LSUSERINFO, *PLSUSERINFO;

// ����������Ϣ wuqy add 2012.3.20
typedef struct tagTkMatchBreakInfo
{
    DWORD dwUserID;

    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwCheck;

    time_t tBreakTime;

    WORD wGameID;
    WORD wReserve;
    DWORD dwSignUpPlanID;
    DWORD dwReserve2;
    DWORD dwReserve3;
    DWORD dwReserve4;
    int nGSPort;

    DWORD dwProductID;
    WORD wGrowDomainID;
    WORD wSignUpType;
    WORD wCntTID;  //�����ͻ����������� eLobbyConnetType
    WORD wCltTID;  //�����ͻ��˳�������

    DWORD dwMatchPoint;  // ������
    DWORD dwTeamID;
    DWORD dwGSNIPInt;

} TKMATCHBREAKINFO, *PTKMATCHBREAKINFO;

typedef struct tagTkGameAtomEvent
{
    DWORD dwEventID;
    DWORD dwEventValue;
} TKGAMEATOMEVENT, *PTKGAMEATOMEVENT;

//===========================================================================================
// ������йص����ݽṹ
//===========================================================================================
/////////////////////////////////////////////////////////////////////////////////////

// Tourney״̬
enum eTourneyState
{
    TOURNEYPROPAGANDIZING = 0,  // ������
    TOURNEYSIGNUPING = 1,       // ������
    TOURNEYSTARTUPING = 2,      // ������
    TOURNEYPROGRESS = 3,        // ������
    TOURNEYSTOP = 4,            // ����
    TOURNEYCANCEL = 5           // ȡ��
};

// Stage״̬
enum eStageState
{
    STAGESTARTUPING = 0,  // ������
    STAGEPROGRESS = 1,    // ������
    STAGESTOP = 2,        // ����
    STAGECANCEL = 3       // ȡ��
};

// Ms��Gs����Ϣ�첽ģʽ
enum eMsg2GsAsyncType
{
    TK_MSG2GS_ASTYPE_NULL = 0,  //
    TK_MSG2GS_ASTYPE_GS = 1,    // GSִ�з����첽
    TK_MSG2GS_ASTYPE_MS = 2,    // MSִ�з����첽
};

//Tourney����������
enum eTouryenMatchPointType
{
    TOURNEYMATCHPOINT_TYPE_RESERVER = 0,

    TOURNEYMATCHPOINT_TYPE_NULL = 1,    // һֱ���ڽ����еı�����û�п�����(���磺���죬�����������䣬Լս)
    TOURNEYMATCHPOINT_TYPE_SINGLE = 2,  // ��������ı���(���磺�೵��SNG����̬�೵)
    TOURNEYMATCHPOINT_TYPE_MULT = 3     // �࿪����ı���(���磺���ӽ�������ϵ����)
};

// Tourney������������
enum eTourneyStartMatchType
{
    TOURNEYSTARTMATCH_RESERVER = 0,  // ����

    TOURNEYSTARTMATCH_MULTTIME = 1,     // �ж�����㿪�����Tourney
    TOURNEYSTARTMATCH_MULTTABLE = 2,    // �ж������ͬʱ���е�Tourney
    TOURNEYSTARTMATCH_BATCHTIME = 3,    // �೵��ʼʱ�䶯̬�仯��Tourney
    TOURNEYSTARTMATCH_SITROOM = 4,      // ���ɷ����Tourney
    TOURNEYSTARTMATCH_ISLAND = 5,       // �������Tourney
    TOURNEYSTARTMATCH_SERIES = 6,       // ϵ�б���Tourney
    TOURNEYSTARTMATCH_DYNTIME = 7,      // ��̬����ʱ���Tourney
    TOURNEYSTARTMATCH_SNG = 8,          // Sit&Go����Tourney
    TOURNEYSTARTMATCH_FIXTITLE = 9,     // ����������
    TOURNEYSTARTMATCH_LBS = 10,         // ���������Tourney
    TOURNEYSTARTMATCH_LADDER = 11,      // ����೵����Tourney
    TOURNEYSTARTMATCH_FDTABLE = 12,     // ������/�Խ�������Tourney
    TOURNEYSTARTMATCH_CUSTOMIZED = 13,  // �Խ�����Tourney
    TOURNEYSTARTMATCH_FAMILY = 14,      // �������Tourney

    TOURNEYSTARTMATCH_RESERVE_END
};

// ������Ԫ����(������/������)
enum eTourneyTeamType
{
    TK_TOURNEY_TEAM_TYPE_RESERVER = 0,  // ����

    TK_TOURNEY_TEAM_TYPE_INDIVIDUAL = 1,       // ������
    TK_TOURNEY_TEAM_TYPE_PAIR = 2,             // ���
    TK_TOURNEY_TEAM_TYPE_TEAM = 3,             // �Ը���Ϊ��λ��������
    TK_TOURNEY_TEAM_TYPE_PAIR_TEAM = 4,        // �ԴΪ��λ��������
    TK_TOURNEY_TEAM_TYPE_DUPLEX = 5,           // ��ʽ��
    TK_TOURNEY_TEAM_TYPE_TEAM_CHAOS = 6,       // ������������
    TK_TOURNEY_TEAM_TYPE_PLAY_ONE_CARD = 7,    // ����һ����������
    TK_TOURNEY_TEAM_TYPE_SINGLE_AND_TEAM = 9,  // ���˺Ͷ���������Ӧ����˫�˰೵��
};

enum eTeamType
{
    TEAM_TYPE_SINGLE = TK_TOURNEY_TEAM_TYPE_INDIVIDUAL,                // ������
    TEAM_TYPE_PAIR = TK_TOURNEY_TEAM_TYPE_PAIR,                        // pair��
    TEAM_TYPE_TEAM = TK_TOURNEY_TEAM_TYPE_TEAM,                        // ���˶�ʽ��
    TEAM_TYPE_PAIR_TEAM = TK_TOURNEY_TEAM_TYPE_PAIR_TEAM,              // ��pairΪ��λ�Ķ�ʽ��
    TEAM_TYPE_DUPLEX = TK_TOURNEY_TEAM_TYPE_DUPLEX,                    // ��ʽ��
    TEAM_TYPE_TEAM_CHAOS = TK_TOURNEY_TEAM_TYPE_TEAM_CHAOS,            // ������������
    TEAM_TYPE_PLAY_ONE_CARD = TK_TOURNEY_TEAM_TYPE_PLAY_ONE_CARD,      // ����һ����������
    TEAM_TYPE_SINGLE_AND_TEAM = TK_TOURNEY_TEAM_TYPE_SINGLE_AND_TEAM,  //���˺Ͷ���������Ӧ����˫�˰೵��
};
//�����������
enum eMatchAwardMessageType
{
    MATCH_AWARD_MESSAGE_TYPE_RESERVER = 0,       // ����
    MATCH_AWARD_MESSAGE_TYPE_LUCKCARD = 1,       // ���˿�����
    MACTH_AWARD_MESSAGE_TYPE_JOYGOLD = 2,        // �콵ϲ��
    MATCH_AWARD_MESSAGE_TYPE_WARE = 3,           // �콵��Ʒ
    MATCH_AWARD_MESSAGE_TYPE_WINNINGSTREAK = 4,  // ��ʤ����
    MATCH_AWARD_MESSAGE_TYPE_TOTALWINNING = 5,   // ��ʤ����
    MATCH_AWARD_MESSAGE_TYPE_TOTALROUND = 6,     // ���̽���
    MATCH_AWARD_MESSAGE_TYPE_PK = 7,             // PK����
    MATCH_AWRAD_MESSAGE_TYPE_MOSAICGOLD = 8,     // �ʽ���
    MATCH_AWARD_MESSAGE_TYPE_PEAKTABLE = 9,      // �۷�Ծֽ���
    MATCH_AWARD_MESSAGE_TYPE_WHEELTABLE = 10,    // ���ֽ̾���
    MATCH_AWARD_MESSAGE_TYPE_BOUTPROMOTE = 11,   // ��̭���ִν����ͽ���
    MATCH_AWARD_MESSAGE_TYPE_FINALBOUT = 12,     // �������ͽ���
    MATCH_AWARD_MESSAGE_TYPE_BREAKHEART = 13,    // ����������
    MATCH_AWARD_MESSAGE_TYPE_BOMB = 14
};

// ����ͨ����ί�����͵��ͻ��˵Ľ�״��׺��������
enum eMatchAwardMessageBizType
{
    MATCH_AWARD_MESSAGE_BIZTYPE_RESERVE = 0,             // ����
    MATCH_AWARD_MESSAGE_BIZTYPE_AI = 1,                  // AI������
    MATCH_AWARD_MESSAGE_BIZTYPE_QUALIFYING = 2,          // ��λ��
    MATCH_AWARD_MESSAGE_BIZTYPE_PROMOTION = 3,           // ������
    MATCH_AWARD_MESSAGE_BIZTYPE_DOUBLEPLAYER = 4,        // ˫��
    MATCH_AWARD_MESSAGE_BIZTYPE_DOUBLECOMPENSATION = 5,  // ˫�˲�������
    MATCH_AWARD_MESSAGE_BIZTYPE_CHICKENMATCH = 6,        // �Լ�ģʽ
    MATCH_AWARD_MESSAGE_BIZTYPE_BOSS = 7,                // Boss������
    MATCH_AWARD_MESSAGE_BIZTYPE_RELIVE = 8,              // �������
};
// ��׺���ݸ�ʽ����(�˶���ΪBID���壬DATA��ʽ����ͻ������ж���)
//{
//	"BUSINESS_LIST": [{ "BID": "3","DATA": {...} }, { "BID": "4","DATA": {...} }, ...]
//}
// �˲���Json������ڰ佱XML�����У���Ӧxml��keyΪ<Scores>

// �����ͻ�����������
enum eLobbyConnetType
{
    LOBBYCONNETTYPE_RESERVE = 0,  // ����
    LOBBYCONNETTYPE_NORMAL = 1,   // ��ͨPC�ͻ���
    LOBBYCONNETTYPE_WEB = 2,      // ��ҳ�ͻ���
    LOBBYCONNETTYPE_MOBILE = 3    // �ֻ��ͻ���
};

// �����ͻ��˳�������
enum eLobbyClientType
{
    TK_LOBBYCLT_NULL = 0,              // ����
    TK_LOBBYCLT_VSLOBBYCLT = 10000,    // �羺(VS)�����ͻ���
    TK_LOBBYCLT_FISHLBYCLT = 10001,    // �������PC�����ͻ���
    TK_LOBBYCLT_FISHMOBCLT = 10002,    // ���������Ϸ�ƶ��ͻ���
    TK_LOBBYCLT_FGAMEMOBCLT = 10003,   // ������Ϸ�ƶ��ͻ���
    TK_LOBBYCLT_BOMBERLBYCLT = 10004,  // ը���˴����ͻ���
};

enum eDicGamePlayerOperator
{
    TK_GAMEPLAYEROPERATORTYPE_NULL = 0,     //
    TK_GAMEPLAYEROPERATORTYPE_PLAYER = 1,   //�û��Լ�����
    TK_GAMEPLAYEROPERATORTYPE_BOT = 2,      //�������Լ�����
    TK_GAMEPLAYEROPERATORTYPE_BOTTRUST = 3  //�û����ߺ��йܸ������˲���
};

enum eDynLeaveGameReason  //��̬�뿪��Ϸ��ԭ��
{
    TK_DYNLEAVEGAME_REASON_NULL = 0,          //
    TK_DYNLEAVEGAME_REASON_CHANGETABLE = 1,   // ����
    TK_DYNLEAVEGAME_REASON_STANDUP = 2,       // ����λ��վ��
    TK_DYNLEAVEGAME_REASON_GAME_KICKOUT = 3,  // ��Ϸ����ǰ��̭
};

enum eDicStageInheritScoreMode
{
    TK_INHERITSCORE_NULL = 0,              //	(Ĭ��)���̳��Ͻ׶ε�nLeaveScore�����Ǳ�Stage��ʼ��
    TK_INHERITSCORE_ENTERSCORESQRT = 1,    //	������һ��Stage���ִ���:sqrt(����)/Dev,MaxLimit
    TK_INHERITSCORE_ENTERORDER = 2,        //	������һ��Stage�����ʹ��ֱ�������(��������ʵ�ִ��ֱ�)
    TK_INHERITSCORE_ENTERSCOREAFFINE = 3,  //	������һ��stage���ֳ���һ��ϵ������:����/Dev,MaxLimit
    TK_INHERITSCORE_ATTENUATE = 4,         //	��һ��stage���ֳ�����һ��stage����scorebase�ٳ˱�stage��scorebase
    TK_INHERITSCORE_ZERO = 5,              //	��ɴ�0�ֿ�ʼ
    TK_INHERITSCORE_ENTERSCORE = 6,        //	�����Ͻ׶ε�nLeaveScore+Stage��ʼ��
    TK_INHERITSCORE_ATTENUATEEX = 7,  //	��ս����ս���ݣ���Stage��ʼ��+����һ��stage����/��һ��stage����scorebase + ϵ��dInherirSocerAdd��*��stage��scorebase*dInheritScoreMul

    //����:�̳��Ͻ׶ε�nLeaveScore�����ϱ�Stage��ʼ��
};

enum eDicMs2LsNotifyID  //MS��LS����֪ͨ
{
    TK_MS2LSNOTIFY_TEAM_CREATE = 0,                     // �������飬ѡ�����������
    TK_MS2LSNOTIFY_TEAM_ALTER_DELETED = 1,              // �޸Ķ��飬δȷ����ұ�ɾ��
    TK_MS2LSNOTIFY_TEAM_ALTER = 2,                      // �޸Ķ��飬֪ͨ���ж�Ա
    TK_MS2LSNOTIFY_TEAM_JOIN = 3,                       // ������飬֪ͨ���ж�Ա
    TK_MS2LSNOTIFY_TEAM_LEAVE = 4,                      // �뿪����, ֪ͨ���ж�Ա
    TK_MS2LSNOTIFY_TEAM_DISBAND = 5,                    // ��ɢ����, ֪ͨ���ж�Ա
    TK_MS2LSNOTIFY_TEAM_CONFIRM = 6,                    //ȷ�϶�����ɣ�֪ͨ���ж�Ա
    TK_MS2LSNOTIFY_PROCESS_RESULT = 7,                  //�����ս����֪ͨ
    TK_MS2LSNOTIFY_CROWFUND_UPDATE = 8,                 //�����ڳ���Ϣ
    TK_MS2LSNOTIFY_CUSTOMIZEDMATCH_SIGNUPRESULT = 100,  // �Խ������������֪ͨ
};

// ��Ϸ��ɫ
enum eDicGameActor
{
    // ������
    TK_GAMEACTOR_LORD_LANDLORD = 0,  // ����
    TK_GAMEACTOR_LORD_FARMER = 1,    // ũ��
    TK_GAMEACTOR_LORD_INVALID = 2,   // δ֪

    // �齫��������
    TK_GAMEACTOR_UPDGRADE_INVALID = 0,        // δ֪
    TK_GAMEACTOR_UPDGRADE_BANKER = 1,         // ׯ
    TK_GAMEACTOR_UPDGRADE_BANKERPARTNER = 2,  // ׯ�
    TK_GAMEACTOR_UPDGRADE_FREER = 3,          // �м�

    // �����˿ˡ�����˳
    TK_GAMEACTOR_POKER_INVALID = 0,     // δ֪
    TK_GAMEACTOR_POKER_BANKER = 1,      // ׯ
    TK_GAMEACTOR_POKER_BIGBLIND = 2,    // ��ä
    TK_GAMEACTOR_POKER_SMALLBLIND = 3,  // Сä

    // ��ţ������ƴʮ��
    TK_GAMEACTOR_DOUNIU_INVALID = 0,    // δ֪
    TK_GAMEACTOR_DOUNIU_BANKER = 1,     // ׯ
    TK_GAMEACTOR_DOUNIU_SUPBANKER = 2,  // ����ׯ
    TK_GAMEACTOR_DOUNIU_FREER = 3,      // �м�
};

// ������ɫ
enum eDicMatchActor
{
    TK_MATCHACTOR_INVALID = 0,        // δ֪
    TK_MATCHACTOR_COMPETITOR = 1,     // ����ѡ��
    TK_MATCHACTOR_ADMINISTRATOR = 2,  // ����Ա
};

// ��Ϸ��ɫ���𣬸�ʽTK_GAMEROLE_��ɫ_����_LEVEL_����
enum GameRoleLevel
{
    TK_GAMEROLE_AI_NORMALAI_LEVEL_0 = 0,    //��ͨAI����0
    TK_GAMEROLE_AI_NORMALAI_LEVEL_1 = 1,    //��ͨAI����1
    TK_GAMEROLE_AI_NORMALAI_LEVEL_2 = 2,    //��ͨAI����2
    TK_GAMEROLE_AI_NORMALAI_LEVEL_3 = 3,    //��ͨAI����3
    TK_GAMEROLE_AI_NORMALAI_LEVEL_4 = 4,    //��ͨAI����4
    TK_GAMEROLE_AI_NORMALAI_LEVEL_5 = 5,    //��ͨAI����5
    TK_GAMEROLE_AI_NORMALAI_LEVEL_6 = 6,    //��ͨAI����6
    TK_GAMEROLE_AI_NORMALAI_LEVEL_7 = 7,    //��ͨAI����7
    TK_GAMEROLE_AI_NORMALAI_LEVEL_8 = 8,    //��ͨAI����8
    TK_GAMEROLE_AI_NORMALAI_LEVEL_9 = 9,    //��ͨAI����9
    TK_GAMEROLE_AI_NORMALAI_LEVEL_10 = 10,  //��ͨAI����10

    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_1 = 91,    //����ѧϰAI����1
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_2 = 92,    //����ѧϰAI����2
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_3 = 93,    //����ѧϰAI����3
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_4 = 94,    //����ѧϰAI����4
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_5 = 95,    //����ѧϰAI����5
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_6 = 96,    //����ѧϰAI����6
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_7 = 97,    //����ѧϰAI����7
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_8 = 98,    //����ѧϰAI����8(AI��ʽ������)
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_9 = 99,    //����ѧϰAI����9
    TK_GAMEROLE_AI_MACHINELEARNAI_LEVEL_10 = 100,  //����ѧϰAI����9(�������о�����)

    TK_GAMEROLE_USER_NORMALUSER_LEVEL_BASE = 150,  //��ͨ��Ҽ������
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_1 = 151,     //��ͨ��Ҽ���1�����û����齫���ã�
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_2 = 152,     //��ͨ��Ҽ���2���쳣�û����齫���ã�
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_3 = 153,     //��ͨ��Ҽ���3�������û����齫���ã�
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_4 = 154,     //��ͨ��Ҽ���4
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_5 = 155,     //��ͨ��Ҽ���5
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_6 = 156,     //��ͨ��Ҽ���6
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_7 = 157,     //��ͨ��Ҽ���7
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_8 = 158,     //��ͨ��Ҽ���8
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_9 = 159,     //��ͨ��Ҽ���9
    TK_GAMEROLE_USER_NORMALUSER_LEVEL_10 = 160,    //��ͨ��Ҽ���10

};

//��ұ���״̬��ȡ��TKID_MS2GS_STAGEBOUTRESULTЭ��xml�е�status [12/15/2020 hehuan]
enum ePlayerMatchStatus
{
    player_match_status_unknow = 0,      //δ֪״̬
    player_match_status_matching = 1,    //"������"
    player_match_status_promotion = 2,   //"����"
    player_match_status_waitresult = 3,  //"�ȴ�"
    player_match_status_out = 4,         //"����"
    player_match_status_notround = 5,    //"�ֿ�"
    player_match_status_end = 6,         //"����"
    player_match_status_waitrelive = 7,  //"������"
    player_match_status_relived = 8,     //"�Ѹ���"
};

/////////////////////////////////////////////////////////////////////////////////////

typedef struct tagMatchIdentify
{
    DWORD dwMPID;
    DWORD dwTourneyID;
    DWORD dwMatchID;
    DWORD dwMatchPoint;
    WORD wGameID;
    WORD wPaddingReserved;
} MATCHIDENTIFY, *PMATCHIDENTIFY;

typedef struct tagMsTourneyPlayAmount
{
    DWORD dwTourneyID;        // Tourney ID
    DWORD dwSignUpAmount;     // ��������
    DWORD dwPlayingAmount;    // ��ǰ��ս����
    DWORD dwRunAmount;        // ��������
    DWORD dwSpectatorAmount;  // �Թ�����
    DWORD dwBetUserAmount;    // ���²�������

} MSTOURNEYPLAYAMOUNT, *PMSTOURNEYPLAYAMOUNT;

// Tourney ��Ϸ����(Tourney ״̬�С�������Ϣ������չ)
// �ˡ�������Ϣ��������Tourney�۲�㣬��ˢ��Ƶ����MatchService���ƣ�
// Add by ouyy 2008.03.06
typedef struct tagLsTourneyPlayAmountEx
{
    DWORD dwTourneyID;      // Tourney ID
    DWORD dwSignUpAmount;   // ��������
    DWORD dwPlayingAmount;  // ��ǰ��ս����
    DWORD dwRunAmount;      // ��������
    WORD wReserve1;         // ����1  �Թ�����
    WORD wReserve2;         // ����2

} LSTOURNEYPLAYAMOUNTEX, *PLSTOURNEYPLAYAMOUNTEX;

// Tourney �����û���Ϣ
typedef struct tagLsTourneySignUpUser
{
    DWORD dwUserID;                       // �û�ID
    DWORD dwMatchPoint;                   // ������
    char szNickName[TK_BUFLEN_NICKNAME];  // �ǳ�

} LSTOURNEYSIGNUPUSER, *PLSTOURNEYSIGNUPUSER;

// Tourney �����û���Ϣ
typedef struct tagMisTourneySignUpUser
{
    DWORD dwUserID;                       // �û�ID
    DWORD dwMatchPoint;                   // ������
    char szNickName[TK_BUFLEN_NICKNAME];  // �ǳ�
    BOOL bHide;                           // ��������б�����ʾ

} MISTOURNEYSIGNUPUSER, *PMISTOURNEYSIGNUPUSER;

// ���û�������Tourney��Ϣ
typedef struct tagLsUserInterfixTourney
{
    DWORD dwTourneyID;
    DWORD dwProductID;  // ��ƷID       wuqy add 2014.12.29
    WORD wGameID;       // GameID       wuqy add 2014.12.29
    WORD wFlags;        // ��Ʒ���Ա�ǣ��μ� TKMatchUniversalDicSrv.h �� eTourneyFlags����, wuqy add 2014.12.29
} LSUSERINTERFIXTOURNEY, *PLSUSERINTERFIXTOURNEY;

// ���û���������ע��Ϣ
typedef struct tagLsUserBetInfo
{
    DWORD dwBetID;
    DWORD dwUserID;

    DWORD dwTourneyID;
    DWORD dwMatchPoint;

    DWORD dwSeededPlayerID;  // ����ѡ��id
    DWORD dwBetMoney;        // ��ע���
    DWORD dwReserve;         // ����,��0

    DWORD dwBetResult;  // ���½��
} LSUSERBETINFO, *PLSUSERBETINFO;

// ���û�������TourneyPoint��Ϣ
typedef struct tagLsUserInterfixTourneyPoint
{
    DWORD dwTourneyID;
    DWORD dwMatchPoint;
    DWORD dwProductID;  // ��ƷID        wuqy add 2014.12.29
    WORD wGameID;       // GameID        wuqy add 2014.12.29
    WORD wFlags;        // ��Ʒ���Ա�ǣ��μ� TKMatchUniversalDicSrv.h �� eTourneyFlags����, wuqy add 2014.12.29
} LSUSERINTERFIXTOURNEYPT, *PLSUSERINTERFIXTOURNEYPT;

//�û������ı���
typedef struct tagLsUserSignupTourney
{
    DWORD dwTourneyID;       //
    time_t tMatchStartTime;  //������ʼʱ�䣬Ϊ0ʱ��ʾ�Ƕ����������SNG����ȣ� wuqy add 2015.3.3
    DWORD dwProductID;       // ��ƷID       wuqy add 2014.12.29
    WORD wGameID;            // GameID       wuqy add 2014.12.29
    WORD wFlags;             // ��Ʒ���Ա�ǣ��μ� TKMatchUniversalDicSrv.h �� eTourneyFlags����, wuqy add 2014.12.29
} LSUSERSIGNUPTOURNEY, *PLSUSERSIGNUPTOURNEY;

//�û������Ŀ�����
typedef struct tagLsUserSignupPoint
{
    DWORD dwTourneyID;   //
    DWORD dwMatchPoint;  //������
    DWORD dwProductID;   // ��ƷID       wuqy add 2014.12.29
    WORD wGameID;        // GameID       wuqy add 2014.12.29
    WORD wFlags;         // ��Ʒ���Ա�ǣ��μ� TKMatchUniversalDicSrv.h �� eTourneyFlags����, wuqy add 2014.12.29
} LSUSERSIGNUPPOINT, *PLSUSERSIGNUPPOINT;

//�û������ı���
typedef struct tagLcUserSignupTourney
{
    DWORD dwTourneyID;       //
    time_t tMatchStartTime;  //������ʼʱ�䣬Ϊ0ʱ��ʾ�Ƕ����������SNG����ȣ� wuqy add 2015.3.3
    DWORD dwProductID;       // ��ƷID       wuqy add 2014.12.29
    WORD wGameID;            // GameID       wuqy add 2014.12.29
    WORD wFlags;             // ��Ʒ���Ա�ǣ��μ� TKMatchUniversalDicSrv.h �� eTourneyFlags����, wuqy add 2014.12.29
} LCUSERSIGNUPTOURNEY, *PLCUSERSIGNUPTOURNEY;

//�û������Ŀ�����
typedef struct tagLcUserSignupPoint
{
    DWORD dwTourneyID;   //
    DWORD dwMatchPoint;  //������
    DWORD dwProductID;   // ��ƷID       wuqy add 2014.12.29
    WORD wGameID;        // GameID       wuqy add 2014.12.29
    WORD wFlags;         // ��Ʒ���Ա�ǣ��μ� TKMatchUniversalDicSrv.h �� eTourneyFlags����, wuqy add 2014.12.29
} LCUSERSIGNUPPOINT, *PLCUSERSIGNUPPOINT;

// Stageѡ����Ϣ
typedef struct tagLsStagePlayer
{
    DWORD dwUserID;                       // UID
    char szNickName[TK_BUFLEN_NICKNAME];  // �ǳ�
    WORD wOrder;                          // ����
    WORD wOutRoundState;                  // ����״̬�� �μ� eOutRoundState
    DWORD dwMatchScore;                   // ��������(��ǰ�����Ļ���)

} LSSTAGEPLAYER, *PLSSTAGEPLAYER;

//====================================================================================================
// Round������Ϣ
typedef struct tagLsRoundInfo
{
    DWORD dwTourneyID;  // ����ID
    DWORD dwMatchID;    // ����ID
    WORD wStageID;      // StageID
    WORD wRoundID;      // RoundID
    WORD wPlayerCount;  // �������

} LSROUNDINFO, *PLSROUNDINFO;

// Roundѡ����Ϣ
typedef struct tagLsRoundPlayer
{
    DWORD dwUserID;                       // �û�ID
    char szNickName[TK_BUFLEN_NICKNAME];  // ����ǳ�
    DWORD dwMatchScore;                   // ��������(��ǰ�����Ļ���)

} LSROUNDPLAYER, *PLSROUNDPLAYER;

// Tourne����Player��Ϣ
typedef struct tagLsTourneyMatchTablePlayer
{
    DWORD dwMatchTableNo;                 // �������
    DWORD dwUserID;                       // �û�ID
    char szNickName[TK_BUFLEN_NICKNAME];  // ����ǳ�
    DWORD dwScore;                        // ����
} LSTOURNEYMATCHTABLEPLAYER, *PLSTOURNEYMATCHTABLEPLAYER;

//�µ�Tourney���ݽṹ wuqy add 2009.6.23
typedef struct tagTkTnyData
{
    DWORD dwTourneyID;  //TourneyID
    DWORD dwProductID;  //ProductID

    time_t tMatchStartTime;  // ������ʼʱ��,һ�����ڱ�����ʼʱ��
    WORD wTourneyState;      // Tourney ״̬
    WORD wGameID;            // GameID,��Ϸ���
    BYTE byStartMatchType;   // ������������(�μ�eTourneyStartMatchType����)
    BYTE byMatchPointType;   // ����������(�μ�eTouryenMatchPointType����)
    BYTE byBoutNo;           // ���α�ţ�һ�����������ڼ����ı�ţ�
    BYTE byTeamType;         // ������Ԫ����(�μ�eTourneyTeamType����)

    DWORD dwMisIP;  // ����������IP(=inet_addr("LocalPublicIP"))
    int nMisPort;   // ����������Port(=20007)
    DWORD dwRisIP;  // ���������IP(=inet_addr("LocalPublicIP"))
    int nRisPort;   // ���������Port(=20012)

} TKTNYDATA, *PTKTNYDATA;

//�µ�Tourney���ݽṹ
typedef struct tagTkTisTnyData
{
    DWORD dwTourneyID;  //TourneyID
    DWORD dwProductID;  //ProductID

    time_t tMatchStartTime;  // ������ʼʱ��,һ�����ڱ�����ʼʱ��
    WORD wTourneyState;      // Tourney ״̬
    WORD wGameID;            // GameID,��Ϸ���
    BYTE byStartMatchType;   // ������������(�μ�eTourneyStartMatchType����)
    BYTE byMatchPointType;   // ����������(�μ�eTouryenMatchPointType����)
    BYTE byBoutNo;           // ���α�ţ�һ�����������ڼ����ı�ţ�
    BYTE byTeamType;         // ������Ԫ����(�μ�eTourneyTeamType����)

    DWORD dwMisIP;  // ����������IP(=inet_addr("LocalPublicIP"))
    int nMisPort;   // ����������Port(=20007)
    DWORD dwRisIP;  // ���������IP(=inet_addr("LocalPublicIP"))
    int nRisPort;   // ���������Port(=20012)

    int nMsPort;  // ����������Port

} TKTISTNYDATA, *PTKTISTNYDATA;

//yangcheng add begin 2011-4-29 14:51:28 --------------------------------------------
//��ʽ��������Ϣ
#define TK_TEAM_FLAG_CLUB 1     // �Ƿ�ս�ӱ���
#define TK_TEAM_FLAG_RANDOM 2   // �Ƿ�������
#define TK_TEAM_FLAG_ALLCLUB 4  // �Ƿ�ֻ����ս�Ӳ���
typedef struct tagTnyTeamInfo
{
    WORD wTeamType;
    WORD wMemberPerTeam;      // ��Ա��
    WORD wSubstitutePerTeam;  // �油��
    BOOL bPushTeamInfo;       // �Ƿ�ʵʱ���Ͷ�Ա��Ա�仯��Ϣ
    DWORD dwTeamFlag;
    tagTnyTeamInfo()
    {
        memset(this, 0, sizeof(tagTnyTeamInfo));
    }
} TKTNYTEAMINFO, *PTKTNYTEAMINFO;
//yangcheng add end   2011-4-29 14:51:28 --------------------------------------------

struct TkTeamMember
{
    DWORD dwSeat;
    DWORD dwUserID;
    char szName[TK_BUFLEN_USERNAME];  // �س�
};
struct TkTeamReserve
{
    DWORD dwSeat;                     // λ��
    char szName[TK_BUFLEN_USERNAME];  // �س�
};
struct TkTeamOptionBase
{
    char szTeamName[TK_BUFLEN_USERNAME];     // ����
    char szCaptainName[TK_BUFLEN_USERNAME];  // �ӳ�
    BOOL bMustFriend;                        // ֻ����������
    BOOL bNoSubstitute;                      // ��Ҫ�油
    DWORD dwRestrictGrowID;                  // �油����
    int nRestrictMin;                        // ��Сֵ
    int nRestrictMax;                        // ���ֵ
    DWORD dwReservesOffset;                  // Ԥ��λ����Ϣƫ��(����ڱ��ṹͷ��)
    DWORD cnReserves;                        // Ԥ��λ�ø���
};
struct TkTeamOption : public TkTeamOptionBase
{
    char szOppositeCaptainName[TK_BUFLEN_USERNAME];  // �Է��ӳ���
    BOOL bHideArrangement;                           // ���������Ƿ�ɼ�
    DWORD dwSnsDisscussID;                           // ������ID  //yc add 2012-2-9
    DWORD dwOptionFlags;                             // ѡ����()
    SNSIDX stSnsID;                                  // ��ϵID
    DWORD dwRestrictGrowID2;                         // ��Ա����2	//zl add 2012-05-25
    int nRestrictMin2;                               // ������Сֵ2
    DWORD dwTeamID;                                  // TeamID
    DWORD dwReserve1;                                // Ԥ���ֶ�1
    DWORD dwReserve2;                                // Ԥ���ֶ�2
                                                     // ���cnReserves��TkTeamReserve
};

//TkTeamOption::dwOptionFlags��λ��Ƕ���
#define TK_TEAMOPTION_FLAGS_NEEDCAPTIONCONFIRM 0x00000001    // ���������Ҫ�ӳ�ȷ��
#define TK_TEAMOPTION_FLAGS_CAPTION_PAY_ALL_FEES 0x00000002  // �Ƿ�ӳ�����

typedef struct tagTkLsTnyData
{
    DWORD dwTourneyID;      // TourneyID
    BYTE byStartMatchType;  // ������������(�μ�eTourneyStartMatchType����)
    BYTE byMatchPointType;  // ����������(�μ�eTouryenMatchPointType����)
    WORD wMsPort;           // ��������˿�
    BYTE byTeamType;        // ������Ԫ����(�μ�eTourneyTeamType����)
    BYTE byReserve;         //
    WORD wDefaultSignFeeType;
} TKLSTNYDATA, *PTKLSTNYDATA;

//LobbySign��Tourney���ݽṹ wuqy add 2013.7.8
typedef struct tagTkSignSTnyData
{
    DWORD dwTourneyID;  //TourneyID
    DWORD dwProductID;  //ProductID

    time_t tMatchStartTime;  // ������ʼʱ��,һ�����ڱ�����ʼʱ��
    WORD wTourneyState;      // Tourney ״̬
    WORD wGameID;            // GameID,��Ϸ���
    BYTE byStartMatchType;   // ������������(�μ�eTourneyStartMatchType����)
    BYTE byMatchPointType;   // ����������(�μ�eTouryenMatchPointType����)
    BYTE byBoutNo;           // ���α�ţ�һ�����������ڼ����ı�ţ�
    BYTE byTeamType;         // ������Ԫ����(�μ�eTourneyTeamType����)
    WORD wMsPort;            // ��������˿�
    WORD wDefaultSignFeeType;
    WORD wFlags;            // ��Ʒ���Ա�ǣ��μ� TKMatchUniversalDicSrv.h �� eTourneyFlags����, wuqy add 2014.12.29
    DWORD dwMaxSignupTeam;  // ��������λ����������ֹͣ������

} TKSIGNSTNYDATA, *PTKSIGNSTNYDATA;

typedef struct tagTkSnsTnyData
{
    DWORD dwTourneyID;                              //TourneyID
    DWORD dwProductID;                              //ProductID
    char szTourneyName[TK_BUFLEN_TOURNEYFULLNAME];  // ��������

    time_t tMatchStartTime;  // ������ʼʱ��,һ�����ڱ�����ʼʱ��
    WORD wTourneyState;      // Tourney ״̬
    WORD wGameID;            // GameID,��Ϸ���
    BYTE byStartMatchType;   // ������������(�μ�eTourneyStartMatchType����)
    BYTE byMatchPointType;   // ����������(�μ�eTouryenMatchPointType����)
    BYTE byBoutNo;           // ���α�ţ�һ�����������ڼ����ı�ţ�
    BYTE byTeamType;         // ������Ԫ����(�μ�eTourneyTeamType����)

    DWORD dwPlayAmount;
    DWORD dwMaxSignupTeam;  // ��������λ����������ֹͣ������
    DWORD dwMinSignupTeam;  // ��С������λ����������ȡ��������

    char szProductURL[TK_BUFLEN_CALLWEBHTTPLINK];

} TKSNSTNYDATA, *PTKSNSTNYDATA;

//�µ�Tourney״̬�ṹ
typedef struct tagTkTnyState
{
    DWORD dwTourneyID;   // Tourney ID
    WORD wTourneyState;  // Tourney ״̬
    WORD wFlags;         // ��ʶ��1��ʾ���̽�״̬�㲥���ͻ���

} TKTNYSTATE, *PTKTNYSTATE;

typedef struct tagTkSnsTnyState
{
    DWORD dwTourneyID;  // Tourney ID
    DWORD dwPlayAmount;
    WORD wTourneyState;  // Tourney ״̬
    WORD wReserver;      //����

} TKSNSTNYSTATE, *PTKSNSTNYSTATE;

//�µ�Stage״̬�ṹ
typedef struct tagTkStageState
{
    DWORD dwTourneyID;  // TourneyID
    DWORD dwMatchID;    // MatchID
    WORD wStageID;      // StageID

    WORD wStageState;   // Stage״̬�������У������У�������ȡ����
                        // �μ��� ö�ٱ��� eStageState��������/������/����/ȡ����
    time_t tStartTime;  // ���ֿ�ʼʱ�䣨����Stageһ����ǰһ��Stage���������ȷ����������Ҫ��������ݷ��������� wuqy add 2007.11.10
} TKSTAGESTATE, *PTKSTAGESTATE;

//�µ�Stage���ݽṹ
typedef struct tagTkStageData
{
    WORD wType;                     // ���ƣ�	��̭����ս����սĬ��Ϊ�����������
    DWORD dwWinnerCount;            // ����Ҫѡ�γ������������
    TKADDRINFO stGameSrvAddr;       // ����Stage����Ϸ��������ַ
    TKADDRINFO stBroadcaseSrvAddr;  // ��Ϸ��ת����������ַ

} TKSTAGEDATA, *PTKSTAGEDATA;

// �µ�Tourne��������Ϣ
typedef struct tagTkTnyMatchPoint
{
    DWORD dwMatchPoint;     // ������
    DWORD dwMatchID;        // MatchID
    DWORD dwSignUpAmount;   // ��������
    DWORD dwPlayingAmount;  // ��ǰ��ս����
    DWORD dwRunAmount;      // ��������
    WORD wState;            // ������״̬
    WORD wParam;            // ���������
} TKTNYMATCHPOINT, *PTKTNYMATCHPOINT;

//�µ� Tourne������Ϣ
typedef struct tagTkTnyMatchTable
{
    DWORD dwMatchTableNo;  // �������
    DWORD dwMatchID;       // MatchID
    WORD wSignUpAmount;    // ��������
    WORD wSitDownAmount;   // ��������
    WORD wMaxAmount;       // ����������
    WORD wReserveB;
    WORD wState;                                  // ����״̬
    WORD wFlags;                                  // ��ǣ�����������TABLE_FLAG_STARTABLE
    char szTableName[TK_BUFLEN_MATCHTABLE_NAME];  //��������
    char szTableNote[TK_BUFLEN_MATCHTABLE_NOTE];  //����˵��
    DWORD dwJoinGold;                             // �볡�����
} TKTNYMATCHTABLE, *PTKTNYMATCHTABLE;

typedef struct tagMSMatchSignupData
{
    DWORD dwTourneyID;  // TourneyID

    char szSignupNoteCondition[TK_BUFLEN_MATCH_SIGNUPNOTE];
    WORD dwGrowConditionCount;  //�ṹ���׺dwGrowConditionCount��MSMATCHGROWSIGNUPCONDITIONITEM����
    WORD dwWareConditionCount;  //�ṹ���׺dwWareConditionCount��MSMATCHWARESIGNUPCONDITIONITEM����

    int cnSignupItem;  // ��׺cnSignupItem��MSMATCHSIGNUPITEM����
    int nSignupDataLength;
} MSMATCHSIGNUPDATA, *PMSMATCHSIGNUPDATA;
// ��������
typedef struct tagMSMatchGrowSignUpConditionItem
{
    DWORD dwGrowID;
    int nMax;
    int nMin;
} MSMATCHGROWSIGNUPCONDITIONITEM, *PMSMATCHGROWSIGNUPCONDITIONITEM;
// ��Ʒ����
typedef struct tagMSMatchWareSignUpCondition
{
    DWORD dwWareTypeID;
    int nMax;
    int nMin;

} MSMATCHWARESIGNUPCONDITIONITEM, *PMSMATCHWARESIGNUPCONDITIONITEM;
typedef struct tagMSMatchSignupItem
{
    int nSignupType;  // ����������
    char szSignupNote[TK_BUFLEN_MATCH_SIGNUPNOTE];
    DWORD dwCoin;
    DWORD dwBonus;
    DWORD dwGold;
    DWORD dwCert;

    WORD dwGrowGainCount;  //�ṹ���׺dwGrowGainCount��MSMATCHSIGNUPGROWITEM����
    WORD dwWareGainCount;  //�ṹ���׺dwWareGainCount��MSMATCHSIGNUPWAREITEM����
} MSMATCHSIGNUPITEM, *PMSMATCHSIGNUPITEM;
// ����
typedef struct tagMSMatchSignUpGrowItem
{
    DWORD dwGrowID;
    int nGrowCount;  // ��Ҫ�۳��Ļ��ָ���

} MSMATCHSIGNUPGROWITEM, *PMSMATCHSIGNUPGROWITEM;
// ��Ʒ
typedef struct tagMSMatchSignUpWareItem
{
    DWORD dwWareTypeID;
    int nWareCount;  // ��Ҫ�۳�����Ʒ����

} MSMATCHSIGNUPWAREITEM, *PMSMATCHSIGNUPWAREITEM;

//ChatData���ݽṹ��
typedef struct tagMSMatchChatData
{
    DWORD dwTourneyID;                          // TourneyID
    DWORD dwChannelType;                        // ����ͨ��������
    DWORD dwChannelId;                          // ����ͨ����Id
    char szChannelName[TK_BUFLEN_CHANNELNAME];  // ����ͨ��������
    char szChatIP[TK_BUFLEN_IP];                //�����ҵ�IP
    DWORD dwPort;                               //�˿�
} MSMATCHCHATDATA, *PMSMATCHCHATDATA;

typedef struct tagTKMsMPPos
{
    int nProductID;
    int nGameID;

    DWORD dwTourneyID;
    DWORD dwMatchPoint;
    DWORD dwMatchID;

    time_t timeStart;  //��ʼʱ��

    DWORD dwIPMS;  //Public IP
    int nPortMS;   //

    DWORD dwIPGS;  //Public IP
    int nPortGS;   //

    DWORD dwFlags;
} TKMSMATCHPOS, *PTKMSMATCHPOS;

//====================================================================================================
//���������ϸ������ݽṹ��������GameService��װ��ͨ��MS͸����DS��wuqy add 2007.11.10��
//�����������ϸ�ṹͷ����
typedef struct tagTKGAMETEAMRESULTDHEADER
{
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    WORD wGameNo;  // �ڼ�����Ϸ
} TKGAMETEAMRESULTDHEADER, *PTKGAMETEAMRESULTDHEADER;
//������ÿ��Team�÷�
typedef struct tagTKGAMETEAMRESULTDETAIL
{
    DWORD dwTeamID;
    int nScore;
} TKGAMETEAMRESULTDETAIL, *PTKGAMETEAMRESULTDETAIL;

//�����������ϸ�ṹͷ����
typedef struct tagTKGAMEPLAYERRESULTDHEADER
{
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    WORD wGameNo;
} TKGAMEPLAYERRESULTDHEADER, *PTKGAMEPLAYERRESULTDHEADER;
//������ÿ��Team�÷�
typedef struct tagTKGAMEPLAYERRESULTDETAIL
{
    WORD wSeatID;
    DWORD dwUserID;
    int nScore;
} TKGAMEPLAYERRESULTDETAIL, *PTKGAMEPLAYERRESULTDETAIL;

//���ֽ����ϸ�Ʒ�
typedef struct tagTKGAMERESULTSPECIAL
{
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    WORD wGameNo;
    WORD wGameID;  //��Ϸ��GameID

    char szScoreDetail[TK_MAX_GAMERESULTSCOREDETAIL];  //ϸ�ڼƷִ�,������Ϸ���Խ���
} TKGAMERESULTSPECIAL, *PTKGAMERESULTSPECIAL;

//�����������׽ṹͷ����
typedef struct tagTKGAMEDETAILHISTORYHEADER
{
    DWORD dwTourneyID;
    DWORD dwMatchID;
    WORD wStageID;
    WORD wRoundID;

    WORD wGameNo;
    WORD wGameID;
} TKGAMEDETAILHISTORYHEADER, *PTKGAMEDETAILHISTORYHEADER;

//�û���ID��NickNameͨ�ýṹ����(�벻Ҫ��չ�͸��� wuqy 2009.11.12)
typedef struct tagTKMATCHUSERIDNICK
{
    DWORD dwUserID;                       // �û�ID
    char szNickName[TK_BUFLEN_NICKNAME];  // ����ǳ�
} TKMATCHUSERIDNICK, *PTKMATCHUSERIDNICK;

typedef struct tagTKMATCHACTION
{
    int nID;     // ActionID
    int nDelay;  // �ӳ�ʱ��(��)
} TKMATCHACTION, *PTKMATCHACTION;

typedef struct tagTKTNYGAMEVERCONFIG
{
    DWORD dwMinMatchClientVersion;  // ��ͱ����ͻ��˰汾��
    DWORD dwMinMatchClientVersionIni;

    DWORD dwMinLobbyVersion;  // ��ʹ����汾��
    DWORD dwMinLobbyVersionIni;

    DWORD dwMinGameDllVersion;  // �����Ϸ��̬��汾��
    DWORD dwMinGameDllVersionIni;
} TKTNYGAMEVERCONFIG, *PTKTNYGAMEVERCONFIG;

//�û���Ϣ��̬�ֶζ���
typedef struct tagTKPLAYERDYNFIELDDEFINE
{
    DWORD dwFieldID;                                 // ��̬�ֶα��
    char szFieldName[TK_BUFLEN_PLAYERDYNFIELDNAME];  // ��̬�ֶ���ʾ����
    DWORD dwGrowID;                                  // ��̬�ֶ���ֵ����ID
    BYTE byDataType;                                 // ��̬�ֶ���������
    BYTE byDateLen;                                  // ��̬�ֶ����ݳ���
    WORD wFlags;                                     // ��̬�ֶ����ݱ�־��Ϣ

} TKPLAYERDYNFIELDDEFINE, *PTKPLAYERDYNFIELDDEFINE;

/*=======================UserMatchInfoeServer ���� Star=============================*/

// ������Ϣ����
typedef struct tagTKMatchInfo
{
    char szMatchName[TK_BUFLEN_TOURNEYSIMPLENAME];  // ��������
    DWORD dwMPID;                                   // ��ƷID
    DWORD dwTourneyID;                              // Tourney ID
    DWORD dwMatchID;                                // ����ID
    DWORD dwGameID;                                 // ��ϷID
    DWORD dwMatchTime;                              // ����ʱ��

    DWORD dwMisIP;    // ���������IP��ַ
    DWORD dwMisPort;  // ��������Ķ˿ں�

    DWORD dwReserve1;  // Ԥ��
    DWORD dwReserve2;  // Ԥ��
} TKMATCHINFO, *PTKMATCHINFO;

/*=======================UserMatchInfoeServer ���� End==============================*/

/*=======================BroadInfoServer����  Star==================================*/

// ת��״̬
enum eBroadCastState
{
    BROADCASTSIGNUPING = 1,   // Ԥ�汨����
    BROADCASTSTARTUPING = 2,  // ������
    BROADCASTPROGRESS = 3,    // ����ת����
    BROADCASTSTOP = 4,        // �����ع�
    BROADCASTCANCEL = 5       // ȡ��
};

// ת���б�����Ϸ����
// ��ˢ��Ƶ����MatchService���ƣ�
typedef struct tagLsBroadCastAmount
{
    DWORD dwTourneyID;        // Tourney ID
    DWORD dwSignUpAmount;     // ��������
    DWORD dwPlayingAmount;    // ��ǰ��ս����
    DWORD dwRunAmount;        // ��������
    DWORD dwSpectatorAmount;  // �Թ�����
    DWORD dwBetUserAmount;    // ���²�������

} LSBROADCASTAMOUNT, *PLSBROADCASTAMOUNT;

//ת���б��б�������
typedef struct tagTkBroadCastData
{
    DWORD dwTourneyID;  //TourneyID
    DWORD dwProductID;  //ProductID

    time_t tMatchStartTime;  // ������ʼʱ��,һ�����ڱ�����ʼʱ��
    WORD wBroadState;        // ת��״̬
    WORD wBetState;          //����״̬���μ�eBRDISBetStateControlType���壩
    WORD wGameID;            // GameID,��Ϸ���
    BYTE byStartMatchType;   // ������������(�μ�eTourneyStartMatchType����)
    BYTE byMatchPointType;   // ����������(�μ�eTouryenMatchPointType����)
    BYTE byBoutNo;           // ���α�ţ�һ�����������ڼ����ı�ţ�
    BYTE byTeamType;         // ������Ԫ����(�μ�eTourneyTeamType����)

    DWORD dwMisIP;  // ����������IP
    int nMisPort;   // ����������

} TKBROADCASTDATA, *PTKBROADCASTDATA;

//ת���б��е���������ת��״̬
typedef struct tagTkBroadCastState
{
    DWORD dwTourneyID;     // Tourney ID
    WORD wBroadCastState;  // ת��״̬
    WORD wBetState;        // ����״̬
    WORD wFlags;           // ��ʶ��1��ʾ���̽�״̬�㲥���ͻ���

} TKBROADCASTSTATE, *PTKBROADCASTSTATE;

// �µ�ת��������������Ϣ
typedef struct tagTkBroadCastMatchPoint
{
    DWORD dwMatchPoint;       // ������
    DWORD dwMatchID;          // MatchID
    DWORD dwSignUpAmount;     // ��������
    DWORD dwPlayingAmount;    // ��ǰ��ս����
    DWORD dwRunAmount;        // ��������
    DWORD dwSpectatorAmount;  // �Թ�����
    DWORD dwBetUserAmount;    // ���²�������
    WORD wState;              // ������״̬
    WORD wBetState;           // ����״̬
    WORD wParam;              // ���������
} TKBROADCASTMATCHPOINT, *PTKBROADCASTMATCHPOINT;

// ҵ��۷ѵ�Ԫ
typedef struct tagTKCostUnit
{
    DWORD dwDCID;     // �������ͣ���1-Money,2-Ware,3-Grow������μ�:eDicTKSTGDCDef��TKSTBillUniversalDic.h
    DWORD dwAccType;  // Ŀǰֻ��Money���ã���0-Ĭ���˻���10-�����䣬100-���˿���1000-������𣬾���μ���eDicMynsAccNumType��TKMoneyUniversalDicSrv.h
    DWORD dwDTID;     // ����������
    DWORD dwDValue;   // ��ֵ

    DWORD dwReserve1;  // �����ֶ�1
    DWORD dwReserve2;  // �����ֶ�2
} TK_COSTUNIT, *PTK_COSTUNIT;

// ҵ��۷ѷ���
typedef struct tagTKCostPlan
{
    // �۷�ҵ������������ͣ��ڴ���չ
#if _MSC_VER >= 1600
    enum DataType : DWORD
#else
    enum DataType
#endif
    {
        ENUM_RELIFE_MAX_RANK = 1,
    };

    DWORD dwCostPlanID;  // �۷ѷ���ID
    DWORD dwScore;       // �����Ļ���
    DWORD dwRank;        // ����������, 0��ʾ������Ч

    DWORD dwReliveBout;  // 0��������һ�֣�1�����ִθ���
    DWORD dwDataType;    // �۷�ҵ�������������
    DWORD dwDataValue;   // �۷�ҵ���������ֵ
    DWORD dwReserve4;    // �����ֶ�

    DWORD dwUnitCount;  // �۷ѵ�Ԫ����
    TKSUFFIXIDX sufData;
} TK_COSTPLAN, *PTK_COSTPLAN;
// ��׺���� dwUnitCount��TK_COSTUNIT

// ҵ��۷ѽ��
enum eDicTypeTKPaymentResult
{
    TK_PAYMENT_RESULT_SUCCESS = 0,  // �ɹ�
    TK_PAYMENT_RESULT_FAILED = 1,   // ʧ��

    TK_PAYMENT_RESULT_MONEY_SRCLACK = 100,  // ���Ҳ��㣨���˽�ң���ȯ������ȯ��������⣩
    TK_PAYMENT_RESULT_GOLD_SRCLACK = 101,   // ��Ҳ���
    TK_PAYMENT_RESULT_BONUS_SRCLACK = 102,  // ��ȯ����
    TK_PAYMENT_RESULT_CERT_SRCLACK = 103,   // ����ȯ����Ʊ������
    TK_PAYMENT_RESULT_WARE_SRCLACK = 104,   // ��Ʒ����
    TK_PAYMENT_RESULT_GROW_SRCLACK = 105,   // ���ֲ���
    TK_PAYMENT_RESULT_NOT_SUPPORT = 200,    // ��֧�ִ���۷�ҵ��
    TK_PAYMENT_RESULT_RESTRICT = 1000,      // �û������Ʋ���,��Σ�û��������Ʋ���ʱ��MatchSrv�ķ���ֵ
};

/*=======================BroadInfoServer����  end==================================*/

//��̬�ֶ���������
#define TK_PLAYERDYNFIELD_DATATYPE_CHAR 1          //�ַ�
#define TK_PLAYERDYNFIELD_DATATYPE_BYTE 2          //
#define TK_PLAYERDYNFIELD_DATATYPE_SHORT 3         //
#define TK_PLAYERDYNFIELD_DATATYPE_WORD 4          //
#define TK_PLAYERDYNFIELD_DATATYPE_LONG 5          //
#define TK_PLAYERDYNFIELD_DATATYPE_DWORD 6         //
#define TK_PLAYERDYNFIELD_DATATYPE_FLOAT 7         //��������4���ֽ�
#define TK_PLAYERDYNFIELD_DATATYPE_STRING 8        //�ַ�����byDateLen����������'\0'
#define TK_PLAYERDYNFIELD_DATATYPE_PERCENT 9       //�ٷ�������4�ֽ�int��ʾ����ʾʱ����ֵ /1000���Ϸ���"%"
#define TK_PLAYERDYNFIELD_DATATYPE_FLOAT10 10      //��������4���ֽ�(/10)
#define TK_PLAYERDYNFIELD_DATATYPE_FLOAT100 11     //��������4���ֽ�(/100)
#define TK_PLAYERDYNFIELD_DATATYPE_FLOAT1000 12    //��������4���ֽ�(/1000)
#define TK_PLAYERDYNFIELD_DATATYPE_FLOAT10000 13   //��������4���ֽ�(/10000)
#define TK_PLAYERDYNFIELD_DATATYPE_FLOAT100000 14  //��������4���ֽ�(/100000)

//��̬�ֶ����ݱ�־��Ϣ����
#define TK_PLAYERDYNFIELD_FLAG_HIDE 0x1000        // ���ֶβ���ʾ
#define TK_PLAYERDYNFIELD_FLAG_VALUE 0x0001       // ֱ�Ӱ�������������ʾ��ֵ
#define TK_PLAYERDYNFIELD_FLAG_CONVERTSTR 0x0002  // ��dwGrowID����ֵ���ݸ�����ת�����ַ�����ʾ
#define TK_PLAYERDYNFIELD_FLAG_MATCHDATA 0x0004   // dwGrowID��ʶ���Ǳ�������

//�������ݶ���
#define TK_MATCHDATA_TYPE_SCORE 1  // ��������
#define TK_MATCHDATA_TYPE_BLOOD 2  // ����ֵ
#define TK_MATCHDATA_TYPE_CHIP 3   // ����

//�����û����ٱ�־
#define TK_MATCHTRACE_FLAGS_OPPINFO 0x00000001  // ��Ҫ�������˶Ծ���Ϣ
#define TK_MATCHTRACE_FLAGS_HDUGROW 0x00000002  // ��Σ�û�����Ҫ����
#define TK_MATCHTRACE_FLAGS_MOBBIND 0x00000004  // �����ֻ��û�
#define TK_MATCHTRACE_FLAGS_SBIE 0x80000000     //
#define TK_MATCHTRACE_FLAGS_VMWARE 0x40000000   //
#define TK_MATCHTRACE_FLAGS_VPC 0x20000000      //
#define TK_MATCHTRACE_FLAGS_VBOX 0x10000000     //
#define TK_MATCHTRACE_FLAGS_QMDLL 0x08000000    //
#define TK_MATCHTRACE_FLAGS_CHEATPC 0x04000000  //
#define TK_MATCHTRACE_FLAGS_OFFLINE 0x02000000  // ��Ϸʱ��������
#define TK_MATCHTRACE_FLAGS_WT 0x01000000       //

//һ��HandGame�Ľ������Ͷ���
#define TK_GAMEOVERTYPE_NOTALLARRIVED 1  // û�е���
#define TK_GAMEOVERTYPE_EXITDIR 2        // ����ʱֱ���˳����ƶ��ˣ�
#define TK_GAMEOVERTYPE_ENTEROVERTIME 3  // ���������ʱ
#define TK_GAMEOVERTYPE_NORNAL 11        // HandGame��������
#define TK_GAMEOVERTYPE_ALLBREAK 12      // �����˶���
#define TK_GAMEOVERTYPE_ALLTRUSTPLAY 13  // �������й�
#define TK_GAMEOVERTYPE_FORCEREGROUP 21  // ��Ϸʱ��̫�����㡰��������MatchSrvҪ��ǿ�ƽ�����������
#define TK_GAMEOVERTYPE_FDTABLE_END 22   // ���ѷ� ��Ϸ��������·������

//һ��HandGame�Ľ�����־��Ϣ
#define TK_GAMEOVERFLAG_NULL 0
#define TK_GAMEOVERFLAG_ONLYDECPLAYER 0x0001  //�������ٵ�ǰ������ѡ�֣�ʣ��ѡ�ֵ�ǰ������
#define TK_GAMEOVERFLAG_FAIL_GETCARDS 0x0002  //1V1ȡ�Ƴ�ʱ

// match action����
#define TK_MATCH_ACTION_PEAKTABLE 2  // ִ���۷�Ծֶ���
// ��׺���� : <action id=2 delay=%d></action>
#define TK_MATCH_ACTION_RANDOMAWARD 3  // ִ������佱��������
// ��׺���� : <action id=3 delay=6 SchemeID=%d></action> delay=6Ϊ�󽱣� delay=4ΪС��
#define TK_MATCH_ACTION_WHEELTABLE 4  // ִ�����ֶ̾���
// ��׺���� : <action id=4 delay=6 SchemeID=%d></action>
#define TK_MATCH_ACTION_MATCH_START 5  // ִ�б�����ʼ����
// ��׺���� : <action id=5 delay=2></action>
#define TK_MATCH_ACTION_BOUT_START 6  // ִ���ֿ�ʼ����
// ��׺���� : <action id=6 delay=2 promote_count=%d game_count=%d is_final_stage=1/0 stage_type=%d></action>
#define TK_MATCH_ACTION_BOUT_RESULT 7  // ִ���ֽ������
// ��׺���� : <action id=7 delay=2 status=����/��̭/�ȴ�/�ھ�/�Ǿ�/����/��%d�� rank=%d player_count=%d></action>
#define TK_MATCH_ACTION_FINAL_TABLE 8  // ִ�йھ���ʤ����
// ��׺���� : <action id=8 delay=2 game_count=%d></action>
#define TK_MATCH_ACTION_BREAKHEART_CHGHEART 9  // ִ�����Ķ���/���Ķ���
// ��׺���� : <action id=9 delay=4 FromHeartCount=%d ToHeartCount=%d></action>
#define TK_MATCH_ACTION_BREAKHEART_CHGSCORE 10  // ִ�мӷֶ���/���ֶ���
// ��׺���� : <action id=10 delay=4 FromMatchScore=%d ToMatchScore=%d></action>
#define TK_MATCH_ACTION_DELAY_READY 11  // ִ�г�ʼ�Ķ���
// ��׺���� : <action id=11 delay=3></action>
#define TK_MATCH_ACTION_HANDWIN 12  // ִ��Ӯ�ƽ�������
// ��׺���� : <action id=12 delay=\"1\" SubType=\"%d\" Param=\"%d\" DataClass=\"%d\" DataID=\"%d\" amount=\"%d\" measure= \"%s\" name=\"%s\" Winner=\"%u\" Note=\"Ӯ�ƻ��%d%s%s\" />
#define TK_MATCH_ACTION_WAIT_TIMEOUT_PAYMENT 13  // ִ�н���೵�����ȴ���ʱ��������
// ��׺���� : <action id=13 delay=\"2\" timeout=\"%d\" payement=\"%d\" note=\"�����ȴ���ʱ���%d�����\"/>
#define TK_MATCH_ACTION_MATCH_ARCHIVE 14  // ִ�н���೵�浵�ɹ�����
// ��׺���� : <action id=14 delay=\"2\" rank=%d player_count=%d win_award=%d timeout_payment=%d keep_days=%d />
#define TK_MATCH_ACTION_WAIT_TIME 15  // ִ�н���೵�����ȴ���ʱ��ʼ����
// ��׺���� : <action id=15 delay=\"2\" rank=%d player_count=%d win_award=%d timeout_payment=%d keep_days=%d />
#define TK_MATCH_ACTION_1V1_MATCH 16  // ִ��1v1��ʽ�����Ŷ���
// ��׺���� : <action id=16 delay=\"2\" rank=%d player_count=%d win_award=%d timeout_payment=%d keep_days=%d />
#define TK_MATCH_ACTION_PLAYER_HUNTCOUT 17  // �����������ɱ����
// ��׺���� : <action id=17 delay=\"2\" use_id0=%d hunt_count0=%d user_id1=%d hunt_count1=%d user_id2=%d hunt_count2=%d user_id3=%d hunt_count3=%d />
#define TK_MATCH_ACTION_IN_SUPERBANKER 18  // ����ׯ�� ��ׯ
// ��׺���� : <action id=18 delay=4 reason=%d note=%s></action>
#define TK_MATCH_ACTION_OUT_SUPERBANKER 19  // ����ׯ�� ��ׯ
// ��׺���� : <action id=19 delay=4 reason=%d note=%s></action>
#define TK_MATCH_ACTION_PLAYER_SCORE 20  // Ʈ�ֶ���
// ��׺���� : <action id=\"20\" delay=\"2\">
// <Item ID=\"%d\" score=\"%d\" SectionScore=\"%d\" userID=\"%d\" UserName=\"%s\"/>
// <Item ID=\"%d\" score=\"%d\" SectionScore=\"%d\" userID=\"%d\" UserName=\"%s\"/>
// <Item ID=\"%d\" score=\"%d\" SectionScore=\"%d\" userID=\"%d\" UserName=\"%s\"/>
// </action>

#define TK_MATCH_ACTION_TEAM_WAIT_TIME 21  // ��������ִ�н����ȴ�����
// ��׺���� : <action id=\"21\" delay=\"2\" TID=\"%d\" TName=\"%d\" TScore=\"%s\" TThirdScore=\"%d\" TRScore=\"%d\" TRSecondScore=\"%d\" matchstate=\"%d\" matchstatevalid=\"%d\" rank=\"%d\" winner=\"%d\" >
// <Item ID=\"%d\" name=\"%s\" roundscore=\"%d\" roundsecscore=\"%d\" score=\"%d\" secscore=\"%d\" sectionscore=\"%d\" inround=\"%d\">
// <Item ID=\"%d\" name=\"%s\" roundscore=\"%d\" roundsecscore=\"%d\" score=\"%d\" secscore=\"%d\" sectionscore=\"%d\" inround=\"%d\">
//  ...
// </action>

#define TK_MATCH_ACTION_PROCESSAWARD 22  // ���̽�������
// ��׺���� :
//<action id=\"22\" delay=\"2\" use_id=\"%d\">
//	<ProcessAwardResult UserID=\"%d\" GameID=\"%d\" MPID=\"%d\" TourneyID=\"%d\" MatchID=\"%d\">
//		<Biz ID=\"%d\" Multiple=\"%d\" CurrentCount=\"%d\" LimitCount=\"%d\">
//			<W ID=\"%d\" V=\"%d\" />
//		</Biz>
//	</ProcessAwardResult>
//</action>

#define TK_MATCH_ACTION_PLAYER_RELIVEINFO 23  // ���ִ���Ҹ����֪ͨ���������������Ϣ
// ��׺���� : <action id=\"23\" delay=\"2\" userid=\"%d\" rank=\"%d\" score=\"%d\" />

#define TK_MATCH_ACTION_CHALLENGEINFO 24  // ��ս����ʤ�۸�֪ͨ
// ��׺���� :
//<action id=\"%d\"  Result=\"%d\" OutResult=\"%d\" FromWinCount=\"%d\" FromFailCount=\"%d\" ToWinCount=\"%d\" ToFailCount=\"%d\"></action>
// ����ׯʧ��ԭ��

#define TK_MATCH_ACTION_PLAYER_CV 25  // ���������ʤ����
// ��׺���� : <action id=25 delay=\"2\" uid0=%u cv0=%d uid1=%u cv1=%d uid2=%u cv2=%d uid3=%u cv3=%d />

#define TK_MATCH_ACTION_DOUBLEPLAYER_MATCH_START 26  // ˫����ӱ�����ʼ
// ��׺���� : <action id=26 delay=2 uid=%u, name=%s formid=%u figid=%u memuid=%u memname=%s memformid=%u memfigid=%u memorder=\"%d\" curboutcount=\"%d\"/>

#define TK_MATCH_ACTION_DOUBLEPLAYER_MATCHOUT 27  // ˫����Ӷ�����̭����
// ��׺���� : <action id=27 delay=2 memuid=%u, memname=%s memorder=%u />

#define TK_MATCH_ACTION_DOUBLEPLAYER_PROMOTION 28  // ˫����Ӷ��ѽ�������
// ��׺���� : <action id=28 delay=2 memuid=%u, memname=%s memorder=%u curboutcount=%u />

#define TK_MATCH_ACTION_DOUBLEPLAYER_FINAL_TABLE 29  // ˫����Ӷ��Ѿ�������ʾ
// ��׺���� : <action id=29 delay=2 memuid=%u, memname=%s memorder=%u matchname=%s />

#define TK_MATCH_ACTION_DOUBLEPLAYER_TEAMINFO 30  // ˫����Ӷ�����Ϣ����
// ��׺���� : <action id=30 delay=2 uid=%u, name=%s formid=%u figid=%u memuid=%u memname=%s memformid=%u memfigid=%u memgsip=%u memgsport=%u membsip=%u membsport=%u enablewatch=%u/>

#define TK_MATCH_ACTION_PLAYER_DIFF_SCORE 31  //�ּ���һ��ֲ�
// ��׺���� : <action id=\"31\" diffscore = %u >
// <Item uid = %u name = %s formid = %s figid = %u/>
// </action>

enum eDicBankerOverReason
{
    TK_BANKER_OVER_NULL = 0,  // δ֪״̬��������ʱ��״̬

    TK_BANKER_BANKER_PLAYING = 1,  // �Ѿ��г���ׯ����Ϸ�У�ֻ���Ŷӵȴ�
    TK_BANKER_GOLD_NOTENOUGH = 2,  // ��Ҳ���
    TK_BANKER_CHIPLOWERLIMIT = 3,  // �����������
    TK_BANKER_BREAKONLINE = 4,     // ��Ҷ���
    TK_BANKER_LEAVE = 5,           // ����뿪
    TK_BANKER_STANDUP = 6,         // ���վ��
    TK_BANKER_TIMEOUT = 7,         // ����ׯ��Ϸʱ�����
    TK_BANKER_CANCEL = 8,          // ���������ׯ
};

//��ɱ������Ϣ 10x10BYTE poker minipoker ThreeCard
typedef struct tagTKHUNTER_B10X10
{
    tagTKHUNTER_B10X10()
    {
        memset(byaQuarry, 0, sizeof(byaQuarry));
    }

    BYTE byaQuarry[10][10];  //10nSeatHunter x 10nSeatHunted, >0����ɱ
} TKHUNTER_B10X10, *PTKHUNTER_B10X10;

//��ɱ������Ϣ 10x3INT Lord Mahjong
typedef struct tagTKHUNTER_I10X3
{
    tagTKHUNTER_I10X3()
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                liParam[i][j] = -1;
            }
        }
    }

    int liParam[10][3];  //10x(nSeatHunter, nSeatHunted, nHuntRate), -1��Ч
} TKHUNTER_I10X3, *PTKHUNTER_I10X3;

#define TK_GAME_RULER_AWARD_HUNTER 1000        // ���˽���(10x3INT Lord Mahjong)
#define TK_GAME_RULER_AWARD_POKER_HUNTER 1001  // ���������˽���(10x10BYTE poker minipoker ThreeCard)
#define TK_GAME_RULER_AWARD_WARE 1002          // ��Ʒ����(2INT WareID,Amount FryUpgrade���׽���)
#define TK_GAME_RULER_AWARD_BREAKHEART 1003    // ���Ľ���
#define TK_GAME_RULER_AWARD_HANDWIN 1004       // Ӯ�ƽ��� wuqy add 2016.11.10
