/*
 * EXTERNAL_EEPROM.h
 *
 *  Created on: 2019Äê10ÔÂ15ÈÕ
 *      Author: Administrator
 */

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include"flexio_i2c1.h"
#include"CAN.h"
#include<stdlib.h>

extern void EX_EEPROM_Init(void);
status_t  EX_EEPROM_C256_write(uint16_t address,uint8_t* write_buff,uint32_t buff_length);
status_t  EX_EEPROM_C256_read(uint16_t address,uint8_t* read_buff,uint32_t read_length);

#endif /* EXTERNAL_EEPROM_H_ */
