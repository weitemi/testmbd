/*
 * CAN_TX.c
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
/*                       发送给电子水泵数据                   */
/*************************************************************/
uint8_t S_PUMP_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t PUMP_Enable=0;//电子水泵使能状态
uint16_t Pump_Target_Speed=0;//电子水泵目标转速
uint8_t Water_Pre_State_Flag=0;//水压状态标志位 0代表正常 其他都是代表异常
/*************************************************************/
/*                       发送给氢循环泵数据                    */
/*************************************************************/
uint8_t S_QXHB_Data[8]={0,0,0,0,0,0,0,0};//
uint16_t H_Pump_Target_Speed=0;//氢气循环泵目标转速
uint8_t H_PUMP_Enable=0X00;//氢气循环泵使能状态 初始设置为不使能状态
uint8_t QXHB_CANLife=0;//循环泵CANLife
/*************************************************************/
/*                        发送给减压阀数据                     */
/*************************************************************/
uint8_t S_JYF_Data[8]={0X40,0,0,0,0,0,0,0};//默认发送关闭定值模式
/*************************************************************/
/*                      发送给空气背压阀数据                   */
/*************************************************************/
uint8_t S_KQBYF_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t KQBYF_CANLife=0;//空气背压阀CANLife
uint8_t KQBYF_Target_Angle=0;//背压阀目标角度
uint16_t LowTempAngleCompensate=0;//背压阀低温下电角度补偿
/*************************************************************/
/*                        发送给DCDC ID                      */
/*************************************************************/
uint8_t S_DC_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t S_DC_State=0X05;//关机 0X05关机 0X0B开机 0X0C主动放电  0X03预充
uint16_t S_DC_I=0;//DC输入电流值 单位0.1A
uint16_t DC_Target_I=0;//升压DC目标电流 精度1A
int16_t I_Cha=0;//降电流时的前后差值
int16_t I_ADD=0;//降电流时的前后增值
int16_t DC_Target_Old=0;
uint8_t LowTemp_FanCon_Flag=0;//低温风扇切换控制标志位
uint8_t StartUpCurrent_Flag=0;
uint8_t Up_DCDC_Enable=1;
static uint16_t Low_Power_Time=0;//延时下电降功率运行计时
/*************************************************************/
/*                    发送给势加透波空压机  ID                 */
/*************************************************************/
uint8_t S_SJKYJ_Data[8]={0,0,0,0,0,0,0,0};//
uint32_t SJKYJ_Target_Speed=0;//空压机目标转速
uint16_t SJKYJ_Target_Speed_Change=0;//空压机目标转速偏移
uint8_t S_SJKYJ_State=0;//势加透波空压机状态 05不使能   0X0A待机   0X0B运行   0XFE故障复位
uint16_t Speed_Adjust_I=0;//空压机转速调节电流
uint32_t BD_SJKYJ_Speed[7]={0,0,0,0,0,0,0};//获取标定的空压机转速点
uint16_t BD_I_Point[7]={0,0,0,0,0,0,0};//获取标定的空压机转速点对应的电流点
uint16_t LowTempSpeedCompensate=0;//空压机低温下电转速补偿
/*************************************************************/
/*                     发送给6kW车载DCDC ID                   */
/*************************************************************/
uint8_t S_6KWDC_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t SDC_State=0;//3kWDCDC状态
uint8_t Down_DC_Enable=1;
/*************************************************************/
/*                         发送给节温器 ID                        */
/*************************************************************/
uint8_t S_JWQ_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t JWQ_Enable=0X00;//节温器使能状态 初始设置为不使能状态
uint8_t JWQ_Target_Angle = 0;//
/*************************************************************/
/*                         发送给PTC ID                        */
/*************************************************************/
uint8_t S_PTC_Data[8]={0,0,0,0,0,0,0,0};//
uint8_t PTC_Flag = 0;
uint8_t PTC_Enable=0X00;//节温器使能状态 初始设置为不使能状态
uint8_t PTC_Target_Temp = 0;//目标温度值
uint8_t PTC_Target_PWM = 0;//目标PWM值

uint8_t YeWei=0;
uint8_t YeWeiTX=0;

uint8_t PTC_test_Time = 0;

uint16_t Time_10min = 0;


