/*
 * Start_Up.c
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
#include "CAN1_TX.h"
#include "CAN_RX.h"
#include "Data_Hand.h"
#include"EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"
#include "RUN.h"
#include "Self_Check.h"
#include "Start_Up.h"

uint8_t S_U_Flag=0;//���������־λ �µ��֮ͣ�������
uint8_t H_Emp_Time=0;//�������ſ�ʱ��
uint8_t LowTempStart_Flag=0;//����������־λ
uint8_t LowTemp_Close_Flag=0;//���¹ػ���־λ



/*************************************************************/
/*                    �����������ſպ���                     */
/*************************************************************/
void H_Emp(void)
{
  REL_KG(H_Out,0X02);//����������
  Out_H_Flag =1;//�Զ������־λ��1
  H_Emp_Time++;
  if(H_Emp_Time>=30)//�ſ�3S
  {
	  H_Emp_Time = 0;
	  REL_KG(H_Out,0X01);//�ر���������1
	  REL_KG(H_GYH,0X01);//�ر���������2
	  Out_H_Flag=0;//���Զ�������־λ
	  S_U_Flag|=0X01;
  }
}
/*************************************************************/
/*                        ��������                           */
/*************************************************************/
void Start_Up(void)
{
	static uint8_t Time_Start=0;


	if(T_100ms==1)
	{
		T_100ms=0;
		if((S_U_Flag&0X03)==0X03)//�����ɹ�
		{
			Time_Start=0;//����Щ��־λ���㣬�´�����
			Out_H_Flag=0;//���Զ�������־λ
			Stack_State|=0X08; //��������
			StartUpCurrent_Flag = 0;
			S_U_Flag=0;
		}
		else
		{
			if((FCU_T1_Data < 45) || (FCU_T2_Data < 45))
			{
				LowTempStart_Flag = 1;
				LowTemp_Close_Flag = 1;
			}
			if(FCU_T2_Data > 75)
			{
				REL_KG(Solenoid_Heat,0X01);//�ر������ŷ�����
			}
			if((FCU_T1_Data > 50)&&(FCU_T2_Data > 50))
			{
				LowTempStart_Flag = 0;
			}

			if(LowTempStart_Flag == 0)
			{
				S_SJKYJ_State=0X0B;//��ѹ��ʹ��
				SJKYJ_Target_Speed = 31000;//��ʼת��31000
				H_PUMP_Enable = 0X01;//����ѭ����ʹ��
				PUMP_Enable = 0X01;//��ѹˮ��ʹ��
				Pump_Target_Speed = 2000;//��ѹˮ��2000ת
				REL_KG(Stack_Pump,0X02);//�򿪸���ϵͳˮ��
				REL_KG(H_IN,0X02);//���������
//				PTC_Enable = 0X00;//PTC�ر�
//				PTC_Target_Temp = 40;//0�棬��40��ƫ����
//				PTC_Target_PWM = 0;//PWM���̶�ֵ0
				Time_Start++;
				if((S_U_Flag&0X01)!=0X01)//�������ſ�
				{
					S_U_Flag|=0X01;
				}
				if(Time_Start>=200)//����ѹ�����ſգ�һ��20����ʱ��
				{
					S_U_Flag|=0X02;
				}
			}
			else
			{
				PUMP_Enable = 0X01;//��ѹˮ��ʹ��
				Pump_Target_Speed = 2000;//��ѹˮ��2000ת
				REL_KG(Stack_Pump,0X02);//�򿪸���ϵͳˮ��
				PTC_Enable = 0X03;//PTC�������������¶ȿ���
				PTC_Target_Temp = 120;//80�棬��40��ƫ����
				PTC_Target_PWM = 100;//PWM���̶�ֵ100
				if(FCU_T2_Data <= 55)
				{
					REL_KG(Solenoid_Heat,0X02);//�������ŷ�����
				}
			}
		}
	}
}
