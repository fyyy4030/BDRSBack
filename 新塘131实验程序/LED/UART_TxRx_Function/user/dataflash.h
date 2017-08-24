#ifndef __dataflash_h
#define __dataflash_h

#include <stdio.h>
#include "NUC131.h"
#include "globle.h"

#define NDFBADR          (0x0001F000)


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



void EEPROMwrite_Hours(void)
{
	//read_hours = DF_ReadHour();
	//if(read_hours != hours)
	SYS_UnlockReg();
	DF_WriteHour(hours);
	SYS_LockReg();
}
void EEPROMread_Hous(void)
{
	SYS_UnlockReg();
	hours = DF_ReadHour();
	
	//if(hours > 9999)//hours>9999
	if(hours > 99999)
	{
		hours = 0;
		DF_WriteHour(0);
		
	}
	SYS_LockReg();
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


void save(void)
{ uint32_t dat0 = 0;
	
 if(flag & BIT(d_save))
  {
    
	if(flag & BIT(av_auto)){save_flag |= BIT(0);}
	  else{save_flag &=~ BIT(0);}
	  /*
	if(flag & BIT(work)){save_flag |= BIT(1);}
	  else{save_flag &=~ BIT(1);}
	  */
	if(ECU_flag & BIT(Miji)){save_flag |= BIT(2);}
	  else{save_flag &=~ BIT(2);}
	if(flag & BIT(av_turn)){save_flag |= BIT(3);}
	  else{save_flag &=~ BIT(3);}
	   //EEPROMwrite(0x03,save_flag);
    // EEPROMwrite(0x04,pagebuff);
		
		//dat0 =pagebuff;
		dat0=(dat0<<8)+save_flag;
		dat0=(dat0<<16)+avtime;
		SYS_UnlockReg();
    DF_WriteSWstate(dat0);
		SYS_LockReg();
    flag &=~ BIT(d_save);
  }
}
void sns_read(void)
{  uint32_t dat0 = 0;
	SYS_UnlockReg();
  dat0=DF_ReadSWstate();
	SYS_LockReg();
	avtime=(unsigned int)dat0;
	save_flag=(unsigned char)(dat0>>16);
	//pagebuff=(unsigned char)(dat0>>24);
  if(avtime>9999)
  {
	 flag |=BIT(d_save);
	 avtime=5;
  }
	
	//pagebuff=EEPROMread(0x04);
	/*
  if(pagebuff>7)
	{
		flag |=BIT(d_save);
  	pagebuff=1;
  }*/
  //save_flag=EEPROMread(0x03);

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


//2017.8.7
//wushengjun add

uint32_t DF_ReadccidFlag(void)
{
	uint32_t dat = 0,dat1 = 0,dat2 = 0;
	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	
	{
		dat2 = DF_Read(NDFBADR+2048+512+dat1);
		if(dat2 == 0xffffffff)
		{
			break;
		} 
		else 
		{
			dat = dat2;
		}
	}
	FMC_DISABLE_ISP();
	return dat;
}

void DF_WriteccidFlag(uint32_t dat)
{
	uint32_t dat1 = 0,dat2 = 0;
	FMC_ENABLE_ISP();
	for(dat1=0;dat1<512;dat1+=4)	
	{
		dat2 = DF_Read(NDFBADR+2048+512+dat1);
		if(dat2 == 0xffffffff)
		{
			break;
		}
	}
	if(dat1==512)
	{
		DF_Erase(NDFBADR+2048+512);
		dat1 = 0;
	}	
	DF_Write(NDFBADR+2048+512+dat1,dat);
	FMC_DISABLE_ISP();
}

uint32_t EEPROMread_ccidFlag(void)
{ 
	uint32_t state;
	SYS_UnlockReg();
	state =DF_ReadccidFlag();
	SYS_LockReg();
	return state;
}

//写CCID标志
void EEPROMwrite_ccidFlag(uint32_t state)
{	
  	SYS_UnlockReg();
	DF_WriteccidFlag(state);
	SYS_LockReg();
}
/***************************************************************************************************************************/
void DF_ReadCCIDNumber(uint32_t dat2[])
{
	//uint32_t dat = 0,dat1 = 0,dat2 = 0;
	uint32_t dat = 0,dat1 = 0;
	//uint32_t dat2[5];
	uint8_t Readnum = 0;

	FMC_ENABLE_ISP();
	/*
	for(dat1=0;dat1<512;dat1+=4)	
	{
		dat2 = DF_Read(NDFBADR+2048+1024+dat1);
		if(dat2 == 0xffffffff)
		{
			break;
		} 
		else 
		{
			dat = dat2;
		}
	}
	FMC_DISABLE_ISP();
	*/

	for(dat1=0;dat1<20;dat1+=4)	
	{
		dat2[Readnum] = DF_Read(NDFBADR+2048+1024+dat1);
		Readnum++;
	}
	Readnum = 0;
	FMC_DISABLE_ISP();
	//return dat2;
}

void DF_WriteCCIDNumber(uint32_t dat[])
{
	uint32_t dat1 = 0,dat2 = 0;
	uint8_t num = 0;
	FMC_ENABLE_ISP();
	/*
	for(dat1=0;dat1<512;dat1+=4)	
	{
		dat2 = DF_Read(NDFBADR+2048+1024+dat1);
		if(dat2 == 0xffffffff)
		{
			break;
		}
	}
	if(dat1==512)
	{
		DF_Erase(NDFBADR+2048+1024);
		dat1 = 0;
	}
	DF_Write(NDFBADR+2048+1024+dat1,dat);
	*/
		
	for(dat1=0;dat1<20;dat1+=4)
	{
		DF_Write(NDFBADR+2048+1024+dat1,dat[num]);
		num++;	
	}	
	FMC_DISABLE_ISP();
}

uint32_t EEPROMread_CCIDNumber(uint32_t dat2[])
{ 
	uint32_t state;
	SYS_UnlockReg();
	//state =DF_ReadCCIDNumber(dat2);
	DF_ReadCCIDNumber(dat2);
	SYS_LockReg();
	return state;
}

//写CCIDNumber标志
void EEPROMwrite_CCIDNumber(uint32_t state[])
{	
  	SYS_UnlockReg();
	DF_WriteCCIDNumber(state);
	SYS_LockReg();
}
/***************************************************************************************************************************/


#endif
