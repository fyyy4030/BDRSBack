
 
#include <stdlib.h>
#include <string.h>	 
#include <stdio.h>
#include "NUC131.h"
#include "M660.h"



typedef uint8_t  UINT8 ;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef unsigned char *   PUINT8;


#define sysTxBufReadNextOne()	(((_sys_uUartTxHead+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxHead+1)
#define sysTxBufWriteNextOne()	(((_sys_uUartTxTail+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxTail+1)
#define UART_BUFFSIZE	256
static uint8_t _sys_ucUartTxBuf[UART_BUFFSIZE];
static uint16_t volatile _sys_uUartTxHead, _sys_uUartTxTail;

#define RX_ARRAY_NUM 100
static uint8_t uart_rx[RX_ARRAY_NUM] = {0};
static uint8_t uart_rx2[RX_ARRAY_NUM] = {0};
static uint32_t volatile rx_cnt = 0;
static uint32_t volatile rx_cnt2 = 0;
 char CCID[20];
char havegetccid = 0;

extern uint8_t  GprsConnectTcpState;

#define COMMSENDING 1
#define COMMSENDED 0

UINT8 VerifyGps(UINT8* GpsDatap);

struct MachineFrame
{
  unsigned short  u16indicateLed;
  unsigned short  u16speed;
  unsigned char   u8oilPressure;
  unsigned char   u8waterTemperature;
  unsigned char   u8fuelVol;
  unsigned long   u32hours;
  unsigned char   u8oilTemperature;
  unsigned short  u16ptoRotateSpeed;
  unsigned char   u8airPressure;
  unsigned char   u8hydraulicFluid;
  unsigned short   u16voltage;
  unsigned short  u16engineRotate;
  unsigned long   u32milsage;
  unsigned short  u16errorcode;
};
extern struct MachineFrame NewMachineFrame;

////IP:122.0.75.201,8880   ;171.8.225.246,8002   106.3.44.168:8880
//const char *at_command[]={"at\r",                  "at+ccid\r", "at+csq\r",   "at+creg?\r", "at+cgdcont=1,\"IP\",\"cmnet\"\r","at+xiic=1\r","at+tcpclose=0\r","at+tcpsetup=0,171.8.225.246,8880\r","at+tcpsend=0," };
//const char *at_command_reply[]={"at\r\r\nOK\r\n","\r\nOK\r\n", "\r\nOK\r\n", "+CREG: 0,1\r\n\r\nOK\r\n", "\r\nOK\r\n",                     "\r\nOK\r\n", "+TCPCLOSE:",     "+TCPSETUP:0,OK",                           "\r\n>","+TCPSEND:0,"};

struct PowerOffComm
{
     uint8_t   Head[2];
	 uint8_t   SN[12];
	 uint8_t   CommID[2];
     uint8_t   SimID[11];
	 uint8_t   Comm[3];
	 uint8_t   End[2];
};
extern struct PowerOffComm   NewPowerOffComm,RpPowerOffComm;
extern struct PowerOffComm   NewPowerOnComm,RpPowerOnComm;


UINT16 gprsNoConnect = 0;
static UINT8 LengthAsc[5];
struct CarData
{
   UINT8   Head[2+1];
   UINT8   SimID[13+1+1];
   UINT8   GpsData[76+1+1];
   UINT8   OilPressur[5+1+1];
   UINT8   WaterTemperature[5+1+1];
   UINT8   FuelVolme[5+1+1];
   UINT8   BatteryVoltage[5+1+1];
   UINT8   RotationHour[8+1+1];
   UINT8   RotationSpeed[6+1+1];
   UINT8   MachineState[6+1+1];
   UINT8   VerifyCode[2+1];
   UINT8   End[2];
};
extern struct CarData NewCarData;
extern struct CarData * CarDataP;
extern uint8_t IsReceivePowOff;
extern uint8_t IsReceivePowOn ;

UINT8  GprsSending = 0;
UINT32 Gprs_Timeout_Count;
UINT8 at_command_order = 0;
UINT8 AtCommSendingState = COMMSENDED; 

static volatile UINT16 bufloc = 0;	  // 
static volatile UINT16 bufloc_pre = 0;
static volatile char strbuf[300]; 
//static UINT32 OverTimes;
static void uartpfnCallback(UINT8* u8Buf,UINT32 u32Len); 
static void uartTOpfnCallback(UINT8* u8Buf,UINT32 u32Len); 
void  GprsSendComm(char *comm) ;


