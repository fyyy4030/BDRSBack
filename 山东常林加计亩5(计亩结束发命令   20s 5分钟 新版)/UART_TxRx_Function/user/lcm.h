#ifndef		_LCM_H_
#define		_LCM_H_


#include <stdio.h>
#include <string.h>
#include "NUC131.h"	 
#include "globle.h"

typedef	unsigned char	Byte;
typedef	unsigned int 	Word;
typedef	unsigned long	Long;

#define sysTxBufReadNextOne1()	(((_sys_uUartTxHead1+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxHead1+1)
#define sysTxBufWriteNextOne1()	(((_sys_uUartTxTail1+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxTail1+1)

#define UART_BUFFSIZE	256
 uint8_t _sys_ucUartTxBuf1[UART_BUFFSIZE];
 uint16_t  _sys_uUartTxHead1, _sys_uUartTxTail1;

//#define RX_ARRAY_NUM 100
//uint8_t uart_rx1[RX_ARRAY_NUM] = {0};
 //uint32_t rx_cnt1 = 0;

 //uint16_t bufloc = 0;	  // 
 //uint16_t bufloc_pre = 0;

 char ComData[12];
 unsigned char rbuf[15];
unsigned char jieshoujishu;


void AVclose(void);
void AVchange(void);
void Send_av1(void );

void UART1_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 module */
    SYS_ResetModule(UART1_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART1, 115200);

	  /* Set UART Rx and RTS trigger level */
	  // UART1->FCR |= (3ul<<UART_FCR_RFITL_Pos);
		 //UART1->FCR |= (ul<<UART_FCR_RFITL_Pos);
	 //Set Rx timeout count
	 UART_SetTimeoutCnt(UART1, 200);
	 //enable Rx timeout counter
	 UART1->IER |= 	UART_IER_TIME_OUT_EN_Msk;

	 /* Enable Interrupt  */
    //UART_EnableInt(UART1, (UART_IER_RDA_IEN_Msk | UART_IER_TOUT_IEN_Msk));
	UART_EnableInt(UART1, UART_IER_RDA_IEN_Msk );
	UART_DisableInt(UART1,  UART_IER_THRE_IEN_Msk );
	_sys_uUartTxHead1 = _sys_uUartTxTail1 = NULL;
	 
}


/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART1_IRQHandler(void)
{
  uint8_t buff = 0,i;
   uint32_t u32IntSts = UART1->ISR;
   uint32_t reg_temp;
   	reg_temp =  UART1->IER ;
   u32IntSts = 	UART1->ISR;
   if(u32IntSts & UART_ISR_RDA_INT_Msk)		 //UART_FSR_RX_POINTER_Msk
   {	 	
	  buff = UART_READ(UART1);
	  switch (jieshoujishu)
   {
      case 0:
 comloop: if(buff==0x5A)
         {
		      jieshoujishu=1;
		      rbuf[0]=buff;
		     }
		    else
		    {jieshoujishu=0;}
   		  break;
      case 1:
          if(buff==0xA5)
		     {
		      jieshoujishu=2;
		      rbuf[1]=buff;
		     }
		     else
		     {
		     goto comloop;
		     }
		     break;
	  case 2:
	      if(buff==0x06)
		    {
		     jieshoujishu=3;
		     rbuf[2]=buff;
		    }
		    else if(buff==0x05)
		    {
		     jieshoujishu=10;
		     rbuf[9]=buff;
		    }
		    else
		    {
		    goto comloop;
		    }
		    break;
	  
	  case 3: 
	      rbuf[3]=buff;
		    jieshoujishu++; 
	      break; 
	  case 4:	
	      rbuf[4]=buff; 
		    jieshoujishu++;
  			
		    break;  
	  case 5:
	 	    rbuf[5]=buff;   
		    jieshoujishu++;
	     break;
	  case 6:
	      rbuf[6]=buff; 
		    jieshoujishu++;
	     break;
	  case 7:
	      rbuf[7]=buff; 
		    jieshoujishu++;
	      break;
	case 8:
	      rbuf[8]=buff; 
		    jieshoujishu=0;
		     flag |= BIT(rx_ok);
		  //P_LED ^= BIT(L_LED);
	     break;
  case 10: 
	      rbuf[10]=buff;
		    jieshoujishu++; 
	      break; 

   case 11: 
		    rbuf[11]=buff;
		    jieshoujishu++; 
		    break; 

  
   case 12: 
			  rbuf[12]=buff;
			  jieshoujishu++; 
			  break; 
  
   case 13: 
		     rbuf[13]=buff;
		     jieshoujishu++; 
		     break; 

   case 14: 
		    rbuf[14]=buff;
	      jieshoujishu=0;
		    flag |= BIT(pag_resok);
		  
		   break; 
		  
	default:
		jieshoujishu=0;
		 break; 
   }
		 
		 
		 
		 
   }  
    //发送命令
   if( reg_temp & 	UART_IER_THRE_IEN_Msk) 
   {
	   if(u32IntSts & UART_ISR_THRE_INT_Msk)
	   {	  		 	
		   	if (_sys_uUartTxHead1 == _sys_uUartTxTail1) 
			{									 
				//Disable interrupt if no any request!  
				UART_DisableInt(UART1,  UART_IER_THRE_IEN_Msk );
			} 
			else
			{
				//Transmit data 
				for (i=0; i<8; i++)
				{ 		
				  UART1->THR =  _sys_ucUartTxBuf1[_sys_uUartTxHead1];			
				  _sys_uUartTxHead1 = sysTxBufReadNextOne1();
				  if (_sys_uUartTxHead1 == _sys_uUartTxTail1) // buffer empty
						break;
				}
			}
			  
	    } 
    } 
}

 void _PutChar_f1(uint8_t ucCh)
{ 
	//while(sysTxBufWriteNextOne1() == _sys_uUartTxHead1) ;	// buffer full		
	_sys_ucUartTxBuf1[_sys_uUartTxTail1] = ucCh;
	_sys_uUartTxTail1 = sysTxBufWriteNextOne1();  
	if( (UART1->IER & UART_IER_THRE_IEN_Msk)== 0)
	UART_EnableInt(UART1,  UART_IER_THRE_IEN_Msk );

}

 void sysPrintf1(char *pcStr,uint32_t u32Len)
{
    uint8_t i;
   	for(i=0;i<u32Len;i++)
	{           
	 	_PutChar_f1(*pcStr++);
	}

}

