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
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 10);   //10HZ周期中断
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
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1);   //1HZ周期中断
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
   // TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 10);   //10HZ周期中断  16777216   备电24h=24*60*60*100=8640000  主电 2h=2*60*60*100=720000  5m=5*60*100=30000
	  //TIMER2->TCSR = TIMER_PERIODIC_MODE | 100;
	  TIMER2->TCSR = TIMER_ONESHOT_MODE  | 100;  //单周期，分频100
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
    TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 10);   //10HZ周期中断
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
				if( GprsConnectTcpState < 2 )//正在连接-1;超时-0;
				 {      
						if(gprsNoConnect < 50*60) //5分钟未建立连接
						{
							gprsNoConnect++;
						}            
						else	//大于1分钟,关闭GPRS电源;
						{
							GPRSPOWEROFF;
							delay_ns = 100;//10s
							gprsNoConnect = 0;	
              at_command_order = 0;//命令序号	
						 AtCommSendingState = COMMSENDED;	//更新 发送命令状态
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
				 /**********///接收数据计时
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
				 
				 //数码管 LED灯自检时间	
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
	
	//GPS终端总通电时间  不休眠+主电休眠
	gps_tem_time++;
	if(gps_tem_time>=60)//分钟
	 {
	  gps_tot_time++;
		 gps_tem_time=0;
	 }
	
	
	//if((dormantlevel==0)||(dormantlevel==2)||(dormant_wakeup==1))
	//if((dormantlevel==0)||(dormant_wakeup==1)||(dormant1stat<5))	
	//if((dormantlevel!=1)||(dormant_wakeup==1)||(dormant1stat<5))	
  if((dormantlevel==0)||(dormant_wakeup==1)||((dormant1stat<4)&&(dormantlevel==1))||((dormant3stat==0)&&(dormantlevel==2)))				
	{		
	 //GPS高电平检测输入异常计时
	if(tongxintime<50)
	{
	  GPShighlevelchecktime++;
   if(GPShighlevelchecktime>=10)//10s
		GPShighlevelchecktime=10;
  }
	else
	GPShighlevelchecktime=0;	
	
	/*******/ //GPS故障时间
	GPSvoidtimes++;
	if(GPSvoidtimes>60)
	{
		GPSvoidtimes=0;
		GPSvoidtimem++;
		if(GPSvoidtimem>367200)//255天
			GPSvoidtimem=367200;
  }
	//CAN总线故障报警时间  PLC有电12分钟扔无can数据
	if(TerminalReply0x110x18.GPS.state1&BIT(5))
	{
	  CANvoidtimes++;
	if(CANvoidtimes>720)//12分钟
		CANvoidtimes=720;
  }
	else
		CANvoidtimes=0;
	
	//gps总里程
	if((TerminalReply0x110x18.GPS.state1&(BIT(7)))&&(speed_s>300)&&(vibrationtime<60)&&(Parameterset.Parametersetitem.GPSwxs0x13>4))//定位成功 速度大于1km/h 有振动时 开始计算里程 大于4颗星
	 gps_tem_lc+=speed_s;
	if(gps_tem_lc>=1000)
	{
		gps_tem_lc-=1000;
		gps_tot_lc++;
		gps_cur_lc++;
		if((Parameterset.Parametersetitem.wzhbcl0x41)&&(speed_s>555))//有定距功能
		gps_dj_lc++;//定距距离
		else
		gps_dj_lc=0;//定距距离	
  }
	//当次点火后里程处理
	 if(CANID0280.zhuansu[4]<10)//>300转
		gps_cur_lc=0;
	 
	//ACCon累计时间
	if(TerminalReply0x110x18.GPS.state1&(BIT(5)))
	{	
	 ACCon_tem_time++;
	 if(ACCon_tem_time>=60)//分钟
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
// 	//GPS终端总通电时间  不休眠+主电休眠
// 	gps_tem_time++;
// 	if(gps_tem_time>=60)//分钟
// 	 {
// 	  gps_tot_time++;
// 		 gps_tem_time=0;
// 	 }
  //登录服务中心等待回复270s
	  ZDParameter.LoginTimer++; 
	 if(ZDParameter.LoginTimer > 270)
		ZDParameter.LoginTimer = 270;
		
	// Timer1Count++;		
    if(ZDParameter.LoginReply)  
	 {	
     Timer1Count++;				 
		if((Timer1Count%(20)) == 0 )	   //20秒
      TimeArrived0x13=1;//终端上传CAN总线			
		
		if((ZDParameter.TraceCount)&&(ZDParameter.IntervalTimer))//次数大于1，按跟踪时间间隔发送 >0
		{
		  if((Timer1Count%(ZDParameter.IntervalTimer)) == 0 )
			{
				TimeArrivedcreg=1;//先查基站	后发送
				TimeArrived0x11=1;//定位查询的回复
				ZDParameter.TraceCount--;
      }
    }
		else if((speed_s>555)&&(TerminalReply0x110x18.GPS.state1&(BIT(7))))//定位成功开始计算里程       //行驶状态下 0.3m/s=1.08km/h执行位置汇报策略  0.555m/s=2km/h
		{
			if(Parameterset.Parametersetitem.wzhbcl0x41==2)//定时定距 注距离不加
			{ 
				//if(((Timer1Count%(Parameterset.Parametersetitem.dssbsj0x42))==0)||((gps_tot_lc%(Parameterset.Parametersetitem.djsbjl0x43*10))==0))//30s	 20=200m
				if(((Timer1Count%(Parameterset.Parametersetitem.dssbsj0x42))==0)||(gps_dj_lc>=(Parameterset.Parametersetitem.djsbjl0x43*10)))//30s	 20=200m	
			  { 
					if(gps_dj_lc>=(Parameterset.Parametersetitem.djsbjl0x43*10))//	 20=200m	 
				  gps_dj_lc=0;
					
				TimeArrivedcreg=1;//先查基站	后发送
				TimeArrived0x18=1;	
				}					
      }	
			else if(Parameterset.Parametersetitem.wzhbcl0x41==1)//定距
			{
				//if((gps_tot_lc%(Parameterset.Parametersetitem.djsbjl0x43*10))==0)//	 20=200m
			 if(gps_dj_lc>=(Parameterset.Parametersetitem.djsbjl0x43*10))//	 20=200m	
			 { 
				gps_dj_lc=0;
				TimeArrivedcreg=1;//先查基站	后发送
			  TimeArrived0x18=1;
			 }						
      }
			else if(Parameterset.Parametersetitem.wzhbcl0x41==0)//定时
			{
				if((Timer1Count%(Parameterset.Parametersetitem.dssbsj0x42))==0)//30s	
				{ 
				TimeArrivedcreg=1;//先查基站	后发送	
        TimeArrived0x18=1;
				}					
      }	
    }
		else if(autorep_loctime)//非行驶状态下
		{
			 //if((Timer1Count%(30)) == 0 )	   //30秒
			 if((Timer1Count%(autorep_loctime)) == 0 )	   //默认240秒
			 { 
				TimeArrivedcreg=1;//先查基站	后发送
		    TimeArrived0x18=1;//自动回报
			 }
    }
		
		   //if((Timer1Count%(28800)) == 0 )	   //8H 60*60*8
			 if((Timer1Count%(7200)) == 0 )	   //2H 60*60*2=7200  主电2小时   备电24h
		   TimeArrived0x28=1;//每2小时上传一次副参数
			 
			 
			//终端上传报警信息  每连续1小时未收到服务器指令  登录成功后计时
		sendbjxx0x17time++;
		if(sendbjxx0x17time>=3600)
			sendbjxx0x17time=3600;

		//终端上传报警信息 等待回复时间 
     if(sendbjxx0x17repovertime)//应答15s	
			 sendbjxx0x17repovertime--;
    
    PA3=~PA3; //登录指示灯
	 } 
	 else
	 { 
		PA3=0; 
		Timer1Count=0;	
	 }
	 
	 
	 //灯光闪烁时间
	 Timer11Count++;		
	 if(numdisp_testok)//数码管LED灯自检完成
	 { 
    //绿灯闪烁与服务器建立连接 tcp
	 if(GprsConnectTcpState < 2)
		PC6=0; 
    else
		{	
      //PC6=~PC6;
			if((Timer11Count%2) == 0) //2秒	
       PC6=0;
			 else
			 PC6=1; 
		}			
		//红灯闪烁GPS定位成功
   	if(TerminalReply0x110x18.GPS.state1&(BIT(7)))//成功定位
		{
		//PA6=~PA6; 
			if((Timer11Count%2) == 0) //2秒	
       PA6=0;
			 else
			 PA6=1; 
		}
    else
    PA6=0;	
		
		//黄灯闪烁收到GSM信号
		if(Parameterset.Parametersetitem.GSMxhqd0x12>10)
		{
			//PB15=~PB15;//执行错误
      if((Timer11Count%2) == 0) //2秒	
       PB15=0;
			 else
			 PB15=1; 
    }
    else
    PB15=0;	
		//小数点闪烁
		if((Timer11Count%2) == 0) //2秒	
		{
     PB9=0;
		 numdisp8();//灭8
		}
		else
		{
		PB9=1; 
    numdisp();	//数显切换
		}		
	 }
		//有效振动持续1分钟
		vibrationtime++;
   if(vibrationtime>=60)
		vibrationtime=60;
		//有效转场持续5分钟
	 if((vibrationtime<60)&&(speed_s>555))//有振动 时速大于2km/h
	 {
		ferrytime++;
		if(ferrytime>=300)//5分钟
		ferrytime=300;
	 }
	 //无效转场持续5分钟
	 if((vibrationtime==60)&&(speed_s<555))//有振动 时速大于2km/h
	 {
		if(ferrytime)//5分钟
		ferrytime--;
	 }
	  
   //收到CAN信息
	 if(CanReceiveEndFlag==1)
     {     
		  PA4=~PA4; 
      CanReceiveEndFlag=0;
     }
		 else
			PA4=0; 
	 }		 
	 //ACCOFF时间计时
// 	if((!(TerminalReply0x110x18.GPS.state1&BIT(5)))&&(voltage<241)&&(vibrationtime>=60)
// 		&&(smsnumber<4)&&(!readsmsnumok)&&(!dxbjhmflag)&&(at_command_order >=10)&&(recgprscom==0 ))//ACC OFF 同时电压低于24.1v 同时无振动 无短信和GPRS命令
	if((!(TerminalReply0x110x18.GPS.state1&BIT(5)))&&(voltage<241)&&(vibrationtime>=60)
		&&(smsnumber<4)&&(!readsmsnumok)&&(!dxbjhmflag)&&(recgprscom==0 ))//ACC OFF 同时电压低于24.1v 同时无振动 无短信和GPRS命令
	 {	 dormant_ACCofftime++;
		 if(dormant_ACCofftime>=600)//10分钟
			 dormant_ACCofftime=600;
		 //if(voltage<Parameterset.Parametersetitem.zdqyz0x1D)//欠压可马上进入休眠
		 if((TerminalReply0x110x18.GPS.state3&BIT(1))||(TerminalReply0x110x18.GPS.state3&BIT(2)))//断电或欠压报警
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
	
	if(((dormantlevel)||(dormant_wakeup))&&(voltage>245))//在休眠或唤醒条件下
	{
		time_245v++;
		if(time_245v>=600)
		time_245v=600;
  }
	else
	time_245v=0;	
	
	//唤醒时间倒计时
	if(wakeuptime2min)//120 2分钟
	 wakeuptime2min--;
	else
	 dormant_wakeup=0;	
	//主电休眠唤醒时间
	if((dormantlevel==1)&&(dormant_wakeup==0)&&(dormant1stat==5))
	{
		dormanttime++;
		//if(dormanttime>=720000)//2小时
		//if(dormanttime>=20)//20s
		if(dormanttime>=60)//60s	
		{
			dormanttime=0;
			send0xAA=1;
			//TerminalReply0x17.baojingflag=0xAA;
// 			TimeArrivedcreg=1;//先查基站	后发送
// 		  TimeArrived0x17=1;//上传报警信息
			//TimeArrived0x28=1;//上传副参数
// 			//主电休眠唤醒
// 			//if((dormantlevel==1)&&(dormant_wakeup==0))
// 			if(dormantlevel==1)
// 		  {
		  wakeuptime2min=120;
		  dormant_wakeup=2;//执行唤醒操作
		// }
    }		
	}
	else
		dormanttime=0;	
	
	//主电休眠小数点闪烁
	 if((dormant1stat==5)&&(dormantlevel==1))
	 PB9=~PB9;
		//GPRS连接失败计时
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
			send0xAA=1;//时间唤醒后报警
// 		if(dormantlevel==1)
// 		{
// 		 wakeuptime2min=120;
// 		 dormant_wakeup=2;//执行唤醒操作
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
		GPIO_SetMode(PA, BIT8, GPIO_PMD_QUASI);PA8=0;//IN33  检测低电平
    GPIO_SetMode(PA, BIT9, GPIO_PMD_INPUT);//IN11  
    GPIO_SetMode(PA, BIT10, GPIO_PMD_OUTPUT); //PW_GPS
	  PA10 = 0;//供电
    GPIO_SetMode(PA, BIT11, GPIO_PMD_INPUT); //REV
		GPIO_SetMode(PA, BIT12, GPIO_PMD_QUASI);PA12=1; //ct_33v
		GPIO_SetMode(PA, BIT13, GPIO_PMD_QUASI);PA13=1; //ct_18v
		GPIO_SetMode(PA, BIT14, GPIO_PMD_INPUT);//BATJC3作为电池存在检测端口
		//GPIO_SetMode(PA, BIT15, GPIO_PMD_INPUT); //ct_24v
    //GPIO_SetMode(PA, BIT15, GPIO_PMD_QUASI); //ct_24v
		//GPIO_SetMode(PA, BIT15, GPIO_PMD_OPEN_DRAIN ); //ct_24v
		GPIO_SetMode(PA, BIT15, GPIO_PMD_OUTPUT);//ct_24v
	  PA15 = 0;//外部供电
	 
	 
	  GPIO_SetMode(PB, BIT0, GPIO_PMD_INPUT);
		GPIO_SetMode(PB, BIT1, GPIO_PMD_INPUT);
		GPIO_SetMode(PB, BIT2, GPIO_PMD_QUASI);//ct_5156
		PB2 = 0;//停充
		GPIO_SetMode(PB, BIT3, GPIO_PMD_OUTPUT);//PW_GPRS
		PB3 = 0;//供电
		GPIO_SetMode(PB, BIT6, GPIO_PMD_INPUT); //IN22
		GPIO_SetMode(PB, BIT7, GPIO_PMD_INPUT); //IN44
		GPIO_SetMode(PB, BIT9, GPIO_PMD_QUASI);PB9=0;//DP1
		GPIO_SetMode(PB, BIT12, GPIO_PMD_INPUT);//VTBJC1
		//GPIO_EnableInt(PB, 12, GPIO_INT_FALLING);//振动1下降沿中断检测
		GPIO_EnableInt(PB, 12, GPIO_INT_BOTH_EDGE);//振动1下降沿中断检测
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
		PD14 = 0;//0 工作，1休眠
		GPIO_SetMode(PD, BIT15, GPIO_PMD_INPUT);//sunjc1
		
		//GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT);//CT_BATT
    GPIO_SetMode(PE, BIT5, GPIO_PMD_OPEN_DRAIN ); //CT_BATT
		PE5 = 1;//外部供电控制
		GPIO_SetMode(PF, BIT4, GPIO_PMD_INPUT);//VTBJC3作为电池存在检测端口
		GPIO_SetMode(PF, BIT5, GPIO_PMD_INPUT);//VTBJC2
		//GPIO_EnableInt(PF, 5, GPIO_INT_FALLING);//振动2下降沿中断检测
		GPIO_EnableInt(PF, 5, GPIO_INT_BOTH_EDGE);//振动2下降沿中断检测
		GPIO_ENABLE_DEBOUNCE(PF, BIT5);
    NVIC_EnableIRQ(GPCDEF_IRQn);
		GPIO_SetMode(PF, BIT8, GPIO_PMD_INPUT);//IN23.75V
		
// 		
// 		//掉电
//     GPIO_SetMode(PB, BIT14, GPIO_PMD_INPUT);
//     GPIO_EnableEINT0(PB, 14, GPIO_INT_LOW);
//     NVIC_EnableIRQ(EINT0_IRQn);
	 
// 	 //时速
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
		
		if(GPIO_GET_INT_FLAG(PA, BIT8)) //acc低
    {
			GPIO_CLR_INT_FLAG(PA, BIT8);
			
// 			//主电休眠唤醒
// 			//if((dormantlevel==1)&&(dormant_wakeup==0))
// 			if(dormantlevel==1)	
// 		  {
// 		  //wakeuptime2min=120;
// 		  dormant_wakeup=2;//执行唤醒操作
// 		  }
    }	
		
		if(GPIO_GET_INT_FLAG(PB, BIT7)) //acc高
    {
			GPIO_CLR_INT_FLAG(PB, BIT7);
// 			vibrationtime=0;
// 			//主电休眠唤醒
// 			//if((dormantlevel==1)&&(dormant_wakeup==0))
// 			if(dormantlevel==1)	
// 		  {
// 		  //wakeuptime2min=120;
// 		  dormant_wakeup=2;//执行唤醒操作
// 		  }
    }	
		
		
		if(GPIO_GET_INT_FLAG(PB, BIT12)) //振动1
    {
			GPIO_CLR_INT_FLAG(PB, BIT12);
			vibrationtime=0;
			//主电休眠唤醒
			//if((dormantlevel==1)&&(dormant_wakeup==0))
			if(dormantlevel==1)	
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//执行唤醒操作
		  }
    }	
		
    	if(GPIO_GET_INT_FLAG(PB, BIT14))
    {
			GPIO_DisableInt(PB, 14);//开盖关盖开中断
			GPIO_CLR_INT_FLAG(PB, BIT14);
			
			//主电休眠唤醒
			if((dormantlevel==1)&&(dormant_wakeup==0))
		 //if(dormantlevel==1)
		  {
		  wakeuptime2min=120;
		  dormant_wakeup=2;//执行唤醒操作
				
		  }
    }			
  }
  void GPCDEF_IRQHandler(void) 
  {
		if(GPIO_GET_INT_FLAG(PF, BIT5))//振动检测2
    {
			GPIO_CLR_INT_FLAG(PF, BIT5);
			vibrationtime=0;
			//主电休眠唤醒
			//if((dormantlevel==1)&&(dormant_wakeup==0))
			if(dormantlevel==1)
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//执行唤醒操作
		 }
    }	
		if(GPIO_GET_INT_FLAG(PF, BIT8))//达到23.1v
    {
			GPIO_DisableInt(PF, 8);//IN23.75V 低电平中断 可由备电近主电
			GPIO_CLR_INT_FLAG(PF, BIT8);
			
			//主电休眠唤醒
			if((dormantlevel==1)&&(dormant_wakeup==0))
		 //if(dormantlevel==1)
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//执行唤醒操作
				
		  }
    }	
		if(GPIO_GET_INT_FLAG(PD, BIT15))
    {
			GPIO_DisableInt(PD, 15);//开盖关盖开中断
			GPIO_CLR_INT_FLAG(PD, BIT15);
			
			//主电休眠唤醒
			if((dormantlevel==1)&&(dormant_wakeup==0))
		 //if(dormantlevel==1)
		  {
		  //wakeuptime2min=120;
		  dormant_wakeup=2;//执行唤醒操作
				
		  }
    }			
  }

	
	
	
