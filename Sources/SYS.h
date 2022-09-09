/*
 * SYS.h
 *
 *  Created on: 2018年8月20日
 *      Author: Administrator
 */

#ifndef SYS_H_
#define SYS_H_

#define Tank_valve      1U  //气罐瓶口阀
#define Stack_Pump      2U  //电堆水泵
#define Stack_Fan   3U //排氢电磁阀加热
#define H_Out           4U //氢气出口
#define Part_Power      5U  //部件电源
#define H_IN            6U  //氢气入口
#define H_GYH           7U  //预留2
#define BMS_ON          8U  //BMS使能信号
#define Solenoid_Heat   9U  //排氢电磁阀加热
//#define MOS9            10U//DC继电器

#define E_SYS_Power 3U

#define PTD_PORT        PORTD
#define GPIO_PTD        PTD
#define PCC_INDEX_D     PCC_PORTD_INDEX


#define D_IN2             15U
#define D_IN_ON           16U//ON档
#define YW_IN              8U//液位传感器

#define PTE_PORT        PORTE
#define GPIO_PTE        PTE
#define PCC_INDEX_E     PCC_PORTE_INDEX

//#define E_SYS_Power       3U


#define E_IN0             8U
#define E_IN1             9U

#define PTA_PORT        PORTA
#define GPIO_PTA        PTA
#define PCC_INDEX_A     PCC_PORTA_INDEX


#define PTB_PORT        PORTB
#define GPIO_PTB        PTB
#define PCC_INDEX_B     PCC_PORTB_INDEX
#define B_Out_5V          4U

#define B_IN_H            5U


extern void SYS_Ini(void);
extern void GPIOInit(void);
#endif /* SYS_H_ */
