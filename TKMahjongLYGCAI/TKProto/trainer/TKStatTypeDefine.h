
#if !defined(__STATTYPEDEFINE__INCLUDED__)
#define __STATTYPEDEFINE__INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//���ٲ���ͳ��
#define	TK_STAT_TYPE_NTV						9000
//nType2=AreaID
//nType3=IdcID
//nType4=�����������ķ����������ܷ���ͨ
//nType5=�ͻ�������������Ƿ�����ͨ
//ll0=Ԥ������ֵ
//ll1=10��ƽ����ʱ(ms)
//ll2=������
//ll3=������


//��ˢ����ҷ��ͳ��
#define	TK_STAT_TYPE_LOBBYTRACECLOSE			9100
//ll0=�˴�
//ll1=ʱ��
//ll2=��ѱ�������
//ll3=�ܱ�������
//ll4=������
//ll5=�����ˮ+
//ll6=�����ˮ-
//ll7=��ҳ�ֵ

//��Σ�����û�ͳ��
#define	TK_STAT_TYPE_HIGHDANGEROUSUSER			9110
//nType2=PlatID
//ll0=��¼�˴�
//ll1=�����˴�
//ll2=�����˴�
//ll3=�ɹ���������
//ll4=�ܾ���������


//��������ͳ��
#define	TK_STAT_TYPE_MATCH						10000
//nType2=ProductID
//ll0=�ٰ쳡��
//ll1=��������
//ll2=ȡ������
//ll3=�׶���
//ll4=����-������(ϵͳ�ӽǣ��޷�������)
//ll5=����-������(ϵͳ�ӽǣ��޷�������)
//ll6=����-�û���ʱ��ϵͳ�ӽǣ��޷������ݣ����Բ����ߺ�ʱ�ۼƣ�
//ll7=�û��ɵþ���(ֻ��Ե�������������뵺ʱ���ţ��ɷ���)


//������Աͳ��
#define	TK_STAT_TYPE_MATCH_PLAYER				10100
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//nType5=LanguageID
//ll0=����(�˴�/����)
//ll1=ȡ������(�˴�/����)
//ll2=��������(�˴�/����)
//ll3=����ȡ��(�˴�/����)
//ll4=����ʱ�˷�(�˴�/����)
//ll5=�������˷�(�˴�/����)
//ll6=����(�˴�/����)
//ll7=��Ѫ(�˴�/����)

//�뵺ͳ��
#define	TK_STAT_TYPE_MATCH_LEAVEISLAND			10101
//nType2=ProductID
//nType3=1�����뵺��2�����뵺
//ll0=С�ڵ���1��(�˴�/����)
//ll1=С�ڵ���2��(�˴�/����)
//ll2=С�ڵ���5��(�˴�/����)
//ll3=С�ڵ���10��(�˴�/����)
//ll4=����10��(�˴�/����)
//ll5=С�ڵ���1����(�˴�/����)
//ll6=С�ڵ���5����(�˴�/����)
//ll7=С�ڵ���30����(�˴�/����)
//ll8=С�ڵ���60����(�˴�/����)
//ll9=����60����(�˴�/����)


//����������ߺ��뿪ͳ��
#define	TK_STAT_TYPE_MATCH_ENTERBREAKEXIT		10102
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//ll0=�������
//ll1=ֱ�˱���
//ll2=�й�
//ll3=����
//ll4=�����ػ�
//ll5=�߿�����
//ll6=���ƶ���
//ll7=�����뿪
//ll8=
//ll9=


//��������ͳ��
#define	TK_STAT_TYPE_MATCH_RELIVE				10200
//nType2=ProductID
//nType3=GameID ������wuqy 2022.11.1��
//ll0=1�θ���ɹ�(�˴�/����)
//ll1=2�θ���ɹ�(�˴�/����)
//ll2=3�θ���ɹ�(�˴�/����)
//ll3=4����ɹ�(�˴�/����)
//ll4=�����θ���ɹ�(�˴�/����)
//ll5=1�θ���ʧ��(�˴�/����)
//ll6=2�θ���ʧ��(�˴�/����)
//ll7=3�θ���ʧ��(�˴�/����)
//ll8=4�θ���ʧ��(�˴�/����)
//ll9=��������ʧ��(�˴�/����)


//�����������ͳ��
#define	TK_STAT_TYPE_MATCH_RELIVE_COST			10201
//nType2=ProductID
//nType3=1��ң�2��Ʒ
//nType4=WareID
//nType5=GameID ������wuqy 2022.11.1��
//ll0=����ʹ�ý����
//ll1=����ʹ����Ʒ��


//�������񸴻�ͳ�ƣ����̣�
#define	TK_STAT_TYPE_MATCH_TASKRELIVE				10202
//nType2=ProductID
//ll0=���񸴻���ִ���(�˴�/����)
//ll1=�û�ѡ�����񸴻�(�˴�/����)
//ll2=���񸴻�ɹ�����(�˴�/����)


//�������񸴻����ͳ�ƣ����̣�
#define	TK_STAT_TYPE_MATCH_TASKRELIVE_COST			10203
//nType2=ProductID
//nType3=GameID
//nType4=1��ң�2��Ʒ
//nType5=WareID
//ll0=����ʹ�ý����
//ll1=����ʹ����Ʒ��
//ll2=����ʹ�ò���������
//ll3=��ý������


//������Ѫͳ��
#define	TK_STAT_TYPE_MATCH_HEMATINIC			10300
//nType2=ProductID
//nType3=GameID ������wuqy 2022.11.1��
//ll0=1���ɹ�(�˴�/����)
//ll1=2���ɹ�(�˴�/����)
//ll2=3���ɹ�(�˴�/����)
//ll3=4���ɹ�(�˴�/����)
//ll4=�������ɹ�(�˴�/����)
//ll5=1��ʧ��(�˴�/����)
//ll6=2��ʧ��(�˴�/����)
//ll7=3��ʧ��(�˴�/����)
//ll8=4��ʧ��(�˴�/����)
//ll9=������ʧ��(�˴�/����)


//������Ѫ����ͳ��
#define	TK_STAT_TYPE_MATCH_HEMATINIC_COST		10301
//nType2=ProductID
//nType3=1��ң�2��Ʒ
//nType4=WareID
//nType5=GameID ������wuqy 2022.11.1��
//ll0=��Ѫʹ�ý����
//ll1=��Ѫʹ����Ʒ��

//��������Ѫͳ��
#define	TK_STAT_TYPE_MATCH_FREE_TABLE_HP		10302
//nType2=ProductID
//nType3=GameID
//ll0=����
//ll1=���Ľ����
//ll2=��Ѫ��

