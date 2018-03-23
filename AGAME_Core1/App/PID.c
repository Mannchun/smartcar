/* PID */
#include "common.h"
#include "PID.h"
#include "include.h"

extern uint8 centerY;

/* PID��ʼ�����ڴ˴����� p i d ���� */
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

/* PID���У�speed Ϊ�趨���룬һ����Ϊ��ת�٣� actualspeed Ϊ�������ɼ���ֵ */
// speed: 0~4.0m 
// actualspeed: 
/****************************************************************************/
float PID_realize(Pid *Pid)
{
    float index = 1;
    Pid->err_2 = Pid->err_1;
    Pid->err_1 = Pid->err_0;
    Pid->err_0 = Pid->set_speed - Pid->real_speed;
    
    /* �����ֱ��ͣ�����u(k)ʱ�������ж���һʱ�̵Ŀ�����u(k-1)�Ƿ��Ѿ������˼��޷�Χ��
       ���u(k-1)>umax����ֻ�ۼӸ�ƫ��; ���u(k-1)<umin����ֻ�ۼ���ƫ��Ӷ������������ʱ��ͣ���ڱ������� */
   // float speederror = Pid->real_speed - Pid->real_speed_1;
   // if(speederror <  70 && speederror > -70 )
      Pid->PWMnum +=  Pid->Kp *(Pid->err_0-Pid->err_1)+index * Pid->Ki*Pid->err_0+Pid->Kd*(Pid->err_0-2*Pid->err_1+Pid->err_2);
      
 
    if(Pid->PWMnum >= 1000) Pid->PWMnum=1000;
    if(Pid->PWMnum <= -500) Pid->PWMnum=-500;
    return (Pid->PWMnum);
    
    /* ���ַ��룺�������������趨ֵƫ��ϴ�ʱ��ȡ����������; ���������ӽ�����ֵʱ��������ֿ��ƣ������������߾��� */

   /* if (Pid->real_speed >= Pid->set_speed )
    {
      
    }
    else
    {
        // �����PID�Ļ���˼�룺 �跨�ı��������ۼ��ٶȣ�ʹ����ƫ���С���Ӧ��ƫ��Խ�󣬻���Խ��; ƫ��ԽС������Խ�졣 
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