#ifndef __GPRSPROTO__H
#define __GPRSPROTO_H

//unsigned char dxbjhm[2][29]={{0X36,0X38,0X33,0X31,0X37,0X39,0X31,0X33,0X30,0X30,0X35,0X31,0X46,0X37}};//8613973100157F  683179130051F7
unsigned char dxbjhm[2][29]={{0X36,0X38,0X35,0X31,0X30,0X38,0X38,0X33,0X37,0X32,0X37,0X36,0X46,0X39}};//8615803827679F  685108837276F9
unsigned char dxbjhm1_lenght,dxbjhmflag,dxbjhmflag0;

struct ID_BH
{
	unsigned short int cjbs;  //3λ ���ұ�ʶ
	unsigned char zdlx; //2λ �ն�����
 	unsigned short int scph;  //4λ �������� YYMM
	unsigned short int wybh;  //4λ Ψһ���
	unsigned short int bbh;   //3λ �汾��
	
}IDBH={0x682,0xB9,0x1705,0x0001,0x001}; //0x2aa=682





//ʹ�ò���
struct ZDParameter_T
{ 
	 //��λ��ѯ����0x14
	 unsigned char 	IntervalTimer;
	 unsigned char 	TraceCount;

   unsigned char ZDID[8];    //�ն�ID
	 unsigned char 	LoginCount;
	 unsigned short int   LoginTimer; //270s �ط�3�� ��ȴ�10���Ӹ�λ����
	 unsigned char 	LoginReply;
	 unsigned short int   zdliushuinum; //������ˮ��1~10000
	 unsigned short int  zhidonghbwzsc;  //�Զ��ر�λ��ʱ��
	 //uint16_t workStateTimer;

}ZDParameter;
  

//�������·�����֡
struct ZDCommand_T
{
     unsigned char start[2];	//��ʼ��
	 unsigned char command;		//������
	 unsigned char Liushui[4];  //��ˮ��
	 unsigned char ZID[8];		//�ն� ID ��
	 unsigned char length[2];   //���ݳ���
	 unsigned char data[500];   //��������
	 unsigned char check;       //У����
	 unsigned char finish;		//������
};
 struct ZDCommand_T    ZDCommand;
//�ն��ϴ�Ӧ��֡
struct ZDReply_T
{
     unsigned char start[2];	//��ʼ��
	 unsigned char command;		//������
	 unsigned char Liushui[4];  //��ˮ��
	 unsigned char ZID[8];		//�ն� ID ��
	 unsigned char length[2];   //���ݳ���
	 //unsigned char data[500];   //��������
	 unsigned char check;       //У����
	 unsigned char finish;		//������
};
 struct ZDReply_T    ZDReply;





/*
����$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
�ֶ�0��$GPRMC�����ID�����������ΪRecommended Minimum Specific GPS/TRANSIT Data��RMC���Ƽ���С��λ��Ϣ
�ֶ�1��UTCʱ�䣬hhmmss.sss��ʽ
�ֶ�2��״̬��A=��λ��V=δ��λ
�ֶ�3��γ��ddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
�ֶ�4��γ��N����γ����S����γ��
�ֶ�5������dddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��
�ֶ�6������E����������W��������
�ֶ�7���ٶȣ��ڣ�Knots
�ֶ�8����λ�ǣ���
�ֶ�9��UTC���ڣ�DDMMYY��ʽ
�ֶ�10����ƫ�ǣ���000 - 180���ȣ�ǰ��λ��������0��
�ֶ�11����ƫ�Ƿ���E=��W=��
�ֶ�16��У��ֵ
*/

struct GPS_T
{
     unsigned char  year;      //λ��/��				
     unsigned char  Month;		/* ���� 120598 ddmmyy */
	 unsigned char  Day;
	 unsigned char  Hour;		/* UTC ʱ�䡣 hhmmss.sss */
	 unsigned char  Min;		/* �� */
	 unsigned char  Sec;		/* �� */
	 //uint16_t mSec;			/* ���� */
	 
	 unsigned char   WD[4];		//γ��
	 //uint16_t WeiDu_Du;			//γ��
	 //uint16_t WeiDu_Fen;			//γ��

     unsigned char   JD[4];		//����
	 //uint16_t JingDu_Du;			//γ��
	 //uint16_t JingDu_Fen;			//γ��

	 unsigned char   speed;		//�ٶ�
	 unsigned char   hx;	    //����
	 unsigned char   state1;    //״̬ 1
	 unsigned char   state2;	//״̬ 2
	 unsigned char   state3;	//״̬ 3
	 unsigned char   state4;	//״̬ 4
};


#define  EAST  0x01
#define  WEST  0x02
#define  SOUTH 0x04
#define  NORTH 0x08


