/*文件名称：任务1
1：采集mpu6050数据——操作硬件I2C1
2：计算角度，角速度——软件完成单位换算
3：数据融合——软件卡尔曼滤波算法
4：计算速度与位移——用上次的PWM数据
5：PID控制算法
6：得到需要的PWMD数值

单片机型号：STM32F103RCT6 32KB 256KB

需要用到的外部资源

I2C1，SCL-PB6;SDA-PB7

库版本  ：ST3.5.0

*/


#include "assignment1.h"
#include "assignment2.h"
#include "assignment3.h"
#include "SysTick.h"
#include "math.h"//数学库

uint32_t I2C_MPU_ACCE_Read[2];//加速度计临时数据
uint32_t I2C_MPU_GYRO_Read[2];//陀螺仪临时数据

void Kalman_Filter(float Accel,float Gyro);

/*定义全局变量*/
float Accel_z;	     //z轴加速度值暂存
float Accel_x;	     //X轴加速度值暂存
float Accel_y;	     //y轴加速度值暂存

float Gyro_y;        //Y轴陀螺仪数据暂存

float Angle_gy;      //由角速度计算的倾斜角度
float Angle_az;      //由加速度计算的倾斜角度

float Angle;         //小车最终倾斜角度

float PWM;//最终得到的PWM驱动值

int   speed_mr;		 //右电机转速
int   speed_ml;		 //左电机转速
int int_PWM;				//整形PWM值

float PWM;           //综合PWM计算
float PWMI;			 //PWM积分值

/*电机参数*/
float speed;        //电机转速滤波
float position;	    //位移
float speed_filter; //转速滤
float integral;//速度积分百分比


float x;
float y;
float z;

/*延时函数*/

/**us延时函数**/
void Delay_us(uint32_t xus)
{
	uint8_t i;
	while(xus--)
	{
		i=7;
		while(i--);
	}
}

/**ms延时函数**/
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}



/***************************************************第1步：采集mpu6050数据******************************************************/

/*初始化I2C1对外口线*/
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd  (RCC_APB2Periph_GPIOB, ENABLE );//使能与 I2C1 有关的时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//PB6-I2C1_SCL、PB7-I2C1_SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

/*硬件配置I2C1*/
static void I2C_Mode_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure;  
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; //I2C 配置 
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	I2C_InitStructure.I2C_OwnAddress1 = MPU_ADRESS; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; 		
	I2C_Cmd(I2C1,ENABLE);//使能 I2C1 
	I2C_Init(I2C1, &I2C_InitStructure);//I2C1 初始化	   
	I2C_AcknowledgeConfig(I2C1, ENABLE);//允许应答模式  
}

/*
 * 函数名：I2C_MPU6050_ByteWrite
 * 描述  ：写一个字节到I2C MPU6050寄存器中
 * 输入  ：-pBuffer 缓冲区指针
 *         -WriteAddr 接收数据的MPU6050寄存器的地址 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：内部调用
 */	
static void I2C_MPU_ByteWrite(u8 pBuffer, u8 WriteAddr)
{
  while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));/*wait until I2C bus is not busy*///等待iic总线是否忙碌
  I2C_GenerateSTART(I2C1, ENABLE);/* Send START condition *///发送开始条件
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on EV5 and clear it *///事件 
  I2C_Send7bitAddress(I2C1, MPU_ADRESS, I2C_Direction_Transmitter);/* Send MMA address for write *///发送设备码
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));/* Test on EV6 and clear it *///事件    
  I2C_SendData(I2C1, WriteAddr);/* Send the MMA's Register address to write to *///发送设备寄存器地址 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/* Test on EV8 and clear it *///事件   
  I2C_SendData(I2C1, pBuffer);/* Send the byte to be written *///发送写入设备寄存器的数据 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/* Test on EV8 and clear it *///事件    
  I2C_GenerateSTOP(I2C1, ENABLE);/* Send STOP condition *///停止信号
}


/*
 * 函数名：I2C_MMA_ByteRead
 * 描述  ：从MMA寄存器里面读取一块数据。 
 * 输入  ： -ReadAddr 接收数据的MMA寄存器的地址。        
 * 输出  ：无
 * 返回  ：读取出的寄存器数据
 * 调用  ：内部调用
 */	

