/*
 * CAN1.h
 *
 *  Created on: 2018年9月4日
 *      Author: Administrator
 */

#ifndef CAN1_H_
#define CAN1_H_

#define CAN1_TX_MAILBOX0  (0U) // 发送0通道
#define CAN1_TX_MAILBOX1  (1U) // 发送1通道
#define ID_S_CAN1_FCU1           0x18FF0112      //发送给FCU报文1
#define ID_S_CAN1_FCU2           0x18FF0212      //发送给FCU报文2
#define ID_S_CAN1_FCU3           0x18FF0312      //发送给FCU报文3
#define ID_S_CAN1_FCU4           0x18FF0412      //发送给FCU报文4
//#define ID_S_CAN1_FCU5           0x18FF0512      //发送给FCU报文5
#define ID_S_CAN1_FCU6           0x18FF0612      //发送给FCU报文6
#define ID_S_CAN1_FCU7           0x18FF0712      //发送给FCU报文7 没写
#define ID_S_CAN1_FCU8           0x18FF0812      //发送给FCU报文8
#define ID_S_CAN1_FCU9           0x18FF0912      //发送给FCU报文9
#define ID_S_CAN1_FCU10          0x18FF0A12      //发送给FCU报文10
#define ID_S_CAN1_FCU11          0x18FF0B12      //发送给FCU报文11
#define ID_S_CAN1_FCU12          0x18FF0C12      //发送给FCU报文11
#define ID_S_CAN1_FCU13          0x18FF0D12      //发送给FCU报文13
#define ID_S_CAN1_FCU14          0x18FF0F12      //发送给FCU报文14


#define ID_S_CAN1_FCU2HMS        0x18C1A6EF      //FCU发送给HMS报文


//#define RX1_MAILBOX_BMS1   (1U)
//#define ID_BMS1          0x14F19CF4       //BMS报文1
//can_message_t recvMsg_BMS1;

//电池SOC
#define RX1_MAILBOX_BMS2   (2U)
#define ID_BMS2           0x18FFA2F3       // 开沃 BMS报文2
can_message_t recvMsg_BMS2;

//电池箱总电压（0,1;0.1V/Bit），总电流(2,3;0.1A/Bit,-5000---+5000)，SOC(4)
#define RX1_MAILBOX_BMS3   (3U)
#define ID_BMS3           0x18FFA7F3      //BMS报文3
can_message_t recvMsg_BMS3;

// 整车里程报文
#define RX1_MAILBOX_VCU2   (4U)
#define ID_VCU2           0x0C20A0A6       //整车里程报文
can_message_t recvMsg_VCU2;

//电池BMS总负接触器
#define RX1_MAILBOX_BMS5   (5U)
#define ID_BMS5           0x18FFB0F3       //BMS报文5
can_message_t recvMsg_BMS5;

//绝缘阻值（2,3；1KΩ/Bit)
#define RX1_MAILBOX_ISO   (6U)
#define ID_ISO           0x18FFA8F3       // 开沃 绝缘监测
can_message_t recvMsg_ISO;

//Ready状态  Byte6,bit3 (0X04 Ready)
#define RX1_MAILBOX_VCU1   (7U)
#define ID_VCU1           0x0C0012A6      // 开沃 VCU报文1
can_message_t recvMsg_VCU1;

//车速 Byte7,0-140KM/H
#define RX1_MAILBOX_CM   (8U)
#define ID_CM           0x08F010A0      // 开沃 整车车速报文
can_message_t recvMsg_CM;

//Byte2输出电压（0.2V/Bit），Byte3,4(0.1A/Bit,-3200偏移)
#define RX1_MAILBOX_SDC   (9U)
#define ID_SDC           0x18E101B8      //降压DC报文
can_message_t recvMsg_SDC;

//HMS供氢系统1
#define RX1_MAILBOX_HMS1   (10U)
#define ID_HMS1           0x18E1EF21      //HMS供氢系统1
can_message_t recvMsg_HMS1;

//HMS供氢系统2
#define RX1_MAILBOX_HMS2   (11U)
#define ID_HMS2           0x18E3EF21      //HMS供氢系统2
can_message_t recvMsg_HMS2;

can_buff_config_t buffCfg1;

can_message_t TX1_message;

void CAN1_Ini(void);
void REN_CAN1_Send(uint32_t ID,uint32_t MAILBOX_ID,uint8_t Data0,uint8_t Data1,uint8_t Data2,uint8_t Data3,
		uint8_t Data4,uint8_t Data5,uint8_t Data6,uint8_t Data7);

#endif /* CAN1_H_ */
