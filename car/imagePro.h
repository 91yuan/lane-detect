#if !defined IMAGEPRO
#define IMAGEPRO

#include <opencv/highgui.h>
#include <iostream>
#include <opencv/cv.h>
#include "linefinder.h"
#include "test.h"
#include "linefit.h"
#include "color.h"
#include "car_move.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

vector<lineProperty> linesBefore;
lineProperty centerLine;

void ProcessImage(Mat Y, Mat Cr,Mat Cb, int *move)
{
	//define the ROI
	cv::Mat Cri=Cr(cv::Rect(0,100,639,379));
	cv::Mat Cbi=Cb(cv::Rect(0,100,639,379));
	cv::Mat Yi =Y(cv::Rect(0,100,639,379));
	cout<<"size: "<<linesBefore.size()<<endl;
	imshow("Cr",Cri);
	imshow("Cb",Cbi);
	if(linesBefore.size()>1)
	{	
		drawDetectRegion(Y,linesBefore,centerLine);
		cv::Mat road=colorDetect(Cri,Cbi);
		imshow("color",road);
		int obs=myMask(road,centerLine,linesBefore);
		if(obs>4)
		{
			move[0]=0;
			move[1]=0;
			cout<<"obstacle"<<endl;
			cout<<endl;
			return ;
		}
	}

	//use canny the detect contours
	medianBlur(Yi,Yi,9);
	GaussianBlur(Yi,Yi,Size(5,5),1.5);
	Mat result;
//	imshow("Y",Yi);
	Canny(Yi,result,125,310);
//	imshow("Canny",result);
	//use probabilistic Hough to detect lines
	LineFinder finder;
	finder.setLineLengthAndGap(20,10);
	finder.setMinVote(10);
	vector<cv::Vec4i>lines=finder.findLines(result);	
	
	linesBefore.clear();
	vector<lineProperty> linesChosed=chooseLines(lines);
	linesBefore=mergeLines(linesChosed,linesBefore);
//		cout<<"draw1"<<endl;
//	cout<<"chosed="<<linesChosed.size()<<endl;
	drawDetectedLines(Y,Cr,Cb,linesBefore);
//	printLine(linesBefore);
//	cout<<"findline"<<endl;
	centerLine=findDirection(&linesBefore);
	printLine(linesBefore);
//		cout<<"draw2"<<endl;
	drawDirection(Y,Cr,Cb, centerLine);
//	cv::imwrite("1.bmp",drawImage);	
	move[0]=7;
	move[1]=setMove(atan(centerLine.k));
	cout<<endl;
	return;
}
/*
int[] ProcessImage(Mat Y,Mat Cr,Mat Cb, int[] *)
{
	//region of interest
/*	cv::Mat ROI;
	vector<cv::Mat>planes;
	cv::cvtColor(image,ROI,CV_BGR2YCrCb);
	cv::split(ROI,planes);
	
	return lineDetect(Y,Cr,Cb);
	
}
*/


#endif
