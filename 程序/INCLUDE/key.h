#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY0 PAin(0)   	
#define KEY1 PAin(1)	

void KEY_Init(void);	//IO初始化
u8 Check_Key1(void);	//检测Key1脚的信号

#endif
