#if !defined TEST
#define TEST

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include "linefit.h"

#define offsetX 0
#define offsetY 240

using namespace std;

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
void drawDetectedLines(cv::Mat &image, vector<lineProperty> linesChosed, cv::Scalar color=cv::Scalar(255,0,0))
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
			cv::line(image,pt1,pt2,color,1);
			++it2;

		}
	}

void drawDirection(cv::Mat &image, lineProperty centerLine)
{
	cv::Point pt1(centerLine.buttom.x+offsetX, centerLine.buttom.y+offsetY);
	cv::Point pt2(centerLine.top.x+offsetX, centerLine.top.y+offsetY);
	
	cv::circle(image, pt1 ,3 , CV_RGB(255,0,0),-1);
	cv::circle(image, pt2 ,3 , CV_RGB(255,0,0),-1);
	cv::line(image,pt1,pt2,CV_RGB(255,0,0));
}

vector<string> ReadLines(const char* filename)
{
  vector<string> images;

  ifstream  file;
  file.open(filename, ifstream::in);
  char buf[5000];
  // read lines and process
  while (file.getline(buf, 5000))
  {
    string str(buf);
    images.push_back(str);
  }
  // close
  file.close();
  return images;
}

void drawDetectRegion(cv::Mat img, vector<lineProperty> linesChosed, lineProperty centerLine)
{
	int bx,by,tx,ty;
	float k;

	int height=30;
	int i=centerLine.weight;
	bx=linesChosed[i].top.x+offsetX; 
	by=linesChosed[i].top.y+offsetY;
	cv::Point pt1(bx,by);
	k=linesChosed[i].k;
	cv::Point pt2(int(-height/k+bx+0.5), by-height);

	bx=linesChosed[i-1].top.x+offsetX; 
	by=linesChosed[i-1].top.y+offsetY;
	cv::Point pt4(bx,by);
	k=linesChosed[i-1].k;
	cv::Point pt3(int(-height/k+bx+0.5), by-height);

	cv::line(img,pt1,pt2,CV_RGB(0,255,0));
	cv::line(img,pt2,pt3,CV_RGB(0,255,0));
	cv::line(img,pt3,pt4,CV_RGB(0,255,0));
	cv::line(img,pt4,pt1,CV_RGB(0,255,0));
}

#endif
