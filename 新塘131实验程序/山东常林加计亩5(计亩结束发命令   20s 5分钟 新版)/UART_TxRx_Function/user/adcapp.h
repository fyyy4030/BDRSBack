#ifndef		_ADCAPP_H_
#define		_ADCAPP_H_

#include <stdio.h>
#include "NUC131.h"
#include "globle.h"
#include "proto.h"

//const Word FUELTABLE[] = {891,801,713,609,497,416,306,209,107};
const Word FUELTABLE[] = {2125,1896,1665,1432,1198,960,718,464,235};
unsigned int voltage,volt_bat;

#if 0
void ADC_INIT()
{
	 /* Disable the GPA0 - GPA7 digital input path to avoid the leakage current. */
    GPIO_DISABLE_DIGITAL_PATH(PA, 0xEF);

	  /* Configure the GPA0 - GPA7 ADC analog input pins */
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA0_Msk | SYS_GPA_MFP_PA1_Msk | SYS_GPA_MFP_PA2_Msk | SYS_GPA_MFP_PA3_Msk) ;
    SYS->GPA_MFP |= SYS_GPA_MFP_PA0_ADC0 | SYS_GPA_MFP_PA1_ADC1 | SYS_GPA_MFP_PA2_ADC2 | SYS_GPA_MFP_PA3_ADC3 ;
	  SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA4_Msk | SYS_GPA_MFP_PA5_Msk | SYS_GPA_MFP_PA6_Msk | SYS_GPA_MFP_PA7_Msk) ;
    SYS->GPA_MFP |=  SYS_GPA_MFP_PA5_ADC5 | SYS_GPA_MFP_PA6_ADC6 | SYS_GPA_MFP_PA7_ADC7 ;
	 /* Power on ADC module */
    ADC_POWER_ON(ADC);
	 /* Clear the A/D interrupt flag for safe */
    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);	
}

int Read_ADC(unsigned char channel)
{
    int i;

	 /* Set the ADC operation mode as single, input mode as single-end and enable the analog input channel 2 */
    ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, 0x1 << channel);

	ADC_START_CONV(ADC);

	/* Wait conversion done */
    while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));

    i = ADC_GET_CONVERSION_DATA(ADC, channel);	 

	  /* Clear the A/D interrupt flag for safe */
            ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);  
    return i;
}


void Adc_Data (void)
{
	Byte i=0;
	long dat0=0,dat5=0,dat7=0;
	unsigned long loop=0;
	Byte loopb=0;
	
	//基准电压采集
	//for(i=0;i<16;i++) {dat7 += Read_ADC(7);}

	//电源电压采集
	for(i=0;i<16;i++) {dat5 += Read_ADC(5);}

	voltage = dat5/171;	   //voltage  xx.x伏
	voltage = Get_Data(voltagetemp,voltage,10);
	//NewMachineFrame.u16voltage =   voltage;	
//油量
	for(i=0;i<16;i++) {dat0 += Read_ADC(0);}
	loop= dat0/16;
	//loop = loop*4095/dat7;
	ylad=loop;
	if(loop<4000)
	{
		if(loop>FUELTABLE[0])
		loop = FUELTABLE[0];
	 if(loop<FUELTABLE[8])
		loop = FUELTABLE[8];
	loopb = 0;
	while(loop<FUELTABLE[loopb])
		loopb++;
	if(0==loopb)
		loop = 0;
	else
		//loop = 180*loopb - (loop-FUELTABLE[loopb])*180/(FUELTABLE[loopb-1]-FUELTABLE[loopb]);
	    loop = (Word)loopb*10-(loop-FUELTABLE[loopb])*10/(FUELTABLE[loopb-1]-FUELTABLE[loopb]);
	if((loop%10) >= 5)//?2???????
	{
		loop = loop+10 ;
	} 
	 oil_m = loop/10;
	}
	else
	{
		oil_m=0;
	}
	
}
#endif


void ADC_INIT()
{
	 /* Disable the GPA0 - GPA7 digital input path to avoid the leakage current. */
    GPIO_DISABLE_DIGITAL_PATH(PA, 0xa3);

	  /* Configure the GPA0 - GPA7 ADC analog input pins */
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA0_Msk | SYS_GPA_MFP_PA1_Msk | SYS_GPA_MFP_PA5_Msk | SYS_GPA_MFP_PA7_Msk) ;
    SYS->GPA_MFP |= SYS_GPA_MFP_PA0_ADC0 | SYS_GPA_MFP_PA1_ADC1 | SYS_GPA_MFP_PA5_ADC5  | SYS_GPA_MFP_PA7_Vref ;
	  //SYS->VREFCR=SYS_VREFCR_ADC_VREFSEL_Msk ;
	  SYS->VREFCR=0 ;
	 /* Power on ADC module */
    ADC_POWER_ON(ADC);
	 /* Clear the A/D interrupt flag for safe */
    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);	
}

int Read_ADC(unsigned char channel)
{
    int i;

	 /* Set the ADC operation mode as single, input mode as single-end and enable the analog input channel 2 */
    ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, 0x1 << channel);

	ADC_START_CONV(ADC);

	/* Wait conversion done */
    while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));

    i = ADC_GET_CONVERSION_DATA(ADC, channel);	 

	  /* Clear the A/D interrupt flag for safe */
            ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);  
    return i;
}


//wushengjun 2017.8.5

void Adc_Data (void)
{
	Byte i=0;
	unsigned long dat0=0,dat1=0;
	
	//备用电压
	for(i=0;i<10;i++) 
	{
		dat0 += Read_ADC(0);
	}
    volt_bat=dat0*453/348160;
	//主电源电压
	for(i=0;i<10;i++) 
	{
		dat1 += Read_ADC(1);
	}
	voltage = dat1*3153/348160+9;	   //voltage  xx.x伏	
}

#endif
