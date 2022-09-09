/*
 * Code generated for Simulink model FCU_F3NEW.
 *
 * FILE    : FCU_F3NEW.h
 *
 * VERSION : 1.91
 *
 * DATE    : Wed Sep  7 15:40:41 2022
 *
 * Copyright 2011-2019 ECUCoder. All Rights Reserved.
 */

#ifndef RTW_HEADER_FCU_F3NEW_h_
#define RTW_HEADER_FCU_F3NEW_h_

//#include "MPC5743R.h"
//#include "can.h"
#include "Std_Types.h"
#include "Cpu.h"
#include "can.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#include "LPIT.h"

//#include "intc.h"
#ifndef FCU_F3NEW_COMMON_INCLUDES_
# define FCU_F3NEW_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "can_message.h"
#endif                                 /* FCU_F3NEW_COMMON_INCLUDES_ */

#include "FCU_F3NEW_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmStepTask
# define rtmStepTask(rtm, idx)         ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

#ifndef rtmTaskCounter
# define rtmTaskCounter(rtm, idx)      ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

#define EnableInterrupts()             asm(" wrteei 1")
#define DisableInterrupts()            asm(" wrteei 0")

/* user code (top of export header file) */
#include "can_message.h"

/* Block signals (default storage) */
typedef struct {
  CAN_DATATYPE FCU2EHI_CAN1_CANPack;   /* '<S5>/FCU2EHI_CAN1_CANPack' */
  CAN_DATATYPE FCU2EHI_CAN2_CANPack;   /* '<S5>/FCU2EHI_CAN2_CANPack' */
  CAN_DATATYPE FCU_CAN2_CANUnPackMessage;/* '<S9>/FCU_CAN2_CANUnPackMessage' */
  CAN_DATATYPE FCU_CAN1_CANUnPackMessage;/* '<S8>/FCU_CAN1_CANUnPackMessage' */
  real_T FCU_CAN1_CANUnPack_o1;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o2;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o3;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o4;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o5;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o6;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o7;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o8;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o9;        /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o10;       /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o11;       /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o12;       /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o13;       /* '<S8>/FCU_CAN1_CANUnPack' */
  real_T FCU_CAN1_CANUnPack_o14;       /* '<S8>/FCU_CAN1_CANUnPack' */
  uint32_T CANReceive1_o3;             /* '<S35>/CANReceive1' */
  uint32_T CANReceive_o3;              /* '<S22>/CANReceive' */
  uint32_T CANReceive_o3_o;            /* '<S14>/CANReceive' */
  uint32_T CAN1_CANReceive_o3;         /* '<S4>/CAN1_CANReceive' */
  uint32_T CAN2_CANReceive_o3;         /* '<S4>/CAN2_CANReceive' */
  uint16_T MEA_FCU2EHI_BYTE4_5_e;      /* '<S3>/RT' */
  uint16_T MEA_FCU2EHI_BYTE2_3_c;      /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BYTE7_a;         /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BYTE6_o;         /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT7_m;          /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT6_k;          /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT5_e;          /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT4_7_b;        /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT4_e;          /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT3_e;          /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT2_i;          /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT1_l;          /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT0_3_f;        /* '<S3>/RT' */
  uint8_T MEA_FCU2EHI_BIT0_c;          /* '<S3>/RT' */
  uint8_T CANReceive1_o2;              /* '<S35>/CANReceive1' */
  uint8_T CANReceive1_o4[8];           /* '<S35>/CANReceive1' */
  uint8_T CANReceive1_o5;              /* '<S35>/CANReceive1' */
  uint8_T CANReceive_o2;               /* '<S22>/CANReceive' */
  uint8_T CANReceive_o4[8];            /* '<S22>/CANReceive' */
  uint8_T CANReceive_o5;               /* '<S22>/CANReceive' */
  uint8_T CANTransmit;                 /* '<S29>/CANTransmit' */
  uint8_T CANReceive_o2_l;             /* '<S14>/CANReceive' */
  uint8_T CANReceive_o4_j[8];          /* '<S14>/CANReceive' */
  uint8_T CANReceive_o5_j;             /* '<S14>/CANReceive' */
  uint8_T CANTransmit_g;               /* '<S21>/CANTransmit' */
  uint8_T CANPackMessage[8];           /* '<S5>/CANPackMessage' */
  uint8_T CANPackMessage1[8];          /* '<S5>/CANPackMessage1' */
  uint8_T CANTransmit_p;               /* '<S5>/CANTransmit' */
  uint8_T CANTransmit1;                /* '<S5>/CANTransmit1' */
  uint8_T CAN1_CANReceive_o4[8];       /* '<S4>/CAN1_CANReceive' */
  uint8_T CAN1_CANReceive_o5;          /* '<S4>/CAN1_CANReceive' */
  uint8_T CAN2_CANReceive_o4[8];       /* '<S4>/CAN2_CANReceive' */
  uint8_T CAN2_CANReceive_o5;          /* '<S4>/CAN2_CANReceive' */
} B_FCU_F3NEW_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int_T FCU2EHI_CAN1_CANPack_ModeSignal;/* '<S5>/FCU2EHI_CAN1_CANPack' */
  int_T FCU2EHI_CAN2_CANPack_ModeSignal;/* '<S5>/FCU2EHI_CAN2_CANPack' */
  int_T FCU_CAN1_CANUnPack_ModeSignalID;/* '<S8>/FCU_CAN1_CANUnPack' */
  int_T FCU_CAN1_CANUnPack_StatusPortID;/* '<S8>/FCU_CAN1_CANUnPack' */
} DW_FCU_F3NEW_T;

