#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"


//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA1
}

//检测Key1脚的信号
//返回值1:连续按下1s以上
//      0:错误的触发	
u8 Check_Key1(void) 
{
	u8 t=0;	//记录按下的时间
	while(1){
		if(!KEY1){
			t++;			//已经按下了 
			delay_ms(10);
			if(t>=100)		//按下超过1秒钟				
				return 1; 	//按下1s以上了
		}else  		
			return 0;		//按下不足1秒
		
	}
} 