//******************************
//GPS数据定义
//
//***********************************
//发送给服务器的数据定义

static volatile UINT16 gpsbufloc = 0;
volatile UINT8 gpsdatsbuf[200] = {0};
volatile UINT8 gpsdats[200] = {0};
volatile UINT8 GpsRecievedNew = 0;

void uart2pfnCallback( UINT8*  u8Buf,UINT32 u32Len);
void uart2TOpfnCallback(UINT8* u8Buf,UINT32 u32Len);

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
	 memset((void *)strbuf,0,200);
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
   if(u32IntSts & UART_ISR_RDA_INT_Msk)		 //UART_FSR_RX_POINTER_Msk
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

   if(u32IntSts & UART_ISR_TOUT_IF_Msk)
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
//	  if(IsReceivePowOff == 0)
//	  {
//		  if((strstr((char *)strbuf,(char *)(&(NewPowerOffComm.CommID)) )!= NULL))
//		  {
//		     IsReceivePowOff = 1;  // PF5 =  0;  
//			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
//			 memcpy((char *)(&(RpPowerOffComm.SN)) , (void *)(pStr+2),12);
//		     memset((void *)strbuf,0,100); bufloc=0;
//		  }
//	  }
//	  if(IsReceivePowOn == 0)
//	  {
//		  if((strstr((char *)strbuf,(char *)(&(NewPowerOnComm.CommID)) )!= NULL))
//		  {
//		  	 IsReceivePowOn = 1;  // PF5 =  0; 
//			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
//			 memcpy((char *)(&(RpPowerOnComm.SN)) , (void *)(pStr+2),12);
//		     memset((void *)strbuf,0,100); bufloc=0;
//		  
//		  }	
//	  }			 
   if( (strstr((char *)strbuf,"+TCPCLOSE:0,Link Closed" )!= NULL) ) 
   {
   	   Gprs_Timeout_Count = 1000;	 GprsConnectTcpState=0;
	   memset((void *)strbuf,0,200); bufloc=0;
	   at_command_order = 0;	
			   AtCommSendingState = COMMSENDED;	
   }
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
   if(u32IntSts2 & UART_ISR_RDA_INT_Msk)		 //UART_FSR_RX_POINTER_Msk
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







void Delay_Ms(UINT32 Tcount)
{
   UINT32 i;
   while(Tcount--)
   {
       for(i=0; i<420; i++) ;  //420  
   }

} 



