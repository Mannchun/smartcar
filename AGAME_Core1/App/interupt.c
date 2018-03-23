#include "common.h"
#include "include.h"
#include "interupt.h"

#define Center	163//复制在main.c中
#define Left	Center-30//复制在main.c中
#define Right	Center+30//复制在main.c中
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
#define LED   PTB23     //LED灯
 
/* PORTC中断服务函数 */
void PORTC_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTC_ISFR);
    flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                         	//清中断标志位

    n = 17;                                   	//场中断
    if(flag & (1 << n))                      	//PTC7触发中断
    {
        camera_vsync();
    }
    #if ( CAMERA_USE_HREF == 1 )            	//使用行中断
        n = 6;
        if(flag & (1 << n))                   	//PTC6触发中断
        {
            camera_href();
        }
    #endif
}

/* DMA0中断服务函数 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/* PIT0中断服务函数 */
void PIT0_IRQHandler(void)
{
    pid.real_pulse = -(ftm_quad_get(FTM2));//获取脉冲计数
    pid.real_speed =  (int16)(pid.real_pulse/V2PWM_10MS*100);//将脉冲数转化为m/s
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
    ftm_quad_clean(FTM2);           //清空FTM脉冲计数
    
    if (stopFlag == 1)//停止或运行PID
    {
        if(BeginFlag == 0)
            ftm_pwm_duty(S3010_FTM, S3010_CH, Left);
        STOP();
        //gpio_set (LED ,1);                      //LED灭
    }
    else 
    {
        RUN(&pid);
        //gpio_set (LED , 0);                     //LED开
    }

    PIT_Flag_Clear(PIT0);       //清中断标志位
}

/* PIT1中断服务函数 */
void PIT1_IRQHandler(void)
{
    sysTime += 2;
    PIT_Flag_Clear(PIT1);       //清中断标志位
}

/* UART4 中断服务函数 */
void uart4_handler(void)
{
    UARTn_e uratn = UART4;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        //用户需要处理接收数据
       char str[1];
       uart_getchar (UART4,str);
       if (str[0]== 's')
       { 
         stopFlag = 1;
         //uart_putstr(UART4, "Y\n");                    //发送字符串，已接收指令
       }
       else if (str[0]== 'r')
       { 
         stopFlag = 0;  
        /* pit_init_ms(PIT1,1000);                                //初始化PIT1，定时时间为： 1000ms
         set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);       //设置PIT0的中断复位函数为 PIT1_IRQHandler
         enable_irq (PIT1_IRQn);                                  //使能PIT1中断
         */
         //uart_putstr(UART4, "Y\n");                    //发送字符串，已接收指令
       }
       else if (str[0]== '2')
       { 
         pid.set_speed = 2*100;
         //uart_putstr(UART4, "Y\n");                    //发送字符串，已接收指令
       }
       else
       {
         //uart_putstr(UART4, "N\n");                    //发送字符串,未接收到正确指令
       }
    }
}

extern uint8 theBar[2];
extern int LockFlag;
/* UART3 中断服务函数 */
void uart3_handler(void)
{
    uart_rx_irq_dis(UART3);         //关串口3接收中断
    UARTn_e uratn = UART3;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
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
   /*  uart_putnum(VCAN_PORT, 0);//信标x坐标
     uart_putchar (VCAN_PORT, ',');
     uart_putnum(VCAN_PORT, 0);//信标y坐标
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
