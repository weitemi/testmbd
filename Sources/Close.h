/*
 * Close.h
 *
 *  Created on: 2018年9月10日
 *      Author: Administrator
 */

#ifndef CLOSE_H_
#define CLOSE_H_
uint8_t time111;
uint8_t In_DC_Flag;
uint8_t closed_flag;//关机动作完成标志位
uint8_t DC_Start_Flag;
uint8_t Normal_Close_Flag;
uint16_t Normal_Close_Time;//关机计数
uint8_t LowTemp_FAN_Flag;//低温风扇运行标志位
void Normal_Shutdown(void);//正常下电
void Standby(void);//待机
void Emergency_Shutdown(void);//急停
#endif /* CLOSE_H_ */
