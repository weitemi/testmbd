/*
 * LPIT.h
 *
 *  Created on: 2018Äê8ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef LPIT_H_
#define LPIT_H_

#define LPIT_CHANNEL	    0UL
#define LPIT_CHANNEL2	    2UL
#define LPIT_Channel_IRQn   LPIT0_Ch0_IRQn

extern uint8_t Flag_1ms;
extern uint8_t Flag_5ms,Flag_10ms,Flag_20ms,Flag_100ms;
extern uint16_t Single_10ms,Single_20ms,Single_100ms;
extern uint8_t Flag_100us;
extern uint32_t Timer_TxMsg;
extern void LPIT_Ini(void);
#endif /* LPIT_H_ */
