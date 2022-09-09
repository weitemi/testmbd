/*
 * Close.c
 *
 *  Created on: 2018��9��10��
 *      Author: Administrator
 */
#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "CAN_TX.h"
#include "CAN_RX.h"
#include "Data_Hand.h"
#include "EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"
#include "Self_Check.h"
#include "Start_Up.h"
#include "Close.h"

uint8_t time111 = 0;
uint8_t In_DC_Flag=0;
uint8_t closed_flag=0;//�ػ�������ɱ�־λ
uint8_t DC_Start_Flag=0;
uint8_t Normal_Close_Flag=0;//��ʱ�ػ���־λ
uint16_t Normal_Close_Time=0;//�ػ���ʱ
uint8_t LowTemp_FAN_Flag=0;//���·������б�־λ
/*************************************************************/
/*                        �µ�����                                                */
/*************************************************************/
void Normal_Shutdown(void)
{
	static uint16_t DC_Time=0;//��ʱ�µ����
	static uint8_t Open_Flag=1;//��ʱ�ػ���־λ����
	static uint8_t Storage_Tank_Flag=0;//�洢����ѹ����־λ
	//  uint8_t Storage_Tank_valve[2]={0,0};//����ѹ���洢������
	uint8_t Save_RUN_Count_Data[2]={0,0}; //�洢���д���
	static uint8_t H2_In_Time=0;//������ڷ��ر���ʱ


	if(T_100ms==1)
	{
		T_100ms=0;

		Normal_Close_Time++;
		if(LowTemp_FAN_Flag == 1)//�ǵ���ʱ���µ�ʱֱ�ӽ���
		{
			if(((FCU_T1_Data>85)||(FCU_T4_Data>90))&&((Stack_State&0X10)==0X10))//�������ˮ�¶���Ҫ����60
			{
			  JWQ_Enable = 0X00;//������ͣ��
			  REL_Auto[12]=1;//������Ϊ�ֶ�����
			  Fan_Speed_Data = 60;//ת����Ϊ60%
			}
			else
			{
			  REL_Auto[12]=0;//������Ϊ�Զ�����
			  Fan_Speed_Data=0;//����ת������Ϊ0
			}
		}
		if(Storage_Tank_Flag==0)//��Ҫ�洢����ѹ����EEPROM����
		{
			Storage_Tank_Flag=1;
			S_RUN_Count_Data++;
			Save_RUN_Count_Data[0]=S_RUN_Count_Data/256;//���ֽ�
			Save_RUN_Count_Data[1]=S_RUN_Count_Data%256;//���ֽ�
			EX_EEPROM_C256_write(28,Save_RUN_Count_Data,2);
		}
		if(DC_Start_Flag==0)//Ϊ����ͣ������ֻ��һ�β���ֵ�ֶ�����
		{
			DC_Start_Flag =1;
			R_DC_MAN_Data[0] = 0X1B;//DC����Ϊ�ֶ�����״̬��  ����
		}

		if(DC_Target_I>=30)
		{
			DC_Target_I-=1;//Ŀ�����ÿ�μ���2A
		}
		else
		{
			if(closed_flag <= 1)//��Ӵ�����ԭ�򣺷�ֹ����������DC�ѹػ���������ڹرպ�closed_flag��־λ��2����DCһֱ���������ŵ��޷��л����ػ�ģʽ��
			{
				if((Normal_Close_Time >= 10)&&(LowTemp_Close_Flag == 1))
				{
					DC_Target_I=20;

					if(Normal_Close_Time >= 3580)
					{
						LowTempSpeedCompensate = 0;//�����µ�ת�ٲ�������
					}
					if(Normal_Close_Time >= 3600)
					{
						LowTemp_FAN_Flag = 1;
						S_SJKYJ_State=0X05;//��ѹ���ر� ��ʹ��
						REL_KG(H_Out,0X02);//����������
						Out_H_Flag =1;//�Զ������־λ��1
						DC_Target_I=0;

						if(Normal_Close_Time >= 3660)
						{
							REL_KG(H_Out,0X01);//�ر���������1
							REL_KG(H_GYH,0X01);//�ر���������2
							Out_H_Flag=0;//���Զ�������־λ
							S_U_Flag|=0X01;
							H_PUMP_Enable = 0X00;//����ѭ���ò�ʹ��

							R_DC_MAN_Data[0] = 0XFC;//DC����Ϊ �����ŵ�
							DC_Target_I=5;
						}
					}
				}
				else//�ǵ��»���ֱ�ӷŵ�
				{
					R_DC_MAN_Data[0] = 0XFC;//DC����Ϊ �����ŵ�
					DC_Target_I=3;
					LowTemp_FAN_Flag = 1;
				}
			}
		}

		if((S_U_Flag==0)&&(DC_Target_I<=10)&&(LowTemp_Close_Flag == 0))//��ɨδ��ɱ�־λ����DCĿ�����С�ڵ���10A��ʱ��ʼ�ſ�
		{
			H_Emp();
		}

		//DC�����ѹС��30V����  ����һ��֮��Ϳ���һֱ�ػ��ˣ���ֹ�������ŵ�תΪ30V����
		if(((DC_IN_V<60)||(In_DC_Flag==1))&&((S_U_Flag&0X01)==0X01)&&(closed_flag<=1))
		{
			In_DC_Flag=1;
			S_DC_State = 0X05;//�ػ�
			R_DC_MAN_Data[0] = 0X05;//DC����Ϊ�Զ����ƹػ�
			DC_Target_I=0;
			PTC_test_Time++;
			closed_flag=1;
//			REL_KG(H_IN,0X01);//�ر��������
		}
		else if(((DC_IN_V>400)&&(In_DC_Flag==0)&&(S_U_Flag==1))||((DC_Fault&0XC0)!=0))//DC�����ѹ����40V ����DC�й��ϵ�ʱ������תΪ�����ŵ�
		{
			R_DC_MAN_Data[0] = 0XFC;//DC����Ϊ�ֶ����� �����ŵ�
			DC_Target_I=3;
		}
		else
		{

		}
		if(((S_U_Flag&0X01)==0X01)&&(LowTemp_Close_Flag == 0))//��ɨ���
		{
			S_SJKYJ_State=0X05;//��ѹ���ر� ��ʹ��
			H_PUMP_Enable = 0X00;//����ѭ���ò�ʹ��
			REL_KG(Tank_valve,0X01);//�ر�ƿ�ڷ�  ����ѹ����������0.2MPA֮���ٹ��������
		}
		if((closed_flag==1)||((DC_Fault&0XC0)!=0))
		{
			H2_In_Time++;
			if(H2_In_Time>=200)
			{
				REL_KG(H_IN,0X01);//�ر��������
				closed_flag=2;
				H2_In_Time=0;
			}
		}
		if((FCU_T1_Data<=85)&&(FCU_T4_Data<=90)&&(closed_flag==2))//�������ˮ�¶���ҪС��40
		{
			REL_Auto[12]=0;//������Ϊ�Զ�����
			Fan_Speed_Data=0;//����ת������Ϊ0
			if(DC_IN_V<40)//����ʹ��DC�ɼ����ܵ�ѹ
			{
				if((S_U_Flag&0X01)==0X01)//��ɨ��ɱ�־λ
				{
					Storage_Tank_Flag=0;//�洢����ѹ����־λ
					Normal_Close_Time=0;
					H2_In_Time =0;
					DC_Time=0;
					LowTemp_Close_Flag = 0;//���¹ػ���־λ����
					LowTemp_FanCon_Flag = 0;//���·����л����Ʊ�־λ
					LowTempSpeedCompensate = 0;//�����µ�ת�ٲ���
					LowTempAngleCompensate= 0;//��ѹ�������µ�ǶȲ���
					Emergency_Shutdown();
				}
			}
		}
		if(((Normal_Close_Time>=6000)&&(Normal_Close_Time<=6050))||(Normal_Close_Time>=140&&VCU_Ready_State==0x05)) //����14�� ���� VCU���ͽ���ͣ��ָ��
		{
			DC_Time=0;
			Emergency_Shutdown();
		}
	}
}
/*************************************************************/
/*                        ����״̬                                                 */
/*************************************************************/
void Standby(void)
{
	if(T_100ms==1)
	{

	}
}
/*************************************************************/
/*                        �����ػ�����                                                */
/*************************************************************/
void Emergency_Shutdown(void)
{
	static uint16_t Time=0;


	S_SJKYJ_State=0X05;//��ѹ���ر� ��ʹ��
	H_PUMP_Enable = 0X00;//����ѭ���ò�ʹ��
	PUMP_Enable = 0X00;//��ѹˮ�ò�ʹ��
	REL_KG(Stack_Pump,0X01);//�رո���ϵͳˮ��
//	REL_KG(FZ_Fan,0X01);//�رո���ϵͳ����
	REL_KG(H_Out,0X01);//�ر���������1
	REL_KG(H_GYH,0X01);//�ر���������2
	JWQ_Enable = 0X00;//������ͣ��
	PTC_Enable = 0X00;//PTC�ر�
	REL_KG(H_IN,0X01);//�ر��������
	REL_Auto[12]=0;//������Ϊ�Զ�����
	Fan_Speed_Data=0;//����ת������Ϊ0
	Flag_6S=0;
	Flag_12S=0;
	StartUpCurrent_Flag = 0;
	LowTemp_Close_Flag = 0;//���¹ػ���־λ����
	LowTemp_FanCon_Flag = 0;//���·����л����Ʊ�־λ
	LowTempSpeedCompensate = 0;//�����µ�ת�ٲ���
	LowTempAngleCompensate= 0;//��ѹ�������µ�ǶȲ���

	Time++;
	if(Time>=20)//2�����Ժ�رղ�����Դ
	{
		DC_Target_Old=0;
		S_U_Flag=0;//�Լ�����㣬��Ϊ�ղ����⽫����1��
		Out_H_Flag=1;//��Ϊ�ֶ�����
		Stack_State=0;//���״̬���
		In_DC_Flag=0;//��ֹ�����ɹػ��������ŵ��л�
		DC_Start_Flag=0;
		closed_flag=0;
		Normal_Close_Time=0;//�ػ���ʱ����
		Normal_Close_Flag=0;
		Time = 0;
//		S_SJKYJ_State=0XFE;//��ѹ���رպ���й��ϸ�λ��Ŀ�ģ�������ֶ�������ʱ���������

		if(ON_Flag==0)//ON���ر�
		{
			HProtectFlag_6S = 0;//���ڴ�λ������Ŀ�ģ��ػ��Ժ�Կ�ײ��ر�ʱ����ѹ��������������������
			HProtectFlag_10S = 0;//
//			REL_KG(BMS_ON,0X01);//�ر�BMSʹ���ź�
			REL_KG(Part_Power,0X01);//�رղ�����Դ
			PINS_DRV_ClearPins(GPIO_PTE,1<<E_SYS_Power);//�ر��Ի���
		}
	}
}










