
#ifndef		_ALART_H_
#define		_ALART_H_
 #include <stdio.h>
#include "NUC131.h"
#include "globle.h"
#include "proto.h"

void alert_bj(void);
void fd_bj(void);


void keyscan(void)
{
  /******************��ת********************/
	if(GPIO_GET_IN_DATA(PA) & BIT10)
   {kgflag1 |= BIT(left);  NewMachineFrame.u32indicateLed |= BIT(LEFTled);}
   else
   {kgflag1 &=~ BIT(left); NewMachineFrame.u32indicateLed &=~BIT(LEFTled);}
  /******************��ת********************/
  if(GPIO_GET_IN_DATA(PB) & BIT7)
   {kgflag1 |= BIT(right);NewMachineFrame.u32indicateLed |= BIT(RIGHTled);}
   else
   {kgflag1 &=~ BIT(right);NewMachineFrame.u32indicateLed &= ~BIT(RIGHTled);}
  /******************Զ��********************/
  if(GPIO_GET_IN_DATA(PA) & BIT9)
   {kgflag1 |= BIT(ygled);NewMachineFrame.u32indicateLed |= BIT(YGled);}
   else
   {kgflag1 &=~ BIT(ygled);NewMachineFrame.u32indicateLed &= ~BIT(YGled);}
  /******************����********************/
  if(GPIO_GET_IN_DATA(PA) & BIT11)
   {kgflag1 |= BIT(jgbp);NewMachineFrame.u32indicateLed |= BIT(JGled);}
   else
   {kgflag1 &=~ BIT(jgbp);NewMachineFrame.u32indicateLed &= ~BIT(JGled);}
  /******************ʾ��********************/
  if(GPIO_GET_IN_DATA(PB) & BIT2)
   {kgflag1 |= BIT(skled);NewMachineFrame.u32indicateLed |= BIT(SKled);}
   else
   {kgflag1 &=~ BIT(skled);NewMachineFrame.u32indicateLed &= ~BIT(SKled);}
  /******************����********************/
  if(GPIO_GET_IN_DATA(PF) & BIT4)
   {kgflag1 |= BIT(fr);}
   else
   {kgflag1 &=~ BIT(fr);}
   /******************��ɲ********************/
  if(GPIO_GET_IN_DATA(PC) & BIT0)
   {kgflag1 &=~ BIT(handbp);NewMachineFrame.u32indicateLed &=~BIT(HANDled);}
   else
   {kgflag1 |= BIT(handbp);NewMachineFrame.u32indicateLed |= BIT(HANDled);}
  /******************����********************/
  if(GPIO_GET_IN_DATA(PB) & BIT3)
   {kgflag2 &=~ BIT(lm);NewMachineFrame.u32indicateLed &=~ BIT(LMled);}
   else
   {kgflag2 |= BIT(lm);NewMachineFrame.u32indicateLed |= BIT(LMled);}
  
	  /******************���********************/

  if(GPIO_GET_IN_DATA(PC) & BIT14)
   {kgflag3 &=~ BIT(cdjc);NewMachineFrame.u32indicateLed &=~BIT(CDled);}
   else
   {kgflag3 |= BIT(cdjc);NewMachineFrame.u32indicateLed|= BIT(CDled);}
	 
////////////////////////////////
	
}




