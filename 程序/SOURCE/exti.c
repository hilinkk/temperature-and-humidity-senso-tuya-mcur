#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "wkup.h"
#include "oled.h"
#include "oled_gui.h"

//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

	//GPIOA0	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOA1	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;  	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
}

u8 KEY0_st = 0;		//KTY0�̰���־λ
u8 KEY1_st = 0;		//KTY1�̰���־λ
u8 KEY1_lt = 0;		//KTY1������־λ

//�ж�,��⵽PA0�ŵ�һ��������.	  
//�ж���0���ϵ��жϼ��
void EXTI0_IRQHandler(void)
{ 	
	delay_ms(10);		//����
	if(KEY0){			//����KEY0����							
		if(Check_WKUP()){					//�ػ�	
			OLED_Clear();
			Sys_Enter_Standby(); 			
		}else{
			KEY0_st = 1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ	
} 

extern int GuiNum;
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
	delay_ms(10);		//����
	if(!KEY1){			//����KEY1����							
		if(Check_Key1()){	//������˳�����	
			KEY1_lt = 1;
		}else{
			KEY1_st = 1;
		}
	}	
	EXTI_ClearITPendingBit(EXTI_Line1);  //���LINE1�ϵ��жϱ�־λ  
}


