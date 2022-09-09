/*
 * RUN.c
 *
 *  Created on: 2018年11月22日
 *      Author: Administrator
 */

#include "Cpu.h"
#include "REL.h"
#include "Data_Hand.h"
#include "CAN_TX.h"
#include "Self_Check.h"
#include "RUN.h"
#include "ADC.h"
#include "SYS.h"
#include "CAL.h"
#include "Start_Up.h"

void RUN_State(void)
{
	static uint8_t Solenoid_flag = 0; // 排氢阀开关标记

	S_DC_State=0X0B;//使能DC

	if((FCU_T2_Data <= 55)&&(Solenoid_flag == 0)&&(LowTemp_Close_Flag == 1))
	{
		REL_KG(Solenoid_Heat,0X02);//打开排氢电磁阀加热
		Solenoid_flag = 1;
	}
	else if((FCU_T2_Data > 85)&&(Solenoid_flag == 1))
	{
		REL_KG(Solenoid_Heat,0X01);//关闭排氢电磁阀加热
		Solenoid_flag = 0;
	}

}
