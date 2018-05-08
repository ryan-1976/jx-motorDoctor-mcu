//http://www.stmcu.org/module/forum/forum.php?mod=viewthread&tid=600975&page=1
//http://blog.sina.com.cn/s/blog_56e19aa70101blq5.html
//https://www.cnblogs.com/menlsh/p/4154070.html
 #include <stdint.h>
 #include <math.h>
 #include "stm32_dsp.h"
//uint32_t input[1024], output[1024], Mag[1024]; 

//void PowerMag(uint16_t _usFFTPoints)
//{
//	int16_t lX,lY;
//	uint16_t i;
//	float32_t mag;


//	for (i=0; i < _usFFTPoints; i++)
//	{
//	  lX= (output[i]<<16)>>16;          
//	lY= (output[i]>> 16);               
//	mag = __sqrtf(lX*lX+ lY*lY);      
//	Mag[i]= mag*2;                    
//	}


//	Mag[0] = Mag[0]>>1;
//}


//void DSP_FFT1024(void)
//{
//uint16_t i;

//for (i = 0; i < 1024; i++)
//{
//input[i] = 0;

//input[i] = 1024 + 1024*sin(2*3.1415926f*50*i/1000) + 512*sin(2*3.1415926f*20*i/1000) ;
//}

//cr4_fft_1024_stm32(output, input, 1024); 

//PowerMag(1024);

//for (i = 0; i < 1024; i++)
//{
//printf("%d\r\n", Mag[i]);
//}
//}

#define NPT  1024
#define PI2  6.28318530717959
#define Fs   44800
uint32_t lBufOutArray[1024], lBufInArray[1024], lBufMagArray[1024]; 

void GetPowerMag()
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}
void InitBufInArray()
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 1500 * sin(PI2 * i * 350.0 / Fs) +
             2700 * sin(PI2 * i * 8400.0 / Fs) +
             4000 * sin(PI2 * i * 18725.0 / Fs);
        lBufInArray[i] = ((signed short)fx) << 16;
    }
}
void DSP_FFT1024(void)
{
	InitBufInArray();
	cr4_fft_1024_stm32(lBufOutArray, lBufInArray, 1024); 
	GetPowerMag();
}
