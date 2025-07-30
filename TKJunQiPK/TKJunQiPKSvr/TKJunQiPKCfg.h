#pragma once
#include "TKDllGame.h"
#include "TKDllPlayer.h"
#include "TKJunQiPKDefine.h"

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
    ECI_HonorHalf = 18,            //���ھ������ޣ��ҵ��ڴ�ֵʱ���۷ּ���
    ECI_WinHonorPlus = 19,         //�ǵ������ľ���ʤ��ʱ����ֵ
    ECI_LossHonorMinus = 20,       //�ǵ������ľ���ʧ��ʱ����ֵ //21 - ���鿪��
    ECI_FeelColdTime = 22,         //������ʱ��
    ECI_CalcType = 23,             //���������ʽ��0-����ʽ���㣬1-�����ü��㣩
    ECI_FriendTable = 24,          //�Ƿ�Ϊ��������0-��1-�ǣ�
    ECI_FriendTime = 25,           //���������ù��򣬲���ʱ��
    ECI_WinExp = 26,               //ʤ��ʱ��Ӿ���ֵ
    ECI_DrawExp = 27,              //ƽ��ʱ��Ӿ���ֵ
    ECI_bottomTime = 28,           //����ʣ�����ʱ��
    ECI_DelayReduceTime = 29,      // �ӳٺ�Ŀ۳��Ĳ���ʱ��
    ECI_ChanseCount = 30,          //����׷�Ӵ�������
    ECI_MatchType = 31,            //�������ͣ�1-��λ����2-��������3-�������
    ECT_PerMatchCount = 32,        //ÿ�ձ���������0-δ���ã�n-����ID��
    ECT_PerMatchTime = 33,         //ÿ�ձ���ʱ����0-δ���ã�n-����ID��
    ECI_LoseExp = 34,              //ʧ��ʱ��Ӿ���ֵ
    ECI_NegativeNoFightStep = 35,  //��������δ������Ϊ����
    ECI_NegativeMatchTime = 36,    //���������Ծ�ʱ��
    ECI_End,                       //���ý�����ʶ
};
const int g_nDefaultValue[ECI_End] = {1, 5,  15, 3,  50, 280, 3, 2,  30, 3, 9, 20, 40, 16, 0, 0, 0,  0, 120,
                                      1, -1, 1,  10, 0,  0,   0, 25, 10, 5, 3, 4,  1,  0,  0, 5, 50, 90};

class CTKJunQiPKCfg
{
public:
    CTKJunQiPKCfg();
    ~CTKJunQiPKCfg();

public:
    void InitCfg(const string& szCfg);
    int GetCfg(ECfgIndex nCfgId);
    void SetCfg(ECfgIndex nCfgId, int nCfgValue);

private:
    int m_nCfgValue[ECI_End];
};
