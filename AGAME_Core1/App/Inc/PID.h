/* PID���ƴ��� */

#ifndef __PID_H__
#define __PID_H__

#define V2PWM_10MS 73.0
extern uint16 speed;

/* PID�ṹ�� */
typedef struct{
    float set_speed;     //�����趨�ٶ�
    int16 set_pulse;     //�����趨������
    int16 real_pulse;    //����ʵ������ֵ
    float real_speed;    //����ʵ���ٶ�ֵ
    float real_speed_1;    //������һ��ʵ���ٶ�ֵ
    float err_0;         //����ƫ��ֵ
    float err_1;         //������һ��ƫ��ֵ
    float err_2;         //��������ǰ��ƫ��ֵ
    float Kp, Ki, Kd;    //������������֡�΢��ϵ��
    float umax;          //�������
    float umin;	  //�������
    int16 PWMnum;        //pwmռ�ձ�
}Pid;


typedef struct{
    int16 set_pwm;     //�����趨�ٶ�
    int16 err_0;         //����ƫ��ֵ
    int16 err_1;         //������һ��ƫ��ֵ
    int16 err_2;         //��������ǰ��ƫ��ֵ
    float Kp, Ki, Kd;    //������������֡�΢��ϵ��
    int16 umax;          //�������
    int16 umin;	  //�������
    int16 PWMnum;        //pwmռ�ձ�
}Pid_steer;

void PID_init(Pid *Pid);
void PID_init_steer(Pid_steer *Pid);
/* PID���У�speed Ϊ�趨���룬һ����Ϊ��ת�٣� actualspeed Ϊ�������ɼ���ֵ */
// speed: 0~4.0m 
// actualspeed: 
/****************************************************************************/
float PID_realize(Pid *Pid);
uint16 PID_realize_steer(Pid_steer *Pid);
#endif
