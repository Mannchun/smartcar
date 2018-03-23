#include "common.h"
#include "VCAN_camera.h"
uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W];
uint8 Lnum[CAMERA_H],Rnum[CAMERA_W]; 
uint8 centerX,centerY;

//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
//解压的时候，里面有个数组，配置黑、白对应的值是多少。
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {0, 255}; //0 和 1 分别对应的颜色
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
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


#define upper_bound 17
#define lower_bound 59

int scanbeacon()
{
      centerY = upper_bound;
      centerX = 0;
      img_extract(img, imgbuff, CAMERA_SIZE);//图像解压为二维数组
      for(uint8 j=upper_bound; j<lower_bound; ++j)
      {
          Rnum[j] = 0;
          for(uint8 i=0; i<CAMERA_W; ++i)
          {
              if (img[j][i] == 0)
                  ++Rnum[j];
          }
      }
      for(uint8 j=upper_bound; j<lower_bound-2; ++j)
      {
          if((Rnum[centerY]+Rnum[centerY+1]+Rnum[centerY+2]) < (Rnum[j+1]+Rnum[j+2]+Rnum[j+3]))
          centerY = j+1;
      }

      for(uint8 i=0; i<CAMERA_W; ++i)
      {
          Lnum[i] = 0;
          for(uint8 j=upper_bound; j<lower_bound; ++j)
          {
              if (img[j][i]==0)
                  ++Lnum[i];
          }
      }
      for(uint8 j=0; j<CAMERA_W-2; ++j)
      {
          if((Lnum[centerX]+Lnum[centerX+1]+Lnum[centerX+2])<(Lnum[j+1]+Lnum[j+2]+Lnum[j+3]))
          centerX = j+1;
      }
      centerX += 1;
      img[centerY][centerX] = ~img[centerY][centerX];
      if(centerY == upper_bound && centerX == 1)
          return 0;
      else
          return 1;
}


int line[60] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 6, 6,
		 6, 6, 7, 7, 7, 8, 8,11,12,13,
		14,15,16,17,17,18,19,20,21,22,
		23,24,25,26,26,26,26,26,26,26,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};