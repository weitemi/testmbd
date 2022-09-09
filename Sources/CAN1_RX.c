/*
 * CAN1_RX.c
 *
 *  Created on: 2018年9月13日
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
#include "CAN_RX.h"
#include "CAN1_TX.h"
#include "CAN1_RX.h"
#include "Data_Hand.h"
#include "EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"
#include "Self_Check.h"
#include "Start_Up.h"
#include "Close.h"






//BMS3报文内容
uint16_t Battery_T_Voltage=0;//电池总电压 0.1V/Bit
uint16_t Battery_T_I=0;//电池总电流 1A/Bit
uint16_t Bms_Max_Recharge_I=0;//电池最大充电电流
uint8_t Soc_Data=0;//SOC值
//BMS5报文内容
uint16_t Battery_H_Voltage=0;//单体最高电压
uint16_t Battery_L_Voltage=0;//单体最低电压
uint8_t bms_main_key1=0;//电池主负接触器
//ISO绝缘监测仪报文
uint16_t Res_Data1=0,Res_Data2=0;//正极绝缘阻值 负极绝缘阻值
//VCU报文1内容
uint8_t VCU_Ready_State=0;//VCU Ready状态 0X04
uint8_t full_power_enable=0;//VCU 发送满功率
uint8_t vcu2fcu_command=0;//VCU发送FCU指令
uint8_t crash_state=0;//VCU发送碰撞状态
uint8_t power_model=0;//VCU发送纯电模式 燃电或纯电
uint8_t add_h2_door_state=0;//VCU发送加氢舱门状态
uint8_t urgent_switch_state=0;//VCU 急停开关状态
uint8_t vcu2fcu_limit_power=0;//VCU 整车限制功率
uint8_t vcu2fcu_can_life=0;//VCU 心跳
//仪表报文内容
uint8_t Car_Speed=1;//车速
//降压DC输出的电压电流内容
uint16_t SDC_Out_V=0;//小DC输出电压
uint16_t SDC_Out_I=0;//小DC输出电流
// HMS
uint8_t hms_fault_level=0,hms_fault_code=0;
uint8_t hms_state=0;
uint8_t hms_h2_soc=0;
uint16_t hms_h2_pressure=0;// 氢气压力 Bar
uint32_t vehicle_mileage=0;// 整车里程 km
uint8_t Low_Power_Flag=0;//低功率运行标记


void CAN1_Receive(void)
{
	static uint8_t i=0;
	switch(i)
	{
	   case 0:
		   i=1;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_BMS2) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_BMS2.id == ID_BMS2)
				{
					Soc_Data = recvMsg_BMS2.data[5]*0.5;//电池SOC
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_BMS2, &recvMsg_BMS2);
			}
		   break;
	   case 1:
		   i=2;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_BMS3) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_BMS3.id == ID_BMS3)
				{
					Bms_Max_Recharge_I = recvMsg_BMS2.data[7]*3;//电池最大充电电流
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_BMS2, &recvMsg_BMS3);
			}
		   break;
	   case 2:
		   i=3;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_VCU2) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_VCU2.id == ID_VCU2)
				{
//					vehicle_mileage = (recvMsg_VCU2.data[4]<<16 + recvMsg_VCU2.data[3]<<8 + recvMsg_VCU2.data[2])*0.1;
					vehicle_mileage = ((recvMsg_VCU2.data[4]<<16) + (recvMsg_VCU2.data[3]<<8) + recvMsg_VCU2.data[2]); // 整车里程
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_VCU2, &recvMsg_VCU2);
			}
		   break;
	   case 3:
		   i=4;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_BMS5) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_BMS5.id == ID_BMS5)
				{
					bms_main_key1 = recvMsg_BMS5.data[0]&0x0C;//总负接触器状态
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_BMS5, &recvMsg_BMS5);
			}
		   break;
	   case 4:
		   i=5;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_ISO) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_ISO.id == ID_ISO)
				{
					Res_Data1 = recvMsg_ISO.data[5]*256 + recvMsg_ISO.data[4]; // 正极绝缘值
			        Res_Data2 = recvMsg_ISO.data[7]*256 + recvMsg_ISO.data[6]; // 负极绝缘值
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_ISO, &recvMsg_ISO);
			}
		   break;
	   case 5:
		   i=6;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_VCU1) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_VCU1.id == ID_VCU1)
				{
//					uint16_t sys_run_cuont_temp = 20;
//					S_RUN_Count_Data[0]=sys_run_cuont_temp/256;//高字节
//					S_RUN_Count_Data[1]=sys_run_cuont_temp%256;//低字节
//				    EX_EEPROM_C256_write(29,S_RUN_Count_Data,2);

					full_power_enable=recvMsg_VCU1.data[0]&0x03;
					vcu2fcu_command=recvMsg_VCU1.data[0]&0x0C;
					crash_state=recvMsg_VCU1.data[2]&0x03;
					power_model=recvMsg_VCU1.data[2]&0x0C;
					add_h2_door_state=(recvMsg_VCU1.data[2]&0x30)>>4;
//					if(vcu2fcu_command==0x02&&power_model==0&&bms_main_key1==0x04)
//					sys_run_cuont++;
					// FCU使能指令 纯电模式 加氢舱门
					if(vcu2fcu_command==0x08
							&&Stack_State==0x03
							&&Soc_Data<80
							&&add_h2_door_state==1
							) // 氢系统发送的氢SOC
					{
						VCU_Ready_State=0X04;// 整车状态OK
						Stack_State |= 0x07;// 开机
					}
					else if((vcu2fcu_command==0x00)&&((Stack_State&0X17)==0X07))
					{
						if(DC_Target_I>=100){ // 目标电流大于100A
							Low_Power_Flag = 1;
							Normal_Close_Flag = 1; // 降功率到50A运行 延时下电
						}
						if(Low_Power_Flag==1&&Normal_Close_Flag==0){
							Low_Power_Flag = 0;//清除下电低功率运行标记
						}
						if(DC_Target_I<100&&Normal_Close_Flag==0)
						{
							VCU_Ready_State = 0x03;
						}
					}
					else if(vcu2fcu_command==0x0C)
					{
						VCU_Ready_State = 0x05;
					}

					urgent_switch_state=(recvMsg_VCU1.data[2]&0xC0>>6);
					vcu2fcu_limit_power=recvMsg_VCU1.data[6];//分辨率 1 偏移量 0
					vcu2fcu_can_life=recvMsg_VCU1.data[7];
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_VCU1, &recvMsg_VCU1);
			}
		   break;
	   case 6:
		   i=7;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_CM) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_CM.id == ID_CM)
				{
					Car_Speed = (((recvMsg_CM.data[7]<<8) + recvMsg_CM.data[6])*0.1) - 50;
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_CM, &recvMsg_CM);
			}
		   break;
	   case 7:
		   i=8;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_HMS2) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_HMS2.id == ID_HMS2)
				{
					hms_h2_pressure = ((recvMsg_HMS2.data[1]<<8) + recvMsg_HMS2.data[0]);
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_HMS2, &recvMsg_HMS2);
			}
		   break;
	   case 8:
		   i=0;
		   if(CAN_GetTransferStatus(INST_CAN_PAL1, RX1_MAILBOX_HMS1) == STATUS_SUCCESS)
			{
				/* Check the received message ID and payload */
				if(recvMsg_HMS1.id == ID_HMS1)
				{
					hms_state = recvMsg_HMS1.data[0]&0x0F; //HMS 状态
					hms_fault_level = recvMsg_HMS1.data[0]>>4; //HMS 故障等级
					hms_fault_code = recvMsg_HMS1.data[4]; //HMS 氢系统故障码
					hms_h2_soc = recvMsg_HMS1.data[1]; //HMS 氢SOC
//					if(Flag_6S>=60)
////					{
						if(hms_fault_level==1)
						{
							Fault_Level_Two |= 0X1000;
							fualt(0XFA00|hms_fault_code);
						}
						if(hms_fault_level==3||hms_fault_level==2)
						{
							Fault_Level_Three |= 0X1000;
							fualt(0XFA00|hms_fault_code);
						}
						if(hms_fault_level==4)
						{
							Fault_Level_Four |= 0X1000;
							fualt(0XFA00|hms_fault_code);
						}
						if(hms_fault_level==0)
						{
							Fault_Level_Two&=0XEFFF;
						}
						// 氢系统 三级以上为严重故障 氢系统要求 他们报了二级故障 需在300s内进行关机处理
						if(hms_fault_level==4||hms_fault_level==3||hms_fault_level==2)
						{
	//						Emergency_Shutdown();
							VCU_Ready_State = 0x03;// 关机
						}
//					}
				}
				CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_HMS1, &recvMsg_HMS1);
			}
		   break;
	}
}











