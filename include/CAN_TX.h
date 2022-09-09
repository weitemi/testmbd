/*
 * CAN_TX.h
 *
 *  Created on: 2018��8��27��
 *      Author: Administrator
 */

#ifndef CAN_TX_H_
#define CAN_TX_H_


/*************************************************************/
/*                       ���͸�����ˮ������                   */
/*************************************************************/
extern uint8_t S_PUMP_Data[8];//
extern uint8_t PUMP_Enable;//����ˮ��ʹ��״̬
extern uint16_t Pump_Target_Speed;//����ˮ��Ŀ��ת��
extern uint8_t Water_Pre_State_Flag;//ˮѹ״̬��־λ 0�������� �������Ǵ����쳣
/*************************************************************/
/*                       ���͸���ѭ��������                    */
/*************************************************************/
extern uint8_t S_QXHB_Data[8];//
extern uint16_t H_Pump_Target_Speed;//����ѭ����Ŀ��ת��
extern uint8_t H_PUMP_Enable;//����ѭ����ʹ��״̬ ��ʼ����Ϊ��ʹ��״̬
/*************************************************************/
/*                        ���͸���ѹ������                     */
/*************************************************************/
extern uint8_t S_JYF_Data[8];//Ĭ�Ϸ��͹رն�ֵģʽ
/*************************************************************/
/*                      ���͸�������ѹ������                   */
/*************************************************************/
extern uint8_t S_KQBYF_Data[8];//
extern uint8_t KQBYF_Target_Angle;//��ѹ��Ŀ��Ƕ�
extern uint16_t LowTempAngleCompensate;//��ѹ�������µ�ǶȲ���
/*************************************************************/
/*                        ���͸�DCDC ID                      */
/*************************************************************/
extern uint8_t S_DC_Data[8];//
extern uint8_t S_DC_State;//�ػ� 0X05�ػ� 0X0B���� 0X0C�����ŵ�  0X03Ԥ��
extern uint16_t S_DC_I;//DC�������ֵ ��λ0.1A
extern uint16_t DC_Target_I;//��ѹDCĿ����� ����1A
int16_t I_Cha;
extern int16_t DC_Target_Old;
extern uint8_t Up_DCDC_Enable;
extern uint8_t LowTemp_FanCon_Flag;//���·����л����Ʊ�־λ
/*************************************************************/
/*                    ���͸��Ƽ�͸����ѹ��  ID                 */
/*************************************************************/
extern uint8_t S_SJKYJ_Data[8];//
extern uint32_t SJKYJ_Target_Speed;//��ѹ��Ŀ��ת��
extern uint16_t SJKYJ_Target_Speed_Change;//��ѹ��Ŀ��ת��ƫ��
extern uint8_t S_SJKYJ_State;//�Ƽ�͸����ѹ��״̬
extern uint16_t Speed_Adjust_I;//��ѹ��ת�ٵ��ڵ���
extern uint32_t BD_SJKYJ_Speed[7];//��ȡ�궨�Ŀ�ѹ��ת�ٵ�
extern uint16_t BD_I_Point[7];//��ȡ�궨�Ŀ�ѹ��ת�ٵ��Ӧ�ĵ�����
extern uint16_t LowTempSpeedCompensate;//�����µ�ת�ٲ���
/*************************************************************/
/*                     ���͸�6KW����DCDC ID                   */
/*************************************************************/
extern uint8_t S_6KWDC_Data[8];//
extern uint8_t SDC_State;//3kWDCDC״̬
extern uint8_t Down_DC_Enable;
/*************************************************************/
/*                         ���͸������� ID                        */
/*************************************************************/
extern uint8_t S_JWQ_Data[8];//
extern uint8_t JWQ_Enable;//������ʹ��״̬ ��ʼ����Ϊ��ʹ��״̬
extern uint8_t JWQ_Target_Angle;//
/*************************************************************/
/*                         ���͸�PTC ID                        */
/*************************************************************/
extern uint8_t S_PTC_Data[8];//
extern uint8_t PTC_Flag;
extern uint8_t PTC_Enable;//������ʹ��״̬ ��ʼ����Ϊ��ʹ��״̬
extern uint8_t PTC_Target_Temp;//Ŀ���¶�ֵ
extern uint8_t PTC_Target_PWM;//Ŀ��PWMֵ

extern uint8_t YeWei;
extern uint8_t YeWeiTX;
extern uint8_t StartUpCurrent_Flag;
extern uint16_t Time_10min;

uint8_t PTC_test_Time;

//extern uint8_t S_RUN_Count_Data[2];//��¼���д���


extern void Send_QXHB(void);


void Send_CBD_Data(void);//���Ͳ����궨��ֵ
void Send_GBD_Data(void);//���͹��ϱ궨��ֵ
void CAN0_Send_Out(void);
void Send_Computer(void);
#endif /* CAN_TX_H_ */
