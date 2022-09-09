/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : adConv1.c
**     Project     : FCU_V4
**     Processor   : S32K144_100
**     Component   : adc
**     Version     : Component SDK_S32K14x_09, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_09
**     Compiler    : GNU C Compiler
**     Date/Time   : 2022-09-07, 16:32, # CodeGen: 505
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file adConv1.c
** @version 01.00
*/
/*!
**  @addtogroup adConv1_module adConv1 module documentation
**  @{
*/

/* MODULE adConv1. */

#include "adConv1.h"

/*! adConv1 configuration structure */
const adc_converter_config_t adConv1_ConvConfig0 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig1 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig2 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig3 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig4 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig5 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig6 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig7 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig8 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig9 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

const adc_converter_config_t adConv1_ConvConfig10 = {
  .clockDivide = ADC_CLK_DIVIDE_1,
  .sampleTime = 12U,
  .resolution = ADC_RESOLUTION_12BIT,
  .inputClock = ADC_CLK_ALT_1,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .pretriggerSel = ADC_PRETRIGGER_SEL_PDB,
  .triggerSel = ADC_TRIGGER_SEL_PDB,
  .dmaEnable = false,
  .voltageRef = ADC_VOLTAGEREF_VREF,
  .continuousConvEnable = false,
  .supplyMonitoringEnable = false,
};

adc_chan_config_t adConv1_ChnConfig0 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT12,
};

const adc_chan_config_t adConv1_ChnConfig1 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT7,
};

const adc_chan_config_t adConv1_ChnConfig2 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT6,
};

const adc_chan_config_t adConv1_ChnConfig3 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT5,
};

const adc_chan_config_t adConv1_ChnConfig4 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT4,
};

const adc_chan_config_t adConv1_ChnConfig5 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT3,
};

const adc_chan_config_t adConv1_ChnConfig6 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT2,
};

const adc_chan_config_t adConv1_ChnConfig7 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT13,
};

const adc_chan_config_t adConv1_ChnConfig8 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT8,
};

const adc_chan_config_t adConv1_ChnConfig9 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT9,
};

const adc_chan_config_t adConv1_ChnConfig10 = {
  .interruptEnable = false,
  .channel = ADC_INPUTCHAN_EXT10,
};

const adc_compare_config_t adConv1_HwCompConfig0 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig1 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig2 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig3 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig4 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig5 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig6 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig7 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig8 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig9 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_compare_config_t adConv1_HwCompConfig10 = {
  .compareEnable = false,
  .compareGreaterThanEnable = false,
  .compareRangeFuncEnable = false,
  .compVal1 = 0U,
  .compVal2 = 0U,
};

const adc_average_config_t adConv1_HwAvgConfig0 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig1 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig2 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig3 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig4 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig5 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig6 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig7 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig8 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig9 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};

const adc_average_config_t adConv1_HwAvgConfig10 = {
  .hwAvgEnable = false,
  .hwAverage = ADC_AVERAGE_4,
};


/* END adConv1. */
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