void Send_charData(char *pcStr,uint32_t u32Len)
{
	 sysPrintf1(pcStr,u32Len);
}

void Send_data(unsigned int add,unsigned int data)
{
  //unsigned char i;
  //for(i=0;i<12;i++)	 ComData[i]=0;
  ComData[0]=0x5a;
  ComData[1]=0xa5;
  ComData[2]=0x05;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x82;  
  ComData[4]=add>>8;
  ComData[5]=add;  
  ComData[6]=data>>8;
  ComData[7]=data;
  Send_charData(ComData,8);
}

void Send_longdata(unsigned int add,unsigned long data)
{
  //unsigned char i;
  //for(i=0;i<12;i++)	 ComData[i]=0;
  ComData[0]=0x5a;
  ComData[1]=0xa5;
  ComData[2]=0x07;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x82;  
  ComData[4]=add>>8;
  ComData[5]=add;  
  ComData[6]=data>>24;
  ComData[7]=data>>16;
  ComData[8]=data>>8;
  ComData[9]=data;
  Send_charData(ComData,10);
}

void Send_chaxu(unsigned int add)    //组织报文
{
  unsigned char i;
  for(i=0;i<12;i++)	 ComData[i]=0;
  ComData[0]=0x5a;
  ComData[1]=0xa5;
  ComData[2]=0x04;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x83;  
  ComData[4]=add>>8;
  ComData[5]=add;  
  ComData[6]=0x01;
  Send_charData(ComData,7);
}

void Send_chaxupag(void)   
{
  ComData[0]=0x5a;
  ComData[1]=0xa5;
  ComData[2]=0x03;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x81;  
  ComData[4]=03;
  ComData[5]=02;  
  Send_charData(ComData,6);
}


void Send_chaxutim(void)   
{
  ComData[0]=0x5a;
  ComData[1]=0xa5;
  ComData[2]=0x03;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x81;  
  ComData[4]=0x20;
  ComData[5]=07;//7???  
  Send_charData(ComData,6);
}




void jiaozhun(void)
{
  ComData[0]=0x5A;
  ComData[1]=0xA5;
  ComData[2]=0x03;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x80;  
  ComData[4]=0xEA;
  ComData[5]=0x5A;  
  Send_charData(ComData,6);
}


