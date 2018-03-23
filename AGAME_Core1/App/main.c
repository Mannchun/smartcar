#include "common.h"
#include "include.h"
#include "switch.h"

#define LED		PTE1      	//LED灯
#define LCD_flag	1

typedef enum
{
  MYKEY_NONE = 0,
  MYKEY_LEFT = 1,
  MYKEY_RIGHT = 2,
  MYKEY_UP = 3,
  MYKEY_DOWN = 4,
  MYKEY_MID = 5,
}MYKEY_e;

extern Pid pid;
extern int line[60];
uint16 outLine = 2;
uint16 LightNum = 0;

uint8 stopFlag = 1;
uint8 BACKFLAG = 1;
uint8 stopPro = 0;
uint8 voidBar = 1;

#define Center	163//163  复制在interupt.c中
#define Left	Center-30//复制在interupt.c中
#define Right	Center+30//复制在interupt.c中
#define CofIMG  39	//正前方信标所在列

static uint16 F_flag = 0;//找到发光信标
static uint16 C_flag = 0;//接近发光信标
uint16 cutOver_far = 10;//侧向切入值
uint16 cutOver_near = 17;
uint16 cutOver_bar = 22;
uint16 cutChangePoint = 30;
uint16 slowPoint = 35;
uint16 turnPoint = 45;
uint16 _count = 0;
uint8 LeftFlag = 1;
int BeginFlag = 1;
int BarFlag = 0;
int LockFlag = 0;

int DIP_swt = 0;
int enter_flag = 0;
int exit_flag = 0;

float higherSpeed = 4.0;//5.2
float lowerSpeed = 2.5;//2.6
float turnSpeed = 2.3;//2.0
uint8 theBar[2] = {0,0};

int key_scan();
void set_speed_mode();
void set_cutOver_mode();
void set_flag_mode();
void DisplayLED();


