/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
 *           常用的有 1us 10us 1ms 中断。         

 * 库版本  ：ST3.5.0
*/

#include "SysTick.h"
#include "assignment1.h"
#include "assignment2.h"
#include "assignment3.h"

extern float Angle;//角度
extern float Gyro_y;//陀螺仪

extern float Accel_x;//X轴加速度值暂存
extern float Accel_y;//Y轴加速度值暂存
extern float Accel_z;//加速度计值暂存

extern float Angle_az;//加速度计算输出角度	   
extern float Angle_gy;//陀螺仪计算的倾角

extern float PWM;//浮点型PEM值

extern int PWM_R;//右轮PWM值计算
extern int PWM_L; //左轮PWM值计算

extern int ADC_Direction;
extern int Direction_Difference;

extern float x;
extern float y;
extern float z;

extern int int_PWM;//整形PWM值

extern float integral;//速度积分百分比

/*启动系统滴答定时器 SysTick */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	 SysTick_Config(SystemCoreClock /200);//200hz系统频率

}

/*定时器中断服务函数，构建系统运行周期*/
float a;
float Kp;

void SysTick_Handler()
{	
	
	a++;
	Kp=a/40;
	if(Kp>12)
	{
	Kp=12;	
	}//调试kp的参数，这个算法用于缓慢加载pid参数
		
	
	Angle_Calcu();//角度计算
	PWM_Calcu();//pwm计算输出
	Direction_Dispose();//差速算法，pwm输出
//	printf("%f\n",Kp);

}




