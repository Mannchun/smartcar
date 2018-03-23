#include "interupt.h"
#include "common.h"
#include "include.h"


extern int find;
extern Barrier Bar;

/* PORTC中断服务函数 */
void PORTC_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTC_ISFR);
    flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //清中断标志位

    n = 17;                                             //场中断
    if(flag & (1 << n))                                 //PTC7触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 6;
    if(flag & (1 << n))                                 //PTC6触发中断
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
    if(find == 1){
        float temp = Bar.centerY - 0.000127*Bar.centerY*Bar.centerY*Bar.centerY+0.01097*Bar.centerY*Bar.centerY-0.6681*Bar.centerY+22.42;
	uint8 mappedY = (uint8)(temp);
	if(temp - mappedY >= 0.5) ++mappedY;
	uart_putchar(CHIP_PORT, 's');  // 数据报头
	uart_putchar(CHIP_PORT, Bar.centerX);
	uart_putchar(CHIP_PORT, mappedY);
    }
    else{
        uart_putchar(CHIP_PORT, 's');  // 数据报头
        uart_putchar(CHIP_PORT, 0);
	uart_putchar(CHIP_PORT, 0);
    }
    PIT_Flag_Clear(PIT0);       //清中断标志位
}