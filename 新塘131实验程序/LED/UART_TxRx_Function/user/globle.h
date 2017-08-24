
#ifndef		_GLOBLE_H_
#define		_GLOBLE_H_

#include <stdint.h>
#include "NUC131.h"

typedef	unsigned char	Byte;
typedef	unsigned int 	Word;
typedef	unsigned long	Long;

	
#ifndef BIT
#define BIT(x)	(1 << (x))
#endif


#define 	P_SWA		((PA->PIN)) // V1/V2
#define 	L_SWA		6
#define 	P_SWB		((PA->PIN)) //��������
#define 	L_SWB		1
#define 	P_LED		((PB->DOUT))
#define 	L_LED		12

#define 	P_vcc		((PA->DOUT)) //VCC���	 1
#define 	L_vcc		13
#define 	P_vcc1		((PA->DOUT)) //��������	 1
#define 	L_vcc1		14
//#define 	P_vcc2		PORTA //�����Դ
//#define 	L_vcc2		1
#define 	P_vcc2		((PA->DOUT)) 	     //1
#define 	L_vcc2		14
#define 	P_vcc3		((PC->DOUT)) //���ֵ�Դ  1
#define 	L_vcc3		8

#define 	P_E1		((PC->DOUT))//������Ƶ���� 1
#define 	L_E1		1 
//#define 	P_E2		PORTB //������Ƶ����
//#define 	L_E2		3
#define 	P_E2		((PC->DOUT)) 
#define 	L_E2		1
#define 	P_E3		((PC->DOUT)) //������Ƶ����	    1
#define 	L_E3		3

#define 	P_BP		((PF->DOUT))//������
#define 	L_BP		5

#define DDJC_PORT ((PB->PIN))
#define DDJC 	  PB14

unsigned char page=0,pagebuff=1,pagread=0,ddflag=0;//????????1???
unsigned char optime10s,opok=0,startokbp=0;
unsigned int avtime,avtimetemp;
//unsigned char chishu;
unsigned int fdj_pulse;
unsigned int fdj_rev=0,fdj_temp[10];
unsigned int voltage;
unsigned int voltagetemp[20];
unsigned char oil_m,oil_p,press_tep,watmp,watmp1;
unsigned char watmp=0,watmptab=0,oiltab=0;
//unsigned int oil_p,oil_m;
unsigned int Wtemp[10],oil_ptemp[10];//
unsigned char errtime=0,err2time=0;//
unsigned long err;
unsigned int shisu;
unsigned int ylad;


unsigned char bj_delay600MS,bj_delay12s,tongxintime=50,delay1s,delay10s,bj_delay6s;
unsigned char MCUtongxintime=0;
unsigned char fd_time,bj_ch;//
unsigned int time6min;//Сʱ�Ƽ�ʱ

unsigned long Engine_hours=0;
Long hours = 0;
unsigned char mmhoursopen=30,mmhours=0;
#define ADDR_HOURS1 0x11
#define ADDR_HOURS2 0x21
unsigned long read_hours;
unsigned long read_hours1;
unsigned long read_hours2;



#define ADDR_KILO1  0x51
#define ADDR_KILO2  0x61
unsigned long read_kilos;
unsigned long read_kilos1;
unsigned long read_kilos2;

#define ADDR_METER1 0x71
#define ADDR_METER2 0x81
unsigned long read_meter;
unsigned long read_meter1;
unsigned long read_meter2;



Byte ECU_flag = 0;
    #define Ecu_dm1 0   //0:???? 1:????
	#define Miji    1   //0:??     1:??
	#define Mijiok  2 
    
	
Byte metertime=5;//0.5s
Byte metertime1=0;//
//ʱ��
struct SHISU_STRUCT
{
 	Word 	pulse; 
	Word 	temppulse;  
	int32_t	value; 
	Byte	chishu;
}Shisu;

Word kilo_pluse = 0;
Long sum_kilometre = 0;
Long cur_meter = 0;
Byte kilometretemp = 0;
Byte shisu_flag=0;
//Byte sudu_temp = 0;
Byte meter_temp = 0;
Byte shisu_jishu = 0;
Word shisu_temp = 0;


Byte 	save_flag = 0;  //�洢��ַ 0x03
  //0  AV�л���־  0:�Զ��л� 1:�ֶ��л�
  //1  ������ʾ    0:��ʾ���߽��� 1:��ʾ��������
/*
Byte 	flag ;				//��־
	#define bp_bj 0 
	//#define work 1 //����������ʾ
	#define pag_resok 1 //ҳ�Ĵ����ض�
	#define kzq 2 //������ͨ��faile
	#define d_save 3   //�洢���
	#define av_turn 4  //AVͨ�� 0-���� 1-����
	#define av_auto 5  //0-�Զ��л� 1-�ֶ��л�
	//#define P_set 6
	#define h_resok 6//�Ĵ����ض����
	#define rx_ok 7
	*/
Word 	flag ;				//��־
	#define bp_bj 0 
	//#define work 1 //����������ʾ
	#define pag_resok 1 //ҳ�Ĵ����ض�
	#define kzq 2 //������ͨ��faile
	#define d_save 3   //�洢���
	#define av_turn 4  //AVͨ�� 0-���� 1-����
	#define av_auto 5  //0-�Զ��л� 1-�ֶ��л�
	//#define P_set 6
	#define h_resok 6//�Ĵ����ض����
	#define rx_ok 7	
	#define work 8 //����������ʾ
  #define P_set 9

	
	
	
	
 Byte 	flag1 ;				//��־
#define P_check 0

 Byte kgflag1 ;
  #define left 0
	#define right 1
	#define ygled 2//Զ��
	#define jgbp 3  //����
	#define handbp 4//��ɲ
	#define skled 5//ʾ��
	#define fr 6//����
  #define klbp 7//����

Byte kgflag2=0;
	#define yygl1 0
	//#define yygl2 1
	#define yr 1
	#define lm 2//����
	#define ys 3//��ˮ
	#define plock 4//�������� ��������Դ
	#define ilockstate 5//��������״̬
	
	/*
	Byte kgflag2;
	#define aqkg 0
	#define zlhkg 1
	#define ptfkg 2
	#define startkg 3
	*/

 Byte kgflag3;
  #define cdjc 0
	#define gt_sup 1
	#define gt_sdown 2
	#define ht_sup 3
	#define ht_sdown 4
	#define lc_sup 5
	#define lc_sdown 6
 Byte kgflag4;
	#define ylf 0
	#define gt_up 1
	#define gt_down 2
	#define ht_up 3
	#define ht_down 4
	#define lc_up 5
	#define lc_down 6
Byte kgflag5;
	#define start_out 0
	#define bpj_xzout 1//��Ƥ����ת
	#define htj_xzout 2//�������ת
	#define zst_xzout 3 //ժ��̨��ת



 Word tbflag1;  //��ʾͼ�� ��������
 Word tbflag2;  //���ý���ͼ��
 Word tbflag3; 
 Word tbflag4;  //�Ŵ�ͼ����ʾ



unsigned int Get_Data(unsigned int *data_pointer, unsigned int data1, unsigned char data_num)	
{
	unsigned int i;
	unsigned long temp;
	//���������Ƚ��ȳ�
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



unsigned int bj_flag=0;
    #define bj_yy 0  //��ѹ
	#define bj_ss 1  //��ɲ
	#define bj_sw 2  //ˮ��
	#define bj_kl 3  //����
	#define bj_yw 4  //???
	#define bj_yey1 5  //?????1
	//#define bj_yey2 6  //?????2
	//#define bj_zil 7
	//#define bj_wr 8
	//#define bj_gq 9

unsigned char bj_flag2=0;
    #define bj_yl 0  //����
	//#define bj_gwx 1 

//GPRS




uint32_t Timer0Count = 0;
uint32_t seeddatatime = 0;
uint32_t seeddatatimecount = 0;

//uint8_t charbuf[3]={'O','K','!'};
uint16_t SOS_Interval = 0; 
#define  RUNLIGHTON	   (PB12 = 0);
#define  RUNLIGHTOFF   (PB12 = 1);
#define	 RUNLIGHTSW	(PB12? (PB12 = 0):(PB12 = 1));
//#define  BUZZERON      (PF5 = 0);
//#define  BUZZEROFF     (PF5 = 1);
//#define	 GPRSPOWEROFF  (PC10 = 1);
//#define	 GPRSPOWERON   (PC10 = 0);









//#define	  GPRS_POWERROFF     (PC10 = 1)
//#define	  GPRS_POWERRON		   (PC10 = 0)

#define	  GPRS_POWERROFF     (PB3 = 1)
#define	  GPRS_POWERRON		   (PB3 = 0)


#define	  GPS_POWERROFF     (PA10 = 1)
#define	  GPS_POWERRON		   (PA10 = 0)

uint16_t  delay_ns = 0;

uint16_t  TestCounter = 0;

#endif
