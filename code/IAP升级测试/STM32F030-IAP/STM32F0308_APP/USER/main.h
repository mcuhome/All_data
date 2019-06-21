/**
  ******************************************************************************
  * @file    RTC/RTC_Calendar/main.h 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
//#define USE_STM320518_EVAL
///* Includes ------------------------------------------------------------------*/
//#include "stm32f0xx.h"
//#ifdef USE_STM320518_EVAL
//  #include "stm320518_eval.h"
//#else 
//  #include "stm32072b_eval.h"
//#endif /* USE_STM320518_EVAL */
#include <stdio.h>
#include "stm32f0xx.h"
#include "delay.h"
#include "led.h"
#include "bmp.h"
#include "rtc.h"
#include "iwdg.h"

#define u8 unsigned char
#define u32 unsigned int
//引脚定义，所有的引脚均在此定义

#define LED_GPIO_CLKA   RCC_AHBPeriph_GPIOA 
#define LED_GPIO_CLKB   RCC_AHBPeriph_GPIOB
#define LED_PORT   	   		GPIOA
#define LED_PIN_2        	GPIO_Pin_15
#define GPIO_LED_R        	GPIO_Pin_8//GPIOB_PIN_8
#define GPIO_LED_G        	GPIO_Pin_0//GPIOF_PIN_0
#define GPIO_LED_B        	GPIO_Pin_1//GPIOF_PIN_1

#define LED1_GPIO_RCC           RCC_AHBPeriph_GPIOA
//#define LED1_GPIO_PORT          GPIOA
#define LED1_GPIO_PIN      			GPIO_Pin_15
#define LED1_ONOFF(x)     			GPIO_WriteBit(GPIOA,GPIO_Pin_15,x);

#define LED_ON 		GPIO_ResetBits(LED_PORT, LED_PIN)
#define LED_OFF 	GPIO_SetBits(LED_PORT, LED_PIN)
//IIC 
#define GPIO_IIC_SCL        	GPIO_Pin_0//GPIOA_PIN_0
#define GPIO_IIC_SDA        	GPIO_Pin_1//GPIOA_PIN_1
#define GPIO_OLED_RES        	GPIO_Pin_2//GPIOA_PIN_2
#define GPIO_TEMPERATURE      	GPIO_Pin_3//GPIOA_PIN_3
#define GPIO_TEST_MODE        	GPIO_Pin_4//GPIOA_PIN_4
#define GPIO_T_CTRL_1        	GPIO_Pin_5//GPIOA_PIN_5
#define GPIO_T_CTRL_2        	GPIO_Pin_6//GPIOA_PIN_6
#define GPIO_T_CTRL_3        	GPIO_Pin_8//GPIOA_PIN_8
#define GPIO_BAT_CHECK        	GPIO_Pin_7//GPIOA_PIN_7
#define GPIO_WAX_CHECK        	GPIO_Pin_0//GPIOB_PIN_0
#define GPIO_OD        			GPIO_Pin_1//GPIOB_PIN_1
//#define GPIO_POWER_EN        GPIO_Pin_2//GPIOB_PIN_0
#define GPIO_KEY_MODE        	GPIO_Pin_7//GPIOB_PIN_7
#define GPIO_KEY_POWER        	GPIO_Pin_6//GPIOB_PIN_6
#define GPIO_KEY_TEM_UP        	GPIO_Pin_5//GPIOB_PIN_5
#define GPIO_KEY_TEM_DOWN       GPIO_Pin_4//GPIOB_PIN_4
#define GPIO_CHARGED        	GPIO_Pin_3//GPIOB_PIN_3
#define GPIO_CHARGING        	GPIO_Pin_15//GPIOA_PIN_15
#define GPIO_POWER_EN        	GPIO_Pin_12//GPIOA_PIN_12
#define GPIO_MOTOR        		GPIO_Pin_11//GPIOA_PIN_12

#define LED_R_PORT        	GPIOB
#define LED_G_PORT        	GPIOF
#define LED_B_PORT        	GPIOF
#define MOTOR_PORT        	GPIOF
#define KEY_PORT        	GPIOB

//led 按键控制
#define LED_R_ON()						GPIO_ResetBits(LED_R_PORT, GPIO_LED_R)
#define LED_G_ON()						GPIO_ResetBits(LED_G_PORT, GPIO_LED_G)
#define LED_B_ON()						GPIO_ResetBits(LED_B_PORT, GPIO_LED_B)
#define LED_R_OFF()						GPIO_SetBits(LED_R_PORT, GPIO_LED_R)
#define LED_G_OFF()						GPIO_SetBits(LED_G_PORT, GPIO_LED_G)
#define LED_B_OFF()						GPIO_SetBits(LED_B_PORT, GPIO_LED_B)
#define MOTOR_ENABLE()					GPIO_ResetBits(MOTOR_PORT, GPIO_MOTOR)
#define MOTOR_DISABLE()					GPIO_SetBits(MOTOR_PORT, GPIO_MOTOR)
#define GET_KEY_MODE()					GPIO_ReadInputDataBit(KEY_PORT,GPIO_KEY_MODE)
#define GET_KEY_POWER()					GPIO_ReadInputDataBit(KEY_PORT,GPIO_KEY_POWER)
#define GET_KEY_TEM_UP()				GPIO_ReadInputDataBit(KEY_PORT,GPIO_KEY_TEM_UP)
#define GET_KEY_TEM_DOWN()				GPIO_ReadInputDataBit(KEY_PORT,GPIO_KEY_TEM_DOWN)


