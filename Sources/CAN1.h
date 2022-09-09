/*
 * CAN1.h
 *
 *  Created on: 2018��9��4��
 *      Author: Administrator
 */

#ifndef CAN1_H_
#define CAN1_H_

#define CAN1_TX_MAILBOX0  (0U) // ����0ͨ��
#define CAN1_TX_MAILBOX1  (1U) // ����1ͨ��
#define ID_S_CAN1_FCU1           0x18FF0112      //���͸�FCU����1
#define ID_S_CAN1_FCU2           0x18FF0212      //���͸�FCU����2
#define ID_S_CAN1_FCU3           0x18FF0312      //���͸�FCU����3
#define ID_S_CAN1_FCU4           0x18FF0412      //���͸�FCU����4
//#define ID_S_CAN1_FCU5           0x18FF0512      //���͸�FCU����5
#define ID_S_CAN1_FCU6           0x18FF0612      //���͸�FCU����6
#define ID_S_CAN1_FCU7           0x18FF0712      //���͸�FCU����7 ûд
#define ID_S_CAN1_FCU8           0x18FF0812      //���͸�FCU����8
#define ID_S_CAN1_FCU9           0x18FF0912      //���͸�FCU����9
#define ID_S_CAN1_FCU10          0x18FF0A12      //���͸�FCU����10
#define ID_S_CAN1_FCU11          0x18FF0B12      //���͸�FCU����11
#define ID_S_CAN1_FCU12          0x18FF0C12      //���͸�FCU����11
#define ID_S_CAN1_FCU13          0x18FF0D12      //���͸�FCU����13
#define ID_S_CAN1_FCU14          0x18FF0F12      //���͸�FCU����14


#define ID_S_CAN1_FCU2HMS        0x18C1A6EF      //FCU���͸�HMS����


//#define RX1_MAILBOX_BMS1   (1U)
//#define ID_BMS1          0x14F19CF4       //BMS����1
//can_message_t recvMsg_BMS1;

//���SOC
#define RX1_MAILBOX_BMS2   (2U)
#define ID_BMS2           0x18FFA2F3       // ���� BMS����2
can_message_t recvMsg_BMS2;

//������ܵ�ѹ��0,1;0.1V/Bit�����ܵ���(2,3;0.1A/Bit,-5000---+5000)��SOC(4)
#define RX1_MAILBOX_BMS3   (3U)
#define ID_BMS3           0x18FFA7F3      //BMS����3
can_message_t recvMsg_BMS3;

// ������̱���
#define RX1_MAILBOX_VCU2   (4U)
#define ID_VCU2           0x0C20A0A6       //������̱���
can_message_t recvMsg_VCU2;

//���BMS�ܸ��Ӵ���
#define RX1_MAILBOX_BMS5   (5U)
#define ID_BMS5           0x18FFB0F3       //BMS����5
can_message_t recvMsg_BMS5;

//��Ե��ֵ��2,3��1K��/Bit)
#define RX1_MAILBOX_ISO   (6U)
#define ID_ISO           0x18FFA8F3       // ���� ��Ե���
can_message_t recvMsg_ISO;

//Ready״̬  Byte6,bit3 (0X04 Ready)
#define RX1_MAILBOX_VCU1   (7U)
#define ID_VCU1           0x0C0012A6      // ���� VCU����1
can_message_t recvMsg_VCU1;

//���� Byte7,0-140KM/H
#define RX1_MAILBOX_CM   (8U)
#define ID_CM           0x08F010A0      // ���� �������ٱ���
can_message_t recvMsg_CM;

//Byte2�����ѹ��0.2V/Bit����Byte3,4(0.1A/Bit,-3200ƫ��)
#define RX1_MAILBOX_SDC   (9U)
#define ID_SDC           0x18E101B8      //��ѹDC����
can_message_t recvMsg_SDC;

//HMS����ϵͳ1
#define RX1_MAILBOX_HMS1   (10U)
#define ID_HMS1           0x18E1EF21      //HMS����ϵͳ1
can_message_t recvMsg_HMS1;

//HMS����ϵͳ2
#define RX1_MAILBOX_HMS2   (11U)
#define ID_HMS2           0x18E3EF21      //HMS����ϵͳ2
can_message_t recvMsg_HMS2;

can_buff_config_t buffCfg1;

can_message_t TX1_message;

void CAN1_Ini(void);
void REN_CAN1_Send(uint32_t ID,uint32_t MAILBOX_ID,uint8_t Data0,uint8_t Data1,uint8_t Data2,uint8_t Data3,
		uint8_t Data4,uint8_t Data5,uint8_t Data6,uint8_t Data7);

#endif /* CAN1_H_ */
