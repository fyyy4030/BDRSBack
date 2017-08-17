#ifndef __dataflash_h
#define __dataflash_h

#include <stdio.h>
#include "NUC131.h"
#include "globle.h"

#define NDFBADR          (0x0001F000)
uint8_t Parameter_setitemdata[512];
unsigned char mem_paramdata[24];
void DF_EnableISP(void);
void DF_DisableISP(void);
void DF_Erase(uint32_t u32addr);
uint32_t DF_Read(uint32_t u32addr);
void DF_Write(uint32_t u32addr, uint32_t u32data);
uint32_t DF_ReadSWstate(void);
void DF_WriteSWstate(uint32_t dat);

void DF_EnableISP(void)
{
	FMC->ISPCON |=  FMC_ISPCON_ISPEN_Msk;
}

void DF_DisableISP(void)
{
	 FMC->ISPCON &= ~FMC_ISPCON_ISPEN_Msk;
}

void DF_Erase(uint32_t u32addr)
{
   FMC_Erase( u32addr);
}

uint32_t DF_Read(uint32_t u32addr){	
 return FMC_Read(u32addr);
}

void DF_Write(uint32_t u32addr, uint32_t u32data)
{
   FMC_Write(u32addr, u32data);
}

void DF_ReadParameter_setitem(void)
{
	uint32_t dat1 = 0;
	uint32_t *dat=(uint32_t *)Parameter_setitemdata;
	FMC_ENABLE_ISP();
// 	for(dat1=0;dat1<194;dat1+=4)	
// 	*(dat+dat1) = DF_Read(NDFBADR+dat1);
	for(dat1=0;dat1<48;dat1++)	 //194/4=48.5  196/4=49
	*(dat+dat1) = DF_Read(NDFBADR+dat1*4);
	
	FMC_DISABLE_ISP();

}
void DF_WriteParameter_setitem(void)
{
	uint32_t dat1 = 0;
	uint32_t *dat=(uint32_t *)Parameter_setitemdata;
	FMC_ENABLE_ISP();
	DF_Erase(NDFBADR);
	for(dat1=0;dat1<48;dat1++)	 //194/4=48.5  196/4=49
	DF_Write(NDFBADR+dat1*4,*(dat+dat1));
	FMC_DISABLE_ISP();
}

void DF_Readmem_param(void)
{
	uint32_t dat1 = 0;
	uint32_t *dat=(uint32_t *)mem_paramdata;
	FMC_ENABLE_ISP();
	for(dat1=0;dat1<6;dat1++)	 //20+4
	*(dat+dat1) = DF_Read(NDFBADR+512+dat1*4);
	
	FMC_DISABLE_ISP();

}
void DF_Writemem_param(void)
{
	uint32_t dat1 = 0;
	uint32_t *dat=(uint32_t *)mem_paramdata;
	FMC_ENABLE_ISP();
	DF_Erase(NDFBADR+512);
	for(dat1=0;dat1<6;dat1++)	 //20+4
	DF_Write(NDFBADR+512+dat1*4,*(dat+dat1));
	FMC_DISABLE_ISP();
}



uint32_t DF_ReadSWstate(void){
	uint32_t dat = 0,dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+dat1);
		if(dat2 == 0xffffffff){
			break;
		} else {
			dat = dat2;
		}
	}
	
	FMC_DISABLE_ISP();

	return dat;
}

void DF_WriteSWstate(uint32_t dat){
	uint32_t dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+dat1);
		if(dat2 == 0xffffffff){
			break;
		}
	}
	if(dat1==512){
		DF_Erase(NDFBADR);
		dat1 = 0;
	}	
	DF_Write(NDFBADR+dat1,dat);
	FMC_DISABLE_ISP();
}

