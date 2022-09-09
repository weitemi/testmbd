/*
 * Fault.h
 *
 *  Created on: 2018年9月6日
 *      Author: Administrator
 */

#ifndef FAULT_H_
#define FAULT_H_
extern uint8_t gu_data2[8];   //每级存4个故障码  5代表故障指针
extern uint8_t gu_data3[8];
extern uint8_t gu_data4[8];
extern uint8_t gu_data5[8];
extern uint8_t GZ[4];
extern void fualt(uint16_t i);
extern void send_fualt(void);//发送故障码报文给上位机
void Erase_GU_Data(void);//擦除故障码
#endif /* FAULT_H_ */
