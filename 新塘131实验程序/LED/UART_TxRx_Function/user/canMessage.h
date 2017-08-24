
#ifndef		_CANMESSAGE_H_
#define		_CANMESSAGE_H_


#include <stdio.h>
#include "NUC131.h"
#include "globle.h"
#include "lockdecode.h"
#include "dataflash.h"

//unsigned char can_transdata[8]={1,2,3,4,5,6,7,8};
//unsigned char can_receivedata[8]={8,7,6,5,4,3,2,1};

//unsigned char CAN_IDbuf[4]={0,0,0,0};
unsigned char CAN_Rxbuf[10][8];
unsigned char CanReceiveEndFlag;
unsigned char DM1_PACK[20][7];
//unsigned char DM1_Total;
unsigned long spn0;
unsigned char FMI0;
unsigned long spn[10];
unsigned char FMI[10];
unsigned char DM1OK=0;
unsigned char SPNch=0,NETSPNch=0;
unsigned char SPN_Displaytime=50;
unsigned int wdnum=0,spnnum=0;
volatile unsigned char *pointdm1=DM1_PACK[1],*pointdm2,*pointdm3;
/*---------------------------------------------------------------------------------------------------------*/
/* Define global variables and constants                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
/* Declare a CAN message structure */
STR_CANMSG_T rrMsg;
STR_CANMSG_T tMsg;
// 解绑参数
unsigned int constantcode=0;  //1234//0x4d2
unsigned long GPSID= 160502001;	//0x9 91 10 F1
//锁车命令
 #define unlimit     0x00
 #define limitstop   0x01
 #define limit       0x10
 unsigned char limitstate;	
 //锁车参数
 unsigned int engzs=0;
 unsigned char engniuju=0;

 unsigned char checkkey[4]; //不与GPSID组合校验码
 unsigned char lockpassword[2];
 unsigned char unboundpassword[2];
 //发动机状态参数
 #define passivelockstatemsk  3<<2  //第4字节，被动锁车状态
 
 #define initlockstatemsk     3<<6  //第4字节，主动锁车状态
 #define initunlock     0<<6  //第4字节，主动未锁车
 #define initlimit     2<<6  //第4字节，主动未锁车
 #define initstop     1<<6  //第4字节，主动未锁车


 #define checkstatemsk         3<<4  //第5字节，校验状态
  #define checkunpass         0<<4  //第5字节，校验未通过
	  #define checkpass         1<<4  //第5字节，校验通过
			  #define uncheck        3<<4  //第5字节，未校验

 #define activationstatemsk    3<<6  //第5字节，激活状态
  #define unactivationstate    0<<6  //第5字节，激活状态
 #define activationstate    1<<6  //第5字节，激活状态

 #define requestsign         3<<0  //第7字节，校验状态
  //#define requestsignunreceive        0<<0  //第7字节，校验状态
	 // #define requestsignreceive        1<<0  //第7字节，校验状态
uint32_t boundstate=0;//0未绑定状态  1未绑定状态
//unsigned char  boundstate=0;//0未绑定状态  1未绑定状态
//#define boundstate 0 //0未绑定状态  1未绑定状态
unsigned char  receivecm=0;
#define boundcommand   0  //绑定命令
#define unboundcommand 1 //解绑命令
#define lockcommand    2 //限制命令
#define unlockcommand  3 //不限制命令
#define stopcommand    4 //停机命令
#define inquirecommand 5 //查询命令


unsigned int  sendcm=0;
#define illebound 0 //非法绑定
#define boundok   1 //绑定成功
#define unboundok 2 //解绑成功
#define limitok   3 //限制成功
#define stopok    4 //停机成功
#define unlimitok 5 //不限制成功
#define passivelock 6 //被动锁车
#define boundwait   7 //绑定等待
#define unboundwait 8 //解绑等待
#define limitwait   9 //限制等待
#define stopwait    10 //停机成功
#define unlimitwait 11 //不限制等待
#define ycmachine   12 //玉柴
#define wcmachine   13 //潍柴
#define qcmachine   14 //全柴
#define setmachine  15 //机型设置




unsigned char step=0,initsend=0,stepbuff=0,timeout=0,opentimeout=0,receivestate=0;



/*
void DM1_clear(void)
{
  unsigned char j,k;
  for(k = 0; k < 10; k++)
  {
	for(j = 0; j < 7; j++)
	{
      DM1_PACK[k][j]=0;
	}
  }
	
}
*/

