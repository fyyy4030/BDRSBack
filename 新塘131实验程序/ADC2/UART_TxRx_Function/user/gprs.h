#ifndef __GPRS__H
#define __GPRS_H
#include <stdlib.h>
#include <string.h>	 
#include <stdio.h>
#include "NUC131.h"
#include "gprsproto.h"
#include "sms.h"
#include "canMessage.h"
// const char *at_command[12]={        "at\r",   "ATE0\r",  "AT+CPIN?\r",      "at+csq\r", "AT+CGATT=1\r",  "AT+CGATT?\r",  "AT+CIPSHUT\r", "AT+CSTT=\"CMNET\"\r",  "AT+CIICR\r",  "AT+CIFSR\r","AT+CIPSTART=\"TCP\",\"171.8.225.246\",8002\r","AT+CIPSEND=" };
// //const char *at_command[12]={        "at\r",   "ATE0\r",  "AT+CPIN?\r",      "at+csq\r", "AT+CGATT=1\r",  "AT+CGATT?\r",  "AT+CIPSHUT\r", "AT+CSTT=\"CMNET\"\r",  "AT+CIICR\r",  "AT+CIFSR\r","AT+CIPSTART=\"TCP\",\"222.240.204.195\",2603\r","AT+CIPSEND=" };
// //const char *at_command_reply[13]={"OK","OK","+CPIN: READY","OK","OK", "+CGATT: 1", "SHUT OK", "OK",            "OK",  "",          "CONNECT OK",                              ">","SEND OK"  };
// const char *at_command_reply[13]={"OK","OK","OK","OK","OK", "+CGATT: 1", "SHUT OK", "OK",            "OK",  "",          "CONNECT OK",                              ">","SEND OK"  };

//const char *at_command[18]={        "at\r",   "ATE0\r", "AT+CMGF=0\r", "AT+CPIN?\r", "AT+CIMI\r",   "at+csq\r","at+creg=2\r","at+creg?\r", "AT+CGATT=1\r",  "AT+CGATT?\r",  "AT+CIPSHUT\r", "at+cipmux?\r","at+cipmux=1\r","AT+CSTT=\"CMNET\"\r",  "AT+CIICR\r",  "AT+CIFSR\r","AT+CIPSTART=0,\"TCP\",\"171.8.225.246\",8002\r","AT+CIPSEND=0," };
const char *at_command[18]={        "at\r",   "ATE0\r", "AT+CPIN?\r","AT+CMGF=0\r", "AT+CIMI\r",   "at+csq\r","at+creg=2\r","at+creg?\r", "AT+CGATT=1\r",  "AT+CGATT?\r",  "AT+CIPSHUT\r", "at+cipmux?\r","at+cipmux=1\r","AT+CSTT=\"CMNET\"\r",  "AT+CIICR\r",  "AT+CIFSR\r","AT+CIPSTART=0,\"TCP\",\"171.8.225.246\",8003\r","AT+CIPSEND=0," };
//const char *at_command[18]={        "at\r",   "ATE0\r", "AT+CPIN?\r","AT+CMGF=0\r", "AT+CIMI\r",   "at+csq\r","at+creg=2\r","at+creg?\r", "AT+CGATT=1\r",  "AT+CGATT?\r",  "AT+CIPSHUT\r", "at+cipmux?\r","at+cipmux=1\r","AT+CSTT=\"CMNET\"\r",  "AT+CIICR\r",  "AT+CIFSR\r","AT+CIPSTART=0,\"TCP\",\"192.168.1.136\",8003\r","AT+CIPSEND=0," };
//const char *at_command[17]={        "at\r",   "ATE0\r",  "AT+CPIN?\r", "AT+CIMI\r",   "at+csq\r","at+creg=2\r","at+creg?\r", "AT+CGATT=1\r",  "AT+CGATT?\r",  "AT+CIPSHUT\r", "at+cipmux?\r","at+cipmux=1\r","AT+CSTT=\"CMNET\"\r",  "AT+CIICR\r",  "AT+CIFSR\r","AT+CIPSTART=0,\"TCP\",\"222.240.204.195\",2603\r","AT+CIPSEND=0," };
//const char *at_command_reply[13]={"OK","OK","+CPIN: READY","OK","OK", "+CGATT: 1", "SHUT OK", "OK",            "OK",  "",          "CONNECT OK",                              ">","SEND OK"  };
const char *at_command_reply[19]={"OK","OK","OK","OK","OK","OK","OK","OK","OK", "+CGATT: 1",  "SHUT OK","OK","OK", "OK",            "OK",  "OK",         "CONNECT OK",                              ">","SEND OK"  };


//#define ATE1
//TCP上传数据  
unsigned   char CommSended = 0;
unsigned   char DataSended = 0;

#define UART_BUFFSIZE	256
#define sysTxBufReadNextOne()	(((_sys_uUartTxHead+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxHead+1)
#define sysTxBufWriteNextOne()	(((_sys_uUartTxTail+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxTail+1)
unsigned char _sys_ucUartTxBuf[UART_BUFFSIZE];
unsigned int  _sys_uUartTxHead, _sys_uUartTxTail;

// #define RX_ARRAY_NUM 200
// unsigned char uart_rx[RX_ARRAY_NUM] = {0};
// unsigned char uart_rx2[RX_ARRAY_NUM] = {0};
// unsigned long rx_cnt = 0;
// unsigned long rx_cnt2 = 0;
char CCID[20];
char havegetccid = 0;


#define COMMSENDING 1
#define COMMSENDED 0
#define	 GPRSPOWEROFF  (PB3=1);
#define	 GPRSPOWERON   (PB3=0);

unsigned char at_command_order = 0, AtCommSendingState = COMMSENDED,GprsSending = 0 ;
unsigned long gprsNoConnect = 0;
unsigned long Gprs_Timeout_Count;
unsigned char bufloc = 0, smsnumbufloc = 0, smsdatabufloc = 0;
uint16_t readsmsnumok;
unsigned char smsnumber,smsdataber,cregdataber;
unsigned char smsdataend,smsdatarecnum,issms,cregdatarecnum;
unsigned char longsms;
char strbuf[50];
char smsnumbuf[50];
char smsnum[10][3];//第3位为空
//char smsdatabuf[3][150];

uint32_t Timer1Count = 0,Timer11Count = 0,GPSvoidtimem=0,CANvoidtimes=0;
uint8_t  GprsConnectTcpState = 0,GPSvoidtimes=0;
uint16_t  delay_ns = 50,autorep_loctime;
uint8_t LengthAsc[3];
unsigned char GprsRevCheck;	
unsigned char GprsSndCheck;
unsigned char GprsRevOver;

uint8_t GprsRevBuf[ 200 ];
uint8_t GprsRevFlag;
uint8_t GprsRevLen;
uint8_t GprsRevCount;
uint8_t GprsRevTurn,GprsRevTurntime=0;

// #define GPRS_NL_BUF_LEN  200
// uint8_t GprsNLBuf[GPRS_NL_BUF_LEN];
// uint8_t GprsCheck;		
uint16_t GprsNLCount;


uint16_t datalenthlogin,datalenth2B;
//uint8_t dataArrived2B; 
uint8_t state=0,TimeArrivedlogin=0,TimeArrived2B=0,TimeArrived0x13=0,TimeArrived0x18=0,TimeArrived0x11=0,TimeArrived0x28=0,TimeArrived12=0,TimeArrived0x17=0; 
//uint8_t datatest1,datatest2;
uint16_t sendbjxx0x17time=0;
uint8_t sendbjxx0x17count,sendbjxx0x17repovertime=15;//应答15s	
uint8_t TimeArrivedcsq=0,TimeArrivedcreg=0;
uint8_t datachar;
uint8_t memchange=0;//数据更改标识，需要做一次存储
uint32_t BaudRate  ; 
//GPS参数
uint8_t GpsRevLen;
uint8_t GpsRevCount,GpggaRevCount;
uint8_t GpsRevTurn,GpggaRevTurn;
uint8_t gpsdats[100],gpggadats[100];
uint8_t GpsRecievedNew;





uint32_t speed_s,gps_tem_lc,gps_tot_lc,gps_cur_lc,ACCon_tot_time,ACCon_cur_time,gps_tot_time,gps_dj_lc;
uint8_t ACCon_tem_time,gps_tem_time,baGPSantrecord;
uint8_t IMSIdata[20],simreadyerror=0,CGATTerrorcount=0,CREGstat=0,JZ_IDnumb,JZ_ID[20],Gprsdatacapstat;
uint32_t bjflag;//相关报警发送过的标记
uint8_t checkcount;//各项检测检测2次
uint8_t vibrationtime=60;//振动检测时间
uint16_t ferrytime=0;//转场时间
uint8_t canBaudRatechange=0;//波特率变化
uint8_t numdisp_testok,numdisp_testtime,GPShighlevelchecktime=0;
uint16_t numdispflag=0x300;//数字显示标志8每次都显示

uint8_t SMSdatacapstat,SMSdatasendstat,smsDataSended,smsCommSended,deletereadedsms,pdutoASC;
uint8_t closegprsnet,dormantgprs,recgprscom;


char revtemp[1];

void Gprs_rev(char*rev,unsigned char selecissms);
void Gprs2Bdatasend(void);
void GprsCANdatasend(void);
void Gprs1811datasend(void);
void Gprs28datasend(void);
void Gprs12datasend(void);
void Gprs1811data_init(void);
void Gprs17datasend(void);

void SMSsend_init(unsigned char len);
unsigned char SMSSendData(unsigned short int len,unsigned char sernum);

void initZDID(void)
{
	ZDParameter.ZDID[0] =(IDBH.cjbs&0xfff)>>4;  //限制3位
	ZDParameter.ZDID[1] =((IDBH.cjbs&0xfff)<<4)+(IDBH.zdlx>>4);  
	ZDParameter.ZDID[2] =(IDBH.zdlx<<4)+(IDBH.scph>>12);  
  ZDParameter.ZDID[3] =((IDBH.scph&0x0ff0)>>4); 
	ZDParameter.ZDID[4] =(IDBH.scph<<4)+(IDBH.wybh>>12); 
	ZDParameter.ZDID[5] =((IDBH.wybh&0x0ff0)>>4); 
	ZDParameter.ZDID[6] =(IDBH.wybh<<4)+(IDBH.bbh>>8); 
	ZDParameter.ZDID[7] =IDBH.bbh; 
}

//交换号码位置
 void moubilenumswitch(void)
 { uint8_t i,temp;
	 for(i=0;i<29;i++)
	  dxbjhm[1][i]=Parameterset.Parametersetitem.dxbjhm0x09[i];
	  dxbjhm1_lenght=strlen((char*)dxbjhm[1]);
	  if(dxbjhm1_lenght%2)//变偶数
		{
			dxbjhm[1][dxbjhm1_lenght]=0X46;//F
			//dxbjhm1_lenght+=1;保持原始数据
		}
	  for(i=0;i<(dxbjhm1_lenght+dxbjhm1_lenght%2);i+=2)
		{
		temp=dxbjhm[1][i];
		dxbjhm[1][i]=dxbjhm[1][i+1];
		dxbjhm[1][i+1]=temp;
		}
 }

/*
*********************************************************************************************************
*	函 数 名: StrToIntFix
*	功能说明: 将ASCII码字符串转换成十进制, 给定长度
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*			 _ucLen : 固定长度
*	返 回 值: 二进制整数值
*********************************************************************************************************
*/
int32_t StrToIntFix(char *_pStr, uint8_t _ucLen)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* 负数 */
		p++;
		_ucLen--;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < _ucLen; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}

/*
*********************************************************************************************************
*	函 数 名: StrToInt
*	功能说明: 将ASCII码字符串转换成十进制
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*	返 回 值: 二进制整数值
*********************************************************************************************************
*/
int32_t StrToInt(char *_pStr)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* 负数 */
		p++;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < 15; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
		{
			p++;
			ucTemp = *p;
			break;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}

/*
*********************************************************************************************************
*	函 数 名: StrToInt
*	功能说明: 将ASCII码字符串转换成十进制
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*	返 回 值: 二进制整数值
*********************************************************************************************************
*/
uint8_t readIMSI(char *_pStr)
{
	uint8_t flag=0;
	char *p=_pStr;
	uint8_t i,j=0;
	uint8_t ucTemp;
	for (i = 0; i < 20; i++)
	IMSIdata[j]=0;//清除记录区
	for (i = 0; i < 50; i++)//strbuf缓冲区是50
	{
		ucTemp = *p;
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			IMSIdata[j]=ucTemp;
			j++;
			if(j>20)
			//break;	
			return 0;//超出20个，数据有误
			p++;
			flag=1;
		}
		else
		{
			if(flag)
			break;
		}
	}
	return j;
}
uint8_t readCREG(char *_pStr)
{
	char *p=_pStr;
	uint8_t i,j=0,k=0;
	uint8_t ucTemp;
	p = (char *)strchr(p, ',');
	p++;
	CREGstat=*p-0x30;
	p+=2;
	for (i = 0; i < 20; i++)
	JZ_ID[i]=0;//清空记录区
	for (i = 0; i < 30; i++)
	{
		ucTemp = *p;
		if (ucTemp == '"') 
		{
			j++;
			if(j>=4)
			break;
		}
		 else
		 {
			JZ_ID[k]=ucTemp;
			 k++;
		 }
			p++;
	}
	if(i>=30)//获取失败
	return 0;
  else	
	return k;
}
//+CREG: 2,1,"39C6","552B"
 void  rec_cregdata(char *rev)//严格控制接收错误
 {
	  unsigned char i;
	 switch(cregdataber)
	 {
		 case 0: if(*rev == '+')	{cregdataber = 1;JZ_IDnumb=0;cregdatarecnum=0;} else cregdataber	= 0; break;
     case 1: if(*rev == 'C')	cregdataber = 2; else cregdataber	= 0; break;
     case 2: if(*rev == 'R')	cregdataber = 3; else cregdataber	= 0; break;
     case 3: if(*rev == 'E')	cregdataber = 4; else cregdataber	= 0; break;
     case 4: if(*rev == 'G')  cregdataber = 5; else cregdataber	= 0; break;
     case 5: cregdatarecnum++;
		         if(cregdatarecnum<6)
				    {
					   if(*rev == ',')
						 { 
							 cregdataber = 6;
							 cregdatarecnum=0;
						 }
				    }
						else
						{cregdataber = 0;cregdatarecnum=0;}	
				 break;
		 case 6:  if((*rev >= 0x30)&&(*rev <= 0x39))
		          {
								CREGstat=*rev-0x30; 
								cregdataber = 7;
								for (i = 0; i < 20; i++) JZ_ID[i]=0;	
              }
							else
							{cregdataber = 0;}	
			        break;   //清空记录区
		 case 7: if(*rev == ',')  cregdataber = 8;else cregdataber	= 0;break;							
     case 8:   if (*rev == '"') 
		          {
							 cregdatarecnum++;
							 if(cregdatarecnum>=4)
							 {
								 for(i=0;i<20;i++)//重新赋值
								TerminalReply0x110x18.JZID[i]=JZ_ID[i];
								switch(CREGstat)
								{
									case 0: numdispflag|=BIT(7);break;// 7无移动网络信号（没有注册到网络也没检测到信号）
									case 1: numdispflag&=~0x0085;break;         //0x0085 注册到本地网络
									case 2: numdispflag|=BIT(2);break;// 2 终端无法注册移动网络（检测到网络但没有注册到网络）
									case 3: numdispflag|=BIT(0);break;// 0注册被拒绝
									case 4: numdispflag&=~0x0085;break;//未知状态
									case 5:numdispflag&=~0x0085;break;         //0x0085 注册到网络 有漫游
									default:break;
                }
								 cregdataber=0;
								 cregdatarecnum=0;
								  JZ_IDnumb=0;
							 }
             }
		         else
		         {
							if(((*rev >= 0x30)&&(*rev <= 0x39))||((*rev >= 0x41)&&(*rev <= 0x46))||(*rev==','))//PDU数据
							{	
		      	   JZ_ID[JZ_IDnumb]=*rev;
			         JZ_IDnumb++;
							 if(JZ_IDnumb>18)//接收错误
							 {
								 JZ_IDnumb=0;
								 cregdataber=0;
								 cregdatarecnum=0;
               }
						  }
							else//接收错误
							 {
								 JZ_IDnumb=0;
								 cregdataber=0;
								 cregdatarecnum=0;
                }		
		         }
		          break;
			default: break;						 
		 }
	}		 
		 
		 

 int32_t gpsspeed(char *p)
 {
	
	uint32_t ulInt=0;
	uint8_t i,uci=0,ucTemp;
	 for (i = 0; i < 4; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
		{
			p++;
			ucTemp = *p;
			uci=i;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			return 0;
		}
	}
	ulInt=ulInt*(uci-1)*10;//扩大1000倍
	ulInt=ulInt*1852/3600;//每秒米数
	return ulInt;
 }
