#include "wb3s.h"
#include "wifi.h"
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

extern u8 LedBit;

static void wifi_work_state_led(u8 state)
{
	switch(state){
		case SMART_CONFIG_STATE:	//绿色LED闪烁
			LEDG_Switch();
			break;
		case AP_STATE:				//绿色LED闪烁(本产品不会进入此模式)
			LEDG_Switch();
			break;
		case WIFI_NOT_CONNECTED:	//绿色LED常暗
			LEDG_OFF();
			break;
		case WIFI_CONNECTED:		//绿色LED常暗
			LEDG_OFF();
			break;
		case WIFI_CONN_CLOUD:		//绿色LED 常亮
			LEDG_ON();
			break;
		case WIFI_LOW_POWER:		//绿色LED 常暗
			LEDG_OFF();
			break;
		case SMART_AND_AP_STATE:	//绿色LED 常暗
			LEDG_OFF();
			break;
		case WIFI_SATE_UNKNOW:		//绿色LED 常暗
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
			printf("Smart 配置状态\n");
			break;
		case AP_STATE:			
			printf("AP 配置状态\n");
			break;
		case WIFI_NOT_CONNECTED:	
			printf("WIFI 配置完成但未连接上路由器\n");
			break;
		case WIFI_CONNECTED:	
			printf("路由器连接成功\n");
			break;
		case WIFI_CONN_CLOUD:		
			printf("wifi 已连上云端\n");
			break;
		case WIFI_LOW_POWER:		
			printf("WIFI 设备处于低功耗模式\n");
			break;
		case SMART_AND_AP_STATE:	
			printf("WIFI 设备处于smartconfig&AP配置状态\n");
			break;
		case WIFI_SATE_UNKNOW:		
			printf("WIFI_SATE_UNKNOW\n");
			break;
		default:
			printf("数据错误\n");
			break;
	}
}

void show_wifi_work_state(void)
{
	u8 state;
	state = mcu_get_wifi_work_state();
//	wifi_work_state_printf(state);		//串口打印wifi模块的当前工作模式
	wifi_work_state_led(state);			//根据wifi模块的当前工作模式来控制LED的状态
}


