#ifndef  _assignment2_H_
#define  _assignment2_H_

#include "stm32f10x.h"

void TIM1_Init(void);
void ADC1_Init(void);

void Direction_Init(void);//方向初始化

void Direction_Dispose(void);//差速算法，pwm输出
					
#endif
