/*
 * TCS3200模块
 * 
 * 用途：TCS3200颜色测试,读取RGB值,LCD1602显示R,G,B值
 * 
 * 作者					日期				备注
 * Huafeng Lin			2010/12/10			新增
 * Huafeng Lin			2010/12/11			修改
 * 
 */

//接线说明：
//模块S2-----单片机P1.1
//模块S3-----单片机P1.0
//模块OUT----单片机P3.5(计数器1输入)
//模块VCC----单片机VCC
//模块GND----单片机GND

#include<REG52.H>	
#include<math.h>       //Keil library  
#include<stdio.h>      //Keil library	
#include<INTRINS.H>

#define uchar unsigned char
#define uint  unsigned int	
#define DataPort P2	   //LCD1602 数据端口
	
sbit    LCM_RS=P0^2;   //LCD1602 控制端口		
sbit    LCM_RW=P0^1;   //LCD1602 控制端口	
sbit    LCM_EN=P0^0;   //LCD1602 控制端口

/**引脚定义**/  
sbit s2=P1^1;        //TCS3200 S2 
sbit s3=P1^0;        //TCS3200 S3
                     //TCS3200 S0 模块内部默认上拉
                     //TCS3200 S1 模块内部默认上拉
                     //TCS3200 OE 模块内部接地
sbit test_pin=P1^2;  //用示波器看这个引脚，可知道定时器中断频率
//变量、常量定义
uchar ge,shi,bai ;
uchar rp=3,gp=3,bp=6; //定义比例因子，具体环境可以修改
uchar count;          //颜色标志位（0:红 1:绿 2:蓝）

//显示数组
uchar disp_R[3];  //红
uchar disp_G[3];  //绿
uchar disp_B[3];  //蓝

//********定义函数*****************************
void    delay(unsigned int k);
void    InitLcd();
void    WriteDataLCM(uchar dataW);
void    WriteCommandLCM(uchar CMD,uchar Attribc);
void    DisplayOneChar(uchar X,uchar Y,uchar DData);

//*********LCD1602初始化**********************
void InitLcd()				
{			
	WriteCommandLCM(0x38,1);	
	WriteCommandLCM(0x08,1);	
	WriteCommandLCM(0x01,1);
	WriteCommandLCM(0x06,1);	
	WriteCommandLCM(0x0c,1);
}

//**********检测忙信号************************
void WaitForEnable(void)	
{					
	DataPort=0xff;		
	LCM_RS=0;LCM_RW=1;_nop_();
	LCM_EN=1;_nop_();_nop_();
	while(DataPort&0x80);	
	LCM_EN=0;				
}
					
//**********写命令至LCD***********************
void WriteCommandLCM(uchar CMD,uchar Attribc)
{					
	if(Attribc)WaitForEnable();	
	LCM_RS=0;LCM_RW=0;_nop_();
	DataPort=CMD;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}	
				
//**********写数据至LCD************************
void WriteDataLCM(uchar dataW)
{					
	WaitForEnable();		
	LCM_RS=1;LCM_RW=0;_nop_();
	DataPort=dataW;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}
					
//*********写一个字符数据到指定的目标***********
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X,0);		
	WriteDataLCM(DData);		
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

/*******************************************
* 函数名称: t0_init()
* 函数功能: 定时器0初始化
* 入口参数: 无
* 出口参数: 无
/********************************************/
void t0_init()
{
	TMOD=0x51;        //T1计数 T0定时 工作方式1
	
	TH1=0x00;        //计数初值
	TL1=0x00;
	
	TH0=0xE0;
	TL0=0x00;        //11。0592M 晶振10ms
	EA=1;            //开中断
	
	ET0=1;        
	TR0=1;           //启动
	TR1=1;
}

//*********************************************
//数值转换出个十百千的ASCII码
//*********************************************
void conversion(uint temp_data)  
{  
    bai=temp_data/100+0x30 ;
    temp_data=temp_data%100;   //取余运算
    shi=temp_data/10+0x30 ;
    ge=temp_data%10+0x30;      //取余运算
}

/*******************************************
* 函数名称: main()
/********************************************/
void main()
{
	delay(10); 
	InitLcd();      //lcd初始化
	s2=0;           //初始设定S2引脚
	s3=0;           //初始设定S3引脚
	t0_init();      //定时计数初使�

	while(1)
	{
		DisplayOneChar(0, 0, 'T');
		DisplayOneChar(1, 0, 'C');
		DisplayOneChar(2, 0, 'S');
		DisplayOneChar(3, 0, '2');
		DisplayOneChar(4, 0, '3');
		DisplayOneChar(5, 0, '0');

		DisplayOneChar(10, 0, 'R'); 
		DisplayOneChar(11, 0, '['); 
		DisplayOneChar(12, 0, disp_R[0]); 
		DisplayOneChar(13, 0, disp_R[1]); 
		DisplayOneChar(14, 0, disp_R[2]);
		DisplayOneChar(15, 0, ']');	
	
		DisplayOneChar(0, 1, 'G'); 
		DisplayOneChar(1, 1, '['); 
		DisplayOneChar(2, 1, disp_G[0]); 
		DisplayOneChar(3, 1, disp_G[1]); 
		DisplayOneChar(4, 1, disp_G[2]);
		DisplayOneChar(5, 1, ']');
		
		DisplayOneChar(10, 1, 'B'); 
		DisplayOneChar(11, 1, '['); 
		DisplayOneChar(12, 1, disp_B[0]); 
		DisplayOneChar(13, 1, disp_B[1]); 
		DisplayOneChar(14, 1, disp_B[2]);
		DisplayOneChar(15, 1, ']');				
		
		delay(100) ;	
	}
}

/*******************************************
* 函数名称: c10ms_out() 
* 函数功能: 定时中断0服务程序
            修改颜色标志disp_tc（0:红 1:绿 2:蓝）
            设置S0 S1 S2 选择滤波器
            计算脉冲，读取色值
* 入口参数: 无
* 出口参数: 无
/********************************************/
void c10ms_out() interrupt 1
{
	uint temp;
	test_pin=!test_pin; //测试定时器中断频率引脚，可以用示波器观察
	TR0=0;              //关闭定时
	TR1=0;              //关闭计数
	//   count+1实现先检测绿色,再检测蓝色,然后检测红色,循环检测       
	if(count==0)
	{
		count++;    
		s2=1;s3=1;             //选择滤波器为绿色     
		
		temp=(8<<TH1)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		temp/=rp;			
		conversion(temp);
		disp_R[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
		disp_R[1]=shi;
		disp_R[0]=bai;
	}	
	else if(count==1)
	{            
		count++;
		s2=1;s3=0;            //选择滤波器为蓝色
		temp=(8<<TH1)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		temp/=gp;			
		conversion(temp);
		disp_G[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
		disp_G[1]=shi;
		disp_G[0]=bai;
	}	
	else if(count==2)
	{            
		count=0;
		s2=0;s3=0;            //选择滤波器为红色
		
		temp=(8<<TH1)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		temp/=bp;		
		conversion(temp);
		disp_B[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
		disp_B[1]=shi;
		disp_B[0]=bai;	
	}
	
	//定时器计数器重赋初值
	TH0=0xE0;
	TL0=0x00; //11。0592M 晶振，为10ms
	TL1=0x00;//计数器清零
	TH1=0x00;//计数器清零
	TR0=1;   //打开定时器
	TR1=1;   //打开计数器
}
