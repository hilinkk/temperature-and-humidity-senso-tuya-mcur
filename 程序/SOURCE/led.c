#include "led.h"

u8 LedBit = 0;		//LED标志位，为0时LED可以点亮

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	//使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;//端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 	//根据设定参数初始化GPIOA
 GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);	//输出高电平，关闭LED

}

//点亮所有LED
void LED_On(void)
{
	LEDR_ON();
	LEDG_ON();
	LEDB_ON();
}

//熄灭所有LED
void LED_Off(void)
{
	LEDR_OFF();
	LEDG_OFF();
	LEDB_OFF();
}

void LEDR_Switch(void)
{
	if(LED_R)
		LEDR_ON();
	else
		LEDR_OFF();
}

void LEDG_Switch(void)
{
	if(LED_G)
		LEDG_ON();
	else
		LEDG_OFF();
}

void LEDB_Switch(void)
{
	if(LED_B)
		LEDB_ON();
	else
		LEDB_OFF();
}

void LED_Switch(void)
{
	LEDR_Switch();
	LEDG_Switch();
	LEDB_Switch();
}

void LED_State_Switch(void)
{	
	LED_Off();
	LedBit = !LedBit;
}