/*
*********************************************************************************************************
*	函 数 名: gpsGPRMC
*	功能说明: 分析0183数据包中的 GPGSV 命令，结果存放到全局变量
*	形    参:  _ucaBuf  收到的数据
*			 _usLen    数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void gpsGPRMC(uint8_t *_ucaBuf)
{
		char *p;
	uint16_t cca;
	uint8_t temp=0;

	p = (char *)_ucaBuf;
	//p[_usLen] = 0;

	/* 字段1 UTC时间，hhmmss.sss格式 */
	p = (char *)strchr(p, ',');
	if (p == 0)
	{
		return;
	}
	p++;
	TerminalReply0x110x18.GPS.Hour = StrToIntFix(p, 2);
	p += 2;
	TerminalReply0x110x18.GPS.Min = StrToIntFix(p, 2);
	p += 2;
	TerminalReply0x110x18.GPS.Sec = StrToIntFix(p, 2);
	p += 3;
	//g_tGPS.mSec = StrToIntFix(p, 3);

	/* 字段2 状态，A=定位，V=未定位 */
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		return;
//	}
	p++;
	if (*p != 'A')
	{
		/* 未定位则直接返回 */
		TerminalReply0x110x18.GPS.state1 &= ~0x80;
//		return;
	}
	else
	TerminalReply0x110x18.GPS.state1 |= 0x80;
	p += 1;

	/* 字段3 纬度ddmm.mmmm，度分格式（前导位数不足则补0） */
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		return;
//	}
	p++;
	TerminalReply0x110x18.GPS.WD[0] = StrToIntFix(p, 2);
	p += 2;
	//TerminalReplyCommand.GPS_Fen = StrToIntFix(p, 2) * 10000;
	//TerminalReplyCommand.GPS.WeiDu_Fen = StrToIntFix(p, 2);
	TerminalReply0x110x18.GPS.WD[1] = StrToIntFix(p, 2);
	//p += 3;
	p += 2;
	cca = StrToIntFix(p, 4);
	TerminalReply0x110x18.GPS.WD[2] = cca >> 8;
	TerminalReply0x110x18.GPS.WD[3] = cca;
	p += 4;

	/* 字段4 纬度N（北纬）或S（南纬）*/
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		return;
//	}
	p++;
	if (*p == 'N')
	{
		temp |= NORTH;
	}
	else if (*p == 'S')
	{
		temp |= SOUTH;
	}
//	else
//	{
//		return;
//	}

	/* 字段5 经度dddmm.mmmm，度分格式（前导位数不足则补0） */
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		return;
//	}
	p++;
	TerminalReply0x110x18.GPS.JD[0] = StrToIntFix(p, 3);
	p += 3;
	//TerminalReplyCommand.GPS.JingDu_Fen = StrToIntFix(p, 2) * 10000;
	//TerminalReplyCommand.GPS.JingDu_Fen = StrToIntFix(p, 2);
	TerminalReply0x110x18.GPS.JD[1] = StrToIntFix(p, 2);
	//p += 3;
	p += 2;
	cca = StrToIntFix(p, 4);
	TerminalReply0x110x18.GPS.JD[2] = cca >> 8;
	TerminalReply0x110x18.GPS.JD[3] = cca;
	p += 4;

	/* 字段6：经度E（东经）或W（西经） */
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		return;
//	}
	p++;
	if (*p == 'E')
	{
//		temp |= EAST;
		if((temp & SOUTH) == SOUTH)
		{
			TerminalReply0x110x18.GPS.year = ES;
		}
		else if((temp & NORTH) == NORTH)
		{
			TerminalReply0x110x18.GPS.year = EN;
		}
	}
	else if (*p == 'W')
	{
//		temp |= WEST;
		if((temp & SOUTH) == SOUTH)
		{
			TerminalReply0x110x18.GPS.year = WS;
		}
		else if((temp & NORTH) == NORTH)
		{
			TerminalReply0x110x18.GPS.year = WN;
		}
	}




	/* 字段7：速度，节，Knots  10.05,*/
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		return;
//	}
	p++;
	speed_s=gpsspeed(p);
	TerminalReply0x110x18.GPS.speed = StrToInt(p);

	/* 字段8：方位角，度 ,324.27 */
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		return;
//	}
	p++;
	TerminalReply0x110x18.GPS.hx = StrToInt(p)/2;

	/* 字段9：UTC日期，DDMMYY格式  150706 */
	p = (char *)strchr(p, ',');
//	if (p == 0)
//	{
//		//return;
//	}
	p++;
	TerminalReply0x110x18.GPS.Day = StrToIntFix(p, 2);
	p += 2;
	TerminalReply0x110x18.GPS.Month = StrToIntFix(p, 2);
	p += 2;
	temp = StrToIntFix(p, 2);
	TerminalReply0x110x18.GPS.year |= temp & 0x3f;
	p += 2;
	
// 	TerminalReply0x1a.TerminalReply0x110x18=TerminalReply0x110x18;
// 	TerminalReply0x17.TerminalReply0x110x18=TerminalReply0x110x18;
	
}




void Gps_rev(char * rev)
{
  	if(GpsRevTurn == 0)
	{
		if(*rev == '$')
		{
		    GpsRevTurn = 1;
			GpsRevCount = 0;
			gpsdats[ GpsRevCount++ ] = *rev;
		}
	    GpsRecievedNew	= 0;
	}
	else if(GpsRevTurn == 1) 	
	{
		if(*rev == 'G')
		{
		   GpsRevTurn = 2;
		   gpsdats[ GpsRevCount++ ] = *rev;
		}
		else
		{
			GpsRevTurn = 0;
		}
	}
	else if(GpsRevTurn == 2)
	{
		if(*rev == 'N')
		{
		   GpsRevTurn = 3;
		   gpsdats[ GpsRevCount++ ] = *rev;
		}
		else
		{
			GpsRevTurn = 0;
		}
	}
	else if(GpsRevTurn == 3)
	{
		if(*rev == 'R')
		{
		   GpsRevTurn = 4;
		   gpsdats[ GpsRevCount++ ] = *rev;
		}
		else
		{
			GpsRevTurn = 0;
		}
	}
	else if(GpsRevTurn == 4)
	{
		if(*rev == 'M')
		{
		   GpsRevTurn = 5;
		   gpsdats[ GpsRevCount++ ] = *rev;
		}
		else
		{
			GpsRevTurn = 0;
		}
	}
	else if(GpsRevTurn == 5)
	{
		if(*rev == 'C')
		{
		   GpsRevTurn = 6;
		   gpsdats[ GpsRevCount++ ] = *rev;		 
		}
		else
		{		
			GpsRevTurn = 0;
		}
	}
	else if(GpsRevTurn == 6)
	{
	    if(*rev == '\n')
		{
		    gpsdats[ GpsRevCount++ ] = *rev;
		//	memcpy((void *)(gpsdats),(void *)(gpsdatsbuf),68);
			//memcpy((void *)(CarDataP->GpsData),(void *)(gpsdats),GpsRevCount-2);	//-2是去掉末尾的回车换行
			gpsGPRMC(gpsdats);

			GpsRevCount = 0;
			GpsRevTurn = 0;
			GpsRecievedNew = 1;			
		}
		else
		{
			gpsdats[ GpsRevCount++ ] = *rev;
		}
	}
	else
	{
		GpsRevTurn = 0;

	}

}
void gpsGPGGA(uint8_t *_ucaBuf)
{
		char *p;
	uint8_t i=0;

	p = (char *)_ucaBuf;
  while((p=(char *)strchr(p, ','))>0)
  {
		p++;
		i++;
		if(i==7)
			break;
		else if(i>7)
			return;
  }
	if (p == 0)
	{
		return;
	}
	p++;
	Parameterset.Parametersetitem.GPSwxs0x13=StrToIntFix(p, 2);	
}



void Gpgga_rev(char * rev)
{
  	if(GpggaRevTurn == 0)
	{
		if(*rev == '$')
		{
		  GpggaRevTurn = 1;
			GpggaRevCount = 0;
			gpggadats[ GpggaRevCount++ ] = *rev;
		}
	    
	}
	else if(GpggaRevTurn == 1) 	
	{
		if(*rev == 'G')
		{
		   GpggaRevTurn = 2;
		   gpggadats[ GpggaRevCount++ ] = *rev;
		}
		else
		{
			GpggaRevTurn = 0;
		}
	}
	else if(GpggaRevTurn == 2)
	{
		if(*rev == 'N')
		{
		   GpggaRevTurn = 3;
		   gpggadats[ GpggaRevCount++ ] = *rev;
		}
		else
		{
			GpggaRevTurn = 0;
		}
	}
	else if(GpggaRevTurn== 3)
	{
		if(*rev == 'G')
		{
		   GpggaRevTurn = 4;
		   gpggadats[ GpggaRevCount++ ] = *rev;
		}
		else
		{
			GpggaRevTurn = 0;
		}
	}
	else if(GpggaRevTurn == 4)
	{
		if(*rev == 'G')
		{
		   GpggaRevTurn = 5;
		   gpggadats[ GpggaRevCount++ ] = *rev;
		}
		else
		{
			GpggaRevTurn = 0;
		}
	}
	else if(GpggaRevTurn == 5)
	{
		if(*rev == 'A')
		{
		   GpggaRevTurn = 6;
		   gpggadats[ GpggaRevCount++ ] = *rev;		 
		}
		else
		{		
			GpggaRevTurn = 0;
		}
	}
	else if(GpggaRevTurn == 6)
	{
	    if(*rev == '\n')
		{
		    gpggadats[ GpggaRevCount++ ] = *rev;
		//	memcpy((void *)(gpsdats),(void *)(gpsdatsbuf),68);
			//memcpy((void *)(CarDataP->GpsData),(void *)(gpsdats),GpsRevCount-2);	//-2是去掉末尾的回车换行
			gpsGPGGA(gpggadats);

			GpggaRevCount = 0;
			GpggaRevTurn = 0;
			
		}
		else
		{
			gpggadats[GpggaRevCount++ ] = *rev;
		}
	}
	else
	{
		GpggaRevTurn = 0;

	}

}



 void  rec_comrep(char *rev)
 {
	 //非服务器下发命令
			if(*rev>0x1f)
			{
			strbuf[bufloc]=*rev ;					
			bufloc++;
			if (bufloc >=50 ) 
		  bufloc = 0;
		  }
	
 }
 //+CMTI: "SM",1
 void  rec_smsnumb(char *rev)
 {
	if(smsnumber == 0)	 //
	{
		if(*rev == '+')		  
		{
		  smsnumber = 1;
// 			smsnumbuf[smsnumbufloc]=*rev;
// 			smsnumbufloc++;
		}
		else
		{
			smsnumber	= 0;			
		}
	    
	}
	else if(smsnumber == 1) 	 //
	{
		if(*rev == 'C')		  
		{
		  smsnumber = 2;
// 			smsnumbuf[smsnumbufloc]=*rev;
// 			smsnumbufloc++;
		}
		else
		{
			smsnumber	= 0;			
		}
	}
	else if(smsnumber == 2) 	 //
	{
		if(*rev == 'M')		  
		{
		  smsnumber = 3;
// 			smsnumbuf[smsnumbufloc]=*rev;
// 			smsnumbufloc++;
		}
		else
		{
			smsnumber	= 0;			
		}	
  }
	else if(smsnumber == 3) 	 //
	{
		if(*rev == 'T')		  
		{
		  smsnumber = 4;
// 			smsnumbuf[smsnumbufloc]=*rev;
// 			smsnumbufloc++;
		}
		else
		{
			smsnumber	= 0;			
		}	
  }
	else if(smsnumber == 4) 	 //
	{
		if(*rev == 'I')		  
		{
		  smsnumber = 5;
			gprsNoConnect=0;//在无GPRS连接时，收到短信后重置断电时间
// 			smsnumbuf[smsnumbufloc]=*rev;
// 			smsnumbufloc++;
		}
		else
		{
			smsnumber	= 0;			
		}	
  }
	else if(smsnumber == 5) 	 //
	{
		if(*rev ==',')//,	 
		{
		 smsnumber=6;
     smsnumbuf[smsnumbufloc]=*rev;
		 smsnumbufloc++;			
		}			
  }		
	else if(smsnumber ==6) 	 //接收,1
	{
		if(*rev == 0x0d)//回车		 
		{
		 smsnumber	= 0;			
		}
		else
		{
			smsnumbuf[smsnumbufloc]=*rev;
		  smsnumbufloc++;
    }
		
  }		
			
 }
void  read_smsnumb(void)
 { unsigned char i,smsnumbufloctemp;
	 char *p=smsnumbuf;
	 smsnumbufloctemp=smsnumbufloc;
	 if((!(readsmsnumok&BIT(10)))&&(smsnumbufloc)&&(!smsnumber))//已读取 ，有数据，接收停止
	 { 
	  for(i=0;i<10;i++)
	  {
			p =(char *)strchr(p, ',');
	   if(p)
		 {
		  p++;
		  if((*p >= 0x30)&&(*p <= 0x39)) 
			{	
	     smsnum[i][0]=*p;
			 readsmsnumok|=BIT(i);
			}
		  else
			continue;
		  p++;
		  if((*p >= 0x30)&&(*p <= 0x39)) 
			{	
	     smsnum[i][1]=*p; 
			 readsmsnumok|=BIT(i);
			}
		  else
			continue; 
			
	   }
	  else
		{ 
      if((smsnumbufloctemp==smsnumbufloc)&&(!smsnumber))//没有接收新数据
      {				
		  memset((void *)smsnumbuf,0,50); smsnumbufloc=0;	
			readsmsnumok|=BIT(10);//读取完成
			}
		 break;
		}
		 
	  }
  }
 }
 
 
 
 //+CMGR: <stat>[,<alpha>],<length><CR><LF><pdu>  OK进命名回复缓冲区
 void  rec_smsdata(char *rev)
 {
	  unsigned char i;
	  if(!smsdataend)//数据正确处理中
	{	
	 switch(smsdataber)
	 {
		 case 0: if(*rev == '+')	smsdataber = 1; else smsdataber	= 0; break;
     case 1: if(*rev == 'C')	smsdataber = 2; else smsdataber	= 0; break;
     case 2: if(*rev == 'M')	smsdataber = 3; else smsdataber	= 0; break;
     case 3: if(*rev == 'G')	smsdataber = 4; else smsdataber	= 0; break;
     case 4: if(*rev == 'R')  smsdataber = 5; else smsdataber	= 0; break;
     case 5: smsdatarecnum++;
						 if(smsdatarecnum<15)
						 {
							if(*rev == 0X0D)
							{
			         smsdataber = 6;
							smsdatarecnum=0;
							}
             }
						 else
		         smsdataend=1; 	 
		         break;
		 case 6: smsdatarecnum++;
						 if(smsdatarecnum<3)
						 {
							if(*rev == 0X30) 
		          {
			         smsdataber = 7;
		           //smsdatabegain =1;
						   //smsdatarecok =1;
							 smsdatarecnum=0;
						  }
             }
						 else
						smsdataend=1; 
						 break; 
			case 7: if(*rev == 0X38)  smsdataber = 8;else smsdataend=1;  break; 
		  case 8: smsdatarecnum++;if(smsdatarecnum == 16)  {smsdataber = 9;smsdatarecnum=0;} break;       
      case 9: smsdatarecnum++;
			     if(((*rev >= 0x30)&&(*rev <= 0x39))||((*rev >= 0x41)&&(*rev <= 0x46)))//PDU数据
					 {  
						 if(*rev >0x39)
              *rev-=0x37;
             	else
              *rev-=0x30;								
						 SMSrec.PDUtype=(SMSrec.PDUtype<<4)+*rev;
						 if(smsdatarecnum>=2)
						 { 
							 smsdataber = 10;
							 smsdatarecnum=0;
							 if(SMSrec.PDUtype&(BIT(6)))
								 longsms=1;
							   else 
								 longsms=0;
             }
					 }
           else
           {
						smsdataend=1;//处理非正确数据
           }						 
				  break; //长短信 

		  case 10:smsdatarecnum++; 
					    if(((*rev >= 0x30)&&(*rev <= 0x39))||((*rev >= 0x41)&&(*rev <= 0x46)))//PDU数据
					  {  
						 if(*rev >0x39)
              *rev-=0x37;
             	else
              *rev-=0x30;								
						 SMSrec.numlenght=(SMSrec.numlenght<<4)+*rev;
						 if(smsdatarecnum>=2)
						 { 
							 if(SMSrec.numlenght%2) 
                SMSrec.numlenght+=1;//偶数
                smsdataber = 11;
                smsdatarecnum=0;
             }
					 }
           else
           {
						smsdataend=1;//处理非正确数据
           }						 
				  break;  //电话号长度 
					 
					
			case 11:smsdatarecnum++; if(smsdatarecnum>=2){smsdataber = 12;smsdatarecnum=0;}break; //号码类别      		 		 
			case 12:                smsdatarecnum++; //号码
			                        //if(smsdatarecnum<=SMSrec.numlenght)
			                         SMSrec.mobile_number[smsdatarecnum-1]=*rev;
															//else
			                        if(smsdatarecnum>=SMSrec.numlenght)
                              {
																	dxbjhmflag=0;//标志那个号码
																	if(SMSrec.numlenght==14)//判断号码1    8613973100157F 
																	{
																	  for(i=0;i<14;i++)
																		{
																			if( SMSrec.mobile_number[i]==dxbjhm[0][i])
																				dxbjhmflag|=BIT(0);//短信报警号码标志
																			else
																			{
																				dxbjhmflag&=~BIT(0);break; 	
                                      }
                                    }
																	}
																	 if(SMSrec.numlenght==(dxbjhm1_lenght+dxbjhm1_lenght%2))//判断号码2
																	{
																	  for(i=0;i<SMSrec.numlenght;i++)
																		{
																			if( SMSrec.mobile_number[i]==dxbjhm[1][i])
																				dxbjhmflag|=BIT(1);
																			else
																			{
																				dxbjhmflag&=~BIT(1);break; 	
                                      }
                                    }
																	}
																	
																	if(dxbjhmflag)
																	{
																		smsdataber = 13;
																		smsdatarecnum=0;
                                  }
																	else
																	smsdataend=1;//数据错误等待结束
																 }
																break; 			 
				case 13:smsdatarecnum++; if(smsdatarecnum>=18){smsdataber = 14;smsdatarecnum=0;}break; //协议标识  编码 	时间   
			  case 14:smsdatarecnum++;
				       if(((*rev >= 0x30)&&(*rev <= 0x39))||((*rev >= 0x41)&&(*rev <= 0x46)))//PDU数据
					    {  
						    if(*rev >0x39)
                *rev-=0x37;
             	  else
                *rev-=0x30;								
						    SMSrec.UDL=(SMSrec.UDL<<4)+*rev;
						   if(smsdatarecnum>=2)
						  { 
							 if(longsms) smsdataber = 15;
               else
								smsdataber = 16;
							  smsdatarecnum=0;
              }
					   }
             else
             {
						  smsdataend=1;//处理非正确数据
             }						 
				     break; //数长度             
				//case 15:smsdatarecnum++; if(smsdatarecnum<=6){SMSrec.UDHI[smsdatarecnum-1]=*rev; }else{smsdataber = 16;smsdatarecnum=0;}break;//有协议头
    	  case 15:smsdatarecnum++; if(smsdatarecnum>=12){smsdataber = 16;smsdatarecnum=0;}break;//有协议头
				case 16:smsdatarecnum++;
					      if(longsms)
				        {
// 									smsdatabuf[SMSrec.UDHI[5]][smsdatarecnum++]=*rev;
// 									smsdatabufloc++;
									smsdataend=1;//接收无长短信
                }
							else
							{
								if(((*rev >= 0x30)&&(*rev <= 0x39))||((*rev >= 0x41)&&(*rev <= 0x46)))//PDU数据
								{  
									if(*rev >0x39)
									*rev-=0x37;
									else
									*rev-=0x30;								
									revtemp[0]=(revtemp[0]<<4)+*rev;
									if(!(smsdatarecnum%2))
										 Gprs_rev(revtemp,1);
									if(smsdatarecnum>=(SMSrec.UDL*2))
									{
									 smsdataber=0;
									 smsdatarecnum=0;
									 //issms=1;//短信数据处理完成	 判断是否成功接收
                  }				  
							  }
							  else
							 {
								smsdataend=1;//处理非正确数据
							 }	
							}break; 
					default: break;			
     }
   }
	else
	 {
		 if(((*rev >= 0x30)&&(*rev <= 0x39))||((*rev >= 0x41)&&(*rev <= 0x46)))//PDU数据
			{}
			 else//接收错误
			{
				smsdatarecnum=0;
				smsdataber = 0;
// 		    smsdatabegain =0;
// 				smsdatarecok =0;
				smsdataend=0;
     }	 
   }
			
 }
 
