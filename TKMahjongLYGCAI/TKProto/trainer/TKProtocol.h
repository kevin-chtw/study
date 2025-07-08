#pragma once

#include "framework.h"
//TK消息包头定义
typedef struct tagTKHEADER
{
    DWORD dwMagic;   //消息魔数
    DWORD dwSerial;  //序列号
    WORD wOrigine;   //消息来源
    WORD wReserve;   //保留
    DWORD dwType;    //消息类型
    DWORD dwParam;   //消息参数（消息版本，返回值，标志位等）
    DWORD dwLength;  //实际数据长度，不包括消息头
} TKHEADER, *PTKHEADER;

#define TKHEADERSIZE (sizeof(TKHEADER))
#define MSG_LENGTH(type) (sizeof(type) - sizeof(TKHEADER))

//消息类型（tagTKHEADER::dwType） 定义
//高8位为类型定义区间
#define TK_REQ 0x00000000  //请求消息类型，第32位为0
#define TK_ACK 0x80000000  //应答消息类型，第32位为1
#define TK_ENC 0x40000000  //加密消息类型，第31位为1
#define TK_ZIP 0x20000000  //压缩消息类型，第30位为1
#define TK_SHR 0x10000000  //共享消息类型，第29位为1
#define TK_PRI 0x08000000  //优先消息类型，第28位为1
#define TK_DLY 0x04000000  //延迟消息类型，第27位为1,只对TK_SHR类型的消息有效
#define TK_SEC 0x01000000  //安全消息类型，第26位为1

#define TK_ACKRESULT_SUCCESS 0
#define TK_ACKRESULT_FAILED 1
#define TK_ACKRESULT_SVRBUSY 2
#define TK_ACKRESULT_LOWVERSION 3  //版本太低
#define TK_ACKRESULT_NOTFINDOBJ 4  //对象没有找到
#define TK_ACKRESULT_OBJEXIST 5    //对象已经存在
#define TK_ACKRESULT_WAITASYNC 6   //等待异步返回结果

//失败消息的ack（为了编程方便）
typedef struct tagTKFAILACKHEADER : public tagTKHEADER
{
    tagTKFAILACKHEADER(){};
    tagTKFAILACKHEADER(DWORD dwMsgType)
    {
        dwType = TK_ACK | dwMsgType;
        dwLength = 0;
        dwParam = TK_ACKRESULT_FAILED;
    };
} TKFAILACKHEADER, *PTKFAILACKHEADER;

//共享发送消息
typedef struct tagTKSHAREHEADER : public tagTKHEADER
{
} TKSHAREHEADER, *PTKSHAREHEADER;

//消息区间起始定义(从消息发起方向来定义)
#define TK_MSG_GENERAL 0x00000000        // 通用消息				区间 共计1024个
#define TK_MSG_DATA 0x00000400           // (Data服务组) Data服务
#define TK_MSG_DATREGISTPUSH 0x00000500  // (Data服务组) 注册推送服务 pangs added 2017.05.19
#define TK_MSG_DATBROKER 0x00000600      // (Data服务组) Data服务代理 huangfc 2019.11.13

#define TK_MSG_MATCH 0x00000800           // Match服务相关的消息
#define TK_MSG_LOBBY 0x00000C00           // Lobby服务相关的消息
#define TK_MSG_GAME 0x00001000            // Game服务相关的消息
#define TK_MSG_ROUNDINFO 0x00001400       // RoundInfo服务相关的消息
#define TK_MSG_SPECROUNDINFO 0x00001500   //SpecRoundInfo yangguang02 2022.3.14
#define TK_MSG_MONEY 0x00001800           // (MGW服务组) 货币服务
#define TK_MSG_SYSMGR 0x00001C00          // 系统管理服务相关的消息
#define TK_MSG_REALPAY 0x00002000         // (Pay服务组)RealPay服务相关消息
#define TK_MSG_MONITOR 0x00002400         //“监控服务”相关消息
#define TK_MSG_HISTORY 0x00002800         // (HIS服务组) 历史服务
#define TK_MSG_WARE 0x00002C00            // (MGW服务组) 物品服务
#define TK_MSG_AUTHENTICATION 0x00003000  //“用户鉴权服务”相关消息

#define TK_MSG_DYNCCONFIG 0x00003400  // (Data服务组) 字典数据服务

#define TK_MSG_COMMITTEE 0x00003800       // (Comit服务组) 组委会服务
#define TK_MSG_CMTASYNCNOTICE 0x00003900  //(CMT服务组) 异步通知服务 add by caowy 2023.3.21
#define TK_MSG_GROW 0x00003C00            // (MGW服务组)养成数据

#define TK_MSG_SALEAGENCY 0x00004000  // 分销商相关消息
#define TK_MSG_CHAT 0x00004400        // 聊天相关消息 add by baiyx add at 2008-05-20
#define TK_MSG_MARKER 0x00004800      // (HIS服务组) 里程碑相关消息 add by baiyx add at 2008-06-12
#define TK_MSG_MATCHSTORY 0x00004C00  // 赛程展播相关消息 add by wuqy add at 2008-08-18

#define TK_MSG_SNSDATA 0x00005000   // (SNS服务组)“关系” 数据服务(SNS -- Social Network System) ouyy add 2008.11.06
#define TK_MSG_SNSPIVOT 0x00005400  // (SNS服务组)“关系” 枢纽服务(SNS -- Social Network System) ouyy add 2008.11.06
#define TK_MSG_SNSLOAD 0x00005800   // (SNS服务组)“关系” 负载服务(SNS -- Social Network System) ouyy add 2008.11.06

#define TK_MSG_USERTOURNEY 0x00005C00    // (SNS服务组) 小黑桌服务相关消息 2008.12.12, wt
#define TK_MSG_MATCHRESULT 0x00006000    // 比赛结果相关消息 add by wuqy add at 2008-12-18
#define TK_MSG_MATCHID 0x00006400        // 比赛ID服务相关消息 add by wuqy add at 2008-12-22
#define TK_MSG_MATCHDB 0x00006500        // 比赛DB服务相关消息 add by wuqy add at 2014-4-10
#define TK_MSG_TABLEID 0x00006600        // 自建桌桌号管理服务相关消息 add by wuqy add at 2017-7-24
#define TK_MSG_MATCHCTRL 0x00006800      // 比赛维护控制服务相关消息 add by wuqy add at 2009-1-16
#define TK_MSG_STAT 0x00006C00           // 统计服务相关消息 add by wuqy 2009.3.18
#define TK_MSG_CARDSREPO 0x00006E00      // 牌库服务相关消息 add by wuqy 2017.8.17
#define TK_MSG_CARDGROUPREPO 0x00006F00  // 游戏服务和比赛服务提供牌组 add by yangguang02 2019.11.01

#define TK_MSG_SNSAPPDATA 0x00007000    // (SNS系统)“关系”应用数据服务相关消息(SNS -- Social Network System) ouyy add 2009.03.23
#define TK_MSG_ANTIWALLOW 0x00007400    // 防沉迷服务相关消息 wuqy add 2009.04.8
#define TK_MSG_MODULEUPDATE 0x00007800  // 客户端模块更新服务相关消息 wuqy add 2009.05.15

#define TK_MSG_SNSTOURNEY 0x00007C00   // SNS自定义比赛消息定义 ouyy add 2009.05.31
#define TK_MSG_TIMETOURNEY 0x00008000  // 定点赛ini文件服务消息定义 wuqy add 2009.07.28

#define TK_MSG_MATCHPOS 0x00008400  // 用户比赛位置服务消息定义 wuqy add 2009.12.24

#define TK_MSG_HISRECORDER 0x00008C00  // (HIS服务组) 历史记录服务消息定义 baiyx add 2010.02.21
#define TK_MSG_HISBROKER 0x00009000    // (HIS服务组) 历史Broker服务消息定义
#define TK_MSG_INFOCENTER 0x00009400   // (Data服务组) "信息中心" 服务 add by ouyy 2010.03.31

#define TK_MSG_CUSTOMERSERVICE 0x00009800    // 客服平台消息定义 add by chengl 2010.03.31
#define TK_MSG_STAFFLOGINSERVICE 0x00010000  // 客服平台内网登录协议	add by chengl 2010.04.09

#define TK_MSG_THRESHOLD 0x00010400  // (Data服务组) "Threshold数据阈值"服务 add by ouyy 2010.04.12
#define TK_MSG_TRIGGER 0x00010800    // (Data服务组) "Trigger数据触发" 服务  add by ouyy 2010.04.12

#define TK_MSG_VIRTUALPAY 0x00010C00  // (Pay服务组) 虚拟支付服务  add by baiyx 2010.05.11

#define TK_MSG_IPINFOSRV 0x00011000       // (Data服务组) IPINFO服务消息定义 add by ouyy 2010.05.25
#define TK_MSG_DATASAFECENTER 0x00011400  // (Data服务组) Data safe center 服务消息定义 add by ouyy 2010.06.22

#define TK_MSG_SMEPHONECALL 0x00011700  // 语音呼叫服务
#define TK_MSG_MOBILEAUTH 0x00011750    // 手机号码一键认证接入服务 fengdh 2019.12.13
#define TK_MSG_SMSCHANNEL 0x00011800    // (Data服务组)短信通道服务 add by baiyx 2010.06.28
#define TK_MSG_SMECAT 0x00011900        // 短信猫服务 pangs added 20140103

#define TK_MSG_USER 0x00011C00      // (User服务组)用户数据服务
#define TK_MSG_USERAUTH 0x00012000  // (User服务组)用户鉴权(含登陆注册)
#define TK_MSG_USERINFO 0x00012400  // (User服务组)用户信息处理

#define TK_MSG_BOSS 0x00012800  // (Data服务组)超级接口服务

//#define TK_MSG_BIZDEAL         0x00012C00 // (BIZ服务组)普通服务
#define TK_MSG_BIZPRESENT 0x00012C00  // (BIZ服务组)系统赠送服务
#define TK_MSG_BIZSALARY 0x00013000   // (BIZ服务组)系统发放用户工资服务 add by baiyx 2010.08.03
#define TK_MSG_BIZPROFIT 0x00013400   // (BIZ服务组)合作方利润结算服务

#define TK_MSG_ECAWEB 0x00013800  // (ECA服务组)网站ECA业务

#define TK_MSG_DOWNLOADCTL 0x00013C00  // 客户端更新下载控制服务

#define TK_MSG_VSNODE 0x00014000  // (VS服务组)对战节点服务服务

#define TK_MSG_MOBILECONN 0x00014800   // 移动连接服务消息 add by chengliang 2010.11.16
#define TK_MSG_MOBILEAGENT 0x00015000  // 移动用户代理消息 add by chengliang 2010.11.16

#define TK_MSG_DOFFLINEMSG 0x00015400   // (Date服务组)离线消息服务 add by ouyy 2010.12.17
#define TK_MSG_DPAYPROXYMSG 0x00015800  // (Date服务组)充值代理服务 add by ouyy 2010.12.30

#define TK_MSG_USERREGISTERMSG 0x00015C00  // (User服务组)用户注册 add by ouyy 2010.12.30

#define TK_MSG_JJWGFLASH 0x00016000  // JJWebGameFlash服务 add by wuqy 2011.3.18

#define TK_MSG_VSLOBBY 0x00016400  // (VS服务组) Lobby服务 add by wuqy 2011.4.11
#define TK_MSG_VSROOM 0x00016800   // (VS服务组) Room服务 add by wuqy 2011.4.11

#define TK_MSG_EXTERNAL 0x00016C00  // (Ext服务组) 外部厂商认证、通知服务 add by ouyy 2011-05-19

#define TK_MSG_PAYIN 0x00017000  // (Pay服务组) 元宝购买服务	add by baiyx 20110609
#define TK_MSG_PAYOT 0x00017400  // (Pay服务组) 元宝支付服务	add by baiyx 20110609

#define TK_MSG_VSPIVOT 0x00017800  // (VS服务组) Pivot服务 add by wuqy 2011.6.17
#define TK_MSG_VSDATA 0x00017C00   // (VS服务组) Data服务  add by wuqy 2011.6.17

#define TK_MSG_COMITSIGNUP 0x00018000            // (Comit服务组) 报名服务 add by ouyy 2011.07.11
#define TK_MSG_COMITAWARD 0x00018400             // (Comit服务组) 颁奖服务 add by ouyy 2011.07.11
#define TK_MSG_COMITPROCESSSTATEDATA 0x00018800  // (Comit服务组) 随机服务 add by ouyy 2011.07.11 -> 组委会过程状态数据服务 pangs modify 2017.10.19
#define TK_MSG_COMITEVENT 0x00018C00             // (Comit服务组) 事件服务 add by ouyy 2011.07.11
#define TK_MSG_COMITCOMM 0x00019000              // (Comit服务组) 通用数据服务 add by ouyy 2011.07.11
#define TK_MSG_COMITCONFIG 0x00019100            // (Comit服务组) 配置服务 add by pangs 2017.03.10
#define TK_MSG_COMITOUT 0x00019400               // (Comit服务组) 外部输出服务 add by ouyy 2011.07.11
#define TK_MSG_COMITREVPROXY 0x00019800          // (Comit服务组) 过程数据反向代理服务 add by pangs 2014.09.19
#define TK_MSG_COMITBETPROXY 0x00019900          // (Comit服务组) 竞彩代理服务 add by pangs 2015.05.28
#define TK_MSG_COMITPOSTDEAL 0x00019A00          //(Comit服务组)后处理服务 add by huangfc 2020.1.13
#define TK_MSG_COMITOUTSIDEAWARD 0x00019B00      //(Comit服务组)外平台颁奖服务 add by huangfc 2020.12.01

#define TK_MSG_BRIDGEIMP 0x00020000          // 桥牌IMP计算服务 add by wuqy 2011.07.27
#define TK_MSG_ROBOTINFOMGR 0x00020200       // 机器人信息管理服务 yangguang02 add by xinxy 2021.3.16
#define TK_MSG_IPUSERTRACE 0x00020400        // IP用户跟踪服务 add by wuqy 2011.08.24
#define TK_MSG_ROBOTDISPATCHCTRL 0x00020600  // 机器人调度控制服务 add by haobc 2021.3.17
#define TK_MSG_RSASECURITY 0x00020800        // RSA安全类服务 add by wuqy 2011.08.30

#define TK_MSG_EXTGOV 0x00020C00  // (Ext服务组) 政府认证服务 add by ouyy 2011-09-29
#define TK_MSG_HISLOC 0x00021000  // (HIS服务组) 用户历史地址服务,记录用户常用IP等等信息 ouyy add 2011.10.26

#define TK_MSG_TGPDB 0x00021400       // (TGP服务组)TGP数据服务  add by ouyy 2011-11-02
#define TK_MSG_TGPSTORE 0x00021500    // (TGP服务组)TGP数据存储服务
#define TK_MSG_TGPBROKER 0x00021800   // (TGP服务组)TGP中转服务  add by ouyy 2011-11-02
#define TK_MSG_TGPLOAD 0x00021C00     // (TGP服务组)TGP承载服务  add by ouyy 2011-11-02
#define TK_MSG_TGPLOGIC 0x00021D00    // (TGP服务组)TGP逻辑服务  added by pangs 2018-04-28
#define TK_MSG_TGPDISPLAY 0x00021E00  // (TGP服务组)TGP展示服务  added by pangs 2018-04-28
#define TK_MSG_TGPCONFIG 0x00021F00   // (TGP服务组)TGP配置服务
#define TK_MSG_TGPCORE 0x00022000     // (TGP服务组)TGP核心业务服务  add by ouyy 2011-11-02
#define TK_MSG_TGPDRIVE 0x00022100    // (TGP服务组)TGP自驱动计算服务
#define TK_MSG_TGPRTC 0x00022200      // (TGP服务组)TGP实时预计算服务 added by pangs 2017-06-08
#define TK_MSG_TGPDETAIL 0x00022300   // (TGP服务组)TGP明细服务

#define TK_MSG_BIZBROKER 0x00022400  // (BIZ服务组)BIZ节点服务 add by ouyy 2011-11-24

#define TK_MSG_DLOCONFIG 0x00022500  // (DLO服务组)DLO配置服务
#define TK_MSG_DLOLOGIC 0x00022600   // (DLO服务组)TDLO逻辑服务

#define TK_MSG_MATCHVIEW 0x00022800  // MatchView服务 add by wuqy 2012-2-1

#define TK_MSG_USEROAUTH 0x00022C00  // (User服务组)User内部鉴权

#define TK_MSG_LOBBYSIGN 0x00023000           // 大厅报名服务 add by wuqy 2012-3-19
#define TK_MSG_LOBBYONLINE 0x00023100         // 大厅在线服务 add by wuqy 2012-3-26
#define TK_MSG_LOBBYPIVOT 0x00023200          // 大厅枢纽服务 add by wuqy 2012-3-26
#define TK_MSG_LOBBYTRACE 0x00023300          // 大厅登录跟踪服务 add by wuqy 2012-3-26
#define TK_MSG_LOBBYONLINECOUNT 0x00023500    //上报转发在线统计数据给监控 add by wanght 2019-11-06
#define TK_MSG_LOBBYONLINESETTING 0x00023600  //大厅在线状态设置服务TKLobbyOnlineSettingService add by wanght 2019-11-11

#define TK_MSG_INFMSG 0x00023400     // INFMSG服务 add by wuqy 2012-3-26
#define TK_MSG_INFTV 0x00023800      // INFTV服务 add by wuqy 2012-3-26
#define TK_MSG_MSGDETAIL 0x00023850  // MSG明细服务

#define TK_MSG_MCR 0x00023C00          // MCardRechargeService服务 baiyx add 2120515
#define TK_MSG_PSEMPRLOGIC 0x00023D00  // PSEMprLogic消息区间起始  fengdh 20161129话费充值逻辑服务

#define TK_MSG_INFCRL 0x00024000  // INFCRL服务 add by wuqy 2012-7-23

#define TK_MSG_WEBTRACE 0x00024400  // 网页访问跟踪服务 add by wuqy 2012-7-30
#define TK_MSG_MSGPROXY 0x00024800  // 南北电信消息代理服务 add by wuqy 2012-8-1

#define TK_MSG_TCSLOGIC 0x00024C00  // (TCS服务组)TCS逻辑服务

#define TK_MSG_SENDEMAIL 0x00025000  // (USR服务组)邮件发送服务
#define TK_MSG_JCC 0x00025400        // (JCC服务组)JJ卡中心服务

#define TK_MSG_ONLINESTAT 0x00025800      // 在线人数数据统计跟踪服务 add by wuqy 2013.3.21
#define TK_MSG_EMAILSENDAGENT 0x00025C00  // 邮件发送代理服务 pangs 2013.3.19
#define TK_MSG_DEMAILBATCH 0x00026000     // 邮件批群发服务 pangs 2013.4.1

#define TK_MSG_USROGW 0x00026400       // USR平台代理层内部请求出口服务 add by lizy 2013-6-19
#define TK_MSG_USEROFFLINE 0x00026500  //User offline add by housl 2021.11.25
#define TK_MSG_OIDOGW 0x00026800       // OpenID平台代理层内部请求出口服务 add by lizy 2013-6-19
#define TK_MSG_OID 0x00026C00          // OpenID平台逻辑承载服务 add by lizy 2013-6-19
#define TK_MSG_OIDDB 0x00027000        // OpenID平台数据服务 add by lizy 2013-6-19

#define TK_MSG_LORDIMP 0x00027400  // 斗地主IMP计算服务 add by lilusj 2013.06.27

#define TK_MSG_RLBCYCLOGIC 0x00027800  // RLBCycleLogic服务 pangs add 2013.7.2

#define TK_MSG_AIDOGW 0x00027C00  // AllyID平台代理层内部请求出口服务
#define TK_MSG_AID 0x00028000     // AllyID平台逻辑承载服务
#define TK_MSG_AIDDB 0x00028400   // AllyID平台数据服务

#define TK_MSG_SSOOGW 0x00028800  // SSO平台代理层内部请求出口服务
#define TK_MSG_SSO 0x00028C00     // SSO平台逻辑承载服务
#define TK_MSG_SSODB 0x00029000   // SSO平台数据服务

#define TK_MSG_DBATCHSRV 0x00029400  // 批处理业务服务

#define TK_MSG_MASTERMATCHINFO 0x00029500   //大师旁观服务 luwq add xinxy 2020.08.12
#define TK_MSG_CUSTOMMATCHCREAT 0x00029600  //自建比赛的创建比赛服务 luwq add xinxy 2020.08.12
#define TK_MSG_CUSTOMTNY 0x00029700         //自建比赛的报名服务 luwq add xinxy 2020.08.12

#define TK_MSG_MATCHMONITER 0x00029800  // MatchMoniter服务add by baiyx 2013.9.3

#define TK_MSG_HISEVT 0x00029C00        // HIS群组 HISEVT  add by baiyx20130730
#define TK_MSG_HEBBROKER 0x0002A000     // HIS群组 HISEVTBROKER  add by baiyx20130906
#define TK_MSG_SNSLOADPROXY 0x0002A400  // (SNS服务组)"关系"负载代理服务 add huangyt 2013.9.13
#define TK_MSG_SNSMGR 0x0002A800        // (SNS服务组)"关系"管理服务 add yangcheng 2013.9.22

#define TK_MSG_PPO 0x0002AC00            // (PAY群组)TKPPOService(PayPreOrder) 支付预订单服务
#define TK_MSG_PSI 0x0002B000            // (PAY群组)TKPSIService(PaySyncIn) 支付同步服务
#define TK_MSG_PSO 0x0002B400            // (PAY群组)TKPSOService(PaySyncOut) 支付对外同步服务
#define TK_MSG_PAYCHDISCOUNT 0x0002B500  // (PAY群组)TKPSOService(PayCHDiscount) 支付渠道优惠服务（查询）
#define TK_MSG_PIF 0x0002B800            // (PAY群组)TKPIFService(PayInterface) 支付信息接口服务（信息查询，处理掉单等）
#define TK_MSG_PAYBATCH 0x0002B900       //TKPayBatchService 充值批处理服务

#define TK_MSG_PCPBROKER 0x0002BA00  //PCP TKPCPBrokerService 泛货币支付节点服务
#define TK_MSG_PCPLOGIC 0x0002BB00   //PCP TKPCPLogicService 泛货币支付逻辑处理服务
#define TK_MSG_PCPISSUE 0x0002BB50   //PCP TKPCPIssueService 泛货币支付金币发放处理服务

#define TK_MSG_RPDLOGIC 0x0002BC00         // 真实财务投放逻辑服务 add by baiyx 2013.12.17
#define TK_MSG_PSEVERIFICATION 0x0002BD00  //PSE Verification消息区间起始
#define TK_MSG_RPDSUPPLIER 0x0002C000      // 真实财务投放第三方服务 add by baiyx 2013.12.17
#define TK_MSG_PSERED 0x0002C100           //PSE 红包消息区间起始 fengdh 2016.10.21
#define TK_MSG_PSEINVOICE 0x0002C200       // PSE发票服务消息区间起始

#define TK_MSG_SNSDATAREDISCACHE 0x0002C400  // (SNS服务组)"关系"数据缓存服务 add yangcheng 2013.12.18

#define TK_MSG_MSPICACHE 0x0002C800    // TKPlayerInfoBroker和TKPlayerInfoHolder Server add by liusj 2014.01.22
#define TK_MSG_RLBBALLOGIC 0x0002CC00  // TKRLBBalLogicService余额排行榜 pangs add 20140304

#define TK_MSG_LOGAGENT 0x0002D000      // (LOG服务组) TKLogAgentService
#define TK_MSG_CLS 0x0002DA00           // (LOG服务组) TKCLSService，条件日志采集服务
#define TK_MSG_LOGCOLLECTOR 0x0002D400  // (LOG服务组) TKLogCollectorService
#define TK_MSG_LOGMANAGER 0x0002D800    // (LOG服务组) TKLogManagerService

#define TK_MSG_HTTPTRANS 0x0002D900   // TKHttpTransService TK服务访问http luwq 2021.4.19
#define TK_MSG_HTTPBROKER 0x0002DB00  // TKHttpBrokerService TK服务访问http luwq 2021.4.19

#define TK_MSG_MBSDK 0x0002DC00  // 移动SDK接口消息 wuqy added 2014.3.28

