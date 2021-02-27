#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"


//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA1
}

//���Key1�ŵ��ź�
//����ֵ1:��������1s����
//      0:����Ĵ���	
u8 Check_Key1(void) 
{
	u8 t=0;	//��¼���µ�ʱ��
	while(1){
		if(!KEY1){
			t++;			//�Ѿ������� 
			delay_ms(10);
			if(t>=100)		//���³���1����				
				return 1; 	//����1s������
		}else  		
			return 0;		//���²���1��
		
	}
} 

