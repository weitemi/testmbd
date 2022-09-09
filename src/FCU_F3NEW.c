/*
 * Code generated for Simulink model FCU_F3NEW.
 *
 * FILE    : FCU_F3NEW.c
 *
 * VERSION : 1.91
 *
 * DATE    : Wed Sep  7 15:40:41 2022
 *
 * Copyright 2011-2019 ECUCoder. All Rights Reserved.
 */

#include "FCU_F3NEW.h"
#include "FCU_F3NEW_private.h"

/* #include "myinclude.h" */
// EC_LLSW_INPOUTMONISTS_TYPE EC_InpOutMonitorSts;

/* Exported block signals */
uint16_T MEA_FCU2EHI_BYTE2_3;          /* '<S6>/Data Type Conversion10'
                                        * ��������ֵ_BYTE2_3
                                        */
uint16_T MEA_FCU2EHI_BYTE4_5;          /* '<S6>/Data Type Conversion11'
                                        * ��������ֵ_BYTE4_5
                                        */
uint8_T MEA_CAN1_OK;                   /* '<S4>/CAN1_CANReceive'
                                        * CAN1����
                                        */
uint8_T MEA_CAN2_OK;                   /* '<S4>/CAN2_CANReceive'
                                        * CAN2����
                                        */
uint8_T MEA_FCU2EHI_BIT0;              /* '<S6>/Data Type Conversion'
                                        * ��������ֵ_BIT0
                                        */
uint8_T MEA_FCU2EHI_BIT0_3;            /* '<S6>/Data Type Conversion1'
                                        * ��������ֵ_BIT0_3
                                        */
uint8_T MEA_FCU2EHI_BYTE6;             /* '<S6>/Data Type Conversion12'
                                        * ��������ֵ_BYTE6
                                        */
uint8_T MEA_FCU2EHI_BYTE7;             /* '<S6>/Data Type Conversion13'
                                        * ��������ֵ_BYTE7
                                        */
uint8_T MEA_FCU2EHI_BIT1;              /* '<S6>/Data Type Conversion2'
                                        * ��������ֵ_BIT1
                                        */
uint8_T MEA_FCU2EHI_BIT2;              /* '<S6>/Data Type Conversion3'
                                        * ��������ֵ_BIT2
                                        */
uint8_T MEA_FCU2EHI_BIT3;              /* '<S6>/Data Type Conversion4'
                                        * ��������ֵ_BIT3
                                        */
uint8_T MEA_FCU2EHI_BIT4;              /* '<S6>/Data Type Conversion5'
                                        * ��������ֵ_BIT4
                                        */
uint8_T MEA_FCU2EHI_BIT4_7;            /* '<S6>/Data Type Conversion6'
                                        * ��������ֵ_BIT4_7
                                        */
uint8_T MEA_FCU2EHI_BIT5;              /* '<S6>/Data Type Conversion7'
                                        * ��������ֵ_BIT5
                                        */
uint8_T MEA_FCU2EHI_BIT6;              /* '<S6>/Data Type Conversion8'
                                        * ��������ֵ_BIT6
                                        */
uint8_T MEA_FCU2EHI_BIT7;              /* '<S6>/Data Type Conversion9'
                                        * ��������ֵ_BIT7
                                        */

/* Block signals (default storage) */
B_FCU_F3NEW_T FCU_F3NEW_B;

/* Block states (default storage) */
DW_FCU_F3NEW_T FCU_F3NEW_DW;

/* Real-time model */
RT_MODEL_FCU_F3NEW_T FCU_F3NEW_M_;
RT_MODEL_FCU_F3NEW_T *const FCU_F3NEW_M = &FCU_F3NEW_M_;
static void rate_monotonic_scheduler(void);
void ISR_PIT_CH3(void)
{
  LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT1, (1 << LPIT_CHANNEL2));
  ECUCoderModelBaseCounter++;
  rate_monotonic_scheduler();
}

void ISR_FlexCAN_3_MB0(void)
{
  /* Call the system: <S13>/CCPReceive */
  {
    /* S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt' */

    /* Output and update for function-call system: '<S13>/CCPReceive' */

    /* S-Function (ec5743_canreceiveslb): '<S35>/CANReceive1' */

    /* Receive CAN message */
    {
      uint8 CAN3BUF0RX[8]= { 0, 0, 0, 0, 0, 0, 0, 0 };

      uint8 can3buf0looprx= 0;
      FCU_F3NEW_B.CANReceive1_o3= 256;
      FCU_F3NEW_B.CANReceive1_o5= 8;
      FCU_F3NEW_B.CANReceive1_o2= ec_can3_receive(0, CAN3BUF0RX);
      FCU_F3NEW_B.CANReceive1_o4[0]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
      FCU_F3NEW_B.CANReceive1_o4[1]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
      FCU_F3NEW_B.CANReceive1_o4[2]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
      FCU_F3NEW_B.CANReceive1_o4[3]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
      FCU_F3NEW_B.CANReceive1_o4[4]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
      FCU_F3NEW_B.CANReceive1_o4[5]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
      FCU_F3NEW_B.CANReceive1_o4[6]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
      FCU_F3NEW_B.CANReceive1_o4[7]= CAN3BUF0RX[can3buf0looprx];
      can3buf0looprx++;
    }

    /* Nothing to do for system: <S35>/Nothing */

    /* End of Outputs for S-Function (ec5743_canreceiveslb): '<S35>/CANReceive1' */

    /* End of Outputs for S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt' */
  }

  /*CAN_3.IFLAG1.B.BUF0I = 1;            /* Clear CAN interrupt flag by writing it to 1 */
}

