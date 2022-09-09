/*
 * Data_Hand.c
 *
 *  Created on: 2018��8��28��
 *      Author: Administrator
 */

#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "Close.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "CAN_TX.h"
#include "CAN1_TX.h"
#include "CAN_RX.h"
#include "CAN1_RX.h"
#include "Data_Hand.h"
#include "Fault.h"
#include "CAL.h"
#include "Start_Up.h"
#include "Self_Check.h"
#include <stdint.h>
#include <stdbool.h>



uint8_t ON_Flag=0,Stop_Flag=0;//ON����־λ ֹͣ��־λ
//������ϵȼ���־λ
uint16_t Fault_Level_Two=0,Fault_Level_Three=0,Fault_Level_Four=0,Fault_Level_Five=0;
//���ȵ���
uint8_t S_Fan_Speed = 0;//����ת��ֵ
uint8_t Fan_Speed_Data=0;//�ֶ����������ٶ�ֵ
//����ʱ�䣬�������ʱ��
uint8_t Out_Time=0;
uint16_t Out_Interva=0,Out_Interva_Old=0;
uint8_t Out_H_Flag=1;//�����־λ  1�����ֶ����� 0�����Զ����� �������������1���ǹ̶�����100ms*Out_H_Flag
//CAN0���͸���λ��������һЩ״̬
uint8_t S_VCU_State=0;//�ܸ���ON������ͣ��Ready���ź�
uint16_t S_Fan_Speed_Change = 0;//���Ȳ����������ݵ�ѳ����¶���ֵ���ӷ���ת�٣�
uint16_t HProtectFlag_10S=0;//��Ready���ʱ10S

/************************************************************/
/*                         ON����ȡ                                                                                 */
/************************************************************/
void ON_Read(void)
{
	uint32_t buttonsPressed= PINS_DRV_ReadPins(PTD);//��ȡPTD��״̬
	uint32_t buttonsPressed_E= PINS_DRV_ReadPins(PTE);//��ȡPTE��״̬
	if(buttonsPressed & ((1 << D_IN_ON)))//ON��λ��    ����
	{
		ON_Flag=0;
		S_VCU_State&=0XBF;//���͸���λ��ON������״̬
		Fault_Level_Four=0;//��ON��֮������ļ�����
		if(((Stack_State&0X17)==0X07))//ֻ���������л��������е�ʱ���ʹ������0X10����Ҫͣ��
		{
			Stack_State|=0X10;//�ػ���־λ����
		}
		else if((Stack_State&0X10)==0)
		{
			HProtectFlag_10S = 0;//��ϵͳδ��������¹ر�Կ��ҲҪ�Ա�־λ����
			PINS_DRV_ClearPins(GPIO_PTE,1<<E_SYS_Power);//�ر��Ի���
//			REL_KG(BMS_ON,0X01);//�ر�BMSʹ���ź�
			REL_KG(H_GYH,0X01);//�ر���������2
		}
		Stack_State&=0XFE;//û��ON��֮������������������Լ�
	}
	else//����ON��12V֮��
	{
		ON_Flag=1;
		Stack_State|=0x01;//���ÿ����Լ��־λ
//		REL_KG(BMS_ON,0X02);//��BMSʹ���ź�
		REL_KG(Part_Power,0X02);//�򿪲�����Դ
//		REL_KG(H_GYH,0X02);//����ȼ��ϵͳ��ѹ��
		PINS_DRV_SetPins(GPIO_PTE,1<<E_SYS_Power);//��ON���ʹ��Ի���
		S_VCU_State|=0X40;//���͸���λ��ON������״̬

		HProtectFlag_10S++;
		if(HProtectFlag_10S >= 500)
		{
			HProtectFlag_10S = 500;
		}
	}
//	if(buttonsPressed_E & ((1 << 9)))//������� �ź��� ��ͣ����
//	{
//		Stop_Flag=1;
//		S_VCU_State|=0X80;//���͸���λ����ͣ����״̬
//	}
//	else//����   ���ź�
//	{
//		Stop_Flag=0;
//		S_VCU_State&=0X7F;//���͸���λ����ͣ����״̬
//	}
	Start_Up_Condition();//���������ж�
}
//���������ж�
void Start_Up_Condition(void)
{
	if(VCU_Ready_State==0X04)//����Ready
	{
		if(Soc_Data<80)//SOC��85���� ��������
		{
			Stack_State|=0X04;//����������־λ����
		}
		if(Soc_Data>89)
		{
			if(((Stack_State&0X17)==0X07))//ֻ���������л��������е�ʱ����ж������ļ����ϣ���Ҫͣ��
			{
				if(DC_Target_I>=100)// Ŀ���������100A
				{
					Normal_Close_Flag = 1; // �����ʵ�50A���� ��ʱ�µ�
//					Stack_State|=0X10;//�ػ���־λ����
				}
				if(DC_Target_I<100&&Normal_Close_Flag==0)
				{
					VCU_Ready_State = 0x03;
				}
			}
			else
			{
				Stack_State&=0XFB;//�������������־λ����
			}
		}
	}
	else
	{
		if(((Stack_State&0X17)==0X07))//ֻ���������л��������е�ʱ����ж������ļ����ϣ���Ҫͣ��
		{
			Stack_State|=0X10;//�ػ���־λ����
		}
		else
		{
			Stack_State&=0XFB;//�������������־λ����
		}
	}
}

