#if !defined IMAGEPRO
#define IMAGEPRO

#include <opencv/highgui.h>
#include <iostream>
#include <opencv/cv.h>
#include "linefinder.h"
#include "test.h"
#include "linefit.h"
#include "color.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

float lineDetect(Mat Y, Mat Cr,Mat Cb)
{
	//define the RO
//	cv::Mat Cri=Cr(cv::Rect(0,150,639,90));
//	cv::Mat Cbi=Cb(cv::Rect(0,150,639,90));
	cv::Mat Yi =Y(cv::Rect(0,100,639,379));
//	cv::Mat Yi=Y;
//	cout<<"canny"<<endl;	
	//use canny the detect contours
	medianBlur(Y,Y,9);
	GaussianBlur(Yi,Yi,Size(5,5),1.5);
	Mat result;
	imshow("Y",Yi);
	Canny(Yi,result,125,310);
	imshow("Canny",result);
	//use probabilistic Hough to detect lines
	LineFinder finder;
	finder.setLineLengthAndGap(20,10);
	finder.setMinVote(10);
	vector<cv::Vec4i>lines=finder.findLines(result);	
	
	vector<lineProperty> linesBefore;
//	cout<<"before="<<" "<<linesBefore.size()<<" ";
//	cout<<"chose"<<endl;
	vector<lineProperty> linesChosed=chooseLines(lines);
	linesBefore=mergeLines(linesChosed,linesBefore);
//		cout<<"draw1"<<endl;
//	cout<<"chosed="<<linesChosed.size()<<endl;
	drawDetectedLines(Y,Cr,Cb,linesBefore);
//	printLine(linesBefore);
//	cout<<"findline"<<endl;
	lineProperty centerLine=findDirection(linesBefore);
	printLine(linesBefore);
//		cout<<"draw2"<<endl;
	drawDirection(Y,Cr,Cb, centerLine);
//	cv::imwrite("1.bmp",drawImage);	
//	drawDetectRegion(drawImage,linesBefore,centerLine);
/*	cv::Mat road=colorDetect(Cr,Cb);
	int obs=myMask(road,centerLine,linesBefore);
	if(obs>2)
		drawDetectRegion(drawImage,linesBefore,centerLine);
*/
	return centerLine.k;
}

float ProcessImage(Mat Y,Mat Cr,Mat Cb)
{
	//region of interest
/*	cv::Mat ROI;
	vector<cv::Mat>planes;
	cv::cvtColor(image,ROI,CV_BGR2YCrCb);
	cv::split(ROI,planes);*/
	
	return lineDetect(Y,Cr,Cb);
	
}



#endif
