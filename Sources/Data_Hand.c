/*
 * Data_Hand.c
 *
 *  Created on: 2018年8月28日
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



uint8_t ON_Flag=0,Stop_Flag=0;//ON档标志位 停止标志位
//总体故障等级标志位
uint16_t Fault_Level_Two=0,Fault_Level_Three=0,Fault_Level_Four=0,Fault_Level_Five=0;
//风扇调速
uint8_t S_Fan_Speed = 0;//风扇转速值
uint8_t Fan_Speed_Data=0;//手动调整风扇速度值
//排气时间，排气间隔时间
uint8_t Out_Time=0;
uint16_t Out_Interva=0,Out_Interva_Old=0;
uint8_t Out_H_Flag=1;//排氢标志位  1代表手动排氢 0代表自动排氢 如果设置它大于1则是固定排氢100ms*Out_H_Flag
//CAN0发送给上位机整车的一些状态
uint8_t S_VCU_State=0;//总负，ON档，急停，Ready等信号
uint16_t S_Fan_Speed_Change = 0;//风扇补偿量（根据电堆出口温度限值增加风扇转速）
uint16_t HProtectFlag_10S=0;//上Ready后计时10S

/************************************************************/
/*                         ON档读取                                                                                 */
/************************************************************/
void ON_Read(void)
{
	uint32_t buttonsPressed= PINS_DRV_ReadPins(PTD);//读取PTD的状态
	uint32_t buttonsPressed_E= PINS_DRV_ReadPins(PTE);//读取PTE的状态
	if(buttonsPressed & ((1 << D_IN_ON)))//ON档位低    悬空
	{
		ON_Flag=0;
		S_VCU_State&=0XBF;//发送给上位机ON档开关状态
		Fault_Level_Four=0;//无ON档之后清除四级故障
		if(((Stack_State&0X17)==0X07))//只有在运行中或者启动中的时候才使他或上0X10，需要停机
		{
			Stack_State|=0X10;//关机标志位置起
		}
		else if((Stack_State&0X10)==0)
		{
			HProtectFlag_10S = 0;//氢系统未运行情况下关闭钥匙也要对标志位置零
			PINS_DRV_ClearPins(GPIO_PTE,1<<E_SYS_Power);//关闭自唤醒
//			REL_KG(BMS_ON,0X01);//关闭BMS使能信号
			REL_KG(H_GYH,0X01);//关闭氢气出口2
		}
		Stack_State&=0XFE;//没有ON档之后不允许启动或允许或自检
	}
	else//接入ON档12V之后
	{
		ON_Flag=1;
		Stack_State|=0x01;//设置可以自检标志位
//		REL_KG(BMS_ON,0X02);//打开BMS使能信号
		REL_KG(Part_Power,0X02);//打开部件电源
//		REL_KG(H_GYH,0X02);//打开氢燃料系统高压盒
		PINS_DRV_SetPins(GPIO_PTE,1<<E_SYS_Power);//有ON档就打开自唤醒
		S_VCU_State|=0X40;//发送给上位机ON档开关状态

		HProtectFlag_10S++;
		if(HProtectFlag_10S >= 500)
		{
			HProtectFlag_10S = 500;
		}
	}
//	if(buttonsPressed_E & ((1 << 9)))//接入地线 信号线 急停开关
//	{
//		Stop_Flag=1;
//		S_VCU_State|=0X80;//发送给上位机急停开关状态
//	}
//	else//悬空   无信号
//	{
//		Stop_Flag=0;
//		S_VCU_State&=0X7F;//发送给上位机急停开关状态
//	}
	Start_Up_Condition();//启动条件判断
}
//启动条件判断
void Start_Up_Condition(void)
{
	if(VCU_Ready_State==0X04)//整车Ready
	{
		if(Soc_Data<80)//SOC在85以下 可以启动
		{
			Stack_State|=0X04;//可以启动标志位置起
		}
		if(Soc_Data>89)
		{
			if(((Stack_State&0X17)==0X07))//只有在运行中或者启动中的时候才判断有无四级故障，需要停机
			{
				if(DC_Target_I>=100)// 目标电流大于100A
				{
					Normal_Close_Flag = 1; // 降功率到50A运行 延时下电
//					Stack_State|=0X10;//关机标志位置起
				}
				if(DC_Target_I<100&&Normal_Close_Flag==0)
				{
					VCU_Ready_State = 0x03;
				}
			}
			else
			{
				Stack_State&=0XFB;//清楚可以启动标志位置起
			}
		}
	}
	else
	{
		if(((Stack_State&0X17)==0X07))//只有在运行中或者启动中的时候才判断有无四级故障，需要停机
		{
			Stack_State|=0X10;//关机标志位置起
		}
		else
		{
			Stack_State&=0XFB;//清楚可以启动标志位置起
		}
	}
}

