
#ifndef		_GLOBLE_H_
#define		_GLOBLE_H_

#include <stdint.h>
#include "NUC131.h"

typedef	unsigned char	Byte;
typedef	unsigned int 	Word;
typedef	unsigned long	Long;

	
#define BIT(x)	(1 << (x))

unsigned int fdj_rev=0,fdj_temp[10];

unsigned char oil_m,oil_p,press_tep,watmp,watmp1;

unsigned int Wtemp[10],oil_ptemp[10];
//unsigned char errtime=0,err2time=0;
unsigned long err;
unsigned int shisu;



unsigned char tongxintime=50;
unsigned long Engine_hours=0;


Long hours = 0;

Word kilo_pluse = 0;
Long sum_kilometre = 0;
Long cur_meter = 0;
Byte kilometretemp = 0;
//数码管
const uint16_t num_dis[]={0x197,0x180,0x10F,0x18B,0x198,0x9B,0x9F,0x181,0x19F,0x19B};
uint8_t disnum,chargestoptime,battakedowncheck_count=0;
uint8_t dormantlevel=0,dormantlevelstat=0,dormant_wakeup;//休眠级别 0主电工作 1主电休眠 2备电工作 3备电休眠
uint8_t dormant1stat,dormant2stat,dormant3stat,wakeuptime2min,wakeup1stat,wakeup2stat;
uint16_t dormant_ACCofftime=580,time_245v;
uint8_t restartgps=0;//重启标志	
uint8_t mem_exeflag=0;//执行过存储标记	
uint32_t dormanttime;	
uint8_t fwq_connectfail=0,send0xAA=0,check_baojingflag,dormantlevellow,gprslinkfailtime,volt_bat37low;	





unsigned int Get_Data(unsigned int *data_pointer, unsigned int data1, unsigned char data_num)	
{
	unsigned int i;
	unsigned long temp;
	//队列右移先进先出
	for(i=0;i<data_num-1;i++)
	{
		*(data_pointer+data_num-i-1) = *(data_pointer+data_num-i-2);
	}
	*(data_pointer) = data1;	
	temp = 0;
	for(i=0;i<data_num;i++)
	{
		temp += *(data_pointer+i);
	}
	return(temp/data_num);
}






#endif
