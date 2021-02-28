#include "wkup.h"
#include "led.h"
#include "delay.h"
#include "power.h"

extern u8 OledRef;
extern u8 LedBit;

void Sys_Standby(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_WakeUpPinCmd(ENABLE);	//使能唤醒管脚功能	
	PWR_EnterSTANDBYMode();		//进入待命（STANDBY）模式 		 
}

//系统进入待机模式
void Sys_Enter_Standby(void)
{			 
	BKP_WriteBackupRegister(BKP_DR2, LedBit);	//向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(BKP_DR3, OledRef);
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);		//复位所有IO口
	Sys_Standby();
}

//检测WKUP脚的信号
//返回值1:连续按下2s以上
//      0:错误的触发	
u8 Check_WKUP(void) 
{
	u8 t=0;	//记录按下的时间
	while(1){
		if(WKUP_KD){
			t++;			//已经按下了 
			delay_ms(20);
			if(t>=100)		//按下超过2秒钟				
				return 1; 	//按下2s以上了
		}else  		
			return 0;		//按下不足2秒
		
	}
} 



