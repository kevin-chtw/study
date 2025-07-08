
#if !defined(__STATTYPEDEFINE__INCLUDED__)
#define __STATTYPEDEFINE__INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//网速测试统计
#define	TK_STAT_TYPE_NTV						9000
//nType2=AreaID
//nType3=IdcID
//nType4=服务器与中心服务器间是能否连通
//nType5=客户端与服务器间是否能连通
//ll0=预估网速值
//ll1=10倍平均延时(ms)
//ll2=丢包数
//ll3=发包数


//防刷反外挂封号统计
#define	TK_STAT_TYPE_LOBBYTRACECLOSE			9100
//ll0=人次
//ll1=时长
//ll2=免费报名次数
//ll3=总报名次数
//ll4=金币余额
//ll5=金币流水+
//ll6=金币流水-
//ll7=金币初值

//高危积分用户统计
#define	TK_STAT_TYPE_HIGHDANGEROUSUSER			9110
//nType2=PlatID
//ll0=登录人次
//ll1=增加人次
//ll2=减少人次
//ll3=成功报名次数
//ll4=拒绝报名次数


//比赛开赛统计
#define	TK_STAT_TYPE_MATCH						10000
//nType2=ProductID
//ll0=举办场次
//ll1=结束场次
//ll2=取消场次
//ll3=阶段数
//ll4=比赛-局制数(系统视角，无分域数据)
//ll5=比赛-局盘数(系统视角，无分域数据)
//ll6=比赛-用户耗时（系统视角，无分域数据，所以参赛者耗时累计）
//ll7=用户可得经验(只针对岛屿或自由桌，离岛时发放，可分域)


//比赛人员统计
#define	TK_STAT_TYPE_MATCH_PLAYER				10100
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//nType5=LanguageID
//ll0=报名(人次/人数)
//ll1=取消报名(人次/人数)
//ll2=比赛参与(人次/人数)
//ll3=比赛取消(人次/人数)
//ll4=开赛时退费(人次/人数)
//ll5=开赛后退费(人次/人数)
//ll6=复活(人次/人数)
//ll7=加血(人次/人数)

//离岛统计
#define	TK_STAT_TYPE_MATCH_LEAVEISLAND			10101
//nType2=ProductID
//nType3=1主动离岛，2被动离岛
//ll0=小于等于1盘(人次/人数)
//ll1=小于等于2盘(人次/人数)
//ll2=小于等于5盘(人次/人数)
//ll3=小于等于10盘(人次/人数)
//ll4=大于10盘(人次/人数)
//ll5=小于等于1分钟(人次/人数)
//ll6=小于等于5分钟(人次/人数)
//ll7=小于等于30分钟(人次/人数)
//ll8=小于等于60分钟(人次/人数)
//ll9=大于60分钟(人次/人数)


//比赛进入断线和离开统计
#define	TK_STAT_TYPE_MATCH_ENTERBREAKEXIT		10102
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//ll0=进入比赛
//ll1=直退比赛
//ll2=托管
//ll3=断线
//ll4=断线重回
//ll5=踢开比赛
//ll6=疑似断线
//ll7=正常离开
//ll8=
//ll9=


//比赛复活统计
#define	TK_STAT_TYPE_MATCH_RELIVE				10200
//nType2=ProductID
//nType3=GameID 新增（wuqy 2022.11.1）
//ll0=1次复活成功(人次/人数)
//ll1=2次复活成功(人次/人数)
//ll2=3次复活成功(人次/人数)
//ll3=4复活成功(人次/人数)
//ll4=其他次复活成功(人次/人数)
//ll5=1次复活失败(人次/人数)
//ll6=2次复活失败(人次/人数)
//ll7=3次复活失败(人次/人数)
//ll8=4次复活失败(人次/人数)
//ll9=其他复活失败(人次/人数)


//比赛复活费用统计
#define	TK_STAT_TYPE_MATCH_RELIVE_COST			10201
//nType2=ProductID
//nType3=1金币，2物品
//nType4=WareID
//nType5=GameID 新增（wuqy 2022.11.1）
//ll0=复活使用金币数
//ll1=复活使用物品数


//比赛任务复活统计（海捞）
#define	TK_STAT_TYPE_MATCH_TASKRELIVE				10202
//nType2=ProductID
//ll0=任务复活出现次数(人次/人数)
//ll1=用户选择任务复活(人次/人数)
//ll2=任务复活成功次数(人次/人数)


//比赛任务复活费用统计（海捞）
#define	TK_STAT_TYPE_MATCH_TASKRELIVE_COST			10203
//nType2=ProductID
//nType3=GameID
//nType4=1金币，2物品
//nType5=WareID
//ll0=复活使用金币数
//ll1=复活使用物品数
//ll2=复活使用参赛积分数
//ll3=获得金币数量


//比赛加血统计
#define	TK_STAT_TYPE_MATCH_HEMATINIC			10300
//nType2=ProductID
//nType3=GameID 新增（wuqy 2022.11.1）
//ll0=1档成功(人次/人数)
//ll1=2档成功(人次/人数)
//ll2=3档成功(人次/人数)
//ll3=4档成功(人次/人数)
//ll4=其他档成功(人次/人数)
//ll5=1档失败(人次/人数)
//ll6=2档失败(人次/人数)
//ll7=3档失败(人次/人数)
//ll8=4档失败(人次/人数)
//ll9=其他档失败(人次/人数)


//比赛加血费用统计
#define	TK_STAT_TYPE_MATCH_HEMATINIC_COST		10301
//nType2=ProductID
//nType3=1金币，2物品
//nType4=WareID
//nType5=GameID 新增（wuqy 2022.11.1）
//ll0=加血使用金币数
//ll1=加血使用物品数

//自由桌加血统计
#define	TK_STAT_TYPE_MATCH_FREE_TABLE_HP		10302
//nType2=ProductID
//nType3=GameID
//ll0=次数
//ll1=消耗金币数
//ll2=加血量

