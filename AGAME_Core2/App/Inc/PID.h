/* PID控制代码 */

#ifndef __PID_H__
#define __PID_H__
/* PID结构体 */
struct _pid {
	float SetSpeed;    //定义设定值
	float ActualSpeed; //定义实际值
	float err;         //定义偏差值
	float err_nest;    //定义上一个偏差值
	float err_last;     //定义最上前的偏差值
	float Kp, Ki, Kd;  //定义比例、积分、微分系数
	float voltage;     //定义电压值（控制执行器的变量）
	float integral;    //定义积分值
	float umax;        //输出上限
	float umin;	   //输出下限
        float PWMnum;      //pwm占空比
};

void PID_init();
/* PID运行，speed 为设定输入，一般作为用转速， actualspeed 为编码器采集数值 */
// speed: 0~4.0m 
// actualspeed: 
/****************************************************************************/
float PID_realize(float setspeed, float actualspeed);



#endif
