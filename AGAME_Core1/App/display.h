#ifndef __DISPLAY_H__
#define __DISPLAY_H__

void displayUintNum8x16(uint16 i,uint8 x,uint8 y);  //��ʾ0~65535 �ֺ� 8x16
void displayintNum8x16(int16 i,uint8 x,uint8 y);  //��ʾ-32767~32768 �ֺ� 8x16
void displayUintNum6x8(uint16 i,uint8 x,uint8 y);  //��ʾ0~65535 �ֺ� 6x8
void displayintNum6x8(int16 i,uint8 x,uint8 y);  //��ʾ-32767~32768 �ֺ� 6x8
void displayUfloatNum8x16(float i,uint8 x,uint8 y); //��ʾ 00.00 �ֺ� 8x16
void displayfloatNum8x16(float i,uint8 x,uint8 y); //��ʾ -00.00~+00.00 �ֺ� 8x16
void displayUfloatNum6x8(float i,uint8 x,uint8 y); //��ʾ 00.00 �ֺ� 6x8
void displayfloatNum6x8(float i,uint8 x,uint8 y); //��ʾ -00.00~+00.00 �ֺ� 6x8

void display_image(uint8 image[60][80]); //��ʾͼƬ

#endif 