/* Real-time Model Data Structure */
struct tag_RTM_FCU_F3NEW_T {
  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint16_T TID[7];
    } TaskCounters;

    struct {
      boolean_T TID3_6;
    } RateInteraction;
  } Timing;
};

/* Block signals (default storage) */
extern B_FCU_F3NEW_T FCU_F3NEW_B;

/* Block states (default storage) */
extern DW_FCU_F3NEW_T FCU_F3NEW_DW;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern uint16_T MEA_FCU2EHI_BYTE2_3;   /* '<S6>/Data Type Conversion10'
                                        * ��������ֵ_BYTE2_3
                                        */
extern uint16_T MEA_FCU2EHI_BYTE4_5;   /* '<S6>/Data Type Conversion11'
                                        * ��������ֵ_BYTE4_5
                                        */
extern uint8_T MEA_CAN1_OK;            /* '<S4>/CAN1_CANReceive'
                                        * CAN1����
                                        */
extern uint8_T MEA_CAN2_OK;            /* '<S4>/CAN2_CANReceive'
                                        * CAN2����
                                        */
extern uint8_T MEA_FCU2EHI_BIT0;       /* '<S6>/Data Type Conversion'
                                        * ��������ֵ_BIT0
                                        */
extern uint8_T MEA_FCU2EHI_BIT0_3;     /* '<S6>/Data Type Conversion1'
                                        * ��������ֵ_BIT0_3
                                        */
extern uint8_T MEA_FCU2EHI_BYTE6;      /* '<S6>/Data Type Conversion12'
                                        * ��������ֵ_BYTE6
                                        */
extern uint8_T MEA_FCU2EHI_BYTE7;      /* '<S6>/Data Type Conversion13'
                                        * ��������ֵ_BYTE7
                                        */
extern uint8_T MEA_FCU2EHI_BIT1;       /* '<S6>/Data Type Conversion2'
                                        * ��������ֵ_BIT1
                                        */
extern uint8_T MEA_FCU2EHI_BIT2;       /* '<S6>/Data Type Conversion3'
                                        * ��������ֵ_BIT2
                                        */
extern uint8_T MEA_FCU2EHI_BIT3;       /* '<S6>/Data Type Conversion4'
                                        * ��������ֵ_BIT3
                                        */
extern uint8_T MEA_FCU2EHI_BIT4;       /* '<S6>/Data Type Conversion5'
                                        * ��������ֵ_BIT4
                                        */
extern uint8_T MEA_FCU2EHI_BIT4_7;     /* '<S6>/Data Type Conversion6'
                                        * ��������ֵ_BIT4_7
                                        */
extern uint8_T MEA_FCU2EHI_BIT5;       /* '<S6>/Data Type Conversion7'
                                        * ��������ֵ_BIT5
                                        */
extern uint8_T MEA_FCU2EHI_BIT6;       /* '<S6>/Data Type Conversion8'
                                        * ��������ֵ_BIT6
                                        */
extern uint8_T MEA_FCU2EHI_BIT7;       /* '<S6>/Data Type Conversion9'
                                        * ��������ֵ_BIT7
                                        */