void I2C_MPU_ByteRead(uint32_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)//第一个参数是指针变量，读第一个地址后加1，
																				   //第二个参数是寄存器地址，只需要填入第一个地址即可																					//在读第二个地址，第三个参数是多少个8位，指针好累加
{   
   while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));//检测忙碌   
  I2C_GenerateSTART(I2C1, ENABLE);//开始信号  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//应答
  I2C_Send7bitAddress(I2C1, MPU_ADRESS, I2C_Direction_Transmitter);//发送设备地址
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//应答 
  I2C_Cmd(I2C1, ENABLE);//使能iic
  I2C_SendData(I2C1, ReadAddr);//发送设备寄存器地址  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//应答    
  I2C_GenerateSTART(I2C1, ENABLE);//开始信号  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//应答  
  I2C_Send7bitAddress(I2C1, MPU_ADRESS, I2C_Direction_Receiver);//发送设备地址码+1，读信号  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//应答  
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)//如果就读一个一节
    {
      I2C_AcknowledgeConfig(I2C1, DISABLE);//非应答      
      I2C_GenerateSTOP(I2C1, ENABLE);//停止信号
    }
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))//应答  
    {      
      *pBuffer = I2C_ReceiveData(I2C1);//读出数据传给指针变量
      pBuffer++;//指针变量地址+1       
      NumByteToRead--;//要读的地址里面的数据个数-1        
    }   
  }
  I2C_AcknowledgeConfig(I2C1, ENABLE);//非应答
}

/*
 * 函数名：I2C_MPU6050_Init
 * 描述  ：I2C 外设(MPU6050)初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void I2C_MPU6050_Init(void)
{	   
 	I2C_GPIO_Config();
	I2C_Mode_Config();
}

/*写入初始化MPU6050*/
void InitMPU6050()
{
	I2C_MPU_ByteWrite(0x00,PWR_MGMT_1);	//解除休眠状态
	I2C_MPU_ByteWrite(0x07,SMPLRT_DIV);//IIC写入时的地址字节数据
	I2C_MPU_ByteWrite(0x06,CONFIG);//低通滤波频率
	I2C_MPU_ByteWrite(0x18,GYRO_CONFIG);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C_MPU_ByteWrite(0x01,ACCEL_CONFIG);//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
}

/*读取数据，合成数据*/
uint32_t ACCE_Data_Z(uint8_t ZOUT_H)//z轴 加速度
{
	uint32_t ACCEL_H;
	uint8_t ACCEL_L;
	I2C_MPU_ByteRead(I2C_MPU_ACCE_Read,ZOUT_H,2);
	
	ACCEL_H=I2C_MPU_ACCE_Read[0];
	ACCEL_L=I2C_MPU_ACCE_Read[1];
	
	return (ACCEL_H<<8)+ACCEL_L; //合成数据
}

uint32_t ACCE_Data_X(uint8_t XOUT_H)//x轴 加速度
{
	uint32_t ACCEL_H;
	uint8_t ACCEL_L;
	I2C_MPU_ByteRead(I2C_MPU_ACCE_Read,XOUT_H,2);
	
	ACCEL_H=I2C_MPU_ACCE_Read[0];
	ACCEL_L=I2C_MPU_ACCE_Read[1];
	
	return (ACCEL_H<<8)+ACCEL_L; //合成数据
}

uint32_t ACCE_Data_Y(uint8_t YOUT_H)//y轴 加速度
{
	uint32_t ACCEL_H;
	uint8_t ACCEL_L;
	I2C_MPU_ByteRead(I2C_MPU_ACCE_Read,YOUT_H,2);
	
	ACCEL_H=I2C_MPU_ACCE_Read[0];
	ACCEL_L=I2C_MPU_ACCE_Read[1];
	
	return (ACCEL_H<<8)+ACCEL_L; //合成数据
}

uint32_t GYRO_Data_Y(uint8_t YOUT_H)//y轴 陀螺仪
{
	uint32_t GYRO_H;
	uint8_t GYRO_L;

	I2C_MPU_ByteRead(I2C_MPU_GYRO_Read,YOUT_H,2);

	GYRO_H=I2C_MPU_GYRO_Read[0];//Y轴陀螺仪高位
	GYRO_L=I2C_MPU_GYRO_Read[1];

	return (GYRO_H<<8)+GYRO_L;   //合成数据
}

