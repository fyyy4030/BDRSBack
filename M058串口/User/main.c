
/****************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 7 $
 * $Date: 15/05/22 3:53p $
 * @brief    Transmit and receive data from PC terminal through RS232 interface.
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "M051Series.h"


#define PLL_CLOCK           50000000
#define RXBUFSIZE 1024


#define sysTxBufReadNextOne()	(((_sys_uUartTxHead+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxHead+1)
#define sysTxBufWriteNextOne()	(((_sys_uUartTxTail+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxTail+1)
#define UART_BUFFSIZE	256
static uint8_t _sys_ucUartTxBuf[UART_BUFFSIZE];
static uint16_t volatile _sys_uUartTxHead, _sys_uUartTxTail;

unsigned char JiaMiCoordinate[5];
unsigned short 	int MiYaoZuoBiaoX;
unsigned short  int MiYaoZuoBiaoY;


unsigned char youliang;
unsigned int TempSendOil = 0;

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t g_u8RecData[RXBUFSIZE]  = {0};
//uint8_t tempReceiveData[RXBUFSIZE] = {0};

volatile uint32_t g_u32comRbytes = 0;
volatile uint32_t g_u32comRhead  = 0;
volatile uint32_t g_u32comRtail  = 0;
volatile int32_t g_bWait         = TRUE;

volatile uint8_t g_u8IsWDTTimeoutINT = 0;


//wsj add 2017.8.24
int CounterLed = 0;
int CounterDelay = 0;
uint32_t counter = 0;
uint8_t TestBuffer[1024] ={0x32,0x33,0x34,0x35};
uint8_t i;
uint8_t k=0;
unsigned char SendBuffer[256] ={0};
 
volatile uint32_t g_u32AdcIntFlag;


/**********************************************************************************************************/
//receive data
//

#define RX_ARRAY_NUM 100
 uint8_t uart_rx[RX_ARRAY_NUM] = {0};
 uint8_t uart_rx2[RX_ARRAY_NUM] = {0};
 uint32_t volatile rx_cnt = 0;
 uint32_t volatile rx_cnt2 = 0;

 uint32_t  u32Count;
 volatile uint16_t bufloc = 0;
  volatile char strbuf[300];
/**********************************************************************************************************/


/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void);
void UART_TEST_HANDLE(void);
void UART_FunctionTest(void);

void UART_SendMiYaoData();
void UART_SendData();
void SendData(uint8_t *BufferSend,uint8_t len);
void  SendToComm(char *comm);
static void _PutChar_f(uint8_t ucCh);
static void sysPrintf(char *pcStr);
void ReceiveData (void);
void uartpfnCallback(uint8_t* u8Buf,uint32_t u32Len);

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

	/*******************************************************************************************/
	/* Enable peripheral clock */
    CLK->APBCLK = CLK_APBCLK_UART0_EN_Msk | CLK_APBCLK_WDT_EN_Msk;

    /* Peripheral clock source */
    CLK->CLKSEL1 = CLK_CLKSEL1_UART_S_PLL | CLK_CLKSEL1_WDT_S_LIRC;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    SystemCoreClockUpdate();
	
	/*******************************************************************************************/



	/* Enable ADC module clock */
    CLK_EnableModuleClock(ADC_MODULE);
    /* ADC clock source is 22.1184MHz, set divider to 7, ADC clock is 22.1184/7 MHz */
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_CLKDIV_ADC(7));




    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set P3 multi-function pins for UART0 RXD and TXD */
    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);

	//Led init
	GPIO_SetMode(P4, BIT2, GPIO_PMD_OUTPUT);//Led init

	//ADC Init
	/* Disable the P1.0 - P1.3 digital input path to avoid the leakage current */
    GPIO_DISABLE_DIGITAL_PATH(P1, 0xF);
	GPIO_DISABLE_DIGITAL_PATH(P1, BIT4);

    /* Configure the P1.0 - P1.3 ADC analog input pins */
    //SYS->P1_MFP &= ~(SYS_MFP_P10_Msk | SYS_MFP_P11_Msk | SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
    //SYS->P1_MFP |= SYS_MFP_P10_AIN0 | SYS_MFP_P11_AIN1 | SYS_MFP_P12_AIN2 | SYS_MFP_P13_AIN3 ;

	SYS->P1_MFP &= ~(SYS_MFP_P10_Msk | SYS_MFP_P11_Msk | SYS_MFP_P12_Msk | SYS_MFP_P13_Msk|SYS_MFP_P14_Msk);
    SYS->P1_MFP |= SYS_MFP_P10_AIN0 | SYS_MFP_P11_AIN1 | SYS_MFP_P12_AIN2 | SYS_MFP_P13_AIN3| SYS_MFP_P14_AIN4 ;

}








