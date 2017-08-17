#ifndef		_ADCAPP_H_
#define		_ADCAPP_H_

#include <stdio.h>
#include "NUC131.h"
#include "globle.h"
unsigned int voltage,volt_bat,in_temputer;
unsigned char adctime=0;
//const uint32_t WENDU[]={34763,31380,27200,22583,18007,13890,10493,7800,5780};
// const uint32_t WENDU[]={34356,31059,27008,23475,17746,13648,10240,7527,5475};
const uint32_t WENDU[]={34253,31000,26932,22145,17693,13610,10140,7405,5433};

uint32_t dataconv,adcbuf;		
uint8_t Temperature,Temperature1;
uint8_t pnbuffstate;

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

uint16_t ReadADCx10( unsigned char channel)
{
	uint8_t i;
	uint16_t templong = 0;
	
	for(i=0;i<10;i++)
	{
		templong += Read_ADC(channel);
	}
	return templong;
}

/* 读取温度                            */
/*******************************************************************/
 void Read_Temperature(void)
{  
	unsigned char i;
	
 	 adcbuf = ReadADCx10(5);
	if(adcbuf>WENDU[0])
		adcbuf= WENDU[0];
	if(adcbuf<WENDU[8])
		adcbuf= WENDU[8];
	i = 0;
	while(adcbuf<WENDU[i])
		i++;
	if(0==i)
		dataconv= 0;
	else
	{
		dataconv = adcbuf;
		adcbuf = i;
		dataconv = 10*adcbuf-(dataconv-WENDU[i])*10/(WENDU[i-1]-WENDU[i]);
	}
	Temperature1 = dataconv; 
	//Temperature1=Get_Data(tempbuff,dataconv,10);
	
	if(Temperature1>=10)
		{ 
			pnbuffstate=0;//正值
			Temperature =Temperature1-10+60;
    }
		else
		{
			pnbuffstate=1;//负值
			//Temperature =10-Temperature1;
			Temperature =60+Temperature1-10;
    }
}


void Adc_Data (void)
{
	Byte i=0;
	unsigned long dat0=0,dat1=0;
	
	//备用电压
	for(i=0;i<10;i++) {dat0 += Read_ADC(0);}
  volt_bat=dat0*453/348160;
	//主电源电压
	for(i=0;i<10;i++) {dat1 += Read_ADC(1);}
	voltage = dat1*3153/348160+9;	   //voltage  xx.x伏	
}

#endif
