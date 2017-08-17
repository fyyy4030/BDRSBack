#ifndef	 _CANPROTO_H_
#define  _CANPROTO_H_
//#include "globle.h"

//n200000001_can_v1.4
unsigned char  Receiveid_0080,Receiveid_0180,Receiveid_0280,Receiveid_0380,Receiveid_0780,Receiveid_0880,Receiveid_0980,Receiveid_0A80,Receiveid_0B80;
unsigned char  Receiveid_2180,Receiveid_2280,Receiveid_2080,Receiveid_2181,Receiveid_2281,Receiveid_2081,Receiveid_0C80,Receiveid_0D80;
unsigned char  Receiveid_0080t,Receiveid_0180t,Receiveid_0280t,Receiveid_0380t,Receiveid_0780t,Receiveid_0880t,Receiveid_0980t,Receiveid_0A80t,Receiveid_0B80t;
unsigned char  Receiveid_2180t,Receiveid_2280t,Receiveid_2080t,Receiveid_2181t,Receiveid_2281t,Receiveid_2081t,Receiveid_0C80t,Receiveid_0D80t;
unsigned char receivedanbaot;

// struct CANID_0080
// {
// 	unsigned long   CANID;
//   unsigned char   canid[4];
// 	unsigned char   leijilicheng[4];//1-4
// 	unsigned char   dangqianlicheng[2];//5-6
//   unsigned char   chesu[2];//7-8
// }CANID0080={0X18FF0080,0X18,0XFF,0X00,0X80 } ;
struct CANID_0080
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];//
	
}CANID0080={0X18FF0080,0X18,0XFF,0X00,0X80 } ;

// struct CANID_0180
// {
// 	unsigned long   CANID;
//   unsigned char   canid[4];
// 	unsigned char   baojingflag;//1
// 	unsigned char   dianpingvol[2];//2-3
//   unsigned char   jiyouyali;//4
// 	unsigned char  cheneiwendu;//5
// 	unsigned char   zhonglei;//6
// 	unsigned char   ganzaodu;//7  
// 	unsigned char   wuyong;//无用8
// }CANID0180={0X18FF0180,0X18,0XFF,0X01,0X80 } ;
struct CANID_0180
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];//
}CANID0180={0X18FF0180,0X18,0XFF,0X01,0X80 } ;

struct CANID_0280 //转速
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   zhuansu[8];//4-5
	
}CANID0280={0X18FF0280,0X18,0XFF,0X02,0X80 } ;


// struct CANID_0280 //转速
// {
// 	unsigned long   CANID;
//   unsigned char   canid[4];
// 	unsigned char   data[8];//
// 	
// }CANID0280={0X18FF0280,0X18,0XFF,0X02,0X80 } ;

// struct CANID_0380
// {
// 	unsigned long   CANID;
//   unsigned char   canid[4];
// 	unsigned char   shuiwen;//1
// 	unsigned char   zuoyeshichang[3];//2-4
// 	unsigned char   ranyouzongxiaohao[4];//5-8
// }CANID0380={0X18FF0380,0X18,0XFF,0X03,0X80 } ;

struct CANID_0380
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];//
}CANID0380={0X18FF0380,0X18,0XFF,0X03,0X80 } ;

struct CANID_0480//单个故障
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   danguzhang[8];//1-8
	
}CANID0480={0X18FF0480,0X18,0XFF,0X04,0X80 } ;
struct CANID_0580//多故障通告
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   duoguzhangtonggao[8];//1-8
	
}CANID0580={0X18FF0580,0X18,0XFF,0X05,0X80 } ;

struct CANID_0680//多故障包
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   duoguzhangbao[8];//4-5
	
}CANID0680={0X18FF0680,0X18,0XFF,0X06,0X80 } ;

// 	struct CANID_0780
// 	{
// 	unsigned long   CANID;
// 	unsigned char   canid[4];
// 	unsigned char   gunlunzhuansu[3];//1-3
// 	unsigned char   futuozhuansu[3];//4-6
// 	unsigned char   wuyong[2];//无用7-8
// 	}CANID0780={0X18FF0780,0X18,0XFF,0X07,0X80 } ;
 struct CANID_0780
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID0780={0X18FF0780,0X18,0XFF,0X07,0X80 } ;
// struct CANID_0880
// {
// 	unsigned long   CANID;
//   unsigned char   canid[4];
// 	unsigned char   shengyunzhuansu[3];//1-3
// 	unsigned char   gechagaodu[3];//4-6
// 	unsigned char   wuyong[2];//无用7-8
// }CANID0880={0X18FF0880,0X18,0XFF,0X08,0X80 } ;
struct CANID_0880
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID0880={0X18FF0880,0X18,0XFF,0X08,0X80 } ;

struct CANID_0980
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID0980={0X18FF0980,0X18,0XFF,0X09,0X80 } ;

struct CANID_0A80
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID0A80={0X18FF0A80,0X18,0XFF,0X0A,0X80 } ;

struct CANID_0B80
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID0B80={0X18FF0B80,0X18,0XFF,0X0B,0X80 } ;

struct CANID_2180
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID2180={0X18FF2180,0X18,0XFF,0X21,0X80 } ;

struct CANID_2280
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID2280={0X18FF2280,0X18,0XFF,0X22,0X80 } ;

struct CANID_2080
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID2080={0X18FF2080,0X18,0XFF,0X20,0X80 } ;

struct CANID_2181
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID2181={0X18FF2181,0X18,0XFF,0X21,0X81 } ;

struct CANID_2281
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID2281={0X18FF2281,0X18,0XFF,0X22,0X81 } ;

struct CANID_2081
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID2081={0X18FF2081,0X18,0XFF,0X20,0X81 } ;

struct CANID_0C80
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID0C80={0X18FF0C80,0X18,0XFF,0X0C,0X80 } ;

struct CANID_0D80
{
	unsigned long   CANID;
  unsigned char   canid[4];
	unsigned char   data[8];
}CANID0D80={0X18FF0D80,0X18,0XFF,0X0D,0X80 } ;

#endif
