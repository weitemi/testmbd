/*
 * Self_Check.c
 *
 *  Created on: 2018��9��8��
 *      Author: Administrator
 */

#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "CAN1.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "CAN_TX.h"
#include "CAN_RX.h"
#include "CAN1_RX.h"
#include "Data_Hand.h"
#include"EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"
#include "Self_Check.h"

uint8_t Slef_Check_Time = 0;//�Լ�ʱ��

uint8_t T_100ms=0;//100ms�ļ��ʱ��
uint8_t Stack_State=0;//���״̬
uint8_t R_Stack_State=0;//���ܽ�����Ƶĵ��״̬
void Self_Check(void)
{
	if(T_100ms)
	  {
		T_100ms = 0;
		Slef_Check_Time++;
	    if(Slef_Check_Time <=200)//���20���Լ�ͨ����ͨ����˵��������
	    {
	      if((ZD_Pump==0)&&(ZD_H_Pump==0)&&(ZD_JYF==0)&&(ZD_KQBYF==0)&&(ZD_SJKYJ1==0)&&(ZD_SJKYJ2==0)&&
	    		  (ZD_DC1==0)&&(ZD_DC2==0)&&(CVM1_Flag==0)&&(CVM2_Flag==0))//���յ��ı���ȫ���յ�
	      {
	        if((Fault_Level_Three==0)&&(Fault_Level_Four==0))//�������ļ�����
	        {
	        	Stack_State|=0x02;//�Լ�ɹ�
	            JWQ_Enable = 0X01;//���������� �Լ���ɱ��ֽ������ر�
	        	Slef_Check_Time=0;
	        }
	      }
	    }
	    else //�Լ�ʧ�� �����ⲻ������
	    {
	    	Stack_State=0;
	    	Slef_Check_Time=0;
	    	if(ZD_Pump!=0)fualt(40105);//�ϱ�������
	    	if(ZD_H_Pump!=0)fualt(40106);//�ϱ�������
	    	if(ZD_JYF!=0)fualt(40107);//�ϱ�������
	    	if(ZD_KQBYF!=0)fualt(40108);//�ϱ�������
	    	if(ZD_SJKYJ1!=0)fualt(40109);//�ϱ�������
	    	if(ZD_SJKYJ2!=0)fualt(40110);//�ϱ�������
	    	if(ZD_DC1!=0)fualt(40111);//�ϱ�������
	    	if(ZD_DC2!=0)fualt(40112);//�ϱ�������
	    	if(CVM1_Flag!=0)fualt(40113);//�ϱ�������
	    	if(CVM2_Flag!=0)fualt(40114);//�ϱ�������
//	    	if(ZD_CVM3!=0)fualt(40115);//�ϱ�������
//	    	if(ZD_CVM4!=0)fualt(40116);//�ϱ�������
	    	if(Fault_Level_Three!=0)fualt(40117);//�ϱ�������
	    	if(Fault_Level_Four!=0)fualt(40118);//�ϱ�������
	    	Fault_Level_Four=0x2000;//�Լ�ʧ�ܹ��ϵȼ�
	    }
	  }

}

