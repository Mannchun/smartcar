#ifndef __INTERUPT_H__
#define __INTERUPT_H__

//���
#define MOTOR1_IO   PTC1
#define MOTOR2_IO   PTC2
#define MOTOR3_IO   PTC3
#define MOTOR4_IO   PTC4

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH0
#define MOTOR2_PWM  FTM_CH1
#define MOTOR3_PWM  FTM_CH2
#define MOTOR4_PWM  FTM_CH3

#define MOTOR_HZ    8000
#define MOTOR_DUTY  50


void PORTC_IRQHandler(void);  /* PORTC�жϷ����� */
void DMA0_IRQHandler(void);   /* DMA0�жϷ����� */
void PIT0_IRQHandler(void);   /* PIT0�жϷ����� */
void PIT1_IRQHandler(void);   /* PIT1�жϷ����� */
void uart4_handler(void);     /* UART4 �жϷ����� */
void uart3_handler(void);     /* UART3 �жϷ����� */
void send_PC(float x, float y, float V, float v, float Pid_L_PWM, float Pid_R_PWM);
void turnback();


#endif /* __MK60_CONF_H__  */