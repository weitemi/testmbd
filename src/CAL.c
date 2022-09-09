/*
 * CAL.c
 *
 *  Created on: 2018��9��5��
 *      Author: Administrator
 */

#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "can.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "Data_Hand.h"
#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "Data_Hand.h"
#include "EXTERNAL_EEPROM.h"
#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "Data_Hand.h"
#include "CAL.h"
#include "Fault.h"

#include <stdint.h>
#include <stdbool.h>


uint8_t CAL_Flag=0;
uint8_t CAL_Data[256]={0};


uint8_t D0=0;
uint16_t CAL_Check_Data=0;
uint8_t CBD_Read_Flag=0;//������ȡ��־λ
uint8_t GBD_Read_Flag=0;//���϶�ȡ��־λ
uint16_t S_RUN_Count_Data=0;//��ȡ���д���

void CAL_Read_Data(void)
{
	uint8_t i=0;

	EX_EEPROM_C256_read(0,CAL_Data,256);

	if((CAL_Data[0]!=0)&&(CAL_Data[56]!=0X38))//˵����û�б궨
	{
		while(!((CAL_Data[0]==0)&&(CAL_Data[56]==0X38)))
		{
		  if(Flag_10ms)
		  {
			Flag_10ms=0;
//			CAN0_Receive_COM();
			//REN_CAN_Send(ID_Relay_State,CAN0_TX_MAILBOX3,REL_State1,REL_State2,REL_State3,0,0,0,0,CAL_Flag); //���ͼ̵���״̬ 0x18F10201
			CAL_Flag = 0;//������֮��������ֹ���ǵ���
		  }
		}
		Erase_GU_Data();//���������
	}
	for(i=0;i<8;i++)
	{
		gu_data2[i]=CAL_Data[i+196];
		if((i%2)==0)
		{
		    if(gu_data2[i]!=0)
			{
				GZ[0]=i+2;
			}
		}
	}
	for(i=0;i<8;i++)
	{
		gu_data3[i]=CAL_Data[i+204];
		if((i%2)==0)
		{
		    if(gu_data3[i]!=0)
			{
				GZ[1]=i+2;
			}
		}
	}
	for(i=0;i<8;i++)
	{
		gu_data4[i]=CAL_Data[i+212];
		if((i%2)==0)
		{
		    if(gu_data4[i]!=0)
			{
				GZ[2]=i+2;
			}
		}
	}
	for(i=0;i<8;i++)
	{
		gu_data5[i]=CAL_Data[i+220];
		if((i%2)==0)
		{
		    if(gu_data5[i]!=0)
			{
				GZ[3]=i+2;
			}
		}
	}
//	S_JYF_Data[0] |= CAL_Data[30];
//	S_JYF_Data[1] = CAL_Data[31];
	S_RUN_Count_Data = CAL_Data[28]*256+CAL_Data[29];// ��ȡ���д���
}

//�����궨����
void CAL_C_Data(void)
{
	D0 = R_CBD_Data[0];
	if(D0==0XEF)//У��
	{
		if(CAL_Check_Data==(R_CBD_Data[1]*256+R_CBD_Data[2]))//У��ɹ�
		{
			CAL_Flag = 0X04;
		}
		else
		{
			CAL_Flag = 0X08;
		}
		CAL_Check_Data =0;
	}
	else if(D0==0XFE)//������ȡ
	{
		CBD_Read_Flag=1;
	}
	else
	{
		CAL_Data[D0]=R_CBD_Data[0];
		CAL_Data[D0+1]=R_CBD_Data[1];
		CAL_Data[D0+2]=R_CBD_Data[2];
		CAL_Data[D0+3]=R_CBD_Data[3];
		CAL_Data[D0+4]=R_CBD_Data[4];
		CAL_Data[D0+5]=R_CBD_Data[5];
		CAL_Data[D0+6]=R_CBD_Data[6];
		CAL_Data[D0+7]=R_CBD_Data[7];
		CAL_Check_Data = CAL_Check_Data + R_CBD_Data[1] + R_CBD_Data[2] + R_CBD_Data[3] +R_CBD_Data[4]+R_CBD_Data[5]
		+R_CBD_Data[6]+R_CBD_Data[7];
		EX_EEPROM_C256_write(R_CBD_Data[0],R_CBD_Data,8);
	}
}
//���ϲ����궨����
void CAL_G_Data(void)
{
	D0 = R_GBD_Data[0];
	if(D0==0XEF)//У��
	{
		if(CAL_Check_Data==(R_GBD_Data[1]*256+R_GBD_Data[2]))//У��ɹ�
		{
			CAL_Flag = 0X01;
		}
		else
		{
			CAL_Flag = 0X02;
		}
		CAL_Check_Data =0;
	}
	else if(D0==0XFE)//���϶�ȡ
	{
		GBD_Read_Flag=1;
	}
	else
	{
		CAL_Data[D0]=R_GBD_Data[0];
		CAL_Data[D0+1]=R_GBD_Data[1];
		CAL_Data[D0+2]=R_GBD_Data[2];
		CAL_Data[D0+3]=R_GBD_Data[3];
		CAL_Data[D0+4]=R_GBD_Data[4];
		CAL_Data[D0+5]=R_GBD_Data[5];
		CAL_Data[D0+6]=R_GBD_Data[6];
		CAL_Data[D0+7]=R_GBD_Data[7];
		CAL_Check_Data = CAL_Check_Data + R_GBD_Data[1] + R_GBD_Data[2] + R_GBD_Data[3] +R_GBD_Data[4]+R_GBD_Data[5]
		+R_GBD_Data[6]+R_GBD_Data[7];
		EX_EEPROM_C256_write(R_GBD_Data[0],R_GBD_Data,8);
	}
}






