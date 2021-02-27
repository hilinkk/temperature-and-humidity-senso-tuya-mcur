#include "sht30.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

////IIC总线地址接口定义
//#define SCL PAout(4)
//#define SDA_OUT PAout(5)
//#define SDA_IN PAin(5)
//#define IIC_INPUT_MODE_SET()  {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=0x00800000;}
//#define IIC_OUTPUT_MODE_SET() {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=0x00300000;}
 
//IIC总线地址接口定义
#define SCL PBout(11)
#define SDA_OUT PBout(10)
#define SDA_IN PBin(10)
#define IIC_INPUT_MODE_SET()  {GPIOB->CRH&=0xFFFFF0FF;GPIOB->CRH|=0x00000800;}
#define IIC_OUTPUT_MODE_SET() {GPIOB->CRH&=0xFFFFF0FF;GPIOB->CRH|=0x00000300;}

float humiture[4];
//u8 Refresh_SHT30_Data=0;
u8 humiture_buff1[20];
u8 humiture_buff2[20];
// 
//u8 Temperature_L=15;
//u8 Humidity_L=50;
//u8 Temperature_H=30;
//u8 Humidity_H=80;


 /*主机发送ACK*/
void IIC_ACK(void)
{
  IIC_OUTPUT_MODE_SET();
  SCL=0;
  delay_us(2); 
  SDA_OUT=0;
  delay_us(2);     
  SCL=1;
  delay_us(2);                  
  SCL=0;                     
  delay_us(1);    
}
 /*主机不发送ACK*/
void IIC_NACK(void)
{
  IIC_OUTPUT_MODE_SET();
  SCL=0;
  delay_us(2); 
  SDA_OUT=1;
  delay_us(2);      
  SCL=1;
  delay_us(2);                   
  SCL=0;                     
  delay_us(1);    
}
 /*主机等待从机的ACK*/
u8 IIC_wait_ACK(void)
{
    u8 t = 200;
    IIC_OUTPUT_MODE_SET();
    SDA_OUT=1;//8位发送完后释放数据线，准备接收应答位 
    delay_us(1);
    SCL=0;
    delay_us(1); 
    IIC_INPUT_MODE_SET();
    delay_us(1); 
    while(SDA_IN)//等待SHT30应答
    {
	t--;
	delay_us(1); 
	if(t==0)
	{
	  SCL=0;
	  return 1;
	}
	delay_us(1); 
    }
    delay_us(1);      
    SCL=1;
    delay_us(1);
    SCL=0;             
    delay_us(1);    
    return 0;	
}
/*******************************************************************
功能:启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void IIC_Start(void)
{
	IIC_OUTPUT_MODE_SET();
	SDA_OUT=1;
	SCL=1;
	delay_us(4);	
	SDA_OUT=0;
	delay_us(4); 
	SCL=0;
}
 
/*******************************************************************
功能:结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void IIC_Stop(void)
{
	IIC_OUTPUT_MODE_SET();
	SCL=0;
	SDA_OUT=0;  
	delay_us(4);	
	SCL=1;
	delay_us(4);
	SDA_OUT=1;
	delay_us(4);
}
 
/*******************************************************************
字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:将数据c发送出去,可以是地址,也可以是数据
********************************************************************/
void  IIC_SendByte(u8 byte)
{
	u8  BitCnt;
	IIC_OUTPUT_MODE_SET();
	SCL=0;
	for(BitCnt=0;BitCnt<8;BitCnt++)//要传送的数据长度为8位
	{
//		if(byte&0x80) SDA_OUT=1;//判断发送位
//		else SDA_OUT=0; 
		SDA_OUT = (byte&0x80) >> 7;
		byte<<=1;
		delay_us(2); 
		SCL=1;
		delay_us(2);
		SCL=0;
		delay_us(2);
	}
}
/*******************************************************************
 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能: 用来接收从器件传来的数据  
********************************************************************/    
u8 IIC_RcvByte(void)
{
  u8 retc;
  u8 BitCnt;
  retc=0; 
  IIC_INPUT_MODE_SET();//置数据线为输入方式
  delay_us(1);                    
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {  
	SCL=0;//置时钟线为低，准备接收数据位
	delay_us(2);               
	SCL=1;//置时钟线为高使数据线上数据有效                
	retc=retc<<1;
	if(SDA_IN) retc |=1;//读数据位,接收的数据位放入retc中 
	delay_us(1);
  }
  SCL=0;    
  return(retc);
}

//SHT30 IIC引脚初始化
void SHT30_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				//初始化GPIOB10,11
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}

float Temperature = 0;
float Humidity = 0;

/*******************************************************************
 温湿度获取函数               
函数原型: SHT30_read_result(u8 addr);
功能: 用来接收从器件采集并合成温湿度
********************************************************************/ 
void SHT30_read_result(u8 addr)
{
	u16 tem = 0,hum = 0;
	u16 buff[6] = {0};	
	IIC_Start();
	IIC_SendByte(addr<<1 | write);//写7位I2C设备地址加0作为写取位,1为读取位
	IIC_wait_ACK();
	IIC_SendByte(0x2C);
	IIC_wait_ACK();
	IIC_SendByte(0x06);
	IIC_wait_ACK();
	IIC_Stop();
	delay_ms(50);
	IIC_Start();
	IIC_SendByte(addr<<1 | read);//写7位I2C设备地址加0作为写取位,1为读取位
	if(IIC_wait_ACK() == 0){
		buff[0]=IIC_RcvByte();
		IIC_ACK();
		buff[1]=IIC_RcvByte();
		IIC_ACK();
		buff[2]=IIC_RcvByte();
		IIC_ACK();
		buff[3]=IIC_RcvByte();
		IIC_ACK();
		buff[4]=IIC_RcvByte();
		IIC_ACK();
		buff[5]=IIC_RcvByte();
		IIC_NACK();
		IIC_Stop();
		
		tem = ((buff[0]<<8) | buff[1]);//温度拼接
		hum = ((buff[3]<<8) | buff[4]);//湿度拼接
		
		/*转换实际温度*/
		Temperature= (175.0*(float)tem/65535.0-45.0);	// T = -45 + 175 * tem / (2^16-1)
		Humidity= (100.0*(float)hum/65535.0);}			// RH = hum*100 / (2^16-1)
			
}




