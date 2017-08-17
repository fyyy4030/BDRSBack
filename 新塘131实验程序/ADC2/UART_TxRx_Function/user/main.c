/****************************************************************************
 * @file     main.c
 * @version  V3.00
 * $Revision: 8 $
 * $Date: 15/01/16 11:44a $
 * @brief    Transmit and receive data from PC terminal through RS232 interface.
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "NUC131.h"
#include "globle.h"
#include "delay.h"
#include "canproto.h"
#include "adcapp.h"
//#include "alart.h"
#include "canMessage.h"
#include "dataflash.h"
//#include "gprsproto.h"
#include "gprs.h"



#define PLL_CLOCK   50000000


void numdisp(void);
void numdisp8(void);
void numdisp_test(void);
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*----------------------------------- ----------------------------------------------------------------------*/

    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

	 //CLK_SetModuleClock(WDT_MODULE,CLK_CLKSEL1_WDT_S_LIRC,0);
	 CLK_SetModuleClock(WDT_MODULE,CLK_CLKSEL1_WDT_S_HCLK_DIV2048 ,0);

	 CLK_EnableModuleClock(WDT_MODULE);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);   

    /* Select UART module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

		    /* Enable UART module clock */
    CLK_EnableModuleClock(UART1_MODULE);

	    /* Select UART module clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

// 	    /* Enable UART module clock */
//     CLK_EnableModuleClock(UART2_MODULE);

//     /* Select UART module clock source */
//     CLK_SetModuleClock(UART2_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

	 /* Enable CAN module clock */
    CLK_EnableModuleClock(CAN0_MODULE);

	    /* Enable ADC module clock */
    CLK_EnableModuleClock(ADC_MODULE);
	    /* ADC clock source is 22.1184MHz, set divider to 7, ADC clock is 22.1184/7 MHz */
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_CLKDIV_ADC(7));

}






void TIMER0_Init()
{
	CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);
    /* Open Timer0 in periodic mode, enable interrupt and 10 interrupt tick per second */
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 10);   //10HZ�����ж�
    TIMER_EnableInt(TIMER0); 	
    /* Enable Timer0  NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);
	/* Start Timer0  counting */
    TIMER_Start(TIMER0);

}

void TIMER1_Init()
{
	  CLK_EnableModuleClock(TMR1_MODULE);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HXT, 0);
    /* Open Timer0 in periodic mode, enable interrupt and 10 interrupt tick per second */
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1);   //1HZ�����ж�
    TIMER_EnableInt(TIMER1); 	
    /* Enable Timer0  NVIC */
    NVIC_EnableIRQ(TMR1_IRQn);
	/* Start Timer0  counting */
    TIMER_Start(TIMER1);

}

void TIMER2_Init()
{
	  CLK_EnableModuleClock(TMR2_MODULE);
    CLK_SetModuleClock(TMR2_MODULE, CLK_CLKSEL1_TMR2_S_LIRC, 0);//100hz=0.01s
   // TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 10);   //10HZ�����ж�  16777216   ����24h=24*60*60*100=8640000  ���� 2h=2*60*60*100=720000  5m=5*60*100=30000
	  //TIMER2->TCSR = TIMER_PERIODIC_MODE | 100;
	  TIMER2->TCSR = TIMER_ONESHOT_MODE  | 100;  //�����ڣ���Ƶ100
    TIMER2->TCMPR = 8640000;
    TIMER_EnableInt(TIMER2); 	
	   TIMER_EnableWakeup(TIMER2);
    /* Enable Timer0  NVIC */
    NVIC_EnableIRQ(TMR2_IRQn);
	/* Start Timer0  counting */
    //TIMER_Start(TIMER2);
}





void TIMER3_Init()
{
	  CLK_EnableModuleClock(TMR3_MODULE);
    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3_S_HXT, 0);
    /* Open Timer0 in periodic mode, enable interrupt and 10 interrupt tick per second */
    TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 10);   //10HZ�����ж�
    TIMER_EnableInt(TIMER3); 	
    /* Enable Timer0  NVIC */
    NVIC_EnableIRQ(TMR3_IRQn);
	/* Start Timer0  counting */
    TIMER_Start(TIMER3);
}
/*---------------------------------------------------------------------------------------------------------*/
/* UART Test Sample                                                                                        */
/* Test Item                                                                                               */
/* It sends the received data to HyperTerminal.                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle TIMER0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void TMR0_IRQHandler(void)			  //10hz
{
  //if(TIMER_GetIntFlag(TIMER0) == 1)
    TIMER_ClearIntFlag(TIMER0);
	 	Gprs_Timeout_Count++;
	//if((dormantlevel==0)||(dormantlevel==2)||(dormant_wakeup==1)||(state))
	  //if((dormantlevel==0)||(dormant_wakeup==1)||(state))	
		if((dormantlevel!=1)||(dormant_wakeup==1)||(state))	
	 { 
/***************************************************************************/				
// 				Gprs_Timeout_Count++;
/***************************************************************************/				
				if( GprsConnectTcpState < 2 )//��������-1;��ʱ-0;
				 {      
						if(gprsNoConnect < 50*60) //5����δ��������
						{
							gprsNoConnect++;
						}            
						else	//����1����,�ر�GPRS��Դ;
						{
							GPRSPOWEROFF;
							delay_ns = 100;//10s
							gprsNoConnect = 0;	
              at_command_order = 0;//�������	
						 AtCommSendingState = COMMSENDED;	//���� ��������״̬
						memset((void *)strbuf,0,50); bufloc=0; 							
						}      
				 }	 
				 if(delay_ns == 50)//5s
				 {
						GPRSPOWERON;            
					  gprsNoConnect = 0; 
					  delay_ns--; 
				 }
				 else
				 {
					 if(delay_ns>0)
					 delay_ns--;
				 }
				 /**********///�������ݼ�ʱ
				 if(GprsRevTurn)
				 {
				 GprsRevTurntime++; 
					 if(GprsRevTurntime>30)//3s
					 {
						 GprsRevTurn=0;
           }
				 }
				 else
	       GprsRevTurntime=0;
				 /*****************/
				  tongxintime++;
	       if(tongxintime>=50)
	        tongxintime=50;
				 
				 //����� LED���Լ�ʱ��	
				 if(numdisp_testtime)
					 numdisp_testtime--;
          numdisp_test();
				 
	 }		 
}


