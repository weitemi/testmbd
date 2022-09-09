/*
 * H_GS.c
 *
 *  Created on: 2018年9月6日
 *      Author: Administrator
 */

#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "CAN_TX.h"
#include "CAN_RX.h"
#include "Data_Hand.h"
#include"EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"

 uint8_t H_Flag = 0;
 uint8_t Flag_5S=0;

/*************************************************************/
/*                       氢气传感器  信号处理                                                                                   */
/*************************************************************/