uint32_t DF_ReadHour(void){
	uint32_t dat = 0,dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+512+dat1);
		if(dat2 == 0xffffffff){
			break;
		} else {
			dat = dat2;
		}
	}
	FMC_DISABLE_ISP();

	return dat;
}
void DF_WriteHour(uint32_t dat){
	uint32_t dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+512+dat1);
		if(dat2 == 0xffffffff){
			break;
		}
	}
	if(dat1==512){
		DF_Erase(NDFBADR+512);
		dat1 = 0;
	}	
	DF_Write(NDFBADR+512+dat1,dat);
	FMC_DISABLE_ISP();
}

uint32_t DF_Readmiji(void){
	uint32_t dat = 0,dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+1024+dat1);
		if(dat2 == 0xffffffff){
			break;
		} else {
			dat = dat2;
		}
	}
	FMC_DISABLE_ISP();

	return dat;
}
void DF_Writemiji(uint32_t dat){
	uint32_t dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+1024+dat1);
		if(dat2 == 0xffffffff){
			break;
		}
	}
	if(dat1==512){
		DF_Erase(NDFBADR+1024);
		dat1 = 0;
	}	
	DF_Write(NDFBADR+1024+dat1,dat);
	FMC_DISABLE_ISP();
}


uint32_t DF_ReadKilo(void){
	uint32_t dat = 0,dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+1024+512+dat1);
		if(dat2 == 0xffffffff){
			break;
		} else {
			dat = dat2;
		}
	}
	FMC_DISABLE_ISP();

	return dat;
}
void DF_WriteKilo(uint32_t dat){
	uint32_t dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+1024+512+dat1);
		if(dat2 == 0xffffffff){
			break;
		}
	}
	if(dat1==512){
		DF_Erase(NDFBADR+1024+512);
		dat1 = 0;
	}	
	DF_Write(NDFBADR+1024+512+dat1,dat);
	FMC_DISABLE_ISP();
}



void EEPROMread_Hous(void)
{
	
	hours = DF_ReadHour();
	
	//if(hours > 9999)//hours>9999
	if(hours > 60000)
	{
		hours = 0;
		DF_WriteHour(0);
		
	}
}


void EEPROMwrite_Kilos(void)
{
	 SYS_UnlockReg();
   DF_WriteKilo(sum_kilometre);
	 SYS_LockReg();
}

void EEPROMread_Kilos(void)
{
	SYS_UnlockReg();
	sum_kilometre = DF_ReadKilo();
	if(sum_kilometre > 999999)
	{
		sum_kilometre = 0;
		DF_WriteKilo(0);
	}
	SYS_LockReg();
}

void EEPROMwrite_Meter(void)
{
	
  SYS_UnlockReg();
	DF_Writemiji(cur_meter);
	SYS_LockReg();
}
void EEPROMread_Meter(void)
{
	SYS_UnlockReg();
	cur_meter = DF_Readmiji();
	if(cur_meter > 9999999)//
	{
		cur_meter = 0;
		DF_Writemiji(0);
	}
	SYS_LockReg();
}



uint32_t DF_Readboundstate(void){
	uint32_t dat = 0,dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+2048+dat1);
		if(dat2 == 0xffffffff){
			break;
		} else {
			dat = dat2;
		}
	}
	FMC_DISABLE_ISP();

	return dat;
}
void DF_Writeboundstate(uint32_t dat){
	uint32_t dat1 = 0,dat2 = 0;

	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	{
		dat2 = DF_Read(NDFBADR+2048+dat1);
		if(dat2 == 0xffffffff){
			break;
		}
	}
	if(dat1==512){
		DF_Erase(NDFBADR+2048);
		dat1 = 0;
	}	
	DF_Write(NDFBADR+2048+dat1,dat);
	FMC_DISABLE_ISP();
}

uint32_t EEPROMread_boundstate(void)
{ uint32_t state;
	SYS_UnlockReg();
	state =DF_Readboundstate();
	SYS_LockReg();
	return state;
}

void EEPROMwrite_boundstate(uint32_t state)
{
	
  SYS_UnlockReg();
	DF_Writeboundstate(state);
	SYS_LockReg();
}


#endif
