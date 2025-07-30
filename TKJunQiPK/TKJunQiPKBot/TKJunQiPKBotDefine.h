#pragma once

#define PLAYER_COUNT 2       //�����
#define CROSS_COUNT 60       //�������
#define SIDE_CROSS_COUNT 30  //ÿ���������
#define SIDE_CHESS_COUNT 25  //ÿ��������

#define JUNQIPK_UNKNOWN -1  //δ֪

enum ECfgIndex
{
    ECI_GameType = 0,        //��Ϸ����
    ECI_InitTime = 1,        //��ʼ��ʱ��
    ECI_MoveTime = 2,        //�ƶ�����ʱ��
    ECI_MoveActionTime = 3,  //�ƶ����ӵĶ���ʱ��
    ECI_NoFightStep = 4,     //����50��û�г�����Ϊ��ǿ�ƺ;֡�
    ECI_TotalStep = 5,  //�ܲ�������280�������ʱû�в����Ļغϲ������Ӳ�������ǿ�ƺ;֡�
    ECI_RepeatStep = 6,    //�˲����ظ���4�Ρ�ǿ���и���
    ECI_PassCount = 7,     //��ʱ���Ρ�ǿ���и���
    ECI_PeaceStep = 8,     //δ��20����������͡�
    ECI_PeaceCount = 9,    //��ʹ����������Σ��������
    ECI_PeaceTime = 10,    //��ͳ�ʱʱ��
    ECI_ConcedeStep = 11,  //δ��10��������Ͷ��
    ECI_HonorLower = 12,   //�������ޣ�ʧ��ʱ���ڴ�ֵʱ���۷�
    ECI_KFactor = 13,      //����Kֵ
    ECI_CleanDiLei = 14,   //�������ƣ�0-��������,1-��С�����ף�
    ECI_CarryJunQi = 15,   //�������ƣ�0-�����ӿ���,1-��С�ӿ��壩
    ECI_FirstWay = 16,     //���ַ�ʽ��0-������֣�1-�����ϵͷ����֣�
    ECI_ConfirmColor = 17,  //ȷ����ɫ��0-����������ͬ��ɫȷ����Ӫ��1-���ַ�������Ϊ���ֵ����ӣ�
    ECI_HonorHalf = 18,       //���ھ������ޣ��ҵ��ڴ�ֵʱ���۷ּ���
    ECI_WinHonorPlus = 19,    //�ǵ������ľ���ʤ��ʱ����ֵ
    ECI_LossHonorMinus = 20,  //�ǵ������ľ���ʧ��ʱ����ֵ
    // 21 - ���鿪�أ�22 - ������ʱ��
    ECI_CalcType = 23,       //���������ʽ��0-����ʽ���㣬1-�����ü��㣩
    ECI_FriendTable = 24,    //�Ƿ�Ϊ��������0-��1-�ǣ�
    ECI_FriendTime = 25,     //���������ù��򣬲���ʱ��
    ECI_WinExp = 26,         //ʤ��ʱ��Ӿ���ֵ
    ECI_DrawExp = 27,        //ƽ��ʱ��Ӿ���ֵ
    ECI_DelayCount = 28,     //�����ӳٴ���
    ECI_DelayMoveTime = 29,  //�ӳٺ���ж�ʱ��
    ECI_ChanseCount = 30,    //����׷�Ӵ�������
};
//���������
enum ECrossType
{
    ECT_BZ,   //��վ
    ECT_XY,   //��Ӫ
    ECT_DBY,  //��Ӫ
};

enum EChessColor
{
    EC_Blue = 0,  //��ɫ
    EC_Red = 1    //��ɫ
};

//��������
enum EChessType
{
    ECT_Unknow = -1,  //δ֪
    ECT_ZhaDan = 0,   //ը��
    ECT_DiLei,        //����
    ECT_JunQi,        //����
    ECT_GongBing,     //����
    ECT_PaiZhang,     //�ų�
    ECT_LianZhang,    //����
    ECT_YingZhang,    //Ӫ��
    ECT_TuanZhang,    //�ų�
    ECT_LvZhang,      //�ó�
    ECT_ShiZhang,     //ʦ��
    ECT_JunZhang,     //����
    ECT_SiLing,       //˾��

    ECT_ChessTypeCount,  //������
};

enum EFightResult
{
    EFR_PerishTogether,  //ͬ���ھ�
    EFR_Win,             //սʤ
    EFR_Loss,            //ս��
};

//ս���¼�
enum EFightEvent
{
    EFE_SiLingDead,  //��˾������
    EFE_JunQiDead,   //�о��챻��
    EFE_Timeout,     //�о��챻��
};

enum GAMESCHEDULE
{
    TKJUNQI_SCHEDULE_INITLAYOUT,
    TKJUNQI_SCHEDULE_SELECTCHESS,
    TKJUNQI_SCHEDULE_MOVECHESS,
    TKJUNQI_SCHEDULE_ACTIONFINISH,
    TKJUNQI_SCHEDULE_CONCEDE,
    TKJUNQI_SCHEDULE_PEACE,
    TKJUNQI_SCHEDULE_PEACERES,
    TKJUNQI_SCHEDULE_SHOWCHESS,
    TKJUNQI_SCHEDULE_INITRULE,
    TKJUNQI_SCHEDULE_CANCELLAYOUT,
};

enum EGameType
{
    GT_None = 0,
    GT_FANFANQI = 1,    // ������
    GT_ERRENANQI = 2,   // ���˰���
    GT_SIGUOSIAN = 3,   // �Ĺ�����
    GT_SHUANGMING = 4,  // �Ĺ�˫��
};

enum ENTRY_TYPE
{
    TKJUNQI_EXACT,
    TKJUNQI_LOWER,
    TKJUNQI_UPPER,
};

#define GongBingCleanDiLei 0  //��������
#define AnyOneCarryJunQi 0    //�����ӿ���
#define RandomFirst 0         //�������
#define TwoConfirmColor 0     //����������ͬ��ɫȷ����Ӫ

#define FANFANQI 1

#define SETRULETIME 6
#define INITWAITTIME 7
#define MOVECHESSWAITTIME 8
#define MOVEACTIONWAITTIME 9
#define PEACEWAITTIME 10

const int SWAP_CHESS_NUM = 2;
const double INIT_ALPHA_VALUE = -200000;
const double INIT_BETA_VALUE = 200000;
const double SEARCH_MAX_VALUE = 100000;
const double SEARCH_MIN_VALUE = -15000;
const double DISTANCE_WEIGHT = 0.00000001;
const double CAMP_WEIGHT = 0.01;
const double MIN_DOUBLE_VALUE = 0.000000001;
const double NO_FIND_IN_TTABLE = 666666;