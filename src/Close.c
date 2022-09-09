/*
 * Close.c
 *
 *  Created on: 2018年9月10日
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
uint8_t closed_flag=0;//关机动作完成标志位
uint8_t DC_Start_Flag=0;
uint8_t Normal_Close_Flag=0;//延时关机标志位
uint16_t Normal_Close_Time=0;//关机计时
uint8_t LowTemp_FAN_Flag=0;//低温风扇运行标志位
/*************************************************************/
/*                        下电流程                                                */
/*************************************************************/
void Normal_Shutdown(void)
{
	static uint16_t DC_Time=0;//延时下电计数
	static uint8_t Open_Flag=1;//延时关机标志位开启
	static uint8_t Storage_Tank_Flag=0;//存储气罐压力标志位
	//  uint8_t Storage_Tank_valve[2]={0,0};//气罐压力存储接数组
	uint8_t Save_RUN_Count_Data[2]={0,0}; //存储运行次数
	static uint8_t H2_In_Time=0;//氢气入口阀关闭延时


	if(T_100ms==1)
	{
		T_100ms=0;

		Normal_Close_Time++;
		if(LowTemp_FAN_Flag == 1)//非低温时候下电时直接降温
		{
			if(((FCU_T1_Data>85)||(FCU_T4_Data>90))&&((Stack_State&0X10)==0X10))//两个电堆水温都需要大于60
			{
			  JWQ_Enable = 0X00;//节温器停机
			  REL_Auto[12]=1;//风扇置为手动控制
			  Fan_Speed_Data = 60;//转速设为60%
			}
			else
			{
			  REL_Auto[12]=0;//风扇置为自动控制
			  Fan_Speed_Data=0;//风扇转速设置为0
			}
		}
		if(Storage_Tank_Flag==0)//需要存储气罐压力到EEPROM当中
		{
			Storage_Tank_Flag=1;
			S_RUN_Count_Data++;
			Save_RUN_Count_Data[0]=S_RUN_Count_Data/256;//高字节
			Save_RUN_Count_Data[1]=S_RUN_Count_Data%256;//低字节
			EX_EEPROM_C256_write(28,Save_RUN_Count_Data,2);
		}
		if(DC_Start_Flag==0)//为了在停机过程只进一次并赋值手动开机
		{
			DC_Start_Flag =1;
			R_DC_MAN_Data[0] = 0X1B;//DC设置为手动控制状态下  开机
		}

		if(DC_Target_I>=30)
		{
			DC_Target_I-=1;//目标电流每次减少2A
		}
		else
		{
			if(closed_flag <= 1)//添加此条件原因：防止主动放完电后DC已关机，氢气入口关闭后closed_flag标志位置2导致DC一直处于主动放电无法切换到关机模式中
			{
				if((Normal_Close_Time >= 10)&&(LowTemp_Close_Flag == 1))
				{
					DC_Target_I=20;

					if(Normal_Close_Time >= 3580)
					{
						LowTempSpeedCompensate = 0;//低温下电转速补偿清零
					}
					if(Normal_Close_Time >= 3600)
					{
						LowTemp_FAN_Flag = 1;
						S_SJKYJ_State=0X05;//空压机关闭 不使能
						REL_KG(H_Out,0X02);//打开氢气出口
						Out_H_Flag =1;//自动排氢标志位置1
						DC_Target_I=0;

						if(Normal_Close_Time >= 3660)
						{
							REL_KG(H_Out,0X01);//关闭氢气出口1
							REL_KG(H_GYH,0X01);//关闭氢气出口2
							Out_H_Flag=0;//打开自动排气标志位
							S_U_Flag|=0X01;
							H_PUMP_Enable = 0X00;//氢气循环泵不使能

							R_DC_MAN_Data[0] = 0XFC;//DC设置为 主动放电
							DC_Target_I=5;
						}
					}
				}
				else//非低温环境直接放电
				{
					R_DC_MAN_Data[0] = 0XFC;//DC设置为 主动放电
					DC_Target_I=3;
					LowTemp_FAN_Flag = 1;
				}
			}
		}

		if((S_U_Flag==0)&&(DC_Target_I<=10)&&(LowTemp_Close_Flag == 0))//吹扫未完成标志位并且DC目标电流小于等于10A的时候开始排空
		{
			H_Emp();
		}

		//DC输入电压小于30V或者  进过一次之后就可以一直关机了，防止由主动放电转为30V以上
		if(((DC_IN_V<60)||(In_DC_Flag==1))&&((S_U_Flag&0X01)==0X01)&&(closed_flag<=1))
		{
			In_DC_Flag=1;
			S_DC_State = 0X05;//关机
			R_DC_MAN_Data[0] = 0X05;//DC设置为自动控制关机
			DC_Target_I=0;
			PTC_test_Time++;
			closed_flag=1;
//			REL_KG(H_IN,0X01);//关闭氢气入口
		}
		else if(((DC_IN_V>400)&&(In_DC_Flag==0)&&(S_U_Flag==1))||((DC_Fault&0XC0)!=0))//DC输入电压大于40V 或者DC有故障的时候主动转为主动放电
		{
			R_DC_MAN_Data[0] = 0XFC;//DC设置为手动控制 主动放电
			DC_Target_I=3;
		}
		else
		{

		}
		if(((S_U_Flag&0X01)==0X01)&&(LowTemp_Close_Flag == 0))//吹扫完成
		{
			S_SJKYJ_State=0X05;//空压机关闭 不使能
			H_PUMP_Enable = 0X00;//氢气循环泵不使能
			REL_KG(Tank_valve,0X01);//关闭瓶口阀  等中压传感器低于0.2MPA之后再关氢气入口
		}
		if((closed_flag==1)||((DC_Fault&0XC0)!=0))
		{
			H2_In_Time++;
			if(H2_In_Time>=200)
			{
				REL_KG(H_IN,0X01);//关闭氢气入口
				closed_flag=2;
				H2_In_Time=0;
			}
		}
		if((FCU_T1_Data<=85)&&(FCU_T4_Data<=90)&&(closed_flag==2))//两个电堆水温都需要小于40
		{
			REL_Auto[12]=0;//风扇置为自动控制
			Fan_Speed_Data=0;//风扇转速设置为0
			if(DC_IN_V<40)//这里使用DC采集的总电压
			{
				if((S_U_Flag&0X01)==0X01)//吹扫完成标志位
				{
					Storage_Tank_Flag=0;//存储气罐压力标志位
					Normal_Close_Time=0;
					H2_In_Time =0;
					DC_Time=0;
					LowTemp_Close_Flag = 0;//低温关机标志位清零
					LowTemp_FanCon_Flag = 0;//低温风扇切换控制标志位
					LowTempSpeedCompensate = 0;//低温下电转速补偿
					LowTempAngleCompensate= 0;//背压阀低温下电角度补偿
					Emergency_Shutdown();
				}
			}
		}
		if(((Normal_Close_Time>=6000)&&(Normal_Close_Time<=6050))||(Normal_Close_Time>=140&&VCU_Ready_State==0x05)) //大于14秒 并且 VCU发送紧急停机指令
		{
			DC_Time=0;
			Emergency_Shutdown();
		}
	}
}
/*************************************************************/
/*                        待机状态                                                 */
/*************************************************************/
void Standby(void)
{
	if(T_100ms==1)
	{

	}
}
/*************************************************************/
/*                        紧急关机函数                                                */
/*************************************************************/
void Emergency_Shutdown(void)
{
	static uint16_t Time=0;


	S_SJKYJ_State=0X05;//空压机关闭 不使能
	H_PUMP_Enable = 0X00;//氢气循环泵不使能
	PUMP_Enable = 0X00;//高压水泵不使能
	REL_KG(Stack_Pump,0X01);//关闭辅助系统水泵
//	REL_KG(FZ_Fan,0X01);//关闭辅助系统风扇
	REL_KG(H_Out,0X01);//关闭氢气出口1
	REL_KG(H_GYH,0X01);//关闭氢气出口2
	JWQ_Enable = 0X00;//节温器停机
	PTC_Enable = 0X00;//PTC关闭
	REL_KG(H_IN,0X01);//关闭氢气入口
	REL_Auto[12]=0;//风扇置为自动控制
	Fan_Speed_Data=0;//风扇转速设置为0
	Flag_6S=0;
	Flag_12S=0;
	StartUpCurrent_Flag = 0;
	LowTemp_Close_Flag = 0;//低温关机标志位清零
	LowTemp_FanCon_Flag = 0;//低温风扇切换控制标志位
	LowTempSpeedCompensate = 0;//低温下电转速补偿
	LowTempAngleCompensate= 0;//背压阀低温下电角度补偿

	Time++;
	if(Time>=20)//2秒钟以后关闭部件电源
	{
		DC_Target_Old=0;
		S_U_Flag=0;//自检的置零，因为刚才排氢将它置1了
		Out_H_Flag=1;//置为手动排氢
		Stack_State=0;//电堆状态清除
		In_DC_Flag=0;//防止老是由关机和主动放电切换
		DC_Start_Flag=0;
		closed_flag=0;
		Normal_Close_Time=0;//关机计时清零
		Normal_Close_Flag=0;
		Time = 0;
//		S_SJKYJ_State=0XFE;//空压机关闭后进行故障复位（目的：如果出现二级故障时进行清除）

		if(ON_Flag==0)//ON档关闭
		{
			HProtectFlag_6S = 0;//放在此位置清零目的：关机以后钥匙不关闭时氢气压力保护函数还会起到作用
			HProtectFlag_10S = 0;//
//			REL_KG(BMS_ON,0X01);//关闭BMS使能信号
			REL_KG(Part_Power,0X01);//关闭部件电源
			PINS_DRV_ClearPins(GPIO_PTE,1<<E_SYS_Power);//关闭自唤醒
		}
	}
}