//为提高反应ACC监测速度
void accstate_check(void)
{ 
	//sta15
	if(!Parameterset.Parametersetitem.PLCsrk0x20)//PLC上电高有效
	{
		if(PB7) 
		{
			if(checkcount&BIT(0))//检测2次确认
			TerminalReply0x110x18.GPS.state1 |= BIT(5);
			checkcount|= BIT(0);
		}	
		else
		{
			checkcount&= ~BIT(0);
		  TerminalReply0x110x18.GPS.state1 &= ~BIT(5);
		}
  }
	else//低有效
	{
		if(!PA8) 
		{
			if(checkcount&BIT(0))//检测2次确认
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
// 	if(!Parameterset.Parametersetitem.PLCsrk0x20)//PLC上电高有效
// 	{
// 		if(PB7) 
// 		{
// 			if(checkcount&BIT(0))//检测2次确认
// 			TerminalReply0x110x18.GPS.state1 |= BIT(5);
// 			checkcount|= BIT(0);
// 		}	
// 		else
// 		{
// 			checkcount&= ~BIT(0);
// 		  TerminalReply0x110x18.GPS.state1 &= ~BIT(5);
// 		}
//   }
// 	else//低有效
// 	{
// 		if(!PA8) 
// 		{
// 			if(checkcount&BIT(0))//检测2次确认
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
	//GPS天线故障判断
	if(!(TerminalReply0x110x18.GPS.state1&(BIT(7))))//没有成功定位
	{
	 if(GPSvoidtimem>10)//10分钟
	 {
		 TerminalReply0x110x18.GPS.state1 |= BIT(1);//GPS天线故障
		 if((!baGPSantrecord)&&(!(memparamset.memparam.flagchange&(BIT(1)))))//本次更新一次，并且上次开机GPS天线正常
		 {
		  memparamset.memparam.baGPSantnum++;//拔GPS天线次数
			baGPSantrecord=1;
			memparamset.memparam.flagchange|=BIT(1);
		 }
	 }
	}
	else
	{//一次正常全部清除
		GPSvoidtimes=0;
		GPSvoidtimem=0;
		TerminalReply0x110x18.GPS.state1 &= ~BIT(1);
		baGPSantrecord=0;//清本次
		memparamset.memparam.flagchange&=~BIT(1);//清上次
	}
	//sta10
	if(tongxintime<50)
	{
		TerminalReply0x110x18.GPS.state1 |= BIT(0);//总线有输出
		CANvoidtimes=0;
	}
	else
	 TerminalReply0x110x18.GPS.state1 &= ~BIT(0);	
	//sta27
	if(memparamset.memparam.simkaghcs)//sim卡更换次数大于0
	TerminalReply0x110x18.GPS.state2 |= BIT(7);//更换报警	
	else
	TerminalReply0x110x18.GPS.state2 &= ~BIT(7);		
// 	//sta26
// 	if((PB14)&&(PD15))//暗
// 	{
// 		if(checkcount&BIT(1))//检测2次确认
// 		TerminalReply0x110x18.GPS.state2 &= ~BIT(6);
// 		checkcount|= BIT(1);
// 	}
//   else	
// 	{
// 		checkcount&= ~BIT(1);
// 	  TerminalReply0x110x18.GPS.state2 |= BIT(6);//开盖报警	
// 	}
	//sta26
	if((PB14)&&(PD15))//暗
	{
		checkcount&= ~BIT(1);
		TerminalReply0x110x18.GPS.state2 &= ~BIT(6);
	}
  else	
	{
		
		if(checkcount&BIT(1))//检测2次确认
		{
	   TerminalReply0x110x18.GPS.state2 |= BIT(6);//开盖报警	
// 			//主电休眠唤醒
// 			if((dormantlevel==1)&&(dormant_wakeup==0))
// 			//if(dormantlevel==1)		
// 		  {
// 		  wakeuptime2min=120;
// 		  dormant_wakeup=2;//执行唤醒操作
// 		  }
		}	
		 checkcount|= BIT(1);
	}
	
	
	
	//sta25
	if(memparamset.memparam.baSIMnum)// 拔SIM卡次数
	TerminalReply0x110x18.GPS.state2 |= BIT(5);//拔卡报警	
	else
	TerminalReply0x110x18.GPS.state2 &= ~BIT(5);	
	//sta36 CAN波特率
	if(Parameterset.Parametersetitem.BaudRate0x07==125)
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(6);
  else
   TerminalReply0x110x18.GPS.state3 |= BIT(6);		
	//sta34  备用电池欠压报警
	if((volt_bat<37)&&(!(TerminalReply0x110x18.GPS.state3&BIT(3))))//3.7v 且备用电池存在情况下检测
	{
	 if(checkcount&BIT(2))//检测2次确认
	 TerminalReply0x110x18.GPS.state3 |= BIT(4);	
	 checkcount|= BIT(2);
	}		
	else
	{
	 checkcount&= ~BIT(2);
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(4);
	}
	
// 	//sta33 备用电池断电报警  在主电工作或备电休眠时
// 	if(volt_bat<2)//2v
// 	TerminalReply0x110x18.GPS.state3 |= BIT(3);		
// 	else
// 	TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
	//sta32 主电源欠压报警
	//if(voltage<=225)//22.5v
	if((voltage<=Parameterset.Parametersetitem.zdqyz0x1D)&&(voltage>90))//断电不算欠压
	{
		if(checkcount&BIT(4))//检测2次确认
		{
	  TerminalReply0x110x18.GPS.state3 |= BIT(2);
// 		 //主电休眠唤醒
// 			if((dormantlevel==1)&&(dormant_wakeup==0))
// 			//if(dormantlevel==1)	
// 		  {
// 		  wakeuptime2min=120;
// 		  dormant_wakeup=2;//执行唤醒操作
// 		  }	
		}			
		checkcount|= BIT(4);
	}		
	else
	{
		checkcount&= ~BIT(4);
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(2);
	}
	//sta31 主电源断电报警
	if(voltage<=50)//5v
	{	
		if(checkcount&BIT(5))//检测2次确认
		{
	  TerminalReply0x110x18.GPS.state3 |= BIT(1);
// 			//主电休眠唤醒
// 			if((dormantlevel==1)&&(dormant_wakeup==0))
// 			//if(dormantlevel==1)	
// 		  {
// 		  wakeuptime2min=120;
// 		  dormant_wakeup=2;//执行唤醒操作
// 		  }
		}
    checkcount|= BIT(5);		
	}		
	else
	{
		checkcount&= ~BIT(5);
	 TerminalReply0x110x18.GPS.state3 &= ~BIT(1);
	}
	//sta30 总线故障报警
	if(CANvoidtimes>=720)//12分钟
	{
	 TerminalReply0x110x18.GPS.state3 |=BIT(0);
		numdispflag|=BIT(5);//5 CAN工况异常显示
	}
	 else
	 {
   TerminalReply0x110x18.GPS.state3 &=~BIT(0);
   numdispflag&=~BIT(5);//5 CAN工况异常	不显示	 
   }
  //sta44 振动有效1 0
   if(vibrationtime<60)//有效振动持续1分钟
	 TerminalReply0x110x18.GPS.state4 |=BIT(4);
	 else
   TerminalReply0x110x18.GPS.state4 &=~BIT(4);	 
	//sta42 运行状态 1休眠 0工作
	 if(dormantlevel==0)
	 TerminalReply0x110x18.GPS.state4 &=~BIT(2);
   else
   TerminalReply0x110x18.GPS.state4 |=BIT(2);			 
	//sta40 转场状态 1
   if(ferrytime>=300)//5分钟
	 TerminalReply0x110x18.GPS.state4 |=BIT(0);
	 else if(!ferrytime)
   TerminalReply0x110x18.GPS.state4 &=~BIT(0);	
	 
	 
	 
	 //GPS高电平检测输入异常
	  if(TerminalReply0x110x18.GPS.state1&BIT(5))
			GPShighlevelchecktime=0;
		if(GPShighlevelchecktime>=10)//10s
			numdispflag|=BIT(6);//6GPS高电平检测输入异常  显示
		else
			numdispflag&=~BIT(6);
	 
	  
}

//报警信息内容
void bjxx_content(void)
{
	//终端上传报警信息  每连续1小时未收到服务器指令
	if((!TerminalReply0x17.baojingflag))
	{
		//if(sendbjxx0x17time>3600)
		if((sendbjxx0x17time>(Parameterset.Parametersetitem.AAbjsjset0x1c*60))||(send0xAA))	
		{ send0xAA=0;
			TerminalReply0x17.baojingflag=0xAA;
			TimeArrivedcreg=1;//先查基站	后发送
		  TimeArrived0x17=1;//上传报警信息
			sendbjxx0x17time=0;
			return;
    }	
  //GPS天线故障
   if(TerminalReply0x110x18.GPS.state1&BIT(1))
	{
		if(!(bjflag&BIT(0)))
		{
		 bjflag|=BIT(0);
		 TerminalReply0x17.baojingflag=0x81;//GPS天线故障报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(0))//若报警过则取消报警，没有报警则不取消
		{
		 bjflag&=~BIT(0);
		 TerminalReply0x17.baojingflag=0x01;//GPS天线故障警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	
	//ACC上电报警
    if(TerminalReply0x110x18.GPS.state1&BIT(5))
	{
		if(!(bjflag&BIT(2)))
		{
		 bjflag|=BIT(2);
		 TerminalReply0x17.baojingflag=0x85;//ACC上电报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(2))
		{
		 bjflag&=~BIT(2);
		 TerminalReply0x17.baojingflag=0x05;//ACC上电报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	
  //SIM卡拔卡标志
	if(TerminalReply0x110x18.GPS.state2&BIT(5))
	{
		if(!(bjflag&BIT(8)))
		{
		 bjflag|=BIT(8);
		 TerminalReply0x17.baojingflag=0x8D;//SIM卡拔卡报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(8))
		{
		 bjflag&=~BIT(8);
		 TerminalReply0x17.baojingflag=0x0D;//SSIM卡拔卡报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }		
  //开盖报警
    if(TerminalReply0x110x18.GPS.state2&BIT(6))
	{
		if(!(bjflag&BIT(9)))
		{
			/**********/
			//主电休眠唤醒
			if((dormantlevel==1)&&(dormant_wakeup==0))
			//if(dormantlevel==1)	
		  {
				if((dormant1stat>0)&&(dormant1stat<5)) 
					return;//正在执行休眠程序时 不打断
				if(dormant1stat==5)
				{
		    //wakeuptime2min=120;
		    dormant_wakeup=2;//执行唤醒操做
				}
		  }	
		 bjflag|=BIT(9);
			memparamset.memparam.kaigainum++;//开盖次数
		 TerminalReply0x17.baojingflag=0x8E;//开盖报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			
			return;
		}
  }
	else
	{
		if(bjflag&BIT(9))
		{
		 bjflag&=~BIT(9);
		 TerminalReply0x17.baojingflag=0x0E;//开盖报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	

  //SIM更换报警
	if(TerminalReply0x110x18.GPS.state2&BIT(7))
	{
		if(!(bjflag&BIT(10)))
		{
		 bjflag|=BIT(10);
		 TerminalReply0x17.baojingflag=0x8f;//SIM更换报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(10))
		{
		 bjflag&=~BIT(10);
		 TerminalReply0x17.baojingflag=0x0f;//SIM更换报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }		
	
	
	
	//总线故障报警
   if(TerminalReply0x110x18.GPS.state3&BIT(0))
	{
		if(!(bjflag&BIT(11)))
		{
		 bjflag|=BIT(11);
		 TerminalReply0x17.baojingflag=0x90;//总线故障报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(11))
		{
		 bjflag&=~BIT(11);
		 TerminalReply0x17.baojingflag=0x10;//总线故障报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	

  //主电源断电报警
   if(TerminalReply0x110x18.GPS.state3&BIT(1))
	{
		if(!(bjflag&BIT(12)))
		{
			/**********/
			//主电休眠唤醒
			if((dormantlevel==1)&&(dormant_wakeup==0))
			//if(dormantlevel==1)	
		  {
				if((dormant1stat>0)&&(dormant1stat<5)) 
					return;//正在执行休眠程序时 不打断
				if(dormant1stat==5)
				{
		    //wakeuptime2min=120;
		    dormant_wakeup=2;//执行唤醒操做
				}
		  }	
		 bjflag|=BIT(12);
		 TerminalReply0x17.baojingflag=0x91;//主电源断电报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			
			return;
		}
  }
	else
	{
		if(bjflag&BIT(12))
		{
		 bjflag&=~BIT(12);
		 TerminalReply0x17.baojingflag=0x11;//主电源断电报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	
  //主电源欠压报警
   if(TerminalReply0x110x18.GPS.state3&BIT(2))
	{ 
		/**********/
			//主电休眠唤醒
			if((dormantlevel==1)&&(dormant_wakeup==0))
			//if(dormantlevel==1)	
		  {
				if((dormant1stat>0)&&(dormant1stat<5)) 
					return;//正在执行休眠程序时 不打断
				if(dormant1stat==5)
				{
		    //wakeuptime2min=120;
		    dormant_wakeup=2;//执行唤醒操做
				}
		  }	
		if(!(bjflag&BIT(13)))
		{
		 bjflag|=BIT(13);
		 TerminalReply0x17.baojingflag=0x92;//主电源欠压报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			
			return;
		}
  }
	else
	{
		if(bjflag&BIT(13))
		{
		 bjflag&=~BIT(13);
		 TerminalReply0x17.baojingflag=0x12;//主电源欠压报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	

   //备用电池断电报警
   if(TerminalReply0x110x18.GPS.state3&BIT(3))
	{
		if(!(bjflag&BIT(14)))
		{
		 bjflag|=BIT(14);
		 TerminalReply0x17.baojingflag=0x93;//备用电池断电报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(14))
		{
		 bjflag&=~BIT(14);
		 TerminalReply0x17.baojingflag=0x13;//备用电池断电报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	

   //备用电池欠压报警
   if(TerminalReply0x110x18.GPS.state3&BIT(4))
	{
		if(!(bjflag&BIT(15)))
		{
		 bjflag|=BIT(15);
		 TerminalReply0x17.baojingflag=0x94;//备用电池欠压报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(15))
		{
		 bjflag&=~BIT(15);
		 TerminalReply0x17.baojingflag=0x14;//备用电池欠压报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	
  //CAN波特率变化报警  只报警不消除
	if(canBaudRatechange)
	{
		 canBaudRatechange=0;
		 TerminalReply0x17.baojingflag=0x97;//备用电池欠压报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
  }
	
	//休眠  只报警不消除
	 if(TerminalReply0x110x18.GPS.state4&BIT(2))
	{
		if(!(bjflag&BIT(19)))
		{
		 bjflag|=BIT(19);
		 TerminalReply0x17.baojingflag=0x98;//休眠报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(19))
		{
		 bjflag&=~BIT(19);
		 TerminalReply0x17.baojingflag=0x18;//休眠报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	
	
	
	//sta40 转场状态 
	 if(TerminalReply0x110x18.GPS.state4&BIT(0))
	{
		if(!(bjflag&BIT(20)))
		{
		 bjflag|=BIT(20);
		 TerminalReply0x17.baojingflag=0xA0;//转场报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(20))
		{
		 bjflag&=~BIT(20);
		 TerminalReply0x17.baojingflag=0x20;//转场报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }	
	
  //sta44 振动有效1 0
	if(TerminalReply0x110x18.GPS.state4&BIT(4))
	{
		if(!(bjflag&BIT(21)))
		{
		 bjflag|=BIT(21);
		 TerminalReply0x17.baojingflag=0xA1;//振动报警
		//TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
			return;
		}
  }
	else
	{
		if(bjflag&BIT(21))
		{
		 bjflag&=~BIT(21);
		 TerminalReply0x17.baojingflag=0x21;//振动报警消除
		 //TimeArrivedcreg=1;//先查基站	后发送
		 TimeArrived0x17=1;//上传报警信息
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
	//发现有漏洞情况：有报警内容，但无上传报警信息标志的情况，导致休眠不能正常进入，而且未找到原因
	if((sendbjxx0x17count==0)&&(TimeArrived0x17==0))
		TimeArrived0x17=1;
}
}

//处理报警信息未应答
void bjxx_noreply(void)
{
		//终端上传报警信息 等待回复 报警内容不同 回复不同
		if(sendbjxx0x17count)//记录发送次数
		{
     if(!sendbjxx0x17repovertime)//应答15s	
		 {
			 if(sendbjxx0x17count>=3)//重启连接
			 {
				 
				 
				 if(state==0)
				 {
					 sendbjxx0x17count=0;  //漏洞在此处
					 TimeArrived0x17=1;//上传报警信息
					if(dormantlevel!=0)//在休眠状态下，发送3次无回应认为连接失败，准备进入休眠
					{
						fwq_connectfail=1;//服务器连接失败
						return;
					} 
				 //sendbjxx0x17count=0;
				 ZDParameter.LoginReply=0;//发送失败后重新登录
				 ZDParameter.LoginTimer =270;//马上重新登录
				 GprsConnectTcpState=0;
				 at_command_order = 10;	 
				// TimeArrived0x17=1;//上传报警信息
					ZDParameter.zdliushuinum=0;//流水重置
				 }
       }
        else
        {
					//TimeArrivedcreg=1;//先查基站	后发送
		      TimeArrived0x17=1;//上传报警信息
        }					
     } 
		}
}

 

void  initParameter_setitem(void)
{ 
	uint8_t i;
	SYS_UnlockReg();
	DF_ReadParameter_setitem();//读取参数
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
	//升级服务器IP
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
	Parameterset.Parametersetitem.ycsj0x15=0;//关升级
  //0x16
	if(Parameterset.Parametersetitem.sdms0x16==0xff)
	Parameterset.Parametersetitem.sdms0x16=1;//开启省电模式
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
	Parameterset.Parametersetitem.PLCsrk0x20=0;//高电平检测有效
	//0x41 位置汇报策略
	if(Parameterset.Parametersetitem.wzhbcl0x41==0xff)
	Parameterset.Parametersetitem.wzhbcl0x41=2;
	//0x42 定时上报时间
	if(Parameterset.Parametersetitem.dssbsj0x42==0xff)
	Parameterset.Parametersetitem.dssbsj0x42=30;//30s
	//0x43 定距上报距离
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
	//0x17 设备编号
	if(Parameterset.Parametersetitem.sbbh0x17[49]==0xff)//第50个为0xff则未初始过
	{
		for(i=0;i<50;i++)
	 Parameterset.Parametersetitem.sbbh0x17[i]=0;
  }
	//0x18 SIM卡号 
	if(Parameterset.Parametersetitem.SIMkh0x18[19]==0xff)//第20个为0xff则未初始过
	{
		for(i=0;i<20;i++)
	 Parameterset.Parametersetitem.SIMkh0x18[i]=0;
		
  }
	//0x19 呼叫中心号码 
	if(Parameterset.Parametersetitem.hjzxhm0x19[19]==0xff)//第20个为0xff则未初始过
	{
		for(i=0;i<20;i++)
	 Parameterset.Parametersetitem.hjzxhm0x19[i]=0;
		
  }
	//0x22 设备类型
	if(Parameterset.Parametersetitem.sblx0x22[19]==0xff)//第20个为0xff则未初始过
	{
		for(i=0;i<20;i++)
	 Parameterset.Parametersetitem.sblx0x22[i]=0;
		
  }
	
	//0x09 短信报警号码
if(Parameterset.Parametersetitem.dxbjhm0x09[28]==0xff)//第29个为0xff则未初始过
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
	
		
	
	moubilenumswitch();//交换号码位置
	autorep_loctime=(Parameterset.Parametersetitem.hbsc0x03[0]<<8)+Parameterset.Parametersetitem.hbsc0x03[1];
}

void  initmem_param(void)
{ 
	uint8_t i;
	 SYS_UnlockReg();
	 DF_Readmem_param();//读取参数
	 SYS_LockReg();
	for(i=0;i<24;i++)
	{
		memparamset.mem_paramdata[i]=mem_paramdata[i];
  }
	if(memparamset.memparam.GPSvoidtimem==0xffffffff)//GPS故障分钟 
		memparamset.memparam.GPSvoidtimem=0;
	 GPSvoidtimem=memparamset.memparam.GPSvoidtimem;
	//GPS 总里程数 
	if((memparamset.memparam.GPS_sum[0]==0xff)&&(memparamset.memparam.GPS_sum[1]==0xff)&&(memparamset.memparam.GPS_sum[2]==0xff)&&(memparamset.memparam.GPS_sum[3]==0xff))//GPS 总里程数 
		for(i=0;i<4;i++)
	{
		memparamset.memparam.GPS_sum[i]=0;
  }
	gps_tot_lc=(memparamset.memparam.GPS_sum[0]<<24)+(memparamset.memparam.GPS_sum[1]<<16)+(memparamset.memparam.GPS_sum[2]<<8)+memparamset.memparam.GPS_sum[3];
	//ACC ON 总累计时间
	if((memparamset.memparam.ACC_ON_sum[0]==0xff)&&(memparamset.memparam.ACC_ON_sum[1]==0xff)&&(memparamset.memparam.ACC_ON_sum[2]==0xff)&&(memparamset.memparam.ACC_ON_sum[3]==0xff))//GPS 总里程数 
		for(i=0;i<4;i++)
	{
		memparamset.memparam.ACC_ON_sum[i]=0;
  }
	ACCon_tot_time=(memparamset.memparam.ACC_ON_sum[0]<<24)+(memparamset.memparam.ACC_ON_sum[1]<<16)+(memparamset.memparam.ACC_ON_sum[2]<<8)+memparamset.memparam.ACC_ON_sum[3];
	
	//GPS总通电时间
	if((memparamset.memparam.GPSztdsj[0]==0xff)&&(memparamset.memparam.GPSztdsj[1]==0xff)&&(memparamset.memparam.GPSztdsj[2]==0xff)&&(memparamset.memparam.GPSztdsj[3]==0xff))//GPS 总里程数 
		for(i=0;i<4;i++)
	{
		memparamset.memparam.GPSztdsj[i]=0;
  }
	gps_tot_time=(memparamset.memparam.GPSztdsj[0]<<24)+(memparamset.memparam.GPSztdsj[1]<<16)+(memparamset.memparam.GPSztdsj[2]<<8)+memparamset.memparam.GPSztdsj[3];
	//开盖次数
	if(memparamset.memparam.kaigainum==0xff)
	   memparamset.memparam.kaigainum=0;
	// 拔GPS天线次数
	if(memparamset.memparam.baGPSantnum==0xff)
	   memparamset.memparam.baGPSantnum=0;
	// 拔SIM卡次数
	if(memparamset.memparam.baSIMnum==0xff)
	   memparamset.memparam.baSIMnum=0;
	//SIM卡更换次数
	if(memparamset.memparam.simkaghcs==0xff)
	   memparamset.memparam.simkaghcs=0;
	if(memparamset.memparam.flagchange==0xffffffff)
	   memparamset.memparam.flagchange=0;
}
void mem_Paramete(void)
{//重启 ACCOFF
	uint8_t i;
	if(memchange)//0x11参数项有改动
	{
		memchange=0;
		for(i=0;i<192;i++)
	 {
		Parameter_setitemdata[i]=Parameterset.Parameter_setitemdata[i];
   }
	 SYS_UnlockReg();
	  DF_WriteParameter_setitem();//写参数
	 SYS_LockReg();
  }
	
	
	//未同步的值
	//gps总里程
	 memparamset.memparam.GPS_sum[0]=gps_tot_lc>>24;
	 memparamset.memparam.GPS_sum[1]=gps_tot_lc>>16;
	 memparamset.memparam.GPS_sum[2]=gps_tot_lc>>8;
	 memparamset.memparam.GPS_sum[3]=gps_tot_lc;
	//ACCon累计时间
	 memparamset.memparam.ACC_ON_sum[0]=ACCon_tot_time>>24;
	 memparamset.memparam.ACC_ON_sum[1]=ACCon_tot_time>>16;
	 memparamset.memparam.ACC_ON_sum[2]=ACCon_tot_time>>8;
	 memparamset.memparam.ACC_ON_sum[3]=ACCon_tot_time;
	//GPS总通电时间
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
	{	//数据有变动执行存储
	  for(i=0;i<24;i++)
	 {
		mem_paramdata[i]=memparamset.mem_paramdata[i];
   }
	 SYS_UnlockReg();
	  DF_Writemem_param();//写参数
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
//数码管 LED灯自检
void numdisp_test(void)
{ 
	uint32_t temp;
	if(!numdisp_testok)
 {	
	if((numdisp_testtime%10)>4)	//亮500ms
	{
	 temp=GPIO_GET_IN_DATA(PC);
	 temp&=0x7F;
	 temp|=(num_dis[8]<<7);
	 GPIO_SET_OUT_DATA(PC, temp);
	 PC6=1;PA6=1;PB15=1;PB9=1;
	}
	else//灭500ms
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
//充电灯
//  void battakedown_check(void)
// 	{
// 					
// 			if(PB2)//充电功能打开
// 			{
// 				if(PF4)//高电平没有充电 
// 				{ PB2=0;
// 					//Delay_Us(500);
// 					Delay_Ms(15);
// 					PB2=1;
// 					//Delay_Us(500);
// 					Delay_Ms(15);
// 					if(PF4)//没有充电 
// 						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//备用电池断电
//           else
// 					 TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
//         }
// 				else//充电功能打开 正充电
// 					TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//备用电池正常
//       }
// 			else//充电功能关闭
// 			{
// 				if(volt_bat>39)
// 				TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//备用电池正常
// 				else
// 				{
// 					PB2=1;//充电功能打开
// 					//Delay_Us(1000);
// 					 Delay_Ms(15);
// 					if(PF4)//没有充电 
// 						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//备用电池断电
//           else
// 					 TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
// 					 PB2=0;
//         }
//       }	
// 		}
 
//充电完成灯
 void battakedown_check(void)
	{
			// if(checkcount&BIT(6))
		if(voltage>Parameterset.Parametersetitem.zdqyz0x1D)//小于22.5v时没有外部电压 则不再坚持备电断电
	  {				 
			if(PB2)//充电功能打开
			{
				//if(!PF4)//充电已完成
				if(!PA14)//充电已完成
				{ PB2=0;
					//Delay_Us(500);
					Delay_Ms(10);
					PB2=1;
					//Delay_Us(500);
					Delay_Ms(11);
// 					if(!PF4)//充电已完成
						if(!PA14)//充电已完成
					{	
						if(checkcount&BIT(3))//检测2次确认
						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//备用电池断电
						checkcount|= BIT(3);
					}
          else
					{
					 checkcount&= ~BIT(3);
					 TerminalReply0x110x18.GPS.state3 &= ~BIT(3);
					}
        }
				else//充电功能打开 正充电
					TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//备用电池正常
      }
			else//充电功能关闭
			{
				if(volt_bat>39)
				TerminalReply0x110x18.GPS.state3 &= ~BIT(3);//备用电池正常
				else
				{
					PB2=1;//充电功能打开
					//Delay_Us(1000);
					 Delay_Ms(11);
					//if(!PF4)//充电已完成
					if(!PA14)//充电已完成
					{
						if(checkcount&BIT(3))//检测2次确认
						TerminalReply0x110x18.GPS.state3 |= BIT(3);	//备用电池断电
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



  //充电管理
  void chargermanager(void)//1s
  {   	

     //sta33 备用电池断电报警  
		if(TerminalReply0x110x18.GPS.state2& BIT(6))//开盖状态下 1s检测
		{
			battakedowncheck_count=0;
		  battakedown_check();
		}
		else if(battakedowncheck_count<3)//关盖状态下 检测2次
		{
			battakedown_check();
			battakedowncheck_count++;
			if(TerminalReply0x110x18.GPS.state3&BIT(3))//电池断电保持监测
				battakedowncheck_count=0;
			  chargestoptime=0; 
    }

// 		
// 		 if((voltage>20)&&(!PA15))//外部供电大于2v都执行断电程序 
// 			chargestoptime=0; 
		 //if((TerminalReply0x110x18.GPS.state3&BIT(2))&&(volt_bat>37))//主电源欠压22.5v 9v 并且备电电压高于3.7v时可准备欠压保护，否者外部供电控制
		//if((TerminalReply0x110x18.GPS.state3&BIT(2))&&(volt_bat>37)&&(!(TerminalReply0x110x18.GPS.state3&BIT(3))))//主电源欠压22.5v 9v 并且备电存在且电压高于3.7v时可准备欠压保护，否者外部供电控制		
		if((TerminalReply0x110x18.GPS.state3&BIT(2))&&(!(TerminalReply0x110x18.GPS.state3&BIT(3))))//主电源欠压22.5v 9v 并且备电存在可准备欠压保护，否者外部供电控制		
		{
			 if(checkcount&BIT(6))//检测2次确认
			 {	 
			 PE5 = 0;//电池强制供电 
			 PA15 = 1;//欠压保护启动
			 }
			 checkcount|= BIT(6);
     }
	   else
		 {
		  checkcount&= ~BIT(6);
			PA15 = 0; //欠压保护关闭
			PE5 = 1;//外部控制断电
		 }
		 
			//if((voltage>(Parameterset.Parametersetitem.zdqyz0x1D+20))&&(Temperature<120))//22.5+2.0=24.5    120-60=60度
			if((voltage>245)&&(Temperature<120))//22.5+2.0=24.5    120-60=60度	
			{	
		   PB2 = 1;//充电
			}
		  else
      {
// 			 if(((voltage<=50)||(PA15))&&(chargestoptime<3)&&(battakedowncheck_count>=3))//控制回电
// 			 {	
//         chargestoptime++;	
//         if(chargestoptime>3)
//         chargestoptime=3;				 
// 		    PB2=1;
// 			 }
// 			 else
			 PB2 = 0;//停充	
       
		  }
				
  }
 
	void mem_exe(void)
	{//(拆盖  重启  休眠 )有数据变动
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
// 			ZDParameter.LoginReply=0;//发送失败后重新登录
// 			ZDParameter.LoginTimer =270;//马上重新登录
// 		  ZDParameter.zdliushuinum=0;//流水重置
// 			
// 		 GPIO_SET_OUT_DATA(PC, 0X0F);
// 		 PA6=0;
// 		 PB15=0;
// 		 PB9=0;
// 		 PA10 = 1;//PW_GPS
// 		 PB3 = 0;//PW_GPRS 可接收短信
// 		 PD14 = 1;//0 工作，1休眠 can通信
			mem_exe();//判断存储		
		 dormant1stat=1;
		}
		else if(dormant1stat==1)
		{
			if(at_command_order>=10)
			{			
		   closegprsnet=1;//关闭GPRS连接
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
// 			 dormantgprs=1;//gprs休眠
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
// 			TIMER2->TCMPR =720000;//2小时
// 			TIMER_Start(TIMER2);
			if(at_command_order>=10)
			 at_command_order = 10;
			else
			{
				at_command_order = 0;
				delay_ns = 50;
				PB3 = 1;//PW_GPRS  无卡状态
			}
			 
			AtCommSendingState = COMMSENDED;
			DataSended = 0;
			CommSended = 0; 
			GprsConnectTcpState=0;
			//state=0;
			dormanttime=0;	
			ZDParameter.LoginReply=0;//发送失败后重新登录
			ZDParameter.LoginTimer =270;//马上重新登录
		  ZDParameter.zdliushuinum=0;//流水重置
			//fwq_connectfail=0;
		 GPIO_SET_OUT_DATA(PC, 0X0F);
		 PA6=0;
		 PB15=0;
		 PB9=0;
		 PA10 = 1;//PW_GPS
		 //PB3 = 0;//PW_GPRS 可接收短信
		 PD14 = 1;//0 工作，1休眠 can通信
		 PA3=0; //登录指示灯
		 PA4=0; //CAN通信指示灯
		 dormant1stat=5;
    }
		else if(dormant1stat==5)
		{ 
// 			Delay_Ms(2000);//主电休眠小数点闪烁
// 			PB9=~PB9;
		}
  }	
	void dormant2(void)
	{ 
// 		if(dormant2stat==0)
// 		{
// 			ZDParameter.LoginReply=0;//发送失败后重新登录
// 			ZDParameter.LoginTimer =270;//马上重新登录
// 		  ZDParameter.zdliushuinum=0;//流水重置
// 		 GPIO_SET_OUT_DATA(PC, 0X0F);
// 		 PA6=0;
// 		 PB15=0;
// 		 PB9=0;
// 		 PA10 = 1;//PW_GPS
// 		 PB3 = 1;//PW_GPRS
// 		 PD14 = 1;//0 工作，1休眠
// 		 // dormant2stat=1;
// 		 SYS_UnlockReg();
// 		 while((UART0->IER)&UART_IER_THRE_IEN_Msk);//等待关闭发送中断
// 		 UART_WAIT_TX_EMPTY(UART0);
// 		 CLK_PowerDown();//进入深度休眠
// 		 SYS_LockReg();
// 		
// 		}
		if(dormant2stat==0)
		{ 
		 dormant2stat=1;
			 mem_exe();//判断存储		
		}
		else if(dormant2stat==1)
		{
		 closegprsnet=1;//关闭GPRS连接
			// dormant2stat=2;
		 dormant2stat=3;
		}
		else if(dormant2stat==2)
		{
			if(closegprsnet==0)
		  {
		   dormant2stat=3;
			 dormantgprs=1;//gprs休眠
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
			ZDParameter.LoginReply=0;//发送失败后重新登录
			ZDParameter.LoginTimer =270;//马上重新登录
		  ZDParameter.zdliushuinum=0;//流水重置
			//fwq_connectfail=0;
		 GPIO_SET_OUT_DATA(PC, 0X0F);
		 PA6=0;
		 PB15=0;
		 PB9=0;
		 PA10 = 1;//PW_GPS
		 PB3 = 0;//PW_GPRS 可接收短信
		 PD14 = 1;//0 工作，1休眠 can通信
		 dormant2stat=5;
    }
		else if(dormant2stat==5)
		{ 
			//需做电平唤醒由深度休眠进入主电休眠  休眠前开中断
			if((PF8)&&(dormantlevellow))
		 {
			 //GPS终端总通电时间  不休眠+主电休眠
		  gps_tem_time=0;
			 
		  TIMER2->TCMPR = 8640000;//24小时
			TIMER_Start(TIMER2);
			GPIO_EnableInt(PF, 8,  GPIO_INT_LOW);//IN23.75V 低电平中断 可由备电近主电
			GPIO_EnableInt(PB, 14, GPIO_INT_BOTH_EDGE);//开盖关盖开中断
			GPIO_EnableInt(PD, 15,  GPIO_INT_BOTH_EDGE);
			SYS_UnlockReg();
		  while((UART0->IER)&UART_IER_THRE_IEN_Msk);//等待关闭发送中断
		  UART_WAIT_TX_EMPTY(UART0);
		  CLK_PowerDown();//进入深度休眠
		  SYS_LockReg();
		 }
		  dormant2stat=0;
		  wakeuptime2min=120;
		  dormant_wakeup=1;	//唤醒
		 
		   PA10 = 0;//PW_GPS
		   PB3 = 0;//PW_GPRS 可接收短信
		   PD14 = 1;//0 工作，1休眠 can通信
		}
		
  }	
	
	void dormant3(void)
	{  uint32_t reg_time;
		
      dormant3stat=1;//限制time2中断执行
			mem_exe();//判断存储		
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
			ZDParameter.LoginReply=0;//发送失败后重新登录
			ZDParameter.LoginTimer =270;//马上重新登录
		  ZDParameter.zdliushuinum=0;//流水重置
			//fwq_connectfail=0;
		
// 		   NVIC_SetPendingIRQ(TMR0_IRQn);
// 		   NVIC_SetPendingIRQ(TMR1_IRQn);
		  GPIO_SET_OUT_DATA(PC, 0X0F);
		  PA6=0;
		  PB15=0;
		  PB9=0;
		  PA10 = 1;//PW_GPS
		  PB3 = 1;//PW_GPRS 可接收短信
		  PD14 = 1;//0 工作，1休眠 can通信
			PA3=0; //登录指示灯
		  PA4=0; //CAN通信指示灯
			
			PA12=0; //ct_33v
			PA13=0; //ct_18v
			//需做电平唤醒由深度休眠进入主电休眠  休眠前开中断
			if((PF8)&&(!dormantlevellow))
		 {
			  ADC_POWER_DOWN(ADC);
			 reg_time=TIMER2->TCSR ;
			 if(reg_time&TIMER_TCSR_CEN_Msk)//若定时器已打开则不再重新赋值
			 {
       }
			 else
			 { 
		    TIMER2->TCMPR = 8640000;//24小时
				//TIMER2->TCMPR = 30000;//5分钟
			  TIMER_Start(TIMER2);
			 }
			 	 
			 
			GPIO_EnableInt(PF, 8,  GPIO_INT_LOW);//IN23.75V 低电平中断 可由备电近主电
			GPIO_EnableInt(PB, 14, GPIO_INT_BOTH_EDGE);//开盖关盖开中断
			GPIO_EnableInt(PD, 15,  GPIO_INT_BOTH_EDGE);
			 //开ACC检测
			 GPIO_EnableInt(PA, 8,  GPIO_INT_BOTH_EDGE);//检测低
			 GPIO_EnableInt(PB, 7,  GPIO_INT_BOTH_EDGE);//检测高
			 //备电保护判断，只留ACC中断检测
// 			if(volt_bat<37) 
// 				volt_bat37low++;
// 			 else
// 				volt_bat37low=0;
// 			 if(volt_bat37low>=2) 
			 if(TerminalReply0x110x18.GPS.state3&BIT(4))//备用电池欠压 启动备电保护 只留ACC中断检测
			 {
				  TIMER_Stop(TIMER2);//关时间唤醒
				  GPIO_DisableInt(PB, 14);//关开盖关盖开中断			 
		    	GPIO_DisableInt(PD, 15);
          GPIO_DisableInt(PB, 12);//关振动中断 
		    	GPIO_DisableInt(PF, 5);	
 //         GPIO_DisableInt(PF, 8);//关IN23.75V 低电平中断 可由备电近主电					 
				 
//         //关ACC检测
//       GPIO_DisableInt(PA, 8);			 
// 			GPIO_DisableInt(PB, 7);  
			 
       }
			 
			 
			SYS_UnlockReg();
		  while((UART0->IER)&UART_IER_THRE_IEN_Msk);//等待关闭发送中断
		  UART_WAIT_TX_EMPTY(UART0);
			if(send0xAA==0)//无唤醒报警
		  CLK_PowerDown();//进入深度休眠
		  SYS_LockReg();
			
      GPIO_DisableInt(PB, 14);			 
			GPIO_DisableInt(PD, 15);	
      //关ACC检测
      GPIO_DisableInt(PA, 8);			 
			GPIO_DisableInt(PB, 7);  
			 
			dormant3stat=0;//限制time2中断执行  解除
			 if(PF8)//仍然为深度休眠
			 {	 
        wakeuptime2min=120;
		    dormant_wakeup=1;	//唤醒
			 }	
       else
       dormantlevel=1;//进入主电休眠				 
			 gprslinkfailtime=0;//重新记录连接失败时间
			 ADC_POWER_ON(ADC);
		 }
		 
		
// 		   NVIC_ClearPendingIRQ(TMR0_IRQn);
// 		   NVIC_ClearPendingIRQ(TMR1_IRQn);
		   PA12=1; //ct_33v
			 PA13=1; //ct_18v
		   PA10 = 0;//PW_GPS
		   PB3 = 0;//PW_GPRS 可接收短信
		   PD14 = 1;//0 工作，1休眠 can通信
		
		
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
		 PB3 = 0;//PW_GPRS 可接收短信
		 PD14 = 1;//0 工作，1休眠 can通信
		 gprslinkfailtime=0;//重新记录连接失败时间
		 wakeup1stat=1;
		}
		
  }
 void	dormant_wakeup2(void)
	{
		if(wakeup2stat==0)
		{
// 			NVIC_EnableIRQ(TMR0_IRQn);//主电休眠——备电休眠——备电唤醒
// 		  NVIC_EnableIRQ(TMR1_IRQn);
// 			TIMER_Start(TIMER0);
// 			TIMER_Start(TIMER1);
// 		 GPIO_SET_OUT_DATA(PC, 0X0F);
// 		 PA6=0;
// 		 PB15=0;
// 		 PB9=0;
		 PA10 = 0;//PW_GPS
		 PB3 = 0;//PW_GPRS
		 PD14 = 0;//0 工作，1休眠
		 wakeup2stat=1;
		}
  }	
	
	
	
	
// dormantlevel=0;//休眠级别 0主电工作 1主电休眠 2备电工作 3备电休眠	
// dormantlevel=0;//休眠级别 0主备电工作 1主电休眠 2备电休眠	
	
void dormant_judge(void)//1s
{
	if((dormantlevel!=dormantlevelstat)||(dormantlevel==0))//进入休眠后重新执行无报警判断
	{
	  check_baojingflag=0;
		dormantlevelstat=dormantlevel;
	}
	
  if(Parameterset.Parametersetitem.sdms0x16)// 省电模式 1开启 0关
	{ 
		//休眠：ACC OFF之后10分钟同时电压低于24.1v 无短信  无振动
		//if((dormant_ACCofftime>=600)||(voltage<Parameterset.Parametersetitem.zdqyz0x1D))//对无主电的情况下有PLC信号，直接10分钟	
    if(dormant_ACCofftime>=600)		
    { 
			 if((TerminalReply0x110x18.GPS.state3&BIT(1))||(TerminalReply0x110x18.GPS.state3&BIT(2)))//断电或欠压报警
			 { 
				 if((dormant1stat==5)||(dormant1stat==0)) 
			   dormantlevel=2;//备电休眠
			 }
			 else
			 {
				 if(dormant2stat==0)//需做电平唤醒由深度休眠进入主电休眠  休眠前开中断
			    dormantlevel=1;//主电休眠	
			 }
      wakeup2stat=0;//可执行唤醒
    }

		//退出休眠进入正常工作模式，若是有短信，ACCON 主电24.5v以上10分钟，振动唤醒 若大于2km/h 5分钟正常工作
	if((smsnumber>3)||(readsmsnumok)||(dxbjhmflag)||(TerminalReply0x110x18.GPS.state1&BIT(5))||(time_245v>=600)||(TerminalReply0x110x18.GPS.state4&BIT(0)))
	{
		dormantlevellow=1;//深度休眠前可用，防止进入深度休眠程序中需退出
		if(((dormant1stat==5)||(dormant1stat==0))&&(dormant2stat==0))//
		{	
		  
		 dormantlevel=0;//主电工作
		 dormant_wakeup=0;//休眠唤醒标志
		 dormant1stat=0;
		}
  }
	else
	dormantlevellow=0;	
	
    if(dormantlevel!=0)	//进入休眠后取消自动发数据
	   Timer1Count=0;				
// 	if((dormantlevel)&&(!smsnumber)&&(!readsmsnumok)&&(!dxbjhmflag)&&(!state)&&(!TimeArrivedlogin)&&(!TimeArrivedcsq)&&(!TimeArrivedcreg)&&(!TimeArrived2B)&&(!TimeArrived0x13)
// 	&&(!TimeArrived0x18)&&(!TimeArrived0x11)&&(!TimeArrived0x17)&&(!TimeArrived12)&&(!TimeArrived0x28))
	
// 	  //执行休眠
// 		if((dormant_wakeup==1)&&((dormant1stat==5)||(dormant1stat==0))&&(dormant2stat==0))//执行唤醒操作
// 		{			
// 		  if(dormantlevel==1)
// 			{ 
// 				dormant1stat=0;//可执行休眠
// 			  dormant_wakeup1();
// 			}
//       if(dormantlevel==2)
// 			{
// 			 dormant2stat=0;//可执行休眠
// 			 dormant_wakeup1();
// 			}				
// 	  }	
//     else
// 		{			
// 		 if((dormantlevel==1)||((dormant1stat>0)&&(dormant1stat<5)))
// 		 {
// 			 wakeup1stat=0;//可执行唤醒
// 			dormant1();//执行主电休眠 	 
// 		 }
//      if(dormantlevel==2)
// 		 { dormant1stat=0;
// 			 wakeup1stat=0;//可执行唤醒
// 			dormant2();//执行备电休眠
// 		 }			 
// 		 if(dormantlevel==0)//取消休眠执行
// 		 {
// 			 dormant_wakeup2();
//      }	
// 	  }
   }
		
	
}	

 void dormant_exe(void)
 {
	   //执行休眠
		//if((dormant_wakeup==1)&&((dormant1stat==5)||(dormant1stat==0))&&(dormant2stat==0))//执行唤醒操作
		//if((dormant_wakeup==1)&&(dormant1stat==5)&&(dormant2stat==0))//执行唤醒操作	
		//if((dormant_wakeup==1)&&(dormant1stat==5))//执行唤醒操作
	 
	 
	 if(dormant_wakeup==2)	
   {
		 if((dormant1stat==5)||(dormant1stat==0))//未执行主电休眠， 或执行完主电休眠才可执行主电唤醒工作否则等待
		 {
			 dormant_wakeup=1;
			 wakeuptime2min=120;
			 
		 }
   }		 
	 
    if(dormant_wakeup==1)			
		{			
		  if(dormantlevel==1)
			{ 
				dormant1stat=0;//可执行休眠
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
			 wakeup1stat=0;//可执行唤醒
			 dormant1();//执行主电休眠 	 
		  }
      if(dormantlevel==2)
		  {
				//清主电休眠
				dormant1stat=0;
			  wakeup1stat=0;//可执行唤醒
			  dormant3();//执行备电休眠
		  }			 
		  if(dormantlevel==0)//取消休眠执行
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