#define TK_MSG_RPDWS 0x0002E000     // RPD基础消息号 baiyx added 2014.5.12
#define TK_MSG_BET 0x0002E400       // Bet竞猜系统基础消息号 wuqy add 2014.5.20
#define TK_MSG_LOBBYBET 0x0002E800  // 大厅竞猜服务 add by wuqy 2014.5.22

#define TK_MSG_RPDTCARDPWD 0x0002EC00      // RPD第三方卡密管理服务 add by baiyx 2014.5.29
#define TK_MSG_PSESYNC 0x0002ED00          //PSE Sync消息区间起始
#define TK_MSG_RPDTCEXTERNERAL 0x0002F000  // RPD第三方卡密外接服务 add by baiyx 2014.5.29
#define TK_MSG_PSESTORAGE 0x0002F100       // 仓储服务
#define TK_MSG_PSEAFTERSALE 0x0002F200     //售后服务基础协议号

#define TK_MSG_RLBBETLOGIC 0x0002F400    // RLB竞彩逻辑服务 pangs 2014.7.11
#define TK_MSG_RLBSNSLOGIC 0x0002F600    // RLB关系逻辑服务 pangs added 2016.5.26
#define TK_MSG_RLBBACKLOGIC 0x0002F800   // RLB后向逻辑服务 pangs 2014.7.25
#define TK_MSG_RLBGROUPLOGIC 0x0002F900  // RLB GroupLogic 服务 pangs 2016.12.15
#define TK_MSG_RLBDATADETAIL 0x0002FA00  // RLB 数据明细服务 pangs added 2017.11.17

#define TK_MSG_FISH 0x0002FC00              // 捕鱼服务系统基础消息号 wuqy add 2014.8.6
#define TK_MSG_SNSMSGSTORAGE_EX 0x00030000  // 新SNS消息转储服务 黄永涛 2014.9.2
#define TK_MSG_GDCCONVERGENCE 0x00031000    // （全局数据缓群组） 应用汇聚服务 add by pangs 2014.09.19
#define TK_MSG_GDCMETADATA 0x00031300       // （全局数据缓群组） 基础元数据缓存 add by pangs 2015.11.06
#define TK_MSG_GDCMETADATABACK 0x00031500   // （全局数据缓群组） 基础元数据缓存后向 add by pangs 2015.11.06
#define TK_MSG_GDCBROKER 0x00031600         // （全局数据缓群组） GDC Broker add by pangs 2016.12.15

#define TK_MSG_FDCBROKER 0x00031700         // （应用格式据缓群组） FDC Broker add by pangs 2017.05.18
#define TK_MSG_FDCREAD 0x00031800           // （应用格式据缓群组） FDC Read   add by pangs 2017.05.18
#define TK_MSG_FDCWRITE 0x00031900          // （应用格式据缓群组） FDC Write   add by pangs 2017.05.18
#define TK_MSG_FDCCONFIG 0x00031A00         // （应用格式据缓群组） FDC Config add by pangs 2017.05.18
#define TK_MSG_FDCTEMSTATEREAD 0x00031B00   // （应用格式据缓群组） FDC TemStateRead
#define TK_MSG_FDCTEMSTATEWRITE 0x00031C00  // （应用格式据缓群组） FDC TemStateWrite

#define TK_MSG_HISEVTCONFIGSVR 0x00032000       // (HISEVT服务群组） HISEVT配置服务	     add by pangs 2014.11.05
#define TK_MSG_HISEVTDECAYSVR 0x00032400        // (HISEVT服务群组） HISEVT数据衰减服务   add by pangs 2014.11.05
#define TK_MSG_HISEVTUSECASETESTSVR 0x00032800  // (HISEVT服务群组） HISEVT自动测试服务   add by pangs 2014.11.05

#define TK_MSG_GAME_PIVOT 0x00032C00      // GamePivot服务基础消息号 liusj add 2014.12.1
#define TK_MSG_RECHARGE_PIVOT 0x00032D00  // RechargePivot服务基础消息号 liusj add 2014.12.1
#define TK_MSG_GAMEDATA_PIVOT 0x00032E00  // GameDataPivot服务基础消息号 wuqy add 2017.03.14

#define TK_MSG_AOSBROKER 0x00033000  // AOS平台节点服务
#define TK_MSG_AOSAID 0x00033400     // AOS平台AID逻辑承载服务
#define TK_MSG_AOSOID 0x00033800     // AOS平台OID逻辑服务
#define TK_MSG_AOSSID 0x00033B00     // AOS平台SID逻辑服务
#define TK_MSG_AOSDB 0x00033C00      // AOS平台数据服务

#define TK_MSG_MSGBROADCAST 0x00033D00  // MSG群组-广播服务 added by pangs 2014-12-17

#define TK_MSG_SNS_FMS 0x00033E00        // 格式化消息存储服务
#define TK_MSG_SNS_DCQ 0x00033F00        // 通用展示可控队列系统服务
#define TK_MSG_SNS_DCQ_PROXY 0x00034000  // 通用展示可控队列系统代理服务

#define TK_MSG_MSGDEVXCST 0x00034400  // MSG群组-设备播服务 added by pangs 2015-04-02
#define TK_MSG_FISHMATCH 0x00034800   // 捕鱼Match服务相关的消息	wuqy 2015.4.16

#define TK_MSG_AAASECURITYCENTER 0x00034B00       // AAA平台安保中心服务
#define TK_MSG_AAABROKER 0x00034C00               // AAA平台节点服务
#define TK_MSG_USERROLE 0x00034D00                // 用户角色管理服务  housl added 2023.2.13
#define TK_MSG_AAAACCOUNT 0x00035000              // AAA平台ACCOUNT逻辑服务
#define TK_MSG_AAAAUTHENTICATION 0x00035400       // AAA平台AUTHENTICATION逻辑服务
#define TK_MSG_AAAAUTHORIZATION 0x00035800        // AAA平台AUTHORIZATION逻辑服务
#define TK_MSG_AAADB 0x00035C00                   // AAA平台DB数据库代理服务
#define TK_MSG_AAABILLSVR 0x00035D00              // AAA账单服务消息段
#define TK_MSG_AAAPOSTDEALSERVICE 0x00035E00      // AAA后处理服务
#define TK_MSG_AAAWEBOUTBROKERSERVICE 0x00035F00  // AAA访问Web接口代理服务

#define TK_MSG_OSS 0x00036000         // (新HIS服务群组) ObjectStorageService 对象存储服务
#define TK_MSG_GSS 0x00036400         // (新HIS服务群组) GeneralSearchService 通用检索服务
#define TK_MSG_HIS 0x00036800         // (新HIS服务群组) 新历史服务
#define TK_MSG_HISCHECK (TK_MSG_HIS)  // (新HIS服务群组) 基于用户历史行为特征值的异常检测服务
#define TK_MSG_AMA 0x00036900         // 欧若拉业务监控Aurora Monitor Aler add by guopc 2023.5.25
#define TK_MSG_HISSTORE 0x00036A00    // (新HIS服务群组) 历史存储服务
#define TK_MSG_MCA 0x00036B00         // 多触点归因服务  add by lixm	2023.5.24
#define TK_MSG_PAYINFO 0x00036C00     // (Pay服务组)PayInfo服务相关消息

#define TK_MSG_EVMLOGIC 0x00036D00   //EVM逻辑服务
#define TK_MSG_PRICECARD 0x00036E00  //有价卡服务

#define TK_MSG_SDKAGENT 0x00037000   // 移动端联运SDK服务相关的消息	wuqy 2015.10.31
#define TK_MSG_SDGRINFO 0x00037400   // 同方位游戏结果信息服务相关的消息	wuqy 2015.11.2
#define TK_MSG_ECAUSER 0x00037800    // (ECA服务组)用户ECA业务
#define TK_MSG_ECABROKER 0x00037C00  // (ECA服务组)ECABroker
#define TK_MSG_ECAPSE 0x00037D00     // (ECA服务组)ECAPSE
#define TK_MSG_ECAPAY 0x00037E00     // (ECA服务组)ECAPAY

#define TK_MSG_SNSTRANSCACHE 0x00038000        // 关系消息存储服务
#define TK_MSG_SNSMSGQUEUE 0x00038400          // 关系消息队列服务
#define TK_MSG_SNSUSERLOGININFO 0x00038800     // 关系登录信息服务
#define TK_MSG_SNSILLEGALUSERCHECK 0x00038C00  // 关系黑名单验证服务
#define TK_MSG_SNSWEBTRANS 0x00039000          // web直连接口服务
#define TK_MSG_SNSDISPATCH 0x00039400          // 关系业务派发服务
#define TK_MSG_SNSPVT 0x00039800               // 私人关系管理服务
#define TK_MSG_SNSORG 0x00039C00               // 组织关系管理服务
#define TK_MSG_SNSLOGIN 0x0003A000             // 关系用户登录服务
#define TK_MSG_RISBROKER 0x0003A400            // 风控broker服务
#define TK_MSG_RISSRV 0x0003A800               // 风控service
#define TK_MSG_PAYSUBSCRIBE 0x0003A900         //add by fengdh 2023.6.14
#define TK_MSG_PAYSIGN 0x0003AC00              // (Pay服务组）PaySign服务相关消息
#define TK_MSG_PCPGATEWAY 0x0003AD00           // (Pay服务组）PCPGate服务相关信息
#define TK_MSG_PAYDC 0x0003AE00                // (Pay服务组）直冲服务相关信息
#define TK_MSG_PAYMCHMANAGE 0x0003AF00         // (Pay服务组）商户管理服务相关信息

#define TK_MSG_CTRCOLLECTOR 0x0003B000      // (CTR服务组） 数据收集服务
#define TK_MSG_CTRCASSANDRAPLAY 0x0003B200  // (CTR服务组） Cassandra写入服务
#define TK_MSG_CTRHDFSPLAY 0x0003B400       // (CTR服务组） HDFS写入服务
#define TK_MSG_CTROLAPPLAY 0x0003B600       // (CTR服务组） OLAP写入服务
#define TK_MSG_CTRREADER 0x0003B800         // (CTR服务组） 数据读取服务
#define TK_MSG_CTRKAFKADISPATCH 0x0003BB00  // (CTR服务组） Kafka数据分发服务
#define TK_MSG_CTRMSG2DBADAPTER 0x0003BD00  // (CTR服务组） TK消息与DB存储适配服务

#define TK_MSG_CDE 0x0003BD40  // 牌库服务
#define TK_MSG_ACL 0x0003BD60  // AI复式赛牌库服务

#define TK_MSG_CMAAGENT (TK_MSG_LOGAGENT)  // (CMA服务组) CMA信息采集服务
#define TK_MSG_CMAINFO 0x0003BE00          // (CMA服务组) CMA信息查询服务
#define TK_MSG_CMAPRODUCER 0x0003BE20      // (CMA服务组) CMA数据拉取服务 消息起始位置
#define TK_MSG_CMAJUDGER 0x0003BE40        // (CMA服务组) CMA报警判断服务 消息起始位置
#define TK_MSG_CMASENDER 0x0003BE60        // (CMA服务组) CMA报警发送服务 消息起始位置
#define TK_MSG_CMAMANAGER 0x0003BE80       // (CMA服务组) CMA管理服务 消息起始位置
#define TK_MSG_DHLAGENT 0x0003BF00         // (DHL服务组) 日志文件采集服
#define TK_MSG_UDB 0x0003C000              //(MID体系)用户设备行为服务

#define TK_MSG_NOS 0x0003C020          // EVT平台，Nosql开放存储系统
#define TK_MSG_EMPRECV 0x0003C040      // 通知接收服务基础协议号
#define TK_MSG_EMPSEND 0x0003C080      // 通知发送服务基础协议号
#define TK_MSG_SDKSECURITY 0x0003C0C0  // 无线SDK安全管理服务

#define TK_MSG_IPDATA 0x0003C400      // IP数据服务 add by wuqy 2016.11.10
#define TK_MSG_SNS_NEARBY 0x0003C800  // 关系用户位置服务 yangcheng 2016.11.14

#define TK_MSG_CHATCONTROL 0x0003D000      // (聊天室服务组)中控服务 yangcheng 2017.3.1
#define TK_MSG_CHATMASTER 0x0003D200       // (聊天室服务组)主服务
#define TK_MSG_CHATSALVE 0x0003D400        // (聊天室服务组)从服务
#define TK_MSG_CHATPROXY 0x0003D600        // (聊天室服务组)代理服务
#define TK_MSG_CHATMSGQUEUE 0x0003D800     // (聊天室服务组)消息服务
#define TK_MSG_CHATPIVOT 0x0003DA00        // (聊天室服务组)枢纽服务
#define TK_MSG_ECACREDIT 0x0003DB00        // (ECA服务组)荣誉ECA业务 add by baiyx 20170227
#define TK_MSG_CHATLOG 0x0003DC00          // (聊天室服务组)日志存储服务
#define TK_MSG_SMALLFILERECORD 0x0003DD00  // 小文件系统记录服务
#define TK_MSG_CHATGAME 0x0003DE00         // (聊天室服务组)游戏聊天室服务

#define TK_MSG_OPENPLATADDR 0x0003E000     // 开放平台地址服务
#define TK_MSG_OPENPLATPROXY 0x0003E200    // 开放平台代理服务
#define TK_MSG_OPENPLATLOGIC 0x0003E400    // 开放平台逻辑服务
#define TK_MSG_OPENPLATMONITOR 0x0003E600  // 开放平台监测服务
#define TK_MSG_OPENPLATPUSH 0x0003E800     // 开放平台推送服务

#define TK_MSG_BACKENDOPENPROXY 0x0003EA00  // 后端开放平台代理服务
#define TK_MSG_BACKENDOPENLOGIC 0x0003EB00  // 后端开放平台逻辑服务

#define TK_MSG_MLBCONTROL 0x0003F000  // (媒体平台服务组)直播控制服务
#define TK_MSG_MLBMEDIA 0x0003F200    // (媒体平台服务组)直播媒体服务
#define TK_MSG_MLBCHANNAL 0x0003F400  // (媒体平台服务组)直播频道服务
#define TK_MSG_MLBPROYX 0x0003F600    // (媒体平台服务组)直播代理服务

#define TK_MSG_MRCCONTROL 0x00040000     // (媒体平台服务组)实时沟通控制服务
#define TK_MSG_MRCMEDIA 0x00040200       // (媒体平台服务组)实时沟通媒体服务
#define TK_MSG_MRCROOM 0x00040400        // (媒体平台服务组)实时沟通房间服务
#define TK_MSG_MRCPROYX 0x00040600       // (媒体平台服务组)实时沟通代理服务
#define TK_MSG_SNSMTR_SVR 0x00040800     // (关系平台监控服务组)监控服务
#define TK_MSG_SNSMTR_AGENT 0x00040A00   // (关系平台监控服务组)监控代理服务
#define TK_MSG_SNS_DATASYN 0x00040B00    // (关系平台服务组)数据同步服务
#define TK_MSG_CHATROOMLIST 0x00040C00   // (聊天室服务组)房间列表服务
#define TK_MSG_CHATWEBTRANS 0x00040E00   // (聊天室服务组)Web接口服务
#define TK_MSG_SNSMTR_RECORD 0x00040F00  // (关系平台监控服务组)监控记录服务

#define TK_MSG_SPDBROKER 0x00041000     // (传播推广)SPDBroker
#define TK_MSG_SPDLOGIC 0x00041A00      // (传播推广) SPDLogic
#define TK_MSG_SPDDB 0x00041B00         // (传播推广) SPDDB
#define TK_MSG_SPDINTERFACE 0x00041C00  // (传播推广) SPDInterface
#define TK_MSG_SPDEXTERNAL 0x00041D00   // (传播推广) SPDExternal

#define TK_MSG_FISHWINTREABROKER 0x00042000  // 捕鱼夺宝broker服务
#define TK_MSG_FISHWINTREALOTTER 0x00042A00  // 捕鱼夺宝逻辑服务
#define TK_MSG_FISHWINTREAAWARD 0x00042B00   // 捕鱼夺宝奖励服务
#define TK_MSG_FISHMONITOR 0x00042C00        // 捕鱼系监控服务 add by xinxy 2020.5.12

#define TK_MSG_SNSOPT_SVR 0x00043000              // (关系平台运维服务组)运维服务
#define TK_MSG_SNSOPT_AGENT 0x00043200            // (关系平台运维服务组)运维代理服务
#define TK_MSG_SNSDATAPIPE_CENTRE_SVR 0x00043400  // (关系平台服务组)数据同步中心服务
#define TK_MSG_SNSDATAPIPE_BORDER_SVR 0x00043800  // (关系平台服务组)数据同步边缘服务
#define TK_MSG_SNSEXTENDDATA 0x00043900           //关系外延数据服务 huangfc 2021.12.22
#define TK_MSG_SNSEXTENDDATA_PRODUCE 0x00043A00   //关系外延数据生成服务 huangfc 2021.12.22
#define TK_MSG_SNSRECOMMAND 0x00043B00            //关系数据推荐服务 huangfc 2021.12.22

#define TK_MSG_VIPBROKER 0x00044000  // (会员服务组) VIPBroker

#define TK_MSG_PSEBROKER 0x00045000         //PSE节点服务   消息起始位置
#define TK_MSG_PSELOGISTICS 0x00045200      //PSE物流服务   消息起始位置
#define TK_MSG_PSECOMMODITY 0x00045300      //PSE商品服务  消息起始位置
#define TK_MSG_PSEDISCOUNT 0x00045400       //PSE优惠服务  消息起始位置
#define TK_MSG_PSEORDER 0x00045500          //PSE订单服务  消息起始位置
#define TK_MSG_PSEMANAGE 0x00045600         //PSE后台管理服务
#define TK_MSG_PSELOTTERY 0x00045700        //PSE彩票服务
#define TK_MSG_PSEPSEPERSONINFO 0x00045800  //PSE个人信息服务
//2022.4.7 (支付2.0 适用linux)fengdh
#define TK_MSG_PAY_BROKER 0x00045900    //PAY2.0 支付节点服务
#define TK_MSG_PAY_LOGIC 0x00045A00     //PAY2.0 支付逻辑处理服务
#define TK_MSG_PAY_GATEWAY 0x00045B00   //PAY2.0 支付网关服务
#define TK_MSG_PAY_MERCHANT 0x00045B20  //PAY2.0 支付商户服务
#define TK_MSG_PAY_RISKCTRL 0x00045F00  //PAY2.0 支付业务风险控制
#define TK_MSG_PAY_REFUND 0x00045F20    //PAY    充值退款 2022.8.4

#define TK_MSG_DTC_COLLECTOR 0x00046000  // DTC数据接收服务  消息起始位置
#define TK_MSG_DTC_DISPATCH 0x00046200   // DTC数据获取服务  消息起始位置
#define TK_MSG_DTC_MANAGE 0x00046400     // DTC管理服务  消息起始位置

#define TK_MSG_DST_GENERAL 0x00046600  // DST 通用测试服务  消息起始位置

#define TK_MSG_SPABROKER 0x00047000          // 自研产品帐号代理服务  消息起始位置
#define TK_MSG_SPALOGIN 0x00047100           // 自研产品帐号登录服务  消息起始位置
#define TK_MSG_SPAMANAGER 0x00047200         // 自研产品帐号信息管理服务  消息起始位置
#define TK_MSG_SPAREGISTER 0x00047300        // 自研产品帐号注册服务  消息起始位置
#define TK_MSG_SPASSO 0x00047400             // 自研产品帐号SSO服务  消息起始位置
#define TK_MSG_SPATHIRDAUTH 0x00047500       // 自研产品帐号三方授权服务  消息起始位置
#define TK_MSG_SPASECURITYCENTER 0x00047600  // 自研产品帐号安保中心服务  消息起始位置

#define TK_MSG_OIDBROKERSERVICE 0x00048000    // (开放帐号平台)代理服务
#define TK_MSG_OIDLOGICSERVICE 0x00048100     // (开放帐号平台)逻辑服务
#define TK_MSG_OIDPOSTDEALSERVICE 0x00048200  // (开放帐号平台)后处理服务
#define TK_MSG_OIDDBSERVICE 0x00048300        // (开放帐号平台)数据服务

#define TK_MSG_TIDBROKERSERVICE 0x00048500    // (游客帐号平台)代理服务
#define TK_MSG_TIDLOGICSERVICE 0x00048600     // (游客帐号平台)逻辑服务
#define TK_MSG_TIDPOSTDEALSERVICE 0x00048700  // (游客帐号平台)后处理服务
#define TK_MSG_TIDDBSERVICE 0x00048800        // (游客帐号平台)数据服务

#define TK_MSG_RIMBROKERSERVICE 0x00049000    // (角色信息管理平台)代理服务
#define TK_MSG_RIMLOGICSERVICE 0x00049100     // (角色信息管理平台)逻辑服务
#define TK_MSG_RIMPOSTDEALSERVICE 0x00049200  // (角色信息管理平台)后处理服务
#define TK_MSG_RIMDBSERVICE 0x00049300        // (角色信息管理平台)数据服务

#define TK_MSG_USCBROKERSERVICE 0x00049500    // (用户安全中心)代理服务
#define TK_MSG_USCINFOSERVICE 0x00049600      // (用户安全中心)安全信息管理服务
#define TK_MSG_USCCERTSERVICE 0x00049700      // (用户安全中心)安全认证服务
#define TK_MSG_USCBADSERVICE 0x00049800       // (用户安全中心)行为异常检测服务(Behavior Anomaly Detection)
#define TK_MSG_USCPOSTDEALSERVICE 0x00049900  // (用户安全中心)后处理服务
#define TK_MSG_USCDBSERVICE 0x00049A00        // (用户安全中心)数据服务
#define TK_MSG_USCOFFLINESERVICE 0x00049B00   // (用户安全中心)离线处理服务

#define TK_MSG_UMSBROKERSERVICE 0x00050000      // (用户会员平台userMembership)代理服务
#define TK_MSG_UMSLOGICSERVICE 0x00050100       // (用户会员平台userMembership)逻辑服务
#define TK_MSG_UMSACTIONSERVICE 0x00050200      // (用户会员平台userMembership)奖励发放服务
#define TK_MSG_MS2PBOTMGRSERVICE 0x00050300     // 陪打机器人管理服务  2019-7-5 liyao
#define TK_MSG_MS2SNUMBOTMGRSERVICE 0x00050350  // 段号机器人管理服务
#define TK_MSG_MS2MRSS 0x00050400               // 比赛推荐系统服务 add by hehuan 2020.08.26
#define TK_MSG_NEWCROWDFUNDSERVICE 0x00050450   // 比赛新众筹服务 add by hehuan 2022.03.29
#define TK_MSG_GENEITEMIDMGR 0x00050500         // 通用号码管理服务 add by yangguang02 2020.8.28
#define TK_MSG_CROWDFUNDBASESERVICE 0x00050600  // 比赛众筹服务 add by hehuan 2020.11.06
#define TK_MSG_CROWDFUNDINFOSERVICE 0x00050650  // 比赛众筹信息服务 add by hehuan 2021.03.25
#define TK_MSG_DYNARANKSERVICE 0x00050700       // 比赛排名服务 add by liuyang10 2021.06.29
#define TK_MSG_MOMSERVICE 0x00050800            // 比赛消息中间件服务 add by liuyang10 2021.06.29
#define TK_MSG_MIDGAMEMATCH 0x00050900          //中局对战比赛服务 add by yangguang02 2021.9.28
#define TK_MSG_FAMILYBIZMGR 0x00050A00          //比赛家族业务管理服务add by liuyang10 2021.11.23
#define TK_MSG_MATCHMPRANK 0x00050B00           //复式岛屿赛MP排名服务 add by liuyang10 2022.5.17
#define TK_MSG_RANKINGMATCH 0x00050C00          //超嗨自建赛服务 add by fanzh 2022.9.26

#define TK_MSG_PSEPORDER 0x00051000       //PSE订单服务
#define TK_MSG_PSEPPURCHASE 0x00051100    //PSE采购服务
#define TK_MSG_PSEPPAY 0x00051200         //PSE支付服务
#define TK_MSG_PSEPLOGISTICS 0x00051300   //PSE物流服务
#define TK_MSG_PSEPSTORAGE 0x00051400     //PSE仓储服务
#define TK_MSG_PSEPAFTERSALES 0x00051500  //PSE售后服务
#define TK_MSG_PSEPINVOICE 0x00051600     //PSE发票服务