void  SPN_Display(void)
{ unsigned char i;
 if(ECU_flag & BIT(Ecu_dm1))//显示多包故障
 { 
	 wdnum=DM1_PACK[0][0]|(DM1_PACK[0][1]<<8);
	if(wdnum>=6)
	{
	  spnnum=(wdnum-2)/4;
		 SPNch++;
	   if(  SPNch<= spnnum)
		 {
		  pointdm2 =pointdm1+1+(SPNch-1)*4;
			spn[SPNch] = *(pointdm2+3) ;		 
			spn[SPNch] >>= 5;
			spn[SPNch] <<= 8; 
			spn[SPNch] |= *(pointdm2+2) ;
      spn[SPNch] <<= 8; 	
      spn[SPNch] |= *(pointdm2+1) ;			 
			FMI[SPNch]=*(pointdm2+3)&0x1f;   
	    spn0 =spn[SPNch];
		 }
		 if( SPNch== spnnum)
			 SPNch=0; 
	 
 }
 }
 else
 {//显示单包
   spn0 = err;
	 SPNch=0;
   //FMI0 = FMI;
	 if(spn0>0)
	  spnnum=1;
	 else
	  spnnum=0;
 }
 
 if(spn0)
  {
	 tbflag1 |= BIT(14);
  }
  else
  {
	 tbflag1 &=~ BIT(14);
  }
}


/*---------------------------------------------------------------------------*/
/*  Show Message Function                                                    */
/*---------------------------------------------------------------------------*/
//void CAN_ShowMsg(STR_CANMSG_T* Msg)
//{
//    uint8_t i;
//
//    /* Show the message information */
//	if(Msg->Id == 0x18feca00)
//    GprsSendComm("Read ID=0x%X, Type=%s, DLC=%d, Data=", Msg->Id, Msg->IdType ? "EXT" : "STD", Msg->DLC);
//    for(i = 0; i < Msg->DLC; i++)
//        GprsSendComm("%X,", Msg->Data[i]);
////    GprsSendComm("\n\n");
//}


/*----------------------------------------------------------------------------*/
/*  Send Tx Msg by Normal Mode Function (With Message RAM)                    */
/*----------------------------------------------------------------------------*/
unsigned char Request_Tx(CAN_T *tCAN) //发送请求帧
{
    //STR_CANMSG_T tMsg;
    /* Send a 29-bit Extended Identifier message */
    tMsg.FrameType = CAN_DATA_FRAME;
    tMsg.IdType   = CAN_EXT_ID;
    tMsg.Id       = 0x18EA0021;
    tMsg.DLC      = 8;
    tMsg.Data[0]  = 0x01;
    tMsg.Data[1]  = 0xFD;
    tMsg.Data[2]  = 0x00;
	tMsg.Data[3]  = 0x00;
	tMsg.Data[4]  = 0x00;
	tMsg.Data[5]  = 0x00;
	tMsg.Data[6]  = 0x00;
	tMsg.Data[7]  = 0x00;
	

    if(CAN_Transmit(tCAN, MSG(10), &tMsg) == FALSE) 
     return FALSE;
     else
      return TRUE;
 /*  CLK_SysTickDelay(1000000); //Delay one second to wait INT

    if(tCAN->ERR == 0)
        printf("\nCheck the receive host received data\n");
    else
        printf("\nCAN bus error!\n");
	*/
}