void Gprs_rev(char * rev,unsigned char selecissms)
{
  	unsigned char i;


	if(GprsRevTurn == 0)	 //Z
	{
		if(*rev == 'Z')		  
		{  
			 GprsNLCount = 0;
		    GprsRevTurn = 1;
			ZDCommand.start[0]= *rev;
			
		}
		else
		{
			GprsRevFlag	= 0;
		    GprsNLCount = 0;
// 			//非服务器下发命令
// 			if(*rev>0x1f)
// 			{
// 			strbuf[bufloc]=*rev ;					
// 			bufloc++;
// 			if (bufloc >=50 ) 
// 		  bufloc = 0;
// 		  }
				
		}
	    
	}
	else if(GprsRevTurn == 1) 	 //L
	{
		GprsRevTurntime=0;
		if(*rev == 'L')
		{
		   GprsRevTurn = 2;
		   ZDCommand.start[1]= *rev;
		   GprsRevCheck = 0;
			recgprscom=1;//收到GPRS命令，休眠判断用
		}
		else
		{
			GprsRevFlag	= 0;
		    GprsNLCount = 0;
			GprsRevTurn = 0;
		}
	}
	else if(GprsRevTurn == 2)  //命令码
	{
		GprsRevTurntime=0;
		ZDCommand.command = *rev;

		GprsRevCheck += *rev;

		GprsRevTurn = 3;
		GprsNLCount = 0;		
	}
	else if(GprsRevTurn == 3) //流水号
	{
		GprsRevTurntime=0;
		ZDCommand.Liushui[GprsNLCount++] = *rev;

		GprsRevCheck += *rev;

		if(GprsNLCount >= 4)
		{
			GprsRevTurn = 4;
			GprsNLCount = 0;
		}
		
	}	
	else if(GprsRevTurn == 4) //终端 ID 号
	{
		GprsRevTurntime=0;
		ZDCommand.ZID[GprsNLCount++] = *rev;
		
		GprsRevCheck += *rev;

		if(GprsNLCount >= 8)
		{
			GprsRevTurn = 5;
			GprsNLCount = 0;

			for(i=0; i<7; i++)
			{
			   if(ZDParameter.ZDID[i] != ZDCommand.ZID[i])
			   {
					 GprsRevTurn = 0;
			         GprsNLCount = 0;
					 break;
			   }
			}
			if((ZDParameter.ZDID[7]&0xf0) != (ZDCommand.ZID[7]&0xf0))
			{
				GprsRevTurn = 0;
			    GprsNLCount = 0;
			}	
		}
	}		
	else if(GprsRevTurn == 5) //数据长度
	{
		GprsRevTurntime=0;
		ZDCommand.length[GprsNLCount++] = *rev;
		
		GprsRevCheck += *rev;

		if(GprsNLCount >= 2)
		{
			GprsRevTurn = 6;
			GprsNLCount = 0;
		}
		
	}			
	else if(GprsRevTurn == 6) //数据
	{
		GprsRevTurntime=0;
		ZDCommand.data[GprsNLCount++] = *rev;

		GprsRevCheck += *rev;
		
		if(GprsNLCount >= (ZDCommand.length[0] * 256 + ZDCommand.length[1]))
		{
			GprsRevTurn = 7;
			GprsNLCount = 0;
		}
		
	}		
	else if(GprsRevTurn == 7) //校验码
	{
		GprsRevTurntime=0;
		ZDCommand.check = *rev;
		
//		GprsRevCheck = ~GprsRevCheck+1;
//		
//		if(NLCommand.check != GprsRevCheck)
//		{
//		   	 GprsRevTurn = 0;
//		}			
//		else
//		{
			 GprsRevTurn = 8;
//		}
						
	}			
	else if(GprsRevTurn == 8) //结束符校验码
	{ 
		GprsRevTurntime=0;
		ZDCommand.finish = *rev;
				
		GprsRevTurn = 0;
		if(ZDCommand.finish == 0x0d)
		{
			GprsRevOver = 1;
			issms=selecissms;//是否为短信接收
		}
		else
		{
		  GprsRevTurn = 0;
		}				
	}		
		
}
void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 module */
    SYS_ResetModule(UART0_RST);
    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 115200);

	  /* Set UART Rx and RTS trigger level */
	// UART0->FCR |= (2ul<<UART_FCR_RFITL_Pos);
	 //Set Rx timeout count
	// UART_SetTimeoutCnt(UART0, 20);
	 //enable Rx timeout counter
	// UART0->IER |= 	UART_IER_TIME_OUT_EN_Msk;

	 /* Enable Interrupt  */
    //UART_EnableInt(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_TOUT_IEN_Msk));
	  //UART_EnableInt(UART0, UART_IER_RDA_IEN_Msk|UART_IER_WKDATIEN_Msk );//休眠时有唤醒功能
	  UART_EnableInt(UART0, UART_IER_RDA_IEN_Msk );
		UART_DisableInt(UART0, UART_IER_THRE_IEN_Msk );
	_sys_uUartTxHead = _sys_uUartTxTail = NULL;
	 //memset((void *)strbuf,0,200);
}

void UART02_IRQHandler(void)
{
   uint32_t u32IntSts = UART0->ISR;
   uint32_t reg_temp	=UART0->IER ;
   uint8_t  temp[1],i;
	 
    if(u32IntSts & UART_ISR_RDA_INT_Msk)
    {       
            
      temp[0]= UART_READ(UART0);
			//GprsRevTurn == 0		smsnumber == 0 smsdataber
			//if((smsnumber==0)&&(smsdataber==0)&&(cregdataber==0))
      if((smsnumber<4)&&(smsdataber<4)&&(cregdataber<3))				
			Gprs_rev((char*)temp,0);//服务器命令
      if((GprsRevTurn==0)&&(smsdataber<4)&&(cregdataber<3))					
		  rec_smsnumb((char*)temp);//短信号
			if(((GprsRevTurn==0)&&(smsnumber<4)&&(cregdataber<3))||(smsdataber>=4))	
			//if((GprsRevTurn==0)&&(smsnumber<4)&&(cregdataber<3))		
      rec_smsdata((char*)temp);//短信数据
			if((GprsRevTurn==0)&&(smsnumber<4)&&(smsdataber<4))			
			rec_cregdata((char*)temp); //基站数据
      if((GprsRevTurn==0)&&(smsnumber<4)&&(smsdataber<4)&&(cregdataber<3))			
      rec_comrep((char*)temp);//命令回应
     //if((smsnumber>=4)||(smsdataber>=4))	memset((void *)strbuf,0,50); bufloc=0;	
    } 
   
	 if(strstr((char *)strbuf,"0, CLOSED" )!= NULL)
   {
   	 Gprs_Timeout_Count = 100;	 GprsConnectTcpState=0;
		 ZDParameter.LoginReply=0;//发送失败后重新登录
		 state=0;
	   memset((void *)strbuf,0,50); bufloc=0;
	   at_command_order = 10;	
		 AtCommSendingState = COMMSENDED;	
		 DataSended = 0;
		 CommSended = 0; 
		 
   }
	 if(strstr((char *)strbuf,"NOT READY" )!= NULL)  
   {
   	 Gprs_Timeout_Count = 100;	gprsNoConnect = 50*60; GprsConnectTcpState=0;
		 ZDParameter.LoginReply=0;//发送失败后重新登录
		 state=0;
	   memset((void *)strbuf,0,50); bufloc=0;
	   at_command_order = 0;	
		 AtCommSendingState = COMMSENDED;	
		 DataSended = 0;
		 CommSended = 0; 
		 
   }
	 
	 
	 
   if( reg_temp & 	UART_IER_THRE_IEN_Msk) 
   {		
	   if(u32IntSts & UART_ISR_THRE_INT_Msk)
	   {	  		 	
		   	if (_sys_uUartTxHead == _sys_uUartTxTail) 
			{									 
				
				UART_DisableInt(UART0,  UART_IER_THRE_IEN_Msk );
			} 
			else
			{
				
				for (i=0; i<8; i++)
				{ 		
				  UART0->THR =  _sys_ucUartTxBuf[_sys_uUartTxHead];			
				  _sys_uUartTxHead = sysTxBufReadNextOne();
				  if (_sys_uUartTxHead == _sys_uUartTxTail) // buffer empty
						break;
				}
			}
			  
	   } 
    }
		
// 	 if(u32IntSts & UART_ISR_DATWKIF_Msk )//深度休眠
// 	 {
//    }
// 		
		
}
void UART1_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 module */
    SYS_ResetModule(UART1_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART1, 115200);

	UART_EnableInt(UART1, UART_IER_RDA_IEN_Msk);
}

void UART1_IRQHandler(void)
{
    uint32_t u32IntSts;
	uint8_t  temp[1];
	u32IntSts = UART1->ISR;
    if(u32IntSts & UART_ISR_RDA_INT_Msk)
    {       
      temp[0] = UART_READ(UART1);      	
			Gps_rev((char*)temp);  
      Gpgga_rev((char*)temp);     	
    } 
}




//发送字符串
static void _PutChar_f(uint8_t ucCh)
{ 
	while(sysTxBufWriteNextOne() == _sys_uUartTxHead) ;	// buffer full		
	_sys_ucUartTxBuf[_sys_uUartTxTail] = ucCh;
	_sys_uUartTxTail = sysTxBufWriteNextOne();  
	if( (UART0->IER & UART_IER_THRE_IEN_Msk)== 0)
	UART_EnableInt(UART0,UART_IER_THRE_IEN_Msk );
}
//调用"发送字符串函数"
static void sysPrintf(char *pcStr)
{
  while (*pcStr)
	{           
	 	_PutChar_f(*pcStr++);
	}
}
//调用...
void  GprsSendComm(char *comm) 
{
    sysPrintf(comm); 
}

// void UART_send( uint8_t *pu8TxBuf, uint16_t WriteBytes)
// {
//     uint16_t  u16Count;

//     for(u16Count = 0; u16Count<WriteBytes; u16Count++)
//      { 
// 			 // datatest1= *pu8TxBuf;
// 			 // datatest2= *pu8TxBuf++;
//        // _PutChar_f(*pu8TxBuf++);
// 		   //GprsSndCheck += (*pu8TxBuf++);
// 			 datachar= *pu8TxBuf++;
// 			_PutChar_f(datachar);
// 			GprsSndCheck +=datachar;
//     }
// }


 void UART_send( uint8_t *pu8TxBuf, uint16_t WriteBytes)
{
    uint16_t  u16Count;
	  uint8_t temp;
    if(pdutoASC)//十六进制转换为ASC
		{
     for(u16Count = 0; u16Count<WriteBytes; u16Count++)
     { 
			 datachar= *pu8TxBuf++;
			 GprsSndCheck +=datachar;
			 temp=datachar>>4;
 			//if((temp>=0)&&(temp<10))
			  if(temp<10)
				 temp+=0x30;
			 else if((temp>=10)&&(temp<=15))
				 temp+=0x37;
			 else
				 temp=0x30;
			_PutChar_f(temp);
			temp=datachar&0x0f;
			 //if((temp>=0)&&(temp<10))
			  if(temp<10)
				 temp+=0x30;
			 else if((temp>=10)&&(temp<=15))
				 temp+=0x37;
			 else
				 temp=0x30;
			_PutChar_f(temp); 
	
     }
	  }
		else
		{
      for(u16Count = 0; u16Count<WriteBytes; u16Count++)
     { 
			 datachar= *pu8TxBuf++;
			_PutChar_f(datachar);
	    GprsSndCheck +=datachar;
     }
    }	
}




void UART_sendhearder( uint8_t *pu8TxBuf, uint16_t WriteBytes)
{
    uint16_t  u16Count;
	  uint8_t temp;
    if(pdutoASC)//十六进制转换为ASC
		{
     for(u16Count = 0; u16Count<WriteBytes; u16Count++)
     { 
			 datachar= *pu8TxBuf++;
			 temp=datachar>>4;
 			//if((temp>=0)&&(temp<10))
			  if(temp<10)
				 temp+=0x30;
			 else if((temp>=10)&&(temp<=15))
				 temp+=0x37;
			 else
				 temp=0x30;
			_PutChar_f(temp);
			temp=datachar&0x0f;
			 //if((temp>=0)&&(temp<10))
			  if(temp<10)
				 temp+=0x30;
			 else if((temp>=10)&&(temp<=15))
				 temp+=0x37;
			 else
				 temp=0x30;
			_PutChar_f(temp); 
	
     }
	  }
		else
		{
      for(u16Count = 0; u16Count<WriteBytes; u16Count++)
     { 
			 datachar= *pu8TxBuf++;
			_PutChar_f(datachar);
	
     }
    }	
}








