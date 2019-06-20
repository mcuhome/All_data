
#include "stm32f10x.h"
#include "SysTick.h"
#include "assignment1.h"
#include "assignment2.h"
#include "assignment3.h"

extern float Angle;//角度

int main(void)
{	

	SysTick_Init();//①系统滴答时钟初始化	
		 
	I2C_MPU6050_Init();//②重力传感器接口初始化
	
	InitMPU6050();//③初始化MPU6050

//	LED_Config();//④显示LED初始化

  TIM1_Init();//⑤定时器tim1，tim8初始化

	ADC1_Init();//⑥ADC初始化

	USART2_Config();//⑦初始化串口2

	Direction_Init();//⑧初始化转向

	while(1)
	{
	}
}



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
