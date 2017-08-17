
#ifndef		_CANMESSAGE_H_
#define		_CANMESSAGE_H_


#include <stdio.h>
#include "NUC131.h"
#include "globle.h"
#include "canproto.h"


//unsigned char CAN_IDbuf[4]={0,0,0,0};
unsigned char CAN_Rxbuf[22][8];
unsigned char CanReceiveEndFlag;
unsigned char DM1_PACK[20][7],GZ_PACK[20][3];

//unsigned char DM1_Total;
unsigned long spn0;
unsigned char FMI0;
unsigned long spn[10];
unsigned char FMI[10];
unsigned char DM1OK=0;
unsigned char SPNch=0,NETSPNch=0;
//unsigned char SPN_Displaytime=50;
unsigned int wdnum=0,spnnum=0;
volatile unsigned char *pointdm1=DM1_PACK[1],*pointdm2,*pointdm3;
/*---------------------------------------------------------------------------------------------------------*/
/* Define global variables and constants                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
/* Declare a CAN message structure */
STR_CANMSG_T rrMsg;
STR_CANMSG_T tMsg;
unsigned char  processGZ, receivedanbao,processdanbao,receiveduobao;

void  SPN_Display(void)
{ 
 if((receiveduobao)&&(processGZ==0))//显示多包故障
 { 
	 wdnum=DM1_PACK[0][0]|(DM1_PACK[0][1]<<8);
	if(wdnum>=6)
	{
	  spnnum=(wdnum-2)/4;
		CANID0580.duoguzhangtonggao[0]= spnnum;
		 //SPNch++;
		 SPNch = 1;
	   //if(  SPNch<= spnnum)
		 while(SPNch<= spnnum) 
		 {
		  pointdm2 =pointdm1+1+(SPNch-1)*4;
			spn[SPNch] = *(pointdm2+3) ;		 
			spn[SPNch] >>= 5;
			spn[SPNch] <<= 8; 
			spn[SPNch] |= *(pointdm2+2) ;
      spn[SPNch] <<= 8; 	
      spn[SPNch] |= *(pointdm2+1) ;			 
			FMI[SPNch]=*(pointdm2+3)&0x1f;   
	    spn0 =spn[SPNch];
			 
			GZ_PACK[SPNch-1][0] = spn0;
			GZ_PACK[SPNch-1][1] = spn0>>8;
			GZ_PACK[SPNch-1][2] = spn0>>16;
			SPNch++; 
		 }
		//processGZ=0;//暂时无用
		// receiveduobao=0;//接收完后开始处理多包  在GPRS发送完后清零
  // 		 if( SPNch== spnnum)
  // 			 SPNch=0; 
   }
 }
}



 /*----------------------------------------------------------------------------*/
