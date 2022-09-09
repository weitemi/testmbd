/*
 * Fault.c
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
#include "Data_Hand.h"
#include"EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"

uint8_t gu_data2[8]={0,0,0,0,0,0,0,0};   //每级存4个故障码  5代表故障指针
uint8_t gu_data3[8]={0,0,0,0,0,0,0,0};
uint8_t gu_data4[8]={0,0,0,0,0,0,0,0};
uint8_t gu_data5[8]={0,0,0,0,0,0,0,0};
uint8_t GZ[4]={0,0,0,0};  //故障指针
/*************************************************************/
/*                      故障码发送                           */
/*************************************************************/
void fualt(uint16_t i)
{
  uint8_t fualt_flag=0; //故障码是否重复标示符
  int8_t m1=0;

  if(i<30000&&i>=20000)  //故障码分类 2级
  {
    if(GZ[0]<=6)
    {
      for(m1=0;m1<=GZ[0];m1+=2)
      {
        if(i==(gu_data2[m1]*256+gu_data2[m1+1]))
        fualt_flag|=0X01;
      }
      if((fualt_flag&0X01)!=0X01)    //如果等于说明有相同
      {
        gu_data2[GZ[0]]=i/256;
        gu_data2[GZ[0]+1]=i%256;
        GZ[0]+=2;
        fualt_flag&=0XFE;
        EX_EEPROM_C256_write(196,gu_data2,8);
      }
    }
  }
  else if(i<40000&&i>=30000) //故障码分类 3级
  {
    if(GZ[1]<=6)
    {
      for(m1=0;m1<=GZ[1];m1+=2)
      {
        if(i==(gu_data3[m1]*256+gu_data3[m1+1]))
        fualt_flag|=0X02;
      }
      if((fualt_flag&0X02)!=0X02)
      {
        gu_data3[GZ[1]]=i/256;
        gu_data3[GZ[1]+1]=i%256;
        GZ[1]+=2;
        fualt_flag&=0XFD;
        EX_EEPROM_C256_write(204,gu_data3,8);
      }
    }
  }
  else if(i<50000&&i>=40000)   //故障码分类 4.1级
  {
    if(GZ[2]<=6)
    {
      for(m1=0;m1<=GZ[2];m1+=2)
      {
        if(i==(gu_data4[m1]*256+gu_data4[m1+1]))
        fualt_flag|=0X04;
      }
      if((fualt_flag&0X04)!=0X04)
      {
        gu_data4[GZ[2]]=i/256;
        gu_data4[GZ[2]+1]=i%256;
        GZ[2]+=2;
        fualt_flag&=0XFD;
        EX_EEPROM_C256_write(212,gu_data4,8);
      }
    }
  }
//  else if(i<60000&&i>=50000)   //故障码分类 4.2级
  else if(i>=50000)   //故障码分类 4.2级
  {
    if(GZ[3]<=6)
    {
      for(m1=0;m1<=GZ[3];m1+=2)
      {
    	if(i==(gu_data5[m1]*256+gu_data5[m1+1]))
        fualt_flag|=0X08;
      }
      if((fualt_flag&0X08)!=0X08)
      {
        gu_data5[GZ[3]]=i/256;
        gu_data5[GZ[3]+1]=i%256;
        GZ[3]+=2;
        fualt_flag&=0XF7;
        EX_EEPROM_C256_write(220,gu_data5,8);
      }
    }
  }
}

/*************************************************************/
/*                     向上位机发送故障ID                    */
/*************************************************************/
void send_fualt(void)
{
  uint8_t i=0;
  static uint8_t t=0;
  i=GZ[0]+GZ[1]+GZ[2]+GZ[3];
  if(i==0)
  return;
  else
  {
	switch(t)
	{
	   case 0:
		 if(GZ[0]>0)
//		 REN_CAN_Send(ID_GU1,CAN0_TX_MAILBOX2,gu_data2[0],gu_data2[1],gu_data2[2],gu_data2[3],
//			     gu_data2[4],gu_data2[5],gu_data2[6],gu_data2[7]);
		 t=1;
	   break;
	   case 1:
		 if(GZ[1]>0)
//		 REN_CAN_Send(ID_GU2,CAN0_TX_MAILBOX2,gu_data3[0],gu_data3[1],gu_data3[2],gu_data3[3],
//				 gu_data3[4],gu_data3[5],gu_data3[6],gu_data3[7]);
		 t=2;
	   break;
	   case 2:
		 if(GZ[2]>0)
//		 REN_CAN_Send(ID_GU3,CAN0_TX_MAILBOX2,gu_data4[0],gu_data4[1],gu_data4[2],gu_data4[3],
//				 gu_data4[4],gu_data4[5],gu_data4[6],gu_data4[7]);
		 t=3;
	   break;
	   case 3:
		 if(GZ[3]>0)
//		 REN_CAN_Send(ID_GU4,CAN0_TX_MAILBOX2,gu_data5[0],gu_data5[1],gu_data5[2],gu_data5[3],
//				 gu_data5[4],gu_data5[5],gu_data5[6],gu_data5[7]);
		 t=0;
	   break;
	}
  }

}

/*************************************************************/
/*                     擦除读取故障码                       */
/*************************************************************/
void Erase_GU_Data(void)
{
   uint8_t i=0;
   GZ[0]=0;
   GZ[1]=0;
   GZ[2]=0;
   GZ[3]=0;
   for(i=0;i<8;i++)
   {
	  gu_data2[i]=0;
	  gu_data3[i]=0;
	  gu_data4[i]=0;
	  gu_data5[i]=0;
   }
   EX_EEPROM_C256_write(196,gu_data2,8);
   EX_EEPROM_C256_write(204,gu_data3,8);
   EX_EEPROM_C256_write(212,gu_data4,8);
   EX_EEPROM_C256_write(220,gu_data5,8);
}















