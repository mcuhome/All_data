//  GY-25
//  使用单片机：STC89C52 
//  晶振：  11.0592 M
//  液晶屏：LCD1602
//  波特率：9600
#include<REG52.H>	
#include<math.h>       //Keil library  
#include<stdio.h>      //Keil library	
#include<INTRINS.H>
#define uchar unsigned char
#define uint  unsigned int	
	
//********定义变量*****************************
uchar   BUF[10];          //数据缓存区
uchar   cnt=0;           //接收数据的累计值
uchar   sign=0;
int YPR[3];
//********定义函数*****************************
void    delay(unsigned int k);
void    SeriPushSend(uchar send_data);
void 	  display(void);
//*********************************************
//串口中断
void serial_serve(void) interrupt 4	
{

  if(RI==1){         //中断标志
     RI=0;
    BUF[cnt]=SBUF;	
	  cnt++;        //接收计数
    if(cnt==8)                //
    {    
       cnt=0;                 //
       sign=1;
    } 
  }
	
}
//*********************************************
//串口初始化
//9600 bps @ 11.059 MHz 
void init_uart()
{
	TMOD=0x21;				
	TH1=0xfd;				
	TL1=0xfd;				
	SCON=0x50;
	PS=1;      //串口中断设为高优先级别
	TR0=1;	   //启动定时器			
	TR1=1;
	ET0=1;     //打开定时器0中断			
	ES=1;	
}

//**********延时函数***************
void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
	for(j=0;j<121;j++)			
	{;}
	}						
}						            
//*********串口数据发送******************
void  SeriPushSend(uchar send_data)
{
    SBUF=send_data;  
	while(!TI);TI=0;	  
}
//*************************************
void dis_play(int num)	
{
	if(num<0)
	{
		num=-num;
		SeriPushSend('-');
	}
	else
		SeriPushSend('+');

	SeriPushSend(0x30|(num/10000));	
	SeriPushSend(0x30|(num%10000/1000));
	SeriPushSend(0x30|(num%1000/100));
	SeriPushSend(0x2e);
	SeriPushSend(0x30|(num%100/10));
	SeriPushSend(0x30|(num%10));
	SeriPushSend(',');
}




//*******************数据接收并显示**********************
void display(void)
{
		
  if(sign){ 
         sign=0;	

	     if(BUF[0]==0XAA&&BUF[7]==0X55){                         //帧头判断   
				 
          YPR[0]=(BUF[1]<<8|BUF[2]);  
          YPR[1]=(BUF[3]<<8|BUF[4]);
          YPR[2]=(BUF[5]<<8|BUF[6]);
					
		 SeriPushSend('#');
	     SeriPushSend('Y');
	     SeriPushSend('P');
	     SeriPushSend('R');
	     SeriPushSend('=');
			 
	     dis_play(YPR[0]);
		 dis_play(YPR[1]);
	     dis_play(YPR[2]);  
			 
		 SeriPushSend(0X0d);
	     SeriPushSend(0X0a);				 
        }
   }  
}
//***************************************
//主程序
void main()
{ 
   delay(2000);	                                              //延时		
   init_uart();                                               //初始化串口
   EA=1;                                                      //开总中断
   cnt=0;
   SeriPushSend(0XA5); 
   SeriPushSend(0X52);                                        //发送1帧读取命令，连续读取
   while(1){ 

   display();                                              

   }                             	                             //end while
} 

