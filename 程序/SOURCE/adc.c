#include "adc.h"
#include "delay.h"

//均值滤波
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

u8 N = 12;//滤波样本数
static u16 AD_result[15];	//存放之前读取到的ADC值
const u8 coe[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//加权系数表
//加权递推平均滤波法
//函数入口：ad_buff，存储之前的采样值，ad_value，新采集到的数据，Ncount，新采样数权值
static u16 WeightValueFilter(u16 *ad_buff,u16 ad_value,u16 Ncount) //加权递推平均滤波法
{
    u8 i,sum_coe = 0;//coe数组为加权系数表，存在程序存储区
    u32 sum = 0;
    ad_buff[Ncount] = ad_value;
    for(i=0;i<Ncount;i++)
    {
        ad_buff[i] = ad_buff[i+1];//所有数据左移，低位仍掉
        sum += ad_buff[i] * coe[i];
        sum_coe += coe[i];        //所有权数相加
    }
    return (u16)(sum / sum_coe);
}

/* ADC 初始化*/
void  Adc_Init(void)
{    
	u8 i = 0;
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	//PA7作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);		//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Cmd(ADC1, ENABLE);					//使能指定的ADC1
	ADC_ResetCalibration(ADC1);				//使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束	
	ADC_StartCalibration(ADC1);	 				//开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 	//等待校准结束
	
	//对读取到的ADC值进行加权递推平均滤波前的准备
	for(i = 0; i < N; i++)
		WeightValueFilter(AD_result,Get_Adc(Battery_Ch),N);
	
}		

/**************************************************************************
函数功能：AD采样
入口参数：ADC1 的通道
返回  值：AD转换结果
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//等待转换结束
	return ADC_GetConversionValue(ADC1);			//返回最近一次ADC1规则组的转换结果
}



/**************************************************************************
函数功能：读取电池电压 
入口参数：无
返回  值：电池电压 单位mV
**************************************************************************/
float Get_battery_volt(void)   
{  
	float Volt;	
	/* 依次“进行均值滤波”和“加权递推平均滤波” */
	u16 Adc_Filter = WeightValueFilter(AD_result,Get_Adc_Average(Battery_Ch,5),N);
	Volt =  Adc_Filter * 3.3 / 4096 * 2;
	return Volt;
}