#define KEY_BUTTON_GPIO_PORT            GPIOB
#define KEY_BUTTON_GPIO_RCC             RCC_AHBPeriph_GPIOB
#define KEY_Status_Read()				GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)


#define LED_ALL_ON()				 do { \
											GPIO_ResetBits(LED_R_PORT, GPIO_LED_R);\
											GPIO_ResetBits(LED_G_PORT, GPIO_LED_G);\
											GPIO_ResetBits(LED_B_PORT, GPIO_LED_B);\
										} while(0U)
#define LED_ALL_OFF()				 do { \
											GPIO_SetBits(LED_R_PORT, GPIO_LED_R);\
											GPIO_SetBits(LED_G_PORT, GPIO_LED_G);\
											GPIO_SetBits(LED_B_PORT, GPIO_LED_B);\
										} while(0U)

#define ON    0
#define OFF   !ON  
#define TRUE  1
#define FALSE !TRUE

#define MOTOR_ON 	1
#define MOTOR_OFF 	!MOTOR_ON

#define DELAY_USE_OVER_TIME		0

#define ADC_BAT_CHANNEL  	4		//电池电量采集口
#define ADC_TEMP_CHANNEL1 	2    //温度采集端口
#define ADC_WAX_CHANNAL 	5    //故障检测口 

#define HOT_MIN_TH			45//50
#define HOT_MAX_TH			250

//Temp 1: 342F/172C (half blue) 
//Temp 2: 356F/180C (blue) 
//Temp 3: 370F/188C (half yellow) 
//Temp 4: 385F/196C (yellow) 
//Temp 5: 392F/204C (half orange) 
//Temp 6: 414F/212C (orange)
//Temp 7: 428F/220C (half red)
//Temp 8: 442F/228C (red)

//#define TEMP_LEVEL_ONE 		168//172C(half blue) 
//#define TEMP_LEVEL_TWO 		174//180C(blue) 
//#define TEMP_LEVEL_THREE 	182//188C(half yellow) 
//#define TEMP_LEVEL_FOUR 	190//196C(yellow) 
//#define TEMP_LEVEL_FIVE 	198//204C(half orange) 
//#define TEMP_LEVEL_SIX 		205//212C(orange)
//#define TEMP_LEVEL_SEVEN 	210//220C(half red)
//#define TEMP_LEVEL_EIGHT 	220//228C(red)

//测试
#define TEMP_LEVEL_ONE 		154//172C(half blue) 
#define TEMP_LEVEL_TWO 		162//180C(blue) 
#define TEMP_LEVEL_THREE 	170//188C(half yellow) 
#define TEMP_LEVEL_FOUR 	178//196C(yellow) 
#define TEMP_LEVEL_FIVE 	183//204C(half orange) 
#define TEMP_LEVEL_SIX 		190//212C(orange)
#define TEMP_LEVEL_SEVEN 	200//220C(half red)
#define TEMP_LEVEL_EIGHT 	208//228C(red)

//For the power extract mode:
//For the power settings in extract mode:
//Temp 1: 55 (half blue) 
//Temp 2: 60 (blue) 
//Temp 3: 65 (half yellow) 
//Temp 4: 70 (yellow) 
//Temp 5: 75 (half orange) 
//Temp 6: 80 (orange)
//Temp 7: 95 (half red)
//Temp 8: 100 (red)
#define wax_tick  10
#define wax1  55*wax_tick
#define wax2  60*wax_tick
#define wax3  65*wax_tick
#define wax4  70*wax_tick
#define wax5  75*wax_tick
#define wax6  80*wax_tick
#define wax7  95*wax_tick
#define wax8  99*wax_tick




#define Kp	50//50
#define Kd	30


 

#define TEMP_TH		5//5//8//3//10
#define TEMP_TL		5//5//8//3//10


#define	MINPWM	0		
#define	MAXPWM	999//80%////0X332//0x3fe////0X3cb//0X280//0X280
#define	ALL_SPEED_PWM	999//80%

#define	LEDON	5//2//5
#define	LEDALL	40
#define	F_HALF_LIGHT	    1  //半亮显示标志
#define	F_BREATHE_LIGHT	    2  //呼吸显示标志
#define	F_ALL_NO_LIGHT	    0  //全部不显示标志

#define	LEDCLOSE 	0	
#define	LEDOPEN 	1	
#define	LEDHALF 	2 
#define	LEDFALSH 	3