void ISR_FlexCAN_3_MB40(void)
{
  /* Call the system: <S13>/CCPTransmit */
  {
    /* S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt1' */

    /* Output and update for function-call system: '<S13>/CCPTransmit' */

    /* S-Function (ec5743_ccpslb2): '<S36>/CCPTXConfirmation' */
    /*ec_can_appltxconfirmation(3,CCP_CAN_TX_BUFNUM);*/

    /* End of Outputs for S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt1' */
  }

  /*CAN_3.IFLAG2.B.BUF63TO32I = 0x00000100; Clear CAN interrupt flag by writing it to 1 */
}

/*
 * Set which subrates need to run this base step (base rate always runs).
 * This function must be called prior to calling the model step function
 * in order to "remember" which rates need to run this base step.  The
 * buffering of events allows for overlapping preemption.
 */
void FCU_F3NEW_SetEventsForThisBaseStep(boolean_T *eventFlags)
{
  /* Task runs when its counter is zero, computed via rtmStepTask macro */
  eventFlags[1] = ((boolean_T)rtmStepTask(FCU_F3NEW_M, 1));
  eventFlags[2] = ((boolean_T)rtmStepTask(FCU_F3NEW_M, 2));
  eventFlags[3] = ((boolean_T)rtmStepTask(FCU_F3NEW_M, 3));
  eventFlags[4] = ((boolean_T)rtmStepTask(FCU_F3NEW_M, 4));
  eventFlags[5] = ((boolean_T)rtmStepTask(FCU_F3NEW_M, 5));
  eventFlags[6] = ((boolean_T)rtmStepTask(FCU_F3NEW_M, 6));
}

/*
 *   This function updates active task flag for each subrate
 * and rate transition flags for tasks that exchange data.
 * The function assumes rate-monotonic multitasking scheduler.
 * The function must be called at model base rate so that
 * the generated code self-manages all its subrates and rate
 * transition flags.
 */
static void rate_monotonic_scheduler(void)
{
  /* To ensure a deterministic data transfer between two rates,
   * data is transferred at the priority of a fast task and the frequency
   * of the slow task.  The following flags indicate when the data transfer
   * happens.  That is, a rate interaction flag is set true when both rates
   * will run, and false otherwise.
   */

  /* tid 3 shares data with slower tid rate: 6 */
  if (FCU_F3NEW_M->Timing.TaskCounters.TID[3] == 0) {
    FCU_F3NEW_M->Timing.RateInteraction.TID3_6 =
      (FCU_F3NEW_M->Timing.TaskCounters.TID[6] == 0);
  }

  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (FCU_F3NEW_M->Timing.TaskCounters.TID[1])++;
  if ((FCU_F3NEW_M->Timing.TaskCounters.TID[1]) > 3) {/* Sample time: [0.001s, 0.0s] */
    FCU_F3NEW_M->Timing.TaskCounters.TID[1] = 0;
  }

  (FCU_F3NEW_M->Timing.TaskCounters.TID[2])++;
  if ((FCU_F3NEW_M->Timing.TaskCounters.TID[2]) > 19) {/* Sample time: [0.005s, 0.0s] */
    FCU_F3NEW_M->Timing.TaskCounters.TID[2] = 0;
  }

  (FCU_F3NEW_M->Timing.TaskCounters.TID[3])++;
  if ((FCU_F3NEW_M->Timing.TaskCounters.TID[3]) > 39) {/* Sample time: [0.01s, 0.0s] */
    FCU_F3NEW_M->Timing.TaskCounters.TID[3] = 0;
  }

  (FCU_F3NEW_M->Timing.TaskCounters.TID[4])++;
  if ((FCU_F3NEW_M->Timing.TaskCounters.TID[4]) > 199) {/* Sample time: [0.05s, 0.0s] */
    FCU_F3NEW_M->Timing.TaskCounters.TID[4] = 0;
  }

  (FCU_F3NEW_M->Timing.TaskCounters.TID[5])++;
  if ((FCU_F3NEW_M->Timing.TaskCounters.TID[5]) > 399) {/* Sample time: [0.1s, 0.0s] */
    FCU_F3NEW_M->Timing.TaskCounters.TID[5] = 0;
  }

  (FCU_F3NEW_M->Timing.TaskCounters.TID[6])++;
  if ((FCU_F3NEW_M->Timing.TaskCounters.TID[6]) > 799) {/* Sample time: [0.2s, 0.0s] */
    FCU_F3NEW_M->Timing.TaskCounters.TID[6] = 0;
  }
}

/*
 * Output and update for action system:
 *    '<S16>/If Action Subsystem'
 *    '<S24>/If Action Subsystem'
 */
void FCU_F3NEW_IfActionSubsystem(void)
{
  /* S-Function (ec5743_bootloaderslb): '<S20>/BootLoader' */
  /*{
    uint16 i= 0;
    N256K_BLOCK_SEL n256KBlockSelect;
    CONTEXT_DATA pgmCtxData;
    n256KBlockSelect.first256KBlockSelect = 0x00000000;
    n256KBlockSelect.second256KBlockSelect = 0x00000000;
    pgmCtxData.pReqCompletionFn = pFlashProgram;
    uint8 CAN2BUF9TX[]= { 11, 12, 12, 13 };

    uint8 returnCode1= 0;
    uint8 bootflag[]= { 1, 0, 0, 0, 0, 0, 0, 0 };

    DisableInterrupts();
    App_FlashErase( &ssdConfig, 0, 0x00000000, 0x00000010, 0x00000000,
                   n256KBlockSelect );
    App_FlashProgram( &ssdConfig, 0, 0x00810000, 8, (uint32)bootflag,
                     &pgmCtxData );
    i= 1000;
    while (i--) {
      ;
    }

    ec_can3_transmit(41, 0, 593, 4, CAN2BUF9TX);
    ec_can3_transmit(42, 0, 161, 4, CAN2BUF9TX);
    i= 10000;
    while (i--) {
      ;
    }
  }*/

  /* S-Function (ec5743_cpuresetslb): '<S20>/CPUReset' */

  /* Perform a microcontroller reset
  MC_ME.MCTL.R = 0X00005AF0;
  MC_ME.MCTL.R = 0X0000A50F;*/
}