void mijichg(void)
{
 if(metertime1>1)
 {
   if(metertime>=5)
   {
    if(metertime1<=20)
    { 
    ECU_flag ^= BIT(Miji);
    flag |= BIT(d_save);
    }
	/*
   else
   	{
   	 if(ECU_flag & BIT(Miji))//?????
        cur_meter=0;
   	}*/
   metertime1=0;//?????,?0
  }
   else
   	{
   	   if(metertime1>20)
   	   	{
   	   	 if(ECU_flag & BIT(Miji))//?????
		 {cur_meter=0;
		 
		  //ECU_flag &=~ BIT( Mijiok );
		  //metertime1=0;//?????,?0
   	   	 }
   	   	}
	   	
   	}
 }

}


void uart_manage(void)
{
  unsigned int dat1,dat2;
 if(flag&BIT(rx_ok))
 {
  if((rbuf[3]==0x83)&&(rbuf[6]==0x01))
  {
	dat1=rbuf[5]|rbuf[4]<<8;
	dat2=rbuf[8]|rbuf[7]<<8;
	switch(dat1)
	{
	case 0x0070:
 		 flag ^= BIT(P_set);
	   //pagebuff=3;
		 break;
	case 0x0071:
	     flag ^= BIT(bp_bj);
		 break;
	case 0x0072:
	     jiaozhun();
		 break;
	case 0x0073:
		 flag ^= BIT(av_auto);
		 flag |= BIT(d_save);
		 break;
	case 0x0074:
	     if(avtime<9999)
		 {
		   avtime++;
		   flag |= BIT(d_save);
		 }
		 break;
	case 0x0075:
	     if(avtime>5)
		 {
		   avtime--;
		   flag |= BIT(d_save);
		 }
		 break;
	case 0x0076:
	     flag ^= BIT(av_turn);
		 avtimetemp=0;
		 break;
	case 0x0077:
	     flag ^= BIT(work);
		 flag |= BIT(d_save);
		 break;
	
	  case 0x007B:
	   ECU_flag ^= BIT(Miji);
	   if(ECU_flag & BIT(Miji))//此时为米计
        cur_meter=0;
		flag |= BIT(d_save);
		 break;
   
	case 0x0049:
		 break;
	case 0x004a:
		 break;
	case 0x004c:
		 break;
	case 0x004d:
		 break;
	default:
		 break;
	

     /*		 
		default:
		 break;*/
	}
  }
  flag &=~ BIT(rx_ok);
  }
 if(flag&BIT(pag_resok))
 {
  if((rbuf[10]==0x81)&&(rbuf[11]==0x03)&&(rbuf[12]==0x02))
  {
	page=rbuf[14];
  }
  flag &=~ BIT(pag_resok);
 }
}



void Send_changepage(unsigned int add)  
{
  ComData[0]=0x5a;
  ComData[1]=0xa5;
  ComData[2]=0x04;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x80; 
  ComData[4]=0x03; 
  ComData[5]=add>>8;
  ComData[6]=add;  
  Send_charData(ComData,7);
}

void Send_rest(void)  
{
  ComData[0]=0x5a;
  ComData[1]=0xa5;
  ComData[2]=0x04;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
  ComData[3]=0x80; 
  ComData[4]=0xEE; 
  ComData[5]=0x5a;
  ComData[6]=0xa5;
  Send_charData(ComData,7);
}

void startingup(void)
	{
	  //5A A5 03 80 01 40
	  unsigned char i;
	  
	  if(!opok)
	 	{ 
          
		 i=optime10s/3;
	 	 if( i>=30)//64
	 	 {
	 	 opok=1;
		 i=0x40;
		// pageturn();//???????
	 	 }   
		 
	     ComData[0]=0x5A;
	     ComData[1]=0xA5;
	     ComData[2]=0x03;																																																																																																																																												//baowen => 0xFA 0xFA 0xFA
	     ComData[3]=0x80;  
	     ComData[4]=0x01;
	     //ComData[5]=0x40;  
		    ComData[5]=i; 
	      Send_charData(ComData,6);
	 	}
	}
	/*
	void pageturn(void)
	{
	  if(pagread>5)
	  	{
	  	 Send_chaxupag(); 
		 pagread=0;
	  	}

	 AVchange();

    if(page!=pagebuff)
     {

	 
      page=pagebuff;
	  Send_changepage(page);
      
	   if(page==3)
		Send_av1();
	   else
	   	AVclose(); 
	 
     }
     //Send_changepage(page);
   }

*/

