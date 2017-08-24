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

#include "adcapp.h"
#include "alart.h"
#include "canMessage.h"
#include "dataflash.h"
#include "lcm.h"
#include "M660.h"
#include "costab.h"
#define GPRS

//#define CCIDASSIM

#define PLL_CLOCK   48000000

int 	lsnum = 0;//for (lsnum = 0; lsnum <3; lsnum++)
unsigned int cdnum = 0;
 
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

	 CLK_SetModuleClock(WDT_MODULE,CLK_CLKSEL1_WDT_S_LIRC,0);

	 CLK_EnableModuleClock(WDT_MODULE);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);   

    /* Select UART module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

		    /* Enable UART module clock */
    CLK_EnableModuleClock(UART1_MODULE);

	    /* Select UART module clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

	    /* Enable UART module clock */
    CLK_EnableModuleClock(UART2_MODULE);

    /* Select UART module clock source */
    CLK_SetModuleClock(UART2_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

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
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 100);   //100HZ�����ж�
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
    CLK_SetModuleClock(TMR2_MODULE, CLK_CLKSEL1_TMR2_S_HXT, 0);
    /* Open Timer0 in periodic mode, enable interrupt and 10 interrupt tick per second */
    TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 10);   //10HZ�����ж�
    TIMER_EnableInt(TIMER2); 	
    /* Enable Timer0  NVIC */
    NVIC_EnableIRQ(TMR2_IRQn);
	/* Start Timer0  counting */
    TIMER_Start(TIMER2);
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
void TMR0_IRQHandler(void)			  //100hz
{
  //if(TIMER_GetIntFlag(TIMER0) == 1)
 // {
    /* Clear Timer0 time-out interrupt flag */
    TIMER_ClearIntFlag(TIMER0);
	if(opok)
  	{
		seeddatatimecount++;
		if(seeddatatimecount>=6000)
		{
			seeddatatimecount=6000;
		    if(tongxintime>=50)
	        seeddatatime=30000;//5����
		    else
			seeddatatime=2000;//20s
	   }
	  else
		seeddatatime=2000;//20s 
	    Timer0Count++;
		//if((Timer0Count % 1500) == 0 )	   //15��
		if((Timer0Count % seeddatatime) == 0 )	   //20��
        TransferTimeArrived = 1;	

		if((Timer0Count % 100) == 0 )	   //100
		{
		  //RUNLIGHTSW;//1s��������˸
		}
		Gprs_Timeout_Count++;

	   if( GprsConnectTcpState < 2 )
	   {      
	      if(gprsNoConnect < 100*60) //1����δ��������
	      {
	        gprsNoConnect++;
	      }            
	      else
	      {
	        GPRS_POWERROFF;
	        delay_ns = 400;
	      	gprsNoConnect = 0;		        
	      }      
	   }	 
       if(delay_ns == 100)
	   {
	      GPRS_POWERRON;            
		  gprsNoConnect = 0; 
		   delay_ns--; 
	   }
	   else
	   {
	     if(delay_ns>0)
	     delay_ns--;
	   }

	   if(SOS_Interval > 0) SOS_Interval--;
	 }
// }
}


/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle TIMER0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void TMR1_IRQHandler(void)
{
 //if(TIMER_GetIntFlag(TIMER1) == 1)
 // {
	   TIMER_ClearIntFlag(TIMER1);

	   //2017.8.18 wsj
	   //test timer 1
	   TestCounter++;
	   if(TestCounter == 10)
	   {
	   	 GPS_POWERROFF;
		 //GPS_POWERRON;

	   }
	   else if(TestCounter == 20)
	   {
	   	   GPS_POWERRON;
	   }
	   else if(TestCounter>21)
		 {
		   		 TestCounter = 0;
		 }


//}
}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle TIMER0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void TMR2_IRQHandler(void)
{
     TIMER_ClearIntFlag(TIMER2);
      optime10s++;	
     if(optime10s>=100)
 	    optime10s=100;
     startingup();
     //���·�������֡
		 timeout++;
		 if(timeout>=15)//1.5s
			 timeout=15;
		 
		 //��ʼ��Ķȥ����2s
		 opdelay++;
		 if(opdelay>=20)
			 opdelay=20;
		 //��Ķÿ1���Ӽ���һ��
		   jmtime++;
		 if(jmtime>=10)
			 jmtime=10;
		 //ֱ��ִ���γɷ��Բ��ʱ
		 exejscloseroundtiem++;
		 if(exejscloseroundtiem>=100)
			 exejscloseroundtiem=100;
    // if(opok)
    // {	 
			 
    // pagread++;
		 
    	tongxintime++;
	   if(tongxintime>=50)
	   {
	    tongxintime=50;
	    flag |= BIT(kzq);
	    tbflag1 |= BIT(10);
      tbflag1 &=~ BIT(13);
			NewMachineFrame.u32indicateLed |=BIT(TXFled); 
			NewMachineFrame.u32indicateLed &=~BIT(TXVled); 
	    }
	    else
	   {
	    flag &=~ BIT(kzq);
	    tbflag1 &=~ BIT(10);
      tbflag1 |= BIT(13);
			NewMachineFrame.u32indicateLed |=BIT(TXVled); 
			NewMachineFrame.u32indicateLed &=~BIT(TXFled); 
	    }
	
	   if(flag & BIT(kzq))
	    {
	     fd_time=30;
	     bj_ch=0;
	    }
	    else
	    {
	     fd_time++;
	     if(fd_time>48)
	     {
	      fd_time=0;
	      bj_ch++;
	     }
	    } 
	
	if(fdj_rev>300)
	  //if(fdj_rev==0)
	  {
	  time6min++;
	  if(time6min>=3600)  //6min
	  //if(time6min>=100)
	  {
	    time6min=0;
	    hours++;
			mmhours=1;
		if(hours>99999)
		{
		  hours=0;
		}
	  }
	}
 //��Сʱ�Ƽ���			
	mmhoursopen++;
 if(mmhoursopen>=30)
  mmhoursopen=30;	

	
			
	    bj_delay600MS++;//ͼ��
	    if(bj_delay600MS >= 11)
		   bj_delay600MS = 0;	
			
	    bj_delay12s++;//����
	    if(bj_delay12s>=41)
	    bj_delay12s=0;
		
	    errtime++;
	   if(errtime>=20)
	   {
	    errtime=20;
	    CAN_Rxbuf[4][2]=0x00;
	    CAN_Rxbuf[4][3]=0x00;
	    CAN_Rxbuf[4][4]=0x00;
	    }
			
			err2time++;//
	if(err2time>=20)
	{
	  err2time=20;
		SPNch=0;
	  ECU_flag &=~ BIT(Ecu_dm1);//�Ե���
	}

	SPN_Displaytime++;
	if(SPN_Displaytime>=50)//5s
	{
		SPN_Displaytime=0;
		SPN_Display();
	}

	
	
	
			/*
    alerttime++;
	 if(alerttime>=20)
	 {alerttime=20;		 
		alert_jc();
	 }*/
  //}
}