#define TK_MSG_DAG_TS 0x00052100           //(DAG新赛制)用户报名服务
#define TK_MSG_DAG_USMS 0x00052200         //(DAG新赛制)用户状态管理服务
#define TK_MSG_DAG_UMS 0x00052300          //(DAG新赛制)用户匹配服务
#define TK_MSG_DAG_URMS 0x00052400         //(DAG新赛制)用户路由管理服务
#define TK_MSG_DAG_RMS 0x00052500          //(DAG新赛制)游戏管理服务
#define TK_MSG_DAG_MPS 0x00052600          //(DAG新赛制)客户端管理服务
#define TK_MSG_DAG_GS 0x00052700           //(DAG新赛制)游戏服务
#define TK_MSG_DAG_MVS 0x00052800          //(DAG新赛制)赛况服务
#define TK_MSG_DAG_MTS 0x00052900          //(DAG新赛制)监控工具服务
#define TK_MSG_DAG_AGENT 0x00053000        //DAG Agent服务
#define TK_MSG_CREDIT_BROKER 0x00053100    //(用户信用体系)代理服务
#define TK_MSG_CREDIT_LOGIC 0x00053200     //(用户信用体系)逻辑服务
#define TK_MSG_CREDIT_POSTDEAL 0x00053300  //(用户信用体系)后处理服务
#define TK_MSG_DAG_TRANSBROKER 0x00053400  //DAG比赛充值交易业务代理服务，用于连接支付系统和比赛系统 xuefeng 2023.6.5

#define TK_MSG_TPVEXTERNAL 0x00054000  //第三方虚拟物品兑换服务

#define TK_MSG_VERIFYCHECK 0x00055000  //验证码校验服务 信息安全部zhangcx 2020.12.26

#define TK_MSG_PAYSECRETMGR 0x00056000  //第三方Token中控服务

#define TK_MSG_AAA_COMSRV_ACCOUNT 0x00057000         //JJ账号统一安全管理平台-帐号组件
#define TK_MSG_AAA_COMSRV_AUTHENTICATION 0x00057F00  //JJ账号统一安全管理平台-认证组件

#define TK_MSG_SME_BROKERS 0x00059000        //短信平台2.0 Broker服务
#define TK_MSG_SME_LOGICS 0x00059100         //短信平台2.0 逻辑服务
#define TK_MSG_SME_CHANNELS 0x00059200       //短信平台2.0 通道服务
#define TK_MSG_SME_DETAILS 0x00059300        //短信平台2.0 明细服务
#define TK_MSG_SME_SENTINELS 0x00059400      //短信平台2.0 渠道管理服务
#define TK_MSG_DAG_ADDRESS 0x00060000        //DAG 寻址服务
#define TK_MSG_DAG_CONFIG_CENTER 0x00060100  //DAG配置中心 luojj01 2022.10.25

//游戏服务器消息起始定义
#define TK_MSG_GAME_LORDBASE 0x00010100                    //斗地主游戏消息
#define TK_MSG_GAME_MAJIANGBASE 0x00010200                 //麻将游戏消息
#define TK_MSG_GAME_UPGRADEBASE 0x00010300                 //升级游戏消息
#define TK_MSG_GAME_LORD4PBASE 0x00010400                  //4人斗地主游戏消息
#define TK_MSG_GAME_BAOHUANG 0x00010500                    //保皇游戏消息
#define TK_MSG_GAME_THREECARD 0x00010600                   //扎金花游戏消息
#define TK_MSG_GAME_POKER 0x00010700                       //Poker游戏消息
#define TK_MSG_GAME_LORDLZ 0x00010800                      //癞子斗地主游戏消息
#define TK_MSG_GAME_CHESS 0x00010900                       //象棋游戏消息
#define TK_MSG_GAME_CSTUD 0x00010A00                       //休闲五张游戏消息
#define TK_MSG_GAME_POKERTRANS 0x00010B00                  //变形金刚(4张德州)游戏消息
#define TK_MSG_GAME_SPR 0x00010C00                         //猜拳游戏消息
#define TK_MSG_GAME_SNATCH 0x00010D00                      //黄金矿游戏消息
#define TK_MSG_GAME_JUNQI 0x00010E00                       //军棋游戏消息
#define TK_MSG_GAME_LORDHL 0x00010F00                      //欢乐斗地主游戏消息
#define TK_MSG_GAME_GINRUMMY 0x00011000                    //金拉米游戏消息
#define TK_MSG_GAME_BIL8 0x00011100                        //JJ8球游戏消息
#define TK_MSG_GAME_DBUCKLE 0x00011200                     //JJ双扣球游戏消息
#define TK_MSG_GAME_INTERIM 0x00011300                     //JJ卡当游戏消息
#define TK_MSG_GAME_POKERMINI 0x00011400                   //德州Mini游戏消息
#define TK_MSG_GAME_WK 0x00011500                          //JJ挖坑游戏消息
#define TK_MSG_GAME_ANOLE 0x00011600                       //JJ变色龙游戏消息
#define TK_MSG_GAME_CHASEPIG 0x00011700                    //JJ拱猪游戏消息
#define TK_MSG_GAME_BRIDGE 0x00011800                      //JJ桥牌游戏消息
#define TK_MSG_GAME_LORDPT 0x00011900                      //搭档斗地主游戏消息
#define TK_MSG_GAME_RUNFAST 0x00011A00                     //JJ跑得快游戏消息
#define TK_MSG_GAME_DOUNIU 0x00011B00                      //JJ斗牛游戏消息
#define TK_MSG_GAME_GO 0x00011C00                          //JJ围棋游戏消息
#define TK_MSG_GAME_GUANDAN 0x00011D00                     //JJ掼蛋游戏消息
#define TK_MSG_GAME_LORDPK 0x00011E00                      //JJ2人斗地主游戏消息
#define TK_MSG_GAME_MJBJ 0x00011F00                        //JJ北京麻将游戏消息
#define TK_MSG_GAME_SIXI 0x00012000                        //JJ四喜游戏消息
#define TK_MSG_GAME_TIANDK 0x00012100                      //JJ填大坑游戏消息
#define TK_MSG_GAME_POKER2F 0x00012200                     //JJ两副牌德州游戏消息
#define TK_MSG_GAME_FEIXINGQI 0x00012300                   //JJ飞行棋游戏消息
#define TK_MSG_GAME_HORSERACE 0x00012400                   //JJ跑马游戏消息
#define TK_MSG_GAME_MJTDH 0x00012500                       //JJ推倒和麻将游戏消息 add by wuqy 2013.1.17
#define TK_MSG_GAME_BADUGI 0x00012600                      //JJ百得之游戏消息 add by wuqy 2013.5.15
#define TK_MSG_GAME_BINGO 0x00012700                       //JJBingo游戏消息 add by wuqy 2013.9.10
#define TK_MSG_GAME_JJLINE 0x00012800                      //JJLine拉霸（JJ连线）游戏消息 add by wuqy 2013.9.22
#define TK_MSG_GAME_DGZ 0x00012900                         //打滚子游戏消息 add by wuqy 2013.12.30
#define TK_MSG_GAME_MJSCN 0x00012A00                       //新川麻游戏消息 add by wuqy 2013.12.30
#define TK_MSG_GAME_LORDTVBASE TK_MSG_GAME_LORDBASE        //斗地主TV游戏消息 add by wuqy 2014.4.2
#define TK_MSG_GAME_UPGRADETVBASE TK_MSG_GAME_UPGRADEBASE  //升级TV游戏消息 add by wuqy 2014.4.2
#define TK_MSG_GAME_MJBW 0x00012B00                        //JJ血战麻将游戏消息 add by wuqy 2014.6.12
#define TK_MSG_GAME_MJDZ 0x00012C00                        //JJ大众麻将游戏消息 add by wuqy 2014.6.12
#define TK_MSG_GAME_FISH 0x00012D00                        //JJ捕鱼达人游戏消息 add by wuqy 2014.8.4
#define TK_MSG_GAME_LORDBTVBASE TK_MSG_GAME_LORDBASE       //斗地主BTV游戏消息 add by wuqy 2014.8.29
#define TK_MSG_GAME_TREASPOKER TK_MSG_GAME_CSTUD           //JJ宝藏扑克游戏消息 add by wuqy 2014.10.14
#define TK_MSG_GAME_LORDSDBASE TK_MSG_GAME_LORDBASE        //闪电斗地主游戏消息 add by wuqy 2014.10.21
#define TK_MSG_GAME_SLOT 0x00012E00                        //JJ老虎机游戏消息 add by wuqy 2015.1.9
#define TK_MSG_GAME_STAREPOKER 0x00012F00                  //JJ干瞪眼游戏消息 add by wuqy 2015.6.10
#define TK_MSG_GAME_BACCARAT 0x00013000                    //JJ百家乐游戏消息 add by wuqy 2015.6.29
#define TK_MSG_GAME_WZQ 0x00013100                         //JJ五子棋游戏消息 add by wuqy 2015.10.17
#define TK_MSG_GAME_BRPINSHI 0x00013200                    //JJ百人牛牛游戏消息 add by wuqy 2015.10.23
#define TK_MSG_GAME_FRYUPGRADE TK_MSG_GAME_UPGRADEBASE     //炒地皮游戏消息 add by wuqy 2015.10.30
#define TK_MSG_GAME_THIRTEEN 0x00013300                    //JJ十三水游戏消息 add by wuqy 2016.1.19
#define TK_MSG_GAME_MJ3P 0x00013400                        //JJ 3人麻将游戏消息 add by wuqy 2016.3.21(改名为卡五星)
#define TK_MSG_GAME_BALLWAR 0x00013500                     //JJ球球游戏消息 add by wuqy 2016.6.21
#define TK_MSG_GAME_MJFK 0x00013600                        //JJ 疯狂麻将游戏消息 add by wuqy 2016.8.9
#define TK_MSG_GAME_MJSR 0x00013700                        //JJ 三人麻将游戏消息 add by wuqy 2017.2.21
#define TK_MSG_GAME_THREECARDPK 0x00013800                 //拼三张游戏消息 add by wuqy 2017.3.1
#define TK_MSG_GAME_MJFKN 0x00013900                       //JJ 新疯狂麻将游戏消息 add by wuqy 2017.4.20
#define TK_MSG_GAME_KILLING 0x00013A00                     //JJ 狼人杀游戏消息 add by wuqy 2017.6.8
#define TK_MSG_GAME_TETRIS 0x00013B00                      //JJ 俄罗斯方块游戏消息 add by wuqy 2017.6.19
#define TK_MSG_GAME_MJHEBN 0x00013C00                      //JJ 新哈尔滨麻将游戏消息 add by wuqy 2017.7.19
#define TK_MSG_GAME_TANK 0x00013D00                        //JJ 坦克大战游戏消息 add by wuqy 2017.7.20
#define TK_MSG_GAME_BLACKJACK 0x00013E00                   //JJ 海外二十一点游戏消息 add by wuqy 2017.7.20
#define TK_MSG_GAME_FISHQPM3D TK_MSG_GAME_FISH             //JJ移动端3D千炮捕鱼游戏消息 add by wuqy 2017.7.21
#define TK_MSG_GAME_LORDWAR 0x00013F00                     //JJ 地主战争消息 add by wuqy 2017.8.8
#define TK_MSG_GAME_MJHB 0x00014000                        //JJ 河北麻将游戏消息 add by wuqy 2017.9.1
#define TK_MSG_GAME_FISHTV TK_MSG_GAME_FISH                //JJ TV捕鱼游戏消息 add by wuqy 2017.9.8
#define TK_MSG_GAME_MJGUANGD 0x00014100                    //JJ 广东麻将游戏消息 add by wuqy 2017.9.29
#define TK_MSG_GAME_MJZHENGZ 0x00014200                    //JJ 郑州麻将戏消息 add by wuqy 2017.9.29
#define TK_MSG_GAME_MJHANGZ 0x00014300                     //JJ 杭州麻将戏消息 add by wuqy 2017.10.27
#define TK_MSG_GAME_MJZHUANZ 0x00014400                    //JJ 转转麻将戏消息 add by wuqy 2017.10.27
#define TK_MSG_GAME_MJSHENY 0x00014500                     //JJ 沈阳麻将戏消息 add by wuqy 2017.10.27
#define TK_MSG_GAME_JUNQIPK 0x00014600                     //JJ 军棋(移动端2人游戏)游戏消息 add by wuqy 2017.10.27
#define TK_MSG_GAME_LORDBB 0x00014700                      //JJ 百变斗地主游戏消息 add by wuqy 2017.12.5
#define TK_MSG_GAME_JJUMP 0x00014800                       //JJ 跳一跳游戏消息 add by wuqy 2018.1.31
#define TK_MSG_GAME_AEROPLANECHESS 0x00014900              //JJ 飞行棋(新版,移动端)游戏消息 add by wuqy 2018.2.9
#define TK_MSG_GAME_MAHJONGHL 0x00014A00                   //JJ 欢乐麻将游戏消息 add by xinxy 2018.4.11
#define TK_MSG_GAME_PARKOURKING 0x00014B00                 //JJ 跑酷之王游戏消息 add by xinxy 2018.4.13
#define TK_MSG_GAME_MAHJONGHLXL 0x00014C00                 //JJ 欢乐血流麻将游戏消息 add by xinxy 2018.4.17
#define TK_MSG_GAME_FISHQPTV TK_MSG_GAME_FISH              //JJ TV千炮捕鱼游戏消息 add by xinxy 2018.4.17
#define TK_MSG_GAME_JJFOOTBALL 0x00014D00                  //JJFootball JJ足球游戏消息 add by xinxy 2018.4.24
#define TK_MSG_GAME_MJMINI 0x00014E00                      //JJ 迷你麻将游戏消息 add by xinxy 2018.5.7
#define TK_MSG_GAME_MJJSPK 0x00014F00                      //JJ 二人极速麻将消息 add by xinxy  2018.5.7
#define TK_MSG_GAME_ANIMAL 0x00015000                      //JJ 斗兽棋消息 add by xinxy 2018.5.10
#define TK_MSG_GAME_RUNFAST2P 0x00015100                   //JJ 二人跑得快消息 add by xinxy 2018.6.11
#define TK_MSG_GAME_LUCKYJJ 0x00015200                     //JJ 金钩钓鱼 add by xinxy 2018.7.5
#define TK_MSG_GAME_DOUSHIPOKER 0x00015300                 //JJ 斗十扑克 add by xinxy 2018.7.11
#define TK_MSG_GAME_MAHJONGXQ 0x00015400                   //JJ 象棋麻将消息 add by xinxy 2018.7.16
#define TK_MSG_GAME_BIGPOKER 0x00015500                    //JJ 大牌德州消息 add by xinxy 2018.8.22
#define TK_MSG_GAME_HUNTER 0x00015600                      //JJ猎人消息 add by xinxy 2018.10.31
#define TK_MSG_GAME_RENJU 0x00015700                       //JJ星源九珠消息 add by xinxy 2018.11.20
#define TK_MSG_GAME_CRAZYANIM 0x00015800                   //JJ疯狂动物消息 add by xinxy 2019.1.18
#define TK_MSG_GAME_GOBLINMINER 0x00015900                 //地精矿工消息 add by xinxy 2019.4.29
#define TK_MSG_GAME_SUPERTEAM 0x00015A00                   //黄金阵容消息 add by xinxy 2019.6.27
#define TK_MSG_GAME_JJMAGICIAN 0x00015B00                  //猎魔达人 add by xinxy 2019.11.01
#define TK_MSG_GAME_BOMBLORD 0x00015C00                    //炸弹斗地主 add by xinxy 2019.11.01
#define TK_MSG_GAME_MAHJONGHBN 0x00015D00                  //新河北麻将 add by xinxy 2020.03.31
#define TK_MSG_GAME_MAGICALTD 0x00015E00                   //猎魔塔防 add by xinxy 	2020.04.23
#define TK_MSG_GAME_MAHJONGZJ 0x00015F00                   //贵阳捉鸡麻将 add by xinxy 2020.04.29
#define TK_MSG_GAME_MAHJONGSYN 0x00016100                  //新沈阳麻将 add by xinxy 2020.5.15
#define TK_MSG_GAME_MAHJONGKWXN 0x00016200                 //新卡五星麻将 add by xinxy 2020.5.15
#define TK_MSG_GAME_JUNQITT 0x00016300                     //军棋头条 add by xinxy 2020.6.9
#define TK_MSG_GAME_MJJPH 0x00016500                       //广东鸡平胡麻将由于VSLobby使用了0x00016400 add by xinxy 2020.7.2
#define TK_MSG_GAME_MJNANJ 0x00016600                      //南京麻将 add by xinxy 2020.7.23
#define TK_MSG_GAME_MAHJONGHANGZ 0x00016700                //九天项目部杭州麻将 add by xinxy 2020.8.5
#define TK_MSG_GAME_MJWUHAN 0x00016800                     //九天项目部武汉麻将 add by xinxy 2020.8.6
#define TK_MSG_GAME_TOWERBATTLE 0x00016900                 //棋牌产品中心塔防对战 add by xinxy 2020.8.12
#define TK_MSG_GAME_MJSHANGH 0x00016A00                    //九天项目部上海麻将 add by xinxy 2020.8.24
#define TK_MSG_GAME_MJCC 0x00016B00                        //麻将项目部长春麻将 add by xinxy 2020.9.4
#define TK_MSG_GAME_MJSHAND 0x00016D00                     //九天项目部山东麻将 add by xinxy 2020.10.9(0x00016C00 被外部厂商认证服务使用)
#define TK_MSG_GAME_MJKOUD 0x00016E00                      //九天项目部扣点麻将 add by xinxy 2020.10.9
#define TK_MSG_GAME_MJHAEB 0x00016F00                      //九天项目部哈尔滨麻将 add by xinxy 2020.10.14
#define TK_MSG_GAME_MAHJONGZHENGZ 0x00017100               //九天项目部郑州麻将 add by xinxy 2020.10.16
#define TK_MSG_GAME_MJNANC 0x00017200                      //九天项目部南昌麻将 add by xinxy 2020.10.16
#define TK_MSG_GAME_MJYIBIN 0x00017300                     //九天项目部宜宾麻将 add by xinxy 2020.11.21
#define TK_MSG_GAME_MJDALIAN 0x00017400                    //九天项目部大连麻将 add by xinxy 2020.11.23
#define TK_MSG_GAME_MJJDN 0x00017500                       //棋牌中心麻将项目部 add by xinxy 2020.12.24
#define TK_MSG_GAME_MJHEFEI 0x00017600                     //九天项目部合肥麻将 add by xinxy 2021.1.6
#define TK_MSG_GAME_MJCHSHA 0x00017700                     //九天项目部长沙麻将 add by xinxy 2021.1.6
#define TK_MSG_GAME_MJBEIJ 0x00017900                      //九天项目部北京麻将 add by xinxy 2021.1.13
#define TK_MSG_GAME_MJTIANJ 0x00017A00                     //九天项目部天津麻将 add by xinxy 2021.1.13
#define TK_MSG_GAME_CDMJXUEZHAN 0x00017B00                 //九天项目部成都血战麻将 add by xinxy 2021.1.21
#define TK_MSG_GAME_CDMJYIBIN 0x00017C00                   //九天项目部四川宜宾麻将 add by xinxy 2021.1.21
#define TK_MSG_GAME_CDMJXUELIU 0x00017D00                  //九天项目部血流麻将 add by xinxy 2021.1.25
#define TK_MSG_GAME_MAHJONGHAIN 0x00017E00                 //九天项目部海南麻将 add by xinxy 2021.2.25
#define TK_MSG_GAME_HXLZD 0x00017F00                       //产品研发中心幻想隆中对 此ID目前没有使用 add by wangchi 2021.8.12
#define TK_MSG_GAME_GODSWORLD 0x00018100                   //产品研发中心神御五行 此ID目前没有使用 add by liuning01 2021.8.13
#define TK_MSG_GAME_LORDHZ 0x00018200                      //产品研发中心混子斗地主 add by luwq 2021.8.18
#define TK_MSG_GAME_MJHZXL 0x00018300                      //产品研发中心红中血流麻将 add by liuhb01 2021.9.3
#define TK_MSG_GAME_LORDDD 0x00018500                      //产品研发中心搭档斗地主 add by luwq 2021.10.21
#define TK_MSG_GAME_MJNCHONG 0x00018600                    //九天项目部南充麻将 add by shaoxj 2022.2.25
#define TK_MSG_GAME_GUANDANCD 0x00018700                   //九天项目部掼蛋成都 add by zhangjs04 2022.3.18
#define TK_MSG_GAME_MJLUZHOU 0x00018900                    //九天项目部泸州麻将 add by shaoxj 2022.4.3
#define TK_MSG_GAME_MJHONGZXL 0x00018A00                   //成都微信麻将小游戏 add by shaoxj 2022.5.16
#define TK_MSG_GAME_MJJISU 0x00018B00                      //九天项目部急速麻将 add by zengchao 2022.5.16
#define TK_MSG_GAME_MJGUIL 0x00018D00                      //九天项目部桂柳麻将 add by dengbw 2022.5.27
#define TK_MSG_GAME_MJFKXL 0x00018E00                      //产品研发中心疯狂血流麻将 add by liuhb01 2022.6.27
#define TK_MSG_GAME_MJCDXZ 0x00018F00                      //九天项目部成都血战麻将 add by shaoxj 2022.6.29
#define TK_MSG_GAME_MJGBXZ 0x00019200                      //产品研发中心国标血战麻将 add by yuanwx 2022.7.29
#define TK_MSG_GAME_MINIHLLORDBXP 0x00019300               //小游戏项目部欢乐不洗牌地主 add by shaoxj 2022.8.4
#define TK_MSG_GAME_MINIHLLORDLZ 0x00019500                //小游戏项目部欢乐连炸地主 add by shaoxj 2022.8.4
#define TK_MSG_GAME_MINIHLLORDJD 0x00019600                //小游戏项目部欢乐经典地主 add by shaoxj 2022.8.4
#define TK_MSG_GAME_MJCDXL 0x00019700                      //小游戏项目部成都血流麻将 add by shaoxj 2022.8.8
#define TK_MSG_GAME_LONGFENGCARD 0x00019800                //九天项目部龙凤牌 add by shaoxj 2022.9.22
#define TK_MSG_GAME_MJHLXZ 0x00019900                      //九天项目部欢乐血战麻将 add by shaoxj 2022.10.12
#define TK_MSG_GAME_MJCLASSICT 0x00019A00                  //九天项目部新经典二麻 add by shaoxj 2022.10.21
#define TK_MSG_GAME_LORDDB 0x00019B00                      //产品研发中心JJ多宝牌 add by zhangkh01 2022.12.6
#define TK_MSG_GAME_MINILORDJDFD 0x00019C00                //小游戏项目部经典斗好友房 add by chenke 2023.1.31
#define TK_MSG_GAME_MINILORDBXPFD 0x00019D00               //小游戏项目部经典斗不洗牌好友房 add by chenke 2023.1.31
#define TK_MSG_GAME_CRAZYSTARE 0x00019E00                  //九天项目部疯狂干瞪眼 add by chentw02 2023.2.23
#define TK_MSG_GAME_MJYJ 0x00019F00                        //九天项目部幺鸡麻将 add by chentw02 2023.4.11
#define TK_MSG_GAME_NEVERLAND 0x0001A000                   //棋牌产品中心-竞娱组千炮捕鱼-梦幻岛 add by wangchi 2023.4.21
#define TK_MSG_GAME_MJKF 0x0001A100                        //棋牌产品中心开封麻将  add by liuhb01 2023.5.25
#define TK_MSG_GAME_GOBANG 0x0001A200                      //九天项目部 五子棋  add by chenke 2023.7.13
#define TK_MSG_GAME_NEVERLANDS 0x0001A300                  //棋牌产品中心-梦幻捕鱼 add by wangchi 2023.8.16
#define TK_MSG_GAME_LORDDJ 0x0001A400                      //棋牌产品中心道具斗地主 add by linxh 2023.9.14
#define TK_MSG_GAME_MJSG 0x0001A500                        //棋牌产品中心三国麻将 add by liuhb01 2023.10.12
#define TK_MSG_GAME_MJGD100 0x0001A600                     //九天项目部 广东100张 add by chentw02 2023.11.13
#define TK_MSG_GAME_MJTS 0x0001A700                        //棋牌产品中心唐山麻将 add by wangcx 2023.12.4
#define TK_MSG_GAME_MJ2MEN 0x0001A800                      //九天项目部 2门麻将 add by chentw02 2023.12.28
#define TK_MSG_GAME_MJQWXL 0x0001A900                      //棋牌产品趣味血流麻将 add by liuhb01 2024.1.22
#define TK_MSG_GAME_MJFEIXJ 0x0001AA00                     //九天项目部云南飞小鸡麻将 add by chentw02 2024.2.28
#define TK_MSG_GAME_MJFUZHOU 0x0001AB00                    //九天项目部福州麻将 add by chentw02 2024.3.12
#define TK_MSG_GAME_MJHUASHUI 0x0001AC00                   //九天项目部划水麻将 add by chentw02 2024.4.15
#define TK_MSG_GAME_MJYIKX 0x0001AD00                      //成都平台棋牌 一口香麻将 add by chentw02 2024.6.17
#define TK_MSG_GAME_MJLYGC 0x0001AE00                      //成都平台棋牌 洛阳杠次麻将 add by chentw02 2024.7.29

