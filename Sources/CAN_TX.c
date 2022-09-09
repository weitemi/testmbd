/*
 * CAN_TX.c
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
#include "CAN1.h"
#include "LPIT.h"
#include "ADC.h"
#include "Close.h"
#include "REL.h"
#include "CAN_TX.h"
#include "CAN_RX.h"
#include "Data_Hand.h"
#include"EXTERNAL_EEPROM.h"
#include "CAN1_RX.h"
#include "CAL.h"
#include "Fault.h"
#include "Self_Check.h"
#include "Temp.h"
#include <stdint.h>
#include <stdbool.h>
#include "Start_Up.h"






/*************************************************************/
/*                       ���͸�����ˮ������                   */
/*************************************************************/
uint8_t S_PUMP_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t PUMP_Enable=0;//����ˮ��ʹ��״̬
uint16_t Pump_Target_Speed=0;//����ˮ��Ŀ��ת��
uint8_t Water_Pre_State_Flag=0;//ˮѹ״̬��־λ 0�������� �������Ǵ����쳣
/*************************************************************/
/*                       ���͸���ѭ��������                    */
/*************************************************************/
uint8_t S_QXHB_Data[8]={0,0,0,0,0,0,0,0};//
uint16_t H_Pump_Target_Speed=0;//����ѭ����Ŀ��ת��
uint8_t H_PUMP_Enable=0X00;//����ѭ����ʹ��״̬ ��ʼ����Ϊ��ʹ��״̬
uint8_t QXHB_CANLife=0;//ѭ����CANLife
/*************************************************************/
/*                        ���͸���ѹ������                     */
/*************************************************************/
uint8_t S_JYF_Data[8]={0X40,0,0,0,0,0,0,0};//Ĭ�Ϸ��͹رն�ֵģʽ
/*************************************************************/
/*                      ���͸�������ѹ������                   */
/*************************************************************/
uint8_t S_KQBYF_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t KQBYF_CANLife=0;//������ѹ��CANLife
uint8_t KQBYF_Target_Angle=0;//��ѹ��Ŀ��Ƕ�
uint16_t LowTempAngleCompensate=0;//��ѹ�������µ�ǶȲ���
/*************************************************************/
/*                        ���͸�DCDC ID                      */
/*************************************************************/
uint8_t S_DC_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t S_DC_State=0X05;//�ػ� 0X05�ػ� 0X0B���� 0X0C�����ŵ�  0X03Ԥ��
uint16_t S_DC_I=0;//DC�������ֵ ��λ0.1A
uint16_t DC_Target_I=0;//��ѹDCĿ����� ����1A
int16_t I_Cha=0;//������ʱ��ǰ���ֵ
int16_t I_ADD=0;//������ʱ��ǰ����ֵ
int16_t DC_Target_Old=0;
uint8_t LowTemp_FanCon_Flag=0;//���·����л����Ʊ�־λ
uint8_t StartUpCurrent_Flag=0;
uint8_t Up_DCDC_Enable=1;
static uint16_t Low_Power_Time=0;//��ʱ�µ罵�������м�ʱ
/*************************************************************/
/*                    ���͸��Ƽ�͸����ѹ��  ID                 */
/*************************************************************/
uint8_t S_SJKYJ_Data[8]={0,0,0,0,0,0,0,0};//
uint32_t SJKYJ_Target_Speed=0;//��ѹ��Ŀ��ת��
uint16_t SJKYJ_Target_Speed_Change=0;//��ѹ��Ŀ��ת��ƫ��
uint8_t S_SJKYJ_State=0;//�Ƽ�͸����ѹ��״̬ 05��ʹ��   0X0A����   0X0B����   0XFE���ϸ�λ
uint16_t Speed_Adjust_I=0;//��ѹ��ת�ٵ��ڵ���
uint32_t BD_SJKYJ_Speed[7]={0,0,0,0,0,0,0};//��ȡ�궨�Ŀ�ѹ��ת�ٵ�
uint16_t BD_I_Point[7]={0,0,0,0,0,0,0};//��ȡ�궨�Ŀ�ѹ��ת�ٵ��Ӧ�ĵ�����
uint16_t LowTempSpeedCompensate=0;//��ѹ�������µ�ת�ٲ���
/*************************************************************/
/*                     ���͸�6kW����DCDC ID                   */
/*************************************************************/
uint8_t S_6KWDC_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t SDC_State=0;//3kWDCDC״̬
uint8_t Down_DC_Enable=1;
/*************************************************************/
/*                         ���͸������� ID                        */
/*************************************************************/
uint8_t S_JWQ_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t JWQ_Enable=0X00;//������ʹ��״̬ ��ʼ����Ϊ��ʹ��״̬
uint8_t JWQ_Target_Angle = 0;//
/*************************************************************/
/*                         ���͸�PTC ID                        */
/*************************************************************/
uint8_t S_PTC_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t PTC_Flag = 0;
uint8_t PTC_Enable=0X00;//������ʹ��״̬ ��ʼ����Ϊ��ʹ��״̬
uint8_t PTC_Target_Temp = 0;//Ŀ���¶�ֵ
uint8_t PTC_Target_PWM = 0;//Ŀ��PWMֵ

uint8_t YeWei=0;
uint8_t YeWeiTX=0;

uint8_t PTC_test_Time = 0;

uint16_t Time_10min = 0;


