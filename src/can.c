/*
 * can.c
 *
 *  Created on: 2022Äê9ÔÂ7ÈÕ
 *      Author: Administrator
 */



#include "can.h"



CAN_tag CAN_1;
CAN_tag CAN_2;






void ec_buffer1_init(uint8 BufNum,uint8 IdType,uint32 Id,uint8 type)
{

  if (BufNum < 32) {
    if (IdType == 0) {
      CAN_1.MB[BufNum].id = Id;
      CAN_1.CF[BufNum].idType = CAN_MSG_ID_STD;
    } else {
      CAN_1.MB[BufNum].id = (uint32)(Id&0x1FFFFFFF);
      CAN_1.CF[BufNum].idType = CAN_MSG_ID_EXT;
    }
    if(type == 0){
        CAN_ConfigRxBuff(INST_CAN_PAL0, BufNum, &CAN_1.CF[BufNum], CAN_1.MB[BufNum].id);
        CAN_Receive(INST_CAN_PAL0, BufNum, &CAN_1.MB[BufNum]);
    }else{
        CAN_ConfigTxBuff(INST_CAN_PAL0, BufNum, &CAN_1.CF[BufNum]);
    }

  }

}




void ec_buffer2_init(uint8 BufNum,uint8 IdType,uint32 Id,uint8 type)
{

  if (BufNum < 32) {
    if (IdType == 0) {
      CAN_2.MB[BufNum].id = Id;
      CAN_2.CF[BufNum].idType = CAN_MSG_ID_STD;
    } else {
      CAN_2.MB[BufNum].id = (uint32)(Id&0x1FFFFFFF);
      CAN_2.CF[BufNum].idType = CAN_MSG_ID_EXT;
    }

    if(type == 0){
        CAN_ConfigRxBuff(INST_CAN_PAL1, BufNum, &CAN_2.CF[BufNum], CAN_2.MB[BufNum].id);
        CAN_Receive(INST_CAN_PAL1, BufNum, &CAN_2.MB[BufNum]);
    }else{
        CAN_ConfigTxBuff(INST_CAN_PAL1, BufNum, &CAN_2.CF[BufNum]);
    }
  }

}



CAN_Return ec_can1_receive(uint8 BufNum,uint8 Rx_Buffer[])
{
  uint8 DataNum,Code,Length;
  uint16 FrameTime;
  uint32 i;
  CAN_Return result = CAN_NOT_OK;

  if(CAN_GetTransferStatus(INST_CAN_PAL0, BufNum) == STATUS_SUCCESS)
  {
	  for (DataNum=0; DataNum<CAN_1.MB[BufNum].length; DataNum++) {
	    Rx_Buffer[DataNum] = (uint8)CAN_1.MB[BufNum].data[DataNum];
	  }
      CAN_Receive(INST_CAN_PAL0, BufNum, &CAN_1.MB[BufNum]);
	  result = CAN_OK;
  }

  return result;
}


CAN_Return ec_can2_receive(uint8 BufNum,uint8 Rx_Buffer[])
{
  uint8 DataNum,Code,Length;
  uint16 FrameTime;
  uint32 i;
  CAN_Return result = CAN_NOT_OK;

  if(CAN_GetTransferStatus(INST_CAN_PAL1, BufNum) == STATUS_SUCCESS)
  {
	  for (DataNum=0; DataNum<CAN_2.MB[BufNum].length; DataNum++) {
	    Rx_Buffer[DataNum] = (uint8)CAN_2.MB[BufNum].data[DataNum];
	  }
      CAN_Receive(INST_CAN_PAL1, BufNum, &CAN_2.MB[BufNum]);
	  result = CAN_OK;
  }

  return result;
}



CAN_Return ec_can1_transmit(uint8 BufNum,uint8 IdType,uint32 Id,uint8 DataLength,
  uint8 Tx_Buffer[])
{
  uint8 i;
  CAN_Return result = CAN_BUSY;
  if (BufNum>32)
    BufNum = 32;
  else {
//    if (CAN_1.MB[BufNum].cs == TX_MB_CODE_TRANSMIT) {
//      result = CAN_BUSY;
//    } else {
      if (IdType == 0) {
        CAN_1.MB[BufNum].id = Id;
      } else {
        CAN_1.MB[BufNum].id = (uint32)(Id&0x1FFFFFFF);
      }

      for (i=0; i<DataLength; i++) {
        CAN_1.MB[BufNum].data[i] = Tx_Buffer[i];
      }

      CAN_1.MB[BufNum].length = DataLength;
      CAN_1.MB[BufNum].cs = TX_MB_CODE_TRANSMIT;
      result = CAN_OK;

      CAN_Send(INST_CAN_PAL0, BufNum, &CAN_1.MB[BufNum]);
      //ec_can_appltxconfirmation(Module,BufNum);
    }
//  }

  return(result);
}


CAN_Return ec_can2_transmit(uint8 BufNum,uint8 IdType,uint32 Id,uint8 DataLength,
  uint8 Tx_Buffer[])
{
  uint8 i;
  CAN_Return result = CAN_BUSY;
  if (BufNum>16)
    BufNum = 16;
  else {
//    if (CAN_2.MB[BufNum].cs == TX_MB_CODE_TRANSMIT) {
//      result = CAN_BUSY;
//    } else {
      if (IdType == 0) {
        CAN_2.MB[BufNum].id = Id;
      } else {
        CAN_2.MB[BufNum].id = (uint32)(Id&0x1FFFFFFF);
      }

      for (i=0; i<DataLength; i++) {
        CAN_2.MB[BufNum].data[i] = Tx_Buffer[i];
      }

      CAN_2.MB[BufNum].length = DataLength;
      CAN_2.MB[BufNum].cs = TX_MB_CODE_TRANSMIT;
      result = CAN_OK;

      //ec_can_appltxconfirmation(Module,BufNum);
      CAN_Send(INST_CAN_PAL1, BufNum, &CAN_2.MB[BufNum]);
//    }
  }

  return(result);
}