void TMR3_IRQHandler(void)
{
	   TIMER_ClearIntFlag(TIMER3);
		
		 if(!shisu_flag) 				 
	     {
		    Shisu.temppulse = Shisu.pulse;//ʱ�� ��������
		    shisu_flag=1;				 	
	     }	
}

void EINT0_IRQHandler(void)	 //����
{
    /* For PB.14, clear the INT flag */
    GPIO_CLR_INT_FLAG(PB, BIT14);
	//NVIC_DisableIRQ(EINT0_IRQn);
	//if( !(GPIO_GET_IN_DATA(PA) & (1<<7)) )
	if( PB14 == 0 )
	{
	 //SYS_UnlockReg();
	  //DF_WriteHour2(hours);
   /*
	  if(DF_ReadHour2() != hours)	//	����Сʱ��ʵ�ʲ�һ��   ���һ���ж�һ��ʱ��
	  {
	  	 DF_WriteHour2(hours);
	  }	
   */		
	  //SYS_LockReg();
		//EEPROMwrite_Meter();
		mmhoursopen=0;
	}
	
}

void GPAB_IRQHandler(void)//����ж�
{
    GPIO_CLR_INT_FLAG(PB, BIT8);
	//Delay_Us(10);
	 if( PB8 == 0)
	 {
		Shisu.pulse++;
	
	  //cur_meter = 12345;//xianshi12.3
    if(Shisu.value < 3000)  //ʱ�����Ƶ��
	   { 
		   meter_temp++;
		   if(meter_temp>=12)
		   {
		    meter_temp=0;
		    cur_meter+=1; //�ʹ�����85/11�й� //0.0086Ϊÿ���������˶�����		  ////
			   if(cur_meter >= 999999)
				  cur_meter = 0;
		   }	
				kilo_pluse++;
				//if(kilo_pluse >= 116279)			/////////////////////
			 if(kilo_pluse >= 11628)	//0.1����
				{
					kilo_pluse = 0;
					sum_kilometre++;
					kilometretemp = 1;
					if(sum_kilometre > 999999)
				  sum_kilometre = 0;
				}			
	    }
     }
  }

#if 0
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

	  SYS->GPD_MFP &= ~(SYS_GPD_MFP_PD14_Msk | SYS_GPD_MFP_PD15_Msk);
    SYS->GPD_MFP |= SYS_GPD_MFP_PD14_UART2_RXD | SYS_GPD_MFP_PD15_UART2_TXD;

	    /* Set PD multi-function pins for CANTX0, CANRX0 */
    SYS->GPD_MFP &= ~(SYS_GPD_MFP_PD6_Msk | SYS_GPD_MFP_PD7_Msk);
    SYS->GPD_MFP |= SYS_GPD_MFP_PD6_CAN0_RXD | SYS_GPD_MFP_PD7_CAN0_TXD;
	
	 
	
	
    GPIO_SetMode(PA, BIT10, GPIO_PMD_INPUT);	//��ת
	  GPIO_SetMode(PA, BIT11, GPIO_PMD_INPUT);	//����
	  GPIO_SetMode(PA, BIT9, GPIO_PMD_INPUT);	  //Զ��
	  GPIO_SetMode(PB, BIT7, GPIO_PMD_INPUT);		//��ת
		GPIO_SetMode(PB, BIT3, GPIO_PMD_INPUT);	  //���
		GPIO_SetMode(PB, BIT2, GPIO_PMD_INPUT);	  //λ��
		GPIO_SetMode(PC, BIT9, GPIO_PMD_INPUT);	  //����
		GPIO_SetMode(PC, BIT0, GPIO_PMD_INPUT);	  //��ɲ
		GPIO_SetMode(PC, BIT14, GPIO_PMD_INPUT);	//����
		GPIO_SetMode(PF, BIT4, GPIO_PMD_INPUT);	  //����

		
    
	 
	  GPIO_SetMode(PA, BIT14, GPIO_PMD_OUTPUT);
    GPIO_SetMode(PA, BIT15, GPIO_PMD_OUTPUT);
	 // GPIO_SetMode(PA, BIT4, GPIO_PMD_OUTPUT);
		GPIO_SetMode(PB, BIT12, GPIO_PMD_OUTPUT);
		GPIO_SetMode(PC, BIT10, GPIO_PMD_OUTPUT);
    GPIO_SetMode(PF, BIT5, GPIO_PMD_OUTPUT);	  
	  PA14   =  0;
	  //PA15 = 0 ;
		PA15 = 1 ;
		//PA4 = 0; //����
		PB12 = 1;//������
    PC10 = 1;//GPRS pow
	  PF5 =  1; //������	
		
		//����
    GPIO_SetMode(PB, BIT14, GPIO_PMD_INPUT);
    //GPIO_EnableEINT0(PB, 14, GPIO_INT_LOW);
		GPIO_EnableEINT0(PB, 14, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT0_IRQn);
	 /*
	 //ʱ��
	 SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB8_Msk );
	 SYS->GPB_MFP |= SYS_GPB_MFP_PB8_GPIO;
	GPIO_SetMode(PB, BIT8, GPIO_PMD_INPUT);
	GPIO_EnableEINT0(PB, 8, GPIO_INT_FALLING);
	//GPIO_EnableEINT0(PB, 8, GPIO_INT_RISING );
	 NVIC_EnableIRQ(GPAB_IRQn);
	 //GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_1);
	// GPIO_ENABLE_DEBOUNCE(PB, BIT8);      
*/
		
	
	 SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB13_Msk );
	 SYS->GPB_MFP |= SYS_GPB_MFP_PB13_GPIO;
	GPIO_SetMode(PB, BIT13, GPIO_PMD_INPUT);
	//GPIO_EnableEINT0(PB, 13, GPIO_INT_FALLING);
	//GPIO_EnableEINT0(PB, 13, GPIO_INT_RISING );
	//NVIC_EnableIRQ(GPAB_IRQn);
  /* 
  SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB14_Msk );
	SYS->GPB_MFP |= (SYS_GPB_MFP_PB14_INT0);
  GPIO_SetMode(PB, BIT14, GPIO_PMD_INPUT);
  //GPIO_EnableEINT0(PB, 14, GPIO_INT_FALLING);//�����ж�
	GPIO_EnableEINT0(PB, 14, GPIO_INT_BOTH_EDGE);//�����ж�
  NVIC_EnableIRQ(EINT0_IRQn);	
	*/
	  	
}
#endif


