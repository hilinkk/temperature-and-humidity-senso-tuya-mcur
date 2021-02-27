#include "wb3s.h"
#include "wifi.h"
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

extern u8 LedBit;

static void wifi_work_state_led(u8 state)
{
	switch(state){
		case SMART_CONFIG_STATE:	//��ɫLED��˸
			LEDG_Switch();
			break;
		case AP_STATE:				//��ɫLED��˸(����Ʒ��������ģʽ)
			LEDG_Switch();
			break;
		case WIFI_NOT_CONNECTED:	//��ɫLED����
			LEDG_OFF();
			break;
		case WIFI_CONNECTED:		//��ɫLED����
			LEDG_OFF();
			break;
		case WIFI_CONN_CLOUD:		//��ɫLED ����
			LEDG_ON();
			break;
		case WIFI_LOW_POWER:		//��ɫLED ����
			LEDG_OFF();
			break;
		case SMART_AND_AP_STATE:	//��ɫLED ����
			LEDG_OFF();
			break;
		case WIFI_SATE_UNKNOW:		//��ɫLED ����
			LEDG_OFF();
			break;
		default:
			LEDG_OFF();
			break;
	}

}

static void wifi_work_state_printf(u8 state)
{
	switch(state){
		case SMART_CONFIG_STATE:	
			printf("Smart ����״̬\n");
			break;
		case AP_STATE:			
			printf("AP ����״̬\n");
			break;
		case WIFI_NOT_CONNECTED:	
			printf("WIFI ������ɵ�δ������·����\n");
			break;
		case WIFI_CONNECTED:	
			printf("·�������ӳɹ�\n");
			break;
		case WIFI_CONN_CLOUD:		
			printf("wifi �������ƶ�\n");
			break;
		case WIFI_LOW_POWER:		
			printf("WIFI �豸���ڵ͹���ģʽ\n");
			break;
		case SMART_AND_AP_STATE:	
			printf("WIFI �豸����smartconfig&AP����״̬\n");
			break;
		case WIFI_SATE_UNKNOW:		
			printf("WIFI_SATE_UNKNOW\n");
			break;
		default:
			printf("���ݴ���\n");
			break;
	}
}

void show_wifi_work_state(void)
{
	u8 state;
	state = mcu_get_wifi_work_state();
//	wifi_work_state_printf(state);		//���ڴ�ӡwifiģ��ĵ�ǰ����ģʽ
	wifi_work_state_led(state);			//����wifiģ��ĵ�ǰ����ģʽ������LED��״̬
}


