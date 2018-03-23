#include "common.h"
#include "VCAN_camera.h"
uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W];

#define Buzzer  PTA5 //main also defined

//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
//解压的时候，里面有个数组，配置黑、白对应的值是多少。
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {0, 255}; //0 和 1 分别对应的颜色
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    //注：0 表示 白色，1表示 黑色
    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *msrc++;
        *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

extern int whiteFlag;
int whiteband(uint8 img[CAMERA_H][CAMERA_W]){
    for(int i=0; i<8; ++i){
        int white = 1;
	for(int j=0; j<CAMERA_H; ++j){
	    if(img[j][i] != 0){
	        white = 0;
		break;
	    };
	}
	if(white == 1) return 1;
    }
    return 0;
}


//255为白色（背景），0为黑色（障碍）；
int barrierDetection(int detectRange[60][2], Barrier *Bar, int *start)//第一次调用start为59
{
    img_extract(img, imgbuff, CAMERA_SIZE);//图像解压为二维数组
    if(whiteFlag==1 && whiteband(img)){
        gpio_set(Buzzer, 1);
	return 0;
    }
    else gpio_set(Buzzer, 0);
    int8 done = 0;
    for(int8 i=*start; i>=0; --i){
        int8 k=(detectRange[i][0]+detectRange[i][1])/2;
        for(int8 j=0; j<(detectRange[i][1]-detectRange[i][0]+1)/2; ++j){
            if(img[i][k+j] == 0){
                Bar->dEdge = Bar->uEdge = i;
                Bar->lEdge = Bar->rEdge = k+j;
                done = 1;
		*start = i;
                break;
            }
            if(img[i][k-j] == 0){
                Bar->dEdge = Bar->uEdge = i;
                Bar->lEdge = Bar->rEdge = k-j;
                done = 1;
		*start = i;
                break;
            }
        }
        if(done == 1) break;
    }
    if(done != 1) return done;
    
    for(int i=Bar->dEdge; i>= Bar->uEdge; --i){
      	if(i<0) break;//到达图像上界退出
        while(Bar->lEdge>detectRange[i][0] && img[i][Bar->lEdge-1]==0){
            --(Bar->lEdge);
        }
        while(Bar->rEdge<detectRange[i][1] && img[i][Bar->rEdge+1]==0){
            ++(Bar->rEdge);
        }
        for(int j=Bar->lEdge; j<=Bar->rEdge; ++j){
            if(img[Bar->uEdge-1][j] == 0){
                --(Bar->uEdge);
                break;
            }
        }
    }
    ++(Bar->lEdge);
    ++(Bar->rEdge);
    ++(Bar->uEdge);
    ++(Bar->dEdge);
    Bar->centerX = (Bar->lEdge + Bar->rEdge)/2;
    Bar->centerY = (Bar->uEdge + Bar->dEdge)/2;
    return done;
}

