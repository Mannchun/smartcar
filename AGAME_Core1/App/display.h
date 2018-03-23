#ifndef __DISPLAY_H__
#define __DISPLAY_H__

void displayUintNum8x16(uint16 i,uint8 x,uint8 y);  //ÏÔÊ¾0~65535 ×ÖºÅ 8x16
void displayintNum8x16(int16 i,uint8 x,uint8 y);  //ÏÔÊ¾-32767~32768 ×ÖºÅ 8x16
void displayUintNum6x8(uint16 i,uint8 x,uint8 y);  //ÏÔÊ¾0~65535 ×ÖºÅ 6x8
void displayintNum6x8(int16 i,uint8 x,uint8 y);  //ÏÔÊ¾-32767~32768 ×ÖºÅ 6x8
void displayUfloatNum8x16(float i,uint8 x,uint8 y); //ÏÔÊ¾ 00.00 ×ÖºÅ 8x16
void displayfloatNum8x16(float i,uint8 x,uint8 y); //ÏÔÊ¾ -00.00~+00.00 ×ÖºÅ 8x16
void displayUfloatNum6x8(float i,uint8 x,uint8 y); //ÏÔÊ¾ 00.00 ×ÖºÅ 6x8
void displayfloatNum6x8(float i,uint8 x,uint8 y); //ÏÔÊ¾ -00.00~+00.00 ×ÖºÅ 6x8

void display_image(uint8 image[60][80]); //ÏÔÊ¾Í¼Æ¬

#endif 