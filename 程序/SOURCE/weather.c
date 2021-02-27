#include "weather.h"
#include "mcu_api.h" 
#include "usart.h"	

extern char Wea_Condition[24];		//������յ�����������
extern u8 *Wea_bmp;
extern u8 cloudy[],thunder[],sunny[],snows[],rain[];
void Weather_Judge(char *value_string)
{
	//rain
	if(
		(my_strcmp(value_string, "��") == 0) 		|| 	
		(my_strcmp(value_string, "С��") == 0) 		|| 	
		(my_strcmp(value_string, "С������") == 0) 	|| 	
		(my_strcmp(value_string, "����") == 0) 		|| 	
		(my_strcmp(value_string, "�е�����") == 0) 	|| 	
		(my_strcmp(value_string, "����") == 0) 		|| 	
		(my_strcmp(value_string, "�󵽱���") == 0) 	|| 		
		(my_strcmp(value_string, "����") == 0) 		|| 	
		(my_strcmp(value_string, "����") == 0) 	|| 	
		(my_strcmp(value_string, "�ش���") == 0) 	|| 	
		(my_strcmp(value_string, "����") == 0)  	|| 		
		(my_strcmp(value_string, "�ֲ�����") == 0) 	|| 	
		(my_strcmp(value_string, "С����") == 0) 	|| 			
		(my_strcmp(value_string, "ǿ����") == 0) 	 			
	){				
		printf("rain\n");
		my_strcpy((char *)Wea_Condition, (char *)"rain");
		Wea_bmp = rain;
	}
	//cloudy   
	else if(
		(my_strcmp(value_string, "����") == 0) 		|| 
		(my_strcmp(value_string, "��") == 0) 		|| 	
		(my_strcmp(value_string, "��") == 0) 	 		
	){			
		printf("cloudy\n");
		my_strcpy((char *)Wea_Condition, (char *)"cloudy");
		Wea_bmp = cloudy;
	}
	//thunder //  
	else if(
		(my_strcmp(value_string, "������") == 0)	|| 	
		(my_strcmp(value_string, "�ױ�") == 0)		|| 	
		(my_strcmp(value_string, "�׵�") == 0) 	 		
	){			
		printf("thunder\n");
		my_strcpy((char *)Wea_Condition, (char *)"thunder");
		Wea_bmp = thunder;
	}
	//snows          
	else if(
		(my_strcmp(value_string, "��ѩ") == 0) 		|| 	
		(my_strcmp(value_string, "Сѩ") == 0) 		|| 	
		(my_strcmp(value_string, "ѩ") == 0) 		|| 	
		(my_strcmp(value_string, "��ѩ") == 0) 		|| 	
		(my_strcmp(value_string, "С����ѩ") == 0) 	|| 	
		(my_strcmp(value_string, "С��ѩ") == 0) 	|| 	
		(my_strcmp(value_string, "��ѩ") == 0) 		|| 		
		(my_strcmp(value_string, "���ѩ") == 0) 	|| 	
		(my_strcmp(value_string, "��ѩ") == 0) 			
	){			
		printf("snows\n");
		my_strcpy((char *)Wea_Condition, (char *)"snows");
		Wea_bmp = snows;
	}
	//�������е����������ⶼ���������죬sunny
	else{		
		printf("sunny\n");
		my_strcpy((char *)Wea_Condition, (char *)"sunny");
		Wea_bmp = sunny;
	}
}


