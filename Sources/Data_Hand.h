/*
 * Data_Hand.h
 *
 *  Created on: 2018��8��28��
 *      Author: Administrator
 */

#ifndef DATA_HAND_H_
#define DATA_HAND_H_



//����ʱ�䣬�������
extern uint8_t Out_Time;
extern uint16_t Out_Interva,Out_Interva_Old;
extern uint8_t Out_H_Flag;//�����־λ  1�����ֶ����� 0�����Զ����� �������������1���ǹ̶�����100ms*Out_H_Flag
//ON����־λ ֹͣ��־λ
uint8_t ON_Flag,Stop_Flag;
uint8_t Fan_Speed_Data;//�����ٶ�ֵ
//������ϵȼ���־λ
uint16_t Fault_Level_Two,Fault_Level_Three,Fault_Level_Four,Fault_Level_Five;
extern uint8_t S_Fan_Speed;
//CAN0���͸���λ��������һЩ״̬
uint8_t S_VCU_State;//�ܸ���ON������ͣ��Ready���ź�
uint16_t S_Fan_Speed_Change;//���Ȳ����������ݵ�ѳ����¶���ֵ���ӷ���ת�٣�
uint16_t HProtectFlag_10S;//��Ready���ʱ10S

//����¶ȿ���
void Temp_Con(void);
//ON�����
void ON_Read(void);
//�Զ����⺯��
void Out_H(void);
//�����й��ϼ�⺯��
void Fault_Check(void);
//���ȵ��ٺ���
void Fan_Speed(void);

void Fan_Speed2(uint8_t open);

//����������������Ҫ�ŵ�main��������
//�������ѹ��
void H_Con(void);
//���������ж�
void Start_Up_Condition(void);
//��ѷ���������
void Stack_P_CAL(void);
//Һλ��غ���
void YeWei_Stack(void);

#endif /* DATA_HAND_H_ */