//碎心赛触发次数及收支统计
#define	TK_STAT_TYPE_MATCH_BREAKHEART_TASK		10303
//nType2=ProductID
//nType3=GameID
//nType4=WareID
//ll0=参与碎心赛次数
//ll1=花费金币数量
//ll2=获得金币数量
//ll3=获得物品数量

#define TK_STAT_TYPE_MATCH_HANDWIN				10304
//nType2=ProductID
//nType3=GameID
//nType4=奖品类型
//nType5=奖品ID
//ll0=发出的奖品数量
//ll1=扣除的奖品数量

#define TK_STAT_TYPE_MATCH_PROMOTION_AWARD        10305
//nType2=ProductID
//nType3=GameID
//nType4=0
//nType5=0
//ll0=发放的金币数量
//ll1=发放的秋卡数量

//比赛桌统计
#define	TK_STAT_TYPE_MATCH_TABLE				10400
//nType2=ProductID
//nType3=MoneyRuler
//nType4=GameRuler
//ll0=发起(场次/人数)
//ll1=进入(人次/人数)
//ll2=游戏(人次/人数)
//ll3=游戏盘数


//比赛金币桌统计
#define	TK_STAT_TYPE_MATCH_GOLDTABLE			10500
//nType2=ProductID
//nType3=0用户，1机器人
//nType4=GameID
//ll0=开桌金币扣除
//ll1=入场金币扣除
//ll2=加血金币扣除
//ll3=台费金币扣除
//ll4=出场金币回兑
//ll5=回兑余数累计
//ll6=加血付费
//ll7=当前未回兑金币
//ll8=期初未回兑金币
//ll9=入场付费

//彩金统计
#define	TK_STAT_TYPE_JACKPOT					10600
//nType2=ProductID
//ll0=税费次数
//ll1=税费数额
//ll2=固定彩金发放次数
//ll3=固定彩金发放数额
//ll4=比例彩金发放次数
//ll5=比例彩金发放数额


//休闲德州彩金统计
#define	TK_STAT_TYPE_JACKPOT_CSTUD				10601
//nType2=ProductID
//nType3=得分类型
//ll0=次数
//ll1=玩家得分
//ll2=玩家输分


// 自建桌建桌统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_CREATE		10700
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=成功建桌次数
//ll1=成功建桌人数


// 自建桌进桌统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_ENTER		10701
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=成功进桌人数
//ll1=成功进桌次数


// 自建桌开赛统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_GAMEBEGIN		10702
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=成功开赛次数
//ll1=未开赛次数
//ll2=有买入行为但未开赛的桌数


// 自建桌买入坐下统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_SITDOWN		10703
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=就座人数
//ll1=买入总次数
//ll2=买入总人数
//ll3=2次买入人数
//ll4=3次买入人数
//ll5=4次以上买入人数
//ll6=旁观人数（每桌）
//ll7=中途退赛的玩家总数


// 自建桌审批统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_PERMITSET		10704
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=审批开关打开桌数
//ll1=审批开关关闭桌数
//ll2=桌主同意玩家就座次数
//ll3=桌主拒绝玩家就座次数
//ll4=发起审批的次数


// 自建桌桌内人数统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_PLAYER_COUNT		10705
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=桌内<3人的桌数
//ll1=桌内3-5人的桌数
//ll2=桌内5-10人的桌数
//ll3=桌内>10人的桌数


// 自建桌踢人统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_KICK		10706
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=被踢玩家总数
//ll1=被设置不许再进的人数


// 自建桌回兑统计
#define	TK_STAT_TYPE_MATCH_FD_TABLE_GOLD		10707
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=买入总金额
//ll1=买入总收益
//ll2=回兑总金额

#define TK_STAT_TYPE_MATCH_WAITTIME				10708
//nType2=ProductID
//nType3=0用户，1机器人
//nType4=GameID
//ll0=0-5s等待人数
//ll1=6-15s等待人数
//ll2=16-30s等待人数
//ll3=31-60s等待人数
//ll4=61s及以上时间等待人数

// 自建桌私密桌相关统计
#define TK_STAT_TYPE_MATCH_FD_TABLE_PRIVATE_PLACE 10709
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间
//ll0=开启私密的牌桌数量
//ll1=关闭私密的牌桌数量

// 自建桌推荐牌桌相关统计
#define TK_STAT_TYPE_MATCH_FD_TABLE_RECOMMEND 10710
//nType2=ProductID
//nType3=GameID
//ll0=申请推荐人数
//ll1=申请推荐次数
//ll2=成功申请到的人数
//ll3=成功申请到的次数

// 自建桌提前结束桌统计
#define TK_STAT_TYPE_MATCH_FD_TABLE_PREGAEMEND 10711
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间/局数
//ll0=桌主主动解散未开赛桌数(有买入行为)
//ll1=桌主主动解散已开赛桌数
//ll2=桌主主动解散总桌数
//ll3=自动解散总桌数

// 海豚建桌统计
#define TK_STAT_TYPE_MATCH_FD_TABLE_DOLPHIN 10712
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间/局数
//ll0=通过海豚创建的桌数
//ll1=通过海豚创建的桌子中开桌数
//ll2=通过海豚加入自建桌的玩家数

//AI加入数量统计
#define TK_STAT_TYPE_MATCH_AIJOIN 10713
//nType2=ProductID
//nType3=GameID
//nType4=轮次
//ll0=nTpye4轮次加入的AI数量

// 约战室建桌统计
#define TK_STAT_TYPE_MATCH_FD_TABLE_INVIETE 10714
//nType2=ProductID
//nType3=GameID
//nType4=级别
//nType5=时间/局数
//ll0=通过约战室创建的桌数
//ll1=通过约战室创建的桌子中开桌数
//ll2=通过约战室加入自建桌的玩家数