//wushengjun add 2017.8.22

/**
 * @brief       IRQ Handler for WDT and WWDT Interrupt
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The WDT_IRQHandler is default IRQ of WDT and WWDT, declared in startup_M051Series.s.
 */
void WDT_IRQHandler(void)
{
    if(WDT_GET_TIMEOUT_INT_FLAG() == 1)
    {
        /* Clear WDT time-out interrupt flag */
        WDT_CLEAR_TIMEOUT_INT_FLAG();

        g_u8IsWDTTimeoutINT = 1;

        //printf("WDT time-out interrupt occurred.\n");
    }
}






void TIMER1_Init()
{
	CLK_EnableModuleClock(TMR1_MODULE);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HXT, 0);
    /* Open Timer0 in periodic mode, enable interrupt and 10 interrupt tick per second */
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 10);   //1HZ周期中断
    TIMER_EnableInt(TIMER1); 	
    /* Enable Timer0  NVIC */
    NVIC_EnableIRQ(TMR1_IRQn);
	/* Start Timer0  counting */
    TIMER_Start(TIMER1);

}

void TMR1_IRQHandler(void)  //1s
{
	TIMER_ClearIntFlag(TIMER1);
	CounterLed++;
	if(CounterLed == 10)
	{
		P42 = 1;
	}
	else if(CounterLed == 20)
	{
		P42 = 0;
		CounterLed = 0;
	}
	CounterDelay++;
	if(CounterDelay == 10)
	{
		CounterDelay = 0;
	}
}





/*---------------------------------------------------------------------------------------------------------*/
/* Function: AdcSingleModeTest                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*   ADC single mode test.                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void AdcSingleModeTest()
{
    uint8_t  u8Option;
    int32_t  i32ConversionData;

    printf("\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("|                      ADC single mode sample code                     |\n");
    printf("+----------------------------------------------------------------------+\n");

    while(1)
    {
	
        printf("Select input mode:\n");
        printf("  [1] Single end input (channel 2 only)\n");
        printf("  [2] Differential input (channel pair 1 only)\n");
        printf("  Other keys: exit single mode test\n");
      
	    //u8Option = getchar();
        u8Option = '1';
		if(u8Option == '1')
        {

            /* Set the ADC operation mode as single, input mode as single-end and enable the analog input channel 2 */
            ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, 0x1 << 2);

            /* Power on ADC module */
            ADC_POWER_ON(ADC);

            /* clear the A/D interrupt flag for safe */
            ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

            /* Enable the ADC interrupt */
            ADC_EnableInt(ADC, ADC_ADF_INT);
            NVIC_EnableIRQ(ADC_IRQn);

            /* Reset the ADC interrupt indicator and Start A/D conversion */
            g_u32AdcIntFlag = 0;
            ADC_START_CONV(ADC);

		

            /* Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function)*/
            while(g_u32AdcIntFlag == 0);

            /* Disable the ADC interrupt */
            ADC_DisableInt(ADC, ADC_ADF_INT);

            /* Get the conversion result of the ADC channel 2 */
            i32ConversionData = ADC_GET_CONVERSION_DATA(ADC, 2);

			youliang = 	i32ConversionData;

            printf("Conversion result of channel 2: 0x%X (%d)\n\n", i32ConversionData, i32ConversionData);

			//while(CounterDelay != 10);

		
        }
        else if(u8Option == '2')
        {

            /* Set the ADC operation mode as single, input mode as differential and
               enable analog input channel 2 for differential input channel pair 1*/
            ADC_Open(ADC, ADC_ADCR_DIFFEN_DIFFERENTIAL, ADC_ADCR_ADMD_SINGLE, 0x1 << 2);

			

            /* Power on ADC module */
            ADC_POWER_ON(ADC);

            /* clear the A/D interrupt flag for safe */
            ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

            /* Enable the ADC interrupt */
            ADC_EnableInt(ADC, ADC_ADF_INT);
            NVIC_EnableIRQ(ADC_IRQn);

            /* Reset the ADC interrupt indicator and Start A/D conversion */
            g_u32AdcIntFlag = 0;
            ADC_START_CONV(ADC);

            /* Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function)*/
            while(g_u32AdcIntFlag == 0);

            /* Disable the ADC interrupt */
            ADC_DisableInt(ADC, ADC_ADF_INT);

            /* Get the conversion result of the specified ADC channel */
            i32ConversionData = ADC_GET_CONVERSION_DATA(ADC, 2);
            printf("Conversion result of channel pair 1: 0x%X (%d)\n\n", i32ConversionData, i32ConversionData);
        }
        else
            return ;

    }
}