/*************************************************************/
/*                     ���͸���λ����������                                  */
/*************************************************************/
void Send_Computer(void)
{
//   static uint8_t n = 0;
   if(ZD_RW>=250)return;//25S�ղ������ֱ��ľͲ��ٷ�����Щ����
   else
   {
   // 100ms ����һ��
	if ((Timer_TxMsg % 10) == 0) {
		REN_CAN_Send(ID_Relay_State,CAN0_TX_MAILBOX1,REL_State1,REL_State2,REL_State3,REL_State4,Stack_State,ON_Flag,S_Fan_Speed,CAL_Flag); //���ͼ̵���״̬ 0x18F10201
		CAL_Flag = 0;//������֮��������ֹ���ǵ���
		REN_CAN_Send(ID_Stack2_Data,CAN0_TX_MAILBOX2,(Stack2_Total_Voltage/256),(Stack2_Total_Voltage%256),(Stack21_Average_Voltage/256),(Stack21_Average_Voltage%256),
						(Stack22_VoltageMin/256),(Stack22_VoltageMin%256),(Stack22_NumberMax),(Stack22_NumberMin)); //���2״̬��Ϣ
	} else if ((Timer_TxMsg % 10) == 1) {
		REN_CAN_Send(ID_Sensor_Pre,CAN0_TX_MAILBOX1,(Tank_Pre/256),(Tank_Pre%256),(Tank_Mid_Pre/256),(Tank_Mid_Pre%256),(Stack1_InWater_Pre/256),(Stack1_InWater_Pre%256),(0/256),(0%256)); //����ѹ����������ֵ 0x18F20201
		Send_CBD_Data();//���Ͳ����궨����ֵ
	} else if ((Timer_TxMsg % 10) == 2) {
		REN_CAN_Send(ID_Sensor_Temp,CAN0_TX_MAILBOX1,0,FCU_T1_Data,0,FCU_T4_Data,0,FCU_T3_Data,0,FCU_T2_Data); //����BMU�յ����¶� 0x18F30201
		Send_GBD_Data();//���͹��ϱ궨����ֵ
	} else if ((Timer_TxMsg % 10) == 3) {
		REN_CAN_Send(ID_Sensor_Data3,CAN0_TX_MAILBOX1,(DC_IN_I/256),(DC_IN_I%256),(DC_IN_V/256),(DC_IN_V%256),Fault_Level_Four>>8,Fault_Level_Four,Fault_Level_Three>>8,Fault_Level_Three); //���͵�����״̬���� 0x18F40201
		// ������Ե�͸�����Ե
		REN_CAN_Send(ID_VCU_Data2,CAN0_TX_MAILBOX2,(Res_Data1/256),(Res_Data1%256),(Res_Data2/256),(Res_Data2%256),Soc_Data,hms_h2_soc,hms_fault_level,VCU_Ready_State); //
	} else if ((Timer_TxMsg % 10) == 4) {
		REN_CAN_Send(ID_Sensor_Data4,CAN0_TX_MAILBOX1,(Send_Stack_Total_P>>24),((Send_Stack_Total_P>>16)%256),((Send_Stack_Total_P>>8)%256),(Send_Stack_Total_P%256),
				0,0,(Out_Interva/256),(Out_Interva%256)); //�¶ȣ���ǰ������ʱ��
		send_fualt();
	} else if ((Timer_TxMsg % 10) == 5) {
		REN_CAN_Send(ID_Sensor_Data5,CAN0_TX_MAILBOX1,(Stack1_H2_Pre/256),(Stack1_H2_Pre%256),(Stack1_RH/256),(Stack1_RH%256),
						(0/256),(0%256),((H2_PPM>>8)/256),(H2_PPM%256)); //
		REN_CAN_Send(ID_VCU_Data1,CAN0_TX_MAILBOX2,(hms_h2_pressure/256),(hms_h2_pressure%256),(vehicle_mileage>>24),(vehicle_mileage>>16),(vehicle_mileage/256),(vehicle_mileage%256),(Car_Speed+50),Out_H_Flag); //��������2
	} else if ((Timer_TxMsg % 10) == 6) {
		REN_CAN_Send(ID_Accessory_Data,CAN0_TX_MAILBOX2,(FCU_Flowmeter>>24),(FCU_Flowmeter>>16),(FCU_Flowmeter/256),(FCU_Flowmeter%256),vcu2fcu_command,Time_10min>>8,Time_10min,YeWeiTX); //
	} else if ((Timer_TxMsg % 10) == 7) {
		REN_CAN_Send(ID_Temporary_Data,CAN0_TX_MAILBOX1,0,vcu2fcu_command,0,vcu2fcu_limit_power,(DC_Target_I/256),(DC_Target_I%256),Normal_Close_Time>>8,Normal_Close_Time); //��ʱ�����۲��ַ

	} else if ((Timer_TxMsg % 10) == 8) {
		REN_CAN_Send(ID_Stack1_Data,CAN0_TX_MAILBOX1,(Stack1_Total_Voltage/256),(Stack1_Total_Voltage%256),(Stack11_Average_Voltage/256),(Stack11_Average_Voltage%256),
							 (Stack12_VoltageMin/256),(Stack12_VoltageMin%256),(Stack12_NumberMax),(Stack12_NumberMin)); //���1״̬��Ϣ
	}else if((Timer_TxMsg % 10)==9){
		REN_CAN_Send(ID_Send_CVM2,CAN0_TX_MAILBOX0,1,0,0,0,0,0,0,0); //CVM�����������Ƶ�ַ
	}

	if((Timer_TxMsg % 500) == 1)//5s ����汾
	{
		REN_CAN_Send(ID_Software_Version,CAN0_TX_MAILBOX2,
										0X20,//Ӳ��״̬�š����汾��
										0X00,//Ӳ���ΰ汾�š��Ӱ汾��
										0X21,//���״̬��:T;���汾��:1;
										0X24,//����ΰ汾��:2;;�Ӱ汾��:4;
										0X15,//21��
										0X0B,//11��
										0X04,//4��
										0); //
	}
  }
}
/*************************************************************/
/*                      ���Ͳ����궨ֵ����                                    */
/*************************************************************/
void Send_CBD_Data(void)
{
	static uint8_t i=0;
    if(CBD_Read_Flag==1)//������ȡ
    {
    	if(i<=7)
    	{
        	REN_CAN_Send(ID_CBD_Data,CAN0_TX_MAILBOX2,(i*8),CAL_Data[(i*8)+1],CAL_Data[(i*8)+2],CAL_Data[(i*8)+3],
        			CAL_Data[(i*8)+4],CAL_Data[(i*8)+5],CAL_Data[(i*8)+6],CAL_Data[(i*8)+7]);
    	}
    	else
    	{
    		REN_CAN_Send(ID_CBD_Data,CAN0_TX_MAILBOX2,0XFF,0,0,0,0,0,0,0);
    	}
    	i++;
		if(i>8)
		{
			CBD_Read_Flag=0;//��ȡ���֮���־λ���㣬���ٷ��ʹ˱���
			i=0;
		}
    }
}
/*************************************************************/
/*                        ���͹��ϱ궨ֵ����                                        */
/*************************************************************/
void Send_GBD_Data(void)
{
	static uint8_t i=0;
    if(GBD_Read_Flag==1)//���϶�ȡ
    {
    	if(i<=11)
    	{
        	REN_CAN_Send(ID_GBD_Data,CAN0_TX_MAILBOX2,(i*8)+64,CAL_Data[(i*8)+65],CAL_Data[(i*8)+66],CAL_Data[(i*8)+67],
        			CAL_Data[(i*8)+68],CAL_Data[(i*8)+69],CAL_Data[(i*8)+70],CAL_Data[(i*8)+71]);
    	}
    	else
    	{
    		REN_CAN_Send(ID_GBD_Data,CAN0_TX_MAILBOX2,0XFF,0,0,0,0,0,0,0);
    	}
    	i++;
		if(i>12)
		{
			GBD_Read_Flag=0;//��ȡ���֮���־λ���㣬���ٷ��ʹ˱���
			i=0;
		}
    }
}
/*************************************************************/
/*                    ���͸�����ˮ�ñ��ĺ���                                 */
/*************************************************************/
void Send_PUMP(void)
{
	static uint8_t i=0;
	int16_t H_Water_Pre_Dif=0;
    if((R_KQBYF_QXHB_PUMP_Data[1]>>4)==0X01)//�ֶ���������
    {
    	REL_State4|=0X40;//���͸���λ���ֶ�����
    	if(((R_KQBYF_QXHB_PUMP_Data[7]<<8)+R_KQBYF_QXHB_PUMP_Data[6])>=2000)
    	{
    		S_PUMP_Data[0]=R_KQBYF_QXHB_PUMP_Data[5];//����ʹ��״̬
    		S_PUMP_Data[2]=R_KQBYF_QXHB_PUMP_Data[6];
    		S_PUMP_Data[3]=R_KQBYF_QXHB_PUMP_Data[7];
    	}
    	else
    	{
    		S_PUMP_Data[0]=R_KQBYF_QXHB_PUMP_Data[5];//���Ͳ�ʹ��״̬
    		S_PUMP_Data[2]=0;
    		S_PUMP_Data[3]=0;
    	}
    }
    else//�Զ�����
    {
    	REL_State4&=0XBF;//���͸���λ���Զ�����
    	H_Water_Pre_Dif = Stack1_H_Pre-Stack1_InWater_Pre;
    	if((PUMP_Enable==0X01)&&(LowTempStart_Flag == 0))//ֻ����ˮ��ʹ�ܵ�״̬�²�ȥ���е���
    	{
    		i++;
//			if((i>=30)&&((Stack_State&0X10)!=0X10))//300ms�ж�һ�Σ���ֹһֱ�ж�ת��һֱ���ӻ����     ���������
    		if(i>=30)//300ms�ж�һ�Σ���ֹһֱ�ж�ת��һֱ���ӻ����
			{
				i=0;
				if((H_Water_Pre_Dif<500)&&(H_Water_Pre_Dif>-500))//��ˮѹ�������50Kpa����
				{
					Water_Pre_State_Flag=0;//ˮѹ״̬λ����
					if(H_Water_Pre_Dif>200)//�����ˮ20KPA
					{
						Pump_Target_Speed+=200;//ˮ��ת������200ת
					}
					else if((H_Water_Pre_Dif<100)||(Stack1_InWater_Pre>2000))//�����ˮ С��10KPA  ����ˮѹ����100kpa��ת��
					{
						if(Pump_Target_Speed>=2700)//��֤ˮ�����ת��2500
						{
							Pump_Target_Speed-=200;//ˮ��ת�ټ���200ת
						}
					}
				}
				else if((H_Water_Pre_Dif>500)&&(Pump_Target_Speed<6400)&&(Stack1_InWater_Pre<1900)&&(Stack1_H_Pre<2200))//�����ˮ50KPA  ת�ٵ���6400ת  ˮѹ������90KPA  ����ѹ��������120KPA(��Ϊ���ˮѹ����ܳ���100KPA)  ������ʱˮѹ100kpa��������ѹ��160kpa�����߱�������50kpaѹ����������������������ѹ������120kpa���ж�ѹ����ϣ�
				{
					Pump_Target_Speed+=500;
					Water_Pre_State_Flag++;
					if(Water_Pre_State_Flag>=10)
					{
						Water_Pre_State_Flag=10;
						Fault_Level_Four|=0X0008;//�ϱ�����
						fualt(40503);//�ϱ�������
					}
				}
				else if(H_Water_Pre_Dif<-500)//��С��ˮ50KPA
				{
					Pump_Target_Speed-=500;
					Water_Pre_State_Flag++;
					if(Water_Pre_State_Flag>=10)
					{
						Water_Pre_State_Flag=10;
						Fault_Level_Four|=0X0008;//�ϱ�����
						fualt(40504);//�ϱ�������
					}
				}

				// 1������µ�״̬  �µ�ʱˮ��ת�ٿ��ٽ��ͣ�������ˮ��ת���½�̫��������ˮѹ�����Ե��Ӱ��
				if((Stack_State&0X10)==0X10)
				{
					Pump_Target_Speed-=400;//ˮ��ת�ټ���200ת
				}
			}

			// 1������µ�״̬  �µ�ʱˮ��ת�ٿ��ٽ��ͣ�������ˮ��ת���½�̫��������ˮѹ�����Ե��Ӱ��
            // 2��ˮѹ����100KPA����ˮ��ת�٣����ˮѹ���ܳ���100KPA��
//			if(((Stack_State&0X10)==0X10)||(Stack1_InWater_Pre >2000))
			if(Stack1_InWater_Pre >2000)
			{
				Pump_Target_Speed-=200;//ˮ��ת�ټ���200ת
			}

		    //��ʹ��״̬�£�����ˮ�õ��������ת��
			if(Pump_Target_Speed>6400)Pump_Target_Speed=6400;
			if(Pump_Target_Speed<2000)Pump_Target_Speed=2000;
    	}
    	else//���ڷ�ʹ��״̬��
    	{
    		Pump_Target_Speed=0;//��ʹ��״̬�£�ת������Ϊ0
    	}

    	if(LowTempStart_Flag == 1)//��������״̬�¶�ת������
    	{
    		if(Pump_Target_Speed>6400)Pump_Target_Speed=6400;
    		if(Pump_Target_Speed<2000)Pump_Target_Speed=2000;
    	}

    	S_PUMP_Data[0] = PUMP_Enable;
		S_PUMP_Data[2]=Pump_Target_Speed%256;
		S_PUMP_Data[3]=Pump_Target_Speed>>8;
    }
	REN_CAN_Send(ID_Send_PUMP,CAN0_TX_MAILBOX0,S_PUMP_Data[0],S_PUMP_Data[1],S_PUMP_Data[2],S_PUMP_Data[3],
			S_PUMP_Data[4],S_PUMP_Data[5],S_PUMP_Data[6],S_PUMP_Data[7]); //
}
///*************************************************************/
///*                    ���͸���ѭ���ñ��ĺ���                                 */
///*************************************************************/
//void Send_QXHB(void)
//{
//	if(R_QXHB_Data[1]==0X0F)//�ֶ���������
//	{
//		REL_State4|=0X80;//���͸���λ���ֶ�����
//		S_QXHB_Data[0]=R_QXHB_Data[0];
//		S_QXHB_Data[1]=R_QXHB_Data[1];
//		S_QXHB_Data[2]=R_QXHB_Data[2];
//		S_QXHB_Data[3]=R_QXHB_Data[3];
//		S_QXHB_Data[4]=R_QXHB_Data[4];
//		S_QXHB_Data[5]=R_QXHB_Data[5];
//		S_QXHB_Data[6]=R_QXHB_Data[6];
//		S_QXHB_Data[7]=R_QXHB_Data[7];
//	}
//	else//�Զ�����
//	{
//		REL_State4&=0X7F;//���͸���λ���Զ�����
//		if(H_PUMP_Enable==0X05)//����ʹ��
//		{
//			H_Pump_Target_Speed = ((DC_IN_I+2)/50)*1000+1000;//ÿ����50A��ѭ����ת������1000ת ����250A��ʱ��ѭ���þ����ת��
//			//��ΪDC��������������ƫ�ƣ�����10��1A��������Ҫ���ϸ�23��2.3A������ֹ����50A��ʱ�����ر䶯
//			//ֻҪ��ʹ��״̬�£���С1000ת�����6000ת
//			if(H_Pump_Target_Speed<=1000)H_Pump_Target_Speed=1000;
//			if(H_Pump_Target_Speed>=5000)H_Pump_Target_Speed=5000;
//		}
//		else//������ʹ��
//		{
//			H_Pump_Target_Speed=0;
//		}
//		S_QXHB_Data[0]=H_PUMP_Enable;
//		S_QXHB_Data[4]=(H_Pump_Target_Speed%256);//���ֽ�
//		S_QXHB_Data[5]=(H_Pump_Target_Speed>>8);//���ֽ�
//	}
//	REN_CAN_Send(ID_Send_QXHB,S_QXHB_Data[0],S_QXHB_Data[1],S_QXHB_Data[2],S_QXHB_Data[3],
//			S_QXHB_Data[4],S_QXHB_Data[5],S_QXHB_Data[6],S_QXHB_Data[7]); //
//}

