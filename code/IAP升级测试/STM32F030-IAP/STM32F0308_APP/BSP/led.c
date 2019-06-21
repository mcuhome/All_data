#include "led.h"
#include "common.h"

#include "delay.h"	
#include "oledfont.h"  
#include "main.h"

#define A_CYCLE 200
#define PWM_TAB_MAX 80
#define PWM_TAB_MIN 1
u8 const HALF_LIGHT_TAB[PWM_TAB_MAX]=//根据三角函数COS()得出 公式=40-40*cos(a)  a=0-90°
{
	0,0,0,0,0,0,0,0,1,1,
	1,1,1,2,2,2,2,2,3,3,3,
	4,4,4,5,5,5,6,6,7,7,8,
	8,9,9,10,10,11,11,12,12,
	13,13,14,15,15,16,16,17,
	18,18,19,20,20,21,22,23,
	23,24,25,25,26,27,28,28,
	29,30,31,31,32,33,34,35,
	35,36,37,38,38,39,40
};
u8 const ALL_LIGHT_TAB[PWM_TAB_MAX]=//根据三角函数COS()得出 公式=200-200*cos(a)  a=0-90°
{
	0,0,0,1,1,1,2,2,3,4,5,6,
	6,8,9,10,11,12,14,15,17,18,
	20,22,24,26,27,29,32,34,36,38,
	41,43,45,48,50,53,56,59,61,64,
	67,70,73,76,79,82,86,89,92,96,
	99,102,106,109,113,116,120,123,
	127,131,134,138,142,146,150,153,
	157,161,165,169,173,176,180,184,
	188,192,196,200
};
//void LED1234(void)//扫描周期50us，查表的好处，可自定义LED的亮度范围
//{   
//    static u8 PwmCnt=0;        //PWM计数器
//    static u8 PwmDirection=1; //亮灭方向标志位
//    static u8 PwmDuty=0;       //占空比初始化为0,占空比比较器，计数器与之相比，多则输出，少则不输出
//    static u8  Lookup_Tab_Pointer=0;     //Lookup table pointer

//    if(++PwmCnt>=A_CYCLE)            //1个周期（200*50us=10ms），调整一次占空比
//    {
//         PwmCnt=0;
//         if(PwmDirection==0)       //从全亮到灭
//         {
//			  if(--Lookup_Tab_Pointer<=PWM_TAB_MIN)
//			  { PwmDirection=1;Lookup_Tab_Pointer= PWM_TAB_MIN;}
//              if((TempLevelNumber==1)||(TempLevelNumber==3)||(TempLevelNumber==5)||(TempLevelNumber==7))
//              {   
//                  PwmDuty=HALF_LIGHT_TAB[Lookup_Tab_Pointer];//半亮控制
//              }
//              else //全亮控制
//              {
//                  PwmDuty=ALL_LIGHT_TAB[Lookup_Tab_Pointer];
//              }
//         }
//         else  //从灭到全亮
//         {
//			  if(++Lookup_Tab_Pointer>=PWM_TAB_MAX)
//			  {PwmDirection=0;Lookup_Tab_Pointer=PWM_TAB_MAX-1;}
//              if((TempLevelNumber==1)||(TempLevelNumber==3)||(TempLevelNumber==5)||(TempLevelNumber==7))
//			  {
//				  PwmDuty=HALF_LIGHT_TAB[Lookup_Tab_Pointer];
//			  }
//              else //全亮控制
//			  {
//                  PwmDuty=ALL_LIGHT_TAB[Lookup_Tab_Pointer];
//			  }
//         }  
//     }
//     if(PwmCnt<=PwmDuty)           //改变输出状态
//     {  
//         switch(TempLevelNumber)
//         {
//            case 1:LED_BLUE= ON;  break;
//            case 2:LED_BLUE= ON;  break;
//            case 3:LED_YELLOW= ON;break;
//            case 4:LED_YELLOW= ON;break;
//            case 5:LED_ORANGE= ON;break;
//            case 6:LED_ORANGE= ON;break;
//            case 7:LED_RED= ON;   break;
//            case 8:LED_RED= ON;   break;      
//         } 
//     }
//     else
//     {
//        LED1= OFF;LED2= OFF;LED3= OFF;LED4= OFF;
//     }
//}

#define A_OTHER_CYCLE 100
void RGB_Driver(void)
{   
    static u32  RGB_PwmCnt=0;       //PWM计数器
    static u8 RGB_PwmDirection=0; //亮灭方向标志位
    static u32  RGB_PwmDuty=A_OTHER_CYCLE;     //占空比初始化为200

        if(++RGB_PwmCnt>=A_OTHER_CYCLE)            //1个周期到，调整一次占空比
        {
             RGB_PwmCnt=0;
             if(RGB_PwmDirection==0)       //从全亮到灭
             {
                  if(--RGB_PwmDuty<=0)     //占空比减1，为0则改变方向标志位
                      RGB_PwmDirection=1;                
             }
             else                      //从灭到全亮
             {
                  if(++RGB_PwmDuty>=A_OTHER_CYCLE)   //占空比加1，到100则该变方向标志位
                      RGB_PwmDirection=0;
             }  
         }
         if(RGB_PwmCnt<=RGB_PwmDuty)           //改变输出状态
         {
//			 u8 f_bat_volt=1;
			LED_ON;
           // switch (f_bat_volt) 
            {
              // case BAT_GREEN:          {IOCTRL(LEDR,OFF);IOCTRL(LEDB,OFF);  IOCTRL(LEDG,ON);} break;
              // case BAT_YELLOW:         {IOCTRL(LEDR,ON); IOCTRL(LEDB,OFF);  IOCTRL(LEDG,ON);} break;
              // default:                 {IOCTRL(LEDR,ON); IOCTRL(LEDB,OFF);  IOCTRL(LEDG,OFF);} break;
            } 
         }
             
         else
		 {
			LED_ALL_OFF();
			 LED_OFF;
		 }
     
}


 
void LED_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_15;	 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xff,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}

//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack()
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
/*	while(1)
	{
		if(!OLED_SDA)				//判断是否接收到OLED 应答信号
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}


	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte((((x)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x)&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//void LED_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;

//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = RUN_LED_Pin;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(RUN_LED_GPIO_Port, &GPIO_InitStructure);
//	RUN_LED_GPIO_Port->BSRR = RUN_LED_Pin;
//}

void LED_Toggle(void)
{
	RUN_LED_GPIO_Port->ODR ^= RUN_LED_Pin;
}
//void LED_ON(void)
//{
//	
//	RUN_LED_GPIO_Port->BRR = RUN_LED_Pin;
//}
//void LED_OFF(void)
//{
//	RUN_LED_GPIO_Port->BSRR = RUN_LED_Pin;
//}
void LED_Flash(void *a)
{
	UNUSED(a);
	RUN_LED_GPIO_Port->ODR ^= RUN_LED_Pin;
}
