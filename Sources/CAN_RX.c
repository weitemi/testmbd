/*
 * CAN_RX.c
 *
 *  Created on: 2018��8��27��
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
#include "CAN1_RX.h"
#include "CAN_RX.h"
#include"EXTERNAL_EEPROM.h"
#include "Data_Hand.h"
#include "CAL.h"
#include "Fault.h"
#include "Self_Check.h"
#include "Temp.h"
#include <stdint.h>
#include <stdbool.h>

uint8_t R_REL_Data[8]={0,0,0,0,0,0,0,0} ;//��ż̵������������
uint8_t R_GBD_Data[8]={0,0,0,0,0,0,0,0} ;//��Ź��ϱ궨������
uint8_t R_CBD_Data[8]={0,0,0,0,0,0,0,0} ;//��Ų����궨������
uint8_t R_RW_Data[8]={0,0,0,0,0,0,0,0} ;//������ֵ�����
uint8_t R_DC_MAN_Data[8]={0,0,0,0,0,0,0,0} ;//�����ѹDC�ֶ����Ƶ�����
uint8_t R_JYFBD_Data[8]={0,0,0,0,0,0,0,0} ;//��ż�ѹ���궨������
uint8_t R_KQBYF_QXHB_PUMP_Data[8]={0,0,0,0,0,0,0,0} ;//���տ�����ѹ���ֶ���������


uint8_t R_SJKYJ_Data[8]={0,0,0,0,0,0,0,0};//���տ�ѹ���ֶ���������
uint8_t R_JWQ_PTC_6KWDC_Data[8]={0,0,0,0,0,0,0,0};//���ս������ֶ���������
uint8_t ZD_RW=0;
uint8_t Small_DC_State=0;//���ս�ѹDC״̬

//����ˮ�õ�����
uint16_t Pump_Speed=0,Pump_Voltage=0;//ˮ��ת��
uint8_t Pump_State1=0,Pump_State2=0,Pump_Current=0;//ˮ��״̬1��״̬2
uint8_t ZD_Pump=1;//��ѯˮ�ñ����Ƿ�֡��־λ
int16_t Pump_Speed_Dif_Val=0;//Ŀ��ת����ʵ��ת�ٲ�ֵ
//��������ѭ���õ�����
uint16_t H_Pump_Speed=0;//����ѭ����ת��
uint8_t H_Pump_State1=0,H_Pump_Power=0;//����ѭ����״̬1
uint8_t ZD_H_Pump=1;//��ѯ����ѭ���ñ����Ƿ�֡��־λ
int16_t H_Pump_Speed_Dif_Val=0;//����ѭ��Ŀ��ת����ʵ��ת�ٲ�ֵ
int16_t H_Pump_Voltage = 0;//�����ѹ
//����ѹ��
uint8_t JYF_State=0;
uint8_t ZD_JYF=1;//��ѯ��ѹ�������Ƿ�֡��־λ
//������ѹ������
uint8_t ZD_KQBYF=1;//��ѯ��ѹ�������Ƿ�֡��־λ
int8_t KQBYF_Dif_Angle=0;//��ѹ���Ƕ����
uint8_t KQBYF_State1=0;//��ѹ��״̬
//�Ƽ�͸����ѹ����1����
uint8_t ZD_SJKYJ1=1;//��ѯ��ѹ������1�Ƿ�֡��־λ
uint8_t SJKYJ_State=0;//��ѹ���������ǹر�״̬
uint32_t SJKYJ_Speed=0;//��ѹ��ת��
int16_t SJKYJ_Speed_Dif_Val=0;//ʵ��ת����Ŀ��ת�ٲ�ֵ
uint8_t SJKYJ_ID1_State=0;//����1��״̬ 0�������� 1�����쳣
uint16_t SJKYJ_Power=0;//��ѹ������
uint16_t SJKYJ_Bus_V=0;//��ѹ��ĸ�ߵ�ѹ
uint8_t SJKYJ_Bus_I=0;//��ѹ��ĸ�ߵ���
//�Ƽ�͸����ѹ����2����
uint8_t ZD_SJKYJ2=1;//��ѯ��ѹ������2�Ƿ�֡��־λ
uint8_t SJKYJ_Temp=0;//��ѹ���������¶�
uint8_t SJKYJ_MotorTemp=0;//��ѹ������¶�
uint8_t SJKYJ_Fualt_Class = 0;//��ѹ�����ϵȼ�
uint16_t SJKYJ_Fualt_Code3 = 0;//��ѹ��2�����ϣ����ɻָ����ϣ�
uint16_t SJKYJ_Fualt_Code2 = 0;//��ѹ��2�����ϣ��ɻָ����ϣ�
uint16_t SJKYJ_Fualt_Code1 = 0;//��ѹ��1�����ϣ��澯��
//��ѹDCDC����1����
uint8_t ZD_DC1=1;//DCDC�ı���1��֡��־λ
uint16_t DC_IN_V = 0,DC_IN_I = 0;//�����ѹ����  �����ѹ��λ1V  ���������λ1A
uint32_t Stack_P=0;//����ܹ��� ��λW
uint8_t DC_State = 0,DC_Temp=0,DC_Fault=0,DC_Fault_code=0;//��ѹ DC״̬��DC�¶�,DC���ϵȼ�,DC������
//��ѹDCDC����2����
uint8_t ZD_DC2=1;//DCDC�ı���2��֡��־λ
uint16_t DC_OUT_I = 0,DC_OUT_V = 0;//�����ѹ����   �����ѹ��λV  ���������λ0.1A
uint8_t DC_Rec_State=0;//DC���ܹ���״̬
uint16_t DC_Rec_I=0;//DC���ܹ�������
uint8_t DC_ID2_State=0;//DC����2��״̬ 0�������� 1�����쳣
//CVM1��������
uint8_t ZD_CVM11=1,ZD_CVM12=1,ZD_CVM13=1;
uint32_t Stack11_Average_Voltage =0,Stack11_L_Voltage=0,Stack1_Total_Voltage=0;
uint16_t Stack12_NumberMin = 0,Stack12_VoltageMin = 0;
uint16_t Stack12_NumberMax = 0,Stack12_VoltageMax = 0;
//CVM6��������
uint8_t ZD_CVM21=1,ZD_CVM22=1,ZD_CVM63=1;
uint32_t Stack21_Average_Voltage =0,Stack61_L_Voltage=0,Stack2_Total_Voltage=0;
uint16_t Stack22_NumberMin = 0,Stack22_VoltageMin = 0;
uint16_t Stack22_NumberMax = 0,Stack22_VoltageMax = 0;
//��ѹDCDC1��������
uint16_t DCDC_Out_voltage = 0;
uint16_t DCDC_In_voltage = 0;
uint16_t DCDC_Out_current = 0;
uint8_t DCDC_temp = 0;
uint8_t DCDC_work_state = 0;// ��ѹDC״̬
uint16_t DCDC_fault_code = 0;
uint16_t DCDC_fault_level = 0;//��ѹDC���ϵȼ�

//��ͨ����������
uint16_t Valve_State = 0;//״̬
uint16_t Valve_fault_code = 0;//������
uint16_t Valve_Open = 0;//������

// PTC
uint16_t PTC_In_voltage = 0;
uint16_t PTC_In_current = 0;
uint16_t PTC_Fault_Code = 0;

uint32_t Stack_Total_Voltage=0;//����ܵ�ѹ ����1mV
uint16_t Stack_High_Voltage=0;//��Ƭ����ѹ ����1mV
uint16_t Stack_High_Number=0;//��Ƭ����ѹ���
uint16_t Stack_Low_Voltage=0;//��Ƭ��С��ѹ ����1mV
uint16_t Stack_Low_Number=0;//��Ƭ��С��ѹ���
uint16_t Stack1_Max_V_Dif=0;//���1��Ƭ���ѹ�� ����1MV
uint16_t Stack2_Max_V_Dif=0;//���2��Ƭ���ѹ�� ����1MV
uint16_t Stack_Average_Voltage=0;//��Ƭƽ����ѹ ����1mV
uint8_t CVM1_Flag = 0,CVM2_Flag = 0;//6��ɼ������ı�־λ

//���ϲ�ѯ6����ʱ��־λ
uint8_t Flag_6S=0;
uint8_t Flag_12S=0;
uint8_t HProtectFlag_6S=0;

uint16_t Test_11=0,Test_12=0;

//����ˮ�ñ��Ĳ�ѯ���պ���
void Pump_ID_Rec(void)
{
	static uint8_t i=0;

	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_Pump) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_Pump.id == ID_REC_Pump)
		{
			ZD_Pump=0;
			Test_11++;
			Pump_Speed = recvMsg_Pump.data[1]*256+recvMsg_Pump.data[0];// ת��
			Pump_Voltage = recvMsg_Pump.data[3]*256+recvMsg_Pump.data[2]; // ��ѹ
			Pump_Current = (recvMsg_Pump.data[4]*50); // ����ΪmA
			Pump_State1 = recvMsg_Pump.data[6];
			Pump_State2 = recvMsg_Pump.data[7];
			if(Flag_6S>=60)
			{
				if((Pump_State1==0)&&((Pump_State2&0XFC)==0))//ˮ��״̬����
				{
					if(Pump_Target_Speed>=2500)//���Ŀ��ת���뵱ǰ��ȡֵ��ͬ
						Pump_Speed_Dif_Val = Pump_Target_Speed-Pump_Speed;//��ֵ����Ŀ��ת�ټ�ȥʵ��ת��
					else Pump_Speed_Dif_Val=0;

					if((Pump_Speed_Dif_Val<200)&&(Pump_Speed_Dif_Val>-200))//�����200ת���ڱ�ʾû����
					{
						i=0;//����
						if(Water_Pre_State_Flag==0)//ˮѹ������־λ
						{
							//ȫ�������������ˮ�ù��ϲ����ı�־λ
							Fault_Level_Two&=0XFFFD;
							Fault_Level_Three&=0XFFF7;
//							Fault_Level_Four&=0XFFF7;
						}
					}
					else
					{
						if((Pump_Speed_Dif_Val<500)&&(Pump_Speed_Dif_Val>-500))//�����500ת���ڶ�������
						{
							i++;
							if(i>30)//3���ӵ�ʱ��û�е�����λ
							{
								Fault_Level_Two|=0X0002;
								fualt(20501);
							}
						}
						else if((Pump_Speed_Dif_Val<1000)&&(Pump_Speed_Dif_Val>-1000))//�����1000ת������������
						{
							i++;
							if(i>30)//3���ӵ�ʱ��û�е�����λ
							{
								Fault_Level_Three|=0X0008;
								fualt(30501);
							}
						}
						else//����1000�����ļ�������
						{
							i++;
							if(i>30)//3���ӵ�ʱ��û�е�����λ
							{
								Fault_Level_Four|=0X0008;
								fualt(40501);
							}
						}
					}
				}
				else//ˮ��״̬������
				{
					if(DC_OUT_V>450)//��DC�����ѹ���ж�ĸ�ߵ�ѹ
					{
						if(((Pump_State1&0XF4)!=0)||((Pump_State2&0X90)!=0))//���ļ�����
						{
							Fault_Level_Four|=0X0008;
						}
						else if(((Pump_State1&0X0B)!=0)||((Pump_State2&0X48)!=0))//����������
						{
							Fault_Level_Three|=0X0008;
						}
						else//�Ǿ�����2��������
						{
							Fault_Level_Two|=0X0002;
						}
						if(Pump_State1!=0)
						fualt(0XF000|Pump_State1);//�ϱ�������
						if((Pump_State2&0XFC)!=0)
						{
							fualt(0XF100|(Pump_State2&0XFC));//�ϱ�������
						}
					}
				}
			}

		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_Pump, &recvMsg_Pump);
	}
}
//��������ѭ���ñ��Ĳ�ѯ���պ���
void H_Pump_ID_Rec(void)
{
   static uint8_t i=0;
   static uint8_t State_Flag=0;
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_H_Pump) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_H_Pump.id == ID_REC_H_Pump)
		{
			ZD_H_Pump=0;
			H_Pump_Speed = recvMsg_H_Pump.data[2]*256+recvMsg_H_Pump.data[1];
			H_Pump_Power = recvMsg_H_Pump.data[3]*10; //�������������
			H_Pump_State1 = recvMsg_H_Pump.data[0];
			H_Pump_Voltage = recvMsg_H_Pump.data[4]*5;//�����ѹ
			if(Flag_6S>=60)
			{
				if((H_Pump_State1==0x55)||(H_Pump_State1==0xAA))
				{
					if(H_PUMP_Enable==0X01)//����ʹ��
					{
						H_Pump_Speed_Dif_Val = H_Pump_Target_Speed-H_Pump_Speed;//��ֵ����Ŀ��ת�ټ�ȥʵ��ת��
					}
					if((H_Pump_Speed_Dif_Val<500)&&(H_Pump_Speed_Dif_Val>-500))//�����200ת���ڱ�ʾû����
					{
						i=0;
						//ȫ�������������ˮ�ù��ϲ����ı�־λ
						Fault_Level_Two&=0XFFF7;
						Fault_Level_Three&=0XFFEF;
//						Fault_Level_Four&=0XFFEF;
					}
					else
					{
						if((H_Pump_Speed_Dif_Val<1000)&&(H_Pump_Speed_Dif_Val>-1000))//�����500ת���ڶ�������
						{
							i++;
							if(i>40)//��3���ӵ���ʱ��
							{
								Fault_Level_Two|=0X0008;
								fualt(20701);
							}
						}
						else if((H_Pump_Speed_Dif_Val<1500)&&(H_Pump_Speed_Dif_Val>-1500))//�����1000ת������������
						{
							i++;
							if(i>40)//��3���ӵ���ʱ��
							{
								Fault_Level_Three|=0X0010;
								fualt(30701);
							}
						}
						else//����1500�����ļ�������
						{
							i++;
							if(i>40)//��4���ӵ���ʱ��
							{
//								Fault_Level_Four|=0X0010;
								fualt(40701);
							}
						}
					}
				}
				else
				{
					State_Flag=1;
					if((H_Pump_State1&0XF6)!=0)//���ļ�����
					{
//						Fault_Level_Four|=0X0010;
					}
					else
					{
						Fault_Level_Three|=0X0010;
					}
					fualt(0XF200|H_Pump_State1);//�ϱ�������
				}
				if(State_Flag>=1)
				{
					if((Stack_State&0X0F)>=0X07)//���ڴ������л���������
					{
						H_PUMP_Enable=0X00;//���Ͳ�ʹ��
						State_Flag++;
						if(State_Flag>=30)
						{
							State_Flag=0;
							H_PUMP_Enable=0X01;//����ʹ��
						}
					}

				}

			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_H_Pump, &recvMsg_H_Pump);
	}
}
//����ѹ�����Ĳ�ѯ����
void JYF_ID_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_JYF) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_JYF.id == ID_REC_JYF)
		{
			ZD_JYF=0;
			Stack1_Air_Pre = recvMsg_JYF.data[1]+recvMsg_JYF.data[0]*256;
			Stack1_H_Pre = recvMsg_JYF.data[3]+ recvMsg_JYF.data[2]*256;
			JYF_State = recvMsg_JYF.data[5];
			if((JYF_State&0X30)==0)//�޹���
			{
				Fault_Level_Two&=0XFFEF;
				Fault_Level_Three&=0XFFDF;
//				Fault_Level_Four&=0XFFDF;
			}
			else
			{
				if((JYF_State&0X20)==0X20)//����������3�������Ϲ���
				{
				   //����ѹ�����͹ر�����
				   Fault_Level_Four|=0X0020;
				   REL_KG(H_IN,0X01);//�ر��������
				   S_JYF_Data[0]&=0X3F;
				   S_JYF_Data[0]|=0X40; //���͸��ر�״̬
				}
				else
				{
					Fault_Level_Three|=0X0020;
				}
				fualt(0XF300|JYF_State);
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_JYF, &recvMsg_JYF);
	}
}
//������ѹ�����Ĳ�ѯ����
void KQBYF_ID_Rec(void)
{
   static uint8_t i=0;
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_KQBYF) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_KQBYF.id == ID_REC_KQBYF)
		{
			ZD_KQBYF=0;

			KQBYF_State1 = recvMsg_KQBYF.data[3];
			if(Flag_6S>=60)
			{
				if((KQBYF_State1&0X1E)==0)//�޹���
				{
					KQBYF_Dif_Angle = KQBYF_Target_Angle-recvMsg_KQBYF.data[1];
					if(KQBYF_Dif_Angle==0)//ȫ��û�����⣬�������
					{
						i=0;
						Fault_Level_Two&=0XFFDF;
						Fault_Level_Three&=0XFFBF;
//						Fault_Level_Four&=0XFFBF;
					}
					else if((KQBYF_Dif_Angle<10)&&(KQBYF_Dif_Angle>-10))
					{
						i++;
						if(i>=30)
						{
							Fault_Level_Two|=0X0020;
							fualt(20601);
						}
					}
					else if((KQBYF_Dif_Angle<20)&&(KQBYF_Dif_Angle>-20))
					{
						i++;
						if(i>30)
						{
							Fault_Level_Three|=0X0040;
							fualt(30601);
						}
					}
					else
					{
						i++;
						if(i>30)
						{
							Fault_Level_Four|=0X0040;
							fualt(40601);
						}
					}
				}
				else
				{
					i++;
					if(i>30)
					{
						Fault_Level_Four|=0X0040;
						fualt(0XF900|KQBYF_State1);//�ϱ�������
					}

				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_KQBYF, &recvMsg_KQBYF);
	}
}
//��ѹ������1��ѯ����
void SJKYJ_ID1_Rec(void)
{
   static uint8_t i=0;
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_SJKYJ1) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_SJKYJ1.id == ID_REC_SJKYJ1)
		{
			ZD_SJKYJ1=0;
			SJKYJ_Power = SJKYJ_Bus_V * SJKYJ_Bus_I;
			SJKYJ_Speed = (recvMsg_SJKYJ1.data[2]<<16)+(recvMsg_SJKYJ1.data[1]<<8)+(recvMsg_SJKYJ1.data[0]);//��ѹ��ת��
			SJKYJ_Bus_I = recvMsg_SJKYJ1.data[3]/2;//�ֱ���0.5  ƫ����0
			SJKYJ_Bus_V = recvMsg_SJKYJ1.data[4]+(recvMsg_SJKYJ1.data[5]<<8);//ĸ�ߵ�ѹ  �ֱ���1 ƫ����0
			SJKYJ_Temp = recvMsg_SJKYJ1.data[6];//��ѹ���������¶���-40��ƫ��
			SJKYJ_MotorTemp = recvMsg_SJKYJ1.data[7];//��ѹ������¶���-40��ƫ��


			if(Flag_12S>=120)
			{
				if(SJKYJ_Speed>31000)
	            SJKYJ_Speed_Dif_Val = SJKYJ_Target_Speed-SJKYJ_Speed;//Ŀ��ת����ʵ��ת�ٲ�
				else SJKYJ_Speed_Dif_Val=0;


				if((SJKYJ_Speed_Dif_Val<1000)&&(SJKYJ_Speed_Dif_Val>-1000))
				{
					i=0;
					SJKYJ_ID1_State=0;//״̬����
				}
				else if((SJKYJ_Speed_Dif_Val<2000)&&(SJKYJ_Speed_Dif_Val>-2000))//Ŀ��ת����ʵ��ת��������
				{
					i++;
					if(i>30)
					{
						SJKYJ_ID1_State=1;//״̬�쳣
						Fault_Level_Two|=0X0040;
						fualt(20301);
					}
				}
				else if((SJKYJ_Speed_Dif_Val<5000)&&(SJKYJ_Speed_Dif_Val>-5000))//Ŀ��ת����ʵ��ת��������
				{
					i++;
					if(i>30)
					{
						SJKYJ_ID1_State=1;//״̬�쳣
						Fault_Level_Three|=0X0080;
						fualt(30301);
					}
				}
				else//��ֵ��������1000
				{
					i++;
					if(i>30)
					{
						SJKYJ_ID1_State=1;//״̬�쳣
						Fault_Level_Four|=0X0080;
						fualt(40301);
					}
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_SJKYJ1, &recvMsg_SJKYJ1);
	}
}
//��ѹ������2��ѯ����
void SJKYJ_ID2_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_SJKYJ2) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_SJKYJ2.id == ID_REC_SJKYJ2)
		{
			ZD_SJKYJ2=0;
			SJKYJ_State = recvMsg_SJKYJ2.data[0];//��ѹ��״̬
			SJKYJ_Fualt_Class = recvMsg_SJKYJ2.data[2]&0X0F;//��ѹ�����ϵȼ�
			SJKYJ_Fualt_Code3 = (recvMsg_SJKYJ2.data[2]>>4)+((recvMsg_SJKYJ2.data[3]&0X3F)<<4);//������Ϣ��ȡ
			SJKYJ_Fualt_Code2 = (recvMsg_SJKYJ2.data[3]>>6)+(recvMsg_SJKYJ2.data[4]<<2)+((recvMsg_SJKYJ2.data[5]&0X7F)<<10);//2������
			SJKYJ_Fualt_Code1 = (recvMsg_SJKYJ2.data[5]>>7)+(recvMsg_SJKYJ2.data[6]<<1)+(recvMsg_SJKYJ2.data[7]<<9);//1������

			if(Flag_6S>=60)
			{
				if(SJKYJ_Fualt_Class <= 1)//�޹��ϻ���1������
				{
					if(SJKYJ_ID1_State==0)//����1Ҳ����
					{
						Fault_Level_Two&=0XFFBF;
						Fault_Level_Three&=0XFF7F;
//						Fault_Level_Four&=0XFF7F;
					}
				}
				else if((SJKYJ_Fualt_Code3!=0) || (SJKYJ_Fualt_Class == 3))//3������
				{
					Fault_Level_Four|=0X0080;
					fualt(0XF600|(SJKYJ_Fualt_Code3&0X00FF));//�ֽ�6
					fualt(0XF700|(SJKYJ_Fualt_Code3>>8));//�ֽ�7
				}
				else//˵����2������
				{
					Fault_Level_Three|=0X0080;
					fualt(0XF400|(SJKYJ_Fualt_Code2&0XC0));//�ֽ�3
					fualt(0XF500|((SJKYJ_Fualt_Code2&0X0BC0)>>8));//�ֽ�4
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_SJKYJ2, &recvMsg_SJKYJ2);
	}
}
//��ѹDC����1��ѯ����
void DC_ID1_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_DC) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_DC.id == ID_REC_DC)
		{
			ZD_DC1=0;
			DC_IN_V = recvMsg_DC.data[0]+(recvMsg_DC.data[1]&0X0F)*256;//DC�����ѹ  1V/bit
			DC_IN_I = (recvMsg_DC.data[2]+(recvMsg_DC.data[3])*256)/10;//DC�������   0.1A/bit,����10�����յ�λΪA


			DC_State=recvMsg_DC.data[4];//DC״̬
			DC_Fault = (recvMsg_DC.data[5]);//DC���ϵȼ�
			DC_Fault_code = DC_Fault&0x3F;//DC
			DC_Temp = recvMsg_DC.data[6];//DC�¶ȣ���40��ƫ��ֵ
			Stack_P = (DC_IN_V*DC_IN_I);//��λW
			if((DC_Fault&0XC0)==0)//�޹���
			{
				if(DC_ID2_State==0)//����2Ҳ����
				{
					//��������Ĺ���λ
					Fault_Level_Three&=0XFEFF;
//					Fault_Level_Four&=0XFEFF;
				}
			}
			else if((DC_Fault&0XC0)==0X40)//1������ ��Ӧ4���ػ�����
			{
				Fault_Level_Four|=0X0100;
				fualt(0XF800|DC_Fault);
			}
			else//�������� ��Ӧ3���޹���
			{
				Fault_Level_Three|=0X0100;
				fualt(0XF800|DC_Fault);
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_DC, &recvMsg_DC);
	}
}
//��ѹDC����2��ѯ����
void DC_ID2_Rec(void)
{
	static uint8_t i=0;
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_DC2) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_DC2.id == ID_REC_DC2)
		{
			ZD_DC2=0;
			DC_OUT_V = recvMsg_DC2.data[0]+(recvMsg_DC2.data[1]&0X0F)*256;//DC�����ѹ  1V/bit
			DC_OUT_I = (recvMsg_DC2.data[2]+(recvMsg_DC2.data[3])*256)/10;//DC�������   0.1A/bit,����10�����յ�λΪA
			DC_Rec_State = recvMsg_DC2.data[4];//DC����FCU�Ŀ���״̬��������֤���Լ����͵��ǲ���һ��
			DC_Rec_I = recvMsg_DC2.data[5]+(recvMsg_DC2.data[6])*256;//DC���ܵĿ��Ƶ���

			if(Flag_6S>=60)
			{
				if((DC_Rec_State==S_DC_State)&&((DC_Rec_I*10)==S_DC_I))//DC���յ�����FCU���͵�һ�� ˵��û����
				{
					i=0;
					DC_ID2_State=0;
				}
				else//��Ϣ��һ��
				{
					i++;
					if(i>100) // 2���Ӽ�� 2021.06.16
					{
						Fault_Level_Four|=0X0100;//ֱ��4������
						fualt(40401);
						DC_ID2_State=1;
					}
				}
			}

		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_DC2, &recvMsg_DC2);
	}
}
//CVM1���Ĳ�ѯ����
void CVM11_ID_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_CVM11) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_CVM11.id == ID_CVM11)
		{
			ZD_CVM11=0;
			Stack11_Average_Voltage = (((recvMsg_CVM11.data[4]&0x0F)<<8)+recvMsg_CVM11.data[3])-1300;//�ɼ���ƽ�������ѹ   ƫ����-1300mv
			Stack1_Total_Voltage = ((recvMsg_CVM11.data[2]<<8)+recvMsg_CVM11.data[1])*0.1;//�ɼ����ܵ�ѹ      �ֱ���0.1V
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_CVM11, &recvMsg_CVM11);
	}
}

void CVM12_ID_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_CVM12) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_CVM12.id == ID_CVM12)
		{
			ZD_CVM12=0;
			Stack12_VoltageMin = (((recvMsg_CVM12.data[1]&0x0F)<<8)+recvMsg_CVM12.data[0]) - 1300;//��Ƭ��͵�ѹ
			Stack12_NumberMin = ((recvMsg_CVM12.data[3]&0x0F)<<8)+recvMsg_CVM12.data[2];//��Ƭ��͵�ѹ���
			Stack12_VoltageMax = (((recvMsg_CVM12.data[5]&0x0F)<<8)+recvMsg_CVM12.data[4]) - 1300;//��Ƭ��ߵ�ѹ
			Stack12_NumberMax = ((recvMsg_CVM12.data[7]&0x0F)<<8)+recvMsg_CVM12.data[6];//��Ƭ��ߵ�ѹ���

		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_CVM12, &recvMsg_CVM12);
	}
}

//CVM2���Ĳ�ѯ����
void CVM21_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_CVM21) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_CVM21.id == ID_CVM21)
		{
			ZD_CVM21=0;
			Stack21_Average_Voltage = (((recvMsg_CVM21.data[4]&0x0F)<<8)+recvMsg_CVM21.data[3])-1300;//�ɼ���ƽ�������ѹ   ƫ����-1300mv
			Stack2_Total_Voltage = ((recvMsg_CVM21.data[2]<<8)+recvMsg_CVM21.data[1])*0.1;//�ɼ����ܵ�ѹ      �ֱ���0.1V
			Stack_Total_Voltage = Stack1_Total_Voltage + Stack2_Total_Voltage;//2���ɼ����ܵ�ѹ
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_CVM21, &recvMsg_CVM21);
	}
}

void CVM22_ID_Rec(void)
{
	static uint8_t Time1_1=0,Time1_2=0,Time1_3=0;
	static uint8_t Time2_1=0,Time2_2=0,Time2_3=0;
	static uint8_t Time3_1=0,Time3_2=0,Time3_3=0;
	static uint8_t offest=36;

	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_CVM22) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_CVM22.id == ID_CVM22)
		{
			ZD_CVM22=0;
			Stack22_VoltageMin = (((recvMsg_CVM22.data[1]&0x0F)<<8)+recvMsg_CVM22.data[0]) - 1300;//��Ƭ��͵�ѹ
			Stack22_NumberMin = ((recvMsg_CVM22.data[3]&0x0F)<<8)+recvMsg_CVM22.data[2];//��Ƭ��͵�ѹ���
			Stack22_VoltageMax = (((recvMsg_CVM22.data[5]&0x0F)<<8)+recvMsg_CVM22.data[4]) - 1300;//��Ƭ��ߵ�ѹ
			Stack22_NumberMax = ((recvMsg_CVM22.data[7]&0x0F)<<8)+recvMsg_CVM22.data[6];//��Ƭ��ߵ�ѹ���


//			//�Ƚϳ���ѵ�Ƭ��͵�ѹ����͵�ѹ���
//			Stack_Low_Voltage = Stack12_VoltageMin;
//			Stack_Low_Number = Stack12_NumberMin;
//			if(Stack22_VoltageMin < Stack_Low_Voltage)
//			{
//				Stack_Low_Voltage = Stack22_VoltageMin;
//				Stack_Low_Number = Stack22_NumberMin + 121;
//			}
//
//
//			//�Ƚϳ���ѵ�Ƭ��ߵ�ѹ����ߵ�ѹ���
//			Stack_High_Voltage = Stack12_VoltageMax;
//			Stack_High_Number = Stack12_NumberMax;
//			if(Stack22_VoltageMax > Stack_High_Voltage)//��������ѹֵ�Ƚ�
//			{
//				Stack_High_Voltage = Stack22_VoltageMax;
//				Stack_High_Number = Stack22_NumberMax + 121;
//			}


//			Stack_Average_Voltage = (Stack11_Average_Voltage + Stack21_Average_Voltage) / 2;
			Stack1_Max_V_Dif = abs(Stack11_Average_Voltage - Stack12_VoltageMin);//��1��������      �ֱ���1mV
			Stack2_Max_V_Dif = abs(Stack21_Average_Voltage - Stack22_VoltageMin);//��2��������      �ֱ���1mV
			if(((Stack_State&0X1F)==0X0F)&&(DC_IN_I>8)&&(FCU_T1_Data>105))//��Ѵ�������״̬���ҵ�ѵ�������8A��ʱ��Ž��е�����ܵ�ѹ���
			{
				if((CVM1_Flag == 0)&&(CVM2_Flag == 0))//CVM1,2,3,4,5,6ȫ�����Ķ����������յ�
				{
					//�Ե�ѵ���ѹ���Ƭ��ͺ��ܵ�ѹ�ֵȼ�����������
//					Stack_Average_Voltage = (Stack11_Average_Voltage + Stack21_Average_Voltage) / 2;
//					Stack_Max_V_Dif = Stack_Average_Voltage - Stack_Low_Voltage;//��������      �ֱ���1mV
					if((Stack1_Max_V_Dif<(CAL_Data[127-offest]*2)) && (Stack2_Max_V_Dif<(CAL_Data[127-offest]*2)))//������ѵ���ѹ��С����С����ֵ
					{
						Time1_1=0;
						Time1_2=0;
						Time1_3=0;

					}
					else if((Stack1_Max_V_Dif>(CAL_Data[191-offest]*2)) || (Stack2_Max_V_Dif>(CAL_Data[191-offest]*2)))//������ѵ���ѹ�������С����ֵ�ļ�����
					{
						Time1_3++;
						if(Time1_3>=20)
						{
							Time1_3=20;
							Fault_Level_Four|=0X0200;
							fualt(41005);//�ϱ�������
						}
					}
					else if((Stack1_Max_V_Dif>(CAL_Data[159-offest]*2)) || (Stack2_Max_V_Dif>(CAL_Data[159-offest]*2)))//����ѹ�������С����ֵ��������
					{
						Time1_2++;
						if(Time1_2>=10)
						{
							Time1_2=10;
							Fault_Level_Three|=0X0200;
							fualt(31001);//�ϱ�������
						}
					}
					else//����ѹ�������С����ֵ��������
					{
						Time1_1++;
						if(Time1_1>=10)
						{
							Time1_1=10;
							Fault_Level_Two|=0X0200;
							fualt(21001);//�ϱ�������
						}
					}

					if((Stack12_VoltageMin>(CAL_Data[126-offest]*5)) && (Stack22_VoltageMin>(CAL_Data[126-offest]*5)))//������׵�ѹ����������Χ֮��
					{
						Time2_1=0;
						Time2_2=0;
						Time2_3=0;
					}
					else if((Stack12_VoltageMin<(CAL_Data[190-offest]*5)) || (Stack22_VoltageMin<(CAL_Data[190-offest]*5)))//��������ļ�����
					{
						 Time2_3++;
						if(Time2_3>=20)
						{
							Time2_3=20;
							Fault_Level_Four|=0X0200;
							fualt(41006);//�ϱ�������
						}
					}
					else if((Stack12_VoltageMin<(CAL_Data[158-offest]*5)) || (Stack22_VoltageMin<(CAL_Data[158-offest]*5)))//���������������
					{
						Time2_2++;
						if(Time2_2>=10)
						{
							Time2_2=10;
							Fault_Level_Three|=0X0200;
							fualt(31002);//�ϱ�������
						}
					}
					else
					{
						Time2_1++;
						if(Time2_1>=10)
						{
							Time2_1=10;
							Fault_Level_Two|=0X0200;
							fualt(21002);//�ϱ�������
						}
					}

					if((Stack1_Total_Voltage>(CAL_Data[125-offest]/2)) && (Stack2_Total_Voltage>(CAL_Data[125-offest]/2)))//�ܵ�ѹ������͹���ֵ ���й��϶�û��
					{
						Time3_1=0;
						Time3_2=0;
						Time3_3=0;
					}
					else if((Stack1_Total_Voltage<(CAL_Data[189-offest]/2)) || (Stack2_Total_Voltage<(CAL_Data[189-offest]/2)))//�ܵ�ѹ�����ļ�����
					{
						Time3_3++;
						if(Time3_3>=10)//1��������
						{
							Time3_3=10;
							Fault_Level_Four|=0X0200;
							fualt(41007);//�ϱ�������
						}
					}
					else if((Stack1_Total_Voltage<(CAL_Data[157-offest]/2)) || (Stack2_Total_Voltage<(CAL_Data[157-offest]/2)))//�ܵ�ѹ������������
					{
						Time3_2++;
						if(Time3_2>=10)
						{
							Time3_2=10;
							Fault_Level_Three|=0X0200;
							fualt(31003);//�ϱ�������
						}
					}
					else
					{
						Time3_1++;
						if(Time3_1>=10)
						{
							Time3_1=10;
							Fault_Level_Two|=0X0200;
							fualt(21003);//�ϱ�������
						}
					}
					if((Time1_1==0)&&(Time1_2==0)&&(Time1_3==0)&&(Time2_1==0)&&(Time2_2==0)&&(Time2_3==0)&&(Time3_1==0)&&(Time3_2==0)&&(Time3_3==0))
					{
						Fault_Level_Two&=0XFDFF;
						Fault_Level_Three&=0XFDFF;
					//	Fault_Level_Four&=0XFDFF;
					}
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_CVM22, &recvMsg_CVM22);
	}
}



//��ѹDCDC1���Ĳ�ѯ����
void DCDC1_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_6KWDCDC1) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_6KWDCDC1.id == ID_6KWDCDC1)
		{
			DCDC_Out_voltage = ((recvMsg_6KWDCDC1.data[3]<<8)+recvMsg_6KWDCDC1.data[2]) * 0.05; // �����ѹ
			DCDC_Out_current = ((((recvMsg_6KWDCDC1.data[5]<<8)+recvMsg_6KWDCDC1.data[4]) - 10000) * 0.05); // ������� �ȱ��˺�ֱ���
			DCDC_temp = recvMsg_6KWDCDC1.data[1]-40;// �¶�
			DCDC_work_state = recvMsg_6KWDCDC1.data[0]&0x03;//����״̬
			DCDC_fault_level = recvMsg_6KWDCDC1.data[0]&0x0C;//���ϵȼ�
			DCDC_fault_code = recvMsg_6KWDCDC1.data[6];// ������

			if(Flag_6S>=60)
			{
				if(DCDC_fault_level == 2)
				{
					Fault_Level_Four|=0X2000;
					fualt(41201);//�ϱ�������
					fualt(0XFB00|DCDC_fault_code);
				}
				else if(DCDC_fault_level == 1)
				{
//					Fault_Level_Three|=0X2000;
					fualt(31201);//�ϱ�������
					fualt(0XFB00|DCDC_fault_code);
				}
				else
				{
					Fault_Level_Three&=0XDFFF;
//					Fault_Level_Four&=0XDFFF;
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_6KWDCDC1, &recvMsg_6KWDCDC1);
	}
}

//��ͨ�����Ĳ�ѯ����
void Valve_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_ThreeWayValve) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_ThreeWayValve.id == ID_ThreeWayValve)
		{
			Valve_fault_code = ((recvMsg_ThreeWayValve.data[6]<<8)+recvMsg_ThreeWayValve.data[5]); // ������
			Valve_Open = recvMsg_ThreeWayValve.data[1];//����
			Valve_State = recvMsg_ThreeWayValve.data[0];//״̬

			if(Flag_6S>=60)
			{
				if((Valve_fault_code == 502) || (Valve_fault_code == 602))
				{
					Fault_Level_Four|=0X4000;
					fualt(41301);//�ϱ�������
				}
				else
				{
//					Fault_Level_Four&=0XBFFF;
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_ThreeWayValve, &recvMsg_ThreeWayValve);
	}
}


//PTC���Ĳ�ѯ����
void PTC_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_PTC) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_PTC.id == ID_PTC)
		{
			PTC_In_voltage = recvMsg_PTC.data[7] * 4; // �����ѹ
			PTC_In_current = recvMsg_PTC.data[6] * 0.1; // �������
			PTC_Fault_Code = recvMsg_PTC.data[4]; // ������
//			DCDC_temp = recvMsg_6KWDCDC1.data[1]-40;// �¶�

			if(Flag_6S>=60)
			{
				if(PTC_Fault_Code != 0)
				{
					Fault_Level_Two|=0X8000;
					fualt(0XFC00|PTC_Fault_Code);
					PTC_Flag = 0;
					PTC_Enable = 0X00;//PTC�ر�
					PTC_Target_Temp = 40;//0�棬��40��ƫ����
					PTC_Target_PWM = 0;//PWM���̶�ֵ0
					if((FCU_T1_Data < 45) || (FCU_T4_Data < 45))
					{
						Fault_Level_Four|=0X8000;
						fualt(40801);//�ϱ�������
					}
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_PTC, &recvMsg_PTC);
	}
}

//�������Ķ�ʧ��� 100ms
void ID_Lose_Check(void)
{
	//ֻ��ͨ���Լ�֮��ſ��Խ��ж�֡���
	if((Stack_State&0X03)==0X03)
	{
		//����ˮ�ñ��Ķ�֡���
		ZD_Pump++;
		if(ZD_Pump>=20)//2�����ղ�������
		{
		   ZD_Pump=20;//��ֹһֱ�����
		   Fault_Level_Four|=0X0008;//�ϱ��ļ�����
		   fualt(40502);//�ϱ�������
		}
		//����ѭ���ñ��Ķ�֡���
		ZD_H_Pump++;
		if(ZD_H_Pump>=20)//2�����ղ�������
		{
		   ZD_H_Pump=20;//��ֹһֱ�����
		   Fault_Level_Four|=0X0010;//�ϱ��ļ�����
		   fualt(40702);//�ϱ�������
		}
		//����ѹ�����Ķ�֡���
		ZD_JYF++;
		if(ZD_JYF>=20)//2�����ղ�������
		{
		   ZD_JYF=20;//��ֹһֱ�����
		   Fault_Level_Four|=0X0020;//�ϱ��ļ�����
		   fualt(40901);//�ϱ�������
		}
		//��ѹ�����Ķ�֡���
		ZD_KQBYF++;
		if(ZD_KQBYF>=50)//2�����ղ�������
		{
		   ZD_KQBYF=50;//��ֹһֱ�����
		   Fault_Level_Four|=0X0040;//�ϱ��ļ�����
		   fualt(40602);//�ϱ�������
		}
		//�Ƽ�͸����ѹ����1��֡���
		ZD_SJKYJ1++;
		if(ZD_SJKYJ1>=20)//2�����ղ�������
		{
		   ZD_SJKYJ1=20;//��ֹһֱ�����
		   Fault_Level_Four|=0X0080;//�ϱ��ļ�����
		   fualt(40302);//�ϱ�������
		}
		//�Ƽ�͸����ѹ����2��֡���
		ZD_SJKYJ2++;
		if(ZD_SJKYJ2>=100)//2�����ղ�������
		{
		   ZD_SJKYJ2=100;//��ֹһֱ�����
		   Fault_Level_Four|=0X0080;//�ϱ��ļ�����
		   fualt(40303);//�ϱ�������
		}
		//��ѹDC����1��֡���
		ZD_DC1++;
		if(ZD_DC1>=20)//2�����ղ�������
		{
		   ZD_DC1=20;//��ֹһֱ�����
		   Fault_Level_Four|=0X0100;//�ϱ��ļ�����
		   fualt(40402);//�ϱ�������
		}
		//��ѹDC����2��֡���
		ZD_DC2++;
		if(ZD_DC2>=20)//2�����ղ�������
		{
		   ZD_DC2=20;//��ֹһֱ�����
		   Fault_Level_Four|=0X0100;//�ϱ��ļ�����
		   fualt(40403);//�ϱ�������
		}
		//CVM1���Ķ�֡���
		ZD_CVM11++;
		ZD_CVM12++;
		if((ZD_CVM11>=50)||(ZD_CVM12>=50))//5�����ղ�������
		{
		   ZD_CVM11=50;//��ֹһֱ�����
		   ZD_CVM12=50;
		   CVM1_Flag = 1;//�ɼ���1ͨ�����쳣
		   Fault_Level_Four|=0X0200;//�ϱ��ļ�����
		   fualt(41001);//�ϱ�������
		}
		else
		{
		   CVM1_Flag = 0;//�ɼ���1ͨ������
		}
		//CVM2���Ķ�֡���
		ZD_CVM21++;
		ZD_CVM22++;
		if((ZD_CVM21>50)||(ZD_CVM22>50))//5�����ղ�������
		{
		   ZD_CVM21=50;//��ֹһֱ�����
		   ZD_CVM22=50;
		   CVM2_Flag = 1;//�ɼ���2ͨ�����쳣
		   Fault_Level_Four|=0X0200;//�ϱ��ļ�����
		   fualt(41002);//�ϱ�������
		}
		else
		{
		   CVM2_Flag = 0;//�ɼ���2ͨ������
		}
	}
}
//CANO��ѯ���ղ�������
void CAN0_Receive(void)
{
	   Pump_ID_Rec();//����ˮ�ñ��Ľ���
	   H_Pump_ID_Rec();//��������ѭ���ñ���
	   JYF_ID_Rec();//��������1����
	   KQBYF_ID_Rec();//������ѹ������
	   SJKYJ_ID1_Rec();//�Ƽ�͸����ѹ����1
	   SJKYJ_ID2_Rec();//�Ƽ�͸����ѹ����2
	   DC_ID1_Rec();//��ѹDC����1
	   DC_ID2_Rec();//��ѹDC����2
	   CVM11_ID_Rec();//CVM11����
	   CVM12_ID_Rec();//CVM12����
	   CVM21_ID_Rec();//CVM21����
	   CVM22_ID_Rec();//CVM22����
	   DCDC1_ID_Rec();//��ѹDCDC1
	   Valve_ID_Rec();//��ͨ��
	   PTC_ID_Rec();//PTC
}

//������λ�����ĺ���
void CAN0_Receive_COM(void)
{
	// ��ȡ RX_MAILBOX_REL
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_REL) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
	  if(recvMsg_REL.id == ID_REL)
	  {
		R_REL_Data[0]=recvMsg_REL.data[0];
		R_REL_Data[1]=recvMsg_REL.data[1];
		R_REL_Data[2]=recvMsg_REL.data[2];
		R_REL_Data[3]=recvMsg_REL.data[3];
		R_REL_Data[4]=recvMsg_REL.data[4];
		if(R_REL_Data[3]>0)Soc_Data = R_REL_Data[3];
		if(R_REL_Data[4]>0)VCU_Ready_State = R_REL_Data[4];
		R_REL_Data[5]=recvMsg_REL.data[5];
		if((recvMsg_REL.data[6]&0XC0)!=0)
		{
			Small_DC_State=recvMsg_REL.data[6];
		}
		if((recvMsg_REL.data[6]&0X1F)!=0)
		{
			Stack_State |= (recvMsg_REL.data[6]&0X1F);
		}
		R_REL_Data[7]=recvMsg_REL.data[7];
		hand_Computer_REL();
	  }
	  CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_REL, &recvMsg_REL);
	}

	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_CBD) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_CBD.id == ID_CBD)
		{
		R_CBD_Data[0]=recvMsg_CBD.data[0];
		R_CBD_Data[1]=recvMsg_CBD.data[1];
		R_CBD_Data[2]=recvMsg_CBD.data[2];
		R_CBD_Data[3]=recvMsg_CBD.data[3];
		R_CBD_Data[4]=recvMsg_CBD.data[4];
		R_CBD_Data[5]=recvMsg_CBD.data[5];
		R_CBD_Data[6]=recvMsg_CBD.data[6];
		R_CBD_Data[7]=recvMsg_CBD.data[7];
		CAL_C_Data();
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_CBD, &recvMsg_CBD);
	}

	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_GBD) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_GBD.id == ID_GBD)
		{
		R_GBD_Data[0]=recvMsg_GBD.data[0];
		R_GBD_Data[1]=recvMsg_GBD.data[1];
		R_GBD_Data[2]=recvMsg_GBD.data[2];
		R_GBD_Data[3]=recvMsg_GBD.data[3];
		R_GBD_Data[4]=recvMsg_GBD.data[4];
		R_GBD_Data[5]=recvMsg_GBD.data[5];
		R_GBD_Data[6]=recvMsg_GBD.data[6];
		R_GBD_Data[7]=recvMsg_GBD.data[7];
		CAL_G_Data();
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_GBD, &recvMsg_GBD);
	}

	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_RW) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_RW.id == ID_GU_RW)
		{
			ZD_RW=0;
			R_RW_Data[0]=recvMsg_RW.data[0];
			R_RW_Data[1]=recvMsg_RW.data[1];
			R_RW_Data[2]=recvMsg_RW.data[2];
			R_RW_Data[3]=recvMsg_RW.data[3];
			R_RW_Data[4]=recvMsg_RW.data[4];
			R_RW_Data[5]=recvMsg_RW.data[5];
			R_RW_Data[6]=recvMsg_RW.data[6];
			R_RW_Data[7]=recvMsg_RW.data[7];
			if(R_RW_Data[0]==0X02)
			{
				Erase_GU_Data();//���������
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_RW, &recvMsg_RW);
	}
	else
	{
		ZD_RW++;
		if(ZD_RW>250)ZD_RW=250; // 25����
	}

	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_DC_MAN) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
	  if(recvMsg_DC_MAN.id == ID_DC_MAN)//��ѹDC
	  {
		S_Flag=1;
		R_DC_MAN_Data[0]=recvMsg_DC_MAN.data[0];
		R_DC_MAN_Data[1]=recvMsg_DC_MAN.data[1];
		R_DC_MAN_Data[2]=recvMsg_DC_MAN.data[2];
		R_DC_MAN_Data[3]=recvMsg_DC_MAN.data[3];
		R_DC_MAN_Data[4]=recvMsg_DC_MAN.data[4];
		R_DC_MAN_Data[5]=recvMsg_DC_MAN.data[5];
		R_DC_MAN_Data[6]=recvMsg_DC_MAN.data[6];
		R_DC_MAN_Data[7]=recvMsg_DC_MAN.data[7];
	  }
	  CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_DC_MAN, &recvMsg_DC_MAN);
	}
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_JYFBD) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
	  if(recvMsg_JYFBD.id == ID_REC_JYFBD)
	  {
		  R_JYFBD_Data[0]=recvMsg_JYFBD.data[0];
		  R_JYFBD_Data[1]=recvMsg_JYFBD.data[1];
		  R_JYFBD_Data[2]=recvMsg_JYFBD.data[2];
		  R_JYFBD_Data[3]=recvMsg_JYFBD.data[3];
		  R_JYFBD_Data[4]=recvMsg_JYFBD.data[4];
		  R_JYFBD_Data[5]=recvMsg_JYFBD.data[5];
		  R_JYFBD_Data[6]=recvMsg_JYFBD.data[6];
		  R_JYFBD_Data[7]=recvMsg_JYFBD.data[7];
		  if((R_JYFBD_Data[0]&0X30)!=0)
		  {
			  S_JYF_Data[0]&=0XCF;
			  S_JYF_Data[0]|=(R_JYFBD_Data[0]&0X30);
			  S_JYF_Data[1] =  R_JYFBD_Data[1];
			  EX_EEPROM_C256_write(30,S_JYF_Data,2);
		  }
	  }
	  CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_JYFBD, &recvMsg_JYFBD);
	}
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_KQBYF_QXHB_PUMP_MAN) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
	  if(recvMsg_KQBYF_QXHB_PUMP_MAN.id == ID_KQBYF_QXHB_PUMP_MAN)
	  {
		R_KQBYF_QXHB_PUMP_Data[0]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[0];
		R_KQBYF_QXHB_PUMP_Data[1]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[1];
		R_KQBYF_QXHB_PUMP_Data[2]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[2];
		R_KQBYF_QXHB_PUMP_Data[3]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[3];
		R_KQBYF_QXHB_PUMP_Data[4]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[4];
		R_KQBYF_QXHB_PUMP_Data[5]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[5];
		R_KQBYF_QXHB_PUMP_Data[6]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[6];
		R_KQBYF_QXHB_PUMP_Data[7]=recvMsg_KQBYF_QXHB_PUMP_MAN.data[7];
	  }
	  CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_KQBYF_QXHB_PUMP_MAN, &recvMsg_KQBYF_QXHB_PUMP_MAN);
	}
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_SJKYJ_MAN) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
	  if(recvMsg_SJKYJ_MAN.id == ID_SJKYJ_MAN)
	  {
		R_SJKYJ_Data[0]=recvMsg_SJKYJ_MAN.data[0];
		R_SJKYJ_Data[1]=recvMsg_SJKYJ_MAN.data[1];
		R_SJKYJ_Data[2]=recvMsg_SJKYJ_MAN.data[2];
		R_SJKYJ_Data[3]=recvMsg_SJKYJ_MAN.data[3];
		R_SJKYJ_Data[4]=recvMsg_SJKYJ_MAN.data[4];
		R_SJKYJ_Data[5]=recvMsg_SJKYJ_MAN.data[5];
		R_SJKYJ_Data[6]=recvMsg_SJKYJ_MAN.data[6];
		R_SJKYJ_Data[7]=recvMsg_SJKYJ_MAN.data[7];
	  }
	  CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_SJKYJ_MAN, &recvMsg_SJKYJ_MAN);
	}
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_JWQ_PTC_6KWDC_MAN) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
	  if(recvMsg_JWQ_PTC_6KWDC_MAN.id == ID_JWQ_PTC_6KWDC_MAN)
	  {
		  R_JWQ_PTC_6KWDC_Data[0]=recvMsg_JWQ_PTC_6KWDC_MAN.data[0];
		  R_JWQ_PTC_6KWDC_Data[1]=recvMsg_JWQ_PTC_6KWDC_MAN.data[1];
		  R_JWQ_PTC_6KWDC_Data[2]=recvMsg_JWQ_PTC_6KWDC_MAN.data[2];
		  R_JWQ_PTC_6KWDC_Data[3]=recvMsg_JWQ_PTC_6KWDC_MAN.data[3];
		  R_JWQ_PTC_6KWDC_Data[4]=recvMsg_JWQ_PTC_6KWDC_MAN.data[4];
		  R_JWQ_PTC_6KWDC_Data[5]=recvMsg_JWQ_PTC_6KWDC_MAN.data[5];
		  R_JWQ_PTC_6KWDC_Data[6]=recvMsg_JWQ_PTC_6KWDC_MAN.data[6];
		  R_JWQ_PTC_6KWDC_Data[7]=recvMsg_JWQ_PTC_6KWDC_MAN.data[7];
	  }
	  CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_JWQ_PTC_6KWDC_MAN, &recvMsg_JWQ_PTC_6KWDC_MAN);
	}

}
