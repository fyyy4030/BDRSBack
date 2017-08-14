#ifndef	 _M660_H_
#define  _M660_H_


#include <stdlib.h>
#include <string.h>	 
#include <stdio.h>
#include "NUC131.h"
#include "proto.h"
//#include "globle.h"
#include "canMessage.h"
#include "alart.h"


//#define ID 		"20000000001"
//#define ID 		"10000000006"
//  #define ID 		"00160516003"
	#define ID 		"00160822018"

//IP:122.0.75.201,8880   ;171.8.225.246,8002   106.3.44.168:8880


//建立连接命令

//IP:122.0.75.201,8880   ;171.8.225.246,8002   106.3.44.168:8880

//const char *at_command[9]={"at\r","at+ccid\r", "at+csq\r","at+creg?\r","at+cgdcont=1,\"IP\",\"cmnet\"\r","at+xiic=1\r","at+tcpclose=0\r","at+tcpsetup=0,171.8.225.246,8880\r","at+tcpsend=0,"};
//const char *at_command_reply[10]={"at\r\r\nOK\r\n","\r\nOK\r\n", "\r\nOK\r\n","+CREG: 0,1 \r\n\r\nOK\r\n","\r\nOK\r\n","\r\nOK\r\n","+TCPCLOSE:","+TCPSETUP:0,OK","\r\n>","+TCPSEND:0,"};

//wushengjun add 2017.7.27
//const char *at_command[18]={"at\r","at+csq\r","AT+CPIN?\r","AT+CMGF=0\r","AT+CIMI\r","at+csq\r","at+creg=2\r","at+creg?\r", "AT+CGATT=1\r",  "AT+CGATT?\r",  "AT+CIPSHUT\r", "at+cipmux?\r","at+cipmux=1\r","AT+CSTT=\"CMNET\"\r",  "AT+CIICR\r",  "AT+CIFSR\r","AT+CIPSTART=0,\"TCP\",\"171.8.225.246\",8003\r","AT+CIPSEND=0," };
const char *at_command[10]={"at\r","AT+CIPSHUT\r","at+cipmux=1\r","at+creg?\r","AT+CSTT=\"CMNET\"\r","AT+CIICR\r","AT+CIFSR\r","AT+CCID\r","AT+CIPSTART=0,\"TCP\",\"171.8.225.246\",8003\r","AT+CIPSEND=0," };

//const char *at_command[9]={"at\r","at+ccid\r","AT+CIPMUX=1\r" "at+csq\r","at+creg?\r","AT+CSTT=CMNET","AT+CIPSTART=0,\"TCP\",\"171.8.225.246\",8003\r","AT+CIPSEND=0,\r"}; //,"at+cgdcont=1,\"IP\",\"cmnet\"\r", "AT+CIICR\r", 
const char *at_command_reply[11]={"at\r\r\nOK\r\n","OK","OK\r\n","+CREG: 0,1\r\n\r\nOK\r\n","OK","OK","\r\n","OK","CONNECT OK",">","SEND OK" };



typedef uint8_t  UINT8 ;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef unsigned char *   PUINT8;



#define sysTxBufReadNextOne()	(((_sys_uUartTxHead+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxHead+1)
#define sysTxBufWriteNextOne()	(((_sys_uUartTxTail+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxTail+1)
#define UART_BUFFSIZE	256
uint8_t _sys_ucUartTxBuf[UART_BUFFSIZE];
uint16_t _sys_uUartTxHead, _sys_uUartTxTail;

#define RX_ARRAY_NUM 100
 uint8_t uart_rx[RX_ARRAY_NUM] = {0};
 uint8_t uart_rx2[RX_ARRAY_NUM] = {0};
 uint32_t volatile rx_cnt = 0;
 uint32_t volatile rx_cnt2 = 0;
char CCID[20];
//wushengjun add  2017.8.7
char SavedCCID[20];
char *PointerSaveCCID = NULL;

uint32_t havegetccid = 0;

 uint8_t  TransferTimeArrived;
 uint8_t  GprsConnectTcpState;
 uint8_t  GprsdataReady = 0;
 uint8_t  GpsdataReady = 0;
 //uint8_t gpsdats[200];
// uint8_t gpsdatsbuf[200];
 
 uint8_t sendstate=0;
 
 
#define COMMSENDING 1
#define COMMSENDED 0

UINT8 VerifyGps(UINT8* GpsDatap);



//UINT8 gprsNoConnect = 0;
UINT8 LengthAsc[5];


UINT8  GprsSending = 0;
UINT32 Gprs_Timeout_Count;
UINT8 at_command_order = 0;
UINT8 AtCommSendingState = COMMSENDED; 

 volatile UINT16 bufloc = 0;	  // 
 volatile UINT16 bufloc_pre = 0;
 volatile char strbuf[300]; 
//static UINT32 OverTimes;
void uartpfnCallback(UINT8* u8Buf,UINT32 u32Len); 
void uartTOpfnCallback(UINT8* u8Buf,UINT32 u32Len); 
void  GprsSendComm(char *comm) ;


//******************************
//GPS数据定义
//
//***********************************
//发送给服务器的数据定义

volatile UINT16 gpsbufloc = 0;
volatile UINT8 gpsdatsbuf[200] = {0};
volatile UINT8 gpsdats[200] = {0};
volatile UINT8 GpsRecievedNew = 0;

void uart2pfnCallback( UINT8*  u8Buf,UINT32 u32Len);
void uart2TOpfnCallback(UINT8* u8Buf,UINT32 u32Len);



/*********************************************************************************************************************/
//Date And Author: 2017.8.3 wushengjun
//Function:	报警变量
//Name: FlagBaoJing
//
unsigned char BJWaiBuDianYuan = 0;
unsigned char BJBeiyongDianchiQianYa = 0;
unsigned char BJSIMCaDifferent = 0;
unsigned char BJGPSCaichu = 0;

unsigned char FlagBaoJing = 1;
unsigned char CCIDReadFromeSimCardFlag = 0;//CCID 读取标志
/*********************************************************************************************************************/

/*********************************************************************************************************************/
//Date And Author: 2017.8.3 wushengjun
//Function:	报警结构体
//Name: 
//
struct BaoJingValue
{
   unsigned char WaiBuDianYuanPowerOff;//外部电源断电报警
   unsigned char GPSCaiChuBaoJing;//GPS拆除报警
   unsigned char  BackBatteryPowerOff;//备用电池断电报警
   unsigned char SIMCardChangeAlarm; //SIM 更换报警
   unsigned char YiHuoCheckSum;	  //异或校验和
};
struct BaoJingValue BaoJingValueObj;

unsigned char SIMCARDBACHUFLAG = 0;
uint32_t FlashSaveCCIDAddress[5];
/*********************************************************************************************************************/

/*********************************************************************************************************************/
//Date And Author: 2017.8.3 wushengjun
//Function:	外部电源断电报警采集函数
//Name:  unsigned char WaiBuDianYuanPowerOffFun();
unsigned char WaiBuDianYuanPowerOffFun()
{
	//主电源断电报警
	if(voltage<=50)//5v
	{	
		return  1;	
	}
	else
	{
	  	return  0;	
	}
}
/*********************************************************************************************************************/

