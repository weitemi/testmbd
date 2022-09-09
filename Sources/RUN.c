/*
 * RUN.c
 *
 *  Created on: 2018��11��22��
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
	static uint8_t Solenoid_flag = 0; // ���ⷧ���ر��

	S_DC_State=0X0B;//ʹ��DC

	if((FCU_T2_Data <= 55)&&(Solenoid_flag == 0)&&(LowTemp_Close_Flag == 1))
	{
		REL_KG(Solenoid_Heat,0X02);//�������ŷ�����
		Solenoid_flag = 1;
	}
	else if((FCU_T2_Data > 85)&&(Solenoid_flag == 1))
	{
		REL_KG(Solenoid_Heat,0X01);//�ر������ŷ�����
		Solenoid_flag = 0;
	}

}
