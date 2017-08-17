#ifndef __GPRSPROTO__H
#define __GPRSPROTO_H

//unsigned char dxbjhm[2][29]={{0X36,0X38,0X33,0X31,0X37,0X39,0X31,0X33,0X30,0X30,0X35,0X31,0X46,0X37}};//8613973100157F  683179130051F7
unsigned char dxbjhm[2][29]={{0X36,0X38,0X35,0X31,0X30,0X38,0X38,0X33,0X37,0X32,0X37,0X36,0X46,0X39}};//8615803827679F  685108837276F9
unsigned char dxbjhm1_lenght,dxbjhmflag,dxbjhmflag0;

struct ID_BH
{
	unsigned short int cjbs;  //3位 厂家标识
	unsigned char zdlx; //2位 终端类型
 	unsigned short int scph;  //4位 生产批号 YYMM
	unsigned short int wybh;  //4位 唯一编号
	unsigned short int bbh;   //3位 版本号
	
}IDBH={0x682,0xB9,0x1705,0x0001,0x001}; //0x2aa=682





//使用参数
struct ZDParameter_T
{ 
	 //定位查询参数0x14
	 unsigned char 	IntervalTimer;
	 unsigned char 	TraceCount;

   unsigned char ZDID[8];    //终端ID
	 unsigned char 	LoginCount;
	 unsigned short int   LoginTimer; //270s 重发3次 后等待10分钟复位重启
	 unsigned char 	LoginReply;
	 unsigned short int   zdliushuinum; //主动流水号1~10000
	 unsigned short int  zhidonghbwzsc;  //自动回报位置时长
	 //uint16_t workStateTimer;

}ZDParameter;
  

//服务器下发命令帧
struct ZDCommand_T
{
     unsigned char start[2];	//起始符
	 unsigned char command;		//命令码
	 unsigned char Liushui[4];  //流水号
	 unsigned char ZID[8];		//终端 ID 号
	 unsigned char length[2];   //数据长度
	 unsigned char data[500];   //数据内容
	 unsigned char check;       //校验码
	 unsigned char finish;		//结束符
};
 struct ZDCommand_T    ZDCommand;
//终端上传应答帧
struct ZDReply_T
{
     unsigned char start[2];	//起始符
	 unsigned char command;		//命令码
	 unsigned char Liushui[4];  //流水号
	 unsigned char ZID[8];		//终端 ID 号
	 unsigned char length[2];   //数据长度
	 //unsigned char data[500];   //数据内容
	 unsigned char check;       //校验码
	 unsigned char finish;		//结束符
};
 struct ZDReply_T    ZDReply;





/*
例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
字段1：UTC时间，hhmmss.sss格式
字段2：状态，A=定位，V=未定位
字段3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
字段4：纬度N（北纬）或S（南纬）
字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
字段6：经度E（东经）或W（西经）
字段7：速度，节，Knots
字段8：方位角，度
字段9：UTC日期，DDMMYY格式
字段10：磁偏角，（000 - 180）度（前导位数不足则补0）
字段11：磁偏角方向，E=东W=西
字段16：校验值
*/

struct GPS_T
{
     unsigned char  year;      //位置/年				
     unsigned char  Month;		/* 日期 120598 ddmmyy */
	 unsigned char  Day;
	 unsigned char  Hour;		/* UTC 时间。 hhmmss.sss */
	 unsigned char  Min;		/* 分 */
	 unsigned char  Sec;		/* 秒 */
	 //uint16_t mSec;			/* 毫秒 */
	 
	 unsigned char   WD[4];		//纬度
	 //uint16_t WeiDu_Du;			//纬度
	 //uint16_t WeiDu_Fen;			//纬度

     unsigned char   JD[4];		//经度
	 //uint16_t JingDu_Du;			//纬度
	 //uint16_t JingDu_Fen;			//纬度

	 unsigned char   speed;		//速度
	 unsigned char   hx;	    //航向
	 unsigned char   state1;    //状态 1
	 unsigned char   state2;	//状态 2
	 unsigned char   state3;	//状态 3
	 unsigned char   state4;	//状态 4
};


#define  EAST  0x01
#define  WEST  0x02
#define  SOUTH 0x04
#define  NORTH 0x08


#define  WS 0x00   //(0x00 >> 6)
#define  WN 0x40   //(0x01 >> 6)
#define  ES 0x80   //(0x02 >> 6)
#define  EN 0xc0   //(0x03 >> 6)