/*********************************************************************************************************************/
//Date And Author: 2017.8.3 wushengjun
//Function:	GPS拆除报警采集函数
//Name:  unsigned char GPSCaiChuBaoJingFun();
unsigned char GPSCaiChuBaoJingFun()
{
	
	return  0;//暂时不做	
}
/*********************************************************************************************************************/
/*********************************************************************************************************************/
//Date And Author: 2017.8.3 wushengjun
//Function:	备用电池断电报警采集函数
//Name:  unsigned char BackBatteryPowerOffFun();
unsigned char BackBatteryPowerOffFun()
{
	//备用电池电源断电报警
	// 备用电池欠压报警
	if((volt_bat<37))//3.7v 且备用电池存在情况下检测
	{
		return 1;//1;//测试用，实际情况是返回1
	}		
	else
	{
		return 0;
	}		
}
/*********************************************************************************************************************/

/*********************************************************************************************************************/
//Date And Author: 2017.8.3 wushengjun
//Function:	SIM 更换报警采集函数
//Name:  unsigned char SIMCardChangeAlarmFun();
unsigned char SIMCardChangeAlarmFun()
{
	if(SIMCARDBACHUFLAG ==1)
	{
		return 1;
	}
	else
	{
	  	return  0;	
	}
}
/*********************************************************************************************************************/

uint8_t  ConverIntToASC(uint8_t num,uint16_t dat,uint8_t *destadd)  //大端存储
{
    if((num > 3) || (num <1) )
    {
       return 0;
    }
    else
    {
        if( num == 3 )
        {   
          *destadd = (dat/100)+0x30;  
          *(destadd+1) = (dat%100/10)+0x30;  
          *(destadd+2) = (dat%10)+0x30; 
           return 1; 
        }
        else if( num == 2 ) 
        {
          *(destadd) = (dat%100/10)+0x30;  
          *(destadd+1) = (dat%10)+0x30; 
           return 1;  
        
        }
        else 
        {
          *(destadd) = (dat%10)+0x30;  
           return 1; 
        
        }    
    
    }

}

 void ConverStateBitToASC(uint16_t dat,uint8_t *destadd) 
{
	uint8_t i;    
  for(i=0;i<16;i++)
  {
	 if(dat&BIT(i))
    *(destadd+i) = 0x31; 
    else
     *(destadd+i) = 0x30; 				 
   }	 

}

void ConverJMsnumToASC(uint16_t dat,uint8_t *destadd) 
{
	uint8_t i;    
 	 //for(i=0;i<7;i++)
  // *(destadd+i) = 0x30; 
   *(destadd+7)=dat/1000+0x30;
   *(destadd+8)=dat%1000/100+0x30; 
   *(destadd+9)=dat%100/10+0x30; 
   *(destadd+10)=dat%10+0x30; 	 	 
	
}
void ConverJMdataToASC(uint16_t dat,uint8_t *destadd) 
{
 	 *(destadd)=dat/10000+0x30;
   *(destadd+1)=dat%10000/1000+0x30;
   *(destadd+2)=dat%1000/100+0x30; 
   *(destadd+3)=dat%100/10+0x30; 
   *(destadd+4)=dat%10+0x30; 	 	 
}

void  NETERROR_SEND(void)
{ unsigned char i;
 if(ECU_flag & BIT(Ecu_dm1))//显示多包故障
 { 
	 wdnum=DM1_PACK[0][0]|(DM1_PACK[0][1]<<8);
	if(wdnum>=6)
	{
	  spnnum=(wdnum-2)/4;
		 CarDataP->TroubleState[2]=spnnum;
		 NETSPNch++;
	   if(  NETSPNch<= spnnum)
		 {
		  pointdm3 =pointdm1+1+(NETSPNch-1)*4; 
      CarDataP->TroubleState[3]= *(pointdm3+1) ;
      CarDataP->TroubleState[4]= *(pointdm3+2) ;			 
      CarDataP->TroubleState[5] = *(pointdm3+3) ;	
		 }
		 if( NETSPNch== spnnum)
			   NETSPNch=0; 
  }
 }
 else
 {//显示单包
	 NETSPNch=0; 
	 err=CAN_Rxbuf[4][4]>>5;
   err=CAN_Rxbuf[4][3]|err<<8;
   err=CAN_Rxbuf[4][2]|err<<8;
   if(err)
	 CarDataP->TroubleState[2]=1;
	 else 
	 CarDataP->TroubleState[2]=0;
	 CarDataP->TroubleState[3]= CAN_Rxbuf[4][2] ;
   CarDataP->TroubleState[4]= CAN_Rxbuf[4][3] ;			 
   CarDataP->TroubleState[5] =CAN_Rxbuf[4][4] ;
	 
 }
}


void ConvertCanToCarDat(void)
{
  
    //车灯工作状态按协议记录
		 /*
      memset((char *)(&(CarDataP->MachineState))+2 , 0x40 + ((NewMachineFrame.u32indicateLed & 0xf000)>>12) ,1);
      memset((char *)(&(CarDataP->MachineState))+3 , 0x40 + ((NewMachineFrame.u32indicateLed & 0xf00)>>8) ,1);  
      memset((char *)(&(CarDataP->MachineState))+4 , 0x40 + ((NewMachineFrame.u32indicateLed & 0x0f)) ,1);  
      memset((char *)(&(CarDataP->MachineState))+5 , 0x40 + ((NewMachineFrame.u32indicateLed & 0xf0) >>4) ,1);*/
     
      memset((char *)(&(CarDataP->MachineState))+2 ,  (NewMachineFrame.u32indicateLed & 0xff) ,1);
      memset((char *)(&(CarDataP->MachineState))+3 ,  ((NewMachineFrame.u32indicateLed & 0xff00)>>8) ,1);  
      memset((char *)(&(CarDataP->MachineState))+4 ,  ((NewMachineFrame.u32indicateLed & 0xff0000)>>16) ,1);  
      memset((char *)(&(CarDataP->MachineState))+5 ,  ((NewMachineFrame.u32indicateLed & 0xff000000)>>24) ,1);  
      
       NETERROR_SEND();		 
  
      if(NewMachineFrame.u16engineRotate > 3600){ NewMachineFrame.u16engineRotate = 3600;}
      if(NewMachineFrame.u8oilPressure > 100 ){NewMachineFrame.u8oilPressure = 100;}
      if(NewMachineFrame.u8waterTemperature > 120){ NewMachineFrame.u8waterTemperature = 120 ;}
      //if(NewMachineFrame.u8fuelVol > 80 ){ NewMachineFrame.u8fuelVol = 80; }
	  if(NewMachineFrame.u8fuelVol > 8 ){ NewMachineFrame.u8fuelVol = 8; }
      if(NewMachineFrame.u32hours > 999999){ NewMachineFrame.u32hours = 999999;}
   
      ConverIntToASC(1, (uint16_t)(NewMachineFrame.u16engineRotate%10000/1000),(uint8_t *)&(CarDataP->RotationSpeed)+2);
      ConverIntToASC(3, (uint16_t)(NewMachineFrame.u16engineRotate%1000),(uint8_t *)&(CarDataP->RotationSpeed)+2+1);
      ConverIntToASC(3, NewMachineFrame.u8oilPressure, (uint8_t *)&(CarDataP->OilPressur) + 2);
      ConverIntToASC(3, NewMachineFrame.u8waterTemperature, (uint8_t *)&(CarDataP->WaterTemperature) + 2);
      ConverIntToASC(3, NewMachineFrame.u8fuelVol, (uint8_t *)&(CarDataP->FuelVolme)+2 );
      ConverIntToASC(3, (uint16_t)((NewMachineFrame.u32hours%1000000)/1000), (uint8_t *)&(CarDataP->RotationHour)+2);
      ConverIntToASC(3, (uint16_t)(NewMachineFrame.u32hours % 1000), (uint8_t *)&(CarDataP->RotationHour)+2+3);
  
     //接收控制器CAN通信传说电压数据
      if(NewMachineFrame.u16voltage > 360){ NewMachineFrame.u16voltage  = 360;}
       ConverIntToASC(3, NewMachineFrame.u16voltage, (uint8_t *)&(CarDataP->BatteryVoltage)+2); 
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
	 UART0->FCR |= (3ul<<UART_FCR_RFITL_Pos);
	 //Set Rx timeout count
	 UART_SetTimeoutCnt(UART0, 200);
	 //enable Rx timeout counter
	 UART0->IER |= 	UART_IER_TIME_OUT_EN_Msk;

	 /* Enable Interrupt  */
    UART_EnableInt(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_TOUT_IEN_Msk));
	UART_DisableInt(UART0,  UART_IER_THRE_IEN_Msk );
	_sys_uUartTxHead = _sys_uUartTxTail = NULL;
	 memset((void *)strbuf,0,300);
}
 
