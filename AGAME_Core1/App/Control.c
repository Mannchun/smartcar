/* 控制算法 */
#include "common.h"
#include "Control.h"
#include "include.h"

/* pwm_L=(-1000~1000) 占空比控制，不能超过1000，-1000 */
void RUN(Pid *Pid)
{
    int16 pwm;
    pwm = (int16)PID_realize(Pid);
    {
        if(pwm <= 1000 && pwm >= -500){
            if(pwm >= 0){
                ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, pwm);
                ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, pwm);
            }
            else{
                pwm = -pwm;
                ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, pwm);
                ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, pwm);
                ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
            }
        }
        else{
            if(pwm > 700){
                ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 1000);
                ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 1000);
            }
            else{
                ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 500);
                ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 500);
                ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
            }
        }
    }
}


/* 停车函数 */
void STOP(void)
{
    ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
    ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
    ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
    ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
}

/* 转向函数 */
void TURN()
{
     
}