//闯关赛相关统计数据
#define TK_STAT_TYPE_MATCH_CHAPTER 10800
//nType2=ProductID
//nType3=GameID
//nType4=关数
//ll0=过关人次
//ll1=过关非断线离开人次
//ll2=过关断线离开人次
//ll3=重打人次
//ll4=重打非断线离开人次
//ll5=重打断线离开人次

//秋卡回兑统计
#define	TK_STAT_TYPE_MATCH_QKTABLE			10900
//nType2=ProductID
//nType3=0用户，1机器人
//nType4=GameID
//ll0=出场秋卡回兑

//游戏输赢和人次统计
#define	TK_STAT_TYPE_GAME_WINLOSS				20000
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//ll0=盘数
//ll1=Win
//ll2=Loss
//ll3=Draw
//ll4=WinAfterTax
//ll5=LossAfterTax
//ll6=DrawAfterTax

//游戏时长统计
#define	TK_STAT_TYPE_GAME_TIME					20001
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//ll0=游戏局-盘数(系统视角，无分域数据)
//ll1=游戏局-时长(系统视角，无分域数据)
//ll2=异常游戏局-盘数(系统视角，无分域数据)
//ll3=异常游戏局-时长(系统视角，无分域数据)
//ll4=用户时长(系统视角，无分域数据，=游戏时长*桌上人数)
//ll5=用户异常时长(系统视角，无分域数据，=异常游戏时长*桌上人数)
//ll6=人盘数(用户视角，可分域，一盘3人游戏，人盘数为3，有分域统计)
//ll7=人时长(用户视角，可分域，一盘3人60s的游戏，人时长为180s，每个玩家的人时长都为60s，有分域统计)
//ll8=异常人盘数（用户视角，可分域...）
//ll9=异常人时长（用户视角，可分域...）

//Poker税收统计
#define	TK_STAT_TYPE_POKER_TAX					20002

//游戏税收和流水统计
#define	TK_STAT_TYPE_GAME_TAX					20003

//游戏用户行为统计
#define	TK_STAT_TYPE_GAME_ACTION				20004
//nType2=GameID

//斗地主出牌延时统计
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

//WareDBExecDelay统计
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

//DBExecDelay次数统计
#define	TK_STAT_TYPE_DBEXECDELAY				20007
//DBExecDelay时长统计
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

//大厅匿名登陆Ack延时统计
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


//LoginDelay次数统计
#define	TK_STAT_TYPE_LOGINDELAY				20010
//LoginDelay时长统计
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


//PartnerLoginDelay次数统计
#define	TK_STAT_TYPE_PARTNERLOGINDELAY				20012
//PartnerLoginDelay时长统计
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


//CPSendMsgDelay次数统计
#define	TK_STAT_TYPE_CPSENDMSGDELAY					20014
//CPSendMsgDelay时长统计
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

//DBExecBlock次数统计
#define	TK_STAT_TYPE_DBEXECBLOCK				20016
//DBExecBlock时长统计
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

//DBExecError次数统计
#define	TK_STAT_TYPE_DBEXECERROR				20018
//nType2=SrvPort
//nType3=nExecWhere

//大厅Loing/NoReg登陆Ack延时统计
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

//斗地主岛屿结束休息延时统计
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


//游戏流水和回馈统计
#define	TK_STAT_TYPE_GAME_FLOW					20022


//捕鱼倍率鱼统计
#define	TK_STAT_TYPE_FISH_MULT					20023
//nType2=GameID
//nType3=ProductID
//nType4=Mult
//nType5=FishID
//ll0=出现次数
//ll1=捕获次数
//ll2=奖励数额


//捕鱼宝箱统计
#define	TK_STAT_TYPE_FISH_TREAS					20024
//nType2=GameID
//nType3=ProductID
//nType4=Mult
//ll0=出现次数
//ll1=
//ll2=奖励鱼币数额
//ll3=奖励物品数额


//捕鱼任务统计
#define	TK_STAT_TYPE_FISH_TASK					20025
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=TaskID
//ll0=出现次数
//ll1=完成次数
//ll2=奖励鱼币数额
//ll3=奖励物品数额


//捕鱼物品鱼统计
#define	TK_STAT_TYPE_FISH_WARE					20026
//nType2=GameID
//nType3=ProductID
//nType4=WareID
//nType5=FishID
//ll0=出现次数
//ll1=捕获次数
//ll2=物品数额


//Boss鱼统计
#define	TK_STAT_TYPE_FISH_BOSS					20027
//nType2=GameID
//nType3=ProductID
//nType4=BossID
//nType5=
//ll0=出现次数
//ll1=捕获次数
//ll2=击中奖励鱼币数
//ll3=捕获奖励鱼币数
//ll4=物品数额


//独立鱼统计
#define	TK_STAT_TYPE_FISH_DIS					20028
//nType2=GameID
//nType3=ProductID
//nType4=BossID
//nType5=
//ll0=出现次数
//ll1=捕获次数
//ll2=击中奖励鱼币数
//ll3=捕获奖励鱼币数
//ll4=物品数额

//JJ捕鱼炮台等级流水统计
#define	TK_STAT_TYPE_FISH_FIRE_FLOW				20029
//nType2=GameID
//nType3=ProductID
//nType4=Level
//nType5=FireType
//ll0=开炮次数
//ll1=开炮流水

#define TK_STAT_TYPE_JJFISH_SUBSIDY				20030
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll1=客户端请求的次数
//ll2=捕鱼服务向pivot发送的请求次数
//ll3=成功的次数

//JJ捕鱼锁定炮台等级流水统计
#define TK_STAT_TYPE_FISH_LOCKGEMPROP_FIRE_FLOW 20031
//nType2=GameID
//nType3=ProductID
//nType4=Level
//ll0=开炮次数
//ll1=开炮流水