void  main(void)
{   //五向开关 初始化
    gpio_init (PTB0, GPI, 1);             //引脚初始化
    gpio_init (PTB3, GPI, 1);             //引脚初始化
    gpio_init (PTB2, GPI, 1);             //引脚初始化
    gpio_init (PTB9, GPI, 1);             //引脚初始化
    gpio_init (PTB1, GPI, 1);             //引脚初始化
    
    //拨码开关 初始化
    gpio_init (PTD4, GPI, 1);             //引脚初始化
    gpio_init (PTD5, GPI, 1);             //引脚初始化
    gpio_init (PTD6, GPI, 1);             //引脚初始化
    
    gpio_init (LED, GPO, 0);                                 //LED灯初始化
    LCD_Init();                                              //LCD显示屏初始化
    PID_init(&pid);                                          //PID初始化
    camera_init(imgbuff);                                    //摄像头初始化
    
    //配置中断服务函数----摄像头中断
    set_vector_handler(PORTC_VECTORn , PORTC_IRQHandler);    //设置 PORTC 的中断服务函数为 PORTC_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);      //设置 DMA0 的中断服务函数为 DMA0_IRQHandler
    
    DIP_swt = DIP_Switch_read();
    switch(DIP_swt)
    {
	case 0:
 	    higherSpeed = 2.5;
	    lowerSpeed = 2.5;
	    turnSpeed = 2.3;
	    slowPoint = 35; 
	    break;
	case 1:
	    higherSpeed = 3.7;
	    lowerSpeed = 2.5;
	    turnSpeed = 2.0;
	    slowPoint = 35; 
	    break;
	case 2:
	    higherSpeed = 4.5;
	    lowerSpeed = 2.5;
	    turnSpeed = 2.3;
	    slowPoint = 32;
	    break;
	case 3:
            higherSpeed = 5.0;
	    lowerSpeed = 2.5;
	    turnSpeed = 2.3;
	    slowPoint = 31;
	    break;
    }
    if(gpio_get(PTD6) == 1) enter_flag = 1;
    
    while(enter_flag == 0)
    {
        byte strin[]= "modeMenu";
        LCD_P6x8Str( 0, 0, strin);
        switch(key_scan())
        {
	    case MYKEY_LEFT: set_speed_mode();break;   //left
	    case MYKEY_UP:   set_flag_mode();break;      //up
	    case MYKEY_RIGHT:DisplayLED();break;      //right
	    case MYKEY_MID:  enter_flag = 1;break;      //mid
	    case MYKEY_DOWN: set_cutOver_mode();break; //down
	    default:break;
        }
	exit_flag = 0;
	if(gpio_get(PTD7) == 1){
	    gpio_set(LED, 0);
	    voidBar = 1;}
	else{
	    gpio_set(LED, 1);
	    voidBar = 0;
	}
    }
    
     //电机 舵机 初始化
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ, Center);      //初始化 舵机 PWM ( 130 ,160 ,190)   
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM, MOTOR_HZ, 0);        //初始化 电机 PWM驱动板左1 负  千万不要搞错
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM, MOTOR_HZ, 0);        //初始化 电机 PWM驱动板左2 正
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM, MOTOR_HZ, 0);        //初始化 电机 PWM驱动板右1 负
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM, MOTOR_HZ, 0);        //初始化 电机 PWM驱动板有2 正
                                                             //LPT0_ALT2-----C5右边电机
    ftm_quad_init(FTM2);         			     //初始化编码器
    
    //配置中断服务函数----PIT定时器中断
   // pit_init_ms(PIT1, 2);                                  //初始化PIT0，定时时间为： 2ms
   // set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);     //设置PIT0的中断复位函数为 PIT0_IRQHandler
   // enable_irq (PIT1_IRQn);                                //使能PIT0中断
    
    //配置中断服务函数----PIT定时器中断
    pit_init_ms(PIT0, 10);                                   //初始化PIT0，定时时间为： 1000ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);       //设置PIT0的中断复位函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);                                  //使能PIT0中断
    
    //配置中断服务函数----串口中断
    uart_init (VCAN_PORT, VCAN_BAUD);                           //蓝牙初始化，
    //set_vector_handler(UART4_RX_TX_VECTORn,uart4_handler);   //设置 uart4 的中断服务函数为 uart3_handler
    //uart_rx_irq_en(UART4); 				     //开串口接收中断

    uart_init (CHIP_PORT, CHIP_BAUD);
    set_vector_handler(UART3_RX_TX_VECTORn,uart3_handler);   //设置 uart3 的中断服务函数为 uart3_handler
    if(voidBar == 1)    
	uart_rx_irq_en(CHIP_PORT); 				     //开串口接收中断
    
    #if LCD_flag 
	byte str1[] = "v-speed :";
	byte str2[] = "x       :";
	byte str3[] = "y       :";
	byte str4[] = "theBarX :";
	byte str5[] = "mappedY :";
	LCD_P6x8Str( 0, 0, str1);
	LCD_P6x8Str( 0, 1, str2);
	LCD_P6x8Str( 0, 2, str3);
	LCD_P6x8Str( 0, 3, str4);
	LCD_P6x8Str( 0, 4, str5);
    #endif      
    uint16 ledcount = 0;
    for(int i=0; i<60; ++i){
        if(line[i] != 0)
	    line[i] += outLine;
    }

    while(1)
    {
        ledcount++;
	if(ledcount == 20){
	   gpio_turn(LED);
	   ledcount = 0;
	}
        camera_get_img();                        	//摄像头获取图像
        //vcan_sendimg(imgbuff, sizeof(imgbuff));    	//发送图像
      /*uart_putn(UART4,n);
        uart_putchar (VCAN_PORT, ',');
        uart_putnum(UART4,pid.real_speed/100.0);
        uart_putchar (VCAN_PORT, ',');
        uart_putnum(UART4,centerY);
        uart_putchar (VCAN_PORT, ',');
        uart_putstr(UART4, "\r");	//发送字符串，已接收指令
        uart_putstr(UART4, "\n");	//发送字符串，已接收指令  
        */
	
      if (scanbeacon() == 1 && (BeginFlag==1 || stopFlag == 0))
         {
	      stopFlag = 0;
	      F_flag = 1;
	      if(BeginFlag == 1){
		  BeginFlag = 0;
	      }
	      if(centerX < CofIMG){ 
		  centerX += (centerY>cutChangePoint)?cutOver_near:cutOver_far; 
	      }
	      else if(centerX >= CofIMG){
		  centerX -= (centerY>cutChangePoint)?cutOver_near:cutOver_far; //设置侧向切入值
	      }
	      /////避障/////
	      if(theBar[0]>0&&theBar[0]<=80&&theBar[1]>5&&theBar[1]<=60){//障碍数据正确
	          if(theBar[1] - centerY >= 3)//信标前面有障碍
		  {
		      if((centerX>=(40-line[centerY]) && centerX <=(40+line[centerY])) || BarFlag == 1){
			  LockFlag = 1;
			  BarFlag = 1;
		          centerX = theBar[0];
			  if(centerX < 40){ 
			      centerX += (centerY>36)?cutOver_near:cutOver_bar; 
			      LeftFlag = 1;
			  }
			  else if(centerX >= 40){
			      centerX -= (centerY>36)?cutOver_near:cutOver_bar; //设置侧向切入值
			      LeftFlag = 0;
			  }
			  LockFlag = 0;
		      }
		      if(theBar[1] > 54) BarFlag = 0;
		  }
	      }
	      /////避障////
              if(centerY <= turnPoint){			//若信标灯较远
                  if(centerX>CofIMG && centerX<80)
                  {
                      float k1 = (centerX-CofIMG)*0.8;
                      ftm_pwm_duty(S3010_FTM, S3010_CH,(uint16)(Center+k1));//舵机右转
                  }
                  else if(centerX<CofIMG && centerX>0)
                  {
                      float k1 = (-centerX+CofIMG)*0.8;
                      ftm_pwm_duty(S3010_FTM, S3010_CH,(uint16)(Center-k1));//舵机左转
                  }
                  pid.set_speed = higherSpeed*100;	//直道速度
              }
              if(centerY>slowPoint && centerY<=60)	//若信标灯较近
	      {
		  pid.set_speed = lowerSpeed*100; 	//减速速度
		  C_flag = 1;
	      }
	      if(turnPoint <= centerY){			//若已经灭灯
		  if(centerX < CofIMG)			//左侧切入则左转
		  {
		      LeftFlag = 1;
		  }
		  else if(centerX >= CofIMG)		//右侧切入则右转
		  {
		      LeftFlag = 0;
		  }
	      }
          }
          else
          {
	      if(F_flag==1 && C_flag==1){
		  F_flag = C_flag = 0;
		  ++_count;
	      }
	      if(LightNum!=0 && _count==LightNum){
	          stopFlag = 1;
		  pid.set_speed = 0.0;
	      }
	      /////无信标时避障/////
	      /*if(theBar[0]!=0&&theBar[1]!=0&&theBar[0]>=1&&theBar[0]<=80&&theBar[1]>43&&theBar[1]<60){//障碍很近
		  if(theBar[0] < CofIMG){ 
		      ftm_pwm_duty(S3010_FTM, S3010_CH, Right);
		  }
		  else if(theBar[0] >= CofIMG){
		      ftm_pwm_duty(S3010_FTM, S3010_CH, Left);
		  }
	      }*/
	      /////无信标时避障////
	      else{
		  pid.set_speed = turnSpeed*100;
		  if(BeginFlag == 1)
		      ftm_pwm_duty(S3010_FTM, S3010_CH, Center);
		  else if(LeftFlag == 1 && stopFlag == 0)
		      ftm_pwm_duty(S3010_FTM, S3010_CH, Left);
		  else if(LeftFlag == 0 && stopFlag == 0)
		      ftm_pwm_duty(S3010_FTM, S3010_CH, Right);
	      }
          }
      
      
        #if LCD_flag 
            float vdisplay = pid.real_speed/100.0;
            displayfloatNum6x8(vdisplay,75,0);
            displayUintNum6x8(centerX,75,1);
            displayUintNum6x8(centerY,75,2);
	    displayUintNum6x8(theBar[0],75,3);
	    displayUintNum6x8(theBar[1],75,4);
        #endif
    }
}