/* External function called from main */
extern void FCU_F3NEW_SetEventsForThisBaseStep(boolean_T *eventFlags);

/* Model entry point functions */
extern void FCU_F3NEW_SetEventsForThisBaseStep(boolean_T *eventFlags);
extern void FCU_F3NEW_initialize(void);
extern void FCU_F3NEW_step(int_T tid);
extern uint8_T ECUCoderModelBaseCounter;
extern uint32_t IntcIsrVectorTable[];
extern uint8_T AfterRunFlags[2];
extern void ISR_PIT_CH3(void);

//extern UINT32 __ram_program_func[0x120];
//extern pFLASHPROGRAM g_FlashProgram;

/* Real-time Model object */
extern RT_MODEL_FCU_F3NEW_T *const FCU_F3NEW_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S9>/FCU_CAN2_CANUnPack' : Unused code path elimination
 * Block '<S2>/Constant' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'FCU_F3NEW'
 * '<S1>'   : 'FCU_F3NEW/CAN_RX_TX'
 * '<S2>'   : 'FCU_F3NEW/RapidECUSetting'
 * '<S3>'   : 'FCU_F3NEW/CAN_RX_TX/CAN1_CAN2'
 * '<S4>'   : 'FCU_F3NEW/CAN_RX_TX/CAN1_CAN2/CAN_RX'
 * '<S5>'   : 'FCU_F3NEW/CAN_RX_TX/CAN1_CAN2/CAN_TX'
 * '<S6>'   : 'FCU_F3NEW/CAN_RX_TX/CAN1_CAN2/CAN_RX/CAN1_Check'
 * '<S7>'   : 'FCU_F3NEW/CAN_RX_TX/CAN1_CAN2/CAN_RX/CAN2_Check'
 * '<S8>'   : 'FCU_F3NEW/CAN_RX_TX/CAN1_CAN2/CAN_RX/CAN1_Check/Subsystem'
 * '<S9>'   : 'FCU_F3NEW/CAN_RX_TX/CAN1_CAN2/CAN_RX/CAN2_Check/Subsystem'
 * '<S10>'  : 'FCU_F3NEW/RapidECUSetting/BL'
 * '<S11>'  : 'FCU_F3NEW/RapidECUSetting/BL1'
 * '<S12>'  : 'FCU_F3NEW/RapidECUSetting/DAQ'
 * '<S13>'  : 'FCU_F3NEW/RapidECUSetting/Polling'
 * '<S14>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem'
 * '<S15>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem/Function-Call Subsystem'
 * '<S16>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem'
 * '<S17>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/Com0'
 * '<S18>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/Com1'
 * '<S19>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/Com2'
 * '<S20>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/If Action Subsystem'
 * '<S21>'  : 'FCU_F3NEW/RapidECUSetting/BL/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/If Action Subsystem1'
 * '<S22>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem'
 * '<S23>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem/Function-Call Subsystem'
 * '<S24>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem'
 * '<S25>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/Com0'
 * '<S26>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/Com1'
 * '<S27>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/Com2'
 * '<S28>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/If Action Subsystem'
 * '<S29>'  : 'FCU_F3NEW/RapidECUSetting/BL1/Function-Call Subsystem/Function-Call Subsystem/Enabled Subsystem/If Action Subsystem1'
 * '<S30>'  : 'FCU_F3NEW/RapidECUSetting/DAQ/daq100ms'
 * '<S31>'  : 'FCU_F3NEW/RapidECUSetting/DAQ/daq10ms'
 * '<S32>'  : 'FCU_F3NEW/RapidECUSetting/DAQ/daq50ms'
 * '<S33>'  : 'FCU_F3NEW/RapidECUSetting/DAQ/daq5ms'
 * '<S34>'  : 'FCU_F3NEW/RapidECUSetting/Polling/CCPBackground'
 * '<S35>'  : 'FCU_F3NEW/RapidECUSetting/Polling/CCPReceive'
 * '<S36>'  : 'FCU_F3NEW/RapidECUSetting/Polling/CCPTransmit'
 * '<S37>'  : 'FCU_F3NEW/RapidECUSetting/Polling/CCPReceive/Nothing'
 */
#endif                                 /* RTW_HEADER_FCU_F3NEW_h_ */

/* File trailer for ECUCoder generated file FCU_F3NEW.h.
 *
 * [EOF]
 */
