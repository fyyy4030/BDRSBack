#ifndef	 _PROTO_H_
#define  _PROTO_H_
//#include "globle.h"

typedef	unsigned char	Byte;
typedef	unsigned int 	Word;
typedef	unsigned long	Long;



struct MachineFrame
{
  unsigned long   u32indicateLed;
  unsigned short  u16speed;
  unsigned char   u8oilPressure;
  unsigned char   u8waterTemperature;
  unsigned char   u8fuelVol;
  unsigned long   u32hours;
  unsigned char   u8oilTemperature;
  unsigned short  u16ptoRotateSpeed;
  unsigned char   u8airPressure;
  unsigned char   u8hydraulicFluid;
  unsigned short   u16voltage;
  unsigned short  u16engineRotate;
  unsigned long   u32milsage;
  //unsigned short  u16errorcode;
	unsigned char    errorcode[3];
};
struct MachineFrame NewMachineFrame;

  #define LEFTled 0
	#define RIGHTled 1
  #define SKled 2  //示宽
	#define YGled 3  //远光
	#define JGled 4  //近光
  #define CDled 5  //充电
  #define SWled 6  //水温
  #define YYled 7  //油压
	#define YLled 8  //油量
  #define KLled 9     //空滤
	#define HANDled 10  //手刹
	#define TXFled  11 //通信失败
	#define TXVled  12 //通信成功
  #define GZled   13 //发动机故障
	#define YS1led  14 //有水1
  #define YRled   15  //预热
  #define LMled   16  //粮满
	#define YS2led  17  //有水2





Word  gprsNoConnect = 0;
 Byte  LengthAsc[5];
struct CarData
{
   Byte    Head[2+1];
   Byte    SimID[13+1+1];
   Byte    GpsData[76+1+1];
   Byte    OilPressur[5+1+1];
   Byte    WaterTemperature[5+1+1];
   Byte    FuelVolme[5+1+1];
   Byte    BatteryVoltage[5+1+1];
   Byte    RotationHour[8+1+1];
   Byte    RotationSpeed[6+1+1];
   Byte    MachineState[6+1+1];
	 Byte    TroubleState[6+1+1];
   Byte    VerifyCode[2+1];
   Byte    End[2];
};
 struct CarData NewCarData;
 struct CarData * CarDataP= &NewCarData;



struct PowerOffComm
{
   Byte   Head[2];
	 Byte   SN[12];
	 Byte   CommID[2];
   Byte   SimID[11];
	 Byte   Comm[3];
	 Byte   End[2];
};
struct PowerOffComm   NewPowerOffComm,RpPowerOffComm,RpPowerOffwat;
struct PowerOffComm   NewPowerOnComm,RpPowerOnComm;
struct PowerOffComm   NewSOS,NewBoundComm,NewUnBoundComm,NewHaltComm,NewUnLimitComm,NewInquireComm;

unsigned char IsReceivePowOff;
unsigned char IsReceivePowOn ;
unsigned char IsSOS = 0,IsBoundComm = 0,IsUnBoundComm = 0,IsHaltComm = 0,IsUnLimitComm = 0,IsInquireComm = 0;

struct LimitParameterComm
{
   Byte   Head[2];
	 Byte   SN[12];
	 Byte   CommID[2];
   Byte   SimID[11];
	 Byte   Comm[3];
	 Byte   ZsPara[4];
	 Byte   NjPara[3];
	 Byte   End[2];
};
struct LimitParameterComm  NewLimitParameterComm;
unsigned char IsLimitParameterComm ;

struct ControlStateRp
{
   Byte   Head[2];
	 Byte   SN[12];
	 Byte   CommID[2];
   Byte   SimID[11];
	 Byte   Comm[3];
	 Byte   StatePara[16];
	 Byte   End[2];
};
struct ControlStateRp  NewControlStateRp;


struct JMStateRp
{
   Byte   Head[2];
	 Byte   SN[12];
	 Byte   CommID[2];
   Byte   SimID[11];
	 Byte   Comm[4];
	 Byte   StatePara[5];
	 Byte   End[2];
};
struct  JMStateRp   JMStateRpOpen,JMStateRpClose;
unsigned char IsJMStateRpOpen,IsJMStateRpClose  ;


#endif
