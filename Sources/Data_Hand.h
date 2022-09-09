/*
 * Data_Hand.h
 *
 *  Created on: 2018年8月28日
 *      Author: Administrator
 */

#ifndef DATA_HAND_H_
#define DATA_HAND_H_



//排气时间，排气间隔
extern uint8_t Out_Time;
extern uint16_t Out_Interva,Out_Interva_Old;
extern uint8_t Out_H_Flag;//排氢标志位  1代表手动排氢 0代表自动排氢 如果设置它大于1则是固定排氢100ms*Out_H_Flag
//ON档标志位 停止标志位
uint8_t ON_Flag,Stop_Flag;
uint8_t Fan_Speed_Data;//风扇速度值
//总体故障等级标志位
uint16_t Fault_Level_Two,Fault_Level_Three,Fault_Level_Four,Fault_Level_Five;
extern uint8_t S_Fan_Speed;
//CAN0发送给上位机整车的一些状态
uint8_t S_VCU_State;//总负，ON档，急停，Ready等信号
uint16_t S_Fan_Speed_Change;//风扇补偿量（根据电堆出口温度限值增加风扇转速）
uint16_t HProtectFlag_10S;//上Ready后计时10S

//电堆温度控制
void Temp_Con(void);
//ON档检测
void ON_Read(void);
//自动排氢函数
void Out_H(void);
//运行中故障检测函数
void Fault_Check(void);
//风扇调速函数
void Fan_Speed(void);

void Fan_Speed2(uint8_t open);

//下面两个函数不需要放到main函数里面
//电堆氢气压力
void H_Con(void);
//启动条件判断
void Start_Up_Condition(void);
//电堆发电量计算
void Stack_P_CAL(void);
//液位监控函数
void YeWei_Stack(void);

#endif /* DATA_HAND_H_ */
