/* OLED������ʾ */
#include "common.h"
#include "display.h"
#include "include.h"

void displayUintNum8x16(uint16 i,uint8 x,uint8 y)  //��ʾ0~65535 �ֺ� 8x16
{
    uint8 vol[5] = {0};
    vol[0] = i / 10000 + 0x30;
    vol[1] = i % 10000 / 1000 + 0x30;
    vol[2] = i % 1000 / 100 + 0x30;
    vol[3] = i % 100 / 10  + 0x30;  
    vol[4] = i % 10  + 0x30;  
    LCD_P8x16Str( x, y, vol);
}

void displayintNum8x16(int16 i,uint8 x,uint8 y)  //��ʾ-32767~32768 �ֺ� 8x16
{
  uint8 vol[6] = {0};
  if(i < 0){
    vol[0] = '-';
    i = -i;
  }
  else{
    vol[0] = '+';
  }
    vol[1] = i / 10000 + 0x30;
    vol[2] = i % 10000 / 1000 + 0x30;
    vol[3] = i % 1000 / 100 + 0x30;
    vol[4] = i % 100 / 10  + 0x30;  
    vol[5] = i % 10  + 0x30;  
    LCD_P8x16Str( x, y, vol);
}

void displayUintNum6x8(uint16 i,uint8 x,uint8 y)  //��ʾ0~65535 �ֺ� 6x8
{
    uint8 vol[5] = {0};
    vol[0] = i / 10000 + 0x30;
    vol[1] = i % 10000 / 1000 + 0x30;
    vol[2] = i % 1000 / 100 + 0x30;
    vol[3] = i % 100 / 10  + 0x30;  
    vol[4] = i % 10  + 0x30;  
    LCD_P6x8Str( x, y, vol);
}

void displayintNum6x8(int16 i,uint8 x,uint8 y)  //��ʾ-32767~32768 �ֺ� 6x8
{
  uint8 vol[6] = {0};
  if(i < 0){
    vol[0] = '-';
    i = -i;
  }
  else{
    vol[0] = '+';
  }
    vol[1] = i / 10000 + 0x30;
    vol[2] = i % 10000 / 1000 + 0x30;
    vol[3] = i % 1000 / 100 + 0x30;
    vol[4] = i % 100 / 10  + 0x30;  
    vol[5] = i % 10  + 0x30;  
    LCD_P6x8Str( x, y, vol);
}
void displayUfloatNum8x16(float i,uint8 x,uint8 y) //��ʾ 00.00 �ֺ� 8x16
{
    uint16 temp = (uint16)(i*100);
    uint8 vol[5] = {0};
    vol[0] = temp / 1000 + 0x30;
    vol[1] = temp % 1000 / 100 + 0x30;
    vol[2] = '.';
    vol[3] = temp % 100 / 10 + 0x30;
    vol[4] = temp % 10  + 0x30;  
    LCD_P8x16Str( x, y, vol);
}
void displayfloatNum8x16(float i,uint8 x,uint8 y) //��ʾ -00.00~+00.00 �ֺ� 8x16
{
    uint8 vol[6] = {0};
    uint16 temp = 0;
    if(i < 0){
      vol[0] = '-';
      temp = (uint16)(-i*100);
    }
    else{
      vol[0] = '+';
      temp = (uint16)(i*100);
    }
    vol[1] = temp / 1000 + 0x30;
    vol[2] = temp % 1000 / 100 + 0x30;
    vol[3] = '.';
    vol[4] = temp % 100 / 10 + 0x30;
    vol[5] = temp % 10  + 0x30;  
    LCD_P8x16Str( x, y, vol);
}
void displayUfloatNum6x8(float i,uint8 x,uint8 y) //��ʾ 00.00 �ֺ� 6x8
{
    uint16 temp = (uint16)(i*100);
    uint8 vol[5] = {0};
    vol[0] = temp / 1000 + 0x30;
    vol[1] = temp % 1000 / 100 + 0x30;
    vol[2] = '.';
    vol[3] = temp % 100 / 10 + 0x30;
    vol[4] = temp % 10  + 0x30;  
    LCD_P6x8Str( x, y, vol);
}
void displayfloatNum6x8(float i,uint8 x,uint8 y) //��ʾ -00.00~+00.00 �ֺ� 6x8
{
    uint8 vol[6] = {0};
    uint16 temp = 0;
    if(i < 0){
      vol[0] = '-';
      temp = (uint16)(-i*100);
    }
    else{
      vol[0] = '+';
      temp = (uint16)(i*100);
    }
    vol[1] = temp / 1000 + 0x30;
    vol[2] = temp % 1000 / 100 + 0x30;
    vol[3] = '.';
    vol[4] = temp % 100 / 10 + 0x30;
    vol[5] = temp % 10  + 0x30;  
    LCD_P6x8Str( x, y, vol);
}
//==============================================================
//�������� void display_image(byte x,byte y,byte X,byte Y,uint8 image[])
//������������ʾBMPͼƬ60��80
//������
//ע�����OLEDÿ����ʾ8�����ص㣬��һ��8�����ڻ�ͼ��ʱ����Ҫ����תΪһ���ֽڣ�������OLED��ʾ
//���أ���
//==============================================================
void display_image(uint8 image[60][80])
{ 
  uint8 i,j;
    for(i=0; i<=6; i++)//7��ѭ�����56*80��ͼ����
        {
            LCD_Set_Pos(0,i);
            for(j=0; j<80; j++)//һ��ѭ�����һ��8*60��ͼ��
            {
                uint8 x = (image[i*8][j]/255)
                    +(image[i*8+1][j])/255*2
                        +(image[i*8+2][j])/255*4
                            +(image[i*8+3][j])/255*8
                                +(image[i*8+4][j])/255*16
                                    +(image[i*8+5][j])/255*32
                                        +(image[i*8+6][j])/255*64
                                            +(image[i*8+7][j])/255*128;//�����ŵ�8�����غϲ���һ���ֽ�
                LCD_WrDat(x);
            }
        }
        //�����һ�γ��������56�����ص���ʾ��ͬ��������ʾʣ��4��
        LCD_Set_Pos(0,7);
        for(j=0; j<80; j++)
        {
            uint8 x = (image[56][j])/255
                +image[57][j]/255*2
                    +image[58][j]/255*4
                        +image[59][j]/255*8;
            LCD_WrDat(x);
        }
}