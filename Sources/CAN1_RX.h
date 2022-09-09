/*
 * CAN1_RX.h
 *
 *  Created on: 2018年9月13日
 *      Author: Administrator
 */

#ifndef CAN1_RX_H_
#define CAN1_RX_H_

uint8_t Soc_Data;//SOC值
uint16_t Battery_H_Voltage;//单体最高电压
uint16_t Battery_L_Voltage;//单体最低电压
uint16_t Battery_T_Voltage;//电池总电压
uint8_t bms_main_key1;//VCU Ready状态
uint8_t VCU_Ready_State;//VCU Ready状态
uint8_t full_power_enable;//VCU 发送满功率
uint8_t vcu2fcu_command;//VCU发送FCU指令
uint8_t crash_state;//VCU发送碰撞状态
uint8_t power_model;//VCU发送纯电模式 燃电或纯电
uint8_t add_h2_door_state;//VCU发送加氢舱门状态
uint8_t urgent_switch_state;//VCU 急停开关状态
uint8_t vcu2fcu_limit_power;//VCU 整车限制功率
uint8_t vcu2fcu_can_life;//VCU 心跳
uint8_t Car_Speed;//车速
uint16_t Res_Data1,Res_Data2;//绝缘阻值
uint16_t Battery_T_I;//电池总电流
uint16_t Bms_Max_Recharge_I;//电池最大充电电流

uint16_t SDC_Out_V;//小DC输出电压
uint16_t SDC_Out_I;//小DC输出电流
uint8_t hms_fault_level,hms_fault_code;//hms故障等级
uint8_t hms_state;//hms 状态
uint8_t hms_h2_soc;//hms SOC

uint16_t hms_h2_pressure;// 氢气压力
uint32_t vehicle_mileage;// 整车里程
uint8_t Low_Power_Flag;//低功率运行标记


void CAN1_Receive(void);

#endif /* CAN1_RX_H_ */