void backled(void)
{
  //5A A5 03 80 01 40
  if((tbflag1 & BIT(2))||(tbflag1 & BIT(3)) ||(tbflag1 & BIT(4))   )
	  {
	    ComData[0]=0x5A;
        ComData[1]=0xA5;
        ComData[2]=0x03;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
        ComData[3]=0x80;  
        ComData[4]=0x01;
        ComData[5]=0x10;  
        Send_charData(ComData,6);
	  }
	  else
	  {
	    ComData[0]=0x5A;
        ComData[1]=0xA5;
        ComData[2]=0x03;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //baowen => 0xFA 0xFA 0xFA
        ComData[3]=0x80;  
        ComData[4]=0x01;
        ComData[5]=0x40;  
        Send_charData(ComData,6);
	  }
}

void pageturn(void)
{
  if(flag & BIT(P_set))
    {
	  page=3;
	}
    else
	{
	  if(flag & BIT(work))
	  {page=1;}
	  else
	  {page=2;}
	}
  Send_changepage(page);
  //backled();
}
/*
void pageturnset(void)
{
  if(flag & BIT(P_set))
    {
	  //page=3;
		pagebuff=3;	
	}
    else
	{
	  if(flag & BIT(work))
	  {//page=1;
			pagebuff=1;	
    }
	  else
	  {
			//page=2;
		 pagebuff=2;	
    }
	}
  
}


	void pageturn(void)
	{ 
		pageturnset();
	  if(pagread>5)
	  	{
	  	 Send_chaxupag(); 
		   pagread=0;
	  	}

    if(page!=pagebuff)
     {
      page=pagebuff;
	    Send_changepage(page);
     }
     
   }
*/






void Send_av1(void)
{
  ComData[0]=0xAA;
  ComData[1]=0xBB;
  ComData[2]=0x06;                                                         
  ComData[3]=0x02; //320*240 
  ComData[4]=0x01;
  /*
  ComData[5]=0x00;  //240
  ComData[6]=0xf0;
  ComData[7]=0x00;  //160
  ComData[8]=0xa0;*/
  ComData[5]=0x00;  //9
  ComData[6]=0x08;
  ComData[7]=0x00;  //114
  ComData[8]=0x75;
  Send_charData(ComData,9);
}

void Send_avdaoche(void)
{
  ComData[0]=0xAA;
  ComData[1]=0xBB;
  ComData[2]=0x06;                                                         
  ComData[3]=0x05; //800*480 
  ComData[4]=0x01;
  ComData[5]=0x00;  //0
  ComData[6]=0x00;
  ComData[7]=0x00;  //50
  ComData[8]=0x32;
  Send_charData(ComData,9);
}

void AVclose(void)
{
  ComData[0]=0xAA;
  ComData[1]=0xBB;
  ComData[2]=0x01;                                                         
  ComData[3]=0x01; 
  Send_charData(ComData,4);
}

void AVchange(void)
{
  if(kgflag1 & BIT(fr))
  {/**/
     P_vcc2 &=~ BIT(L_vcc2);
	 P_vcc3 &=~ BIT(L_vcc3);
	 P_E2 &=~ BIT(L_E2);
	 P_E3 &=~ BIT(L_E3);
     P_vcc1 |= BIT(L_vcc1);
	 P_E1 |= BIT(L_E1);
     Send_avdaoche();
  }
  else
  {
     if(page==1)
	 {/**/
		 	  
	   if(flag & BIT(av_turn))
       {
	     P_vcc1 &=~ BIT(L_vcc1);
	     P_vcc2 &=~ BIT(L_vcc2);
	 	 P_E1 &=~ BIT(L_E1);
	 	 P_E2 &=~ BIT(L_E2);
     	 P_vcc3 |= BIT(L_vcc3);
	 	 P_E3 |= BIT(L_E3);
	   }
	   else
	   {
	     P_vcc1 &=~ BIT(L_vcc1);
	     P_vcc3 &=~ BIT(L_vcc3);
	 	 P_E1 &=~ BIT(L_E1);
	 	 P_E3 &=~ BIT(L_E3);
     	 P_vcc2 |= BIT(L_vcc2);
	 	 P_E2 |= BIT(L_E2);
	   }
		 
	   Send_av1();
	 }
     else
	 {
	   AVclose();
	   P_vcc1 &=~ BIT(L_vcc1);
	   P_vcc3 &=~ BIT(L_vcc3);
	   P_E1 &=~ BIT(L_E1);
	   P_E3 &=~ BIT(L_E3);
       P_vcc2 &=~ BIT(L_vcc2);
	   P_E2 &=~ BIT(L_E2);
	 }
  }
}


#endif