#define  WS 0x00   //(0x00 >> 6)
#define  WN 0x40   //(0x01 >> 6)
#define  ES 0x80   //(0x02 >> 6)
#define  EN 0xc0   //(0x03 >> 6)

//��λ��ѯӦ��0x11,�Զ���ʱ�ϱ�0x18
struct TerminalReply_0x110x18
{
	 struct GPS_T GPS;		           //λ����Ϣ
   unsigned char   fuel;		       //�ͺĲɼ�
	 unsigned char   GPS_sum[4];	   //GPS �������
	 unsigned char   ACC_ON_sum[4];	   //ACC ON ���ۼ�ʱ��
	 unsigned char   dh[2];            //���ε��������
	 unsigned char   ACC_ON_sum_c[4];  //���� ACC ON �ۼ�ʱ��
	 unsigned char   JZID[20];	       //��վ ID

}TerminalReply0x110x18;

struct mem_param
{
	unsigned int GPSvoidtimem ;//GPS���Ϸ���         4
	/*******0x110x18*****/
	unsigned char   GPS_sum[4];//GPS �������        4
	unsigned char   ACC_ON_sum[4];//ACC ON ���ۼ�ʱ��   4  ������
	/******������***/
	unsigned char   GPSztdsj[4];  //GPS��ͨ��ʱ��
	unsigned char   kaigainum;	       //���Ǵ���
	unsigned char   baGPSantnum;	       // ��GPS���ߴ���
	unsigned char   baSIMnum;	       // ��SIM������
	/****************/
	unsigned char   simkaghcs ;//SIM����������
	unsigned int    flagchange ;//�����ĸ���״̬  0���Ǵ�������  1��GPS���ߴ�������  2��SIM���������� 
};
union memparam_set
 {
	struct mem_param	memparam;
	unsigned char mem_paramdata[24];
 }memparamset;

//��������0x12
struct TerminalReply_0x12
{
	unsigned char   caozuoma;		 //������
	unsigned char   duxieflag;	  //��д��־
	unsigned char * sblx;             		 //����ֵ
}TerminalReply0x12;

//����������
struct Parameter_setitem
{
	unsigned char sbyhm0x00[8];   //�豸�û���
	unsigned char hbsc0x03[2];		//�Զ��ر�λ����Ϣʱ��
	unsigned char fwqIP0x04[6];		//������IP��ַ���˿ں�
	/*********///�Ǳ������в���
	unsigned char sjfwqIP0x04[6];		//0x41�����֣�����������IP��ַ���˿ں�
	/*********/
	unsigned char BaudRate0x07;		    //250 Ĭ��
  unsigned char IMSI0x11[20];		    // ֻ��  SIMΨһʶ����
	
	/******/
  unsigned char ycsj0x15;	    //Զ������ 0�ر� 1��ʼ  0
  unsigned char sdms0x16;	    //ʡ��ģʽ1���� 0�ر�   1
	
	unsigned char xtbkz0x1b;	    //���������� 1���� 0�ر�
	unsigned char AAbjsjset0x1c;	    //0XAA����ʱ������
	unsigned char zdqyz0x1D;	    //����ԴǷѹֵ  11.5v 22.5v
  unsigned char bdsbzq0x1E;	    //���ñ��õ�Դ���ϱ�����  24Сʱ
	unsigned char PLCsrk0x20;	    //PLC��������� 0�ߵ�ƽ 1�͵�ƽ
  unsigned char wzhbcl0x41;	    //λ�û㱨����   0 ��ʱ�㱨 1����㱨 2��ʱ�Ͷ���
	unsigned char dssbsj0x42;	    //��ʻ�ж�ʱ�ϱ�ʱ�� 30s
  unsigned char djsbjl0x43;	    //��ʻ�ж������ϱ����� 200m=20  �ֱ���10m

	unsigned  char sbbh0x17[50];	  //�豸���      "" <50
	unsigned  char SIMkh0x18[20];	//���ն�SIM���� "" <20
  unsigned  char hjzxhm0x19[20];	//�������ĺ���  "" <20
	unsigned  char sblx0x22[20];	  //�豸���� ""<=20
  unsigned  char dxbjhm0x09[29];	//""  ���ű�������  106575531157170  �ݶ�29λ
	 /*******///���ñ�����  13  ����205-13=192 =48*4
	unsigned  char APN0x05[5];  //""  <=64  CMNET ֻ��
	 unsigned char UDPTCP0x06;		    //1 TCPֻ��
	 unsigned char ckgn0x0e;		    //0 ���� ֻ��
	 unsigned char GSMxhqd0x12;		    // ֻ�� GSM�ź�ǿ��
	 unsigned char GPSwxs0x13;		    // ֻ��  GPS���ǿ���
	 unsigned char GPScslc0x14[4];	    //��ʼ�����
   
};