//定位查询应答0x11,自动定时上报0x18
struct TerminalReply_0x110x18
{
	 struct GPS_T GPS;		           //位置信息
   unsigned char   fuel;		       //油耗采集
	 unsigned char   GPS_sum[4];	   //GPS 总里程数
	 unsigned char   ACC_ON_sum[4];	   //ACC ON 总累计时间
	 unsigned char   dh[2];            //当次点火后里程数
	 unsigned char   ACC_ON_sum_c[4];  //当次 ACC ON 累计时间
	 unsigned char   JZID[20];	       //基站 ID

}TerminalReply0x110x18;

struct mem_param
{
	unsigned int GPSvoidtimem ;//GPS故障分钟         4
	/*******0x110x18*****/
	unsigned char   GPS_sum[4];//GPS 总里程数        4
	unsigned char   ACC_ON_sum[4];//ACC ON 总累计时间   4  副参数
	/******副参数***/
	unsigned char   GPSztdsj[4];  //GPS总通电时间
	unsigned char   kaigainum;	       //开盖次数
	unsigned char   baGPSantnum;	       // 拔GPS天线次数
	unsigned char   baSIMnum;	       // 拔SIM卡次数
	/****************/
	unsigned char   simkaghcs ;//SIM卡更换次数
	unsigned int    flagchange ;//次数的更新状态  0开盖次数更新  1拔GPS天线次数更新  2拔SIM卡次数更新 
};
union memparam_set
 {
	struct mem_param	memparam;
	unsigned char mem_paramdata[24];
 }memparamset;

//参数设置0x12
struct TerminalReply_0x12
{
	unsigned char   caozuoma;		 //操作码
	unsigned char   duxieflag;	  //读写标志
	unsigned char * sblx;             		 //参数值
}TerminalReply0x12;

//参数设置项
struct Parameter_setitem
{
	unsigned char sbyhm0x00[8];   //设备用户码
	unsigned char hbsc0x03[2];		//自动回报位置信息时长
	unsigned char fwqIP0x04[6];		//服务器IP地址，端口号
	/*********///非本命令中参数
	unsigned char sjfwqIP0x04[6];		//0x41命令字，升级服务器IP地址，端口号
	/*********/
	unsigned char BaudRate0x07;		    //250 默认
  unsigned char IMSI0x11[20];		    // 只读  SIM唯一识别码
	
	/******/
  unsigned char ycsj0x15;	    //远程升级 0关闭 1开始  0
  unsigned char sdms0x16;	    //省电模式1开启 0关闭   1
	
	unsigned char xtbkz0x1b;	    //心跳包控制 1启动 0关闭
	unsigned char AAbjsjset0x1c;	    //0XAA报警时间设置
	unsigned char zdqyz0x1D;	    //主电源欠压值  11.5v 22.5v
  unsigned char bdsbzq0x1E;	    //启用备用电源后上报周期  24小时
	unsigned char PLCsrk0x20;	    //PLC输入口设置 0高电平 1低电平
  unsigned char wzhbcl0x41;	    //位置汇报策略   0 定时汇报 1定距汇报 2定时和定距
	unsigned char dssbsj0x42;	    //行驶中定时上报时间 30s
  unsigned char djsbjl0x43;	    //行驶中定距离上报距离 200m=20  分辨率10m

	unsigned  char sbbh0x17[50];	  //设备编号      "" <50
	unsigned  char SIMkh0x18[20];	//本终端SIM卡号 "" <20
  unsigned  char hjzxhm0x19[20];	//呼叫中心号码  "" <20
	unsigned  char sblx0x22[20];	  //设备类型 ""<=20
  unsigned  char dxbjhm0x09[29];	//""  短信报警号码  106575531157170  暂定29位
	 /*******///不用保存项  13  保存205-13=192 =48*4
	unsigned  char APN0x05[5];  //""  <=64  CMNET 只读
	 unsigned char UDPTCP0x06;		    //1 TCP只读
	 unsigned char ckgn0x0e;		    //0 国内 只读
	 unsigned char GSMxhqd0x12;		    // 只读 GSM信号强度
	 unsigned char GPSwxs0x13;		    // 只读  GPS卫星颗数
	 unsigned char GPScslc0x14[4];	    //初始化里程
   
};

union Parameter_set
 {
	struct Parameter_setitem	Parametersetitem;
	unsigned char Parameter_setitemdata[205];
 }Parameterset;