unsigned char key_Tx(CAN_T *tCAN) //	发送校验帧
{
    //STR_CANMSG_T tMsg;
    /* Send a 29-bit Extended Identifier message */
    tMsg.FrameType = CAN_DATA_FRAME;
    tMsg.IdType   = CAN_EXT_ID;
    tMsg.Id       = 0x18FE02FB;
    tMsg.DLC      = 8;
    tMsg.Data[0]  = checkkey[0];
    tMsg.Data[1]  = GPSID;
    tMsg.Data[2]  = checkkey[1];
	tMsg.Data[3]  = GPSID>>8;
	tMsg.Data[4]  = checkkey[2];
	tMsg.Data[5]  = GPSID>>16;
	tMsg.Data[6]  = checkkey[3];
	tMsg.Data[7]  = GPSID>>24;
	

    if(CAN_Transmit(tCAN, MSG(10), &tMsg) == FALSE)
       return FALSE;
   else
      return TRUE;

 /*  CLK_SysTickDelay(1000000); //Delay one second to wait INT

    if(tCAN->ERR == 0)
        printf("\nCheck the receive host received data\n");
    else
        printf("\nCAN bus error!\n");
	*/
}

  unsigned char Bound_Tx(CAN_T *tCAN) //	发送绑定命令
{
    //STR_CANMSG_T tMsg;
    /* Send a 29-bit Extended Identifier message */
    tMsg.FrameType = CAN_DATA_FRAME;
    tMsg.IdType   = CAN_EXT_ID;
    tMsg.Id       = 0x18FE01FB;
    tMsg.DLC      = 8;
    tMsg.Data[0]  = 0x00;
    tMsg.Data[1]  = 0x00;
    tMsg.Data[2]  = GPSID;
	tMsg.Data[3]  = GPSID>>8;
	tMsg.Data[4]  = GPSID>>16;
	tMsg.Data[5]  = GPSID>>24;
	tMsg.Data[6]  = constantcode;
	tMsg.Data[7]  = constantcode>>8;
	
    if(CAN_Transmit(tCAN, MSG(10), &tMsg) == FALSE)
      return FALSE;
    else
      return TRUE;
 /*  CLK_SysTickDelay(1000000); //Delay one second to wait INT

    if(tCAN->ERR == 0)
        printf("\nCheck the receive host received data\n");
    else
        printf("\nCAN bus error!\n");
	*/
}

   unsigned char UNBound_Tx(CAN_T *tCAN) //	发送解绑命令
{
    //STR_CANMSG_T tMsg;
    /* Send a 29-bit Extended Identifier message */
    tMsg.FrameType = CAN_DATA_FRAME;
    tMsg.IdType   = CAN_EXT_ID;
    tMsg.Id       = 0x18FE01FB;
    tMsg.DLC      = 8;
    tMsg.Data[0]  = unboundpassword[0];
    tMsg.Data[1]  = unboundpassword[1];
    tMsg.Data[2]  = 0x00;
	tMsg.Data[3]  = 0x00;
	tMsg.Data[4]  = 0x00;
	tMsg.Data[5]  = 0x00;
	tMsg.Data[6]  = 0x00;
	tMsg.Data[7]  = 0x00;
	

    if(CAN_Transmit(tCAN, MSG(10), &tMsg) == FALSE)
       return FALSE;
    else
      return TRUE;

 /*  CLK_SysTickDelay(1000000); //Delay one second to wait INT

    if(tCAN->ERR == 0)
        printf("\nCheck the receive host received data\n");
    else
        printf("\nCAN bus error!\n");
	*/
}


   unsigned char InitiativeLock_Tx(CAN_T *tCAN) //	发送主动锁车命令
 {
    //STR_CANMSG_T tMsg;
    /* Send a 29-bit Extended Identifier message */
	  
	
	 if(receivecm&BIT(unlockcommand))
	   limitstate=unlimit;
	 else if(receivecm&BIT(lockcommand))
		 limitstate=limit ;
	 else if(receivecm&BIT(stopcommand))
		 limitstate=limitstop ;
	
    tMsg.FrameType = CAN_DATA_FRAME;
    tMsg.IdType   = CAN_EXT_ID;
    tMsg.Id       = 0x18FE03FB;
    tMsg.DLC      = 8;
    tMsg.Data[0]  = limitstate;
	 if(unlimit == limitstate )//执行不锁车命令
	 {
	  tMsg.Data[1]  = 0xff;
    tMsg.Data[2]  = 0xff;
	  tMsg.Data[3]  = 0xff;
   }
	 else
	 {		 
    tMsg.Data[1]  = engzs*8;
    tMsg.Data[2]  = (engzs*8)>>8;
	  tMsg.Data[3]  = engniuju;
	 }
	tMsg.Data[4]  = lockpassword[0];
	tMsg.Data[5]  = lockpassword[1];
	
	tMsg.Data[6]  = 0x00;
	tMsg.Data[7]  = 0x00;
	

    if(CAN_Transmit(tCAN, MSG(10), &tMsg) == FALSE)
       return FALSE;
    else
      return TRUE;

 /*  CLK_SysTickDelay(1000000); //Delay one second to wait INT

    if(tCAN->ERR == 0)
        printf("\nCheck the receive host received data\n");
    else
        printf("\nCAN bus error!\n");
	*/
}
  void yclockmachine(void)
  {
		if((timeout>=15)&&(opentimeout))
			{
			 step=stepbuff;
				opentimeout=0;
				 initsend=0;
				receivestate=0;//若超时情况下收到则不处理
			}
		
		
		if(receivestate)//收到发动机状态帧
		{if(initsend)//主动发送后接收
			{
			 opentimeout=0;//关超时判断
			 stepbuff++;
			 step=stepbuff;
			 initsend=0;
			}
			else//被动接收 处理所有数据并回传网络
			{
				//处理数据 发送发动机状态
				//校验未通过检查是否进入被动锁车状态
				if(CAN_Rxbuf[7][4]&passivelockstatemsk)
				{
				 sendcm|=BIT(passivelock);//进入被动锁车状态
				 kgflag2|= BIT(plock);//请重启电源
				}
				//本次上电查询主动锁车状态
				if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//收到主动锁车回复 判断锁车状态
					{     
					sendcm|=BIT(limitok);//进入限制状态 
					sendcm&=~BIT(unlimitok);
					sendcm&=~BIT(stopok );
						
					sendcm&=~BIT(limitwait);	
          kgflag2|= BIT(ilockstate);//显示锁车状态					
					}										 
			 else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
					{
						sendcm|=BIT(unlimitok);//进入不限制状态  
						sendcm&=~BIT(stopok );
						sendcm&=~BIT(limitok);
						
					 sendcm&=~BIT(unlimitwait);
					 kgflag2&=~BIT(ilockstate);//不显示锁车状态	
					}											
			else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
					{
            sendcm|=BIT(stopok );//进入停机状态 
            sendcm&=~BIT(limitok);
						sendcm&=~BIT(unlimitok);
	
					  sendcm&=~BIT(stopwait);
						kgflag2|= BIT(ilockstate);//显示锁车状态	
					}
			if(CAN_Rxbuf[7][5]&activationstatemsk)// 发现激活状况
			{
        sendcm|=BIT(boundok );//解绑成功
				sendcm&=~BIT(boundwait);
			}
			else
			{		
				sendcm|=BIT(unboundok );
			  sendcm&=~BIT(unboundwait);
				boundstate=0;//进入未绑定的状态进程
			}              
	
      }
			receivestate=0;
		}
		
		
		if(!boundstate)//未绑定状态下
		{
			if(receivecm&BIT(boundcommand))//收到绑定命令
			{
				switch(step)
				{
					case 0:if((Request_Tx(CAN0)==TRUE))
				            {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} break;
					case 1:	 if(CAN_Rxbuf[7][5]&activationstatemsk)
					          {
										 sendcm|=BIT(illebound);//非法绑定	不发绑定命令
										  boundstate=1;//绑定状态
											
											receivecm&=~BIT(boundcommand);//取消收到绑定命令
											//发送发动机状态
										}
										else
										{if(Bound_Tx(CAN0)==TRUE)
											 {stepbuff=1;step=30;initsend=1;timeout=0;opentimeout=1;}
                    }
								    break;	
					case 2: if(CAN_Rxbuf[7][5]&activationstatemsk)//激活成功
					          {
										  sendcm|=BIT(boundok);//绑定成功
										  boundstate=1;//绑定状态
											//EEPROMwrite_boundstate(boundstate);//记录绑定状态
											EEPROMwrite_boundstate(sendcm);//记录绑定状态
											step=0;
											receivecm&=~BIT(boundcommand);//取消收到绑定命令
											//发送发动机状态
										} 
                    else
										{
											step=1;//重新发送绑定命令
											//opentimeout=0;
                    }											
									 break;		
			 }
				
      }
    }
		else if(boundstate==1)//绑定状态下
		{//校验阶段
			switch(step)
				{
					case 0:if((Request_Tx(CAN0)==TRUE))//
				            {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} break;
					case 1:	 if((!CAN_Rxbuf[7][0])&&(!CAN_Rxbuf[7][1])&&(!CAN_Rxbuf[7][2])&&(!CAN_Rxbuf[7][3]))//成功收到seed
					          { 
											seedmask[0]=CAN_Rxbuf[7][0];
											seedmask[1]=CAN_Rxbuf[7][1];
											seedmask[2]=CAN_Rxbuf[7][2];
											seedmask[3]=CAN_Rxbuf[7][3];
											seedmask[4]=MASKCODE;
											seedmask[5]=MASKCODE>>8;
											seedmask[6]=MASKCODE>>16;
											seedmask[7]=MASKCODE>>24;
											get_rand_str(seedmask, maparray,8);
										  MD5Init (&Md5context);
                      MD5Update (&Md5context, maparray, 8);
                      MD5Final ( MD5128code,&Md5context);
										 //取64位密码
											YCcode[0]=MD5128code[4];
											YCcode[1]=MD5128code[5];
											YCcode[2]=MD5128code[6];
											YCcode[3]=MD5128code[7];
											YCcode[4]=MD5128code[8];
											YCcode[5]=MD5128code[9];
											YCcode[6]=MD5128code[10];
											YCcode[7]=MD5128code[11];
											checkkey[0]=YCcode[2];
											checkkey[1]=YCcode[3];
											checkkey[2]=YCcode[4];
											checkkey[3]=YCcode[5];
											//主动锁车密码
											lockpassword[0]=YCcode[4];
											lockpassword[1]=YCcode[5];
											//解绑密码
                      unboundpassword[0]=YCcode[6];
											unboundpassword[0]=YCcode[7];
											if(key_Tx(CAN0)==TRUE)//发送校验帧
											{stepbuff=1;step=30;initsend=1;timeout=0;opentimeout=1;}
										}	
											//本次上电查询主动锁车状态
										if(CAN_Rxbuf[7][4]&passivelockstatemsk)
										{
				              sendcm|=BIT(passivelock);//进入被动锁车状态
										  kgflag2|= BIT(plock);//请重启电源
										}
				               //本次上电查询主动锁车状态
				           if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//收到主动锁车回复 判断锁车状态
					          {     
					           sendcm|=BIT(limitok);//进入限制状态 
					           sendcm&=~BIT(unlimitok);
					           sendcm&=~BIT(stopok );
						
					           sendcm&=~BIT(limitwait);	
                     kgflag2|= BIT(ilockstate);//显示锁车状态													
					          }										 
			            else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
					       {
						       sendcm|=BIT(unlimitok);//进入不限制状态  
						       sendcm&=~BIT(stopok );
						       sendcm&=~BIT(limitok);
						
					         sendcm&=~BIT(unlimitwait);
									 kgflag2&=~BIT(ilockstate);//不显示锁车状态	
					        }											
			            else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
					       {
                  sendcm|=BIT(stopok );//进入停机状态 
                  sendcm&=~BIT(limitok);
						      sendcm&=~BIT(unlimitok);
	
					        sendcm&=~BIT(stopwait);
									kgflag2|= BIT(ilockstate);//显示锁车状态		 
					       }
			         if(CAN_Rxbuf[7][5]&activationstatemsk)// 发现激活状况
			          {
                 sendcm|=BIT(boundok );//解绑成功
				         sendcm&=~BIT(boundwait);
			          }
			          else
			          {		
				         sendcm|=BIT(unboundok );
			           sendcm&=~BIT(unboundwait);
				         boundstate=0;//进入未绑定的状态进程
			          }  	
								break;	
					case 2: 										//发送发动机状态
											//本次上电查询主动锁车状态
										if(CAN_Rxbuf[7][4]&passivelockstatemsk)
										{
				              sendcm|=BIT(passivelock);//进入被动锁车状态
										  kgflag2|= BIT(plock);//请重启电源
										}
				            //本次上电查询主动锁车状态
			            	if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//收到主动锁车回复 判断锁车状态
					           {     
					            sendcm|=BIT(limitok);//进入限制状态 
					            sendcm&=~BIT(unlimitok);
					            sendcm&=~BIT(stopok );
						
					            sendcm&=~BIT(limitwait);
                      kgflag2|= BIT(ilockstate);//显示锁车状态													 
					           }										 
			              else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
					          {
						         sendcm|=BIT(unlimitok);//进入不限制状态  
						         sendcm&=~BIT(stopok );
						         sendcm&=~BIT(limitok);
						
					            sendcm&=~BIT(unlimitwait);
											kgflag2&=~BIT(ilockstate);//不显示锁车状态	
					          }											
			              else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
					          {
                     sendcm|=BIT(stopok );//进入停机状态 
                     sendcm&=~BIT(limitok);
						         sendcm&=~BIT(unlimitok);
	
					            sendcm&=~BIT(stopwait);
											kgflag2|= BIT(ilockstate);//显示锁车状态		
					          }
			             if(CAN_Rxbuf[7][5]&activationstatemsk)// 发现激活状况
			              {
                     sendcm|=BIT(boundok );//解绑成功
				             sendcm&=~BIT(boundwait);
			              }
			              else
			              {		
				             sendcm|=BIT(unboundok );
			               sendcm&=~BIT(unboundwait);
				             boundstate=0;//进入未绑定的状态进程
			              }  
									
                 /*   else
										{//校验未通过检查是否进入被动锁车状态
											if(CAN_Rxbuf[7][4]&passivelockstatemsk)
												sendcm|=BIT(passivelock);//进入被动锁车状态
											else
											step=1;//校验不通过 重新发送请求帧
											//opentimeout=0;
                    }	
								 */
                if((CAN_Rxbuf[7][5]&checkstatemsk)==checkpass)//校验通过
					          {
										  boundstate=2;//校验成功
											step=30;
											if(sendcm&BIT(illebound))
											{
											 sendcm&=~BIT(illebound);//非法绑定后校验成功 清楚非法绑定
											 EEPROMwrite_boundstate(sendcm);//记录绑定状态	
											}
											sendcm|=BIT(boundok);//绑定成功
										}											
                  else
									{ //step=1;//校验不通过 进入被动锁车状态
                    sendcm|=BIT(passivelock);//进入被动锁车状态
										kgflag2|= BIT(plock);//请重启电源
									}										
									 break;		
			 }
     }
		 else if(boundstate==2)//校验成功状态下  可执行主动锁车，解绑命令
		 { if(receivecm&0x1c)//收到限制命令 收到不限制命令 停机命令
				 step=0;
			 else if(receivecm&BIT(unboundcommand))//解绑命令
			     {if(sendcm&BIT(unlimitok))//非限制状态下才可执行解绑命令
				    step=2;
						else//限制状态下启动解绑命令
						{
						 sendcm|=BIT(unlimitwait);
					   sendcm&=~BIT(limitwait);
             sendcm&=~BIT(stopwait);
						 receivecm|=BIT(unlockcommand);	//不限制等待
             receivecm&=~BIT(stopcommand);
             receivecm&=~BIT(lockcommand);	
						} 
			     }
			 else
				 step=30;//无命令
			 switch(step)
				{
					case 0:if(InitiativeLock_Tx(CAN0)==TRUE)//
				         {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} break;
					case 1:	 if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//收到主动锁车回复 判断锁车状态
					         {     
										 sendcm|=BIT(limitok);//进入限制状态 
										 sendcm&=~BIT(limitwait);//清等待
										 sendcm&=~BIT(unlimitok);
										 sendcm&=~BIT(stopok );
										 kgflag2|= BIT(ilockstate);//显示锁车状态		
									 }										 
					          else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
										{
											sendcm|=BIT(unlimitok);//进入不限制状态 
                      sendcm&=~BIT(unlimitwait);//清等待											
											sendcm&=~BIT(stopok );
											sendcm&=~BIT(limitok);
											kgflag2&=~BIT(ilockstate);//不显示锁车状态	
										}											
									  else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
										{
                      sendcm|=BIT(stopok );//进入停机状态 
											sendcm&=~BIT(stopwait);//清等待	
                      sendcm&=~BIT(limitok);
											sendcm&=~BIT(unlimitok);
											kgflag2|= BIT(ilockstate);//显示锁车状态		
										}
										receivecm&=~0x1c;//清除主动锁车命令
								    break;
          case 2:if(UNBound_Tx(CAN0)==TRUE)//
				         {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} 										
								break;
					case 3:	 if(!(CAN_Rxbuf[7][5]&activationstatemsk))//未激活
					         {
						        sendcm|=BIT(unboundok);//
										 //EEPROMwrite_boundstate(0);//记录绑定状态	
										EEPROMwrite_boundstate(sendcm);//记录绑定状态
										receivecm&=~BIT(unboundcommand);//清除解绑命令 
									 }
									 else
										step=2;//重新解绑
								    break;			
								
								
			  }
     }
  }

 /*----------------------------------------------------------------------------*/
