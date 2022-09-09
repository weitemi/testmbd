/*
 * CAN1_RX.h
 *
 *  Created on: 2018��9��13��
 *      Author: Administrator
 */

#ifndef CAN1_RX_H_
#define CAN1_RX_H_

uint8_t Soc_Data;//SOCֵ
uint16_t Battery_H_Voltage;//������ߵ�ѹ
uint16_t Battery_L_Voltage;//������͵�ѹ
uint16_t Battery_T_Voltage;//����ܵ�ѹ
uint8_t bms_main_key1;//VCU Ready״̬
uint8_t VCU_Ready_State;//VCU Ready״̬
uint8_t full_power_enable;//VCU ����������
uint8_t vcu2fcu_command;//VCU����FCUָ��
uint8_t crash_state;//VCU������ײ״̬
uint8_t power_model;//VCU���ʹ���ģʽ ȼ��򴿵�
uint8_t add_h2_door_state;//VCU���ͼ������״̬
uint8_t urgent_switch_state;//VCU ��ͣ����״̬
uint8_t vcu2fcu_limit_power;//VCU �������ƹ���
uint8_t vcu2fcu_can_life;//VCU ����
uint8_t Car_Speed;//����
uint16_t Res_Data1,Res_Data2;//��Ե��ֵ
uint16_t Battery_T_I;//����ܵ���
uint16_t Bms_Max_Recharge_I;//�����������

uint16_t SDC_Out_V;//СDC�����ѹ
uint16_t SDC_Out_I;//СDC�������
uint8_t hms_fault_level,hms_fault_code;//hms���ϵȼ�
uint8_t hms_state;//hms ״̬
uint8_t hms_h2_soc;//hms SOC

uint16_t hms_h2_pressure;// ����ѹ��
uint32_t vehicle_mileage;// �������
uint8_t Low_Power_Flag;//�͹������б��


void CAN1_Receive(void);

#endif /* CAN1_RX_H_ */
