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
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	PowerInit();			//外设供电初始化
	EXTIX_Init();			//外部中断初始化
	if(Check_WKUP()==0)Sys_Enter_Standby();    		//不是开机,进入待机模式		
	LED_Init();		  		//LED初始化
	LED_On();				//LED全亮
	wifi_protocol_init();	//协议串口初始化
	uart1_init(115200);		//串口初始化为115200
	uart2_init(115200);		//串口初始化为115200	
	RTC_Init();				//RTC初始化	
	Adc_Init();				//ADC初始化
	SHT30_IIC_Init();		//温湿度传感器“SHT30”初始化	
	PWM_Init(5000-1, 288-1);//50HZ,即PWM周期为20ms		
	OLED_Init();			//OLED初始化	
	TIM2_Int_Init(1000-1, 7200-1);		//中断时间为0.1s,在中断中更新OLED显示,读取温湿度传感器等数据
	StartUp();				//显示开机界面			
	delay_ms(1000);
	LED_Off();
	while(1){						
		switch (GuiNum){
			case FeaSetN:
				OledGui_FeaSet();		//设置界面
				break;
			case GuiTempHum:
				OledGui_Temp_Hum();		//温湿度界面
				break;
			case GuiTime:
				OledGui_Time();			//时钟界面
				break;
			case GuiWeather:
				OledGui_Weather();		//天气界面
				break;
		}		
	}
}

