#include "common.h"
#include "include.h"

//LED灯
#define LED_R   PTA14
#define LED_Y   PTA15
#define LED_B   PTA16

#define Buzzer  PTA5 //VCNA_camera also defined

int find;
Barrier Bar;
int whiteFlag = 1;

void  main(void)
{
    int detectRange[60][2] = {
    40,41,
    40,41,
    40,41,
    40,41,
    40,41,
    24,57,
    24,57,
    23,58,
    23,58,
    22,59,
    22,59,
    21,60,
    21,60,
    20,61,
    20,61,
    19,61,
    19,61,
    18,62,
    18,62,
    17,63,
    17,63,
    16,63,
    16,64,
    15,64,
    15,65,
    14,65,
    14,66,
    13,66,
    12,66,
    12,67,
    
    12,67,
    11,68,
    11,68,
    11,68,
    11,68,
    10,68,
    10,68,
    10,69,
    9,69,
    9,69,
    9,70,
    8,70,
    8,70,
    8,71,
    7,71,
    7,71,
    7,71,
    6,72,
    6,72,
    6,72,
    6,73,
    5,73,
    5,73,
    5,73,
    4,74,
    4,74,
    4,74,
    4,74,
    4,75,
    4,75};
    gpio_init (LED_Y, GPO, 0);//LED灯初始化 
    gpio_init (LED_R, GPO, 1);//LED灯初始化
    gpio_init (Buzzer, GPO, 0);//蜂鸣器初始化
    uint16 ledcount = 0;

    camera_init(imgbuff);//摄像头初始化
    
    uart_init (VCAN_PORT, VCAN_BAUD);
    uart_init (CHIP_PORT, CHIP_BAUD);
    //配置中断服务函数                            
    set_vector_handler(PORTC_VECTORn , PORTC_IRQHandler);//设置 PORTC 的中断服务函数为 PORTC_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);//设置 DMA0 的中断服务函数为 DMA0_IRQHandler
    
    pit_init_ms(PIT0, 25); //初始化PIT0，定时时间为:25ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);       //设置PIT0的中断复位函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);                                  //使能PIT0中断

    while(1)
    {
	ledcount++;
	if(ledcount == 20){
	    gpio_turn (LED_R);
	    ledcount = 0;
	}
	//uart_putstr (UART2, "%OK");
	camera_get_img();                                        //摄像头获取图像
	int start = 59;
	find = barrierDetection(detectRange, &Bar, &start);
	//vcan_sendimg(imgbuff, sizeof(imgbuff));                 //发送图像 
    }
}