/*---------------------------------------------------------------------------------------------------------*/
// ADC Oil capture
// Function:  ADC oil
// Function Name:   CaptureOilValue
// Author And Date: wushengjun 2017.8.29 
/*---------------------------------------------------------------------------------------------------------*/
void CaptureOilValue()
{
			int32_t  i32ConversionData;

            /* Set the ADC operation mode as single, input mode as single-end and enable the analog input channel 2 */
            //ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, 0x1 << 2);
			ADC_Open(ADC, ADC_ADCR_DIFFEN_DIFFERENTIAL, ADC_ADCR_ADMD_SINGLE, BIT4);

            /* Power on ADC module */
            ADC_POWER_ON(ADC);

            /* clear the A/D interrupt flag for safe */
            ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

            /* Enable the ADC interrupt */
            ADC_EnableInt(ADC, ADC_ADF_INT);
            NVIC_EnableIRQ(ADC_IRQn);

            /* Reset the ADC interrupt indicator and Start A/D conversion */
            g_u32AdcIntFlag = 0;
            ADC_START_CONV(ADC);

            /* Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function)*/
            while(g_u32AdcIntFlag == 0);

            /* Disable the ADC interrupt */
            ADC_DisableInt(ADC, ADC_ADF_INT);

            /* Get the conversion result of the ADC channel 2 */
            i32ConversionData = ADC_GET_CONVERSION_DATA(ADC, 4);
            //printf("Conversion result of channel 2: 0x%X (%d)\n\n", i32ConversionData, i32ConversionData);

		
		// 应用
		//
		#if 1
		if((i32ConversionData>143)&&(i32ConversionData<162))// 148 ~ 157
		{
			youliang = 0x12; //8 格
		}
		else if((i32ConversionData>350)&&(i32ConversionData<370))//355 ~ 365
		{
			youliang = 0x11; //7 格
		}
		else if((i32ConversionData>579)&&(i32ConversionData<597))//584 ~ 592
		{
			youliang = 0x10; //6 格
		}
		else if((i32ConversionData>785)&&(i32ConversionData<806))//790 ~ 801
		{
			youliang = 0x0F; //5 格
		}
		else if((i32ConversionData>999)&&(i32ConversionData<1020))// 1004 ~ 1015
		{
			youliang = 0x0E; //4 格
		}
		else if((i32ConversionData>1190)&&(i32ConversionData<1211))//1195 ~ 1206
		{
			youliang = 0x0D; //3 格
		}
		else if((i32ConversionData>1404)&&(i32ConversionData<1423))// 1409 ~ 1418
		{
			youliang = 0x0C; //2 格
		}
		else if((i32ConversionData>1625)&&(i32ConversionData<1637))//1620 ~ 1632
		{
			youliang = 0x0B; //1 格
		}
		else if((i32ConversionData>1760)&&(i32ConversionData<1777))//1765 ~ 1772
		{
			youliang = 0x0A; //0 格
		}

		printf("Conversion result of channel 2: 0x%X (%d)\n\n", youliang, youliang-10);

		#endif
		
}                                                                                         
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* ADC interrupt handler                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void ADC_IRQHandler(void)
{
    g_u32AdcIntFlag = 1;
    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT); /* clear the A/D conversion flag */
}



