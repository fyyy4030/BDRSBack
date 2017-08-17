#ifndef _SMS_H_
#define _SMS_H_
//                                               PDU格式       列举未读"+CMGL:"  删除所有    读短信
// const char *at_SMScom[]={"AT+CNMI=2,2,0,0,0\r","AT+CMGF=0\r","AT+CMGL=0\r", "AT+CMGD=1,2\r","AT+CMGR=", "AT+CMGS="}; 
// const char *at_SMScom_rep[]={"OK",               "OK",          "OK",                                        "<","+CMGS:"};

//const char *at_SMScom[4]={ "AT+CSCLK=2\r","AT+CMGR=","AT+CMGD=1,2\r", "AT+CMGS="}; 
const char *at_SMScom[4]={ "AT+CSCLK=2\r","AT+CMGR=","AT+CMGDA=6\r", "AT+CMGS="}; 
//const char *at_SMScom_rep[5]={"OK", "OK",   "OK",            "<","+CMGS:"};
const char *at_SMScom_rep[5]={"OK", "OK",   "OK",            "<","OK"};
unsigned char AtsmsCommSend;





//发送pdu格式
struct SMS_send
{
   unsigned char SCA;	//短信中心  00
	 unsigned char PDUtype;		//发送参数 11  51有协议头
	 unsigned char MR;  //消息基准值 00
	 unsigned char numlenght;  //目标号码长度 0D
	 unsigned char numtype;  //目标号码格式   91  A1
	 unsigned char mobile_number[29];		//手机号
	 unsigned char PID;   //协议标识  00
	 unsigned char DCS;   //数据编码方案 00-7bit  04-8bit 08-ucs2 
	 unsigned char vp;   //有效期 00  5min  01  10min
	 unsigned char UDL;   //信息长度
	 unsigned char UDHI[6];   //协议头
	 unsigned char data[140];   //数据内容 140-6
	 
}SMSsend;
 
//发送pdu格式
struct SMS_rec
{
   unsigned char SCA;	//短信中心  00
	 unsigned char PDUtype;		//发送参数 11  51有协议头
	 unsigned char MR;  //消息基准值 00
	 unsigned char numlenght;  //目标号码长度 0D
	 unsigned char numtype;  //目标号码格式   91  A1
	 unsigned char mobile_number[29];		//手机号
	 unsigned char PID;   //协议标识  00
	 unsigned char DCS;   //数据编码方案 00-7bit  04-8bit 08-ucs2 
	 unsigned char vp[7];   //服务中心时间戳 年月日 时分秒 时区
	 unsigned char UDL;   //信息长度
	 unsigned char UDHI[6];   //协议头
	 unsigned char data[140];   //数据内容 140-6
	 
}SMSrec;




#endif
