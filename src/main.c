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
    PINS_DRV_SetPins(GPIO_PTE,0<<8);//20200519  Һλ����������

    /*while(1)
    {
     	if((Stack_State&0X10)==0X10)//�µ�  ���״̬
    	{
    		Normal_Shutdown();
    	}
        else if((Stack_State&0X0F)==0X0F)//����
    	{
        	RUN_State();
    	}
    	else if((Stack_State&0X0F)==0X07)//���� �ػ���ʱ�򷢵�17 17&0F���ǵ�07
    	{
    		Start_Up();
    	}
        else if((Stack_State&0X03)==0X01)//�����Լ�
    	{
    		Self_Check();
    	}

    	if(Flag_100us)
    	{
    		Flag_100us=0;
    		Fan_Speed();//���ȵ��ٺ���
    		Fan_Speed2(S_fan_Open);

    	}
    	if(Flag_10ms)
    	{
    		Flag_10ms=0;
    		CAN1_Receive();//����CAN�������
    		CAN0_Send_Out();//�ڲ�CAN���緢��
    		Send_Computer();//���͸���λ��
    		Get_ADC();//����ADC�ɼ���תΪѹ���ź�
    		CAN1_Send_Out2();//����CAN���緢��
    	}
    	if(Flag_20ms)
    	{
    		Flag_20ms=0;
    		ON_Read();//ON�����
    		CAN0_Receive();//����CAN0�����ı���
    	}
    	if(Flag_100ms)
    	{
    		Flag_100ms = 0;
    		T_100ms=1;
    		Fault_Check();//���ϼ��
    		ID_Lose_Check();//��֡����
    		Temp_Con();//�¶ȿ���
    		Out_H();//�Զ�����
    		Stack_P_CAL();//��ѷ���������
    		CAN0_Receive_COM();//��λ�����͵��ֶ����Ƶ�һЩ����
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
