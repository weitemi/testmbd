/*
 * Close.h
 *
 *  Created on: 2018��9��10��
 *      Author: Administrator
 */

#ifndef CLOSE_H_
#define CLOSE_H_
uint8_t time111;
uint8_t In_DC_Flag;
uint8_t closed_flag;//�ػ�������ɱ�־λ
uint8_t DC_Start_Flag;
uint8_t Normal_Close_Flag;
uint16_t Normal_Close_Time;//�ػ�����
uint8_t LowTemp_FAN_Flag;//���·������б�־λ
void Normal_Shutdown(void);//�����µ�
void Standby(void);//����
void Emergency_Shutdown(void);//��ͣ
#endif /* CLOSE_H_ */