#define TK_MSG_GAME_XIYOUJIE 0x00020100   //西游劫游戏消息
#define TK_MSG_GAME_UNO 0x00020300        //尤诺游戏消息
#define TK_MSG_GAME_CAPFISH 0x00020400    //捕鱼达人游戏消息
#define TK_MSG_GAME_SKR 0x00020500        //跑跑龟游戏消息
#define TK_MSG_GAME_DOMINOCLC 0x00020700  //海外印尼Domino classic add by busy 2021.6.30
#define TK_MSG_GAME_DOMINOBET 0x00020900  //海外印尼Domino bet add by busy 2021.6.30
#define TK_MSG_GAME_DOMINOQIU 0x00020A00  //海外印尼Domino Qiu add by busy 2021.6.30
#define TK_MSG_GAME_BANDARQQ 0x00020B00   //海外BandarQQ add by busy 2022.2.28
#define TK_MSG_GAME_REMI 0x00020C00       //海外Remi add by busy 2022.11.24
#define TK_MSG_GAME_LUDO 0x00020D00       //海外Ludo add by busy 2023.04.10

//网络连接类型定义(服务器连出)
#define TK_CONNECTIONTYPE_UNKNOW 0              // 未知
#define TK_CONNECTIONTYPE_DATASVR 1             // (Data服务组) Data全局数据服务
#define TK_CONNECTIONTYPE_MATCHSVR 2            //
#define TK_CONNECTIONTYPE_LOBBYSVR 3            //
#define TK_CONNECTIONTYPE_GAMESVR 4             //
#define TK_CONNECTIONTYPE_GAMEBOTSVR 5          //
#define TK_CONNECTIONTYPE_DOWNLOAD 6            //
#define TK_CONNECTIONTYPE_ROUNDINFOSVR 7        // 对局信息服务（原旁观服务，用于旁观对局游戏过程 wuqy 2014.8.29）
#define TK_CONNECTIONTYPE_WEBLANCOM 8           // (Data服务组) 内网管理Com组件
#define TK_CONNECTIONTYPE_CHATSVR 9             //
#define TK_CONNECTIONTYPE_GROWSVR 10            // (MGW服务组)积分服务，由从未使用的TK_CONNECTIONTYPE_PROPSVR道具更改而来 wuqy 2017.12.27
#define TK_CONNECTIONTYPE_IMSVR 11              //
#define TK_CONNECTIONTYPE_ADVERTSVR 12          //
#define TK_CONNECTIONTYPE_BETSVR 13             //
#define TK_CONNECTIONTYPE_MASTERSVR 14          //
#define TK_CONNECTIONTYPE_AVATARSVR 15          // Avatar
#define TK_CONNECTIONTYPE_MONEYSVR 16           // (MGW服务组) 虚拟金融服务
#define TK_CONNECTIONTYPE_WEBCOMM 17            // (Data服务组)外网管理Com组件
#define TK_CONNECTIONTYPE_SYSBGMGRSVR 18        // 系统后台管理
#define TK_CONNECTIONTYPE_REALPAYSVR 19         // (Pay服务组) 真实货币付费
#define TK_CONNECTIONTYPE_MONITORSVR 20         // 监控
#define TK_CONNECTIONTYPE_HISTORYSVR 21         // 游戏历史纪录服务
#define TK_CONNECTIONTYPE_WARESVR 22            // (MGW服务组)物品服务
#define TK_CONNECTIONTYPE_AUTHENTICATIONSVR 23  // 用户鉴权服务
#define TK_CONNECTIONTYPE_DAEMONSRV 24          // 守护服务

#define TK_CONNECTIONTYPE_DYNCCONFIGSRV 25  // (Data服务组)动态配置服务
#define TK_CONNECTIONTYPE_COMMITTEESRV 26   // (Comit服务组) 组委会服务
#define TK_CONNECTIONTYPE_SALEAGENCYSRV 27  // 分销商管理服务

#define TK_CONNECTIONTYPE_TIMETOURNEYSRV 28   // 定点赛ini文件服务 wuqy add 2008.5.18
#define TK_CONNECTIONTYPE_CHATSRV 29          // 聊天服务 baiyx add at 2008-05-20
#define TK_CONNECTIONTYPE_MARKERSRV 30        // (HIS服务组) 里程碑数据服务 baiyx add at 2008-06-12
#define TK_CONNECTIONTYPE_CHANNELSRV 31       // 频道服务 ouyy add 2008.06.30
#define TK_CONNECTIONTYPE_MARKETEXTENDSRV 32  // (BIZ服务组) 市场推广服务 ouyy add 2008.08.12
#define TK_CONNECTIONTYPE_MATCHSTORYSRV 33    // 比赛展播服务 add by wuqy add at 2008-08-18

#define TK_CONNECTIONTYPE_SNSDATASRV 34   // (SNS服务组)“关系” 数据服务(SNS -- Social Network System) ouyy add 2008.11.06
#define TK_CONNECTIONTYPE_SNSPIVOTSRV 35  // (SNS服务组)“关系” 枢纽服务(SNS -- Social Network System) ouyy add 2008.11.06
#define TK_CONNECTIONTYPE_SNSLOADSRV 36   // (SNS服务组)“关系” 负载服务(SNS -- Social Network System) ouyy add 2008.11.06

#define TK_CONNECTIONTYPE_UTSVR 37           // 小黑桌服务 2008.12.12, wt
#define TK_CONNECTIONTYPE_MATCHRESULTSRV 38  // 比赛过程选手结果排名服务，用于跟踪用户比赛排名信息，比赛故障后可以提取补偿用户名单 2008.12.18, wuqy
#define TK_CONNECTIONTYPE_MATCHCTRLSRV 39    // 比赛管理控制服务 2009.1.16, wuqy
#define TK_CONNECTIONTYPE_USERTRIGGERSVR 40  // 用户状态触发服务 2009.2.20 baiyx
#define TK_CONNECTIONTYPE_STATSVR 41         // 统计服务 2009.3.18 wuqy

#define TK_CONNECTIONTYPE_SNSAPPDATASRV 42    // (SNS服务组) “关系” 应用数据服务(SNS -- Social Network System) ouyy add 2008.03.23
#define TK_CONNECTIONTYPE_ANTIWALLOWSRV 43    // 防沉迷服务 wuqy add 2009.04.8
#define TK_CONNECTIONTYPE_MODULEUPDATESRV 44  // 客户端模块更新服务 wuqy add 2009.05.15

#define TK_CONNECTIONTYPE_SNSTOURNEYSRV 45  // SNS自定义比赛服务 ouyy add 2009.05.31

#define TK_CONNECTIONTYPE_TNYINFOSRV 46     // Tourney信息服务 wuqy add 2009.06.23
#define TK_CONNECTIONTYPE_MATCHINFOSRV 47   // Match信息服务 wuqy add 2009.06.23
#define TK_CONNECTIONTYPE_WEBTOURNEYSRV 48  // WebTourney服务 wuqy add 2009.09.07

#define TK_CONNECTIONTYPE_MATCHPOSSRV 49  // 用户比赛位置服务 wuqy add 2009.12.24

#define TK_CONNECTIONTYPE_HISRECORDERSRV 50  // (HIS服务组)历史记录服务 baiyx add 2010.01.21
#define TK_CONNECTIONTYPE_THRESHOLDSRV 51    // (Data服务组) "Threshold数据阈值"服务 add by ouyy 2010.02.25
#define TK_CONNECTIONTYPE_HISBROKERSRV 52    // (HIS服务组)历史broker服务 baiyx add 2010.03.10

#define TK_CONNECTIONTYPE_CUSTOMSERVICE 53  // 客服平台服务器 add by chengl 2010.03.31

#define TK_CONNECTIONTYPE_INFOCENTERSRV 54  // (Data服务组) "信息中心" 服务 add by ouyy 2010.03.31
#define TK_CONNECTIONTYPE_TRIGGERSRV 55     // (Data服务组) "Trigger数据触发" 服务 add by ouyy 2010.03.31

#define TK_CONNECTIONTYPE_STAFFLOGINSERVICE 56  // 客服平台服务 用于内网客服登录 add by chengl 2010.04.09

#define TK_CONNECTIONTYPE_BRDELAY 57  // 转播延迟服务 add by wuqy 2010.04.30

#define TK_CONNECTIONTYPE_VIRTUALPAYSVR 58  // (Pay服务组) 虚拟支付服务 add by wuqy 2010.05.11

#define TK_CONNECTIONTYPE_IPINFOSRV 59  // IP信息服务

#define TK_CONNECTIONTYPE_ROOMINFOSRV 60  // 房间信息服务 add by wuqy 2010.6.7

//#define TK_CONNECTIONTYPE_SMSCHANNELSRV         61    // 短信通道服务 add by baiyx 2010.06.28
//#define TK_CONNECTIONTYPE_DATASAFECENTER        62    // (Data服务组) Data safe center 服务 add by ouyy 2010.06.28

#define TK_CONNECTIONTYPE_USER 61          // (User服务组)User数据服务
#define TK_CONNECTIONTYPE_USERREGISTER 62  // (User服务组)User注册服务
#define TK_CONNECTIONTYPE_USERAUTH 63      // (User服务组)User内部鉴权
#define TK_CONNECTIONTYPE_USERINFO 64      // (User服务组)User信息处理

//#define TK_CONNECTIONTYPE_BIZDEAL               65  // (BIZ服务组)BIZ业务
#define TK_CONNECTIONTYPE_BIZPRESENT 65  // (BIZ服务组)系统赠送服务
#define TK_CONNECTIONTYPE_BIZSALARY 66   // (BIZ服务组)系统发放用户工资服务 add by baiyx 2010.08.03
//#define TK_CONNECTIONTYPE_BIZPROFIT               67  // (BIZ服务组)合作方利润结算服务
#define TK_CONNECTIONTYPE_BIZBROKER 67  // (BIZ服务组)BIZ节点服务

#define TK_CONNECTIONTYPE_ECAWEB 68     // (ECA服务组)网站ECA业务
#define TK_CONNECTIONTYPE_ECATGP 69     // (ECA服务组)网站ECA业务
#define TK_CONNECTIONTYPE_ECACREDIT 70  // (ECA服务组)荣誉系统

#define TK_CONNECTIONTYPE_MOBILECONNSRV 71   // 移动连接服务 add by chengliang 2010-11-09
#define TK_CONNECTIONTYPE_MOBILEAGENTSRV 72  // 移动用户代理服务 add by chengliang 2010-11-09

#define TK_CONNECTIONTYPE_VSNODESRV 73   // (VS服务组)对战节点服务  add by wuqy 2010-11-11
#define TK_CONNECTIONTYPE_VSLOBBYSRV 74  // (VS服务组)VS大厅服务  add by wuqy 2011-4-11
#define TK_CONNECTIONTYPE_VSROOMSRV 75   // (VS服务组)VSRoom服务  add by wuqy 2011-4-11

#define TK_CONNECTIONTYPE_EXTERNALSRV 76   // (Ext服务组) 外部厂商认证、通知服务 add by ouyy 2011-05
#define TK_CONNECTIONTYPE_STATQUERYSVR 77  // 统计查询服务 2011.6.3 wuqy
#define TK_CONNECTIONTYPE_EXTGOVSRV 78     // (Ext服务组) 政府官方认证、通知服务 add by ouyy 2011-09-29

#define TK_CONNECTIONTYPE_DATASAFECENTER 80  // (Data服务组) Data safe center 服务 add by ouyy 2010.06.28
#define TK_CONNECTIONTYPE_SMSCHANNELSRV 81   // (Data服务组) 短信通道服务 add by baiyx 2010.06.28
#define TK_CONNECTIONTYPE_DOFFLINEMSGSRV 82  // (Data服务组) 离线消息服务 add by ouyy 2010.12.17
#define TK_CONNECTIONTYPE_DPAYPROXYSRV 83    // (Data服务组) 充值代理服务 add by ouyy 2010.12.30
#define TK_CONNECTIONTYPE_DBATCHSRV 84       // (Data服务组) 批处理业务服务 add by ouyy 2010.04.11

#define TK_CONNECTIONTYPE_PAYINSVR 85  // (Pay服务组） 元宝购买服务	add by baiyx 20110609
#define TK_CONNECTIONTYPE_PAYOTSVR 86  // (Pay服务组） 元宝支付服务	add by baiyx 20110609

#define TK_CONNECTIONTYPE_VSPIVOT 87  // (VS服务组)节点服务 add by wuqy 2011.6.17
#define TK_CONNECTIONTYPE_VSDATA 88   // (VS服务组)数据服务 add by wuqy 2011.6.17

#define TK_CONNECTIONTYPE_PINFOSRV 89  // (Pay服务组） 订单查询服务	add by baiyx 20150720
#define TK_CONNECTIONTYPE_ECAUSER 90   // (ECA服务组）用户ECA业务

#define TK_CONNECTIONTYPE_COMITSIGNUP 91            // (Comit服务组) 报名服务 add by ouyy 2011.07.11
#define TK_CONNECTIONTYPE_COMITAWARD 92             // (Comit服务组) 颁奖服务 add by ouyy 2011.07.11
#define TK_CONNECTIONTYPE_COMITPROCESSSTATEDATA 93  // (Comit服务组) 过程状态数据服务
#define TK_CONNECTIONTYPE_COMITEVENT 94             // (Comit服务组) 事件服务 add by ouyy 2011.07.11
#define TK_CONNECTIONTYPE_COMITCOMM 95              // (Comit服务组) 通用数据服务 add by ouyy 2011.07.11
#define TK_CONNECTIONTYPE_COMITOUT 96               // (Comit服务组) 外部输出服务 add by ouyy 2011.07.11
#define TK_CONNECTIONTYPE_COMITREVPROXY 97          // (Comit服务组) 过程数据反向代理服务 add by pangs 2014.09.19
#define TK_CONNECTIONTYPE_COMITBETPROXY 98          // (Comit服务组) 竞彩代理服务 add by pangs 2015.05.28
#define TK_CONNECTIONTYPE_COMITCONFIG 99            // (Comit服务组) 配置服务 add by pangs 2017.03.10
// 97-99 请留给组委会服务组

#define TK_CONNECTIONTYPE_DOWNLOADCTLSVR 100  // 客户端更新下载控制服务
#define TK_CONNECTIONTYPE_ECAPAY 101          // (ECA服务组) 充值ECA add by ouyy 2011.07.18
#define TK_CONNECTIONTYPE_BRIDGEIMPSVR 102    // 桥牌IMP计算服务 wuqy add 2011.7.27
#define TK_CONNECTIONTYPE_IPUSERTRACESVR 103  // IP用户跟踪服务 wuqy add 2011.8.24
#define TK_CONNECTIONTYPE_RSAGENESVR 104      // RSA生成服务 wuqy add 2011.8.30
#define TK_CONNECTIONTYPE_RSACALCSVR 105      // RSA计算服务 wuqy add 2011.8.30

#define TK_CONNECTIONTYPE_HISLOCSVR 106  // (HIS服务组) 用户历史地址服务,记录用户常用IP等等信息 ouyy add 2011.10.26

#define TK_CONNECTIONTYPE_BIZBROKERSVR 107  // (BIZ服务组)BIZ节点服务add by ouyy 2011-11-24

#define TK_CONNECTIONTYPE_MATCHVIEWSRV 108    //  MatchView信息服务 wuqy add 2012.12.14
#define TK_CONNECTIONTYPE_LOBBYSIGN 109       //  大厅报名服务 wuqy add 2012.3.19
#define TK_CONNECTIONTYPE_LOBBYONLINE 110     //  大厅在线服务 wuqy add 2012.3.26
#define TK_CONNECTIONTYPE_LOBBYTRANS 111      //  大厅消息透传服务， 临时用于处理来自后台服务的消息 wuqy add 2012.3.26
#define TK_CONNECTIONTYPE_LOBBYTRACE 112      //  大厅登录跟踪服务 wuqy add 2012.3.26
#define TK_CONNECTIONTYPE_TABLEINFOSRV 113    //  Table信息服务 wuqy add 2012.05.28
#define TK_CONNECTIONTYPE_ISLANDINFOSRV 114   //  Island信息服务 wuqy add 2012.06.14
#define TK_CONNECTIONTYPE_WEBTRACE 115        //  网页访问跟踪服务 add by wuqy 2012-7-30
#define TK_CONNECTIONTYPE_MSGPROXYSRV 116     //  南北电信消息代理服务 add by wuqy 2012-8-1
#define TK_CONNECTIONTYPE_CLTNETPROXYSRV 117  //  客户端网络消息代理服务 add by wuqy 2012-12-18
#define TK_CONNECTIONTYPE_ONLINESTATSRV 118   //  在线人数数据统计跟踪服务 add by wuqy 2013.3.21
#define TK_CONNECTIONTYPE_WEBAGENTSRV 119     //  网页比赛代理服务 add wuqy 2013.5.29

#define TK_CONNECTIONTYPE_INFMSGSRV 120  //  INFMSG服务 wuqy add 2012.3.26
#define TK_CONNECTIONTYPE_INFTVSRV 121   //  INFTV服务 wuqy add 2012.3.26

#define TK_CONNECTIONTYPE_MCRSRV 122                                      // MCardRechargeService服务 baiyx add 2120515
#define TK_CONNECTIONTYPE_INFCRLSRV 123                                   //  INFCRL服务(排行榜服务) wuqy add 2012.7.23
#define TK_CONNECTIONTYPE_ECAPRESENTSRV 124                               //  ECA赠送服务 wuqy add 2012.8.2
#define TK_CONNECTIONTYPE_STATDBSVR 125                                   // 统计数据访问服务 2012.8.31 wuqy
#define TK_CONNECTIONTYPE_STATBROKERSVR 126                               // 统计Broker服务 2012.8.31 wuqy
#define TK_CONNECTIONTYPE_STATAGENTSVR (TK_CONNECTIONTYPE_STATBROKERSVR)  // 统计Agent服务,Broker子节点 2016.8.25 wuqy
#define TK_CONNECTIONTYPE_STATUNISVR 127                                  // 统计独立ID 2012.8.31 wuqy
#define TK_CONNECTIONTYPE_AWARDTRANSSVR 128                               // 颁奖消息传递服务器，原颁奖推送Broker服务(Award Push Broker) 2012.9.4 wuqy
#define TK_CONNECTIONTYPE_STATPERFSVR 129                                 // 性能统计服务 2014.5.6 wuqy

#define TK_CONNECTIONTYPE_SENDEMAIL 130           // (USR服务组)邮件发送服务
#define TK_CONNECTIONTYPE_JCC 131                 // (JCC服务组)JJ卡中心服务
#define TK_CONNECTIONTYPE_EMAILSENDAGENT 132      // 邮件发送代理服务 pangs 2013.3.19
#define TK_CONNECTIONTYPE_DEMAILBATCH 133         // 邮件批群发服务 pangs 2013.4.1
#define TK_CONNECTIONTYPE_SNSMSGSTORAGE 134       // SNS消息转储服务 liwei01 2013.4.9
#define TK_CONNECTIONTYPE_LORDIMPSVR 135          // 斗地主IMP计算服务 liusj add 2013.6.27
#define TK_CONNECTIONTYPE_RLBCYCLOGIC 136         // RLBCycleLogic服务 pangs add 2013.7.2
#define TK_CONNECTIONTYPE_HISEVTSVR 137           // HISEVT baiyx add 20130730
#define TK_CONNECTIONTYPE_HEBBROKERSVR 138        // HISEVT 后端Broker baiyx add 20130906
#define TK_CONNECTIONTYPE_SMECATSRV 139           // 短信猫 pangs added 20140103
#define TK_CONNECTIONTYPE_RLBBALLOGICSRV 140      // TKRLBBalLogicService余额排行榜 pangs added 20140304
#define TK_CONNECTIONTYPE_RLBBETLOGICSRV 141      // TKRLBBetLogicService竞猜排行榜 pangs 2014.7.11
#define TK_CONNECTIONTYPE_RLBBACKLOGICSRV 142     // TKRLBBackLogicService后向逻辑处理服务 pangs 2014.7.25
#define TK_CONNECTIONTYPE_TKECABROKERSRV 143      // TKECABrokerService服务
#define TK_CONNECTIONTYPE_TKRLBSNSLOGICSRV 144    // TKRLBSNSLogicService服务 pangs added 2016.5.26
#define TK_CONNECTIONTYPE_TKRLBGROUPLOGICSRV 145  // RLB 分组排行榜服务 pangs added 2016.12.15
#define TK_CONNECTIONTYPE_TKRLBDATADETAILSRV 146  // RLB 数据明细服务 pangs added 2017.11.17
#define TK_CONNECTIONTYPE_USERROLESVR 147         //用户角色管理服务 housl added 2023.2.13

#define TK_CONNECTIONTYPE_MATCHMONITERSVR 150  // MatchMoniter服务add by baiyx 2013.9.3
#define TK_CONNECTIONTYPE_MAINTAINSVR 151      // Maintain运维服务add by wuqy 2013.9.9
#define TK_CONNECTIONTYPE_SNSLOADPROXYSRV 152  // (SNS服务组)"关系"负载代理服务 add huangyt 2013.9.13
#define TK_CONNECTIONTYPE_SNSMGR 153           // (SNS服务组)"关系"管理服务 add yangcheng 2013.9.22

#define TK_CONNECTIONTYPE_RPDLOGIC 154     // 真实财务投放逻辑服务 add by baiyx 2013.12.17
#define TK_CONNECTIONTYPE_RPDSUPPLIER 155  // 真实财务投放第三方服务 add by baiyx 2013.12.17

#define TK_CONNECTIONTYPE_SNSDATAREDISCACHE 156  // (SNS服务组)"关系"数据缓存服务 add yangcheng 2013.12.18
#define TK_CONNECTIONTYPE_PIBROKERSRV 157        // TKPlayerInfoBroker liusj added 20140121
#define TK_CONNECTIONTYPE_PIHOLDERSRV 158        // TKPlayerInfoHolder liusj added 20140121

#define TK_CONNECTIONTYPE_MSDKLOADSRV 159  // 移动SDK接口服务（TKMSDKLoadService） wuqy added 2014.3.26
#define TK_CONNECTIONTYPE_MOBILELOADSRV (TK_CONNECTIONTYPE_MSDKLOADSRV)

#define TK_CONNECTIONTYPE_LOGAGENT 160      // (LOG服务组) TKLogAgentService
#define TK_CONNECTIONTYPE_LOGCOLLECTOR 161  // (LOG服务组) TKLogCollectorService
#define TK_CONNECTIONTYPE_LOGMANAGER 162    // (LOG服务组) TKLogManagerService

#define TK_CONNECTIONTYPE_BETSRV 163        // 竞猜服务 wuqy add 2014.5.20
#define TK_CONNECTIONTYPE_BETINFOSRV 164    // 竞猜信息服务 wuqy add 2014.5.20
#define TK_CONNECTIONTYPE_BROADINFOSRV 165  // 转播服务 wuqy add 2014.5.20
#define TK_CONNECTIONTYPE_LOBBYBETSRV 166   // 大厅竞猜服务 wuqy add 2014.5.22

