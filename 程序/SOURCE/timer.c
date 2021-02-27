#include "timer.h"
#include "led.h"
#include "adc.h" 	 
#include "sht30.h"
#include "usart.h"
#include "oled.h"

//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2!
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

u8 OledRef = 0; 
float BatVolt  = 0;		//��ص�ѹ
int   BatElect = 0;		//��ص���
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	static u8 t = 0;
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //���TIM3�����жϷ������		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 			
		if(OledRef)
			OLED_Refresh_Gram_ref();	//����OLED��ʾ,��ӳ��ʾ
		else 
			OLED_Refresh_Gram();		//����OLED��ʾ		
		t++;
		if(t > 10){
			t = 0;
			/* �����ص��� */
			BatVolt = Get_battery_volt();
			BatElect = (BatVolt - 3.6) / 0.6 * 100;
			if(BatElect > 100)
				BatElect = 100;
			else if(BatElect < 0)
				BatElect = 0;
			
			/* ��ȡ��ʪ��ֵ */
			SHT30_read_result(SHT30_ADDR);
		}
	}
}