void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 Baudrate */
    //UART_Open(UART0, 115200);
	UART_Open(UART0, 9600);

	_sys_uUartTxHead = _sys_uUartTxTail = NULL;
}



/*-------------------------------------------------------------------------------------------------------------*/


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

void  SendToComm(char *comm) 
{
    sysPrintf(comm); 
}




void uartpfnCallback(uint8_t* u8Buf,uint32_t u32Len)
{
     uint8_t  j;  
      if(bufloc >= 300) 
	  {
	  	memset((void *)strbuf,0,300);
		bufloc =0;
	  }   
    for(j = 0;j<u32Len;j++)
    {    
       *(strbuf+bufloc)  = *(u8Buf+j);
      // *(u8Buf+j) = 0;  
        bufloc++;
    }  
    
}


/*---------------------------------------------------------------------------------------------------------*/
//Function:
//
void GetSrand(unsigned int seeder,unsigned char TempArray[])
{
	 srand(seeder);
	for(i = 0; i < seeder; i++)
	{	 
		
		TempArray[i]=rand()%200;
		//printf("temparray[%d]  = %d\t",i,TempArray[i]);
	}
}
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
    UART_TEST_HANDLE();
}

/*---------------------------------------------------------------------------------------------------------*/
/* UART Callback function                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void UART_TEST_HANDLE()
{
    uint8_t u8InChar = 0xFF;
    uint32_t u32IntSts = UART0->ISR;

#if 0
    if(u32IntSts & UART_ISR_RDA_INT_Msk)
    {
        //printf("\nInput:");

        /* Get all the input characters */
        while(UART_IS_RX_READY(UART0))
        {
            /* Get the character from UART Buffer */
            u8InChar = UART_READ(UART0);

            //printf("%c ", u8InChar);

            if(u8InChar == '0')
            {
                //g_bWait = FALSE;
            }

            /* Check if buffer full */
            if(g_u32comRbytes < RXBUFSIZE)
            {
                /* Enqueue the character */
                g_u8RecData[g_u32comRtail] = u8InChar;
                g_u32comRtail = (g_u32comRtail == (RXBUFSIZE - 1)) ? 0 : (g_u32comRtail + 1);
                g_u32comRbytes++;
            }

					
        }
        //printf("\nTransmission Test:");		
    }
#endif


	   //u32IntSts = 	UART0->ISR;
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


  #if 0
    if(u32IntSts & UART_ISR_THRE_INT_Msk)
    {
        uint16_t tmp;
        tmp = g_u32comRtail;
        if(g_u32comRhead != tmp)
        {
            u8InChar = g_u8RecData[g_u32comRhead];
            while(UART_IS_TX_FULL(UART0));  /* Wait Tx is not full to transmit data */            
            UART_WRITE(UART0, u8InChar);
            g_u32comRhead = (g_u32comRhead == (RXBUFSIZE - 1)) ? 0 : (g_u32comRhead + 1);
            g_u32comRbytes--;
        }
    }
 #endif

//	 if(g_u8RecData[2] == '3')
//	 {
//	 	 printf("\r\nHello world!!!!\r\n");
//	 }
 
   

#if 1
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
#endif
		ReceiveData();



}



