#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    

//#define X_WIDTH 	128
//#define Y_WIDTH 	64

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_CHR_NORMAL  	0	//正常显示模式
#define OLED_CHR_FILL 		1	//填充显示模式

#define FSIZE32 32
#define FSIZE16 16
#define FSIZE12 12

//-----------------OLED IIC端口定义----------------  					   
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_9)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_8)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_8)


//OLED控制用函数
void OLED_Init(void);	    		//初始化SSD1306	
void OLED_Display_On(void);			//开启OLED显示 
void OLED_Display_Off(void);		//关闭OLED显示 
void OLED_Refresh_Gram(void);  		//更新显存到LCD	
void OLED_Refresh_Gram_ref(void);	//更新显存到LCD,倒映显示
void OLED_Clear(void);				//清屏函数
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size,u8 mode);

void OLED_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode);
void OLED_ShowNumber0(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode);
void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size, u8 mode);

void OLED_C16x16Str(u8 x,u8 y, u8 *ch);						//显示一行汉字
void OLED_DrawBMP(u8 x0, u8 y0,u8 wide, u8 high,u8 *BMP);	//显示BMP图片
void Shows_Crollbar(u8 x, u8 y);							//显示滚动条

void OLED_Draw_Circle(u16 x0,u16 y0,u8 r);					//画圆 
void OLED_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);			//画线
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);	//画矩形

#endif  
	 

