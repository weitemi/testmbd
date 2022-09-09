/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K14x
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "SYS.h"
#include "CAN.h"
#include "LPIT.h"
#include "ADC.h"
#include "REL.h"
#include "Data_Hand.h"
#include "EXTERNAL_EEPROM.h"
#include "CAL.h"
#include "Fault.h"
#include "H_GS.h"
#include "Self_Check.h"
#include "Start_Up.h"
#include "RUN.h"
#include "Close.h"
#include "Temp.h"
#include "PWM.h"
volatile int exit_code = 0;
#include <stdint.h>
#include <stdbool.h>


//flash_ssd_config_t flashSSDConfig;
/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/



#include "FCU_F3NEW.h"

uint8_T ECUCoderModelBaseCounter = 0;
uint8_T AfterRunFlags[2] = { 0, 0 };

static boolean_T ECUCoderOverrunFlags[7] = { 0, 0, 0, 0, 0, 0, 0 };

uint32_t *fs_free_param;
void rt_OneStep(void)
{
  boolean_T ECUCodereventFlags[7];     /* Model has 7 rates */
  int_T i;

  /* Check base rate for overrun */
  if (ECUCoderOverrunFlags[0]++) {
    ((void) 0);
    return;
  }

  FCU_F3NEW_SetEventsForThisBaseStep(ECUCodereventFlags);

  /* Set model inputs associated with base rate here */
  ECUCoderOverrunFlags[0]--;

  /* Check subrates for overrun */
  for (i = 1; i < 7; i++) {
    if (ECUCodereventFlags[i]) {
      if (ECUCoderOverrunFlags[i]++) {
        ((void) 0);
        return;
      }

      FCU_F3NEW_step(i);

      /* Get model outputs here */
      ECUCoderOverrunFlags[i]--;
    }
  }
}



int main(void)
{
  /* Write your local variable definition here */
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    SYS_Ini();
    GPIOInit();
    EX_EEPROM_Init();
//    CAN0_Ini();
//    CAN1_Ini();

    CAN_Init(INST_CAN_PAL0, &can_pal0_Config0);
    CAN_Init(INST_CAN_PAL1, &can_pal1_Config0);
    FCU_F3NEW_initialize();
    LPIT_Ini();
    ADC_Ini();
    CAL_Read_Data();
    PINS_DRV_SetPins(GPIO_PTE,0<<8);//20200519  液位传感器引脚

    /*while(1)
    {
     	if((Stack_State&0X10)==0X10)//下电  电堆状态
    	{
    		Normal_Shutdown();
    	}
        else if((Stack_State&0X0F)==0X0F)//运行
    	{
        	RUN_State();
    	}
    	else if((Stack_State&0X0F)==0X07)//启动 关机的时候发的17 17&0F还是等07
    	{
    		Start_Up();
    	}
        else if((Stack_State&0X03)==0X01)//进行自检
    	{
    		Self_Check();
    	}

    	if(Flag_100us)
    	{
    		Flag_100us=0;
    		Fan_Speed();//风扇调速函数
    		Fan_Speed2(S_fan_Open);

    	}
    	if(Flag_10ms)
    	{
    		Flag_10ms=0;
    		CAN1_Receive();//整车CAN网络接收
    		CAN0_Send_Out();//内部CAN网络发送
    		Send_Computer();//发送给上位机
    		Get_ADC();//接收ADC采集并转为压力信号
    		CAN1_Send_Out2();//整车CAN网络发送
    	}
    	if(Flag_20ms)
    	{
    		Flag_20ms=0;
    		ON_Read();//ON档检测
    		CAN0_Receive();//接收CAN0部件的报文
    	}
    	if(Flag_100ms)
    	{
    		Flag_100ms = 0;
    		T_100ms=1;
    		Fault_Check();//故障检测
    		ID_Lose_Check();//丢帧报文
    		Temp_Con();//温度控制
    		Out_H();//自动排氢
    		Stack_P_CAL();//电堆发电量计算
    		CAN0_Receive_COM();//上位机发送的手动控制的一些报文
    	}
    	if((Timer_TxMsg % 100) ==1)
    	{
    		YeWei_Stack();
    	}
    }*/

    while (1) {
      if (ECUCoderModelBaseCounter >0 ) {
        ECUCoderModelBaseCounter = 0;
        rt_OneStep();
      }
    }

    return 0;
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
//  for(;;) {
//    if(exit_code != 0) {
//      break;
//    }
//  }
//  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
