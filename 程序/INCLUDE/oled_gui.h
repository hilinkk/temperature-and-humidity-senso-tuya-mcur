#ifndef __OLED_GUI_H
#define __OLED_GUI_H

#include "sys.h"

#define FeaSetN	    	0
#define GuiTempHum 		1
#define GuiTime 		2
#define GuiWeather 		3

void StartUp(void);				//��ʾ��������
void OledGui_FeaSet(void);		//���ý���
void OledGui_Temp_Hum(void);	//��ʪ�Ƚ���
void OledGui_Time(void);		//ʱ����ʾ����
void OledGui_Weather(void);		//������ʾ����

#endif