#define TK_CONNECTIONTYPE_PSEVERIFICATION 167  // PSE Verification连接类型 add by 20160816
#define TK_CONNECTIONTYPE_PSESTORAGE 168       // PSE 仓储服务
//#define TK_CONNECTIONTYPE_RPDTCEXTERNERAL		 168 // RPD卡密管理外接服务 add by 20140529

#define TK_CONNECTIONTYPE_LANTVGAME 169  // 局域网TV版游戏服务 add by 20140604

#define TK_CONNECTIONTYPE_FISHLOADSRV 170       // 捕鱼游戏Load服务 add by wuqy 20140806
#define TK_CONNECTIONTYPE_FISHLOADSMOBSRV 171   // 捕鱼游戏Load服务（移动端弱联网版） add by wuqy 20140806
#define TK_CONNECTIONTYPE_GAMEPIVOTSRV 172      // 捕鱼游戏Pivot服务 add by wuqy 20140806
#define TK_CONNECTIONTYPE_FISHGAMEMOBSRV 173    // 捕鱼游戏服务（移动端） add by wuqy 20140806
#define TK_CONNECTIONTYPE_SNSMSGSTORAGE_EX 174  // 新SNS消息转储服务 黄永涛 2014.9.2

#define TK_CONNECTIONTYPE_RECHARGEPIVOTSRV 175    // 充值Pivot服务 add by liusj 20141105
#define TK_CONNECTIONTYPE_FGAMEPIVOTSRV 176       // 海外游戏Pivot服务 add by liusj 20141105
#define TK_CONNECTIONTYPE_FRECHARGEPIVOTSRV 177   // 海外充值Pivot服务 add by liusj 20141105
#define TK_CONNECTIONTYPE_FLOADMOBSRV 178         // 海外游戏Load服务（移动端） add by wuqy 20141124
#define TK_CONNECTIONTYPE_FISHLOADNMOBSRV 179     // 捕鱼游戏Load服务（移动端强联网版） add by wuqy 20141222
#define TK_CONNECTIONTYPE_GDCCONVERGENCE 180      // (全局数据缓群组） 应用汇聚服务 add by pangs 2014.09.19
#define TK_CONNECTIONTYPE_GDCMETADATASRV 181      // (全局数据缓群组） 基础元数据缓存 add by pangs 2015.11.06
#define TK_CONNECTIONTYPE_GDCMETADATABACKSRV 182  // (全局数据缓群组） 基础元数据缓存后向 add by pangs 2015.11.06
#define TK_CONNECTIONTYPE_GDCBROKERSRV 183        // (全局数据缓群组） GDC Broker add by pangs 2016.12.15
// 180-186 请留给全局数据缓群组

#define TK_CONNECTIONTYPE_HISEVTCONFIGSVR 187       // (HISEVT服务群组） HISEVT配置服务	      add by pangs 2014.11.05
#define TK_CONNECTIONTYPE_HISEVTDECAYSVR 188        // (HISEVT服务群组） HISEVT数据衰减服务   add by pangs 2014.11.05
#define TK_CONNECTIONTYPE_HISEVTUSECASETESTSVR 189  // (HISEVT服务群组） HISEVT自动测试服务   add by pangs 2014.11.05
#define TK_CONNECTIONTYPE_UDBSRV 190                // (MID体系)         用户设备行为服务     add by baiyx 2016.09.06
#define TK_CONNECTIONTYPE_NOSSVR 191                // EVT平台 Nosql开放存储系统
#define TK_CONNECTIONTYPE_CDESVR 192                // 牌库服务

#define TK_CONNECTIONTYPE_PSEAFTERSALE 193  //售后服务连接类型
#define TK_CONNECTIONTYPE_PSEMPRLOGIC 194   //PSEMprLogic连接类型 话费充值逻辑服务 fengdh 2016.11.29
#define TK_CONNECTIONTYPE_RISBROKER 195     //风控broker
#define TK_CONNECTIONTYPE_RISSRV 196        //风控Service

#define TK_CONNECTIONTYPE_EVMLOGIC 197   //EVM逻辑服务
#define TK_CONNECTIONTYPE_PRICECARD 198  //有价卡服务
#define TK_CONNECTIONTYPE_PSESYNC 199    //PSE Sync连接类型
#define TK_CONNECTIONTYPE_PSERED 200     //PSE 红包连接类型 fengdh 2016.10.21

//
#define TK_CONNECTIONTYPE_WEBUSER 201  // (WebSrv服务组) User业务

#define TK_CONNECTIONTYPE_WEBBIZPRESENT 202  // (WebSrv服务组) BIZ业务系统赠送服务
#define TK_CONNECTIONTYPE_WEBBIZSALARY 203   // (WebSrv服务组) 系统发放用户工资服务
//#define TK_CONNECTIONTYPE_WEBBIZPROFIT           204  // (WebSrv服务组) 合作方利润结算服务
#define TK_CONNECTIONTYPE_WEBBIZBROKER 204  // (WebSrv服务组) BIZ节点服务
#define TK_CONNECTIONTYPE_WEBBIZECA 205     // (WebSrv服务组) 网站ECA业务

#define TK_CONNECTIONTYPE_WEBBOSS 206  // (WebSrv服务组) 超级接口服务

#define TK_CONNECTIONTYPE_WEBVPAYSRV 207    // (WebSrv服务组) 虚拟货币充值服务
#define TK_CONNECTIONTYPE_WEBCOMMONSRV 208  // (WebSrv服务组) 虚拟货币充值服务

#define TK_CONNECTIONTYPE_WEBPAYINSRV 209  // (WebSrv服务组) 元宝购买服务	add by baiyx 20110609
#define TK_CONNECTIONTYPE_WEBPAYOTSRV 210  // (WebSrv服务组) 元宝支付服务	add by baiyx 20110609
#define TK_CONNECTIONTYPE_WEBMGWSRV 211    // (WebSrv服务组) MGW WebService

#define TK_CONNECTIONTYPE_WEBOAUTH 212        // (WebSrv服务组) OAuth服务   add by baiyx 20120322
#define TK_CONNECTIONTYPE_WEBMATCHCENTER 213  // (WebSrv服务组) 网页赛事中心组件服务   add by wuqy 20120824

#define TK_CONNECTIONTYPE_WEBHISSRV 214       // (WebSrv服务组) 历史WEB接口，取代之前HISOCOM add by baiyx 20121227
#define TK_CONNECTIONTYPE_WEBJCCSRV 215       // (WebSrv服务组) CardCenterWeb接口，add by baiyx 20130308
#define TK_CONNECTIONTYPE_WEBMATCHSRV 216     // (WebSrv服务组) MatchCtrl接口，add by baiyx 20130703
#define TK_CONNECTIONTYPE_WEBDATBATCHSRV 217  // (WebSrv服务组) BatCh接口，add by baiyx 20130815
#define TK_CONNECTIONTYPE_WEBTCSLSRV 218      // (WebSrv服务组) TCS交易约束系统供网站查询校验接口，add by baiyx 20130909
#define TK_CONNECTIONTYPE_WEBRLBSVR 219       // (WebSrv服务组) RLB，add by baiyx 20131022

#define TK_CONNECTIONTYPE_WEBRPDSRV 220      // (WebSrv服务组) RPD，add by baiyx 20131225
#define TK_CONNECTIONTYPE_WEBPAYPOSRV 221    // (WebSrv服务组) PAYPO，add by baiyx 20140102
#define TK_CONNECTIONTYPE_WEBPAYSISRV 222    // (WebSrv服务组) PAYSI，add by baiyx 20140102
#define TK_CONNECTIONTYPE_WEBTGPBROKER 223   // (WebSrv服务组) TGPBROKER，add by baiyx 20140120
#define TK_CONNECTIONTYPE_WEBHEBSRV 224      // (WebSrv服务组) HisEvtBroker，add by baiyx 20140512
#define TK_CONNECTIONTYPE_WSRPDSRV 225       // (WebSrv服务组) RPD，add by baiyx 20140512
#define TK_CONNECTIONTYPE_WEBWBSSTATSRV 226  // (WebSrv服务组) WebService监控统计服务
#define TK_CONNECTIONTYPE_WEBMARKERSRV 227   // (WebSrv服务组) marker add by baiyx20150115

#define TK_CONNECTIONTYPE_WEBMSGDEVXCSTSRV 228  // (WebSrv服务组) 安卓push消息 add by baiyx20150403
#define TK_CONNECTIONTYPE_WEBDATASTATSRV 229    // (WebSrv服务组) TKStat add by wuqy20150528

#define TK_CONNECTIONTYPE_WEBPAYINFOSRV 230     // (WebSrv服务组) 元宝购买服务	add by baiyx 20150720
#define TK_CONNECTIONTYPE_WEBEVMBROKER 231      // (WebSrv服务组) 内外非金融型虚拟财务购买	add by baiyx 20160714
#define TK_CONNECTIONTYPE_WEBNOTIFYRECV 232     // (WebSrv服务组) 微信通知	add by baiyx 20160920
#define TK_CONNECTIONTYPE_WEBSNSNEARBYRECV 233  // (WebSrv服务组) 关系位置服务	add by baiyx 20161122
#define TK_CONNECTIONTYPE_WEBCMT 234            // (WebSrv服务组) CMT接口服务	add by liyao 20190520
#define TK_CONNECTIONTYPE_WEBDAG 235            // (WebSrv服务组) DAG监控服务 add by luojj01 2022.6.28

// 211-249 请留给WebSrv服务组

#define TK_CONNECTIONTYPE_TGPDB 250       // (TGP服务组)TGP数据服务  add by ouyy 2011-11-02
#define TK_CONNECTIONTYPE_TGPBROKER 251   // (TGP服务组)TGP中转服务  add by ouyy 2011-11-02
#define TK_CONNECTIONTYPE_TGPLOAD 252     // (TGP服务组)TGP承载服务  add by ouyy 2011-11-02
#define TK_CONNECTIONTYPE_TGPCORE 253     // (TGP服务组)TGP核心业务服务  add by ouyy 2011-11-02
#define TK_CONNECTIONTYPE_TGPRTC 254      // (TGP服务组)TGP实时预计算服务 added by pangs 2017-06-08
#define TK_CONNECTIONTYPE_TGPLOGIC 255    // (TGP服务组)TGP逻辑服务 added by pangs 2018-04-28
#define TK_CONNECTIONTYPE_TGPDISPLAY 256  // (TGP服务组)TGP展示服务 added by pangs 2018-04-28
#define TK_CONNECTIONTYPE_TGPCONFIG 257   // (TGP服务组)TGP配置服务
#define TK_CONNECTIONTYPE_TGPDETAIL 258   // (TGP服务组)TGP明细服务
#define TK_CONNECTIONTYPE_TGPDRIVE 259    // (TGP服务组)自TGP驱动计算服务

#define TK_CONNECTIONTYPE_USEROAUTH 260    // (USR服务组)User外部鉴权
#define TK_CONNECTIONTYPE_USROGWSRV 261    // (User服务组)USR平台代理层内部请求出口服务  add by lizy 2013-6-19
#define TK_CONNECTIONTYPE_USEROFFLINE 262  // (User服务组)User offline add by housl 2021.11.25
#define TK_CONNECTIONTYPE_AAABILLSVR 263   // (AAA服务组)AAA账单服务

//3A服务体系网络连接类型与消息类型定义：
#define TK_CONNECTIONTYPE_AAASECURITYCENTER 264  // (AAA服务组)AAA平台安保中心服务
#define TK_CONNECTIONTYPE_AAABROKER 265          // (AAA服务组)AAA平台节点服务  add by lizy 2015-5-22
#define TK_CONNECTIONTYPE_AAAACCOUNT 266         // (AAA服务组)AAA平台ACCOUNT逻辑服务  add by lizy 2015-5-22
#define TK_CONNECTIONTYPE_AAAAUTHENTICATION 267  // (AAA服务组)AAA平台AUTHENTICATION逻辑服务  add by lizy 2015-5-22
#define TK_CONNECTIONTYPE_AAAAUTHORIZATION 268   // (AAA服务组)AAA平台AUTHORIZATION逻辑服务  add by lizy 2015-5-22
#define TK_CONNECTIONTYPE_AAADB 269              // (AAA服务组)AAA平台DB数据库代理服务  add by lizy 2015-5-22

#define TK_CONNECTIONTYPE_TCSLOGICSRV 270  // (TCS服务组)TCS逻辑服务

#define TK_CONNECTIONTYPE_AAAWEBOUTBROKER 273     // (AAA服务组)AAA访问Web接口代理服务
#define TK_CONNECTIONTYPE_AAAPOSTDEALSERVICE 274  // (AAA服务组)AAA平台后处理服务

#define TK_CONNECTIONTYPE_SDKSECURITY 275  // 无线SDK安全管理
#define TK_CONNECTIONTYPE_OIDDBSRV 280     // (OpenID服务组)OpenID平台数据服务  add by lizy 2013-6-19
#define TK_CONNECTIONTYPE_OIDSRV 281       // (OpenID服务组)OpenID平台逻辑承载服务  add by lizy 2013-6-19
#define TK_CONNECTIONTYPE_OIDOGWSRV 282    // (OpenID服务组)OpenID平台代理层内部请求出口服务  add by lizy 2013-6-19
#define TK_CONNECTIONTYPE_AOSBROKER 283    // (AOS服务组)AOS平台节点服务  add by lizy 2014-12-6
#define TK_CONNECTIONTYPE_AOSAID 284       // (AOS服务组)AOS平台AID逻辑服务  add by lizy 2014-12-6
#define TK_CONNECTIONTYPE_AOSOID 285       // (AOS服务组)AOS平台OID逻辑服务  add by lizy 2014-12-6
#define TK_CONNECTIONTYPE_AOSDB 286        // (AOS服务组)AOS平台数据服务  add by lizy 2014-12-6
#define TK_CONNECTIONTYPE_AOSSID 287       // (AOS服务组)AOS平台SID逻辑服务  add by lizy 2015-10-21

#define TK_CONNECTIONTYPE_MSGBROADCAST 288  // MSG群组-广播服务  added by pangs 2014-12-17
#define TK_CONNECTIONTYPE_MSGDEVXCST 289    // MSG群组-设备播服务  added by pangs 2015-04-02

#define TK_CONNECTIONTYPE_AIDOGWSRV 290  // (AllyID服务组)AllyID平台代理层内部请求出口服务
#define TK_CONNECTIONTYPE_AIDSRV 291     // (AllyID服务组)AllyID平台逻辑承载服务
#define TK_CONNECTIONTYPE_AIDDBSRV 292   // (AllyID服务组)AllyID平台数据服务

#define TK_CONNECTIONTYPE_OSS 293                           // (新HIS服务群组) ObjectStorageService 对象存储服务
#define TK_CONNECTIONTYPE_GSS 294                           // (新HIS服务群组) GeneralSearchService 通用检索服务
#define TK_CONNECTIONTYPE_HIS 295                           // (新HIS服务群组) 新历史服务
#define TK_CONNECTIONTYPE_HISCHECK (TK_CONNECTIONTYPE_HIS)  // (新HIS服务群组) 基于用户历史行为特征值的异常检测服务
#define TK_CONNECTIONTYPE_HISSTORAGE 296                    // (新HIS服务群组) 历史存储服务
#define TK_CONNECTIONTYPE_CLSSRV 297                        // (LOG服务组) 条件日志采集服务
#define TK_CONNECTIONTYPE_ACLSVR 298                        //  AI复式赛牌库服务
#define TK_CONNECTIONTYPE_LOBBYONLINECOUNT 299              //上报转发在线统计数据给监控 wanght 2019-11-06
#define TK_CONNECTIONTYPE_LOBBYONLINESETTING 300            // 大厅在线状态设置服务TKLobbyOnlineSettingService wanght 2019-11-11

#define TK_CONNECTIONTYPE_SGLSRV 301           // 外部服务组，三国令对接VPAY服务
#define TK_CONNECTIONTYPE_WGPROXYSRV 302       // WebGameFlash代理服务，JJ网页比赛
#define TK_CONNECTIONTYPE_WGSNSFLASH 303       // WebGameFlash的WebService连接（Php） add by wuqy 2011-3-18
#define TK_CONNECTIONTYPE_FTPHONEAGENTSRV 304  // Feature Phone(功能手机)用户代理服务 add by wuqy 2011-11-08
#define TK_CONNECTIONTYPE_SNSORGSRV 305        // Sns Org服务 wuqy add 2012.5.2
#define TK_CONNECTIONTYPE_MATCHIDSRV 306       // MatchID服务 wuqy add 2014.4.10
#define TK_CONNECTIONTYPE_MATCHDBSRV 307       // MatchDB服务 wuqy add 2014.4.10
#define TK_CONNECTIONTYPE_JAVAMATCHSRV 308     // JavaMatch服务(H5猜拳) wuqy add 2017.4.24
#define TK_CONNECTIONTYPE_AMA 309              // 欧若拉业务监控Aurora Monitor Alert add by guopc 2023.5.25

#define TK_CONNECTIONTYPE_SSOOGWSRV 310  // (SSO服务组)SSO平台代理层内部请求出口服务
#define TK_CONNECTIONTYPE_SSOSRV 311     // (SSO服务组)SSO平台逻辑承载服务
#define TK_CONNECTIONTYPE_SSODBSRV 312   // (SSO服务组)SSO平台数据服务

#define TK_CONNECTIONTYPE_PAYSIGNSRV 313       // (Pay服务组）PaySign服务
#define TK_CONNECTIONTYPE_EMPSENDSRV 314       //通知发送服务连接类型宏定义
#define TK_CONNECTIONTYPE_EMPRECVSRV 315       //通知接收服务连接类型宏定义
#define TK_CONNECTIONTYPE_SMEPHONECALLSRV 316  //语音呼叫服务
#define TK_CONNECTIONTYPE_MOBILEAUTHSRV 317    //一键登录接入第三方服务 2019.12.13 fengdh add
#define TK_CONNECTIONTYPE_PAYCHDISCOUNT 318    //充值-支付渠道优惠服务 2020.7.16 fengdh add
#define TK_CONNECTIONTYPE_PAYSECRETMGR 319     //第三方Token中控服务 2021.1.20 lvfj add

#define TK_CONNECTIONTYPE_CTRCOLLECTOR 320      // (CTR服务组） 数据收集服务
#define TK_CONNECTIONTYPE_CTRCASSANDRAPLAY 321  // (CTR服务组） Cassandra写入服务
#define TK_CONNECTIONTYPE_CTRHDFSPLAY 322       // (CTR服务组） HDFS写入服务
#define TK_CONNECTIONTYPE_CTROLAPPLAY 323       // (CTR服务组） OLAP写入服务
#define TK_CONNECTIONTYPE_CTRREADER 324         // (CTR服务组） 数据读取服务
#define TK_CONNECTIONTYPE_CTRKAFKADISPATCH 325  // (CTR服务组） Kafka数据分发服务
#define TK_CONNECTIONTYPE_CTRMSG2DBADAPTER 326  // (CTR服务组） TK消息与DB存储适配服务

#define TK_CONNECTIONTYPE_CMAINFO 327                            // (CMA服务组） CMA信息查询服务
#define TK_CONNECTIONTYPE_CMAAGENT (TK_CONNECTIONTYPE_LOGAGENT)  // (CMA服务组） CMA信息采集服务
#define TK_CONNECTIONTYPE_DHLAGENT 328                           //（DHL服务组）日志文件采集服务
#define TK_CONNECTIONTYPE_MCA 329                                //多触点归因服务 add by lixm 2023.5.24

#define TK_CONNECTIONTYPE_OPENPLATADDR 330     // 开放平台地址服务
#define TK_CONNECTIONTYPE_OPENPLATPROXY 331    // 开放平台代理服务
#define TK_CONNECTIONTYPE_OPENPLATLOGIC 332    // 开放平台逻辑服务
#define TK_CONNECTIONTYPE_OPENPLATMONITOR 333  // 开放平台监测服务
#define TK_CONNECTIONTYPE_OPENPLATPUSH 334     // 开放平台推送服务

#define TK_CONNECTIONTYPE_BACKENDOPENPLATPROXY 335  //后端开放平台代理服务
#define TK_CONNECTIONTYPE_BACKENDOPENPLATLOGIC 336  //后端开放平台逻辑服务

#define TK_CONNECTIONTYPE_DTCCOLLECTOR 340  // (DTC服务组) 数据接收服务
#define TK_CONNECTIONTYPE_DTCDISPATCH 341   // (DTC服务组) 数据获取服务
#define TK_CONNECTIONTYPE_DTCMANAGE 342     // (DTC服务组) 管理服务
#define TK_CONNECTIONTYPE_CMAPRODUCER 343   // (CMA服务组) CMA数据拉取服务
#define TK_CONNECTIONTYPE_CMAJUDGER 344     // (CMA服务组) CMA报警判断服务
#define TK_CONNECTIONTYPE_CMASENDER 345     // (CMA服务组) CMA报警发送服务
#define TK_CONNECTIONTYPE_CMAMANAGER 346    // (CMA服务组) CMA管理服务

#define TK_CONNECTIONTYPE_DSTGENERAL 350  // (DST 服务组) 通用测试服务

#define TK_CONNECTIONTYPE_PPSPROXY 359                 //（家族服务组） 家族业务代理服务 caolk
#define TK_CONNECTIONTYPE_MSGDETAILSRV 360             //MSG明细服务
#define TK_CONNECTIONTYPE_MSGDEVANDROIDSRV 361         //MSG设备安卓服务
#define TK_CONNECTIONTYPE_MSGDEVINTERFACESRV 362       //MSG设备接口服务
#define TK_CONNECTIONTYPE_MSGMISINTERFACESRV 363       //MSG与Mis接口服务
#define TK_CONNECTIONTYPE_MSGDEVDETAILSRV 364          //MSG设备Push明细服务
#define TK_CONNECTIONTYPE_MSGDEVCERTIFICATEMGRSRV 365  //MSG设备证书管理服务
#define TK_CONNECTIONTYPE_MSGDEVLOGICSRV 366           //MSG设备逻辑服务
#define TK_CONNECTIONTYPE_MSGDEVTOKENMGRSRV 367        //MSG设备Token管理服务

#define TK_CONNECTIONTYPE_SPABROKERSERVICE 370          // 自研产品帐号代理服务
#define TK_CONNECTIONTYPE_SPALOGINSERVICE 371           // 自研产品帐号登录服务
#define TK_CONNECTIONTYPE_SPAMANAGERSERVICE 372         // 自研产品帐号信息管理服务
#define TK_CONNECTIONTYPE_SPAREGISTERSERVICE 373        // 自研产品帐号注册服务
#define TK_CONNECTIONTYPE_SPASSOSERVICE 374             // 自研产品帐号SSO服务
#define TK_CONNECTIONTYPE_SPATHIRDAUTHSERVICE 375       // 自研产品帐号三方授权服务
#define TK_CONNECTIONTYPE_SPASECURITYCENTERSERVICE 376  // 自研产品帐号安保中心服务

#define TK_CONNECTIONTYPE_HTTPTRANS 380   // TKHttpTransService TK服务访问http luwq 2021.4.19
#define TK_CONNECTIONTYPE_HTTPBROKER 381  // TKHttpBrokerService TK服务访问http luwq 2021.4.19

#define TK_CONNECTIONTYPE_SMEBROKERS 390    //短信平台2.0 Broker服务
#define TK_CONNECTIONTYPE_SMELOGICS 391     //短信平台2.0 逻辑服务
#define TK_CONNECTIONTYPE_SMECHANNELS 392   //短信平台2.0 通道服务
#define TK_CONNECTIONTYPE_SMEDETAILS 393    //短信平台2.0 明细服务
#define TK_CONNECTIONTYPE_SMESENTINELS 394  //短信平台2.0 渠道管理服务

#define TK_CONNECTIONTYPE_OUT_TENNISSRV 401  // 外部服务，常州网球服务 add by wuqy 2012.3.27
#define TK_CONNECTIONTYPE_OUT_ARCSRV 411     // 外部服务，ARC平台服务

#define TK_CONNECTIONTYPE_SNS_FMS 412        // 格式化消息存储服务
#define TK_CONNECTIONTYPE_SNS_DCQ 413        // 通用展示可控队列系统服务
#define TK_CONNECTIONTYPE_SNS_DCQ_PROXY 414  // 通用展示可控队列系统代理服务

