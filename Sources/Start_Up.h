/*
 * Start_Up.h
 *
 *  Created on: 2018年9月8日
 *      Author: Administrator
 */

#ifndef START_UP_H_
#define START_UP_H_


extern uint8_t LowTempStart_Flag;//低温启动标志位
extern uint8_t LowTemp_Close_Flag;//低温关机标志位

extern uint8_t S_U_Flag;
extern void H_Emp(void);
extern void Start_Up(void);
#endif /* START_UP_H_ */