/************************************************************/
/*                        ����¶ȿ���                                                                              */
/************************************************************/
void Temp_Con(void)
{
	static uint8_t TempDifferenceFlag_6S=0;//
	static uint8_t CheckFaultCount=0;//��ˮ�¶ȴ��ڽ�ˮ�¶ȼ������

	if(REL_Auto[12]==1)//�����ֶ�����
	{
		S_Fan_Speed = Fan_Speed_Data;
	}
	else//�����Զ�����
	{
		if((FCU_T1_Data<=CAL_Data[43])&&(FCU_T4_Data<=CAL_Data[44]))//��ѽ���С��65���ҳ���С��66��
		{
			S_Fan_Speed=0;//ת������Ϊ0
			if(FCU_T4_Data>=FCU_T1_Data)//��ˮ�¶ȸ�
			{
				if((FCU_T4_Data-FCU_T1_Data)<=(CAL_Data[47]-1))//��ˮ�¶����ˮ�¶Ȳ�ֵ��4������
				{
					Fault_Level_Two&=0XFEFF;
					Fault_Level_Three&=0XFBFF;
//					Fault_Level_Four&=0XFBFF;
				}
			}
		}
		//�Խ����¶ȿ��Ʋ���
		if((FCU_T1_Data>=(CAL_Data[41]))&&(LowTemp_FanCon_Flag == 0))//��ѽ��ڴ���69��
		{
			S_Fan_Speed=15;
			if(FCU_T1_Data>=(CAL_Data[41]+1))//��ѽ��ڴ���70��
			{
				S_Fan_Speed=15;
				if(FCU_T1_Data>=(CAL_Data[41]+2))//��ѽ��ڴ���71��
				{
					S_Fan_Speed=25;
					if(FCU_T1_Data>=(CAL_Data[41]+3))//��ѽ��ڴ���72��
					{
						S_Fan_Speed=30;
						if(FCU_T1_Data>=(CAL_Data[41]+4))//��ѽ��ڴ���73��
						{
							S_Fan_Speed=40;
							if(FCU_T1_Data>=(CAL_Data[41]+5))//��ѽ��ڴ���74��
							{
								S_Fan_Speed=50;
								if(FCU_T1_Data>=(CAL_Data[41]+6))//��ѽ��ڴ���75��
								{
									S_Fan_Speed=60;
									if(FCU_T1_Data>=(CAL_Data[41]+7))//��ѽ��ڴ���76��
									{
										S_Fan_Speed=70;
										if(FCU_T1_Data>=(CAL_Data[41]+8))//��ѽ��ڴ���77��
										{
											S_Fan_Speed=80;
											if(FCU_T1_Data>=(CAL_Data[41]+9))//��ѽ��ڴ���78��
											{
												S_Fan_Speed=90;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if(((FCU_T1_Data>=(CAL_Data[41]+3))||(FCU_T4_Data>=(CAL_Data[42]+3)))&&(LowTemp_FanCon_Flag == 1))//��ѽ��ڴ���69��
		{
			S_Fan_Speed=15;
			if((FCU_T1_Data>=(CAL_Data[41]+4))||(FCU_T4_Data>=(CAL_Data[42]+4)))//��ѽ��ڴ���70��
			{
				S_Fan_Speed=25;
				if((FCU_T1_Data>=(CAL_Data[41]+5))||(FCU_T4_Data>=(CAL_Data[42]+5)))//��ѽ��ڴ���71��
				{
					S_Fan_Speed=35;
					if((FCU_T1_Data>=(CAL_Data[41]+6))||(FCU_T4_Data>=(CAL_Data[42]+6)))//��ѽ��ڴ���72��
					{
						S_Fan_Speed=45;
						if((FCU_T1_Data>=(CAL_Data[41]+7))||(FCU_T4_Data>=(CAL_Data[42]+7)))//��ѽ��ڴ���73��
						{
							S_Fan_Speed=60;
							if((FCU_T1_Data>=(CAL_Data[41]+8))||(FCU_T4_Data>=(CAL_Data[42]+8)))//��ѽ��ڴ���74��
							{
								S_Fan_Speed=80;
							}
						}
					}
				}
			}
		}
		else
		{
			S_Fan_Speed=0;
		}
		//�Ե�ѳ����¶ȿ��Ʋ���
		if((FCU_T4_Data>=(CAL_Data[42]))&&(LowTemp_FanCon_Flag == 0))//��ѳ��ڴ���76��  �����µ�ʱ����Ҫ���д���
		{
			S_Fan_Speed_Change = 15;//������  ��ֹ���ȴ���ֹͣģʽʱ�������������15
			if((FCU_T4_Data>=(CAL_Data[42]+2)))//��ѳ��ڴ���78��
			{
				S_Fan_Speed_Change = 20;//������
				if((FCU_T4_Data>=(CAL_Data[42]+4)))//��ѳ��ڴ���80��
				{
					S_Fan_Speed_Change = 30;//������
				}
			}
		}
		else
		{
			S_Fan_Speed_Change = 0;//����������
		}

		if(DC_Target_I>300)//�߹������¶��ڼ�ƫ��ֵ
		{
			if(DC_Target_I<350)
			{
				S_Fan_Speed = S_Fan_Speed + S_Fan_Speed_Change + 10;//����ת�����¼���
			}
			else
			{
				S_Fan_Speed = S_Fan_Speed + S_Fan_Speed_Change + 15;//����ת�����¼���
			}
		}
		else
		{
			S_Fan_Speed = S_Fan_Speed + S_Fan_Speed_Change;
		}


		if((FCU_T1_Data>=(CAL_Data[41]+6))||(FCU_T4_Data>=(CAL_Data[42]+6)))//��ѽ��ڴ���76�Ȼ���ڴ���81��
		{
			Fault_Level_Two|=0X0400;//�ϱ����¶�������
			fualt(20101);//�ϱ�������
			if((FCU_T1_Data>=(CAL_Data[41]+8))||(FCU_T4_Data>=(CAL_Data[42]+9)))//��ѽ��ڴ���78�Ȼ���ڴ���82��
			{
				Fault_Level_Three|=0X0400;//�ϱ�������������
				fualt(30101);//�ϱ�������
				if((FCU_T1_Data>=(CAL_Data[41]+9))||(FCU_T4_Data>=(CAL_Data[42]+10)))//��ѽ��ڴ���79�Ȼ���ڴ���83��
				{
					Fault_Level_Four|=0X0400;//�ϱ������ļ�����
					fualt(40101);//�ϱ�������
				}
			}
		}

		if((Stack_State&0X1F)==0X0F)//��Ѵ�������״̬����²�
		{
			TempDifferenceFlag_6S++;
			if(TempDifferenceFlag_6S>=60)//ϵ���к���ȴҺѭ��6s���ټ���²�
			{
				TempDifferenceFlag_6S = 60;
				if((FCU_T4_Data>=FCU_T1_Data)&&(JWQ_Target_Angle>30))//��ˮ�¶ȸ�  �����������ȴ���30���ж��²�
				{
					if((FCU_T4_Data-FCU_T1_Data)>=(CAL_Data[47]))//��ˮ�¶����ˮ�¶Ȳ�ֵ��6������
					{
						CheckFaultCount++;
						if(CheckFaultCount>=15){//����1500ms
							CheckFaultCount=15;
							Fault_Level_Two|=0X0400;//�ϱ��¶ȶ�������
							fualt(20102);//�ϱ������� �������²����
							if((FCU_T4_Data-FCU_T1_Data)>=(CAL_Data[47]+2))//��ˮ�¶����ˮ�¶Ȳ�ֵ��8������
							{
								Fault_Level_Three|=0X0400;//�ϱ�������������
								fualt(30102);//�ϱ������� �������²����
								if((FCU_T4_Data-FCU_T1_Data)>=(CAL_Data[47]+6))//��ˮ�¶����ˮ�¶Ȳ�ֵ��12������
								{
									Fault_Level_Four|=0X0400;//�ϱ������ļ�����
									fualt(40102);//�ϱ������� �������²����
								}
							}
						}
					}
					else
					{
						CheckFaultCount=0;
						Fault_Level_Two|=0XFBFF;
					}
				}
			}
		}
		else
		{
			TempDifferenceFlag_6S = 0;
		}
	}
//	if((DC_Temp>CAL_Data[45]) || (SJKYJ_Temp > 90) ||(SJKYJ_MotorTemp > 150))//��ѹDC�¶ȴ��ڿ����¶�  ��ѹ���������¶ȴ���50  ��ѹ���¶ȴ���110
//	{
//		REL_KG(FZ_Fan,0X02);//FZ_Fan�ں궨�����涨��һ��9  �򿪷���
//	}
//	else if((DC_Temp<=CAL_Data[46])&&(SJKYJ_Temp <= 89)&&(SJKYJ_MotorTemp <= 149))//��ѹDC�¶�С�ڿ����¶�
//	{
//		REL_KG(FZ_Fan,0X01);//FZ_Fan�ں궨�����涨��һ��9 �رշ���
//	}
}

/************************************************************/
/*                      ����ѹ����������                                                                            */
/************************************************************/
void H_Con(void)
{
	if((Stack1_H_Pre<2300)&&(Stack1_H2_Pre<2300))//������ѹ130KPA���£�����
	{
		if((Stack1_H_Pre>=950)&&(Stack1_H2_Pre>=950))//-5KPA����
		{
			Fault_Level_Three&=0XF7FF;
//			Fault_Level_Four&=0XF7FF;
		}
	}

	if((Stack1_H_Pre<700)||(Stack1_H2_Pre<700))//-30Kpa
	{
		Fault_Level_Four|=0X0800;
		fualt(40104);//�ϱ�������
		Out_H_Flag=4;//����400ms
	}
	else if((Stack1_H_Pre<800)||(Stack1_H2_Pre<800))//-20Kpa
	{
		Fault_Level_Three|=0X0800;
		fualt(30104);//�ϱ�������
		Out_H_Flag=4;//����400ms
	}
	else if((Stack1_H_Pre<900)||(Stack1_H2_Pre<900))//-10Kpa
	{
		Out_H_Flag=4;//����400ms
	}
   //��1000�Ǽ��ϱ�ѹֵ��ƫ�ƣ�����0.1KPA��1000����100KPA
	else if((Stack1_H_Pre>=(CAL_Data[177-36]*10+1000))||(Stack1_H2_Pre>=(CAL_Data[177-36]*10+1000)))//�ļ�����  ������ѹ180KPA  ����0.1KPA ��ѹ ��ѹ����Ҫ���Ⲣ�رռ�ѹ�����ر�ƿ�ڷ�
	{
		if(Flag_6S >= 60)
		{
			Fault_Level_Four|=0X0800;
			fualt(40103);//�ϱ�������
		}
		Out_H_Flag=4;//����400ms
	}
	else if((Stack1_H_Pre>=(CAL_Data[145-36]*10+1000))||(Stack1_H2_Pre>=(CAL_Data[145-36]*10+1000)))//��������  ������ѹ150KPA  ����0.1KPA ��ѹ ��ѹ����Ҫ���Ⲣ�رռ�ѹ�����ر�ƿ�ڷ�
	{
		Fault_Level_Three|=0X0800;
		fualt(30103);//�ϱ�������
		Out_H_Flag=4;//����400ms
	}
	else
	{

	}

}

/*************************************************************/
/*                        ���⺯��                                                                                         */
/*************************************************************/
void Out_H(void) //��Ϊ�Ƿ���100ms��־λ������Ҫ������ٴ�100ms����
{
	static uint8_t HFlag_6S=0;//6���Ӽ��һ��ѹ���Ƿ����
	static uint8_t H2_Flag=0,H2_Time=0;//����򿪱�־
	static uint16_t time=0,Long_Out_Time=0;//timeΪ��������  Long_Out_Time��ʱ��������ʱ

	Out_Time = CAL_Data[17]/10; //����ж��ٴ�100ms

	if((Stack_State&0X0F)==0X07){ //�����׶�3s��һ��
		Out_H_Flag = 0;
		Out_Interva = 30; //�����׶�3s��һ��
	}
	else if(DC_IN_I<(CAL_Data[11]-2))//������1
	{
		Out_Interva = (CAL_Data[1]*10);
	}
	else if(DC_IN_I>=((CAL_Data[14]*2+100)-2))//350A
	{
		Out_Interva = (CAL_Data[7]*10);
	}
	else if(DC_IN_I>=((CAL_Data[14]*2+50)-2))//300A
	{
		Out_Interva = (CAL_Data[6]*10);
	}
	else if(DC_IN_I>=((CAL_Data[14]*2)-2))//������5   250A
	{
		Out_Interva = (CAL_Data[5]*10);
	}
	else if(DC_IN_I>=((CAL_Data[13]*2)-2))//������4   200A
	{
		Out_Interva = (CAL_Data[4]*10);
	}
	else if(DC_IN_I>=(CAL_Data[12]-2))//������3   150A
	{
		Out_Interva = (CAL_Data[3]*10);
	}
	else if(DC_IN_I>=(CAL_Data[11]-2))//������2   100A
	{
		Out_Interva = (CAL_Data[2]*10);
	}


	//��ֹϵͳ���ϵ��ʱ������ѹ���ɼ�˲������쳣ֵ��Ϊ�������ϵ�˲���쳣ֵ��   ����HProtectFlag_10S��־ԭ�������ϵ絫��ϵͳδ����ʱ��HProtectFlag_6S��־λ������м������±��������𲻵�����
	if((HProtectFlag_6S>=60) || (HProtectFlag_10S >= 500))
	{
		H_Con();//����ѹ����������
	}

	HFlag_6S++;//
	if(HFlag_6S>=60)//6�����ж�һ��ѹ���Ƿ����6S�����ж�һ�Σ�
	{
		HFlag_6S=0;
		if((Stack1_Max_V_Dif<2000) || (Stack2_Max_V_Dif<2000))//ͣ����ʱ�����ѹ�� ��5374��ֵ������Ҫ���μ�ֵ
		{
			if((Stack1_Max_V_Dif>CAL_Data[23]) || (Stack2_Max_V_Dif>CAL_Data[23]))
			{
			  time = Out_Interva-Out_Time-1;
			}
		}
	}
	/*ƿ�ڷ�������ִ���Զ����⣬�ر�ƿ�ڷ����������ڳ���      */
	if((REL_State2&0X10)==0X10)//������ڿ���
	{
		if(Out_H_Flag>1)//��ʱ������������ʱ��ΪOut_H_Flag*100ms
		{
			REL_KG(H_Out,0X02);//����������
			Long_Out_Time++;
			if(Long_Out_Time>=Out_H_Flag)
			{
				Long_Out_Time=0;
				REL_KG(H_Out,0X01);//�ر���������
				time=0;
				if(REL_Auto[4]==1)//ȷ��һ���Զ������Ƿ��
				{
				   Out_H_Flag=1;
				}
				else
				{
				   Out_H_Flag=0;
				}
			}
		}

		if(Out_H_Flag==0)//�Զ�����״̬
		{
			time++;//��ʱ�����ʱ
			if(time==Out_Interva)//130*100 16*100 ÿ1.6���һ�� 130-4 126
			{
				REL_KG(H_Out,0X02);//����������
				H2_Flag=1;
			}
			else if(time>=Out_Interva+Out_Time) // 24*100 2-1.6
			{
				REL_KG(H_Out,0X01);//�ر���������
				time=0;
				H2_Flag=0;
				H2_Time=0;
			}
			if(H2_Flag){
				H2_Time++;
				if(H2_Time>Out_Time){
					REL_KG(H_Out,0X01);//�ر���������
				}
			}
		}
	}
	else
	{
	   if((Stack1_H_Pre<900)||(Stack1_H2_Pre<900))//-10Kpa
		{
			REL_KG(H_Out,0X02);//����������1   �µ�ʱ����ѹ����ѹ̫���Զ������޷��������Ҫ����
		}
	   else
	   {
		  Out_H_Flag=1;//���������ڷ����ó��ֶ�����
		  REL_KG(H_Out,0X01);//�ر���������1
		  REL_KG(H_GYH,0X01);//�ر���������2
	   }

	}
}


/*************************************************************/
/*                          �����жϺ���                                       */
/*************************************************************/
void Fault_Check(void)
{
	if(((Stack_State&0X17)==0X07))//ֻ���������л��������е�ʱ����ж������ļ����ϣ���Ҫͣ��
	{
		if(LowTempStart_Flag == 0)
		{
			Flag_6S++;
			Flag_12S++;
			HProtectFlag_6S++;
			if(Flag_6S>=60)Flag_6S=60;
			if(Flag_12S>=120)Flag_12S=120;
			if(HProtectFlag_6S>=60)HProtectFlag_6S=60;
		}
		if(Fault_Level_Four!=0)//���ļ�����
		{
		   Stack_State|=0X10;//�ػ���־λ����
		}
	}
	else
	{
//		Flag_6S=0;
//		Flag_12S=0;
	}
}
/*************************************************************/
/*                          ���ȵ��ٺ���                                                                           */
/*************************************************************/
void Fan_Speed(void)
{
	static uint8_t i=0;
	if(S_Fan_Speed>90)S_Fan_Speed=90;
	if(i>=S_Fan_Speed)//��
	{
		//���ݲ�ͬ����ƥ����ȸ���
		if((Stack_State&0X1F)==0X0F)//����
		{
			if(DC_Target_I<=100)
			{
//				PINS_DRV_SetPins(GPIO_PTA,1<<11);//����2������
				PINS_DRV_SetPins(GPIO_PTE,1<<6);//����4������
			}
			else if((DC_Target_I>100)&&(DC_Target_I<=200))
			{
				PINS_DRV_SetPins(GPIO_PTE,1<<6);//����4������
				PINS_DRV_SetPins(GPIO_PTA,1<<11);//����2������
			}
			else if(DC_Target_I>200)
			{
				PINS_DRV_SetPins(GPIO_PTE,1<<6);//����4������
				PINS_DRV_SetPins(GPIO_PTA,1<<11);//����2������
			}
			else
			{

			}
		}
		else
		{
			PINS_DRV_SetPins(GPIO_PTE,1<<6);//����4������
			PINS_DRV_SetPins(GPIO_PTA,1<<11);//����2������
		}
	}
	else
	{
		if((Stack_State&0X1F)==0X0F)//����
		{
			if(DC_Target_I<=100)
			{
//				PINS_DRV_ClearPins(GPIO_PTA,1<<11);//����2������
				PINS_DRV_ClearPins(GPIO_PTE,1<<6);//����4������
			}
			else if((DC_Target_I>100)&&(DC_Target_I<=200))
			{
				PINS_DRV_ClearPins(GPIO_PTE,1<<6);//����4������
				PINS_DRV_ClearPins(GPIO_PTA,1<<11);//����2������
			}
			else if(DC_Target_I>200)
			{
				PINS_DRV_ClearPins(GPIO_PTE,1<<6);//����4������
				PINS_DRV_ClearPins(GPIO_PTA,1<<11);//����2������
			}
			else
			{

			}
		}
		else
		{
			PINS_DRV_ClearPins(GPIO_PTE,1<<6);//����4������
			PINS_DRV_ClearPins(GPIO_PTA,1<<11);//����2������
		}
	}
	i++;
	if(i>=99)
	{
		i = 0;
	}
}

void Fan_Speed2(uint8_t open)
{
	static uint8_t i=0;
	if(open==1)
	{
		if(i<80)
		{
			i++;
			PINS_DRV_SetPins(GPIO_PTE,1<<7);
		}
		else
		{
			i++;
			if(i>99)
			{
				i=0;
			}
			PINS_DRV_ClearPins(GPIO_PTE,1<<7);
		}
	}
}

/*************************************************************/
/*                         ��ѷ���������                                                                          */
/*************************************************************/
void Stack_P_CAL(void)
{
	Stack_Total_P_LS += DC_IN_V*DC_IN_I;
	 if(Stack_Total_P_LS>=36000000)
	 {
		 Stack_Total_P+=1000;//�ܹ�������
		 Stack_Total_P_LS-=36000000;
	 }
	 Send_Stack_Total_P = Stack_Total_P+(Stack_Total_P_LS/36000);
}

/*************************************************************/
/*                           Һλ��غ���                                                                        */
/*************************************************************/
void YeWei_Stack(void)
{
	static uint8_t YeWei_Flag=0;
	uint32_t buttonsPressed_E= PINS_DRV_ReadPins(PTE);//��ȡPTE��״̬
	if(buttonsPressed_E & (1 << YW_IN))
	{
		YeWei_Flag=1;//����ˮ��Һλ��������־λ   ˮλ����
		YeWeiTX = YeWei_Flag;
	}
	else
	{
		YeWei_Flag=0;//����ˮ��Һλ��������־λ   ˮλȱˮ
		YeWeiTX = YeWei_Flag;
	}
}
