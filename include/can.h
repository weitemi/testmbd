/*
 * can.h
 *
 *  Created on: 2022��9��7��
 *      Author: Administrator
 */

#ifndef CAN_H_
#define CAN_H_

#include "Cpu.h"
#include "Std_Types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "S32K144.h"


/*                        ���͸���ѹ������                     */
/*************************************************************/
extern uint8_t S_JYF_Data[8];//Ĭ�Ϸ��͹رն�ֵģʽ

uint8_t R_REL_Data[8];//��ż̵������������
uint8_t R_GBD_Data[8];//��Ź��ϱ궨������
uint8_t R_CBD_Data[8];//��Ų����궨������

uint8_t Flag_6S;

#define TX_MB_CODE_TRANSMIT            0xC

typedef struct CAN_struct_tag{
	can_message_t MB[32];
	can_buff_config_t CF[32];
} CAN_tag;


typedef enum {
  CAN_NOT_OK=0,
  CAN_OK,
  CAN_BUSY
} CAN_Return;


extern void ec_buffer1_init(uint8 BufNum,uint8 IdType,uint32 Id,uint8 type);
extern void ec_buffer2_init(uint8 BufNum,uint8 IdType,uint32 Id,uint8 type);


extern CAN_Return ec_can0_receive(uint8 BufNum,uint8 Rx_Buffer[]);
extern CAN_Return ec_can1_receive(uint8 BufNum,uint8 Rx_Buffer[]);
extern CAN_Return ec_can2_receive(uint8 BufNum,uint8 Rx_Buffer[]);

extern CAN_Return ec_can1_transmit(uint8 BufNum,uint8 IdType,uint32 Id, uint8 DataLength,uint8 Tx_Buffer[]);
extern CAN_Return ec_can2_transmit(uint8 BufNum,uint8 IdType,uint32 Id, uint8 DataLength,uint8 Tx_Buffer[]);


#endif /* CAN_H_ */
