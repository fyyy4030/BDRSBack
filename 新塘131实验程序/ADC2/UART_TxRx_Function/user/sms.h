#ifndef _SMS_H_
#define _SMS_H_
//                                               PDU��ʽ       �о�δ��"+CMGL:"  ɾ������    ������
// const char *at_SMScom[]={"AT+CNMI=2,2,0,0,0\r","AT+CMGF=0\r","AT+CMGL=0\r", "AT+CMGD=1,2\r","AT+CMGR=", "AT+CMGS="}; 
// const char *at_SMScom_rep[]={"OK",               "OK",          "OK",                                        "<","+CMGS:"};

//const char *at_SMScom[4]={ "AT+CSCLK=2\r","AT+CMGR=","AT+CMGD=1,2\r", "AT+CMGS="}; 
const char *at_SMScom[4]={ "AT+CSCLK=2\r","AT+CMGR=","AT+CMGDA=6\r", "AT+CMGS="}; 
//const char *at_SMScom_rep[5]={"OK", "OK",   "OK",            "<","+CMGS:"};
const char *at_SMScom_rep[5]={"OK", "OK",   "OK",            "<","OK"};
unsigned char AtsmsCommSend;





//����pdu��ʽ
struct SMS_send
{
   unsigned char SCA;	//��������  00
	 unsigned char PDUtype;		//���Ͳ��� 11  51��Э��ͷ
	 unsigned char MR;  //��Ϣ��׼ֵ 00
	 unsigned char numlenght;  //Ŀ����볤�� 0D
	 unsigned char numtype;  //Ŀ������ʽ   91  A1
	 unsigned char mobile_number[29];		//�ֻ���
	 unsigned char PID;   //Э���ʶ  00
	 unsigned char DCS;   //���ݱ��뷽�� 00-7bit  04-8bit 08-ucs2 
	 unsigned char vp;   //��Ч�� 00  5min  01  10min
	 unsigned char UDL;   //��Ϣ����
	 unsigned char UDHI[6];   //Э��ͷ
	 unsigned char data[140];   //�������� 140-6
	 
}SMSsend;
 
//����pdu��ʽ
struct SMS_rec
{
   unsigned char SCA;	//��������  00
	 unsigned char PDUtype;		//���Ͳ��� 11  51��Э��ͷ
	 unsigned char MR;  //��Ϣ��׼ֵ 00
	 unsigned char numlenght;  //Ŀ����볤�� 0D
	 unsigned char numtype;  //Ŀ������ʽ   91  A1
	 unsigned char mobile_number[29];		//�ֻ���
	 unsigned char PID;   //Э���ʶ  00
	 unsigned char DCS;   //���ݱ��뷽�� 00-7bit  04-8bit 08-ucs2 
	 unsigned char vp[7];   //��������ʱ��� ������ ʱ���� ʱ��
	 unsigned char UDL;   //��Ϣ����
	 unsigned char UDHI[6];   //Э��ͷ
	 unsigned char data[140];   //�������� 140-6
	 
}SMSrec;




#endif
