#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define PWM1   TIM3->CCR3
#define PWM2   TIM3->CCR4

void PWM_Init(u16 arr,u16 psc);
void steering_control(void);

#endif

