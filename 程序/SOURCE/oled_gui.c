#include "oled_gui.h"
#include "oled.h"
#include "rtc.h" 
#include "adc.h"
#include "sht30.h"
#include "led.h"
#include "delay.h"
#include "bmp.h"
#include "key.h"
#include "usart.h"
#include "wifi.h"
#include "wkup.h"
#include "wb3s.h"
#include "mcu_api.h"
#include "pwm.h"

extern int BatElect;
extern u8 LedBit;		//LED标志位，为1时LED可以点亮
extern u8 KEY0_st;
extern u8 KEY1_st;
extern u8 KEY1_lt;
extern u8 OledRef;
u8 GuiNum = GuiTempHum;
u8 GuiNum_Ago = FeaSetN;
//页面切换
static void GuiSwitch(void)
{
	if(KEY0_st || KEY1_st){	//检测按键是否按下
		if(KEY0_st){
			KEY0_st = 0;
			GuiNum--;
		}
		if(KEY1_st){
			KEY1_st = 0;
			GuiNum++;
		}
		if(GuiNum > 3){
			GuiNum = 1;
		}else if(GuiNum < 1){
			GuiNum = 3;
		}
	} 
}

//开机
void StartUp(void)
{
	OLED_Clear();
	OLED_DrawBMP(24, 0, 80, 64, BiliBili1);
	delay_ms(1000);
	delay_ms(1000);
	OLED_Clear();	
	OLED_DrawBMP(34, 0, 60, 48, BiliBili2);	
	OLED_C16x16Str(0, 6, (unsigned char*)"正在开机");
	delay_ms(500);	
	OLED_C16x16Str(0, 6, (unsigned char*)"正在开机。");
	delay_ms(500);
	OLED_C16x16Str(0, 6, (unsigned char*)"正在开机。。");
	delay_ms(500);
	OLED_C16x16Str(0, 6, (unsigned char*)"正在开机。。。");
	delay_ms(500);
	OLED_C16x16Str(0, 6, (unsigned char*)"正在开机。。。。");
	delay_ms(500);
	OLED_Clear();

}

//所有显示界面都要完成的功能
//time :LED闪烁的时间，数值越大闪烁越慢
static void AllForGui(u16 time)
{
	static u16 t = 0;
	static u8 min = 0;
	t++;
	wifi_uart_service();	//wifi串口数据处理服务
	steering_control();		//舵机控制
	
	if(KEY1_lt){			//检测到KEY1长按则进入或退出设置界面
		KEY1_lt = 0;
		if(GuiNum != FeaSetN){
			GuiNum_Ago = GuiNum;
			GuiNum = FeaSetN;
		}else{
			GuiNum = GuiNum_Ago;
			GuiNum_Ago = FeaSetN;
		}
	}
	
	if (min != calendar.min){
		min = calendar.min;
		mcu_get_system_time();		//每分钟更新一次时间
	}
	if(t > time){		
		t = 0;
		show_wifi_work_state();
		all_data_update();				//上报数据
		LEDB_Switch();					//蓝色LED闪烁，表示程序正在运行		
		if(0 == BatElect){				//根据当前电量控制红色LED
			Sys_Enter_Standby();
		}else if(BatElect < 20){		/* 电量少于20%时红灯闪烁 */			
			LEDR_Switch();
		}else{
			LEDR_OFF();
		}
	}
}