//wushengjun add 2017.7.27

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

 	  SYS->GPD_MFP &= ~(SYS_GPD_MFP_PD14_Msk | SYS_GPD_MFP_PD15_Msk);
     SYS->GPD_MFP |= SYS_GPD_MFP_PD14_UART2_RXD | SYS_GPD_MFP_PD15_UART2_TXD;

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
		//	GPIO_SetMode(PA, BIT8, GPIO_PMD_OUTPUT);PA8=0;//IN33  ���͵�ƽ

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
  
	   /*
	  GPIO_SetMode(PD, BIT14, GPIO_PMD_OUTPUT);//TCAN1042 STB
		PD14 = 0;//0 ������1����
		
		GPIO_SetMode(PD, BIT15, GPIO_PMD_INPUT);//sunjc1
		*/
		GPIO_SetMode(PE, BIT5, GPIO_PMD_OUTPUT);//CT_BATT   GPIO_PMD_OUTPUT
        //GPIO_SetMode(PE, BIT5, GPIO_PMD_OPEN_DRAIN ); //CT_BATT
		//PE5 = 1;//�ⲿ�������
		PE5 = 0;	

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



void jsshisu(void)//
{  
	
	if(shisu_flag)								 
	{
	    shisu_temp+=Shisu.temppulse;  
	    shisu_jishu++;
		if(shisu_jishu>=10)
		{
		  shisu_jishu=0;
		  Shisu.value = shisu_temp*109/35;
		  shisu_temp=0;
		}
		
  //if((Shisu.value < 100)||(Shisu.value > 3000))
	   if(Shisu.value < 50)
		 Shisu.value = 0;
	   if(Shisu.value>=3000)	
     Shisu.value = 3000;
	
	    Shisu.pulse = 0;
		//TCMPR0 = 1200000;  
    TIMER_SET_CMP_VALUE(TIMER3,1200000 );			
		shisu_flag=0;
	}
	 
}

void jwdcap(void)
{
  unsigned char a[10],b[10],i,j,k;
	//float jd,wd;
 	//unsigned long jd,wd,cwd;
	uint64_t jd,wd,cwd;
//if((jmopen)&&(!closeround))//��ʼ��Ķ ����û���γɷ��Բ
if(!closeround)//��ʼ��Ķ ����û���γɷ��Բ
{
	i=0;j=0;k=0;
	for(i=0;i<47;i++)
	{ k=i;
		if(*(gpsdats+i)==',')
			j++;
		if(j==2)
		break;	
  }
	//i=0;
	if(*(gpsdats+k+1)=='A')
	{	//��ȡά��ASC
	 for(i=0;i<9;i++)	
	 {
	  a[i]=	*(gpsdats+3+k)-0x30;
		 if(i==3)//ȥ��С����
		 k++;
		 k++;
	 }

	//wd=a[0]*10+a[1]+(a[2]*1000000+a[3]*100000+a[4]*10000+a[5]*1000+a[6]*100+a[7]*10+a[8])/60;//������100000��
	wd=a[0]*1000000+a[1]*100000+(a[2]*1000000+a[3]*100000+a[4]*10000+a[5]*1000+a[6]*100+a[7]*10+a[8])/60;//������100000��

	
  if((a[2]*10+a[3])>0)
	cwd=ctable[(a[0]*10+a[1])]-((ctable[(a[0]*10+a[1])]-ctable[(a[0]*10+a[1])+1])*(a[2]*10+a[3])/60);
	else
  cwd=ctable[(a[0]*10+a[1])];	
	 
	 
	 //��ȡ����ASC
	 for(i=0;i<10;i++)	
	 {
	    b[i]=	*(gpsdats+6+k)-0x30;
		 if(i==4)//ȥ��С����
		 k++;
		 k++;
	 }
   //jd=b[0]*100+b[1]*10+b[2]+(b[3]*1000000+b[4]*100000+b[5]*10000+b[6]*1000+b[7]*100+b[8]*10+b[9])/60;//������100000��
   jd=b[0]*10000000+b[1]*1000000+b[2]*100000+(b[3]*1000000+b[4]*100000+b[5]*10000+b[6]*1000+b[7]*100+b[8]*10+b[9])/60;//������100000��

	  
	//����ת�� 
	 //Xi = 2RLi cosBi /360
   //Yi = 2pRBi /360
	// X[1]=2*3.14*6371116*jd*cwd/360/100000/100000;  //4001060848/16=250066303
	 //Y[1]=2*3.14*6371116*wd/360/100000/100000;
	 
	// X[1]=2*3.14*6371116/360*jd*cwd/100000/100000;  
	 //Y[1]=2*3.14*6371116/360*wd/100000;
	 
	// X[1]=2*314*6371116/36000*jd/100000*cwd/100000;  
	 //Y[1]=2*314*6371116/36000*wd/100000;
	 
	  X[0]=X[1];
	  Y[0]=Y[1];
	  X[1]=250066303/2250*cwd/100000*jd/100000;  
	  Y[1]=250066303/2250*wd/100000;
	 
	 
	 
	 
	 if(firstone)//�ڶ��λ�ȡ��γ������
	 {
		 if(exejscloseround!=2)
		 {	 
		 //���Բ�ж�
		 if( X[2]>X[1])
			 X1X2= X[2]-X[1];
		 else
		  X1X2= X[1]-X[2];
		 
		 if( Y[2]>Y[1])
			 Y1Y2= Y[2]-Y[1];
		 else
		   Y1Y2= Y[1]-Y[2];
		 L=X1X2*X1X2+Y1Y2*Y1Y2;
		 if(L>81)//9��
		 jscloseround=1; 
		 
		 if((jscloseround)&&(L<=36)&&(firstone>=3))//6����������Բ �ɼ���������3
		 {
		  jscloseround=0; 
			 X[1]=X[2];
	     Y[1]=Y[2];
			 closeround=1; //���Բ�γ�
		 }
	  }
		else//��Ķ���عر�ֱ���γɷ��Բ
		{ if(firstone>=3)
			{
			 exejscloseround=0;//�رռ�Ķ����
			 jscloseround=0; 
			 X[1]=X[2];
	     Y[1]=Y[2];
			 closeround=1; //���Բ�γ�
			}
    }
		 
		 X0Y1= X[0]* Y[1];
		 X1Y0= X[1]* Y[0];
		 
		 if(!polar)//�����Ϊ����
		 {
		   if(X0Y1>=X1Y0)
		   {
			  S1=X0Y1-X1Y0;
				S=S+S1;
		   }
			 else
		  {	//ʵʱ���Ϊ����
		   S1=X1Y0-X0Y1;
			 if(S>=S1)
			 {
				 S=S-S1;
       }
			 else
			 {
				 S=S1-S;
				 polar=1;//�����Ϊ����
       }
	    }
     }
	   else//�����Ϊ����
	   {
		  if(X0Y1<=X1Y0)//ʵʱ���Ϊ����
		   {
			  S1=X1Y0-X0Y1;
				S=S+S1;
		   }
			 else
		  {	//ʵʱ���Ϊ����
		   S1=X0Y1-X1Y0;
			 if(S>=S1)
			 {
				 S=S-S1;
       }
			 else
			 {
				 S=S1-S;
				 polar=0;//�����Ϊ����
       }
	    }  
     }
/*
     if(closeround)//�γɷ��Բ��������� 1ƽ����=0.0015Ķ
		 {
			 //S=S/2;//ƽ����
			 //S=S*15/2/1000;//Ķ ����10
			 S=S*3/400;//Ķ ����10
		   firstone=0;
			 //���ͼ�Ķ��������
     }
*/

  		 
   } 
	 else
	 {//��¼���Բ���
		 X[2]=X[1];
	   Y[2]=Y[1];
   }
   firstone++;//�ȵڶ��λ�ȡ����ʱ��ʼ����
	 if(firstone>=3)//��֤3�������ϲſ�ֱ��ִ���γɷ��Բ
		 firstone=3;
  }
	 if(closeround)//�γɷ��Բ��������� 1ƽ����=0.0015Ķ
		{
			 //S=S/2;//ƽ����
			 //S=S*15/2/1000;//Ķ ����10
			 S=S*3/400;//Ķ ����10
		   firstone=0;
			 //S=400;
			 ConverJMdataToASC(S,JMStateRpClose.StatePara); 						
			//���ͼ�Ķ��������		
			IsJMStateRpClose=1;
			seedcosejm=0;//�Ƚ�����Ķʱ����������	
    }
		else
		{
		 if((exejscloseround==2)&&(exejscloseroundtiem>=100))//ִ��ֱ���γɷ��Բ��ʱ10s
		 {
			 exejscloseround=0;
			 S=0;
			 firstone=0;
			  ConverJMdataToASC(S,JMStateRpClose.StatePara); 						
			//���ͼ�Ķ��������		
			 IsJMStateRpClose=1;
			 seedcosejm=0;//�Ƚ�����Ķʱ����������	
     }
    }
	
 }
 
}


