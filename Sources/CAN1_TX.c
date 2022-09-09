/*
 * CAN1_TX.c
 *
 *  Created on: 2018��9��13��
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
#include"EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"
#include "Self_Check.h"
#include "Start_Up.h"
#include "Close.h"

uint16_t Send_Tank_Pre=0;
uint8_t S_DPU_Stack_State=0;//���͸�DPU�Ǳ�ĵ��״̬
uint8_t FCU_CANLife=0;//FCU CANLife
uint8_t Down_DCDC_CANLife=0;//��ѹDCDC CANLife
uint8_t S_FCU_Fault_Level=0;//FCU���ϵȼ�
uint8_t S_SJKYJ_VCU_State=0;//��ѹ��״̬
uint8_t S_H_Pump_VCU_State=0;//����ѭ����״̬
uint8_t S_Pump_VCU_State=0;//��ѹˮ��״̬
uint8_t S_Radiator_VCU_State=0;//��ɢ�ȷ���״̬

uint8_t S_DPU_DC_State=0;
uint16_t sys_run_cuont=0;

t_TxMsgCAN1_FCU1 p_TxMsgCAN1_FCU1;
t_TxMsgCAN1_FCU2 p_TxMsgCAN1_FCU2;
t_TxMsgCAN1_FCU3 p_TxMsgCAN1_FCU3;
t_TxMsgCAN1_FCU4 p_TxMsgCAN1_FCU4;
t_TxMsgCAN1_FCU6 p_TxMsgCAN1_FCU6;
t_TxMsgCAN1_FCU7 p_TxMsgCAN1_FCU7;
t_TxMsgCAN1_FCU8 p_TxMsgCAN1_FCU8;
t_TxMsgCAN1_FCU9 p_TxMsgCAN1_FCU9;
t_TxMsgCAN1_FCU10 p_TxMsgCAN1_FCU10;
t_TxMsgCAN1_FCU11 p_TxMsgCAN1_FCU11;
t_TxMsgCAN1_FCU12 p_TxMsgCAN1_FCU12;
t_TxMsgCAN1_FCU13 p_TxMsgCAN1_FCU13;
t_TxMsgCAN1_FCU14 p_TxMsgCAN1_FCU14;


/************************************************************/
/*               ȡ���ֵ����                 */
/************************************************************/
uint8_t Cal_Max_T(void)
{
	uint8_t Check_Data=0;
	if(FCU_T1_Data>Check_Data)Check_Data = FCU_T1_Data;
	if(FCU_T2_Data>Check_Data)Check_Data = FCU_T2_Data;
	if(FCU_T3_Data>Check_Data)Check_Data = FCU_T3_Data;
	if(FCU_T4_Data>Check_Data)Check_Data = FCU_T4_Data;
	return Check_Data;
}