#define TK_CONNECTIONTYPE_FISHMATCHSVR 415      // 捕鱼比赛服务 wuqy 2015.4.16
#define TK_CONNECTIONTYPE_VSLLOADSRV 416        // VSLobby Load服务 wuqy 2015.5.26 解决电竞大厅直连LS的IP的问题
#define TK_CONNECTIONTYPE_STORYLOADSRV 417      // Story Load服务 wuqy 2015.7.27 赛况直连Story服务的IP的问题
#define TK_CONNECTIONTYPE_SDKAGENT 418          // 移动端联运SDK服务 wuqy 2015.10.31
#define TK_CONNECTIONTYPE_SDGRINFO 419          // 同方位游戏结果信息服务 wuqy 2015.11.2
#define TK_CONNECTIONTYPE_TGPPROXYSRV 420       // TGP Proxy服务 wuqy 2015.11.28 解决移动端连接TGP的用户ID到IP的负载均衡问题
#define TK_CONNECTIONTYPE_STATGOLSVR 421        // 游戏在线人数统计服务 2016.4.7 wuqy
#define TK_CONNECTIONTYPE_BOMBERLOADSRV 422     // 炸弹人游戏Load服务 add by wuqy 2016.08.22
#define TK_CONNECTIONTYPE_MASTERROUNDSVR 423    // 全局游戏大师对局服务（用于统一维护全局统一的旁观列表，比如移动端象棋的旁观入口 wuqy 2016.8.29）
#define TK_CONNECTIONTYPE_IPDATAGENESVR 424     // IP数据生成服务 add by wuqy 2016.11.10
#define TK_CONNECTIONTYPE_IPDATAQUERYSVR 425    // IP数据查询服务 add by wuqy 2016.11.10
#define TK_CONNECTIONTYPE_TABLEIDSVR 426        // 自建桌桌号管理服务 add by wuqy 2017.7.24
#define TK_CONNECTIONTYPE_CARDSREPOSVR 427      // 牌库服务 add by wuqy 2017.8.17
#define TK_CONNECTIONTYPE_USERMATCHINFOSVR 428  // 用户比赛信息服务,缓存用户进行中的比赛列表和信息 wuqy 2018.3.7
#define TK_CONNECTIONTYPE_CARDGROUPREPOSVR 429  //游戏服务和比赛服务提供牌组		yangguang02 2019.11.01

#define TK_CONNECTIONTYPE_SNSTRANSCACHE 430        // 关系消息存储服务
#define TK_CONNECTIONTYPE_SNSMSGQUEUE 431          // 关系消息队列服务
#define TK_CONNECTIONTYPE_SNSUSERLOGININFO 432     // 关系登录信息服务
#define TK_CONNECTIONTYPE_SNSILLEGALUSERCHECK 433  // 关系黑名单验证服务
#define TK_CONNECTIONTYPE_SNSWEBTRANS 434          // web直连接口服务
#define TK_CONNECTIONTYPE_SNSDISPATCH 435          // 关系业务派发服务
#define TK_CONNECTIONTYPE_SNSPVT 436               // 私人关系管理服务
#define TK_CONNECTIONTYPE_SNSORG 437               // 组织关系管理服务
#define TK_CONNECTIONTYPE_SNSLOGIN 438             // 关系用户登录服务
#define TK_CONNECTIONTYPE_NEARBY 439               // 关系用户位置服务
#define TK_CONNECTIONTYPE_CHATCONTROL 440          // (聊天室服务组)中控服务 yangcheng 2017.3.1
#define TK_CONNECTIONTYPE_CHATMASTER 441           // (聊天室服务组)主服务
#define TK_CONNECTIONTYPE_CHATSALVE 442            // (聊天室服务组)从服务
#define TK_CONNECTIONTYPE_CHATPROXY 443            // (聊天室服务组)代理服务
#define TK_CONNECTIONTYPE_CHATMSGQUEUE 444         // (聊天室服务组)消息服务
#define TK_CONNECTIONTYPE_CHATPIVOT 445            // (聊天室服务组)枢纽服务
#define TK_CONNECTIONTYPE_CHATLOG 446              // (聊天室服务组)日志存储服务
#define TK_CONNECTIONTYPE_SMALLFILERECORD 447      // 小文件系统记录服务
#define TK_CONNECTIONTYPE_CHATGAME 448             // (聊天室服务组)游戏聊天室服务

#define TK_CONNECTIONTYPE_FDCBROKERSRV 450         // (应用格式缓群组） FDC Broker add by pangs 2017.05.18
#define TK_CONNECTIONTYPE_FDCREADSRV 451           // (应用格式缓群组） FDC Read add by pangs 2017.05.18
#define TK_CONNECTIONTYPE_FDCWRITESRV 452          // (应用格式缓群组） FDC Write add by pangs 2017.05.18
#define TK_CONNECTIONTYPE_FDCCONFIGSRV 453         // (应用格式缓群组） FDC Config add by pangs 2017.05.18
#define TK_CONNECTIONTYPE_FDCTEMSTATEREADSRV 454   //(应用格式缓群组）FDC临时状态数据读服务
#define TK_CONNECTIONTYPE_FDCTEMSTATEWRITESRV 455  //(应用格式缓群组）FDC临时状态数据写服务

#define TK_CONNECTIONTYPE_DATREGISTPUSHSVR 460  // (DAT群组) DAT注册推送服务 pangs added 20170519
#define TK_CONNECTIONTYPE_DATBROKERSVR 461      // (DAT群组) DAT服务代理 huangfc 2019.11.13

#define TK_CONNECTIONTYPE_PMSWEB 469  // PMSWebService 2020.10.24

#define TK_CONNECTIONTYPE_MLBCONTROL 470  // (媒体平台服务组)直播控制服务 yangcheng 2017.9.18
#define TK_CONNECTIONTYPE_MLBMEDIA 471    // (媒体平台服务组)直播媒体服务
#define TK_CONNECTIONTYPE_MLBCHANNAL 472  // (媒体平台服务组)直播频道服务
#define TK_CONNECTIONTYPE_MLBPROYX 473    // (媒体平台服务组)直播代理服务

#define TK_CONNECTIONTYPE_MRCCONTROL 480  // (媒体平台服务组)实时沟通控制服务 yangcheng 2017.9.18
#define TK_CONNECTIONTYPE_MRCMEDIA 481    // (媒体平台服务组)实时沟通媒体服务
#define TK_CONNECTIONTYPE_MRCROOM 482     // (媒体平台服务组)实时沟通房间服务
#define TK_CONNECTIONTYPE_MRCPROYX 483    // (媒体平台服务组)实时沟通代理服务

#define TK_CONNECTIONTYPE_CUSTOMMATCHCREAT 484      // 自建比赛的创建比赛服务 luwq add by xinxy 2020.08.12
#define TK_CONNECTIONTYPE_CUSTOMTNY 485             // 自建比赛的报名服务	luwq add by xinxy 2020.08.12
#define TK_CONNECTIONTYPE_ROBOTINFOMGRSRV 486       // 机器人信息管理服务	yangguang02 add by xinxy 2021.3.16
#define TK_CONNECTIONTYPE_ROBOTDISPATCHCTRLSRV 487  // 机器人调度控制服务 haobc 2021.3.17
#define TK_CONNECTIONTYPE_TEAMCUSTOMTNY 488         //自建团赛报名服务 luwq add 2022.1.17

#define TK_CONNECTIONTYPE_ECAPSE 490  // (ECA服务组)ECAPSE业务

#define TK_CONNECTIONTYPE_TGPSTORE 495       // (TGP服务组)TGP存储服务
#define TK_CONNECTIONTYPE_TGPRULEENGINE 496  // (TGP服务组)TGP规则引擎服务
#define TK_CONNECTIONTYPE_DLOCONFIG 497      // (DLO服务组)DLO配置服务
#define TK_CONNECTIONTYPE_DLOLOGIC 498       // (DLO服务组)DLO逻辑服务

#define TK_CONNECTIONTYPE_SNSMTR_SVR 500             // (关系平台监控服务组)监控服务
#define TK_CONNECTIONTYPE_SNSMTR_AGENT 501           // (关系平台监控服务组)监控代理服务
#define TK_CONNECTIONTYPE_SNSMTR_RECORD 502          // (关系平台监控服务组)监控记录服务
#define TK_CONNECTIONTYPE_SNSEXTENDDATA 503          // 关系外延数据服务 huangch 2021.12.22
#define TK_CONNECTIONTYPE_SNSEXTENDDATA_PRODUCE 504  //关系外延数据生成服务 huangch 2021.12.22
#define TK_CONNECTIONTYPE_SNSRECOMMAND 505           //关系数据推荐服务 huangch 2021.12.22
#define TK_CONNECTIONTYPE_CHATAGENT 506              //聊天中转服务部署在linux环境下提供协议转换服务 luwq 2022.1.12
#define TK_CONNECTIONTYPE_SNS_DATASYN 510            // (关系平台服务组)数据同步服务
#define TK_CONNECTIONTYPE_CHATROOMLIST 520           // (聊天室服务组)房间列表服务
#define TK_CONNECTIONTYPE_CHATWEBTRANS 521           // (聊天室服务组)Web接口服务
#define TK_CONNECTIONTYPE_SNSOPT_SVR 522             // (关系平台运维服务组)运维服务
#define TK_CONNECTIONTYPE_SNSOPT_AGENT 523           // (关系平台运维服务组)运维代理服务
#define TK_CONNECTIONTYPE_SNSDATAPIPE_CENTRE 524     // (关系平台服务组)数据同步中心服务
#define TK_CONNECTIONTYPE_SNSDATAPIPE_BORDER 525     // (关系平台服务组)数据同步边缘服务

#define TK_CONNECTIONTYPE_SPDBROKER 530     // (传播推广) SPDBroker
#define TK_CONNECTIONTYPE_SPDLOGIC 531      // (传播推广) SPDLogic
#define TK_CONNECTIONTYPE_SPDDB 532         // (传播推广) SPDDB
#define TK_CONNECTIONTYPE_SPDINTERFACE 533  // (传播推广) SPDInterface
#define TK_CONNECTIONTYPE_SPDEXTERNAL 534   // (传播推广) SPDExternal
#define TK_CONNECTIONTYPE_PAYSUBSCRIBE 535  // add by fengdh 2023.6.14

#define TK_CONNECTIONTYPE_VERIFYCHECK 540  //验证码校验服务 信息安全部zhangcx 2020.12.26

#define TK_CONNECTIONTYPE_FISHWINTREABROKERSRV 550   // 捕鱼夺宝broker服务
#define TK_CONNECTIONTYPE_FISHWINTREALOTTERYSRV 551  // 捕鱼夺宝逻辑服务
#define TK_CONNECTIONTYPE_FISHWINTREAAWARDSRV 552    // 捕鱼夺宝奖励服务
#define TK_CONNECTIONTYPE_FISHDBQUERYSRV 553         // 捕鱼数据库查询服务
#define TK_CONNECTIONTYPE_FISHAVGPOTSRV 554          // 捕鱼奖池平分服务
#define TK_CONNECTIONTYPE_FISHDATASRV 555            // 捕鱼抽奖服务 add by 产品研发游戏二部 zhangjun

#define TK_CONNECTIONTYPE_VIPBROKER 560         // (会员服务组) VIPBroker
#define TK_CONNECTIONTYPE_UMSBROKERSERVICE 561  // (用户会员平台userMembership)代理服务
#define TK_CONNECTIONTYPE_UMSLOGICSERVICE 562   // (用户会员平台userMembership)逻辑服务
#define TK_CONNECTIONTYPE_UMSACTIONSERVICE 563  // (用户会员平台userMembership)奖励发放服务

#define TK_CONNECTIONTYPE_OIDBROKERSERVICE 570    // (开放帐号平台)代理服务
#define TK_CONNECTIONTYPE_OIDLOGICSERVICE 571     // (开放帐号平台)逻辑服务
#define TK_CONNECTIONTYPE_OIDPOSTDEALSERVICE 572  // (开放帐号平台)后处理服务
#define TK_CONNECTIONTYPE_OIDDBSERVICE 573        // (开放帐号平台)数据服务

#define TK_CONNECTIONTYPE_TIDBROKERSERVICE 575    // (游客帐号平台)代理服务
#define TK_CONNECTIONTYPE_TIDLOGICSERVICE 576     // (游客帐号平台)逻辑服务
#define TK_CONNECTIONTYPE_TIDPOSTDEALSERVICE 577  // (游客帐号平台)后处理服务
#define TK_CONNECTIONTYPE_TIDDBSERVICE 578        // (游客帐号平台)数据服务

#define TK_CONNECTIONTYPE_RIMBROKERSERVICE 585    // (角色信息管理平台)代理服务
#define TK_CONNECTIONTYPE_RIMLOGICSERVICE 586     // (角色信息管理平台)逻辑服务
#define TK_CONNECTIONTYPE_RIMPOSTDEALSERVICE 587  // (角色信息管理平台)后处理服务
#define TK_CONNECTIONTYPE_RIMDBSERVICE 588        // (角色信息管理平台)数据服务

#define TK_CONNECTIONTYPE_USCBROKERSERVICE 590    // (用户安全中心)代理服务
#define TK_CONNECTIONTYPE_USCINFOSERVICE 591      // (用户安全中心)安全信息管理服务
#define TK_CONNECTIONTYPE_USCCERTSERVICE 592      // (用户安全中心)安全认证服务
#define TK_CONNECTIONTYPE_USCBADSERVICE 593       // (用户安全中心)行为异常检测服务(Behavior Anomaly Detection)
#define TK_CONNECTIONTYPE_USCPOSTDEALSERVICE 594  // (用户安全中心)后处理服务
#define TK_CONNECTIONTYPE_USCDBSERVICE 595        // (用户安全中心)数据服务
#define TK_CONNECTIONTYPE_USCOFFLINESERVICE 596   // (用户安全中心)离线处理服务

#define TK_CONNECTIONTYPE_COMITPOSTDEAL 600   //(Comit服务组)后处理服务add by huangfc 2020.1.13
#define TK_CONNECTIONTYPE_OUTSIDEAWARD 601    //(Comit服务组)外部平台颁奖服务add by huangfc 2020.12.1
#define TK_CONNECTIONTYPE_CMTASYNCNOTICE 602  //(CMT服务组) 异步通知服务 add by caowy 2023.3.21

/* 610-659 JJ账号统一安全管理平台*/
#define TK_CONNECTIONTYPE_AAA_APPREGLOGIN 610        //(JJ账号统一安全管理平台)注册登录应用服务
#define TK_CONNECTIONTYPE_AAA_COMAUTHENTICATION 630  //(JJ账号统一安全管理平台)认证组件服务
#define TK_CONNECTIONTYPE_AAA_COMACCOUNT 631         //(JJ账号统一安全管理平台)帐号组件服务
#define TK_CONNECTIONTYPE_AAA_COMPERSONALINFO 632    //(JJ账号统一安全管理平台)个人信息组件服务
#define TK_CONNECTIONTYPE_AAA_COMSTATUS 633          //(JJ账号统一安全管理平台)帐号状态组件服务
#define TK_CONNECTIONTYPE_AAA_COMABTEST 634          //(JJ账号统一安全管理平台)AB测试结果校验服务
#define TK_CONNECTIONTYPE_AAA_COMDEVICE 635          //(JJ账号统一安全管理平台)帐号设备组件服务
#define TK_CONNECTIONTYPE_AAA_COMSAFEMOBILE 636      //(JJ账号统一安全管理平台)帐号安保手机组件服务
#define TK_CONNECTIONTYPE_AAA_ACCOUNTSAFE 637        //(JJ账号统一安全管理平台)帐号安全应用服务
#define TK_CONNECTIONTYPE_AAA_COMSTRATECTL 638       //(JJ账号统一安全管理平台)TKAAACOMStrategyControlService组件 22.6.6 housl

/* 660-669 预留给用户信用体系 */
#define TK_CONNECTIONTYPE_CREDIT_BROKER 660    //(用户信用体系)代理服务
#define TK_CONNECTIONTYPE_CREDIT_LOGIC 661     //(用户信用体系)逻辑服务
#define TK_CONNECTIONTYPE_CREDIT_POSTDEAL 662  //(用户信用体系)后处理服务

/* 700 -799 预留给海外游戏服务*/
#define TK_CONNECTIONTYPE_FGAMESRVBEGIN 700  // 海外游戏服务保留段起始 add by wuqy 20151116
#define TK_CONNECTIONTYPE_LOBBYBONUS 701     // 海外游戏大厅Bonus游戏服务  add by xudm 20151117
#define TK_CONNECTIONTYPE_VERMANAGE 702      // 海外游戏版本管理服务       add by xudm 20151117
#define TK_CONNECTIONTYPE_FGAMESRVEND 799    // 海外游戏服务保留段结束 add by wuqy 20151116

/* 800 -820 预留给PAY，讨个彩头*/
#define TK_CONNECTIONTYPE_PPOSRV 800        // TKPPOService(PayPreOrder) 支付预订单服务
#define TK_CONNECTIONTYPE_PSISRV 801        // TKPSIService(PaySyncIn) 支付同步服务
#define TK_CONNECTIONTYPE_PSOSRV 802        // TKPSOService(PaySyncOut) 支付对外同步服务
#define TK_CONNECTIONTYPE_PIFSRV 803        // TKPIFService(PayInterface) 支付信息接口服务（信息查询，处理掉单等）
#define TK_CONNECTIONTYPE_PCPBROKER 804     // TKPCPBrokerService 泛货币支付节点
#define TK_CONNECTIONTYPE_PCPLOGIC 805      // TKPCPLogicService 泛货币支付逻辑
#define TK_CONNECTIONTYPE_PAYBATCHSRV 806   // TKPayBatchService 充值批处理服务
#define TK_CONNECTIONTYPE_PAYACTSRV 807     // TKPayActService 充值活动服务
#define TK_CONNECTIONTYPE_PCPISSUE 808      // TKPCPISSUEService 支付金币发放服务
#define TK_CONNECTIONTYPE_PCPGATEWAY 809    // TKPCPISSUEGATEWAY 支付发放服务
#define TK_CONNECTIONTYPE_PAYDCSRV 810      // TKPAYDCService 直充服务
#define TK_CONNECTIONTYPE_PAYMCHMANAGE 811  // TKPAYMchManageSerivce 商户管理服务
//PAY 2.0(支付2.0 适用linux版本)
#define TK_CONNECTIONTYPE_PAY_BROKER 812     // TKPAYBrokerService    充值节点服务
#define TK_CONNECTIONTYPE_PAY_LOGIC 813      // TKPAYLogicService	   充值逻辑处理服务
#define TK_CONNECTIONTYPE_PAY_GATEWAY 814    // TKPAYGatewayService   支付网关服务
#define TK_CONNECTIONTYPE_PAY_MERCHANT 815   // TKPAYMerchantService  支付渠道商户服务
#define TK_CONNECTIONTYPE_PAY_RISKCTRL 816   // TKPAYRiskCtrlService  支付业务风险控制
#define TK_CONNECTIONTYPE_PAY_REFUNDSRV 817  //TKPAYRefundService充值退款

#define TK_CONNECTIONTYPE_PSEINVOICE 850        // PSE发票服务
#define TK_CONNECTIONTYPE_PSEBROKERSVR 851      // PSE节点服务 网络连接类型
#define TK_CONNECTIONTYPE_PSELOGISTICSSVR 852   // PSE物流服务 网络连接类型
#define TK_CONNECTIONTYPE_PSECOMMODITYSVR 853   // PSE商品服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEDISCOUNTSVR 854    // PSE优惠服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEORDERSVR 855       // PSE订单服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEMANAGESVR 856      // PSE后台管理服务
#define TK_CONNECTIONTYPE_PSELOTTERYSVR 857     // PSE彩票服务
#define TK_CONNECTIONTYPE_PSEPERSONINFOSVR 858  // PSE个人信息服务

#define TK_CONNECTIONTYPE_PSEPORDERSVR 860      // PSEP订单服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEPPURCHASESVR 861   // PSEP采购服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEPPAYSVR 862        // PSEP支付服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEPLOGISTICSSVR 863  // PSEP物流服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEPSTORAGESVR 864    // PSEP仓储服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEPAFTERSALESVR 865  // PSEP售后服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEPINVOICESVR 866    // PSEP发票服务 网络连接类型
#define TK_CONNECTIONTYPE_PACERBOTMGRSRV 867    // 陪打机器人管理服务 网络连接类型
#define TK_CONNECTIONTYPE_PSEPDELIVERSVR 868    // PSEP发货服务 网络连接类型
#define TK_CONNECTIONTYPE_TPVEXTERNAL 869       //PSE第三方虚拟物品兑换服务add by huangfc 2020.12.1

#define TK_CONNECTIONTYPE_DAG_USMS 900           // DAG相关服务
#define TK_CONNECTIONTYPE_DAG_TS 901             // DAG报名服务
#define TK_CONNECTIONTYPE_DAG_PTMS 902           // DAG用户排名
#define TK_CONNECTIONTYPE_DAG_LBS 903            // DAG负载均衡
#define TK_CONNECTIONTYPE_DAG_USS 904            // DAG用户调度
#define TK_CONNECTIONTYPE_DAG_UMS 905            // DAG用户匹配
#define TK_CONNECTIONTYPE_DAG_URMS 906           // DAG用户路由
#define TK_CONNECTIONTYPE_DAG_MPS 907            // DAG用户连接服务
#define TK_CONNECTIONTYPE_DAG_RMS 908            // DAG游戏管理服务
#define TK_CONNECTIONTYPE_DAG_PAS 909            // DAG适配服务
#define TK_CONNECTIONTYPE_DAG_MTS 910            // DAG监控工具服务
#define TK_CONNECTIONTYPE_DAG_RIMS 911           // DAGRoundID服务
#define TK_CONNECTIONTYPE_DAG_AGENT 912          //DAG Agent服务
#define TK_CONNECTIONTYPE_DAG_ADDRESS 913        //DAG 寻址服务
#define TK_CONNECTIONTYPE_DAG_CONFIG_CENTER 914  //DAG配置中心 luojj01 2022.10.25
#define TK_CONNECTIONTYPE_DAG_NEW_TS 915         //DAG新报名服务 luojj01 2023.1.4
#define TK_CONNECTIONTYPE_DAG_SNBS 916           //DAG号段机器人服务 luojj01 2023.1.4
#define TK_CONNECTIONTYPE_DAG_TRANSBROKER 917    //DAG比赛充值交易业务代理服务 xuefeng 2023..6.5

#define TK_CONNECTIONTYPE_SPECROUNDINFOSRV 920  // SpecRoundInfo add by yangguang02 2022.3.14
#define TK_CONNECTIONTYPE_STATHISSVR 921        // TKStat历史数据服务 2022.5.19 wuqy
#define TK_CONNECTIONTYPE_STATMONSVR 922        // TKStat监控数据服务 2022.5.19 wuqy

// WebService 连出
#define TK_CONNECTIONTYPE_WEBSAFECENTERSRV 10000  // (WebSrv服务组)安全中心服务 add by ouyy 2010-06-28	稍后会取消
#define TK_CONNECTIONTYPE_WEBSMSSRV 10100         // (WebSrv服务组)短信代理服务 add by ouyy 2010-06-28

