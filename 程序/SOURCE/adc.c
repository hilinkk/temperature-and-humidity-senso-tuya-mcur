#include "adc.h"
#include "delay.h"

//��ֵ�˲�
static u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
//		delay_ms(1);
	}
	return temp_val/times;
} 

u8 N = 12;//�˲�������
static u16 AD_result[15];	//���֮ǰ��ȡ����ADCֵ
const u8 coe[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//��Ȩϵ����
//��Ȩ����ƽ���˲���
//������ڣ�ad_buff���洢֮ǰ�Ĳ���ֵ��ad_value���²ɼ��������ݣ�Ncount���²�����Ȩֵ
static u16 WeightValueFilter(u16 *ad_buff,u16 ad_value,u16 Ncount) //��Ȩ����ƽ���˲���
{
    u8 i,sum_coe = 0;//coe����Ϊ��Ȩϵ�������ڳ���洢��
    u32 sum = 0;
    ad_buff[Ncount] = ad_value;
    for(i=0;i<Ncount;i++)
    {
        ad_buff[i] = ad_buff[i+1];//�����������ƣ���λ�Ե�
        sum += ad_buff[i] * coe[i];
        sum_coe += coe[i];        //����Ȩ�����
    }
    return (u16)(sum / sum_coe);
}

/* ADC ��ʼ��*/
void  Adc_Init(void)
{    
	u8 i = 0;
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//PA7��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);		//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC1, ENABLE);					//ʹ��ָ����ADC1
	ADC_ResetCalibration(ADC1);				//ʹ�ܸ�λУ׼  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����	
	ADC_StartCalibration(ADC1);	 				//����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 	//�ȴ�У׼����
	
	//�Զ�ȡ����ADCֵ���м�Ȩ����ƽ���˲�ǰ��׼��
	for(i = 0; i < N; i++)
		WeightValueFilter(AD_result,Get_Adc(Battery_Ch),N);
	
}		

/**************************************************************************
�������ܣ�AD����
��ڲ�����ADC1 ��ͨ��
����  ֵ��ADת�����
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			//ʹ��ָ����ADC1�����ת����������		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);			//�������һ��ADC1�������ת�����
}



/**************************************************************************
�������ܣ���ȡ��ص�ѹ 
��ڲ�������
����  ֵ����ص�ѹ ��λmV
**************************************************************************/
float Get_battery_volt(void)   
{  
	float Volt;	
	/* ���Ρ����о�ֵ�˲����͡���Ȩ����ƽ���˲��� */
	u16 Adc_Filter = WeightValueFilter(AD_result,Get_Adc_Average(Battery_Ch,5),N);
	Volt =  Adc_Filter * 3.3 / 4096 * 2;
	return Volt;
}
