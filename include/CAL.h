/*
 * CAL.h
 *
 *  Created on: 2018��9��5��
 *      Author: Administrator
 */

#ifndef CAL_H_
#define CAL_H_


uint8_t CAL_Flag;

uint8_t CAL_Data[256];
uint8_t CBD_Read_Flag;//������ȡ��־λ
uint8_t GBD_Read_Flag;//���϶�ȡ��־λ
uint16_t S_RUN_Count_Data;
void CAL_Read_Data(void);
void CAL_C_Data(void);
void CAL_G_Data(void);

#endif /* CAL_H_ */
