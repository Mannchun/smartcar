#ifndef	_LCD_H_
#define _LCD_H_


#define DC     PTB21_OUT
#define RESET  PTB17_OUT
#define D1     PTB16_OUT
#define D0     PTB11_OUT

 typedef unsigned char byte;
 typedef unsigned int word;
 extern byte beyond96x64[512];
 
 void LCD_WrDat(uint8 data);                    //LCDҺ����ʾд��1���ֽ�����
 void LCD_Set_Pos(byte x, byte y);              //д��ָ��
 void LCD_Init(void);                           //LCD��ʼ��
 void LCD_CLS(void);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);     //д��һ���׼ASCII�ַ���
 void LCD_P8x16Str(byte x,byte y,byte ch[]);
 void LCD_P14x16Str(byte x,byte y,byte ch[]);
 void LCD_Print(byte x, byte y, byte ch[]);
 void LCD_PutPixel(byte x,byte y);              //����һ���㣨x,y��
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
 void LCD_Fill(byte dat);
 void Display(int16 i,uint8 x,uint8 y);
 void DisplayFloat(int16 i,uint8 x,uint8 y);

#endif