static void _PutChar_f(uint8_t ucCh)
{ 
	while(sysTxBufWriteNextOne() == _sys_uUartTxHead) ;	// buffer full		
	_sys_ucUartTxBuf[_sys_uUartTxTail] = ucCh;
	_sys_uUartTxTail = sysTxBufWriteNextOne();  
	if( (UART0->IER & UART_IER_THRE_IEN_Msk)== 0)
	UART_EnableInt(UART0,  UART_IER_THRE_IEN_Msk );

}

static void sysPrintf(char *pcStr)
{
   	while (*pcStr)
	{           
	 	_PutChar_f(*pcStr++);
	}

}

void  GprsSendComm(char *comm) 
{
    sysPrintf(comm); 
}
void  GprsSendhex(char *comm) 
{
	unsigned char i;
    //sysPrintf(comm);
	for(i=0;i<7;i++)
	 	_PutChar_f(*comm++);
}
/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART02_IRQHandler(void)
{
   volatile uint8_t u8InChar = 0xFF,i;
   volatile uint8_t u32Count = 0,	 u32Count2 = 0;
   volatile uint32_t u32IntSts = UART0->ISR;
    volatile uint32_t u32IntSts2 = UART2->ISR;
    volatile uint32_t reg_temp1,reg_temp2;
   	reg_temp1 =  UART0->IER ;
//	(PF5 = 0);
   //*****************************
   //  for m660  send and receive
   //********************************
   u32IntSts = 	UART0->ISR;
   if(u32IntSts & UART_ISR_RDA_INT_Msk)		 //UART_FSR_RX_POINTER_Msk	UART0 GPRS接收中断
   {	 //(PF5 = 0);
       
       u32Count = ((UART0->FSR & UART_FSR_RX_POINTER_Msk)>>UART_FSR_RX_POINTER_Pos);  
	   	for(i=0;i<u32Count;i++)		
		{	
			if ( rx_cnt == RX_ARRAY_NUM ) 
			{
				rx_cnt = 0;
			}			
			u8InChar = uart_rx[rx_cnt] = UART_READ(UART0);			
			rx_cnt++;					
		}
		uartpfnCallback(&(uart_rx[0]), u32Count);
		rx_cnt = 0;	
   }  

   if(u32IntSts & UART_ISR_TOUT_IF_Msk)	 //UART0 GPRS发送中断
   {		 	
       	//  (PF5 = 0);
        u32Count = ((UART0->FSR & UART_FSR_RX_POINTER_Msk)>>UART_FSR_RX_POINTER_Pos);
		for(i=0;i<u32Count;i++)		
		{	
			if ( rx_cnt == RX_ARRAY_NUM ) 
			{
				rx_cnt = 0;
			}			
			uart_rx[rx_cnt] = UART_READ(UART0);		
			rx_cnt++;			
		}
		uartTOpfnCallback(&(uart_rx[0]), u32Count);
		rx_cnt = 0;	
		// (PF5 = 0);		
   }
 
/*	
   if( (strstr((char *)strbuf,"SEND OK")!= NULL) ) 
   {
   	   Gprs_Timeout_Count = 1000;	 GprsConnectTcpState=0;
	   memset((void *)strbuf,0,300); bufloc=0;
	   at_command_order = 0;	
			   AtCommSendingState = COMMSENDED;	
   }
*/
   
   if( reg_temp1 & 	UART_IER_THRE_IEN_Msk) 
   {		//   (PF5 = 0); 
	   if(u32IntSts & UART_ISR_THRE_INT_Msk)
	   {	  		 	
		   	if (_sys_uUartTxHead == _sys_uUartTxTail) 
			{									 
				//Disable interrupt if no any request!  
				UART_DisableInt(UART0,  UART_IER_THRE_IEN_Msk );
			} 
			else
			{
				//Transmit data 
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
   //********************************
   //FOR GPS receive
   //*******************************
   u32IntSts2 = 	UART2->ISR;	  
   reg_temp2 =  UART2->IER ;	
  // if(reg_temp2 & 	UART_IER_RDA_IEN_Msk) 	(PF5 = 0); 	 else 	  (PF5 = 1);
   if(u32IntSts2 & UART_ISR_RDA_INT_Msk)		 //UART_FSR_RX_POINTER_Msk	UART_ISR_RDA_INT_Msk
   {	// (PF5 = 1);	 
       u32Count2 = ((UART2->FSR & UART_FSR_RX_POINTER_Msk)>>UART_FSR_RX_POINTER_Pos);  
	   	for(i=0;i<u32Count2;i++)		
		{	
			if ( rx_cnt2 == RX_ARRAY_NUM ) 
			{
				rx_cnt2 = 0;
			}			
			u8InChar = uart_rx2[rx_cnt2] = UART_READ(UART2);			
			rx_cnt2++;					
		}
		uart2pfnCallback(&(uart_rx2[0]), u32Count2);	
		rx_cnt2 = 0;

   }  

   if(u32IntSts2 & UART_ISR_TOUT_IF_Msk)
   {		 	
        u32Count2 = ((UART2->FSR & UART_FSR_RX_POINTER_Msk)>>UART_FSR_RX_POINTER_Pos);
		for(i=0;i<u32Count2;i++)		
		{	
			if ( rx_cnt2 == RX_ARRAY_NUM ) 
			{
				rx_cnt2 = 0;
			}			
			uart_rx2[rx_cnt2] = UART_READ(UART2);		
			rx_cnt2++;			
		}
		uart2TOpfnCallback(&(uart_rx2[0]), u32Count2);
		rx_cnt2 = 0;	
		// (PF5 = 0);		
   }  
}




void Delay_1ms(void)
{
	unsigned int i;
	for(i=0;i<1142;i++)
		;
}

void Delay_Ms(unsigned int xms)
{
	unsigned int i;
	for(i=0;i<xms;i++)
		Delay_1ms();
}


/*
void Delay_Ms(UINT32 Tcount)
{
   UINT32 i;
   while(Tcount--)
   {
       for(i=0; i<420; i++) ;  //420  
   }

} 
*/


PUINT8 FrameLengthToASC(struct CarData NewCarData)
{
   UINT8  totallength;
   struct CarData *  datap  = &NewCarData;

  /*
   totallength = strlen((char *)(datap->Head)) + strlen((char *)(datap->SimID)) + strlen((char *)(datap->GpsData))+1 +strlen((char *)(datap->OilPressur)) +strlen((char *)(datap->WaterTemperature));
   totallength += strlen((char *)(datap->FuelVolme)) + strlen((char *)(datap->BatteryVoltage)) +strlen((char *)(datap->RotationHour));
   totallength += strlen((char *)(datap->RotationSpeed)) + strlen((char *)(datap->MachineState))+strlen((char *)(datap->TroubleState)) +1;// + strlen((char *)(datap->VerifyCode)) 
   //totallength  =  124;*/
	
   totallength = strlen((char *)(datap->Head)) + strlen((char *)(datap->SimID)) + strlen((char *)(datap->GpsData))+1 +strlen((char *)(datap->OilPressur)) +strlen((char *)(datap->WaterTemperature));
   totallength += strlen((char *)(datap->FuelVolme)) + strlen((char *)(datap->BatteryVoltage)) +strlen((char *)(datap->RotationHour));
   totallength += strlen((char *)(datap->RotationSpeed)) + 7+7 +1;// + strlen((char *)(datap->VerifyCode))
	
   LengthAsc[0]  = totallength%1000/100 + 0x30;
   LengthAsc[1]  = totallength%100/10 + 0x30;
   LengthAsc[2]  = totallength%10 + 0x30;
   LengthAsc[3]  = '\r';
   LengthAsc[4]  = 0;
   
   return LengthAsc;
}



UINT8 GprsConnectTcp(void)
{    
      unsigned char i;
      char *pdata = NULL;
	  
   if(at_command_order  < sizeof(at_command))
   { 
   	
      if(AtCommSendingState == COMMSENDED)
      {	  
           memset((void *)strbuf,0,300); bufloc=0;  
         GprsSendComm((char *)at_command[at_command_order]);
	
         AtCommSendingState = COMMSENDING;	
         //Delay_Ms(500);
         Gprs_Timeout_Count = 0;	
      } 
      if(at_command_order != 3) 
      {
	     if(strstr((char *)strbuf,at_command_reply[at_command_order]) == NULL )
	     {	  
		     
		    if(Gprs_Timeout_Count >= 1000)      //在发送建立TCP连接时，需要时间更长（几秒），现在是在这几秒内由于超时会发送AT命令，到建立几次TCP连接后，TCP命令回码就不会超时
		    {			//	   GprsSendComm((char *)strbuf); 
		       Gprs_Timeout_Count = 0;			     			   
		       memset((void *)strbuf,0,300); bufloc=0;
		       Delay_Ms(10);
			   at_command_order = 0;	
			   AtCommSendingState = COMMSENDED;	
			   //havegetccid = 0;	   
			    return 0;		 //超时
		    }
		   	 return 1;			//正在连接
        }
	    else
	    {//	 (PF5 = 1);       
		   Delay_Ms(1); 
		   AtCommSendingState = COMMSENDED;
		   //#ifdef CCIDASSIM
		   if(at_command_order == 7) 
		   {
		   	  // +CCID: 898602b3161630012255
			  if( (pdata = strstr((void *)strbuf,"AT+CCID")) != NULL)
			  {
			      for( i=0;i<20;pdata++,i++)
				  {
				  	CCID[i] = *(pdata + 10);				  
				  }	 
				  //memcpy((char *)(&(CarDataP->SimID))+2, &CCID[9],11);
				   i = 0;
				  //读Flash
				   havegetccid = EEPROMread_ccidFlag();
				   EEPROMread_CCIDNumber(FlashSaveCCIDAddress);
				   
				  
				  if(havegetccid == 0)
				  {				  	 
					 //把havegetccid  = 1，写入Flash
					 havegetccid = 0x00000001;
					 EEPROMwrite_ccidFlag(havegetccid);

					 //把CCID 号写入 Flash ;
					 memcpy(FlashSaveCCIDAddress,CCID,20);
					 //FlashSaveCCIDAddress = 0x55667788;					 
					 EEPROMwrite_CCIDNumber(FlashSaveCCIDAddress);	 
				  }
				  else
				  {
				  	  //比较Flash 的 CCID号 是否和现在度出的CCID号一致，一致的话不做处理，不一致的话 把SIM卡把出标志置为一
					  //1.度Flash的CCID号，
					  if(memcmp(FlashSaveCCIDAddress,CCID,20) == 0)//
					  {
					  	  SIMCARDBACHUFLAG = 0;
					  }			   
					  else
					  {
					  	  SIMCARDBACHUFLAG = 1;//SIM卡拔出标志置为一
					  }

				  }	
				  		  
			  }	
			    		   
		   }
		   //#endif

		   memset((void *)strbuf,0,300); bufloc=0;
		   Delay_Ms(10);
	       at_command_order++;
	       if(at_command_order >= 9)//指令能执行完说明能连接上
	       { 
	   	      AtCommSendingState = COMMSENDED;
	   	      gprsNoConnect = 0;
			  GprsdataReady = 1;
			  //sysPrintf("connecting.....wait one moment!!!!!!\r\n");
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
	    
	   	 if( (strstr((char *)strbuf,at_command_reply[at_command_order]) == NULL) &&  (strstr((char *)strbuf,"+CREG: 0,2\r\n\r\nOK\r\n") == NULL) )
	     {	  
		    if(Gprs_Timeout_Count >= 1000)
		    {
		       Gprs_Timeout_Count = 0;
		       memset((void *)strbuf,0,300); bufloc=0;
			   at_command_order = 0;	
			   AtCommSendingState = COMMSENDED;
			   return 0;	  
		    }
		    return 1;	 //正在连接
         }
	     else
	    {	      
		   Delay_Ms(1); 
		   AtCommSendingState = COMMSENDED;
		   memset((void *)strbuf,0,300); bufloc=0;
	       at_command_order++;
	       if(at_command_order >= 9)
	       //if(at_command_order >= 5)
		   { 
	   	      AtCommSendingState = COMMSENDED;
	   	      gprsNoConnect = 0;
	   	      Gprs_Timeout_Count = 0;
			  return 2;
		   } 
		   else
		   {
		      Gprs_Timeout_Count = 0;
		      return 1;		   	//正在连接
		   }
	    }
	   
	 }
 }
 else
 {
    memset((void *)strbuf,0,300); bufloc=0;
	at_command_order = 0;	
    return 0;	
   
 }
	  
 

}


int GprsDataSum(struct CarData NewCarData)
{
    struct CarData *  datap  = &NewCarData;
    char *p;
    UINT8 sum=0,i;
     NewCarData.VerifyCode[0] = 0;
     p = (char *)datap;
    if(*p != 0x5A)
    {
       return -1;    
    }
    else
    {  
       p++;
       for(i=0 ; i <10;i++)
       {
          while( *p !=  NULL)
          {
             if(*p != ';')
             sum += *p;
             p++;
          }
       }
    
    }
    return sum-0x5a;


}


UINT8 GprsSendData(struct CarData NewCarData)
{
    static char CommSended = 0;
    static char DataSended = 0;
    struct  CarData *  datap  = &NewCarData;
    if( DataSended == 0 )   //数据未发送完
    {
       if(CommSended)  //命令已发送
       {
          if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) ) 
		  //if( (strstr((char *)strbuf,at_command_reply[4]) == NULL) )
          {           
             if(Gprs_Timeout_Count >= 500)
		     {
		        Gprs_Timeout_Count = 0;
		        memset((void *)strbuf,0,300); bufloc=0;
			    at_command_order = 0;	
			    AtCommSendingState = COMMSENDED;
			    DataSended = 0;
                CommSended = 0; 
                return 0;	
                	    	  
		     }  
             return 3;
          }
          else
          {
             //Delay_Ms(1);
             GprsSendComm( (char *)(&(datap->Head)) );
             GprsSendComm( (char *)(&(datap->SimID)) );
			 //GprsSendComm("1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70");//,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100
			 #if 1
             GprsSendComm( (char *)(&(datap->GpsData)) );
             GprsSendComm(";");
             GprsSendComm( (char *)(&(datap->OilPressur)) );
             GprsSendComm( (char *)(&(datap->WaterTemperature)) );
             GprsSendComm( (char *)(&(datap->FuelVolme)) );
             GprsSendComm( (char *)(&(datap->BatteryVoltage)) );
             GprsSendComm( (char *)(&(datap->RotationHour)) );
             GprsSendComm( (char *)(&(datap->RotationSpeed)) );
             
//			 GprsSendComm( (char *)(&(datap->MachineState)) );
//			 GprsSendComm( (char *)(&(datap->TroubleState)) );			
			GprsSendhex( (char *)(&(datap->MachineState)) );
			GprsSendhex( (char *)(&(datap->TroubleState)) );
			#endif
						
             //NewCarData.VerifyCode[0]=GprsDataSum(NewCarData);
            // GprsSendComm( (char *)(&(datap->VerifyCode)) );
             GprsSendComm("\r");

             DataSended  = 1;    
             Gprs_Timeout_Count = 0;        
              return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_command[9]);
         GprsSendComm((char *)FrameLengthToASC(NewCarData));
		 //GprsSendComm("138");
		  //GprsSendComm("\r");
         Gprs_Timeout_Count = 0; 
         CommSended = 1;
          return 3; 
      }
   }
   else    //数据发送完
   {
      
	  //if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) )  //未收到发送完回码
      if( (strstr((char *)strbuf,"$GNRMC") == NULL) )  //未收到发送完回码	  "SEND OK"
	  {
          if(Gprs_Timeout_Count >= 1000)
		  {
		      Gprs_Timeout_Count = 0;
		      memset((void *)strbuf,0,300); bufloc=0;
			  at_command_order = 0;	
			  AtCommSendingState = COMMSENDED;	
			  DataSended = 0;
              CommSended = 0; 
              return 0;		    
		  }  
          return 3;	
       }
       else
       {
          memset((void *)strbuf,0,300); bufloc=0;
          DataSended = 0;
          CommSended = 0; 
          gprsNoConnect = 0;
          Gprs_Timeout_Count = 0;
          return 4;
       }   
    }
      
}


UINT8 GprsSendRp(struct PowerOffComm NewPowerOffComm)
{
    static char CommSended = 0;
    static char DataSended = 0;
    struct PowerOffComm *  datap  = &NewPowerOffComm;
    if( DataSended == 0 )   //数据未发送完
    {
       if(CommSended)  //命令已发送
       {
          if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) )
		  //if( (strstr((char *)strbuf,at_command_reply[4]) == NULL) )  
          {           
             if(Gprs_Timeout_Count >= 500)
		     {
		        Gprs_Timeout_Count = 0;
		        memset((void *)strbuf,0,300); bufloc=0;
			    at_command_order = 0;	
			    AtCommSendingState = COMMSENDED;
			    DataSended = 0;
                CommSended = 0; 
                return 0;
                	    	  
		     }  
             return 3;
          }
          else
          {
             Delay_Ms(1);
             GprsSendComm( (char *)datap );
                GprsSendComm("\r");
             DataSended  = 1;    
             Gprs_Timeout_Count = 0;        
              return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_command[9]);
		 //GprsSendComm((char *)at_command[5]);
		     LengthAsc[0]  = 32%1000/100 + 0x30;
         LengthAsc[1]  = 32%100/10 + 0x30;
         LengthAsc[2]  = 32%10 + 0x30;
         LengthAsc[3]  = '\r';
         LengthAsc[4]  = 0;	   
         GprsSendComm((char *)LengthAsc);
         Gprs_Timeout_Count = 0; 
         CommSended = 1;
          return 3; 
      }
   }
   else    //数据发送完
   {
      if( (strstr((char *)strbuf,at_command_reply[10]) == NULL) )  //未收到发送完回码
      {
          if(Gprs_Timeout_Count >= 1000)
		  {
		      Gprs_Timeout_Count = 0;
		      memset((void *)strbuf,0,300); bufloc=0;
			  at_command_order = 0;	
			  AtCommSendingState = COMMSENDED;	
			  DataSended = 0;
              CommSended = 0; 
              return 0;		    
		  }  
          return 3;	
       }
       else
       {
          memset((void *)strbuf,0,300); bufloc=0;
          DataSended = 0;
          CommSended = 0; 
          gprsNoConnect = 0;
          Gprs_Timeout_Count = 0;
          return 4;
       }   
    }
      
}

UINT8 GprsSendJMRp(struct JMStateRp NewPowerOffComm)
{
    static char CommSended = 0;
    static char DataSended = 0;
    struct  JMStateRp *  datap  = &NewPowerOffComm;
    if( DataSended == 0 )   //数据未发送完
    {
       if(CommSended)  //命令已发送
       {
          if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) )
		  //if( (strstr((char *)strbuf,at_command_reply[4]) == NULL) )   
          {           
             if(Gprs_Timeout_Count >= 500)
		     {
		        Gprs_Timeout_Count = 0;
		        memset((void *)strbuf,0,300); bufloc=0;
			    at_command_order = 0;	
			    AtCommSendingState = COMMSENDED;
			    DataSended = 0;
                CommSended = 0; 
                return 0;
                	    	  
		     }  
             return 3;
          }
          else
          {
             Delay_Ms(1);
             GprsSendComm( (char *)datap );
                GprsSendComm("\r");
             DataSended  = 1;    
             Gprs_Timeout_Count = 0;        
              return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_command[9]);
		 //GprsSendComm((char *)at_command[5]);
		     LengthAsc[0]  = 38%1000/100 + 0x30;
         LengthAsc[1]  = 38%100/10 + 0x30;
         LengthAsc[2]  = 38%10 + 0x30;
         LengthAsc[3]  = '\r';
         LengthAsc[4]  = 0;	   
         GprsSendComm((char *)LengthAsc);
         Gprs_Timeout_Count = 0; 
         CommSended = 1;
          return 3; 
      }
   }
   else    //数据发送完
   {
      if( (strstr((char *)strbuf,at_command_reply[10]) == NULL) )  //未收到发送完回码
      {
          if(Gprs_Timeout_Count >= 1000)
		  {
		      Gprs_Timeout_Count = 0;
		      memset((void *)strbuf,0,300); bufloc=0;
			  at_command_order = 0;	
			  AtCommSendingState = COMMSENDED;	
			  DataSended = 0;
              CommSended = 0; 
              return 0;		    
		  }  
          return 3;	
       }
       else
       {
          memset((void *)strbuf,0,300); bufloc=0;
          DataSended = 0;
          CommSended = 0; 
          gprsNoConnect = 0;
          Gprs_Timeout_Count = 0;
          return 4;
       }   
    }
      
}


 UINT8 GprsSendControlState(struct ControlStateRp NewControlStateRp)
{
    static char CommSended = 0;
    static char DataSended = 0;
    struct PowerOffComm *  datap  = &NewPowerOffComm;
    if( DataSended == 0 )   //数据未发送完
    {
       if(CommSended)  //命令已发送
       {
          if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) ) 
          //if( (strstr((char *)strbuf,at_command_reply[4]) == NULL) ) 
		  {           
             if(Gprs_Timeout_Count >= 500)
		     {
		        Gprs_Timeout_Count = 0;
		        memset((void *)strbuf,0,300); bufloc=0;
			    at_command_order = 0;	
			    AtCommSendingState = COMMSENDED;
			    DataSended = 0;
                CommSended = 0; 
                return 0;
                	    	  
		     }  
             return 3;
          }
          else
          {
             Delay_Ms(1);
             GprsSendComm( (char *)datap );
                GprsSendComm("\r");
             DataSended  = 1;    
             Gprs_Timeout_Count = 0;        
              return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_command[9]);
		 //GprsSendComm((char *)at_command[5]);
		     LengthAsc[0]  = 48%1000/100 + 0x30;
         LengthAsc[1]  = 48%100/10 + 0x30;
         LengthAsc[2]  = 48%10 + 0x30;
         LengthAsc[3]  = '\r';
         LengthAsc[4]  = 0;	   
         GprsSendComm((char *)LengthAsc);
         Gprs_Timeout_Count = 0; 
         CommSended = 1;
          return 3; 
      }
   }
   else    //数据发送完
   {
      if( (strstr((char *)strbuf,at_command_reply[10]) == NULL) )  //未收到发送完回码
      {
          if(Gprs_Timeout_Count >= 1000)
		  {
		      Gprs_Timeout_Count = 0;
		      memset((void *)strbuf,0,300); bufloc=0;
			  at_command_order = 0;	
			  AtCommSendingState = COMMSENDED;	
			  DataSended = 0;
              CommSended = 0; 
              return 0;		    
		  }  
          return 3;	
       }
       else
       {
          memset((void *)strbuf,0,300); bufloc=0;
          DataSended = 0;
          CommSended = 0; 
          gprsNoConnect = 0;
          Gprs_Timeout_Count = 0;
          return 4;
       }   
    }
      
}


