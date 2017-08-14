#ifndef __UARTPRROC_H__
#define __UARTPROC_H__
#include <stdlib.h>
#include <string.h>	 
#include <stdio.h>
#include "NUC131.h"

#define sysTxBufReadNextOne()	(((_sys_uUartTxHead+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxHead+1)
#define sysTxBufWriteNextOne()	(((_sys_uUartTxTail+1)==UART_BUFFSIZE)? NULL: _sys_uUartTxTail+1)
#define UART_BUFFSIZE	256
uint8_t _sys_ucUartTxBuf[UART_BUFFSIZE];
uint16_t _sys_uUartTxHead, _sys_uUartTxTail;

#define RX_ARRAY_NUM 100
 uint8_t uart_rx[RX_ARRAY_NUM] = {0};
 uint8_t uart_rx2[RX_ARRAY_NUM] = {0};
 uint32_t rx_cnt = 0;
 uint32_t rx_cnt2 = 0;

 
void UART0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 module */
    SYS_ResetModule(UART0_RST);
    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 115200);																	   http://time.qq.com/?pgv_ref=aiotime

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
	 //memset((void *)strbuf,0,300);
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

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

#endif