/*************************************************************/
/*                     发送给上位机函数函数                                  */
/*************************************************************/
void Send_Computer(void)
{
//   static uint8_t n = 0;
   if(ZD_RW>=250)return;//25S收不到握手报文就不再发送这些报文
   else
   {
   // 100ms 发送一次
	if ((Timer_TxMsg % 10) == 0) {
		REN_CAN_Send(ID_Relay_State,CAN0_TX_MAILBOX1,REL_State1,REL_State2,REL_State3,REL_State4,Stack_State,ON_Flag,S_Fan_Speed,CAL_Flag); //发送继电器状态 0x18F10201
		CAL_Flag = 0;//发送完之后就清零防止老是弹框
		REN_CAN_Send(ID_Stack2_Data,CAN0_TX_MAILBOX2,(Stack2_Total_Voltage/256),(Stack2_Total_Voltage%256),(Stack21_Average_Voltage/256),(Stack21_Average_Voltage%256),
						(Stack22_VoltageMin/256),(Stack22_VoltageMin%256),(Stack22_NumberMax),(Stack22_NumberMin)); //电堆2状态信息
	} else if ((Timer_TxMsg % 10) == 1) {
		REN_CAN_Send(ID_Sensor_Pre,CAN0_TX_MAILBOX1,(Tank_Pre/256),(Tank_Pre%256),(Tank_Mid_Pre/256),(Tank_Mid_Pre%256),(Stack1_InWater_Pre/256),(Stack1_InWater_Pre%256),(0/256),(0%256)); //发送压力传感器数值 0x18F20201
		Send_CBD_Data();//发送参数标定的数值
	} else if ((Timer_TxMsg % 10) == 2) {
		REN_CAN_Send(ID_Sensor_Temp,CAN0_TX_MAILBOX1,0,FCU_T1_Data,0,FCU_T4_Data,0,FCU_T3_Data,0,FCU_T2_Data); //发送BMU收到的温度 0x18F30201
		Send_GBD_Data();//发送故障标定的数值
	} else if ((Timer_TxMsg % 10) == 3) {
		REN_CAN_Send(ID_Sensor_Data3,CAN0_TX_MAILBOX1,(DC_IN_I/256),(DC_IN_I%256),(DC_IN_V/256),(DC_IN_V%256),Fault_Level_Four>>8,Fault_Level_Four,Fault_Level_Three>>8,Fault_Level_Three); //发送电流环状态报文 0x18F40201
		// 正极绝缘和负极绝缘
		REN_CAN_Send(ID_VCU_Data2,CAN0_TX_MAILBOX2,(Res_Data1/256),(Res_Data1%256),(Res_Data2/256),(Res_Data2%256),Soc_Data,hms_h2_soc,hms_fault_level,VCU_Ready_State); //
	} else if ((Timer_TxMsg % 10) == 4) {
		REN_CAN_Send(ID_Sensor_Data4,CAN0_TX_MAILBOX1,(Send_Stack_Total_P>>24),((Send_Stack_Total_P>>16)%256),((Send_Stack_Total_P>>8)%256),(Send_Stack_Total_P%256),
				0,0,(Out_Interva/256),(Out_Interva%256)); //温度，当前排氢间隔时间
		send_fualt();
	} else if ((Timer_TxMsg % 10) == 5) {
		REN_CAN_Send(ID_Sensor_Data5,CAN0_TX_MAILBOX1,(Stack1_H2_Pre/256),(Stack1_H2_Pre%256),(Stack1_RH/256),(Stack1_RH%256),
						(0/256),(0%256),((H2_PPM>>8)/256),(H2_PPM%256)); //
		REN_CAN_Send(ID_VCU_Data1,CAN0_TX_MAILBOX2,(hms_h2_pressure/256),(hms_h2_pressure%256),(vehicle_mileage>>24),(vehicle_mileage>>16),(vehicle_mileage/256),(vehicle_mileage%256),(Car_Speed+50),Out_H_Flag); //整车报文2
	} else if ((Timer_TxMsg % 10) == 6) {
		REN_CAN_Send(ID_Accessory_Data,CAN0_TX_MAILBOX2,(FCU_Flowmeter>>24),(FCU_Flowmeter>>16),(FCU_Flowmeter/256),(FCU_Flowmeter%256),vcu2fcu_command,Time_10min>>8,Time_10min,YeWeiTX); //
	} else if ((Timer_TxMsg % 10) == 7) {
		REN_CAN_Send(ID_Temporary_Data,CAN0_TX_MAILBOX1,0,vcu2fcu_command,0,vcu2fcu_limit_power,(DC_Target_I/256),(DC_Target_I%256),Normal_Close_Time>>8,Normal_Close_Time); //临时变量观测地址

	} else if ((Timer_TxMsg % 10) == 8) {
		REN_CAN_Send(ID_Stack1_Data,CAN0_TX_MAILBOX1,(Stack1_Total_Voltage/256),(Stack1_Total_Voltage%256),(Stack11_Average_Voltage/256),(Stack11_Average_Voltage%256),
							 (Stack12_VoltageMin/256),(Stack12_VoltageMin%256),(Stack12_NumberMax),(Stack12_NumberMin)); //电堆1状态信息
	}else if((Timer_TxMsg % 10)==9){
		REN_CAN_Send(ID_Send_CVM2,CAN0_TX_MAILBOX0,1,0,0,0,0,0,0,0); //CVM报文数量控制地址
	}

	if((Timer_TxMsg % 500) == 1)//5s 软件版本
	{
		REN_CAN_Send(ID_Software_Version,CAN0_TX_MAILBOX2,
										0X20,//硬件状态号、主版本号
										0X00,//硬件次版本号、子版本号
										0X21,//软件状态号:T;主版本号:1;
										0X24,//软件次版本号:2;;子版本号:4;
										0X15,//21年
										0X0B,//11月
										0X04,//4日
										0); //
	}
  }
}
/*************************************************************/
/*                      发送参数标定值函数                                    */
/*************************************************************/
void Send_CBD_Data(void)
{
	static uint8_t i=0;
    if(CBD_Read_Flag==1)//参数读取
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
			CBD_Read_Flag=0;//读取完成之后标志位清零，不再发送此报文
			i=0;
		}
    }
}
/*************************************************************/
/*                        发送故障标定值函数                                        */
/*************************************************************/
void Send_GBD_Data(void)
{
	static uint8_t i=0;
    if(GBD_Read_Flag==1)//故障读取
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
			GBD_Read_Flag=0;//读取完成之后标志位清零，不再发送此报文
			i=0;
		}
    }
}
/*************************************************************/
/*                    发送给电子水泵报文函数                                 */
/*************************************************************/
void Send_PUMP(void)
{
	static uint8_t i=0;
	int16_t H_Water_Pre_Dif=0;
    if((R_KQBYF_QXHB_PUMP_Data[1]>>4)==0X01)//手动控制命令
    {
    	REL_State4|=0X40;//发送给上位机手动控制
    	if(((R_KQBYF_QXHB_PUMP_Data[7]<<8)+R_KQBYF_QXHB_PUMP_Data[6])>=2000)
    	{
    		S_PUMP_Data[0]=R_KQBYF_QXHB_PUMP_Data[5];//发送使能状态
    		S_PUMP_Data[2]=R_KQBYF_QXHB_PUMP_Data[6];
    		S_PUMP_Data[3]=R_KQBYF_QXHB_PUMP_Data[7];
    	}
    	else
    	{
    		S_PUMP_Data[0]=R_KQBYF_QXHB_PUMP_Data[5];//发送不使能状态
    		S_PUMP_Data[2]=0;
    		S_PUMP_Data[3]=0;
    	}
    }
    else//自动控制
    {
    	REL_State4&=0XBF;//发送给上位机自动控制
    	H_Water_Pre_Dif = Stack1_H_Pre-Stack1_InWater_Pre;
    	if((PUMP_Enable==0X01)&&(LowTempStart_Flag == 0))//只有在水泵使能的状态下才去进行调整
    	{
    		i++;
//			if((i>=30)&&((Stack_State&0X10)!=0X10))//300ms判断一次，防止一直判断转速一直增加或减少     电堆启动下
    		if(i>=30)//300ms判断一次，防止一直判断转速一直增加或减少
			{
				i=0;
				if((H_Water_Pre_Dif<500)&&(H_Water_Pre_Dif>-500))//氢水压差处于正负50Kpa以内
				{
					Water_Pre_State_Flag=0;//水压状态位正常
					if(H_Water_Pre_Dif>200)//氢大于水20KPA
					{
						Pump_Target_Speed+=200;//水泵转速增加200转
					}
					else if((H_Water_Pre_Dif<100)||(Stack1_InWater_Pre>2000))//氢大于水 小于10KPA  或者水压超过100kpa降转速
					{
						if(Pump_Target_Speed>=2700)//保证水泵最低转速2500
						{
							Pump_Target_Speed-=200;//水泵转速减少200转
						}
					}
				}
				else if((H_Water_Pre_Dif>500)&&(Pump_Target_Speed<6400)&&(Stack1_InWater_Pre<1900)&&(Stack1_H_Pre<2200))//氢大于水50KPA  转速低于6400转  水压不超过90KPA  氢气压力不超过120KPA(因为电堆水压最大不能超过100KPA)  满功率时水压100kpa，而氢气压力160kpa（两者本来存在50kpa压差属于正常现象，所以氢气压力超过120kpa后不判断压差故障）
				{
					Pump_Target_Speed+=500;
					Water_Pre_State_Flag++;
					if(Water_Pre_State_Flag>=10)
					{
						Water_Pre_State_Flag=10;
						Fault_Level_Four|=0X0008;//上报故障
						fualt(40503);//上报故障码
					}
				}
				else if(H_Water_Pre_Dif<-500)//氢小于水50KPA
				{
					Pump_Target_Speed-=500;
					Water_Pre_State_Flag++;
					if(Water_Pre_State_Flag>=10)
					{
						Water_Pre_State_Flag=10;
						Fault_Level_Four|=0X0008;//上报故障
						fualt(40504);//上报故障码
					}
				}

				// 1、电堆下电状态  下电时水泵转速快速降低，避免因水泵转速下降太慢导致氢水压差过大对电堆影响
				if((Stack_State&0X10)==0X10)
				{
					Pump_Target_Speed-=400;//水泵转速减少200转
				}
			}

			// 1、电堆下电状态  下电时水泵转速快速降低，避免因水泵转速下降太慢导致氢水压差过大对电堆影响
            // 2、水压超过100KPA，降水泵转速（电堆水压不能超过100KPA）
//			if(((Stack_State&0X10)==0X10)||(Stack1_InWater_Pre >2000))
			if(Stack1_InWater_Pre >2000)
			{
				Pump_Target_Speed-=200;//水泵转速减少200转
			}

		    //在使能状态下，限制水泵的最大跟最低转速
			if(Pump_Target_Speed>6400)Pump_Target_Speed=6400;
			if(Pump_Target_Speed<2000)Pump_Target_Speed=2000;
    	}
    	else//处于非使能状态下
    	{
    		Pump_Target_Speed=0;//非使能状态下，转速设置为0
    	}

    	if(LowTempStart_Flag == 1)//在冷启动状态下对转速限制
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
///*                    发送给氢循环泵报文函数                                 */
///*************************************************************/
//void Send_QXHB(void)
//{
//	if(R_QXHB_Data[1]==0X0F)//手动控制命令
//	{
//		REL_State4|=0X80;//发送给上位机手动控制
//		S_QXHB_Data[0]=R_QXHB_Data[0];
//		S_QXHB_Data[1]=R_QXHB_Data[1];
//		S_QXHB_Data[2]=R_QXHB_Data[2];
//		S_QXHB_Data[3]=R_QXHB_Data[3];
//		S_QXHB_Data[4]=R_QXHB_Data[4];
//		S_QXHB_Data[5]=R_QXHB_Data[5];
//		S_QXHB_Data[6]=R_QXHB_Data[6];
//		S_QXHB_Data[7]=R_QXHB_Data[7];
//	}
//	else//自动控制
//	{
//		REL_State4&=0X7F;//发送给上位机自动控制
//		if(H_PUMP_Enable==0X05)//启动使能
//		{
//			H_Pump_Target_Speed = ((DC_IN_I+2)/50)*1000+1000;//每增加50A，循环泵转速增加1000转 到达250A的时候循环泵就最大转速
//			//因为DC检测电流在整点有偏移，正负10（1A），所以要加上个23（2.3A），防止他在50A的时候来回变动
//			//只要在使能状态下，最小1000转，最大6000转
//			if(H_Pump_Target_Speed<=1000)H_Pump_Target_Speed=1000;
//			if(H_Pump_Target_Speed>=5000)H_Pump_Target_Speed=5000;
//		}
//		else//启动不使能
//		{
//			H_Pump_Target_Speed=0;
//		}
//		S_QXHB_Data[0]=H_PUMP_Enable;
//		S_QXHB_Data[4]=(H_Pump_Target_Speed%256);//低字节
//		S_QXHB_Data[5]=(H_Pump_Target_Speed>>8);//高字节
//	}
//	REN_CAN_Send(ID_Send_QXHB,S_QXHB_Data[0],S_QXHB_Data[1],S_QXHB_Data[2],S_QXHB_Data[3],
//			S_QXHB_Data[4],S_QXHB_Data[5],S_QXHB_Data[6],S_QXHB_Data[7]); //
//}

/*************************************************************/
/*                    发送给氢循环泵报文函数  东德                                */
/*************************************************************/
void Send_QXHB(void)
{
	QXHB_CANLife++;
		if(QXHB_CANLife > 255)
		{
			KQBYF_CANLife = 0;
		}
	if((R_KQBYF_QXHB_PUMP_Data[1]&0X04)==0x04)//手动控制命令
	{
		REL_State4|=0X80;//发送给上位机手动控制
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
	else//自动控制
	{
		REL_State4&=0X7F;//发送给上位机自动控制
		if(H_PUMP_Enable==0X01)//启动使能
		{
			H_Pump_Target_Speed = ((DC_IN_I+2)/40)*500+2500;//每增加40A，循环泵转速增加500转 到达440A的时候循环泵就最大转速
			if(DC_IN_I>440)
			{
				H_Pump_Target_Speed = 8000;
			}
//			//因为DC检测电流在整点有偏移，正负10（1A），所以要加上个23（2.3A），防止他在50A的时候来回变动
			//只要在使能状态下，最小1000转，最大10000转
			if(H_Pump_Target_Speed<=1000)H_Pump_Target_Speed=1000;
			if(H_Pump_Target_Speed>=8000)H_Pump_Target_Speed=8000;

//			if(DC_IN_I<=200)              //电流小于等于200A
//			{
//				H_Pump_Target_Speed = 3000;//氢气循环泵转速3000转
//			}
//			else if(DC_IN_I<=250)         //电流小于等于250A
//			{
//				H_Pump_Target_Speed = 3500;//氢气循环泵转速3500转
//			}
//			else if(DC_IN_I<=300)         //电流小于等于300A
//			{
//				H_Pump_Target_Speed = 5000;//氢气循环泵转速5000转
//			}
//			else                          //电流大于300A
//			{
//				H_Pump_Target_Speed = 6000;//氢气循环泵转速6000转
//			}
		}
		else//启动不使能
		{
			H_Pump_Target_Speed=0;
		}
		S_QXHB_Data[4]=H_PUMP_Enable;
		S_QXHB_Data[2]=(H_Pump_Target_Speed%256);//低字节
		S_QXHB_Data[3]=(H_Pump_Target_Speed>>8);//高字节
		S_QXHB_Data[7]=QXHB_CANLife;
	}
	REN_CAN_Send(ID_Send_QXHB,CAN0_TX_MAILBOX0,S_QXHB_Data[0],S_QXHB_Data[1],S_QXHB_Data[2],S_QXHB_Data[3],
			S_QXHB_Data[4],S_QXHB_Data[5],S_QXHB_Data[6],S_QXHB_Data[7]); //
}
/*************************************************************/
/*                   发送给喷轨减压阀报文函数                                */
/*************************************************************/
void Send_JYF(void)
{
  REN_CAN_Send(ID_Send_JYF,CAN0_TX_MAILBOX1,S_JYF_Data[0],S_JYF_Data[1],S_JYF_Data[2],S_JYF_Data[3],
		  S_JYF_Data[4],S_JYF_Data[5],S_JYF_Data[6],S_JYF_Data[7]); //
}
/*************************************************************/
/*                    发送给空气背压阀报文函数                             */
/*************************************************************/
void Send_KQBYF(void)
{
	KQBYF_CANLife++;
	if(KQBYF_CANLife > 128)
	{
		KQBYF_CANLife = 0;
	}
	if((R_KQBYF_QXHB_PUMP_Data[1]&0X01)==0X01)//手动控制
	{
		REL_State3|=0X01;//发送给上位机手动控制
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
	else//自动控制
	{
		REL_State3&=0XFE;//发送给上位机手动控制
		if(S_SJKYJ_State==0X05&&SJKYJ_Speed<=500)//空压机不使能时，背压阀关闭（目的：将空气路密闭）
		{
			KQBYF_Target_Angle=0;//背压阀角度0度
		}
		else
		{
			if(SJKYJ_Target_Speed<=60000)//空压机目标转速小于53000
			{
				KQBYF_Target_Angle=50;//背压阀角度50度
			}
			else if(SJKYJ_Target_Speed<=65000)//空压机目标转速60000-70000
			{
				KQBYF_Target_Angle=60;//背压阀角度50度
			}
			else if(SJKYJ_Target_Speed<=70000)//空压机目标转速60000-70000
			{
				KQBYF_Target_Angle=70;//背压阀角度60度
			}
			else if(SJKYJ_Target_Speed<=75000)//空压机目标转速70000-75000
			{
				KQBYF_Target_Angle=80;//背压阀角度70度
			}
			else if(SJKYJ_Target_Speed<=80000)//空压机目标转速75000-80000
			{
				KQBYF_Target_Angle=90;//背压阀角度90度
			}
			else if(SJKYJ_Target_Speed<=87000)//空压机目标转速80000-87000
			{
				KQBYF_Target_Angle=110;//背压阀角度110度
			}
			else
			{
				KQBYF_Target_Angle=120;//背压阀角度120度
			}
			if((Stack_State&0X0F)==0X07){ //启动阶段角度开到最大 利于吹扫
				KQBYF_Target_Angle=90;//背压阀角度90度
			}
			if(LowTemp_FanCon_Flag == 1)//低温停机时候，调节阴极计量比
			{
				KQBYF_Target_Angle=120;//背压阀角度120度
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
/*                    发送给DCDC报文函数                                       */
/*************************************************************/
void Send_DC(void)
{
	static uint8_t Life=0;
	static uint8_t Time_1S=0;
	static uint8_t stack_flag = 0; // 压差限制标记
	static uint8_t first_flag = 0; // 第一次启动标记
	static uint8_t soc_flag = 0;
	//  static uint8_t Time_30S=0;
	static uint16_t I_Rise_Time=0;//电流上升时间
	int16_t DC_In_Last=0;

	if((R_DC_MAN_Data[0]&0XF0)==0XF0)//手动控制来了
	{
		REL_State3|=0X02;//发送给上位机手动控制
		S_DC_State = (R_DC_MAN_Data[0]&0X0F);
		S_DC_I = (R_DC_MAN_Data[4]<<8)+R_DC_MAN_Data[5];
		DC_Target_I = (S_DC_I/10);
		//手动控制的时候不需要调节精度 上位机已经赋值
	}
//	else if((R_DC_MAN_Data[0]&0XF0)==0X10)//关机的时候采用这个
//	{
//		S_DC_State = (R_DC_MAN_Data[0]&0X0F);
//		S_DC_I = DC_Target_I*10;
//	}
	else if((Stack_State&0X10)!=0X10)//自动控制    添加电堆非关机状态原因：防止关机时进入下面语句导致下电降电流时按照10A降电流。
	{
		REL_State3&=0XFD;//发送给上位机自动控制


		if(S_DC_State==0X0B)//现在是开启状态
		{
			Up_DCDC_Enable = 0;

			I_Rise_Time++;
			if(I_Rise_Time>=(CAL_Data[9]*10))//到达了电流上升时间
			{
				if(first_flag==0){
					first_flag = 1;
				}
				I_Rise_Time=0;
				DC_Target_I+=(CAL_Data[10]);//每次的上升电流
			}
			if(Soc_Data>CAL_Data[57])//SOC处于 功率点1  100A
			{
				if(DC_Target_I>=CAL_Data[11])
					DC_Target_I = CAL_Data[11];
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[58])//SOC处于 功率点2  150A
			{
				if(DC_Target_I>=CAL_Data[12])
					DC_Target_I = CAL_Data[12];
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[59])//SOC处于 功率点3  200A
			{
				if(DC_Target_I>=(CAL_Data[13]*2))//因为这里标定的值是经过除以2的，比如200A，实际标定值是100。
					DC_Target_I = CAL_Data[13]*2;
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[60])//SOC处于 功率点4  250A
			{
				if(DC_Target_I>=(CAL_Data[14]*2))//因为这里标定的值是经过除以2的，比如250A，实际标定值是125。
					DC_Target_I = CAL_Data[14]*2;
					soc_flag = Soc_Data;
			}
			else if(Soc_Data>CAL_Data[61])//SOC处于 功率点4  300A
			{
				if(DC_Target_I>=(CAL_Data[15]*2))//因为这里标定的值是经过除以2的，比如300A，实际标定值是150。
					DC_Target_I = CAL_Data[15]*2;
					soc_flag = Soc_Data;
			}
			else //SOC处于 功率点5  360A
			{
				if(DC_Target_I>=(CAL_Data[15]*2+60))//加60因为标定窗口不够用，在上一个窗口基础上加60
					DC_Target_I = CAL_Data[15]*2+60;
					soc_flag = Soc_Data;
			}
			//电堆入口水温小于68度 最大电流100A 因为有40度的偏移量，所以68度就是108
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
			//  离均差 检测
//			if(((Stack1_Max_V_Dif>50)||(Stack2_Max_V_Dif>50)||(stack_flag==1))&&(DC_Target_I>=50)) // 维持系统运行至低压差 小于65
//			{
//				DC_Target_I=50;
//				stack_flag=1;
//			}
//			if((Stack1_Max_V_Dif<35) && (Stack2_Max_V_Dif<35))
//			{
//				stack_flag=0;//不限制
//			}
			// 原地长时间大功率工作 进行限功策略
			if(DC_Target_I>150&&Car_Speed==0)
			{
				Time_10min++;
				if(Time_10min>=6000)  // 10分钟 当前函数周期100ms
				{
					DC_Target_I = 150;
					Time_10min=6000;
				}
			}
			if(Car_Speed>2){ //车动了重新计数
				Time_10min=0;
			}

			if(vcu2fcu_command==0x08&&(DC_Target_I>((vcu2fcu_limit_power*1250)/DC_IN_V)))//根据VCU的限制功率做计算
			{
				DC_Target_I=(uint16_t)((vcu2fcu_limit_power*1250)/DC_IN_V);
			}
//			if(vcu2fcu_command==0x08&&(DC_Target_I>((Bms_Max_Recharge_I*DC_OUT_V)/DC_IN_V)))//根据BMS的充电电流做计算
//			{
//				DC_Target_I=(uint16_t)((Bms_Max_Recharge_I*DC_OUT_V)/DC_IN_V);
//			}
			// 增加降功率延时下电
			if(Normal_Close_Flag==1&&DC_Target_I>=50)
			{
				DC_Target_I=50;
				if(DC_IN_I<60)
				{
					Low_Power_Time++;
					if(LowTemp_Close_Flag == 1)
					{
						LowTemp_FanCon_Flag = 1;
						if(Low_Power_Time>=1800)//低功率运行3分钟
						{
							Normal_Close_Flag=0;//延时关机完成
							VCU_Ready_State = 0x03;//进入关机流程
							Low_Power_Time=0;
						}
					}
					else
					{
						if(Low_Power_Time>=200)//低功率运行20秒
						{
							Normal_Close_Flag=0;//延时关机完成
							VCU_Ready_State = 0x03;//进入关机流程
							Low_Power_Time=0;
						}
					}
				}
			}

			Time_1S++;
			if(Time_1S>=20)//2秒过后调整 三级故障降功率
			{
				if(Fault_Level_Three)//有三级故障，需要限制功率为当前一半
				{
					Time_1S=20;
		//				if(Time_30S)
		//				DC_Target_I = DC_Target_I/2;//目标电流限制到当前一半
					DC_In_Last = DC_Target_I*5;//目标电流限制到当前一半
				}
				else//无三级故障
				{
					Time_1S=0;
					DC_In_Last = DC_Target_I*10;//1秒钟以后才将目标电流值赋给发送值，让空压机转速先调整 S_DC_I的精度是0.1A，DC_Target_I精度是1A，所以需要乘以10
					I_ADD = DC_In_Last - (DC_Target_Old*10);//
					if(I_ADD>=200)//防止三级故障消失后电流直接恢复目标值
					{
						DC_In_Last = DC_Target_Old*10;
						DC_Target_I = DC_Target_Old;
					}
				}

				I_Cha=(DC_Target_Old*10)-DC_In_Last;
				if(I_Cha>=100)//降电流幅度超过10A，进行缓慢降
				{
					DC_Target_Old-=20;//目标电流每次减少2A
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
		else//当前处于不是启动状态
		{
			Up_DCDC_Enable = 1;
		//		S_DC_State &=0x0f ;
			S_DC_I=0;//直接发送0A就行 也可能是主动放电状态
		//		if((SJKYJ_Speed > 25000)&(SJKYJ_Speed < 33000))
		//		{
		//			S_DC_State=0X0C;//DC主动放电模式
		//			DC_Target_I=10;
		//		}
		//		else
		//		{
		//
		//		}
		}
	}

//	if(((Stack_State&0X10)==0X10)&&((R_DC_MAN_Data[0]&0XF0)==0XF0))//防止切换到手动控制时电流赋值为0A，导致0A和10A来回切换
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
		  S_DC_Data[4],S_DC_Data[5],S_DC_Data[6],Life); //发送给DCDC 0x0CFF01A7
	Life++;
}

/*************************************************************/
/*                    发送给DCDC2报文函数                                      */
/*************************************************************/
void Send_DC2(void)
{
	static uint8_t Life=0;
	  REN_CAN_Send(ID_Send_DC2,CAN0_TX_MAILBOX0,0XDC,0X05,0X64,0X19,
			  0,0,0,Life); //发送给DCDC报文2 最大电流150A 最高电压650V
	  Life++;
}
/*************************************************************/
/*                发送给势加透波空压机报文函数                              */
/*************************************************************/
void Send_SJKYJ(void)
{
	static uint8_t SJKYJ_Life=0;//空压机Life
	uint8_t i=0;
	static uint16_t SpeedChange_Count=0;//转速补偿延时计数

	if(R_SJKYJ_Data[7]==0X0F)//手动控制来了
	{
		REL_State3|=0X08;//发送给上位机手动控制
		S_SJKYJ_State=R_SJKYJ_Data[0];
		S_SJKYJ_Data[1]=R_SJKYJ_Data[1];
		S_SJKYJ_Data[3]=R_SJKYJ_Data[3];
		S_SJKYJ_Data[4]=R_SJKYJ_Data[4];
		S_SJKYJ_Data[5]=R_SJKYJ_Data[5];
		SJKYJ_Target_Speed = R_SJKYJ_Data[3]+(R_SJKYJ_Data[4]<<8)+(R_SJKYJ_Data[5]<<16);
		S_SJKYJ_Data[6]=0;
		S_SJKYJ_Data[7]=0;
	}
	else//自动控制
	{
		REL_State3&=0XF7;//发送给上位机自动控制
		if(S_SJKYJ_State == 0X0B)//空压机状态处于使能位
		{
//			if((Fault_Level_Two&0X0200)==0X0200)SJKYJ_Target_Speed_Change=2000;//CVM单体或总电压有二级故障  空压机转速增加1000转
//			if((Fault_Level_Three&0X0200)==0X0200)SJKYJ_Target_Speed_Change=3000;//CVM单体或总电压有三级故障 空压机转速增加1000转
			if(((Fault_Level_Two&0X0200)!=0X0200)&&((Fault_Level_Three&0X0200)!=0X0200))//即没有二级故障也没有三级故障
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
			if((DC_Target_I*10)>S_DC_I)//目标电流大于实际电流
			{
			   Speed_Adjust_I =  DC_Target_I; //调节电流就根据目标电流来计算 精度1A
			}
			else Speed_Adjust_I = (S_DC_I/10);//调节电流就根据实际电流来计算  精度1A
			for(i=1;i<7;i++)
			{
			   if(Speed_Adjust_I<=(CAL_Data[49+i]*2))//位于当前电流点  还需要加上当前偏移点
			   {
				   SJKYJ_Target_Speed =SJKYJ_Target_Speed_Change +  (CAL_Data[33+i-1]*1000) +
				(Speed_Adjust_I - (CAL_Data[49+i-1]*2))*((CAL_Data[33+i] - CAL_Data[33+i-1])*1000)/((CAL_Data[49+i]-CAL_Data[49+i-1])*2);
				   break;
			   }
			}
		}
		else
		{
			SJKYJ_Target_Speed=0;//不使能状态下，转速直接为0
		}
	}
	if((LowTemp_Close_Flag == 1)&&(LowTemp_FanCon_Flag == 1)&&(Low_Power_Time<=7200))//低温下电时候阴极计量比需要给到10，在50A时加大空压机转速
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
    	S_SJKYJ_State = 0xFE;// 二级故障复位
    }
	REN_CAN_Send(ID_Send_SJKYJ,CAN0_TX_MAILBOX0,S_SJKYJ_State,S_SJKYJ_Data[1],SJKYJ_Life,S_SJKYJ_Data[3],
			S_SJKYJ_Data[4],S_SJKYJ_Data[5],S_SJKYJ_Data[6],S_SJKYJ_Data[7]); //
	if(SJKYJ_Fualt_Class==2&&S_SJKYJ_State==0xFE){
		S_SJKYJ_State = 0X0B;// 复位后启动
	}
	SJKYJ_Life++;
}

/*************************************************************/
/*                    发送给6kW车载DCDC报文函数                           */
/*************************************************************/
void Send_6KWDC(void)
{
	static uint8_t F_Enable = 0;

	if(R_JWQ_PTC_6KWDC_Data[2]==0X0F)//手动控制
	{
		REL_State4|=0X08;//发送给上位机手动控制

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
		REL_State4&=0XF7;//发送给上位机自动控制

		if(DC_OUT_V < 450)
		{
			F_Enable = 0x01;//关机
			Down_DC_Enable = 1;
		}
		else if(DC_OUT_V >=450)
		{
			F_Enable = 0x02;//开机
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
/*                       发送给节温器报文函数                                                                   */
/*************************************************************/
void Send_JWQ(void)
{

//	static uint8_t JWQ_Angle_Flag = 0;//根据进水温度控制节温器角度标志位

	if(R_JWQ_PTC_6KWDC_Data[1]==0X0F)//手动控制
	{
		REL_State4|=0X10;//发送给上位机手动控制
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
		REL_State4&=0XEF;//发送给上位机自动控制
		if(JWQ_Enable == 0X01)//节温器处于运行状态
		{
			if(((Stack_State&0X17)==0X07))//只有在运行中或者启动中的时候做如下操作
			{
				if(FCU_T1_Data <= 104)//温度小于65度 调小角度 全自加热通道
				{
					JWQ_Target_Angle = 0;//JWQ加热路打开，散热器路开启一点
				}
				else if((FCU_T1_Data > 104)&&(FCU_T1_Data <= 108))
				{
					JWQ_Target_Angle = 15;
				}
				else if((FCU_T1_Data > 108)&&(FCU_T1_Data <= 109))
				{
					JWQ_Target_Angle = 30;//JWQ加热路打开，散热器路开启一点
				}
	//			else if((FCU_T1_Data > 109)&&(FCU_T1_Data <= 110))
	//			{
	//				JWQ_Target_Angle = 40;//JWQ加热路和散热器路全通
	//			}
	//			else if((FCU_T1_Data > 110)&&(FCU_T1_Data <= 113))
	//			{
	//				JWQ_Target_Angle = 50;//JWQ加热路和散热器路全通
	//			}
				else if(FCU_T1_Data > 109)
				{
					JWQ_Target_Angle = 60;//角度100 水路全部走散热器通道 关闭自加热
				}
				if(DC_Target_I>300)
				{
					JWQ_Target_Angle = 60;
				}
			}
			else
			{
				if((LowTemp_Close_Flag == 1) && (DC_Target_I > 10))//增加电流判断原因：在低温关机20A运行时候防止水箱内的冷水进入电堆内导致单片电压上升
				{
					if(FCU_T1_Data <= 112)//温度小于70度 调小角度 全自加热通道
					{
						JWQ_Target_Angle = 0;//
					}
					else if(FCU_T1_Data > 113)
					{
						JWQ_Target_Angle = 60;//角度60
					}
				}
				else
				{
					JWQ_Target_Angle = 60; //角度60 水路全部走散热器通道
				}
			}
		}
		else
		{
			JWQ_Target_Angle = 60; //角度60 水路全部走散热器通道
		}

		S_JWQ_Data[0]=0x01;//电机工作模式运行
		S_JWQ_Data[7]=JWQ_Target_Angle;//电机开度
	}

	REN_CAN_Send(ID_Send_JWQ,CAN0_TX_MAILBOX0,S_JWQ_Data[0],S_JWQ_Data[1],S_JWQ_Data[2],S_JWQ_Data[3],
			S_JWQ_Data[4],S_JWQ_Data[5],S_JWQ_Data[6],S_JWQ_Data[7]); //
}

/*************************************************************/
/*                       发送给PTC报文函数                                                                   */
/*************************************************************/
void Send_PTC(void)
{

	if(R_JWQ_PTC_6KWDC_Data[4]==0X0F)//手动控制
	{
		REL_State4|=0X20;//发送给上位机手动控制
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
		REL_State4&=0XDF;//发送给上位机自动控制
		if(((FCU_T1_Data < 45)||(FCU_T4_Data < 45))&&((Stack_State&0X0F)==0X07) &&(Pump_Speed > 1500))//电堆出入口温度低于5℃，电堆启动时，启动PTC加热
		{
			PTC_Enable = 0X03;//PTC启动，并进行温度控制
//			PTC_Flag = 1;
			PTC_test_Time++;
		}
		else if((Stack_State&0X10)==0X10)//下电  电堆状态
		{
			PTC_Enable = 0X00;//PTC关闭
			PTC_Target_Temp = 40;//0℃，有40的偏移量
			PTC_Target_PWM = 0;//PWM给固定值0
		}

		if(FCU_T1_Data > 80)//当电堆入口温度达到25℃后，关闭PTC
		{
//			PTC_Flag = 0;
			PTC_Enable = 0X00;//PTC关闭
			PTC_Target_Temp = 40;//0℃，有40的偏移量
			PTC_Target_PWM = 0;//PWM给固定值0
		}

		if(PTC_Enable == 0X03)//PTC处于运行状态
		{
			PTC_Target_Temp = 120;//80℃，有40的偏移量
			PTC_Target_PWM = 100;//PWM给固定值100
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
		Send_PUMP();//发送给水泵
	} else if ((Timer_TxMsg % 10) == 1) {
		Send_QXHB();//发送给氢气循环泵
	} else if ((Timer_TxMsg % 10) == 2) {
		Send_KQBYF();//发送给空气背压阀
	} else if ((Timer_TxMsg % 10) == 3) {
		Send_DC();//发送给升压DC报文1
	} else if ((Timer_TxMsg % 10) == 4) {
		Send_DC2();//发送给升压DC报文2
	} else if ((Timer_TxMsg % 10) == 5) {
		Send_SJKYJ();//发送给势加透博空压机报文
	} else if ((Timer_TxMsg % 10) == 6) {
		Send_JYF();//发送给喷轨减压阀
	}


	//200ms
	if((Timer_TxMsg % 20) == 7){
		Send_6KWDC();//发送给6KW降压小DC
	}else if((Timer_TxMsg % 20)==8){
		Send_PTC();//发送给PTC
	}else if((Timer_TxMsg % 20)==9){
		Send_JWQ();//发送给节温器
	}else if((Timer_TxMsg % 20)==6){
//		REN_CAN_Send(ID_Send_CVM1,CAN0_TX_MAILBOX0,1,0,0,0,0,0,0,0); //CVM报文数量控制地址
	}
}










