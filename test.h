#if !defined TEST
#define TEST

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include "linefit.h"

#define offsetX 42
#define offsetY 250

using namespace std;

void printLine(vector<lineProperty> linesChosed)
{
	vector<lineProperty>::const_iterator it2=linesChosed.begin();
	while(it2!=linesChosed.end())
	{
		cout<<"("<<(*it2).pt1.x<<","<<(*it2).pt1.y<<")"<<" ";
		cout<<"("<<(*it2).pt2.x<<","<<(*it2).pt2.y<<")"<<" ";
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
void drawDetectedLines(cv::Mat &image, vector<lineProperty> linesChosed, cv::Scalar color=cv::Scalar(255,0,0))
	{
		float deltay,deltax;
		std::vector<lineProperty>::iterator it2=linesChosed.begin();
		while(it2!=linesChosed.end())
		{
			cv::Point pt1=(*it2).pt1;
			cv::Point pt2=(*it2).pt2;
			pt1.x=pt1.x+offsetX;
			pt1.y=pt1.y+offsetY;
			pt2.x=pt2.x+offsetX;
			pt2.y=pt2.y+offsetY;
			cv::line(image,pt1,pt2,color,3);
			++it2;

		}
	}

void drawDirection(cv::Mat &image, lineProperty centerLine)
{
	centerLine.pt1.x=centerLine.pt1.x+offsetX;
	centerLine.pt1.y=centerLine.pt1.y+offsetY;
	centerLine.pt2.x=centerLine.pt2.x+offsetX;
	centerLine.pt2.y=centerLine.pt2.y+offsetY;
	
	cv::circle(image, centerLine.pt1 ,3 , CV_RGB(255,0,0),-1);
	cv::circle(image, centerLine.pt2 ,3 , CV_RGB(255,0,0),-1);
	cv::line(image,centerLine.pt1,centerLine.pt2,CV_RGB(255,0,0));
}

#endif