/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle TIMER0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void TMR1_IRQHandler(void)  //1s
{
  TIMER_ClearIntFlag(TIMER1);
	 adctime++;
	
	//GPS�ն���ͨ��ʱ��  ������+��������
	gps_tem_time++;
	if(gps_tem_time>=60)//����
	 {
	  gps_tot_time++;
		 gps_tem_time=0;
	 }
	
	
	//if((dormantlevel==0)||(dormantlevel==2)||(dormant_wakeup==1))
	//if((dormantlevel==0)||(dormant_wakeup==1)||(dormant1stat<5))	
	//if((dormantlevel!=1)||(dormant_wakeup==1)||(dormant1stat<5))	
  if((dormantlevel==0)||(dormant_wakeup==1)||((dormant1stat<4)&&(dormantlevel==1))||((dormant3stat==0)&&(dormantlevel==2)))				
	{		
	 //GPS�ߵ�ƽ��������쳣��ʱ
	if(tongxintime<50)
	{
	  GPShighlevelchecktime++;
   if(GPShighlevelchecktime>=10)//10s
		GPShighlevelchecktime=10;
  }
	else
	GPShighlevelchecktime=0;	
	
	/*******/ //GPS����ʱ��
	GPSvoidtimes++;
	if(GPSvoidtimes>60)
	{
		GPSvoidtimes=0;
		GPSvoidtimem++;
		if(GPSvoidtimem>367200)//255��
			GPSvoidtimem=367200;
  }
	//CAN���߹��ϱ���ʱ��  PLC�е�12��������can����
	if(TerminalReply0x110x18.GPS.state1&BIT(5))
	{
	  CANvoidtimes++;
	if(CANvoidtimes>720)//12����
		CANvoidtimes=720;
  }
	else
		CANvoidtimes=0;
	
	//gps�����
	if((TerminalReply0x110x18.GPS.state1&(BIT(7)))&&(speed_s>300)&&(vibrationtime<60)&&(Parameterset.Parametersetitem.GPSwxs0x13>4))//��λ�ɹ� �ٶȴ���1km/h ����ʱ ��ʼ������� ����4����
	 gps_tem_lc+=speed_s;
	if(gps_tem_lc>=1000)
	{
		gps_tem_lc-=1000;
		gps_tot_lc++;
		gps_cur_lc++;
		if((Parameterset.Parametersetitem.wzhbcl0x41)&&(speed_s>555))//�ж��๦��
		gps_dj_lc++;//�������
		else
		gps_dj_lc=0;//�������	
  }
	//���ε�����̴���
	 if(CANID0280.zhuansu[4]<10)//>300ת
		gps_cur_lc=0;
	 
	//ACCon�ۼ�ʱ��
	if(TerminalReply0x110x18.GPS.state1&(BIT(5)))
	{	
	 ACCon_tem_time++;
	 if(ACCon_tem_time>=60)//����
	 {
		ACCon_tem_time=0;
		ACCon_tot_time++;
		ACCon_cur_time++;
   }
  }
	else
	{
		ACCon_tem_time=0;
		ACCon_cur_time=0;
  }
// 	//GPS�ն���ͨ��ʱ��  ������+��������
// 	gps_tem_time++;
// 	if(gps_tem_time>=60)//����
// 	 {
// 	  gps_tot_time++;
// 		 gps_tem_time=0;
// 	 }
  //��¼�������ĵȴ��ظ�270s
	  ZDParameter.LoginTimer++; 
	 if(ZDParameter.LoginTimer > 270)
		ZDParameter.LoginTimer = 270;
		
	// Timer1Count++;		
    if(ZDParameter.LoginReply)  
	 {	
     Timer1Count++;				 
		if((Timer1Count%(20)) == 0 )	   //20��
      TimeArrived0x13=1;//�ն��ϴ�CAN����			
		
		if((ZDParameter.TraceCount)&&(ZDParameter.IntervalTimer))//��������1��������ʱ�������� >0
		{
		  if((Timer1Count%(ZDParameter.IntervalTimer)) == 0 )
			{
				TimeArrivedcreg=1;//�Ȳ��վ	����
				TimeArrived0x11=1;//��λ��ѯ�Ļظ�
				ZDParameter.TraceCount--;
      }
    }
		else if((speed_s>555)&&(TerminalReply0x110x18.GPS.state1&(BIT(7))))//��λ�ɹ���ʼ�������       //��ʻ״̬�� 0.3m/s=1.08km/hִ��λ�û㱨����  0.555m/s=2km/h
		{
			if(Parameterset.Parametersetitem.wzhbcl0x41==2)//��ʱ���� ע���벻��
			{ 
				//if(((Timer1Count%(Parameterset.Parametersetitem.dssbsj0x42))==0)||((gps_tot_lc%(Parameterset.Parametersetitem.djsbjl0x43*10))==0))//30s	 20=200m
				if(((Timer1Count%(Parameterset.Parametersetitem.dssbsj0x42))==0)||(gps_dj_lc>=(Parameterset.Parametersetitem.djsbjl0x43*10)))//30s	 20=200m	
			  { 
					if(gps_dj_lc>=(Parameterset.Parametersetitem.djsbjl0x43*10))//	 20=200m	 
				  gps_dj_lc=0;
					
				TimeArrivedcreg=1;//�Ȳ��վ	����
				TimeArrived0x18=1;	
				}					
      }	
			else if(Parameterset.Parametersetitem.wzhbcl0x41==1)//����
			{
				//if((gps_tot_lc%(Parameterset.Parametersetitem.djsbjl0x43*10))==0)//	 20=200m
			 if(gps_dj_lc>=(Parameterset.Parametersetitem.djsbjl0x43*10))//	 20=200m	
			 { 
				gps_dj_lc=0;
				TimeArrivedcreg=1;//�Ȳ��վ	����
			  TimeArrived0x18=1;
			 }						
      }
			else if(Parameterset.Parametersetitem.wzhbcl0x41==0)//��ʱ
			{
				if((Timer1Count%(Parameterset.Parametersetitem.dssbsj0x42))==0)//30s	
				{ 
				TimeArrivedcreg=1;//�Ȳ��վ	����	
        TimeArrived0x18=1;
				}					
      }	
    }
		else if(autorep_loctime)//����ʻ״̬��
		{
			 //if((Timer1Count%(30)) == 0 )	   //30��
			 if((Timer1Count%(autorep_loctime)) == 0 )	   //Ĭ��240��
			 { 
				TimeArrivedcreg=1;//�Ȳ��վ	����
		    TimeArrived0x18=1;//�Զ��ر�
			 }
    }
		
		   //if((Timer1Count%(28800)) == 0 )	   //8H 60*60*8
			 if((Timer1Count%(7200)) == 0 )	   //2H 60*60*2=7200  ����2Сʱ   ����24h
		   TimeArrived0x28=1;//ÿ2Сʱ�ϴ�һ�θ�����
			 
			 
			//�ն��ϴ�������Ϣ  ÿ����1Сʱδ�յ�������ָ��  ��¼�ɹ����ʱ
		sendbjxx0x17time++;
		if(sendbjxx0x17time>=3600)
			sendbjxx0x17time=3600;

		//�ն��ϴ�������Ϣ �ȴ��ظ�ʱ�� 
     if(sendbjxx0x17repovertime)//Ӧ��15s	
			 sendbjxx0x17repovertime--;
    
    PA3=~PA3; //��¼ָʾ��
	 } 
	 else
	 { 
		PA3=0; 
		Timer1Count=0;	
	 }
	 
	 
	 //�ƹ���˸ʱ��
	 Timer11Count++;		
	 if(numdisp_testok)//�����LED���Լ����
	 { 
    //�̵���˸��������������� tcp
	 if(GprsConnectTcpState < 2)
		PC6=0; 
    else
		{	
      //PC6=~PC6;
			if((Timer11Count%2) == 0) //2��	
       PC6=0;
			 else
			 PC6=1; 
		}			
		//�����˸GPS��λ�ɹ�
   	if(TerminalReply0x110x18.GPS.state1&(BIT(7)))//�ɹ���λ
		{
		//PA6=~PA6; 
			if((Timer11Count%2) == 0) //2��	
       PA6=0;
			 else
			 PA6=1; 
		}
    else
    PA6=0;	
		
		//�Ƶ���˸�յ�GSM�ź�
		if(Parameterset.Parametersetitem.GSMxhqd0x12>10)
		{
			//PB15=~PB15;//ִ�д���
      if((Timer11Count%2) == 0) //2��	
       PB15=0;
			 else
			 PB15=1; 
    }
    else
    PB15=0;	
		//С������˸
		if((Timer11Count%2) == 0) //2��	
		{
     PB9=0;
		 numdisp8();//��8
		}
		else
		{
		PB9=1; 
    numdisp();	//�����л�
		}		
	 }
		//��Ч�񶯳���1����
		vibrationtime++;
   if(vibrationtime>=60)
		vibrationtime=60;
		//��Чת������5����
	 if((vibrationtime<60)&&(speed_s>555))//���� ʱ�ٴ���2km/h
	 {
		ferrytime++;
		if(ferrytime>=300)//5����
		ferrytime=300;
	 }
	 //��Чת������5����
	 if((vibrationtime==60)&&(speed_s<555))//���� ʱ�ٴ���2km/h
	 {
		if(ferrytime)//5����
		ferrytime--;
	 }
	  
   //�յ�CAN��Ϣ
	 if(CanReceiveEndFlag==1)
     {     
		  PA4=~PA4; 
      CanReceiveEndFlag=0;
     }
		 else
			PA4=0; 
	 }		 
	 //ACCOFFʱ���ʱ
// 	if((!(TerminalReply0x110x18.GPS.state1&BIT(5)))&&(voltage<241)&&(vibrationtime>=60)
// 		&&(smsnumber<4)&&(!readsmsnumok)&&(!dxbjhmflag)&&(at_command_order >=10)&&(recgprscom==0 ))//ACC OFF ͬʱ��ѹ����24.1v ͬʱ���� �޶��ź�GPRS����
	if((!(TerminalReply0x110x18.GPS.state1&BIT(5)))&&(voltage<241)&&(vibrationtime>=60)
		&&(smsnumber<4)&&(!readsmsnumok)&&(!dxbjhmflag)&&(recgprscom==0 ))//ACC OFF ͬʱ��ѹ����24.1v ͬʱ���� �޶��ź�GPRS����
	 {	 dormant_ACCofftime++;
		 if(dormant_ACCofftime>=600)//10����
			 dormant_ACCofftime=600;
		 //if(voltage<Parameterset.Parametersetitem.zdqyz0x1D)//Ƿѹ�����Ͻ�������
		 if((TerminalReply0x110x18.GPS.state3&BIT(1))||(TerminalReply0x110x18.GPS.state3&BIT(2)))//�ϵ��Ƿѹ����
		 { if(dormant_ACCofftime<480)
			 dormant_ACCofftime=480; 
		 }
	}
	else
	{
		//dormant_ACCofftime=0;
	  dormant_ACCofftime=580;
		if(recgprscom) recgprscom=0;
	}
	
	if(((dormantlevel)||(dormant_wakeup))&&(voltage>245))//�����߻���������
	{
		time_245v++;
		if(time_245v>=600)
		time_245v=600;
  }
	else
	time_245v=0;	
	
	//����ʱ�䵹��ʱ
	if(wakeuptime2min)//120 2����
	 wakeuptime2min--;
	else
	 dormant_wakeup=0;	
	//�������߻���ʱ��
	if((dormantlevel==1)&&(dormant_wakeup==0)&&(dormant1stat==5))
	{
		dormanttime++;
		//if(dormanttime>=720000)//2Сʱ
		//if(dormanttime>=20)//20s
		if(dormanttime>=60)//60s	
		{
			dormanttime=0;
			send0xAA=1;
			//TerminalReply0x17.baojingflag=0xAA;
// 			TimeArrivedcreg=1;//�Ȳ��վ	����
// 		  TimeArrived0x17=1;//�ϴ�������Ϣ
			//TimeArrived0x28=1;//�ϴ�������
// 			//�������߻���
// 			//if((dormantlevel==1)&&(dormant_wakeup==0))
// 			if(dormantlevel==1)
// 		  {
		  wakeuptime2min=120;
		  dormant_wakeup=2;//ִ�л��Ѳ���
		// }
    }		
	}
	else
		dormanttime=0;	
	
	//��������С������˸
	 if((dormant1stat==5)&&(dormantlevel==1))
	 PB9=~PB9;
		//GPRS����ʧ�ܼ�ʱ
	 if(at_command_order<17)
		 gprslinkfailtime++;
	 else
		 gprslinkfailtime=0;
	 if(gprslinkfailtime>=120)
		 gprslinkfailtime=120;
}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle TIMER0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void TMR2_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER2) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER2);

       
    }

    if(TIMER_GetWakeupFlag(TIMER2) == 1)
    {
        /* Clear Timer0 wake-up flag */
        TIMER_ClearWakeupFlag(TIMER2);
    }
			send0xAA=1;//ʱ�份�Ѻ󱨾�
// 		if(dormantlevel==1)
// 		{
// 		 wakeuptime2min=120;
// 		 dormant_wakeup=2;//ִ�л��Ѳ���
// 		}
}


void TMR3_IRQHandler(void)
{
	   TIMER_ClearIntFlag(TIMER3);
// 		
// 			  if(!shisu_flag) 				 
// 	     {
// 		    Shisu.temppulse = Shisu.pulse;
// 		    shisu_flag=1;				 	
// 	     }
	
}