PUINT8 FrameLengthToASC(struct CarData NewCarData)
{
   UINT8  totallength;
   struct CarData *  datap  = &NewCarData;

 
   totallength = strlen((char *)(datap->Head)) + strlen((char *)(datap->SimID)) + strlen((char *)(datap->GpsData))+1 +strlen((char *)(datap->OilPressur)) +strlen((char *)(datap->WaterTemperature));
   totallength += strlen((char *)(datap->FuelVolme)) + strlen((char *)(datap->BatteryVoltage)) +strlen((char *)(datap->RotationHour));
   totallength += strlen((char *)(datap->RotationSpeed)) + strlen((char *)(datap->MachineState)) +1;// + strlen((char *)(datap->VerifyCode)) 
   //totallength  =  124;
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
           memset((void *)strbuf,0,200); bufloc=0;  
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
		       memset((void *)strbuf,0,200); bufloc=0;
		       Delay_Ms(10);
			   at_command_order = 0;	
			   AtCommSendingState = COMMSENDED;	
			   havegetccid = 0;	   
			    return 0;		 //超时
		    }
		   	 return 1;			//正在连接
        }
	    else
	    {//	 (PF5 = 1);       
		   Delay_Ms(1); 
		   AtCommSendingState = COMMSENDED;
		   #ifdef CCIDASSIM
		   if((at_command_order == 1) && (havegetccid == 0))
		   {
		   	  // +CCID: 898602a316130a048278
			  if( (pdata = strstr((void *)strbuf,"+CCID:")) != NULL)
			  {
			      for( i=0;i<20;pdata++,i++)
				  {
				  	CCID[i] = *(pdata + 6);				  
				  }	 
				  memcpy((char *)(&(CarDataP->SimID))+2, &CCID[9],11); 	
				  havegetccid = 1;		  
			  }	
			    		   
		   }
		   #endif
		   memset((void *)strbuf,0,200); bufloc=0;
		   Delay_Ms(10);
	       at_command_order++;
	       if(at_command_order >= 8)
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
	    
	   	 if( (strstr((char *)strbuf,at_command_reply[at_command_order]) == NULL) &&  (strstr((char *)strbuf,"+CREG: 0,5\r\n\r\nOK\r\n") == NULL) )
	     {	  
		    if(Gprs_Timeout_Count >= 1000)
		    {
		       Gprs_Timeout_Count = 0;
		       memset((void *)strbuf,0,200); bufloc=0;
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
		   memset((void *)strbuf,0,200); bufloc=0;
	       at_command_order++;
	       if(at_command_order >= 8)
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
    memset((void *)strbuf,0,200); bufloc=0;
	at_command_order = 0;	
    return 0;	
   
 }
	  
   
   
  /*GprsSendComm("at\r"); 
  for(OverTimes=0;OverTimes<50000;OverTimes++)
  {
  
    if(strstr((void *)strbuf,at_command_reply[0])){  sysprintf("IM OK\r"); break;}
  }
    memset((void *)strbuf,0,100); bufloc=0;
  GprsSendComm((VOID *)at_command[1]); 
    for(OverTimes=0;OverTimes<50000;OverTimes++)
    {
    if(strstr((void *)strbuf,at_command_reply[1]) )break;  
    }
     
    memset((void *)strbuf,0,100); bufloc=0;
     GprsSendComm((VOID *)at_command[2]);
       for(OverTimes=0;OverTimes<50000;OverTimes++)
       {
      if(strstr((void *)strbuf,at_command_reply[2])) break;
      }
      memset((void *)strbuf,0,100); bufloc=0;
       GprsSendComm((VOID *)at_command[3]);
        for(OverTimes=0;OverTimes<50000;OverTimes++)
         {
       	    while(strstr((void *)strbuf,at_command_reply[3]))break;
       	 }
       	  memset((void *)strbuf,0,100); bufloc=0;*/

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
          if( (strstr((char *)strbuf,at_command_reply[8]) == NULL) ) 
          {           
             if(Gprs_Timeout_Count >= 500)
		     {
		        Gprs_Timeout_Count = 0;
		        memset((void *)strbuf,0,200); bufloc=0;
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
             GprsSendComm( (char *)(&(datap->Head)) );
             GprsSendComm( (char *)(&(datap->SimID)) );
             GprsSendComm( (char *)(&(datap->GpsData)) );
              GprsSendComm(";");
             GprsSendComm( (char *)(&(datap->OilPressur)) );
             GprsSendComm( (char *)(&(datap->WaterTemperature)) );
             GprsSendComm( (char *)(&(datap->FuelVolme)) );
             GprsSendComm( (char *)(&(datap->BatteryVoltage)) );
             GprsSendComm( (char *)(&(datap->RotationHour)) );
             GprsSendComm( (char *)(&(datap->RotationSpeed)) );
             GprsSendComm( (char *)(&(datap->MachineState)) );
             NewCarData.VerifyCode[0]=GprsDataSum(NewCarData);
            // GprsSendComm( (char *)(&(datap->VerifyCode)) );
                GprsSendComm("\r");
             DataSended  = 1;    
             Gprs_Timeout_Count = 0;        
              return 3; 
          }
              
      }
      else    //命令未发送
      {
         GprsSendComm((char *)at_command[8]);
         GprsSendComm((char *)FrameLengthToASC(NewCarData));
         Gprs_Timeout_Count = 0; 
         CommSended = 1;
          return 3; 
      }
   }
   else    //数据发送完
   {
      if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) )  //未收到发送完回码
      {
          if(Gprs_Timeout_Count >= 1000)
		  {
		      Gprs_Timeout_Count = 0;
		      memset((void *)strbuf,0,200); bufloc=0;
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
          memset((void *)strbuf,0,200); bufloc=0;
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
          if( (strstr((char *)strbuf,at_command_reply[8]) == NULL) ) 
          {           
             if(Gprs_Timeout_Count >= 500)
		     {
		        Gprs_Timeout_Count = 0;
		        memset((void *)strbuf,0,200); bufloc=0;
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
         GprsSendComm((char *)at_command[8]);
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
      if( (strstr((char *)strbuf,at_command_reply[9]) == NULL) )  //未收到发送完回码
      {
          if(Gprs_Timeout_Count >= 1000)
		  {
		      Gprs_Timeout_Count = 0;
		      memset((void *)strbuf,0,200); bufloc=0;
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
          memset((void *)strbuf,0,200); bufloc=0;
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
	  if(IsReceivePowOff == 0)
	  {
		  if((strstr((char *)strbuf, (char *)(&(NewPowerOffComm.Comm)))!= NULL))			  //  CommID 
		  {																					
		     IsReceivePowOff = 1;  // PF5 =  0;  
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			 memcpy((char *)(&(RpPowerOffComm.SN)) , (void *)(pStr+2),12);
		     memset((void *)strbuf,0,100); bufloc=0;
		  }
	  }
	  if(IsReceivePowOn == 0)
	  {
		  if((strstr((char *)strbuf,(char *)(&(NewPowerOnComm.Comm)) )!= NULL))  //  CommID
		  {
		  	 IsReceivePowOn = 1;  // PF5 =  0; 
			 if( ((pStr = strstr((char *)strbuf,"Z;" )) != NULL) )
			 memcpy((char *)(&(RpPowerOnComm.SN)) , (void *)(pStr+2),12);
		     memset((void *)strbuf,0,100); bufloc=0;
		  
		  }	
	  }	  


}



//***************************************************************************//
//内容：以下为GPS功能调用，GPS使用UART2，与UART0(GPRS)共用中断向量
//***************************************************************************


void UART2_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 module */
    SYS_ResetModule(UART2_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART2, 9600);

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


void GpsInterfaceIni(void)
{
   UART2_Init();
}


void uart2pfnCallback( UINT8* u8Buf,UINT32 u32Len)
{
     volatile UINT8  j,i;
    // UINT8 NUM[4];    
    	//	PF5 =0;
    for(j = 0;j<u32Len;j++)
    {  	
    //  NUM[0] =  gpsbufloc%100/10 +0x30;
    //  NUM[1] =  gpsbufloc%10 +0x30; 
     
        if(gpsbufloc >= 199)gpsbufloc=0;
       gpsdatsbuf[gpsbufloc]  = *(u8Buf+j);          
       if(gpsdatsbuf[gpsbufloc] == '\n') 
       {     	 
          //sysPrintf((void *)gpsdatsbuf);    sysPrintf("in one"); sysPrintf((void *)u8Buf);          
          if(strstr((void *)(gpsdatsbuf) , "$GPGGA") != NULL)
          {
             if(GpsRecievedNew == 0)
             {
                memcpy((void *)(gpsdats),(void *)(gpsdatsbuf),76);
                GpsRecievedNew = 1;    
             }          
          }
        // sysPrintf((void *)(CarDataP->GpsData));
         for(i = 0;i<200;i++)
         {
            gpsdatsbuf[i]  = 0;
         }
         gpsbufloc =0;
               
       }    
       else
       {
          gpsbufloc++;       
       }
    }
//	sysPrintf((void *)gpsdatsbuf); 
   
}


void uart2TOpfnCallback(UINT8* u8Buf,UINT32 u32Len)
{
    UINT8  j,i; 
	//	PF5 =0;
    for(j = 0;j<u32Len;j++)
    {  
       
	      if(gpsbufloc >= 199)gpsbufloc=0;
	      gpsdatsbuf[gpsbufloc]  = *(u8Buf+j); 
	       
	      if(gpsdatsbuf[gpsbufloc] == '\n') 
	      {    // sysPrintf((void *)gpsdatsbuf);     sysPrintf("in two");    
	        if(strstr((void *)gpsdatsbuf , "$GPGGA") != NULL)
	        {
	           if(GpsRecievedNew == 0)
	           {
	              memcpy((void *)(gpsdats),(void *)gpsdatsbuf,76);
	              GpsRecievedNew = 1;    
	           }
	        }
	        for(i = 0;i<200;i++)
	        {
	            gpsdatsbuf[i]  = 0;
	        }
	        gpsbufloc =0;
	        
	    } 
	    else
	    {
	        gpsbufloc++;      
	    }   
   }
  
}