unsigned char GprsConnectTcp(void)
{  char *p=0;  
   uint8_t i;
   if((at_command_order+1) < sizeof(at_command))//数组命令;判断整个命令数组没有发送完毕
   { 	
      if(AtCommSendingState == COMMSENDED)
      {	  
           
         GprsSendComm((char *)at_command[at_command_order]);//发送-当前-命令
	
         AtCommSendingState = COMMSENDING;//更新 发送命令状态	1
				
         Gprs_Timeout_Count = 0;	
				 //return 1;			//正在连接
      } 
			//else
		 {	
			if(at_command_order == 14)
			{
				 if(strstr((char *)strbuf,at_command_reply[at_command_order]) == NULL )
	     {	  
					if(Gprs_Timeout_Count >= 200)    
					{			
						 Gprs_Timeout_Count = 0;			     			   
						
						// at_command_order = 0;//命令序号
             at_command_order = 10;						
						 AtCommSendingState = COMMSENDED;	//更新 发送命令状态
						memset((void *)strbuf,0,50); bufloc=0; 
						 return 0;		 //超时
					}
					return 1;			//正在连接
        }
				else						//如果查找到命令状态
				{
					 memset((void *)strbuf,0,50); bufloc=0;
					 AtCommSendingState = COMMSENDED;	// 更新 发送命令状态 0
					 at_command_order++;
					 if(at_command_order >= 17)//发送TCP数据
					 { 
						 AtCommSendingState = COMMSENDED;
						 gprsNoConnect = 0;
						 return 2;			  //已经连接
					 }
					 else
					 {
							return 1;		   	 //正在连接
					 }
				 }
				
      }	
				else if(at_command_order == 15)
			{
				if((strlen(strbuf)==0)||(Gprs_Timeout_Count < 20))  
				//if(strstr((char *)strbuf,at_command_reply[at_command_order]) == NULL )	
				{ 
					if(Gprs_Timeout_Count >= 100)    
					{			
						 Gprs_Timeout_Count = 0;			     			   
						 //at_command_order = 0;//命令序号	
						 at_command_order = 10;		
						 AtCommSendingState = COMMSENDED;	//更新 发送命令状态
						memset((void *)strbuf,0,50); bufloc=0;
						 return 0;		 //超时
					}
					return 1;			//正在连接
					
        }
				else						//如果查找到命令状态
				{
					memset((void *)strbuf,0,50); bufloc=0;
					 AtCommSendingState = COMMSENDED;	// 更新 发送命令状态 0
					 at_command_order++;
					 if(at_command_order >= 17)//发送TCP数据
					 { 
						 AtCommSendingState = COMMSENDED;
						 gprsNoConnect = 0;
						 return 2;			  //已经连接
					 }
					 else
					 {
							return 1;		   	 //正在连接
					 }
				 }
			}
			else if(at_command_order == 16)
			{
				 if(strstr((char *)strbuf,at_command_reply[at_command_order]) == NULL )//如果比较发送数组没有查找到当前命令状态
	     {	  
					if(Gprs_Timeout_Count >= 200)    
					{			
						 Gprs_Timeout_Count = 0;			     			   
						
						 //at_command_order = 0;//命令序号	
						 at_command_order = 10;		
						 AtCommSendingState = COMMSENDED;	//更新 发送命令状态
						 memset((void *)strbuf,0,50); bufloc=0;
						 return 0;		 //超时
					}
					return 1;			//正在连接
        }
				else						//如果查找到命令状态
				{
					memset((void *)strbuf,0,50); bufloc=0;
					 AtCommSendingState = COMMSENDED;	// 更新 发送命令状态 0
					 at_command_order++;
					 if(at_command_order >= 17)//发送TCP数据
					 { 
						 AtCommSendingState = COMMSENDED;
						 gprsNoConnect = 0;
						 return 2;			  //已经连接
					 }
					 else
					 {
							return 1;		   	 //正在连接
					 }
				 }
				
      }	
     else
      { 
	     if(strstr((char *)strbuf,at_command_reply[at_command_order]) == NULL )//如果比较发送数组没有查找到当前命令状态
	     {	  
					if(Gprs_Timeout_Count >= 50)//5s      
					{	
					 if(at_command_order==2)//查询SIM卡准备就绪
					 {
						 Parameterset.Parametersetitem.GSMxhqd0x12=0;
						 simreadyerror++;
						 if((simreadyerror==4)&&(!(memparamset.memparam.flagchange&(BIT(2)))))//无卡超过4次，并且上次卡正常
						 {
							 memparamset.memparam.baSIMnum++;//拔SIM卡次数
							 memparamset.memparam.flagchange|=BIT(2);//本次次数更新
							 numdispflag|=BIT(1);//sim卡损坏显示
						 }
						 if(simreadyerror>4)
						 {
						   simreadyerror=5;
						 }
					 }
					 else if(at_command_order==9)//GPRS登录状态
					 {
						 CGATTerrorcount++;
						 if(CGATTerrorcount>2)
						 {
							 CGATTerrorcount=3;
						  numdispflag|=BIT(4); //4登录不了GPRS 显示
						 }
					 }
					   Gprs_Timeout_Count = 0;			     			   
						 //at_command_order = 0;//命令序号	
					 if(at_command_order<10)
					 at_command_order = 0;
					 else
					 at_command_order = 10;
					 
						 AtCommSendingState = COMMSENDED;	//更新 发送命令状态
					 memset((void *)strbuf,0,50); bufloc=0;
						 return 0;		 //超时
					}
					return 1;			//正在连接
        }
				else						//如果查找到命令状态
				{
					
					
					 if(at_command_order==4)//
					 {
						 //到此说明2号命令执行成功
						 simreadyerror=0;
						 numdispflag&=~BIT(1);//取消sim卡损坏显示
						 memparamset.memparam.flagchange&=~BIT(2);//本次次数更新清除
					  if(readIMSI(strbuf))//若成功获取IMSI
						{
						 if(Parameterset.Parametersetitem.IMSI0x11[0]==0xff)//未初始化
						 {
							for(i=0;i<20;i++)
							Parameterset.Parametersetitem.IMSI0x11[i]=IMSIdata[i];	
							memchange=1;//0x11参数项有改动
             }
						 else
						 {
							 for(i=0;i<20;i++)
							 {
							  if(Parameterset.Parametersetitem.IMSI0x11[i]!=IMSIdata[i])
								{
									memparamset.memparam.simkaghcs++;//SIM卡更换次数加1
									for(i=0;i<20;i++)//重新赋值
							    Parameterset.Parametersetitem.IMSI0x11[i]=IMSIdata[i];
									memchange=1;//0x11参数项有改动
                  break;									
                }
							 }
             }
						 
					  }
           }
					
					 else if(at_command_order==5)//
					 {
					  if((p=strstr((char *)strbuf,"+CSQ:"))!= NULL)
						Parameterset.Parametersetitem.GSMxhqd0x12=StrToInt(p+6);
           }
// 					  else if(at_command_order==7)//
// 					 {
// 					  if((p=strstr((char *)strbuf,"+CREG:"))!= NULL)
// 						{
// 							if(readCREG(p)>1)//基站信息获取成功
// 							{
// 								for(i=0;i<20;i++)//重新赋值
// 								TerminalReply0x110x18.JZID[i]=JZ_ID[i];
// 								switch(CREGstat)
// 								{
// 									case 0: numdispflag|=BIT(7);break;// 7无移动网络信号（没有注册到网络也没检测到信号）
// 									case 1: numdispflag&=~0x0085;break;         //0x0085 注册到本地网络
// 									case 2: numdispflag|=BIT(2);break;// 2 终端无法注册移动网络（检测到网络但没有注册到网络）
// 									case 3: numdispflag|=BIT(0);break;// 0注册被拒绝
// 									case 4: numdispflag&=~0x0085;break;//未知状态
// 									case 5:numdispflag&=~0x0085;break;         //0x0085 注册到网络 有漫游
// 									default:break;
//                 }
//               }
// 						}
//            }
					 else if(at_command_order==9)//GPRS登录状态
					 {
						  numdispflag&=~BIT(4); //4登录不了GPRS 不显示
						  CGATTerrorcount=0;
					 }
					 else if(at_command_order==11)//查询多连接
					 {
					  if(strstr((char *)strbuf,"+CIPMUX: 1") != NULL )
						at_command_order++;//已为多连接则不再重设多连接	
           }
						 
					memset((void *)strbuf,0,50); bufloc=0;
					 AtCommSendingState = COMMSENDED;	// 更新 发送命令状态 0
					 
					 at_command_order++;
					#ifdef ATE1
					if(at_command_order==1)
						 at_command_order++;
					#endif
					
					
					
					
					 if(at_command_order >= 17)//发送TCP数据
					 { 
						 AtCommSendingState = COMMSENDED;
						 gprsNoConnect = 0;
						 return 2;			  //已经连接
					 }
					 else
					 {
							return 1;		   	 //正在连接
					 }
				 }
	     }
		 }
   }
	 else
	 {
			memset((void *)strbuf,0,50); bufloc=0;
			at_command_order = 0;	
			return 0;	
	 }
}



unsigned short int GprsLogindata(void)
{
	
	 uint16_t i;
	 //if(ZDParameter.LoginReply == 0)
	// {		 
// 		 if(((ZDParameter.LoginCount < 3) && (ZDParameter.LoginTimer > 30)) || (ZDParameter.LoginCount == 0))
// 		 {
// 			ZDParameter.LoginCount++;
// 			ZDParameter.LoginTimer = 0;

			ZDReply.start[0] = 'Z';
			ZDReply.start[1] = 'L';
			ZDReply.command = 0x19;	 
// 			for(i=0; i<4; i++)
// 			{
// 				ZDReply.Liushui[i] = ZDCommand.Liushui[i];
// 			}
			 ZDParameter.zdliushuinum++;
			 if(ZDParameter.zdliushuinum>10000)//主动流水从1到10000
				ZDParameter.zdliushuinum=1;
			 ZDReply.Liushui[0]=0;
			 ZDReply.Liushui[1]=0;
			 ZDReply.Liushui[2]=ZDParameter.zdliushuinum>>8;
			 ZDReply.Liushui[3]=ZDParameter.zdliushuinum&0xff;

			
			for(i=0; i<8; i++)
			{
			 ZDReply.ZID[i] = ZDParameter.ZDID[i];
			}
     
			for(i=0; i<8; i++)
			{
				//LoginCen.sbyhm[i] = 0x33;
				LoginCen.sbyhm[i] = Parameterset.Parametersetitem.sbyhm0x00[i];
			}
			
			//
			LoginCen.GPS_gzts = GPSvoidtimem/60/24;//分钟转换为天数
			LoginCen.xhr[0] = 0;
			LoginCen.xhr[1] = 0;
			//自动回报位置信息时长
			LoginCen.hbsc[0] = Parameterset.Parametersetitem.hbsc0x03[0];
			LoginCen.hbsc[1] = Parameterset.Parametersetitem.hbsc0x03[1];//30s
			
// 			LoginCen.bjhm = "106575531157170";			
// 			LoginCen.sbbh = "12345678a";
// 			LoginCen.SIMh = "12345678b";
// 			LoginCen.sblx = "12345678c";
      
			LoginCen.bjhm = (char*)Parameterset.Parametersetitem.dxbjhm0x09;			
			LoginCen.sbbh = (char*)Parameterset.Parametersetitem.sbbh0x17;
			LoginCen.SIMh = (char*)Parameterset.Parametersetitem.SIMkh0x18;
			LoginCen.sblx = (char*)Parameterset.Parametersetitem.sblx0x22;
		
			i = 8+1+2+2+strlen(LoginCen.bjhm)+strlen(LoginCen.sbbh)+strlen(LoginCen.SIMh)+strlen(LoginCen.sblx)+8;
			ZDReply.length[0] = i >> 8;
			ZDReply.length[1] = i;

	
      //return i+19+8;//+引号
			return i+19;//+引号
//     }
   //  }
}

      void GprsLoginsend(void)
     {
		     uint8_t bb=0x22;

	       UART_send((uint8_t *)(&ZDReply.start[0]),2);
	        GprsSndCheck = 0;
	        UART_send((uint8_t *)(&ZDReply.command),15);	
          UART_send((uint8_t *)(&LoginCen.sbyhm[0]),13);
          UART_send( &bb,1);   //结束符	
	        UART_send((uint8_t *)(LoginCen.bjhm),strlen(LoginCen.bjhm));
	        UART_send(&bb,1);   //结束符
	        UART_send(&bb,1);   //结束符
					UART_send( (uint8_t *)(LoginCen.sbbh),strlen(LoginCen.sbbh));
					UART_send( &bb,1);   //结束符
					UART_send( &bb,1);   //结束符
					UART_send( (uint8_t *)(LoginCen.SIMh),strlen(LoginCen.SIMh));
					UART_send( &bb,1);   //结束符
					UART_send( &bb,1);   //结束符
					UART_send( (uint8_t *)(LoginCen.sblx),strlen(LoginCen.sblx));
					UART_send( &bb,1);   //结束符

					ZDReply.check = ~GprsSndCheck+1;
					UART_send((uint8_t *)(&ZDReply.check),1);
					//GprsSendComm("\r");  //结束符	  
					UART_send((uint8_t *)("\r"),1);	
					
 }



void sendcontent(unsigned char sernum)
{
	switch(sernum)
	{
		case 0: GprsLoginsend();break;
		case 1: Gprs2Bdatasend();break;
	  case 2: GprsCANdatasend();break;
		case 3: Gprs1811datasend();break;
		case 4: Gprs28datasend();break;
		case 5: Gprs12datasend();break;
		case 6: Gprs17datasend();break;
		default://漏洞处理
			    break;
		
		
  }
	
}
//发送数据长度-:Asc(<字符串表达式>)-求字符串表达式的第一个字符的ASCII 编码
uint8_t* FrameLengthToASC(uint16_t len)
{
   if((len>0)&&(len<10))
	 {
		 LengthAsc[0]  = len%10 + 0x30;
     LengthAsc[1]  = 0;
     LengthAsc[2]  = 0;
   }
	 else if((len>=10)&&(len<100))
	 {
		LengthAsc[0]  = len/10%10 + 0x30;
    LengthAsc[1]  = len%10 + 0x30;
    LengthAsc[2]  = 0;
   }
	  else if((len>=100)&&(len<1000))
	 {
		LengthAsc[0]  = len/100%10 + 0x30; 
		LengthAsc[1]  = len/10%10 + 0x30;
    LengthAsc[2]  = len%10 + 0x30;
   }
	 else
	 {
		 LengthAsc[0]  = 0;
     LengthAsc[1]  = 0;
     LengthAsc[2]  = 0;
   }
		 
   return LengthAsc;
}


unsigned char Gprsdatacap(int8_t at_commandnum)
{  char *p=0;  
  // uint8_t i;
   if(AtCommSendingState == COMMSENDED)
    {	 
      GprsSendComm((char *)at_command[at_commandnum]);//发送-当前-命令
      AtCommSendingState = COMMSENDING;//更新 发送命令状态	1
      Gprs_Timeout_Count = 0;	
				 return 1;			//正在连接
    } 	
   else
   { 
	      if(strstr((char *)strbuf,at_command_reply[at_commandnum]) == NULL )
	      {	  
					if(Gprs_Timeout_Count >= 50)//5s      
					{	
						 Gprs_Timeout_Count = 0;			     			   
						 AtCommSendingState = COMMSENDED;	//更新 发送命令状态
					 memset((void *)strbuf,0,50); bufloc=0;
						 return 0;		 //超时
					}
					return 1;			//正在连接
        }
				else						//如果查找到命令状态
				{
					 if(at_commandnum==5)//
					 {
					  if((p=strstr((char *)strbuf,"+CSQ:"))!= NULL)
						Parameterset.Parametersetitem.GSMxhqd0x12=StrToInt(p+6);
           }
// 					  else if(at_commandnum==7)//
// 					 {
// 					  if((p=strstr((char *)strbuf,"+CREG:"))!= NULL)
// 						{
// 							if(readCREG(p)>1)//基站信息获取成功
// 							{
// 								for(i=0;i<20;i++)//重新赋值
// 								TerminalReply0x110x18.JZID[i]=JZ_ID[i];
// 								
//               }
// 						}
//            }
					memset((void *)strbuf,0,50); bufloc=0;
					AtCommSendingState = COMMSENDED;	// 更新 发送命令状态 0	
					 return 2;		
	      }
     }
}

unsigned char SMSdatacap(int8_t at_commandnum)
{  //char *p=0;  
   //uint8_t i;
   if(!AtsmsCommSend)
    {	 
      GprsSendComm((char *)at_SMScom[at_commandnum]);//发送-当前-命令
      AtsmsCommSend = 1;;//更新 发送命令状态	1
      Gprs_Timeout_Count = 0;	
				 return 1;			//正在连接
    } 	
   else
   { 
	      if(strstr((char *)strbuf,at_SMScom_rep[at_commandnum]) == NULL )
	      {	  
					if(Gprs_Timeout_Count >= 50)//5s      
					{	
						 Gprs_Timeout_Count = 0;			     			   
						 AtsmsCommSend = 0;	//更新 发送命令状态
					   memset((void *)strbuf,0,50); bufloc=0;
						 return 0;		 //超时
					}
					return 1;			//正在连接
        }
				else						//如果查找到命令状态
				{
					memset((void *)strbuf,0,50); bufloc=0;
					AtsmsCommSend = 0;	// 更新 发送命令状态 0	
					 return 2;		
	      }
     }
}

unsigned char SMSdata_read(void)
{ 
	  unsigned char i=0;
	  if(!AtsmsCommSend)
    {	 
			while(!(readsmsnumok&BIT(i))) 
			{
				i++;
				if(i>=10)readsmsnumok=0;//
      }
      GprsSendComm((char *)at_SMScom[1]);//发送-当前-命令
			GprsSendComm((char *)smsnum[i]);//发送ASCII数据;
			GprsSendComm("\r");
      AtsmsCommSend = 1;//更新 发送命令状态	1
      Gprs_Timeout_Count = 0;	
				 return 1;			//正在连接
    } 	
   else
   { 
	      if(strstr((char *)strbuf,at_SMScom_rep[1]) == NULL )
	      {	  
					if(Gprs_Timeout_Count >= 50)//5s      
					{	
						 Gprs_Timeout_Count = 0;			     			   
						 AtsmsCommSend = 0;	//更新 发送命令状态
					   memset((void *)strbuf,0,50); bufloc=0;
						 return 0;		 //超时
					}
					return 1;			//正在连接
        }
				else						//如果查找到命令状态
				{
					memset((void *)strbuf,0,50); bufloc=0;
					AtsmsCommSend = 0;	
					 return 2;		
	      }
     }
}






