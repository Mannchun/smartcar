#ifndef __INTERUPT_H__
#define __INTERUPT_H__

void PORTC_IRQHandler();//PORTA中断服务函数
void DMA0_IRQHandler(); //DMA0中断服务函数
void PIT0_IRQHandler(void);//PIT0中断服务函数

#endif