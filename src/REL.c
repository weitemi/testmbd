/*
 * REL.c
 *
 *  Created on: 2018年8月27日
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
#include "Start_Up.h"
#include "Data_Hand.h"
#include <stdint.h>
#include <stdbool.h>

//继电器状态
uint8_t REL_State1=0,REL_State2=0,REL_State3=0,REL_State4=0;
//继电器默认全部为自动控制
//uint8_t REL_Auto[15]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; //1为手动控制 0位自动控制
uint8_t REL_Auto[15]={0,0,0,0,0,0,0,0,0,0,0,1,0,0,0}; // 0位自动控制 空压机设置为手动控制，因为一打开自动控制他就开始转动了
uint8_t S_Flag=0,S_fan_Open=0;

/************************************************************/
/*               处理上位机发来的继电器命令                 */
/************************************************************/
void hand_Computer_REL(void)
{
  uint8_t i=0;
  if(R_REL_Data[0]==0X01)//发送手动控制才执行否则就是自动控制
  {
   REL_Auto[R_REL_Data[1]] = 1;//将该项的项目编号置位手动控制
   if((R_REL_Data[2])==0X04)//单独某项复位，设置为自动控制
   {
	REL_Auto[R_REL_Data[1]] = 0;
   }
   else
   {
	 REL_Auto[R_REL_Data[1]] = 0;//先将此设置为自动控制，这样才能进入控制
     REL_KG((R_REL_Data[1]),(R_REL_Data[2]));
     REL_Auto[R_REL_Data[1]] = 1;//使用完之后恢复为手动控制
   }
   switch(R_REL_Data[1])
   {
     case 1:
    	 if(REL_Auto[1]==1)//手动控制
    		 REL_State1|=0X80;//
    	 else REL_State1&=0X7F;
    break;
     case 2:
    	 if(REL_Auto[2]==1)
    		 REL_State1|=0X20;
    	 else REL_State1&=0XDF;
    break;
     case 3:
    	 if(REL_Auto[3]==1)
    		 REL_State1|=0X08;
    	 else REL_State1&=0XF7;
    break;
     case 4:
    	 if(REL_Auto[4]==1)
    	 {
    		 REL_State1|=0X02;
    		 Out_H_Flag=1;
    	 }
    	 else
		 {
			 REL_State1&=0XFD;
			 Out_H_Flag=0;
		 }
    break;
     case 5:
    	 if(REL_Auto[5]==1)
    		 REL_State2|=0X80;
    	 else REL_State2&=0X7F;
    break;
     case 6:
    	 if(REL_Auto[6]==1)
    		 REL_State2|=0X20;
    	 else REL_State2&=0XDF;
    break;
     case 7:
    	 if(REL_Auto[7]==1)
    	 {
    		 REL_State2|=0X08;
    	 }
    	 else
         {
    		 REL_State2&=0XF7;
         }
    break;
     case 8:
    	 if(REL_Auto[8]==1)
    		 REL_State2|=0X02;
    	 else REL_State2&=0XFD;
    break;
     case 9:
    	 if(REL_Auto[9]==1)
    		 REL_State3|=0X80;
    	 else REL_State3&=0X7F;
    break;
     case 10:
    	 if(REL_Auto[10]==1)
    		 REL_State3|=0X20;
    	 else REL_State3&=0XDF;
    break;
     case 11:
    	 if(REL_Auto[11]==1)//空压机转速
    	 {
      	   REL_State3|=0X08;
    	 }
    	 else REL_State3&=0XF7;
    break;
     case 12:
    	 if(REL_Auto[12]==1)//风扇调速
    	 {
      	   REL_State3|=0X04;
      	   Fan_Speed_Data = R_REL_Data[5];
    	 }
    	 else REL_State3&=0XFB;
    break;
   }
//
  }
  else if(R_REL_Data[0]==0X02)//自动控制 全部复位
  {
   for(i=0;i<15;i++)
   {
	   REL_Auto[i]=0;
   }
   REL_State1&=0X55;
   REL_State2&=0X55;
   REL_State3&=0X55;
  }
}
/************************************************************/
/*                      发送控制继电器命令                                  */
/************************************************************/
void REL_KG(uint8_t a,uint8_t b)
{
	b = b-1;
    switch(a)
    {
      case 1: //瓶口电磁阀
	   if(REL_Auto[1]==1)
       return;
	   else
	   {
		  REL_State1&=0XBF;
		  if(b==1) //打开
		  {
		   PINS_DRV_SetPins(GPIO_PTB,1<<12);
		   REL_State1|=0X40;
		  }
		  else
		  {
		   PINS_DRV_ClearPins(GPIO_PTB,1<<12);
		  }
	   }
      break;
      case 2: //电堆水泵
	   if(REL_Auto[2]==1)
	   return;
	   else
	   {
		  REL_State1&=0XEF;
		  if(b==1) //打开
		  {
		   PINS_DRV_SetPins(GPIO_PTD,1<<4);
		   REL_State1|=0X10;
		  }
		  else
		  {
		   PINS_DRV_ClearPins(GPIO_PTD,1<<4);
		  }
	   }

	 break;
      case 3: //排氢电磁阀加热
	   if(REL_Auto[3]==1)
	   return;
	   else
	   {
		  REL_State1&=0XFB;
		  if(b==1) //打开
		  {
		   PINS_DRV_SetPins(GPIO_PTE,1<<10);
		   REL_State1|=0X04;
		  }
		  else
		  {
		   PINS_DRV_ClearPins(GPIO_PTE,1<<10);
		  }
	   }
	 break;
      case 4: //氢气出口1
     	   if(REL_Auto[4]==1)  //如果使用手动控制权，关闭自动排氢
     	   return;
     	   else
     	   {
     		  REL_State1&=0XFE;
     		  if(b==1) //打开
     		  {
     		   PINS_DRV_SetPins(GPIO_PTE,1<<11);
     		   REL_State1|=0X01;
     		  }
     		  else
     		  {
     		   PINS_DRV_ClearPins(GPIO_PTE,1<<11);
     		  }
     	   }
     	 break;
      case 5: //部件电源
     	   if(REL_Auto[5]==1)
     	   return;
     	   else
     	   {
     		  REL_State2&=0XBF;
     		  if(b==1) //打开
     		  {
     		   PINS_DRV_SetPins(GPIO_PTD,1<<0);
     		   REL_State2|=0X40;
     		  }
     		  else
     		  {
     		   PINS_DRV_ClearPins(GPIO_PTD,1<<0);
     		  }
     	   }
     	 break;
      case 6: //氢气入口
     	   if(REL_Auto[6]==1)
     	   return;
     	   else
     	   {
     		  REL_State2&=0XEF;
     		  if(b==1) //打开
     		  {
     		   PINS_DRV_SetPins(GPIO_PTD,1<<1);
     		   REL_State2|=0X10;
     		   S_JYF_Data[0]&=0X3F;
     		   S_JYF_Data[0]|=0X80;//发送给减压阀控制器开启状态
     		  }
     		  else
     		  {
     		   PINS_DRV_ClearPins(GPIO_PTD,1<<1);
     		   S_JYF_Data[0]&=0X3F;
     		   S_JYF_Data[0]|=0X40; //发送给关闭状态
     		  }
     	   }
     	 break;
      case 7: //氢气出口2
		   if(REL_Auto[7]==1)
		   return;
		   else
		   {
			  REL_State2&=0XFB;
			  if(b==1) //打开
			  {
			   PINS_DRV_SetPins(GPIO_PTE,1<<1);
			   REL_State2|=0X04;
			  }
			  else
			  {
			   PINS_DRV_ClearPins(GPIO_PTE,1<<1);
			  }
		   }
         break;
      case 8: //放电电阻
		   if(REL_Auto[8]==1)
		   return;
		   else
		   {
			   REL_State2&=0XFE;
//			  if(b==1) //打开
//			  {
//			   PINS_DRV_SetPins(GPIO_PTA,1<<11);
//			   REL_State2|=0X01;
//			  }
//			  else
//			  {
//			   PINS_DRV_ClearPins(GPIO_PTA,1<<11);
//			  }
		   }
         break;
      case 9: //排氢电磁阀加热
		   if(REL_Auto[9]==1)
		   return;
		   else
		   {
			  REL_State3&=0XBF;
			  if(b==1) //打开
			  {
			   PINS_DRV_SetPins(GPIO_PTE,1<<10);
	           REL_State3|=0X40;
			  }
			  else
			  {
			   PINS_DRV_ClearPins(GPIO_PTE,1<<10);
			  }
		   }


         break;
      case 10: //预留 MOS9
		   if(REL_Auto[10]==1)
		   return;
		   else
		   {
			  REL_State3&=0XEF;
			  Send_Stack_Total_P=0,Stack_Total_P=0,Stack_Total_P_LS=0;//电堆总功率单位W/H
			  Send_Out_Total_P=0,Out_Total_P=0,Out_Total_P_LS=0;//净输出总功率单位W/H
		   }
         break;
    }
}








