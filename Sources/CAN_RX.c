/*
 * CAN_RX.c
 *
 *  Created on: 2018年8月27日
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

uint8_t R_REL_Data[8]={0,0,0,0,0,0,0,0} ;//存放继电器命令的数据
uint8_t R_GBD_Data[8]={0,0,0,0,0,0,0,0} ;//存放故障标定的数据
uint8_t R_CBD_Data[8]={0,0,0,0,0,0,0,0} ;//存放参数标定的数据
uint8_t R_RW_Data[8]={0,0,0,0,0,0,0,0} ;//存放握手的数据
uint8_t R_DC_MAN_Data[8]={0,0,0,0,0,0,0,0} ;//存放升压DC手动控制的数据
uint8_t R_JYFBD_Data[8]={0,0,0,0,0,0,0,0} ;//存放减压阀标定的数据
uint8_t R_KQBYF_QXHB_PUMP_Data[8]={0,0,0,0,0,0,0,0} ;//接收空气背压阀手动控制数据


uint8_t R_SJKYJ_Data[8]={0,0,0,0,0,0,0,0};//接收空压机手动控制数据
uint8_t R_JWQ_PTC_6KWDC_Data[8]={0,0,0,0,0,0,0,0};//接收节温器手动控制数据
uint8_t ZD_RW=0;
uint8_t Small_DC_State=0;//接收降压DC状态

//接收水泵的数据
uint16_t Pump_Speed=0,Pump_Voltage=0;//水泵转速
uint8_t Pump_State1=0,Pump_State2=0,Pump_Current=0;//水泵状态1，状态2
uint8_t ZD_Pump=1;//查询水泵报文是否丢帧标志位
int16_t Pump_Speed_Dif_Val=0;//目标转速与实际转速差值
//接收氢气循环泵的数据
uint16_t H_Pump_Speed=0;//氢气循环泵转速
uint8_t H_Pump_State1=0,H_Pump_Power=0;//氢气循环泵状态1
uint8_t ZD_H_Pump=1;//查询氢气循环泵报文是否丢帧标志位
int16_t H_Pump_Speed_Dif_Val=0;//氢气循环目标转速与实际转速差值
int16_t H_Pump_Voltage = 0;//输入电压
//喷轨减压阀
uint8_t JYF_State=0;
uint8_t ZD_JYF=1;//查询减压阀报文是否丢帧标志位
//空气背压阀数据
uint8_t ZD_KQBYF=1;//查询背压阀报文是否丢帧标志位
int8_t KQBYF_Dif_Angle=0;//背压阀角度误差
uint8_t KQBYF_State1=0;//背压阀状态
//势加透波空压报文1数据
uint8_t ZD_SJKYJ1=1;//查询空压机报文1是否丢帧标志位
uint8_t SJKYJ_State=0;//空压机开启还是关闭状态
uint32_t SJKYJ_Speed=0;//空压机转速
int16_t SJKYJ_Speed_Dif_Val=0;//实际转速与目标转速差值
uint8_t SJKYJ_ID1_State=0;//报文1的状态 0代表正常 1代表异常
uint16_t SJKYJ_Power=0;//空压机功率
uint16_t SJKYJ_Bus_V=0;//空压机母线电压
uint8_t SJKYJ_Bus_I=0;//空压机母线电流
//势加透波空压报文2数据
uint8_t ZD_SJKYJ2=1;//查询空压机报文2是否丢帧标志位
uint8_t SJKYJ_Temp=0;//空压机控制器温度
uint8_t SJKYJ_MotorTemp=0;//空压机电机温度
uint8_t SJKYJ_Fualt_Class = 0;//空压机故障等级
uint16_t SJKYJ_Fualt_Code3 = 0;//空压机2级故障（不可恢复故障）
uint16_t SJKYJ_Fualt_Code2 = 0;//空压机2级故障（可恢复故障）
uint16_t SJKYJ_Fualt_Code1 = 0;//空压机1级故障（告警）
//升压DCDC报文1数据
uint8_t ZD_DC1=1;//DCDC的报文1丢帧标志位
uint16_t DC_IN_V = 0,DC_IN_I = 0;//输入电压电流  输入电压单位1V  输入电流单位1A
uint32_t Stack_P=0;//电堆总功率 单位W
uint8_t DC_State = 0,DC_Temp=0,DC_Fault=0,DC_Fault_code=0;//升压 DC状态，DC温度,DC故障等级,DC故障码
//升压DCDC报文2数据
uint8_t ZD_DC2=1;//DCDC的报文2丢帧标志位
uint16_t DC_OUT_I = 0,DC_OUT_V = 0;//输出电压电流   输出电压单位V  输出电流单位0.1A
uint8_t DC_Rec_State=0;//DC接受工作状态
uint16_t DC_Rec_I=0;//DC接受工作电流
uint8_t DC_ID2_State=0;//DC报文2的状态 0代表正常 1代表异常
//CVM1报文数据
uint8_t ZD_CVM11=1,ZD_CVM12=1,ZD_CVM13=1;
uint32_t Stack11_Average_Voltage =0,Stack11_L_Voltage=0,Stack1_Total_Voltage=0;
uint16_t Stack12_NumberMin = 0,Stack12_VoltageMin = 0;
uint16_t Stack12_NumberMax = 0,Stack12_VoltageMax = 0;
//CVM6报文数据
uint8_t ZD_CVM21=1,ZD_CVM22=1,ZD_CVM63=1;
uint32_t Stack21_Average_Voltage =0,Stack61_L_Voltage=0,Stack2_Total_Voltage=0;
uint16_t Stack22_NumberMin = 0,Stack22_VoltageMin = 0;
uint16_t Stack22_NumberMax = 0,Stack22_VoltageMax = 0;
//降压DCDC1报文数据
uint16_t DCDC_Out_voltage = 0;
uint16_t DCDC_In_voltage = 0;
uint16_t DCDC_Out_current = 0;
uint8_t DCDC_temp = 0;
uint8_t DCDC_work_state = 0;// 降压DC状态
uint16_t DCDC_fault_code = 0;
uint16_t DCDC_fault_level = 0;//降压DC故障等级

//三通阀报文数据
uint16_t Valve_State = 0;//状态
uint16_t Valve_fault_code = 0;//故障码
uint16_t Valve_Open = 0;//阀开度

// PTC
uint16_t PTC_In_voltage = 0;
uint16_t PTC_In_current = 0;
uint16_t PTC_Fault_Code = 0;

uint32_t Stack_Total_Voltage=0;//电堆总电压 精度1mV
uint16_t Stack_High_Voltage=0;//单片最大电压 精度1mV
uint16_t Stack_High_Number=0;//单片最大电压编号
uint16_t Stack_Low_Voltage=0;//单片最小电压 精度1mV
uint16_t Stack_Low_Number=0;//单片最小电压编号
uint16_t Stack1_Max_V_Dif=0;//电堆1单片最大压差 精度1MV
uint16_t Stack2_Max_V_Dif=0;//电堆2单片最大压差 精度1MV
uint16_t Stack_Average_Voltage=0;//单片平均电压 精度1mV
uint8_t CVM1_Flag = 0,CVM2_Flag = 0;//6块采集器报文标志位

//故障查询6秒延时标志位
uint8_t Flag_6S=0;
uint8_t Flag_12S=0;
uint8_t HProtectFlag_6S=0;

uint16_t Test_11=0,Test_12=0;

//电子水泵报文查询接收函数
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
			Pump_Speed = recvMsg_Pump.data[1]*256+recvMsg_Pump.data[0];// 转速
			Pump_Voltage = recvMsg_Pump.data[3]*256+recvMsg_Pump.data[2]; // 电压
			Pump_Current = (recvMsg_Pump.data[4]*50); // 电流为mA
			Pump_State1 = recvMsg_Pump.data[6];
			Pump_State2 = recvMsg_Pump.data[7];
			if(Flag_6S>=60)
			{
				if((Pump_State1==0)&&((Pump_State2&0XFC)==0))//水泵状态正常
				{
					if(Pump_Target_Speed>=2500)//如果目标转速与当前获取值不同
						Pump_Speed_Dif_Val = Pump_Target_Speed-Pump_Speed;//差值等于目标转速减去实际转速
					else Pump_Speed_Dif_Val=0;

					if((Pump_Speed_Dif_Val<200)&&(Pump_Speed_Dif_Val>-200))//误差在200转以内表示没问题
					{
						i=0;//清零
						if(Water_Pre_State_Flag==0)//水压正常标志位
						{
							//全部正常，清除因水泵故障产生的标志位
							Fault_Level_Two&=0XFFFD;
							Fault_Level_Three&=0XFFF7;
//							Fault_Level_Four&=0XFFF7;
						}
					}
					else
					{
						if((Pump_Speed_Dif_Val<500)&&(Pump_Speed_Dif_Val>-500))//误差在500转以内二级故障
						{
							i++;
							if(i>30)//3秒钟的时间没有调整到位
							{
								Fault_Level_Two|=0X0002;
								fualt(20501);
							}
						}
						else if((Pump_Speed_Dif_Val<1000)&&(Pump_Speed_Dif_Val>-1000))//误差在1000转以内三级故障
						{
							i++;
							if(i>30)//3秒钟的时间没有调整到位
							{
								Fault_Level_Three|=0X0008;
								fualt(30501);
							}
						}
						else//大于1000就是四级故障了
						{
							i++;
							if(i>30)//3秒钟的时间没有调整到位
							{
								Fault_Level_Four|=0X0008;
								fualt(40501);
							}
						}
					}
				}
				else//水泵状态不正常
				{
					if(DC_OUT_V>450)//用DC输出电压来判断母线电压
					{
						if(((Pump_State1&0XF4)!=0)||((Pump_State2&0X90)!=0))//有四级故障
						{
							Fault_Level_Four|=0X0008;
						}
						else if(((Pump_State1&0X0B)!=0)||((Pump_State2&0X48)!=0))//有三级故障
						{
							Fault_Level_Three|=0X0008;
						}
						else//那就是有2级故障了
						{
							Fault_Level_Two|=0X0002;
						}
						if(Pump_State1!=0)
						fualt(0XF000|Pump_State1);//上报故障码
						if((Pump_State2&0XFC)!=0)
						{
							fualt(0XF100|(Pump_State2&0XFC));//上报故障码
						}
					}
				}
			}

		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_Pump, &recvMsg_Pump);
	}
}
//艾尔氢气循环泵报文查询接收函数
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
			H_Pump_Power = recvMsg_H_Pump.data[3]*10; //反馈的输出功率
			H_Pump_State1 = recvMsg_H_Pump.data[0];
			H_Pump_Voltage = recvMsg_H_Pump.data[4]*5;//输入电压
			if(Flag_6S>=60)
			{
				if((H_Pump_State1==0x55)||(H_Pump_State1==0xAA))
				{
					if(H_PUMP_Enable==0X01)//启动使能
					{
						H_Pump_Speed_Dif_Val = H_Pump_Target_Speed-H_Pump_Speed;//差值等于目标转速减去实际转速
					}
					if((H_Pump_Speed_Dif_Val<500)&&(H_Pump_Speed_Dif_Val>-500))//误差在200转以内表示没问题
					{
						i=0;
						//全部正常，清除因水泵故障产生的标志位
						Fault_Level_Two&=0XFFF7;
						Fault_Level_Three&=0XFFEF;
//						Fault_Level_Four&=0XFFEF;
					}
					else
					{
						if((H_Pump_Speed_Dif_Val<1000)&&(H_Pump_Speed_Dif_Val>-1000))//误差在500转以内二级故障
						{
							i++;
							if(i>40)//留3秒钟调节时间
							{
								Fault_Level_Two|=0X0008;
								fualt(20701);
							}
						}
						else if((H_Pump_Speed_Dif_Val<1500)&&(H_Pump_Speed_Dif_Val>-1500))//误差在1000转以内三级故障
						{
							i++;
							if(i>40)//留3秒钟调节时间
							{
								Fault_Level_Three|=0X0010;
								fualt(30701);
							}
						}
						else//大于1500就是四级故障了
						{
							i++;
							if(i>40)//留4秒钟调节时间
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
					if((H_Pump_State1&0XF6)!=0)//有四级故障
					{
//						Fault_Level_Four|=0X0010;
					}
					else
					{
						Fault_Level_Three|=0X0010;
					}
					fualt(0XF200|H_Pump_State1);//上报故障码
				}
				if(State_Flag>=1)
				{
					if((Stack_State&0X0F)>=0X07)//现在处于运行或启动当中
					{
						H_PUMP_Enable=0X00;//发送不使能
						State_Flag++;
						if(State_Flag>=30)
						{
							State_Flag=0;
							H_PUMP_Enable=0X01;//发送使能
						}
					}

				}

			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_H_Pump, &recvMsg_H_Pump);
	}
}
//喷轨减压阀报文查询接收
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
			if((JYF_State&0X30)==0)//无故障
			{
				Fault_Level_Two&=0XFFEF;
				Fault_Level_Three&=0XFFDF;
//				Fault_Level_Four&=0XFFDF;
			}
			else
			{
				if((JYF_State&0X20)==0X20)//喷轨控制器有3级及以上故障
				{
				   //给减压阀发送关闭命令
				   Fault_Level_Four|=0X0020;
				   REL_KG(H_IN,0X01);//关闭氢气入口
				   S_JYF_Data[0]&=0X3F;
				   S_JYF_Data[0]|=0X40; //发送给关闭状态
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
//空气背压阀报文查询接收
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
				if((KQBYF_State1&0X1E)==0)//无故障
				{
					KQBYF_Dif_Angle = KQBYF_Target_Angle-recvMsg_KQBYF.data[1];
					if(KQBYF_Dif_Angle==0)//全部没有问题，清除故障
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
						fualt(0XF900|KQBYF_State1);//上报故障码
					}

				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_KQBYF, &recvMsg_KQBYF);
	}
}
//空压机报文1查询接收
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
			SJKYJ_Speed = (recvMsg_SJKYJ1.data[2]<<16)+(recvMsg_SJKYJ1.data[1]<<8)+(recvMsg_SJKYJ1.data[0]);//空压机转速
			SJKYJ_Bus_I = recvMsg_SJKYJ1.data[3]/2;//分辨率0.5  偏移量0
			SJKYJ_Bus_V = recvMsg_SJKYJ1.data[4]+(recvMsg_SJKYJ1.data[5]<<8);//母线电压  分辨率1 偏移量0
			SJKYJ_Temp = recvMsg_SJKYJ1.data[6];//空压机控制器温度有-40度偏移
			SJKYJ_MotorTemp = recvMsg_SJKYJ1.data[7];//空压机电机温度有-40度偏移


			if(Flag_12S>=120)
			{
				if(SJKYJ_Speed>31000)
	            SJKYJ_Speed_Dif_Val = SJKYJ_Target_Speed-SJKYJ_Speed;//目标转速与实际转速差
				else SJKYJ_Speed_Dif_Val=0;


				if((SJKYJ_Speed_Dif_Val<1000)&&(SJKYJ_Speed_Dif_Val>-1000))
				{
					i=0;
					SJKYJ_ID1_State=0;//状态正常
				}
				else if((SJKYJ_Speed_Dif_Val<2000)&&(SJKYJ_Speed_Dif_Val>-2000))//目标转速与实际转速相差过大
				{
					i++;
					if(i>30)
					{
						SJKYJ_ID1_State=1;//状态异常
						Fault_Level_Two|=0X0040;
						fualt(20301);
					}
				}
				else if((SJKYJ_Speed_Dif_Val<5000)&&(SJKYJ_Speed_Dif_Val>-5000))//目标转速与实际转速相差过大
				{
					i++;
					if(i>30)
					{
						SJKYJ_ID1_State=1;//状态异常
						Fault_Level_Three|=0X0080;
						fualt(30301);
					}
				}
				else//差值大于正负1000
				{
					i++;
					if(i>30)
					{
						SJKYJ_ID1_State=1;//状态异常
						Fault_Level_Four|=0X0080;
						fualt(40301);
					}
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_SJKYJ1, &recvMsg_SJKYJ1);
	}
}
//空压机报文2查询接收
void SJKYJ_ID2_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_SJKYJ2) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_SJKYJ2.id == ID_REC_SJKYJ2)
		{
			ZD_SJKYJ2=0;
			SJKYJ_State = recvMsg_SJKYJ2.data[0];//空压机状态
			SJKYJ_Fualt_Class = recvMsg_SJKYJ2.data[2]&0X0F;//空压机故障等级
			SJKYJ_Fualt_Code3 = (recvMsg_SJKYJ2.data[2]>>4)+((recvMsg_SJKYJ2.data[3]&0X3F)<<4);//故障信息获取
			SJKYJ_Fualt_Code2 = (recvMsg_SJKYJ2.data[3]>>6)+(recvMsg_SJKYJ2.data[4]<<2)+((recvMsg_SJKYJ2.data[5]&0X7F)<<10);//2级故障
			SJKYJ_Fualt_Code1 = (recvMsg_SJKYJ2.data[5]>>7)+(recvMsg_SJKYJ2.data[6]<<1)+(recvMsg_SJKYJ2.data[7]<<9);//1级故障

			if(Flag_6S>=60)
			{
				if(SJKYJ_Fualt_Class <= 1)//无故障或者1级故障
				{
					if(SJKYJ_ID1_State==0)//报文1也正常
					{
						Fault_Level_Two&=0XFFBF;
						Fault_Level_Three&=0XFF7F;
//						Fault_Level_Four&=0XFF7F;
					}
				}
				else if((SJKYJ_Fualt_Code3!=0) || (SJKYJ_Fualt_Class == 3))//3级故障
				{
					Fault_Level_Four|=0X0080;
					fualt(0XF600|(SJKYJ_Fualt_Code3&0X00FF));//字节6
					fualt(0XF700|(SJKYJ_Fualt_Code3>>8));//字节7
				}
				else//说明有2级故障
				{
					Fault_Level_Three|=0X0080;
					fualt(0XF400|(SJKYJ_Fualt_Code2&0XC0));//字节3
					fualt(0XF500|((SJKYJ_Fualt_Code2&0X0BC0)>>8));//字节4
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_SJKYJ2, &recvMsg_SJKYJ2);
	}
}
//升压DC报文1查询接收
void DC_ID1_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_DC) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_DC.id == ID_REC_DC)
		{
			ZD_DC1=0;
			DC_IN_V = recvMsg_DC.data[0]+(recvMsg_DC.data[1]&0X0F)*256;//DC输入电压  1V/bit
			DC_IN_I = (recvMsg_DC.data[2]+(recvMsg_DC.data[3])*256)/10;//DC输入电流   0.1A/bit,除以10，最终单位为A


			DC_State=recvMsg_DC.data[4];//DC状态
			DC_Fault = (recvMsg_DC.data[5]);//DC故障等级
			DC_Fault_code = DC_Fault&0x3F;//DC
			DC_Temp = recvMsg_DC.data[6];//DC温度，有40的偏移值
			Stack_P = (DC_IN_V*DC_IN_I);//单位W
			if((DC_Fault&0XC0)==0)//无故障
			{
				if(DC_ID2_State==0)//报文2也正常
				{
					//清除产生的故障位
					Fault_Level_Three&=0XFEFF;
//					Fault_Level_Four&=0XFEFF;
				}
			}
			else if((DC_Fault&0XC0)==0X40)//1级故障 对应4级关机故障
			{
				Fault_Level_Four|=0X0100;
				fualt(0XF800|DC_Fault);
			}
			else//二级故障 对应3级限功率
			{
				Fault_Level_Three|=0X0100;
				fualt(0XF800|DC_Fault);
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_DC, &recvMsg_DC);
	}
}
//升压DC报文2查询接收
void DC_ID2_Rec(void)
{
	static uint8_t i=0;
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_DC2) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_DC2.id == ID_REC_DC2)
		{
			ZD_DC2=0;
			DC_OUT_V = recvMsg_DC2.data[0]+(recvMsg_DC2.data[1]&0X0F)*256;//DC输出电压  1V/bit
			DC_OUT_I = (recvMsg_DC2.data[2]+(recvMsg_DC2.data[3])*256)/10;//DC输出电流   0.1A/bit,除以10，最终单位为A
			DC_Rec_State = recvMsg_DC2.data[4];//DC接受FCU的控制状态，用来验证跟自己发送的是不是一样
			DC_Rec_I = recvMsg_DC2.data[5]+(recvMsg_DC2.data[6])*256;//DC接受的控制电流

			if(Flag_6S>=60)
			{
				if((DC_Rec_State==S_DC_State)&&((DC_Rec_I*10)==S_DC_I))//DC接收到的与FCU发送的一致 说明没问题
				{
					i=0;
					DC_ID2_State=0;
				}
				else//信息不一致
				{
					i++;
					if(i>100) // 2秒钟检测 2021.06.16
					{
						Fault_Level_Four|=0X0100;//直接4级故障
						fualt(40401);
						DC_ID2_State=1;
					}
				}
			}

		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_DC2, &recvMsg_DC2);
	}
}
//CVM1报文查询接收
void CVM11_ID_Rec(void)
{
   if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_CVM11) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_CVM11.id == ID_CVM11)
		{
			ZD_CVM11=0;
			Stack11_Average_Voltage = (((recvMsg_CVM11.data[4]&0x0F)<<8)+recvMsg_CVM11.data[3])-1300;//采集器平均单体电压   偏移量-1300mv
			Stack1_Total_Voltage = ((recvMsg_CVM11.data[2]<<8)+recvMsg_CVM11.data[1])*0.1;//采集器总电压      分辨率0.1V
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
			Stack12_VoltageMin = (((recvMsg_CVM12.data[1]&0x0F)<<8)+recvMsg_CVM12.data[0]) - 1300;//单片最低电压
			Stack12_NumberMin = ((recvMsg_CVM12.data[3]&0x0F)<<8)+recvMsg_CVM12.data[2];//单片最低电压编号
			Stack12_VoltageMax = (((recvMsg_CVM12.data[5]&0x0F)<<8)+recvMsg_CVM12.data[4]) - 1300;//单片最高电压
			Stack12_NumberMax = ((recvMsg_CVM12.data[7]&0x0F)<<8)+recvMsg_CVM12.data[6];//单片最高电压编号

		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_CVM12, &recvMsg_CVM12);
	}
}

//CVM2报文查询接收
void CVM21_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_CVM21) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_CVM21.id == ID_CVM21)
		{
			ZD_CVM21=0;
			Stack21_Average_Voltage = (((recvMsg_CVM21.data[4]&0x0F)<<8)+recvMsg_CVM21.data[3])-1300;//采集器平均单体电压   偏移量-1300mv
			Stack2_Total_Voltage = ((recvMsg_CVM21.data[2]<<8)+recvMsg_CVM21.data[1])*0.1;//采集器总电压      分辨率0.1V
			Stack_Total_Voltage = Stack1_Total_Voltage + Stack2_Total_Voltage;//2个采集器总电压
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
			Stack22_VoltageMin = (((recvMsg_CVM22.data[1]&0x0F)<<8)+recvMsg_CVM22.data[0]) - 1300;//单片最低电压
			Stack22_NumberMin = ((recvMsg_CVM22.data[3]&0x0F)<<8)+recvMsg_CVM22.data[2];//单片最低电压编号
			Stack22_VoltageMax = (((recvMsg_CVM22.data[5]&0x0F)<<8)+recvMsg_CVM22.data[4]) - 1300;//单片最高电压
			Stack22_NumberMax = ((recvMsg_CVM22.data[7]&0x0F)<<8)+recvMsg_CVM22.data[6];//单片最高电压编号


//			//比较出电堆单片最低电压和最低电压编号
//			Stack_Low_Voltage = Stack12_VoltageMin;
//			Stack_Low_Number = Stack12_NumberMin;
//			if(Stack22_VoltageMin < Stack_Low_Voltage)
//			{
//				Stack_Low_Voltage = Stack22_VoltageMin;
//				Stack_Low_Number = Stack22_NumberMin + 121;
//			}
//
//
//			//比较出电堆单片最高电压和最高电压编号
//			Stack_High_Voltage = Stack12_VoltageMax;
//			Stack_High_Number = Stack12_NumberMax;
//			if(Stack22_VoltageMax > Stack_High_Voltage)//单体最大电压值比较
//			{
//				Stack_High_Voltage = Stack22_VoltageMax;
//				Stack_High_Number = Stack22_NumberMax + 121;
//			}


//			Stack_Average_Voltage = (Stack11_Average_Voltage + Stack21_Average_Voltage) / 2;
			Stack1_Max_V_Dif = abs(Stack11_Average_Voltage - Stack12_VoltageMin);//堆1离均差计算      分辨率1mV
			Stack2_Max_V_Dif = abs(Stack21_Average_Voltage - Stack22_VoltageMin);//堆2离均差计算      分辨率1mV
			if(((Stack_State&0X1F)==0X0F)&&(DC_IN_I>8)&&(FCU_T1_Data>105))//电堆处于运行状态并且电堆电流大于8A的时候才进行单体跟总电压检测
			{
				if((CVM1_Flag == 0)&&(CVM2_Flag == 0))//CVM1,2,3,4,5,6全部报文都可以正常收到
				{
					//对电堆单体压差、单片最低和总电压分等级做保护处理
//					Stack_Average_Voltage = (Stack11_Average_Voltage + Stack21_Average_Voltage) / 2;
//					Stack_Max_V_Dif = Stack_Average_Voltage - Stack_Low_Voltage;//离均差计算      分辨率1mV
					if((Stack1_Max_V_Dif<(CAL_Data[127-offest]*2)) && (Stack2_Max_V_Dif<(CAL_Data[127-offest]*2)))//两个电堆单体压差小于最小故障值
					{
						Time1_1=0;
						Time1_2=0;
						Time1_3=0;

					}
					else if((Stack1_Max_V_Dif>(CAL_Data[191-offest]*2)) || (Stack2_Max_V_Dif>(CAL_Data[191-offest]*2)))//两个电堆单体压差大于最小故障值四级故障
					{
						Time1_3++;
						if(Time1_3>=20)
						{
							Time1_3=20;
							Fault_Level_Four|=0X0200;
							fualt(41005);//上报故障码
						}
					}
					else if((Stack1_Max_V_Dif>(CAL_Data[159-offest]*2)) || (Stack2_Max_V_Dif>(CAL_Data[159-offest]*2)))//单体压差大于最小故障值三级故障
					{
						Time1_2++;
						if(Time1_2>=10)
						{
							Time1_2=10;
							Fault_Level_Three|=0X0200;
							fualt(31001);//上报故障码
						}
					}
					else//单体压差大于最小故障值二级故障
					{
						Time1_1++;
						if(Time1_1>=10)
						{
							Time1_1=10;
							Fault_Level_Two|=0X0200;
							fualt(21001);//上报故障码
						}
					}

					if((Stack12_VoltageMin>(CAL_Data[126-offest]*5)) && (Stack22_VoltageMin>(CAL_Data[126-offest]*5)))//单体最底电压处于正常范围之内
					{
						Time2_1=0;
						Time2_2=0;
						Time2_3=0;
					}
					else if((Stack12_VoltageMin<(CAL_Data[190-offest]*5)) || (Stack22_VoltageMin<(CAL_Data[190-offest]*5)))//单体过低四级故障
					{
						 Time2_3++;
						if(Time2_3>=20)
						{
							Time2_3=20;
							Fault_Level_Four|=0X0200;
							fualt(41006);//上报故障码
						}
					}
					else if((Stack12_VoltageMin<(CAL_Data[158-offest]*5)) || (Stack22_VoltageMin<(CAL_Data[158-offest]*5)))//单体过低三级故障
					{
						Time2_2++;
						if(Time2_2>=10)
						{
							Time2_2=10;
							Fault_Level_Three|=0X0200;
							fualt(31002);//上报故障码
						}
					}
					else
					{
						Time2_1++;
						if(Time2_1>=10)
						{
							Time2_1=10;
							Fault_Level_Two|=0X0200;
							fualt(21002);//上报故障码
						}
					}

					if((Stack1_Total_Voltage>(CAL_Data[125-offest]/2)) && (Stack2_Total_Voltage>(CAL_Data[125-offest]/2)))//总电压大于最低故障值 所有故障都没有
					{
						Time3_1=0;
						Time3_2=0;
						Time3_3=0;
					}
					else if((Stack1_Total_Voltage<(CAL_Data[189-offest]/2)) || (Stack2_Total_Voltage<(CAL_Data[189-offest]/2)))//总电压过低四级故障
					{
						Time3_3++;
						if(Time3_3>=10)//1秒钟以内
						{
							Time3_3=10;
							Fault_Level_Four|=0X0200;
							fualt(41007);//上报故障码
						}
					}
					else if((Stack1_Total_Voltage<(CAL_Data[157-offest]/2)) || (Stack2_Total_Voltage<(CAL_Data[157-offest]/2)))//总电压过低三级故障
					{
						Time3_2++;
						if(Time3_2>=10)
						{
							Time3_2=10;
							Fault_Level_Three|=0X0200;
							fualt(31003);//上报故障码
						}
					}
					else
					{
						Time3_1++;
						if(Time3_1>=10)
						{
							Time3_1=10;
							Fault_Level_Two|=0X0200;
							fualt(21003);//上报故障码
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



//降压DCDC1报文查询接收
void DCDC1_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_6KWDCDC1) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_6KWDCDC1.id == ID_6KWDCDC1)
		{
			DCDC_Out_voltage = ((recvMsg_6KWDCDC1.data[3]<<8)+recvMsg_6KWDCDC1.data[2]) * 0.05; // 输出电压
			DCDC_Out_current = ((((recvMsg_6KWDCDC1.data[5]<<8)+recvMsg_6KWDCDC1.data[4]) - 10000) * 0.05); // 输出电流 先便宜后分辨率
			DCDC_temp = recvMsg_6KWDCDC1.data[1]-40;// 温度
			DCDC_work_state = recvMsg_6KWDCDC1.data[0]&0x03;//工作状态
			DCDC_fault_level = recvMsg_6KWDCDC1.data[0]&0x0C;//故障等级
			DCDC_fault_code = recvMsg_6KWDCDC1.data[6];// 故障码

			if(Flag_6S>=60)
			{
				if(DCDC_fault_level == 2)
				{
					Fault_Level_Four|=0X2000;
					fualt(41201);//上报故障码
					fualt(0XFB00|DCDC_fault_code);
				}
				else if(DCDC_fault_level == 1)
				{
//					Fault_Level_Three|=0X2000;
					fualt(31201);//上报故障码
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

//三通阀报文查询接收
void Valve_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_ThreeWayValve) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_ThreeWayValve.id == ID_ThreeWayValve)
		{
			Valve_fault_code = ((recvMsg_ThreeWayValve.data[6]<<8)+recvMsg_ThreeWayValve.data[5]); // 故障码
			Valve_Open = recvMsg_ThreeWayValve.data[1];//开度
			Valve_State = recvMsg_ThreeWayValve.data[0];//状态

			if(Flag_6S>=60)
			{
				if((Valve_fault_code == 502) || (Valve_fault_code == 602))
				{
					Fault_Level_Four|=0X4000;
					fualt(41301);//上报故障码
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


//PTC报文查询接收
void PTC_ID_Rec(void)
{
	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_PTC) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
		if(recvMsg_PTC.id == ID_PTC)
		{
			PTC_In_voltage = recvMsg_PTC.data[7] * 4; // 输入电压
			PTC_In_current = recvMsg_PTC.data[6] * 0.1; // 输入电流
			PTC_Fault_Code = recvMsg_PTC.data[4]; // 故障码
//			DCDC_temp = recvMsg_6KWDCDC1.data[1]-40;// 温度

			if(Flag_6S>=60)
			{
				if(PTC_Fault_Code != 0)
				{
					Fault_Level_Two|=0X8000;
					fualt(0XFC00|PTC_Fault_Code);
					PTC_Flag = 0;
					PTC_Enable = 0X00;//PTC关闭
					PTC_Target_Temp = 40;//0℃，有40的偏移量
					PTC_Target_PWM = 0;//PWM给固定值0
					if((FCU_T1_Data < 45) || (FCU_T4_Data < 45))
					{
						Fault_Level_Four|=0X8000;
						fualt(40801);//上报故障码
					}
				}
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_PTC, &recvMsg_PTC);
	}
}

//部件报文丢失检查 100ms
void ID_Lose_Check(void)
{
	//只有通过自检之后才可以进行丢帧检查
	if((Stack_State&0X03)==0X03)
	{
		//电子水泵报文丢帧检查
		ZD_Pump++;
		if(ZD_Pump>=20)//2秒钟收不到报文
		{
		   ZD_Pump=20;//防止一直加溢出
		   Fault_Level_Four|=0X0008;//上报四级故障
		   fualt(40502);//上报故障码
		}
		//氢气循环泵报文丢帧检查
		ZD_H_Pump++;
		if(ZD_H_Pump>=20)//2秒钟收不到报文
		{
		   ZD_H_Pump=20;//防止一直加溢出
		   Fault_Level_Four|=0X0010;//上报四级故障
		   fualt(40702);//上报故障码
		}
		//喷轨减压阀报文丢帧检查
		ZD_JYF++;
		if(ZD_JYF>=20)//2秒钟收不到报文
		{
		   ZD_JYF=20;//防止一直加溢出
		   Fault_Level_Four|=0X0020;//上报四级故障
		   fualt(40901);//上报故障码
		}
		//背压阀报文丢帧检查
		ZD_KQBYF++;
		if(ZD_KQBYF>=50)//2秒钟收不到报文
		{
		   ZD_KQBYF=50;//防止一直加溢出
		   Fault_Level_Four|=0X0040;//上报四级故障
		   fualt(40602);//上报故障码
		}
		//势加透波空压报文1丢帧检查
		ZD_SJKYJ1++;
		if(ZD_SJKYJ1>=20)//2秒钟收不到报文
		{
		   ZD_SJKYJ1=20;//防止一直加溢出
		   Fault_Level_Four|=0X0080;//上报四级故障
		   fualt(40302);//上报故障码
		}
		//势加透波空压报文2丢帧检查
		ZD_SJKYJ2++;
		if(ZD_SJKYJ2>=100)//2秒钟收不到报文
		{
		   ZD_SJKYJ2=100;//防止一直加溢出
		   Fault_Level_Four|=0X0080;//上报四级故障
		   fualt(40303);//上报故障码
		}
		//升压DC报文1丢帧检查
		ZD_DC1++;
		if(ZD_DC1>=20)//2秒钟收不到报文
		{
		   ZD_DC1=20;//防止一直加溢出
		   Fault_Level_Four|=0X0100;//上报四级故障
		   fualt(40402);//上报故障码
		}
		//升压DC报文2丢帧检查
		ZD_DC2++;
		if(ZD_DC2>=20)//2秒钟收不到报文
		{
		   ZD_DC2=20;//防止一直加溢出
		   Fault_Level_Four|=0X0100;//上报四级故障
		   fualt(40403);//上报故障码
		}
		//CVM1报文丢帧检查
		ZD_CVM11++;
		ZD_CVM12++;
		if((ZD_CVM11>=50)||(ZD_CVM12>=50))//5秒钟收不到报文
		{
		   ZD_CVM11=50;//防止一直加溢出
		   ZD_CVM12=50;
		   CVM1_Flag = 1;//采集器1通信有异常
		   Fault_Level_Four|=0X0200;//上报四级故障
		   fualt(41001);//上报故障码
		}
		else
		{
		   CVM1_Flag = 0;//采集器1通信正常
		}
		//CVM2报文丢帧检查
		ZD_CVM21++;
		ZD_CVM22++;
		if((ZD_CVM21>50)||(ZD_CVM22>50))//5秒钟收不到报文
		{
		   ZD_CVM21=50;//防止一直加溢出
		   ZD_CVM22=50;
		   CVM2_Flag = 1;//采集器2通信有异常
		   Fault_Level_Four|=0X0200;//上报四级故障
		   fualt(41002);//上报故障码
		}
		else
		{
		   CVM2_Flag = 0;//采集器2通信正常
		}
	}
}
//CANO查询接收部件报文
void CAN0_Receive(void)
{
	   Pump_ID_Rec();//电子水泵报文接收
	   H_Pump_ID_Rec();//艾尔氢气循环泵报文
	   JYF_ID_Rec();//喷轨控制器1报文
	   KQBYF_ID_Rec();//空气背压阀报文
	   SJKYJ_ID1_Rec();//势加透波空压报文1
	   SJKYJ_ID2_Rec();//势加透波空压报文2
	   DC_ID1_Rec();//升压DC报文1
	   DC_ID2_Rec();//升压DC报文2
	   CVM11_ID_Rec();//CVM11报文
	   CVM12_ID_Rec();//CVM12报文
	   CVM21_ID_Rec();//CVM21报文
	   CVM22_ID_Rec();//CVM22报文
	   DCDC1_ID_Rec();//降压DCDC1
	   Valve_ID_Rec();//三通阀
	   PTC_ID_Rec();//PTC
}

//接受上位机报文函数
void CAN0_Receive_COM(void)
{
	// 读取 RX_MAILBOX_REL
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
				Erase_GU_Data();//清除故障码
			}
		}
		CAN_Receive(INST_CAN_PAL0, RX_MAILBOX_RW, &recvMsg_RW);
	}
	else
	{
		ZD_RW++;
		if(ZD_RW>250)ZD_RW=250; // 25秒钟
	}

	if(CAN_GetTransferStatus(INST_CAN_PAL0, RX_MAILBOX_DC_MAN) == STATUS_SUCCESS)
	{
		/* Check the received message ID and payload */
	  if(recvMsg_DC_MAN.id == ID_DC_MAN)//升压DC
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