//JJ捕鱼宝石道具使用次数统计
#define TK_STAT_TYPE_FISH_GEMPROP_USECNT		20032
//nType2=GameID
//nType3=ProductID
//110=锁定使用次数
//111=锁定消耗的技能石数量
//112=冰冻使用次数
//113=冰冻消耗的技能石数量
//114=召唤使用次数
//115=召唤消耗的技能石数量

// JJ捕鱼其他流水回馈统计
#define TK_STAT_TYPE_FISH_WATER_FEEDBACK		20033
//nType2=GameID
//nType3=ProductID
//nType4=Type 
//nType5=
//ll0=流水
//ll1=回馈

//千炮抽奖统计
#define	TK_STAT_TYPE_QPFISH_DRAW				20050
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=投注次数
//ll1=投注金额
//ll2=彩池奖励次数
//ll3=彩池奖励金额
//ll4=固定奖1次数
//ll5=固定奖1金额
//ll6=固定奖2次数
//ll7=固定奖2金额

//千炮能量技能统计
#define TK_STAT_TYPE_FISH_ENERGYSKILL			20051
//nType2=GameID
//nType3=ProductID
//nType4=SkillID
//nType5=
//ll0=使用次数
//ll1=技能消耗鱼币
//ll2=技能收获鱼币

//物品消耗统计
#define TK_STAT_TYPE_FISH_GOODSCOST				20052
//nType2=GameID
//nType3=ProductID
//nType4=GoodsID
//nType5=
//ll0=总消耗数
//ll1=升级消耗数
//ll2=使用狂暴消耗数
//ll3=使用锁定消耗数
//ll4=使用冰冻消耗数
//ll5=使用召唤消耗数

//千炮能量技能统计(新)
#define TK_STAT_TYPE_FISH_ENERGYSKILL_NEW		20053
//nType2=ProductID
//nType3=SkillID
//nType4=SkillLevel
//nType5=CannonLevel
//ll0=使用次数
//ll1=技能消耗鱼币
//ll2=技能收获鱼币

//千炮财神鱼统计
#define TK_STAT_TYPE_FISH_MAMMONFISH		20054
//nType2=GameID
//nType3=ProductID
//nType4=FishID
//nType5=Stage
//ll0=出现次数
//ll1=捕获次数
//ll2=消耗炮弹数
//ll3=捕获鱼币数
//ll4=变身掉落

//千炮夺宝次数统计
#define TK_STAT_TYPE_FISH_LOTTERY_COUNT		20055
//nType2=ProductID
//nType3=LotteryID
//nType4=
//nType5=
//ll0=开奖次数
//ll1=参与人次

//千炮表情使用次数统计
#define TK_STAT_TYPE_FISH_EMOTE_COUNT   20056
//nType2=GameID
//nType3=ProductID
//nType4=EmoteID
//nType5=
//ll0=使用次数
//ll1=消耗钻石量

//游戏时长统计
#define TK_STAT_TYPE_GAME_TIME_EX 20057
//nType2=GameID
//nType3=ProductID
//nType4=PlatID
//nType4=PlatID
//nType5=LanguageID
//ll0=混合轮盘数
//ll1=混合轮异常盘数

//千炮海神献礼奖励统计
#define TK_STAT_TYPE_FISH_LOTTERY_AWARD 20058
//nType2=ProductID
//nType3=LotteryID
//ll0=获得金币数量
//ll1=获得钻石数量
//ll2=获得秋卡数量

//炸弹人统计
#define	TK_STAT_TYPE_BOMBMAN				20060

//////////////////////////////////////////////////////////////////////////
// JJ猎人服务端统计
// 金融控制统计
#define TK_STAT_TYPE_HUNTER_FINANCECONTROL			20069
//nType2=GameID
//nType3=ProductID
//ll0=亏损次数
//ll1=亏损补偿金额
//ll2=盈利次数
//ll3=盈利扣除金额

//Boss统计
#define TK_STAT_TYPE_HUNTER_BOSS			20070
//nType2=GameID
//nType3=ProductID
//nType4=MonsterKindID
//nType5=
//ll0=出现次数
//ll1=捕获次数
//ll2=破甲狩猎币
//ll3=破甲秋卡
//ll4=捕获狩猎币
//ll5=捕获秋卡
//ll6=消耗子弹


//精英怪统计
#define TK_STAT_TYPE_HUNTER_ELITE			20071
//nType2=GameID
//nType3=ProductID
//nType4=MonsterKindID
//nType5=Multi
//ll0=出现次数
//ll1=捕获次数
//ll2=消耗子弹
//ll3=捕获狩猎币
//114=奖池投注狩猎币



//特殊怪统计
#define TK_STAT_TYPE_HUNTER_SPECIAL			20072
//nType2=GameID
//nType3=ProductID
//nType4=MonsterType
//nType5=
//ll0=出现次数
//ll1=捕获次数
//ll2=捕获狩猎币
//ll3=捕获秋卡
//ll4=消耗子弹


//狩猎任务统计
#define TK_STAT_TYPE_HUNTER_TASK			20073
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=触发次数
//ll1=任务1完成次数
//ll2=任务2完成次数
//ll3=颁奖次数
//ll4=触发人次
//ll5=完成人次
//ll6=评价S人次
//ll7=评价A人次
//ll8=评价B人次
//ll9=评价C人次


//狩猎任务奖励统计
#define TK_STAT_TYPE_HUNTER_TASK_AWARD		20074
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=钻石奖励
//ll1=水晶奖励
//ll2=月光石奖励
//ll3=尖晶石奖励


//物品消耗统计
#define TK_STAT_TYPE_HUNTER_COSTGOODS		20075
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=冰冻道具次数
//ll1=冰冻消耗钻石
//ll2=召唤道具次数
//ll3=召唤消耗钻石
//ll4=狂暴道具次数
//ll5=狂暴消耗钻石
//ll6=分身道具次数
//ll7=分身消耗钻石
//ll8=升级消耗钻石


