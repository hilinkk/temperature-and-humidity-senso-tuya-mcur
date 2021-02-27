#include "weather.h"
#include "mcu_api.h" 
#include "usart.h"	

extern char Wea_Condition[24];		//保存接收到的中文天气
extern u8 *Wea_bmp;
extern u8 cloudy[],thunder[],sunny[],snows[],rain[];
void Weather_Judge(char *value_string)
{
	//rain
	if(
		(my_strcmp(value_string, "雨") == 0) 		|| 	
		(my_strcmp(value_string, "小雨") == 0) 		|| 	
		(my_strcmp(value_string, "小到中雨") == 0) 	|| 	
		(my_strcmp(value_string, "中雨") == 0) 		|| 	
		(my_strcmp(value_string, "中到大雨") == 0) 	|| 	
		(my_strcmp(value_string, "大雨") == 0) 		|| 	
		(my_strcmp(value_string, "大到暴雨") == 0) 	|| 		
		(my_strcmp(value_string, "暴雨") == 0) 		|| 	
		(my_strcmp(value_string, "大暴雨") == 0) 	|| 	
		(my_strcmp(value_string, "特大暴雨") == 0) 	|| 	
		(my_strcmp(value_string, "阵雨") == 0)  	|| 		
		(my_strcmp(value_string, "局部阵雨") == 0) 	|| 	
		(my_strcmp(value_string, "小阵雨") == 0) 	|| 			
		(my_strcmp(value_string, "强阵雨") == 0) 	 			
	){				
		printf("rain\n");
		my_strcpy((char *)Wea_Condition, (char *)"rain");
		Wea_bmp = rain;
	}
	//cloudy   
	else if(
		(my_strcmp(value_string, "多云") == 0) 		|| 
		(my_strcmp(value_string, "阴") == 0) 		|| 	
		(my_strcmp(value_string, "雾") == 0) 	 		
	){			
		printf("cloudy\n");
		my_strcpy((char *)Wea_Condition, (char *)"cloudy");
		Wea_bmp = cloudy;
	}
	//thunder //  
	else if(
		(my_strcmp(value_string, "雷阵雨") == 0)	|| 	
		(my_strcmp(value_string, "雷暴") == 0)		|| 	
		(my_strcmp(value_string, "雷电") == 0) 	 		
	){			
		printf("thunder\n");
		my_strcpy((char *)Wea_Condition, (char *)"thunder");
		Wea_bmp = thunder;
	}
	//snows          
	else if(
		(my_strcmp(value_string, "阵雪") == 0) 		|| 	
		(my_strcmp(value_string, "小雪") == 0) 		|| 	
		(my_strcmp(value_string, "雪") == 0) 		|| 	
		(my_strcmp(value_string, "大雪") == 0) 		|| 	
		(my_strcmp(value_string, "小到中雪") == 0) 	|| 	
		(my_strcmp(value_string, "小阵雪") == 0) 	|| 	
		(my_strcmp(value_string, "中雪") == 0) 		|| 		
		(my_strcmp(value_string, "雨夹雪") == 0) 	|| 	
		(my_strcmp(value_string, "暴雪") == 0) 			
	){			
		printf("snows\n");
		my_strcpy((char *)Wea_Condition, (char *)"snows");
		Wea_bmp = snows;
	}
	//除上面列到的条件以外都当成是晴天，sunny
	else{		
		printf("sunny\n");
		my_strcpy((char *)Wea_Condition, (char *)"sunny");
		Wea_bmp = sunny;
	}
}