//TCP上传数据  //len:上传数据长度，  sernum:上传序号
unsigned char GprsSendData(unsigned short int len,unsigned char sernum)
{
	//只在本函数中有效的静态常量
//     static char CommSended = 0; 
//     static char DataSended = 0;
	
    if( DataSended == 0 )   //数据未发送完
    {
       if(CommSended)  //命令已发送
       {
          if( (strstr((char *)strbuf,at_command_reply[17]) == NULL) ) //有返回命令;
          {           
						 if(Gprs_Timeout_Count >= 50)
						 {
										Gprs_Timeout_Count = 0;
										//memset((void *)strbuf,0,200); bufloc=0;
							      memset((void *)strbuf,0,50); bufloc=0;
										//at_command_order = 0;	
							      at_command_order = 10;	
										AtCommSendingState = COMMSENDED;
										DataSended = 0;
										CommSended = 0; 
							      state=0;
							      ZDParameter.LoginReply=0;//发送失败后重新登录
							      ZDParameter.LoginTimer =270;//马上重新登录
							      ZDParameter.zdliushuinum=0;//流水重置
										return 0;	
							     
						 }  
								 return 3;
          }
          else				//没有返回命令;
          {

// 							GprsSendComm( (char *)(&(datap->Head)) );
// 							GprsSendComm( (char *)(&(datap->SimID)) );
// 						
// 							GprsSendComm( (char *)(&(datap->TotalWalt)) );
// 							GprsSendComm( (char *)(&(datap->TotalTime)) );

// 							GprsSendComm( (char *)(&(datap->CurrentTempture)) );

//               GprsSendComm( (char *)(&(datap->CurrentStateOne)) );
//               GprsSendComm( (char *)(&(datap->CurrentStateTwo)) );
// 						  GprsSendComm( (char *)(&(datap->send_T)) );
// 						  GprsSendComm( (char *)(&(datap->walt_value)) );
// 						  GprsSendComm( (char *)(&(datap->ptotectgz)) );
// 						  GprsSendComm( (char *)(&(datap->ptotectld)) );
							/********************************************/	
              sendcontent(sernum);
						  GprsSendComm("\r");
             DataSended  = 1;    
             Gprs_Timeout_Count = 0;        
             return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_command[17]);//发送上传数据命令;
         GprsSendComm((char *)FrameLengthToASC(len));//发送ASCII数据;
				 GprsSendComm("\r");
         Gprs_Timeout_Count = 0; 
         CommSended = 1;
         return 3; 
      }
   }
   else    //数据发送完
   {
      if( (strstr((char *)strbuf,at_command_reply[18]) == NULL) )  //未收到发送完回码
      {
					if(Gprs_Timeout_Count >= 150)//15s
					{
							Gprs_Timeout_Count = 0;
							//memset((void *)strbuf,0,200); bufloc=0;
						  memset((void *)strbuf,0,50); bufloc=0;
						  at_command_order = 10;	
							AtCommSendingState = COMMSENDED;	
							DataSended = 0;
							CommSended = 0; 
						  state=0;
						  ZDParameter.LoginReply=0;//发送失败后重新登录
						  ZDParameter.zdliushuinum=0;//流水重置
							return 0;		    
					}  
          return 3;	
       }
       else
       {
          //memset((void *)strbuf,0,200); bufloc=0;
				  memset((void *)strbuf,0,50); bufloc=0;
          DataSended = 0;
          CommSended = 0; 
          gprsNoConnect = 0;
          Gprs_Timeout_Count = 0;
          return 4;
       }   
    }
}

      void logincenter(void)
    {
       if((ZDParameter.LoginReply == 0)||(state==1))
	     {
			    if((ZDParameter.LoginCount < 3) && (ZDParameter.LoginTimer >=270)) 	//270s 
		      {
						 //ZDParameter.LoginTimer = 0;	 
				   TimeArrivedlogin=1;
			    }
					 else if((ZDParameter.LoginCount >= 3) && (ZDParameter.LoginTimer >= 270))//相当于3次不成功540s后重新登录
		      {
			      ZDParameter.LoginCount = 0;
			      ZDParameter.LoginTimer = 0;
			      GprsConnectTcpState = 0;
					  at_command_order = 10;	
						 ZDParameter.zdliushuinum=0;//流水重置
// 						fwq_connectfail=1;//服务器连接失败
		      }
	        if((dormantlevel!=0)&&(ZDParameter.LoginCount>=1))	
					{
						 ZDParameter.LoginTimer = 0;	 
						 ZDParameter.LoginCount = 0;
						fwq_connectfail=1;//服务器连接失败
						return;
					}
					else
						fwq_connectfail=0;
			     if((TimeArrivedlogin)&&((state==0)||(state==1)))//
					 {  if(state==0)
						  {
							 datalenthlogin=GprsLogindata();
							 state=1;
						  }	
							 GprsConnectTcpState = GprsSendData(datalenthlogin,0);
							 //if(GprsConnectTcpState == 4)
							 if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))
							 {
								state=0;
								 ZDParameter.LoginCount++;
			          ZDParameter.LoginTimer = 0;	 
                TimeArrivedlogin=0;								 
							 }
					 }
			   }  		 
   }


	 unsigned short int Gprs2Bdata(void)
	    {  
				uint16_t i;
				ZDReply.start[0] = 'Z';
				ZDReply.start[1] = 'L';
				ZDReply.command = 0x1a;	 //2.B
				for(i=0; i<4; i++)
				{
					ZDReply.Liushui[i] = ZDCommand.Liushui[i];
				}
				for(i=0; i<8; i++)
				{
					ZDReply.ZID[i] = ZDCommand.ZID[i];
				}
				Gprs1811data_init();//同步未同步值
				//i=sizeof(TerminalReply0x1a);
				//i=sizeof(TerminalReply0x1a)+strlen((char*)TerminalReply0x110x18.JZID)+2-20;
				i=37+strlen((char*)TerminalReply0x110x18.JZID)+2;
				ZDReply.length[0] = i >> 8;
				ZDReply.length[1] = i;

         return i+19;//
      }
	 
	 
	 void Gprs2Bdatasend(void)
	 {
		 uint8_t bb=0x22;
		 UART_send((uint8_t *)(&ZDReply.start[0]),2);
		 GprsSndCheck = 0;
		 UART_send( (uint8_t *)(&ZDReply.command),15);	
		 //UART_send( (uint8_t *)(&TerminalReply0x1a.command),ZDReply.length[0]*256+ZDReply.length[1]);
		 UART_send( (uint8_t *)(&TerminalReply0x1a),37);
		 UART_send(&bb,1);   //结束符
		 UART_send( (uint8_t *)(&TerminalReply0x110x18.JZID),strlen((char*)TerminalReply0x110x18.JZID));
		 UART_send(&bb,1);   //结束符
		 ZDReply.check = ~GprsSndCheck+1;
		 UART_send( (uint8_t *)(&ZDReply.check),1);
		 //GprsSendComm("\r");  //结束符	
		 UART_send((uint8_t *)("\r"),1);	
				
   }
	 
	 unsigned short int GprsCANdata(void)
	    {  
				uint16_t i;
				ZDReply.start[0] = 'Z';
				ZDReply.start[1] = 'L';
				ZDReply.command = 0x13;	 
// 				for(i=0; i<4; i++)
// 				{
// 					ZDReply.Liushui[i] = ZDCommand.Liushui[i];
// 				}
				ZDParameter.zdliushuinum++;
			 if(ZDParameter.zdliushuinum>10000)//主动流水从1到10000
				ZDParameter.zdliushuinum=1;
			 ZDReply.Liushui[0]=0;
			 ZDReply.Liushui[1]=0;
			 ZDReply.Liushui[2]=ZDParameter.zdliushuinum>>8;
			 ZDReply.Liushui[3]=ZDParameter.zdliushuinum&0xff;
			 
				for(i=0; i<8; i++)
				{
					ZDReply.ZID[i] = ZDCommand.ZID[i];
				}
				 CAN_data();
				 //0480,0580,0680,//单个故障 多故障通告 多故障包
				//0080,0180,0280,0380,0780,0880  ,0980,0A80,0B80,2180,2280,2080,2181,2281,2081,0C80,0B80         
// 				i=12*6+receivedanbao*12+receiveduobao*(spnnum+1)*12;
					i=12*(Receiveid_0080t+Receiveid_0180t+Receiveid_0280t+Receiveid_0380t+Receiveid_0780t+Receiveid_0880t
				       +Receiveid_0980t+Receiveid_0A80t+Receiveid_0B80t+Receiveid_2180t+Receiveid_2280t+Receiveid_2080t
				       +Receiveid_2181t+Receiveid_2281t+Receiveid_2081t+Receiveid_0C80t+Receiveid_0D80t)+receivedanbaot*12+receiveduobao*(spnnum+1)*12;
				ZDReply.length[0] = i >> 8;
				ZDReply.length[1] = i;

         return i+19;//
      }
	 
	 
	 void GprsCANdatasend(void)
	 {
		 unsigned char i=0;
		 UART_send((uint8_t *)(&ZDReply.start[0]),2);
		 GprsSndCheck = 0;
		 UART_send( (uint8_t *)(&ZDReply.command),15);
      if(Receiveid_0080t)
		 {		 
		  UART_send( (uint8_t *)(&CANID0080.canid[0]),12);
			Receiveid_0080t=0; 
		 }
		 if(Receiveid_0180t)
		 {
		  UART_send( (uint8_t *)(&CANID0180.canid[0]),12);
			Receiveid_0180t=0;
		 }
		 if(Receiveid_0280t)
		 {
		 UART_send( (uint8_t *)(&CANID0280.canid[0]),12);
			Receiveid_0280t=0;
		 } 
		 if(Receiveid_0380t)
		 {
		 UART_send( (uint8_t *)(&CANID0380.canid[0]),12);
			Receiveid_0380t=0;
		 } 
     if(Receiveid_0780t)
		 {			 
		 UART_send( (uint8_t *)(&CANID0780.canid[0]),12);
			Receiveid_0780t=0;
		 } 
		 if(Receiveid_0880t)
		 { 
		 UART_send( (uint8_t *)(&CANID0880.canid[0]),12);
		 Receiveid_0880t=0;
		 } 
		  if(Receiveid_0980t)
		 { 
		 UART_send( (uint8_t *)(&CANID0980.canid[0]),12);
		 Receiveid_0980t=0;
		 } 
		  if(Receiveid_0A80t)
		 { 
		 UART_send( (uint8_t *)(&CANID0A80.canid[0]),12);
		 Receiveid_0A80t=0;
		 } 
		  if(Receiveid_0B80t)
		 { 
		 UART_send( (uint8_t *)(&CANID0B80.canid[0]),12);
		 Receiveid_0B80t=0;
		 } 
		 if(Receiveid_2180t)
		 { 
		 UART_send( (uint8_t *)(&CANID2180.canid[0]),12);
		 Receiveid_2180t=0;
		 } 
		  if(Receiveid_2280t)
		 { 
		 UART_send( (uint8_t *)(&CANID2280.canid[0]),12);
		 Receiveid_2280t=0;
		 } 
		  if(Receiveid_2080t)
		 { 
		 UART_send( (uint8_t *)(&CANID2080.canid[0]),12);
		 Receiveid_2080t=0;
		 }
     if(Receiveid_2181t)
		 { 
		 UART_send( (uint8_t *)(&CANID2181.canid[0]),12);
		 Receiveid_2181t=0;
		 } 		 
		  if(Receiveid_2281t)
		 { 
		 UART_send( (uint8_t *)(&CANID2281.canid[0]),12);
		 Receiveid_2281t=0;
		 } 
		  if(Receiveid_2081t)
		 { 
		 UART_send( (uint8_t *)(&CANID2081.canid[0]),12);
		 Receiveid_2081t=0;
		 } 
		  if(Receiveid_0C80t)
		 { 
		 UART_send( (uint8_t *)(&CANID0C80.canid[0]),12);
		 Receiveid_0C80t=0;
		 } 
		  if(Receiveid_0D80t)
		 { 
		 UART_send( (uint8_t *)(&CANID0D80.canid[0]),12);
		 Receiveid_0D80t=0;
		 } 
		  
		 
     if(receivedanbaot)
		 {
	 	   UART_send( (uint8_t *)(&CANID0480.canid[0]),12); 
			 receivedanbaot=0;
		 }
		 if(receiveduobao*spnnum)
		 {
			UART_send( (uint8_t *)(&CANID0580.canid[0]),12); 
			while(spnnum)
			{
				CANID0680.duoguzhangbao[0]=GZ_PACK[i][0];
				CANID0680.duoguzhangbao[1]=GZ_PACK[i][1];
				CANID0680.duoguzhangbao[2]=GZ_PACK[i][2];		
			  UART_send( (uint8_t *)(&CANID0680.canid[0]),12);
				i++;
			 spnnum--;
			}
      receiveduobao=0;			
     }
		 
		 ZDReply.check = ~GprsSndCheck+1;
		 UART_send( (uint8_t *)(&ZDReply.check),1);
		 //GprsSendComm("\r");  //结束符	
			UART_send((uint8_t *)("\r"),1);		
   }
	 
	 void Gprs1811data_init(void)
	 {
		 
		//未同步的值
	//gps总里程
	 TerminalReply0x110x18.GPS_sum[0]=gps_tot_lc>>24;
	 TerminalReply0x110x18.GPS_sum[1]=gps_tot_lc>>16;
	 TerminalReply0x110x18.GPS_sum[2]=gps_tot_lc>>8;
	 TerminalReply0x110x18.GPS_sum[3]=gps_tot_lc;
	//ACCon累计时间
	 TerminalReply0x110x18.ACC_ON_sum[0]=ACCon_tot_time>>24;
	 TerminalReply0x110x18.ACC_ON_sum[1]=ACCon_tot_time>>16;
	 TerminalReply0x110x18.ACC_ON_sum[2]=ACCon_tot_time>>8;
	 TerminalReply0x110x18.ACC_ON_sum[3]=ACCon_tot_time;
	 //当次点火后里程处理
	 TerminalReply0x110x18.dh[0]=gps_cur_lc>>8;
	 TerminalReply0x110x18.dh[1]=gps_cur_lc;
	//当次ACCON累计时间
	 TerminalReply0x110x18.ACC_ON_sum_c[0]=ACCon_cur_time>>24;
	 TerminalReply0x110x18.ACC_ON_sum_c[1]=ACCon_cur_time>>16;
	 TerminalReply0x110x18.ACC_ON_sum_c[2]=ACCon_cur_time>>8;
	 TerminalReply0x110x18.ACC_ON_sum_c[3]=ACCon_cur_time; 
		 
		TerminalReply0x1a.TerminalReply0x110x18=TerminalReply0x110x18;
	  TerminalReply0x17.TerminalReply0x110x18=TerminalReply0x110x18;
   }
	 unsigned short int Gprs1811data(unsigned char comm)
	    {  
				uint16_t i;
				ZDReply.start[0] = 'Z';
				ZDReply.start[1] = 'L';
				ZDReply.command = comm;	 //
// 				for(i=0; i<4; i++)
// 				{
// 					ZDReply.Liushui[i] = ZDCommand.Liushui[i];
// 				}
				ZDParameter.zdliushuinum++;
			 if(ZDParameter.zdliushuinum>10000)//主动流水从1到10000
				ZDParameter.zdliushuinum=1;
			 ZDReply.Liushui[0]=0;
			 ZDReply.Liushui[1]=0;
			 ZDReply.Liushui[2]=ZDParameter.zdliushuinum>>8;
			 ZDReply.Liushui[3]=ZDParameter.zdliushuinum&0xff;
			 
				
				
				for(i=0; i<8; i++)
				{
					ZDReply.ZID[i] = ZDCommand.ZID[i];
				}
				Gprs1811data_init();//同步未同步值
				//i=sizeof(TerminalReply0x110x18)+strlen((char*)TerminalReply0x110x18.JZID)+2-20;
				i=35+strlen((char*)TerminalReply0x110x18.JZID)+2;
				ZDReply.length[0] = i >> 8;
				ZDReply.length[1] = i;

         return i+19;//
      }
	 
	 
	 void Gprs1811datasend(void)
	 {
		 uint8_t bb=0x22;
		 UART_send((uint8_t *)(&ZDReply.start[0]),2);
		 GprsSndCheck = 0;
		 UART_send( (uint8_t *)(&ZDReply.command),15);	
		 //UART_send( (uint8_t *)(&TerminalReply0x110x18),ZDReply.length[0]*256+ZDReply.length[1]);
		 UART_send( (uint8_t *)(&TerminalReply0x110x18),35);
		 UART_send(&bb,1);   //结束符
		 UART_send( (uint8_t *)(&TerminalReply0x110x18.JZID),strlen((char*)TerminalReply0x110x18.JZID));
		 UART_send(&bb,1);   //结束符
		 ZDReply.check = ~GprsSndCheck+1;
		 UART_send( (uint8_t *)(&ZDReply.check),1);
		 //GprsSendComm("\r");  //结束符
     UART_send((uint8_t *)("\r"),1);			 
				
   }
	unsigned short int Gprs17data(void)
	    {  
				uint16_t i;
				ZDReply.start[0] = 'Z';
				ZDReply.start[1] = 'L';
				ZDReply.command = 0x17;	 //
// 				for(i=0; i<4; i++)
// 				{
// 					ZDReply.Liushui[i] = ZDCommand.Liushui[i];
// 				}
				ZDParameter.zdliushuinum++;
			 if(ZDParameter.zdliushuinum>10000)//主动流水从1到10000
				ZDParameter.zdliushuinum=1;
			 ZDReply.Liushui[0]=0;
			 ZDReply.Liushui[1]=0;
			 ZDReply.Liushui[2]=ZDParameter.zdliushuinum>>8;
			 ZDReply.Liushui[3]=ZDParameter.zdliushuinum&0xff;
			 
				
				
				for(i=0; i<8; i++)
				{
					ZDReply.ZID[i] = ZDCommand.ZID[i];
				}
				Gprs1811data_init();//同步未同步值
				//i=sizeof(TerminalReply0x110x18)+strlen((char*)TerminalReply0x110x18.JZID)+2-20;
				i=36+strlen((char*)TerminalReply0x110x18.JZID)+2;
				ZDReply.length[0] = i >> 8;
				ZDReply.length[1] = i;

         return i+19;//
      }
	 
	 
	 void Gprs17datasend(void)
	 {
		 uint8_t bb=0x22;
		 UART_send((uint8_t *)(&ZDReply.start[0]),2);
		 GprsSndCheck = 0;
		 UART_send( (uint8_t *)(&ZDReply.command),15);	
		 UART_send( (uint8_t *)(&TerminalReply0x17),36);
		 UART_send(&bb,1);   //结束符
		 UART_send( (uint8_t *)(&TerminalReply0x110x18.JZID),strlen((char*)TerminalReply0x110x18.JZID));
		 UART_send(&bb,1);   //结束符
		 ZDReply.check = ~GprsSndCheck+1;
		 UART_send( (uint8_t *)(&ZDReply.check),1);
		 //GprsSendComm("\r");  //结束符	
		 UART_send((uint8_t *)("\r"),1);	
				
   } 
	 
	 //副参数赋值
