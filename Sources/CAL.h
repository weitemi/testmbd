/*
 * CAL.h
 *
 *  Created on: 2018年9月5日
 *      Author: Administrator
 */

#ifndef CAL_H_
#define CAL_H_


uint8_t CAL_Flag;

uint8_t CAL_Data[256];
uint8_t CBD_Read_Flag;//参数读取标志位
uint8_t GBD_Read_Flag;//故障读取标志位
uint16_t S_RUN_Count_Data;
void CAL_Read_Data(void);
void CAL_C_Data(void);
void CAL_G_Data(void);

#endif /* CAL_H_ */