void uartpfnCallback(UINT8* u8Buf,UINT32 u32Len)
{
     UINT8  j;  
      if(bufloc >= 300) {memset((void *)strbuf,0,300);bufloc_pre=bufloc =0;}   
    for(j = 0;j<u32Len;j++)
    {    
       *(strbuf+bufloc)  = *(u8Buf+j);
      // *(u8Buf+j) = 0;  
        bufloc++;
    }  
    
}


void uartTOpfnCallback(UINT8* u8Buf,UINT32 u32Len)
{
     UINT8  j;  
     if(bufloc >= 300) {memset((void *)strbuf,0,300);bufloc_pre=bufloc =0;}    
    for(j = 0;j<u32Len;j++)
    {       
       *(strbuf+bufloc)  = *(u8Buf+j);        
      //  *(u8Buf+j) = 0;   
       bufloc++;
    }
}


void rev_judge(void)
{
	 volatile char *pStr = NULL;
	/*
	  if(IsReceivePowOff == 0)
	  {
		  if((strstr((char *)strbuf, (char *)(&(NewPowerOffComm.Comm)))!= NULL))	//  CommID 
		  {																					
		     IsReceivePowOff = 1;  // PF5 =  0;  
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			 memcpy((char *)(&(RpPowerOffComm.SN)) , (void *)(pStr+2),12);
			 memcpy((char *)(&(RpPowerOffwat.SN)) , (void *)(pStr+2),12);
		     memset((void *)strbuf,0,300); bufloc=0;
		  }
	  }
	  if(IsReceivePowOn == 0)
	  {
		  if((strstr((char *)strbuf,(char *)(&(NewPowerOnComm.Comm)) )!= NULL))  //  CommID
		  {
		  	 IsReceivePowOn = 1;  // PF5 =  0; 
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			 memcpy((char *)(&(RpPowerOnComm.SN)) , (void *)(pStr+2),12);
		     memset((void *)strbuf,0,300); bufloc=0;
		  
		  }	
	  }	  
   */
		
		if(IsBoundComm == 0)
	  {
		  if((strstr((char *)strbuf, (char *)(&(NewBoundComm.Comm)))!= NULL))	
		  {																					
		     IsBoundComm = 1;  
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			 memcpy((char *)(&(NewControlStateRp.SN)) , (void *)(pStr+2),12);
		   memset((void *)strbuf,0,300); bufloc=0;
		  }
	  }
	  if(IsUnBoundComm == 0)
	  {
		  if((strstr((char *)strbuf,(char *)(&(NewUnBoundComm.Comm)) )!= NULL)) 
		  {
		  	 IsUnBoundComm  = 1;   
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			  memcpy((char *)(&(NewControlStateRp.SN)) , (void *)(pStr+2),12);
		    memset((void *)strbuf,0,300); bufloc=0;
		  }	
	  }	  
	  
		if(IsHaltComm == 0)
	  {
		  if((strstr((char *)strbuf, (char *)(&(NewHaltComm.Comm)))!= NULL))	
		  {																					
		   IsHaltComm = 1;    
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			 memcpy((char *)(&(NewControlStateRp.SN)) , (void *)(pStr+2),12);
		   memset((void *)strbuf,0,300); bufloc=0;
		  }
	  }
	  if(IsUnLimitComm == 0)
	  {
		  if((strstr((char *)strbuf,(char *)(&(NewUnLimitComm.Comm)) )!= NULL)) 
		  {
		  	 IsUnLimitComm  = 1; 
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			  memcpy((char *)(&(NewControlStateRp.SN)) , (void *)(pStr+2),12);
		    memset((void *)strbuf,0,300); bufloc=0;
		  }	
	  }	  
	  
		if(IsInquireComm == 0)
	  {
		  if((strstr((char *)strbuf, (char *)(&(NewInquireComm.Comm)))!= NULL))	
		  {																					
		     IsInquireComm = 1;  
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			 memcpy((char *)(&(NewControlStateRp.SN)) , (void *)(pStr+2),12);
		   memset((void *)strbuf,0,300); bufloc=0;
		  }
	  }
	  if(IsLimitParameterComm == 0)
	  {
		 if ((pStr = strstr((char *)strbuf,(char *)(&(NewLimitParameterComm.Comm))))!= NULL)
		  {
		  	IsLimitParameterComm = 1; 
				memcpy((char *)(&(NewLimitParameterComm.Comm)) , (void *)(pStr+3),4);
				memcpy((char *)(&(NewLimitParameterComm.Comm)) , (void *)(pStr+7),3);

			 if((pStr = strstr((char *)strbuf,"Z;" )) != NULL )
			  memcpy((char *)(&(NewControlStateRp.SN)) , (void *)(pStr+2),12);
		    memset((void *)strbuf,0,300); bufloc=0;
		  }	
	  }	  
	  
		
		
		
		
		
}