void SendData(uint8_t *BufferSend,uint8_t len)
{
	
	for(counter = 0; counter < len; counter++)
	{		
		UART_WRITE(UART0, BufferSend[counter]);
		counter++;
		if( (UART0->IER & UART_IER_THRE_IEN_Msk)== 0)
		UART_EnableInt(UART0,  UART_IER_THRE_IEN_Msk );	
	}

	counter = 0;		
}

/*---------------------------------------------------------------------------------------------------------*/
/*  UART Function Test                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void UART_FunctionTest()
{
    printf("+-----------------------------------------------------------+\n");
    printf("|  UART Function Test                                       |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|  Description :                                            |\n");
    printf("|    The sample code will print input char on terminal      |\n");
    printf("|    Please enter any to start     (Press '0' to exit)      |\n");
    printf("+-----------------------------------------------------------+\n");

    /*
        Using a RS232 cable to connect UART0 and PC.
        UART0 is set to debug port. UART0 is enable RDA and RLS interrupt.
        When inputing char to terminal screen, RDA interrupt will happen and
        UART0 will print the received char on screen.
    */

    /* Enable Interrupt and install the call back function */
    UART_ENABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    NVIC_EnableIRQ(UART0_IRQn);
    //while(g_bWait);
	while(g_bWait)
	{
	    SendToComm((char *)TestBuffer);
		SendToComm("\r\n");
	}

    /* Disable Interrupt */
    //UART_DISABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    //NVIC_DisableIRQ(UART0_IRQn);
    g_bWait = TRUE;
    printf("\nUART Sample Demo End.\n");

}




/*-----------------------------------------------------------------------------------------------------------*/
//Author And Date: wsj 2017.8.24
//Function:
//Function Name:
//
/*-----------------------------------------------------------------------------------------------------------*/
const unsigned char MIYAO[] = { 
						0xbe,0x06,0x7c,0x5e,0xc6,0xda,0xfa,0x12,0xb3,0x98,
						0x0e,0xfe,0xde,0x00,0x40,0xf8,0xe0,0x87,0x33,0xf4,
						0x80,0x75,0x55,0xfb,0xdf,0x79,0xf9,0xae,0xa3,0x3b,
						0x1e,0x65,0x4e,0x30,0x11,0x21,0x07,0x4d,0x7b,0x2f,
						0x25,0x69,0x5f,0x6d,0x8b,0xc5,0xb3,0xa5,0x87,0x01,
						0x86,0xa9,0xab,0xb0,0x06,0xff,0xeb,0x35,0x19,0x95,
						0x88,0x43,0x21,0x70,0xfd,0xad,0x8b,0x56,0xaf,0xda,
						0x73,0x32,0xf1,0x8b,0xb9,0x23,0x12,0x34,0x07,0x0f,
						0x00,0x39,0x28,0x15,0xad,0xce,0x5c,0xb3,0xa5,0xcd,
						0x0a,0x65,0x77,0x43,0xf3,0xd4,0xfe,0xea,0x99,0x54,
						
						0x33,0x12,0x34,0xfa,0xad,0x0a,0xa9,0xf8,0x7c,0x3b,
						0x99,0xaa,0xfd,0xd4,0xcb,0x3e,0xee,0x03,0x98,0xfc,
						0x22,0x19,0xae,0x00,0x56,0x67,0x7f,0xbd,0xac,0xe4,
						0x27,0x36,0x08,0x89,0x60,0x44,0xf6,0xa9,0xa0,0x2f,
						0x88,0x7d,0xab,0xbe,0x35,0x2b,0xbc,0xf5,0x45,0x2a,
						0x14,0x41,0xd5,0x44,0x39,0xa4,0x2b,0x9d,0x2c,0x35,
						0x90,0x05,0xb5,0x15,0xa6,0x4b,0x33,0x78,0xce,0x06,
						0x01,0x95,0xa3,0x22,0x10,0x0a,0x0f,0x25,0x13,0x5d,
						0x38,0x07,0xf4,0x5d,0x3a,0x08,0x3c,0xc2,0xaa,0x1b,
						0x99,0x8f,0x54,0xf2,0xfb,0x21,0x56,0xda,0x59,0x0e
					};






