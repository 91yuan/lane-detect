#if !defined TEST
#define TEST

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include "linefit.h"

#define offsetX 0
#define offsetY 100
//offsetY = the first row of the ROI in the image

using namespace std;
using namespace cv;

void printLine(vector<lineProperty> linesChosed)
{
	vector<lineProperty>::const_iterator it2=linesChosed.begin();
	while(it2!=linesChosed.end())
	{
		cout<<"("<<(*it2).buttom.x<<","<<(*it2).buttom.y<<")"<<" ";
		cout<<"("<<(*it2).top.x<<","<<(*it2).top.y<<")"<<" ";
		cout<<"k="<<(*it2).k<<" ";
		cout<<"weight="<<(*it2).weight<<endl;	
		++it2;
	}
}
/*
void printCenter(lineProperty line)
{
	cout<<"("<<line.pt1.x<<","<<line.pt1.y<<")"<<" ";
	cout<<"("<<line.pt2.x<<","<<line.pt2.y<<")"<<" ";
	cout<<"k="<<line.k<<endl;
}
*/
//bule:41,240,110; red=82,90,240; green=145,54,34
void drawDetectedLines(Mat y,Mat cr,Mat cb, vector<lineProperty> linesChosed)
	{
		float deltay,deltax;
		std::vector<lineProperty>::iterator it2=linesChosed.begin();
		while(it2!=linesChosed.end())
		{
			cv::Point pt1=(*it2).buttom;
			cv::Point pt2=(*it2).top;
			pt1.x=pt1.x+offsetX;
			pt1.y=pt1.y+offsetY;
			pt2.x=pt2.x+offsetX;
			pt2.y=pt2.y+offsetY;
			cv::line(y,pt1,pt2,Scalar(145),2);
			cv::line(cr,pt1,pt2,Scalar(54),2);
			cv::line(cb,pt1,pt2,Scalar(34),2);
			++it2;

		}
	}

void drawDirection(Mat y,Mat cr,Mat cb, lineProperty centerLine)
{
	cv::Point pt1(centerLine.buttom.x+offsetX, centerLine.buttom.y+offsetY);
	cv::Point pt2(centerLine.top.x+offsetX, centerLine.top.y+offsetY);
	
//	cv::circle(image, pt1 ,3 , CV_RGB(255,0,0),-1);
//	cv::circle(image, pt2 ,3 , CV_RGB(255,0,0),-1);
//	cv::line(image,pt1,pt2,CV_RGB(255,0,0));
	cv::line(y,pt1,pt2,Scalar(82),2);
	cv::line(cr,pt1,pt2,Scalar(90),2);
	cv::line(cb,pt1,pt2,Scalar(240),2);
}

void drawDetectRegion(Mat y, vector<lineProperty> linesChosed, lineProperty centerLine)
{
	int bx,by,tx,ty;
	float k;

	int height=y.rows;
	int i=centerLine.weight;
	bx=linesChosed[i].buttom.x+offsetX; 
	by=linesChosed[i].buttom.y+offsetY;
	cv::Point pt1(bx,by);

	bx=linesChosed[i].top.x+offsetX; 
	by=linesChosed[i].top.y+offsetY;
	cv::Point pt2(bx,by);

	bx=linesChosed[i-1].buttom.x+offsetX; 
	by=linesChosed[i-1].buttom.y+offsetY;
	cv::Point pt4(bx,by);

	bx=linesChosed[i-1].top.x+offsetX; 
	by=linesChosed[i-1].top.y+offsetY;
	cv::Point pt3(bx,by);

	cv::line(y,pt1,pt2,Scalar(82),2);
	cv::line(y,pt2,pt3,Scalar(82),2);
	cv::line(y,pt3,pt4,Scalar(82),2);
	cv::line(y,pt4,pt1,Scalar(82),2);
}

#endif
