#if !defined TEST
#define TEST

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include "linefit.h"

using namespace std;

void printLine(vector<cv::Vec4i> lines)
{
	vector<cv::Vec4i>::const_iterator it2=lines.begin();
	int i=0;
	cout<<"work"<<endl;
	while(it2!=lines.end())
	{
		cout<<"("<<(*it2)[0]<<","<<(*it2)[1]<<")"<<" ";
		cout<<"("<<(*it2)[2]<<","<<(*it2)[3]<<")"<<endl;	
		++it2;
		i++;
	}
	cout<<i<<endl;
}

void drawDetectedLines(cv::Mat &image,int x,int y, vector<lineProperty> linesChosed, cv::Scalar color=cv::Scalar(255,0,0))
	{
		float deltay,deltax;
		std::vector<lineProperty>::iterator it2=linesChosed.begin();
		while(it2!=linesChosed.end())
		{
			cv::Point pt1=(*it2).pt1;
			cv::Point pt2=(*it2).pt2;
			pt1.x=pt1.x+x;
			pt1.y=pt1.y+y;
			pt2.x=pt2.x+x;
			pt2.y=pt2.y+y;
			cv::line(image,pt1,pt2,color);
			++it2;

		}
	}


#endif