/*************************************************************/
/*                    ���͸���ѭ���ñ��ĺ���  ����                                */
/*************************************************************/
void Send_QXHB(void)
{
	QXHB_CANLife++;
		if(QXHB_CANLife > 255)
		{
			KQBYF_CANLife = 0;
		}
	if((R_KQBYF_QXHB_PUMP_Data[1]&0X04)==0x04)//�ֶ���������
	{
		REL_State4|=0X80;//���͸���λ���ֶ�����
		S_QXHB_Data[0]=0;
		S_QXHB_Data[1]=0;
		S_QXHB_Data[2]=R_KQBYF_QXHB_PUMP_Data[2];
		S_QXHB_Data[3]=R_KQBYF_QXHB_PUMP_Data[3];
		S_QXHB_Data[4]=R_KQBYF_QXHB_PUMP_Data[4];
		S_QXHB_Data[5]=0;
		S_QXHB_Data[6]=0;
//		S_QXHB_Data[7]=0;
		S_QXHB_Data[7]=QXHB_CANLife;
		H_Pump_Target_Speed=S_QXHB_Data[2]+S_QXHB_Data[3]*256;
	}
	else//�Զ�����
	{
		REL_State4&=0X7F;//���͸���λ���Զ�����
		if(H_PUMP_Enable==0X01)//����ʹ��
		{
			H_Pump_Target_Speed = ((DC_IN_I+2)/40)*500+2500;//ÿ����40A��ѭ����ת������500ת ����440A��ʱ��ѭ���þ����ת��
			if(DC_IN_I>440)
			{
				H_Pump_Target_Speed = 8000;
			}
//			//��ΪDC��������������ƫ�ƣ�����10��1A��������Ҫ���ϸ�23��2.3A������ֹ����50A��ʱ�����ر䶯
			//ֻҪ��ʹ��״̬�£���С1000ת�����10000ת
			if(H_Pump_Target_Speed<=1000)H_Pump_Target_Speed=1000;
			if(H_Pump_Target_Speed>=8000)H_Pump_Target_Speed=8000;

//			if(DC_IN_I<=200)              //����С�ڵ���200A
//			{
//				H_Pump_Target_Speed = 3000;//����ѭ����ת��3000ת
//			}
//			else if(DC_IN_I<=250)         //����С�ڵ���250A
//			{
//				H_Pump_Target_Speed = 3500;//����ѭ����ת��3500ת
//			}
//			else if(DC_IN_I<=300)         //����С�ڵ���300A
//			{
//				H_Pump_Target_Speed = 5000;//����ѭ����ת��5000ת
//			}
//			else                          //��������300A
//			{
//				H_Pump_Target_Speed = 6000;//����ѭ����ת��6000ת
//			}
		}
		else//������ʹ��
		{
			H_Pump_Target_Speed=0;
		}
		S_QXHB_Data[4]=H_PUMP_Enable;
		S_QXHB_Data[2]=(H_Pump_Target_Speed%256);//���ֽ�
		S_QXHB_Data[3]=(H_Pump_Target_Speed>>8);//���ֽ�
		S_QXHB_Data[7]=QXHB_CANLife;
	}
	REN_CAN_Send(ID_Send_QXHB,CAN0_TX_MAILBOX0,S_QXHB_Data[0],S_QXHB_Data[1],S_QXHB_Data[2],S_QXHB_Data[3],
			S_QXHB_Data[4],S_QXHB_Data[5],S_QXHB_Data[6],S_QXHB_Data[7]); //
}
/*************************************************************/
/*                   ���͸�����ѹ�����ĺ���                                */
/*************************************************************/
void Send_JYF(void)
{
  REN_CAN_Send(ID_Send_JYF,CAN0_TX_MAILBOX1,S_JYF_Data[0],S_JYF_Data[1],S_JYF_Data[2],S_JYF_Data[3],
		  S_JYF_Data[4],S_JYF_Data[5],S_JYF_Data[6],S_JYF_Data[7]); //
}
/*************************************************************/
/*                    ���͸�������ѹ�����ĺ���                             */
/*************************************************************/
void Send_KQBYF(void)
{
	KQBYF_CANLife++;
	if(KQBYF_CANLife > 128)
	{
		KQBYF_CANLife = 0;
	}
	if((R_KQBYF_QXHB_PUMP_Data[1]&0X01)==0X01)//�ֶ�����
	{
		REL_State3|=0X01;//���͸���λ���ֶ�����
		KQBYF_Target_Angle = R_KQBYF_QXHB_PUMP_Data[0];
		if(KQBYF_Target_Angle < 0)
		{
			KQBYF_Target_Angle = 0;
		}
		else if(KQBYF_Target_Angle > 120)
		{
			KQBYF_Target_Angle = 120;
		}
	}
	else//�Զ�����
	{
		REL_State3&=0XFE;//���͸���λ���ֶ�����
		if(S_SJKYJ_State==0X05&&SJKYJ_Speed<=500)//��ѹ����ʹ��ʱ����ѹ���رգ�Ŀ�ģ�������·�ܱգ�
		{
			KQBYF_Target_Angle=0;//��ѹ���Ƕ�0��
		}
		else
		{
			if(SJKYJ_Target_Speed<=60000)//��ѹ��Ŀ��ת��С��53000
			{
				KQBYF_Target_Angle=50;//��ѹ���Ƕ�50��
			}
			else if(SJKYJ_Target_Speed<=65000)//��ѹ��Ŀ��ת��60000-70000
			{
				KQBYF_Target_Angle=60;//��ѹ���Ƕ�50��
			}
			else if(SJKYJ_Target_Speed<=70000)//��ѹ��Ŀ��ת��60000-70000
			{
				KQBYF_Target_Angle=70;//��ѹ���Ƕ�60��
			}
			else if(SJKYJ_Target_Speed<=75000)//��ѹ��Ŀ��ת��70000-75000
			{
				KQBYF_Target_Angle=80;//��ѹ���Ƕ�70��
			}
			else if(SJKYJ_Target_Speed<=80000)//��ѹ��Ŀ��ת��75000-80000
			{
				KQBYF_Target_Angle=90;//��ѹ���Ƕ�90��
			}
			else if(SJKYJ_Target_Speed<=87000)//��ѹ��Ŀ��ת��80000-87000
			{
				KQBYF_Target_Angle=110;//��ѹ���Ƕ�110��
			}
			else
			{
				KQBYF_Target_Angle=120;//��ѹ���Ƕ�120��
			}
			if((Stack_State&0X0F)==0X07){ //�����׶νǶȿ������ ���ڴ�ɨ
				KQBYF_Target_Angle=90;//��ѹ���Ƕ�90��
			}
			if(LowTemp_FanCon_Flag == 1)//����ͣ��ʱ�򣬵�������������
			{
				KQBYF_Target_Angle=120;//��ѹ���Ƕ�120��
			}
		}
	}
	S_KQBYF_Data[0]=KQBYF_Target_Angle;
	S_KQBYF_Data[1]=KQBYF_CANLife;
	S_KQBYF_Data[2]=0;
	S_KQBYF_Data[3]=0;
	S_KQBYF_Data[4]=0;
	S_KQBYF_Data[5]=0;
	S_KQBYF_Data[6]=0;
	S_KQBYF_Data[7]=0;
	REN_CAN_Send(ID_Send_KQBYF,CAN0_TX_MAILBOX0,S_KQBYF_Data[0],S_KQBYF_Data[1],S_KQBYF_Data[2],S_KQBYF_Data[3],
			S_KQBYF_Data[4],S_KQBYF_Data[5],S_KQBYF_Data[6],S_KQBYF_Data[7]); //
}
/*************************************************************/
/*                    ���͸�DCDC���ĺ���                                       */
/*************************************************************/
void Send_DC(void)
{
	static uint8_t Life=0;
	static uint8_t Time_1S=0;
	static uint8_t stack_flag = 0; // ѹ�����Ʊ��
	static uint8_t first_flag = 0; // ��һ���������
	static uint8_t soc_flag = 0;
	//  static uint8_t Time_30S=0;
	static uint16_t I_Rise_Time=0;//��������ʱ��
	int16_t DC_In_Last=0;

	if((R_DC_MAN_Data[0]&0XF0)==0XF0)//�ֶ���������
	{
		REL_State3|=0X02;//���͸���λ���ֶ�����
		S_DC_State = (R_DC_MAN_Data[0]&0X0F);
		S_DC_I = (R_DC_MAN_Data[4]<<8)+R_DC_MAN_Data[5];
		DC_Target_I = (S_DC_I/10);
		//�ֶ����Ƶ�ʱ����Ҫ���ھ��� ��λ���Ѿ���ֵ
	}
//	else if((R_DC_MAN_Data[0]&0XF0)==0X10)//�ػ���ʱ��������
//	{
//		S_DC_State = (R_DC_MAN_Data[0]&0X0F);
//		S_DC_I = DC_Target_I*10;
//	}
	else if((Stack_State&0X10)!=0X10)//�Զ�����    ��ӵ�ѷǹػ�״̬ԭ�򣺷�ֹ�ػ�ʱ����������䵼���µ罵����ʱ����10A��������
	{
		REL_State3&=0XFD;//���͸���λ���Զ�����


		if(S_DC_State==0X0B)//�����ǿ���״̬
		{
			Up_DCDC_Enable = 0;

			I_Rise_Time++;
			if(I_Rise_Time>=(CAL_Data[9]*10))//�����˵�������ʱ��
			{
				if(first_flag==0){
					first_flag = 1;
				}
				I_Rise_Time=0;
				DC_Target_I+=(CAL_Data[10]);//ÿ�ε���������
			}
			if(Soc_Data>CAL_Data[57])//SOC���� ���ʵ�1  100A
			{
				if(DC_Target_I>=CAL_Data[11])
					DC_Target_I = CAL_Data[11];
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[58])//SOC���� ���ʵ�2  150A
			{
				if(DC_Target_I>=CAL_Data[12])
					DC_Target_I = CAL_Data[12];
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[59])//SOC���� ���ʵ�3  200A
			{
				if(DC_Target_I>=(CAL_Data[13]*2))//��Ϊ����궨��ֵ�Ǿ�������2�ģ�����200A��ʵ�ʱ궨ֵ��100��
					DC_Target_I = CAL_Data[13]*2;
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[60])//SOC���� ���ʵ�4  250A
			{
				if(DC_Target_I>=(CAL_Data[14]*2))//��Ϊ����궨��ֵ�Ǿ�������2�ģ�����250A��ʵ�ʱ궨ֵ��125��
					DC_Target_I = CAL_Data[14]*2;
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[61])//SOC���� ���ʵ�4  300A
			{
				if(DC_Target_I>=(CAL_Data[15]*2))//��Ϊ����궨��ֵ�Ǿ�������2�ģ�����300A��ʵ�ʱ궨ֵ��150��
					DC_Target_I = CAL_Data[15]*2;
					soc_flag = Soc_Data;
			}
			else //SOC���� ���ʵ�5  360A
			{
				if(DC_Target_I>=(CAL_Data[15]*2+60))//��60��Ϊ�궨���ڲ����ã�����һ�����ڻ����ϼ�60
					DC_Target_I = CAL_Data[15]*2+60;
					soc_flag = Soc_Data;
			}
			//������ˮ��С��68�� ������100A ��Ϊ��40�ȵ�ƫ����������68�Ⱦ���108
			if((FCU_T1_Data<70)&&(StartUpCurrent_Flag == 0))
			{
				if(DC_Target_I>=(CAL_Data[11]-70))DC_Target_I=(CAL_Data[11]-70);
			}
			else if((FCU_T1_Data>=70)&&(FCU_T1_Data<90)&&(StartUpCurrent_Flag == 0))
			{
				if(DC_Target_I>=(CAL_Data[11]-40))DC_Target_I=(CAL_Data[11]-40);
			}
			else if((FCU_T1_Data>=90)&&(FCU_T1_Data<108)&&(StartUpCurrent_Flag == 0))
			{
				if(DC_Target_I>=CAL_Data[11])DC_Target_I=CAL_Data[11];
			}
			else
			{
				StartUpCurrent_Flag = 1;
			}
			//  ����� ���
//			if(((Stack1_Max_V_Dif>50)||(Stack2_Max_V_Dif>50)||(stack_flag==1))&&(DC_Target_I>=50)) // ά��ϵͳ��������ѹ�� С��65
//			{
//				DC_Target_I=50;
//				stack_flag=1;
//			}
//			if((Stack1_Max_V_Dif<35) && (Stack2_Max_V_Dif<35))
//			{
//				stack_flag=0;//������
//			}
			// ԭ�س�ʱ����ʹ��� �����޹�����
			if(DC_Target_I>150&&Car_Speed==0)
			{
				Time_10min++;
				if(Time_10min>=6000)  // 10���� ��ǰ��������100ms
				{
					DC_Target_I = 150;
					Time_10min=6000;
				}
			}
			if(Car_Speed>2){ //���������¼���
				Time_10min=0;
			}

			if(vcu2fcu_command==0x08&&(DC_Target_I>((vcu2fcu_limit_power*1250)/DC_IN_V)))//����VCU�����ƹ���������
			{
				DC_Target_I=(uint16_t)((vcu2fcu_limit_power*1250)/DC_IN_V);
			}
//			if(vcu2fcu_command==0x08&&(DC_Target_I>((Bms_Max_Recharge_I*DC_OUT_V)/DC_IN_V)))//����BMS�ĳ�����������
//			{
//				DC_Target_I=(uint16_t)((Bms_Max_Recharge_I*DC_OUT_V)/DC_IN_V);
//			}
			// ���ӽ�������ʱ�µ�
			if(Normal_Close_Flag==1&&DC_Target_I>=50)
			{
				DC_Target_I=50;
				if(DC_IN_I<60)
				{
					Low_Power_Time++;
					if(LowTemp_Close_Flag == 1)
					{
						LowTemp_FanCon_Flag = 1;
						if(Low_Power_Time>=1800)//�͹�������3����
						{
							Normal_Close_Flag=0;//��ʱ�ػ����
							VCU_Ready_State = 0x03;//����ػ�����
							Low_Power_Time=0;
						}
					}
					else
					{
						if(Low_Power_Time>=200)//�͹�������20��
						{
							Normal_Close_Flag=0;//��ʱ�ػ����
							VCU_Ready_State = 0x03;//����ػ�����
							Low_Power_Time=0;
						}
					}
				}
			}

			Time_1S++;
			if(Time_1S>=20)//2�������� �������Ͻ�����
			{
				if(Fault_Level_Three)//���������ϣ���Ҫ���ƹ���Ϊ��ǰһ��
				{
					Time_1S=20;
		//				if(Time_30S)
		//				DC_Target_I = DC_Target_I/2;//Ŀ��������Ƶ���ǰһ��
					DC_In_Last = DC_Target_I*5;//Ŀ��������Ƶ���ǰһ��
				}
				else//����������
				{
					Time_1S=0;
					DC_In_Last = DC_Target_I*10;//1�����Ժ�Ž�Ŀ�����ֵ��������ֵ���ÿ�ѹ��ת���ȵ��� S_DC_I�ľ�����0.1A��DC_Target_I������1A��������Ҫ����10
					I_ADD = DC_In_Last - (DC_Target_Old*10);//
					if(I_ADD>=200)//��ֹ����������ʧ�����ֱ�ӻָ�Ŀ��ֵ
					{
						DC_In_Last = DC_Target_Old*10;
						DC_Target_I = DC_Target_Old;
					}
				}

				I_Cha=(DC_Target_Old*10)-DC_In_Last;
				if(I_Cha>=100)//���������ȳ���10A�����л�����
				{
					DC_Target_Old-=20;//Ŀ�����ÿ�μ���2A
					S_DC_I=DC_Target_Old*10;
				}
				else
				{
					S_DC_I=DC_In_Last;
//					DC_Target_Old=DC_Target_I;
					if(Fault_Level_Three!=0)
					{
						;
					}
					else
					{
						DC_Target_Old=DC_Target_I;
					}
				}
			}
		}
		else//��ǰ���ڲ�������״̬
		{
			Up_DCDC_Enable = 1;
		//		S_DC_State &=0x0f ;
			S_DC_I=0;//ֱ�ӷ���0A���� Ҳ�����������ŵ�״̬
		//		if((SJKYJ_Speed > 25000)&(SJKYJ_Speed < 33000))
		//		{
		//			S_DC_State=0X0C;//DC�����ŵ�ģʽ
		//			DC_Target_I=10;
		//		}
		//		else
		//		{
		//
		//		}
		}
	}

//	if(((Stack_State&0X10)==0X10)&&((R_DC_MAN_Data[0]&0XF0)==0XF0))//��ֹ�л����ֶ�����ʱ������ֵΪ0A������0A��10A�����л�
	if((Stack_State&0X10)==0X10)
    {
		S_DC_State = (R_DC_MAN_Data[0]&0X0F);
		S_DC_I = DC_Target_I*10;//100;//DC_Target_I*10;

		if((R_DC_MAN_Data[0]&0XF0)==0XF0)
		{
			S_DC_I = 100;
		}
    }
	S_DC_Data[0] = S_DC_I%256;
	S_DC_Data[1] = S_DC_I/256;
	S_DC_Data[2] = 0;
	S_DC_Data[3] = 0;
	S_DC_Data[4] = 0;
	S_DC_Data[5] = S_DC_State;
	S_DC_Data[6] = 0;
	REN_CAN_Send(ID_Send_DC,CAN0_TX_MAILBOX0,S_DC_Data[0],S_DC_Data[1],S_DC_Data[2],S_DC_Data[3],
		  S_DC_Data[4],S_DC_Data[5],S_DC_Data[6],Life); //���͸�DCDC 0x0CFF01A7
	Life++;
}