//物品发放统计
#define TK_STAT_TYPE_HUNTER_GETGOODS		20076
//nType2=GameID
//nType3=ProductID
//nType4=GoodsID
//nType5=
//ll0=掉落次数
//ll1=掉落数量
//ll2=获得物品总次数
//ll3=获得物品总量


//武器技能统计
#define TK_STAT_TYPE_HUNTER_SKILL			20077
//nType2=GameID
//nType3=ProductID
//nType4=SkillID
//nType5=
//ll0=使用次数
//ll1=消耗鱼币
//ll2=获得鱼币


//玩家相关统计
#define TK_STAT_TYPE_HUNTER_PLAYER			20078
//nType2=GameID
//nType3=ProductID
//nType4=
//nType5=
//ll0=死亡次数
//ll1=复活次数
//ll2=治愈使用次数
//ll3=成功领取破产补助次数

// 流水统计
#define TK_STAT_TYPE_HUNTER_FLOW			20079
//nType2=GameID
//nType3=ProductID
//nType4=FireLevel
//nType5=FireType
//ll0=开炮次数
//ll1=开炮流水
//////////////////////////////////////////////////////////////////////////

// Sit&Go（方块大战）用户报名成功到开赛的时长统计
#define TK_STAT_TYPE_USERMATCHINGTIME		20080
//nType2=MPID
//nType3=GameID
//ll0=匹配次数累计
//ll1=匹配时长累计

// 斗地主机器人胜率统计
#define TK_STAT_TYPE_LORD_BOTAI  20090
//nType2=GameID
//nType3=ProductID
//ll0=机器人参与盘数
//ll1=机器人胜盘数
//ll2=机器人地主盘数
//ll3=机器人地主胜盘数

//陪打机器人统计
#define	TK_STAT_TYPE_PACER_BOT_PLAYER	20091
//nType2=ProductID
//nType3=GameID
//ll0=陪打机器人进入比赛(人次/人数)
//ll1=陪打机器人离开比赛(人次/人数)

// 机器人游戏对局结果统计（游戏包括：保皇）
#define  TK_STAT_TYPE_AI_ROUNDRESULT	20092 
//nType2=GameId
//nType3=ProductID
//nType4=Character  （保皇身份: 1~皇帝, 2~侍卫, 3~反贼, 4~独保）
//ll0=参赛总局数
//ll1=胜局数
//ll2=平局数
//ll3=败局数

//服务器性能统计
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


//外旁观统计
#define	TK_STAT_TYPE_BROADCAST					21000
//nType2=ProductID
//nType3=GameID
//ll0=外旁观人次/人数


//大厅匿名登录IP统计
#define TK_STAT_TYPE_LOBBY_LOGINIP_ANONYMOUS	30000
//ll0=成功
//ll1=已登录
//ll2=版本太低
//ll3=版本太高
//114=匿名时断开
//115=对象异步析构


//大厅用户登录IP统计
#define TK_STAT_TYPE_LOBBY_LOGINIP_USER			30001
//ll0=成功(人次/人数)
//ll1=已登录
//ll2=未匿名登录
//ll3=DS拒绝(人次/人数)
//ll4=LS跳转
//ll5=已在别处登录
//ll6=踢出失败
//ll7=登录时断开(人次/人数)
//ll8=对象异步析构
//ll9=成功踢出(人次/人数)


//大厅用户登出IP统计
#define TK_STAT_TYPE_LOBBY_LOGOUTIP_USER		30002
//ll0=成功(人次/人数)
//ll1=未登录
//ll2=对象删除失败


//大厅用户登录时长IP统计
#define TK_STAT_TYPE_LOBBY_LOGINIP_TIME			30003
//nType2=OffLineType(下线类型)
//ll0=总累计时长
//ll1=小于1分钟(人次/人数)
//ll2=小于10分钟(人次/人数)
//ll3=小于30分钟(人次/人数)
//ll4=小于1小时(人次/人数)
//ll5=小于2小时(人次/人数)
//ll6=小于3小时(人次/人数)
//ll7=小于5小时(人次/人数)
//ll8=小于8小时(人次/人数)
//ll9=大于8小时(人次/人数)

//大厅用户登录踢开统计
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

//大厅匿名登录分平台IP统计
#define TK_STAT_TYPE_LOBBY_LOGINIP_ANONYMOUS_PLAT	30010
//nType2=PlatID
//ll0=成功
//ll1=已登录
//ll2=版本太低
//ll3=版本太高
//114=匿名时断开
//115=对象异步析构


//大厅用户登录分平台IP统计
#define TK_STAT_TYPE_LOBBY_LOGINIP_USER_PLAT			30011
//nType2=PlatID
//ll0=成功(人次/人数)
//ll1=已登录
//ll2=未匿名登录
//ll3=DS拒绝(人次/人数)
//ll4=LS跳转
//ll5=已在别处登录
//ll6=踢出失败
//ll7=登录时断开(人次/人数)
//ll8=对象异步析构
//ll9=成功踢出(人次/人数)


//大厅用户登出分平台IP统计
#define TK_STAT_TYPE_LOBBY_LOGOUTIP_USER_PLAT		30012
//nType2=PlatID
//ll0=成功(人次/人数)
//ll1=未登录
//ll2=对象删除失败


//大厅用户登录时长分平台IP统计
#define TK_STAT_TYPE_LOBBY_LOGINIP_TIME_PLAT			30013
//nType2=OffLineType(下线类型)
//nType3=PlatID
//ll0=总累计时长
//ll1=小于1分钟(人次/人数)
//ll2=小于10分钟(人次/人数)
//ll3=小于30分钟(人次/人数)
//ll4=小于1小时(人次/人数)
//ll5=小于2小时(人次/人数)
//ll6=小于3小时(人次/人数)
//ll7=小于5小时(人次/人数)
//ll8=小于8小时(人次/人数)
//ll9=大于8小时(人次/人数)