void alert_jc(void)
{
 if(kgflag1 & BIT(left)){tbflag1 |= BIT(0);} else {tbflag1 &=~ BIT(0);}  //��ת
 if(kgflag1 & BIT(right)){tbflag1 |= BIT(1);} else {tbflag1 &=~ BIT(1);}  //��ת
 if(kgflag1 & BIT(ygled)){tbflag1 |= BIT(2);} else {tbflag1 &=~ BIT(2);}  //Զ��
 if(kgflag1 & BIT(jgbp)){tbflag1 |= BIT(3);} else {tbflag1 &=~ BIT(3);}  //����
 if(kgflag1 & BIT(skled)){tbflag1 |= BIT(4);} else {tbflag1 &=~ BIT(4);}  //ʾ��
 if(kgflag3 & BIT(cdjc)){tbflag1 |= BIT(11);} else {tbflag1 &=~ BIT(11);}  //���
 /*
 if(kgflag4 & BIT(ylf)){tbflag2 |= BIT(0);} else {tbflag2 &=~ BIT(0);}  //???
 if(kgflag4 & BIT(gt_up)){tbflag2 |= BIT(1);} else {tbflag2 &=~ BIT(1);} //?????
 if(kgflag4 & BIT(gt_down)){tbflag2 |= BIT(2);} else {tbflag2 &=~ BIT(2);} //?????
 if(kgflag4 & BIT(ht_up)){tbflag2 |= BIT(3);} else {tbflag2 &=~ BIT(3);} //?????
 if(kgflag4 & BIT(ht_down)){tbflag2 |= BIT(4);} else {tbflag2 &=~ BIT(4);} //?????
 if(kgflag4 & BIT(lc_up)){tbflag2 |= BIT(5);} else {tbflag2 &=~ BIT(5);} //?????
 if(kgflag4 & BIT(lc_down)){tbflag2 |= BIT(6);} else {tbflag2 &=~ BIT(6);} //?????
 
 if(kgflag3 & BIT(gt_sup)){tbflag2 |= BIT(7);} else {tbflag2 &=~ BIT(7);} //?????
 if(kgflag3 & BIT(gt_sdown)){tbflag2 |= BIT(8);} else {tbflag2 &=~ BIT(8);} //?????
 if(kgflag3 & BIT(ht_sup)){tbflag2 |= BIT(9);} else {tbflag2 &=~ BIT(9);} //?????
 if(kgflag3 & BIT(ht_sdown)){tbflag2 |= BIT(10);} else {tbflag2 &=~ BIT(10);} //?????
 if(kgflag3 & BIT(lc_sup)){tbflag2 |= BIT(11);} else {tbflag2 &=~ BIT(11);} //?????
 if(kgflag3 & BIT(lc_sdown)){tbflag2 |= BIT(12);} else {tbflag2 &=~ BIT(12);} //?????
*/
 

/*************��ɲ����******************/
  if(kgflag1 & BIT(handbp))
  {
    bj_flag |= BIT(bj_ss);
	if(bj_delay600MS <=6)
	 {tbflag1 |= BIT(5);}
	 else
   {tbflag1 &=~ BIT(5);}
  }
  else
  {
    bj_flag &=~ BIT(bj_ss);
	 tbflag1 &=~ BIT(5);
  }
  
  
/*************��ѹ����*****************
  if(oil_p<1)
  {
    bj_flag |= BIT(bj_yy);
	if(bj_delay600MS == 1)
	 {tbflag1 |= BIT(8);}
	 else{tbflag1 &=~ BIT(8);}
  }
  else
  {
    bj_flag &=~ BIT(bj_yy);
	tbflag1 &=~ BIT(8);
  }*/
/*************��ѹ����******************/
  if(bj_flag & BIT(bj_yy))
  {
		NewMachineFrame.u32indicateLed |=BIT(YYled);
	if(bj_delay600MS <=5)
	 {tbflag1 |= BIT(8);}
	 else
   {tbflag1 &=~ BIT(8);}
  }
  else
  {
	 NewMachineFrame.u32indicateLed &=~BIT(YYled);
	 tbflag1 &=~ BIT(8);
  }

/*************���˱���******************/
  if(kgflag1 & BIT(klbp))
  {
    bj_flag |= BIT(bj_kl);
	if(bj_delay600MS<=5)
	 {tbflag1 |= BIT(6);}
	 else{tbflag1 &=~ BIT(6);}
  }
  else
  {
    bj_flag &=~ BIT(bj_kl);
	tbflag1 &=~ BIT(6);
  } 
/****************ˮ�±���********************/
  if(watmp>100)
  { 
		NewMachineFrame.u32indicateLed |=BIT(SWled);
    bj_flag |= BIT(bj_sw);
	if(bj_delay600MS <=5)
	 {
	   tbflag1 |= BIT(7);
	   tbflag3 |= BIT(15);
	 }
	 else
	 {
	   tbflag1 &=~ BIT(7);
	   tbflag3 &=~ BIT(15);
	 }
  }
  else
  {
		NewMachineFrame.u32indicateLed &=~BIT(SWled);
    bj_flag &=~ BIT(bj_sw);
	 tbflag1 &=~ BIT(7);
	 tbflag3 &=~ BIT(15);
  } 
/****************��������********************/
  if(oil_m==0)
  {
		NewMachineFrame.u32indicateLed |=BIT(YLled);
    bj_flag2 |= BIT(bj_yl);
	 if(bj_delay600MS <=5)
	 {tbflag1 |= BIT(12);}
	 else{tbflag1 &=~ BIT(12);}
  }
  else
  {
		NewMachineFrame.u32indicateLed &=~BIT(YLled);
    bj_flag2 &=~ BIT(bj_yl);
	  tbflag1 &=~ BIT(12);
  } 
/*******************���¸�********************/
	/*
 if(press_tep>90)
 {
    bj_flag |= BIT(bj_yw);
	if(bj_delay600MS == 1)
	 {
	   tbflag1 |= BIT(9);
	   tbflag3 |= BIT(14);
	 }
	 else
	 {
	   tbflag1 &=~ BIT(9);
	   tbflag3 &=~ BIT(14);
	 }
  }
  else
  {
    bj_flag &=~ BIT(bj_yw);
	tbflag1 &=~ BIT(9);
	tbflag3 &=~ BIT(14);
  } */
/***************Һѹ������1****************/ 
/*
if(kgflag2 & BIT(yygl1))
  {
    bj_flag |= BIT(bj_yey1);
	if(bj_delay600MS <=5)
	 {tbflag1 |= BIT(14);}
	 else{tbflag1 &=~ BIT(14);}
  }
  else
  {
    bj_flag &=~ BIT(bj_yey1);
	tbflag1 &=~ BIT(14);
  } 
	*/
/***************Һѹ������2*************** 
if(kgflag2 & BIT(yygl2))
  {
    bj_flag |= BIT(bj_yey2);
	if(bj_delay600MS == 1)
	 {tbflag1 |= BIT(15);}
	 else{tbflag1 &=~ BIT(15);}
  }
  else
  {
    bj_flag &=~ BIT(bj_yey2);
	tbflag1 &=~ BIT(15);
  } */ 
/***************Ԥ��***************/ 
/*if(kgflag2 & BIT(yr))
  {
	tbflag1 |= BIT(15);
  }
  else
  {
	tbflag1 &=~ BIT(15);
  }
*/
	/***************��ˮ***************/ 
if(kgflag2 & BIT(ys))
  {
	 NewMachineFrame.u32indicateLed |=BIT(YS2led);
	 tbflag1 |= BIT(15);
  }
  else
  {
	 NewMachineFrame.u32indicateLed &=~BIT(YS2led);
	 tbflag1 &=~ BIT(15);
  }
	
	/***************����***************/ 
if(kgflag2 & BIT(lm))
  {
	tbflag1 |= BIT(9);
  }
  else
  {
	tbflag1 &=~ BIT(9);
  }
	
	
/*************����׼�******************/
if(ECU_flag & BIT(Miji))
  {   
		tbflag2 &=~ BIT(12);
    tbflag2 |= BIT(11);//�׼�
  }
   else
 {  
	 tbflag2 &=~ BIT(11);
   tbflag2 |= BIT(12);
  } 	
/**************��Ƶ�Զ�&�ֶ��л�***************/
if(flag & BIT(av_auto))
  {
   tbflag2 &=~ BIT(13);
	 tbflag2 |= BIT(14);
  }
 else
 {
   tbflag2 &=~ BIT(14);
	 tbflag2 |= BIT(13);
 }
/****************��Ƶ��ʾ�л�***************/
if(flag & BIT(av_turn))
  {
    tbflag3 |= BIT(13);
    tbflag3 &=~ BIT(12);
	
	//tbflag3 &=~ BIT(14);
    //tbflag3 &=~ BIT(15);
  }
 else
 {
    
	tbflag3 &=~ BIT(13);
	tbflag3 |= BIT(12);
	//??
    //tbflag3 |= BIT(14);
    //tbflag3 |= BIT(15);
 }


/*******************����***********************/
if(flag & BIT(bp_bj))
  {
    tbflag2 &=~ BIT(15);
  }
 else
 {
   tbflag2 |= BIT(15);
 }

 if(fd_time>=3)
{
  fd_bj();
}
else
{tbflag4=0;}

alert_bj();
}