void GPIO_INIT()
{
	 /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set GPB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD;

	    /* Set GPB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
   SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB4_Msk | SYS_GPB_MFP_PB5_Msk);
    SYS->GPB_MFP |= SYS_GPB_MFP_PB4_UART1_RXD | SYS_GPB_MFP_PB5_UART1_TXD;

// 	  SYS->GPD_MFP &= ~(SYS_GPD_MFP_PD14_Msk | SYS_GPD_MFP_PD15_Msk);
//     SYS->GPD_MFP |= SYS_GPD_MFP_PD14_UART2_RXD | SYS_GPD_MFP_PD15_UART2_TXD;

	    /* Set PD multi-function pins for CANTX0, CANRX0 */
    SYS->GPD_MFP &= ~(SYS_GPD_MFP_PD6_Msk | SYS_GPD_MFP_PD7_Msk);
    SYS->GPD_MFP |= SYS_GPD_MFP_PD6_CAN0_RXD | SYS_GPD_MFP_PD7_CAN0_TXD;
	

    GPIO_SetMode(PA, BIT0, GPIO_PMD_INPUT);//BATJC
    GPIO_SetMode(PA, BIT1, GPIO_PMD_INPUT);//DDJC
		GPIO_SetMode(PA, BIT2, GPIO_PMD_INPUT);//FULL
    GPIO_SetMode(PA, BIT3, GPIO_PMD_QUASI);PA3=0;//LED_LOGIN
    GPIO_SetMode(PA, BIT4, GPIO_PMD_QUASI);PA4=0;//LED_CAN
    GPIO_SetMode(PA, BIT5, GPIO_PMD_INPUT);//TEMPJC
    GPIO_SetMode(PA, BIT6, GPIO_PMD_QUASI);PA6=0;//LEDR1
    GPIO_SetMode(PA, BIT7, GPIO_PMD_INPUT);//VREF
    //GPIO_SetMode(PA, BIT8, GPIO_PMD_INPUT);//IN33
		GPIO_SetMode(PA, BIT8, GPIO_PMD_QUASI);PA8=0;//IN33  ���͵�ƽ
    GPIO_SetMode(PA, BIT9, GPIO_PMD_INPUT);//IN11  
    GPIO_SetMode(PA, BIT10, GPIO_PMD_OUTPUT); //PW_GPS
	  PA10 = 0;//����
    GPIO_SetMode(PA, BIT11, GPIO_PMD_INPUT); //REV
		GPIO_SetMode(PA, BIT12, GPIO_PMD_QUASI);PA12=1; //ct_33v
		GPIO_SetMode(PA, BIT13, GPIO_PMD_QUASI);PA13=1; //ct_18v
		GPIO_SetMode(PA, BIT14, GPIO_PMD_INPUT);//BATJC3��Ϊ��ش��ڼ��˿�
		//GPIO_SetMode(PA, BIT15, GPIO_PMD_INPUT); //ct_24v
    //GPIO_SetMode(PA, BIT15, GPIO_PMD_QUASI); //ct_24v
		//GPIO_SetMode(PA, BIT15, GPIO_PMD_OPEN_DRAIN ); //ct_24v
		GPIO_SetMode(PA, BIT15, GPIO_PMD_OUTPUT);//ct_24v
	  PA15 = 0;//�ⲿ����
	 
	 
	  GPIO_SetMode(PB, BIT0, GPIO_PMD_INPUT);
		GPIO_SetMode(PB, BIT1, GPIO_PMD_INPUT);
		GPIO_SetMode(PB, BIT2, GPIO_PMD_QUASI);//ct_5156
		PB2 = 0;//ͣ��
		GPIO_SetMode(PB, BIT3, GPIO_PMD_OUTPUT);//PW_GPRS
		PB3 = 0;//����
		GPIO_SetMode(PB, BIT6, GPIO_PMD_INPUT); //IN22
		GPIO_SetMode(PB, BIT7, GPIO_PMD_INPUT); //IN44
		GPIO_SetMode(PB, BIT9, GPIO_PMD_QUASI);PB9=0;//DP1
		GPIO_SetMode(PB, BIT12, GPIO_PMD_INPUT);//VTBJC1
		//GPIO_EnableInt(PB, 12, GPIO_INT_FALLING);//��1�½����жϼ��
		GPIO_EnableInt(PB, 12, GPIO_INT_BOTH_EDGE);//��1�½����жϼ��
		//GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_256);
		GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_1024 );
	  GPIO_ENABLE_DEBOUNCE(PB, BIT12);      
    NVIC_EnableIRQ(GPAB_IRQn);
		
		GPIO_SetMode(PB, BIT14, GPIO_PMD_INPUT);//SUNJC2
		GPIO_SetMode(PB, BIT15, GPIO_PMD_QUASI);PB15=0;//LEDY1
   

     GPIO_SetMode(PC, BIT0, GPIO_PMD_QUASI);PC0=1;//GD25_CS
		 GPIO_SetMode(PC, BIT1, GPIO_PMD_QUASI);PC1=1;//GD25_SCK
     GPIO_SetMode(PC, BIT2, GPIO_PMD_INPUT);//GD25_SO
		 GPIO_SetMode(PC, BIT3, GPIO_PMD_QUASI);PC3=1;//GD25_SI
     //GPIO_SetMode(PC, BIT6, GPIO_PMD_QUASI);PC6=0;//LEDG1
		 GPIO_SetMode(PC, BIT6, GPIO_PMD_OUTPUT);PC6=0;//LEDG1
		 GPIO_SetMode(PC, BIT7, GPIO_PMD_QUASI);PC7=0;//A1
		 GPIO_SetMode(PC, BIT8, GPIO_PMD_QUASI);PC8=0;//D1
		 GPIO_SetMode(PC, BIT9, GPIO_PMD_QUASI);PC9=0;//E1
		 GPIO_SetMode(PC, BIT10, GPIO_PMD_QUASI);PC10=0;//G1
		 GPIO_SetMode(PC, BIT11, GPIO_PMD_QUASI);PC11=0;//F1
		 GPIO_SetMode(PC, BIT14, GPIO_PMD_QUASI);PC14=0;//C1
     GPIO_SetMode(PC, BIT15, GPIO_PMD_QUASI);PC15=0;//B1
  
	 
	  GPIO_SetMode(PD, BIT14, GPIO_PMD_OUTPUT);//TCAN1042 STB
		PD14 = 0;//0 ������1����
		GPIO_SetMode(PD, BIT15, GPIO_PMD_INPUT);//sunjc1
		
		//GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT);//CT_BATT
    GPIO_SetMode(PE, BIT5, GPIO_PMD_OPEN_DRAIN ); //CT_BATT
		PE5 = 1;//�ⲿ�������
		GPIO_SetMode(PF, BIT4, GPIO_PMD_INPUT);//VTBJC3��Ϊ��ش��ڼ��˿�
		GPIO_SetMode(PF, BIT5, GPIO_PMD_INPUT);//VTBJC2
		//GPIO_EnableInt(PF, 5, GPIO_INT_FALLING);//��2�½����жϼ��
		GPIO_EnableInt(PF, 5, GPIO_INT_BOTH_EDGE);//��2�½����жϼ��
		GPIO_ENABLE_DEBOUNCE(PF, BIT5);
    NVIC_EnableIRQ(GPCDEF_IRQn);
		GPIO_SetMode(PF, BIT8, GPIO_PMD_INPUT);//IN23.75V
		
// 		
// 		//����
//     GPIO_SetMode(PB, BIT14, GPIO_PMD_INPUT);
//     GPIO_EnableEINT0(PB, 14, GPIO_INT_LOW);
//     NVIC_EnableIRQ(EINT0_IRQn);
	 
// 	 //ʱ��
// 	 SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB8_Msk );
// 	 SYS->GPB_MFP |= SYS_GPB_MFP_PB8_GPIO;
// 	GPIO_SetMode(PB, BIT8, GPIO_PMD_INPUT);
// 	GPIO_EnableEINT0(PB, 8, GPIO_INT_FALLING);
// 	//GPIO_EnableEINT0(PB, 8, GPIO_INT_RISING );
// 	 NVIC_EnableIRQ(GPAB_IRQn);
// 	
	  	
}

  void GPAB_IRQHandler(void) 
  {
		
		if(GPIO_GET_INT_FLAG(PA, BIT8)) //acc��
    {
			GPIO_CLR_INT_FLAG(PA, BIT8);
			
// 			//�������߻���
// 			//if((dormantlevel==1)&&(dormant_wakeup==0))
// 			if(dormantlevel==1)	
// 		  {
// 		  //wakeuptime2min=120;
// 		  dormant_wakeup=2;//ִ�л��Ѳ���
// 		  }
    }	
		
		if(GPIO_GET_INT_FLAG(PB, BIT7)) //acc��
    {
			GPIO_CLR_INT_FLAG(PB, BIT7);
// 			vibrationtime=0;
// 			//�������߻���
// 			//if((dormantlevel==1)&&(dormant_wakeup==0))
// 			if(dormantlevel==1)	
// 		  {
// 		  //wakeuptime2min=120;
// 		  dormant_wakeup=2;//ִ�л��Ѳ���
// 		  }
    }	
		
		
		if(GPIO_GET_INT_FLAG(PB, BIT12)) //��1
    {
			GPIO_CLR_INT_FLAG(PB, BIT12);
			vibrationtime=0;
			//�������߻���
			//if((dormantlevel==1)&&(dormant_wakeup==0))
			if(dormantlevel==1)	
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//ִ�л��Ѳ���
		  }
    }	
		
    	if(GPIO_GET_INT_FLAG(PB, BIT14))
    {
			GPIO_DisableInt(PB, 14);//���ǹظǿ��ж�
			GPIO_CLR_INT_FLAG(PB, BIT14);
			
			//�������߻���
			if((dormantlevel==1)&&(dormant_wakeup==0))
		 //if(dormantlevel==1)
		  {
		  wakeuptime2min=120;
		  dormant_wakeup=2;//ִ�л��Ѳ���
				
		  }
    }			
  }
  void GPCDEF_IRQHandler(void) 
  {
		if(GPIO_GET_INT_FLAG(PF, BIT5))//�񶯼��2
    {
			GPIO_CLR_INT_FLAG(PF, BIT5);
			vibrationtime=0;
			//�������߻���
			//if((dormantlevel==1)&&(dormant_wakeup==0))
			if(dormantlevel==1)
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//ִ�л��Ѳ���
		 }
    }	
		if(GPIO_GET_INT_FLAG(PF, BIT8))//�ﵽ23.1v
    {
			GPIO_DisableInt(PF, 8);//IN23.75V �͵�ƽ�ж� ���ɱ��������
			GPIO_CLR_INT_FLAG(PF, BIT8);
			
			//�������߻���
			if((dormantlevel==1)&&(dormant_wakeup==0))
		 //if(dormantlevel==1)
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//ִ�л��Ѳ���
				
		  }
    }	
		if(GPIO_GET_INT_FLAG(PD, BIT15))
    {
			GPIO_DisableInt(PD, 15);//���ǹظǿ��ж�
			GPIO_CLR_INT_FLAG(PD, BIT15);
			
			//�������߻���
			if((dormantlevel==1)&&(dormant_wakeup==0))
		 //if(dormantlevel==1)
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//ִ�л��Ѳ���
				
		  }
    }			
  }

	
	
	