/*---------------------------------------------------------------------------------------------------------*/
/*  UART Function Send MIYAO                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#if 0
void UART_SendMiYao()
{

    /* Enable Interrupt and install the call back function */
    UART_ENABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    NVIC_EnableIRQ(UART0_IRQn);
    //while(g_bWait);
	while((g_bWait)&&(g_u8IsWDTTimeoutINT == 0))
	{
		//GetSrand(2,JiaMiCoordinate);
		//CaptureOilValue();
		UART_SendMiYaoData();
		while(CounterDelay == 100);
		WDT_RESET_COUNTER();//喂狗
	}

    /* Disable Interrupt */
    //UART_DISABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    //NVIC_DisableIRQ(UART0_IRQn);
    g_bWait = TRUE;
    printf("\nUART Sample Demo End.\n");

}
#endif

void UART_SendMiYao()
{

    /* Enable Interrupt and install the call back function */
    UART_ENABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    NVIC_EnableIRQ(UART0_IRQn);
	while((g_bWait)&&(g_u8IsWDTTimeoutINT == 0))
	{
		GetSrand(4,JiaMiCoordinate);
		MiYaoZuoBiaoX = JiaMiCoordinate[0]<<8;
		MiYaoZuoBiaoX |= JiaMiCoordinate[1];
		//MiYaoZuoBiaoX *= 327;

		MiYaoZuoBiaoY = JiaMiCoordinate[2]<<8;
		MiYaoZuoBiaoY |= JiaMiCoordinate[3];
		//MiYaoZuoBiaoY *= 176;
		
		CaptureOilValue();
		UART_SendMiYaoData();
		//while(CounterDelay != 1);
		WDT_RESET_COUNTER();//喂狗
	}

    /* Disable Interrupt */
    //UART_DISABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    //NVIC_DisableIRQ(UART0_IRQn);
    g_bWait = TRUE;
    //printf("\nUART Sample Demo End.\n");

	//SendToComm((char *)tempReceiveData);

}