union Parameter_set
 {
	struct Parameter_setitem	Parametersetitem;
	unsigned char Parameter_setitemdata[205];
 }Parameterset;

//can���������·��������ϴ�0x13 �ظ�2.b�ն�Ӧ��
//2.b�ն�Ӧ��
struct TerminalReply_0x1a
{
   unsigned char   command;          //������
	 unsigned char   success;	       //�ɹ���־
// 	 struct GPS_T GPS;		           //λ����Ϣ
//    unsigned char   fuel;		       //�ͺĲɼ�
// 	 unsigned char   GPS_sum[4];	   //GPS �������
// 	 unsigned char   ACC_ON_sum[4];	   //ACC ON ���ۼ�ʱ��
// 	 unsigned char   dh[2];            //���ε��������
// 	 unsigned char   ACC_ON_sum_c[4];  //���� ACC ON �ۼ�ʱ��
// 	 unsigned char   JZID[20];	       //��վ ID
	struct TerminalReply_0x110x18 TerminalReply0x110x18;

}TerminalReply0x1a;

//����Ӧ������ 0x14


//�ն��ϴ�������Ϣ0x17 ������Ӧ��0x14  15s��Ӧ���ط�3��
struct TerminalReply_0x17
{
	 unsigned char   baojingflag;	      //������־
// 	 struct GPS_T GPS;		           //λ����Ϣ
//    unsigned char   fuel;		       //�ͺĲɼ�
// 	 unsigned char   GPS_sum[4];	   //GPS �������
// 	 unsigned char   ACC_ON_sum[4];	   //ACC ON ���ۼ�ʱ��
// 	 unsigned char   dh[2];            //���ε��������
// 	 unsigned char   ACC_ON_sum_c[4];  //���� ACC ON �ۼ�ʱ��
// 	 unsigned char   JZID[20];	       //��վ ID
	struct TerminalReply_0x110x18 TerminalReply0x110x18;

}TerminalReply0x17;

//�������ϱ�
struct TerminalReply_0x28
{
	 unsigned char   zhudianvol[2];	  //�����ѹ
	 unsigned char   beidianvol;	   //�����ѹ
   unsigned char   neibuwendu;		 //�ն��ڲ��¶�
	 unsigned char   zhudianXMJG;	   //�������߼��
	 unsigned char   beidianXMJG;	   //�������߼��
	 unsigned char   ACC_ON_sum_c[4];  //���� ACC ON �ۼ�ʱ��
	 unsigned char   GPSztdsj[4];  //GPS��ͨ��ʱ��
	 unsigned char   kaigainum;	       //���Ǵ���
	 unsigned char   baGPSantnum;	       // ��GPS���ߴ���
	 unsigned char   baSIMnum;	       // ��SIM������
	 unsigned char   GSMxinhao;	       // GSM�ź�ǿ��

}TerminalReply0x28;


 

//0x19��¼���ķ�����
struct LoginCen_T
{
   unsigned char sbyhm[8];    //�豸�û���
	 unsigned char GPS_gzts;    //GPS ��������
	 unsigned char xhr[2];      //GSM/GPRS �ź�������Сʱ
	 unsigned char hbsc[2];		//�Զ��ر�λ����Ϣʱ��
	 char *	 bjhm;	            //���ű�������
   char * sbbh;               //�豸���
   char * SIMh;               //���ն��� SIM ����
   char * sblx;               //�豸����
};

 struct LoginCen_T  LoginCen;






// struct Capacity_T
// {
// 	unsigned char command;
// 	unsigned char len;
// 	unsigned char data[3];
// };
// struct GZ_T
// {
// 	unsigned char command;
// 	unsigned char len;
// 	unsigned char count;
// 	unsigned char data[10][3];
// };





// #define GuoManDeng    0x80  //������
// #define YeWeiDeng     0x40  //Һλ��
// #define QiYaDeng      0x20  //��ѹ������
// #define YouLiangDeng  0x10	//����������
// #define YouYaDeng     0x08	//��ѹ������
// #define ShuiWenDeng   0x04	//ˮ�±�����
// #define YouShuiDeng   0x02  //��ˮ�����
// #define GuoZhangDeng  0x01	//���ϵ�

// //#define GuoManDeng    0x80  //(Ԥ��)
// #define ZhucheDeng     0x40  //פ���ƶ���
// #define ChongDianDeng  0x20  //����
// #define YouLvDeng      0x10	//���˵�
// #define YuReDeng       0x08	//Ԥ�ȵ�
// #define KongLvDeng     0x04	//���˵�
// #define jiaoLongDeng   0x02  //���Ž�����
// #define QiShaDeng      0x01	//��ɲ��
// extern uint8_t deng[3]; 



#endif 