//������������������֧ͳ��
#define	TK_STAT_TYPE_MATCH_BREAKHEART_TASK		10303
//nType2=ProductID
//nType3=GameID
//nType4=WareID
//ll0=��������������
//ll1=���ѽ������
//ll2=��ý������
//ll3=�����Ʒ����

#define TK_STAT_TYPE_MATCH_HANDWIN				10304
//nType2=ProductID
//nType3=GameID
//nType4=��Ʒ����
//nType5=��ƷID
//ll0=�����Ľ�Ʒ����
//ll1=�۳��Ľ�Ʒ����

#define TK_STAT_TYPE_MATCH_PROMOTION_AWARD        10305
//nType2=ProductID
//nType3=GameID
//nType4=0
//nType5=0
//ll0=���ŵĽ������
//ll1=���ŵ��￨����

//������ͳ��
#define	TK_STAT_TYPE_MATCH_TABLE				10400
//nType2=ProductID
//nType3=MoneyRuler
//nType4=GameRuler
//ll0=����(����/����)
//ll1=����(�˴�/����)
//ll2=��Ϸ(�˴�/����)
//ll3=��Ϸ����


//���������ͳ��
#define	TK_STAT_TYPE_MATCH_GOLDTABLE			10500
//nType2=ProductID
//nType3=0�û���1������
//nType4=GameID
//ll0=������ҿ۳�
//ll1=�볡��ҿ۳�
//ll2=��Ѫ��ҿ۳�
//ll3=̨�ѽ�ҿ۳�
//ll4=������һض�
//ll5=�ض������ۼ�
//ll6=��Ѫ����
//ll7=��ǰδ�ضҽ��
//ll8=�ڳ�δ�ضҽ��
//ll9=�볡����

//�ʽ�ͳ��
#define	TK_STAT_TYPE_JACKPOT					10600
//nType2=ProductID
//ll0=˰�Ѵ���
//ll1=˰������
//ll2=�̶��ʽ𷢷Ŵ���
//ll3=�̶��ʽ𷢷�����
//ll4=�����ʽ𷢷Ŵ���
//ll5=�����ʽ𷢷�����


//���е��ݲʽ�ͳ��
#define	TK_STAT_TYPE_JACKPOT_CSTUD				10601
//nType2=ProductID
//nType3=�÷�����
//ll0=����
//ll1=��ҵ÷�
//ll2=������


// �Խ�������ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_CREATE		10700
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=�ɹ���������
//ll1=�ɹ���������


// �Խ�������ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_ENTER		10701
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=�ɹ���������
//ll1=�ɹ���������


// �Խ�������ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_GAMEBEGIN		10702
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=�ɹ���������
//ll1=δ��������
//ll2=��������Ϊ��δ����������


// �Խ�����������ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_SITDOWN		10703
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=��������
//ll1=�����ܴ���
//ll2=����������
//ll3=2����������
//ll4=3����������
//ll5=4��������������
//ll6=�Թ�������ÿ����
//ll7=��;�������������


// �Խ�������ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_PERMITSET		10704
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=�������ش�����
//ll1=�������عر�����
//ll2=����ͬ����Ҿ�������
//ll3=�����ܾ���Ҿ�������
//ll4=���������Ĵ���


// �Խ�����������ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_PLAYER_COUNT		10705
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=����<3�˵�����
//ll1=����3-5�˵�����
//ll2=����5-10�˵�����
//ll3=����>10�˵�����


// �Խ�������ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_KICK		10706
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=�����������
//ll1=�����ò����ٽ�������


// �Խ����ض�ͳ��
#define	TK_STAT_TYPE_MATCH_FD_TABLE_GOLD		10707
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=�����ܽ��
//ll1=����������
//ll2=�ض��ܽ��

#define TK_STAT_TYPE_MATCH_WAITTIME				10708
//nType2=ProductID
//nType3=0�û���1������
//nType4=GameID
//ll0=0-5s�ȴ�����
//ll1=6-15s�ȴ�����
//ll2=16-30s�ȴ�����
//ll3=31-60s�ȴ�����
//ll4=61s������ʱ��ȴ�����

// �Խ���˽�������ͳ��
#define TK_STAT_TYPE_MATCH_FD_TABLE_PRIVATE_PLACE 10709
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��
//ll0=����˽�ܵ���������
//ll1=�ر�˽�ܵ���������

// �Խ����Ƽ��������ͳ��
#define TK_STAT_TYPE_MATCH_FD_TABLE_RECOMMEND 10710
//nType2=ProductID
//nType3=GameID
//ll0=�����Ƽ�����
//ll1=�����Ƽ�����
//ll2=�ɹ����뵽������
//ll3=�ɹ����뵽�Ĵ���

// �Խ�����ǰ������ͳ��
#define TK_STAT_TYPE_MATCH_FD_TABLE_PREGAEMEND 10711
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��/����
//ll0=����������ɢδ��������(��������Ϊ)
//ll1=����������ɢ�ѿ�������
//ll2=����������ɢ������
//ll3=�Զ���ɢ������

// ���ཨ��ͳ��
#define TK_STAT_TYPE_MATCH_FD_TABLE_DOLPHIN 10712
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��/����
//ll0=ͨ�����ഴ��������
//ll1=ͨ�����ഴ���������п�����
//ll2=ͨ����������Խ����������

//AI��������ͳ��
#define TK_STAT_TYPE_MATCH_AIJOIN 10713
//nType2=ProductID
//nType3=GameID
//nType4=�ִ�
//ll0=nTpye4�ִμ����AI����

// Լս�ҽ���ͳ��
#define TK_STAT_TYPE_MATCH_FD_TABLE_INVIETE 10714
//nType2=ProductID
//nType3=GameID
//nType4=����
//nType5=ʱ��/����
//ll0=ͨ��Լս�Ҵ���������
//ll1=ͨ��Լս�Ҵ����������п�����
//ll2=ͨ��Լս�Ҽ����Խ����������

//���������ͳ������
#define TK_STAT_TYPE_MATCH_CHAPTER 10800
//nType2=ProductID
//nType3=GameID
//nType4=����
//ll0=�����˴�
//ll1=���طǶ����뿪�˴�
//ll2=���ض����뿪�˴�
//ll3=�ش��˴�
//ll4=�ش�Ƕ����뿪�˴�
//ll5=�ش�����뿪�˴�

//�￨�ض�ͳ��
#define	TK_STAT_TYPE_MATCH_QKTABLE			10900
//nType2=ProductID
//nType3=0�û���1������
//nType4=GameID
//ll0=�����￨�ض�

//��Ϸ��Ӯ���˴�ͳ��
#define	TK_STAT_TYPE_GAME_WINLOSS				20000
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//ll0=����
//ll1=Win
//ll2=Loss
//ll3=Draw
//ll4=WinAfterTax
//ll5=LossAfterTax
//ll6=DrawAfterTax

//��Ϸʱ��ͳ��
#define	TK_STAT_TYPE_GAME_TIME					20001
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//ll0=��Ϸ��-����(ϵͳ�ӽǣ��޷�������)
//ll1=��Ϸ��-ʱ��(ϵͳ�ӽǣ��޷�������)
//ll2=�쳣��Ϸ��-����(ϵͳ�ӽǣ��޷�������)
//ll3=�쳣��Ϸ��-ʱ��(ϵͳ�ӽǣ��޷�������)
//ll4=�û�ʱ��(ϵͳ�ӽǣ��޷������ݣ�=��Ϸʱ��*��������)
//ll5=�û��쳣ʱ��(ϵͳ�ӽǣ��޷������ݣ�=�쳣��Ϸʱ��*��������)
//ll6=������(�û��ӽǣ��ɷ���һ��3����Ϸ��������Ϊ3���з���ͳ��)
//ll7=��ʱ��(�û��ӽǣ��ɷ���һ��3��60s����Ϸ����ʱ��Ϊ180s��ÿ����ҵ���ʱ����Ϊ60s���з���ͳ��)
//ll8=�쳣���������û��ӽǣ��ɷ���...��
//ll9=�쳣��ʱ�����û��ӽǣ��ɷ���...��

//Poker˰��ͳ��
#define	TK_STAT_TYPE_POKER_TAX					20002

//��Ϸ˰�պ���ˮͳ��
#define	TK_STAT_TYPE_GAME_TAX					20003

//��Ϸ�û���Ϊͳ��
#define	TK_STAT_TYPE_GAME_ACTION				20004
//nType2=GameID

//������������ʱͳ��
#define	TK_STAT_TYPE_GAME_LORDTAKEOUTDELAY		20005
//nType2=SrvPort
//nType3=SubType
//ll0=0ms
//ll1=1ms
//ll2=2-3ms
//ll3=4-7ms
//ll4=8-15ms
//ll5=16-31ms
//ll6=32-63ms
//ll7=64-127ms
//ll8=128-255ms
//ll9=>=256ms

//WareDBExecDelayͳ��
#define	TK_STAT_TYPE_DBEXECDELAY_WARESRV		20006
//nType2=nExecWhere
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms

//DBExecDelay����ͳ��
#define	TK_STAT_TYPE_DBEXECDELAY				20007
//DBExecDelayʱ��ͳ��
#define	TK_STAT_TYPE_DBEXECDELAY_TICK			20008
//nType2=SrvPort
//nType3=nExecWhere
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms

//����������½Ack��ʱͳ��
#define	TK_STAT_TYPE_LOBBY_ANONY		20009
//nType2=SrvPort
//nType3=SubType
//ll0=0ms
//ll1=1ms
//ll2=2-3ms
//ll3=4-7ms
//ll4=8-15ms
//ll5=16-31ms
//ll6=32-63ms
//ll7=64-127ms
//ll8=128-255ms
//ll9=>=256ms


//LoginDelay����ͳ��
#define	TK_STAT_TYPE_LOGINDELAY				20010
//LoginDelayʱ��ͳ��
#define	TK_STAT_TYPE_LOGINDELAY_TICK			20011
//nType2=SrvPort
//nType3=nExecWhere
//nType4=iCurHour
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms


//PartnerLoginDelay����ͳ��
#define	TK_STAT_TYPE_PARTNERLOGINDELAY				20012
//PartnerLoginDelayʱ��ͳ��
#define	TK_STAT_TYPE_PARTNERLOGINDELAY_TICK			20013
//nType2=SrvPort
//nType3=dwPartnerID
//nType4=iCurHour
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms


//CPSendMsgDelay����ͳ��
#define	TK_STAT_TYPE_CPSENDMSGDELAY					20014
//CPSendMsgDelayʱ��ͳ��
#define	TK_STAT_TYPE_CPSENDMSGDELAY_TICK			20015
//nType2=OrgType
//nType3=DstPort
//nType4=dwMsgType
//nType5=iCurHour
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms

//DBExecBlock����ͳ��
#define	TK_STAT_TYPE_DBEXECBLOCK				20016
//DBExecBlockʱ��ͳ��
#define	TK_STAT_TYPE_DBEXECBLOCK_TICK			20017
//nType2=SrvPort
//nType3=nExecWhere
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms

//DBExecError����ͳ��
#define	TK_STAT_TYPE_DBEXECERROR				20018
//nType2=SrvPort
//nType3=nExecWhere

//����Loing/NoReg��½Ack��ʱͳ��
#define	TK_STAT_TYPE_LOBBY_LOGIN		20019
#define	TK_STAT_TYPE_LOBBY_NOREG		20020
//nType2=SrvPort
//nType3=SubType
//ll0=0ms
//ll1=1ms
//ll2=2-3ms
//ll3=4-7ms
//ll4=8-15ms
//ll5=16-31ms
//ll6=32-63ms
//ll7=64-127ms
//ll8=128-255ms
//ll9=>=256ms

//���������������Ϣ��ʱͳ��
#define	TK_STAT_TYPE_GAME_LORDWAITRESTDELAY		20021
//nType2=SrvPort
//nType3=SubType
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms


//��Ϸ��ˮ�ͻ���ͳ��
#define	TK_STAT_TYPE_GAME_FLOW					20022


//���㱶����ͳ��
#define	TK_STAT_TYPE_FISH_MULT					20023
//nType2=GameID
//nType3=ProductID
//nType4=Mult
//nType5=FishID
//ll0=���ִ���
//ll1=�������
//ll2=��������


//���㱦��ͳ��
#define	TK_STAT_TYPE_FISH_TREAS					20024
//nType2=GameID
//nType3=ProductID
//nType4=Mult
//ll0=���ִ���
//ll1=
//ll2=�����������
//ll3=������Ʒ����


//��������ͳ��
#define	TK_STAT_TYPE_FISH_TASK					20025
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=TaskID
//ll0=���ִ���
//ll1=��ɴ���
//ll2=�����������
//ll3=������Ʒ����


//������Ʒ��ͳ��
#define	TK_STAT_TYPE_FISH_WARE					20026
//nType2=GameID
//nType3=ProductID
//nType4=WareID
//nType5=FishID
//ll0=���ִ���
//ll1=�������
//ll2=��Ʒ����


//Boss��ͳ��
#define	TK_STAT_TYPE_FISH_BOSS					20027
//nType2=GameID
//nType3=ProductID
//nType4=BossID
//nType5=
//ll0=���ִ���
//ll1=�������
//ll2=���н��������
//ll3=�����������
//ll4=��Ʒ����


//������ͳ��
#define	TK_STAT_TYPE_FISH_DIS					20028
//nType2=GameID
//nType3=ProductID
//nType4=BossID
//nType5=
//ll0=���ִ���
//ll1=�������
//ll2=���н��������
//ll3=�����������
//ll4=��Ʒ����

//JJ������̨�ȼ���ˮͳ��
#define	TK_STAT_TYPE_FISH_FIRE_FLOW				20029
//nType2=GameID
//nType3=ProductID
//nType4=Level
//nType5=FireType
//ll0=���ڴ���
//ll1=������ˮ

#define TK_STAT_TYPE_JJFISH_SUBSIDY				20030
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll1=�ͻ�������Ĵ���
//ll2=���������pivot���͵��������
//ll3=�ɹ��Ĵ���

//JJ����������̨�ȼ���ˮͳ��
#define TK_STAT_TYPE_FISH_LOCKGEMPROP_FIRE_FLOW 20031
//nType2=GameID
//nType3=ProductID
//nType4=Level
//ll0=���ڴ���
//ll1=������ˮ

//JJ���㱦ʯ����ʹ�ô���ͳ��
#define TK_STAT_TYPE_FISH_GEMPROP_USECNT		20032
//nType2=GameID
//nType3=ProductID
//110=����ʹ�ô���
//111=�������ĵļ���ʯ����
//112=����ʹ�ô���
//113=�������ĵļ���ʯ����
//114=�ٻ�ʹ�ô���
//115=�ٻ����ĵļ���ʯ����

// JJ����������ˮ����ͳ��
#define TK_STAT_TYPE_FISH_WATER_FEEDBACK		20033
//nType2=GameID
//nType3=ProductID
//nType4=Type 
//nType5=
//ll0=��ˮ
//ll1=����

//ǧ�ڳ齱ͳ��
#define	TK_STAT_TYPE_QPFISH_DRAW				20050
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=Ͷע����
//ll1=Ͷע���
//ll2=�ʳؽ�������
//ll3=�ʳؽ������
//ll4=�̶���1����
//ll5=�̶���1���
//ll6=�̶���2����
//ll7=�̶���2���

//ǧ����������ͳ��
#define TK_STAT_TYPE_FISH_ENERGYSKILL			20051
//nType2=GameID
//nType3=ProductID
//nType4=SkillID
//nType5=
//ll0=ʹ�ô���
//ll1=�����������
//ll2=�����ջ����

//��Ʒ����ͳ��
#define TK_STAT_TYPE_FISH_GOODSCOST				20052
//nType2=GameID
//nType3=ProductID
//nType4=GoodsID
//nType5=
//ll0=��������
//ll1=����������
//ll2=ʹ�ÿ�������
//ll3=ʹ������������
//ll4=ʹ�ñ���������
//ll5=ʹ���ٻ�������

//ǧ����������ͳ��(��)
#define TK_STAT_TYPE_FISH_ENERGYSKILL_NEW		20053
//nType2=ProductID
//nType3=SkillID
//nType4=SkillLevel
//nType5=CannonLevel
//ll0=ʹ�ô���
//ll1=�����������
//ll2=�����ջ����

//ǧ�ڲ�����ͳ��
#define TK_STAT_TYPE_FISH_MAMMONFISH		20054
//nType2=GameID
//nType3=ProductID
//nType4=FishID
//nType5=Stage
//ll0=���ִ���
//ll1=�������
//ll2=�����ڵ���
//ll3=���������
//ll4=�������

//ǧ�ڶᱦ����ͳ��
#define TK_STAT_TYPE_FISH_LOTTERY_COUNT		20055
//nType2=ProductID
//nType3=LotteryID
//nType4=
//nType5=
//ll0=��������
//ll1=�����˴�

//ǧ�ڱ���ʹ�ô���ͳ��
#define TK_STAT_TYPE_FISH_EMOTE_COUNT   20056
//nType2=GameID
//nType3=ProductID
//nType4=EmoteID
//nType5=
//ll0=ʹ�ô���
//ll1=������ʯ��

//��Ϸʱ��ͳ��
#define TK_STAT_TYPE_GAME_TIME_EX 20057
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//nType4=PlatID
//nType5=LanguageID
//ll0=���������
//ll1=������쳣����

//ǧ�ں���������ͳ��
#define TK_STAT_TYPE_FISH_LOTTERY_AWARD 20058
//nType2=ProductID
//nType3=LotteryID
//ll0=��ý������
//ll1=�����ʯ����
//ll2=����￨����

//ը����ͳ��
#define	TK_STAT_TYPE_BOMBMAN				20060

//////////////////////////////////////////////////////////////////////////
// JJ���˷����ͳ��
// ���ڿ���ͳ��
#define TK_STAT_TYPE_HUNTER_FINANCECONTROL			20069
//nType2=GameID
//nType3=ProductID
//ll0=�������
//ll1=���𲹳����
//ll2=ӯ������
//ll3=ӯ���۳����

//Bossͳ��
#define TK_STAT_TYPE_HUNTER_BOSS			20070
//nType2=GameID
//nType3=ProductID
//nType4=MonsterKindID
//nType5=
//ll0=���ִ���
//ll1=�������
//ll2=�Ƽ����Ա�
//ll3=�Ƽ��￨
//ll4=�������Ա�
//ll5=�����￨
//ll6=�����ӵ�


//��Ӣ��ͳ��
#define TK_STAT_TYPE_HUNTER_ELITE			20071
//nType2=GameID
//nType3=ProductID
//nType4=MonsterKindID
//nType5=Multi
//ll0=���ִ���
//ll1=�������
//ll2=�����ӵ�
//ll3=�������Ա�
//114=����Ͷע���Ա�



//�����ͳ��
#define TK_STAT_TYPE_HUNTER_SPECIAL			20072
//nType2=GameID
//nType3=ProductID
//nType4=MonsterType
//nType5=
//ll0=���ִ���
//ll1=�������
//ll2=�������Ա�
//ll3=�����￨
//ll4=�����ӵ�


//��������ͳ��
#define TK_STAT_TYPE_HUNTER_TASK			20073
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=��������
//ll1=����1��ɴ���
//ll2=����2��ɴ���
//ll3=�佱����
//ll4=�����˴�
//ll5=����˴�
//ll6=����S�˴�
//ll7=����A�˴�
//ll8=����B�˴�
//ll9=����C�˴�


//����������ͳ��
#define TK_STAT_TYPE_HUNTER_TASK_AWARD		20074
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=��ʯ����
//ll1=ˮ������
//ll2=�¹�ʯ����
//ll3=�⾧ʯ����


//��Ʒ����ͳ��
#define TK_STAT_TYPE_HUNTER_COSTGOODS		20075
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=�������ߴ���
//ll1=����������ʯ
//ll2=�ٻ����ߴ���
//ll3=�ٻ�������ʯ
//ll4=�񱩵��ߴ���
//ll5=��������ʯ
//ll6=������ߴ���
//ll7=����������ʯ
//ll8=����������ʯ


//��Ʒ����ͳ��
#define TK_STAT_TYPE_HUNTER_GETGOODS		20076
//nType2=GameID
//nType3=ProductID
//nType4=GoodsID
//nType5=
//ll0=�������
//ll1=��������
//ll2=�����Ʒ�ܴ���
//ll3=�����Ʒ����


//��������ͳ��
#define TK_STAT_TYPE_HUNTER_SKILL			20077
//nType2=GameID
//nType3=ProductID
//nType4=SkillID
//nType5=
//ll0=ʹ�ô���
//ll1=�������
//ll2=������


//������ͳ��
#define TK_STAT_TYPE_HUNTER_PLAYER			20078
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=��������
//ll1=�������
//ll2=����ʹ�ô���
//ll3=�ɹ���ȡ�Ʋ���������

// ��ˮͳ��
#define TK_STAT_TYPE_HUNTER_FLOW			20079
//nType2=GameID
//nType3=ProductID
//nType4=FireLevel
//nType5=FireType
//ll0=���ڴ���
//ll1=������ˮ
//////////////////////////////////////////////////////////////////////////

// Sit&Go�������ս���û������ɹ���������ʱ��ͳ��
#define TK_STAT_TYPE_USERMATCHINGTIME		20080
//nType2=MPID
//nType3=GameID
//ll0=ƥ������ۼ�
//ll1=ƥ��ʱ���ۼ�

// ������������ʤ��ͳ��
#define TK_STAT_TYPE_LORD_BOTAI  20090
//nType2=GameID
//nType3=ProductID
//ll0=�����˲�������
//ll1=������ʤ����
//ll2=�����˵�������
//ll3=�����˵���ʤ����

//��������ͳ��
#define	TK_STAT_TYPE_PACER_BOT_PLAYER	20091
//nType2=ProductID
//nType3=GameID
//ll0=�������˽������(�˴�/����)
//ll1=���������뿪����(�˴�/����)

// ��������Ϸ�Ծֽ��ͳ�ƣ���Ϸ���������ʣ�
#define  TK_STAT_TYPE_AI_ROUNDRESULT	20092 
//nType2=GameId
//nType3=ProductID
//nType4=Character  ���������: 1~�ʵ�, 2~����, 3~����, 4~������
//ll0=�����ܾ���
//ll1=ʤ����
//ll2=ƽ����
//ll3=�ܾ���

//����������ͳ��
#define	TK_STAT_TYPE_PERF_ERROR				20100
#define	TK_STAT_TYPE_PERF_BLOCKCOUNT		20200
#define	TK_STAT_TYPE_PERF_BLOCKDELAY		20300
#define	TK_STAT_TYPE_PERF_EXECCOUNT			20400
#define	TK_STAT_TYPE_PERF_EXECDELAY			20500
//nType2=wNodePort
//nType3=wType
//nType4=wParam
//nType5=dwIFID
//ll0=0-9ms
//ll1=10-19ms
//ll2=20-30ms
//ll3=40-70ms
//ll4=80-150ms
//ll5=160-310ms
//ll6=320-630ms
//ll7=640-1270ms
//ll8=1280-2550ms
//ll9=>=2560ms


//���Թ�ͳ��
#define	TK_STAT_TYPE_BROADCAST					21000
//nType2=ProductID
//nType3=GameID
//ll0=���Թ��˴�/����


//����������¼IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_ANONYMOUS	30000
//ll0=�ɹ�
//ll1=�ѵ�¼
//ll2=�汾̫��
//ll3=�汾̫��
//114=����ʱ�Ͽ�
//115=�����첽����


//�����û���¼IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_USER			30001
//ll0=�ɹ�(�˴�/����)
//ll1=�ѵ�¼
//ll2=δ������¼
//ll3=DS�ܾ�(�˴�/����)
//ll4=LS��ת
//ll5=���ڱ𴦵�¼
//ll6=�߳�ʧ��
//ll7=��¼ʱ�Ͽ�(�˴�/����)
//ll8=�����첽����
//ll9=�ɹ��߳�(�˴�/����)


//�����û��ǳ�IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGOUTIP_USER		30002
//ll0=�ɹ�(�˴�/����)
//ll1=δ��¼
//ll2=����ɾ��ʧ��


//�����û���¼ʱ��IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_TIME			30003
//nType2=OffLineType(��������)
//ll0=���ۼ�ʱ��
//ll1=С��1����(�˴�/����)
//ll2=С��10����(�˴�/����)
//ll3=С��30����(�˴�/����)
//ll4=С��1Сʱ(�˴�/����)
//ll5=С��2Сʱ(�˴�/����)
//ll6=С��3Сʱ(�˴�/����)
//ll7=С��5Сʱ(�˴�/����)
//ll8=С��8Сʱ(�˴�/����)
//ll9=����8Сʱ(�˴�/����)

//�����û���¼�߿�ͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_KICK		30005
//nType2=PlatID
//ll0=
//ll1=
//ll2=
//ll3=
//ll4=
//ll5=
//ll6=
//ll7=
//ll8=
//ll9=

//����������¼��ƽ̨IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_ANONYMOUS_PLAT	30010
//nType2=PlatID
//ll0=�ɹ�
//ll1=�ѵ�¼
//ll2=�汾̫��
//ll3=�汾̫��
//114=����ʱ�Ͽ�
//115=�����첽����


//�����û���¼��ƽ̨IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_USER_PLAT			30011
//nType2=PlatID
//ll0=�ɹ�(�˴�/����)
//ll1=�ѵ�¼
//ll2=δ������¼
//ll3=DS�ܾ�(�˴�/����)
//ll4=LS��ת
//ll5=���ڱ𴦵�¼
//ll6=�߳�ʧ��
//ll7=��¼ʱ�Ͽ�(�˴�/����)
//ll8=�����첽����
//ll9=�ɹ��߳�(�˴�/����)


//�����û��ǳ���ƽ̨IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGOUTIP_USER_PLAT		30012
//nType2=PlatID
//ll0=�ɹ�(�˴�/����)
//ll1=δ��¼
//ll2=����ɾ��ʧ��


//�����û���¼ʱ����ƽ̨IPͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_TIME_PLAT			30013
//nType2=OffLineType(��������)
//nType3=PlatID
//ll0=���ۼ�ʱ��
//ll1=С��1����(�˴�/����)
//ll2=С��10����(�˴�/����)
//ll3=С��30����(�˴�/����)
//ll4=С��1Сʱ(�˴�/����)
//ll5=С��2Сʱ(�˴�/����)
//ll6=С��3Сʱ(�˴�/����)
//ll7=С��5Сʱ(�˴�/����)
//ll8=С��8Сʱ(�˴�/����)
//ll9=����8Сʱ(�˴�/����)


//�����û���¼��ƽ̨IP�߿�ͳ��
#define TK_STAT_TYPE_LOBBY_LOGINIP_KICK_PLAT	30015
//nType2=PlatID
//ll0=
//ll1=
//ll2=
//ll3=
//ll4=
//ll5=
//ll6=
//ll7=
//ll8=
//ll9=


//#define TK_STAT_TYPE_LOBBY_LOGIN_ANONYMOUS	30100//ռλ


//�����û���¼ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_USER			30101
//ll0=�ɹ�(�˴�/����)
//ll1=�ѵ�¼
//ll2=δ������¼
//ll3=DS�ܾ�(�˴�/����)
//ll4=LS��ת
//ll5=���ڱ𴦵�¼
//ll6=�߳�ʧ��
//ll7=��¼ʱ�Ͽ�(�˴�/����)
//ll8=�����첽����
//ll9=�ɹ��߳�(�˴�/����)


//�����û��ǳ�ͳ��
#define TK_STAT_TYPE_LOBBY_LOGOUT_USER			30102
//ll0=�ɹ�(�˴�/����)
//ll1=δ��¼
//ll2=����ɾ��ʧ��


//�����û���¼ʱ��ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_TIME			30103
//nType2=OffLineType(��������)
//ll0=���ۼ�ʱ��
//ll1=С��1����(�˴�/����)
//ll2=С��10����(�˴�/����)
//ll3=С��30����(�˴�/����)
//ll4=С��1Сʱ(�˴�/����)
//ll5=С��2Сʱ(�˴�/����)
//ll6=С��3Сʱ(�˴�/����)
//ll7=С��5Сʱ(�˴�/����)
//ll8=С��8Сʱ(�˴�/����)
//ll9=����8Сʱ(�˴�/����)

//�����û���¼�ܾ���ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_EXPERIENCE			30104
//ll0=[0, 1000]
//ll1=[1001, 10000]
//ll2=[10001, 20000]
//ll3=[20001, 50000]
//ll4=[50001, 100000]
//ll5=[100001, 200000]
//ll6=[200001, 500000]
//ll7=[500001, 1000000]
//ll8=[1000001 ����)

//�����û���¼�߿�ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_KICK			30105
//nType2=PlatID
//ll0=
//ll1=
//ll2=
//ll3=
//ll4=
//ll5=
//ll6=
//ll7=
//ll8=
//ll9=


//�����û���¼��ƽ̨ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_USER_PLAT			30111
//nType2=PlatID
//ll0=�ɹ�(�˴�/����)
//ll1=�ѵ�¼
//ll2=δ������¼
//ll3=DS�ܾ�(�˴�/����)
//ll4=LS��ת
//ll5=���ڱ𴦵�¼
//ll6=�߳�ʧ��
//ll7=��¼ʱ�Ͽ�(�˴�/����)
//ll8=�����첽����
//ll9=�ɹ��߳�(�˴�/����)


//�����û��ǳ���ƽ̨ͳ��
#define TK_STAT_TYPE_LOBBY_LOGOUT_USER_PLAT			30112
//nType2=PlatID
//ll0=�ɹ�(�˴�/����)
//ll1=δ��¼
//ll2=����ɾ��ʧ��


//�����û���¼ʱ����ƽ̨ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_TIME_PLAT			30113
//nType2=OffLineType(��������)
//nType3=PlatID
//ll0=���ۼ�ʱ��
//ll1=С��1����(�˴�/����)
//ll2=С��10����(�˴�/����)
//ll3=С��30����(�˴�/����)
//ll4=С��1Сʱ(�˴�/����)
//ll5=С��2Сʱ(�˴�/����)
//ll6=С��3Сʱ(�˴�/����)
//ll7=С��5Сʱ(�˴�/����)
//ll8=С��8Сʱ(�˴�/����)
//ll9=����8Сʱ(�˴�/����)

//�����û���¼�ܾ����ƽ̨ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_EXPERIENCE_PLAT			30114
//nType2=PlatID
//ll0=[0, 1000]
//ll1=[1001, 10000]
//ll2=[10001, 20000]
//ll3=[20001, 50000]
//ll4=[50001, 100000]
//ll5=[100001, 200000]
//ll6=[200001, 500000]
//ll7=[500001, 1000000]
//ll8=[1000001 ����)

//�����û���¼��ƽ̨�߿�ͳ��
#define TK_STAT_TYPE_LOBBY_LOGIN_KICK_PLAT	30115
//nType2=PlatID
//ll0=
//ll1=
//ll2=
//ll3=
//ll4=
//ll5=
//ll6=
//ll7=
//ll8=
//ll9=

//��ҳ�û���¼IPͳ��
#define TK_STAT_TYPE_WEB_LOGIN_IP			30201
//ll0=�ɹ�(�˴�/����)
//ll1=����ʧ��(�˴�/����)
//ll2=�������(�˴�/����)
//ll3=�ܾ���¼(�˴�/����)
//ll4=������¼(�˴�/����)
//ll5=��������(�˴�/����)
//ll6=��Ȩʧ��(�˴�/����)

//��ҳ�û���¼ͳ��
#define TK_STAT_TYPE_WEB_LOGIN_USER			30301
//ll0=�ɹ�(�˴�/����)
//ll1=����ʧ��(�˴�/����)
//ll2=�������(�˴�/����)
//ll3=�ܾ���¼(�˴�/����)
//ll4=������¼(�˴�/����)
//ll5=��������(�˴�/����)
//ll6=��Ȩʧ��(�˴�/����)

//��ҳ�û�SSO��¼ͳ��
#define TK_STAT_TYPE_WEB_SSO_LOGIN_USER		30302
//ll0=�ܴ���
//ll1=�ɹ�
//ll2=��ʱ
//ll3=���ȴ���
//ll4=MD5����
//ll5=����ʧ��
//ll6=��������

//��ҳ�û�FPWD��֤ͳ��
#define TK_STAT_TYPE_WEB_VERIFY_FPWD_USER		30303
//ll0=�ܴ���
//ll1=�ɹ�
//ll2=����ʧ��
//ll3=�������
//ll4=UID������
//ll5=����������

//��ҳ�û�SPWD��֤ͳ��
#define TK_STAT_TYPE_WEB_VERIFY_SPWD_USER		30304
//ll0=�ܴ���
//ll1=�ɹ�
//ll2=����ʧ��
//ll3=�������
//ll4=UID������
//ll5=����������

//TGP�û���¼ͳ��
#define TK_STAT_TYPE_TGP_LOGIN_USER			31101
//ll0=�ɹ�(�˴�/����)
//ll1=�ѵ�¼
//ll2=δ������¼
//ll3=DS�ܾ�(�˴�/����)
//ll4=LS��ת
//ll5=���ڱ𴦵�¼
//ll6=�߳�ʧ��
//ll7=��¼ʱ�Ͽ�(�˴�/����)
//ll8=�����첽����
//ll9=�ɹ��߳�(�˴�/����)

//��ί�����ͳ��
#define TK_STAT_TYPE_COMMITTEE_MONEY			40001
//nType2=�٣����
//nType3=ProductID
//nType4=GameID
//ll0=��ҿ۳�����
//ll1=��ҷ��Ŵ���
//ll2=��ȯ�۳�����
//ll3=��ȯ���Ŵ���
//ll4=-���
//ll5=+���
//ll6=-��ȯ
//ll7=+��ȯ
//ll8=-��������
//ll9=+��������


//��ί�����(���˿�)ͳ��
#define TK_STAT_TYPE_COMMITTEE_MONEY_LUCKCARD		40002
//nType2=�٣����
//nType3=ProductID
//nType4=GameID
//ll0=��ҿ۳�����
//ll1=��ҷ��Ŵ���
//ll2=��ȯ�۳�����
//ll3=��ȯ���Ŵ���
//ll4=-���
//ll5=+���
//ll6=-��ȯ
//ll7=+��ȯ
//ll8=-��������
//ll9=+��������

//��ί����ڻ���ת��ͳ��
#define TK_STAT_TYPE_COMMITTEE_TRANSFERMNY		40003
//nType2=ProductID
//nType3=GameID
//nType4=AccTypeSrc
//nType5=AccTypeDst
//ll0=����
//ll1=Coin
//ll2=Bouns
//ll3=Gold
//ll4=Cert

//��ί��Growͳ��
#define TK_STAT_TYPE_COMMITTEE_GROW			40004
//nType2=ProductID
//nType3=GameID
//nType4=GrowID
//ll0=���Ŵ���
//ll1=�۳�����
//ll2=+Value
//ll3=-Value

//��ί��64λGrowͳ��
#define TK_STAT_TYPE_COMMITTEE_GROW_64		40005
//nType2=ProductID
//nType3=GameID
//nType4=GrowID
//ll0=���Ŵ���
//ll1=�۳�����
//ll2=+Value
//ll3=-Value

//��ί�������������ͳ��
#define TK_STAT_TYPE_COMMITTEE_MONEY_OTHER		40099
//nType2=�٣����
//nType3=ProductID
//nType4=GameID
//ll0=��ҿ۳�����
//ll1=��ҷ��Ŵ���
//ll2=��ȯ�۳�����
//ll3=��ȯ���Ŵ���
//ll4=-���
//ll5=+���
//ll6=-��ȯ
//ll7=+��ȯ
//ll8=-��������
//ll9=+��������

//��ί�����ҵ��ͳ��-���ѱ���
#define TK_STAT_TYPE_COMMITTEE_MONEY_EXPRESSION 40100
//nType2=ProductID
//nType3=GameID
//ll0=��������
//ll1=������ս��
//ll2=ʹ������(�˴�/����)


//���Ҷһ�ͳ��
#define TK_STAT_TYPE_DATA_EXCHANGEMNY		50101
//nType2=Place
//nType3=MoneySrcType
//nType4=MoneyDstType
//ll0=����
//ll1=MoneySrc
//ll2=MoneyDst


//��ȯ�ҽ�ͳ��
#define TK_STAT_TYPE_DATA_AWARDCACHIN		50201
//nType2=Place
//ll0=����
//ll1=Bonus


//�ֽ���Ԫ��ͳ��
#define TK_STAT_TYPE_DATA_RMB2COIN			50301
//nType2=Place
//ll0=����
//ll1=Rmb
//ll2=Rmb*RmbPer/100000
//ll3=Coin


//���ֻ���֮��Ķһ�����������һ�ֻ�����JJԪ��
#define TK_STAT_TYPE_PAYIN_BANK					50302//��ֵ����������ͳ��
#define TK_STAT_TYPE_PAYIN_AGENT				50303//֧�������̷���ͳ��
#define TK_STAT_TYPE_PAYIN_ZONE					50304//֧���������ͳ��
#define TK_STAT_TYPE_PAYIN_TYPE					50305//�������ͷ���ͳ��
//nType2=dwSrcMID Դ��������
//nType3=dwDstMID Ŀ���������
//nType4=dwEndMID ����ת���Ļ�������
//nType5=dwPWBankID/dwPWAgentID/dwPWZoneID/dwPWTypeID ��ֵ������/֧��������/֧������/��������
//ll0=����
//ll1=dwSrcAmount Դ��������
//ll2=dwSrcAmount*dwSrcPer/100000 ʵ��
//ll3=dwDstAmount Ŀ���������
//ll4=dwEndAmount ����ת���Ļ�������


//Ԫ��������������ͳ��
#define TK_STAT_TYPE_DATA_COIN2OUT			50310
//nType2=dwOMnyID	�ⲿ����ID
//nType3=dwORegion	�ⲿ����������Ϸ����
//nType4=dwOPayT	�ⲿ��������
//ll0=����
//ll1=Coin			Ԫ������
//ll2=dwOMny		�ⲿ��������
//ll3=Rmb
//ll4=Rmb*RmbPer/100000


//�����������������ͳ��
#define TK_STAT_TYPE_DATA_VPAY				50320
//nType2=AccType	�ʺ�����
//nType3=dwMonyID	�ڲ�����ID
//nType4=dwOMonyID	�ⲿ����ID
//ll0=�������
//ll1=dwMonyMC		�ڲ���������
//ll2=dwIMonyMC		�ⲿ��������
//ll5=��������
//ll6=dwMonyMC		�ڲ���������
//ll7=dwOMonyMC		�ⲿ��������


//VIPȯ���������ͳ��
#define TK_STAT_TYPE_DATA_VIPCERT2MNY		50401
//nType2=Place
//nType3=VipType
//nType4=VipCertType
//ll0=����
//ll1=VipCertAmount
//ll2=Other
//ll3=Coin
//ll4=Bouns
//ll5=Gold
//ll6=Cert