/*************************************************************/
/*                    ���͸�DCDC2���ĺ���                                      */
/*************************************************************/
void Send_DC2(void)
{
	static uint8_t Life=0;
	  REN_CAN_Send(ID_Send_DC2,CAN0_TX_MAILBOX0,0XDC,0X05,0X64,0X19,
			  0,0,0,Life); //���͸�DCDC����2 ������150A ��ߵ�ѹ650V
	  Life++;
}
/*************************************************************/
/*                ���͸��Ƽ�͸����ѹ�����ĺ���                              */
/*************************************************************/
void Send_SJKYJ(void)
{
	static uint8_t SJKYJ_Life=0;//��ѹ��Life
	uint8_t i=0;
	static uint16_t SpeedChange_Count=0;//ת�ٲ�����ʱ����

	if(R_SJKYJ_Data[7]==0X0F)//�ֶ���������
	{
		REL_State3|=0X08;//���͸���λ���ֶ�����
		S_SJKYJ_State=R_SJKYJ_Data[0];
		S_SJKYJ_Data[1]=R_SJKYJ_Data[1];
		S_SJKYJ_Data[3]=R_SJKYJ_Data[3];
		S_SJKYJ_Data[4]=R_SJKYJ_Data[4];
		S_SJKYJ_Data[5]=R_SJKYJ_Data[5];
		SJKYJ_Target_Speed = R_SJKYJ_Data[3]+(R_SJKYJ_Data[4]<<8)+(R_SJKYJ_Data[5]<<16);
		S_SJKYJ_Data[6]=0;
		S_SJKYJ_Data[7]=0;
	}
	else//�Զ�����
	{
		REL_State3&=0XF7;//���͸���λ���Զ�����
		if(S_SJKYJ_State == 0X0B)//��ѹ��״̬����ʹ��λ
		{
//			if((Fault_Level_Two&0X0200)==0X0200)SJKYJ_Target_Speed_Change=2000;//CVM������ܵ�ѹ�ж�������  ��ѹ��ת������1000ת
//			if((Fault_Level_Three&0X0200)==0X0200)SJKYJ_Target_Speed_Change=3000;//CVM������ܵ�ѹ���������� ��ѹ��ת������1000ת
			if(((Fault_Level_Two&0X0200)!=0X0200)&&((Fault_Level_Three&0X0200)!=0X0200))//��û�ж�������Ҳû����������
		    {
				SpeedChange_Count++;
				if(SpeedChange_Count >= 500)
				{
					SpeedChange_Count = 500;
					SJKYJ_Target_Speed_Change=0;
				}
		    }
			else
			{
				SpeedChange_Count = 0;
			}
			if((DC_Target_I*10)>S_DC_I)//Ŀ���������ʵ�ʵ���
			{
			   Speed_Adjust_I =  DC_Target_I; //���ڵ����͸���Ŀ����������� ����1A
			}
			else Speed_Adjust_I = (S_DC_I/10);//���ڵ����͸���ʵ�ʵ���������  ����1A
			for(i=1;i<7;i++)
			{
			   if(Speed_Adjust_I<=(CAL_Data[49+i]*2))//λ�ڵ�ǰ������  ����Ҫ���ϵ�ǰƫ�Ƶ�
			   {
				   SJKYJ_Target_Speed =SJKYJ_Target_Speed_Change +  (CAL_Data[33+i-1]*1000) +
				(Speed_Adjust_I - (CAL_Data[49+i-1]*2))*((CAL_Data[33+i] - CAL_Data[33+i-1])*1000)/((CAL_Data[49+i]-CAL_Data[49+i-1])*2);
				   break;
			   }
			}
		}
		else
		{
			SJKYJ_Target_Speed=0;//��ʹ��״̬�£�ת��ֱ��Ϊ0
		}
	}
	if((LowTemp_Close_Flag == 1)&&(LowTemp_FanCon_Flag == 1)&&(Low_Power_Time<=7200))//�����µ�ʱ��������������Ҫ����10����50Aʱ�Ӵ��ѹ��ת��
	{
		LowTempSpeedCompensate = LowTempSpeedCompensate + 500;
		if(LowTempSpeedCompensate > 20000)
		{
			LowTempSpeedCompensate = 20000;
		}
		SJKYJ_Target_Speed = SJKYJ_Target_Speed + LowTempSpeedCompensate;
	}
	else
	{
		LowTempSpeedCompensate = 0;
	}
    if(SJKYJ_Target_Speed>65535)
    {
    	if(SJKYJ_Target_Speed>93000)SJKYJ_Target_Speed=93000;
    	S_SJKYJ_Data[5] = 1;
    	S_SJKYJ_Data[4] =((SJKYJ_Target_Speed-65536)>>8);
    	S_SJKYJ_Data[3] =((SJKYJ_Target_Speed-65536)%256);
    }
    else
    {
    	S_SJKYJ_Data[5]=0;
    	S_SJKYJ_Data[4] =((SJKYJ_Target_Speed-65536)>>8);
    	S_SJKYJ_Data[3] =((SJKYJ_Target_Speed-65536)%256);
    }
    if(SJKYJ_Fualt_Class==2&&S_SJKYJ_State==0X0B){
    	S_SJKYJ_State = 0xFE;// �������ϸ�λ
    }
	REN_CAN_Send(ID_Send_SJKYJ,CAN0_TX_MAILBOX0,S_SJKYJ_State,S_SJKYJ_Data[1],SJKYJ_Life,S_SJKYJ_Data[3],
			S_SJKYJ_Data[4],S_SJKYJ_Data[5],S_SJKYJ_Data[6],S_SJKYJ_Data[7]); //
	if(SJKYJ_Fualt_Class==2&&S_SJKYJ_State==0xFE){
		S_SJKYJ_State = 0X0B;// ��λ������
	}
	SJKYJ_Life++;
}

