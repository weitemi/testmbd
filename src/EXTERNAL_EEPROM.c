/*
 * EXTERNAL_EEPROM.c
 *
 *  Created on: 2019Äê10ÔÂ15ÈÕ
 *      Author: Administrator
 */
#include"EXTERNAL_EEPROM.h"

status_t  EX_EEPROM_C256_writeOneByte(uint16_t address,uint8_t* write_buff,uint8_t write_length);
status_t  EX_EEPROM_C256_readOneByte(uint16_t address,uint8_t* read_buff,uint8_t read_length);

flexio_i2c_master_state_t i2cMasterState;
flexio_device_state_t flexIODeviceState;


void EX_EEPROM_Init(void)
{
    /* Init FlexIO device */
   FLEXIO_DRV_InitDevice(INST_FLEXIO_I2C1, &flexIODeviceState);

    /* Initialize FlexIO I2C driver as bus master */
    FLEXIO_I2C_DRV_MasterInit(INST_FLEXIO_I2C1, &flexio_i2c1_MasterConfig0, &i2cMasterState);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EX_EEPROM_C256_write
 * Description   : 1.Flexio_i2c read & write maxsize:13 byets ,so this function deal 8 bytes every time.
 * 				2.The parameter write_length can't over 255;
 * Implements : external_EEPROM write
 *
 *END**************************************************************************/
status_t EX_EEPROM_C256_write(uint16_t address,uint8_t* write_buff,uint32_t write_length)//
{
	status_t ret=STATUS_ERROR;
	uint16_t i;
	uint8_t j=0;
	uint8_t onebyte[8];
	for(i=0;i<write_length;i++)
	{
		 onebyte[j++]=*(write_buff++);
		 if(j%8 == 0 )
		 {
			 ret=EX_EEPROM_C256_writeOneByte(address,onebyte,8);
			 address=address+8;
			 j=0;
		 }
		 else if(i ==write_length-1)
		 {
			 ret=EX_EEPROM_C256_writeOneByte(address,onebyte,i%8+1);
			 return ret;
		 }


	}
	return ret;
}
status_t  EX_EEPROM_C256_writeOneByte(uint16_t address,uint8_t* write_buff,uint8_t write_length)
{
	status_t ret=STATUS_ERROR;
	//uint8_t* buff=(uint8_t*)malloc(sizeof(uint8_t)*( write_length+2));
	uint8_t buff[10];
	uint8_t i;
	buff[0]=address>>8;
	buff[1]=address%256;
	for(i=2;i<write_length+2;i++)
	{
		buff[i]=write_buff[i-2];
	}
	ret=FLEXIO_I2C_DRV_MasterSendDataBlocking(&i2cMasterState, buff,write_length+2, true, OSIF_WAIT_FOREVER);
	 OSIF_TimeDelay(5);
	return ret;
}

status_t EX_EEPROM_C256_read(uint16_t address,uint8_t* read_buff,uint32_t read_length)//flexio_i2c read & write maxsize:11 byets once
{

	status_t ret=STATUS_ERROR;
	uint16_t i;
	uint8_t j=0;
	uint8_t onebyte[8];
	for(i=0;i<read_length;i++)
	{
		 if(j%8 == 0 )
		 {
			 ret=EX_EEPROM_C256_readOneByte(address,onebyte,8);
			 address=address+8;
			 j=0;
		 }
		 if(onebyte[j+1]!=0xFF)
			 read_buff[i]=onebyte[j++];
	}
	return ret;

}
status_t  EX_EEPROM_C256_readOneByte(uint16_t address,uint8_t* read_buff,uint8_t read_length)
{
	status_t ret=STATUS_ERROR;
	uint8_t read_address[2]={0,0};
	read_address[0]=address>>8;
	read_address[1]=address%256;
	FLEXIO_I2C_DRV_MasterSendDataBlocking(&i2cMasterState,read_address, 2, false, OSIF_WAIT_FOREVER);
	OSIF_TimeDelay(5);
	ret = FLEXIO_I2C_DRV_MasterReceiveDataBlocking(&i2cMasterState,read_buff, read_length, true, OSIF_WAIT_FOREVER);
	 OSIF_TimeDelay(5);
	return ret ;
}