//can总线命令下发与数据上传0x13 回复2.b终端应答
//2.b终端应答
struct TerminalReply_0x1a
{
   unsigned char   command;          //命令字
	 unsigned char   success;	       //成功标志
// 	 struct GPS_T GPS;		           //位置信息
//    unsigned char   fuel;		       //油耗采集
// 	 unsigned char   GPS_sum[4];	   //GPS 总里程数
// 	 unsigned char   ACC_ON_sum[4];	   //ACC ON 总累计时间
// 	 unsigned char   dh[2];            //当次点火后里程数
// 	 unsigned char   ACC_ON_sum_c[4];  //当次 ACC ON 累计时间
// 	 unsigned char   JZID[20];	       //基站 ID
	struct TerminalReply_0x110x18 TerminalReply0x110x18;

}TerminalReply0x1a;

//中心应答命令 0x14


//终端上传报警信息0x17 有中心应答0x14  15s无应答重发3次
struct TerminalReply_0x17
{
	 unsigned char   baojingflag;	      //报警标志
// 	 struct GPS_T GPS;		           //位置信息
//    unsigned char   fuel;		       //油耗采集
// 	 unsigned char   GPS_sum[4];	   //GPS 总里程数
// 	 unsigned char   ACC_ON_sum[4];	   //ACC ON 总累计时间
// 	 unsigned char   dh[2];            //当次点火后里程数
// 	 unsigned char   ACC_ON_sum_c[4];  //当次 ACC ON 累计时间
// 	 unsigned char   JZID[20];	       //基站 ID
	struct TerminalReply_0x110x18 TerminalReply0x110x18;

}TerminalReply0x17;

//副参数上报
struct TerminalReply_0x28
{
	 unsigned char   zhudianvol[2];	  //主电电压
	 unsigned char   beidianvol;	   //备电电压
   unsigned char   neibuwendu;		 //终端内部温度
	 unsigned char   zhudianXMJG;	   //主动休眠间隔
	 unsigned char   beidianXMJG;	   //备电休眠间隔
	 unsigned char   ACC_ON_sum_c[4];  //当次 ACC ON 累计时间
	 unsigned char   GPSztdsj[4];  //GPS总通电时间
	 unsigned char   kaigainum;	       //开盖次数
	 unsigned char   baGPSantnum;	       // 拔GPS天线次数
	 unsigned char   baSIMnum;	       // 拔SIM卡次数
	 unsigned char   GSMxinhao;	       // GSM信号强度

}TerminalReply0x28;


 

//0x19登录中心服务器
struct LoginCen_T
{
   unsigned char sbyhm[8];    //设备用户码
	 unsigned char GPS_gzts;    //GPS 故障天数
	 unsigned char xhr[2];      //GSM/GPRS 信号弱报警小时
	 unsigned char hbsc[2];		//自动回报位置信息时长
	 char *	 bjhm;	            //短信报警号码
   char * sbbh;               //设备编号
   char * SIMh;               //本终端内 SIM 卡号
   char * sblx;               //设备类型
};

 struct LoginCen_T  LoginCen;






// struct Capacity_T
// {
// 	unsigned char command;
// 	unsigned char len;
// 	unsigned char data[3];
// };
// struct GZ_T
// {
// 	unsigned char command;
// 	unsigned char len;
// 	unsigned char count;
// 	unsigned char data[10][3];
// };





// #define GuoManDeng    0x80  //谷满灯
// #define YeWeiDeng     0x40  //液位灯
// #define QiYaDeng      0x20  //气压报警灯
// #define YouLiangDeng  0x10	//油量报警灯
// #define YouYaDeng     0x08	//油压报警灯
// #define ShuiWenDeng   0x04	//水温报警灯
// #define YouShuiDeng   0x02  //油水分离灯
// #define GuoZhangDeng  0x01	//故障灯

// //#define GuoManDeng    0x80  //(预留)
// #define ZhucheDeng     0x40  //驻车制动灯
// #define ChongDianDeng  0x20  //充电灯
// #define YouLvDeng      0x10	//油滤灯
// #define YuReDeng       0x08	//预热灯
// #define KongLvDeng     0x04	//空滤灯
// #define jiaoLongDeng   0x02  //二号搅龙灯
// #define QiShaDeng      0x01	//气刹灯
// extern uint8_t deng[3]; 



#endif 








