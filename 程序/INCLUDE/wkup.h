#ifndef __WKUP_H
#define __WKUP_H	 
#include "sys.h"
					    
#define WKUP_KD PAin(0)			//PA0 ����Ƿ��ⲿWK_UP��������
	 
void Sys_Standby(void);
u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PA0 WKUP���ѳ�ʼ��
void Sys_Enter_Standby(void);	//ϵͳ�������ģʽ
#endif


