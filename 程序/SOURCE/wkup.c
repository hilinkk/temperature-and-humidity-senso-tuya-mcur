#include "wkup.h"
#include "led.h"
#include "delay.h"
#include "power.h"

extern u8 OledRef;
extern u8 LedBit;

void Sys_Standby(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);	//ʹ�ܻ��ѹܽŹ���	
	PWR_EnterSTANDBYMode();		//���������STANDBY��ģʽ 		 
}

//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{			 
	BKP_WriteBackupRegister(BKP_DR2, LedBit);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	BKP_WriteBackupRegister(BKP_DR3, OledRef);
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);		//��λ����IO��
	Sys_Standby();
}

//���WKUP�ŵ��ź�
//����ֵ1:��������2s����
//      0:����Ĵ���	
u8 Check_WKUP(void) 
{
	u8 t=0;	//��¼���µ�ʱ��
	while(1){
		if(WKUP_KD){
			t++;			//�Ѿ������� 
			delay_ms(20);
			if(t>=100)		//���³���2����				
				return 1; 	//����2s������
		}else  		
			return 0;		//���²���2��
		
	}
} 



