#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    

//#define X_WIDTH 	128
//#define Y_WIDTH 	64

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define OLED_CHR_NORMAL  	0	//������ʾģʽ
#define OLED_CHR_FILL 		1	//�����ʾģʽ

#define FSIZE32 32
#define FSIZE16 16
#define FSIZE12 12

//-----------------OLED IIC�˿ڶ���----------------  					   
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_9)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_8)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_8)


//OLED�����ú���
void OLED_Init(void);	    		//��ʼ��SSD1306	
void OLED_Display_On(void);			//����OLED��ʾ 
void OLED_Display_Off(void);		//�ر�OLED��ʾ 
void OLED_Refresh_Gram(void);  		//�����Դ浽LCD	
void OLED_Refresh_Gram_ref(void);	//�����Դ浽LCD,��ӳ��ʾ
void OLED_Clear(void);				//��������
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size,u8 mode);

void OLED_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode);
void OLED_ShowNumber0(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode);
void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size, u8 mode);

void OLED_C16x16Str(u8 x,u8 y, u8 *ch);						//��ʾһ�к���
void OLED_DrawBMP(u8 x0, u8 y0,u8 wide, u8 high,u8 *BMP);	//��ʾBMPͼƬ
void Shows_Crollbar(u8 x, u8 y);							//��ʾ������

void OLED_Draw_Circle(u16 x0,u16 y0,u8 r);					//��Բ 
void OLED_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);			//����
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);	//������

#endif  
	 

