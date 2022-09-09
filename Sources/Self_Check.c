/*
 * Self_Check.c
 *
 *  Created on: 2018年9月8日
 *      Author: Administrator
 */

#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "CAN1.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "CAN_TX.h"
#include "CAN_RX.h"
#include "CAN1_RX.h"
#include "Data_Hand.h"
#include"EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"
#include "Self_Check.h"

uint8_t Slef_Check_Time = 0;//自检时间

uint8_t T_100ms=0;//100ms的间隔时间
uint8_t Stack_State=0;//电堆状态
uint8_t R_Stack_State=0;//接受界面控制的电堆状态
void Self_Check(void)
{
	if(T_100ms)
	  {
		T_100ms = 0;
		Slef_Check_Time++;
	    if(Slef_Check_Time <=200)//最多20秒自检通过，通不过说明有问题
	    {
	      if((ZD_Pump==0)&&(ZD_H_Pump==0)&&(ZD_JYF==0)&&(ZD_KQBYF==0)&&(ZD_SJKYJ1==0)&&(ZD_SJKYJ2==0)&&
	    		  (ZD_DC1==0)&&(ZD_DC2==0)&&(CVM1_Flag==0)&&(CVM2_Flag==0))//该收到的报文全部收到
	      {
	        if((Fault_Level_Three==0)&&(Fault_Level_Four==0))//无三级四级故障
	        {
	        	Stack_State|=0x02;//自检成功
	            JWQ_Enable = 0X01;//节温器运行 自检完成保持节温器关闭
	        	Slef_Check_Time=0;
	        }
	      }
	    }
	    else //自检失败 有问题不能启动
	    {
	    	Stack_State=0;
	    	Slef_Check_Time=0;
	    	if(ZD_Pump!=0)fualt(40105);//上报故障码
	    	if(ZD_H_Pump!=0)fualt(40106);//上报故障码
	    	if(ZD_JYF!=0)fualt(40107);//上报故障码
	    	if(ZD_KQBYF!=0)fualt(40108);//上报故障码
	    	if(ZD_SJKYJ1!=0)fualt(40109);//上报故障码
	    	if(ZD_SJKYJ2!=0)fualt(40110);//上报故障码
	    	if(ZD_DC1!=0)fualt(40111);//上报故障码
	    	if(ZD_DC2!=0)fualt(40112);//上报故障码
	    	if(CVM1_Flag!=0)fualt(40113);//上报故障码
	    	if(CVM2_Flag!=0)fualt(40114);//上报故障码
//	    	if(ZD_CVM3!=0)fualt(40115);//上报故障码
//	    	if(ZD_CVM4!=0)fualt(40116);//上报故障码
	    	if(Fault_Level_Three!=0)fualt(40117);//上报故障码
	    	if(Fault_Level_Four!=0)fualt(40118);//上报故障码
	    	Fault_Level_Four=0x2000;//自检失败故障等级
	    }
	  }

}

