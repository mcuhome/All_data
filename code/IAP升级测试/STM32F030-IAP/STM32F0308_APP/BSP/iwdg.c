

#include "iwdg.h"
#include "stm32f0xx_iwdg.h"
 /**
  * @file   IWDG_Config
  * @brief  初始化独立看门狗
  * @param  无
  * @retval 无
  */
void IWDG_Config(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对寄存器IWDG_PR、IWDG_RLR的写操作
	IWDG_SetPrescaler(IWDG_Prescaler_32);		  //设置IWDG预分频值：256分频最大
	IWDG_SetReload(40000/64);	   //设置IWDG的重装载值	:范围0~0x0FFF
	IWDG_ReloadCounter();	   //喂狗：重新装载计数器
	IWDG_Enable();			   //使能IWDG定时器
}

  /* 
设定IWDG重载溢出时间 500ms
     Counter Reload Value = 500ms/IWDG counter clock period
                          = 500ms / (LSI/32)
                          = 0.5s / (LsiFreq/32)
                          = LsiFreq/(32 * 2)
                          = LsiFreq/64
  */