/*************************************************************/
/*                    ���͸�6kW����DCDC���ĺ���                           */
/*************************************************************/
void Send_6KWDC(void)
{
	static uint8_t F_Enable = 0;

	if(R_JWQ_PTC_6KWDC_Data[2]==0X0F)//�ֶ�����
	{
		REL_State4|=0X08;//���͸���λ���ֶ�����

		S_6KWDC_Data[0]=0;
		S_6KWDC_Data[1]=0;
		S_6KWDC_Data[2]=0;
		S_6KWDC_Data[3]=R_JWQ_PTC_6KWDC_Data[3];
		S_6KWDC_Data[4]=0;
		S_6KWDC_Data[5]=0;
		S_6KWDC_Data[6]=0;
		S_6KWDC_Data[7]=0;

	}
	else
	{
		REL_State4&=0XF7;//���͸���λ���Զ�����

		if(DC_OUT_V < 450)
		{
			F_Enable = 0x01;//�ػ�
			Down_DC_Enable = 1;
		}
		else if(DC_OUT_V >=450)
		{
			F_Enable = 0x02;//����
			Down_DC_Enable = 0;
		}
		else
		{
			;
		}

		S_6KWDC_Data[3]=F_Enable;
	}

	REN_CAN_Send(ID_Send_6KWDC,CAN0_TX_MAILBOX0,S_6KWDC_Data[0],S_6KWDC_Data[1],S_6KWDC_Data[2],S_6KWDC_Data[3],
			S_6KWDC_Data[4],S_6KWDC_Data[5],S_6KWDC_Data[6],S_6KWDC_Data[7]); //
}

