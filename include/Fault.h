/*
 * Fault.h
 *
 *  Created on: 2018��9��6��
 *      Author: Administrator
 */

#ifndef FAULT_H_
#define FAULT_H_
extern uint8_t gu_data2[8];   //ÿ����4��������  5�������ָ��
extern uint8_t gu_data3[8];
extern uint8_t gu_data4[8];
extern uint8_t gu_data5[8];
extern uint8_t GZ[4];
extern void fualt(uint16_t i);
extern void send_fualt(void);//���͹����뱨�ĸ���λ��
void Erase_GU_Data(void);//����������
#endif /* FAULT_H_ */