void DeputyParameter_init(void)
{
	TerminalReply0x28.zhudianvol[0]=voltage>>8;
	TerminalReply0x28.zhudianvol[1]=voltage;
	if(TerminalReply0x110x18.GPS.state3&BIT(3))//备用电池断电
	TerminalReply0x28.beidianvol=0;	
	else
	TerminalReply0x28.beidianvol=volt_bat;
	TerminalReply0x28.neibuwendu=Temperature;
	TerminalReply0x28.zhudianXMJG=8;//8小时
	TerminalReply0x28.beidianXMJG=Parameterset.Parametersetitem.bdsbzq0x1E;
	TerminalReply0x28.ACC_ON_sum_c[3]=ACCon_tot_time;
	TerminalReply0x28.ACC_ON_sum_c[2]=ACCon_tot_time>>8;
	TerminalReply0x28.ACC_ON_sum_c[1]=ACCon_tot_time>>16;
	TerminalReply0x28.ACC_ON_sum_c[0]=ACCon_tot_time>>24;
	TerminalReply0x28.GPSztdsj[3]=gps_tot_time;
	TerminalReply0x28.GPSztdsj[2]=gps_tot_time>>8;
	TerminalReply0x28.GPSztdsj[1]=gps_tot_time>>16;
	TerminalReply0x28.GPSztdsj[0]=gps_tot_time>>24;
	TerminalReply0x28.kaigainum=memparamset.memparam.kaigainum;
	TerminalReply0x28.baGPSantnum=memparamset.memparam.baGPSantnum;
	TerminalReply0x28.baSIMnum=memparamset.memparam.baSIMnum;
	TerminalReply0x28.GSMxinhao=Parameterset.Parametersetitem.GSMxhqd0x12;
}
	 unsigned short int Gprs28data(void)
	    {  
				uint16_t i;
				ZDReply.start[0] = 'Z';
				ZDReply.start[1] = 'L';
				ZDReply.command = 0x28;	 //
// 				for(i=0; i<4; i++)
// 				{
// 					ZDReply.Liushui[i] = ZDCommand.Liushui[i];
// 				}
				ZDParameter.zdliushuinum++;
			 if(ZDParameter.zdliushuinum>10000)//主动流水从1到10000
				ZDParameter.zdliushuinum=1;
			 ZDReply.Liushui[0]=0;
			 ZDReply.Liushui[1]=0;
			 ZDReply.Liushui[2]=ZDParameter.zdliushuinum>>8;
			 ZDReply.Liushui[3]=ZDParameter.zdliushuinum&0xff;
			 
				for(i=0; i<8; i++)
				{
					ZDReply.ZID[i] = ZDCommand.ZID[i];
				}
				DeputyParameter_init();
				i=sizeof(TerminalReply0x28);
				ZDReply.length[0] = i >> 8;
				ZDReply.length[1] = i;

         return i+19;//
      }
	 
	 
	 void Gprs28datasend(void)
	 {
		 UART_send((uint8_t *)(&ZDReply.start[0]),2);
		 GprsSndCheck = 0;
		 UART_send( (uint8_t *)(&ZDReply.command),15);	
		 UART_send( (uint8_t *)(&TerminalReply0x28),ZDReply.length[0]*256+ZDReply.length[1]);
		 ZDReply.check = ~GprsSndCheck+1;
		 UART_send( (uint8_t *)(&ZDReply.check),1);
		 //GprsSendComm("\r");  //结束符	
		 UART_send((uint8_t *)("\r"),1);		
   }
	 
	  unsigned short int Gprs12data(void)
	    {  
				uint16_t i;
				ZDReply.start[0] = 'Z';
				ZDReply.start[1] = 'L';
				ZDReply.command = 0x12;	 //
				for(i=0; i<4; i++)
				{
					ZDReply.Liushui[i] = ZDCommand.Liushui[i];
				}
// 				ZDParameter.zdliushuinum++;
// 			 if(ZDParameter.zdliushuinum>10000)//主动流水从1到10000
// 				ZDParameter.zdliushuinum=1;
// 			 ZDReply.Liushui[0]=0;
// 			 ZDReply.Liushui[1]=0;
// 			 ZDReply.Liushui[2]=ZDParameter.zdliushuinum>>8;
// 			 ZDReply.Liushui[3]=ZDParameter.zdliushuinum&0xff;
			 
				for(i=0; i<8; i++)
				{
					ZDReply.ZID[i] = ZDCommand.ZID[i];
				}
				
				switch(ZDCommand.data[0])
				{	 
				  case 0x00:	  //设备用户码
					     i=8+2;	 
						 break;
	
	
					case 0x03:	  //自动回报位置信息时长
						  i=2+2;	
						 break;
	
					case 0x04:	  //服务器 IP 地址、端口号
					     i=6+2;
						 break;
	
					case 0x05:	  //APN CMNET 不做更改  只读
					   i=5+2+2;
						 break;
	
					case 0x06:	  //0： UDP； 1： TCP  只读
						 i=1+2;
					
						 break;
	
					case 0x07:	  //设置 CAN 波特率
					   i=1+2;
						 break;
	
					case 0x09:	  //短信报警号码
					  i=strlen((char*)Parameterset.Parametersetitem.dxbjhm0x09)+2+2;
						 break;
	
	
					case 0x0e:	  //出口版/国内版1： 出口 0：国内  只读
					     i=1+2;
						 break;
	
					case 0x11:	  //SIM 卡 的 唯一 识别（ IMSI） 码 只读
					  	i=strlen((char*)Parameterset.Parametersetitem.IMSI0x11)+2+2;
						 break;
	
					case 0x12:	  //GSM 信号强度   只读
					    i=1+2;
						 break;
	
					case 0x13:	  //GPS 卫星颗数  只读
					     i=1+2;
						 break;
	
					case 0x14:	  //GPS 初始里程
					     i=4+2; 
						 break;
	
					case 0x15:	  //远程升级功能1： 开启 0： 关闭      
					    i=1+2;
						 break;
	
					case 0x16:	  //省电模式的开启、关闭 1：开启 0：关闭
					     i=1+2;
						 break;
	
					case 0x17:	  //设备编号 
					    i=strlen((char*)Parameterset.Parametersetitem.sbbh0x17)+2+2; 
						 break;
	
					case 0x18:	  //sim卡号
					     i=strlen((char*)Parameterset.Parametersetitem.SIMkh0x18)+2+2; 
						 break;
	
					case 0x19:	  //呼叫中心号码  
					     i=strlen((char*)Parameterset.Parametersetitem.hjzxhm0x19)+2+2; 
						 break;
	
	
					case 0x1b:	  //心跳包控制1：启动 0：关闭
					     i=1+2;
						 break;
	
					case 0x1c:	  //OxAA 报警时间设置 多长时间未收到报警上报报警信息
					     i=1+2;
						 break;
	
					case 0x1d:	  //主电源欠压值值/10。
					      i=1+2;
						 break;
	
					case 0x1e:	  //启用备用电源后上报周期
					     i=1+2;
						 break;
	
					case 0x20:	  //使用哪个输入口作为PLC/ECU 上电检查：0:高电平检测有效1:低电平检测有效
					     i=1+2;
						 break;
	
	
					case 0x22:	  //设备类型  
					     i=strlen((char*)Parameterset.Parametersetitem.sblx0x22)+2+2; 
						 break;
	
	
					case 0x58:	  //终端副参数查询
						   DeputyParameter_init();
				       i=sizeof(TerminalReply0x28)+2;
						 break;
	
					case 0x41:	  //位置汇报策略   2 定时和定距
					     i=1+2;
						 break;
					
					case 0x42:	  //行驶中定时上报距离  30s
					      i=1+2;
						 break;
					case 0x43:	  //行驶中定距上报距离  20=200m
					     i=1+2;
						 break;
					
					default: break;
				}
				ZDReply.length[0] = i >> 8;
				ZDReply.length[1] = i;

         return i+19;//
      }
	 
	 
	 void Gprs12datasend(void)
	 {
		  uint8_t bb=0x22;
		 UART_send((uint8_t *)(&ZDReply.start[0]),2);
		 GprsSndCheck = 0;
		 UART_send( (uint8_t *)(&ZDReply.command),15);	
		 //UART_send( (uint8_t *)(&TerminalReply0x28),ZDReply.length[0]*256+ZDReply.length[1]);
		 switch(ZDCommand.data[0])
				{	 
				  case 0x00:	  //设备用户码
						UART_send( (uint8_t *)(&ZDCommand.data),2);			
				    UART_send( (uint8_t *)(&Parameterset.Parametersetitem.sbyhm0x00),ZDReply.length[0]*256+ZDReply.length[1]-2);					
						 break;
	
	
					case 0x03:	  //自动回报位置信息时长
						 UART_send( (uint8_t *)(&ZDCommand.data),2);	
						 UART_send( (uint8_t *)(&Parameterset.Parametersetitem.hbsc0x03),ZDReply.length[0]*256+ZDReply.length[1]-2);					
						 break;
	
					case 0x04:	  //服务器 IP 地址、端口号
						 UART_send( (uint8_t *)(&ZDCommand.data),2);	
					   UART_send( (uint8_t *)(&Parameterset.Parametersetitem.fwqIP0x04),ZDReply.length[0]*256+ZDReply.length[1]-2);					
						 break;
	
					case 0x05:	  //APN CMNET 不做更改  只读
						UART_send( (uint8_t *)(&ZDCommand.data),2);	
						UART_send( &bb,1);   //结束符	
					  UART_send( (uint8_t *)(&Parameterset.Parametersetitem.APN0x05),ZDReply.length[0]*256+ZDReply.length[1]-4);
            UART_send( &bb,1);   //结束符						
						 break;
	
					case 0x06:	  //0： UDP； 1： TCP  只读
						UART_send( (uint8_t *)(&ZDCommand.data),2);	
					 UART_send( (uint8_t *)(&Parameterset.Parametersetitem.UDPTCP0x06),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x07:	  //设置 CAN 波特率
						  UART_send( (uint8_t *)(&ZDCommand.data),2);	
					   	UART_send( (uint8_t *)(&Parameterset.Parametersetitem.BaudRate0x07),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x09:	  //短信报警号码
						UART_send( (uint8_t *)(&ZDCommand.data),2);	
					  UART_send( &bb,1);   //结束符	
					  UART_send( (uint8_t *)(&Parameterset.Parametersetitem.dxbjhm0x09),ZDReply.length[0]*256+ZDReply.length[1]-4);
            UART_send( &bb,1);   //结束符		
						 break;
	
	
					case 0x0e:	  //出口版/国内版1： 出口 0：国内  只读
						  UART_send( (uint8_t *)(&ZDCommand.data),2);	
					   	UART_send( (uint8_t *)(&Parameterset.Parametersetitem.ckgn0x0e),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x11:	  //SIM 卡 的 唯一 识别（ IMSI） 码 只读
						  UART_send( (uint8_t *)(&ZDCommand.data),2);	
					    UART_send( &bb,1);   //结束符	
					    UART_send( (uint8_t *)(&Parameterset.Parametersetitem.IMSI0x11),ZDReply.length[0]*256+ZDReply.length[1]-4);
              UART_send( &bb,1);   //结束符		
						 break;
	
					case 0x12:	  //GSM 信号强度   只读
						UART_send( (uint8_t *)(&ZDCommand.data),2);	
	        	UART_send( (uint8_t *)(&Parameterset.Parametersetitem.GSMxhqd0x12),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x13:	  //GPS 卫星颗数  只读
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					     UART_send( (uint8_t *)(&Parameterset.Parametersetitem.GPSwxs0x13),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x14:	  //GPS 初始里程
						  UART_send( (uint8_t *)(&ZDCommand.data),2);	
					    UART_send( (uint8_t *)(&Parameterset.Parametersetitem.GPScslc0x14),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x15:	  //远程升级功能1： 开启 0： 关闭 
             UART_send( (uint8_t *)(&ZDCommand.data),2);							
					   UART_send( (uint8_t *)(&Parameterset.Parametersetitem.ycsj0x15),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x16:	  //省电模式的开启、关闭 1：开启 0：关闭
						  UART_send( (uint8_t *)(&ZDCommand.data),2);	
					    UART_send( (uint8_t *)(&Parameterset.Parametersetitem.sdms0x16),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x17:	  //设备编号 
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					     UART_send( &bb,1);   //结束符	
					     UART_send( (uint8_t *)(&Parameterset.Parametersetitem.sbbh0x17),ZDReply.length[0]*256+ZDReply.length[1]-4);
               UART_send( &bb,1);   //结束符		
						 break;
	
					case 0x18:	  //sim卡号
						    UART_send( (uint8_t *)(&ZDCommand.data),2);	
					      UART_send( &bb,1);   //结束符	
					      UART_send( (uint8_t *)(&Parameterset.Parametersetitem.SIMkh0x18),ZDReply.length[0]*256+ZDReply.length[1]-4);
                UART_send( &bb,1);   //结束符		
						 break;
	
					case 0x19:	  //呼叫中心号码  
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					      UART_send( &bb,1);   //结束符	
					      UART_send( (uint8_t *)(&Parameterset.Parametersetitem.hjzxhm0x19),ZDReply.length[0]*256+ZDReply.length[1]-4);
                UART_send( &bb,1);   //结束符		
						 break;
	
	
					case 0x1b:	  //心跳包控制1：启动 0：关闭
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					     UART_send( (uint8_t *)(&Parameterset.Parametersetitem.xtbkz0x1b),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x1c:	  //OxAA 报警时间设置 多长时间未收到报警上报报警信息
						  UART_send( (uint8_t *)(&ZDCommand.data),2);	
					    UART_send( (uint8_t *)(&Parameterset.Parametersetitem.AAbjsjset0x1c),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x1d:	  //主电源欠压值值/10
						    UART_send( (uint8_t *)(&ZDCommand.data),2);	
					      UART_send( (uint8_t *)(&Parameterset.Parametersetitem.zdqyz0x1D),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x1e:	  //启用备用电源后上报周期
						  UART_send( (uint8_t *)(&ZDCommand.data),2);	
					    UART_send( (uint8_t *)(&Parameterset.Parametersetitem.bdsbzq0x1E),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
					case 0x20:	  //使用哪个输入口作为PLC/ECU 上电检查：0:高电平检测有效1:低电平检测有效
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					    UART_send( (uint8_t *)(&Parameterset.Parametersetitem.PLCsrk0x20),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
	
	
					case 0x22:	  //设备类型  
						    UART_send( (uint8_t *)(&ZDCommand.data),2);	
					      UART_send( &bb,1);   //结束符	
					      UART_send( (uint8_t *)(&Parameterset.Parametersetitem.sblx0x22),ZDReply.length[0]*256+ZDReply.length[1]-4);
                UART_send( &bb,1);   //结束符		
						 break;
	
	
					case 0x58:	  //终端副参数查询
						 UART_send( (uint8_t *)(&ZDCommand.data),2);	
						 UART_send( (uint8_t *)(&TerminalReply0x28),ZDReply.length[0]*256+ZDReply.length[1]-2);
						 break;
	
					case 0x41:	  //位置汇报策略   2 定时和定距
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					     UART_send( (uint8_t *)(&Parameterset.Parametersetitem.wzhbcl0x41),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
					
					case 0x42:	  //行驶中定时上报距离  30s
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					     UART_send( (uint8_t *)(&Parameterset.Parametersetitem.dssbsj0x42),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
					case 0x43:	  //行驶中定距上报距离  20=200m
						   UART_send( (uint8_t *)(&ZDCommand.data),2);	
					     UART_send( (uint8_t *)(&Parameterset.Parametersetitem.djsbjl0x43),ZDReply.length[0]*256+ZDReply.length[1]-2);			
						 break;
					
					default: break;
				}
		 ZDReply.check = ~GprsSndCheck+1;
		 UART_send( (uint8_t *)(&ZDReply.check),1);
		 //GprsSendComm("\r");  //结束符
     UART_send((uint8_t *)("\r"),1);					
				
   }
	 
	 
	 
	 
	 /*
	   1.终端登陆中心服务器 0x19  回复尝试3次 2.B
	   2.终端上传报警信息   0x17  重复3次 15s  2.B
	   3.终端上传CAN总线    0x13
     4.自动定时上报位置   0x18  按设定时间   
     5.定位查询终端回传   0x11  次数与间隔时间
	   6.副参数数据块上报   0x28  刚上电，每8小时
	 */
	 //自动上传中心与回复中心
      void Autotransmit_Replycenter(void)
    {
			uint8_t i=0;
			if((ZDParameter.LoginReply)&&((state<11)||(state==0)))  
	  {	
			//记录上一个发送状态
			//TimeArrived2B  TimeArrived0x11   TimeArrived12
      //if((dxbjhmflag)&&(state==0))  dxbjhmflag0=1;//可消除接收短信标志
			
			if((TimeArrivedcsq)&&((state==0)||(state==2)))//信号质量查询
			{ 
				if(state==0)
				 {
					state=2;
					}	
					Gprsdatacapstat = Gprsdatacap(5);
				 if((Gprsdatacapstat == 2)||(Gprsdatacapstat == 0))	
				 {
					state=0;
          TimeArrivedcsq=0;
				 }
		  }
			else if((TimeArrivedcreg)&&((state==0)||(state==3)))//查基站
			{ 
				if(state==0)
				 {
					state=3;
					}	
					Gprsdatacapstat = Gprsdatacap(7);
				if((Gprsdatacapstat == 2)||(Gprsdatacapstat == 0))
				 {
					state=0;
          TimeArrivedcreg=0;								 
				 }
		  }
			
			else if((TimeArrived2B)&&((state==0)||(state==4)))//终端回复2.B应答命令 0x1A
			{ 
				if(state==0)
				 {
					datalenthlogin=Gprs2Bdata();
					state=4;
					}	
					GprsConnectTcpState = GprsSendData(datalenthlogin,1);
				if(GprsConnectTcpState == 4)
				 //if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))	
				 {
					state=0;
					if(TimeArrived2B==2)
						dxbjhmflag=0; 
            TimeArrived2B=0;
					//if(dxbjhmflag&&dxbjhmflag0) {dxbjhmflag=0;dxbjhmflag0=0;}
          if( TerminalReply0x1a.command==0X40)	
          		restartgps=1;	//执行终端重启			 
				 }
		  }
			else if((TimeArrived0x13)&&((state==0)||(state==5)))//上传CAN
			{ 
				if(state==0)
				 {
					datalenthlogin=GprsCANdata();
					state=5;
					}	
					GprsConnectTcpState = GprsSendData(datalenthlogin,2);
				if(GprsConnectTcpState == 4)
				//if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))
				 {
					state=0;
          TimeArrived0x13=0;								 
				 }
		  }
			else if((TimeArrived0x18)&&((state==0)||(state==6)))//自动定时上报位置
			{ 
				if(state==0)
				 {
					datalenthlogin=Gprs1811data(0x18);
					state=6;
					}	
					GprsConnectTcpState = GprsSendData(datalenthlogin,3);
				if(GprsConnectTcpState == 4)
				//if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))
				 {
					state=0;
          TimeArrived0x18=0;								 
				 }
		  }
			else if((TimeArrived0x11)&&((state==0)||(state==7)))//定位查询回传
			{  
				if(state==0)
				 {
					datalenthlogin=Gprs1811data(0x11);
					state=7;
					}	
					GprsConnectTcpState = GprsSendData(datalenthlogin,3);
				if(GprsConnectTcpState == 4)
				 //if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))	
				 {
					state=0;
					 if(TimeArrived0x11==2)
						dxbjhmflag=0; 
           TimeArrived0x11=0;	
          //if(dxbjhmflag&&dxbjhmflag0) {dxbjhmflag=0;dxbjhmflag0=0;}					 
				 }
		  }
			else if((TimeArrived0x17)&&((state==0)||(state==8)))//报警信息上传
			{  
				if(state==0)
				 {
					datalenthlogin=Gprs17data();
					state=8;
					}	
					GprsConnectTcpState = GprsSendData(datalenthlogin,6);
				if(GprsConnectTcpState == 4)//重新登录成功必须发送
				 //if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))	
				 {
					state=0;
          TimeArrived0x17=0;
          sendbjxx0x17repovertime=15;//应答15s	
          sendbjxx0x17count++;//记录发送次数					 
				 }
		  }	
			else if((TimeArrived12)&&((state==0)||(state==9)))//参数设置命令帧
			{  
				if(state==0)
				 {
					datalenthlogin=Gprs12data();
					state=9;
					}	
					GprsConnectTcpState = GprsSendData(datalenthlogin,5);
				if(GprsConnectTcpState == 4)
				 //if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))	
				 {
					state=0;
					if(TimeArrived12==2)
						dxbjhmflag=0; 
          TimeArrived12=0;
          //if(dxbjhmflag&&dxbjhmflag0) {dxbjhmflag=0;dxbjhmflag0=0;}					 
				 }
		  }	
			else if((TimeArrived0x28)&&((state==0)||(state==10)))//副参数上传
			{  
				if(state==0)
				 {
					datalenthlogin=Gprs28data();
					state=10;
					}	
					GprsConnectTcpState = GprsSendData(datalenthlogin,4);
				if(GprsConnectTcpState == 4)//重新登录成功必须发送
				 //if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))	
				 {
					state=0;
          TimeArrived0x28=0;	
         
				 }
		  }	
			
		}
		//空stater=11做分割，重新发送短信用
		else if((dxbjhmflag)&&(((state>11)&&(state<=20))||(state==0)))
		{
			if((TimeArrivedcsq==2)&&((state==0)||(state==12)))//信号质量查询
			{ 
				if(state==0)
				 {
					state=12;
					}	
					Gprsdatacapstat = Gprsdatacap(5);
				 if((Gprsdatacapstat == 2)||(Gprsdatacapstat == 0))	
				 {
					state=0;
          TimeArrivedcsq=0;
				 }
		  }
			else if((TimeArrivedcreg==2)&&((state==0)||(state==13)))//查基站
			{ 
				if(state==0)
				 {
					state=13;
					}	
					Gprsdatacapstat = Gprsdatacap(7);
				if((Gprsdatacapstat == 2)||(Gprsdatacapstat == 0))
				 {
					state=0;
          TimeArrivedcreg=0;								 
				 }
		  }
			
			else if((TimeArrived2B==2)&&((state==0)||(state==14)))//终端回复2.B应答命令 0x1A
			{ 
				if(state==0)
				 {
					 datalenthlogin=Gprs2Bdata();
					 if(datalenthlogin<=140)
					 { 
				    SMSsend_init(datalenthlogin);
					  state=14;
					 }
					 else
					 {
						 dxbjhmflag=0;//清短信接收区分号标志
						return;
					 }
					}	
					SMSdatasendstat=SMSSendData(datalenthlogin,1);
				if(SMSdatasendstat == 4)
				 //if((SMSdatasendstat == 4)||(SMSdatasendstat == 0))	
				 {
					state=0;
          TimeArrived2B=0;
					dxbjhmflag=0;//清短信接收区分号标志
          //if( TerminalReply0x1a.command==0X40)	//执行终端重启			 
				 }
		  }
// 			else if((TimeArrived0x13)&&((state==0)||(state==15)))//上传CAN
// 			{ 
// 				if(state==0)
// 				 {
// 					datalenthlogin=GprsCANdata();
// 					state=15;
// 					}	
// 					GprsConnectTcpState = GprsSendData(datalenthlogin,2);
// 				//if(GprsConnectTcpState == 4)
// 				if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))
// 				 {
// 					state=0;
//           TimeArrived0x13=0;								 
// 				 }
// 		  }
// 			else if((TimeArrived0x18)&&((state==0)||(state==16)))//自动定时上报位置
// 			{ 
// 				if(state==0)
// 				 {
// 					datalenthlogin=Gprs1811data(0x18);
// 					state=16;
// 					}	
// 					GprsConnectTcpState = GprsSendData(datalenthlogin,3);
// 				//if(GprsConnectTcpState == 4)
// 				if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))
// 				 {
// 					state=0;
//           TimeArrived0x18=0;								 
// 				 }
// 		  }
			else if((TimeArrived0x11==2)&&((state==0)||(state==17)))//定位查询回传
			{  
				if(state==0)
				 {
					 datalenthlogin=Gprs1811data(0x11);
					 if(datalenthlogin<=140)
					 { 
				    SMSsend_init(datalenthlogin);
					  state=17;
					 }
					 else
					 {
						 dxbjhmflag=0;//清短信接收区分号标志
						 return;
					 }
					}	
					
					SMSdatasendstat=SMSSendData(datalenthlogin,3);
				if(SMSdatasendstat== 4)
				 //if((SMSdatasendstat == 4)||(SMSdatasendstat == 0))	
				 {
					state=0;
          TimeArrived0x11=0;	
          dxbjhmflag=0;//清短信接收区分号标志					 
				 }
		  }
// 			else if((TimeArrived0x17)&&((state==0)||(state==18)))//报警信息上传
// 			{  
// 				if(state==0)
// 				 {
// 					datalenthlogin=Gprs17data();
// 					  if(datalenthlogin<=140)
// 					 { 
// 				    SMSsend_init(datalenthlogin);
// 					  state=18;
// 					 }
// 					 else
// 					 {
// 						 dxbjhmflag=0;//清短信接收区分号标志
// 						 return;
// 					 } 
// 					}	
// 					SMSdatasendstat=SMSSendData(datalenthlogin,6);
// 				if(SMSdatasendstat == 4)//重新登录成功必须发送
// 				 //if((SMSdatasendstat == 4)||(SMSdatasendstat == 0))	
// 				 {
// 					state=0;
//           TimeArrived0x17=0;
// 					dxbjhmflag=0;//清短信接收区分号标志					 
//           //sendbjxx0x17repovertime=15;//应答15s	
//           //sendbjxx0x17count++;//记录发送次数					 
// 				 }
// 		  }	
			else if((TimeArrived12==2)&&((state==0)||(state==19)))//参数设置命令帧
			{  
				if(state==0)
				 {
					datalenthlogin=Gprs12data();
					 if(datalenthlogin<=140)
					 { 
				    SMSsend_init(datalenthlogin);
					  state=19;
					 } 
						else
					 {
						 dxbjhmflag=0;//清短信接收区分号标志
						 return;
					 }  
					}	

					SMSdatasendstat=SMSSendData(datalenthlogin,5);
				 if(SMSdatasendstat == 4)
				 //if((SMSdatasendstat == 4)||(SMSdatasendstat == 0))	
				 {
					state=0;
          TimeArrived12=0;
          dxbjhmflag=0;//清短信接收区分号标志					 					 
				 }
		  }	
// 			else if((TimeArrived0x28)&&((state==0)||(state==20)))//副参数上传
// 			{  
// 				if(state==0)
// 				 {
// 					datalenthlogin=Gprs28data();
// 					state=20;
// 					}	
// 					GprsConnectTcpState = GprsSendData(datalenthlogin,4);
// 				if(GprsConnectTcpState == 4)//重新登录成功必须发送
// 				 //if((GprsConnectTcpState == 4)||(GprsConnectTcpState == 0))	
// 				 {
// 					state=0;
//           TimeArrived0x28=0;	
//           dxbjhmflag=0;//清短信接收区分号标志										 
// 				 }
// 		  }	
			
			
    }	
		
		
		//
		if((deletereadedsms)&&((state==0)||(state==21)))//删除已读短信
			{ 
				if(state==0)
				 {
					state=21;
				 }	
					SMSdatacapstat = SMSdatacap(2);
				 //if((SMSdatacapstat == 2)||(SMSdatacapstat == 0))	
				 if(SMSdatacapstat == 2) 
				 {
					state=0;
					deletereadedsms=0;
				 }
		  }
		//else if((!dxbjhmflag)&&(readsmsnumok&0x1ff)&&(((state==0)&&(GprsRevOver==0))||(state==22)))//读短信内容  有N条短信需要读取   上条短信处理完成
	  else if((readsmsnumok&0x1ff)&&(((state==0)&&(GprsRevOver==0))||(state==22)))//读短信内容  有N条短信需要读取   上条短信处理完成
		{ 
				if(state==0)
				 {
					 smsdatarecnum=0;
				   smsdataber = 0;
				   smsdataend=0;
					 state=22;
				 }	
					SMSdatacapstat = SMSdata_read();
				 //if((SMSdatacapstat == 2)||(SMSdatacapstat == 0))	
				 if(SMSdatacapstat == 2) 
				 {
					state=0;
					 while(!(readsmsnumok&BIT(i))) 
				   i++;
					 readsmsnumok&=~BIT(i);//清本次短信号
				 }
		  }
			
			if(readsmsnumok==(BIT(10)))//全部短信读完 
			{
				memset((void *)smsnum,0,30);
				readsmsnumok=0;
				deletereadedsms=1;
			}
			
			if((closegprsnet)&&((state==0)||(state==23)))//关闭所有GPRS连接
			{ 
				if(state==0)
				 {
					 state=23;
					}	
					Gprsdatacapstat = Gprsdatacap(10);
				 if((Gprsdatacapstat == 2)||(Gprsdatacapstat == 0))	
				 {
					state=0;
          closegprsnet=0;
				 }
		  }
			
			if((dormantgprs)&&((state==0)||(state==24)))//休眠GPRS
			{ 
				if(state==0)
				 {
					state=24;
				 }	
					SMSdatacapstat = SMSdatacap(0);
				 //if((SMSdatacapstat == 2)||(SMSdatacapstat == 0))	
				 if(SMSdatacapstat == 2) 
				 {
					state=0;
					dormantgprs=0;
				 }
		  }
			
 }
	 
	 
	 void GprsProtocolAnalysis(void)
{
	uint8_t i=0;
	if(GprsRevOver == 1)
	{	   	
		if(ZDCommand.command<70) //70,71,72,73,74为升级指令
		sendbjxx0x17time=0;//终端上传报警信息  每连续1小时未收到服务器指令
		switch(ZDCommand.command)
		{
	     case 0x11: //中心发送定位查询
				      
               if(!ZDCommand.data[1])//次数为0
							 {
								 TerminalReply0x1a.command=0X11;
			           TerminalReply0x1a.success=0x02;
                }
								else if(ZDCommand.data[1]==1)//次数为1
								{	
			           TerminalReply0x1a.command=0X11;
			           TerminalReply0x1a.success=0;
								}
								else if((ZDCommand.data[1]>1)&&(ZDCommand.data[0]))//次数大于1，间隔不为0
								{
									 if(ZDCommand.data[0]<5)//限制间隔时间至少5s
										 ZDCommand.data[0]=5;
									 ZDParameter.IntervalTimer =ZDCommand.data[0];
				           ZDParameter.TraceCount =	ZDCommand.data[1];
									 TerminalReply0x1a.command=0X11;
			            TerminalReply0x1a.success=0x00;
                }
							 TimeArrivedcreg=1+issms;//先查基站	后发送
			         TimeArrived2B=1+issms; 
			         issms=0;
		        break;
	
		   case 0x12:   //中心发送参数设置命令帧
	
				switch(ZDCommand.data[0])
				{	 
				  case 0x00:	  //设备用户码
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.sbyhm0x00[0]=ZDCommand.data[2];
								 Parameterset.Parametersetitem.sbyhm0x00[1]=ZDCommand.data[3];
								 Parameterset.Parametersetitem.sbyhm0x00[2]=ZDCommand.data[4];
								 Parameterset.Parametersetitem.sbyhm0x00[3]=ZDCommand.data[5];
								 Parameterset.Parametersetitem.sbyhm0x00[4]=ZDCommand.data[6];
								 Parameterset.Parametersetitem.sbyhm0x00[5]=ZDCommand.data[7];
								 Parameterset.Parametersetitem.sbyhm0x00[6]=ZDCommand.data[8];
								 Parameterset.Parametersetitem.sbyhm0x00[7]=ZDCommand.data[9];
               }
							   
								 TimeArrived12=1+issms;  
								 issms=0;
						 break;
	
	
					case 0x03:	  //自动回报位置信息时长
						  if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.hbsc0x03[0]=ZDCommand.data[2];
								 Parameterset.Parametersetitem.hbsc0x03[1]=ZDCommand.data[3];
								 autorep_loctime=(Parameterset.Parametersetitem.hbsc0x03[0]<<8)+Parameterset.Parametersetitem.hbsc0x03[1];
							 } 
							   
								 TimeArrived12=1+issms;  
					       issms=0;
						 break;
	
					case 0x04:	  //服务器 IP 地址、端口号
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.fwqIP0x04[0]=ZDCommand.data[2];
								 Parameterset.Parametersetitem.fwqIP0x04[1]=ZDCommand.data[3];
								 Parameterset.Parametersetitem.fwqIP0x04[2]=ZDCommand.data[4];
								 Parameterset.Parametersetitem.fwqIP0x04[3]=ZDCommand.data[5];
								 Parameterset.Parametersetitem.fwqIP0x04[4]=ZDCommand.data[6];
								 Parameterset.Parametersetitem.fwqIP0x04[5]=ZDCommand.data[7];
							 } 
							  
								 TimeArrived12=1+issms;
							   issms=0;  
						 break;
	
					case 0x05:	  //APN CMNET 不做更改  只读
					   Parameterset.Parametersetitem.APN0x05[0]='C'; 
					   Parameterset.Parametersetitem.APN0x05[1]='M' ; 
					   Parameterset.Parametersetitem.APN0x05[2]='N' ; 
					   Parameterset.Parametersetitem.APN0x05[3]='E' ; 
					   Parameterset.Parametersetitem.APN0x05[4]='T' ; 
					  
					  TimeArrived12=1+issms;  issms=0;
						 break;
	
					case 0x06:	  //0： UDP； 1： TCP  只读
						  Parameterset.Parametersetitem.UDPTCP0x06=1;
					    
					    TimeArrived12=1+issms;  issms=0;
					
						 break;
	
					case 0x07:	  //设置 CAN 波特率
					   if(ZDCommand.data[1])//写标志
							 {
								if(Parameterset.Parametersetitem.BaudRate0x07!=ZDCommand.data[2])
								{	
                 canBaudRatechange=1;//波特率变化									
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.BaudRate0x07=ZDCommand.data[2];
								 BaudRate = Parameterset.Parametersetitem.BaudRate0x07*1000;//250k
	               CAN_Open(CAN0,  BaudRate, CAN_NORMAL_MODE);
	               while(!CANRx_initial(CAN0)); 
								}
							 } 
							  
								 TimeArrived12=1+issms;  issms=0;
			
						 break;
	
					case 0x09:	  //短信报警号码
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 memset((void *)Parameterset.Parametersetitem.dxbjhm0x09,0,29);//清除号码 
								for(i=0;i< ZDCommand.length[1]-4;i++) //""
								Parameterset.Parametersetitem.dxbjhm0x09[i]=ZDCommand.data[3+i];
								 moubilenumswitch();//交换号码位置
							 } 
							  
								 TimeArrived12=1+issms;  issms=0;  
					
						 break;
	
	
					case 0x0e:	  //出口版/国内版1： 出口 0：国内  只读
					     Parameterset.Parametersetitem.ckgn0x0e=0;
					    
					    TimeArrived12=1+issms;  issms=0; 
						 break;
	
					case 0x11:	  //SIM 卡 的 唯一 识别（ IMSI） 码 只读
					  
					       TimeArrived12=1+issms;  issms=0;   
						 break;
	
					case 0x12:	  //GSM 信号强度   只读
					      TimeArrivedcsq=1+issms;;
					      TimeArrived12=1+issms;  issms=0; 
						 break;
	
					case 0x13:	  //GPS 卫星颗数  只读
					     TimeArrived12=1+issms;  issms=0; 
						 break;
	
					case 0x14:	  //GPS 初始里程
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								TerminalReply0x110x18.GPS_sum[0]=ZDCommand.data[2];
								TerminalReply0x110x18.GPS_sum[1]=ZDCommand.data[3]; 
								TerminalReply0x110x18.GPS_sum[2]=ZDCommand.data[4];
								TerminalReply0x110x18.GPS_sum[3]=ZDCommand.data[5];  
							 } 
							  
								 TimeArrived12=1+issms;  issms=0;  
						 break;
	
					case 0x15:	  //远程升级功能1： 开启 0： 关闭      
					    if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.ycsj0x15=ZDCommand.data[2];
							 } 
							 
								 TimeArrived12=1+issms;  issms=0; 
						 break;
	
					case 0x16:	  //省电模式的开启、关闭 1：开启 0：关闭
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.sdms0x16=ZDCommand.data[2];
							 } 
							 
								  TimeArrived12=1+issms;  issms=0; 
						 break;
	
					case 0x17:	  //设备编号
						 if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 memset((void *)Parameterset.Parametersetitem.sbbh0x17,0,50);//清除号码 
								for(i=0;i< ZDCommand.length[1]-4;i++) //""
								Parameterset.Parametersetitem.sbbh0x17[i]=ZDCommand.data[3+i];
							 } 
							
								 TimeArrived12=1+issms;  issms=0;  
					     
						 break;
	
					case 0x18:	  //sim卡号
						    if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 memset((void *)Parameterset.Parametersetitem.SIMkh0x18,0,20);//清除号码 
								for(i=0;i< ZDCommand.length[1]-4;i++) //""
								Parameterset.Parametersetitem.SIMkh0x18[i]=ZDCommand.data[3+i];
							 } 
							
								 TimeArrived12=1+issms;  issms=0;  
					     
						 break;
	
					case 0x19:	  //呼叫中心号码
					     
					   if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 memset((void *)Parameterset.Parametersetitem.hjzxhm0x19,0,20);//清除号码 
								for(i=0;i< ZDCommand.length[1]-4;i++) //""
								Parameterset.Parametersetitem.hjzxhm0x19[i]=ZDCommand.data[3+i];
							 } 
							  
								 TimeArrived12=1+issms;  issms=0;    
					
						 break;
	
	
					case 0x1b:	  //心跳包控制1：启动 0：关闭
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.xtbkz0x1b=ZDCommand.data[2];
							 } 
							  
								 TimeArrived12=1+issms;  issms=0; 
						 break;
	
					case 0x1c:	  //OxAA 报警时间设置 多长时间未收到报警上报报警信息
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.AAbjsjset0x1c=ZDCommand.data[2];
							 } 
							  
								 TimeArrived12=1+issms;  issms=0; 
						 break;
	
					case 0x1d:	  //主电源欠压值值/10。
					      if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.zdqyz0x1D=ZDCommand.data[2];
							 } 
							   
								  TimeArrived12=1+issms;  issms=0; 
					  
						 break;
	
					case 0x1e:	  //启用备用电源后上报周期
					      if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.bdsbzq0x1E=ZDCommand.data[2];
							 } 
							   
								  TimeArrived12=1+issms;  issms=0; 
					   
						 break;
	
					case 0x20:	  //使用哪个输入口作为PLC/ECU 上电检查：0:高电平检测有效1:低电平检测有效
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.PLCsrk0x20=ZDCommand.data[2];
							 } 
							   
								  TimeArrived12=1+issms;  issms=0; 
						 break;
	
	
					case 0x22:	  //设备类型
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 memset((void *)Parameterset.Parametersetitem.sblx0x22,0,20);//清除号码 
								for(i=0;i< ZDCommand.length[1]-4;i++) //""
								Parameterset.Parametersetitem.sblx0x22[i]=ZDCommand.data[3+i];
							 } 
							  
								 TimeArrived12=1+issms;  issms=0; 
					
						 break;
	
	
					case 0x58:	  //终端副参数查询
						  
					     TimeArrived12=1+issms;  issms=0; 
					     //TimeArrived0x28=1;
						 break;
	
					case 0x41:	  //位置汇报策略   2 定时和定距
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.wzhbcl0x41=ZDCommand.data[2];
							 } 
							   
								  TimeArrived12=1+issms;  issms=0; 
						 break;
					
					case 0x42:	  //行驶中定时上报距离  30s
					      if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.dssbsj0x42=ZDCommand.data[2];
							 } 
							  
								  TimeArrived12=1+issms;  issms=0; 
						 break;
					case 0x43:	  //行驶中定距上报距离  20=200m
					     if(ZDCommand.data[1])//写标志
							 {
								 memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem. djsbjl0x43=ZDCommand.data[2];
							 } 
							   
								 TimeArrived12=1+issms;  issms=0; 
						 break;
					
					default:
			             break;
				}
				
		        break;
	
		   case 0x13:   //中心发送CAN总线命令 回复2.B
				 TerminalReply0x1a.command=0X13;
			   TerminalReply0x1a.success=0;
			   TimeArrivedcreg=1+issms;  issms=0; //先查基站	后发送
			       TimeArrived2B=1+issms;  issms=0;  
		        break;
	
		   case 0x14:   //中心应答命令	 0X19：登录成功      0x17:终端上传报警信息格式
				   if((ZDCommand.data[0] == 0x19) && (ZDCommand.data[1] == 0))
				   {
						 if(TimeArrivedlogin) {GprsRevOver = 0;return;}
						ZDParameter.LoginCount = 0;
			      ZDParameter.LoginTimer = 270;
					  ZDParameter.LoginReply = 1;
						TimeArrived0x28=1;//登录一次马上上传副参数
				   }
					 if((ZDCommand.data[0] == 0x17) && (ZDCommand.data[1] == 0))
				   {
						 if(TimeArrived0x17) return;
						 sendbjxx0x17count=0;//记录发送次数	
						 sendbjxx0x17repovertime=15;//应答15s	
						 if(TerminalReply0x17.baojingflag==0x98)//休眠报警
						 {
             }
						 TerminalReply0x17.baojingflag=0;
				   }
		        break;
	
		   case 0x16:   //中心设置 CAN 口定时发送数据
				    TerminalReply0x1a.command=0X16;
			      TerminalReply0x1a.success=0;
			      TimeArrivedcreg=1+issms;  issms=0; //先查基站	后发送
		         TimeArrived2B=1+issms;  issms=0;  
		        break;
	
		   case 0x40:   //中心发送终端复位重启	 ================================
            TerminalReply0x1a.command=0X40;
			      TerminalReply0x1a.success=0;
			      TimeArrivedcreg=1+issms;  issms=0; //先查基站	后发送
				    TimeArrived2B=1+issms;  issms=0;  //发送完重启
				break;
			 
		   case 0x41:   //中心发送升级服务器IP配置 ================================
				          memchange=1;//数据更改标识，需要做一次存储
								 Parameterset.Parametersetitem.sjfwqIP0x04[0]=ZDCommand.data[0];
								 Parameterset.Parametersetitem.sjfwqIP0x04[1]=ZDCommand.data[1];
								 Parameterset.Parametersetitem.sjfwqIP0x04[2]=ZDCommand.data[2];
								 Parameterset.Parametersetitem.sjfwqIP0x04[3]=ZDCommand.data[3];
								 Parameterset.Parametersetitem.sjfwqIP0x04[4]=ZDCommand.data[4];
								 Parameterset.Parametersetitem.sjfwqIP0x04[5]=ZDCommand.data[5];
				 
