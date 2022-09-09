/*
 * CAN1_TX.h
 *
 *  Created on: 2018年9月13日
 *      Author: Administrator
 */

#ifndef CAN1_TX_H_
#define CAN1_TX_H_
#define CANAPP_EXT extern


extern uint16_t Send_Tank_Pre;
extern uint8_t S_VCU_Stack_State;//发送给VCU的电堆状态
extern uint8_t S_FCU_Fault_Level;//FCU故障等级
extern uint8_t FCU_CANLife;//FCU CANLife
extern uint8_t Down_DCDC_CANLife; //降压DCDC life
extern uint16_t sys_run_cuont; //系统运行次数
extern uint8_t S_SJKYJ_VCU_State;//空压机状态
extern uint8_t S_H_Pump_VCU_State;//氢气循环泵状态
extern uint8_t S_Pump_VCU_State;//高压水泵状态
extern uint8_t S_H_Pump_VCU_State;//主散热风扇状态


typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t                                                   : 4;
		uint8_t             CAN1_FCU1_s04_fcu_fault_level         : 4;

		uint8_t                                                   : 8;
		uint8_t                                                   : 8;

		uint8_t             CAN1_FCU1_s24_fcu_main_state          : 4;
		uint8_t                                                   : 4;

		uint8_t             CAN1_FCU1_s32_fcu_power               : 8;
		uint8_t             CAN1_FCU1_s40_fcu_IdleSpeed_power     : 8;
		uint8_t             CAN1_FCU1_s48_fcu_allow_power         : 8;
		uint8_t             CAN1_FCU1_s56_fcu_can_life            : 8;

    }Bits;
}t_TxMsgCAN1_FCU1;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU2_s00_fcu_stack_vol_L         : 8;

		uint8_t             CAN1_FCU2_s00_fcu_stack_vol_H         : 4;
		uint8_t                                                   : 4;

		uint8_t             CAN1_FCU2_s16_fcu_stack_crt_L         : 8;
		uint8_t             CAN1_FCU2_s16_fcu_stack_crt_H         : 4;
		uint8_t                                                   : 4;

		uint8_t             CAN1_FCU2_s32_air_pump_state          : 2;
		uint8_t             CAN1_FCU2_s34_h2_pump_state           : 2;
		uint8_t             CAN1_FCU2_s36_water_pump_state        : 2;
		uint8_t             CAN1_FCU2_s38_heat_sink_state         : 2;

		uint8_t             CAN1_FCU2_s40_up_dcdc_fault1_code     : 8;
		uint8_t             CAN1_FCU2_s48_up_dcdc_fault2_code     : 8;
		uint8_t             CAN1_FCU2_s56_up_dcdc_fault3_code     : 8;

    }Bits;
}t_TxMsgCAN1_FCU2;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU3_s00_fcu_fault1_code_L       : 8;
		uint8_t             CAN1_FCU3_s00_fcu_fault1_code_H       : 8;

		uint8_t             CAN1_FCU3_s16_fcu_fault2_code_L       : 8;
		uint8_t             CAN1_FCU3_s16_fcu_fault2_code_H       : 8;

		uint8_t             CAN1_FCU3_s32_fcu_fault3_code_L       : 8;
		uint8_t             CAN1_FCU3_s32_fcu_fault3_code_H       : 8;

		uint8_t             CAN1_FCU3_s48_fcu_fault4_code_L       : 8;
		uint8_t             CAN1_FCU3_s48_fcu_fault4_code_H       : 8;

    }Bits;
}t_TxMsgCAN1_FCU3;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU4_s00_fcu_run_tiem_L          : 8;
		uint8_t             CAN1_FCU4_s00_fcu_run_tiem_H          : 8;

		uint8_t             CAN1_FCU4_s16_fcu_run_count_L         : 8;
		uint8_t             CAN1_FCU4_s16_fcu_run_count_H         : 8;

		uint8_t             CAN1_FCU4_s32_fcu_total_power_L       : 8;
		uint8_t             CAN1_FCU4_s32_fcu_total_power_ML      : 8;
		uint8_t             CAN1_FCU4_s32_fcu_total_power_MH      : 8;
		uint8_t             CAN1_FCU4_s32_fcu_total_power_H       : 8;

    }Bits;
}t_TxMsgCAN1_FCU4;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t                                                   : 8;
		uint8_t                                                   : 4;
		uint8_t             CAN1_FCU6_s12_water_inp_temp_L        : 4;
		uint8_t             CAN1_FCU6_s12_water_inp_temp_H        : 8;

		uint8_t             CAN1_FCU6_s24_water_oup_temp_L        : 8;
		uint8_t             CAN1_FCU6_s24_water_oup_temp_H        : 4;
		uint8_t                                                   : 4;

		uint8_t                                                   : 8;
		uint8_t                                                   : 8;

		uint8_t             CAN1_FCU6_s56_fcu_h2_temp_num         : 2;
		uint8_t             CAN1_FCU6_s58_main_sink_water_state   : 1;
		uint8_t             CAN1_FCU6_s59_fcu2dcdc_enable         : 1;
		uint8_t                                                   : 4;

    }Bits;
}t_TxMsgCAN1_FCU6;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU7_s00_water_pump_power        : 8;
		uint8_t             CAN1_FCU7_s08_heat_sink_power         : 8;
		uint8_t             CAN1_FCU7_s16_air_pump_power          : 8;
		uint8_t             CAN1_FCU7_s24_h2_pump_power           : 8;

		uint8_t             CAN1_FCU7_s32_ptc_power               : 8;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;

    }Bits;
}t_TxMsgCAN1_FCU7;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU8_s00_air_pump_com_temp       : 8;
		uint8_t             CAN1_FCU8_s08_air_pump_temp           : 8;
		uint8_t             CAN1_FCU8_s16_air_pump_airin_temp     : 8;
		uint8_t             CAN1_FCU8_s24_air_pump_vol_L          : 8;

		uint8_t             CAN1_FCU8_s32_air_pump_vol_H          : 8;
		uint8_t             CAN1_FCU8_s40_air_pump_cur_L          : 8;
		uint8_t             CAN1_FCU8_s48_air_pump_cur_H          : 8;
		uint8_t                                                   : 8;

    }Bits;
}t_TxMsgCAN1_FCU8;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU9_s00_soft_version_year       : 8;
		uint8_t             CAN1_FCU9_s08_soft_version_month      : 8;
		uint8_t             CAN1_FCU9_s16_soft_version_day        : 8;
		uint8_t             CAN1_FCU9_s24_soft_version_num        : 8;

		uint8_t             CAN1_FCU9_s32_fcu_factory             : 8;
		uint8_t             CAN1_FCU9_s40_fcu_recharge_mileage    : 8;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;

    }Bits;
}t_TxMsgCAN1_FCU9;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU10_s00_down_dcdc_state        : 2;
		uint8_t                                                   : 6;
		uint8_t             CAN1_FCU10_s08_down_dcdc_out_vol_L    : 8;
		uint8_t             CAN1_FCU10_s08_down_dcdc_out_vol_H    : 8;

		uint8_t             CAN1_FCU10_s24_down_dcdc_out_crt_L    : 8;
		uint8_t             CAN1_FCU10_s24_down_dcdc_out_crt_H    : 8;

		uint8_t             CAN1_FCU10_s40_down_dcdc_in_vol_L     : 8;
		uint8_t             CAN1_FCU10_s40_down_dcdc_in_vol_H     : 8;
		uint8_t             CAN1_FCU10_s56_down_dcdc_temp         : 8;

    }Bits;
}t_TxMsgCAN1_FCU10;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU11_s00_down_dcdc_fault1       : 1; //输入欠压
		uint8_t             CAN1_FCU11_s01_down_dcdc_fault2       : 1; //输入过压
		uint8_t             CAN1_FCU11_s02_down_dcdc_fault3       : 1; //输出过压
		uint8_t             CAN1_FCU11_s03_down_dcdc_fault4       : 1; //输出欠压
		uint8_t             CAN1_FCU11_s04_down_dcdc_fault5       : 1; //限流
		uint8_t             CAN1_FCU11_s05_down_dcdc_fault6       : 1; //过流
		uint8_t             CAN1_FCU11_s06_down_dcdc_fault7       : 1; //短路
		uint8_t             CAN1_FCU11_s07_down_dcdc_fault8       : 1; //过温

		uint8_t             CAN1_FCU11_s08_down_dcdc_fault9       : 1; //硬件故障
		uint8_t             CAN1_FCU11_s09_down_dcdc_fault10      : 1; //限功率
		uint8_t             CAN1_FCU11_s10_down_dcdc_fault11      : 1; //通讯超时
		uint8_t                                                   : 1;
		uint8_t             CAN1_FCU11_s12_down_dcdc_life         : 4; //dc life

		uint8_t             CAN1_FCU11_s16_down_dcdc_soft_version : 8;

		uint8_t                                                   : 8;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;

    }Bits;
}t_TxMsgCAN1_FCU11;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;
		uint8_t             CAN1_FCU12_s32_down_h2_pump_vol_L     : 8;
		uint8_t             CAN1_FCU12_s40_down_h2_pump_vol_H     : 8;
		uint8_t             CAN1_FCU12_s48_down_h2_pump_crt_L     : 8;
		uint8_t             CAN1_FCU12_s56_down_h2_pump_crt_H     : 8;

    }Bits;
}t_TxMsgCAN1_FCU12;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU13_s00_stack_total_pcs_L      : 8;
		uint8_t             CAN1_FCU13_s00_stack_total_pcs_H      : 8;

		uint8_t             CAN1_FCU13_s16_single_pcs_min_vol_L   : 8;
		uint8_t             CAN1_FCU13_s16_single_pcs_min_vol_H   : 4;

		uint8_t             CAN1_FCU13_s28_number_min_voltage_L   : 4;
		uint8_t             CAN1_FCU13_s28_number_min_voltage_H   : 8;

		uint8_t             CAN1_FCU13_s40_single_pcs_max_vol_L   : 8;
		uint8_t             CAN1_FCU13_s40_single_pcs_max_vol_H   : 4;
		uint8_t             CAN1_FCU13_s52_number_max_voltage_L   : 4;
		uint8_t             CAN1_FCU13_s52_number_max_voltage_H   : 8;

    }Bits;
}t_TxMsgCAN1_FCU13;

