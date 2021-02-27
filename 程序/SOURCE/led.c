#include "led.h"

u8 LedBit = 0;		//LED��־λ��Ϊ0ʱLED���Ե���

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	//ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;//�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 	//�����趨������ʼ��GPIOA
 GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);	//����ߵ�ƽ���ر�LED

}

//��������LED
void LED_On(void)
{
	LEDR_ON();
	LEDG_ON();
	LEDB_ON();
}

//Ϩ������LED
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