/* Model step function for TID0 */
void FCU_F3NEW_step0(void)             /* Sample time: [0.00025s, 0.0s] */
{
  {                                    /* Sample time: [0.00025s, 0.0s] */
    rate_monotonic_scheduler();
  }
}

/* Model step function for TID1 */
void FCU_F3NEW_step1(void)             /* Sample time: [0.001s, 0.0s] */
{
  /* S-Function (fcncallgen): '<S13>/Function-Call Generator' incorporates:
   *  SubSystem: '<S13>/CCPBackground'
   */

  /* S-Function (ec5743_ccpslb): '<S34>/CCPBackground' */

  /* End of Outputs for S-Function (fcncallgen): '<S13>/Function-Call Generator' */
}

/* Model step function for TID2 */
void FCU_F3NEW_step2(void)             /* Sample time: [0.005s, 0.0s] */
{
  /* S-Function (fcncallgen): '<S12>/5ms' incorporates:
   *  SubSystem: '<S12>/daq5ms'
   */

  /* S-Function (ec5743_ccpslb1): '<S33>/CCPDAQ' */

  /* End of Outputs for S-Function (fcncallgen): '<S12>/5ms' */

  /* S-Function (ec5743_5mstf3): '<S2>/5msTask' */
}

/* Model step function for TID3 */
void FCU_F3NEW_step3(void)             /* Sample time: [0.01s, 0.0s] */
{
  /* S-Function (fcncallgen): '<S3>/10ms' incorporates:
   *  SubSystem: '<S3>/CAN_RX'
   */
  /* S-Function (ec5743_canreceiveslb): '<S4>/CAN1_CANReceive' */

  /* Receive CAN message */
  {
    uint8 CAN1BUF0RX[8]= { 0, 0, 0, 0, 0, 0, 0, 0 };

    uint8 can1buf0looprx= 0;
    FCU_F3NEW_B.CAN1_CANReceive_o3= 419365125;
    FCU_F3NEW_B.CAN1_CANReceive_o5= 8;
    MEA_CAN1_OK= ec_can1_receive(0, CAN1BUF0RX);
    FCU_F3NEW_B.CAN1_CANReceive_o4[0]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
    FCU_F3NEW_B.CAN1_CANReceive_o4[1]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
    FCU_F3NEW_B.CAN1_CANReceive_o4[2]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
    FCU_F3NEW_B.CAN1_CANReceive_o4[3]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
    FCU_F3NEW_B.CAN1_CANReceive_o4[4]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
    FCU_F3NEW_B.CAN1_CANReceive_o4[5]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
    FCU_F3NEW_B.CAN1_CANReceive_o4[6]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
    FCU_F3NEW_B.CAN1_CANReceive_o4[7]= CAN1BUF0RX[can1buf0looprx];
    can1buf0looprx++;
  }

  /* Call the system: <S4>/CAN1_Check */

  /* Output and update for function-call system: '<S4>/CAN1_Check' */

  /* Outputs for Enabled SubSystem: '<S6>/Subsystem' incorporates:
   *  EnablePort: '<S8>/Enable'
   */
  if (MEA_CAN1_OK > 0) {
    /* S-Function (ecucoder_canunmessage): '<S8>/FCU_CAN1_CANUnPackMessage' */

    /*Unpack CAN message*/
    {
      uint8 canunpackloop= 0;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Length = 8;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.ID = FCU_F3NEW_B.CAN1_CANReceive_o3;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Extended = 1;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[0];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[1];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[2];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[3];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[4];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[5];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[6];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN1_CANReceive_o4[7];
      canunpackloop++;
    }

    /* S-Function (scanunpack): '<S8>/FCU_CAN1_CANUnPack' */
    {
      /* S-Function (scanunpack): '<S8>/FCU_CAN1_CANUnPack' */
      if ((8 == FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Length) &&
          (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.ID != INVALID_CAN_ID) ) {
        if ((419365125 == FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.ID) && (1U ==
             FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Extended) ) {
          {
            /* --------------- START Unpacking signal 0 ------------------
             *  startBit                = 7
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x80U)) >> 7);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o1 = result;
              }
            }

            /* --------------- START Unpacking signal 1 ------------------
             *  startBit                = 12
             *  length                  = 4
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[1]) & (uint8_T)
                      (0xF0U)) >> 4);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o2 = result;
              }
            }

            /* --------------- START Unpacking signal 2 ------------------
             *  startBit                = 6
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x40U)) >> 6);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o3 = result;
              }
            }

            /* --------------- START Unpacking signal 3 ------------------
             *  startBit                = 5
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x20U)) >> 5);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o4 = result;
              }
            }

            /* --------------- START Unpacking signal 4 ------------------
             *  startBit                = 4
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x10U)) >> 4);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o5 = result;
              }
            }

            /* --------------- START Unpacking signal 5 ------------------
             *  startBit                = 3
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x8U)) >> 3);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o6 = result;
              }
            }

            /* --------------- START Unpacking signal 6 ------------------
             *  startBit                = 8
             *  length                  = 4
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[1]) & (uint8_T)
                      (0xFU));
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o7 = result;
              }
            }

            /* --------------- START Unpacking signal 7 ------------------
             *  startBit                = 2
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x4U)) >> 2);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o8 = result;
              }
            }

            /* --------------- START Unpacking signal 8 ------------------
             *  startBit                = 1
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x2U)) >> 1);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o9 = result;
              }
            }

            /* --------------- START Unpacking signal 9 ------------------
             *  startBit                = 0
             *  length                  = 1
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[0]) & (uint8_T)
                      (0x1U));
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o10 = result;
              }
            }

            /* --------------- START Unpacking signal 10 ------------------
             *  startBit                = 24
             *  length                  = 16
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint16_T unpackedValue = 0;

                {
                  uint16_T tempValue = (uint16_T) (0);

                  {
                    tempValue = tempValue | (uint16_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[3]);
                    tempValue = tempValue | (uint16_T)((uint16_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[2]) << 8);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o11 = result;
              }
            }

            /* --------------- START Unpacking signal 11 ------------------
             *  startBit                = 32
             *  length                  = 16
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint16_T unpackedValue = 0;

                {
                  uint16_T tempValue = (uint16_T) (0);

                  {
                    tempValue = tempValue | (uint16_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[4]);
                    tempValue = tempValue | (uint16_T)((uint16_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[5]) << 8);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o12 = result;
              }
            }

            /* --------------- START Unpacking signal 12 ------------------
             *  startBit                = 48
             *  length                  = 8
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[6]);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o13 = result;
              }
            }

            /* --------------- START Unpacking signal 13 ------------------
             *  startBit                = 56
             *  length                  = 8
             *  desiredSignalByteLayout = BIGENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real64_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)
                      (FCU_F3NEW_B.FCU_CAN1_CANUnPackMessage.Data[7]);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real64_T) (unpackedValue);
              }

              {
                real64_T result = (real64_T) outValue;
                FCU_F3NEW_B.FCU_CAN1_CANUnPack_o14 = result;
              }
            }
          }
        }
      }
    }
  }

  /* End of Outputs for SubSystem: '<S6>/Subsystem' */

  /* DataTypeConversion: '<S6>/Data Type Conversion' */
  MEA_FCU2EHI_BIT0 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o1;

  /* DataTypeConversion: '<S6>/Data Type Conversion1' */
  MEA_FCU2EHI_BIT0_3 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o2;

  /* DataTypeConversion: '<S6>/Data Type Conversion10' */
  MEA_FCU2EHI_BYTE2_3 = (uint16_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o11;

  /* DataTypeConversion: '<S6>/Data Type Conversion11' */
  MEA_FCU2EHI_BYTE4_5 = (uint16_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o12;

  /* DataTypeConversion: '<S6>/Data Type Conversion12' */
  MEA_FCU2EHI_BYTE6 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o13;

  /* DataTypeConversion: '<S6>/Data Type Conversion13' */
  MEA_FCU2EHI_BYTE7 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o14;

  /* DataTypeConversion: '<S6>/Data Type Conversion2' */
  MEA_FCU2EHI_BIT1 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o3;

  /* DataTypeConversion: '<S6>/Data Type Conversion3' */
  MEA_FCU2EHI_BIT2 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o4;

  /* DataTypeConversion: '<S6>/Data Type Conversion4' */
  MEA_FCU2EHI_BIT3 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o5;

  /* DataTypeConversion: '<S6>/Data Type Conversion5' */
  MEA_FCU2EHI_BIT4 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o6;

  /* DataTypeConversion: '<S6>/Data Type Conversion6' */
  MEA_FCU2EHI_BIT4_7 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o7;

  /* DataTypeConversion: '<S6>/Data Type Conversion7' */
  MEA_FCU2EHI_BIT5 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o8;

  /* DataTypeConversion: '<S6>/Data Type Conversion8' */
  MEA_FCU2EHI_BIT6 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o9;

  /* DataTypeConversion: '<S6>/Data Type Conversion9' */
  MEA_FCU2EHI_BIT7 = (uint8_T)FCU_F3NEW_B.FCU_CAN1_CANUnPack_o10;

  /* End of Outputs for S-Function (ec5743_canreceiveslb): '<S4>/CAN1_CANReceive' */

  /* S-Function (ec5743_canreceiveslb): '<S4>/CAN2_CANReceive' */

  /* Receive CAN message */
  {
    uint8 CAN2BUF0RX[8]= { 0, 0, 0, 0, 0, 0, 0, 0 };

    uint8 can2buf0looprx= 0;
    FCU_F3NEW_B.CAN2_CANReceive_o3= 419365381;
    FCU_F3NEW_B.CAN2_CANReceive_o5= 8;
    MEA_CAN2_OK= ec_can2_receive(0, CAN2BUF0RX);
    FCU_F3NEW_B.CAN2_CANReceive_o4[0]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
    FCU_F3NEW_B.CAN2_CANReceive_o4[1]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
    FCU_F3NEW_B.CAN2_CANReceive_o4[2]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
    FCU_F3NEW_B.CAN2_CANReceive_o4[3]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
    FCU_F3NEW_B.CAN2_CANReceive_o4[4]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
    FCU_F3NEW_B.CAN2_CANReceive_o4[5]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
    FCU_F3NEW_B.CAN2_CANReceive_o4[6]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
    FCU_F3NEW_B.CAN2_CANReceive_o4[7]= CAN2BUF0RX[can2buf0looprx];
    can2buf0looprx++;
  }

  /* Call the system: <S4>/CAN2_Check */

  /* Output and update for function-call system: '<S4>/CAN2_Check' */

  /* Outputs for Enabled SubSystem: '<S7>/Subsystem' incorporates:
   *  EnablePort: '<S9>/Enable'
   */
  if (MEA_CAN2_OK > 0) {
    /* S-Function (ecucoder_canunmessage): '<S9>/FCU_CAN2_CANUnPackMessage' */

    /*Unpack CAN message*/
    {
      uint8 canunpackloop= 0;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Length = 8;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.ID = FCU_F3NEW_B.CAN2_CANReceive_o3;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Extended = 1;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[0];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[1];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[2];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[3];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[4];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[5];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[6];
      canunpackloop++;
      FCU_F3NEW_B.FCU_CAN2_CANUnPackMessage.Data[canunpackloop]=
        FCU_F3NEW_B.CAN2_CANReceive_o4[7];
      canunpackloop++;
    }
  }

  /* End of Outputs for SubSystem: '<S7>/Subsystem' */

  /* End of Outputs for S-Function (ec5743_canreceiveslb): '<S4>/CAN2_CANReceive' */
  /* End of Outputs for S-Function (fcncallgen): '<S3>/10ms' */

  /* RateTransition: '<S3>/RT' */
  if (FCU_F3NEW_M->Timing.RateInteraction.TID3_6) {
    FCU_F3NEW_B.MEA_FCU2EHI_BYTE7_a = MEA_FCU2EHI_BYTE7;
    FCU_F3NEW_B.MEA_FCU2EHI_BYTE6_o = MEA_FCU2EHI_BYTE6;
    FCU_F3NEW_B.MEA_FCU2EHI_BYTE4_5_e = MEA_FCU2EHI_BYTE4_5;
    FCU_F3NEW_B.MEA_FCU2EHI_BYTE2_3_c = MEA_FCU2EHI_BYTE2_3;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT7_m = MEA_FCU2EHI_BIT7;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT6_k = MEA_FCU2EHI_BIT6;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT5_e = MEA_FCU2EHI_BIT5;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT4_7_b = MEA_FCU2EHI_BIT4_7;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT4_e = MEA_FCU2EHI_BIT4;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT3_e = MEA_FCU2EHI_BIT3;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT2_i = MEA_FCU2EHI_BIT2;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT1_l = MEA_FCU2EHI_BIT1;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT0_3_f = MEA_FCU2EHI_BIT0_3;
    FCU_F3NEW_B.MEA_FCU2EHI_BIT0_c = MEA_FCU2EHI_BIT0;
  }

  /* End of RateTransition: '<S3>/RT' */

  /* S-Function (fcncallgen): '<S12>/10ms' incorporates:
   *  SubSystem: '<S12>/daq10ms'
   */
  /* S-Function (ec5743_ccpslb1): '<S31>/CCPDAQ' */

  /* End of Outputs for S-Function (fcncallgen): '<S12>/10ms' */

  /* S-Function (ec5743_10mstf3new): '<S2>/10msTask' */
#if defined EC_UDS_ENABLE

  ec_UDS_tp();

#endif

}

/* Model step function for TID4 */
void FCU_F3NEW_step4(void)             /* Sample time: [0.05s, 0.0s] */
{
  /* S-Function (fcncallgen): '<S12>/50ms' incorporates:
   *  SubSystem: '<S12>/daq50ms'
   */

  /* S-Function (ec5743_ccpslb1): '<S32>/CCPDAQ' */
  /* ccpDaq(2); */

  /* End of Outputs for S-Function (fcncallgen): '<S12>/50ms' */

  /* S-Function (ec5743_50mstfcu): '<S2>/50msTask' */
  /* ecbsw_outputdiagnostic(&EC_InpOutMonitorSts);*/
}

/* Model step function for TID5 */
void FCU_F3NEW_step5(void)             /* Sample time: [0.1s, 0.0s] */
{
  /* S-Function (fcncallgen): '<S10>/100MS' incorporates:
   *  SubSystem: '<S10>/Function-Call Subsystem'
   */
  /* S-Function (ec5743_canreceiveslb): '<S14>/CANReceive' */

  /* Receive CAN message */

  /* End of Outputs for S-Function (fcncallgen): '<S12>/100ms' */
}

/* Model step function for TID6 */
void FCU_F3NEW_step6(void)             /* Sample time: [0.2s, 0.0s] */
{
  /* S-Function (fcncallgen): '<S3>/200ms' incorporates:
   *  SubSystem: '<S3>/CAN_TX'
   */

  /* S-Function (scanpack): '<S5>/FCU2EHI_CAN1_CANPack' */
  /* S-Function (scanpack): '<S5>/FCU2EHI_CAN1_CANPack' */
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.ID = 419366145U;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Length = 8U;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Extended = 1U;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Remote = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[1] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[2] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[3] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[4] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[5] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[6] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[7] = 0;

  {
    /* --------------- START Packing signal 0 ------------------
     *  startBit                = 7
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT0_c);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 7));
          }
        }
      }
    }

    /* --------------- START Packing signal 1 ------------------
     *  startBit                = 12
     *  length                  = 4
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT0_3_f);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(15)) {
          packedValue = (uint8_T) 15;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[1] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[1] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0xFU) << 4));
          }
        }
      }
    }

    /* --------------- START Packing signal 2 ------------------
     *  startBit                = 6
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT1_l);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 6));
          }
        }
      }
    }

    /* --------------- START Packing signal 3 ------------------
     *  startBit                = 5
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT2_i);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 5));
          }
        }
      }
    }

    /* --------------- START Packing signal 4 ------------------
     *  startBit                = 4
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT3_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 4));
          }
        }
      }
    }

    /* --------------- START Packing signal 5 ------------------
     *  startBit                = 3
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT4_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 3));
          }
        }
      }
    }

    /* --------------- START Packing signal 6 ------------------
     *  startBit                = 8
     *  length                  = 4
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT4_7_b);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(15)) {
          packedValue = (uint8_T) 15;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[1] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[1] | (uint8_T)((uint8_T)
              (packedValue & (uint8_T)0xFU));
          }
        }
      }
    }

    /* --------------- START Packing signal 7 ------------------
     *  startBit                = 2
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT5_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 2));
          }
        }
      }
    }

    /* --------------- START Packing signal 8 ------------------
     *  startBit                = 1
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT6_k);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 1));
          }
        }
      }
    }

    /* --------------- START Packing signal 9 ------------------
     *  startBit                = 0
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT7_m);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[0] | (uint8_T)((uint8_T)
              (packedValue & (uint8_T)0x1U));
          }
        }
      }
    }

    /* --------------- START Packing signal 10 ------------------
     *  startBit                = 24
     *  length                  = 16
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE2_3_c);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint16_T packedValue;
        packedValue = (uint16_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[3] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[3] | (uint8_T)((uint16_T)
              (packedValue & (uint16_T)0xFFU));
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[2] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[2] | (uint8_T)((uint16_T)
              ((uint16_T)(packedValue & (uint16_T)0xFF00U) >> 8));
          }
        }
      }
    }

    /* --------------- START Packing signal 11 ------------------
     *  startBit                = 32
     *  length                  = 16
     *  desiredSignalByteLayout = LITTLEENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE4_5_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint16_T packedValue;
        packedValue = (uint16_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[4] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[4] | (uint8_T)((uint16_T)
              (packedValue & (uint16_T)0xFFU));
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[5] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[5] | (uint8_T)((uint16_T)
              ((uint16_T)(packedValue & (uint16_T)0xFF00U) >> 8));
          }
        }
      }
    }

    /* --------------- START Packing signal 12 ------------------
     *  startBit                = 48
     *  length                  = 8
     *  desiredSignalByteLayout = LITTLEENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE6_o);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        packedValue = (uint8_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[6] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[6] | (uint8_T)(packedValue);
          }
        }
      }
    }

    /* --------------- START Packing signal 13 ------------------
     *  startBit                = 56
     *  length                  = 8
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE7_a);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        packedValue = (uint8_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[7] =
              FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[7] | (uint8_T)(packedValue);
          }
        }
      }
    }
  }

  /* S-Function (ecucoder_canmessage): '<S5>/CANPackMessage' */

  /*Pack CAN message*/
  {
    uint8 canpackloop= 0;
    FCU_F3NEW_B.CANPackMessage[0]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage[1]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage[2]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage[3]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage[4]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage[5]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage[6]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage[7]=
      FCU_F3NEW_B.FCU2EHI_CAN1_CANPack.Data[canpackloop];
    canpackloop++;
  }

  /* S-Function (scanpack): '<S5>/FCU2EHI_CAN2_CANPack' */
  /* S-Function (scanpack): '<S5>/FCU2EHI_CAN2_CANPack' */
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.ID = 419366146U;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Length = 8U;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Extended = 1U;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Remote = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[1] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[2] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[3] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[4] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[5] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[6] = 0;
  FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[7] = 0;

  {
    /* --------------- START Packing signal 0 ------------------
     *  startBit                = 7
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT0_c);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 7));
          }
        }
      }
    }

    /* --------------- START Packing signal 1 ------------------
     *  startBit                = 12
     *  length                  = 4
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT0_3_f);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(15)) {
          packedValue = (uint8_T) 15;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[1] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[1] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0xFU) << 4));
          }
        }
      }
    }

    /* --------------- START Packing signal 2 ------------------
     *  startBit                = 6
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT1_l);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 6));
          }
        }
      }
    }

    /* --------------- START Packing signal 3 ------------------
     *  startBit                = 5
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT2_i);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 5));
          }
        }
      }
    }

    /* --------------- START Packing signal 4 ------------------
     *  startBit                = 4
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT3_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 4));
          }
        }
      }
    }

    /* --------------- START Packing signal 5 ------------------
     *  startBit                = 3
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT4_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 3));
          }
        }
      }
    }

    /* --------------- START Packing signal 6 ------------------
     *  startBit                = 8
     *  length                  = 4
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT4_7_b);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(15)) {
          packedValue = (uint8_T) 15;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[1] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[1] | (uint8_T)((uint8_T)
              (packedValue & (uint8_T)0xFU));
          }
        }
      }
    }

    /* --------------- START Packing signal 7 ------------------
     *  startBit                = 2
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT5_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 2));
          }
        }
      }
    }

    /* --------------- START Packing signal 8 ------------------
     *  startBit                = 1
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT6_k);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              ((uint8_T)(packedValue & (uint8_T)0x1U) << 1));
          }
        }
      }
    }

    /* --------------- START Packing signal 9 ------------------
     *  startBit                = 0
     *  length                  = 1
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BIT7_m);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        if (packingValue > (uint8_T)(1)) {
          packedValue = (uint8_T) 1;
        } else {
          packedValue = (uint8_T) (packingValue);
        }

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[0] | (uint8_T)((uint8_T)
              (packedValue & (uint8_T)0x1U));
          }
        }
      }
    }

    /* --------------- START Packing signal 10 ------------------
     *  startBit                = 24
     *  length                  = 16
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE2_3_c);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint16_T packedValue;
        packedValue = (uint16_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[3] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[3] | (uint8_T)((uint16_T)
              (packedValue & (uint16_T)0xFFU));
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[2] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[2] | (uint8_T)((uint16_T)
              ((uint16_T)(packedValue & (uint16_T)0xFF00U) >> 8));
          }
        }
      }
    }

    /* --------------- START Packing signal 11 ------------------
     *  startBit                = 32
     *  length                  = 16
     *  desiredSignalByteLayout = LITTLEENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE4_5_e);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint16_T packedValue;
        packedValue = (uint16_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[4] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[4] | (uint8_T)((uint16_T)
              (packedValue & (uint16_T)0xFFU));
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[5] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[5] | (uint8_T)((uint16_T)
              ((uint16_T)(packedValue & (uint16_T)0xFF00U) >> 8));
          }
        }
      }
    }

    /* --------------- START Packing signal 12 ------------------
     *  startBit                = 48
     *  length                  = 8
     *  desiredSignalByteLayout = LITTLEENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE6_o);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        packedValue = (uint8_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[6] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[6] | (uint8_T)(packedValue);
          }
        }
      }
    }

    /* --------------- START Packing signal 13 ------------------
     *  startBit                = 56
     *  length                  = 8
     *  desiredSignalByteLayout = BIGENDIAN
     *  dataType                = UNSIGNED
     *  factor                  = 1.0
     *  offset                  = 0.0
     *  minimum                 = 0.0
     *  maximum                 = 0.0
     * -----------------------------------------------------------------------*/
    {
      uint32_T packingValue = 0;

      {
        uint32_T result = (uint32_T) (FCU_F3NEW_B.MEA_FCU2EHI_BYTE7_a);

        /* no scaling required */
        packingValue = result;
      }

      {
        uint8_T packedValue;
        packedValue = (uint8_T) (packingValue);

        {
          {
            FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[7] =
              FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[7] | (uint8_T)(packedValue);
          }
        }
      }
    }
  }

  /* S-Function (ecucoder_canmessage): '<S5>/CANPackMessage1' */

  /*Pack CAN message*/
  {
    uint8 canpackloop= 0;
    FCU_F3NEW_B.CANPackMessage1[0]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage1[1]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage1[2]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage1[3]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage1[4]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage1[5]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage1[6]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
    FCU_F3NEW_B.CANPackMessage1[7]=
      FCU_F3NEW_B.FCU2EHI_CAN2_CANPack.Data[canpackloop];
    canpackloop++;
  }

  /* S-Function (ec5743_cantransmitslb): '<S5>/CANTransmit' */

  /*Transmit CAN message*/
  {
    uint8 CAN1BUF40TX[8];
    uint8 can1buf40looptx= 0;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[0];
    can1buf40looptx++;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[1];
    can1buf40looptx++;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[2];
    can1buf40looptx++;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[3];
    can1buf40looptx++;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[4];
    can1buf40looptx++;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[5];
    can1buf40looptx++;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[6];
    can1buf40looptx++;
    CAN1BUF40TX[can1buf40looptx]= FCU_F3NEW_B.CANPackMessage[7];
    can1buf40looptx++;
    FCU_F3NEW_B.CANTransmit_p= ec_can1_transmit(1, 1, 419366145U, 8, CAN1BUF40TX);
  }

  /* S-Function (ec5743_cantransmitslb): '<S5>/CANTransmit1' */

  /*Transmit CAN message*/
  {
    uint8 CAN2BUF40TX[8];
    uint8 can2buf40looptx= 0;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[0];
    can2buf40looptx++;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[1];
    can2buf40looptx++;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[2];
    can2buf40looptx++;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[3];
    can2buf40looptx++;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[4];
    can2buf40looptx++;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[5];
    can2buf40looptx++;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[6];
    can2buf40looptx++;
    CAN2BUF40TX[can2buf40looptx]= FCU_F3NEW_B.CANPackMessage1[7];
    can2buf40looptx++;
    FCU_F3NEW_B.CANTransmit1= ec_can2_transmit(1, 1, 419366145U, 8, CAN2BUF40TX);
  }

  /* End of Outputs for S-Function (fcncallgen): '<S3>/200ms' */
}

/* Model step wrapper function for compatibility with a static main program */
void FCU_F3NEW_step(int_T tid)
{
  switch (tid) {
   case 0 :
    FCU_F3NEW_step0();
    break;

   case 1 :
    FCU_F3NEW_step1();
    break;

   case 2 :
    FCU_F3NEW_step2();
    break;

   case 3 :
    FCU_F3NEW_step3();
    break;

   case 4 :
    FCU_F3NEW_step4();
    break;

   case 5 :
    FCU_F3NEW_step5();
    break;

   case 6 :
    FCU_F3NEW_step6();
    break;

   default :
    break;
  }
}

/* Model initialize function */
void FCU_F3NEW_initialize(void)
{
  /* Start for S-Function (fcncallgen): '<S3>/10ms' incorporates:
   *  SubSystem: '<S3>/CAN_RX'
   */
  /* Start for S-Function (ec5743_canreceiveslb): '<S4>/CAN1_CANReceive' incorporates:
   *  SubSystem: '<S4>/CAN1_Check'
   */
  /* Start for function-call system: '<S4>/CAN1_Check' */

  /* Start for Enabled SubSystem: '<S6>/Subsystem' */

  /* Start for S-Function (scanunpack): '<S8>/FCU_CAN1_CANUnPack' */

  /*-----------S-Function Block: <S8>/FCU_CAN1_CANUnPack -----------------*/

  /* End of Start for SubSystem: '<S6>/Subsystem' */
  ec_buffer1_init(0,1,419365125,0);

  /* End of Start for S-Function (ec5743_canreceiveslb): '<S4>/CAN1_CANReceive' */

  /* Start for S-Function (ec5743_canreceiveslb): '<S4>/CAN2_CANReceive' */
  ec_buffer2_init(0,1,419365381,0);

  /* End of Start for S-Function (ec5743_canreceiveslb): '<S4>/CAN2_CANReceive' */
  /* End of Start for S-Function (fcncallgen): '<S3>/10ms' */
  /* Start for S-Function (ec5743_cantransmitslb): '<S5>/CANTransmit' */
  ec_buffer1_init(1,1,419366145U,1);

  /* Start for S-Function (ec5743_cantransmitslb): '<S5>/CANTransmit1' */
  ec_buffer2_init(1,1,419366145U,1);

  /* End of Start for S-Function (fcncallgen): '<S3>/200ms' */

  /* Start for S-Function (fcncallgen): '<S10>/100MS' incorporates:
   *  SubSystem: '<S10>/Function-Call Subsystem'
   */
  /* Start for S-Function (ec5743_canreceiveslb): '<S14>/CANReceive' incorporates:
   *  SubSystem: '<S14>/Function-Call Subsystem'
   */
  /* Start for function-call system: '<S14>/Function-Call Subsystem' */

  /* Start for Enabled SubSystem: '<S15>/Enabled Subsystem' */
  /* Start for IfAction SubSystem: '<S16>/If Action Subsystem1' */
  /* Start for S-Function (ec5743_cantransmitslb): '<S21>/CANTransmit' incorporates:
   *  Constant: '<S21>/Constant'
   */
  /*ec_buffer3_init(41,0,593U);*/

  /* End of Start for SubSystem: '<S16>/If Action Subsystem1' */
  /* End of Start for SubSystem: '<S15>/Enabled Subsystem' */
  /*ec_buffer3_init(1,0,278);*/

  /* End of Start for S-Function (ec5743_canreceiveslb): '<S14>/CANReceive' */
  /* End of Start for S-Function (fcncallgen): '<S10>/100MS' */

  /* Start for S-Function (fcncallgen): '<S11>/100MS' incorporates:
   *  SubSystem: '<S11>/Function-Call Subsystem'
   */
  /* Start for S-Function (ec5743_canreceiveslb): '<S22>/CANReceive' incorporates:
   *  SubSystem: '<S22>/Function-Call Subsystem'
   */
  /* Start for function-call system: '<S22>/Function-Call Subsystem' */

  /* Start for Enabled SubSystem: '<S23>/Enabled Subsystem' */
  /* Start for IfAction SubSystem: '<S24>/If Action Subsystem1' */
  /* Start for S-Function (ec5743_cantransmitslb): '<S29>/CANTransmit' incorporates:
   *  Constant: '<S29>/Constant'
   */
  /*ec_buffer3_init(42,0,161U);*/

  /* End of Start for SubSystem: '<S24>/If Action Subsystem1' */
  /* End of Start for SubSystem: '<S23>/Enabled Subsystem' */
  /*ec_buffer3_init(2,0,176);*/

  /* End of Start for S-Function (ec5743_canreceiveslb): '<S22>/CANReceive' */
  /* End of Start for S-Function (fcncallgen): '<S11>/100MS' */

  /* Start for S-Function (fcncallgen): '<S13>/Function-Call Generator' incorporates:
   *  SubSystem: '<S13>/CCPBackground'
   */
  /* Start for S-Function (ec5743_ccpslb): '<S34>/CCPBackground' */
  /*ccpInit();*/

  /* End of Start for S-Function (fcncallgen): '<S13>/Function-Call Generator' */

  /* Start for S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt' incorporates:
   *  SubSystem: '<S13>/CCPReceive'
   */
  /* Start for function-call system: '<S13>/CCPReceive' */

  /* Start for S-Function (ec5743_canreceiveslb): '<S35>/CANReceive1' */
  /*ec_buffer3_init(0,0,CCP_CRO_ID);*/

  /* End of Start for S-Function (ec5743_canreceiveslb): '<S35>/CANReceive1' */
  /*ec_bufint3_init(0);
  INTC_0.PSR[697].B.PRIN = 7;
  IntcIsrVectorTable[697] = (uint32_t)&ISR_FlexCAN_3_MB0;*/

  /* End of Start for S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt' */

  /* Start for S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt1' */
  /*ec_bufint3_init(40);
  INTC_0.PSR[700].B.PRIN = 10;
  IntcIsrVectorTable[700] = (uint32_t)&ISR_FlexCAN_3_MB40;*/

  /* End of Start for S-Function (ec5743_caninterruptslb1): '<S13>/ReceiveandTransmitInterrupt1' */

  /* Start for S-Function (ec5743_f3newbsb): '<S2>/XCS' */
  /*ec_spi3_init();
  ec_sent1_init();

  //SENT FMSG ISR INIT
  INTC_0.PSR[579].B.PRIN = 9;
  IntcIsrVectorTable[579] = (uint32)&ISR_sent1_Channel0_FAST;

  //SENT FMSG ISR INIT
  INTC_0.PSR[582].B.PRIN = 7;
  IntcIsrVectorTable[582] = (uint32)&ISR_sent1_Channel1_FAST;

  //SENT FMSG ISR INIT
  INTC_0.PSR[585].B.PRIN = 7;
  IntcIsrVectorTable[585] = (uint32)&ISR_sent1_Channel2_FAST;

  //SENT1CH0 SMSG ISR INIT
  INTC_0.PSR[580].B.PRIN = 8;
  IntcIsrVectorTable[580] = (uint32)&ISR_sent1_Channel0_SLOW;

  //SENT1CH1 SMSG ISR INIT
  INTC_0.PSR[583].B.PRIN = 8;
  IntcIsrVectorTable[583] = (uint32)&ISR_sent1_Channel1_SLOW;

  //SENT1CH2 SMSG ISR INIT
  INTC_0.PSR[586].B.PRIN = 8;
  IntcIsrVectorTable[586] = (uint32)&ISR_sent1_Channel2_SLOW;
  ec_Motor_HSD_LSD_Init();*/

  /* Read all data for FRAM address */
}

/* File trailer for ECUCoder generated file FCU_F3NEW.c.
 *
 * [EOF]
 */
