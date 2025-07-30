#pragma once
#include <Shlwapi.h>
#include <TKDllServer.h>
#include <limits.h>
#include <map>
#include <vector>

#define SIDE_CHESS_COUNT 25  // ÿ��������
#define SIDE_CROSS_COUNT 30  // ÿ���������
#define QUICK2_POS_START 5   // �����淨2��ʼ����λ��
#define JUNQIPK_UNKNOWN -1   // δ֪

#define MAX_TRACESTRINGSIZE (1024 * 100)  // ÿ��д��־�ó���
#define MAX_TRACESTRINGTIME (5 * 60)  // �ϴ�д���ʱ�����´���־�洢���������ڸ�ʱ���д���ļ�����λ��second��

#define LOG_ERROR(fmt, ...) \
    TKWriteLog("%s(%d)<%s>[ERROR]:" fmt, PathFindFileName(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__)

// ���������
enum ECrossType
{
    ECT_BZ,   // ��վ
    ECT_XY,   // ��Ӫ
    ECT_DBY,  // ��Ӫ
};

enum EChessColor
{
    EC_Blue,  // ��ɫ
    EC_Red,   // ��ɫ
    EC_All,   // ������ɫ
};

// ��������
enum EChessType
{
    ECT_ZhaDan,     // ը��
    ECT_DiLei,      // ����
    ECT_JunQi,      // ����
    ECT_GongBing,   // ����
    ECT_PaiZhang,   // �ų�
    ECT_LianZhang,  // ����
    ECT_YingZhang,  // Ӫ��
    ECT_TuanZhang,  // �ų�
    ECT_LvZhang,    // �ó�
    ECT_ShiZhang,   // ʦ��
    ECT_JunZhang,   // ����
    ECT_SiLing,     // ˾��

    ECT_ChessTypeCount,  // ������
};

enum EFightResult
{
    EFR_PerishTogether,  // ͬ���ھ�
    EFR_Win,             // սʤ
    EFR_Loss,            // ս��
};

// ս���¼�
enum EFightEvent
{
    EFE_SiLingDead,  // ��˾������
    EFE_JunQiDead,   // �о��챻��
    EFE_Timeout,     // �о��챻��
};

// ������״̬
enum EStage
{
    ES_Distributing,           // ���ڲ���
    ES_WaitingMoveChess,       // ���ڵȴ�����
    ES_WaitingActionComplete,  // ���ڵȴ��������
    ES_WaitingPeace,           // ������ͽ׶Σ���Ϸ��ͣ
    ES_SetRule,                // ���ù���
    ES_Finished,               // ��Ϸ�ѽ���
};

enum EGrowId
{
    EG_DayWinCount = 50540006,        // ÿ��ʤ������
    EG_DayPlayCount = 50540007,       // ÿ����Ϸ����
    EG_DayDesDiLeiCount = 50540010,   // ÿ��������׸���
    EG_DayDesSiLingCount = 50540011,  // ÿ������˾�����
    EG_DayDesChessCount = 50540012,   // ÿ���������Ӹ���
    EG_HundredStepWin = 50540013,     // ��100����ȡʤ����
    EG_PlayFFQRankGame = 50540015,    // �淭������λ��       --����
    EG_PlayAQRankGame = 50540016,     // �氵����λ��         --����
    EG_PlayPKGame = 50540017,         // �μ�һ��PK��
    EG_PlayPassGame = 50540018,       // �μ�һ��������

    EG_FFQRankCount = 50550006,  // �����嶨��������
    EG_AQRankCount = 50550007,   // ���嶨��������
    EG_SARankCount = 50550008,   // �İ�����������
    EG_SMRankCount = 50550009,   // ˫������������

    EG_Experience = 50560001,  // ����PK�ܾ������

    EG_JunQiJunGong = 50560005,        // �������              - ����
    EG_AQZongJuShu = 50560006,         // �����ܾ���
    EG_AQShengJuShu = 50560007,        // ����ʤ����
    EG_AQSaiJiShengJuShu = 50553028,   // ��������ʤ����  ���ڼ�¼�����������ʤ����
    EG_AQFuJuShu = 50560008,           // ���帺����
    EG_AQHeJuShu = 50560009,           // ����;���
    EG_AQDuanXianJuShu = 50560010,     // ������߾���
    EG_FFQZongJuShu = 50560011,        // �������ܾ���
    EG_FFQShengJuShu = 50560012,       // ������ʤ����
    EG_FFQSaiJiShengJuShu = 50553027,  // ����������ʤ����  ���ڼ�¼��ҷ����嵥��������ʤ����
    EG_FFQFuJuShu = 50560013,          // �����帺����
    EG_FFQHeJuShu = 50560014,          // ������;���
    EG_FFQDuanXianJuShu = 50560015,    // ��������߾���
    EG_Season = 50560016,              // ��������              - ����
    EG_ThroughPassGame = 50560027,     // ͨ�ش�����             - ����
    EG_FFQSeasonRank = 50560033,       // ��������λ����
    EG_AQSeasonRank = 50560034,        // ������λ����

    EG_DoubleCard = 50560048,             // ������
    EG_KeepScoreCard = 50560049,          // ���ֿ�
    EG_DayDoubleCardUseCount = 50553039,  // ������ÿ��ʹ�ô���

    EG_SASeasonRank = 50580105,       // �İ���λ����
    EG_SMSeasonRank = 50580106,       // ˫����λ����
    EG_SAZongJuShu = 50580107,        // �İ��ܾ���
    EG_SAShengJuShu = 50580108,       // �İ�ʤ����
    EG_SASaiJiShengJuShu = 50553029,  // �Ĺ��İ�����ʤ����  ���ڼ�¼����Ĺ��İ�����ʤ����
    EG_SAHeJuShu = 50580109,          // �İ�ƽ����
    EG_SAFuJuShu = 50580110,          // �İ�������
    EG_SADuanXianJuShu = 50580111,    // �İ����߾���
    EG_SMZongJuShu = 50580112,        // ˫���ܾ���
    EG_SMShengJuShu = 50580113,       // ˫��ʤ����
    EG_SMSaiJiShengJuShu = 50553030,  // �Ĺ�˫������ʤ����  ���ڼ�¼����Ĺ�˫������ʤ����
    EG_SMHeJuShu = 50580114,          // ˫��ƽ����
    EG_SMFuJuShu = 50580115,          // ˫��������
    EG_SMDuanXianJuShu = 50580116,    // ˫�����߾���
    EG_S_Experience = 50580117,       // ����PK�Ĺ��İ�˫������ֵ(������޸�50560001)
};

enum
{
    EVT_GameResult = 10000236,  // ����Ծֽ��(0:ƽ��  1:ʤ�� 2:ʧ��)
};

enum ETipsType
{
    ETS_TimeoutLoss = 1,       // ��ʱ�и�
    ETS_RepeatMoveLoss = 2,    // �ظ������и�
    ETS_UnFightDraw = 3,       // �����Ӻ���
    ETS_TotalStepDraw = 4,     // �ܲ�������
    ETS_UnFightTips = 5,       // ��������ʾ
    ETS_TotalStepTips = 6,     // �ܲ�����ʾ
    ETS_UnMoveAbleLoss = 7,    // ���������и�
    ETS_ConcedeLoss = 8,       // Ͷ���и�
    ETS_CarryJunQi = 9,        // ���屻��
    ETS_ChessPowerOver = 10,   // ������������
    ETS_DelayTimePulish = 11,  // ��ʱ�ͷ�
};

enum EGameType
{
    GT_FANFANQI = 1,    // ������
    GT_ERRENANQI = 2,   // ���˰���
    GT_SIGUOSIAN = 3,   // �Ĺ�����
    GT_SHUANGMING = 4,  // �Ĺ�˫��
};

enum EEndType
{
    EET_Normal = 0,   // ��������
    EET_ErrInit = 1,  // ��ʼ���쳣
    EET_Concede = 2,  // Ͷ��
    EET_Peace = 3,    // ���
    EET_UnFight = 4,  // δ��ս
    EET_MaxStep = 5,  // �ܲ�������
};

// ������������
enum ENegativeType
{
    ENT_NOT_NEGATIVE = 0,     // û����������
    ENT_TIMEOUT_AFK = 1,      // ��ʱ�һ�
    ENT_INITIATIVE_LOSE = 2,  // ��������
};

enum ENegativeAtomEvent
{
    ENA_EVENT = 10007301,  // ����-������������
};

// Debug��־���� 1��ӡ��־
#define OPEN_DEBUG_LOG 0

#define GongBingCleanDiLei 0  // ��������
#define AnyOneCarryJunQi 0    // �����ӿ���
#define RandomFirst 0         // �������
#define TwoConfirmColor 0     // ����������ͬ��ɫȷ����Ӫ

#define SETRULETIME 6
#define INITWAITTIME 7
#define MOVECHESSWAITTIME 8
#define MOVEACTIONWAITTIME 9
#define PEACEWAITTIME 10
const int Max_Round_Timmer = 11;
const int DELAY_TIPS_TIME = 5;

const int PK_Player_Count = 2;
const int Int_True = 1;
const int Max_Round_Time = 3600;

const int SWAP_CHESS_NUM = 2;    // �����ٸ����ӽ���
const int SWAP_SHOW_COUNT = 20;  // ��������ʱ����
const int UNSWAP_PERCENT1 = 10;  // ���滻�ĸ���
const int UNSWAP_PERCENT2 = 40;  // ���滻�ĸ���
const int ONE_HUNDRED = 100;

const int RANK_MATCH = 1;
const int PK_MATCH = 2;

const int CHASE_REPORT_COUNT = 2;
const int DELAY_TIME = 5;

const int g_nLossScore = 1500;  // �������ޣ����ٷ�����
const int g_GradingCount = 1;   // ����������
const int g_GradingWin = 50;    // ������ʤ���÷�
const int g_GradingDraw = 20;   // ������ƽ�ֵ÷�
const int g_GradingLoss = 10;   // ������ʧ�ܵ÷�

const int g_RankScoreCfgCol = 7;  // ��λ���÷���������
const int g_RankScoreCfgRow = 4;  // ��λ���÷���������
const int g_RankScoreArray[][g_RankScoreCfgRow] = {
    {0, 30, 0, 7},      {501, 30, -5, 5},   {1001, 30, -10, 3}, {2001, 30, -15, 2},
    {3001, 30, -20, 1}, {4001, 30, -25, 0}, {6001, 30, -30, 0},
};  // ������Ͱ��壬��λ�Ծֵ÷�(1��λ����,2ʤ���÷�,3ʧ�ܵ÷�,4ƽ�ֵ÷�)
const int g_SiGuoRankScoreArray[][g_RankScoreCfgRow] = {
    {0, 50, 0, 10},     {501, 50, -10, 7},  {1001, 50, -20, 5}, {2001, 50, -30, 3},
    {3001, 50, -40, 2}, {4001, 50, -50, 1}, {6001, 50, -50, 0},
};  // �Ĺ��İ����Ĺ�˫������λ�Ծֵ÷�(1��λ����,2ʤ���÷�,3ʧ�ܵ÷�,4ƽ�ֵ÷�)

const int g_RankDiffRow = 2;
const int g_UnMinusValue[] = {0, 50, 150, 300, 500, 800, 1200, 1600, 2000, 2500, 3000, 3500, 4000};  // ���۷ַ���
const int g_RankDiff[] = {200,  400,  600,  800,  1000, 1200, 1500,  2000,
                          2500, 3000, 4000, 5000, 6000, 8000, 10000, INT_MAX};  // ��λ���ֲ�±����ڷ����仯ֵ
const int g_SiRankDiff[][g_RankDiffRow] = {{200, 0},   {400, 1},   {600, 2},   {800, 3},   {1000, 4},   {1200, 5},
                                           {1400, 6},  {1600, 7},  {2000, 8},  {2500, 9},  {3000, 10},  {3500, 12},
                                           {4000, 14}, {5000, 16}, {6000, 18}, {8000, 20}, {10000, 22}, {INT_MAX, 25}};

const int g_nChessCount[ECT_ChessTypeCount] = {2, 3, 1, 3, 3, 3, 2, 2, 2, 2, 1, 1};   // ���Ӹ���
const int g_nQuickChess[ECT_ChessTypeCount] = {5, 0, 1, 3, 3, 3, 2, 2, 2, 2, 1, 1};   // ���ٷ���������
const int g_nQuick2Chess[ECT_ChessTypeCount] = {2, 2, 1, 2, 0, 3, 2, 2, 2, 2, 1, 1};  // ���ٷ���������
const int g_nChessScore[ECT_ChessTypeCount] = {0,  0,   1000, 10,  20,  50,
                                               80, 100, 200,  300, 400, 500};  // ���ٷ��������ӷ���
const int g_nChessPos[SIDE_CHESS_COUNT] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 12, 14,
                                           15, 16, 18, 19, 20, 22, 24, 25, 26, 27, 28, 29};  // ���ӵĲ���λ��
const int g_nJunqiUnpos[] = {5,  10, 15, 20, 9,  14, 19, 24, 25, 29, 35, 40, 45,
                             50, 55, 39, 44, 49, 54, 59, 26, 27, 28, 56, 57, 58};
const int g_nDileiUnpos[] = {25, 27, 29, 59, 57, 55};
const int g_nDileiUnPosLen = 2;
const int g_nSilingUnpos[] = {0, 1, 2, 3, 4, 30, 31, 32, 33, 34};
const int g_nKeyPos[] = {7, 12, 15, 16, 18, 19, 22, 27, 37, 42, 45, 46, 48, 49, 52, 57};
const int g_nKeyPosChessNum = 8;  // �ؼ�λ��������
const int g_nSpecialPos[] = {0, 1, 2, 3, 4, 30, 31, 32, 33, 34};
const std::map<int, std::vector<int>> g_specialCtrlPos = {
    {6, {0, 1, 2, 5, 7, 10, 12}},       {7, {1, 2, 3, 6, 8, 12}},       {8, {2, 3, 4, 7, 9, 12, 14}},
    {36, {30, 31, 32, 35, 37, 40, 42}}, {37, {31, 32, 33, 36, 38, 42}}, {38, {32, 33, 34, 37, 39, 42, 44}}};