//设置界面
void OledGui_FeaSet(void)
{
	u8 i = 0;
	OLED_Clear();
	OLED_C16x16Str(16, 0, (unsigned char*)"重置");
	OLED_ShowString(48, 0, (u8 *)"wifi", 16, OLED_CHR_NORMAL);
	OLED_C16x16Str(16, 2, (unsigned char*)"开灯");
	OLED_C16x16Str(56, 2, (unsigned char*)"关灯");
	OLED_ShowChar(48, 2, '/', 16, OLED_CHR_NORMAL);
	OLED_C16x16Str(16, 4, (unsigned char*)"更新时间");
	OLED_C16x16Str(16, 6, (unsigned char*)"镜像显示");
	while(1){
		AllForGui(2500);
		if(GuiNum != FeaSetN)
			break;		
		Shows_Crollbar(0, i);	//显示滚动条	
		//检测按键0是否按下，并执行相应的功能
		if(KEY0_st){
			KEY0_st = 0;
			switch(i){
				case 0:									//重置wifi
					mcu_set_wifi_mode(SMART_CONFIG);	//smart配网模式
					break;
				case 1:									//开灯/关灯
					LED_State_Switch();
					break;
				case 2:									//更新时间
					mcu_get_system_time();				//联网更新时间					
					break;
				case 3:
					OledRef = !OledRef;
					break;
			}
		}
		OLED_ShowNumber0(88, 4, calendar.hour, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(112, 4, calendar.min, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowString(104, 4, (u8 *)":", 16, OLED_CHR_NORMAL);
		//检测按键1是否按下，并改变滚动条的位置
		if(KEY1_st){
			KEY1_st = 0;
			i++;
			if(i > 3)
				i = 0;				
		}																			
	}	
}

//温湿度界面
void OledGui_Temp_Hum(void)
{
	OLED_Clear();
	OLED_ShowString(16, 0, (u8 *)":", 16, OLED_CHR_NORMAL);
	OLED_ShowChar(120, 0, '%', 16, OLED_CHR_NORMAL);
	OLED_C16x16Str(0, 4, (unsigned char*)"温度");
	OLED_C16x16Str(0, 6, (unsigned char*)"湿度");
	OLED_ShowChar(32, 4, ':', 16, OLED_CHR_NORMAL);
	OLED_ShowChar(32, 6, ':', 16, OLED_CHR_NORMAL);
	OLED_ShowChar(72, 4, '.', 16, OLED_CHR_NORMAL);
	OLED_ShowChar(72, 6, '.', 16, OLED_CHR_NORMAL);
	while(1){
		AllForGui(1000);
		GuiSwitch();
		if(GuiNum != GuiTempHum)
			break;
		/* 显示时间 */		
		OLED_ShowNumber0(0, 0, calendar.hour, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(24, 0, calendar.min, 2, 16, OLED_CHR_NORMAL);
		
		/* 显示电量 */
		OLED_ShowNumber(96, 0, BatElect, 3, 16, OLED_CHR_NORMAL);
				
		/* 显示温湿度 */
		OLED_ShowNum(48, 4, (int)Temperature, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(80, 4, (Temperature-(int)Temperature)*100, 2, 16, OLED_CHR_NORMAL);	//显示小数
		OLED_ShowNum(48, 6, (int)Humidity, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(80, 6, (Humidity-(int)Humidity)*100, 2, 16, OLED_CHR_NORMAL);		
	}
	
}

//时间显示界面
void OledGui_Time(void)
{
	OLED_Clear();
	OLED_C16x16Str(0, 0, (unsigned char*)"当前时间");
	OLED_C16x16Str(80, 6, (unsigned char*)"星期");
	OLED_ShowString(72, 0, (u8 *)"(UTC+8)", 16, OLED_CHR_NORMAL);
	OLED_ShowString(0, 6, (u8 *)"    /  /", 16, OLED_CHR_NORMAL);
	OLED_ShowString(0, 2, (u8 *)"  :  :", 32, OLED_CHR_NORMAL);
	while(1){
		AllForGui(600);
		GuiSwitch();
		if(GuiNum != GuiTime)
			break;	
		OLED_ShowNumber0(0,  6, calendar.w_year,  4, 16, OLED_CHR_NORMAL);	//显示年份
		OLED_ShowNumber0(40, 6, calendar.w_month, 2, 16, OLED_CHR_NORMAL);	//显示月份
		OLED_ShowNumber0(64, 6, calendar.w_date,  2, 16, OLED_CHR_NORMAL);	//显示日期
		
		OLED_ShowNumber0(0,  2, calendar.hour, 2, 32, OLED_CHR_NORMAL);	 	//时
		OLED_ShowNumber0(48, 2, calendar.min,  2, 32, OLED_CHR_NORMAL);	 	//分
		OLED_ShowNumber0(96, 2, calendar.sec,  2, 32, OLED_CHR_NORMAL);	 	//秒
		
		/* 显示星期几 */
		switch(RTC_Get_Week(calendar.w_year, \
				calendar.w_month, calendar.w_date)){
			case 1:
				OLED_C16x16Str(112, 6, (unsigned char*)"一");
				break;
			case 2:
				OLED_C16x16Str(112, 6, (unsigned char*)"二");
				break;
			case 3:
				OLED_C16x16Str(112, 6, (unsigned char*)"三");
				break;
			case 4:
				OLED_C16x16Str(112, 6, (unsigned char*)"四");
				break;
			case 5:
				OLED_C16x16Str(112, 6, (unsigned char*)"五");
				break;
			case 6:
				OLED_C16x16Str(112, 6, (unsigned char*)"六");
				break;
			case 7:
				OLED_C16x16Str(112, 6, (unsigned char*)"日");
				break;
		}
	}	
}

int Wea_Temp = 0;				//温度
int Wea_Humidity = 0;			//湿度
char Wea_WindSpeed[8] = {0};	//风速
char Wea_Condition[8] = {0};	//天气
u8 *Wea_bmp = sunny;

//cloudy,thunder,sunny,snows,rain
//天气显示界面
void OledGui_Weather(void)
{
	OLED_Clear();
	OLED_ShowString(64, 2, (u8 *)"Temp:", 16, OLED_CHR_NORMAL);
	OLED_ShowString(64, 4, (u8 *)"Humi:", 16, OLED_CHR_NORMAL);
	OLED_ShowString(64, 6, (u8 *)"Wine:", 16, OLED_CHR_NORMAL);
	
	while(1){
		
		AllForGui(800);
		GuiSwitch();
		if(GuiNum != GuiWeather)
			break;	
		OLED_ShowString(64, 0, (u8 *)Wea_Condition, 16, OLED_CHR_NORMAL);
		OLED_DrawBMP(0, 0, 64, 64, Wea_bmp);
		OLED_ShowNum(104, 2, Wea_Temp, 2, 16, OLED_CHR_NORMAL);	 			//温度
		OLED_ShowNumber(112, 4, Wea_Humidity,  2, 16, OLED_CHR_NORMAL);	 	//湿度
		OLED_ShowString(104, 6, (u8 *)Wea_WindSpeed, 16, OLED_CHR_NORMAL);		//风速
	}
		
}

