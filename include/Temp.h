/*
 * Temp.h
 *
 *  Created on: 2019Äê1ÔÂ7ÈÕ
 *      Author: Lenovo
 */

#ifndef TEMP_H_
#define TEMP_H_

uint8_t Temp_Rec(uint16_t AD_DY);
uint8_t Temp_Rec1(uint32_t AD_DY);
uint8_t Temp_Rec2(uint16_t AD_DY);
uint8_t FineTab(uint16_t *a,uint16_t data);
uint16_t Flowmeter_data[39][2];
uint16_t Flowmeter_dataLng;
uint16_t Flowmeter_Rec(uint16_t AD_FT);
uint32_t GetLineValue(uint16_t A_array[][2], uint16_t x0, uint16_t length);

#endif /* TEMP_H_ */
