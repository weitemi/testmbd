/*
 * SYS.c
 *
 *  Created on: 2018年8月20日
 *      Author: Administrator
 */
#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "SYS.h"
void SYS_Ini(void)
{
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
}

void GPIOInit(void)
{
    /* Output direction for LEDs */
//    PINS_DRV_SetPinsDirection(GPIO_PTD, (1 << D_Tank_valve) | (1 << D_Stack_Pump) | (1 << D_Part_Power) | (1 << D_MOS5));
//    PINS_DRV_SetPinsDirection(GPIO_PTD, ~(1 << D_IN2) | (1 << D_IN_ON));
//
//    PINS_DRV_SetPinsDirection(GPIO_PTE, (1 << E_H_Out) | (1 << E_SYS_Power)| (1 << E_Stack_Fan)
//    		| (1 << E_MOS6)| (1 << E_MOS8)| (1 << E_MOS9));
//    PINS_DRV_SetPinsDirection(GPIO_PTE, ~(1 << E_IN0) | (1 << E_IN1)| (1 << E_IN_CHG));



    /* Set Output value LEDs */
    PINS_DRV_ClearPins(GPIO_PTD, (1 << 0)|(1 << 1)|(1 << 4)|(1 << 5));//MOS4 MOS5 MOS1
    PINS_DRV_ClearPins(GPIO_PTE, (1 << 10)|(1 << 11)|(1 << 1)|(1 << 2)|(1 << 6)); //MOS2 MOS3 MOS6 MOS8 MOS9
    PINS_DRV_ClearPins(GPIO_PTA, 1 << 11);//MOS7
    PINS_DRV_ClearPins(GPIO_PTB, 1 << 12);//MOS0 瓶口阀
//    PINS_DRV_SetPins(GPIO_PTE,1<<E_SYS_Power);//打开自唤醒


}