/*************************************************************/
/*                       ���͸����������ĺ���                                                                   */
/*************************************************************/
void Send_JWQ(void)
{

//	static uint8_t JWQ_Angle_Flag = 0;//���ݽ�ˮ�¶ȿ��ƽ������Ƕȱ�־λ

	if(R_JWQ_PTC_6KWDC_Data[1]==0X0F)//�ֶ�����
	{
		REL_State4|=0X10;//���͸���λ���ֶ�����
		if (R_JWQ_PTC_6KWDC_Data[0]==1)
		{

		}
		S_JWQ_Data[0]=R_JWQ_PTC_6KWDC_Data[0];
		S_JWQ_Data[1]=R_JWQ_PTC_6KWDC_Data[1];
		S_JWQ_Data[2]=R_JWQ_PTC_6KWDC_Data[2];
		S_JWQ_Data[3]=R_JWQ_PTC_6KWDC_Data[3];
		S_JWQ_Data[4]=R_JWQ_PTC_6KWDC_Data[4];
		S_JWQ_Data[5]=R_JWQ_PTC_6KWDC_Data[5];
		S_JWQ_Data[6]=R_JWQ_PTC_6KWDC_Data[6];
		S_JWQ_Data[7]=R_JWQ_PTC_6KWDC_Data[7];
	}
	else
	{
		REL_State4&=0XEF;//���͸���λ���Զ�����
		if(JWQ_Enable == 0X01)//��������������״̬
		{
			if(((Stack_State&0X17)==0X07))//ֻ���������л��������е�ʱ�������²���
			{
				if(FCU_T1_Data <= 104)//�¶�С��65�� ��С�Ƕ� ȫ�Լ���ͨ��
				{
					JWQ_Target_Angle = 0;//JWQ����·�򿪣�ɢ����·����һ��
				}
				else if((FCU_T1_Data > 104)&&(FCU_T1_Data <= 108))
				{
					JWQ_Target_Angle = 15;
				}
				else if((FCU_T1_Data > 108)&&(FCU_T1_Data <= 109))
				{
					JWQ_Target_Angle = 30;//JWQ����·�򿪣�ɢ����·����һ��
				}
	//			else if((FCU_T1_Data > 109)&&(FCU_T1_Data <= 110))
	//			{
	//				JWQ_Target_Angle = 40;//JWQ����·��ɢ����·ȫͨ
	//			}
	//			else if((FCU_T1_Data > 110)&&(FCU_T1_Data <= 113))
	//			{
	//				JWQ_Target_Angle = 50;//JWQ����·��ɢ����·ȫͨ
	//			}
				else if(FCU_T1_Data > 109)
				{
					JWQ_Target_Angle = 60;//�Ƕ�100 ˮ·ȫ����ɢ����ͨ�� �ر��Լ���
				}
				if(DC_Target_I>300)
				{
					JWQ_Target_Angle = 60;
				}
			}
			else
			{
				if((LowTemp_Close_Flag == 1) && (DC_Target_I > 10))//���ӵ����ж�ԭ���ڵ��¹ػ�20A����ʱ���ֹˮ���ڵ���ˮ�������ڵ��µ�Ƭ��ѹ����
				{
					if(FCU_T1_Data <= 112)//�¶�С��70�� ��С�Ƕ� ȫ�Լ���ͨ��
					{
						JWQ_Target_Angle = 0;//
					}
					else if(FCU_T1_Data > 113)
					{
						JWQ_Target_Angle = 60;//�Ƕ�60
					}
				}
				else
				{
					JWQ_Target_Angle = 60; //�Ƕ�60 ˮ·ȫ����ɢ����ͨ��
				}
			}
		}
		else
		{
			JWQ_Target_Angle = 60; //�Ƕ�60 ˮ·ȫ����ɢ����ͨ��
		}

		S_JWQ_Data[0]=0x01;//�������ģʽ����
		S_JWQ_Data[7]=JWQ_Target_Angle;//�������
	}

	REN_CAN_Send(ID_Send_JWQ,CAN0_TX_MAILBOX0,S_JWQ_Data[0],S_JWQ_Data[1],S_JWQ_Data[2],S_JWQ_Data[3],
			S_JWQ_Data[4],S_JWQ_Data[5],S_JWQ_Data[6],S_JWQ_Data[7]); //
}