typedef union
{
	uint8_t   DataFld[8];
    struct
    {
		uint8_t             CAN1_FCU14_s00_up_dcdc_voltage_L      : 8;
		uint8_t             CAN1_FCU14_s00_up_dcdc_voltage_H      : 8;

		uint8_t             CAN1_FCU14_s16_up_dcdc_current_L      : 8;
		uint8_t             CAN1_FCU14_s16_up_dcdc_current_H      : 8;

		uint8_t             CAN1_FCU14_s32_up_dcdc_temp           : 8;
		uint8_t             CAN1_FCU14_s40_up_dcdc_state          : 4;
		uint8_t             CAN1_FCU13_s44_up_dcdc_fault_level    : 4;
		uint8_t                                                   : 8;
		uint8_t                                                   : 8;

    }Bits;
}t_TxMsgCAN1_FCU14;


CANAPP_EXT    t_TxMsgCAN1_FCU1             p_TxMsgCAN1_FCU1              ;
CANAPP_EXT    t_TxMsgCAN1_FCU2             p_TxMsgCAN1_FCU2              ;
CANAPP_EXT    t_TxMsgCAN1_FCU3             p_TxMsgCAN1_FCU3              ;
CANAPP_EXT    t_TxMsgCAN1_FCU4             p_TxMsgCAN1_FCU4              ;
//CANAPP_EXT    t_TxMsgCAN1_FCU5             p_TxMsgCAN1_FCU5              ;
CANAPP_EXT    t_TxMsgCAN1_FCU6             p_TxMsgCAN1_FCU6              ;
CANAPP_EXT    t_TxMsgCAN1_FCU7             p_TxMsgCAN1_FCU7              ;
CANAPP_EXT    t_TxMsgCAN1_FCU8             p_TxMsgCAN1_FCU8              ;
CANAPP_EXT    t_TxMsgCAN1_FCU9             p_TxMsgCAN1_FCU9              ;
CANAPP_EXT    t_TxMsgCAN1_FCU10            p_TxMsgCAN1_FCU10             ;
CANAPP_EXT    t_TxMsgCAN1_FCU11            p_TxMsgCAN1_FCU11             ;
CANAPP_EXT    t_TxMsgCAN1_FCU12            p_TxMsgCAN1_FCU12             ;
CANAPP_EXT    t_TxMsgCAN1_FCU13            p_TxMsgCAN1_FCU13             ;
CANAPP_EXT    t_TxMsgCAN1_FCU14            p_TxMsgCAN1_FCU14             ;

void CAN1_Send_Out100(void);
void CAN1_Send_Out200(void);
void CAN1_Send_Out1000(void);
void CAN1_Send_Out2(void);
#endif /* CAN1_TX_H_ */