/*  Send Rx Msg by Normal Mode Function (With Message RAM)                    */
/*----------------------------------------------------------------------------*/
unsigned char CANRx_initial(CAN_T *tCAN)
{
    CAN_EnableInt(tCAN, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);      /* Enable CAN interrupt and corresponding NVIC of CAN */
    NVIC_SetPriority(CAN0_IRQn, (1 << __NVIC_PRIO_BITS) - 2);   /* Install CAN call back functions */
    NVIC_EnableIRQ(CAN0_IRQn);
	//if(CAN_SetRxMsg(tCAN, MSG(0), CAN_EXT_ID,0x0cf00400 ) == FALSE)	 //发动机
	if(CAN_SetRxMsg(tCAN, MSG(0), CAN_EXT_ID,0x18ff0080) == FALSE)	 
  {
        return 0;	    
	}
	//if(CAN_SetRxMsg(tCAN, MSG(1), CAN_EXT_ID, 0x18feef00 ) == FALSE)	 //油压
	  if(CAN_SetRxMsg(tCAN, MSG(1), CAN_EXT_ID, 0x18ff0180 ) == FALSE)	
    {
        return 0;	    
    }
	//if(CAN_SetRxMsg(tCAN, MSG(2), CAN_EXT_ID,0x18feee00 ) == FALSE)	 //水温
		if(CAN_SetRxMsg(tCAN, MSG(2), CAN_EXT_ID, 0x18ff0280 ) == FALSE)	
    {
        return 0;	   
    }
	if(CAN_SetRxMsg(tCAN, MSG(3), CAN_EXT_ID,0x18feca00 ) == FALSE)	 //故障码
    {
        return 0;	   
    }
	//if(CAN_SetRxMsg(tCAN, MSG(4), CAN_EXT_ID,0x18fF11F6 ) == FALSE)	 //模拟量
	 if(CAN_SetRxMsg(tCAN, MSG(4), CAN_EXT_ID, 0x18ff0380 ) == FALSE)	
    {
        return 0;	   
    }
		
	  //if(CAN_SetRxMsg(tCAN, MSG(5), CAN_EXT_ID,0x18fF21F6  ) == FALSE)	 //频率量
		if(CAN_SetRxMsg(tCAN, MSG(5), CAN_EXT_ID, 0x18ff0780 ) == FALSE)	
    {
        return 0;	   
    }
		//if(CAN_SetRxMsg(tCAN, MSG(6), CAN_EXT_ID,0x18fF31F6  ) == FALSE)	 //模拟量
		 if(CAN_SetRxMsg(tCAN, MSG(6), CAN_EXT_ID, 0x18ff0880 ) == FALSE)		
    {
        return 0;	   
    }
	 if(CAN_SetRxMsg(tCAN, MSG(7), CAN_EXT_ID,0x18ecff00 ) == FALSE)	 //多包头
    {
        return 0;	   
    }
	 if(CAN_SetRxMsg(tCAN, MSG(8), CAN_EXT_ID,0x18ebff00 ) == FALSE)	 //多包
    {
        return 0;	   
    }
		 //if(CAN_SetRxMsg(tCAN, MSG(9), CAN_EXT_ID,0x18ff41f6 ) == FALSE)	 //室温
		if(CAN_SetRxMsg(tCAN, MSG(9), CAN_EXT_ID, 0x18ff0980 ) == FALSE)		 
    {
        return 0;	   
    }
	  if(CAN_SetRxMsg(tCAN, MSG(10), CAN_EXT_ID, 0x18ff0A80 ) == FALSE)		 
    {
        return 0;	   
    }	
		 if(CAN_SetRxMsg(tCAN, MSG(11), CAN_EXT_ID, 0x18ff0B80 ) == FALSE)		 
    {
        return 0;	   
    }	
		 if(CAN_SetRxMsg(tCAN, MSG(12), CAN_EXT_ID, 0x18ff2180 ) == FALSE)		 
    {
        return 0;	   
    }	
		 if(CAN_SetRxMsg(tCAN, MSG(13), CAN_EXT_ID, 0x18ff2280 ) == FALSE)		 
    {
        return 0;	   
    }	
		 if(CAN_SetRxMsg(tCAN, MSG(14), CAN_EXT_ID, 0x18ff2080 ) == FALSE)		 
    {
        return 0;	   
    }	
		 if(CAN_SetRxMsg(tCAN, MSG(15), CAN_EXT_ID, 0x18ff2181 ) == FALSE)		 
    {
        return 0;	   
    }	
		
		if(CAN_SetRxMsg(tCAN, MSG(16), CAN_EXT_ID, 0x18ff2281 ) == FALSE)		 
    {
        return 0;	   
    }	
		
		if(CAN_SetRxMsg(tCAN, MSG(17), CAN_EXT_ID, 0x18ff2081 ) == FALSE)		 
    {
        return 0;	   
    }	
		
		if(CAN_SetRxMsg(tCAN, MSG(18), CAN_EXT_ID, 0x18ff0C80 ) == FALSE)		 
    {
        return 0;	   
    }	
			if(CAN_SetRxMsg(tCAN, MSG(19), CAN_EXT_ID, 0x18ff0D80 ) == FALSE)		 
    {
        return 0;	   
    }	
		return 1;	   	
}