//***************************************************************************//
//内容：以下为GPS功能调用，GPS使用UART2，与UART0(GPRS)共用中断向量
//***************************************************************************

#if 0
void UART2_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 module */
    SYS_ResetModule(UART2_RST);

    /* Configure UART0 and set UART0 Baudrate */
    //UART_Open(UART2, 9600);
	UART_Open(UART2, 115200);

	  /* Set UART Rx and RTS trigger level */
	 UART2->FCR |= (3ul<<UART_FCR_RFITL_Pos);	 //14bytes
	 //Set Rx timeout count
	 UART_SetTimeoutCnt(UART2, 200);
	 //enable Rx timeout counter
	 UART2->IER |= 	UART_IER_TIME_OUT_EN_Msk;

	 /* Enable Interrupt  */
   UART_EnableInt(UART2, (UART_IER_RDA_IEN_Msk | UART_IER_TOUT_IEN_Msk));
	UART_DisableInt(UART2,  UART_IER_THRE_IEN_Msk );
	 memset((void *)gpsdatsbuf,0,200);
	 memset((void *)gpsdats,0,200);
	 GprsSendComm(" UART2 set ok");
}
#endif
//wushengjun 2017.8.2
void UART2_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 module */
    SYS_ResetModule(UART2_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART2, 115200);

	UART_EnableInt(UART2, UART_IER_RDA_IEN_Msk);
}

