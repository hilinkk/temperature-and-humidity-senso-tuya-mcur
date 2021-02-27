#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "wkup.h"
#include "oled.h"
#include "oled_gui.h"

//外部中断0服务程序
void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

	//GPIOA0	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOA1	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;  	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级1， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//子优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
}

u8 KEY0_st = 0;		//KTY0短按标志位
u8 KEY1_st = 0;		//KTY1短按标志位
u8 KEY1_lt = 0;		//KTY1长按标志位

//中断,检测到PA0脚的一个上升沿.	  
//中断线0线上的中断检测
void EXTI0_IRQHandler(void)
{ 	
	delay_ms(10);		//消抖
	if(KEY0){			//按键KEY0按下							
		if(Check_WKUP()){					//关机	
			OLED_Clear();
			Sys_Enter_Standby(); 			
		}else{
			KEY0_st = 1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0); // 清除LINE10上的中断标志位	
} 

extern int GuiNum;
//外部中断1服务程序
void EXTI1_IRQHandler(void)
{
	delay_ms(10);		//消抖
	if(!KEY1){			//按键KEY1按下							
		if(Check_Key1()){	//进入或退出设置	
			KEY1_lt = 1;
		}else{
			KEY1_st = 1;
		}
	}	
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE1上的中断标志位  
}