//             TerminalReply0x1a.command=0X41;
// 			      TerminalReply0x1a.success=0;
// 			      TimeArrivedcreg=1;//先查基站	后发送
// 				    TimeArrived2B=1; //发送完重启
				break;
	
		   case 0x1b:   //中心清除各种状态标志 ================================
	
				switch(ZDCommand.data[0])
				{	 
				    
	
					case 0x0d:	  //清除“ SIM 卡曾拔卡标志”
					     TerminalReply0x28.baSIMnum=0; //拔SIM卡次数为0
						 break;
	
					case 0x0f:	  //清除“ SIM 卡更换报警”    
					    memparamset.memparam.simkaghcs=0;//sim卡更换次数为0 
						 break;
	
					case 0x16:	  //
					     TerminalReply0x28.kaigainum=0;//开盖次数为0
						 break;
					
					default:
			             break;
				}
          TerminalReply0x1a.command=0X1b;
					TerminalReply0x1a.success=0;
				   TimeArrivedcreg=1+issms;  issms=0; //先查基站	后发送
	         TimeArrived2B=1+issms;  issms=0;  
		        break;

		  
		   default:
			    break;
	
		}
		GprsRevOver = 0;
	}
}
 void SMSsend_init(unsigned char len)
{unsigned char i;
	
   SMSsend.SCA=0;
   SMSsend.PDUtype=0X11;
   SMSsend.MR=0;
 if(dxbjhmflag&BIT(0))//13973100157 11
 {
	 SMSsend.numlenght=0x0D;
	 SMSsend.numtype=0x91;
	for(i=0;i<29;i++)
	SMSsend.mobile_number[i]=0;
	for(i=0;i<29;i++)
	SMSsend.mobile_number[i]=dxbjhm[0][i]; 
 }
 else//0/2  //106575531157170 15
 {
	 //SMSsend.numlenght=0x0F;
	 SMSsend.numlenght=dxbjhm1_lenght;//原始电话号码长度
	 SMSsend.numtype=0xA1;
	 for(i=0;i<29;i++)
	SMSsend.mobile_number[i]=0;
	for(i=0;i<29;i++)
	SMSsend.mobile_number[i]=dxbjhm[1][i]; 
	if((SMSsend.mobile_number[0]=='6')&&(SMSsend.mobile_number[1]=='8'))
   SMSsend.numtype=0x91;		
 } 
  //dxbjhm[1][i]
 SMSsend.PID=0;
 SMSsend.DCS=0x04;
 SMSsend.vp=0;
 SMSsend.UDL=len;

 
}
  
 void SMSSend_hearder(void)
{ pdutoASC=1;//十六进制转ASC BCD码
  UART_sendhearder( (uint8_t *)(&SMSsend.SCA),5);	
	pdutoASC=0;
	UART_sendhearder( (uint8_t *)(&SMSsend.mobile_number),strlen((char*)SMSsend.mobile_number));
	pdutoASC=1;
	UART_sendhearder( (uint8_t *)(&SMSsend.PID),4);	
	pdutoASC=0;
}
 