/*  Send Rx Msg by Normal Mode Function (With Message RAM)                    */
/*----------------------------------------------------------------------------*/
void CANRx_initial(CAN_T *tCAN)
{
    CAN_EnableInt(tCAN, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);      /* Enable CAN interrupt and corresponding NVIC of CAN */
    NVIC_SetPriority(CAN0_IRQn, (1 << __NVIC_PRIO_BITS) - 2);   /* Install CAN call back functions */
    NVIC_EnableIRQ(CAN0_IRQn);
	if(CAN_SetRxMsg(tCAN, MSG(0), CAN_EXT_ID,0x0cf00400 ) == FALSE)	 //发动机
    {
        return;	    
	}
	if(CAN_SetRxMsg(tCAN, MSG(1), CAN_EXT_ID, 0x18feef00 ) == FALSE)	 //油压
    {
        return;	    
    }
	if(CAN_SetRxMsg(tCAN, MSG(2), CAN_EXT_ID,0x18feee00 ) == FALSE)	 //水温
    {
        return;	   
    }
	if(CAN_SetRxMsg(tCAN, MSG(3), CAN_EXT_ID,0x18feca00 ) == FALSE)	 //故障码
    {
        return;	   
    }
	if(CAN_SetRxMsg(tCAN, MSG(4), CAN_EXT_ID,0x18fee500 ) == FALSE)	 //hours
    {
        return;	   
    }
		
	if(CAN_SetRxMsg(tCAN, MSG(5), CAN_EXT_ID,0x18feff00 ) == FALSE)	 //油水
    {
        return;	   
    }
		if(CAN_SetRxMsg(tCAN, MSG(6), CAN_EXT_ID,0x18FD0100 ) == FALSE)	 //发动机状态
    {
        return;	   
    }
	 if(CAN_SetRxMsg(tCAN, MSG(7), CAN_EXT_ID,0x18ecff00 ) == FALSE)	 //多包头
    {
        return;	   
    }
	 if(CAN_SetRxMsg(tCAN, MSG(8), CAN_EXT_ID,0x18ebff00 ) == FALSE)	 //多包
    {
        return;	   
    }
//	GprsSendComm(" CAN set ok");
}