//

UINT8 AscToHex(char ASC)
{ 
   if((ASC >= '0') && (ASC <= '9') )
   {
      return ASC-0x30;
   
   }
   else if((ASC >= 'A') && (ASC <= 'F'))
   {
      return ASC-0x41+0x0A;
   
   }
   else
   {
      return 0xff;  
   }
}


UINT8 VerifyGps(UINT8* GpsDatap)
{
   UINT8 sum=0;
   UINT8  verifycode[3];
   //UINT8  sumasc[4];
   if(*GpsDatap != '$')
   {
      return 0;
   }
   else
   {     
      GpsDatap++;    
      while(*GpsDatap != '*')
      {      
         {
            sum ^= (*GpsDatap);
         }
         GpsDatap++;   
      } 
      verifycode[0] =  *(++GpsDatap);//
      verifycode[1] =  *(++GpsDatap);//  
     //  sumasc[0]  = sum%1000/100+0x30;
      //  sumasc[1]  = sum%100/10+0x30;
      //  sumasc[2]  = sum%10+0x30;
       //  sysPrintf((char *)sumasc); 
       
      if( sum == (AscToHex(verifycode[0]))*16 + AscToHex(verifycode[1]) )
      {
         return 1;
      }
      else
      {
         return 0;
      }
   }
}




