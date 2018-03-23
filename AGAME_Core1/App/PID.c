/* PID */
#include "common.h"
#include "PID.h"
#include "include.h"

extern uint8 centerY;

/* PID初始化，在此处调节 p i d 参数 */
void PID_init(Pid *Pid)
{
    Pid->set_speed = 0.0;
    Pid->real_pulse = 0.0;
    Pid->real_speed = 0.0;
    Pid->real_speed_1=0.0;
    Pid->err_0 = 0.0;
    Pid->err_1 = 0.0;
    Pid->err_2 = 0.0;
    Pid->Kp = 4.0;
    Pid->Ki = 0.8;//0.8
    Pid->Kd = 1.0;//1.0
    Pid->umax = 0.0;
    Pid->umin = 0.0;
    Pid->PWMnum = 0;
}

/* PID运行，speed 为设定输入，一般作为用转速， actualspeed 为编码器采集数值 */
// speed: 0~4.0m 
// actualspeed: 
/****************************************************************************/
float PID_realize(Pid *Pid)
{
    float index = 1;
    Pid->err_2 = Pid->err_1;
    Pid->err_1 = Pid->err_0;
    Pid->err_0 = Pid->set_speed - Pid->real_speed;
    
    /* 抗积分饱和：计算u(k)时，首先判断上一时刻的控制量u(k-1)是否已经超出了极限范围，
       如果u(k-1)>umax，则只累加负偏差; 如果u(k-1)<umin，则只累加正偏差。从而避免控制量长时间停留在饱和区。 */
   // float speederror = Pid->real_speed - Pid->real_speed_1;
   // if(speederror <  70 && speederror > -70 )
      Pid->PWMnum +=  Pid->Kp *(Pid->err_0-Pid->err_1)+index * Pid->Ki*Pid->err_0+Pid->Kd*(Pid->err_0-2*Pid->err_1+Pid->err_2);
      
 
    if(Pid->PWMnum >= 1000) Pid->PWMnum=1000;
    if(Pid->PWMnum <= -500) Pid->PWMnum=-500;
    return (Pid->PWMnum);
    
    /* 积分分离：当当被控量与设定值偏差较大时，取消积分作用; 当被控量接近给定值时，引入积分控制，以消除静差，提高精度 */

   /* if (Pid->real_speed >= Pid->set_speed )
    {
      
    }
    else
    {
        // 变积分PID的基本思想： 设法改变积分项的累加速度，使其与偏差大小相对应：偏差越大，积分越慢; 偏差越小，积分越快。 
        //
        //else
          index = 1;
    }*/
}


void PID_init_steer(Pid_steer *Pid)
{
    Pid->set_pwm = 72;
    Pid->err_0 = 0.0;
    Pid->err_1 = 0.0;
    Pid->err_2 = 0.0;
    Pid->Kp = 0.45;
    Pid->Ki = 0.0;//0.8
    Pid->Kd = 0.0;//1.0
    Pid->umax = 95;
    Pid->umin = 49;
    Pid->PWMnum = 72;
}

uint16 PID_realize_steer(Pid_steer *Pid)
{
    Pid->err_2 = Pid->err_1;
    Pid->err_1 = Pid->err_0;
    Pid->err_0 = centerY - 40;
    Pid->PWMnum +=  Pid->Kp *(Pid->err_0-Pid->err_1)+Pid->Ki*Pid->err_0+Pid->Kd*(Pid->err_0-2*Pid->err_1+Pid->err_2);
    if(Pid->PWMnum >= Pid->umax) Pid->PWMnum=Pid->umax;
    if(Pid->PWMnum <= Pid->umin) Pid->PWMnum=Pid->umin;
    return (Pid->PWMnum);
}