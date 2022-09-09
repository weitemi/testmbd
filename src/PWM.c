/*
 * PWM.c
 *
 *  Created on: 2019Äê9ÔÂ10ÈÕ
 *      Author: Lenovo
 */

#include"PWM.h"
#include"flexTimer_pwm1.h"
void PWM_Init(void)
{
	ftm_state_t ftmStateStruct;
    FTM_DRV_Init(INST_FLEXTIMER_PWM1, &flexTimer_pwm1_InitConfig, &ftmStateStruct);
    /* Initialize FTM PWM combined channels 1 and 0 */
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM1, &flexTimer_pwm1_PwmConfig);
}