/*---------------------------------------------------------------------------------------------------------*/
//Date And Author :2017.8.11 wushengjun
//Function:	Battery Charge Mangment;  ﮵�س�����
//Function Name: BatteryChargeMangment()
//

void BatteryChargeMangment()
{

  
	if(volt_bat<37)
	{
		if(!PA14)//��������
		{
			PB2=0;
		}
		else
		{
			PB2=1;	
		}
	}
	else if(voltage > 220)
	{
		 if(!PA14)//��������
		{
			PB2=0;
		}
		else
		{
			PB2=1;	
		}
	}
	/*
	else if((volt_bat>37)&&(voltage < 50))//�ڵ�����﮵�ص�ʱ��Ҳ��������������ʱcan�Ѿ�ֹͣ������
	{
			tongxintime=0;
	}
	*/	
}


/*---------------------------------------------------------------------------------------------------------*/
//2017.8.18 wushengjun
//Function: �͹��ģ����ߣ�BDRS,GPS,CAN�� ��������û���ⲿ��Դ�ұ��õ�ص�ѹ��3.7V����
//Function Name: void dormant_exe(void)
void dormant_exe(void)
{
	if((volt_bat>37)&&(voltage < 50))//�ڵ�����﮵�ص�ʱ��Ҳ��������������ʱcan�Ѿ�ֹͣ������
	{
		
	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{ 
	
     unsigned char k=0;
	 unsigned int  limidata=0;
	  uint32_t BaudRate = 0;
   CAN_T *tCAN;
    tCAN = (CAN_T *) CAN0;	  
    /* Unlock protected registers */
    SYS_UnlockReg();
	  WDT_Close();
    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();
    /* Lock protected registers */
    SYS_LockReg();
	 
   SYS_UnlockReg();
  GPIO_INIT();
  ADC_INIT();
  SYS_LockReg();
  #if 1
  	
    /* Init UART0 for printf and testing */
  UART0_Init();//gprs
  UART1_Init();//ping
	UART2_Init();//gps

	TIMER0_Init();
	TIMER1_Init(); 
  TIMER2_Init(); 
  TIMER3_Init(); 	
	AVclose();
	Send_changepage(0);	  
	//************************************
	sendcm=EEPROMread_boundstate();//��ȡ��״̬	
	if(sendcm&BIT(boundok))
	boundstate=1;//��״̬
	else
	boundstate=0;	
	
	sendcm&=~BIT(passivelock);//ÿ���ϵ��屻������״̬
  kgflag2&=~BIT(plock);
	
	BaudRate = 250000;
	CAN_Open(tCAN,  BaudRate, CAN_NORMAL_MODE);
	CANRx_initial(tCAN);
	
 /* Unlock protected registers */
    SYS_UnlockReg();
	WDT_Open(WDT_TIMEOUT_2POW18,WDT_RESET_DELAY_1026CLK,TRUE,FALSE);
/* Lock protected registers */
    SYS_LockReg();
	  
	 P_BP &=~ BIT(L_BP);//��������
  Delay_Ms(800);
  P_BP |=  BIT(L_BP);//�ط�����
  //PORTG |= BIT(1);
  flag |= BIT(bp_bj);
	
  EEPROMread_Hous();
  //EEPROMread_Kilos();
 // EEPROMread_Meter();
  sns_read();//��Ƶʱ�䱨�����ʼ��
  /*
  pagebuff=EEPROMread(0x04);
  if(pagebuff>7)
  	pagebuff=1;
  
  save_flag=EEPROMread(0x03);
	*/
	/*
  if(save_flag & BIT(0)){flag |= BIT(av_auto); tbflag1|= BIT(4 );}//??????
     else{flag &=~ BIT(av_auto);  tbflag1|= BIT(4 );}//???????*/
	
  if(save_flag & BIT(1)){flag |= BIT(work);}
     else{flag &=~ BIT(work);}
     
  if(save_flag & BIT(2)){ECU_flag |= BIT(Miji);}
     else{ECU_flag &=~ BIT(Miji);}
  if(save_flag & BIT(3)){flag |= BIT(av_turn);   }
     else{flag &=~ BIT(av_turn);}

  
// P_vcc &=~ BIT(L_vcc);//?5V??
  //CAN_Rxbuf[2][3]=0;
  //flag |= BIT(av_turn);
  /*
    while(optime10s<95)
  	{
       
        Adc_Data();
  	}
   
  P_BP &=~ BIT(L_BP);//��������
  Delay_Ms(400);
  P_BP |=  BIT(L_BP);//�ط�����
 Delay_Ms(400);
 P_BP &=~ BIT(L_BP);//��������
   Delay_Ms(400);
  P_BP |=	BIT(L_BP);//�ط�����
 
   */
	 
	  //��Э���ʽ��ʼ���������� Ϩ�������  ֻ�����ַ�����ʽ���ڲſɷ������� ��Ҫ����ַ���������'/0'
    memset((char *)(&NewCarData) , 0 , sizeof(NewCarData));
    memcpy((char *)(&(CarDataP->Head)), "Z;",2+1);
    memcpy((char *)(&(CarDataP->SimID)), "ID00170816003;",13+1+1);  //18203623263//10000000001 // 10000000002 //10000000003 //10000000004
 	 #ifdef CCIDASSIM
	  memcpy((char *)(&(CarDataP->SimID))+2, &CCID[8],11); 
	  #endif
    memcpy((char *)(&(CarDataP->OilPressur)) , "OP050;",5+1+1);
    memcpy((char *)(&(CarDataP->WaterTemperature)) , "WT100;",5+1+1);
    memcpy((char *)(&(CarDataP->FuelVolme)) , "FV060;",5+1+1);
    memcpy((char *)(&(CarDataP->BatteryVoltage)) , "BV120;",5+1+1);
    memcpy((char *)(&(CarDataP->RotationHour)) , "RH123456;",8+1+1);
    memcpy((char *)(&(CarDataP->RotationSpeed)) , "RS1234;",6+1+1);
    memcpy((char *)(&(CarDataP->MachineState)) , "SAFFFF;",6+1+1);
		memcpy((char *)(&(CarDataP->TroubleState)) , "TRFFFF;",6+1+1);
    memcpy((char *)(&(CarDataP->End)) , "\r",2);
	 
	 memcpy((char *)(&(NewBoundComm.Head)) , "Z;",2);
	 memcpy((char *)(&(NewBoundComm.SN)) , "00000000000;",12);
	 memcpy((char *)(&(NewBoundComm.CommID)) , "CM",2);
	 memcpy((char *)(&(NewBoundComm.SimID)) , ID,11); 
	 memcpy((char *)(&(NewBoundComm.Comm)) , "BOD",3);
	 memcpy((char *)(&(NewBoundComm.End)) , ";",2);

	 memcpy((char *)(&(NewUnBoundComm.Head)) , "Z;",2);
	 memcpy((char *)(&(NewUnBoundComm.SN)) , "00000000000;",12);
	 memcpy((char *)(&(NewUnBoundComm.CommID)) , "CM",2);
	 memcpy((char *)(&(NewUnBoundComm.SimID)) , ID,11);
	 memcpy((char *)(&(NewUnBoundComm.Comm)) , "NBO",3);
	 memcpy((char *)(&(NewUnBoundComm.End)) , ";",2);
	 
	 memcpy((char *)(&(NewUnLimitComm.Head)) , "Z;",2);
	 memcpy((char *)(&(NewUnLimitComm.SN)) , "00000000000;",12);
	 memcpy((char *)(&(NewUnLimitComm.CommID)) , "CM",2);
	 memcpy((char *)(&(NewUnLimitComm.SimID)) , ID,11); 
	 memcpy((char *)(&(NewUnLimitComm.Comm)) , "NLI",3);
	 memcpy((char *)(&(NewUnLimitComm.End)) , ";",2);

	 memcpy((char *)(&(NewHaltComm.Head)) , "Z;",2);
	 memcpy((char *)(&(NewHaltComm.SN)) , "00000000000;",12);
	 memcpy((char *)(&(NewHaltComm.CommID)) , "CM",2);
	 memcpy((char *)(&(NewHaltComm.SimID)) , ID,11);
	 memcpy((char *)(&(NewHaltComm.Comm)) , "HAL",3);
	 memcpy((char *)(&(NewHaltComm.End)) , ";",2);
	 
	 memcpy((char *)(&(NewHaltComm.Head)) , "Z;",2);
	 memcpy((char *)(&(NewHaltComm.SN)) , "00000000000;",12);
	 memcpy((char *)(&(NewHaltComm.CommID)) , "CM",2);
	 memcpy((char *)(&(NewHaltComm.SimID)) , ID,11);
	 memcpy((char *)(&(NewHaltComm.Comm)) , "INQ",3);
	 memcpy((char *)(&(NewHaltComm.End)) , ";",2);
	 
	 memcpy((char *)(&(NewLimitParameterComm.Head)) , "Z;",2);
	 memcpy((char *)(&(NewLimitParameterComm.SN)) , "00000000000;",12);
	 memcpy((char *)(&(NewLimitParameterComm.CommID)) , "CM",2);
	 memcpy((char *)(&(NewLimitParameterComm.SimID)) , ID,11); 
	 memcpy((char *)(&(NewLimitParameterComm.Comm)) , "LIM",3);
	 memcpy((char *)(&(NewLimitParameterComm.ZsPara)) , "0000",3);
	 memcpy((char *)(&(NewLimitParameterComm.NjPara)) , "000",3);
	 memcpy((char *)(&(NewLimitParameterComm.End)) , ";",2);

	 memcpy((char *)(&(NewControlStateRp.Head)) , "U;",2);
	 memcpy((char *)(&(NewControlStateRp.SN)) , "00000000000;",12);
	 memcpy((char *)(&(NewControlStateRp.CommID)) , "RP",2);
	 memcpy((char *)(&(NewControlStateRp.SimID)) , ID,11);
	 memcpy((char *)(&(NewControlStateRp.Comm)) , "EXE",3);
	 memcpy((char *)(&(NewControlStateRp.StatePara)) , "0000000000000000",3);
	 memcpy((char *)(&(NewControlStateRp.End)) , ";",2);
	 
	
	 memcpy((char *)(&(NewSOS.Head)) , "U;",2);
	 memcpy((char *)(&(NewSOS.SN)) , "00000000001;",12);
	 memcpy((char *)(&(NewSOS.CommID)) , "CM",2);
	 memcpy((char *)(&(NewSOS.SimID)) , ID,11);
	 memcpy((char *)(&(NewSOS.Comm)) , "SOS",3);
	 memcpy((char *)(&(NewSOS.End)) , ";",2);
	 //��Ķ
	 memcpy((char *)(&(JMStateRpOpen.Head)) , "U;",2);
	 memcpy((char *)(&(JMStateRpOpen.SN)) , "00000000000;",12);
	 memcpy((char *)(&(JMStateRpOpen.CommID)) , "RP",2);
	 memcpy((char *)(&(JMStateRpOpen.SimID)) , ID,11);
	 memcpy((char *)(&(JMStateRpOpen.Comm)) , "MMO;",4);
	 memcpy((char *)(&(JMStateRpOpen.StatePara)) , "00000",5);
	 memcpy((char *)(&(JMStateRpOpen.End)) , ";",2);
	 
	 memcpy((char *)(&(JMStateRpClose.Head)) , "U;",2);
	 memcpy((char *)(&(JMStateRpClose.SN)) , "00000000000;",12);
	 memcpy((char *)(&(JMStateRpClose.CommID)) , "RP",2);
	 memcpy((char *)(&(JMStateRpClose.SimID)) , ID,11);
	 memcpy((char *)(&(JMStateRpClose.Comm)) , "MMC;",4);
	 memcpy((char *)(&(JMStateRpClose.StatePara)) , "00000",5);
	 memcpy((char *)(&(JMStateRpClose.End)) , ";",2);
	 
	 
	/* 
	//ģ�����
	 CarDataP->TroubleState[2]=1;
	 CarDataP->TroubleState[3]= 0x5b;
   CarDataP->TroubleState[4]= 0 ;			 
   CarDataP->TroubleState[5] =0x04 ; 
	*/

	
	SOS_Interval = 0;
	GPRS_POWERRON;
	
//	//
//	//wushengjun 2017.8.5 ���� 	
	//Flash Chaozuo 



	 
  while(1)
  {
    GprsdataReady=1;
   // Anjian_Data ();
   //Anjianproc ();
	    
   	if(CanReceiveEndFlag==1)
     {     
		   P_LED ^= BIT(L_LED); 
        CanReceiveEndFlag=0;
     }
   
   /* Unlock protected registers */
    SYS_UnlockReg();
	WDT_RESET_COUNTER();
	
	//wushengjun 2017.8.11
	BatteryChargeMangment();
		
	 /* Lock protected registers */
    SYS_LockReg();
		 
	  CAN_data();
	 //rev_js();
	  //yclockmachine();

	  //wsj 2017.8.15
	  	//Adc_Data();

	 
  if(opok)
  {
	  
      #ifdef GPRS
	  if(delay_ns == 0)	//400 >100 >0=�ػ�400ʱ��100���� ��0ʱ��ʼ������ʹ��
	  {
		  if(GprsConnectTcpState < 2)
		  {		            
		   GprsConnectTcpState =  GprsConnectTcp();	//����GPRS���� 	
		   //
		   //sysPrintf("connecting.....wait one moment!!!!!!\r\n"); 
		   	  	   
		    //Send_data(0x5F,0);	
        //GprsdataReady = 0;	
        //TransferTimeArrived = 0;				
		  }
		  else
		  {	
			   //rev_judge(); //����Ϩ�������ж�
			   #ifdef CCIDASSIM
			   if(strstr((char *)(&(NewPowerOnComm.SimID)),(char *)(&(CarDataP->SimID))) == NULL)	//SIMID��ͬ	   Ϩ�������복�������е�SIMID��ͬ
			   { 			 
			    memcpy((char *)(&(NewPowerOnComm.SimID)), (char *)(&(CarDataP->SimID))+2,11); 
				  memcpy((char *)(&(NewPowerOffComm.SimID)), (char *)(&(CarDataP->SimID))+2,11); 
				  memcpy((char *)(&(RpPowerOnComm.SimID)),(char *)(&(CarDataP->SimID))+2,11); 
				  memcpy((char *)(&(RpPowerOffComm.SimID)), (char *)(&(CarDataP->SimID))+2,11); 
			   }			
			   #endif

			 if(( IsBoundComm )&&((sendstate==1)||(sendstate==0)))	 
			   {	
					if(!sendstate)
					{
					 sendcm|=BIT(boundwait);
					 ConverStateBitToASC(sendcm,NewControlStateRp.StatePara); 
					}
					  sendstate=1;
				   GprsConnectTcpState = GprsSendControlState(NewControlStateRp); 
				  // if((GprsConnectTcpState == 4 ) ||  (GprsConnectTcpState == 0 ))//�ɹ��ͳ�ʱ�����㣬GPRS���������ӣ��ٽ����ٻظ�
				   if(GprsConnectTcpState == 4 )//�ɹ����� 
		        {
						 receivecm|=BIT(boundcommand);
		         IsBoundComm  = 0;	
					   sendstate=0;			
		        }
			   }
			 else if((IsUnBoundComm)&&((sendstate==2)||(sendstate==0)))		
			 { 	
			   		if(!sendstate)
					 {
           				sendcm|=BIT(unboundwait);
					 	ConverStateBitToASC(sendcm,NewControlStateRp.StatePara); 
					 }
					  sendstate=2;
				   GprsConnectTcpState = GprsSendControlState(NewControlStateRp); 					 
			     //if( (GprsConnectTcpState == 4 ) ||  (GprsConnectTcpState == 0 ))	 
				if(GprsConnectTcpState == 4 )//�ɹ����� 
		       {	
            	receivecm|=BIT(unboundcommand);						 
		        IsUnBoundComm  = 0;
					  sendstate=0; 
		       }			   
			 }

				  else if((IsHaltComm)&&((sendstate==3)||(sendstate==0)))		
			   { 	if(!sendstate)
					 {
            sendcm|=BIT(stopwait);
					  sendcm&=~BIT(unlimitwait);
					  sendcm&=~BIT(limitwait);
					  ConverStateBitToASC(sendcm,NewControlStateRp.StatePara); 
					 }
					 sendstate=3; 
				   GprsConnectTcpState = GprsSendControlState(NewControlStateRp); 					 
			     //if( (GprsConnectTcpState == 4 ) ||  (GprsConnectTcpState == 0 ))	 
					if(GprsConnectTcpState == 4 )//�ɹ����� 
		       {	
            receivecm|=BIT(stopcommand);
            receivecm&=~BIT(unlockcommand);
            receivecm&=~BIT(lockcommand);						 
		        IsHaltComm  = 0;
					  sendstate=0; 
		       }			   
			   }

				  else if((IsUnLimitComm)&&((sendstate==4)||(sendstate==0)))		
			   { 	if(!sendstate)
					 {
           sendcm|=BIT(unlimitwait);
					 sendcm&=~BIT(limitwait);
           sendcm&=~BIT(stopwait);
					 ConverStateBitToASC(sendcm,NewControlStateRp.StatePara); 
					 }
					  sendstate=4;
				   GprsConnectTcpState = GprsSendControlState(NewControlStateRp); 					 
			     //if( (GprsConnectTcpState == 4 ) ||  (GprsConnectTcpState == 0 ))	 
					if(GprsConnectTcpState == 4 )//�ɹ����� 
		       {	
            receivecm|=BIT(unlockcommand);	
            receivecm&=~BIT(stopcommand);
            receivecm&=~BIT(lockcommand);							 
		        IsUnLimitComm  = 0;
					  sendstate=0; 
		       }			   
			   }

				  else if((IsInquireComm )&&((sendstate==5)||(sendstate==0)))		
			   { sendstate=5; 	
           //���������ϵ��ʼ�� 
				   GprsConnectTcpState = GprsSendControlState(NewControlStateRp); 					 
			     //if( (GprsConnectTcpState == 4 ) ||  (GprsConnectTcpState == 0 ))	 
					if(GprsConnectTcpState == 4 )//�ɹ����� 
		       {	
            //receivecm|=BIT(inquirecommand);						 						 
		        IsInquireComm = 0;
					  sendstate=0; 
		       }			   
			   }
				 
				else if((IsLimitParameterComm)&&((sendstate==6)||(sendstate==0)))		
			   { 	if(!sendstate)
					 {
            sendcm|=BIT(limitwait);
					  sendcm&=~BIT(unlimitwait);
            sendcm&=~BIT(stopwait);
					  ConverStateBitToASC(sendcm,NewControlStateRp.StatePara); 
					  limidata=NewLimitParameterComm.ZsPara[0]-0x30;
					  limidata= limidata*10+NewLimitParameterComm.ZsPara[1]-0x30;
					  limidata= limidata*10+NewLimitParameterComm.ZsPara[2]-0x30;
					  engzs= limidata*10+NewLimitParameterComm.ZsPara[3]-0x30;
					  
					  limidata=NewLimitParameterComm.NjPara[0]-0x30;
					  limidata= limidata*10+NewLimitParameterComm.NjPara[1]-0x30;
					  engniuju= limidata*10+NewLimitParameterComm.NjPara[2]-0x30;
					 }
					 sendstate=6; 
				   GprsConnectTcpState = GprsSendControlState(NewControlStateRp); 					 
			     //if( (GprsConnectTcpState == 4 ) ||  (GprsConnectTcpState == 0 ))	 
					if(GprsConnectTcpState == 4 )//�ɹ����� 
		       {	
            receivecm|=BIT(lockcommand);
            receivecm&=~BIT(stopcommand);
            receivecm&=~BIT(unlockcommand);
		        IsLimitParameterComm  = 0;
					  sendstate=0; 
		       }			   
			   }
 
				 
			 else if((IsSOS)&&((sendstate==7)||(sendstate==0)))
			   {  sendstate=7;
			   	 GprsConnectTcpState = GprsSendRp(NewSOS); 
				   //	Send_data(0x5F,1);
				  if( GprsConnectTcpState == 4 )
		      {  sendstate=0;
		        IsSOS  = 0;
					  //Send_data(0x5F,3);	  //���ͳɹ�
					  SOS_Interval = 9900;
		       }
				  else if(GprsConnectTcpState == 0 )
				  {  sendstate=0;
				  	 IsSOS  = 0; 	
					  //Send_data(0x5F,4);		//����ʧ��	
					  SOS_Interval = 9900;  
				  }
				   //else
				  // Send_data(0x5F,2);	   //������	  					
			   }
			   else if((IsJMStateRpOpen )&&((sendstate==9)||(sendstate==0)))		
			   { sendstate=9; 	
				   GprsConnectTcpState = GprsSendJMRp(JMStateRpOpen); 					 
					if(GprsConnectTcpState == 4 )//�ɹ����� 
		       		{					 						 
		        		IsJMStateRpOpen = 0;
					    sendstate=0; 
		       		}			   
			   }
				 else if((IsJMStateRpClose )&&(seedcosejm)&&((sendstate==10)||(sendstate==0)))		
			   	 { sendstate=10; 	
				   GprsConnectTcpState = GprsSendJMRp(JMStateRpClose); 					 
					if(GprsConnectTcpState == 4 )//�ɹ����� 
		       		{	
            					 						 
		        			IsJMStateRpClose = 0;
						 	//seedcosejm=0;
					  		sendstate=0; 
		       		}			   
			     }
			    else
			    {//û������ʱ�ϴ�����
			   	 //if(SOS_Interval == 0)
					 //Send_data(0x5F,1);
				   if(((GprsdataReady ) && (TransferTimeArrived))&&((sendstate==8)||(sendstate==0)))   //TransferTimeAr �ϵ� �ҵ�����ʱ��
			       {   if(!sendstate)
			 		       ConvertCanToCarDat();
							   sendstate=8;
			          //GprsConnectTcpState = GprsSendData(NewCarData);//���ͳ���״̬��Ϣ
					  
					  
					    //wushengjun add 2017.8.3
					    //
						
						BaoJingValueObj.WaiBuDianYuanPowerOff = WaiBuDianYuanPowerOffFun();
						BaoJingValueObj.GPSCaiChuBaoJing = GPSCaiChuBaoJingFun();
						BaoJingValueObj.BackBatteryPowerOff = BackBatteryPowerOffFun();
						BaoJingValueObj.SIMCardChangeAlarm = SIMCardChangeAlarmFun();

						 
						if(BaoJingValueObj.WaiBuDianYuanPowerOff == 1)
						{
							FlagBaoJing = 1;
							//BJWaiBuDianYuan++;	
						}
						if(BaoJingValueObj.GPSCaiChuBaoJing == 1)//��ʱ������
						{
							FlagBaoJing = 1;
							//BJGPSCaichu++;	
						}
						if(BaoJingValueObj.BackBatteryPowerOff ==1)
						{
						   	FlagBaoJing = 1;
							//BJBeiyongDianchiQianYa++;	
						}
						if(BaoJingValueObj.SIMCardChangeAlarm == 1)
						{
							FlagBaoJing = 1;
							//BJSIMCaDifferent++;	
						}
					
						//
						if((BaoJingValueObj.WaiBuDianYuanPowerOff == 0)&&(BaoJingValueObj.GPSCaiChuBaoJing == 0)&&(BaoJingValueObj.BackBatteryPowerOff ==0)&&(BaoJingValueObj.SIMCardChangeAlarm == 0))
						{
							FlagBaoJing = 0;						
						}

						#if 0
						if(((FlagBaoJing)&&(BJWaiBuDianYuan<3))||((FlagBaoJing)&&(BJBeiyongDianchiQianYa<3))||((FlagBaoJing)&&(BJSIMCaDifferent<3))||((FlagBaoJing)&&(BJGPSCaichu<3)))
						{
						  BJWaiBuDianYuan++;
						  AlarmBDRSSend(NewCarData); 	//GprsConnectTcpState =
						}
						else
						{
						 GprsConnectTcpState = GprsSendData(NewCarData);//���ͳ���״̬��Ϣ
						}

						#endif

							/*
							if((volt_bat>37)&&(voltage < 50))//�ڵ�����﮵�ص�ʱ��Ҳ��������������ʱcan�Ѿ�ֹͣ������
							{
									tongxintime=0;
							}
							*/
		   
						   #if 1
						   if((lsnum<3)&&(FlagBaoJing == 1))
						   {
						   		
						   		GprsConnectTcpState = AlarmBDRSSend(NewCarData); 	//GprsConnectTcpState =	
								lsnum++;
							
						   }
						   else
						   {
						   		GprsConnectTcpState = GprsSendData(NewCarData);//���ͳ���״̬��Ϣ
								/*
								cdnum++;
								if(cdnum>655)
								{
									lsnum = 0;
									cdnum = 0;
								}
								*/
						   }
						   #endif


						   //GprsConnectTcpState = GprsSendData(NewCarData);//���ͳ���״̬��Ϣ



						if((GprsConnectTcpState == 4 )||(GprsConnectTcpState == 0 ))
						{
							TransferTimeArrived  = 0;
							// GprsdataReady = 0;
							sendstate=0;
						}
				  } 
			   }	    
		    }
			if(GpsRecievedNew)//GpsRecievedNew ���յ��µ�GPS��λ��Ϣ
			{		
			   memcpy((void *)(CarDataP->GpsData),(void *)(gpsdats),strlen((void *)gpsdats)-2);	//-2��ȥ��ĩβ�Ļس�����
			   GpsRecievedNew = 0;   
				
				#if 1
        		if(GPIO_GET_IN_DATA(PB) & BIT13)
				{
					if( opdelay>=20)//��ʱ2s��ʼ��Ķ
					{
					 if(!jmopen)
					 {	
						closeround=0;
						firstone=0; 
						jmopen=1;
						S=0;//��Ķ��ʼ�����ϴβ�����
						polar=0;//��������� 
						snum++;
						ConverJMsnumToASC(snum,JMStateRpOpen.SN);
						ConverJMsnumToASC(snum,JMStateRpClose.SN);
            //ConverJMdataToASC(S,JMStateRpOpen.StatePara); 						
						//���ͼ�Ķ��ʼ����
						//GprsSendJMRp(JMStateRpOpen);
						 IsJMStateRpOpen=1;
					 }
					 exejscloseround=1;//��Ķ��ʼ
				  }
				}
				else
				{	
					  opdelay=0;
            		  jmopen=0;					 
					/*if((exejscloseround==1)&&(!closeround)) //��ʼ��Ķ��û���γɷ��Բʱֱ���γɷ��Բ����
					 {
						 exejscloseround=2;
						 exejscloseroundtiem=0;
					 } 
					 else if(!exejscloseround)
					 {
						// exejscloseround=0;
						 closeround=0;
						 firstone=0;
					 } */
					 
					 if(exejscloseround==1)//��ʼ��Ķ��û���γɷ��Բʱֱ���γɷ��Բ����
					 {	
					   if(!closeround)
					   {
						 exejscloseround=2;
						 exejscloseroundtiem=0;
					   }
            		   else 
					   {
						 exejscloseround=0;
						 seedcosejm=1;//������Ķʱ����������	
						 closeround=0;
						 firstone=0;
            		   }	
				     }					 
					 else if(!exejscloseround)
					 {
						 //exejscloseround=0;
						 seedcosejm=1;//������Ķʱ����������		
						 closeround=0;
						 firstone=0;
					 } 					 					 					
				}				 
				if(exejscloseround) 
				{
					if((jmtime>=10)||(exejscloseround==2))
					{
					jmtime=0;
					jwdcap();
					}
        		}
				#endif
				
			   if( VerifyGps(CarDataP->GpsData)) //��GPS���ݸ�ʽ���ݺ�У����У��
			   {
		      		GpsdataReady = 1;  //��ȷ���ձ�־
			   }
			   else
			   GpsdataReady = 0;
		 }
	} 
	#endif	

	/*	 
	jsshisu();
	  keyscan();	 
    alert_jc();
	uart_manage();
	// mijichg();	  
	*/

#if 1
	Delay_Ms(400);
 
	/*
	//can
	Send_longdata(0x60,Engine_hours);
	NewMachineFrame.u32hours = Engine_hours;
  */	
	//�Լ�ʱ
  Send_longdata(0x60,hours);
	NewMachineFrame.u32hours = hours;		
	
	if(oil_m>8){oil_m=8;}
	Send_data(0x50,oil_m);
	//NewMachineFrame.u8fuelVol  =  oil_m;
	//if(oil_p>10){oil_p=10;}
	Send_data(0x51,oil_p);
  NewMachineFrame.u8oilPressure =  oil_p;	
	Send_data(0x52,watmp);
	NewMachineFrame.u8waterTemperature = watmp;
	if(watmp<40) {watmp1=40;}
	if(watmp>120){watmp1=120;}
	Send_data(0x55,watmp1);
	
	//if(press_tep>120){press_tep=120;}
	//Send_data(0x53,press_tep);
	Send_data(0x54,fdj_rev);
	NewMachineFrame.u16engineRotate = fdj_rev;
	//Send_data(0x63,avtime);
	Send_data(0x64,voltage);
	NewMachineFrame.u16voltage = 	 voltage;
	
	Send_data(0x5a,tbflag1);
	Send_data(0x5b,tbflag2);
	Send_data(0x5c,tbflag3);
	Send_data(0x5d,tbflag4);
	//Send_longdata(0x67,err); 
	Send_longdata(0x67,spn0); 
 
	
	Send_data(0x78,Shisu.value/10);
	if(ECU_flag & BIT(Miji))
	{Send_longdata(0x79,cur_meter);}
	else
	{Send_longdata(0x79,sum_kilometre);}
	NewMachineFrame.u32milsage = sum_kilometre;
	
	//Send_longdata(0x79,ylad);//������
	/* k++;
	if(k>10)
	{
	  k=0;

	  if(!startokbp)
    {
    P_BP &=~ BIT(L_BP);//��������
    Delay_Ms(400);
    P_BP |=  BIT(L_BP);//�ط�����
    Delay_Ms(400);
    P_BP &=~ BIT(L_BP);//��������
    Delay_Ms(400);
     P_BP |=BIT(L_BP);//�ط�����
	 startokbp=1;
    }
	 pageturn();
		AVchange();
	  //EEPROMwrite_Hours();
		 if(kilometretemp)
		 {
			 EEPROMwrite_Kilos();
			 kilometretemp=0;
		 }
	  save();
	  backled();
	 }*/

#endif

		Adc_Data();
		if(mmhoursopen>=30)
		{
			if(mmhours)
			{
		   EEPROMwrite_Hours();
			 mmhours=0;
			}
		}


	}    
  }
	 
   
#endif
}








