/* PID */
#include "common.h"
#include "PID.h"

struct _pid  pid;
/* PID初始化，在此处调节 p i d 参数 */
void PID_init()
{
	printf("PIT_init begin \n");
	pid.SetSpeed = 0.0;
	pid.ActualSpeed = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.Kp = 0.2;
	pid.Ki = 0.2;
	pid.Kd = 0.2;
	pid.umax = 700;
	pid.umin = 100;
        pid.PWMnum   = 0;
	printf("PIT_init end \n");
}
/* PID运行，speed 为设定输入，一般作为用转速， actualspeed 为编码器采集数值 */
// speed: 0~4.0m 
// actualspeed: 
/****************************************************************************/
float PID_realize(float setspeed, float actualspeed)
{
	float index;
	float error;
	pid.SetSpeed = setspeed*7150.0;
        pid.ActualSpeed = actualspeed;
	pid.err = pid.SetSpeed - pid.ActualSpeed;
	error = pid.err;
	if (pid.err < 0) error = -error;

	/* 抗积分饱和：计算u(k)时，首先判断上一时刻的控制量u(k-1)是否已经超出了极限范围，
           如果u(k-1)>umax，则只累加负偏差; 如果u(k-1)<umin，则只累加正偏差。从而避免控制量长时间停留在饱和区。 */
	/* 积分分离：当当被控量与设定值偏差较大时，取消积分作用; 当被控量接近给定值时，引入积分控制，以消除静差，提高精度 */

	if (error > setspeed)
	{
		index = 0;
	}
	else
	{
		/* 变积分PID的基本思想： 设法改变积分项的累加速度，使其与偏差大小相对应：偏差越大，积分越慢; 偏差越小，积分越快。 */
		if (error > (setspeed*0.9)) index = (setspeed - error) / 50;
		else index = 1;
	}

	float incrementSpeed = pid.Kp*(pid.err - pid.err_nest) + index* pid.Ki*pid.err + pid.Kd*(pid.err - 2 * pid.err_nest + pid.err_last);
	pid.PWMnum += incrementSpeed;
	pid.err_last = pid.err_nest;
	pid.err_nest = pid.err;
	return (pid.PWMnum/7.15);
}