/*---------------------------------------------------------------------------------------------------------*/
/*  UART Function UART_SendMiYaoData()                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

#if 0
void UART_SendMiYaoData()
{
	SendBuffer[0] = 0xfe;
	SendBuffer[1] = 0xfd;

	SendBuffer[2]  = 0x90;//(JiaMiCoordinate[2]&0x7f)
	SendBuffer[3] = 0x88;//((JiaMiCoordinate[0]>>8)&0xff);
	SendBuffer[4] = 0xad;//(JiaMiCoordinate[0]&0xff);
	SendBuffer[5] = 0x80;//(JiaMiCoordinate[1]&0xff);
	SendBuffer[6] = 0x05;//((JiaMiCoordinate[1]>>8)&0xff);
	SendBuffer[7] = 0xdf;//(0xff&(youliang>>16));
	SendBuffer[8] = 0x26;//(0xff&(youliang>>8));
	SendBuffer[9] = 0x3d;//(0xff&youliang);
	SendBuffer[10] = (SendBuffer[2]^SendBuffer[3]^SendBuffer[4]^SendBuffer[5]^SendBuffer[6]^SendBuffer[7]^SendBuffer[8]^SendBuffer[9]);
   
	#if 0
	for(i=0;i<11;i++)
	{
			_PutChar_f(SendBuffer[i]);
	}
	#endif

	SendToComm((char *)SendBuffer);
	


	//GprsSendComm(char *comm);	
}
#endif

#if 1
void UART_SendMiYaoData()
{
	SendBuffer[0] = 0xfe;
	SendBuffer[1] = 0xfd;

	SendBuffer[2]  = 0x90;//(JiaMiCoordinate[2]&0x7f)

	SendBuffer[3] = (((MiYaoZuoBiaoX*327)>>8)&0xff);
	SendBuffer[4] = ((MiYaoZuoBiaoX*327)&0xff);

	SendBuffer[5] = ((MiYaoZuoBiaoY*176)&0xff);
	SendBuffer[6] = (((MiYaoZuoBiaoY*176)>>8)&0xff);


	SendBuffer[7] = 0xdf;//(0xff&(youliang>>16));
	SendBuffer[8] = 0x26;//(0xff&(youliang>>8));
	SendBuffer[9] = 0x3d;//(0xff&youliang);
	SendBuffer[10] = (SendBuffer[2]^SendBuffer[3]^SendBuffer[4]^SendBuffer[5]^SendBuffer[6]^SendBuffer[7]^SendBuffer[8]^SendBuffer[9]);
   
	#if 0
	for(i=0;i<11;i++)
	{
			_PutChar_f(SendBuffer[i]);
	}
	#endif

	SendToComm((char *)SendBuffer);
	while(CounterDelay != 1);
	


	//GprsSendComm(char *comm);	
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*  UART Function UART_SendData()                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

#if 0
void UART_SendData()
{
	SendBuffer[0] = 0xfe;
	SendBuffer[1] = 0xfd;

	SendBuffer[2]  = 0x90;//(JiaMiCoordinate[2]&0x7f)
	SendBuffer[3] = 0x88;//((JiaMiCoordinate[0]>>8)&0xff);
	SendBuffer[4] = 0xad;//(JiaMiCoordinate[0]&0xff);
	SendBuffer[5] = 0x80;//(JiaMiCoordinate[1]&0xff);
	SendBuffer[6] = 0x05;//((JiaMiCoordinate[1]>>8)&0xff);

	SendBuffer[7] = (0xff&(youliang>>16));
	SendBuffer[8] = (0xff&(youliang>>8));
	SendBuffer[9] = (0xff&youliang);

	SendBuffer[10] = (SendBuffer[2]^SendBuffer[3]^SendBuffer[4]^SendBuffer[5]^SendBuffer[6]^SendBuffer[7]^SendBuffer[8]^SendBuffer[9]);
   
	#if 0
	for(i=0;i<11;i++)
	{
			_PutChar_f(SendBuffer[i]);
	}
	#endif

	SendToComm((char *)SendBuffer);
	


	//GprsSendComm(char *comm);	
}
#endif

void UART_SendData()
{
	SendBuffer[0] = 0xfe;
	SendBuffer[1] = 0xfd;

	SendBuffer[2]  = (0x90&0x7f);//(JiaMiCoordinate[2]&0x7f)

	SendBuffer[3] = ((MiYaoZuoBiaoX>>8)&0xff);
	SendBuffer[4] = (MiYaoZuoBiaoX&0xff);

	SendBuffer[5] = (MiYaoZuoBiaoY&0xff);
	SendBuffer[6] = ((MiYaoZuoBiaoY>>8)&0xff);

 	TempSendOil = youliang;
	TempSendOil = ((TempSendOil + 13)*85324 - 44);
	TempSendOil &= 0xFFFFFF;

	SendBuffer[7] = (0xff&(TempSendOil>>16));
	SendBuffer[8] = (0xff&(TempSendOil>>8));
	SendBuffer[9] = (0xff&TempSendOil);

	SendBuffer[10] = (SendBuffer[2]^SendBuffer[3]^SendBuffer[4]^SendBuffer[5]^SendBuffer[6]^SendBuffer[7]^SendBuffer[8]^SendBuffer[9]);
   
	#if 0
	for(i=0;i<11;i++)
	{
			_PutChar_f(SendBuffer[i]);
	}
	#endif

	SendToComm((char *)SendBuffer);
	//while(CounterDelay != 10);
}


/*---------------------------------------------------------------------------------------------------------*/
/* UART Receive Data                                                                                       */
/* Function: receive data from uart0                                                                       */
/* Function name: void ReceiveData (void)                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void ReceiveData (void)
{
	  #if 0
	    //receive
			
		if(((g_u8RecData[2]^g_u8RecData[3]^g_u8RecData[4]^g_u8RecData[5]^g_u8RecData[6]^g_u8RecData[7]^g_u8RecData[8]^g_u8RecData[9]) == g_u8RecData[10])&&(g_u8RecData[10] != 0x00))
		{
			printf("\n*** Receive  R... RECEIVE DAtA ***\n");
			//if((MIYAO[MiYaoZuoBiaoX] == g_u8RecData[6]))// &&(MIYAO[MiYaoZuoBiaoY] == g_u8RecData[7])
			{
					
					//memcpy(tempReceiveData,g_u8RecData,1024);	
					g_bWait = FALSE;
				

//					for(k=0;k<9;k++)
//					{
//						SendToComm((char *)g_u8RecData[k]);
//						//printf("\n*** 123546546546546  w...666 RECEIVE DAtA ***\n");
//					}
			}
		
		}
		else
		{
			WDT_CLEAR_TIMEOUT_INT_FLAG();
			//g_u8IsWDTTimeoutINT = 0;	
		}
		//else
		//{
		//	memset(g_u8RecData,0,1024);
		//}
		#endif

#if 0
	 if((g_u8RecData[2] == 0x90)&&(g_u8RecData[6] == 0x88))
	 {
	 	 printf("\r\nHello world!!!!\r\n");
		 g_bWait = FALSE;
	 }
#endif
	if((strbuf[2]==0x90)&&(strbuf[6] == 0x88))
	{
		 printf("\r\nHello world!!!!\r\n");
		 g_bWait = FALSE;
	}
		
}


/*---------------------------------------------------------------------------------------------------------*/
/* UART Test Sample                                                                                        */
/* Test Item                                                                                               */
/* It sends the received data to HyperTerminal.                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

	/* Timer init */
	TIMER1_Init();

    /* Init UART0 for printf and testing */
    UART0_Init();