/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle CAN interrupt event                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void CAN_MsgInterrupt(CAN_T *tCAN, uint32_t u32IIDR)
{  unsigned char i,j;
	switch(u32IIDR)
	{
	case 1:  //发动机		       
	    CanReceiveEndFlag=1;
	     Receiveid_0080=1;
		   tongxintime=0;
		   i=1;    
		 
		 break;
	case 2:  //油压
	      CanReceiveEndFlag=1;
		    tongxintime=0;
	      Receiveid_0180=1;
		    i=2;
		    break;
	case 3:   //水温
        CanReceiveEndFlag=1;
		    tongxintime=0;
	      Receiveid_0280=1;
		     i=3;
		     break;
	case 4:  //单包
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		    receivedanbao=1;
		    i=4;
		    break;
	 case 5: //hours
	      CanReceiveEndFlag=1;
		     tongxintime=0;
	       Receiveid_0380=1;
		     i=5;
		    break;
	 case 6:  //油水
	     CanReceiveEndFlag=1;
		   tongxintime=0;
	     Receiveid_0780=1;
		 i=6;
		 break;
	  case 7:  //发动机状态
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_0880=1;
		 i=7;
		 break;
	 
	  case 8: //多包头
	       CanReceiveEndFlag=1;
		     tongxintime=0;
		     i=8;
		    break;
	 case 9:  //多包
	     CanReceiveEndFlag=1;
		 tongxintime=0;
		 i=9;
	  break;
	  case 10:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_0980=1;
		   i=10;
		   break;
		case 11:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_0A80=1;
		   i=11;
		   break;
		
		case 12:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_0B80=1;
		   i=12;
		   break;
		
		case 13:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_2180=1;
		   i=13;
		   break;
		
		case 14:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_2280=1;
		   i=14;
		   break;
		
		case 15:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_2080=1;
		   i=15;
		   break;
		case 16:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_2181=1;
		   i=16;
		   break;
		
		case 17:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_2281=1;
		   i=17;
		   break;
		case 18:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_2081=1;
		   i=18;
		   break;
		case 19:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_0C80=1;
		   i=19;
		   break;
		case 20:  
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		   Receiveid_0D80=1;
		   i=20;
		   break;
		
		
	  default:
		 i=0;
		 break;
	}
	if(i)	
	{
	 CAN_Receive(tCAN, (i-1), &rrMsg);
	 for(j = 0; j < 8; j++) 
    { 
      CAN_Rxbuf[i][j]=rrMsg.Data[j];          
    } 
	}
	if(i==8)	
	 {
		 //if(CAN_Rxbuf[8][5]==0xca)
		 if((CAN_Rxbuf[8][5]==0xca)&&(!receiveduobao))//为故障包  并且上个故障包已经处理完
		 {
		  DM1OK=1;
			 for(j = 0; j < 7; j++) 
	      DM1_PACK[0][j] = CAN_Rxbuf[8][j+1];
		  //ECU_flag |= BIT(Ecu_dm1);//显示多包
		 }
   }
	 if(i==9)
	{
		if(DM1OK)
		{
			 i=CAN_Rxbuf[9][0];//取数据包序号
	    for(j = 0; j < 7; j++) 
	     DM1_PACK[i][j] = CAN_Rxbuf[9][j+1];
			
			if(i== DM1_PACK[0][2])
			{
			DM1OK=0;
			receiveduobao=1;//接收完后开始处理多包
			}
		}
  }

}

/*---------------------------------------------------------------------------------------------------------*/
/* CAN0 interrupt handler                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void CAN0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;

    u8IIDRstatus = CAN0->IIDR;

    if(u8IIDRstatus == 0x00008000)        /* Check Status Interrupt Flag (Error status Int and Status change Int) */
    {
	   
        /**************************/
        /* Status Change interrupt*/
        /**************************/
        if(CAN0->STATUS & CAN_STATUS_RXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear RxOK status*/
        }

        if(CAN0->STATUS & CAN_STATUS_TXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_TXOK_Msk;    /* Clear TxOK status*/
        }

        /**************************/
        /* Error Status interrupt */
        /**************************/
//        if(CAN0->STATUS & CAN_STATUS_BOFF_Msk)
//        {
//            GprsSendComm("BOFF INT\n") ;
//        }
//        else if(CAN0->STATUS & CAN_STATUS_EWARN_Msk)
//        {
//            GprsSendComm("EWARN INT\n") ;
//        }
//        else if((CAN0->ERR & CAN_ERR_TEC_Msk) != 0)
//        {
//            GprsSendComm("Transmit error!\n") ;
//        }
//        else if((CAN0->ERR & CAN_ERR_REC_Msk) != 0)
//        {
//            GprsSendComm("Receive error!\n") ;
//        }
    }
    else if((u8IIDRstatus >= 0x1) && (u8IIDRstatus <= 0x20))
    {
        CAN_MsgInterrupt(CAN0, u8IIDRstatus);
		 
        CAN_CLR_INT_PENDING_BIT(CAN0, (u8IIDRstatus - 1)); /* Clear Interrupt Pending */
    }
    else if(CAN0->WU_STATUS == 1)
    {
     //   GprsSendComm("Wake up\n");

        CAN0->WU_STATUS = 0;    /* Write '0' to clear */
    }

}