void uart2pfnCallback( UINT8* u8Buf,UINT32 u32Len)
{
     volatile UINT8  j,i;

    for(j = 0;j<u32Len;j++)
    {       
        if(gpsbufloc >= 199)gpsbufloc=0;
       gpsdatsbuf[gpsbufloc]  = *(u8Buf+j);
	   
	   	   if(gpsdatsbuf[gpsbufloc] == '$')
		   {
		      for(i = 0;i<200;i++)
              {
                 gpsdatsbuf[i]  = 0;
              }
		   	  gpsbufloc = 0;
			  gpsdatsbuf[gpsbufloc] = '$';		   
		   }
		   else
		   {
		      if(gpsdatsbuf[gpsbufloc] == '\n') 
			  {
		          if(gpsdatsbuf[0] == '$')
				  {
						/*
				     if(strstr((void *)(gpsdatsbuf) , "$GPGGA") != NULL)
		             {
		                if(GpsRecievedNew == 0)
		                {
		                   memcpy((void *)(gpsdats),(void *)(gpsdatsbuf),76);//加回车 换行共76
		                   GpsRecievedNew = 1;    
		                }          
		             } 
							*/
                 						
				     //if(strstr((void *)(gpsdatsbuf) , "$GPRMC") != NULL)
				     if(strstr((void *)(gpsdatsbuf) , "$GNRMC") != NULL)
		             {
		                if(GpsRecievedNew == 0)
		                {
		                   memcpy((void *)(gpsdats),(void *)(gpsdatsbuf),68);//加回车 换行共76
		                   GpsRecievedNew = 1;    
		                }          
		             } 
								 		 
								 
					 else if(strstr((void *)(gpsdatsbuf) , "$GPVTG") != NULL)
					 {
					 
		
					 }
					 else
					 {
					 	 for(i = 0;i<200;i++)
		                 {
		                    gpsdatsbuf[i]  = 0;
		                 }
			   	         gpsbufloc = 0;	 				 
					 }
				 }	
				 else
				 {
				 	 for(i = 0;i<200;i++)
                     {
                        gpsdatsbuf[i]  = 0;
                     }
		   	         gpsbufloc = 0;	
				 }		  
			  }
		   }
		   gpsbufloc++;
		   
		             
//       if(gpsdatsbuf[gpsbufloc] == '\n') 
//       {     	 
            
//          if(strstr((void *)(gpsdatsbuf) , "$GPGGA") != NULL)
//          {
//             if(GpsRecievedNew == 0)
//             {
//                memcpy((void *)(gpsdats),(void *)(gpsdatsbuf),76);
//                GpsRecievedNew = 1;    
//             }          
//          }
//    
//         for(i = 0;i<200;i++)
//         {
//            gpsdatsbuf[i]  = 0;
//         }
//         gpsbufloc =0;
//               
//       }    
//       else
//       {
//          gpsbufloc++;       
//       }
    }

   
}


