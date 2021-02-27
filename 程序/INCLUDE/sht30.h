#ifndef __SHT30_H
#define __SHT30_H

#include "sys.h"
 
#define write 0
#define read  1
#define SHT30_ADDR 0x44	//IIC地址，ADDR脚接地时为0x44，接高电平时为0x45

//extern u8 humiture_buff1[20];
//extern u8 humiture_buff2[20];
//extern u8 Refresh_SHT30_Data;
//extern u8 send_data_fleg;
//extern u8 Temperature_L;
//extern u8 Humidity_L;
//extern u8 Temperature_H;
//extern u8 Humidity_H;
 
extern float Temperature;
extern float Humidity;

void SHT30_IIC_Init(void);
void IIC_ACK(void);
void IIC_NACK(void);
u8 IIC_wait_ACK(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(u8 byte);
u8 IIC_RcvByte(void);
void SHT30_read_result(u8 addr);

#endif

