/*
 * CAN1.c
 *
 *  Created on: 2018Äê9ÔÂ4ÈÕ
 *      Author: Administrator
 */

#include "Cpu.h"

#include "clockMan1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN1.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "S32K144.h"

 can_buff_config_t buffCfg1 =  {
     .enableFD = false,
     .enableBRS = true,
     .fdPadding = 0U,
     .idType = CAN_MSG_ID_EXT,
     .isRemote = false
 };

void CAN1_Ini(void)
{
	CAN_Init(INST_CAN_PAL1,&can_pal1_Config0);
    /* Configure TX buffer with index TX_MAILBOX*/
    CAN_ConfigTxBuff(INST_CAN_PAL1, CAN1_TX_MAILBOX0, &buffCfg1);
    CAN_ConfigTxBuff(INST_CAN_PAL1, CAN1_TX_MAILBOX1, &buffCfg1);


    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_BMS2, &buffCfg1, ID_BMS2);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_BMS3, &buffCfg1, ID_BMS3);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_BMS5, &buffCfg1, ID_BMS5);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_ISO, &buffCfg1, ID_ISO);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_VCU1, &buffCfg1, ID_VCU1);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_VCU2, &buffCfg1, ID_VCU2);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_CM, &buffCfg1, ID_CM);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_SDC, &buffCfg1, ID_SDC);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_HMS1, &buffCfg1, ID_HMS1);
    CAN_ConfigRxBuff(INST_CAN_PAL1, RX1_MAILBOX_HMS2, &buffCfg1, ID_HMS2);

    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_BMS2, &recvMsg_BMS2);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_BMS3, &recvMsg_BMS3);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_BMS5, &recvMsg_BMS5);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_ISO, &recvMsg_ISO);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_VCU1, &recvMsg_VCU1);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_VCU2, &recvMsg_VCU2);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_CM, &recvMsg_CM);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_SDC, &recvMsg_SDC);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_HMS1, &recvMsg_HMS1);
    CAN_Receive(INST_CAN_PAL1, RX1_MAILBOX_HMS2, &recvMsg_HMS2);
}

void REN_CAN1_Send(uint32_t ID, uint32_t MAILBOX_ID,uint8_t Data0,uint8_t Data1,uint8_t Data2,uint8_t Data3,
		uint8_t Data4,uint8_t Data5,uint8_t Data6,uint8_t Data7)
{
	TX1_message.cs = 0U;
	TX1_message.id = ID;
	TX1_message.data[0]=Data0;
	TX1_message.data[1]=Data1;
	TX1_message.data[2]=Data2;
	TX1_message.data[3]=Data3;
	TX1_message.data[4]=Data4;
	TX1_message.data[5]=Data5;
	TX1_message.data[6]=Data6;
	TX1_message.data[7]=Data7;
	TX1_message.length = 8U;
    CAN_Send(INST_CAN_PAL1, MAILBOX_ID, &TX1_message);

}


