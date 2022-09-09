/*
 * ADC.c
 *
 *  Created on: 2018年8月21日
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


uint16_t Flowmeter=0;//空气流量
uint32_t FCU_Flowmeter=0;
uint16_t Honeywell_R1 = 0;
uint32_t Honeywell_R2 = 0;
uint16_t Honeywell_R3 = 0;
uint16_t Honeywell_R4 = 0;


//气罐总压力跟二级减压阀压力
uint16_t Tank_Pre=0,Tank_DY = 0,Tank_Mid_DY=0,Tank_Mid_Pre=0;
uint16_t Stack1_H2_Pre=0,Stack1_H2_DY=0;//FCU采集氢气压力2
uint16_t Stack1_InWater_Pre=0,Stack1_InWater_DY=0;//进水压力
uint16_t Stack1_RH=0,Stack1_RH_DY=0;//电堆内湿度
//接收EHI喷轨控制器的压力采集
uint16_t Stack1_H_Pre=1000,Stack1_Air_Pre=0;
//温度，T1代表进水温度，T4代表出水温度，T2代表预留，T3堆内温度
uint32_t FCU_T1_DY=0,FCU_T2_DY=0,FCU_T3_DY=0,FCU_T4_DY=0;
uint8_t FCU_T1_Data=0,FCU_T2_Data=0,FCU_T3_Data=0,FCU_T4_Data=0;
//电流环电压采集
uint16_t I_DY=0,Out_I=0;
//电源电压采集与5V传感器供电电压采集 暂时没有用到
uint8_t Sensor_Data,Battery_Data;
//传感器连续出问题的次数记录
uint8_t Sensor_Fault_Times[11]={0,0,0,0,0,0,0,0,0,0,0};

//电堆总功率
uint32_t Send_Stack_Total_P=0,Stack_Total_P=0,Stack_Total_P_LS=0;//电堆总功率单位W/H
uint32_t Send_Out_Total_P=0,Out_Total_P=0,Out_Total_P_LS=0;//净输出总功率单位W/H
//氢气浓度传感器参数
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
/*                        起动AD转换                         */
/*************************************************************/
uint16_t AD_capture(uint8_t chanel)
{
	 uint16_t AD_data;
	 switch(chanel)
	 {
	  case 0:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig0);//对应 PTC14 47引脚  进水压力采集
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 1:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig1);//对应PTB3 48引脚  FCU氢气压力采集
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 2:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig2);//对应 PTB2 49引脚   中压压力传感器采集
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 3:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig3);//对应 PTB1 50引脚 气罐压力采集
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 4:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig4);//对应 PTB0 19引脚  氢气浓度1
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 5:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig5);//对应 PTA7  20引脚  氢气浓度2
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	  case 6:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig6);//对应 PTA6   21引脚  电流环信号采集
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	 case 7:
  		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig7);//对应 PTC15 T1 46引脚 进水温度
  		/* Wait for the conversion to be done */
  		ADC_DRV_WaitConvDone(ADC_INSTANCE);
  		/* Store the channel result into a local variable */
  		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
		break;
	 case 8:
	   		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig8);//对应 PTC0  18引脚 空气温度
	   		/* Wait for the conversion to be done */
	   		ADC_DRV_WaitConvDone(ADC_INSTANCE);
	   		/* Store the channel result into a local variable */
	   		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
	 		break;
	 case 9:
	   		ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig9);//对应 PTC1   17引脚    温度采集T3预留
	   		/* Wait for the conversion to be done */
	   		ADC_DRV_WaitConvDone(ADC_INSTANCE);
	   		/* Store the channel result into a local variable */
	   		ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
	 		break;
	 case 10:
			ADC_DRV_ConfigChan(ADC_INSTANCE, 0U, &adConv1_ChnConfig10);//对应 PTC2   45引脚    出水温度
			/* Wait for the conversion to be done */
			ADC_DRV_WaitConvDone(ADC_INSTANCE);
			/* Store the channel result into a local variable */
			ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, &AD_data);
			break;
	 }
    return AD_data;
}
/************************************************************/
/*                          赋值                            */
/************************************************************/
void Get_ADC(void)
{
  static uint8_t i=0;
  switch(i)
  {
     case 0: //47脚 进水压力
         i=1;
         Stack1_InWater_DY=(uint16_t)((uint32_t)((maopao(0)))*5000/4095);
         if((Stack1_InWater_DY>4800)||(Stack1_InWater_DY<200))//电压异常
         {
        	 Sensor_Fault_Times[0]++;
        	 if(Sensor_Fault_Times[0]>=100)//电压异常连续出现100次说明传感器失效
        	 {
        		 Sensor_Fault_Times[0]=100;//防止一直自加溢出
        		 Fault_Level_Four|=0X0002;
        		 fualt(40202);
        		 Stack1_InWater_Pre=1000;
        	 }
         }
         else//电压正常值，正常计算
         {
        	 Sensor_Fault_Times[0]=0;//连续自加置0
//        	 Fault_Level_Four&=0XFFFD;//清除产生的故障位
        	 if(Stack1_InWater_DY<500)Stack1_InWater_DY=500;//仍然需要检查一下是否小于0.5V
//        	 Stack1_InWater_Pre = (uint16_t)((uint32_t)(Stack1_InWater_DY-500)/8*10)+1000; //电压值计算   旧传感器公式
//        	 Stack1_InWater_Pre = (uint16_t)((uint32_t)(Stack1_InWater_DY-500)/2)+1000; //电压值计算   康宇传感器公式（应该除以20，但精度0.1就除以2）
        	 Stack1_InWater_Pre = (uint16_t)((uint32_t)(Stack1_InWater_DY-500)/16*10)+1000; //电压值计算   GSEM传感器公式
         }

     break;
     case 1: //48脚 FCU氢气压力  PTB3
		 i=2;
		 Stack1_H2_DY=(uint16_t)((uint32_t)((maopao(1))*5000/4095));
		 if((Stack1_H2_DY>4800)||(Stack1_H2_DY<200))
		 {
        	 Sensor_Fault_Times[1]++;
        	 if(Sensor_Fault_Times[1]>=100)//电压异常连续出现100次说明传感器失效
        	 {
        		 Sensor_Fault_Times[1]=100;//防止一直自加溢出
        		 Fault_Level_Two|=0X0001;
        		 fualt(20202);
        		 Stack1_H2_Pre=1000;
        	 }
		 }
		 else
		 {
        	 Sensor_Fault_Times[1]=0;//连续自加置0
        	 Fault_Level_Two&=0XFFFE;//清除产生的故障位
			 Stack1_H2_Pre = (uint16_t)((uint32_t)((Stack1_H2_DY)*20-5700)*10/211);//更换成广东和宇公司传感器20200216
//        	 Stack1_H2_Pre = (uint16_t)((uint32_t)(Stack1_H2_DY)*19/23); //
		 }
	 break;
     case 2: // 49引脚  PTB2  修改为堆内湿度检测  日期：20210817
         i=3;
         Stack1_RH_DY = (uint16_t)((uint32_t)(maopao(2))*5000/4095);
//			Honeywell_R4 = (uint16_t)(((uint32_t)(10000*FCU_T4_DY))/(5000-FCU_T4_DY));
		if((Stack1_RH_DY>4800)||(Stack1_RH_DY<200))//电压异常
		 {
//			 Sensor_Fault_Times[0]++;
//			 if(Sensor_Fault_Times[0]>=100)//电压异常连续出现100次说明传感器失效
//			 {
//				 Sensor_Fault_Times[0]=100;//防止一直自加溢出
//				 Fault_Level_Four|=0X0002;
//				 fualt(40202);
//				 Stack1_RH=1000;
//			 }
		 }
		 else//电压正常值，正常计算
		 {
			 Sensor_Fault_Times[0]=0;//连续自加置0
//        	 Fault_Level_Four&=0XFFFD;//清除产生的故障位
			 if(Stack1_RH_DY<500)Stack1_RH_DY=500;//仍然需要检查一下是否小于0.5V
			 Stack1_RH = (uint16_t)(Stack1_RH_DY-1032)/26.23; //电压值计算  温湿度传感器 湿度计算公式
		 }
     break;
     case 3: //50引脚 空气流量采集  PTB1   //------空气流量.日期：20211014
         i=4;
         Flowmeter=(uint16_t)((uint32_t)((maopao(3))*5000/4095));
         FCU_Flowmeter = (Flowmeter_Rec(Flowmeter)*1289/100);//空气流量转换函数
//         Tank_DY = (uint16_t)((uint32_t)(maopao(3))*5000/4095);
//         if((Tank_DY>4800)||(Tank_DY<200))//异常状态
//         {
//        	 Sensor_Fault_Times[3]++;
//        	 if(Sensor_Fault_Times[3]>=10)//电压异常连续出现10次说明传感器失效
//        	 {
//        		 Sensor_Fault_Times[3]=10;//防止一直自加溢出
//        		 Fault_Level_Four|=0X0004;
//        		 fualt(40203);
//        		 Tank_Pre=1000;
//        	 }
//         }
//         else
//         {
//        	 Sensor_Fault_Times[3]=0;//防止一直自加溢出
//			 if(Tank_DY<500)Tank_DY=500;//防止到负压
//			 if((REL_State1&0X40)==0X40)//瓶口阀打开，用现在采集的数值
//		     Tank_Pre = (Tank_DY-500)/8+1000;//单位是bar
//		     else Tank_Pre =CAL_Data[96]*256+CAL_Data[97];//如果气罐没打开，气罐压力发送上次关机存储的数值
//			  if(Tank_Pre>1020)//气罐压力大于2兆帕
//			  {
//				  Fault_Level_Four&=0XFFFB;
//				  Fault_Level_Three&=0XEFFF;//清除3，4级故障
//			  }
//
//			  else if(Tank_Pre>1010)//如果气罐压力小于1MPA限功率
//			  {
//				  Fault_Level_Three|=0X1000;//限功率
//			  }
//			  else if(Tank_Pre>1005)//如果气罐压力小于0.5MPA关机
//			  {
//				  Fault_Level_Four|=0X0004;//关机
//			  }
//         }
	 break;
     case 4: //19引脚  氢气浓度1         //临时占用引脚------空气流量.日期：20200220
         i=5;
//		 Flowmeter=(uint16_t)((uint32_t)((maopao(4))*5000/4095));
//		 FCU_Flowmeter = (Flowmeter_Rec(Flowmeter)*1289/100);//空气流量转换函数
//		 H1_PPM_DY = ((uint32_t)((maopao(4))*5000/4095));//氢气传感器1
//		 if(H1_PPM_DY<500)H1_PPM_DY=500;
//		 H1_PPM = (H1_PPM_DY-500)*25/4;//氢气传感器1浓度计算
	 break;
     case 5: //21引脚  电堆吹扫口 氢气浓度
		 i =6;
		 H2_PPM_DY = ((uint32_t)((maopao(6))*5000/4095));//氢气传感器2
		 H2_PPM = (H2_PPM_DY-500)*10;//氢气浓度=（输出电压-0.5V）*10000  鸿擎第二代
		 if((H2_PPM_DY>4800)||(H2_PPM_DY<200))
		 {
			 Sensor_Fault_Times[5]++;
			 if(Sensor_Fault_Times[5]>=100)//电压异常连续出现100次说明传感器失效10s
			 {
				 Sensor_Fault_Times[5]=100;//防止一直自加溢出
				 Fault_Level_Two|=0X0004;
				 fualt(21102);//氢气浓度传感器异常
				 H2_PPM=505;//505目的出现问题的时候显示不是0，方便判断
			 }
		 }
		 else
		 {
			 if(Flag_6S>=60)
			 {
				 if(H2_PPM_DY<500)//限制最小值
				 {
					 H2_PPM_DY=500;
				 }
				 else if(H2_PPM_DY>4500)//限制最大值
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
				 if(H2_PPM > 22000)//系统停机，并关闭瓶口阀
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 1000)
					 {
						 fualt(41102);
//						 Fault_Level_Four|=0X0004;//关机
					 }
				 }
				 else if(H2_PPM > 15000)//系统停机，并上发给整车给出提示
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 1000)
					 {
						 fualt(41101);
//						 Fault_Level_Four|=0X0004;//关机
					 }
				 }
				 else if(H2_PPM > 6000)//系统降功率处理，并上发给整车给出提示
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 100)
					 {
						 fualt(31101);
//						 Fault_Level_Three|=0X0004;//
					 }
				 }
				 else if(H2_PPM > 3000)//系统不做处理，上发给整车给出提示
				 {
					 H2_PPM_Time++;
					 if(H2_PPM_Time >= 100)
					 {
						 fualt(21101);
						 Fault_Level_Two|=0X0004;//上发给整车给出提示
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
     case 6: //T1  46脚 进水温度
          i=7;
		   FCU_T1_DY=(uint16_t)((uint32_t)((maopao(7))*5000/4095));
		   Honeywell_R1 = (uint16_t)(((uint32_t)(10000*FCU_T1_DY))/(5000-FCU_T1_DY));
		   if((FCU_T1_DY>4900)||(FCU_T1_DY<20))//电压太高或太低说明温度传感器没有接入
		   {
			 Sensor_Fault_Times[7]++;
//			 FCU_T1_Data = Temp_Rec2(FCU_T1_DY);//NTC10K温度传感器计算
			 FCU_T1_Data = Temp_Rec2(Honeywell_R1);
			 if(Sensor_Fault_Times[7]>=100)//电压异常连续出现100次说明传感器失效
			 {
				 Sensor_Fault_Times[7]=100;//防止一直自加溢出
				 Fault_Level_Three|=0X0001;
				 fualt(30201);
				 if(Sensor_Fault_Times[10]>=100)//如果这时候出水温度传感器也异常了 就是四级故障了
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
//			   FCU_T1_Data = Temp_Rec2(FCU_T1_DY);//NTC10K温度传感器计算
//			   Honeywell_R1 = (uint16_t)(((uint32_t)(10000*FCU_T1_DY))/(5000-FCU_T1_DY));
			   FCU_T1_Data = Temp_Rec2(Honeywell_R1);
			   if(Sensor_Fault_Times[11]<5)//出水温度也正常，清除掉四级故障
			   {
//				   Fault_Level_Four&=0XFFFE;
			   }
		   }
	  break;
	  case 7: //18引脚 温度采集T3  PTC0  堆内温度
		  i=8;
			FCU_T3_DY=(uint16_t)((uint32_t)((maopao(8))*5000/4095));
			if((FCU_T3_DY>4900)||(FCU_T3_DY<20))//空气温度异常
			{
	//			Sensor_Fault_Times[8]++;
		//			FCU_T3_Data =Temp_Rec(FCU_T3_DY);//NTC10K温度传感器计算
	//			if(Sensor_Fault_Times[8]>=100)//电压异常连续出现100次说明传感器失效
	//			{
	//				Sensor_Fault_Times[8]=100;//防止一直自加溢出
	//	//				Fault_Level_Two|=0X0001;
	//	//				fualt(20201);
	//			}
			}
			else
			{
	//			Sensor_Fault_Times[8]=0;
	//			Fault_Level_Two&=0XFFFE;
				FCU_T3_Data =Temp_Rec(FCU_T3_DY);//NTC10K温度传感器计算
			}
	break;
	case 8: //温度采集T2  PTC1  ASCO排氢电磁阀
	     i=9;
	     FCU_T2_DY=(uint32_t)((uint32_t)((maopao(9))*5000/4095));
		  Honeywell_R2 = (uint32_t)(((uint32_t)(10000*FCU_T2_DY))/(5000-FCU_T2_DY));
		  if((FCU_T2_DY>4900)||(FCU_T2_DY<20))//空气温度异常
		  {
			 Sensor_Fault_Times[8]++;
			 FCU_T2_Data = Temp_Rec1(Honeywell_R2);
			 if(Sensor_Fault_Times[8]>=100)//电压异常连续出现100次说明传感器失效
			 {
				 Sensor_Fault_Times[8]=100;//防止一直自加溢出
				 if((FCU_T1_Data < 45)||(FCU_T4_Data < 45))
				 {
					 Fault_Level_Four|=0X0004;
					 fualt(40205);
				 }
				 else
				 {
					 Fault_Level_Two|=0X0001;
					 fualt(20203);//排氢电磁阀温度传感器
				 }
			 }
		  }
		  else
		  {
			  Sensor_Fault_Times[8]=0;
			  Fault_Level_Two&=0XFFFE;
 //			  FCU_T2_Data =Temp_Rec(FCU_T2_DY);//NTC10K温度传感器计算
 //			  Honeywell_R2 = (uint16_t)(((uint32_t)(10000*FCU_T2_DY))/(5000-FCU_T2_DY));
			  FCU_T2_Data = Temp_Rec1(Honeywell_R2);
		  }
	break;
	case 9: // 温度采集T4  PTC2  出水温度
			i=0;
			FCU_T4_DY=(uint16_t)((uint32_t)((maopao(10))*5000/4095));
			Honeywell_R4 = (uint16_t)(((uint32_t)(10000*FCU_T4_DY))/(5000-FCU_T4_DY));
			if((FCU_T4_DY>4900)||(FCU_T4_DY<20))//电压太高或太低说明温度传感器没有接入
			{
			 Sensor_Fault_Times[10]++;
			 FCU_T4_Data = Temp_Rec2(Honeywell_R4);//NTC10K温度传感器计算
			 if(Sensor_Fault_Times[10]>=100)//电压异常连续出现100次说明传感器失效
			 {
				 Sensor_Fault_Times[10]=100;//防止一直自加溢出
				 Fault_Level_Three|=0X0002;
				 fualt(30202);
				 if(Sensor_Fault_Times[7]>=100)//如果这时候进水温度传感器也异常了 就是四级故障了
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
				FCU_T4_Data = Temp_Rec2(Honeywell_R4);//NTC10K温度传感器计算
//				Honeywell_R4 = (uint16_t)(((uint32_t)(10000*FCU_T4_DY))/(5000-FCU_T4_DY));
//				FCU_T4_Data = Temp_Rec2(Honeywell_R4);//温度转换函数
				if(Sensor_Fault_Times[7]<5)//出水温度也正常，清除掉四级故障
				{
//				   Fault_Level_Four&=0XFFFE;
				}
			}
      break;
  }
}

/************************************************************/
/*                    冒泡排序                         */
/************************************************************/
uint16_t maopao(uint8_t chanel)
{
  uint8_t i,j;
  uint16_t t ;
  uint16_t mpdata[7] ;     //冒泡处理的数据
  uint16_t average;
  for(i=0;i<=6;i++)    //先存7个数字到数组里面
  {
    mpdata[i] = AD_capture(chanel);
  }
   for(i=1;i<=6;i++)     //有7个数 一共比较6轮
  {
      for(j=0;j<=6-i;j++)   //在每轮中要进行（7-i）次两两比较，但是j从0开始，所以从0到9-i与从1到10-i  次数一样
      {
           if(mpdata[j]<mpdata[j+1]) //如果前面的数小于后面的数，
          {
              t=mpdata[j+1];       //交换两个数的位置

              mpdata[j+1]=mpdata[j];

              mpdata[j]=t;
          }
      }
  }
  average = ( mpdata[2]+mpdata[3]+mpdata[4] )/3 ;
  return average ;
}

/************************************************************/












