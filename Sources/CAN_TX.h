/*
 * CAN_TX.h
 *
 *  Created on: 2018年8月27日
 *      Author: Administrator
 */

#ifndef CAN_TX_H_
#define CAN_TX_H_


/*************************************************************/
/*                       发送给电子水泵数据                   */
/*************************************************************/
extern uint8_t S_PUMP_Data[8];//
extern uint8_t PUMP_Enable;//电子水泵使能状态
extern uint16_t Pump_Target_Speed;//电子水泵目标转速
extern uint8_t Water_Pre_State_Flag;//水压状态标志位 0代表正常 其他都是代表异常
/*************************************************************/
/*                       发送给氢循环泵数据                    */
/*************************************************************/
extern uint8_t S_QXHB_Data[8];//
extern uint16_t H_Pump_Target_Speed;//氢气循环泵目标转速
extern uint8_t H_PUMP_Enable;//氢气循环泵使能状态 初始设置为不使能状态
/*************************************************************/
/*                        发送给减压阀数据                     */
/*************************************************************/
extern uint8_t S_JYF_Data[8];//默认发送关闭定值模式
/*************************************************************/
/*                      发送给空气背压阀数据                   */
/*************************************************************/
extern uint8_t S_KQBYF_Data[8];//
extern uint8_t KQBYF_Target_Angle;//背压阀目标角度
extern uint16_t LowTempAngleCompensate;//背压阀低温下电角度补偿
/*************************************************************/
/*                        发送给DCDC ID                      */
/*************************************************************/
extern uint8_t S_DC_Data[8];//
extern uint8_t S_DC_State;//关机 0X05关机 0X0B开机 0X0C主动放电  0X03预充
extern uint16_t S_DC_I;//DC输入电流值 单位0.1A
extern uint16_t DC_Target_I;//升压DC目标电流 精度1A
int16_t I_Cha;
extern int16_t DC_Target_Old;
extern uint8_t Up_DCDC_Enable;
extern uint8_t LowTemp_FanCon_Flag;//低温风扇切换控制标志位
/*************************************************************/
/*                    发送给势加透波空压机  ID                 */
/*************************************************************/
extern uint8_t S_SJKYJ_Data[8];//
extern uint32_t SJKYJ_Target_Speed;//空压机目标转速
extern uint16_t SJKYJ_Target_Speed_Change;//空压机目标转速偏移
extern uint8_t S_SJKYJ_State;//势加透波空压机状态
extern uint16_t Speed_Adjust_I;//空压机转速调节电流
extern uint32_t BD_SJKYJ_Speed[7];//获取标定的空压机转速点
extern uint16_t BD_I_Point[7];//获取标定的空压机转速点对应的电流点
extern uint16_t LowTempSpeedCompensate;//低温下电转速补偿
/*************************************************************/
/*                     发送给6KW车载DCDC ID                   */
/*************************************************************/
extern uint8_t S_6KWDC_Data[8];//
extern uint8_t SDC_State;//3kWDCDC状态
extern uint8_t Down_DC_Enable;
/*************************************************************/
/*                         发送给节温器 ID                        */
/*************************************************************/
extern uint8_t S_JWQ_Data[8];//
extern uint8_t JWQ_Enable;//节温器使能状态 初始设置为不使能状态
extern uint8_t JWQ_Target_Angle;//
/*************************************************************/
/*                         发送给PTC ID                        */
/*************************************************************/
extern uint8_t S_PTC_Data[8];//
extern uint8_t PTC_Flag;
extern uint8_t PTC_Enable;//节温器使能状态 初始设置为不使能状态
extern uint8_t PTC_Target_Temp;//目标温度值
extern uint8_t PTC_Target_PWM;//目标PWM值

extern uint8_t YeWei;
extern uint8_t YeWeiTX;
extern uint8_t StartUpCurrent_Flag;
extern uint16_t Time_10min;

uint8_t PTC_test_Time;

//extern uint8_t S_RUN_Count_Data[2];//纪录运行次数


extern void Send_QXHB(void);


void Send_CBD_Data(void);//发送参数标定的值
void Send_GBD_Data(void);//发送故障标定的值
void CAN0_Send_Out(void);
void Send_Computer(void);
#endif /* CAN_TX_H_ */
