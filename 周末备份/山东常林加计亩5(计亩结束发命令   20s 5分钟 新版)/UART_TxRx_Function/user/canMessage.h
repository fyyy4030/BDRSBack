
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
// ������
unsigned int constantcode=0;  //1234//0x4d2
unsigned long GPSID= 160502001;	//0x9 91 10 F1
//��������
 #define unlimit     0x00
 #define limitstop   0x01
 #define limit       0x10
 unsigned char limitstate;	
 //��������
 unsigned int engzs=0;
 unsigned char engniuju=0;

 unsigned char checkkey[4]; //����GPSID���У����
 unsigned char lockpassword[2];
 unsigned char unboundpassword[2];
 //������״̬����
 #define passivelockstatemsk  3<<2  //��4�ֽڣ���������״̬
 
 #define initlockstatemsk     3<<6  //��4�ֽڣ���������״̬
 #define initunlock     0<<6  //��4�ֽڣ�����δ����
 #define initlimit     2<<6  //��4�ֽڣ�����δ����
 #define initstop     1<<6  //��4�ֽڣ�����δ����


 #define checkstatemsk         3<<4  //��5�ֽڣ�У��״̬
  #define checkunpass         0<<4  //��5�ֽڣ�У��δͨ��
	  #define checkpass         1<<4  //��5�ֽڣ�У��ͨ��
			  #define uncheck        3<<4  //��5�ֽڣ�δУ��

 #define activationstatemsk    3<<6  //��5�ֽڣ�����״̬
  #define unactivationstate    0<<6  //��5�ֽڣ�����״̬
 #define activationstate    1<<6  //��5�ֽڣ�����״̬

 #define requestsign         3<<0  //��7�ֽڣ�У��״̬
  //#define requestsignunreceive        0<<0  //��7�ֽڣ�У��״̬
	 // #define requestsignreceive        1<<0  //��7�ֽڣ�У��״̬
uint32_t boundstate=0;//0δ��״̬  1δ��״̬
//unsigned char  boundstate=0;//0δ��״̬  1δ��״̬
//#define boundstate 0 //0δ��״̬  1δ��״̬
unsigned char  receivecm=0;
#define boundcommand   0  //������
#define unboundcommand 1 //�������
#define lockcommand    2 //��������
#define unlockcommand  3 //����������
#define stopcommand    4 //ͣ������
#define inquirecommand 5 //��ѯ����


unsigned int  sendcm=0;
#define illebound 0 //�Ƿ���
#define boundok   1 //�󶨳ɹ�
#define unboundok 2 //���ɹ�
#define limitok   3 //���Ƴɹ�
#define stopok    4 //ͣ���ɹ�
#define unlimitok 5 //�����Ƴɹ�
#define passivelock 6 //��������
#define boundwait   7 //�󶨵ȴ�
#define unboundwait 8 //���ȴ�
#define limitwait   9 //���Ƶȴ�
#define stopwait    10 //ͣ���ɹ�
#define unlimitwait 11 //�����Ƶȴ�
#define ycmachine   12 //���
#define wcmachine   13 //Ϋ��
#define qcmachine   14 //ȫ��
#define setmachine  15 //��������




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
 if(ECU_flag & BIT(Ecu_dm1))//��ʾ�������
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
 {//��ʾ����
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
unsigned char Request_Tx(CAN_T *tCAN) //��������֡
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

unsigned char key_Tx(CAN_T *tCAN) //	����У��֡
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

  unsigned char Bound_Tx(CAN_T *tCAN) //	���Ͱ�����
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

   unsigned char UNBound_Tx(CAN_T *tCAN) //	���ͽ������
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


   unsigned char InitiativeLock_Tx(CAN_T *tCAN) //	����������������
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
	 if(unlimit == limitstate )//ִ�в���������
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
				receivestate=0;//����ʱ������յ��򲻴���
			}
		
		
		if(receivestate)//�յ�������״̬֡
		{if(initsend)//�������ͺ����
			{
			 opentimeout=0;//�س�ʱ�ж�
			 stepbuff++;
			 step=stepbuff;
			 initsend=0;
			}
			else//�������� �����������ݲ��ش�����
			{
				//�������� ���ͷ�����״̬
				//У��δͨ������Ƿ���뱻������״̬
				if(CAN_Rxbuf[7][4]&passivelockstatemsk)
				{
				 sendcm|=BIT(passivelock);//���뱻������״̬
				 kgflag2|= BIT(plock);//��������Դ
				}
				//�����ϵ��ѯ��������״̬
				if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//�յ����������ظ� �ж�����״̬
					{     
					sendcm|=BIT(limitok);//��������״̬ 
					sendcm&=~BIT(unlimitok);
					sendcm&=~BIT(stopok );
						
					sendcm&=~BIT(limitwait);	
          kgflag2|= BIT(ilockstate);//��ʾ����״̬					
					}										 
			 else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
					{
						sendcm|=BIT(unlimitok);//���벻����״̬  
						sendcm&=~BIT(stopok );
						sendcm&=~BIT(limitok);
						
					 sendcm&=~BIT(unlimitwait);
					 kgflag2&=~BIT(ilockstate);//����ʾ����״̬	
					}											
			else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
					{
            sendcm|=BIT(stopok );//����ͣ��״̬ 
            sendcm&=~BIT(limitok);
						sendcm&=~BIT(unlimitok);
	
					  sendcm&=~BIT(stopwait);
						kgflag2|= BIT(ilockstate);//��ʾ����״̬	
					}
			if(CAN_Rxbuf[7][5]&activationstatemsk)// ���ּ���״��
			{
        sendcm|=BIT(boundok );//���ɹ�
				sendcm&=~BIT(boundwait);
			}
			else
			{		
				sendcm|=BIT(unboundok );
			  sendcm&=~BIT(unboundwait);
				boundstate=0;//����δ�󶨵�״̬����
			}              
	
      }
			receivestate=0;
		}
		
		
		if(!boundstate)//δ��״̬��
		{
			if(receivecm&BIT(boundcommand))//�յ�������
			{
				switch(step)
				{
					case 0:if((Request_Tx(CAN0)==TRUE))
				            {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} break;
					case 1:	 if(CAN_Rxbuf[7][5]&activationstatemsk)
					          {
										 sendcm|=BIT(illebound);//�Ƿ���	����������
										  boundstate=1;//��״̬
											
											receivecm&=~BIT(boundcommand);//ȡ���յ�������
											//���ͷ�����״̬
										}
										else
										{if(Bound_Tx(CAN0)==TRUE)
											 {stepbuff=1;step=30;initsend=1;timeout=0;opentimeout=1;}
                    }
								    break;	
					case 2: if(CAN_Rxbuf[7][5]&activationstatemsk)//����ɹ�
					          {
										  sendcm|=BIT(boundok);//�󶨳ɹ�
										  boundstate=1;//��״̬
											//EEPROMwrite_boundstate(boundstate);//��¼��״̬
											EEPROMwrite_boundstate(sendcm);//��¼��״̬
											step=0;
											receivecm&=~BIT(boundcommand);//ȡ���յ�������
											//���ͷ�����״̬
										} 
                    else
										{
											step=1;//���·��Ͱ�����
											//opentimeout=0;
                    }											
									 break;		
			 }
				
      }
    }
		else if(boundstate==1)//��״̬��
		{//У��׶�
			switch(step)
				{
					case 0:if((Request_Tx(CAN0)==TRUE))//
				            {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} break;
					case 1:	 if((!CAN_Rxbuf[7][0])&&(!CAN_Rxbuf[7][1])&&(!CAN_Rxbuf[7][2])&&(!CAN_Rxbuf[7][3]))//�ɹ��յ�seed
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
										 //ȡ64λ����
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
											//������������
											lockpassword[0]=YCcode[4];
											lockpassword[1]=YCcode[5];
											//�������
                      unboundpassword[0]=YCcode[6];
											unboundpassword[0]=YCcode[7];
											if(key_Tx(CAN0)==TRUE)//����У��֡
											{stepbuff=1;step=30;initsend=1;timeout=0;opentimeout=1;}
										}	
											//�����ϵ��ѯ��������״̬
										if(CAN_Rxbuf[7][4]&passivelockstatemsk)
										{
				              sendcm|=BIT(passivelock);//���뱻������״̬
										  kgflag2|= BIT(plock);//��������Դ
										}
				               //�����ϵ��ѯ��������״̬
				           if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//�յ����������ظ� �ж�����״̬
					          {     
					           sendcm|=BIT(limitok);//��������״̬ 
					           sendcm&=~BIT(unlimitok);
					           sendcm&=~BIT(stopok );
						
					           sendcm&=~BIT(limitwait);	
                     kgflag2|= BIT(ilockstate);//��ʾ����״̬													
					          }										 
			            else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
					       {
						       sendcm|=BIT(unlimitok);//���벻����״̬  
						       sendcm&=~BIT(stopok );
						       sendcm&=~BIT(limitok);
						
					         sendcm&=~BIT(unlimitwait);
									 kgflag2&=~BIT(ilockstate);//����ʾ����״̬	
					        }											
			            else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
					       {
                  sendcm|=BIT(stopok );//����ͣ��״̬ 
                  sendcm&=~BIT(limitok);
						      sendcm&=~BIT(unlimitok);
	
					        sendcm&=~BIT(stopwait);
									kgflag2|= BIT(ilockstate);//��ʾ����״̬		 
					       }
			         if(CAN_Rxbuf[7][5]&activationstatemsk)// ���ּ���״��
			          {
                 sendcm|=BIT(boundok );//���ɹ�
				         sendcm&=~BIT(boundwait);
			          }
			          else
			          {		
				         sendcm|=BIT(unboundok );
			           sendcm&=~BIT(unboundwait);
				         boundstate=0;//����δ�󶨵�״̬����
			          }  	
								break;	
					case 2: 										//���ͷ�����״̬
											//�����ϵ��ѯ��������״̬
										if(CAN_Rxbuf[7][4]&passivelockstatemsk)
										{
				              sendcm|=BIT(passivelock);//���뱻������״̬
										  kgflag2|= BIT(plock);//��������Դ
										}
				            //�����ϵ��ѯ��������״̬
			            	if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//�յ����������ظ� �ж�����״̬
					           {     
					            sendcm|=BIT(limitok);//��������״̬ 
					            sendcm&=~BIT(unlimitok);
					            sendcm&=~BIT(stopok );
						
					            sendcm&=~BIT(limitwait);
                      kgflag2|= BIT(ilockstate);//��ʾ����״̬													 
					           }										 
			              else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
					          {
						         sendcm|=BIT(unlimitok);//���벻����״̬  
						         sendcm&=~BIT(stopok );
						         sendcm&=~BIT(limitok);
						
					            sendcm&=~BIT(unlimitwait);
											kgflag2&=~BIT(ilockstate);//����ʾ����״̬	
					          }											
			              else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
					          {
                     sendcm|=BIT(stopok );//����ͣ��״̬ 
                     sendcm&=~BIT(limitok);
						         sendcm&=~BIT(unlimitok);
	
					            sendcm&=~BIT(stopwait);
											kgflag2|= BIT(ilockstate);//��ʾ����״̬		
					          }
			             if(CAN_Rxbuf[7][5]&activationstatemsk)// ���ּ���״��
			              {
                     sendcm|=BIT(boundok );//���ɹ�
				             sendcm&=~BIT(boundwait);
			              }
			              else
			              {		
				             sendcm|=BIT(unboundok );
			               sendcm&=~BIT(unboundwait);
				             boundstate=0;//����δ�󶨵�״̬����
			              }  
									
                 /*   else
										{//У��δͨ������Ƿ���뱻������״̬
											if(CAN_Rxbuf[7][4]&passivelockstatemsk)
												sendcm|=BIT(passivelock);//���뱻������״̬
											else
											step=1;//У�鲻ͨ�� ���·�������֡
											//opentimeout=0;
                    }	
								 */
                if((CAN_Rxbuf[7][5]&checkstatemsk)==checkpass)//У��ͨ��
					          {
										  boundstate=2;//У��ɹ�
											step=30;
											if(sendcm&BIT(illebound))
											{
											 sendcm&=~BIT(illebound);//�Ƿ��󶨺�У��ɹ� ����Ƿ���
											 EEPROMwrite_boundstate(sendcm);//��¼��״̬	
											}
											sendcm|=BIT(boundok);//�󶨳ɹ�
										}											
                  else
									{ //step=1;//У�鲻ͨ�� ���뱻������״̬
                    sendcm|=BIT(passivelock);//���뱻������״̬
										kgflag2|= BIT(plock);//��������Դ
									}										
									 break;		
			 }
     }
		 else if(boundstate==2)//У��ɹ�״̬��  ��ִ�������������������
		 { if(receivecm&0x1c)//�յ��������� �յ����������� ͣ������
				 step=0;
			 else if(receivecm&BIT(unboundcommand))//�������
			     {if(sendcm&BIT(unlimitok))//������״̬�²ſ�ִ�н������
				    step=2;
						else//����״̬�������������
						{
						 sendcm|=BIT(unlimitwait);
					   sendcm&=~BIT(limitwait);
             sendcm&=~BIT(stopwait);
						 receivecm|=BIT(unlockcommand);	//�����Ƶȴ�
             receivecm&=~BIT(stopcommand);
             receivecm&=~BIT(lockcommand);	
						} 
			     }
			 else
				 step=30;//������
			 switch(step)
				{
					case 0:if(InitiativeLock_Tx(CAN0)==TRUE)//
				         {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} break;
					case 1:	 if((CAN_Rxbuf[7][4]&initlockstatemsk)==initunlock)//�յ����������ظ� �ж�����״̬
					         {     
										 sendcm|=BIT(limitok);//��������״̬ 
										 sendcm&=~BIT(limitwait);//��ȴ�
										 sendcm&=~BIT(unlimitok);
										 sendcm&=~BIT(stopok );
										 kgflag2|= BIT(ilockstate);//��ʾ����״̬		
									 }										 
					          else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initlimit)
										{
											sendcm|=BIT(unlimitok);//���벻����״̬ 
                      sendcm&=~BIT(unlimitwait);//��ȴ�											
											sendcm&=~BIT(stopok );
											sendcm&=~BIT(limitok);
											kgflag2&=~BIT(ilockstate);//����ʾ����״̬	
										}											
									  else if((CAN_Rxbuf[7][4]&initlockstatemsk)==initstop )
										{
                      sendcm|=BIT(stopok );//����ͣ��״̬ 
											sendcm&=~BIT(stopwait);//��ȴ�	
                      sendcm&=~BIT(limitok);
											sendcm&=~BIT(unlimitok);
											kgflag2|= BIT(ilockstate);//��ʾ����״̬		
										}
										receivecm&=~0x1c;//���������������
								    break;
          case 2:if(UNBound_Tx(CAN0)==TRUE)//
				         {stepbuff=0;step=30;initsend=1;timeout=0;opentimeout=1;} 										
								break;
					case 3:	 if(!(CAN_Rxbuf[7][5]&activationstatemsk))//δ����
					         {
						        sendcm|=BIT(unboundok);//
										 //EEPROMwrite_boundstate(0);//��¼��״̬	
										EEPROMwrite_boundstate(sendcm);//��¼��״̬
										receivecm&=~BIT(unboundcommand);//���������� 
									 }
									 else
										step=2;//���½��
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
	if(CAN_SetRxMsg(tCAN, MSG(0), CAN_EXT_ID,0x0cf00400 ) == FALSE)	 //������
    {
        return;	    
	}
	if(CAN_SetRxMsg(tCAN, MSG(1), CAN_EXT_ID, 0x18feef00 ) == FALSE)	 //��ѹ
    {
        return;	    
    }
	if(CAN_SetRxMsg(tCAN, MSG(2), CAN_EXT_ID,0x18feee00 ) == FALSE)	 //ˮ��
    {
        return;	   
    }
	if(CAN_SetRxMsg(tCAN, MSG(3), CAN_EXT_ID,0x18feca00 ) == FALSE)	 //������
    {
        return;	   
    }
	if(CAN_SetRxMsg(tCAN, MSG(4), CAN_EXT_ID,0x18fee500 ) == FALSE)	 //hours
    {
        return;	   
    }
		
	if(CAN_SetRxMsg(tCAN, MSG(5), CAN_EXT_ID,0x18feff00 ) == FALSE)	 //��ˮ
    {
        return;	   
    }
		if(CAN_SetRxMsg(tCAN, MSG(6), CAN_EXT_ID,0x18FD0100 ) == FALSE)	 //������״̬
    {
        return;	   
    }
	 if(CAN_SetRxMsg(tCAN, MSG(7), CAN_EXT_ID,0x18ecff00 ) == FALSE)	 //���ͷ
    {
        return;	   
    }
	 if(CAN_SetRxMsg(tCAN, MSG(8), CAN_EXT_ID,0x18ebff00 ) == FALSE)	 //���
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
	case 1:  //������		       
	    CanReceiveEndFlag=1;
		   tongxintime=0;
		   i=1;    
		 
		 break;
	case 2:  //��ѹ
	      CanReceiveEndFlag=1;
		    tongxintime=0;
		    i=2;
		    break;
	case 3:   //ˮ��
        CanReceiveEndFlag=1;
		    tongxintime=0;
		     i=3;
		     break;
	case 4:  //����
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
	 case 6:  //��ˮ
	     CanReceiveEndFlag=1;
		 tongxintime=0;
		 i=6;
		 break;
	  case 7:  //������״̬
	     CanReceiveEndFlag=1;
		 receivestate=1;
		 tongxintime=0;
		 i=7;
		 break;
	 
	  case 8: //���ͷ
	      CanReceiveEndFlag=1;
		     tongxintime=0;
		     i=8;
		    break;
	 case 9:  //���
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
	 {if(CAN_Rxbuf[8][5]==0xca)//Ϊ���ϰ�
		 {
		  err2time=0;
		  DM1OK=1;
			 for(j = 0; j < 7; j++) 
	      DM1_PACK[0][j] = CAN_Rxbuf[8][j+1];
		  //ECU_flag |= BIT(Ecu_dm1);//��ʾ���
		 }
   }
	 if(i==9)
	{
		if(DM1OK)
		{
			err2time=0;
	   i=CAN_Rxbuf[9][0];//ȡ���ݰ����
			if(i== DM1_PACK[0][2])
				DM1OK=0;
	   for(j = 0; j < 7; j++) 
	   DM1_PACK[i][j] = CAN_Rxbuf[9][j+1];
			ECU_flag |= BIT(Ecu_dm1);//��ʾ���
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
	
	
	
	
  //������
  fdj_rev=CAN_Rxbuf[1][3]|CAN_Rxbuf[1][4]<<8;
  fdj_rev = Get_Data(fdj_temp,fdj_rev,10);
  fdj_rev =fdj_rev/8;
  if(fdj_rev>3000){fdj_rev=3000;} 
  //��ѹ
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
  //ˮ��
  watmp=CAN_Rxbuf[3][0];
  watmp = Get_Data(Wtemp,watmp,10);
  if(watmp>40){watmp -= 40;}
  else{watmp=0;}
  watmp1=watmp;
  //DM1����
  err=CAN_Rxbuf[4][4]>>5;
  err=CAN_Rxbuf[4][3]|err<<8;
  err=CAN_Rxbuf[4][2]|err<<8;
  //hours
  Engine_hours=CAN_Rxbuf[5][3];
  Engine_hours=CAN_Rxbuf[5][2]|Engine_hours<<8;
  Engine_hours=CAN_Rxbuf[5][1]|Engine_hours<<8;
  Engine_hours=CAN_Rxbuf[5][0]|Engine_hours<<8;
  Engine_hours=Engine_hours/2;
	
  /*//Ԥ�ȵ�
  temp=CAN_Rxbuf[6][3];
  temp &= 0x03;
  if(temp==0x01){kgflag2 |= BIT(yr);}
   else {kgflag2 &=~ BIT(yr);}
	 */
	 //��ˮ
  temp=CAN_Rxbuf[6][0];
  temp &= 0x03;
  if(temp==0x01)
  kgflag2 |= BIT(ys);
  else 
  kgflag2 &=~ BIT(ys);
		
}



#endif