/*************************************************************/
/*                       ���͸�PTC���ĺ���                                                                   */
/*************************************************************/
void Send_PTC(void)
{

	if(R_JWQ_PTC_6KWDC_Data[4]==0X0F)//�ֶ�����
	{
		REL_State4|=0X20;//���͸���λ���ֶ�����
		S_PTC_Data[0]=R_JWQ_PTC_6KWDC_Data[0];
		S_PTC_Data[1]=R_JWQ_PTC_6KWDC_Data[1];
		S_PTC_Data[2]=R_JWQ_PTC_6KWDC_Data[2];
		S_PTC_Data[3]=R_JWQ_PTC_6KWDC_Data[3];
		S_PTC_Data[4]=100;
		S_PTC_Data[5]=120;
		S_PTC_Data[6]=R_JWQ_PTC_6KWDC_Data[6];
		S_PTC_Data[7]=R_JWQ_PTC_6KWDC_Data[7];
	}
	else
	{
		REL_State4&=0XDF;//���͸���λ���Զ�����
		if(((FCU_T1_Data < 45)||(FCU_T4_Data < 45))&&((Stack_State&0X0F)==0X07) &&(Pump_Speed > 1500))//��ѳ�����¶ȵ���5�棬�������ʱ������PTC����
		{
			PTC_Enable = 0X03;//PTC�������������¶ȿ���
//			PTC_Flag = 1;
			PTC_test_Time++;
		}
		else if((Stack_State&0X10)==0X10)//�µ�  ���״̬
		{
			PTC_Enable = 0X00;//PTC�ر�
			PTC_Target_Temp = 40;//0�棬��40��ƫ����
			PTC_Target_PWM = 0;//PWM���̶�ֵ0
		}

		if(FCU_T1_Data > 80)//���������¶ȴﵽ25��󣬹ر�PTC
		{
//			PTC_Flag = 0;
			PTC_Enable = 0X00;//PTC�ر�
			PTC_Target_Temp = 40;//0�棬��40��ƫ����
			PTC_Target_PWM = 0;//PWM���̶�ֵ0
		}

		if(PTC_Enable == 0X03)//PTC��������״̬
		{
			PTC_Target_Temp = 120;//80�棬��40��ƫ����
			PTC_Target_PWM = 100;//PWM���̶�ֵ100
		}

		S_PTC_Data[4]=PTC_Target_PWM;
		S_PTC_Data[5]=PTC_Target_Temp;
		S_PTC_Data[6]=PTC_Enable;
	}

	REN_CAN_Send(ID_Send_PTC,CAN0_TX_MAILBOX0,S_PTC_Data[0],S_PTC_Data[1],S_PTC_Data[2],S_PTC_Data[3],
			S_PTC_Data[4],S_PTC_Data[5],S_PTC_Data[6],S_PTC_Data[7]); //
}