/*
下   PTB0
右   PTB3
上   PTB2
中   PTB9
左   PTB1
*/

int key_scan()
{
  static char preKey = 0;
  static char sameKey = 0;
  MYKEY_e key = MYKEY_NONE;
  
  if(gpio_get(PTB9) == 0)   // 下 按? ??
    key = MYKEY_MID;
  else if(gpio_get(PTB0) == 0)   // 下 按? ??
    key = MYKEY_RIGHT;
  else if(gpio_get(PTB3) == 0)   // 下 按? ??
    key = MYKEY_UP;
  else if(gpio_get(PTB2) == 0)   // 下 按? ??
    key = MYKEY_LEFT;
  else if(gpio_get(PTB1) == 0)   // 下 按? ??
    key = MYKEY_DOWN;
  systick_delay_ms(50);
  if(key == preKey)
  {
    if(sameKey == 0)
    {
      sameKey = 1;
      return key;
    }
  }
  else sameKey = 0;
  preKey = key;
  return 0;
}

void set_speed_mode()
{
  LCD_CLS();
  float *select = &higherSpeed;
  uint8 count = 0;
  while(exit_flag == 0)
  {
    switch(count)
    {
	case 0:select = &higherSpeed;break;
	case 1:select = &lowerSpeed;break;
	case 2:select = &turnSpeed;break;
	default:break;
    }
    switch(key_scan())
    {
	case MYKEY_DOWN: *select -= 0.1f;break;
	case MYKEY_UP:   *select += 0.1f;break;
	case MYKEY_RIGHT:count = (count+1)%3;break;
    	case MYKEY_LEFT: count = (count==0)?3:count;count = (count-1)%3;break;
	case MYKEY_MID:  exit_flag =1;break;
	default:break;
    }
    byte mode_str0[] = ">";
    byte mode_str1[] = " higherSpeed";
    byte mode_str2[] = " lowerSpeed";
    byte mode_str3[] = " turnSpeed";
    LCD_P6x8Str( 0, 0, mode_str1);
    LCD_P6x8Str( 0, 1, mode_str2);
    LCD_P6x8Str( 0, 2, mode_str3);
    LCD_P6x8Str( 0, count, mode_str0);
    displayfloatNum6x8((float)(higherSpeed),75,0);
    displayfloatNum6x8((float)(lowerSpeed),75,1);
    displayfloatNum6x8((float)(turnSpeed),75,2);
  }
  LCD_CLS();
}

