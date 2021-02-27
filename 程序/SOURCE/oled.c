#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"


//OLED的显存		
u8 OLED_GRAM[128][8] = {0};	 

/* IIC Start */
static void OLED_IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/* IIC Stop */
static void OLED_IIC_Stop()
{
	OLED_SCLK_Set();
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

static void OLED_IIC_Wait_Ack()
{
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}

/* IIC Write byte */
static void OLED_Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da = IIC_Byte;
	OLED_SCLK_Clr();
	for(i = 0; i < 8; i++){
		m = da;
		m = m&0x80;
		if(m == 0x80){
			OLED_SDIN_Set();
		}else 
			OLED_SDIN_Clr();
		da = da << 1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}

/* IIC Write Command */
static void OLED_Write_IIC_Command(unsigned char IIC_Command)
{
	OLED_IIC_Start();
	OLED_Write_IIC_Byte(0x78);  	//Slave address,SA0=0
	OLED_IIC_Wait_Ack();	
	OLED_Write_IIC_Byte(0x00);		//write command
	OLED_IIC_Wait_Ack();	
	OLED_Write_IIC_Byte(IIC_Command); 
	OLED_IIC_Wait_Ack();	
	OLED_IIC_Stop();
}

/* IIC Write Data */
static void OLED_Write_IIC_Data(unsigned char IIC_Data)
{
		OLED_IIC_Start();
		OLED_Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		OLED_IIC_Wait_Ack();	
		OLED_Write_IIC_Byte(0x40);			//write data
		OLED_IIC_Wait_Ack();	
		OLED_Write_IIC_Byte(IIC_Data);
		OLED_IIC_Wait_Ack();	
		OLED_IIC_Stop();
}


//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
static void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	if(cmd){
		OLED_Write_IIC_Data(dat);
	}else{
		OLED_Write_IIC_Command(dat);
	}   	  
} 

//初始化SSD1306					    
void OLED_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				//初始化GPIOB8,9
 	GPIO_SetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9);	

	delay_ms(800);
	OLED_WR_Byte(0xAE,OLED_CMD);	//--display off
	OLED_WR_Byte(0x00,OLED_CMD);	//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);	//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);	//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);	//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); 	// contract control
	OLED_WR_Byte(0xFF,OLED_CMD);	//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);	//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);	//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);	//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);	//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);	//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);	//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);	//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		

////坐标设置
//static void OLED_Set_Pos(unsigned char x, unsigned char y) 
//{ 	
//	OLED_WR_Byte(0xb0 + y, OLED_CMD);
//	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
//	OLED_WR_Byte((x & 0x0f), OLED_CMD); 
//} 

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i = 0; i < 8; i++)  
	{  
		OLED_WR_Byte (0xb0 + i, OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00, OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10, OLED_CMD);      //设置显示位置—列高地址   
		for(n = 0; n < 128; n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA); 
	}   
}

//将一个8位数据倒置过来
static u8 char_ref(u8 chr)
{
	u8 chr1=0, i;
	for(i = 0; i < 8; i++)
		chr1 |= ((chr>>i)&0x01)<<(7-i);
	return chr1;
}

//更新显存到LCD,倒映显示		 
void OLED_Refresh_Gram_ref(void)
{
	u8 i,j;		   
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(j=0;j<128;j++)
			OLED_WR_Byte(char_ref(OLED_GRAM[j][7-i]),OLED_DATA); 
	}   
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++){
		for(n=0;n<128;n++){
			OLED_GRAM[n][i]=0X00;
		}
	}	
	OLED_Refresh_Gram();
}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
static void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127 || y>63)
		return;//超出范围了.
	pos = y/8;
	bx = y%8;
	temp = 1<<bx;
	if(t)
		OLED_GRAM[x][pos] |= temp;
	else 
		OLED_GRAM[x][pos] &= ~temp;	    
}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++){
		for(y=y1;y<=y2;y++)
		OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63				 
//size:选择字体 32/16/12 
//mode:模式	0,正常模式;1,填充模式
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{  
	u8 i, j;
	u8 *temp = NULL;
	u8 csize1 = size/8, csize2 = size/2;
	
	if((x > 127) || (y > 7))
		return;
	chr = chr-' ';//得到偏移后的值	
	if(mode)
		mode = 0xff;	
	switch(size){
		case 12:
			temp = (u8 *)(&F6X8[chr*6]); 	 	//调用0806字体
			break;
		case 16:
			temp = (u8 *)(&F8X16[chr*16]); 	 	//调用1608字体
			break;
		case 32:
			temp = (u8 *)(&F16X32[chr*16*4]); 	//调用3216字体
			break;
		default:
			return;
	}
	for(j = 0; j < csize1; j++){
		for(i = 0; i < csize2; i++){
			OLED_GRAM[x+i][y+j] = (*temp++ ^ mode);
		}
	}
	
}

//m^n函数
static u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}  

//显示一个字符号串
//x,y :起点坐标	 
//size:字体大小
//mode:模式	0,正常模式;1,填充模式
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size,u8 mode)
{
	if((x > 127) || (y > 7))return;
	while (*chr != '\0'){		
		OLED_ShowChar(x, y, *chr, size, mode);
		x += size/2;
		if(x > 127){
			return;
		}
		chr++;
	}
}