//大厅用户登录分平台IP踢开统计
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


//#define TK_STAT_TYPE_LOBBY_LOGIN_ANONYMOUS	30100//占位


//大厅用户登录统计
#define TK_STAT_TYPE_LOBBY_LOGIN_USER			30101
//ll0=成功(人次/人数)
//ll1=已登录
//ll2=未匿名登录
//ll3=DS拒绝(人次/人数)
//ll4=LS跳转
//ll5=已在别处登录
//ll6=踢出失败
//ll7=登录时断开(人次/人数)
//ll8=对象异步析构
//ll9=成功踢出(人次/人数)


//大厅用户登出统计
#define TK_STAT_TYPE_LOBBY_LOGOUT_USER			30102
//ll0=成功(人次/人数)
//ll1=未登录
//ll2=对象删除失败


//大厅用户登录时长统计
#define TK_STAT_TYPE_LOBBY_LOGIN_TIME			30103
//nType2=OffLineType(下线类型)
//ll0=总累计时长
//ll1=小于1分钟(人次/人数)
//ll2=小于10分钟(人次/人数)
//ll3=小于30分钟(人次/人数)
//ll4=小于1小时(人次/人数)
//ll5=小于2小时(人次/人数)
//ll6=小于3小时(人次/人数)
//ll7=小于5小时(人次/人数)
//ll8=小于8小时(人次/人数)
//ll9=大于8小时(人次/人数)

//大厅用户登录总经验统计
#define TK_STAT_TYPE_LOBBY_LOGIN_EXPERIENCE			30104
//ll0=[0, 1000]
//ll1=[1001, 10000]
//ll2=[10001, 20000]
//ll3=[20001, 50000]
//ll4=[50001, 100000]
//ll5=[100001, 200000]
//ll6=[200001, 500000]
//ll7=[500001, 1000000]
//ll8=[1000001 以上)

//大厅用户登录踢开统计
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


//大厅用户登录分平台统计
#define TK_STAT_TYPE_LOBBY_LOGIN_USER_PLAT			30111
//nType2=PlatID
//ll0=成功(人次/人数)
//ll1=已登录
//ll2=未匿名登录
//ll3=DS拒绝(人次/人数)
//ll4=LS跳转
//ll5=已在别处登录
//ll6=踢出失败
//ll7=登录时断开(人次/人数)
//ll8=对象异步析构
//ll9=成功踢出(人次/人数)


//大厅用户登出分平台统计
#define TK_STAT_TYPE_LOBBY_LOGOUT_USER_PLAT			30112
//nType2=PlatID
//ll0=成功(人次/人数)
//ll1=未登录
//ll2=对象删除失败


//大厅用户登录时长分平台统计
#define TK_STAT_TYPE_LOBBY_LOGIN_TIME_PLAT			30113
//nType2=OffLineType(下线类型)
//nType3=PlatID
//ll0=总累计时长
//ll1=小于1分钟(人次/人数)
//ll2=小于10分钟(人次/人数)
//ll3=小于30分钟(人次/人数)
//ll4=小于1小时(人次/人数)
//ll5=小于2小时(人次/人数)
//ll6=小于3小时(人次/人数)
//ll7=小于5小时(人次/人数)
//ll8=小于8小时(人次/人数)
//ll9=大于8小时(人次/人数)

//大厅用户登录总经验分平台统计
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
//ll8=[1000001 以上)

//大厅用户登录分平台踢开统计
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

//网页用户登录IP统计
#define TK_STAT_TYPE_WEB_LOGIN_IP			30201
//ll0=成功(人次/人数)
//ll1=网络失败(人次/人数)
//ll2=密码错误(人次/人数)
//ll3=拒绝登录(人次/人数)
//ll4=保护登录(人次/人数)
//ll5=其他错误(人次/人数)
//ll6=鉴权失败(人次/人数)

//网页用户登录统计
#define TK_STAT_TYPE_WEB_LOGIN_USER			30301
//ll0=成功(人次/人数)
//ll1=网络失败(人次/人数)
//ll2=密码错误(人次/人数)
//ll3=拒绝登录(人次/人数)
//ll4=保护登录(人次/人数)
//ll5=其他错误(人次/人数)
//ll6=鉴权失败(人次/人数)

//网页用户SSO登录统计
#define TK_STAT_TYPE_WEB_SSO_LOGIN_USER		30302
//ll0=总次数
//ll1=成功
//ll2=超时
//ll3=长度错误
//ll4=MD5错误
//ll5=网络失败
//ll6=次数错误

//网页用户FPWD验证统计
#define TK_STAT_TYPE_WEB_VERIFY_FPWD_USER		30303
//ll0=总次数
//ll1=成功
//ll2=网络失败
//ll3=密码错误
//ll4=UID不存在
//ll5=其他数错误

//网页用户SPWD验证统计
#define TK_STAT_TYPE_WEB_VERIFY_SPWD_USER		30304
//ll0=总次数
//ll1=成功
//ll2=网络失败
//ll3=密码错误
//ll4=UID不存在
//ll5=其他数错误

//TGP用户登录统计
#define TK_STAT_TYPE_TGP_LOGIN_USER			31101
//ll0=成功(人次/人数)
//ll1=已登录
//ll2=未匿名登录
//ll3=DS拒绝(人次/人数)
//ll4=LS跳转
//ll5=已在别处登录
//ll6=踢出失败
//ll7=登录时断开(人次/人数)
//ll8=对象异步析构
//ll9=成功踢出(人次/人数)