//网络连接类型定义(客户端连出)
#define TK_CONNECTIONTYPE_MINTYPEVALUE 1000       // 最小类型的值
#define TK_CONNECTIONTYPE_CTRLTOOLS 1000          //
#define TK_CONNECTIONTYPE_LOBBYCLT 1100           //
#define TK_CONNECTIONTYPE_MBSDKCLT 1101           // 移动SDK客户端 wuqy added 2014.3.26
#define TK_CONNECTIONTYPE_GAMECLT 1200            //
#define TK_CONNECTIONTYPE_LANTVGAMECLT 1201       // 局域网TV版游戏客户端 add by 20140604
#define TK_CONNECTIONTYPE_FISHLBYCLT 1202         // 捕鱼达人PC大厅客户端 add by wuqy 2014.8.4
#define TK_CONNECTIONTYPE_FISHMOBCLT 1203         // 捕鱼达人游戏移动客户端 add by wuqy 2014.8.4
#define TK_CONNECTIONTYPE_FGAMEMOBCLT 1204        // 海外游戏移动客户端 add by wuqy 2014.11.24
#define TK_CONNECTIONTYPE_BOMBERLBYCLT 1205       // 炸弹人大厅客户端 add by wuqy 2016.8.22
#define TK_CONNECTIONTYPE_DOWNLOADCLT 1300        //
#define TK_CONNECTIONTYPE_BROADCASTCLT 1400       //
#define TK_CONNECTIONTYPE_CHATCLT 1500            //
#define TK_CONNECTIONTYPE_PROPCLT 1600            //
#define TK_CONNECTIONTYPE_IMCLT 1700              //
#define TK_CONNECTIONTYPE_ADVERTCLT 1800          //
#define TK_CONNECTIONTYPE_BETCLT 1900             //
#define TK_CONNECTIONTYPE_MASTERCLT 2000          //
#define TK_CONNECTIONTYPE_MASTERMATCHINFO 2001    // 大师旁观服务 luwq add by xinxy 2020.08.12
#define TK_CONNECTIONTYPE_RECOMMENDERSYSTEM 2002  // 比赛推荐系统服务 add by hehuan 2020.08.20
#define TK_CONNECTIONTYPE_CROWDFUNDMASTER 2003    // 比赛众筹主服务 add by hehuan 2020.11.06
#define TK_CONNECTIONTYPE_CROWDFUNDINFO 2004      // 比赛众筹信息服务 add by hehuan 2022.03.25
#define TK_CONNECTIONTYPE_NEWCROWDFUND 2005       // 比赛新众筹服务 add by hehuan 2022.03.29
#define TK_CONNECTIONTYPE_CROWDFUNDGATEWAY 2006   // 比赛众筹网关服务 add by hehuan 2022.04.01
#define TK_CONNECTIONTYPE_CROWDFUNDID 2007        // 比赛众筹ID服务 add by hehuan 2022.04.12
#define TK_CONNECTIONTYPE_CROWDFUNDRD 2008        // 比赛众筹RedisData服务 add by hehuan 2022.04.12
#define TK_CONNECTIONTYPE_CROWDFUNDQT 2009        // 比赛众筹查询工具服务 add by hehuan 2022.04.29
#define TK_CONNECTIONTYPE_CROWDFUNDINFOSL 2010    // 比赛众筹信息从服务 add by hehuan 2022.05.20
#define TK_CONNECTIONTYPE_CROWDFUNDPROXY 2011     // 比赛众筹代理服务 add by hehuan 2022.09.06
#define TK_CONNECTIONTYPE_AVATARCLT 2100          //
#define TK_CONNECTIONTYPE_SYSMGRCLT 2200          // 系统消息管理客户端
#define TK_CONNECTIONTYPE_DIRECTCHARGER 2300      // 分销商直充客户端
#define TK_CONNECTIONTYPE_CHATCTRL 2400           // 聊天控件 baiyx add at 2008-05-20
#define TK_CONNECTIONTYPE_MATCHSTORYCLT 2500      // 比赛展播客户端 add by wuqy add at 2008-08-18
#define TK_CONNECTIONTYPE_SNSCLT 2600             // “关系”客户端 (SNS -- Social Network System) add by ouyy 2008.11.06
#define TK_CONNECTIONTYPE_MATCHRESULTCLT 2700     // 工具客户端，比赛故障后可以提取补偿用户名单 2008.12.18, wuqy
#define TK_CONNECTIONTYPE_MATCHCTRLCLT 2800       // 比赛管理控制户端 2009.1.16, wuqy
#define TK_CONNECTIONTYPE_MATCHTRACECLT 2801      // 比赛用户跟踪管理户端 2011.8.24, wuqy
#define TK_CONNECTIONTYPE_IPUSERTRACECLT 2802     // IP用户跟踪管理户端 2011.8.24, wuqy
#define TK_CONNECTIONTYPE_ONLINESTATCLT 2803      // 在线人数数据统计跟踪户端 2013.3.21, wuqy
#define TK_CONNECTIONTYPE_MAINTAINCLT 2804        // 服务器运维管理客户端 2013.8.20, wuqy
#define TK_CONNECTIONTYPE_PROXYTOOLCLT 2805       // 代理服务工具客户端 2017.7.25, liyao

#define TK_CONNECTIONTYPE_BTO_MPTSERVICE 2850      // tkmptservice 2021.2.2  wanggy
#define TK_CONNECTIONTYPE_BTO_TESTDATCOLLECT 2851  //TestDataCollector 2021.2.22 liuyang10

#define TK_CONNECTIONTYPE_GAMECHATCLT 2900        // 用于游戏场场景下的聊天控件 baiyx add at 2009-02-04
#define TK_CONNECTIONTYPE_STATCLT 3000            // 统计浏览工具客户端 2009.3.18 wuqy
#define TK_CONNECTIONTYPE_ANTIWALLOWCLT 3100      // 防沉迷工具客户端 wuqy add 2009.04.8
#define TK_CONNECTIONTYPE_MODULEUPDATECLT 3200    // 客户端模块更新工具 wuqy add 2009.05.15
#define TK_CONNECTIONTYPE_MODULEUPDATEWBS 3201    // WBS模块更新工具 wuqy add 2009.05.15
#define TK_CONNECTIONTYPE_TIMETOURNEYCLT 3300     // 定点赛ini文件服务工具客户端 wuqy add 2009.7.28
#define TK_CONNECTIONTYPE_BROADCASTMGRCLT 3400    // 转播管理客户端 wuqy add 2009.11.2
#define TK_CONNECTIONTYPE_CUSTOMERCLT 3500        // 客服平台用户端 add by chengl 2010.03.31
#define TK_CONNECTIONTYPE_STAFFCLT 3501           // 客服平台客服端 add by chengl 2010.03.31
#define TK_CONNECTIONTYPE_FISHMONITOR 3502        // 捕鱼系监控服务 add by xinxy 2020.5.12
#define TK_CONNECTIONTYPE_GENEITEMIDMGRSRV 3503   // 通用号码管理服务 add by yangguang02 2020.8.28
#define TK_CONNECTIONTYPE_MATCHDYNARANKQUE 3504   // 比赛群体排名，提供客户端查询接口 add by liuyang10 2021.6.22
#define TK_CONNECTIONTYPE_MATCHMOM 3505           // 比赛后台订阅/发布，消息代理服务 add by liuyang10 2021.6.22
#define TK_CONNECTIONTYPE_MIDGAMEMATCHSRV 3506    //中局对战比赛服务 add by yangguang02 2021.9.28
#define TK_CONNECTIONTYPE_FAMILYBIZMGR 3507       //比赛家族业务管理服,提供家族业务统一输入输出接口addby liuyang10 21.11.23
#define TK_CONNECTIONTYPE_FAMILYMATCH 3508        //家族比赛服负责家族棋桌比赛addby liuyang10 21.11.23
#define TK_CONNECTIONTYPE_MATCHMPRANK 3509        //复式岛屿赛MP排名服务 add by liuyang10 22.5.17
#define TK_CONNECTIONTYPE_RANKINGMATCH 3510       //超嗨自建赛 add by fanzh 2022.9.26
#define TK_CONNECTIONTYPE_MATCHHISRECBROKER 3511  //Match Game数据到HIS推送代理 add by zhangqt01 2023.2.24

#define TK_CONNECTIONTYPE_COMITCLT 3600   // 组委会客户端工具 add by ouyy 2010-06-12
#define TK_CONNECTIONTYPE_MOBILECLT 3700  // 移动客户端 add by chengl 2010-11-09

#define TK_CONNECTIONTYPE_JJWEBGAME 3800   // JJWebGame连接 add by ouyy 2011-01-05
#define TK_CONNECTIONTYPE_VSLOBBYCLT 3900  // VS大厅客户端连接 add by wuqy 2011-4-11
#define TK_CONNECTIONTYPE_TGPCLT 4000      // TGP客户端连接 add by ouyy 2011-11-24
#define TK_CONNECTIONTYPE_CMACLT 4001      // CMA客户端连接

#define TK_CONNECTIONTYPE_MLBSDKCLT 4100  // 媒体直播SDK客户端连接
#define TK_CONNECTIONTYPE_MRCSDKCLT 4101  // 媒体实时沟通SDK客户端连接

#define TK_CONNECTIONTYPE_SNSMTRSDKCLT 4200  // 关系平台监控SDK客户端连接

#define TK_CONNECTIONTYPE_SMARTX_IVR 4300  //SmaxtX  系统 客服 IVR电话系统对接服务

// 网络组件连出
#define TK_CONNECTIONTYPE_HISOCOM 5000        // 历史服务的外网组件
#define TK_CONNECTIONTYPE_HISICOM 5100        // 历史服务的内网组件
#define TK_CONNECTIONTYPE_RPYOCOM 5200        // Realpay服务的内网组件
#define TK_CONNECTIONTYPE_RPYICOM 5300        // Realpay服务的内网组件
#define TK_CONNECTIONTYPE_DATAOCOM 5400       // data服务的内网组件
#define TK_CONNECTIONTYPE_DATAICOM 5500       // data历史服务的内网组件
#define TK_CONNECTIONTYPE_SNSOCOM 5600        // Sns服务的外网组件
#define TK_CONNECTIONTYPE_SNSICOM 5700        // Sns服务的内网组件
#define TK_CONNECTIONTYPE_MARKEROCOM 5800     // Marker服务的外网组件
#define TK_CONNECTIONTYPE_MARKERICOM 5900     // Marker服务的内网组件
#define TK_CONNECTIONTYPE_VIRPAYOCOM 6000     // VirtualPay服务的外网组件
#define TK_CONNECTIONTYPE_VIRPAYICOM 6100     // VirtualPay服务的内网组件
#define TK_CONNECTIONTYPE_MATCHOCOM 6200      // Match服务的外网组件 wuqy add 2011.5.4
#define TK_CONNECTIONTYPE_MATCHICOM 6300      // Match服务的内网组件 wuqy add 2011.5.4
#define TK_CONNECTIONTYPE_CMTASIGNUP 6400     // CMT 活动报名
#define TK_CONNECTIONTYPE_CMTAAWARD 6401      //CMT 活动颁奖
#define TK_CONNECTIONTYPE_CMTSCHEDULE 6402    //CMT 日程
#define TK_CONNECTIONTYPE_CMTPUSHFILTER 6403  //CMT 推送过滤

//
#define TK_GAMEID_LORD 1001            //JJ斗地主
#define TK_GAMEID_MJ 1002              //JJ麻将
#define TK_GAMEID_UPDGRADE 1003        //JJ升级
#define TK_GAMEID_MJDB 1004            //JJ东北麻将
#define TK_GAMEID_MJGD 1005            //JJ广东麻将
#define TK_GAMEID_BAOHUANG 1006        //JJ保皇      wuqy add 2009.3.4
#define TK_GAMEID_LORD4P 1007          //JJ4人斗地主 wuqy add 2009.3.4
#define TK_GAMEID_THREECARD 1008       //JJ扎金花 wuqy add 2009.3.18
#define TK_GAMEID_POKER 1009           //JJPoker wuqy add 2009.3.18
#define TK_GAMEID_LORDLZ 1010          //JJ癞子斗地主 wuqy add 2009.6.1
#define TK_GAMEID_CHESS 1011           //JJ象棋 wuqy add 2009.7.22
#define TK_GAMEID_MJSC 1012            //JJ四川麻将 wuqy add 2009.7.22
#define TK_GAMEID_CSTUD 1013           //JJ休闲德州 wuqy add 2009.8.12
#define TK_GAMEID_POKERTRANS 1014      //JJ变形金刚(4张德州)游戏消息 wuqy add 2011.12.28
#define TK_GAMEID_SPR 1015             //JJ猜拳 wuqy add 2009.10.20
#define TK_GAMEID_SNATCH 1016          //JJ黄金矿工 wuqy add 2009.11.18
#define TK_GAMEID_MJT 1017             //JJ二人麻将 wuqy add 2010.01.13
#define TK_GAMEID_JUNQI 1018           //JJ军棋 wuqy add 2010.03.10
#define TK_GAMEID_LORDHL 1019          //JJ欢乐斗地主 wuqy add 2010.07.08
#define TK_GAMEID_GINRUMMY 1020        //JJ金拉米 wuqy add 2010.08.11
#define TK_GAMEID_BIL8 1021            //JJ8球 wuqy add 2010.10.20
#define TK_GAMEID_DBUCKLE 1022         //JJ双扣 wuqy add 2010.11.15
#define TK_GAMEID_INTERIM 1023         //JJ卡当 wuqy add 2010.12.18
#define TK_GAMEID_POKERMINI 1024       //PokerMini wuqy add 2011.2.10
#define TK_GAMEID_WK 1025              //JJ挖坑 wuqy add 2011.2.10
#define TK_GAMEID_ANOLE 1026           //JJ变色龙 wuqy add 2011.3.11
#define TK_GAMEID_CHASEPIG 1027        //JJ拱猪 wuqy add 2011.3.15
#define TK_GAMEID_MJBR 1028            //JJ麻将血流成河 wuqy add 2011.4.2
#define TK_GAMEID_BRIDGE 1029          //JJ桥牌 wuqy add 2011.5.17
#define TK_GAMEID_LORDPT 1030          //JJ搭档斗地主 wuqy add 2011.7.12
#define TK_GAMEID_RUNFAST 1031         //JJ跑得快 wuqy add 2011.9.28
#define TK_GAMEID_DOUNIU 1032          //JJ斗牛 wuqy add 2011.10.18
#define TK_GAMEID_GO 1033              //JJ围棋 wuqy add 2011.11.30
#define TK_GAMEID_GUANDAN 1034         //JJ掼蛋 wuqy add 2012.5.14
#define TK_GAMEID_LORDPK 1035          //JJ2人斗地主 wuqy add 2012.5.14
#define TK_GAMEID_MJBJ 1036            //JJ北京麻将
#define TK_GAMEID_SIXI 1037            //JJ四喜 wuqy add 2012.7.23
#define TK_GAMEID_TIANDK 1038          //JJ填大坑 wuqy add 2012.7.23
#define TK_GAMEID_POKER2F 1039         //JJ两副牌德州 wuqy add 2012.7.27
#define TK_GAMEID_FEIXINGQI 1040       //JJ行棋 wuqy add 2012.8.13
#define TK_GAMEID_HORSERACE 1041       //JJ跑马 wuqy add 2012.8.13
#define TK_GAMEID_MJTDH 1042           //JJ推倒和麻将 wuqy add 2013.1.17
#define TK_GAMEID_BADUGI 1043          //JJ百得之 wuqy add 2013.5.15
#define TK_GAMEID_BINGO 1044           //JJBingo wuqy add 2013.9.10
#define TK_GAMEID_JJLINE 1045          //JJLine拉霸（JJ连线） wuqy add 2013.9.22
#define TK_GAMEID_DGZ 1046             //打滚子 wuqy add 2013.12.30
#define TK_GAMEID_MJSCN 1047           //新川麻 wuqy add 2013.12.30
#define TK_GAMEID_LORDTV 1048          //JJ斗地主TV add by wuqy 2014.4.2
#define TK_GAMEID_UPDGRADETV 1049      //JJ升级TV add by wuqy 2014.4.2
#define TK_GAMEID_MJBW 1050            //JJ血战麻将 add by wuqy 2014.6.12
#define TK_GAMEID_MJDZ 1051            //JJ大众麻将 add by wuqy 2014.6.12
#define TK_GAMEID_LORDBTV 1052         //JJ斗地主BTV add by wuqy 2014.8.29
#define TK_GAMEID_TREASPOKER 1053      //JJ宝藏扑克 add by wuqy 2014.10.14
#define TK_GAMEID_LORDSD 1054          //JJ闪电斗地主 add by wuqy 2014.10.21
#define TK_GAMEID_SLOT 1055            //JJ老虎机 add by wuqy 2015.1.9
#define TK_GAMEID_STAREPOKER 1056      //JJ干瞪眼 add by wuqy 2015.5.10
#define TK_GAMEID_BACCARAT 1057        //JJ百家乐 add by wuqy 2015.5.29
#define TK_GAMEID_WZQ 1058             //JJ五子棋 add by wuqy 2015.10.17
#define TK_GAMEID_BRPINSHI 1059        //JJ百人牛牛 add by wuqy 2015.10.23
#define TK_GAMEID_FRYUPGRADE 1060      //JJ炒地皮 add by wuqy 2015.10.30
#define TK_GAMEID_THIRTEEN 1061        //JJ十三水 add by wuqy 2016.1.19
#define TK_GAMEID_ROULETTE 1062        //JJ轮盘 add by wuqy 2016.3.8 海外游戏
#define TK_GAMEID_MJ3P 1063            //JJ三人麻将 add by wuqy 2016.3.21
#define TK_GAMEID_MJFK 1064            //JJ疯狂麻将 add by wuqy 2016.8.9
#define TK_GAMEID_MJSR 1065            //JJ三人麻将 add by wuqy 2017.2.21
#define TK_GAMEID_THREECARDPK 1066     //JJ拼三张 wuqy add 2017.3.1
#define TK_GAMEID_MJFKN 1067           //JJ新疯狂麻将 add by wuqy 2017.4.20
#define TK_GAMEID_MJHEBN 1068          //JJ新哈尔滨麻将 add by wuqy 2017.7.19
#define TK_GAMEID_LORDWAR 1069         //JJ地主战争 add by wuqy 2017.8.8
#define TK_GAMEID_MJHB 1070            //JJ河北麻将 add by wuqy 2017.9.1
#define TK_GAMEID_MJGUANGD 1071        //JJ广东麻将新版 add by wuqy 2017.9.29
#define TK_GAMEID_MJZHENGZ 1072        //JJ郑州麻将 add by wuqy 2017.9.29
#define TK_GAMEID_MJHANGZ 1073         //JJ杭州麻将 add by wuqy 2017.10.27
#define TK_GAMEID_MJZHUANZ 1074        //JJ转转麻将 add by wuqy 2017.10.27
#define TK_GAMEID_MJSHENY 1075         //JJ沈阳麻将 add by wuqy 2017.10.27
#define TK_GAMEID_JUNQIPK 1076         //JJ军棋(移动端2人游戏) add by wuqy 2017.10.27
#define TK_GAMEID_LORDBB 1077          //JJ百变斗地主 add by wuqy 2017.12.5
#define TK_GAMEID_AEROPLANECHESS 1078  //JJ飞行棋(新版,移动端) add by wuqy 2018.2.9
#define TK_GAMEID_MAHJONGHL 1079       //JJ欢乐麻将 add by xinxy 2018.4.11
#define TK_GAMEID_MAHJONGHLXL 1080     //JJ欢乐血流麻将 add by xinxy 2018.4.17
#define TK_GAMEID_MJMINI 1081          //JJ 迷你麻将 add by xinxy 2018.5.7
#define TK_GAMEID_MJJSPK 1082          //JJ 二人极速麻将 add by xinxy 2018.5.7
#define TK_GAMEID_ANIMAL 1083          //JJ 斗兽棋 add by xinxy 2018.5.10
#define TK_GAMEID_RUNFAST2P 1084       //JJ 二人跑得快 add by xinxy 2018.6.11
#define TK_GAMEID_LUCKYJJ 1085         //JJ 金钩钓鱼 add by xinxy 2018.7.5
#define TK_GAMEID_DOUSHIPOKER 1086     //JJ 斗十扑克 add by xinxy 2018.7.11
#define TK_GAMEID_MAHJONGXQ 1087       //JJ 象棋麻将 add by xinxy  2018.7.16
#define TK_GAMEID_BIGPOKER 1088        //JJ 大牌德州 add by xinxy 	2018.8.22
#define TK_GAMEID_RENJU 1089           //JJ 星源九珠 add by xinxy 2018.11.20
#define TK_GAMEID_CRAZYANIM 1090       //JJ 疯狂动物 add by xinxy 2019.1.18
#define TK_GAMEID_SUPERTEAM 1091       // 黄金阵容GameID add by xinxy 2019.6.27
#define TK_GAMEID_BOMBLORD 1092        //炸弹斗地主 add by xinxy 2019.11.01
#define TK_GAMEID_MAHJONGHBN 1093      //新河北麻将 add by xinxy 2020.3.31
#define TK_GAMEID_MAHJONGZJ 1094       //贵阳捉鸡麻将 add by xinxy 2020.4.29
#define TK_GAMEID_MAHJONGSYN 1095      //新沈阳麻将 add by xinxy 2020.5.15
#define TK_GAMEID_MAHJONGKWXN 1096     //新卡五星麻将 add by xinxy 2020.5.15
#define TK_GAMEID_JUNQITT 1097         //军棋头条 add by xinxy 2020.6.9
#define TK_GAMEID_MJJPH 1098           //广东鸡平胡麻将 add by xinxy 2020.7.2
#define TK_GAMEID_MJNANJ 1099          //南京麻将 add by xinxy 2020.7.23
#define TK_GAMEID_MAHJONGHANGZ 1100    //九天项目部杭州麻将 add by xinxy 2020.8.5
#define TK_GAMEID_MJWUHAN 1101         //九天项目部武汉麻将 add by xinxy 2020.8.6
#define TK_GAMEID_MJSHANGH 1102        //九天项目部上海麻将 add by xinxy 2020.8.24
#define TK_GAMEID_MJCC 1103            //麻将项目部长春麻将 add by xinxy 2020.9.4
#define TK_GAMEID_MJSHAND 1104         //九天项目部山东麻将 add by xinxy 2020.10.9
#define TK_GAMEID_MJKOUD 1105          //九天项目部扣点麻将 add by xinxy 2020.10.9
#define TK_GAMEID_MJHAEB 1106          //九天项目部哈尔滨麻将 add by xinxy 2020.10.14
#define TK_GAMEID_MAHJONGZHENGZ 1107   //九天项目部郑州麻将 add by xinxy 2020.10.16
#define TK_GAMEID_MJNANC 1108          //九天项目部南昌麻将 add by xinxy 2020.10.16
#define TK_GAMEID_MJYIBIN 1109         //九天项目部宜宾麻将 add by xinxy 2020.11.21
#define TK_GAMEID_MJDALIAN 1110        //九天项目部大连麻将 add by xinxy 2020.11.23
#define TK_GAMEID_MJJDN 1111           //棋牌中心麻将项目部 add by xinxy 2020.12.24
#define TK_GAMEID_MJHEFEI 1112         //九天项目部合肥麻将 add by xinxy 2021.1.6
#define TK_GAMEID_MJCHSHA 1113         //九天项目部长沙麻将 add by xinxy 2021.1.6
#define TK_GAMEID_MJBEIJ 1114          //九天项目部北京麻将 add by xinxy 2021.1.13
#define TK_GAMEID_MJTIANJ 1115         //九天项目部天津麻将 add by xinxy 2021.1.13
#define TK_GAMEID_CDMJXUEZHAN 1116     //九天项目部成都血战麻将 add by xinxy 2021.1.21
#define TK_GAMEID_CDMJYIBIN 1117       //九天项目部四川宜宾麻将 add by xinxy 2021.1.21
#define TK_GAMEID_CDMJXUELIU 1118      //九天项目部血流麻将 add by xinxy 2021.1.25
#define TK_GAMEID_MAHJONGHAIN 1119     //九天项目部海南麻将 add by xinxy 2021.2.25
#define TK_GAMEID_LORDDD 1120          //棋牌产品中心搭档斗地主 2021.10.21
#define TK_GAMEID_MJNCHONG 1121        //九天项目部南充麻将 add by shaoxj 2022.2.25
#define TK_GAMEID_GUANDANCD 1122       //九天项目部掼蛋成都 add by zhangjs04 2022.3.18
#define TK_GAMEID_MJLUZHOU 1123        //九天项目部泸州麻将 add by shaoxj 2022.4.3
#define TK_GAMEID_MJHONGZXL 1124       //成都微信麻将小游戏 add by shaoxj 2022.5.16
#define TK_GAMEID_MJJISU 1125          //九天项目部急速麻将 add by zengchao 2022.5.16
#define TK_GAMEID_MJGUIL 1126          //九天项目部桂柳麻将 add by dengbw 2022.5.27
#define TK_GAMEID_MJFKXL 1127          //棋牌产品中心疯狂血流麻将 add by liuhb01 2022.6.27
#define TK_GAMEID_MJCDXZ 1128          //九天项目部成都血战麻将 add by shaoxj 2022.6.29
#define TK_GAMEID_MJGBXZ 1129          //产品研发中心国标血战麻将 add by yuanwx 2022.7.29
#define TK_GAMEID_MINIHLLORDBXP 1130   //小游戏项目部欢乐不洗牌地主 add by shaoxj 2022.8.4
#define TK_GAMEID_MINIHLLORDLZ 1131    //小游戏项目部欢乐连炸地主 add by shaoxj 2022.8.4
#define TK_GAMEID_MINIHLLORDJD 1132    //小游戏项目部欢乐经典地主 add by shaoxj 2022.8.4
#define TK_GAMEID_MJCDXL 1133          //小游戏项目部成都血流麻将 add by shaoxj 2022.8.8
#define TK_GAMEID_LONGFENGCARD 1134    //九天项目部龙凤牌 add by shaoxj 2022.9.22
#define TK_GAMEID_MJHLXZ 1135          //九天项目部欢乐血战麻将 add by shaoxj 2022.10.12
#define TK_GAMEID_MJCLASSICT 1136      //九天项目部新经典二麻 add by shaoxj 2022.10.21
#define TK_GAMEID_LORDDB 1137          //产品研发中心JJ多宝牌 add by zhangkh01 2022.12.6
#define TK_GAMEID_MINILORDJDFD 1138    //小游戏项目部经典斗好友房 add by chenke 2023.1.31
#define TK_GAMEID_MINILORDBXPFD 1139   //小游戏项目部经典斗好友房 add by chenke 2023.1.31
#define TK_GAMEID_CRAZYSTARE 1140      //九天项目部疯狂干瞪眼 add by chentw02 2023.2.23
#define TK_GAMEID_MJYJ 1141            //九天项目部幺鸡麻将 add by chentw02 2023.4.11
#define TK_GAMEID_MJKF 1142            //棋牌产品中心开封麻将  add by liuhb01 2023.5.25
#define TK_GAMEID_GOBANG 1143          //九天项目部 五子棋  add by chenke 2023.7.13
#define TK_GAMEID_LORDDJ 1144          //棋牌产品中心道具斗地主 add by linxh 2023.9.14
#define TK_GAMEID_MJSG 1145            //棋牌产品中心三国麻将 add by liuhb01 2023.10.12
#define TK_GAMEID_MJGD100 1146         //九天项目部 广东100张 add by chentw02 2023.11.13
#define TK_GAMEID_MJTS 1147            //棋牌产品中心唐山麻将 add by wangcx 2023.12.4
#define TK_GAMEID_MJ2MEN 1148          //九天项目部 2门麻将 add by chentw02 2023.12.28
#define TK_GAMEID_MJQWXL 1149          //棋牌产品趣味血流麻将 add by liuhb01 2024.1.22
#define TK_GAMEID_MJFEIXJ 1150         //九天项目部云南飞小鸡麻将 add by chentw02 2024.2.28
#define TK_GAMEID_MJFUZHOU 1151        //九天项目部福州麻将 add by chentw02 2024.3.12
#define TK_GAMEID_MJHUASHUI 1152       //九天项目部划水麻将 add by chentw02 2024.4.15
#define TK_GAMEID_MJYIKX 1153          //成都平台棋牌 一口香麻将 add by chentw02 2024.6.17
#define TK_GAMEID_MJLYGC 1154          //成都平台棋牌 洛阳杠次麻将 add by chentw02 2024.7.29

