#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
 
extern u8 LedBit;

#define LED_R PAout(4)
#define LED_G PAout(5)	
#define LED_B PAout(6)

#define LEDR_ON() 	(LED_R = LedBit)	//����LEDR��ֻ��LedBit����0ʱ�ŻῪ��
#define LEDG_ON() 	(LED_G = LedBit)
#define LEDB_ON() 	(LED_B = LedBit)

#define LEDR_OFF() 	(LED_R = 1)			//�ر�LEDR
#define LEDG_OFF() 	(LED_G = 1)
#define LEDB_OFF() 	(LED_B = 1)

void LED_Init(void);	//OLED��ʼ��
void LED_On(void);		//��������LED
void LED_Off(void);		//Ϩ������LED
void LEDR_Switch(void);
void LEDG_Switch(void);
void LEDB_Switch(void);
void LED_Switch(void);
void LED_State_Switch(void);
		 				    
#endif
