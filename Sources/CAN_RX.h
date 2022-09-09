/*
 * CAN_RX.h
 *
 *  Created on: 2018��8��27��
 *      Author: Administrator
 */

#ifndef CAN_RX_H_
#define CAN_RX_H_


uint8_t R_REL_Data[8];//��ż̵������������
uint8_t R_GBD_Data[8];//��Ź��ϱ궨������
uint8_t R_CBD_Data[8];//��Ų����궨������
uint8_t R_RW_Data[8];//������ֵ�����
uint8_t R_DC_MAN_Data[8];//�����ѹDC�ֶ����Ƶ�����
uint8_t R_JYFBD_Data[8];//��ż�ѹ���궨������
uint8_t R_KQBYF_QXHB_PUMP_Data[8];//���ձ�ѹ��������ѭ���á�����ˮ���ֶ���������


uint8_t R_SJKYJ_Data[8];//���տ�ѹ���ֶ���������
uint8_t R_JWQ_PTC_6KWDC_Data[8];//���ս������ֶ���������
uint8_t ZD_RW;
uint8_t Small_DC_State;//���ս�ѹDC״̬

//����ˮ�õ�����
uint16_t Pump_Speed,Pump_Voltage;//ˮ��ת��
uint8_t Pump_State1,Pump_Current,Pump_State2;//ˮ��״̬1��״̬2
uint8_t ZD_Pump;//��ѯˮ�ñ����Ƿ�֡��־λ
int16_t Pump_Speed_Dif_Val;//Ŀ��ת����ʵ��ת�ٲ�ֵ
//��������ѭ���õ�����
uint16_t H_Pump_Speed;//����ѭ����ת��
uint8_t H_Pump_State1,H_Pump_Power;//����ѭ����״̬1
uint8_t ZD_H_Pump;//��ѯ����ѭ���ñ����Ƿ�֡��־λ
int16_t H_Pump_Speed_Dif_Val;//����ѭ��Ŀ��ת����ʵ��ת�ٲ�ֵ
int16_t H_Pump_Voltage;//�����ѹ
//����ѹ��
uint8_t JYF_State;
uint8_t ZD_JYF;//��ѯ��ѹ�������Ƿ�֡��־λ
//������ѹ������
uint8_t ZD_KQBYF;//��ѯ��ѹ�������Ƿ�֡��־λ
int8_t KQBYF_Dif_Angle;//��ѹ���Ƕ����
uint8_t KQBYF_State1;//��ѹ��״̬
//�Ƽ�͸����ѹ����1����
uint8_t ZD_SJKYJ1;//��ѯ��ѹ������1�Ƿ�֡��־λ
uint8_t SJKYJ_State;//��ѹ���������ǹر�״̬
uint32_t SJKYJ_Speed;//��ѹ��ת��
int16_t SJKYJ_Speed_Dif_Val;//ʵ��ת����Ŀ��ת�ٲ�ֵ
uint8_t SJKYJ_ID1_State;//����1��״̬ 0�������� 1�����쳣
uint16_t SJKYJ_Power;//��ѹ������
uint16_t SJKYJ_Bus_V;//��ѹ��ĸ�ߵ�ѹ
uint8_t SJKYJ_Bus_I;//��ѹ��ĸ�ߵ���
//�Ƽ�͸����ѹ����2����
uint8_t ZD_SJKYJ2;//��ѯ��ѹ������2�Ƿ�֡��־λ
uint8_t SJKYJ_Temp;//��ѹ���������¶�
uint8_t SJKYJ_MotorTemp;//��ѹ������¶�
uint8_t SJKYJ_Fualt_Class;//��ѹ�����ϵȼ�
uint16_t SJKYJ_Fualt_Code3;//��ѹ��3�����ϣ����ɻָ����ϣ�
uint16_t SJKYJ_Fualt_Code2;//��ѹ��2�����ϣ��ɻָ����ϣ�
uint16_t SJKYJ_Fualt_Code1;//��ѹ��1�����ϣ��澯��
//��ѹDCDC����1����
uint8_t ZD_DC1;//DCDC�ı���1��֡��־λ
uint16_t DC_IN_V,DC_IN_I;//�����ѹ����  �����ѹ��λV  ���������λA
uint32_t Stack_P;//����ܹ��� ��λW
uint8_t DC_State,DC_Temp,DC_Fault,DC_Fault_code;//��ѹ DC״̬��DC�¶�,DC���ϵȼ�
//��ѹDCDC����2����
uint8_t ZD_DC2;//DCDC�ı���2��֡��־λ
uint16_t DC_OUT_I,DC_OUT_V;//�����ѹ����   �����ѹ��λV  ���������λ0.1A
uint8_t DC_Rec_State;//DC���ܹ���״̬
uint16_t DC_Rec_I;//DC���ܹ�������
uint8_t DC_ID2_State;//DC����2��״̬ 0�������� 1�����쳣
//CVM1��������
uint8_t ZD_CVM11,ZD_CVM12,ZD_CVM13;
uint32_t Stack11_Average_Voltage,Stack11_L_Voltage,Stack1_Total_Voltage;
uint16_t Stack12_NumberMin,Stack12_VoltageMin;
uint16_t Stack12_NumberMax,Stack12_VoltageMax;
//CVM2��������
uint8_t ZD_CVM21,ZD_CVM22;
uint32_t Stack21_Average_Voltage,Stack61_L_Voltage,Stack2_Total_Voltage;
uint16_t Stack22_NumberMin,Stack22_VoltageMin;
uint16_t Stack22_NumberMax,Stack22_VoltageMax;

uint32_t Stack_Total_Voltage;//����ܵ�ѹ ����1mV
uint16_t Stack_High_Voltage;//��Ƭ����ѹ ����1mV
uint16_t Stack_High_Number;//��Ƭ����ѹ���
uint16_t Stack_Low_Voltage;//��Ƭ��С��ѹ ����1mV
uint16_t Stack_Low_Number;//��Ƭ��С��ѹ���
uint16_t Stack1_Max_V_Dif;//���1��Ƭ���ѹ�� ����1MV
uint16_t Stack2_Max_V_Dif;//���2��Ƭ���ѹ�� ����1MV
uint16_t Stack_Average_Voltage;//��Ƭƽ����ѹ ����1mV
uint8_t CVM1_Flag,CVM2_Flag;//6��ɼ������ı�־λ

//��ѹDCDC
uint16_t DCDC_Out_voltage;
uint16_t DCDC_In_voltage;
uint16_t DCDC_Out_current;
uint8_t DCDC_temp;
uint8_t DCDC_work_state;
uint16_t DCDC_fault_code;
uint16_t DCDC_fault_level;//��ѹDC���ϵȼ�

//��ͨ����������
uint16_t Valve_State;//״̬
uint16_t Valve_fault_code;//������
uint16_t Valve_Open;//������

//PTC
uint16_t PTC_In_voltage;// PTC�����ѹ
uint16_t PTC_In_current;// PTC�������
uint16_t PTC_Fault_Code;// PTC������

//���ϲ�ѯ6����ʱ��־λ
uint8_t Flag_6S;
uint8_t Flag_12S;
uint8_t HProtectFlag_6S;

uint16_t Test_11,Test_12;

void CAN0_Receive(void);//CAN0���ղ������ĺ���
void CAN0_Receive_COM(void);//CAN0������λ�����ĺ���
void ID_Lose_Check(void);//��֡���Ĳ�ѯ

#endif /* CAN_RX_H_ */
