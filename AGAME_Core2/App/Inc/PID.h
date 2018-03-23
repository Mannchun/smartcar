/* PID���ƴ��� */

#ifndef __PID_H__
#define __PID_H__
/* PID�ṹ�� */
struct _pid {
	float SetSpeed;    //�����趨ֵ
	float ActualSpeed; //����ʵ��ֵ
	float err;         //����ƫ��ֵ
	float err_nest;    //������һ��ƫ��ֵ
	float err_last;     //��������ǰ��ƫ��ֵ
	float Kp, Ki, Kd;  //������������֡�΢��ϵ��
	float voltage;     //�����ѹֵ������ִ�����ı�����
	float integral;    //�������ֵ
	float umax;        //�������
	float umin;	   //�������
        float PWMnum;      //pwmռ�ձ�
};

void PID_init();
/* PID���У�speed Ϊ�趨���룬һ����Ϊ��ת�٣� actualspeed Ϊ�������ɼ���ֵ */
// speed: 0~4.0m 
// actualspeed: 
/****************************************************************************/
float PID_realize(float setspeed, float actualspeed);



#endif