/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle CAN interrupt event                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void CAN_MsgInterrupt(CAN_T *tCAN, uint32_t u32IIDR)
{  unsigned char i,j;
	switch(u32IIDR)
	{
	case 1:  //发动机		       
	    CanReceiveEndFlag=1;
		   tongxintime=0;
		   i=1;    
		 
		 break;
	case 2:  //油压
	      CanReceiveEndFlag=1;
		    tongxintime=0;
		    i=2;
		    break;
	case 3:   //水温
        CanReceiveEndFlag=1;
		    tongxintime=0;
		     i=3;
		     break;
	case 4:  //单包
	     CanReceiveEndFlag=1;
		   tongxintime=0;
		    errtime=0;
		    i=4;
		    break;
	 case 5: //hours
	      CanReceiveEndFlag=1;
		     tongxintime=0;
		     i=5;
		    break;
	 case 6:  //油水
	     CanReceiveEndFlag=1;
		 tongxintime=0;
		 i=6;
		 break;
	  case 7:  //发动机状态
	     CanReceiveEndFlag=1;
		 receivestate=1;
		 tongxintime=0;
		 i=7;
		 break;
	 
	  case 8: //多包头
	      CanReceiveEndFlag=1;
		     tongxintime=0;
		     i=8;
		    break;
	 case 9:  //多包
	     CanReceiveEndFlag=1;
		 tongxintime=0;
		 i=9;
		 break;
	  default:
		 i=0;
		 break;
	}
	if(i)	
	{
	 CAN_Receive(tCAN, (i-1), &rrMsg);
	 for(j = 0; j < 8; j++) 
    { 
      CAN_Rxbuf[i][j]=rrMsg.Data[j];          
    } 
	}
	if(i==8)	
	 {if(CAN_Rxbuf[8][5]==0xca)//为故障包
		 {
		  err2time=0;
		  DM1OK=1;
			 for(j = 0; j < 7; j++) 
	      DM1_PACK[0][j] = CAN_Rxbuf[8][j+1];
		  //ECU_flag |= BIT(Ecu_dm1);//显示多包
		 }
   }
	 if(i==9)
	{
		if(DM1OK)
		{
			err2time=0;
	   i=CAN_Rxbuf[9][0];//取数据包序号
			if(i== DM1_PACK[0][2])
				DM1OK=0;
	   for(j = 0; j < 7; j++) 
	   DM1_PACK[i][j] = CAN_Rxbuf[9][j+1];
			ECU_flag |= BIT(Ecu_dm1);//显示多包
		}
  }

}

