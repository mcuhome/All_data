/*文件名称：任务2
1：配置采集方向信号——操作硬件ADC
2：配置定时器配置——tim1，tim8
3：差速算法，速度合成
4：PWM输出

单片机型号：STM32F103RCT6 32KB 256KB

需要用到的资源
PB13 34 TIM1_CH1N	   PB14 35 TIM1_CH2N
PA8  41 TIM1_CH1	   PA9  42 TIM1_CH2

PA7 23 TIM8_CH1N	   PB0 26 TIM8_CH1N
PC6 37 TIM8_CH1		   PC7 38 TIM8_CH2

库版本  ：ST3.5.0
*/

#include "assignment2.h"

#include "stm32f10x.h"
/**************************************************第1步：方向信号采集，配置所用的ADC***************************************/

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_ConvertedValue[4];//ad值暂存数组
int ADC_ConvertedValueLocal; //电池电压值
int ADC_Direction;//方向ad值原始值，上电时储存
int Direction_Difference;//转向差

extern float PWM;//从前面文件提取全局变量
extern int int_PWM;//整形PWM值

int PWM_R;         //右轮PWM值计算
int PWM_L;         //左轮PWM值计算
extern float Angle;//角度

/*
 * 函数名：ADC1_GPIO_Config */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PC1,输入时不用设置速率
}


/* 函数名：ADC1_Mode_Config*/
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 4;//开辟4个储存空间
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增使能
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字 16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 4;	 	//要转换的通道数目1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*ADCx,通道编号，扫描顺序，采样周期 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);//电池电压
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_1Cycles5);//转把
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_1Cycles5);//左电机电流
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_1Cycles5);//右电机电流
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*函数名：ADC1_Init */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


/****************************************************第2步：配置定时器***************************************/
uint16_t Channel1Pulse = 0, Channel2Pulse = 0;

void TIM1_Mode_Init(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

/*定时器初始化*/
	Channel1Pulse = 0;//初始化，没有任何pwm输出
	Channel2Pulse = 0;

  TIM_TimeBaseStructure.TIM_Prescaler = 12;         //设置用来作为TIM 时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式  /* 向上计数模式 */
  TIM_TimeBaseStructure.TIM_Period = 999;        //自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //时钟分割 不分频
//  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;        //设置 周期 计数值
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

/*TIM1 配置*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;           //脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         //使能输出比较状态
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //使能  互补 输出状态

  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;                //给1通道填入初始值
  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; //给2通道填入初始值

  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //输出比较极性低
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//互补 输出极性低
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;   //MOE=0 设置 TIM1输出比较空闲状态
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;//MOE=0 重置 TIM1输出比较空闲状态

//  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; //给2通道填入初始值

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);//设定1通道好的参数 初始化TIM
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);//设定1通道好的参数 初始化TIM
  
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);//初始化
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);//初始化
		   
/*设置刹车特性  死区时间  锁电平 OSSI OSSR 状态 AOE(自动输出使能)*/
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;  //设置在运行模式下 非工作状态选项 OSSR
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;  //设置在运行模式下 非工作状态选项 OSSI
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;  //使用锁电平1
  TIM_BDTRInitStructure.TIM_DeadTime = 80;		//指定了输出和打开 关闭状态之间的延时 约 400ns  
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出功能使能

  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);	//初始化设定好的参数
  TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);	//初始化设定好的参数

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
  TIM_Cmd(TIM8, ENABLE);

  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM8, ENABLE);

}

/*初始化时钟*/
void RCC_Configuration(void)
{
  /* TIM1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8|RCC_APB2Periph_TIM1|
  						 RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|
						 RCC_APB2Periph_AFIO, ENABLE);
}
/*配置所用到的口线*/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOB Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_13 | GPIO_Pin_14 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*初始化定时器1所需要的资源*/
void TIM1_Init(void)
{
	RCC_Configuration();//配置时钟
	
	GPIO_Configuration();//配置口线
	
	TIM1_Mode_Init();//配置定时器

}
/******************************************************第3部：差速算法，速度合成*********************************/
/*方向初始化函数*/
void Direction_Init(void)//Direction：方向
{
	ADC_Direction=ADC_ConvertedValue[1];//传送方向ad值	
}

/*差速算法，速度合成*/
void Direction_Dispose(void)//Dispose:处理
{
	int Direction_Read;

  Direction_Read=ADC_ConvertedValue[1];//读取数据
	Direction_Difference=(ADC_Direction-Direction_Read)*0.1;//差=原始数据-现在数据


/*pwm电压调节器，电池电压变化pwm不变*/

// （读出数据/4096）*3.3*21（倍）=实际电压 原始pwm*（32/测量电压）=输出pwm
//	ADC_ConvertedValueLocal =(float) ADC_ConvertedValue[2]/4096*3.28*21;

//	P=32/ADC_ConvertedValueLocal;

	if(int_PWM>0)//判断向前还是向后，如果向前
	{
		if(Direction_Difference>0)//判断转向数据正数还是负数，如果证书
			{
			PWM_R=int_PWM-(int_PWM*Direction_Difference/100);//向前的PWM数据-转向值=右//读取PWM信号*（读取的转向/100），意思就是差速信号是转向是PWM的百分比
			PWM_L=int_PWM+(int_PWM*Direction_Difference/100);//左边转向值
			}																	 //机器向右转
		else
			{
			PWM_R=int_PWM-(int_PWM*Direction_Difference/100);//右边转向值
			PWM_L=int_PWM+(int_PWM*Direction_Difference/100);//左边转向值
			}																	//机器向左转
	TIM_SetCompare1(TIM1,PWM_L);//送入左电机，第一通道，设定向前运动
	TIM_SetCompare1(TIM8,PWM_R);//送入右电机，第一通道，设定向前运动
	}
	
	else
	{	
		if(Direction_Difference>0)
			{
			PWM_R=(-int_PWM)-(int_PWM*Direction_Difference/100);//向前的PWM数据-转向值=右边转向值
			PWM_L=(-int_PWM)+(int_PWM*Direction_Difference/100);//左边转向值		
			}																	 //机器向右转
		else
			{
			PWM_R=(-int_PWM)-(int_PWM*Direction_Difference/100);//右边转向值
			PWM_L=(-int_PWM)+(int_PWM*Direction_Difference/100);//左边转向值
			} 																 //机器向左转		
	TIM_SetCompare2(TIM1,PWM_L);//送入左电机，第二通道，设定电机向后运动
	TIM_SetCompare2(TIM8,PWM_R);//送入左电机，第二通道，设定电机向后运动
	}

}	
	
	
	
	
	
	