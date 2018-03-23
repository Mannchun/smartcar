/* PID */
#include "common.h"
#include "PID.h"

struct _pid  pid;
/* PID��ʼ�����ڴ˴����� p i d ���� */
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
/* PID���У�speed Ϊ�趨���룬һ����Ϊ��ת�٣� actualspeed Ϊ�������ɼ���ֵ */
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

	/* �����ֱ��ͣ�����u(k)ʱ�������ж���һʱ�̵Ŀ�����u(k-1)�Ƿ��Ѿ������˼��޷�Χ��
           ���u(k-1)>umax����ֻ�ۼӸ�ƫ��; ���u(k-1)<umin����ֻ�ۼ���ƫ��Ӷ������������ʱ��ͣ���ڱ������� */
	/* ���ַ��룺�������������趨ֵƫ��ϴ�ʱ��ȡ����������; ���������ӽ�����ֵʱ��������ֿ��ƣ������������߾��� */

	if (error > setspeed)
	{
		index = 0;
	}
	else
	{
		/* �����PID�Ļ���˼�룺 �跨�ı��������ۼ��ٶȣ�ʹ����ƫ���С���Ӧ��ƫ��Խ�󣬻���Խ��; ƫ��ԽС������Խ�졣 */
		if (error > (setspeed*0.9)) index = (setspeed - error) / 50;
		else index = 1;
	}

	float incrementSpeed = pid.Kp*(pid.err - pid.err_nest) + index* pid.Ki*pid.err + pid.Kd*(pid.err - 2 * pid.err_nest + pid.err_last);
	pid.PWMnum += incrementSpeed;
	pid.err_last = pid.err_nest;
	pid.err_nest = pid.err;
	return (pid.PWMnum/7.15);
}