/*---------------------------------------------------------------------------------------------------------*/
/* CAN0 interrupt handler                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void CAN0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;

    u8IIDRstatus = CAN0->IIDR;

    if(u8IIDRstatus == 0x00008000)        /* Check Status Interrupt Flag (Error status Int and Status change Int) */
    {
	   
        /**************************/
        /* Status Change interrupt*/
        /**************************/
        if(CAN0->STATUS & CAN_STATUS_RXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear RxOK status*/
        }

        if(CAN0->STATUS & CAN_STATUS_TXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_TXOK_Msk;    /* Clear TxOK status*/
        }

        /**************************/
        /* Error Status interrupt */
        /**************************/
//        if(CAN0->STATUS & CAN_STATUS_BOFF_Msk)
//        {
//            GprsSendComm("BOFF INT\n") ;
//        }
//        else if(CAN0->STATUS & CAN_STATUS_EWARN_Msk)
//        {
//            GprsSendComm("EWARN INT\n") ;
//        }
//        else if((CAN0->ERR & CAN_ERR_TEC_Msk) != 0)
//        {
//            GprsSendComm("Transmit error!\n") ;
//        }
//        else if((CAN0->ERR & CAN_ERR_REC_Msk) != 0)
//        {
//            GprsSendComm("Receive error!\n") ;
//        }
    }
    else if((u8IIDRstatus >= 0x1) && (u8IIDRstatus <= 0x20))
    {
        CAN_MsgInterrupt(CAN0, u8IIDRstatus);
		 
        CAN_CLR_INT_PENDING_BIT(CAN0, (u8IIDRstatus - 1)); /* Clear Interrupt Pending */
    }
    else if(CAN0->WU_STATUS == 1)
    {
     //   GprsSendComm("Wake up\n");

        CAN0->WU_STATUS = 0;    /* Write '0' to clear */
    }

}