void set_cutOver_mode()
{
  uint16 *select = &LightNum;
  uint8 count = 0;
  LCD_CLS();
  while(exit_flag == 0)
  {
    switch(count)
    {
        case 0:select = &LightNum;break;
	case 1:select = &slowPoint;break;
	case 2:select = &turnPoint;break;
	case 3:select = &cutOver_far;break;
	case 4:select = &cutOver_near;break;
	case 5:select = &cutOver_bar;break;
    	case 6:select = &outLine;break;
	
	default:break;
    }
    switch(key_scan())
    {
	case MYKEY_DOWN:*select -= 1;break;
	case MYKEY_UP:  *select += 1;break;
	case MYKEY_RIGHT:count = (count+1)%7;break;
	case MYKEY_LEFT:count = (count==0)?7:count;count = (count-1)%7;break;
	case MYKEY_MID: exit_flag =1;break;
	default:break;
    }
    byte mode_str0[] = ">";
    byte mode_str1[] = " LightNum";
    byte mode_str2[] = " slowPoint";
    byte mode_str3[] = " turnPoint";
    byte mode_str4[] = " cutFar";
    byte mode_str5[] = " cutNear";
    byte mode_str6[] = " cutBar";
    byte mode_str7[] = " outLine";
    LCD_P6x8Str( 0, 0, mode_str1);
    LCD_P6x8Str( 0, 1, mode_str2);
    LCD_P6x8Str( 0, 2, mode_str3);
    LCD_P6x8Str( 0, 3, mode_str4);
    LCD_P6x8Str( 0, 4, mode_str5);
    LCD_P6x8Str( 0, 5, mode_str6);
    LCD_P6x8Str( 0, 6, mode_str7);
    LCD_P6x8Str( 0, count, mode_str0);
    displayUintNum6x8(LightNum,75,0);
    displayUintNum6x8(slowPoint,75,1);
    displayUintNum6x8(turnPoint,75,2);
    displayUintNum6x8(cutOver_far,75,3);
    displayUintNum6x8(cutOver_near,75,4);
    displayUintNum6x8(cutOver_bar,75,5);
    displayUintNum6x8(outLine,75,6);
  }
  LCD_CLS();
}

void set_flag_mode()
{
  uint8 *select = &stopPro;
  uint8 count = 0;
  LCD_CLS();
  while(exit_flag == 0)
  {
    switch(count)
    {
	case 0:select = &stopPro;break;
	case 1:select = &voidBar;break;
	case 2:select = &BACKFLAG;break;
        case 3:select = &stopFlag;break;
	default:break;
    }
    switch(key_scan())
    {
	case MYKEY_DOWN:*select = (*select+1)%2;break;
	case MYKEY_UP:  *select = (*select+1)%2;break;
	case MYKEY_RIGHT:count = (count+1)%4;break;
	case MYKEY_LEFT:count = (count==0)?4:count;count = (count-1)%4;break;
	case MYKEY_MID: exit_flag =1;break;
	default:break;
    }
    byte mode_str0[] = ">";
    byte mode_str1[] = " stopPro";
    byte mode_str2[] = " voidBar";
    byte mode_str3[] = " BACKFLAG";
    byte mode_str4[] = " stopFlag";
    LCD_P6x8Str( 0, 0, mode_str1);
    LCD_P6x8Str( 0, 1, mode_str2);
    LCD_P6x8Str( 0, 2, mode_str3);
    LCD_P6x8Str( 0, 3, mode_str4);
    LCD_P6x8Str( 0, count, mode_str0);
    displayfloatNum6x8((float)(stopPro),75,0);
    displayfloatNum6x8((float)(voidBar),75,1);
    displayfloatNum6x8((float)(BACKFLAG),75,2);
    displayfloatNum6x8((float)(stopFlag),75,3);
  }
  LCD_CLS();
}
 


void DisplayLED()
{
  LCD_CLS();
  while(exit_flag == 0)
  {
    switch(key_scan())
    {
	case MYKEY_MID: exit_flag =1;break;
	default:break;
    }
    camera_get_img();
    img_extract(img, imgbuff, CAMERA_SIZE);
    display_image(img);
  }
  LCD_CLS();
}
