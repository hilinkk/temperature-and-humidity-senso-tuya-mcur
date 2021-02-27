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
extern u8 LedBit;		//LED��־λ��Ϊ1ʱLED���Ե���
extern u8 KEY0_st;
extern u8 KEY1_st;
extern u8 KEY1_lt;
extern u8 OledRef;
u8 GuiNum = GuiTempHum;
u8 GuiNum_Ago = FeaSetN;
//ҳ���л�
static void GuiSwitch(void)
{
	if(KEY0_st || KEY1_st){	//��ⰴ���Ƿ���
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

//����
void StartUp(void)
{
	OLED_Clear();
	OLED_DrawBMP(24, 0, 80, 64, BiliBili1);
	delay_ms(1000);
	delay_ms(1000);
	OLED_Clear();	
	OLED_DrawBMP(34, 0, 60, 48, BiliBili2);	
	OLED_C16x16Str(0, 6, (unsigned char*)"���ڿ���");
	delay_ms(500);	
	OLED_C16x16Str(0, 6, (unsigned char*)"���ڿ�����");
	delay_ms(500);
	OLED_C16x16Str(0, 6, (unsigned char*)"���ڿ�������");
	delay_ms(500);
	OLED_C16x16Str(0, 6, (unsigned char*)"���ڿ���������");
	delay_ms(500);
	OLED_C16x16Str(0, 6, (unsigned char*)"���ڿ�����������");
	delay_ms(500);
	OLED_Clear();

}

//������ʾ���涼Ҫ��ɵĹ���
//time :LED��˸��ʱ�䣬��ֵԽ����˸Խ��
static void AllForGui(u16 time)
{
	static u16 t = 0;
	static u8 min = 0;
	t++;
	wifi_uart_service();	//wifi�������ݴ������
	steering_control();		//�������
	
	if(KEY1_lt){			//��⵽KEY1�����������˳����ý���
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
		mcu_get_system_time();		//ÿ���Ӹ���һ��ʱ��
	}
	if(t > time){		
		t = 0;
		show_wifi_work_state();
		all_data_update();				//�ϱ�����
		LEDB_Switch();					//��ɫLED��˸����ʾ������������		
		if(0 == BatElect){				//���ݵ�ǰ�������ƺ�ɫLED
			Sys_Enter_Standby();
		}else if(BatElect < 20){		/* ��������20%ʱ�����˸ */			
			LEDR_Switch();
		}else{
			LEDR_OFF();
		}
	}
}

//���ý���
void OledGui_FeaSet(void)
{
	u8 i = 0;
	OLED_Clear();
	OLED_C16x16Str(16, 0, (unsigned char*)"����");
	OLED_ShowString(48, 0, (u8 *)"wifi", 16, OLED_CHR_NORMAL);
	OLED_C16x16Str(16, 2, (unsigned char*)"����");
	OLED_C16x16Str(56, 2, (unsigned char*)"�ص�");
	OLED_ShowChar(48, 2, '/', 16, OLED_CHR_NORMAL);
	OLED_C16x16Str(16, 4, (unsigned char*)"����ʱ��");
	OLED_C16x16Str(16, 6, (unsigned char*)"������ʾ");
	while(1){
		AllForGui(2500);
		if(GuiNum != FeaSetN)
			break;		
		Shows_Crollbar(0, i);	//��ʾ������	
		//��ⰴ��0�Ƿ��£���ִ����Ӧ�Ĺ���
		if(KEY0_st){
			KEY0_st = 0;
			switch(i){
				case 0:									//����wifi
					mcu_set_wifi_mode(SMART_CONFIG);	//smart����ģʽ
					break;
				case 1:									//����/�ص�
					LED_State_Switch();
					break;
				case 2:									//����ʱ��
					mcu_get_system_time();				//��������ʱ��					
					break;
				case 3:
					OledRef = !OledRef;
					break;
			}
		}
		OLED_ShowNumber0(88, 4, calendar.hour, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(112, 4, calendar.min, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowString(104, 4, (u8 *)":", 16, OLED_CHR_NORMAL);
		//��ⰴ��1�Ƿ��£����ı��������λ��
		if(KEY1_st){
			KEY1_st = 0;
			i++;
			if(i > 3)
				i = 0;				
		}																			
	}	
}

//��ʪ�Ƚ���
void OledGui_Temp_Hum(void)
{
	OLED_Clear();
	OLED_ShowString(16, 0, (u8 *)":", 16, OLED_CHR_NORMAL);
	OLED_ShowChar(120, 0, '%', 16, OLED_CHR_NORMAL);
	OLED_C16x16Str(0, 4, (unsigned char*)"�¶�");
	OLED_C16x16Str(0, 6, (unsigned char*)"ʪ��");
	OLED_ShowChar(32, 4, ':', 16, OLED_CHR_NORMAL);
	OLED_ShowChar(32, 6, ':', 16, OLED_CHR_NORMAL);
	OLED_ShowChar(72, 4, '.', 16, OLED_CHR_NORMAL);
	OLED_ShowChar(72, 6, '.', 16, OLED_CHR_NORMAL);
	while(1){
		AllForGui(1000);
		GuiSwitch();
		if(GuiNum != GuiTempHum)
			break;
		/* ��ʾʱ�� */		
		OLED_ShowNumber0(0, 0, calendar.hour, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(24, 0, calendar.min, 2, 16, OLED_CHR_NORMAL);
		
		/* ��ʾ���� */
		OLED_ShowNumber(96, 0, BatElect, 3, 16, OLED_CHR_NORMAL);
				
		/* ��ʾ��ʪ�� */
		OLED_ShowNum(48, 4, (int)Temperature, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(80, 4, (Temperature-(int)Temperature)*100, 2, 16, OLED_CHR_NORMAL);	//��ʾС��
		OLED_ShowNum(48, 6, (int)Humidity, 2, 16, OLED_CHR_NORMAL);
		OLED_ShowNumber0(80, 6, (Humidity-(int)Humidity)*100, 2, 16, OLED_CHR_NORMAL);		
	}
	
}

//ʱ����ʾ����
void OledGui_Time(void)
{
	OLED_Clear();
	OLED_C16x16Str(0, 0, (unsigned char*)"��ǰʱ��");
	OLED_C16x16Str(80, 6, (unsigned char*)"����");
	OLED_ShowString(72, 0, (u8 *)"(UTC+8)", 16, OLED_CHR_NORMAL);
	OLED_ShowString(0, 6, (u8 *)"    /  /", 16, OLED_CHR_NORMAL);
	OLED_ShowString(0, 2, (u8 *)"  :  :", 32, OLED_CHR_NORMAL);
	while(1){
		AllForGui(600);
		GuiSwitch();
		if(GuiNum != GuiTime)
			break;	
		OLED_ShowNumber0(0,  6, calendar.w_year,  4, 16, OLED_CHR_NORMAL);	//��ʾ���
		OLED_ShowNumber0(40, 6, calendar.w_month, 2, 16, OLED_CHR_NORMAL);	//��ʾ�·�
		OLED_ShowNumber0(64, 6, calendar.w_date,  2, 16, OLED_CHR_NORMAL);	//��ʾ����
		
		OLED_ShowNumber0(0,  2, calendar.hour, 2, 32, OLED_CHR_NORMAL);	 	//ʱ
		OLED_ShowNumber0(48, 2, calendar.min,  2, 32, OLED_CHR_NORMAL);	 	//��
		OLED_ShowNumber0(96, 2, calendar.sec,  2, 32, OLED_CHR_NORMAL);	 	//��
		
		/* ��ʾ���ڼ� */
		switch(RTC_Get_Week(calendar.w_year, \
				calendar.w_month, calendar.w_date)){
			case 1:
				OLED_C16x16Str(112, 6, (unsigned char*)"һ");
				break;
			case 2:
				OLED_C16x16Str(112, 6, (unsigned char*)"��");
				break;
			case 3:
				OLED_C16x16Str(112, 6, (unsigned char*)"��");
				break;
			case 4:
				OLED_C16x16Str(112, 6, (unsigned char*)"��");
				break;
			case 5:
				OLED_C16x16Str(112, 6, (unsigned char*)"��");
				break;
			case 6:
				OLED_C16x16Str(112, 6, (unsigned char*)"��");
				break;
			case 7:
				OLED_C16x16Str(112, 6, (unsigned char*)"��");
				break;
		}
	}	
}

int Wea_Temp = 0;				//�¶�
int Wea_Humidity = 0;			//ʪ��
char Wea_WindSpeed[8] = {0};	//����
char Wea_Condition[8] = {0};	//����
u8 *Wea_bmp = sunny;

//cloudy,thunder,sunny,snows,rain
//������ʾ����
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
		OLED_ShowNum(104, 2, Wea_Temp, 2, 16, OLED_CHR_NORMAL);	 			//�¶�
		OLED_ShowNumber(112, 4, Wea_Humidity,  2, 16, OLED_CHR_NORMAL);	 	//ʪ��
		OLED_ShowString(104, 6, (u8 *)Wea_WindSpeed, 16, OLED_CHR_NORMAL);		//����
	}
		
}

