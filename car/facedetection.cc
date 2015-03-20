#include "opencv2/opencv.hpp"
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#include "imagePro.h"
#include "car_move.h"

using namespace std;
using namespace cv;
unsigned char y_buffer[640*480],Cr_buffer[640*480],Cb_buffer[640*480];
unsigned char *buffer;
pid_t ppid;

int convert_yuv_to_rgb_buffer(unsigned char *yuv_buffer_pointer,unsigned char *y_buffer,unsigned char *Cr_buffer,unsigned char *Cb_buffer)
{
    unsigned long in, out = 0;
    int yb=0,cb=0;
    int r, g, b;
    for(in = 0; in < 640 * 480 * 2; in += 4,out +=2)
    {
        y_buffer [out] = yuv_buffer_pointer[in + 0];
        Cr_buffer[out]   = yuv_buffer_pointer[in + 1];
        y_buffer [out+1] = yuv_buffer_pointer[in + 2];
		Cb_buffer[out] = yuv_buffer_pointer[in + 3];

		Cr_buffer[out+1] = Cr_buffer[out];
		Cb_buffer[out+1] = Cb_buffer[out];
    }
    return 0;
}

int convert_rgb_to_yuv_buffer(unsigned char *yuv_buffer_pointer,unsigned char *y_buffer,unsigned char *Cr_buffer,unsigned char *Cb_buffer)
{
    unsigned long out, in= 0;
    for(out = 0; out < 640 * 480 * 2; out += 4, in +=2)
    {         		
		yuv_buffer_pointer[out + 0]= y_buffer [in];
        yuv_buffer_pointer[out + 1]= Cr_buffer[in];
        yuv_buffer_pointer[out + 2]= y_buffer [in+1];
        yuv_buffer_pointer[out + 3]= Cb_buffer[in] ;
    }
    return 0;
}


void facedetection(unsigned char * buf)
{	
	convert_yuv_to_rgb_buffer(buf,y_buffer, Cr_buffer, Cb_buffer);
	Mat Yframe(480,640,CV_8UC1, y_buffer);
	Mat Crframe(480,640,CV_8UC1,Cr_buffer);
	Mat Cbframe(480,640,CV_8UC1,Cb_buffer);
 
	ProcessImage(Yframe,Crframe,Cbframe);

	convert_rgb_to_yuv_buffer(buf,Yframe.data, Crframe.data, Cbframe.data);            	
	//-- Release
	//frame_gray.release();
    Yframe.release();
	Crframe.release();
	Cbframe.release();
}

void detection(int sign_no)
{
	//printf("song1\n");
	facedetection(buffer);	
	//kill(ppid,SIGUSR2);
	//printf("song2\n");
}

int main()
{
	key_t key;
	int shmid;

//	if( !face_cascade.load( face_cascade_name ) ){
//            printf("load wrong!\n");
//	    exit(1);
//        }
	//sleep(1);
	key=ftok("/tmp/a",'a');
	shmid=shmget(key,640*480*3,IPC_CREAT|0666);
	if(shmid<0)
		{
			printf("get share memory wrong!\n");
			exit(0);
		}
	buffer =(unsigned char *)shmat(shmid,0,0);
		if(buffer==NULL)
		{
			printf("map share memory wrong!\n");
			exit(0);
		}
	ppid = getppid();
	//printf("ppid:%d\n",ppid);
	signal(SIGUSR1,detection);
	while(1)
	{
	  pause();
	}
	return 0;
}