//������תͳ��
#define TK_STAT_TYPE_DATA_CHARGE			50501
//nType2=Place
//nType3=Type(��,eDicMnysTradeEntryType)
//nType4=SubType
//ll0=����
//ll1=+Coin
//ll2=-Coin
//ll3=+Bouns
//ll4=-Bouns
//ll5=+Gold
//ll6=-Gold
//ll7=+Cert
//ll8=-Cert


//����ת��ͳ��
#define TK_STAT_TYPE_DATA_TRANSFERMNY		50601
//nType2=SubType
//nType3=AccTypeSrc
//nType4=AccTypeDst
//ll0=����
//ll1=Coin
//ll2=Bouns
//ll3=Gold
//ll4=Cert


//��Ʒͳ��
#define TK_STAT_TYPE_WARE					60001
//nType2=1��������2����վҵ��
//nType3=SubType(����ID��ҵ��ID)
//nType4=WareID
//ll0=���Ӵ���
//ll1=��������
//ll2=�ۼ�����
//ll3=�ۼ�����


//�����û���Ϊͳ��
#define TK_STAT_TYPE_LOBBY_USERACT			70001
//nType2=���ܺ�
//nType3=��Ϊ����
//nType4=�û�����
//ll0=����
//ll1=����

//��������ͳ��
#define TK_STAT_TYPE_UPDATE			70002

//������ͳ��
#define TK_STAT_TYPE_CHAT			70003


//���ע����Ϣ��֤ͳ��(��¼�����ǳ�)
#define TK_STAT_TYPE_REGINFOCHECK					80001
//nType2=1����վ	2������		3:����
//nType3=1����¼��	2���ǳ�
//ll0=	���������
//ll1=	�����������쳣
//ll2=	Υ���������
//ll3=	�����Ƿ��ؼ���
//ll4=	�Ѿ�����


//���ע�����ͳ��
#define TK_STAT_TYPE_REGISTER					80002
//nType2=1����վ	2������		3������
//ll0=	���������
//ll1=	�����������쳣
//ll2=	��¼������
//ll3=	��¼���Ѿ�����
//ll4=	�ǳƴ���
//ll5=	�ǳ��Ѿ�����
//ll6=	�����ʽ�Ƿ�
//ll7=	��֤�����
//ll8=	ͬIPע������Ƶ��̫���̫��


//Sns��Ծͳ��
#define TK_STAT_TYPE_SNSACTSCORE					90000
//ll0=	�ǻ�ԾȺ
//ll1=	��ԾȺ1
//ll2=	��ԾȺ2-50
//ll3=	��ԾȺ>50
//ll5=	�ǻ�Ծ����
//ll6=	��Ծ����1
//ll7=	��Ծ����2-50
//ll8=	��Ծ����>50

//Sns����ͳ��
#define TK_STAT_TYPE_SNSCHATMSG					90001
//ll0=	˽���������
//ll1=	��֯�������
//ll2=	�Ӻ��Ѵ���
//ll3=	�ӹ�ע����
//ll5=	Ⱥ�������
//ll6=	�����������

//Sns��ϵ��¼�û���ͳ��
#define TK_STAT_TYPE_SNSLogin				90002
//ll0=	��¼�û���
//ll1=	���ѹ�ϵ��¼�û���
//ll2=	��ע��ϵ��¼�û���
//ll3=	�����ϵ��¼�û���
//ll4=	Ⱥ��ϵ��¼�û���
//ll5=	���ɹ�ϵ��¼�û���

//Sns��ϵͳ��
#define TK_STAT_TYPE_SNS			90003
//nType2=1��������


//������ͳ��
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=�����Ӳ���������/�˴Σ�
//ll1=������Ӳ���������/�˴Σ�
//ll2=�������飨����/�˴Σ�
//ll3=��ɢ���飨����/�˴Σ�
#define TK_STAT_TYPE_MATCH_TEAM			90004

//DAG˫�˰೵ͳ��
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=���˱���������/�˴Σ�
//ll1=���ƥ�䣨����/�˴Σ�
//ll2=��ӱ���������/�˴Σ�
#define TK_STAT_TYPE_DOUBLE_LADDER     90005

//�ڳ�ҵ��ͳ��
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=�����ڳ����/�˴Σ�
//ll1=ȡ���ڳ����/�˴Σ�
//ll2=�����ڳ����/�˴Σ�
#define TK_STAT_TYPE_CROWD_FUNDING     90006


//����������ͳ��
#define	TK_STAT_TYPE_MATCH_GOLDTABLE_GROW	90007
//nType2=ProductID
//nType3=0�û���1������
//nType4=GameID
//nType5=GrowID
//ll0=�������ֿ۳�
//ll1=�볡���ֿ۳�
//ll2=��Ѫ���ֿ۳�
//ll3=̨�ѻ��ֿ۳�
//ll4=�������ֻض�
//ll5=�ض������ۼ�
//ll6=��Ѫ����
//ll7=��ǰδ�ضһ���
//ll8=�ڳ�δ�ضһ���
//ll9=�볡����

//������Ѫ����ͳ��(����)
#define	TK_STAT_TYPE_MATCH_HEMATINIC_COST_GROW	90008
//nType2=ProductID
//nType3=GrowID
//nType4=GameID ������wuqy 2022.11.1��
//ll0=��Ѫʹ�û�����

//�����������ͳ��
#define	TK_STAT_TYPE_MATCH_RELIVE_COST_GROW		90009
//nType2=ProductID
//nType3=GrowID
//nType4=GameID ������wuqy 2022.11.1��
//ll0=����ʹ�û�����ֵ

//�����ض�����
#define TK_STAT_TYPE_MUST_PROMOTE               90010
//nType2=ProductID
//nType3=StageID
//nType4=BoutID
//ll0=����̭������/�˴Σ�
//ll1=�����ض�����������/�˴Σ�

//��Ϸ�и������ͳ��
#define	TK_STAT_TYPE_MATCH_GAMING_RELIVE_COST		90011
//nType2=ProductID
//nType3=���λ
//nType4=��Ȩ�ȼ�����
//nType5=GameID
//ll0=�������������/�˴Σ�
//ll1=�������ֵ
//ll2=���������ֵ

//����ڳ�ҵ��ͳ��
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=��ӷ����ڳ����/�˴Σ�
//ll1=���ȡ���ڳ����/�˴Σ�
//ll2=��ӹ����ڳ����/�˴Σ�
#define TK_STAT_TYPE_DOUBLE_CROWD_FUNDING     90012

//WBSͳ��
#define	TK_STAT_TYPE_WBS_BEGIN				100000
#define	TK_STAT_TYPE_WBS_END				199999



#endif  //__STATTYPEDEFINE__INCLUDED__