void CAN_data(void)
{
  unsigned char temp=0;
	unsigned char j,k;
	if(tongxintime>=50)
	{
	   for(k = 0; k < 10; k++)
	   {
		  for(j = 0; j < 8; j++)
	     CAN_Rxbuf[k][j]=0;
	   }
	 
	 // CAN_Rxbuf[2][3]=250;
	  CAN_Rxbuf[3][0]=40; 
 	} 
	
	
	
	
  //发动机
  fdj_rev=CAN_Rxbuf[1][3]|CAN_Rxbuf[1][4]<<8;
  fdj_rev = Get_Data(fdj_temp,fdj_rev,10);
  fdj_rev =fdj_rev/8;
  if(fdj_rev>3000){fdj_rev=3000;} 
  //油压
  oil_p=CAN_Rxbuf[2][3];
  if(oil_p<20)
	bj_flag |= BIT(bj_yy);
   else
  bj_flag &=~ BIT(bj_yy);
  if(oil_p>250)
  {oil_p=250;}
    //oil_p =oil_p/25;
  oil_p = Get_Data(oil_ptemp,oil_p,10);
  oil_p =oil_p/5*2;
  //水温
  watmp=CAN_Rxbuf[3][0];
  watmp = Get_Data(Wtemp,watmp,10);
  if(watmp>40){watmp -= 40;}
  else{watmp=0;}
  watmp1=watmp;
  //DM1故障
  err=CAN_Rxbuf[4][4]>>5;
  err=CAN_Rxbuf[4][3]|err<<8;
  err=CAN_Rxbuf[4][2]|err<<8;
  //hours
  Engine_hours=CAN_Rxbuf[5][3];
  Engine_hours=CAN_Rxbuf[5][2]|Engine_hours<<8;
  Engine_hours=CAN_Rxbuf[5][1]|Engine_hours<<8;
  Engine_hours=CAN_Rxbuf[5][0]|Engine_hours<<8;
  Engine_hours=Engine_hours/2;
	
  /*//预热灯
  temp=CAN_Rxbuf[6][3];
  temp &= 0x03;
  if(temp==0x01){kgflag2 |= BIT(yr);}
   else {kgflag2 &=~ BIT(yr);}
	 */
	 //油水
  temp=CAN_Rxbuf[6][0];
  temp &= 0x03;
  if(temp==0x01)
  kgflag2 |= BIT(ys);
  else 
  kgflag2 &=~ BIT(ys);
		
}



#endif