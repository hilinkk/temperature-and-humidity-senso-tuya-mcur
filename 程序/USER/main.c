#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "exti.h"
#include "oled.h"
#include "power.h"
#include "rtc.h" 
#include "wkup.h"
#include "oled_gui.h"
#include "adc.h"
#include "timer.h"
#include "SHT30.h"
#include "wkup.h"
#include "wifi.h"
#include "wb3s.h"
#include "pwm.h"

extern int BatElect;
extern u8 GuiNum;

int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	PowerInit();			//���蹩���ʼ��
	EXTIX_Init();			//�ⲿ�жϳ�ʼ��
	if(Check_WKUP()==0)Sys_Enter_Standby();    		//���ǿ���,�������ģʽ		
	LED_Init();		  		//LED��ʼ��
	LED_On();				//LEDȫ��
	wifi_protocol_init();	//Э�鴮�ڳ�ʼ��
	uart1_init(115200);		//���ڳ�ʼ��Ϊ115200
	uart2_init(115200);		//���ڳ�ʼ��Ϊ115200	
	RTC_Init();				//RTC��ʼ��	
	Adc_Init();				//ADC��ʼ��
	SHT30_IIC_Init();		//��ʪ�ȴ�������SHT30����ʼ��	
	PWM_Init(5000-1, 288-1);//50HZ,��PWM����Ϊ20ms		
	OLED_Init();			//OLED��ʼ��	
	TIM2_Int_Init(1000-1, 7200-1);		//�ж�ʱ��Ϊ0.1s,���ж��и���OLED��ʾ,��ȡ��ʪ�ȴ�����������
	StartUp();				//��ʾ��������			
	delay_ms(1000);
	LED_Off();
	while(1){						
		switch (GuiNum){
			case FeaSetN:
				OledGui_FeaSet();		//���ý���
				break;
			case GuiTempHum:
				OledGui_Temp_Hum();		//��ʪ�Ƚ���
				break;
			case GuiTime:
				OledGui_Time();			//ʱ�ӽ���
				break;
			case GuiWeather:
				OledGui_Weather();		//��������
				break;
		}		
	}
}