#define TK_GAMEID_XIYOUJIE 2001     //西游劫 wuqy add 2011.7.25
#define TK_GAMEID_WAR3_DOTA 2002    //War3.Dota wuqy add 2011.7.26
#define TK_GAMEID_UNO 2003          //尤诺 wuqy add 2011.12.19
#define TK_GAMEID_CAPFISH 2004      //捕鱼达人 wuqy add 2012.4.11
#define TK_GAMEID_SKR 2005          //跑跑龟 wuqy add 2012.5.18
#define TK_GAMEID_BOXING 2006       //拳皇 wuqy add 2012.11.19
#define TK_GAMEID_FISH 2007         //捕鱼 wuqy add 2014.8.4
#define TK_GAMEID_FISH_SMOB 2008    //捕鱼(移动端-单机版/弱联网版) wuqy add 2014.8.4
#define TK_GAMEID_FISH_NMOB 2009    //捕鱼(移动端-网络版/强联网版) wuqy add 2015.3.4
#define TK_GAMEID_BALLWAR 2010      //球球 wuqy add 2016.6.21
#define TK_GAMEID_BOMBER 2011       //炸弹人 wuqy add 2016.8.22
#define TK_GAMEID_FISHQP 2013       //千炮捕鱼 wuqy add 2016.8.30
#define TK_GAMEID_FISHQP_MOB 2014   //千炮捕鱼(移动端) wuqy add 2016.8.30
#define TK_GAMEID_FISHQP_FMOB 2015  //海外千炮捕鱼(移动端) wuqy add 2017.3.30
#define TK_GAMEID_KILLING 2016      //JJ 狼人杀游戏 wuqy add 2017.6.8
#define TK_GAMEID_TETRIS 2017       //JJ 俄罗斯方块游戏 wuqy add 2017.6.19
#define TK_GAMEID_TANK 2018         //JJ 坦克大战游戏 wuqy add 2017.6.20
#define TK_GAMEID_BLACKJACK 2019    //JJ 海外二十一点游戏 wuqy add 2017.6.20
#define TK_GAMEID_FISH_QPM3D 2020   //移动端3D千炮捕鱼 wuqy add 2017.6.21
#define TK_GAMEID_FISHTV 2021       //TV捕鱼 wuqy add 2017.9.8
#define TK_GAMEID_JJUMP 2022        //JJ跳一跳 add by wuqy 2018.1.31
#define TK_GAMEID_PARKOURKING 2023  //JJ跑酷之王 add by xinxy 2018.4.13
#define TK_GAMEID_FISHQPTV 2024     //JJ TV千炮捕鱼 add by xinxy 2018.4.17
#define TK_GAMEID_JJFOOTBALL 2025   //JJFootball JJ足球 add by xinxy 2018.4.24
#define TK_GAMEID_FISHQPMSP 2026    //TKFishQPMSP 千炮捕鱼单包 add by xinxy 2018.10.10
#define TK_GAMEID_HUNTER_MOB 2027   //JJ猎人 add by xinxy 2018.10.31
#define TK_GAMEID_GOBLINMINER 2028  //地精矿工 add by xinxy 2019.4.29
#define TK_GAMEID_HUNTINGJOY 2029   //萌动工作室只申请了荒岛狩猎的Gameid add by xinxy 2019.5.23
#define TK_GAMEID_GOKART 2030       //起源工作室只申请了JJ卡丁车的Gameid add by xinxy 2019.6.20
#define TK_GAMEID_JJMAGICIAN 2031   //猎魔达人 add by xinxy 2019.11.01
#define TK_GAMEID_MAGICALTD 2032    //猎魔塔防 add by xinxy  2020.4.23
#define TK_GAMEID_TOWERBATTLE 2033  //棋牌产品中心塔防对战 add by xinxy 2020.8.12
#define TK_GAMEID_HXLZD 2034        //棋牌产品中心幻想隆中对 add by wangchi 2021.8.12
#define TK_GAMEID_GODSWORLD 2035    //棋牌产品中心神御五行 add by liuning01 2021.8.13
#define TK_GAMEID_LORDHZ 2036       //棋牌产品中心混子斗地主 add by luwq 2021.8.18
#define TK_GAMEID_MJHZXL 2037       //棋牌产品中心红中血流麻将 add by liuhb01 2021.9.3
#define TK_GAMEID_NEVERLAND 2038    //棋牌产品中心-竞娱组千炮捕鱼-梦幻岛 add by wangchi 2023.4.21

#define TK_GAMEID_JJGROUP 3000  //JJ群 wuqy add 2011.7.29
#define TK_GAMEID_LANTV 3001    //局域网TV版 wuqy 2014.6.4

//非平台架构上直接开发,GameID只是用于数据对接,便于后天数据统计
#define TK_GAMEID_XXDZZ 4001      //消消大作战 wuqy 2018.3.19
#define TK_GAMEID_KNIGHT 4002     //萌宠大作战 xinxy 2021.3.10
#define TK_GAMEID_DOMINOCLC 4003  //印尼Domino classic add by busy 2021.6.30
#define TK_GAMEID_DOMINOBET 4004  //印尼Domino Bet add by busy 2021.6.30
#define TK_GAMEID_DOMINOQIU 4005  //印尼Domino Qiu add by busy 2021.6.30
#define TK_GAMEID_BANDARQQ 4006   //海外BandarQQ add by busy 2022.2.28
#define TK_GAMEID_REMI 4007       //海外Remi add by busy 2022.11.24
#define TK_GAMEID_LUDO 4008       //海外Ludo add by busy 2023.04.10

#define TK_GAMEID_H5_SPR 5001        //H5猜拳小游戏wuqy 2017.5.3 （需求方:综合应用部-肖春胜）
#define TK_GAMEID_H5_SPORTTERY 5002  //H5竞彩赚金wuqy 2017.11.24

#define TK_BUFLEN_USERNAME 20  // 用户登陆帐号数据长度	(19+'\0')
#define TK_BUFLEN_NICKNAME 20  // 用户游戏昵称数据长度	(19+'\0')
#define TK_BUFLEN_PASSWORD 20  // 用户登陆帐号密码长度	(19+'\0')

#define TK_BUFLEN_SIGN 128  // 用户签名长度

#define TK_BUFLEN_SZCOMM 256  // 用户信息通用长度。

#define TK_BUFLEN_POST 8       // 邮编
#define TK_BUFLEN_COUNTY 64    // 县(区)
#define TK_BUFLEN_ADDRESS 128  // 地址

#define TK_BUFLEN_EMAIL 64                   // Email
#define TK_BUFLEN_MOBILENO 16                // 手机号码
#define TK_BUFLEN_PSWQUESTION 64             // 密码提示问题
#define TK_BUFLEN_PSWANSWER 64               // 密码提示问题答案
#define TK_BUFLEN_URL 128                    // URL
#define TK_BUFLEN_NEWPLAYERCARD 20           // 新手卡号
#define TK_BUFLEN_WEBNOTE (63 + 1)           // 网站的注释
#define TK_BUFLEN_CALLWEBHTTPLINK (255 + 1)  // 调用WebService的链接长度

#define TK_BUFLEN_PERSONALID 20  // 用户注册身份证件号	(18+'\0\0')
#define TK_BUFLEN_REALNAME 20    // 用户注册时的真实姓名	(8+'\0\0')
#define TK_BUFLEN_VERIFYCODE 20  // 验证码长度
#define TK_BUFLEN_DATETIME 32    // 字符串日期长度,存储DATATIME

#define TK_BUFLEN_SYSMSGTITLELEN (50)  // 系统消息Title长度
#define TK_BUFLEN_SYSMSG (512)         // 系统消息内容长度
#define TK_BUFLEN_NOTE (64)            // 标准注释

#define TK_BUFLEN_TOURNEYFULLNAME 80       // 赛事全称
#define TK_BUFLEN_TOURNEYSIMPLENAME 40     // 赛事简称
#define TK_BUFLEN_STAGENAME 40             // Stage名称
#define TK_BUFLEN_ROUNDRULERNAME 20        // 局制名称
#define TK_BUFLEN_MATCHAWARDNOTE 40        // 颁奖注释
#define TK_BUFLEN_CLIENTTITLE 60           // 客户端标题长度（比如斗地主客户端标题显示比赛名称相关信息）
#define TK_BUFLEN_ROUNDRULERNOTE 40        // 游戏局制规则说明
#define TKMATCHACTION_PARAM_LEN (255 + 1)  // 比赛过程Action参数字符串长度
#define TK_BUFLEN_PLACERORDERTITLE 60      // 比赛结果信息标题长度
#define TK_BUFLEN_MATCHROOMNAME 40         // 游戏房间名称

#define TK_BUFLEN_HOST 40      // 主机名
#define TK_BUFLEN_IP 16        // IP长度
#define TK_BUFLEN_MAC 24       // Mac地址长度
#define TK_BUFLEN_DEVICEID 33  // 设备ID 如 移动终端：IMEI，定义长度与MD5兼容

#define TK_BUFLEN_MD5 33           // MD5加密串长度
#define TK_BUFLEN_CURRENCYSIGN 20  // 货币符号
#define TK_BUFLEN_PAYORDEROD 64    // PayIN订单号

#define TK_BUFLEN_ENTERGAMETICKET 128  // 连接游戏服务器的Ticket长度

#define TK_BUFLEN_DYNCFILENAMELEN 40  // 需服务下载到客户端的，动态界面图片，头像，配置文件等等 的文件名长度

#define TK_BUFLEN_VIPTYPE_NAME (19 + 1)  // 大客户名缓冲区最大长度
#define TK_BUFLEN_VIPCER_NAME (19 + 1)   // 大客户货币券名缓冲区最大长度

#define TK_BUFLEN_GROWNAME (29 + 1)    // "养成"数据名长度
#define TK_BUFLEN_DOMAINNAME (29 + 1)  // "作用域"名长度(如：组委会)

#define TK_BUFLEN_MATCH_SIGNUPNOTE (128)  // 报名条件费用文字说明

#define TK_BUFLEN_SMS_CHECKCODE (4 + 1)  // 短信校验码长度
#define TK_BUFLEN_SMS_LONGCODE (31 + 1)  // 短信长代码长度

#define TK_BUFLEN_CHANNELNAME (63 + 1)       // 聊天室名字缓冲区长度
#define TK_BUFLEN_CHATKEY (35 + 1)           // 如果聊天需要加密的话，加密串缓冲区大小，实际大小拟用32
#define TK_BUFLEN_ENTERCHATTICKET (35 + 1)   // 如果聊天室需要票进入的话，票缓冲区长度
#define TK_BUFLEN_PROTOCOL_VERSION (15 + 1)  // 聊天协议缓存大小格式XXX.XXX.XXX.XXX

#define TK_BUFLEN_PLAYERDYNFIELDNAME 20  // 房间用户动态字段名称长度

#define TK_BUFFLEN_PARTNER_USERID (64 + 1)      // 对接 "合作方" 用户编号ID
#define TK_BUFFLEN_PARTNER_SESSIONID (256 + 1)  // 对接 "合作方" SessionID
#define TK_BUFFLEN_PARTNER_REGSIGN (512 + 1)    // 对接 "合作方" 验证签名
#define TK_BUFFLEN_PARTNER_NICK (63 + 1)        //合作方昵称

#define TK_BUFFLEN_BAIDU_SESSIONID (16 + 1)  // "百度" 用户状态唯一标识
#define TK_BUFFLEN_BAIDU_TIMESTAMP (20 + 1)  // "百度" 时间戳

#define TK_MAX_FREETOURNEYTEAM 81         //常规赛的每个小比赛的最大参赛Team数
#define TK_MAX_USERONETEAM 8              //每个参赛Team最大UserID数
#define TK_MAX_ROUNDPLAYER 10             //每个Round中的最大UserID数
#define TK_MAX_GAMEPROPLEN 40             //游戏属性字符串最大长度
#define TK_MAX_GAMEPROPEXLEN 256          //游戏扩展属性字符串最大长度
#define TK_MAX_GAMERESULTSCOREDETAIL 256  //游戏细节计分串

#define TK_BUFLEN_SNS_NAME (63 + 1)  // “关系系统”关系名称长度
#define TK_BUFLEN_SNS_NOTE (128)     // “关系系统”注释

#define TK_BUFLEN_MATCHTABLE_NAME 20  //赛桌名称
#define TK_BUFLEN_MATCHTABLE_NOTE 40  //赛桌说明

//==========================================================================================
// 用户数字ID区段定义
//#define TK_MIN_GAMEBOTUSERID		    400000000			//机器人UserID最小值
//#define TK_MAX_GAMEBOTUSERID		    100000000			//机器人UserID最大值
//#define ISBOTUSERID(id) (id<TK_MAX_GAMEBOTUSERID)			//是否机器人ID
//#define ISBOTUSERID(id) (id>=400000000)					//是否机器人ID

// 机器人ID
#define TK_USERDBPID_BEGIN_ROBOT (1000000)
#define TK_USERDBPID_END_ROBOT (1999999)

// 公社ID
#define TK_USERDBPID_BEGIN_COMMUNE (2000000)
#define TK_USERDBPID_END_COMMUNE (2499999)

// 网吧ID
#define TK_USERDBPID_BEGIN_NETBAR (2500000)
#define TK_USERDBPID_END_NETBAR (2999999)

// 产品讨论组
#define TK_USERDBPID_BEGIN_PRODUCTDISCUSS (3000000)
#define TK_USERDBPID_END_PRODUCTDISCUSS (3099999)

// Dota站队?
#define TK_USERDBPID_BEGIN_DOTAMATCHTEAM (3100000)
#define TK_USERDBPID_END_DOTAMATCHTEAM (3499999)

// 战队
#define TK_USERDBPID_BEGIN_MATCHTEAM (3500000)
#define TK_USERDBPID_END_MATCHTEAM (3999999)

// 队伍讨论组
#define TK_USERDBPID_BEGIN_TEAMDISCUSS (4000000)
#define TK_USERDBPID_END_TEAMDISCUSS (4499999)

// 群
#define TK_USERDBPID_BEGIN_GROUP (4500000)
#define TK_USERDBPID_END_GROUP (4999999)

// 应用测试号段
#define TK_USERDBPID_BEGIN_APPTEST (5000000)
#define TK_USERDBPID_END_APPTEST (5999999)

// 应用测试号段——猜拳机器人
#define TK_USERDBPID_BEGIN_APPTEST_H5SPR (5000000)
#define TK_USERDBPID_END_APPTEST_H5SPR (5000999)

/////////////////////////////////////////////////////////////////////////////////////////////
// 网页游戏来宾
#define TK_USERDBPID_BEGIN_WEBGUEST (90000000)
#define TK_USERDBPID_END_WEBGUEST (90999999)

// 大厅游戏来宾
#define TK_USERDBPID_BEGIN_LOBBYGUEST (91000000)
#define TK_USERDBPID_END_LOBBYGUEST (91999999)

// 手机游戏来宾
#define TK_USERDBPID_BEGIN_MOBILEGUEST (92000000)
#define TK_USERDBPID_END_MOBILEGUEST (92999999)

// 真实玩家
#define TK_USERDBPID_BEGIN_USERID (100000000)
#define TK_USERDBPID_END_USERID (1999999999)

//
#define ISBOTUSERID(id) (id >= 1000000 && id < 2000000)         // 是否机器人ID
#define ISCOMMUNEID(id) (id >= 2000000 && id < 2500000)         // 是否"公社"ID
#define ISNETBARID(id) (id >= 2500000 && id < 3000000)          // 是否"网吧"ID
#define ISPRODUCTDISCUSSID(id) (id >= 2500000 && id < 3100000)  // 是否"产品讨论组"ID
#define ISDOTAMATCHTEAMID(id) (id >= 3100000 && id < 3500000)   // 是否"Dota战队"ID
#define ISMATCHTEAMID(id) (id >= 3500000 && id < 4000000)       // 是否"战队"ID
#define ISTEAMDISCUSSID(id) (id >= 4000000 && id < 4500000)     // 是否"队伍讨论组"ID
#define ISGROUPID(id) (id >= 4500000 && id < 5000000)           // 是否"群"ID
#define ISWARBANDID(id) (id >= 5000000 && id < 20000000)        // 是否"新战队"ID
#define ISCLANID(id) (id >= 20000000 && id < 30000000)          // 是否"家族"ID
#define ISCYCLICALTEAMID(id) (id >= 40000000 && id < 50000000)  // 是否"周期性团队"ID

//
#define ISGUESTID(id) (id >= 90000000 && id < 100000000)
#define ISWEBGUESTID(id) (id >= 90000000 && id < 91000000)     // 是否"网页游戏"来宾ID
#define ISLOBBYGUESTID(id) (id >= 91000000 && id < 92000000)   // 是否"大厅"来宾ID
#define ISMOBILEGUESTID(id) (id >= 92000000 && id < 93000000)  // 是否"手机"来宾ID

#define ISREALUSERID(id) (id >= 100000000 && id < 2000000000)   // 是否真实注册用户ID
#define ISPRODUCTUSERID(id) (id >= 2000000000 && id < INT_MAX)  // 渠道发行的自研产品帐号ID
#define ISFORMALUSERID(id) (id >= 100000000 && id < INT_MAX)    // 是否真实注册用户ID

// 服务器之间传输文件的头结构
typedef struct tagTKTranFileHead
{
    char szFileName[TK_BUFLEN_DYNCFILENAMELEN];  // 文件名称
    DWORD dwFileLen;                             // 文件长度

} TKTRANFILEHEAD, *PTKTRANFILEHEAD;

// 通用后缀数据偏移索引
typedef struct tagTkSuffixIdx
{
    DWORD dwSuffixOffSet;  // 后缀数据偏移量，相对于消息缓冲区顶端，用于协议兼容
    DWORD dwSuffixSize;    // 后缀数据大小，用于校验

} TKSUFFIXIDX, *PTKSUFFIXIDX;

typedef struct tagTKADDRINFO
{
    DWORD dwIP;
    int nPort;
    char szIP[TK_BUFLEN_IP];
    char szHost[TK_BUFLEN_HOST];
} TKADDRINFO, *PTKADDRINFO;

struct TKIPAREA
{
    TKIPAREA()
    {
        Area_un.dwArea = 0;
    };
    DWORD Area()
    {
        return Area_un.dwArea;
    };
    void Area(DWORD dwArea)
    {
        Area_un.dwArea = dwArea;
    };
    DWORD Country()
    {
        return Area_un.Area_st.dwCountry;
    };
    void Country(DWORD dwCountry)
    {
        Area_un.Area_st.dwCountry = dwCountry;
    };
    DWORD City()
    {
        return Area_un.Area_st.dwCity;
    };
    void City(DWORD dwCity)
    {
        Area_un.Area_st.dwCity = dwCity;
    };
    DWORD ISP()
    {
        return Area_un.Area_st.dwISP;
    };
    void ISP(DWORD dwISP)
    {
        Area_un.Area_st.dwISP = dwISP;
    };

    union
    {
        DWORD dwArea;
        struct
        {
            DWORD dwCity : 20;     //城市编码，比如110113,北京顺义区
            DWORD dwCountry : 10;  //国家编码，比如 CN,dwCountry=(('C'-'A')<<5) +('N'-'A');
            DWORD dwISP : 2;       //网络运营商：01=联通，10=电信，11=移动，00=其他
        } Area_st;
    } Area_un;
};

typedef struct tagENTERROUNDPARAM
{
    DWORD dwMatchID;
    DWORD dwCheck;
    DWORD dwUserID;  //用户帐号对应的唯一数字编号ID
    time_t timeEnter;

    //以下三项信息有Lobby传递给GameClient，GameClient上发给GameService(数据是由LobbyService加密的)
    char szNickName[TK_BUFLEN_NICKNAME];
    int nFigureID;
    int nScore;  //比赛积分
} ENTERROUNDPARAM, *PENTERROUNDPARAM;

typedef struct tagTKAGENTPARAM
{
    DWORD dwType;
    ULONGLONG ullID;

    DWORD dwChannel;
    DWORD dwParam;

} TKAGENTPARAM, *PTKAGENTPARAM;

//-----------------------------------------------------------------------------------------------------
// “关系”索引
typedef struct tagSnsIdx
{
    WORD wSnsCID;  // “关系”类别ID：表示关系大类
                   //  例 如：“固定双向私人关系”，“临时双向组织关系”，……

    WORD wSnsTID;  // “关系”类型ID：表示关系种类
                   //  例 如：“好友关系”(属于“固定双向私人关系”类别)……
                   //  例 如：“公社关系”(属于“固定双向组织关系”类别)……
                   //  例 如：“网吧关系”(属于“临时双向组织关系”类别)……

    DWORD dwPID;  // “关系”主ID：
                  //  例 如：“好友关系”时为用户ID
                  //  例 如：“公社关系”时为组织ID
} SNSIDX, *PSNSIDX;