//SMS上传数据  //len:上传数据长度，  sernum:上传序号
unsigned char SMSSendData(unsigned short int len,unsigned char sernum)
{
	//只在本函数中有效的静态常量
//     static char CommSended = 0; 
//     static char DataSended = 0;
	
    if( smsDataSended == 0 )   //数据未发送完
    {
       if(smsCommSended)  //命令已发送
       {
          if( (strstr((char *)strbuf,at_SMScom_rep[3]) == NULL) ) //有返回命令;
          {           
						 if(Gprs_Timeout_Count >= 50)
						 {
										Gprs_Timeout_Count = 0;
										//memset((void *)strbuf,0,200); bufloc=0;
							      memset((void *)strbuf,0,50); bufloc=0;
										//at_command_order = 0;	
										//AtCommSendingState = COMMSENDED;
										smsDataSended = 0;
										smsCommSended = 0; 
							      state=0;
							      //ZDParameter.LoginReply=0;//发送失败后重新登录
							      //ZDParameter.LoginTimer =270;//马上重新登录
										return 0;	
							     
						 }  
								 return 3;
          }
          else				//没有返回命令;
          {

// 							GprsSendComm( (char *)(&(datap->Head)) );
// 							GprsSendComm( (char *)(&(datap->SimID)) );
// 						
// 							GprsSendComm( (char *)(&(datap->TotalWalt)) );
// 							GprsSendComm( (char *)(&(datap->TotalTime)) );

// 							GprsSendComm( (char *)(&(datap->CurrentTempture)) );

//               GprsSendComm( (char *)(&(datap->CurrentStateOne)) );
//               GprsSendComm( (char *)(&(datap->CurrentStateTwo)) );
// 						  GprsSendComm( (char *)(&(datap->send_T)) );
// 						  GprsSendComm( (char *)(&(datap->walt_value)) );
// 						  GprsSendComm( (char *)(&(datap->ptotectgz)) );
// 						  GprsSendComm( (char *)(&(datap->ptotectld)) );
							/********************************************/	
						
						 
						  SMSSend_hearder();
						  pdutoASC=1;//十六进制转ASC BCD码
              sendcontent(sernum);
						  pdutoASC=0;
						  //GprsSendComm("\r");
						 _PutChar_f(0x1a); 
             smsDataSended  = 1;    
             Gprs_Timeout_Count = 0;        
             return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_SMScom[3]);//发送上传数据命令;
				 if(dxbjhmflag&BIT(0))//13973100157 11
				  GprsSendComm((char *)FrameLengthToASC(15+len));//发送ASCII数据;
         else //0/2
         GprsSendComm((char *)FrameLengthToASC(8+dxbjhm1_lenght/2+dxbjhm1_lenght%2+len));//发送ASCII数据;
				 GprsSendComm("\r");
         Gprs_Timeout_Count = 0; 
         smsCommSended = 1;
         return 3; 
      }
   }
   else    //数据发送完
   {
      if( (strstr((char *)strbuf,at_SMScom_rep[4]) == NULL) )  //未收到发送完回码
      {
					if(Gprs_Timeout_Count >= 150)//15s
					{
							Gprs_Timeout_Count = 0;
							//memset((void *)strbuf,0,200); bufloc=0;
						  memset((void *)strbuf,0,50); bufloc=0;
							//at_command_order = 0;	
							AtCommSendingState = COMMSENDED;	
							smsDataSended = 0;
							smsCommSended = 0; 
						  state=0;
						  //ZDParameter.LoginReply=0;//发送失败后重新登录
							return 0;		    
					}  
          return 3;	
       }
       else
       {
          //memset((void *)strbuf,0,200); bufloc=0;
				  memset((void *)strbuf,0,50); bufloc=0;
          smsDataSended = 0;
          smsCommSended = 0; 
          //gprsNoConnect = 0;
          Gprs_Timeout_Count = 0;
          return 4;
       }   
    }
}









#endif 
































