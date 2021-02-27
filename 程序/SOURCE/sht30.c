#include "sht30.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

////IIC���ߵ�ַ�ӿڶ���
//#define SCL PAout(4)
//#define SDA_OUT PAout(5)
//#define SDA_IN PAin(5)
//#define IIC_INPUT_MODE_SET()  {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=0x00800000;}
//#define IIC_OUTPUT_MODE_SET() {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=0x00300000;}
 
//IIC���ߵ�ַ�ӿڶ���
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


 /*��������ACK*/
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
 /*����������ACK*/
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
 /*�����ȴ��ӻ���ACK*/
u8 IIC_wait_ACK(void)
{
    u8 t = 200;
    IIC_OUTPUT_MODE_SET();
    SDA_OUT=1;//8λ��������ͷ������ߣ�׼������Ӧ��λ 
    delay_us(1);
    SCL=0;
    delay_us(1); 
    IIC_INPUT_MODE_SET();
    delay_us(1); 
    while(SDA_IN)//�ȴ�SHT30Ӧ��
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
����:����I2C����,������I2C��ʼ����.  
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
����:����I2C����,������I2C��������.  
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
�ֽ����ݷ��ͺ���               
����ԭ��: void  SendByte(UCHAR c);
����:������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������
********************************************************************/
void  IIC_SendByte(u8 byte)
{
	u8  BitCnt;
	IIC_OUTPUT_MODE_SET();
	SCL=0;
	for(BitCnt=0;BitCnt<8;BitCnt++)//Ҫ���͵����ݳ���Ϊ8λ
	{
//		if(byte&0x80) SDA_OUT=1;//�жϷ���λ
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
 �ֽ����ݽ��պ���               
����ԭ��: UCHAR  RcvByte();
����: �������մ���������������  
********************************************************************/    
u8 IIC_RcvByte(void)
{
  u8 retc;
  u8 BitCnt;
  retc=0; 
  IIC_INPUT_MODE_SET();//��������Ϊ���뷽ʽ
  delay_us(1);                    
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {  
	SCL=0;//��ʱ����Ϊ�ͣ�׼����������λ
	delay_us(2);               
	SCL=1;//��ʱ����Ϊ��ʹ��������������Ч                
	retc=retc<<1;
	if(SDA_IN) retc |=1;//������λ,���յ�����λ����retc�� 
	delay_us(1);
  }
  SCL=0;    
  return(retc);
}

//SHT30 IIC���ų�ʼ��
void SHT30_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��B�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				//��ʼ��GPIOB10,11
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}

float Temperature = 0;
float Humidity = 0;

/*******************************************************************
 ��ʪ�Ȼ�ȡ����               
����ԭ��: SHT30_read_result(u8 addr);
����: �������մ������ɼ����ϳ���ʪ��
********************************************************************/ 
void SHT30_read_result(u8 addr)
{
	u16 tem = 0,hum = 0;
	u16 buff[6] = {0};	
	IIC_Start();
	IIC_SendByte(addr<<1 | write);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
	IIC_wait_ACK();
	IIC_SendByte(0x2C);
	IIC_wait_ACK();
	IIC_SendByte(0x06);
	IIC_wait_ACK();
	IIC_Stop();
	delay_ms(50);
	IIC_Start();
	IIC_SendByte(addr<<1 | read);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
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
		
		tem = ((buff[0]<<8) | buff[1]);//�¶�ƴ��
		hum = ((buff[3]<<8) | buff[4]);//ʪ��ƴ��
		
		/*ת��ʵ���¶�*/
		Temperature= (175.0*(float)tem/65535.0-45.0);	// T = -45 + 175 * tem / (2^16-1)
		Humidity= (100.0*(float)hum/65535.0);}			// RH = hum*100 / (2^16-1)
			
}