//组委会金融统计
#define TK_STAT_TYPE_COMMITTEE_MONEY			40001
//nType2=纲：类别
//nType3=ProductID
//nType4=GameID
//ll0=金币扣除次数
//ll1=金币发放次数
//ll2=奖券扣除次数
//ll3=奖券发放次数
//ll4=-金币
//ll5=+金币
//ll6=-奖券
//ll7=+奖券
//ll8=-参赛积分
//ll9=+参赛积分


//组委会金融(幸运卡)统计
#define TK_STAT_TYPE_COMMITTEE_MONEY_LUCKCARD		40002
//nType2=纲：类别
//nType3=ProductID
//nType4=GameID
//ll0=金币扣除次数
//ll1=金币发放次数
//ll2=奖券扣除次数
//ll3=奖券发放次数
//ll4=-金币
//ll5=+金币
//ll6=-奖券
//ll7=+奖券
//ll8=-参赛积分
//ll9=+参赛积分

//组委会金融货币转移统计
#define TK_STAT_TYPE_COMMITTEE_TRANSFERMNY		40003
//nType2=ProductID
//nType3=GameID
//nType4=AccTypeSrc
//nType5=AccTypeDst
//ll0=次数
//ll1=Coin
//ll2=Bouns
//ll3=Gold
//ll4=Cert

//组委会Grow统计
#define TK_STAT_TYPE_COMMITTEE_GROW			40004
//nType2=ProductID
//nType3=GameID
//nType4=GrowID
//ll0=发放次数
//ll1=扣除次数
//ll2=+Value
//ll3=-Value

//组委会64位Grow统计
#define TK_STAT_TYPE_COMMITTEE_GROW_64		40005
//nType2=ProductID
//nType3=GameID
//nType4=GrowID
//ll0=发放次数
//ll1=扣除次数
//ll2=+Value
//ll3=-Value

//组委会金融其他类型统计
#define TK_STAT_TYPE_COMMITTEE_MONEY_OTHER		40099
//nType2=纲：类别
//nType3=ProductID
//nType4=GameID
//ll0=金币扣除次数
//ll1=金币发放次数
//ll2=奖券扣除次数
//ll3=奖券发放次数
//ll4=-金币
//ll5=+金币
//ll6=-奖券
//ll7=+奖券
//ll8=-参赛积分
//ll9=+参赛积分

//组委会金融业务统计-付费表情
#define TK_STAT_TYPE_COMMITTEE_MONEY_EXPRESSION 40100
//nType2=ProductID
//nType3=GameID
//ll0=表情数量
//ll1=表情回收金币
//ll2=使用数量(人次/人数)


//货币兑换统计
#define TK_STAT_TYPE_DATA_EXCHANGEMNY		50101
//nType2=Place
//nType3=MoneySrcType
//nType4=MoneyDstType
//ll0=次数
//ll1=MoneySrc
//ll2=MoneyDst


//奖券兑奖统计
#define TK_STAT_TYPE_DATA_AWARDCACHIN		50201
//nType2=Place
//ll0=次数
//ll1=Bonus


//现金购买元宝统计
#define TK_STAT_TYPE_DATA_RMB2COIN			50301
//nType2=Place
//ll0=次数
//ll1=Rmb
//ll2=Rmb*RmbPer/100000
//ll3=Coin


//两种货币之间的兑换，其中有且一种货币是JJ元宝
#define TK_STAT_TYPE_PAYIN_BANK					50302//充值银行名分类统计
#define TK_STAT_TYPE_PAYIN_AGENT				50303//支付渠道商分类统计
#define TK_STAT_TYPE_PAYIN_ZONE					50304//支付区域分类统计
#define TK_STAT_TYPE_PAYIN_TYPE					50305//银行类型分类统计
//nType2=dwSrcMID 源货币类型
//nType3=dwDstMID 目标货币类型
//nType4=dwEndMID 最终转换的货币类型
//nType5=dwPWBankID/dwPWAgentID/dwPWZoneID/dwPWTypeID 充值银行名/支付渠道商/支付区域/银行类型
//ll0=次数
//ll1=dwSrcAmount 源货币数量
//ll2=dwSrcAmount*dwSrcPer/100000 实收
//ll3=dwDstAmount 目标货币数量
//ll4=dwEndAmount 最终转换的货币数量


//元宝购买其他货币统计
#define TK_STAT_TYPE_DATA_COIN2OUT			50310
//nType2=dwOMnyID	外部货币ID
//nType3=dwORegion	外部作用域，如游戏的区
//nType4=dwOPayT	外部购买类型
//ll0=次数
//ll1=Coin			元宝数量
//ll2=dwOMny		外部货币数量
//ll3=Rmb
//ll4=Rmb*RmbPer/100000


//买入或卖出其他货币统计
#define TK_STAT_TYPE_DATA_VPAY				50320
//nType2=AccType	帐号类型
//nType3=dwMonyID	内部货币ID
//nType4=dwOMonyID	外部货币ID
//ll0=买入次数
//ll1=dwMonyMC		内部货币数量
//ll2=dwIMonyMC		外部货币数量
//ll5=卖出次数
//ll6=dwMonyMC		内部货币数量
//ll7=dwOMonyMC		外部货币数量


//VIP券买虚拟货币统计
#define TK_STAT_TYPE_DATA_VIPCERT2MNY		50401
//nType2=Place
//nType3=VipType
//nType4=VipCertType
//ll0=次数
//ll1=VipCertAmount
//ll2=Other
//ll3=Coin
//ll4=Bouns
//ll5=Gold
//ll6=Cert


//虚拟流转统计
#define TK_STAT_TYPE_DATA_CHARGE			50501
//nType2=Place
//nType3=Type(纲,eDicMnysTradeEntryType)
//nType4=SubType
//ll0=次数
//ll1=+Coin
//ll2=-Coin
//ll3=+Bouns
//ll4=-Bouns
//ll5=+Gold
//ll6=-Gold
//ll7=+Cert
//ll8=-Cert


