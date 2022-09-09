/*
 * ADC.h
 *
 *  Created on: 2018年8月21日
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

uint16_t Stack1_H_Pre,Stack1_Air_Pre;//接收EHI喷轨控制器的压力采集
uint16_t Stack1_H2_Pre;//1堆氢气压力2
uint16_t Stack1_InWater_Pre,Stack1_InWater_DY;//1/堆进水压力
uint16_t Stack1_RH,Stack1_RH_DY;//电堆内湿度
uint16_t Tank_Pre,Tank_Mid_Pre;//气罐总压力跟二级减压阀压力
uint8_t FCU_T1_Data,FCU_T2_Data,FCU_T3_Data,FCU_T4_Data;
uint16_t Out_I;//电流环电压采集
uint16_t  Sensor_DY,Battery_DY;
uint16_t Flowmeter;//空气流量
uint32_t FCU_Flowmeter;//空气流量
uint16_t Honeywell_R1;
uint32_t Honeywell_R2;
uint16_t Honeywell_R3;
uint16_t Honeywell_R4;

uint16_t H1_PPM,H2_PPM;
uint16_t H1_PPM_DY,H2_PPM_DY,H2_PPM_Time;

//电堆总功率
uint32_t Send_Stack_Total_P,Stack_Total_P,Stack_Total_P_LS;//电堆总功率单位W/H
uint32_t Send_Out_Total_P,Out_Total_P,Out_Total_P_LS;//净输出总功率单位W/H

void ADC_Ini(void);
void Get_ADC(void);
uint16_t AD_capture(uint8_t chanel);
uint16_t maopao(uint8_t chanel);
#endif /* ADC_H_ */