/*********************************************************第2步：计算角度，角速度**********************************************/

/*角度计算*/
void Angle_Calcu(void)	 
{

//范围为2g时，换算关系：16384 LSB/g
//deg = rad*180/3.14

/*利用z轴上的重力加速度计算出，芯片与水平面的夹角*/
		 	
   	Accel_x=ACCE_Data_X(ACCEL_XOUT_H);//x轴
//	Accel_y=ACCE_Data_Y(ACCEL_YOUT_H);//y轴
	Accel_z=ACCE_Data_Z(ACCEL_ZOUT_H);//z轴

	Gyro_y = GYRO_Data_Y(GYRO_YOUT_H);//GetData合成的原始数据


	if(Accel_x<32764)
	{
		x=Accel_x/16384;
	}
	else
	{
		x=1-(Accel_x-49152)/16384;
	}

//	if(Accel_y<32764)
//	{
//		y=Accel_y/16384;
//	}
//	else
//	{
//		y=1-(Accel_y-49152)/16384;
//	}

	if(Accel_z<32764)
	{
		z=Accel_z/16384;
	}
	else
	{
		z=(Accel_z-49152)/16384;
	}

	Angle_az=(atan(x/z))*180/3.14;

/*角度的正负号*/											
	if(Accel_x<32764)
	{
	Angle_az=+Angle_az;
	}
	if(Accel_x>32764)
	{
	Angle_az=-Angle_az;
	}

    /*角速度*/
	if(Gyro_y<32768)//向前运动
	{
	Gyro_y=-(Gyro_y/16.4);//范围为1000deg/s时，换算关系：16.4 LSB/(deg/s)
	}
	if(Gyro_y>32768)//向后运动
	{
	Gyro_y=+(65535-Gyro_y)/16.4;
	}
	
	//Angle_gy = Angle_gy + Gyro_y*0.025;  //角速度积分得到倾斜角度.越大积分出来的角度越大	
	 	
	//-------卡尔曼滤波融合-----------------------

	Kalman_Filter(Angle_az,Gyro_y);       //卡尔曼滤波计算倾角
															  
} 



/**********************************************************第3步：卡尔曼参数*************************************************/
		
float Q_angle=0.001;//0.001  
float Q_gyro=0.003;//0.03
float R_angle=0.5;//0.5
float dt=0.01;//0.1	                  //dt为kalman滤波器采样时间;
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

/*卡尔曼函数*/
void Kalman_Filter(float Accel,float Gyro)		
{
	Angle+=(Gyro - Q_bias) * dt; //先验估计

	
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	Gyro_y   = Gyro - Q_bias;	 //输出值(后验估计)的微分=角速度

}


/****************************************************第4 步：电机转速和位移值计算******************************************/


//void Psn_Calcu(void)	 
//{
//	
//	speed_r_l =PWM*0.5;
//	speed *= 0.7;		                  //车轮速度滤波
//	speed += speed_r_l*0.3;	
//	position += speed;	                  //积分得到位移
//}

/******************************************************第5步：电机PID算法**********************************************/
extern float Kp;
static float Kd =12.0;	    //2.6PID参数
// static float Kpn = 0.0;    //0.01PID参数
// static float Ksp = 0.0;	    //2.0PID参数

void PWM_Calcu()	 
{
	
// 	speed=PWM*0.04;//把PWM信号替代速度信号	
// 	
// 	speed_filter*=0.9;		
// 	speed_filter+=speed*0.1;//速度信号滤波	
// 	
// 	position+=speed_filter;//速度转化成位移信号
// 	
// 		
// 	PWM  = Kp*Angle+Kd*Gyro_y+Kpn*position + Ksp*speed_filter;          //PID：PWM= kp*倾斜角+kd*角速度 + kpn*位移量 +ksp*电机转速滤波后的值		
//	speed=PWM*0.1;
	
	
	PWM=1000*((Angle/90)*Kp)+Kd*Gyro_y;
//	PWM  = 12*Kp*Angle+Kd*Gyro_y;
	
	
	int_PWM=(int)PWM;//强制转化成整形PWM
}


