/************************************************************/
/*               ������ת��                 */
/************************************************************/
uint16_t FaultCode(uint16_t code)
{
	uint16_t code_data = 0;
	switch(code)
	{
//		case 20201://���������� 50��
//			code_data = 2001;
//		break;
//		case 20202:
//			code_data = 2002;
//		break;
		case 30201:
			code_data = 2003;
		break;
		case 30202:
			code_data = 2004;
		break;
		case 30203:
			code_data = 2005;
		break;
		case 30204:
			code_data = 2006;
		break;
		case 40201:
			code_data = 2007;
		break;
		case 40202:
			code_data = 2008;
		break;
		case 40203:
			code_data = 2009;
		break;
		case 40204:
			code_data = 2010;
		break;
//		case 20301:// ��ѹ������ 50��
//			code_data = 2051;
//		break;
		case 30301:
			code_data = 2052;
		break;
		case 40301:
			code_data = 2053;
		break;
		case 40302:
			code_data = 2054;
		break;
		case 40303:
			code_data = 2055;
		break;
//		case 20401:// DCDC���� 50��
//			code_data = 2101;
//		break;
//		case 30401:// DCDC����
//			code_data = 2102;
//		break;
		case 40401:// DCDC����
			code_data = 2103;
		break;
		case 40402:// DCDC����
			code_data = 2104;
		break;
		case 40403:// DCDC����
			code_data = 2105;
		break;
		case 31201:// DCL����
			code_data = 2106;
		break;
		case 41201:// DCL����
			code_data = 2107;
		break;
//		case 20501:// ����ˮ�ù��� 50��
//			code_data = 2151;
//		break;
		case 30501:// ����ˮ�ù���
			code_data = 2152;
		break;
		case 40501:// ����ˮ�ù���
			code_data = 2153;
		break;
		case 40502:// ����ˮ�ù���
			code_data = 2154;
		break;
		case 40503:// ����ˮ�ù���
			code_data = 2155;
		break;
		case 40504:// ����ˮ�ù���
			code_data = 2156;
		break;
//		case 20601:// ������ѹ�� 50��
//			code_data = 2201;
//		break;
		case 30601:// ������ѹ��
			code_data = 2202;
		break;
		case 40601:// ������ѹ��
			code_data = 2203;
		break;
		case 40602:// ������ѹ��
			code_data = 2204;
		break;
//		case 20701:// ����ѭ���� 50��
//			code_data = 2251;
//		break;
		case 30701:// ����ѭ����
			code_data = 2252;
		break;
		case 40701:// ����ѭ����
			code_data = 2253;
		break;
		case 40702:// ����ѭ����
			code_data = 2254;
		break;
//		case 21001:// CVM�ɼ��� 50��
//			code_data = 2301;
//		break;
//		case 21002:// CVM�ɼ���
//			code_data = 2302;
//		break;
//		case 21003:// CVM�ɼ���
//			code_data = 2303;
//		break;
		case 31001:// CVM�ɼ���
			code_data = 2304;
		break;
		case 31002:// CVM�ɼ���
			code_data = 2305;
		break;
		case 31003:// CVM�ɼ���
			code_data = 2306;
		break;
		case 41001:// CVM�ɼ���
			code_data = 2307;
		break;
		case 41002:// CVM�ɼ���
			code_data = 2308;
		break;
		case 41005:// CVM�ɼ���
			code_data = 2309;
		break;
		case 41006:// CVM�ɼ���
			code_data = 2310;
		break;
		case 41007:// CVM�ɼ���
			code_data = 2311;
		break;
		case 40901:// �������� 50��
			code_data = 2351;
		break;
//		case 20101:// ��� 50��
//			code_data = 2401;
//		break;
//		case 20102:// ���
//			code_data = 2402;
//		break;
		case 30101:// ���
			code_data = 2403;
		break;
		case 30102:// ���
			code_data = 2404;
		break;
		case 30103:// ���
			code_data = 2405;
		break;
		case 30104:// ���
			code_data = 2406;
		break;
		case 40101:// ���
			code_data = 2407;
		break;
		case 40102:// ���
			code_data = 2408;
		break;
		case 40103:// ���
			code_data = 2409;
		break;
		case 40104:// ���
			code_data = 2410;
		break;
		case 40105:// ���
			code_data = 2411;
		break;
		case 40106:// ���
			code_data = 2412;
		break;
		case 40107:// ���
			code_data = 2413;
		break;
		case 40108:// ���
			code_data = 2414;
		break;
		case 40109:// ���
			code_data = 2415;
		break;
		case 40110:// ���
			code_data = 2416;
		break;
		case 40111:// ���
			code_data = 2417;
		break;
		case 40112:// ���
			code_data = 2418;
		break;
		case 40113:// ���
			code_data = 2419;
		break;
		case 40114:// ���
			code_data = 2420;
		break;
		case 40115:// ���
			code_data = 2421;
		break;
		case 40116:// ���
			code_data = 2422;
		break;
//		case 40117:// ���
//			code_data = 2423;
//		break;
//		case 40118:// ���
//			code_data = 2424;
//		break;
//		case 21101:// ����Ũ��
//			code_data = 2425;
//		break;
//		case 31101:// ����Ũ��
//			code_data = 2426;
//		break;
//		case 41101:// ����Ũ��
//			code_data = 2427;
//		break;
//		case 41102:// ����Ũ��
//			code_data = 2428;
//		break;
		case 41301:// ��ͨ��
			code_data = 2601;
		break;
	}
	return code_data;
}

