/*
 * Start_Up.c
 *
 *  Created on: 2018年9月8日
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

uint8_t S_U_Flag=0;//启动步骤标志位 下电或急停之后才清零
uint8_t H_Emp_Time=0;//氢气道排空时间
uint8_t LowTempStart_Flag=0;//低温启动标志位
uint8_t LowTemp_Close_Flag=0;//低温关机标志位



/*************************************************************/
/*                    氢气道空气排空函数                     */
/*************************************************************/
void H_Emp(void)
{
  REL_KG(H_Out,0X02);//打开氢气出口
  Out_H_Flag =1;//自动排氢标志位置1
  H_Emp_Time++;
  if(H_Emp_Time>=30)//排空3S
  {
	  H_Emp_Time = 0;
	  REL_KG(H_Out,0X01);//关闭氢气出口1
	  REL_KG(H_GYH,0X01);//关闭氢气出口2
	  Out_H_Flag=0;//打开自动排气标志位
	  S_U_Flag|=0X01;
  }
}
/*************************************************************/
/*                        启动函数                           */
/*************************************************************/
void Start_Up(void)
{
	static uint8_t Time_Start=0;


	if(T_100ms==1)
	{
		T_100ms=0;
		if((S_U_Flag&0X03)==0X03)//启动成功
		{
			Time_Start=0;//把这些标志位清零，下次再用
			Out_H_Flag=0;//打开自动排气标志位
			Stack_State|=0X08; //可以运行
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
				REL_KG(Solenoid_Heat,0X01);//关闭排氢电磁阀加热
			}
			if((FCU_T1_Data > 50)&&(FCU_T2_Data > 50))
			{
				LowTempStart_Flag = 0;
			}

			if(LowTempStart_Flag == 0)
			{
				S_SJKYJ_State=0X0B;//空压机使能
				SJKYJ_Target_Speed = 31000;//初始转速31000
				H_PUMP_Enable = 0X01;//氢气循环泵使能
				PUMP_Enable = 0X01;//高压水泵使能
				Pump_Target_Speed = 2000;//高压水泵2000转
				REL_KG(Stack_Pump,0X02);//打开辅助系统水泵
				REL_KG(H_IN,0X02);//打开氢气入口
//				PTC_Enable = 0X00;//PTC关闭
//				PTC_Target_Temp = 40;//0℃，有40的偏移量
//				PTC_Target_PWM = 0;//PWM给固定值0
				Time_Start++;
				if((S_U_Flag&0X01)!=0X01)//氢气道排空
				{
					S_U_Flag|=0X01;
				}
				if(Time_Start>=200)//给空压机跟排空，一共20秒钟时间
				{
					S_U_Flag|=0X02;
				}
			}
			else
			{
				PUMP_Enable = 0X01;//高压水泵使能
				Pump_Target_Speed = 2000;//高压水泵2000转
				REL_KG(Stack_Pump,0X02);//打开辅助系统水泵
				PTC_Enable = 0X03;//PTC启动，并进行温度控制
				PTC_Target_Temp = 120;//80℃，有40的偏移量
				PTC_Target_PWM = 100;//PWM给固定值100
				if(FCU_T2_Data <= 55)
				{
					REL_KG(Solenoid_Heat,0X02);//打开排氢电磁阀加热
				}
			}
		}
	}
}
