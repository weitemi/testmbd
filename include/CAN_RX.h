/*
 * CAN_RX.h
 *
 *  Created on: 2018年8月27日
 *      Author: Administrator
 */

#ifndef CAN_RX_H_
#define CAN_RX_H_


uint8_t R_REL_Data[8];//存放继电器命令的数据
uint8_t R_GBD_Data[8];//存放故障标定的数据
uint8_t R_CBD_Data[8];//存放参数标定的数据
uint8_t R_RW_Data[8];//存放握手的数据
uint8_t R_DC_MAN_Data[8];//存放升压DC手动控制的数据
uint8_t R_JYFBD_Data[8];//存放减压阀标定的数据
uint8_t R_KQBYF_QXHB_PUMP_Data[8];//接收背压阀、氢气循环泵、电子水泵手动控制数据


uint8_t R_SJKYJ_Data[8];//接收空压机手动控制数据
uint8_t R_JWQ_PTC_6KWDC_Data[8];//接收节温器手动控制数据
uint8_t ZD_RW;
uint8_t Small_DC_State;//接收降压DC状态

//接收水泵的数据
uint16_t Pump_Speed,Pump_Voltage;//水泵转速
uint8_t Pump_State1,Pump_Current,Pump_State2;//水泵状态1，状态2
uint8_t ZD_Pump;//查询水泵报文是否丢帧标志位
int16_t Pump_Speed_Dif_Val;//目标转速与实际转速差值
//接收氢气循环泵的数据
uint16_t H_Pump_Speed;//氢气循环泵转速
uint8_t H_Pump_State1,H_Pump_Power;//氢气循环泵状态1
uint8_t ZD_H_Pump;//查询氢气循环泵报文是否丢帧标志位
int16_t H_Pump_Speed_Dif_Val;//氢气循环目标转速与实际转速差值
int16_t H_Pump_Voltage;//输入电压
//喷轨减压阀
uint8_t JYF_State;
uint8_t ZD_JYF;//查询减压阀报文是否丢帧标志位
//空气背压阀数据
uint8_t ZD_KQBYF;//查询背压阀报文是否丢帧标志位
int8_t KQBYF_Dif_Angle;//背压阀角度误差
uint8_t KQBYF_State1;//背压阀状态
//势加透波空压报文1数据
uint8_t ZD_SJKYJ1;//查询空压机报文1是否丢帧标志位
uint8_t SJKYJ_State;//空压机开启还是关闭状态
uint32_t SJKYJ_Speed;//空压机转速
int16_t SJKYJ_Speed_Dif_Val;//实际转速与目标转速差值
uint8_t SJKYJ_ID1_State;//报文1的状态 0代表正常 1代表异常
uint16_t SJKYJ_Power;//空压机功率
uint16_t SJKYJ_Bus_V;//空压机母线电压
uint8_t SJKYJ_Bus_I;//空压机母线电流
//势加透波空压报文2数据
uint8_t ZD_SJKYJ2;//查询空压机报文2是否丢帧标志位
uint8_t SJKYJ_Temp;//空压机控制器温度
uint8_t SJKYJ_MotorTemp;//空压机电机温度
uint8_t SJKYJ_Fualt_Class;//空压机故障等级
uint16_t SJKYJ_Fualt_Code3;//空压机3级故障（不可恢复故障）
uint16_t SJKYJ_Fualt_Code2;//空压机2级故障（可恢复故障）
uint16_t SJKYJ_Fualt_Code1;//空压机1级故障（告警）
//升压DCDC报文1数据
uint8_t ZD_DC1;//DCDC的报文1丢帧标志位
uint16_t DC_IN_V,DC_IN_I;//输入电压电流  输入电压单位V  输入电流单位A
uint32_t Stack_P;//电堆总功率 单位W
uint8_t DC_State,DC_Temp,DC_Fault,DC_Fault_code;//升压 DC状态，DC温度,DC故障等级
//升压DCDC报文2数据
uint8_t ZD_DC2;//DCDC的报文2丢帧标志位
uint16_t DC_OUT_I,DC_OUT_V;//输出电压电流   输出电压单位V  输出电流单位0.1A
uint8_t DC_Rec_State;//DC接受工作状态
uint16_t DC_Rec_I;//DC接受工作电流
uint8_t DC_ID2_State;//DC报文2的状态 0代表正常 1代表异常
//CVM1报文数据
uint8_t ZD_CVM11,ZD_CVM12,ZD_CVM13;
uint32_t Stack11_Average_Voltage,Stack11_L_Voltage,Stack1_Total_Voltage;
uint16_t Stack12_NumberMin,Stack12_VoltageMin;
uint16_t Stack12_NumberMax,Stack12_VoltageMax;
//CVM2报文数据
uint8_t ZD_CVM21,ZD_CVM22;
uint32_t Stack21_Average_Voltage,Stack61_L_Voltage,Stack2_Total_Voltage;
uint16_t Stack22_NumberMin,Stack22_VoltageMin;
uint16_t Stack22_NumberMax,Stack22_VoltageMax;

uint32_t Stack_Total_Voltage;//电堆总电压 精度1mV
uint16_t Stack_High_Voltage;//单片最大电压 精度1mV
uint16_t Stack_High_Number;//单片最大电压编号
uint16_t Stack_Low_Voltage;//单片最小电压 精度1mV
uint16_t Stack_Low_Number;//单片最小电压编号
uint16_t Stack1_Max_V_Dif;//电堆1单片最大压差 精度1MV
uint16_t Stack2_Max_V_Dif;//电堆2单片最大压差 精度1MV
uint16_t Stack_Average_Voltage;//单片平均电压 精度1mV
uint8_t CVM1_Flag,CVM2_Flag;//6块采集器报文标志位

//降压DCDC
uint16_t DCDC_Out_voltage;
uint16_t DCDC_In_voltage;
uint16_t DCDC_Out_current;
uint8_t DCDC_temp;
uint8_t DCDC_work_state;
uint16_t DCDC_fault_code;
uint16_t DCDC_fault_level;//降压DC故障等级

//三通阀报文数据
uint16_t Valve_State;//状态
uint16_t Valve_fault_code;//故障码
uint16_t Valve_Open;//阀开度

//PTC
uint16_t PTC_In_voltage;// PTC输入电压
uint16_t PTC_In_current;// PTC输入电流
uint16_t PTC_Fault_Code;// PTC故障码

//故障查询6秒延时标志位
uint8_t Flag_6S;
uint8_t Flag_12S;
uint8_t HProtectFlag_6S;

uint16_t Test_11,Test_12;

void CAN0_Receive(void);//CAN0接收部件报文函数
void CAN0_Receive_COM(void);//CAN0接收上位机报文函数
void ID_Lose_Check(void);//丢帧报文查询

#endif /* CAN_RX_H_ */
