/* PID控制代码 */

#ifndef __PID_H__
#define __PID_H__

#define V2PWM_10MS 73.0
extern uint16 speed;

/* PID结构体 */
typedef struct{
    float set_speed;     //定义设定速度
    int16 set_pulse;     //定义设定脉冲数
    int16 real_pulse;    //定义实际脉冲值
    float real_speed;    //定义实际速度值
    float real_speed_1;    //定义上一刻实际速度值
    float err_0;         //定义偏差值
    float err_1;         //定义上一个偏差值
    float err_2;         //定义最上前的偏差值
    float Kp, Ki, Kd;    //定义比例、积分、微分系数
    float umax;          //输出上限
    float umin;	  //输出下限
    int16 PWMnum;        //pwm占空比
}Pid;


typedef struct{
    int16 set_pwm;     //定义设定速度
    int16 err_0;         //定义偏差值
    int16 err_1;         //定义上一个偏差值
    int16 err_2;         //定义最上前的偏差值
    float Kp, Ki, Kd;    //定义比例、积分、微分系数
    int16 umax;          //输出上限
    int16 umin;	  //输出下限
    int16 PWMnum;        //pwm占空比
}Pid_steer;

void PID_init(Pid *Pid);
void PID_init_steer(Pid_steer *Pid);
/* PID运行，speed 为设定输入，一般作为用转速， actualspeed 为编码器采集数值 */
// speed: 0~4.0m 
// actualspeed: 
/****************************************************************************/
float PID_realize(Pid *Pid);
uint16 PID_realize_steer(Pid_steer *Pid);
#endif
