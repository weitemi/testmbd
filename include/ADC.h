/*
 * ADC.h
 *
 *  Created on: 2018��8��21��
 *      Author: Administrator
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC_INSTANCE    0UL
#define ADC_INSTANCE1   1UL
#define ADC_CHN12       12U
#define ADC_CHN6        6U
#define ADC_VREFH       5.0f
#define ADC_VREFL       0.0f
uint16_t adcMax;

uint16_t Stack1_H_Pre,Stack1_Air_Pre;//����EHI����������ѹ���ɼ�
uint16_t Stack1_H2_Pre;//1������ѹ��2
uint16_t Stack1_InWater_Pre,Stack1_InWater_DY;//1/�ѽ�ˮѹ��
uint16_t Stack1_RH,Stack1_RH_DY;//�����ʪ��
uint16_t Tank_Pre,Tank_Mid_Pre;//������ѹ����������ѹ��ѹ��
uint8_t FCU_T1_Data,FCU_T2_Data,FCU_T3_Data,FCU_T4_Data;
uint16_t Out_I;//��������ѹ�ɼ�
uint16_t  Sensor_DY,Battery_DY;
uint16_t Flowmeter;//��������
uint32_t FCU_Flowmeter;//��������
uint16_t Honeywell_R1;
uint32_t Honeywell_R2;
uint16_t Honeywell_R3;
uint16_t Honeywell_R4;

uint16_t H1_PPM,H2_PPM;
uint16_t H1_PPM_DY,H2_PPM_DY,H2_PPM_Time;

//����ܹ���
uint32_t Send_Stack_Total_P,Stack_Total_P,Stack_Total_P_LS;//����ܹ��ʵ�λW/H
uint32_t Send_Out_Total_P,Out_Total_P,Out_Total_P_LS;//������ܹ��ʵ�λW/H

void ADC_Ini(void);
void Get_ADC(void);
uint16_t AD_capture(uint8_t chanel);
uint16_t maopao(uint8_t chanel);
#endif /* ADC_H_ */