//Ϊ��߷�ӦACC����ٶ�
void accstate_check(void)
{ 
	//sta15
	if(!Parameterset.Parametersetitem.PLCsrk0x20)//PLC�ϵ����Ч
	{
		if(PB7) 
		{
			if(checkcount&BIT(0))//���2��ȷ��
			TerminalReply0x110x18.GPS.state1 |= BIT(5);
			checkcount|= BIT(0);
		}	
		else
		{
			checkcount&= ~BIT(0);
		  TerminalReply0x110x18.GPS.state1 &= ~BIT(5);
		}
  }
	else//����Ч
	{
		if(!PA8) 
		{
			if(checkcount&BIT(0))//���2��ȷ��
			TerminalReply0x110x18.GPS.state1 |= BIT(5);
			checkcount|= BIT(0);
		}	
		else
		{	
			checkcount&= ~BIT(0);
		 TerminalReply0x110x18.GPS.state1 &= ~BIT(5);
		}
  }
}	
	
	
void state_check(void)
{
	//sta17
// 	//sta15
// 	if(!Parameterset.Parametersetitem.PLCsrk0x20)//PLC�ϵ����Ч
// 	{
// 		if(PB7) 
// 		{
// 			if(checkcount&BIT(0))//���2��ȷ��
// 			TerminalReply0x110x18.GPS.state1 |= BIT(5);
// 			checkcount|= BIT(0);
// 		}	
// 		else
// 		{
// 			checkcount&= ~BIT(0);
// 		  TerminalReply0x110x18.GPS.state1 &= ~BIT(5);
// 		}
//   }
// 	else//����Ч
// 	{
// 		if(!PA8) 
// 		{
// 			if(checkcount&BIT(0))//���2��ȷ��
// 			TerminalReply0x110x18.GPS.state1 |= BIT(5);
// 			checkcount|= BIT(0);
// 		}	
// 		else
// 		{	
// 			checkcount&= ~BIT(0);
// 		 TerminalReply0x110x18.GPS.state1 &= ~BIT(5);
// 		}
//   }
		
	
	//sta11
	//GPS���߹����ж�
	if(!(TerminalReply0x110x18.GPS.state1&(BIT(7))))//û�гɹ���λ
	{
	 if(GPSvoidtimem>10)//10����
	 {
		 TerminalReply0x110x18.GPS.state1 |= BIT(1);//GPS���߹���
		 if((!baGPSantrecord)&&(!(memparamset.memparam.flagchange&(BIT(1)))))//���θ���һ�Σ������ϴο���GPS��������
		 {
		  memparamset.memparam.baGPSantnum++;//��GPS���ߴ���
			baGPSantrecord=1;
			memparamset.memparam.flagchange|=BIT(1);
		 }
	 }
	}
	else
	{//һ������ȫ�����
		GPSvoidtimes=0;
		GPSvoidtimem=0;
		TerminalReply0x110x18.GPS.state1 &= ~BIT(1);
		baGPSantrecord=0;//�屾��
		memparamset.memparam.flagchange&=~BIT(1);//���ϴ�
	}
	//sta10
	if(tongxintime<50)
	{
		TerminalReply0x110x18.GPS.state1 |= BIT(0);//���������
		CANvoidtimes=0;
	}
	else
	 TerminalReply0x110x18.GPS.state1 &= ~BIT(0);	
	//sta27
	if(memparamset.memparam.simkaghcs)//sim��������������0
	TerminalReply0x110x18.GPS.state2 |= BIT(7);//��������	
	else
	TerminalReply0x110x18.GPS.state2 &= ~BIT(7);		
// 	//sta26
// 	if((PB14)&&(PD15))//��
// 	{
// 		if(checkcount&BIT(1))//���2��ȷ��
// 		TerminalReply0x110x18.GPS.state2 &= ~BIT(6);
// 		checkcount|= BIT(1);
// 	}
//   else	
// 	{
// 		checkcount&= ~BIT(1);
// 	  TerminalReply0x110x18.GPS.state2 |= BIT(6);//���Ǳ���	
// 	}
	//sta26
	if((PB14)&&(PD15))//��
	{
		checkcount&= ~BIT(1);
		TerminalReply0x110x18.GPS.state2 &= ~BIT(6);
	}
  else	
	{
		
		if(checkcount&BIT(1))//���2��ȷ��
		{
	   TerminalReply0x110x18.GPS.state2 |= BIT(6);//���Ǳ���	
// 			//�������߻���
// 			if((dormantlevel==1)&&(dormant_wakeup==0))
// 			//if(dormantlevel==1)		
// 		  {
// 		  wakeuptime2min=120;
// 		  dormant_wakeup=2;//ִ�л��Ѳ���
// 		  }
		}	
		 checkcount|= BIT(1);
	}
	
	
	
	//sta25
	if(memparamset.memparam.baSIMnum)// ��SIM������
	TerminalReply0x110x18.GPS.state2 |= BIT(5);//�ο�����	
	else
	TerminalReply0x110x18.GPS.state2 &= ~BIT(5);	
	//sta36 CAN������
	if(Parameterset.Parametersetitem.BaudRate0x07==125)
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(6);
  else
   TerminalReply0x110x18.GPS.state3 |= BIT(6);		
	//sta34  ���õ��Ƿѹ����
	if((volt_bat<37)&&(!(TerminalReply0x110x18.GPS.state3&BIT(3))))//3.7v �ұ��õ�ش�������¼��
	{
	 if(checkcount&BIT(2))//���2��ȷ��
	 TerminalReply0x110x18.GPS.state3 |= BIT(4);	
	 checkcount|= BIT(2);
	}		
	else
	{
	 checkcount&= ~BIT(2);
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(4);
	}
	
// 	//sta33 ���õ�ضϵ籨��  �����繤���򱸵�����ʱ
// 	if(volt_bat<2)//2v
// 	TerminalReply0x110x18.GPS.state3 |= BIT(3);		
// 	else
// 	TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
	//sta32 ����ԴǷѹ����
	//if(voltage<=225)//22.5v
	if((voltage<=Parameterset.Parametersetitem.zdqyz0x1D)&&(voltage>90))//�ϵ粻��Ƿѹ
	{
		if(checkcount&BIT(4))//���2��ȷ��
		{
	  TerminalReply0x110x18.GPS.state3 |= BIT(2);
// 		 //�������߻���
// 			if((dormantlevel==1)&&(dormant_wakeup==0))
// 			//if(dormantlevel==1)	
// 		  {
// 		  wakeuptime2min=120;
// 		  dormant_wakeup=2;//ִ�л��Ѳ���
// 		  }	
		}			
		checkcount|= BIT(4);
	}		
	else
	{
		checkcount&= ~BIT(4);
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(2);
	}
	//sta31 ����Դ�ϵ籨��
	if(voltage<=50)//5v
	{	
		if(checkcount&BIT(5))//���2��ȷ��
		{
	  TerminalReply0x110x18.GPS.state3 |= BIT(1);
// 			//�������߻���
// 			if((dormantlevel==1)&&(dormant_wakeup==0))
// 			//if(dormantlevel==1)	
// 		  {
// 		  wakeuptime2min=120;
// 		  dormant_wakeup=2;//ִ�л��Ѳ���
// 		  }
		}
    checkcount|= BIT(5);		
	}		
	else
	{
		checkcount&= ~BIT(5);
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(1);
	}
	//sta30 ���߹��ϱ���
	if(CANvoidtimes>=720)//12����
	{
	 TerminalReply0x110x18.GPS.state3 |=BIT(0);
		numdispflag|=BIT(5);//5 CAN�����쳣��ʾ
	}
	 else
	 {
   TerminalReply0x110x18.GPS.state3 &=~BIT(0);
   numdispflag&=~BIT(5);//5 CAN�����쳣	����ʾ	 
   }
  //sta44 ����Ч1 0
   if(vibrationtime<60)//��Ч�񶯳���1����
	 TerminalReply0x110x18.GPS.state4 |=BIT(4);
	 else
   TerminalReply0x110x18.GPS.state4 &=~BIT(4);	 
	//sta42 ����״̬ 1���� 0����
	 if(dormantlevel==0)
	 TerminalReply0x110x18.GPS.state4 &=~BIT(2);
   else
   TerminalReply0x110x18.GPS.state4 |=BIT(2);			 
	//sta40 ת��״̬ 1
   if(ferrytime>=300)//5����
	 TerminalReply0x110x18.GPS.state4 |=BIT(0);
	 else if(!ferrytime)
   TerminalReply0x110x18.GPS.state4 &=~BIT(0);	
	 
	 
	 
	 //GPS�ߵ�ƽ��������쳣
	  if(TerminalReply0x110x18.GPS.state1&BIT(5))
			GPShighlevelchecktime=0;
		if(GPShighlevelchecktime>=10)//10s
			numdispflag|=BIT(6);//6GPS�ߵ�ƽ��������쳣  ��ʾ
		else
			numdispflag&=~BIT(6);
	 
	  
}

