/*
 * ADC.c
 *
 *  Created on: 2018��8��21��
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
#include "Temp.h"
#include "REL.h"
#include "CAL.h"
#include "Data_Hand.h"
#include "Fault.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


uint16_t Flowmeter=0;//��������
uint32_t FCU_Flowmeter=0;
uint16_t Honeywell_R1 = 0;
uint32_t Honeywell_R2 = 0;
uint16_t Honeywell_R3 = 0;
uint16_t Honeywell_R4 = 0;


//������ѹ����������ѹ��ѹ��
uint16_t Tank_Pre=0,Tank_DY = 0,Tank_Mid_DY=0,Tank_Mid_Pre=0;
uint16_t Stack1_H2_Pre=0,Stack1_H2_DY=0;//FCU�ɼ�����ѹ��2
uint16_t Stack1_InWater_Pre=0,Stack1_InWater_DY=0;//��ˮѹ��
uint16_t Stack1_RH=0,Stack1_RH_DY=0;//�����ʪ��
//����EHI����������ѹ���ɼ�
uint16_t Stack1_H_Pre=1000,Stack1_Air_Pre=0;
//�¶ȣ�T1�����ˮ�¶ȣ�T4�����ˮ�¶ȣ�T2����Ԥ����T3�����¶�
uint32_t FCU_T1_DY=0,FCU_T2_DY=0,FCU_T3_DY=0,FCU_T4_DY=0;
uint8_t FCU_T1_Data=0,FCU_T2_Data=0,FCU_T3_Data=0,FCU_T4_Data=0;
//��������ѹ�ɼ�
uint16_t I_DY=0,Out_I=0;
//��Դ��ѹ�ɼ���5V�����������ѹ�ɼ� ��ʱû���õ�
uint8_t Sensor_Data,Battery_Data;
//����������������Ĵ�����¼
uint8_t Sensor_Fault_Times[11]={0,0,0,0,0,0,0,0,0,0,0};

//����ܹ���
uint32_t Send_Stack_Total_P=0,Stack_Total_P=0,Stack_Total_P_LS=0;//����ܹ��ʵ�λW/H
uint32_t Send_Out_Total_P=0,Out_Total_P=0,Out_Total_P_LS=0;//������ܹ��ʵ�λW/H
//����Ũ�ȴ���������
uint16_t H1_PPM=0,H2_PPM=0;
uint16_t H1_PPM_DY=0,H2_PPM_DY=0,H2_PPM_Time=0;
void ADC_Ini(void)
{
	if (adConv1_ConvConfig0.resolution == ADC_RESOLUTION_8BIT)
			adcMax = (uint16_t) (1 << 8);
		else if (adConv1_ConvConfig0.resolution == ADC_RESOLUTION_10BIT)
			adcMax = (uint16_t) (1 << 10);
		else
			adcMax = (uint16_t) (1 << 12);

	if (adConv2_ConvConfig0.resolution == ADC_RESOLUTION_8BIT)
			adcMax = (uint16_t) (1 << 8);
		else if (adConv2_ConvConfig0.resolution == ADC_RESOLUTION_10BIT)
			adcMax = (uint16_t) (1 << 10);
		else
			adcMax = (uint16_t) (1 << 12);

	ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig0);
	DEV_ASSERT(adConv1_ChnConfig0.channel == ADC_CHN12);
	ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig1);
	DEV_ASSERT(adConv1_ChnConfig1.channel == ADC_CHN6);

	ADC_DRV_ConfigConverter(ADC_INSTANCE1, &adConv2_ConvConfig0);
	DEV_ASSERT(adConv2_ConvConfig0.channel == ADC_CHN8);
	ADC_DRV_ConfigConverter(ADC_INSTANCE1, &adConv2_ConvConfig0);
	DEV_ASSERT(adConv2_ConvConfig1.channel == ADC_CHN6);

}
/*************************************************************/
/*                        ��ADת��                         */
/*************************************************************/
uint16_t AD_capture(uint8_t chanel)
{
	 uint16_t AD_data;
	 switch(chanel)
	 {
	  case 0:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig0);//��Ӧ PTC14 47����  ��ˮѹ���ɼ�
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 1:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig1);//��ӦPTB3 48����  FCU����ѹ���ɼ�
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 2:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig2);//��Ӧ PTB2 49����   ��ѹѹ���������ɼ�
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 3:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig3);//��Ӧ PTB1 50���� ����ѹ���ɼ�
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 4:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig4);//��Ӧ PTB0 19����  ����Ũ��1
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 5:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig5);//��Ӧ PTA7  20����  ����Ũ��2
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 6:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig6);//��Ӧ PTA6   21����  �������źŲɼ�
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	 case 7:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig7);//��Ӧ PTC15 T1 46���� ��ˮ�¶�
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	 case 8:
	   		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig8);//��Ӧ PTC0  18���� �����¶�
	   		/* Wait for the conversion to be done */
	   		ADC_DRV_WaitConvDone(ADC_INSTANCE);
	   		/* Store the channel result into a local variable */
	   		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
	 		break;
	 case 9:
	   		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig9);//��Ӧ PTC1   17����    �¶Ȳɼ�T3Ԥ��
	   		/* Wait for the conversion to be done */
	   		ADC_DRV_WaitConvDone(ADC_INSTANCE);
	   		/* Store the channel result into a local variable */
	   		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
	 		break;
	 case 10:
			ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig10);//��Ӧ PTC2   45����    ��ˮ�¶�
			/* Wait for the conversion to be done */
			ADC_DRV_WaitConvDone(ADC_INSTANCE);
			/* Store the channel result into a local variable */
			ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
			break;
	 }
    return AD_data;
}
/************************************************************/
/*                          ��ֵ                            */
/************************************************************/
void Get_ADC(void)
{
  static uint8_t i=0;
  switch(i)
  {
     case 0: //47�� ��ˮѹ��
         i=1;
         Stack1_InWater_DY=(uint16_t)((uint32_t)((maopao(0)))*5000/4095);
         if((Stack1_InWater_DY>4800)||(Stack1_InWater_DY<200))//��ѹ�쳣
         {
        	 Sensor_Fault_Times[0]++;
        	 if(Sensor_Fault_Times[0]>=100)//��ѹ�쳣��������100��˵��������ʧЧ
        	 {
        		 Sensor_Fault_Times[0]=100;//��ֹһֱ�Լ����
        		 Fault_Level_Four|=0X0002;
        		 fualt(40202);
        		 Stack1_InWater_Pre=1000;
        	 }
         }
         else//��ѹ����ֵ����������
         {
        	 Sensor_Fault_Times[0]=0;//�����Լ���0
//        	 Fault_Level_Four&=0XFFFD;//��������Ĺ���λ
        	 if(Stack1_InWater_DY<500)Stack1_InWater_DY=500;//��Ȼ��Ҫ���һ���Ƿ�С��0.5V
//        	 Stack1_InWater_Pre = (uint16_t)((uint32_t)(Stack1_InWater_DY-500)/8*10)+1000; //��ѹֵ����   �ɴ�������ʽ
//        	 Stack1_InWater_Pre = (uint16_t)((uint32_t)(Stack1_InWater_DY-500)/2)+1000; //��ѹֵ����   ���������ʽ��Ӧ�ó���20��������0.1�ͳ���2��
        	 Stack1_InWater_Pre = (uint16_t)((uint32_t)(Stack1_InWater_DY-500)/16*10)+1000; //��ѹֵ����   GSEM��������ʽ
         }

     break;
     case 1: //48�� FCU����ѹ��  PTB3
		 i=2;
		 Stack1_H2_DY=(uint16_t)((uint32_t)((maopao(1))*5000/4095));
		 if((Stack1_H2_DY>4800)||(Stack1_H2_DY<200))
		 {
        	 Sensor_Fault_Times[1]++;
        	 if(Sensor_Fault_Times[1]>=100)//��ѹ�쳣��������100��˵��������ʧЧ
        	 {
        		 Sensor_Fault_Times[1]=100;//��ֹһֱ�Լ����
        		 Fault_Level_Two|=0X0001;
        		 fualt(20202);
        		 Stack1_H2_Pre=1000;
        	 }
		 }
		 else
		 {
        	 Sensor_Fault_Times[1]=0;//�����Լ���0
        	 Fault_Level_Two&=0XFFFE;//��������Ĺ���λ
			 Stack1_H2_Pre = (uint16_t)((uint32_t)((Stack1_H2_DY)*20-5700)*10/211);//�����ɹ㶫���˾������20200216
//        	 Stack1_H2_Pre = (uint16_t)((uint32_t)(Stack1_H2_DY)*19/23); //
		 }
	 break;
     case 2: // 49����  PTB2  �޸�Ϊ����ʪ�ȼ��  ���ڣ�20210817
         i=3;
         Stack1_RH_DY = (uint16_t)((uint32_t)(maopao(2))*5000/4095);
//			Honeywell_R4 = (uint16_t)(((uint32_t)(10000*FCU_T4_DY))/(5000-FCU_T4_DY));
		if((Stack1_RH_DY>4800)||(Stack1_RH_DY<200))//��ѹ�쳣
		 {
//			 Sensor_Fault_Times[0]++;
//			 if(Sensor_Fault_Times[0]>=100)//��ѹ�쳣��������100��˵��������ʧЧ
//			 {
//				 Sensor_Fault_Times[0]=100;//��ֹһֱ�Լ����
//				 Fault_Level_Four|=0X0002;
//				 fualt(40202);
//				 Stack1_RH=1000;
//			 }
		 }
		 else//��ѹ����ֵ����������
		 {
			 Sensor_Fault_Times[0]=0;//�����Լ���0
//        	 Fault_Level_Four&=0XFFFD;//��������Ĺ���λ
			 if(Stack1_RH_DY<500)Stack1_RH_DY=500;//��Ȼ��Ҫ���һ���Ƿ�С��0.5V
			 Stack1_RH = (uint16_t)(Stack1_RH_DY-1032)/26.23; //��ѹֵ����  ��ʪ�ȴ����� ʪ�ȼ��㹫ʽ
		 }
     break;
     case 3: //50���� ���������ɼ�  PTB1   //------��������.���ڣ�20211014
         i=4;
         Flowmeter=(uint16_t)((uint32_t)((maopao(3))*5000/4095));
         FCU_Flowmeter = (Flowmeter_Rec(Flowmeter)*1289/100);//��������ת������
//         Tank_DY = (uint16_t)((uint32_t)(maopao(3))*5000/4095);
//         if((Tank_DY>4800)||(Tank_DY<200))//�쳣״̬
//         {
//        	 Sensor_Fault_Times[3]++;
//        	 if(Sensor_Fault_Times[3]>=10)//��ѹ�쳣��������10��˵��������ʧЧ
//        	 {
//        		 Sensor_Fault_Times[3]=10;//��ֹһֱ�Լ����
//        		 Fault_Level_Four|=0X0004;
//        		 fualt(40203);
//        		 Tank_Pre=1000;
//        	 }
//         }
//         else
//         {
//        	 Sensor_Fault_Times[3]=0;//��ֹһֱ�Լ����
//			 if(Tank_DY<500)Tank_DY=500;//��ֹ����ѹ
//			 if((REL_State1&0X40)==0X40)//ƿ�ڷ��򿪣������ڲɼ�����ֵ
//		     Tank_Pre = (Tank_DY-500)/8+1000;//��λ��bar
//		     else Tank_Pre =CAL_Data[96]*256+CAL_Data[97];//�������û�򿪣�����ѹ�������ϴιػ��洢����ֵ
//			  if(Tank_Pre>1020)//����ѹ������2����
//			  {
//				  Fault_Level_Four&=0XFFFB;
//				  Fault_Level_Three&=0XEFFF;//���3��4������
//			  }
//
//			  else if(Tank_Pre>1010)//�������ѹ��С��1MPA�޹���
//			  {
//				  Fault_Level_Three|=0X1000;//�޹���
//			  }
//			  else if(Tank_Pre>1005)//�������ѹ��С��0.5MPA�ػ�
//			  {
//				  Fault_Level_Four|=0X0004;//�ػ�
//			  }
//         }
	 break;
     case 4: //19����  ����Ũ��1         //��ʱռ������------��������.���ڣ�20200220
         i=5;
//		 Flowmeter=(uint16_t)((uint32_t)((maopao(4))*5000/4095));
//		 FCU_Flowmeter = (Flowmeter_Rec(Flowmeter)*1289/100);//��������ת������
//		 H1_PPM_DY = ((uint32_t)((maopao(4))*5000/4095));//����������1
//		 if(H1_PPM_DY<500)H1_PPM_DY=500;
//		 H1_PPM = (H1_PPM_DY-500)*25/4;//����������1Ũ�ȼ���
	 break;
     case 5: //21����  ��Ѵ�ɨ�� ����Ũ��
		 i =6;
		 H2_PPM_DY = ((uint32_t)((maopao(6))*5000/4095));//����������2
		 H2_PPM = (H2_PPM_DY-500)*10;//����Ũ��=�������ѹ-0.5V��*10000  ����ڶ���
		 if((H2_PPM_DY>4800)||(H2_PPM_DY<200))
		 {
			 Sensor_Fault_Times[5]++;
			 if(Sensor_Fault_Times[5]>=100)//��ѹ�쳣��������100��˵��������ʧЧ10s
			 {
				 Sensor_Fault_Times[5]=100;//��ֹһֱ�Լ����
				 Fault_Level_Two|=0X0004;
				 fualt(21102);//����Ũ�ȴ������쳣
				 H2_PPM=505;//505Ŀ�ĳ��������ʱ����ʾ����0�������ж�
			 }
		 }
		 else
		 {
			 if(Flag_6S>=60)
			 {
				 if(H2_PPM_DY<500)//������Сֵ
				 {
					 H2_PPM_DY=500;
				 }
				 else if(H2_PPM_DY>4500)//�������ֵ
				 {
					 H2_PPM_DY=4500;
				 }
				 else
				 {
					 ;
				 }
				 if(H2_PPM < 2000)
				 {
					 H2_PPM = 0;
					 H2_PPM_Time = 0;
				 }
				 if(H2_PPM > 22000)//ϵͳͣ�������ر�ƿ�ڷ�
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 1000)
					 {
						 fualt(41102);
//						 Fault_Level_Four|=0X0004;//�ػ�
					 }
				 }
				 else if(H2_PPM > 15000)//ϵͳͣ�������Ϸ�������������ʾ
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 1000)
					 {
						 fualt(41101);
//						 Fault_Level_Four|=0X0004;//�ػ�
					 }
				 }
				 else if(H2_PPM > 6000)//ϵͳ�����ʴ������Ϸ�������������ʾ
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 100)
					 {
						 fualt(31101);
//						 Fault_Level_Three|=0X0004;//
					 }
				 }
				 else if(H2_PPM > 3000)//ϵͳ���������Ϸ�������������ʾ
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 100)
					 {
						 fualt(21101);
						 Fault_Level_Two|=0X0004;//�Ϸ�������������ʾ
					 }
				 }
				 else//
				 {
					 H2_PPM_Time = 0;
					 Fault_Level_Two&=0XFFFB;
					 Fault_Level_Three&=0XFFFB;
		 //			 Fault_Level_Four&=0XFFFB;
				 }
			 }

		 }

     break;
     case 6: //T1  46�� ��ˮ�¶�
          i=7;
		   FCU_T1_DY=(uint16_t)((uint32_t)((maopao(7))*5000/4095));
		   Honeywell_R1 = (uint16_t)(((uint32_t)(10000*FCU_T1_DY))/(5000-FCU_T1_DY));
		   if((FCU_T1_DY>4900)||(FCU_T1_DY<20))//��ѹ̫�߻�̫��˵���¶ȴ�����û�н���
		   {
			 Sensor_Fault_Times[7]++;
//			 FCU_T1_Data = Temp_Rec2(FCU_T1_DY);//NTC10K�¶ȴ���������
			 FCU_T1_Data = Temp_Rec2(Honeywell_R1);
			 if(Sensor_Fault_Times[7]>=100)//��ѹ�쳣��������100��˵��������ʧЧ
			 {
				 Sensor_Fault_Times[7]=100;//��ֹһֱ�Լ����
				 Fault_Level_Three|=0X0001;
				 fualt(30201);
				 if(Sensor_Fault_Times[10]>=100)//�����ʱ���ˮ�¶ȴ�����Ҳ�쳣�� �����ļ�������
				 {
					 Fault_Level_Four|=0X0001;
					 fualt(40201);
				 }
			 }
		   }
		   else
		   {
			   Sensor_Fault_Times[7]=0;
			   Fault_Level_Three&=0XFFFE;
//			   FCU_T1_Data = Temp_Rec2(FCU_T1_DY);//NTC10K�¶ȴ���������
//			   Honeywell_R1 = (uint16_t)(((uint32_t)(10000*FCU_T1_DY))/(5000-FCU_T1_DY));
			   FCU_T1_Data = Temp_Rec2(Honeywell_R1);
			   if(Sensor_Fault_Times[11]<5)//��ˮ�¶�Ҳ������������ļ�����
			   {
//				   Fault_Level_Four&=0XFFFE;
			   }
		   }
	  break;
	  case 7: //18���� �¶Ȳɼ�T3  PTC0  �����¶�
		  i=8;
			FCU_T3_DY=(uint16_t)((uint32_t)((maopao(8))*5000/4095));
			if((FCU_T3_DY>4900)||(FCU_T3_DY<20))//�����¶��쳣
			{
	//			Sensor_Fault_Times[8]++;
		//			FCU_T3_Data =Temp_Rec(FCU_T3_DY);//NTC10K�¶ȴ���������
	//			if(Sensor_Fault_Times[8]>=100)//��ѹ�쳣��������100��˵��������ʧЧ
	//			{
	//				Sensor_Fault_Times[8]=100;//��ֹһֱ�Լ����
	//	//				Fault_Level_Two|=0X0001;
	//	//				fualt(20201);
	//			}
			}
			else
			{
	//			Sensor_Fault_Times[8]=0;
	//			Fault_Level_Two&=0XFFFE;
				FCU_T3_Data =Temp_Rec(FCU_T3_DY);//NTC10K�¶ȴ���������
			}
	break;
	case 8: //�¶Ȳɼ�T2  PTC1  ASCO�����ŷ�
	     i=9;
	     FCU_T2_DY=(uint32_t)((uint32_t)((maopao(9))*5000/4095));
		  Honeywell_R2 = (uint32_t)(((uint32_t)(10000*FCU_T2_DY))/(5000-FCU_T2_DY));
		  if((FCU_T2_DY>4900)||(FCU_T2_DY<20))//�����¶��쳣
		  {
			 Sensor_Fault_Times[8]++;
			 FCU_T2_Data = Temp_Rec1(Honeywell_R2);
			 if(Sensor_Fault_Times[8]>=100)//��ѹ�쳣��������100��˵��������ʧЧ
			 {
				 Sensor_Fault_Times[8]=100;//��ֹһֱ�Լ����
				 if((FCU_T1_Data < 45)||(FCU_T4_Data < 45))
				 {
					 Fault_Level_Four|=0X0004;
					 fualt(40205);
				 }
				 else
				 {
					 Fault_Level_Two|=0X0001;
					 fualt(20203);//�����ŷ��¶ȴ�����
				 }
			 }
		  }
		  else
		  {
			  Sensor_Fault_Times[8]=0;
			  Fault_Level_Two&=0XFFFE;
 //			  FCU_T2_Data =Temp_Rec(FCU_T2_DY);//NTC10K�¶ȴ���������
 //			  Honeywell_R2 = (uint16_t)(((uint32_t)(10000*FCU_T2_DY))/(5000-FCU_T2_DY));
			  FCU_T2_Data = Temp_Rec1(Honeywell_R2);
		  }
	break;
	case 9: // �¶Ȳɼ�T4  PTC2  ��ˮ�¶�
			i=0;
			FCU_T4_DY=(uint16_t)((uint32_t)((maopao(10))*5000/4095));
			Honeywell_R4 = (uint16_t)(((uint32_t)(10000*FCU_T4_DY))/(5000-FCU_T4_DY));
			if((FCU_T4_DY>4900)||(FCU_T4_DY<20))//��ѹ̫�߻�̫��˵���¶ȴ�����û�н���
			{
			 Sensor_Fault_Times[10]++;
			 FCU_T4_Data = Temp_Rec2(Honeywell_R4);//NTC10K�¶ȴ���������
			 if(Sensor_Fault_Times[10]>=100)//��ѹ�쳣��������100��˵��������ʧЧ
			 {
				 Sensor_Fault_Times[10]=100;//��ֹһֱ�Լ����
				 Fault_Level_Three|=0X0002;
				 fualt(30202);
				 if(Sensor_Fault_Times[7]>=100)//�����ʱ���ˮ�¶ȴ�����Ҳ�쳣�� �����ļ�������
				 {
					 Fault_Level_Four|=0X0001;
					 fualt(40201);
				 }
			 }
			}
			else
			{
				Sensor_Fault_Times[10]=0;
				Fault_Level_Three&=0XFFFD;
				FCU_T4_Data = Temp_Rec2(Honeywell_R4);//NTC10K�¶ȴ���������
//				Honeywell_R4 = (uint16_t)(((uint32_t)(10000*FCU_T4_DY))/(5000-FCU_T4_DY));
//				FCU_T4_Data = Temp_Rec2(Honeywell_R4);//�¶�ת������
				if(Sensor_Fault_Times[7]<5)//��ˮ�¶�Ҳ������������ļ�����
				{
//				   Fault_Level_Four&=0XFFFE;
				}
			}
      break;
  }
}

/************************************************************/
/*                    ð������                         */
/************************************************************/
uint16_t maopao(uint8_t chanel)
{
  uint8_t i,j;
  uint16_t t ;
  uint16_t mpdata[7] ;     //ð�ݴ��������
  uint16_t average;
  for(i=0;i<=6;i++)    //�ȴ�7�����ֵ���������
  {
    mpdata[i] = AD_capture(chanel);
  }
   for(i=1;i<=6;i++)     //��7���� һ���Ƚ�6��
  {
      for(j=0;j<=6-i;j++)   //��ÿ����Ҫ���У�7-i���������Ƚϣ�����j��0��ʼ�����Դ�0��9-i���1��10-i  ����һ��
      {
           if(mpdata[j]<mpdata[j+1]) //���ǰ�����С�ں��������
          {
              t=mpdata[j+1];       //������������λ��

              mpdata[j+1]=mpdata[j];

              mpdata[j]=t;
          }
      }
  }
  average = ( mpdata[2]+mpdata[3]+mpdata[4] )/3 ;
  return average ;
}

/************************************************************/












