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

#define PLL_CLOCK   48000000


 
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
		GPIO_SetMode(PA, BIT8, GPIO_PMD_QUASI);PA8=0;//IN33  检测低电平
		//	GPIO_SetMode(PA, BIT8, GPIO_PMD_OUTPUT);PA8=0;//IN33  检测低电平

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
  
	   /*
	  GPIO_SetMode(PD, BIT14, GPIO_PMD_OUTPUT);//TCAN1042 STB
		PD14 = 0;//0 工作，1休眠
		
		GPIO_SetMode(PD, BIT15, GPIO_PMD_INPUT);//sunjc1
		*/
		GPIO_SetMode(PE, BIT5, GPIO_PMD_OUTPUT);//CT_BATT   GPIO_PMD_OUTPUT
        //GPIO_SetMode(PE, BIT5, GPIO_PMD_OPEN_DRAIN ); //CT_BATT
		//PE5 = 1;//外部供电控制
		PE5 = 0;	

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

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{
    int32_t i32Err;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

	GPIO_INIT();

    /* Lock protected registers */
    SYS_LockReg();

    
    while(1)
	{	
		PB3 = 0;
		PA10 = 1;
		
	}
}