void alert_bj(void)
{
  if((flag&BIT(bp_bj))&&(bj_flag & 0x007f)&&(fdj_rev>300))
		{
			if(bj_delay600MS <=3)
			{
				P_BP &=~ BIT(L_BP);//�ط�����
			}
			else
			{
				P_BP |= BIT(L_BP);//��������
			}
		}
		else
		{
			//if((flag&BIT(bp_bj))&&(bj_flag2 & 0x01)&&(fdj_rev>300))//����
		if((flag&BIT(bp_bj))&&(bj_flag2 & 0x01))//����

			{
			  if(bj_delay12s <=3)
			 {
				
				P_BP &=~ BIT(L_BP);//�ط�����
			 }
			 else
			  {
				P_BP |=  BIT(L_BP);//��������
			  }
			}
			else
			{
			  P_BP |= BIT(L_BP);//�ط�����
			}
			
		}
  
}

void fd_bj(void)
{
  switch(bj_ch)
	{
	case 0:
 		 if(flag & BIT(kzq))
		 {tbflag4=8;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	case 1:
	     if(bj_flag & BIT(bj_sw)) 
		 {tbflag4=1;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	case 2:
	     if(bj_flag & BIT(bj_yy))
		 {tbflag4=2;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	case 3:
	     if(bj_flag & BIT(bj_yw))
		 {tbflag4=3;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	case 4:
	     if(kgflag1 & BIT(klbp)) 
		 {tbflag4=4;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	case 5:
	     if(kgflag2 & BIT(plock)) 
		 {tbflag4=5;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	case 6:
	
	     if(kgflag2 & BIT(ilockstate))
		 {tbflag4=6;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 bj_ch++;
		 break;
	case 7:
	     if(bj_flag2 & BIT(bj_yl))  
		 {tbflag4=7;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	case 8:
	     if(kgflag1 & BIT(handbp))
		 {tbflag4=9;}
		 else
		 {
		   tbflag4=0;
		   bj_ch++;
		 }
		 break;
	default:
		 bj_ch=0;
		 break;
	}
}




#endif