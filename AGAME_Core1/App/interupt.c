#include "common.h"
#include "include.h"
#include "interupt.h"

#define Center	163//������main.c��
#define Left	Center-30//������main.c��
#define Right	Center+30//������main.c��
uint8 startpid = 1;
Pid pid;
uint16 pulse;
uint8 steerflag = 0;
int stallTimer = 0;
int backLock = 100;
volatile uint32 sysTime = 0; 
extern uint8 LeftFlag;
extern uint8 BACKFLAG;
extern uint8 stopFlag;
extern int BeginFlag;
extern uint8 stopPro;
static uint8 backCount = 0;
#define LED   PTB23     //LED��
 
/* PORTC�жϷ����� */
void PORTC_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTC_ISFR);
    flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                         	//���жϱ�־λ

    n = 17;                                   	//���ж�
    if(flag & (1 << n))                      	//PTC7�����ж�
    {
        camera_vsync();
    }
    #if ( CAMERA_USE_HREF == 1 )            	//ʹ�����ж�
        n = 6;
        if(flag & (1 << n))                   	//PTC6�����ж�
        {
            camera_href();
        }
    #endif
}

/* DMA0�жϷ����� */
void DMA0_IRQHandler()
{
    camera_dma();
}

/* PIT0�жϷ����� */
void PIT0_IRQHandler(void)
{
    pid.real_pulse = -(ftm_quad_get(FTM2));//��ȡ�������
    pid.real_speed =  (int16)(pid.real_pulse/V2PWM_10MS*100);//��������ת��Ϊm/s
    if(0<=pid.real_speed && pid.real_speed<0.7 && stopFlag == 0 && backLock == 0){
        ++stallTimer;
	if(stallTimer > 8 && BACKFLAG == 1){
	    if(stopPro == 1){
	        ++backCount;
		if(backCount == 4) {stopFlag = 1;}
		else {
		    turnback();
		    backLock = 100;
		}
	    }
	    else {
	        turnback();
		backLock = 100;
	    }
	    stallTimer = 0;
	}
    }
    if(backLock>0) --backLock;
    ftm_quad_clean(FTM2);           //���FTM�������
    
    if (stopFlag == 1)//ֹͣ������PID
    {
        if(BeginFlag == 0)
            ftm_pwm_duty(S3010_FTM, S3010_CH, Left);
        STOP();
        //gpio_set (LED ,1);                      //LED��
    }
    else 
    {
        RUN(&pid);
        //gpio_set (LED , 0);                     //LED��
    }

    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}

/* PIT1�жϷ����� */
void PIT1_IRQHandler(void)
{
    sysTime += 2;
    PIT_Flag_Clear(PIT1);       //���жϱ�־λ
}

/* UART4 �жϷ����� */
void uart4_handler(void)
{
    UARTn_e uratn = UART4;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        //�û���Ҫ�����������
       char str[1];
       uart_getchar (UART4,str);
       if (str[0]== 's')
       { 
         stopFlag = 1;
         //uart_putstr(UART4, "Y\n");                    //�����ַ������ѽ���ָ��
       }
       else if (str[0]== 'r')
       { 
         stopFlag = 0;  
        /* pit_init_ms(PIT1,1000);                                //��ʼ��PIT1����ʱʱ��Ϊ�� 1000ms
         set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);       //����PIT0���жϸ�λ����Ϊ PIT1_IRQHandler
         enable_irq (PIT1_IRQn);                                  //ʹ��PIT1�ж�
         */
         //uart_putstr(UART4, "Y\n");                    //�����ַ������ѽ���ָ��
       }
       else if (str[0]== '2')
       { 
         pid.set_speed = 2*100;
         //uart_putstr(UART4, "Y\n");                    //�����ַ������ѽ���ָ��
       }
       else
       {
         //uart_putstr(UART4, "N\n");                    //�����ַ���,δ���յ���ȷָ��
       }
    }
}

extern uint8 theBar[2];
extern int LockFlag;
/* UART3 �жϷ����� */
void uart3_handler(void)
{
    uart_rx_irq_dis(UART3);         //�ش���3�����ж�
    UARTn_e uratn = UART3;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        uint8 temp;
	uart_getuint8(UART3,&temp);
	if(temp == 's' && LockFlag == 0){
	    uart_getuint8(UART3,&theBar[0]);
	    uart_getuint8(UART3,&theBar[1]);
	}
	//uart_putstr(VCAN_PORT,theBar);
    }
    uart_rx_irq_en(UART3); 
}

void send_PC(float x, float y, float V, float v, float U1, float U2)
{
   /*  uart_putnum(VCAN_PORT, 0);//�ű�x����
     uart_putchar (VCAN_PORT, ',');
     uart_putnum(VCAN_PORT, 0);//�ű�y����
     uart_putchar (VCAN_PORT, ',');
  */ 
     v=v/100.0;
     uart_putnum(VCAN_PORT, V);
     uart_putchar (VCAN_PORT, ',');
     
     uart_putnum(VCAN_PORT, v);
     uart_putchar (VCAN_PORT, ',');
     uart_putn(VCAN_PORT, U1);
     //uart_putchar (VCAN_PORT, ',');
     //uart_putnum(VCAN_PORT, U2);
     
     uart_putchar (VCAN_PORT, '\r');
     uart_putchar (VCAN_PORT, '\n');
     
}

void turnback()
{
    if(LeftFlag == 0)
        ftm_pwm_duty(FTM1, FTM_CH0, Left);
    else if(LeftFlag == 1)
        ftm_pwm_duty(FTM1, FTM_CH0, Right);
    ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 800);
    ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
    ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 800);
    ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
    systick_delay_ms(400);
}
