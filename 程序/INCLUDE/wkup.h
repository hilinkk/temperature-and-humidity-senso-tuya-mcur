#ifndef __WKUP_H
#define __WKUP_H	 
#include "sys.h"
					    
#define WKUP_KD PAin(0)			//PA0 检测是否外部WK_UP按键按下
	 
void Sys_Standby(void);
u8 Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PA0 WKUP唤醒初始化
void Sys_Enter_Standby(void);	//系统进入待机模式
#endif