//货币转移统计
#define TK_STAT_TYPE_DATA_TRANSFERMNY		50601
//nType2=SubType
//nType3=AccTypeSrc
//nType4=AccTypeDst
//ll0=次数
//ll1=Coin
//ll2=Bouns
//ll3=Gold
//ll4=Cert


//物品统计
#define TK_STAT_TYPE_WARE					60001
//nType2=1：比赛、2：网站业务
//nType3=SubType(比赛ID或业务ID)
//nType4=WareID
//ll0=增加次数
//ll1=增加数量
//ll2=扣减次数
//ll3=扣减数量


//大厅用户行为统计
#define TK_STAT_TYPE_LOBBY_USERACT			70001
//nType2=功能号
//nType3=行为类型
//nType4=用户类型
//ll0=次数
//ll1=参数

//更新下载统计
#define TK_STAT_TYPE_UPDATE			70002

//聊天室统计
#define TK_STAT_TYPE_CHAT			70003


//玩家注册信息验证统计(登录名和昵称)
#define TK_STAT_TYPE_REGINFOCHECK					80001
//nType2=1：网站	2：大厅		3:比赛
//nType3=1：登录名	2：昵称
//ll0=	总请求次数
//ll1=	服务器处理异常
//ll2=	违法定义规则
//ll3=	包含非法关键字
//ll4=	已经存在


//玩家注册错误统计
#define TK_STAT_TYPE_REGISTER					80002
//nType2=1：网站	2：大厅		3：比赛
//ll0=	总请求次数
//ll1=	服务器处理异常
//ll2=	登录名错误
//ll3=	登录名已经存在
//ll4=	昵称错误
//ll5=	昵称已经存在
//ll6=	密码格式非法
//ll7=	验证码错误
//ll8=	同IP注册请求频率太快或太多


//Sns活跃统计
#define TK_STAT_TYPE_SNSACTSCORE					90000
//ll0=	非活跃群
//ll1=	活跃群1
//ll2=	活跃群2-50
//ll3=	活跃群>50
//ll5=	非活跃公社
//ll6=	活跃公社1
//ll7=	活跃公社2-50
//ll8=	活跃公社>50

//Sns聊天统计
#define TK_STAT_TYPE_SNSCHATMSG					90001
//ll0=	私人聊天次数
//ll1=	组织聊天次数
//ll2=	加好友次数
//ll3=	加关注次数
//ll5=	群聊天次数
//ll6=	公社聊天次数

//Sns关系登录用户数统计
#define TK_STAT_TYPE_SNSLogin				90002
//ll0=	登录用户数
//ll1=	好友关系登录用户数
//ll2=	关注关系登录用户数
//ll3=	公社关系登录用户数
//ll4=	群关系登录用户数
//ll5=	网吧关系登录用户数

//Sns关系统计
#define TK_STAT_TYPE_SNS			90003
//nType2=1：讨论组


//团体赛统计
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=随机组队参赛（人数/人次）
//ll1=自由组队参赛（人数/人次）
//ll2=创建队伍（人数/人次）
//ll3=解散队伍（人数/人次）
#define TK_STAT_TYPE_MATCH_TEAM			90004

//DAG双人班车统计
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=个人报名（人数/人次）
//ll1=随机匹配（人数/人次）
//ll2=组队报名（人数/人次）
#define TK_STAT_TYPE_DOUBLE_LADDER     90005

//众筹业务统计
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=发起众筹（人数/人次）
//ll1=取消众筹（人数/人次）
//ll2=购买众筹（人数/人次）
#define TK_STAT_TYPE_CROWD_FUNDING     90006


//比赛积分桌统计
#define	TK_STAT_TYPE_MATCH_GOLDTABLE_GROW	90007
//nType2=ProductID
//nType3=0用户，1机器人
//nType4=GameID
//nType5=GrowID
//ll0=开桌积分扣除
//ll1=入场积分扣除
//ll2=加血积分扣除
//ll3=台费积分扣除
//ll4=出场积分回兑
//ll5=回兑余数累计
//ll6=加血付费
//ll7=当前未回兑积分
//ll8=期初未回兑积分
//ll9=入场付费

//比赛加血费用统计(积分)
#define	TK_STAT_TYPE_MATCH_HEMATINIC_COST_GROW	90008
//nType2=ProductID
//nType3=GrowID
//nType4=GameID 新增（wuqy 2022.11.1）
//ll0=加血使用积分数

//比赛复活费用统计
#define	TK_STAT_TYPE_MATCH_RELIVE_COST_GROW		90009
//nType2=ProductID
//nType3=GrowID
//nType4=GameID 新增（wuqy 2022.11.1）
//ll0=复活使用积分数值

//触发必定晋级
#define TK_STAT_TYPE_MUST_PROMOTE               90010
//nType2=ProductID
//nType3=StageID
//nType4=BoutID
//ll0=被淘汰（人数/人次）
//ll1=触发必定晋级（人数/人次）

//游戏中复活费用统计
#define	TK_STAT_TYPE_MATCH_GAMING_RELIVE_COST		90011
//nType2=ProductID
//nType3=复活挡位
//nType4=特权等级倍数
//nType5=GameID
//ll0=复活次数（人数/人次）
//ll1=复活发放数值
//ll2=复活回收数值

//组队众筹业务统计
//nType2=ProductID
//nType3=GameID
//nType4=wCntTID
//ll0=组队发起众筹（人数/人次）
//ll1=组队取消众筹（人数/人次）
//ll2=组队购买众筹（人数/人次）
#define TK_STAT_TYPE_DOUBLE_CROWD_FUNDING     90012

//WBS统计
#define	TK_STAT_TYPE_WBS_BEGIN				100000
#define	TK_STAT_TYPE_WBS_END				199999



#endif  //__STATTYPEDEFINE__INCLUDED__