void CAN_data(void)
{
	//unsigned long temp=0;
	unsigned char j,k;
	if(tongxintime>=50)
	{
   for(k = 0; k < 22; k++)
   {
	  for(j = 0; j < 8; j++)
     CAN_Rxbuf[k][j]=0;
   }
	 // CAN_Rxbuf[2][3]=250;
	  //CAN_Rxbuf[3][0]=40; 
   } 
	
	
  //发动机
//   fdj_rev=CAN_Rxbuf[1][3]|CAN_Rxbuf[1][4]<<8;
//   fdj_rev = Get_Data(fdj_temp,fdj_rev,10);
//   fdj_rev =fdj_rev/8;
//   if(fdj_rev>3000){fdj_rev=3000;} 
//   
//   //油压
//   oil_p=CAN_Rxbuf[2][3];
//   
//   if(oil_p>250)
//   {oil_p=250;}
//     //oil_p =oil_p/25;
//   oil_p = Get_Data(oil_ptemp,oil_p,10);
//   oil_p =oil_p/5*2;
//   //水温
//   watmp=CAN_Rxbuf[3][0];
//   watmp = Get_Data(Wtemp,watmp,10);
//   if(watmp>40){watmp -= 40;}
//   else{watmp=0;}
//   watmp1=watmp;


//   CANID0080.leijilicheng[0]=CAN_Rxbuf[7][5];
// 	CANID0080.leijilicheng[1]=CAN_Rxbuf[7][6];
//   CANID0080.leijilicheng[2]=CAN_Rxbuf[7][7];
//   CANID0080.chesu[0]=CAN_Rxbuf[6][6];
// 	CANID0080.chesu[1]=CAN_Rxbuf[6][7];


//    temp=CAN_Rxbuf[7][5]+(CAN_Rxbuf[7][6]<<8)+(CAN_Rxbuf[7][7]<<16); 
// 	 temp=temp*10;
//   CANID0080.leijilicheng[0]=temp;
// 	CANID0080.leijilicheng[1]=temp>>8;
//   CANID0080.leijilicheng[2]=temp>>16;
// 	CANID0080.leijilicheng[3]=temp>>24;

// 	
// 	temp=CAN_Rxbuf[6][6]+(CAN_Rxbuf[6][7]<<8);
// 	temp=temp*1000000/39063;
//   CANID0080.chesu[0]=temp;
// 	CANID0080.chesu[1]=temp>>8;
// 	
	
	
	
	
// 	
// 	CANID0180.dianpingvol[0]=CAN_Rxbuf[7][0];
// 	CANID0180.dianpingvol[1]=CAN_Rxbuf[7][1];
// 	if(CAN_Rxbuf[10][0]<60)
// 		CANID0180.cheneiwendu=0;
// 	else
// 	CANID0180.cheneiwendu=CAN_Rxbuf[10][0]-60;
// 	CANID0180.jiyouyali=CAN_Rxbuf[2][3];
// 	
// 	//发动机
// 	CANID0280.zhuansu[3]=CAN_Rxbuf[1][3];
// 	CANID0280.zhuansu[4]=CAN_Rxbuf[1][4];
// 	
// 	CANID0380.shuiwen=CAN_Rxbuf[3][0];
	
	  if(Receiveid_0080)
		{
	  Receiveid_0080t=Receiveid_0080;
		Receiveid_0080=0;
		}
		if(Receiveid_0180)
		{
		Receiveid_0180t=Receiveid_0180;
		Receiveid_0180=0;
		}	
		if(Receiveid_0280)
		{	
		Receiveid_0280t=Receiveid_0280;
		Receiveid_0280=0;
		}	
		if(Receiveid_0380)
		{	
		Receiveid_0380t=Receiveid_0380;
		Receiveid_0380=0;
		}	
		if(Receiveid_0780)
		{	
		Receiveid_0780t=Receiveid_0780;
		Receiveid_0780=0;
		}	
		if(Receiveid_0880)
		{	
		Receiveid_0880t=Receiveid_0880;
		Receiveid_0880=0;
		}	
		if(Receiveid_0980)
		{	
		Receiveid_0980t=Receiveid_0980;
		Receiveid_0980=0;
		}	
		if(Receiveid_0A80)
		{	
		Receiveid_0A80t=Receiveid_0A80;
		Receiveid_0A80=0;
		}	
		if(Receiveid_0B80)
		{	
		Receiveid_0B80t=Receiveid_0B80;
		Receiveid_0B80=0;
		}	
		if(Receiveid_2180)
		{	
    Receiveid_2180t=Receiveid_2180;
		Receiveid_2180=0;
		}	
		if(Receiveid_2280)
		{	
		Receiveid_2280t=Receiveid_2280;
		Receiveid_2280=0;
		}	
		if(Receiveid_2080)
		{	
		Receiveid_2080t=Receiveid_2080;
		Receiveid_2080=0;
		}	
		if(Receiveid_2181)
		{	
		Receiveid_2181t=Receiveid_2181;
		Receiveid_2181=0;
		}	
		if(Receiveid_2281)
		{	
		Receiveid_2281t=Receiveid_2281;
		Receiveid_2281=0;
		}	
		if(Receiveid_2081)
		{	
		Receiveid_2081t=Receiveid_2081;
		Receiveid_2081=0;
		}	
		if(Receiveid_0C80)
		{	
		Receiveid_0C80t=Receiveid_0C80;
		Receiveid_0C80=0;
		}	
		if(Receiveid_0D80)
		{	
		Receiveid_0D80t=Receiveid_0D80;
		Receiveid_0D80=0;
		}	
    if(receivedanbao)
		{
	  receivedanbaot=receivedanbao;
	  receivedanbao=0;
		}	
	
	
	if(receivedanbaot)
	{
    //DM1故障
  err=CAN_Rxbuf[4][4]>>5;
  err=CAN_Rxbuf[4][3]|err<<8;
  err=CAN_Rxbuf[4][2]|err<<8;
		
	CANID0480.danguzhang[0]=err&0xff;
	CANID0480.danguzhang[1]=(err>>8)&0xff;
	CANID0480.danguzhang[2]=(err>>16)&0xff;
	//receivedanbao=0; //在GPRS发送完后清零
	//processdanbao=1;//暂时无用
	}
	//多包在发送时循环赋值
	SPN_Display();
	
// 	CANID0780.gunlunzhuansu[0]=CAN_Rxbuf[6][4];
// 	CANID0780.gunlunzhuansu[1]=CAN_Rxbuf[6][5];
// 	CANID0780.futuozhuansu[0]=CAN_Rxbuf[6][0];
// 	CANID0780.futuozhuansu[1]=CAN_Rxbuf[6][1];
// 	
// 	CANID0880.shengyunzhuansu[0]=CAN_Rxbuf[6][2];
// 	CANID0880.shengyunzhuansu[1]=CAN_Rxbuf[6][3];
// 	CANID0880.gechagaodu[0]=CAN_Rxbuf[5][4];
// 	CANID0880.gechagaodu[1]=CAN_Rxbuf[5][5];
	
	 for(j = 0; j < 8; j++)
   CANID0080.data[j]= CAN_Rxbuf[1][j];
	 for(j = 0; j < 8; j++)
   CANID0180.data[j]= CAN_Rxbuf[2][j];
	 for(j = 0; j < 8; j++)
   CANID0280.zhuansu[j]= CAN_Rxbuf[3][j];
	 
	 for(j = 0; j < 8; j++)
   CANID0380.data[j]= CAN_Rxbuf[5][j];
	 
	 for(j = 0; j < 8; j++)
   CANID0780.data[j]= CAN_Rxbuf[6][j];
	 for(j = 0; j < 8; j++)
   CANID0880.data[j]= CAN_Rxbuf[7][j]; 
	 
	 for(j = 0; j < 8; j++)
   CANID0980.data[j]= CAN_Rxbuf[10][j];
	  for(j = 0; j < 8; j++)
   CANID0A80.data[j]= CAN_Rxbuf[11][j];
	 for(j = 0; j < 8; j++)
   CANID0B80.data[j]= CAN_Rxbuf[12][j];
	 for(j = 0; j < 8; j++)
   CANID2180.data[j]= CAN_Rxbuf[13][j];
	  for(j = 0; j < 8; j++)
   CANID2280.data[j]= CAN_Rxbuf[14][j];
	  for(j = 0; j < 8; j++)
   CANID2080.data[j]= CAN_Rxbuf[15][j];
	  for(j = 0; j < 8; j++)
   CANID2181.data[j]= CAN_Rxbuf[16][j];
	  for(j = 0; j < 8; j++)
   CANID2281.data[j]= CAN_Rxbuf[17][j];
	  for(j = 0; j < 8; j++)
   CANID2081.data[j]= CAN_Rxbuf[18][j];
	 for(j = 0; j < 8; j++)
   CANID0C80.data[j]= CAN_Rxbuf[19][j]; 
	 for(j = 0; j < 8; j++)
   CANID0D80.data[j]= CAN_Rxbuf[20][j]; 
	
}



#endif
