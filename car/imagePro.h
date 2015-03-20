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

vector<lineProperty> linesBefore;

void lineDetect(Mat Y, Mat Cr,Mat Cb)
{
	//use canny the detect contours
	Mat result;
	Canny(Y,result,125,310);

	//use probabilistic Hough to detect lines
	LineFinder finder;
	finder.setLineLengthAndGap(15,20);
	finder.setMinVote(10);
	vector<cv::Vec4i>lines=finder.findLines(result);	
	
//	cout<<"before="<<" "<<linesBefore.size()<<" ";
	vector<lineProperty> linesChosed=chooseLines(lines);
	linesBefore=mergeLines(linesChosed,linesBefore);
	
//	cout<<"chosed="<<linesChosed.size()<<endl;
	drawDetectedLines(Y,Cr,Cb,linesBefore);
/*	printLine(linesBefore);
	
	lineProperty centerLine=findDirection(linesBefore);
	drawDirection(drawImage, centerLine);
//	cv::imwrite("1.bmp",drawImage);	
//	drawDetectRegion(drawImage,linesBefore,centerLine);
	cv::Mat road=colorDetect(Cr,Cb);
	int obs=myMask(road,centerLine,linesBefore);
	if(obs>2)
		drawDetectRegion(drawImage,linesBefore,centerLine);
*/
}

void ProcessImage(Mat Y,Mat Cr,Mat Cb)
{
	//region of interest
/*	cv::Mat ROI;
	vector<cv::Mat>planes;
	cv::cvtColor(image,ROI,CV_BGR2YCrCb);
	cv::split(ROI,planes);
	cv::Mat Cr=planes[1](cv::Rect(42,220,546,30));
	cv::Mat Cb=planes[2](cv::Rect(42,220,546,30));
	cv::Mat Y =planes[0](cv::Rect(42,250,546,95));
*/	
	lineDetect(Y,Cr,Cb);
}



#endif
