#ifndef __LED_H
#define	__LED_H

#include "stm32f0xx.h"

#define u8 unsigned char
#define u32 unsigned int
#define LED_GPIO_CLKA   RCC_AHBPeriph_GPIOA 
#define LED_GPIO_CLKB   RCC_AHBPeriph_GPIOB
#define LED_PORT   	   GPIOA
#define LED_PIN        GPIO_Pin_15
#define Max_Column	128
#define Max_Row		64
#define SIZE 16
void LED_Init(void);
//void LED_ON(void);
//void LED_OFF(void);
void LED_GPIO_Config(void);
void LEDXToggle(uint8_t ledx);



#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)



#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void RGB_Driver(void);
//OLED控制用函数

void OLED_WR_Byte(unsigned dat,unsigned cmd);      
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
//void IIC_Start();
//void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);


void LED_Init(void);
void LED_Toggle(void);
void LED_ON(void);
void LED_OFF(void);
void LED_Flash(void* a);
#endif /* __LED_H */