/************************************************************/
/*                        电堆温度控制                                                                              */
/************************************************************/
void Temp_Con(void)
{
	static uint8_t TempDifferenceFlag_6S=0;//
	static uint8_t CheckFaultCount=0;//出水温度大于进水温度检测周期

	if(REL_Auto[12]==1)//风扇手动控制
	{
		S_Fan_Speed = Fan_Speed_Data;
	}
	else//风扇自动控制
	{
		if((FCU_T1_Data<=CAL_Data[43])&&(FCU_T4_Data<=CAL_Data[44]))//电堆进口小于65度且出口小于66度
		{
			S_Fan_Speed=0;//转速设置为0
			if(FCU_T4_Data>=FCU_T1_Data)//出水温度高
			{
				if((FCU_T4_Data-FCU_T1_Data)<=(CAL_Data[47]-1))//出水温度与进水温度差值在4度以内
				{
					Fault_Level_Two&=0XFEFF;
					Fault_Level_Three&=0XFBFF;
//					Fault_Level_Four&=0XFBFF;
				}
			}
		}
		//对进口温度控制策略
		if((FCU_T1_Data>=(CAL_Data[41]))&&(LowTemp_FanCon_Flag == 0))//电堆进口大于69度
		{
			S_Fan_Speed=15;
			if(FCU_T1_Data>=(CAL_Data[41]+1))//电堆进口大于70度
			{
				S_Fan_Speed=15;
				if(FCU_T1_Data>=(CAL_Data[41]+2))//电堆进口大于71度
				{
					S_Fan_Speed=25;
					if(FCU_T1_Data>=(CAL_Data[41]+3))//电堆进口大于72度
					{
						S_Fan_Speed=30;
						if(FCU_T1_Data>=(CAL_Data[41]+4))//电堆进口大于73度
						{
							S_Fan_Speed=40;
							if(FCU_T1_Data>=(CAL_Data[41]+5))//电堆进口大于74度
							{
								S_Fan_Speed=50;
								if(FCU_T1_Data>=(CAL_Data[41]+6))//电堆进口大于75度
								{
									S_Fan_Speed=60;
									if(FCU_T1_Data>=(CAL_Data[41]+7))//电堆进口大于76度
									{
										S_Fan_Speed=70;
										if(FCU_T1_Data>=(CAL_Data[41]+8))//电堆进口大于77度
										{
											S_Fan_Speed=80;
											if(FCU_T1_Data>=(CAL_Data[41]+9))//电堆进口大于78度
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
		else if(((FCU_T1_Data>=(CAL_Data[41]+3))||(FCU_T4_Data>=(CAL_Data[42]+3)))&&(LowTemp_FanCon_Flag == 1))//电堆进口大于69度
		{
			S_Fan_Speed=15;
			if((FCU_T1_Data>=(CAL_Data[41]+4))||(FCU_T4_Data>=(CAL_Data[42]+4)))//电堆进口大于70度
			{
				S_Fan_Speed=25;
				if((FCU_T1_Data>=(CAL_Data[41]+5))||(FCU_T4_Data>=(CAL_Data[42]+5)))//电堆进口大于71度
				{
					S_Fan_Speed=35;
					if((FCU_T1_Data>=(CAL_Data[41]+6))||(FCU_T4_Data>=(CAL_Data[42]+6)))//电堆进口大于72度
					{
						S_Fan_Speed=45;
						if((FCU_T1_Data>=(CAL_Data[41]+7))||(FCU_T4_Data>=(CAL_Data[42]+7)))//电堆进口大于73度
						{
							S_Fan_Speed=60;
							if((FCU_T1_Data>=(CAL_Data[41]+8))||(FCU_T4_Data>=(CAL_Data[42]+8)))//电堆进口大于74度
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
		//对电堆出口温度控制策略
		if((FCU_T4_Data>=(CAL_Data[42]))&&(LowTemp_FanCon_Flag == 0))//电堆出口大于76度  低温下电时候不需要进行处理
		{
			S_Fan_Speed_Change = 15;//补偿量  防止风扇处于停止模式时，风扇最低启动15
			if((FCU_T4_Data>=(CAL_Data[42]+2)))//电堆出口大于78度
			{
				S_Fan_Speed_Change = 20;//补偿量
				if((FCU_T4_Data>=(CAL_Data[42]+4)))//电堆出口大于80度
				{
					S_Fan_Speed_Change = 30;//补偿量
				}
			}
		}
		else
		{
			S_Fan_Speed_Change = 0;//补偿量清零
		}

		if(DC_Target_I>300)//高功率下温度在加偏移值
		{
			if(DC_Target_I<350)
			{
				S_Fan_Speed = S_Fan_Speed + S_Fan_Speed_Change + 10;//风扇转速重新计算
			}
			else
			{
				S_Fan_Speed = S_Fan_Speed + S_Fan_Speed_Change + 15;//风扇转速重新计算
			}
		}
		else
		{
			S_Fan_Speed = S_Fan_Speed + S_Fan_Speed_Change;
		}


		if((FCU_T1_Data>=(CAL_Data[41]+6))||(FCU_T4_Data>=(CAL_Data[42]+6)))//电堆进口大于76度或出口大于81度
		{
			Fault_Level_Two|=0X0400;//上报过温二级故障
			fualt(20101);//上报故障码
			if((FCU_T1_Data>=(CAL_Data[41]+8))||(FCU_T4_Data>=(CAL_Data[42]+9)))//电堆进口大于78度或出口大于82度
			{
				Fault_Level_Three|=0X0400;//上报过温三级故障
				fualt(30101);//上报故障码
				if((FCU_T1_Data>=(CAL_Data[41]+9))||(FCU_T4_Data>=(CAL_Data[42]+10)))//电堆进口大于79度或出口大于83度
				{
					Fault_Level_Four|=0X0400;//上报过温四级故障
					fualt(40101);//上报故障码
				}
			}
		}

		if((Stack_State&0X1F)==0X0F)//电堆处于运行状态检测温差
		{
			TempDifferenceFlag_6S++;
			if(TempDifferenceFlag_6S>=60)//系运行后冷却液循环6s后再检测温差
			{
				TempDifferenceFlag_6S = 60;
				if((FCU_T4_Data>=FCU_T1_Data)&&(JWQ_Target_Angle>30))//出水温度高  节温器开启度大于30再判断温差
				{
					if((FCU_T4_Data-FCU_T1_Data)>=(CAL_Data[47]))//出水温度与进水温度差值在6度以内
					{
						CheckFaultCount++;
						if(CheckFaultCount>=15){//持续1500ms
							CheckFaultCount=15;
							Fault_Level_Two|=0X0400;//上报温度二级故障
							fualt(20102);//上报故障码 进出口温差过温
							if((FCU_T4_Data-FCU_T1_Data)>=(CAL_Data[47]+2))//出水温度与进水温度差值在8度以内
							{
								Fault_Level_Three|=0X0400;//上报过温三级故障
								fualt(30102);//上报故障码 进出口温差过温
								if((FCU_T4_Data-FCU_T1_Data)>=(CAL_Data[47]+6))//出水温度与进水温度差值在12度以内
								{
									Fault_Level_Four|=0X0400;//上报过温四级故障
									fualt(40102);//上报故障码 进出口温差过温
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
//	if((DC_Temp>CAL_Data[45]) || (SJKYJ_Temp > 90) ||(SJKYJ_MotorTemp > 150))//升压DC温度大于开启温度  空压机控制器温度大于50  空压机温度大于110
//	{
//		REL_KG(FZ_Fan,0X02);//FZ_Fan在宏定义里面定义一个9  打开风扇
//	}
//	else if((DC_Temp<=CAL_Data[46])&&(SJKYJ_Temp <= 89)&&(SJKYJ_MotorTemp <= 149))//升压DC温度小于开启温度
//	{
//		REL_KG(FZ_Fan,0X01);//FZ_Fan在宏定义里面定义一个9 关闭风扇
//	}
}

/************************************************************/
/*                      氢气压力保护控制                                                                            */
/************************************************************/
void H_Con(void)
{
	if((Stack1_H_Pre<2300)&&(Stack1_H2_Pre<2300))//氢气过压130KPA以下，正常
	{
		if((Stack1_H_Pre>=950)&&(Stack1_H2_Pre>=950))//-5KPA以上
		{
			Fault_Level_Three&=0XF7FF;
//			Fault_Level_Four&=0XF7FF;
		}
	}

	if((Stack1_H_Pre<700)||(Stack1_H2_Pre<700))//-30Kpa
	{
		Fault_Level_Four|=0X0800;
		fualt(40104);//上报故障码
		Out_H_Flag=4;//排氢400ms
	}
	else if((Stack1_H_Pre<800)||(Stack1_H2_Pre<800))//-20Kpa
	{
		Fault_Level_Three|=0X0800;
		fualt(30104);//上报故障码
		Out_H_Flag=4;//排氢400ms
	}
	else if((Stack1_H_Pre<900)||(Stack1_H2_Pre<900))//-10Kpa
	{
		Out_H_Flag=4;//排氢400ms
	}
   //加1000是加上表压值得偏移，精度0.1KPA，1000就是100KPA
	else if((Stack1_H_Pre>=(CAL_Data[177-36]*10+1000))||(Stack1_H2_Pre>=(CAL_Data[177-36]*10+1000)))//四级故障  氢气过压180KPA  精度0.1KPA 绝压 过压了需要排氢并关闭减压阀，关闭瓶口阀
	{
		if(Flag_6S >= 60)
		{
			Fault_Level_Four|=0X0800;
			fualt(40103);//上报故障码
		}
		Out_H_Flag=4;//排氢400ms
	}
	else if((Stack1_H_Pre>=(CAL_Data[145-36]*10+1000))||(Stack1_H2_Pre>=(CAL_Data[145-36]*10+1000)))//三级故障  氢气过压150KPA  精度0.1KPA 绝压 过压了需要排氢并关闭减压阀，关闭瓶口阀
	{
		Fault_Level_Three|=0X0800;
		fualt(30103);//上报故障码
		Out_H_Flag=4;//排氢400ms
	}
	else
	{

	}

}

/*************************************************************/
/*                        排氢函数                                                                                         */
/*************************************************************/
void Out_H(void) //因为是放在100ms标志位，所以要算出多少次100ms即可
{
	static uint8_t HFlag_6S=0;//6秒钟检测一次压差是否过大
	static uint8_t H2_Flag=0,H2_Time=0;//排氢打开标志
	static uint16_t time=0,Long_Out_Time=0;//time为氢气出口  Long_Out_Time长时间排气计时

	Out_Time = CAL_Data[17]/10; //算出有多少次100ms

	if((Stack_State&0X0F)==0X07){ //启动阶段3s排一次
		Out_H_Flag = 0;
		Out_Interva = 30; //启动阶段3s排一次
	}
	else if(DC_IN_I<(CAL_Data[11]-2))//最大电流1
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
	else if(DC_IN_I>=((CAL_Data[14]*2)-2))//最大电流5   250A
	{
		Out_Interva = (CAL_Data[5]*10);
	}
	else if(DC_IN_I>=((CAL_Data[13]*2)-2))//最大电流4   200A
	{
		Out_Interva = (CAL_Data[4]*10);
	}
	else if(DC_IN_I>=(CAL_Data[12]-2))//最大电流3   150A
	{
		Out_Interva = (CAL_Data[3]*10);
	}
	else if(DC_IN_I>=(CAL_Data[11]-2))//最大电流2   100A
	{
		Out_Interva = (CAL_Data[2]*10);
	}


	//防止系统刚上电的时候氢气压力采集瞬间出现异常值（为了屏蔽上电瞬间异常值）   增加HProtectFlag_10S标志原因：整车上电但是系统未运行时，HProtectFlag_6S标志位不会进行计数导致保护函数起不到作用
	if((HProtectFlag_6S>=60) || (HProtectFlag_10S >= 500))
	{
		H_Con();//氢气压力保护函数
	}

	HFlag_6S++;//
	if(HFlag_6S>=60)//6秒钟判断一次压差是否过大（6S秒钟判断一次）
	{
		HFlag_6S=0;
		if((Stack1_Max_V_Dif<2000) || (Stack2_Max_V_Dif<2000))//停机的时候最大压差 是5374假值，所以要屏蔽假值
		{
			if((Stack1_Max_V_Dif>CAL_Data[23]) || (Stack2_Max_V_Dif>CAL_Data[23]))
			{
			  time = Out_Interva-Out_Time-1;
			}
		}
	}
	/*瓶口阀开启后执行自动排氢，关闭瓶口阀后氢气出口常闭      */
	if((REL_State2&0X10)==0X10)//氢气入口开启
	{
		if(Out_H_Flag>1)//长时间排气，排气时间为Out_H_Flag*100ms
		{
			REL_KG(H_Out,0X02);//打开氢气出口
			Long_Out_Time++;
			if(Long_Out_Time>=Out_H_Flag)
			{
				Long_Out_Time=0;
				REL_KG(H_Out,0X01);//关闭氢气出口
				time=0;
				if(REL_Auto[4]==1)//确定一下自动控制是否打开
				{
				   Out_H_Flag=1;
				}
				else
				{
				   Out_H_Flag=0;
				}
			}
		}

		if(Out_H_Flag==0)//自动排氢状态
		{
			time++;//定时排氢计时
			if(time==Out_Interva)//130*100 16*100 每1.6秒打开一次 130-4 126
			{
				REL_KG(H_Out,0X02);//打开氢气出口
				H2_Flag=1;
			}
			else if(time>=Out_Interva+Out_Time) // 24*100 2-1.6
			{
				REL_KG(H_Out,0X01);//关闭氢气出口
				time=0;
				H2_Flag=0;
				H2_Time=0;
			}
			if(H2_Flag){
				H2_Time++;
				if(H2_Time>Out_Time){
					REL_KG(H_Out,0X01);//关闭氢气出口
				}
			}
		}
	}
	else
	{
	   if((Stack1_H_Pre<900)||(Stack1_H2_Pre<900))//-10Kpa
		{
			REL_KG(H_Out,0X02);//打开氢气出口1   下电时氢气压力负压太大，自动排氢无法解决，需要长开
		}
	   else
	   {
		  Out_H_Flag=1;//将氢气出口阀设置成手动控制
		  REL_KG(H_Out,0X01);//关闭氢气出口1
		  REL_KG(H_GYH,0X01);//关闭氢气出口2
	   }

	}
}


/*************************************************************/
/*                          故障判断函数                                       */
/*************************************************************/
void Fault_Check(void)
{
	if(((Stack_State&0X17)==0X07))//只有在运行中或者启动中的时候才判断有无四级故障，需要停机
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
		if(Fault_Level_Four!=0)//有四级故障
		{
		   Stack_State|=0X10;//关机标志位置起
		}
	}
	else
	{
//		Flag_6S=0;
//		Flag_12S=0;
	}
}
/*************************************************************/
/*                          风扇调速函数                                                                           */
/*************************************************************/
void Fan_Speed(void)
{
	static uint8_t i=0;
	if(S_Fan_Speed>90)S_Fan_Speed=90;
	if(i>=S_Fan_Speed)//打开
	{
		//根据不同电流匹配风扇个数
		if((Stack_State&0X1F)==0X0F)//运行
		{
			if(DC_Target_I<=100)
			{
//				PINS_DRV_SetPins(GPIO_PTA,1<<11);//控制2个风扇
				PINS_DRV_SetPins(GPIO_PTE,1<<6);//控制4个风扇
			}
			else if((DC_Target_I>100)&&(DC_Target_I<=200))
			{
				PINS_DRV_SetPins(GPIO_PTE,1<<6);//控制4个风扇
				PINS_DRV_SetPins(GPIO_PTA,1<<11);//控制2个风扇
			}
			else if(DC_Target_I>200)
			{
				PINS_DRV_SetPins(GPIO_PTE,1<<6);//控制4个风扇
				PINS_DRV_SetPins(GPIO_PTA,1<<11);//控制2个风扇
			}
			else
			{

			}
		}
		else
		{
			PINS_DRV_SetPins(GPIO_PTE,1<<6);//控制4个风扇
			PINS_DRV_SetPins(GPIO_PTA,1<<11);//控制2个风扇
		}
	}
	else
	{
		if((Stack_State&0X1F)==0X0F)//运行
		{
			if(DC_Target_I<=100)
			{
//				PINS_DRV_ClearPins(GPIO_PTA,1<<11);//控制2个风扇
				PINS_DRV_ClearPins(GPIO_PTE,1<<6);//控制4个风扇
			}
			else if((DC_Target_I>100)&&(DC_Target_I<=200))
			{
				PINS_DRV_ClearPins(GPIO_PTE,1<<6);//控制4个风扇
				PINS_DRV_ClearPins(GPIO_PTA,1<<11);//控制2个风扇
			}
			else if(DC_Target_I>200)
			{
				PINS_DRV_ClearPins(GPIO_PTE,1<<6);//控制4个风扇
				PINS_DRV_ClearPins(GPIO_PTA,1<<11);//控制2个风扇
			}
			else
			{

			}
		}
		else
		{
			PINS_DRV_ClearPins(GPIO_PTE,1<<6);//控制4个风扇
			PINS_DRV_ClearPins(GPIO_PTA,1<<11);//控制2个风扇
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
/*                         电堆发电量计算                                                                          */
/*************************************************************/
void Stack_P_CAL(void)
{
	Stack_Total_P_LS += DC_IN_V*DC_IN_I;
	 if(Stack_Total_P_LS>=36000000)
	 {
		 Stack_Total_P+=1000;//总功率增加
		 Stack_Total_P_LS-=36000000;
	 }
	 Send_Stack_Total_P = Stack_Total_P+(Stack_Total_P_LS/36000);
}

/*************************************************************/
/*                           液位监控函数                                                                        */
/*************************************************************/
void YeWei_Stack(void)
{
	static uint8_t YeWei_Flag=0;
	uint32_t buttonsPressed_E= PINS_DRV_ReadPins(PTE);//读取PTE的状态
	if(buttonsPressed_E & (1 << YW_IN))
	{
		YeWei_Flag=1;//补偿水壶液位传感器标志位   水位正常
		YeWeiTX = YeWei_Flag;
	}
	else
	{
		YeWei_Flag=0;//补偿水壶液位传感器标志位   水位缺水
		YeWeiTX = YeWei_Flag;
	}
}