//显示无符号的多位数字，多余的位用' '填充
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	
void OLED_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode)
{         	
	u8 t,temp;
	u8 enshow = 0;						   
	for(t = 0; t < len; t++){
		temp = (num/oled_pow(10, len-t-1)) % 10;
		if(enshow == 0 && t < (len-1)){
			if(temp == 0){
				OLED_ShowChar(x+(size/2)*t, y, ' ', size, mode);
				continue;
			}else enshow=1; 		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t, y, temp+'0', size, mode); 
	}
}  

//显示无符号的多位数字，多余的位用'0'填充
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	
void OLED_ShowNumber0(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode)
{         	
	u8 t,temp;
	u8 enshow = 0;						   
	for(t = 0; t < len; t++){
		temp = (num/oled_pow(10, len-t-1)) % 10;
		if(enshow == 0 && t < (len-1)){
			if(temp == 0){
				OLED_ShowChar(x+(size/2)*t, y, '0', size, mode);
				continue;
			}else enshow=1; 		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t, y, temp+'0', size, mode); 
	}
}

//显示有符号的多位数字,正数在前面加' ',负数则在前面加'-'
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值;	
void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size, u8 mode)
{
	if(num<0){ 
		num = -num;
		OLED_ShowChar(x, y, '-', size, mode);
		OLED_ShowNumber(x+8, y, num, len, size, mode); 
	}else{
		OLED_ShowChar(x, y, ' ', size, mode);
		OLED_ShowNumber(x+8, y, num, len, size, mode);
	}       
}

//显示一行汉字,大小为16X16
//x,y :起点坐标	 
//ch  :汉字字符串	
void OLED_C16x16Str(u8 x,u8 y, u8 *ch)
{
	unsigned char wm=0,ii = 0;
    unsigned int adder=1; 
    
    while(ch[ii] != '\0'){
        wm = 0;
        adder = 1;
        while(C16x16_Idx[wm] > 127){
            if(C16x16_Idx[wm] == (unsigned char)ch[ii]){
                if(C16x16_Idx[wm + 1] == ch[ii + 1]){
                    adder = wm * 16;
                    break;
				}
			}
            wm += 2;
		}
        if(adder != 1){		// 显示汉字					     
            for(wm = 0;wm < 16;wm++)
				OLED_GRAM[x+wm][y] = C16x16[adder++];    
            for(wm = 0;wm < 16;wm++)
				OLED_GRAM[x+wm][y+1] = C16x16[adder++];  		
        }else{				//显示空白字符			       
            ii += 1;
            for(wm = 0;wm < 16;wm++)       
				OLED_GRAM[x+wm][y] = 0;
			for(wm = 0;wm < 16;wm++)               		
				OLED_GRAM[x+wm][y+1] = 0;
		}
        x += 16;
        ii += 2;
    }
}

//显示BMP图片
//x0:起点x坐标,0~127
//y0:起点y坐标,0~63
//wide:图片宽度
//high:图片高度
//BMP:要显示的图像
void OLED_DrawBMP(u8 x0, u8 y0,u8 wide, u8 high,u8 *BMP)
{ 	
	u8 x, y, x1, y1;
	u32	i=0;
	x1 = x0 + wide; 
	y1 = (y0+high) / 8;      	
	for(y = y0/8; y < y1; y++){
		if(y > 7)return;
		for(x = x0; x < x1; x++){      
			OLED_GRAM[x][y] = BMP[i++];
		}
	}
}

//功能：显示滚动条
//x: 滚动条的x坐标，取值范围 0 ~ 127
//y: 滚动条的y坐标，取值范围 0 ~ 3
void Shows_Crollbar(u8 x, u8 y)
{
	int i, j;
	if(x > 127)
		x = x % 128;
	if(y > 3)
		y = y % 4;
	for(j = 0;j < 8;j++){
		if((j == y*2) || (j == y*2+1)){
			for(i = 0; i < 8; i++)	
				OLED_GRAM[x+i][j] = 0xff;
		}else{
			for(i = 0; i < 8; i++)
				OLED_GRAM[x+i][j] = 0;
		}
	}
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void OLED_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		OLED_DrawPoint(x0+a,y0-b,1);             //5
 		OLED_DrawPoint(x0+b,y0-a,1);             //0           
		OLED_DrawPoint(x0+b,y0+a,1);             //4               
		OLED_DrawPoint(x0+a,y0+b,1);             //6 
		OLED_DrawPoint(x0-a,y0+b,1);             //1       
 		OLED_DrawPoint(x0-b,y0+a,1);             
		OLED_DrawPoint(x0-a,y0-b,1);             //2             
  		OLED_DrawPoint(x0-b,y0-a,1);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void OLED_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		OLED_DrawPoint(uRow,uCol,1);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	OLED_DrawLine(x1,y1,x2,y1);
	OLED_DrawLine(x1,y1,x1,y2);
	OLED_DrawLine(x1,y2,x2,y2);
	OLED_DrawLine(x2,y1,x2,y2);
}