#define MODE_SHUTDOWN           0
#define MODE_POWER              0x01
#define MODE_WAX                0x02
#define MODE_WAX_PREPARE        0x03
#define MODE_POWER_BEFORE_WAX   0x04
#define MODE_Heating            0x05
#define MODE_Preheating         0x06
#define MODE_BOOST              0x07
#define MODE_SENSOR1_ERR        0x08
#define MODE_WAX_ERR            0x09
#define MODE_CHARGING           10
#define MODE_Standby            11
#define MODE_PreShutdown        12 //关机前准备，先震动一下，然后关机
#define MODE_BAT_LOW            13 
#define MODE_WAIT               14 
//#define MODE_AFTER_CHANGE       15 //一般充电时，显示蓝灯5秒后关机

#define F_MODE_ON     1
#define F_MODE_OFF    0

#define OPEN     1
#define SHUTDOWN    0


//测试模式
//#define SECOND_TICK                         2           //每秒的心跳数
//#define TASKTICK_POWER                      1*SECOND_TICK    //电量显示时间 1s
//#define TASKTICK_HOT                        6*SECOND_TICK    //加热时间
//#define TASKTICK_SPARETIME                  6*SECOND_TICK    //空闲运行时间
//#define TASKTICK_LOWER                      1*SECOND_TICK    //低电运行时间
//#define TASKTICK_BOOST                      8*SECOND_TICK    //BOOST运行时间
//#define TASKTICK_WAX_STOP                   10*SECOND_TICK    //wax停止时间
//#define TASKTICK_WAX_ALL_SPEED              1*SECOND_TICK    //wax全速执行时间
//#define TASKTICK_WAX_TARGET_SPEED           10*SECOND_TICK    //wax到达目的温度后执行时间
//#define TASKTICK_SENSOR_ERR                 1*SECOND_TICK    //传感器故障后执行时间
//#define TASKTICK_SHUTDOWN_PREPARE_TIME      3 //1*SECOND_TICK
//#define TASKTICK_SENSOR_CHECK_TIME          400*SECOND_TICK    //传感器检测时间
//#define TASKTICK_WAITTIME                  1*SECOND_TICK    //按键设置温度显示时间
//#define TASKTICK_HOT_PREPARE_TIME          1      //加热前led显示时间

//#define TASKTICK_HOT_COMPLETE               250*SECOND_TICK    //低电压时加热时间时间
//#define TASKTICK_BOOST_COMPLETE             250*SECOND_TICK    //低电压时加热时间时间

#define SECOND_TICK                         4                //每秒的心跳数
#define TASKTICK_POWER                      3    //电量显示时间 1.5s
#define TASKTICK_HOT                        22*SECOND_TICK   //加热持续时间 60s 
#define TASKTICK_SPARETIME                  62*SECOND_TICK   //空闲运行时间 180s
#define TASKTICK_LOWER                      1*SECOND_TICK    //低电压运行时间
#define TASKTICK_BOOST                      8*SECOND_TICK    //BOOST运行时间
#define TASKTICK_WAX_STOP                   10*SECOND_TICK   //wax停止时间
#define TASKTICK_WAX_ALL_SPEED              2*SECOND_TICK    //wax全速执行时间
#define TASKTICK_WAX_TARGET_SPEED           10*SECOND_TICK    //wax到达目的温度后执行时间
#define TASKTICK_SENSOR_ERR                 3//1*SECOND_TICK    //传感器故障后执行时间
#define TASKTICK_SHUTDOWN_PREPARE_TIME      2                //关机前准备时间
#define TASKTICK_SENSOR_CHECK_TIME          10*SECOND_TICK    //传感器检测时间
#define TASKTICK_WAITTIME                   1*SECOND_TICK    //按键设置温度显示时间（用于温度设置，但不加热时LED显示）
#define TASKTICK_HOT_PREPARE_TIME           2      //加热前led显示时间

#define TASKTICK_HOT_COMPLETE               25*SECOND_TICK    //低电压时加热时间时间
#define TASKTICK_BOOST_COMPLETE             10*SECOND_TICK    //低电压时加热时间时间



//延时标志位
#define F_SHUTDOWN_TIME             0
#define F_POWER_TIME                11
#define F_HOT_TIME                  12
#define F_HOT_PREPARE_TIME          13
#define F_WAX_STOP_TIME             14
#define F_WAX_ALL_SPEED_TIME        15
#define F_WAX_TARGET_SPEED_TIME     16
#define F_SPARETIME_TIME            17
#define F_BOOST_TIME                18
#define F_SENSOR_ERR_TIME           19
#define F_SHUTDOWN_MOVE             20
#define F_SHUTDOWN_PREPARE_TIME     21
#define F_LOWER_TIME                22
#define F_WAIT_TIME                 23


#define WAX_STOP                0
#define WAX_ALL_SPEED           1
#define WAX_TARGET_SPEED        2
										
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_TimeRegulate(void);
void RTC_TimeShow(void);
void RTC_AlarmShow(void);
uint8_t USART_Scanf(uint32_t value);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
