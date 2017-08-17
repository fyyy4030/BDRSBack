//*  AUTHAR£∫ 	jerrydong	
//*  DATE:		20120802
//*	 FUNCTION£∫	—” ±
#ifndef _DELAY_H_
#define _DELAY_H_


void Delay_Us(uint32_t dat){
	for(;dat!=0;dat--){
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
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




#endif