// CAN1 ����������
void CAN1_Send_Out2(void) {
	// 100ms
	if ((Timer_TxMsg % 10) == 0) {

		if(((Stack_State&0X10)==0X10)&&(DC_State==13))//�ŵ�
		{
			S_DPU_Stack_State = 0x04;//FCU�ŵ�
		}
		else if((Stack_State&0X10)==0X10)
		{
			S_DPU_Stack_State = 0x05;//FCU�ػ���
		}
		else if((Stack_State&0X0F)==0X0F)//����
		{
			S_DPU_Stack_State = 0x03;//FCU����
		}
		else if((Stack_State&0X0F)==0X07)//����
		{
			S_DPU_Stack_State = 0x02;//FCU����
		}
		else if((Stack_State&0X02)==0X02)//�Լ�ɹ���׼������
		{
			S_DPU_Stack_State = 0x01;//FCU׼������
		}
		else if((Stack_State&0X03)==0X01)//�����Լ�
		{
			S_DPU_Stack_State = 0x00;//FCU�ϵ�
		}

		if ((DC_State & 0X02) == 0X02) //DC����
		{
			S_DPU_DC_State = 0X01;
		} else {
			S_DPU_DC_State = 0X02;
		}
		FCU_CANLife++;
		if (FCU_CANLife > 128) {
			FCU_CANLife = 0;
		}
//		if(Fault_Level_Three!=0||Fault_Level_Two!=0) // FCU 2����3�� ������1��
		if(Fault_Level_Three!=0) // FCU 3�� ������1��
		{
			S_FCU_Fault_Level = 0x1;
		}
		if(Fault_Level_Four!=0)//FCU 4�� ������2��
		{
			S_FCU_Fault_Level = 0x2;
		}
//		if(Fault_Level_Three==0&&Fault_Level_Two==0&&Fault_Level_Four==0)
		if(Fault_Level_Three==0&&Fault_Level_Four==0)
		{
			S_FCU_Fault_Level = 0;
		}

		if(hms_fault_level==4)
		{
			S_FCU_Fault_Level = 0x3;
		}
		else if(hms_fault_level==3||hms_fault_level==2)
		{
			S_FCU_Fault_Level = 0x2;
		}

		p_TxMsgCAN1_FCU1.Bits.CAN1_FCU1_s04_fcu_fault_level = S_FCU_Fault_Level;
//		p_TxMsgCAN1_FCU1.Bits.CAN1_FCU1_s04_fcu_fault_level = hms_fault_level;
		p_TxMsgCAN1_FCU1.Bits.CAN1_FCU1_s24_fcu_main_state = S_DPU_Stack_State;
		p_TxMsgCAN1_FCU1.Bits.CAN1_FCU1_s32_fcu_power =	(uint8_t) (((DC_IN_V * DC_IN_I)*0.8)/1000);
		p_TxMsgCAN1_FCU1.Bits.CAN1_FCU1_s40_fcu_IdleSpeed_power = 15;
		p_TxMsgCAN1_FCU1.Bits.CAN1_FCU1_s48_fcu_allow_power = 115;
		p_TxMsgCAN1_FCU1.Bits.CAN1_FCU1_s56_fcu_can_life = FCU_CANLife;
		REN_CAN1_Send(
		ID_S_CAN1_FCU1, CAN1_TX_MAILBOX0, p_TxMsgCAN1_FCU1.DataFld[0],
				p_TxMsgCAN1_FCU1.DataFld[1], p_TxMsgCAN1_FCU1.DataFld[2],
				p_TxMsgCAN1_FCU1.DataFld[3], p_TxMsgCAN1_FCU1.DataFld[4],
				p_TxMsgCAN1_FCU1.DataFld[5], p_TxMsgCAN1_FCU1.DataFld[6],
				p_TxMsgCAN1_FCU1.DataFld[7]);

	} else if ((Timer_TxMsg % 10) == 1) {
		uint8_t j=0;
	   static uint8_t gu2_i=0,gu3_i=0,gu4_i=0;
	   j=GZ[0]+GZ[1]+GZ[2];

	   if(j>0&&GZ[1]>0) // FCU�������Ϸ�����1��
	   {
		   if(gu3_i<=6&&gu_data3[gu3_i]>0)
		   {
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_H = FaultCode(gu_data3[gu3_i]*256+gu_data3[gu3_i+1])>>8;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_L = FaultCode(gu_data3[gu3_i]*256+gu_data3[gu3_i+1]);
			   gu3_i +=2;
		   }
		   else
		   {
			   gu3_i = 0;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_H = FaultCode(gu_data3[gu3_i]*256+gu_data3[gu3_i+1])>>8;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_L = FaultCode(gu_data3[gu3_i]*256+gu_data3[gu3_i+1]);
		   }
	   }
	   else if(j>0&&GZ[0]>0) // FCUһ�����Ϸ�����1��
	   {
		   if(gu2_i<=6&&gu_data2[gu2_i]>0)
		   {
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_H = FaultCode(gu_data2[gu2_i]*256+gu_data2[gu2_i+1])>>8;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_L = FaultCode(gu_data2[gu2_i]*256+gu_data2[gu2_i+1]);
			   gu2_i+=2;
		   }
		   else
		   {
			   gu2_i = 0;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_H = FaultCode(gu_data2[gu2_i]*256+gu_data2[gu2_i+1])>>8;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_L = FaultCode(gu_data2[gu2_i]*256+gu_data2[gu2_i+1]);
		   }
	   }
	   else
	   {
		   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_H = 0;
		   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s00_fcu_fault1_code_L = 0;
	   }
	   if(j>0&&GZ[2]>0) // ��������
	   {
		   if(gu4_i<=6&&gu_data4[gu4_i]>0)
		   {
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s16_fcu_fault2_code_H = FaultCode(gu_data4[gu4_i]*256+gu_data4[gu4_i+1])>>8;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s16_fcu_fault2_code_L = FaultCode(gu_data4[gu4_i]*256+gu_data4[gu4_i+1]);
			   gu4_i +=2;
		   }
		   else
		   {
			   gu4_i = 0;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s16_fcu_fault2_code_H = FaultCode(gu_data4[gu4_i]*256+gu_data4[gu4_i+1])>>8;
			   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s16_fcu_fault2_code_L = FaultCode(gu_data4[gu4_i]*256+gu_data4[gu4_i+1]);
		   }
	   }
	   else
	   {
		   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s16_fcu_fault2_code_H = 0;
		   p_TxMsgCAN1_FCU3.Bits.CAN1_FCU3_s16_fcu_fault2_code_L = 0;
	   }
	   REN_CAN1_Send(
			   ID_S_CAN1_FCU3,
			   CAN1_TX_MAILBOX0,
			   p_TxMsgCAN1_FCU3.DataFld[0],
			   p_TxMsgCAN1_FCU3.DataFld[1],
			   p_TxMsgCAN1_FCU3.DataFld[2],
			   p_TxMsgCAN1_FCU3.DataFld[3],
			   p_TxMsgCAN1_FCU3.DataFld[4],
			   p_TxMsgCAN1_FCU3.DataFld[5],
			   p_TxMsgCAN1_FCU3.DataFld[6],
			   p_TxMsgCAN1_FCU3.DataFld[7]
				);
	} else if ((Timer_TxMsg % 10) == 2) {

		   REN_CAN1_Send(
				   ID_S_CAN1_FCU2HMS,
				   CAN1_TX_MAILBOX0,
				   (REL_State2&0X10)==0X10, 0, 0, 0, 0, 0, 0, 0); // ����������ڿ���ָ��

	}
    //200ms
	if((Timer_TxMsg % 20) == 0){
		uint16_t temp_FCU_T1_Data = 0;
	   temp_FCU_T1_Data = FCU_T1_Data*10;
	   uint16_t temp_FCU_T4_Data = 0;
	   temp_FCU_T4_Data = FCU_T4_Data*10;
	   p_TxMsgCAN1_FCU6.Bits.CAN1_FCU6_s12_water_inp_temp_H = (uint8_t)(temp_FCU_T1_Data>>4);
	   p_TxMsgCAN1_FCU6.Bits.CAN1_FCU6_s12_water_inp_temp_L = temp_FCU_T1_Data;
	   p_TxMsgCAN1_FCU6.Bits.CAN1_FCU6_s24_water_oup_temp_H = (temp_FCU_T4_Data>>8);
	   p_TxMsgCAN1_FCU6.Bits.CAN1_FCU6_s24_water_oup_temp_L = (uint8_t)temp_FCU_T4_Data;
	   p_TxMsgCAN1_FCU6.Bits.CAN1_FCU6_s56_fcu_h2_temp_num = 3;
	   p_TxMsgCAN1_FCU6.Bits.CAN1_FCU6_s58_main_sink_water_state = 0;//!YeWeiTX;
	   p_TxMsgCAN1_FCU6.Bits.CAN1_FCU6_s59_fcu2dcdc_enable = Up_DCDC_Enable;
	   REN_CAN1_Send(
			   ID_S_CAN1_FCU6,
			   CAN1_TX_MAILBOX1,
			   p_TxMsgCAN1_FCU6.DataFld[0],
			   p_TxMsgCAN1_FCU6.DataFld[1],
			   p_TxMsgCAN1_FCU6.DataFld[2],
			   p_TxMsgCAN1_FCU6.DataFld[3],
			   p_TxMsgCAN1_FCU6.DataFld[4],
			   p_TxMsgCAN1_FCU6.DataFld[5],
			   p_TxMsgCAN1_FCU6.DataFld[6],
			   p_TxMsgCAN1_FCU6.DataFld[7]
										);
	}else if((Timer_TxMsg % 20)==1){
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s00_stack_total_pcs_L          = (uint8_t)484;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s00_stack_total_pcs_H          = 484>>8;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s16_single_pcs_min_vol_L       = ((Stack_Low_Voltage/1000)+3)*1000;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s16_single_pcs_min_vol_H       = ((Stack_Low_Voltage/1000)+3)*1000>>8;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s28_number_min_voltage_L       = Stack_Low_Number;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s28_number_min_voltage_H       = Stack_Low_Number>>4;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s40_single_pcs_max_vol_L       = ((Stack_High_Voltage/1000)+3)*1000;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s40_single_pcs_max_vol_H       = ((Stack_High_Voltage/1000)+3)*1000>>8;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s52_number_max_voltage_L       = Stack_High_Number;
	   p_TxMsgCAN1_FCU13.Bits.CAN1_FCU13_s52_number_max_voltage_H       = Stack_High_Number>>4;
	   REN_CAN1_Send(
			   ID_S_CAN1_FCU13,
			   CAN1_TX_MAILBOX1,
			   p_TxMsgCAN1_FCU13.DataFld[0],
			   p_TxMsgCAN1_FCU13.DataFld[1],
			   p_TxMsgCAN1_FCU13.DataFld[2],
			   p_TxMsgCAN1_FCU13.DataFld[3],
			   p_TxMsgCAN1_FCU13.DataFld[4],
			   p_TxMsgCAN1_FCU13.DataFld[5],
			   p_TxMsgCAN1_FCU13.DataFld[6],
			   p_TxMsgCAN1_FCU13.DataFld[7]
				);

	}else if ((Timer_TxMsg % 20)==2){
		uint8_t up_DCDC_state = 0,up_dc_fault = 0;
	   if(DC_State==0x4) // ����
	   {
		   up_DCDC_state = 0;
	   }
	   else if((DC_State==0x2) || (DC_State==13)) // ����
	   {
		   up_DCDC_state = 0x3;
	   }
	   else if(DC_State==12||DC_State==6) // ����
	   {
		   up_DCDC_state = 0x8;
	   }
	   if((DC_Fault&0XC0)==0XC0)// ��Ӧ�����ع���
	   {
		   up_dc_fault = 3;
	   }
	   else if((DC_Fault&0XC0)==0) // �޹���
	   {
		   up_dc_fault = 0;
	   }
	   else
	   {
		   up_dc_fault = 1;
	   }
	   p_TxMsgCAN1_FCU14.Bits.CAN1_FCU14_s00_up_dcdc_voltage_L         = (uint8_t)(DC_OUT_V*10);
	   p_TxMsgCAN1_FCU14.Bits.CAN1_FCU14_s00_up_dcdc_voltage_H         = (uint8_t)((DC_OUT_V*10)>>8);
	   p_TxMsgCAN1_FCU14.Bits.CAN1_FCU14_s16_up_dcdc_current_L         = (uint8_t)(DC_OUT_I*10);
	   p_TxMsgCAN1_FCU14.Bits.CAN1_FCU14_s16_up_dcdc_current_H         = (uint8_t)((DC_OUT_I*10)>>8);
	   p_TxMsgCAN1_FCU14.Bits.CAN1_FCU14_s32_up_dcdc_temp              = DC_Temp;
	   p_TxMsgCAN1_FCU14.Bits.CAN1_FCU14_s40_up_dcdc_state             = up_DCDC_state;
	   p_TxMsgCAN1_FCU14.Bits.CAN1_FCU13_s44_up_dcdc_fault_level       = up_dc_fault;
	   REN_CAN1_Send(
			   ID_S_CAN1_FCU14,
			   CAN1_TX_MAILBOX1,
			   p_TxMsgCAN1_FCU14.DataFld[0],
			   p_TxMsgCAN1_FCU14.DataFld[1],
			   p_TxMsgCAN1_FCU14.DataFld[2],
			   p_TxMsgCAN1_FCU14.DataFld[3],
			   p_TxMsgCAN1_FCU14.DataFld[4],
			   p_TxMsgCAN1_FCU14.DataFld[5],
			   p_TxMsgCAN1_FCU14.DataFld[6],
			   p_TxMsgCAN1_FCU14.DataFld[7]
				);
	}
	// 1000ms
	if((Timer_TxMsg % 100)==3){

		//��ѹ��״̬�ж�
		if(S_SJKYJ_State == 0x05)
		{
			S_SJKYJ_VCU_State = 0;//ֹͣ
		}
		else if(S_SJKYJ_State == 0x0B)
		{
			S_SJKYJ_VCU_State = 1;//����
		}
		if(SJKYJ_Fualt_Class >= 2)
		{
			S_SJKYJ_VCU_State = 2;//�쳣
		}

		//����ѭ����״̬�ж�
		if(H_Pump_State1 == 0xAA)
		{
			S_H_Pump_VCU_State = 0;//ֹͣ
		}
		else if(H_Pump_State1 == 0x55)
		{
			S_H_Pump_VCU_State = 1;//����
		}
		else if(H_Pump_State1 == 0xBB)
		{
			S_H_Pump_VCU_State = 2;//�쳣
		}

		//��ѹˮ��״̬�ж�
		if(Pump_Speed <= 0)
		{
			S_Pump_VCU_State = 0;//ֹͣ
		}
		else if(SJKYJ_State > 0)
		{
			S_Pump_VCU_State = 1;//����
		}
		else if((Pump_State1!=0)&&((Pump_State2&0XFC)!=0))
		{
			S_Pump_VCU_State = 2;//�쳣
		}

		//��ɢ����״̬�ж�
		if(S_Fan_Speed == 0)
		{
			S_Radiator_VCU_State = 0;//ֹͣ
		}
		else if(S_Fan_Speed >= 15)
		{
			S_Radiator_VCU_State = 1;//����
		}
		else if((FCU_T1_Data >= 120) || (FCU_T4_Data >= 125))
		{
			S_Radiator_VCU_State = 2;//�쳣
		}

	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s00_fcu_stack_vol_H = ((DC_IN_V+1000)*2)>>8;
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s00_fcu_stack_vol_L = (DC_IN_V+1000)*2;
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s16_fcu_stack_crt_H = ((DC_IN_I+1000)*2)>>8;
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s16_fcu_stack_crt_L = (DC_IN_I+1000)*2;
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s32_air_pump_state = S_SJKYJ_VCU_State;
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s34_h2_pump_state = S_H_Pump_VCU_State;
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s36_water_pump_state = S_Pump_VCU_State;
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s38_heat_sink_state = S_Radiator_VCU_State;
	   if((DC_Fault&0XC0)==0X40)
	   {
		   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s40_up_dcdc_fault1_code = DC_Fault_code;
	   }
	   else
	   {
		   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s48_up_dcdc_fault2_code = DC_Fault_code;
	   }
	   p_TxMsgCAN1_FCU2.Bits.CAN1_FCU2_s56_up_dcdc_fault3_code = 0;
	   REN_CAN1_Send(
			   ID_S_CAN1_FCU2,
			   CAN1_TX_MAILBOX0,
			   p_TxMsgCAN1_FCU2.DataFld[0],
			   p_TxMsgCAN1_FCU2.DataFld[1],
			   p_TxMsgCAN1_FCU2.DataFld[2],
			   p_TxMsgCAN1_FCU2.DataFld[3],
			   p_TxMsgCAN1_FCU2.DataFld[4],
			   p_TxMsgCAN1_FCU2.DataFld[5],
			   p_TxMsgCAN1_FCU2.DataFld[6],
			   p_TxMsgCAN1_FCU2.DataFld[7]
				);

	}else if((Timer_TxMsg % 100)==4){
	   static uint16_t run_time=0,hour=0;
	   run_time++;
	   if(run_time>3600){
		   run_time=1;
		   hour++;
	   }
//	   sys_run_cuont = S_RUN_Count_Data;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s00_fcu_run_tiem_H = hour>>8;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s00_fcu_run_tiem_L = hour;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s16_fcu_run_count_H = S_RUN_Count_Data>>8;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s16_fcu_run_count_L = S_RUN_Count_Data;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s32_fcu_total_power_H = Send_Stack_Total_P>>24;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s32_fcu_total_power_MH = Send_Stack_Total_P>>16;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s32_fcu_total_power_ML = Send_Stack_Total_P>>8;
	   p_TxMsgCAN1_FCU4.Bits.CAN1_FCU4_s32_fcu_total_power_L = Send_Stack_Total_P;
	   REN_CAN1_Send(
			   ID_S_CAN1_FCU4,
			   CAN1_TX_MAILBOX0,
			   p_TxMsgCAN1_FCU4.DataFld[0],
			   p_TxMsgCAN1_FCU4.DataFld[1],
			   p_TxMsgCAN1_FCU4.DataFld[2],
			   p_TxMsgCAN1_FCU4.DataFld[3],
			   p_TxMsgCAN1_FCU4.DataFld[4],
			   p_TxMsgCAN1_FCU4.DataFld[5],
			   p_TxMsgCAN1_FCU4.DataFld[6],
			   p_TxMsgCAN1_FCU4.DataFld[7]
				);

		p_TxMsgCAN1_FCU7.Bits.CAN1_FCU7_s00_water_pump_power  = (((Pump_Voltage*Pump_Current)/1000)/0.05)/1000;
		if(S_Radiator_VCU_State==1)
		{
			p_TxMsgCAN1_FCU7.Bits.CAN1_FCU7_s08_heat_sink_power  = (((DCDC_Out_voltage*DCDC_Out_current)-0.39)/0.05)/1000;
		}
		else
		{
			p_TxMsgCAN1_FCU7.Bits.CAN1_FCU7_s08_heat_sink_power  = 0;
		}
		p_TxMsgCAN1_FCU7.Bits.CAN1_FCU7_s16_air_pump_power  = ((SJKYJ_Bus_V*SJKYJ_Bus_I)/1000)/0.2;
		p_TxMsgCAN1_FCU7.Bits.CAN1_FCU7_s24_h2_pump_power   = (H_Pump_Power/0.05)/1000;
		p_TxMsgCAN1_FCU7.Bits.CAN1_FCU7_s32_ptc_power  = ((PTC_In_voltage*PTC_In_current)/0.05)/1000;
		REN_CAN1_Send(
			   ID_S_CAN1_FCU7,
			   CAN1_TX_MAILBOX1,
			   p_TxMsgCAN1_FCU7.DataFld[0],
			   p_TxMsgCAN1_FCU7.DataFld[1],
			   p_TxMsgCAN1_FCU7.DataFld[2],
			   p_TxMsgCAN1_FCU7.DataFld[3],
			   p_TxMsgCAN1_FCU7.DataFld[4],
			   p_TxMsgCAN1_FCU7.DataFld[5],
			   p_TxMsgCAN1_FCU7.DataFld[6],
			   p_TxMsgCAN1_FCU7.DataFld[7]
			);

	}else if((Timer_TxMsg % 100)==5){
	   p_TxMsgCAN1_FCU8.Bits.CAN1_FCU8_s00_air_pump_com_temp = SJKYJ_Temp;
	   p_TxMsgCAN1_FCU8.Bits.CAN1_FCU8_s08_air_pump_temp = SJKYJ_MotorTemp;
	   p_TxMsgCAN1_FCU8.Bits.CAN1_FCU8_s16_air_pump_airin_temp = FCU_T2_Data;
	   p_TxMsgCAN1_FCU8.Bits.CAN1_FCU8_s24_air_pump_vol_L = SJKYJ_Bus_V*10;
	   p_TxMsgCAN1_FCU8.Bits.CAN1_FCU8_s32_air_pump_vol_H = (SJKYJ_Bus_V*10)>>8;
	   p_TxMsgCAN1_FCU8.Bits.CAN1_FCU8_s40_air_pump_cur_L = (SJKYJ_Bus_I+1000)*10;
	   p_TxMsgCAN1_FCU8.Bits.CAN1_FCU8_s48_air_pump_cur_H = ((SJKYJ_Bus_I+1000)*10)>>8;
	   REN_CAN1_Send(
			   ID_S_CAN1_FCU8,
			   CAN1_TX_MAILBOX0,
			   p_TxMsgCAN1_FCU8.DataFld[0],
			   p_TxMsgCAN1_FCU8.DataFld[1],
			   p_TxMsgCAN1_FCU8.DataFld[2],
			   p_TxMsgCAN1_FCU8.DataFld[3],
			   p_TxMsgCAN1_FCU8.DataFld[4],
			   p_TxMsgCAN1_FCU8.DataFld[5],
			   p_TxMsgCAN1_FCU8.DataFld[6],
			   p_TxMsgCAN1_FCU8.DataFld[7]
			);
	   REN_CAN1_Send(
	   				ID_S_CAN1_FCU9,
					CAN1_TX_MAILBOX1,
	   			   0X15, // ��
	   			   0X0B, // ��
	   			   0X04, // ��
	   			   0X05, // �汾��
	   			   0X03, // 3-��ʱ��
	   			   0X00, // �������
	   			   0,
	   			   0
	   			   );
	}else if((Timer_TxMsg % 100)==6){
		uint8_t dc_state = 0;
		if((DCDC_work_state&0x01)==0x01||(DCDC_work_state&0x02)==0x02){ // ����л��߳�����
			dc_state = 1; // ����
		}else
		{
			dc_state = 0; // ����
		}
		if(DCDC_fault_code!=0)
		{
			dc_state = 2; // ����
		}
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s00_down_dcdc_state = dc_state;
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s08_down_dcdc_out_vol_H = ((DCDC_Out_voltage*10)>>8);
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s08_down_dcdc_out_vol_L = DCDC_Out_voltage*10;
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s24_down_dcdc_out_crt_H = ((DCDC_Out_current*10)>>8);
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s24_down_dcdc_out_crt_L = DCDC_Out_current*10;
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s40_down_dcdc_in_vol_H = ((DC_OUT_V*10)>>8);
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s40_down_dcdc_in_vol_L = DC_OUT_V*10;
		p_TxMsgCAN1_FCU10.Bits.CAN1_FCU10_s56_down_dcdc_temp = DCDC_temp+40;
		REN_CAN1_Send(
			   ID_S_CAN1_FCU10,
			   CAN1_TX_MAILBOX0,
			   p_TxMsgCAN1_FCU10.DataFld[0],
			   p_TxMsgCAN1_FCU10.DataFld[1],
			   p_TxMsgCAN1_FCU10.DataFld[2],
			   p_TxMsgCAN1_FCU10.DataFld[3],
			   p_TxMsgCAN1_FCU10.DataFld[4],
			   p_TxMsgCAN1_FCU10.DataFld[5],
			   p_TxMsgCAN1_FCU10.DataFld[6],
			   p_TxMsgCAN1_FCU10.DataFld[7]
			);
		Down_DCDC_CANLife++;
		if(Down_DCDC_CANLife > 15)
		{
			Down_DCDC_CANLife = 0;
		}

		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s00_down_dcdc_fault1  = DCDC_fault_code==0x03;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s01_down_dcdc_fault2  = DCDC_fault_code==0x02;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s02_down_dcdc_fault3  = DCDC_fault_code==0x08;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s03_down_dcdc_fault4  = DCDC_fault_code==0x07;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s04_down_dcdc_fault5  = 0;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s05_down_dcdc_fault6  = 0;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s06_down_dcdc_fault7  = DCDC_fault_code==0x05;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s07_down_dcdc_fault8  = DCDC_fault_code==0x06;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s08_down_dcdc_fault9  = DCDC_fault_code==0x09;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s09_down_dcdc_fault10 = 0;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s10_down_dcdc_fault11 = 0;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s12_down_dcdc_life  = Down_DCDC_CANLife;
		p_TxMsgCAN1_FCU11.Bits.CAN1_FCU11_s16_down_dcdc_soft_version  = 0xA0;
		REN_CAN1_Send(
			   ID_S_CAN1_FCU11,
			   CAN1_TX_MAILBOX1,
			   p_TxMsgCAN1_FCU11.DataFld[0],
			   p_TxMsgCAN1_FCU11.DataFld[1],
			   p_TxMsgCAN1_FCU11.DataFld[2],
			   p_TxMsgCAN1_FCU11.DataFld[3],
			   p_TxMsgCAN1_FCU11.DataFld[4],
			   p_TxMsgCAN1_FCU11.DataFld[5],
			   p_TxMsgCAN1_FCU11.DataFld[6],
			   p_TxMsgCAN1_FCU11.DataFld[7]
			);
	}else if((Timer_TxMsg % 100)==7){
		p_TxMsgCAN1_FCU12.Bits.CAN1_FCU12_s32_down_h2_pump_vol_L  = H_Pump_Voltage*10;
		p_TxMsgCAN1_FCU12.Bits.CAN1_FCU12_s40_down_h2_pump_vol_H  = (H_Pump_Voltage*10)>>8;
		p_TxMsgCAN1_FCU12.Bits.CAN1_FCU12_s48_down_h2_pump_crt_L  = (H_Pump_Power/H_Pump_Voltage+1000)*10;
		p_TxMsgCAN1_FCU12.Bits.CAN1_FCU12_s56_down_h2_pump_crt_H  = ((H_Pump_Power/H_Pump_Voltage+1000)*10)>>8;
		REN_CAN1_Send(
			   ID_S_CAN1_FCU12,
			   CAN1_TX_MAILBOX1,
			   p_TxMsgCAN1_FCU12.DataFld[0],
			   p_TxMsgCAN1_FCU12.DataFld[1],
			   p_TxMsgCAN1_FCU12.DataFld[2],
			   p_TxMsgCAN1_FCU12.DataFld[3],
			   p_TxMsgCAN1_FCU12.DataFld[4],
			   p_TxMsgCAN1_FCU12.DataFld[5],
			   p_TxMsgCAN1_FCU12.DataFld[6],
			   p_TxMsgCAN1_FCU12.DataFld[7]
			);
	}

	Timer_TxMsg++;
}