void uart2TOpfnCallback(UINT8* u8Buf,UINT32 u32Len)
{
    UINT8  j,i; 
	//	PF5 =0;
    for(j = 0;j<u32Len;j++)
    {        
	      if(gpsbufloc >= 199)gpsbufloc=0;
	      gpsdatsbuf[gpsbufloc]  = *(u8Buf+j); 

		   if(gpsdatsbuf[gpsbufloc] == '$')
		   {
		      for(i = 0;i<200;i++)
              {
                 gpsdatsbuf[i]  = 0;
              }
		   	  gpsbufloc = 0;
			  gpsdatsbuf[gpsbufloc] = '$';		   
		   }
		   else
		   {
		      if(gpsdatsbuf[gpsbufloc] == '\n') 
			  {
		          if(gpsdatsbuf[0] == '$')
				  {
				     //if(strstr((void *)(gpsdatsbuf) , "$GPGGA") != NULL)
					 if(strstr((void *)(gpsdatsbuf) , "$GNGGA") != NULL)
		             {
		                if(GpsRecievedNew == 0)
		                {
		                   memcpy((void *)(gpsdats),(void *)(gpsdatsbuf),76);
		                   GpsRecievedNew = 1;    
		                }          
		             } 
					 //else if(strstr((void *)(gpsdatsbuf) , "$GPRMC") != NULL)
					 else if(strstr((void *)(gpsdatsbuf) , "$GNRMC") != NULL)
					 {
					 
		
					 }
					 else
					 {
					 	 for(i = 0;i<200;i++)
		                 {
		                    gpsdatsbuf[i]  = 0;
		                 }
			   	         gpsbufloc = 0;	 				 
					 }
				 }	
				 else
				 {
				 	 for(i = 0;i<200;i++)
                     {
                        gpsdatsbuf[i]  = 0;
                     }
		   	         gpsbufloc = 0;	
				 }		  
			  }
		   }
		   gpsbufloc++;
	       
//	      if(gpsdatsbuf[gpsbufloc] == '\n') 
//	      {    // sysPrintf((void *)gpsdatsbuf);     sysPrintf("in two");    
//	        if(strstr((void *)gpsdatsbuf , "$GPGGA") != NULL)
//	        {
//	           if(GpsRecievedNew == 0)
//	           {
//	              memcpy((void *)(gpsdats),(void *)gpsdatsbuf,76);
//	              GpsRecievedNew = 1;    
//	           }
//	        }
//	        for(i = 0;i<200;i++)
//	        {
//	            gpsdatsbuf[i]  = 0;
//	        }
//	        gpsbufloc =0;
//	        
//	    } 
//	    else
//	    {
//	        gpsbufloc++;      
//	    }   
   }
  
}
#endif



/***********************************************************************************************************/
//2017.8.3 Author: wushengjun
//Function: 断电报警,GPS拆除报警,备用电池断电报警,SIM卡更换报警. 
//Function Name:  AlarmBDRSSend(); 
/***********************************************************************************************************/
UINT8 AlarmBDRSSend(struct CarData NewCarData)
{
    static char CommSended = 0;
    static char DataSended = 0;
    struct  CarData *  AlarmDataSend  = &NewCarData;
	
    if( DataSended == 0 )   //数据未发送完
    {
       if(CommSended)  //命令已发送
       {
          if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) ) 
		  //if( (strstr((char *)strbuf,at_command_reply[4]) == NULL) )
          {           
             if(Gprs_Timeout_Count >= 500)
		     {
		        Gprs_Timeout_Count = 0;
		        memset((void *)strbuf,0,300); bufloc=0;
			    at_command_order = 0;	
			    AtCommSendingState = COMMSENDED;
			    DataSended = 0;
                CommSended = 0; 
                return 0;	              	    	  
		     }  
             return 3;
          }
          else
          {

					//2017.8.5 wushengjun 
					BaoJingValueObj.WaiBuDianYuanPowerOff = WaiBuDianYuanPowerOffFun() + 0x30;
					BaoJingValueObj.GPSCaiChuBaoJing = GPSCaiChuBaoJingFun() + 0x30;
					BaoJingValueObj.BackBatteryPowerOff = BackBatteryPowerOffFun() + 0x30;
					BaoJingValueObj.SIMCardChangeAlarm = SIMCardChangeAlarmFun() + 0x30;
					//
					Delay_Ms(1);
					GprsSendComm("X;");	//2
					
					GprsSendComm((char *)(&(AlarmDataSend->SimID)) );//13+1
					GprsSendComm((char *)(&(AlarmDataSend->GpsData)));//66个
					GprsSendComm(";");	//1
					GprsSendComm((char *)(&(BaoJingValueObj.WaiBuDianYuanPowerOff)));//外接电源断电报警	 //1
					GprsSendComm((char *)(&(BaoJingValueObj.GPSCaiChuBaoJing)));//GPS拆除报警	  //1
					GprsSendComm((char *)(&(BaoJingValueObj.BackBatteryPowerOff)));//备用电池断电报警  //1
					GprsSendComm((char *)(&(BaoJingValueObj.SIMCardChangeAlarm)));//SIM卡更换报警  //1
					GprsSendComm((char *)(&(BaoJingValueObj.YiHuoCheckSum)));//异或校验和	//1			
					GprsSendComm("\r");//END 	   //1
			

             DataSended  = 1;    
             Gprs_Timeout_Count = 0;        
              return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_command[9]);
		 //wushengjun add 2017.8.5

         	GprsSendComm("89");//length == 83;
		 
		 GprsSendComm("\r");
         Gprs_Timeout_Count = 0; 
         CommSended = 1;
          return 3; 
      }
   }
   else    //数据发送完
   {
      
	  //if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) )  //未收到发送完回码
      if( (strstr((char *)strbuf,"SEND OK") == NULL) )  //未收到发送完回码	  "SEND OK"
	  {
          if(Gprs_Timeout_Count >= 1000)
		  {
		      Gprs_Timeout_Count = 0;
		      memset((void *)strbuf,0,300); bufloc=0;
			  at_command_order = 0;	
			  AtCommSendingState = COMMSENDED;	
			  DataSended = 0;
              CommSended = 0; 
              return 0;		    
		  }  
          return 3;	
       }
       else
       {					
          memset((void *)strbuf,0,300); bufloc=0;
          DataSended = 0;
          CommSended = 0; 
          gprsNoConnect = 0;
          Gprs_Timeout_Count = 0;

		  //FlagBaoJing = 0;
          return 4;
       }   
    }
      
}
/***********************************************************************************************************/