#if 1
/*--------------------------------------------------------------------------------------------------------------*/
// Watch dog init 
//
    if(WDT_GET_RESET_FLAG() == 1)
    {
        /* Use P0.0 to check time-out period time */
        WDT_CLEAR_RESET_FLAG();
        //printf("\n*** WDT time-out reset occurred ***\n");
        //while(1);
    }

	SYS_UnlockReg();
    g_u8IsWDTTimeoutINT = 0;
    WDT_Open(WDT_TIMEOUT_2POW14, WDT_RESET_DELAY_1026CLK, TRUE, FALSE);

    /* Enable WDT interrupt function */
    WDT_EnableInt();

    /* Enable WDT NVIC */
    NVIC_EnableIRQ(WDT_IRQn);
	/*---------------------------------------------------------------------------------------------------------*/

    /*---------------------------------------------------------------------------------------------------------*/
    /* SAMPLE CODE                                                                                             */
    /*---------------------------------------------------------------------------------------------------------*/

    printf("\n\nCPU @ %dHz\n", SystemCoreClock);
    printf("\n\nUART Sample Program\n");

    /* UART sample function */
    UART_FunctionTest();
	//UART_SendMiYao();

	ReceiveData(); 
    //while(1)
	while(g_u8IsWDTTimeoutINT == 0)
	{
	 	//printf("\r\nwushengjun wubinghan\n");
		UART_SendData();
		WDT_RESET_COUNTER();//喂狗
	}
#endif

#if 0
    /*---------------------------------------------------------------------------------------------------------*/
    /* SAMPLE CODE                                                                                             */
    /*---------------------------------------------------------------------------------------------------------*/

    printf("\nSystem clock rate: %d Hz", SystemCoreClock);

    /* Single Mode test */
    //AdcSingleModeTest();
	while (1)
	{
		CaptureOilValue();
    }
	/* Disable ADC module */
    ADC_Close(ADC);

    /* Disable ADC IP clock */
    CLK_DisableModuleClock(ADC_MODULE);

    /* Disable External Interrupt */
    NVIC_DisableIRQ(ADC_IRQn);

    printf("\nExit ADC sample code\n");

    while(1);
#endif

}




