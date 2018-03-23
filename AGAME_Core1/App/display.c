/* OLED界面显示 */
#include "common.h"
#include "display.h"
#include "include.h"

void displayUintNum8x16(uint16 i,uint8 x,uint8 y)  //显示0~65535 字号 8x16
{
    uint8 vol[5] = {0};
    vol[0] = i / 10000 + 0x30;
    vol[1] = i % 10000 / 1000 + 0x30;
    vol[2] = i % 1000 / 100 + 0x30;
    vol[3] = i % 100 / 10  + 0x30;  
    vol[4] = i % 10  + 0x30;  
    LCD_P8x16Str( x, y, vol);
}

void displayintNum8x16(int16 i,uint8 x,uint8 y)  //显示-32767~32768 字号 8x16
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

void displayUintNum6x8(uint16 i,uint8 x,uint8 y)  //显示0~65535 字号 6x8
{
    uint8 vol[5] = {0};
    vol[0] = i / 10000 + 0x30;
    vol[1] = i % 10000 / 1000 + 0x30;
    vol[2] = i % 1000 / 100 + 0x30;
    vol[3] = i % 100 / 10  + 0x30;  
    vol[4] = i % 10  + 0x30;  
    LCD_P6x8Str( x, y, vol);
}

void displayintNum6x8(int16 i,uint8 x,uint8 y)  //显示-32767~32768 字号 6x8
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
void displayUfloatNum8x16(float i,uint8 x,uint8 y) //显示 00.00 字号 8x16
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
void displayfloatNum8x16(float i,uint8 x,uint8 y) //显示 -00.00~+00.00 字号 8x16
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
void displayUfloatNum6x8(float i,uint8 x,uint8 y) //显示 00.00 字号 6x8
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
void displayfloatNum6x8(float i,uint8 x,uint8 y) //显示 -00.00~+00.00 字号 6x8
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
//函数名： void display_image(byte x,byte y,byte X,byte Y,uint8 image[])
//功能描述：显示BMP图片60×80
//参数：
//注意事项：OLED每次显示8个像素点，即一列8个，在画图的时候需要将其转为一个字节，在送入OLED显示
//返回：无
//==============================================================
void display_image(uint8 image[60][80])
{ 
  uint8 i,j;
    for(i=0; i<=6; i++)//7次循环完成56*80的图像区
        {
            LCD_Set_Pos(0,i);
            for(j=0; j<80; j++)//一次循环完成一个8*60的图像
            {
                uint8 x = (image[i*8][j]/255)
                    +(image[i*8+1][j])/255*2
                        +(image[i*8+2][j])/255*4
                            +(image[i*8+3][j])/255*8
                                +(image[i*8+4][j])/255*16
                                    +(image[i*8+5][j])/255*32
                                        +(image[i*8+6][j])/255*64
                                            +(image[i*8+7][j])/255*128;//将竖着的8个像素合并成一个字节
                LCD_WrDat(x);
            }
        }
        //上面的一段程序完成了56行像素的显示，同理，下面显示剩下4行
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