/*************************************************************/
/*                                                           */
/*************************************************************/
void CAN0_Send_Out(void)
{
	// 100ms
	if ((Timer_TxMsg % 10) == 0) {
		Send_PUMP();//���͸�ˮ��
	} else if ((Timer_TxMsg % 10) == 1) {
		Send_QXHB();//���͸�����ѭ����
	} else if ((Timer_TxMsg % 10) == 2) {
		Send_KQBYF();//���͸�������ѹ��
	} else if ((Timer_TxMsg % 10) == 3) {
		Send_DC();//���͸���ѹDC����1
	} else if ((Timer_TxMsg % 10) == 4) {
		Send_DC2();//���͸���ѹDC����2
	} else if ((Timer_TxMsg % 10) == 5) {
		Send_SJKYJ();//���͸��Ƽ�͸����ѹ������
	} else if ((Timer_TxMsg % 10) == 6) {
		Send_JYF();//���͸�����ѹ��
	}


	//200ms
	if((Timer_TxMsg % 20) == 7){
		Send_6KWDC();//���͸�6KW��ѹСDC
	}else if((Timer_TxMsg % 20)==8){
		Send_PTC();//���͸�PTC
	}else if((Timer_TxMsg % 20)==9){
		Send_JWQ();//���͸�������
	}else if((Timer_TxMsg % 20)==6){
//		REN_CAN_Send(ID_Send_CVM1,CAN0_TX_MAILBOX0,1,0,0,0,0,0,0,0); //CVM�����������Ƶ�ַ
	}
}










