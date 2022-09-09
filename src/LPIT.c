/*
 * LPIT.c
 *
 *  Created on: 2018Äê8ÔÂ20ÈÕ
 *      Author: Administrator
 */

#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "LPIT.h"
#include <stdint.h>
#include <stdbool.h>
#include "FCU_F3NEW.h"

uint8_t Flag_5ms=0;
uint8_t Flag_1ms=0;
uint8_t Flag_10ms=0,Flag_20ms=0,Flag_100ms=0;
uint16_t Single_5ms=0,Single_10ms=0,Single_20ms=0,Single_100ms=0;
uint8_t Flag_100us=0;
uint32_t Timer_TxMsg;

void LPIT_ISR(void)
{
	LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL));
	Flag_1ms=1;
	Single_5ms++;
	Single_10ms++;
	Single_20ms++;
	Single_100ms++;
	if(Single_5ms>=5)
	{
		Single_5ms=0;
		Flag_5ms=1;
	}
	if(Single_10ms>=10)
	{
		Flag_10ms=1;
		Single_10ms = 0;
	}
	if(Single_20ms>=20)
	{
		Flag_20ms=1;
		Single_20ms = 0;
	}
	if(Single_100ms>=100)
	{
		Flag_100ms=1;
		Single_100ms = 0;
	}
}

void LPIT_Ini(void)
{
	INT_SYS_InstallHandler(LPIT0_Ch0_IRQn,&LPIT_ISR,(isr_t *)0);
	LPIT_DRV_Init(INST_LPIT1, &lpit1_InitConfig);
    LPIT_DRV_InitChannel(INST_LPIT1, LPIT_CHANNEL, &lpit1_ChnConfig0);
    LPIT_DRV_StartTimerChannels(INST_LPIT1,1<<0);

    INT_SYS_InstallHandler(LPIT0_Ch2_IRQn,&ISR_PIT_CH3,(isr_t *)0);
	LPIT_DRV_InitChannel(INST_LPIT1, LPIT_CHANNEL2, &lpit1_ChnConfig2);
	LPIT_DRV_StartTimerChannels(INST_LPIT1,1<<2);
}








