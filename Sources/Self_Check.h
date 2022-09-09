/*
 * Self_Check.h
 *
 *  Created on: 2018年9月8日
 *      Author: Administrator
 */

#ifndef SELF_CHECK_H_
#define SELF_CHECK_H_

extern uint8_t Slef_Check_Time;//自检时间
extern uint8_t T_100ms;//自检,启动，关机都用
extern uint8_t Stack_State;//电堆状态
extern uint8_t R_Stack_State;//接受界面控制的电堆状态
extern void Self_Check(void);


#endif /* SELF_CHECK_H_ */