//������Ϣ����
void bjxx_content(void)
{
	//�ն��ϴ�������Ϣ  ÿ����1Сʱδ�յ�������ָ��
	if((!TerminalReply0x17.baojingflag))
	{
		//if(sendbjxx0x17time>3600)
		if((sendbjxx0x17time>(Parameterset.Parametersetitem.AAbjsjset0x1c*60))||(send0xAA))	
		{ send0xAA=0;
			TerminalReply0x17.baojingflag=0xAA;
			TimeArrivedcreg=1;//�Ȳ��վ	����
		  TimeArrived0x17=1;//�ϴ�������Ϣ
			sendbjxx0x17time=0;
			return;
    }	
  //GPS���߹���
   if(TerminalReply0x110x18.GPS.state1&BIT(1))
	{
		if(!(bjflag&BIT(0)))
		{
		 bjflag|=BIT(0);
		 TerminalReply0x17.baojingflag=0x81;//GPS���߹��ϱ���
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(0))//����������ȡ��������û�б�����ȡ��
		{
		 bjflag&=~BIT(0);
		 TerminalReply0x17.baojingflag=0x01;//GPS���߹��Ͼ�����
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	
	//ACC�ϵ籨��
    if(TerminalReply0x110x18.GPS.state1&BIT(5))
	{
		if(!(bjflag&BIT(2)))
		{
		 bjflag|=BIT(2);
		 TerminalReply0x17.baojingflag=0x85;//ACC�ϵ籨��
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(2))
		{
		 bjflag&=~BIT(2);
		 TerminalReply0x17.baojingflag=0x05;//ACC�ϵ籨������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	
  //SIM���ο���־
	if(TerminalReply0x110x18.GPS.state2&BIT(5))
	{
		if(!(bjflag&BIT(8)))
		{
		 bjflag|=BIT(8);
		 TerminalReply0x17.baojingflag=0x8D;//SIM���ο�����
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(8))
		{
		 bjflag&=~BIT(8);
		 TerminalReply0x17.baojingflag=0x0D;//SSIM���ο���������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }		
  //���Ǳ���
    if(TerminalReply0x110x18.GPS.state2&BIT(6))
	{
		if(!(bjflag&BIT(9)))
		{
			/**********/
			//�������߻���
			if((dormantlevel==1)&&(dormant_wakeup==0))
			//if(dormantlevel==1)	
		  {
				if((dormant1stat>0)&&(dormant1stat<5)) 
					return;//����ִ�����߳���ʱ �����
				if(dormant1stat==5)
				{
		    //wakeuptime2min=120;
		    dormant_wakeup=2;//ִ�л��Ѳ���
				}
		  }	
		 bjflag|=BIT(9);
			memparamset.memparam.kaigainum++;//���Ǵ���
		 TerminalReply0x17.baojingflag=0x8E;//���Ǳ���
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			
			return;
		}
  }
	else
	{
		if(bjflag&BIT(9))
		{
		 bjflag&=~BIT(9);
		 TerminalReply0x17.baojingflag=0x0E;//���Ǳ�������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	

  //SIM��������
	if(TerminalReply0x110x18.GPS.state2&BIT(7))
	{
		if(!(bjflag&BIT(10)))
		{
		 bjflag|=BIT(10);
		 TerminalReply0x17.baojingflag=0x8f;//SIM��������
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(10))
		{
		 bjflag&=~BIT(10);
		 TerminalReply0x17.baojingflag=0x0f;//SIM������������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }		
	
	
	
	//���߹��ϱ���
   if(TerminalReply0x110x18.GPS.state3&BIT(0))
	{
		if(!(bjflag&BIT(11)))
		{
		 bjflag|=BIT(11);
		 TerminalReply0x17.baojingflag=0x90;//���߹��ϱ���
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(11))
		{
		 bjflag&=~BIT(11);
		 TerminalReply0x17.baojingflag=0x10;//���߹��ϱ�������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	

  //����Դ�ϵ籨��
   if(TerminalReply0x110x18.GPS.state3&BIT(1))
	{
		if(!(bjflag&BIT(12)))
		{
			/**********/
			//�������߻���
			if((dormantlevel==1)&&(dormant_wakeup==0))
			//if(dormantlevel==1)	
		  {
				if((dormant1stat>0)&&(dormant1stat<5)) 
					return;//����ִ�����߳���ʱ �����
				if(dormant1stat==5)
				{
		    //wakeuptime2min=120;
		    dormant_wakeup=2;//ִ�л��Ѳ���
				}
		  }	
		 bjflag|=BIT(12);
		 TerminalReply0x17.baojingflag=0x91;//����Դ�ϵ籨��
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			
			return;
		}
  }
	else
	{
		if(bjflag&BIT(12))
		{
		 bjflag&=~BIT(12);
		 TerminalReply0x17.baojingflag=0x11;//����Դ�ϵ籨������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	
  //����ԴǷѹ����
   if(TerminalReply0x110x18.GPS.state3&BIT(2))
	{ 
		/**********/
			//�������߻���
			if((dormantlevel==1)&&(dormant_wakeup==0))
			//if(dormantlevel==1)	
		  {
				if((dormant1stat>0)&&(dormant1stat<5)) 
					return;//����ִ�����߳���ʱ �����
				if(dormant1stat==5)
				{
		    //wakeuptime2min=120;
		    dormant_wakeup=2;//ִ�л��Ѳ���
				}
		  }	
		if(!(bjflag&BIT(13)))
		{
		 bjflag|=BIT(13);
		 TerminalReply0x17.baojingflag=0x92;//����ԴǷѹ����
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			
			return;
		}
  }
	else
	{
		if(bjflag&BIT(13))
		{
		 bjflag&=~BIT(13);
		 TerminalReply0x17.baojingflag=0x12;//����ԴǷѹ��������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	

   //���õ�ضϵ籨��
   if(TerminalReply0x110x18.GPS.state3&BIT(3))
	{
		if(!(bjflag&BIT(14)))
		{
		 bjflag|=BIT(14);
		 TerminalReply0x17.baojingflag=0x93;//���õ�ضϵ籨��
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(14))
		{
		 bjflag&=~BIT(14);
		 TerminalReply0x17.baojingflag=0x13;//���õ�ضϵ籨������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	

   //���õ��Ƿѹ����
   if(TerminalReply0x110x18.GPS.state3&BIT(4))
	{
		if(!(bjflag&BIT(15)))
		{
		 bjflag|=BIT(15);
		 TerminalReply0x17.baojingflag=0x94;//���õ��Ƿѹ����
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(15))
		{
		 bjflag&=~BIT(15);
		 TerminalReply0x17.baojingflag=0x14;//���õ��Ƿѹ��������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	
  //CAN�����ʱ仯����  ֻ����������
	if(canBaudRatechange)
	{
		 canBaudRatechange=0;
		 TerminalReply0x17.baojingflag=0x97;//���õ��Ƿѹ����
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
  }
	
	//����  ֻ����������
	 if(TerminalReply0x110x18.GPS.state4&BIT(2))
	{
		if(!(bjflag&BIT(19)))
		{
		 bjflag|=BIT(19);
		 TerminalReply0x17.baojingflag=0x98;//���߱���
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(19))
		{
		 bjflag&=~BIT(19);
		 TerminalReply0x17.baojingflag=0x18;//���߱�������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	
	
	
	//sta40 ת��״̬ 
	 if(TerminalReply0x110x18.GPS.state4&BIT(0))
	{
		if(!(bjflag&BIT(20)))
		{
		 bjflag|=BIT(20);
		 TerminalReply0x17.baojingflag=0xA0;//ת������
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(20))
		{
		 bjflag&=~BIT(20);
		 TerminalReply0x17.baojingflag=0x20;//ת����������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	
	
  //sta44 ����Ч1 0
	if(TerminalReply0x110x18.GPS.state4&BIT(4))
	{
		if(!(bjflag&BIT(21)))
		{
		 bjflag|=BIT(21);
		 TerminalReply0x17.baojingflag=0xA1;//�񶯱���
		//TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }
	else
	{
		if(bjflag&BIT(21))
		{
		 bjflag&=~BIT(21);
		 TerminalReply0x17.baojingflag=0x21;//�񶯱�������
		 //TimeArrivedcreg=1;//�Ȳ��վ	����
		 TimeArrived0x17=1;//�ϴ�������Ϣ
			return;
		}
  }	
	check_baojingflag++;
  if(check_baojingflag>=3)
   check_baojingflag=3;		
 }
else
{
	check_baojingflag=0;
	//������©��������б������ݣ������ϴ�������Ϣ��־��������������߲����������룬����δ�ҵ�ԭ��
	if((sendbjxx0x17count==0)&&(TimeArrived0x17==0))
		TimeArrived0x17=1;
}
}

//��������ϢδӦ��
void bjxx_noreply(void)
{
		//�ն��ϴ�������Ϣ �ȴ��ظ� �������ݲ�ͬ �ظ���ͬ
		if(sendbjxx0x17count)//��¼���ʹ���
		{
     if(!sendbjxx0x17repovertime)//Ӧ��15s	
		 {
			 if(sendbjxx0x17count>=3)//��������
			 {
				 
				 
				 if(state==0)
				 {
					 sendbjxx0x17count=0;  //©���ڴ˴�
					 TimeArrived0x17=1;//�ϴ�������Ϣ
					if(dormantlevel!=0)//������״̬�£�����3���޻�Ӧ��Ϊ����ʧ�ܣ�׼����������
					{
						fwq_connectfail=1;//����������ʧ��
						return;
					} 
				 //sendbjxx0x17count=0;
				 ZDParameter.LoginReply=0;//����ʧ�ܺ����µ�¼
				 ZDParameter.LoginTimer =270;//�������µ�¼
				 GprsConnectTcpState=0;
				 at_command_order = 10;	 
				// TimeArrived0x17=1;//�ϴ�������Ϣ
					ZDParameter.zdliushuinum=0;//��ˮ����
				 }
       }
        else
        {
					//TimeArrivedcreg=1;//�Ȳ��վ	����
		      TimeArrived0x17=1;//�ϴ�������Ϣ
        }					
     } 
		}
}

 

void  initParameter_setitem(void)
{ 
	uint8_t i;
	SYS_UnlockReg();
	DF_ReadParameter_setitem();//��ȡ����
	 SYS_LockReg();
	for(i=0;i<192;i++)
	{
		Parameterset.Parameter_setitemdata[i]=Parameter_setitemdata[i];
  }
	//0x00

	//0x03
	
	if((Parameterset.Parametersetitem.hbsc0x03[0]==0xff)&&(Parameterset.Parametersetitem.hbsc0x03[1]==0xff))
	{
		Parameterset.Parametersetitem.hbsc0x03[0]=0x0;
		Parameterset.Parametersetitem.hbsc0x03[1]=240;
  }
	//0x04
	if((Parameterset.Parametersetitem.fwqIP0x04[0]==0xff)&&(Parameterset.Parametersetitem.fwqIP0x04[1]==0xff))
	{
		Parameterset.Parametersetitem.fwqIP0x04[0]=222;
		Parameterset.Parametersetitem.fwqIP0x04[1]=240;
		Parameterset.Parametersetitem.fwqIP0x04[2]=204;
		Parameterset.Parametersetitem.fwqIP0x04[3]=195;
		Parameterset.Parametersetitem.fwqIP0x04[4]=26;
		Parameterset.Parametersetitem.fwqIP0x04[5]=3;
  }
	//����������IP
	if((Parameterset.Parametersetitem.sjfwqIP0x04[0]==0xff)&&(Parameterset.Parametersetitem.sjfwqIP0x04[1]==0xff))
	{
		Parameterset.Parametersetitem.sjfwqIP0x04[0]=222;
		Parameterset.Parametersetitem.sjfwqIP0x04[1]=240;
		Parameterset.Parametersetitem.sjfwqIP0x04[2]=204;
		Parameterset.Parametersetitem.sjfwqIP0x04[3]=197;
		Parameterset.Parametersetitem.sjfwqIP0x04[4]=88;
		Parameterset.Parametersetitem.sjfwqIP0x04[5]=69;
  }

	
	//0x07
	if(Parameterset.Parametersetitem.BaudRate0x07==0xff)
	 Parameterset.Parametersetitem.BaudRate0x07=250;
	
	//0x11 IMSI
	
	//0x15
	
	if(Parameterset.Parametersetitem.ycsj0x15==0xff)
	Parameterset.Parametersetitem.ycsj0x15=0;//������
  //0x16
	if(Parameterset.Parametersetitem.sdms0x16==0xff)
	Parameterset.Parametersetitem.sdms0x16=1;//����ʡ��ģʽ
  //0x1B
	if(Parameterset.Parametersetitem.xtbkz0x1b==0xff)
		Parameterset.Parametersetitem.xtbkz0x1b=0;
  //0x1C
	if(Parameterset.Parametersetitem.AAbjsjset0x1c==0xff)
	Parameterset.Parametersetitem.AAbjsjset0x1c=60;//
   //0x1D
	if(Parameterset.Parametersetitem.zdqyz0x1D==0xff)
	Parameterset.Parametersetitem.zdqyz0x1D=225;//
	 //0x1E
	if(Parameterset.Parametersetitem.bdsbzq0x1E==0xff)
	Parameterset.Parametersetitem.bdsbzq0x1E=24;//
	//0x20
	if(Parameterset.Parametersetitem.PLCsrk0x20==0xff)
	Parameterset.Parametersetitem.PLCsrk0x20=0;//�ߵ�ƽ�����Ч
	//0x41 λ�û㱨����
	if(Parameterset.Parametersetitem.wzhbcl0x41==0xff)
	Parameterset.Parametersetitem.wzhbcl0x41=2;
	//0x42 ��ʱ�ϱ�ʱ��
	if(Parameterset.Parametersetitem.dssbsj0x42==0xff)
	Parameterset.Parametersetitem.dssbsj0x42=30;//30s
	//0x43 �����ϱ�����
	if(Parameterset.Parametersetitem.djsbjl0x43==0xff)
	Parameterset.Parametersetitem.djsbjl0x43=20;//200m
	
// 		//0x05
// 	for(i=55;i<119;i++)
// 	*(dat0+i+6)=*(dat1+i);
// 	if((*(dat0+55+6)==0xff)&&(*(dat0+56+6)==0xff))
// 	{
// 		*(dat0+55+6)='C';
// 		*(dat0+56+6)='M';
// 		*(dat0+57+6)='N';
// 		*(dat0+58+6)='E';
// 		*(dat0+59+6)='T';
// 		
//   }
	/***********" "**************/   
	//0x17 �豸���
	if(Parameterset.Parametersetitem.sbbh0x17[49]==0xff)//��50��Ϊ0xff��δ��ʼ��
	{
		for(i=0;i<50;i++)
	 Parameterset.Parametersetitem.sbbh0x17[i]=0;
  }
	//0x18 SIM���� 
	if(Parameterset.Parametersetitem.SIMkh0x18[19]==0xff)//��20��Ϊ0xff��δ��ʼ��
	{
		for(i=0;i<20;i++)
	 Parameterset.Parametersetitem.SIMkh0x18[i]=0;
		
  }
	//0x19 �������ĺ��� 
	if(Parameterset.Parametersetitem.hjzxhm0x19[19]==0xff)//��20��Ϊ0xff��δ��ʼ��
	{
		for(i=0;i<20;i++)
	 Parameterset.Parametersetitem.hjzxhm0x19[i]=0;
		
  }
	//0x22 �豸����
	if(Parameterset.Parametersetitem.sblx0x22[19]==0xff)//��20��Ϊ0xff��δ��ʼ��
	{
		for(i=0;i<20;i++)
	 Parameterset.Parametersetitem.sblx0x22[i]=0;
		
  }
	
	//0x09 ���ű�������
if(Parameterset.Parametersetitem.dxbjhm0x09[28]==0xff)//��29��Ϊ0xff��δ��ʼ��
	{
		
		//106575531157170
		Parameterset.Parametersetitem.dxbjhm0x09[0]=0x31;
		Parameterset.Parametersetitem.dxbjhm0x09[1]=0x30;
		Parameterset.Parametersetitem.dxbjhm0x09[2]=0x36;
		Parameterset.Parametersetitem.dxbjhm0x09[3]=0x35;
		Parameterset.Parametersetitem.dxbjhm0x09[4]=0x37;
		Parameterset.Parametersetitem.dxbjhm0x09[5]=0x35;
		Parameterset.Parametersetitem.dxbjhm0x09[6]=0x35;
		Parameterset.Parametersetitem.dxbjhm0x09[7]=0x33;
		Parameterset.Parametersetitem.dxbjhm0x09[8]=0x31;
		Parameterset.Parametersetitem.dxbjhm0x09[9]=0x31;
		Parameterset.Parametersetitem.dxbjhm0x09[10]=0x35;
		Parameterset.Parametersetitem.dxbjhm0x09[11]=0x37;
		Parameterset.Parametersetitem.dxbjhm0x09[12]=0x31;
		Parameterset.Parametersetitem.dxbjhm0x09[13]=0x37;
		Parameterset.Parametersetitem.dxbjhm0x09[14]=0x30;
		for(i=15;i<29;i++)
	  Parameterset.Parametersetitem.dxbjhm0x09[i]=0;
		
		  //0x05
			//0x06
		  //0x0E
		  //0x12
	   //0x13
	   //0x14
  }	
	
		
	
	moubilenumswitch();//��������λ��
	autorep_loctime=(Parameterset.Parametersetitem.hbsc0x03[0]<<8)+Parameterset.Parametersetitem.hbsc0x03[1];
}

void  initmem_param(void)
{ 
	uint8_t i;
	 SYS_UnlockReg();
	 DF_Readmem_param();//��ȡ����
	 SYS_LockReg();
	for(i=0;i<24;i++)
	{
		memparamset.mem_paramdata[i]=mem_paramdata[i];
  }
	if(memparamset.memparam.GPSvoidtimem==0xffffffff)//GPS���Ϸ��� 
		memparamset.memparam.GPSvoidtimem=0;
	 GPSvoidtimem=memparamset.memparam.GPSvoidtimem;
	//GPS ������� 
	if((memparamset.memparam.GPS_sum[0]==0xff)&&(memparamset.memparam.GPS_sum[1]==0xff)&&(memparamset.memparam.GPS_sum[2]==0xff)&&(memparamset.memparam.GPS_sum[3]==0xff))//GPS ������� 
		for(i=0;i<4;i++)
	{
		memparamset.memparam.GPS_sum[i]=0;
  }
	gps_tot_lc=(memparamset.memparam.GPS_sum[0]<<24)+(memparamset.memparam.GPS_sum[1]<<16)+(memparamset.memparam.GPS_sum[2]<<8)+memparamset.memparam.GPS_sum[3];
	//ACC ON ���ۼ�ʱ��
	if((memparamset.memparam.ACC_ON_sum[0]==0xff)&&(memparamset.memparam.ACC_ON_sum[1]==0xff)&&(memparamset.memparam.ACC_ON_sum[2]==0xff)&&(memparamset.memparam.ACC_ON_sum[3]==0xff))//GPS ������� 
		for(i=0;i<4;i++)
	{
		memparamset.memparam.ACC_ON_sum[i]=0;
  }
	ACCon_tot_time=(memparamset.memparam.ACC_ON_sum[0]<<24)+(memparamset.memparam.ACC_ON_sum[1]<<16)+(memparamset.memparam.ACC_ON_sum[2]<<8)+memparamset.memparam.ACC_ON_sum[3];
	
	//GPS��ͨ��ʱ��
	if((memparamset.memparam.GPSztdsj[0]==0xff)&&(memparamset.memparam.GPSztdsj[1]==0xff)&&(memparamset.memparam.GPSztdsj[2]==0xff)&&(memparamset.memparam.GPSztdsj[3]==0xff))//GPS ������� 
		for(i=0;i<4;i++)
	{
		memparamset.memparam.GPSztdsj[i]=0;
  }
	gps_tot_time=(memparamset.memparam.GPSztdsj[0]<<24)+(memparamset.memparam.GPSztdsj[1]<<16)+(memparamset.memparam.GPSztdsj[2]<<8)+memparamset.memparam.GPSztdsj[3];
	//���Ǵ���
	if(memparamset.memparam.kaigainum==0xff)
	   memparamset.memparam.kaigainum=0;
	// ��GPS���ߴ���
	if(memparamset.memparam.baGPSantnum==0xff)
	   memparamset.memparam.baGPSantnum=0;
	// ��SIM������
	if(memparamset.memparam.baSIMnum==0xff)
	   memparamset.memparam.baSIMnum=0;
	//SIM����������
	if(memparamset.memparam.simkaghcs==0xff)
	   memparamset.memparam.simkaghcs=0;
	if(memparamset.memparam.flagchange==0xffffffff)
	   memparamset.memparam.flagchange=0;
}
void mem_Paramete(void)
{//���� ACCOFF
	uint8_t i;
	if(memchange)//0x11�������иĶ�
	{
		memchange=0;
		for(i=0;i<192;i++)
	 {
		Parameter_setitemdata[i]=Parameterset.Parameter_setitemdata[i];
   }
	 SYS_UnlockReg();
	  DF_WriteParameter_setitem();//д����
	 SYS_LockReg();
  }
	
	
	//δͬ����ֵ
	//gps�����
	 memparamset.memparam.GPS_sum[0]=gps_tot_lc>>24;
	 memparamset.memparam.GPS_sum[1]=gps_tot_lc>>16;
	 memparamset.memparam.GPS_sum[2]=gps_tot_lc>>8;
	 memparamset.memparam.GPS_sum[3]=gps_tot_lc;
	//ACCon�ۼ�ʱ��
	 memparamset.memparam.ACC_ON_sum[0]=ACCon_tot_time>>24;
	 memparamset.memparam.ACC_ON_sum[1]=ACCon_tot_time>>16;
	 memparamset.memparam.ACC_ON_sum[2]=ACCon_tot_time>>8;
	 memparamset.memparam.ACC_ON_sum[3]=ACCon_tot_time;
	//GPS��ͨ��ʱ��
	 memparamset.memparam.GPSztdsj[0]=gps_tot_time>>24;
	 memparamset.memparam.GPSztdsj[1]=gps_tot_time>>16;
	 memparamset.memparam.GPSztdsj[2]=gps_tot_time>>8;
	 memparamset.memparam.GPSztdsj[3]=gps_tot_time;
// 	
// 	for(i=0;i<20;i++)
// 	{
// 		mem_paramdata[i]=memparamset.mem_paramdata[i];
//   }
	
	 for(i=0;i<24;i++)
	{
		if(mem_paramdata[i]!=memparamset.mem_paramdata[i])
			break;
  }
	
	if(i<24)
	{	//�����б䶯ִ�д洢
	  for(i=0;i<24;i++)
	 {
		mem_paramdata[i]=memparamset.mem_paramdata[i];
   }
	 SYS_UnlockReg();
	  DF_Writemem_param();//д����
	 SYS_LockReg();
  }
	
}

void numdisp8(void)
{ uint32_t temp;
	if((disnum==8)||(disnum==9))
	{
	 temp=GPIO_GET_IN_DATA(PC);
	 temp&=0x7F;
	 GPIO_SET_OUT_DATA(PC, temp);
	}
}
void numdisp(void)
{uint32_t temp;
	disnum++;
	while(!(numdispflag&BIT(disnum)))
	{
		disnum++;
		if(disnum>10)
		disnum=0;
	}
	temp=GPIO_GET_IN_DATA(PC);
	temp&=0x7F;
	if(disnum==9)
	temp|=(num_dis[8]<<7);
	else if(disnum==10)
	temp|=(num_dis[9]<<7);	
	else
	temp|=(num_dis[disnum]<<7);
	GPIO_SET_OUT_DATA(PC, temp);
// 	disnum++;
// 	if(disnum>9)
// 		disnum=0;
}
//����� LED���Լ�
void numdisp_test(void)
{ 
	uint32_t temp;
	if(!numdisp_testok)
 {	
	if((numdisp_testtime%10)>4)	//��500ms
	{
	 temp=GPIO_GET_IN_DATA(PC);
	 temp&=0x7F;
	 temp|=(num_dis[8]<<7);
	 GPIO_SET_OUT_DATA(PC, temp);
	 PC6=1;PA6=1;PB15=1;PB9=1;
	}
	else//��500ms
	 {	
	 temp=GPIO_GET_IN_DATA(PC);
	 temp&=0x7F;
	 GPIO_SET_OUT_DATA(PC, temp);
	 PC6=0;PA6=0;PB15=0;PB9=0;
	 }
	 
	  if(!numdisp_testtime)
		{
			numdisp_testok=1;
			disnum=7;
			numdisp();
			PB9=1; 
		}
	}
}
//����
//  void battakedown_check(void)
// 	{
// 					
// 			if(PB2)//��繦�ܴ�
// 			{
// 				if(PF4)//�ߵ�ƽû�г�� 
// 				{ PB2=0;
// 					//Delay_Us(500);
// 					Delay_Ms(15);
// 					PB2=1;
// 					//Delay_Us(500);
// 					Delay_Ms(15);
// 					if(PF4)//û�г�� 
// 						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//���õ�ضϵ�
//           else
// 					 TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
//         }
// 				else//��繦�ܴ� �����
// 					TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//���õ������
//       }
// 			else//��繦�ܹر�
// 			{
// 				if(volt_bat>39)
// 				TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//���õ������
// 				else
// 				{
// 					PB2=1;//��繦�ܴ�
// 					//Delay_Us(1000);
// 					 Delay_Ms(15);
// 					if(PF4)//û�г�� 
// 						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//���õ�ضϵ�
//           else
// 					 TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
// 					 PB2=0;
//         }
//       }	
// 		}
 
//�����ɵ�
 void battakedown_check(void)
	{
			// if(checkcount&BIT(6))
		if(voltage>Parameterset.Parametersetitem.zdqyz0x1D)//С��22.5vʱû���ⲿ��ѹ ���ټ�ֱ���ϵ�
	  {				 
			if(PB2)//��繦�ܴ�
			{
				//if(!PF4)//��������
				if(!PA14)//��������
				{ PB2=0;
					//Delay_Us(500);
					Delay_Ms(10);
					PB2=1;
					//Delay_Us(500);
					Delay_Ms(11);
// 					if(!PF4)//��������
						if(!PA14)//��������
					{	
						if(checkcount&BIT(3))//���2��ȷ��
						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//���õ�ضϵ�
						checkcount|= BIT(3);
					}
          else
					{
					 checkcount&= ~BIT(3);
					 TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
					}
        }
				else//��繦�ܴ� �����
					TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//���õ������
      }
			else//��繦�ܹر�
			{
				if(volt_bat>39)
				TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//���õ������
				else
				{
					PB2=1;//��繦�ܴ�
					//Delay_Us(1000);
					 Delay_Ms(11);
					//if(!PF4)//��������
					if(!PA14)//��������
					{
						if(checkcount&BIT(3))//���2��ȷ��
						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//���õ�ضϵ�
						checkcount|= BIT(3);
					}
          else
					{
					 checkcount&= ~BIT(3);
					 TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
					}
					 PB2=0;
        }
      }	
		}
	}



  //������
  void chargermanager(void)//1s
  {   	

     //sta33 ���õ�ضϵ籨��  
		if(TerminalReply0x110x18.GPS.state2& BIT(6))//����״̬�� 1s���
		{
			battakedowncheck_count=0;
		  battakedown_check();
		}
		else if(battakedowncheck_count<3)//�ظ�״̬�� ���2��
		{
			battakedown_check();
			battakedowncheck_count++;
			if(TerminalReply0x110x18.GPS.state3&BIT(3))//��ضϵ籣�ּ��
				battakedowncheck_count=0;
			  chargestoptime=0; 
    }

// 		
// 		 if((voltage>20)&&(!PA15))//�ⲿ�������2v��ִ�жϵ���� 
// 			chargestoptime=0; 
		 //if((TerminalReply0x110x18.GPS.state3&BIT(2))&&(volt_bat>37))//����ԴǷѹ22.5v 9v ���ұ����ѹ����3.7vʱ��׼��Ƿѹ�����������ⲿ�������
		//if((TerminalReply0x110x18.GPS.state3&BIT(2))&&(volt_bat>37)&&(!(TerminalReply0x110x18.GPS.state3&BIT(3))))//����ԴǷѹ22.5v 9v ���ұ�������ҵ�ѹ����3.7vʱ��׼��Ƿѹ�����������ⲿ�������		
		if((TerminalReply0x110x18.GPS.state3&BIT(2))&&(!(TerminalReply0x110x18.GPS.state3&BIT(3))))//����ԴǷѹ22.5v 9v ���ұ�����ڿ�׼��Ƿѹ�����������ⲿ�������		
		{
			 if(checkcount&BIT(6))//���2��ȷ��
			 {	 
			 PE5 = 0;//���ǿ�ƹ��� 
			 PA15 = 1;//Ƿѹ��������
			 }
			 checkcount|= BIT(6);
     }
	   else
		 {
		  checkcount&= ~BIT(6);
			PA15 = 0; //Ƿѹ�����ر�
			PE5 = 1;//�ⲿ���ƶϵ�
		 }
		 
			//if((voltage>(Parameterset.Parametersetitem.zdqyz0x1D+20))&&(Temperature<120))//22.5+2.0=24.5    120-60=60��
			if((voltage>245)&&(Temperature<120))//22.5+2.0=24.5    120-60=60��	
			{	
		   PB2 = 1;//���
			}
		  else
      {
// 			 if(((voltage<=50)||(PA15))&&(chargestoptime<3)&&(battakedowncheck_count>=3))//���ƻص�
// 			 {	
//         chargestoptime++;	
//         if(chargestoptime>3)
//         chargestoptime=3;				 
// 		    PB2=1;
// 			 }
// 			 else
			 PB2 = 0;//ͣ��	
       
		  }
				
  }
 
	void mem_exe(void)
	{//(���  ����  ���� )�����ݱ䶯
		if((TerminalReply0x110x18.GPS.state2&BIT(6))||(restartgps)||dormantlevel)
		{
			if(!mem_exeflag)
			{
       mem_Paramete();
			 mem_exeflag=1;
			}
		}
		else
			mem_exeflag=0;	
	}	

	
	
 void restart_exe(void)
	{
		if(restartgps)
		{
			SYS_UnlockReg();
			SYS_ResetChip();
			SYS_LockReg();
			restartgps=0;
    }
  }
	
 void dormant1(void)
	{  //dormant1stat=1;
		if(dormant1stat==0)
		{ 
// 			NVIC_DisableIRQ(TMR0_IRQn);
// 			NVIC_DisableIRQ(TMR1_IRQn);
// 			TIMER_Stop(TIMER0);
// 			TIMER_Stop(TIMER1);
// 			
// 			ZDParameter.LoginReply=0;//����ʧ�ܺ����µ�¼
// 			ZDParameter.LoginTimer =270;//�������µ�¼
// 		  ZDParameter.zdliushuinum=0;//��ˮ����
// 			
// 		 GPIO_SET_OUT_DATA(PC, 0X0F);
// 		 PA6=0;
// 		 PB15=0;
// 		 PB9=0;
// 		 PA10 = 1;//PW_GPS
// 		 PB3 = 0;//PW_GPRS �ɽ��ն���
// 		 PD14 = 1;//0 ������1���� canͨ��
			mem_exe();//�жϴ洢		
		 dormant1stat=1;
		}
		else if(dormant1stat==1)
		{
			if(at_command_order>=10)
			{			
		   closegprsnet=1;//�ر�GPRS����
		   //dormant1stat=2;
			 dormant1stat=3;
			}
			else
			dormant1stat=4;	
		}
// 		else if(dormant1stat==2)
// 		{
// 			if(closegprsnet==0)
// 		  {
// 		   dormant1stat=3;
// 			 dormantgprs=1;//gprs����
// 			}
//     }
		else if(dormant1stat==3)
		{
			//if(dormantgprs==0)
			if(closegprsnet==0)
		   dormant1stat=4;
    }
		else if(dormant1stat==4)
		{
// 			NVIC_DisableIRQ(TMR0_IRQn);
// 			NVIC_DisableIRQ(TMR1_IRQn);
// 			TIMER_Stop(TIMER0);
// 			TIMER_Stop(TIMER1);
// 			TIMER2->TCMPR =720000;//2Сʱ
// 			TIMER_Start(TIMER2);
			if(at_command_order>=10)
			 at_command_order = 10;
			else
			{
				at_command_order = 0;
				delay_ns = 50;
				PB3 = 1;//PW_GPRS  �޿�״̬
			}
			 
			AtCommSendingState = COMMSENDED;
			DataSended = 0;
			CommSended = 0; 
			GprsConnectTcpState=0;
			//state=0;
			dormanttime=0;	
			ZDParameter.LoginReply=0;//����ʧ�ܺ����µ�¼
			ZDParameter.LoginTimer =270;//�������µ�¼
		  ZDParameter.zdliushuinum=0;//��ˮ����
			//fwq_connectfail=0;
		 GPIO_SET_OUT_DATA(PC, 0X0F);
		 PA6=0;
		 PB15=0;
		 PB9=0;
		 PA10 = 1;//PW_GPS
		 //PB3 = 0;//PW_GPRS �ɽ��ն���
		 PD14 = 1;//0 ������1���� canͨ��
		 PA3=0; //��¼ָʾ��
		 PA4=0; //CANͨ��ָʾ��
		 dormant1stat=5;
    }
		else if(dormant1stat==5)
		{ 
// 			Delay_Ms(2000);//��������С������˸
// 			PB9=~PB9;
		}
  }	
	void dormant2(void)
	{ 
// 		if(dormant2stat==0)
// 		{
// 			ZDParameter.LoginReply=0;//����ʧ�ܺ����µ�¼
// 			ZDParameter.LoginTimer =270;//�������µ�¼
// 		  ZDParameter.zdliushuinum=0;//��ˮ����
// 		 GPIO_SET_OUT_DATA(PC, 0X0F);
// 		 PA6=0;
// 		 PB15=0;
// 		 PB9=0;
// 		 PA10 = 1;//PW_GPS
// 		 PB3 = 1;//PW_GPRS
// 		 PD14 = 1;//0 ������1����
// 		 // dormant2stat=1;
// 		 SYS_UnlockReg();
// 		 while((UART0->IER)&UART_IER_THRE_IEN_Msk);//�ȴ��رշ����ж�
// 		 UART_WAIT_TX_EMPTY(UART0);
// 		 CLK_PowerDown();//�����������
// 		 SYS_LockReg();
// 		
// 		}
		if(dormant2stat==0)
		{ 
		 dormant2stat=1;
			 mem_exe();//�жϴ洢		
		}
		else if(dormant2stat==1)
		{
		 closegprsnet=1;//�ر�GPRS����
			// dormant2stat=2;
		 dormant2stat=3;
		}
		else if(dormant2stat==2)
		{
			if(closegprsnet==0)
		  {
		   dormant2stat=3;
			 dormantgprs=1;//gprs����
			}
    }
		else if(dormant2stat==3)
		{
			if(dormantgprs==0)
		  dormant2stat=4;
    }
		else if(dormant2stat==4)
		{	
			at_command_order = 10;	
			AtCommSendingState = COMMSENDED;
			DataSended = 0;
			CommSended = 0; 
			//state=0;
			GprsConnectTcpState=0;
			ZDParameter.LoginReply=0;//����ʧ�ܺ����µ�¼
			ZDParameter.LoginTimer =270;//�������µ�¼
		  ZDParameter.zdliushuinum=0;//��ˮ����
			//fwq_connectfail=0;
		 GPIO_SET_OUT_DATA(PC, 0X0F);
		 PA6=0;
		 PB15=0;
		 PB9=0;
		 PA10 = 1;//PW_GPS
		 PB3 = 0;//PW_GPRS �ɽ��ն���
		 PD14 = 1;//0 ������1���� canͨ��
		 dormant2stat=5;
    }
		else if(dormant2stat==5)
		{ 
			//������ƽ������������߽�����������  ����ǰ���ж�
			if((PF8)&&(dormantlevellow))
		 {
			 //GPS�ն���ͨ��ʱ��  ������+��������
		  gps_tem_time=0;
			 
		  TIMER2->TCMPR = 8640000;//24Сʱ
			TIMER_Start(TIMER2);
			GPIO_EnableInt(PF, 8,  GPIO_INT_LOW);//IN23.75V �͵�ƽ�ж� ���ɱ��������
			GPIO_EnableInt(PB, 14, GPIO_INT_BOTH_EDGE);//���ǹظǿ��ж�
			GPIO_EnableInt(PD, 15,  GPIO_INT_BOTH_EDGE);
			SYS_UnlockReg();
		  while((UART0->IER)&UART_IER_THRE_IEN_Msk);//�ȴ��رշ����ж�
		  UART_WAIT_TX_EMPTY(UART0);
		  CLK_PowerDown();//�����������
		  SYS_LockReg();
		 }
		  dormant2stat=0;
		  wakeuptime2min=120;
		  dormant_wakeup=1;	//����
		 
		   PA10 = 0;//PW_GPS
		   PB3 = 0;//PW_GPRS �ɽ��ն���
		   PD14 = 1;//0 ������1���� canͨ��
		}
		
  }	
	
	void dormant3(void)
	{  uint32_t reg_time;
		
      dormant3stat=1;//����time2�ж�ִ��
			mem_exe();//�жϴ洢		
			//at_command_order = 10;	
// 		  if(at_command_order>=10)
// 			 at_command_order = 10;
// 			else
// 			 at_command_order = 0;
		   at_command_order = 0;	
			AtCommSendingState = COMMSENDED;
			DataSended = 0;
			CommSended = 0; 
			//state=0;
			GprsConnectTcpState=0;
			ZDParameter.LoginReply=0;//����ʧ�ܺ����µ�¼
			ZDParameter.LoginTimer =270;//�������µ�¼
		  ZDParameter.zdliushuinum=0;//��ˮ����
			//fwq_connectfail=0;
		
// 		   NVIC_SetPendingIRQ(TMR0_IRQn);
// 		   NVIC_SetPendingIRQ(TMR1_IRQn);
		  GPIO_SET_OUT_DATA(PC, 0X0F);
		  PA6=0;
		  PB15=0;
		  PB9=0;
		  PA10 = 1;//PW_GPS
		  PB3 = 1;//PW_GPRS �ɽ��ն���
		  PD14 = 1;//0 ������1���� canͨ��
			PA3=0; //��¼ָʾ��
		  PA4=0; //CANͨ��ָʾ��
			
			PA12=0; //ct_33v
			PA13=0; //ct_18v
			//������ƽ������������߽�����������  ����ǰ���ж�
			if((PF8)&&(!dormantlevellow))
		 {
			  ADC_POWER_DOWN(ADC);
			 reg_time=TIMER2->TCSR ;
			 if(reg_time&TIMER_TCSR_CEN_Msk)//����ʱ���Ѵ��������¸�ֵ
			 {
       }
			 else
			 { 
		    TIMER2->TCMPR = 8640000;//24Сʱ
				//TIMER2->TCMPR = 30000;//5����
			  TIMER_Start(TIMER2);
			 }
			 	 
			 
			GPIO_EnableInt(PF, 8,  GPIO_INT_LOW);//IN23.75V �͵�ƽ�ж� ���ɱ��������
			GPIO_EnableInt(PB, 14, GPIO_INT_BOTH_EDGE);//���ǹظǿ��ж�
			GPIO_EnableInt(PD, 15,  GPIO_INT_BOTH_EDGE);
			 //��ACC���
			 GPIO_EnableInt(PA, 8,  GPIO_INT_BOTH_EDGE);//����
			 GPIO_EnableInt(PB, 7,  GPIO_INT_BOTH_EDGE);//����
			 //���籣���жϣ�ֻ��ACC�жϼ��
// 			if(volt_bat<37) 
// 				volt_bat37low++;
// 			 else
// 				volt_bat37low=0;
// 			 if(volt_bat37low>=2) 
			 if(TerminalReply0x110x18.GPS.state3&BIT(4))//���õ��Ƿѹ �������籣�� ֻ��ACC�жϼ��
			 {
				  TIMER_Stop(TIMER2);//��ʱ�份��
				  GPIO_DisableInt(PB, 14);//�ؿ��ǹظǿ��ж�			 
		    	GPIO_DisableInt(PD, 15);
          GPIO_DisableInt(PB, 12);//�����ж� 
		    	GPIO_DisableInt(PF, 5);	
 //         GPIO_DisableInt(PF, 8);//��IN23.75V �͵�ƽ�ж� ���ɱ��������					 
				 
//         //��ACC���
//       GPIO_DisableInt(PA, 8);			 
// 			GPIO_DisableInt(PB, 7);  
			 
       }
			 
			 
			SYS_UnlockReg();
		  while((UART0->IER)&UART_IER_THRE_IEN_Msk);//�ȴ��رշ����ж�
		  UART_WAIT_TX_EMPTY(UART0);
			if(send0xAA==0)//�޻��ѱ���
		  CLK_PowerDown();//�����������
		  SYS_LockReg();
			
      GPIO_DisableInt(PB, 14);			 
			GPIO_DisableInt(PD, 15);	
      //��ACC���
      GPIO_DisableInt(PA, 8);			 
			GPIO_DisableInt(PB, 7);  
			 
			dormant3stat=0;//����time2�ж�ִ��  ���
			 if(PF8)//��ȻΪ�������
			 {	 
        wakeuptime2min=120;
		    dormant_wakeup=1;	//����
			 }	
       else
       dormantlevel=1;//������������				 
			 gprslinkfailtime=0;//���¼�¼����ʧ��ʱ��
			 ADC_POWER_ON(ADC);
		 }
		 
		
// 		   NVIC_ClearPendingIRQ(TMR0_IRQn);
// 		   NVIC_ClearPendingIRQ(TMR1_IRQn);
		   PA12=1; //ct_33v
			 PA13=1; //ct_18v
		   PA10 = 0;//PW_GPS
		   PB3 = 0;//PW_GPRS �ɽ��ն���
		   PD14 = 1;//0 ������1���� canͨ��
		
		
  }	
	
 void	dormant_wakeup1(void)
	{
		if(wakeup1stat==0)
		{ 
// 		  NVIC_EnableIRQ(TMR0_IRQn); 
// 		  NVIC_EnableIRQ(TMR1_IRQn);
// 			TIMER_Start(TIMER0);
// 			TIMER_Start(TIMER1);
// 		 GPIO_SET_OUT_DATA(PC, 0X0F);
// 		 PA6=0;
// 		 PB15=0;
// 		 PB9=0;
		 PA10 = 0;//PW_GPS
		 PB3 = 0;//PW_GPRS �ɽ��ն���
		 PD14 = 1;//0 ������1���� canͨ��
		 gprslinkfailtime=0;//���¼�¼����ʧ��ʱ��
		 wakeup1stat=1;
		}
		
  }
 void	dormant_wakeup2(void)
	{
		if(wakeup2stat==0)
		{
// 			NVIC_EnableIRQ(TMR0_IRQn);//�������ߡ����������ߡ������绽��
// 		  NVIC_EnableIRQ(TMR1_IRQn);
// 			TIMER_Start(TIMER0);
// 			TIMER_Start(TIMER1);
// 		 GPIO_SET_OUT_DATA(PC, 0X0F);
// 		 PA6=0;
// 		 PB15=0;
// 		 PB9=0;
		 PA10 = 0;//PW_GPS
		 PB3 = 0;//PW_GPRS
		 PD14 = 0;//0 ������1����
		 wakeup2stat=1;
		}
  }	
	
	
	
	
// dormantlevel=0;//���߼��� 0���繤�� 1�������� 2���繤�� 3��������	
// dormantlevel=0;//���߼��� 0�����繤�� 1�������� 2��������	
	
void dormant_judge(void)//1s
{
	if((dormantlevel!=dormantlevelstat)||(dormantlevel==0))//�������ߺ�����ִ���ޱ����ж�
	{
	  check_baojingflag=0;
		dormantlevelstat=dormantlevel;
	}
	
  if(Parameterset.Parametersetitem.sdms0x16)// ʡ��ģʽ 1���� 0��
	{ 
		//���ߣ�ACC OFF֮��10����ͬʱ��ѹ����24.1v �޶���  ����
		//if((dormant_ACCofftime>=600)||(voltage<Parameterset.Parametersetitem.zdqyz0x1D))//����������������PLC�źţ�ֱ��10����	
    if(dormant_ACCofftime>=600)		
    { 
			 if((TerminalReply0x110x18.GPS.state3&BIT(1))||(TerminalReply0x110x18.GPS.state3&BIT(2)))//�ϵ��Ƿѹ����
			 { 
				 if((dormant1stat==5)||(dormant1stat==0)) 
			   dormantlevel=2;//��������
			 }
			 else
			 {
				 if(dormant2stat==0)//������ƽ������������߽�����������  ����ǰ���ж�
			    dormantlevel=1;//��������	
			 }
      wakeup2stat=0;//��ִ�л���
    }

		//�˳����߽�����������ģʽ�������ж��ţ�ACCON ����24.5v����10���ӣ��񶯻��� ������2km/h 5������������
	if((smsnumber>3)||(readsmsnumok)||(dxbjhmflag)||(TerminalReply0x110x18.GPS.state1&BIT(5))||(time_245v>=600)||(TerminalReply0x110x18.GPS.state4&BIT(0)))
	{
		dormantlevellow=1;//�������ǰ���ã���ֹ����������߳��������˳�
		if(((dormant1stat==5)||(dormant1stat==0))&&(dormant2stat==0))//
		{	
		  
		 dormantlevel=0;//���繤��
		 dormant_wakeup=0;//���߻��ѱ�־
		 dormant1stat=0;
		}
  }
	else
	dormantlevellow=0;	
	
    if(dormantlevel!=0)	//�������ߺ�ȡ���Զ�������
	   Timer1Count=0;				
// 	if((dormantlevel)&&(!smsnumber)&&(!readsmsnumok)&&(!dxbjhmflag)&&(!state)&&(!TimeArrivedlogin)&&(!TimeArrivedcsq)&&(!TimeArrivedcreg)&&(!TimeArrived2B)&&(!TimeArrived0x13)
// 	&&(!TimeArrived0x18)&&(!TimeArrived0x11)&&(!TimeArrived0x17)&&(!TimeArrived12)&&(!TimeArrived0x28))
	
// 	  //ִ������
// 		if((dormant_wakeup==1)&&((dormant1stat==5)||(dormant1stat==0))&&(dormant2stat==0))//ִ�л��Ѳ���
// 		{			
// 		  if(dormantlevel==1)
// 			{ 
// 				dormant1stat=0;//��ִ������
// 			  dormant_wakeup1();
// 			}
//       if(dormantlevel==2)
// 			{
// 			 dormant2stat=0;//��ִ������
// 			 dormant_wakeup1();
// 			}				
// 	  }	
//     else
// 		{			
// 		 if((dormantlevel==1)||((dormant1stat>0)&&(dormant1stat<5)))
// 		 {
// 			 wakeup1stat=0;//��ִ�л���
// 			dormant1();//ִ���������� 	 
// 		 }
//      if(dormantlevel==2)
// 		 { dormant1stat=0;
// 			 wakeup1stat=0;//��ִ�л���
// 			dormant2();//ִ�б�������
// 		 }			 
// 		 if(dormantlevel==0)//ȡ������ִ��
// 		 {
// 			 dormant_wakeup2();
//      }	
// 	  }
   }
		
	
}	

 void dormant_exe(void)
 {
	   //ִ������
		//if((dormant_wakeup==1)&&((dormant1stat==5)||(dormant1stat==0))&&(dormant2stat==0))//ִ�л��Ѳ���
		//if((dormant_wakeup==1)&&(dormant1stat==5)&&(dormant2stat==0))//ִ�л��Ѳ���	
		//if((dormant_wakeup==1)&&(dormant1stat==5))//ִ�л��Ѳ���
	 
	 
	 if(dormant_wakeup==2)	
   {
		 if((dormant1stat==5)||(dormant1stat==0))//δִ���������ߣ� ��ִ�����������߲ſ�ִ�����绽�ѹ�������ȴ�
		 {
			 dormant_wakeup=1;
			 wakeuptime2min=120;
			 
		 }
   }		 
	 
    if(dormant_wakeup==1)			
		{			
		  if(dormantlevel==1)
			{ 
				dormant1stat=0;//��ִ������
			  dormant_wakeup1();
			}		
	  }	
    else
		{		
		 //if((fwq_connectfail)||(TerminalReply0x17.baojingflag==0))
		//if((fwq_connectfail)||(check_baojingflag>=2)) 
			if((fwq_connectfail)||(check_baojingflag>=3)||(dormant1stat)||(gprslinkfailtime>=120)) 
		 {
		  //if((dormantlevel==1)||((dormant1stat>0)&&(dormant1stat<5)))
			if(dormantlevel==1)
		  {
			 wakeup1stat=0;//��ִ�л���
			 dormant1();//ִ���������� 	 
		  }
      if(dormantlevel==2)
		  {
				//����������
				dormant1stat=0;
			  wakeup1stat=0;//��ִ�л���
			  dormant3();//ִ�б�������
		  }			 
		  if(dormantlevel==0)//ȡ������ִ��
		  {
			 dormant_wakeup2();
      }	
	  }
	 }
	 
	 
	 
 }



/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{   
    
    SYS_UnlockReg();
	  WDT_Close();
    SYS_Init();
    SYS_LockReg();

	 SYS_UnlockReg();
	 GPIO_INIT();
	  ADC_INIT();
	
  while(1)
  {
	Adc_Data();
  }
}







