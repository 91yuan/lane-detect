#if !defined COLOR
#define COLOR

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include "linefit.h"

using namespace std;

cv::Mat detectColor(cv::Mat Cr, cv::Mat Cb)
{
	int row=Cr.rows;
	int col=Cr.cols;	
	int R=Cr.at<uchar>(95,280);
	int B=Cb.at<uchar>(95,280);
	cv::Mat road(row,col,CV_8U,cv::Scalar(0));

	for(int i=0;i<row;i++)
	{
		uchar* dataR=Cr.ptr<uchar>(i);
		uchar* dataB=Cb.ptr<uchar>(i);
		uchar* data =road.ptr<uchar>(i);
		for(int j=0;j<col;j++)
		{
			if(abs(*(dataR++)-R)+abs(*(dataB++)-B)>4)
				*data++= 0;
			else
				*data++= 255;
		}		
	}
	return road;
}

cv::Mat colorDetect(cv::Mat Cr, cv::Mat Cb)
{
	cv::Mat road=detectColor(Cr, Cb);
	cv::dilate(road,road,cv::Mat());
	cv::erode(road,road,cv::Mat());
//	imshow("Binary",road);
	return road;
}

int detectObstacle(cv::Mat img, int threshold)
{
	int row=img.rows;
	int col=img.cols;
	int count, obstacle=0;
	for(int i=0;i<row;i++)
	{
		uchar* data=img.ptr<uchar>(i);
		count=0;
		for(int j=0;j<col;j++)
		{
			if(*(data++) || 0)
				count++;
		}	
		if(count<threshold)
			obstacle++;	
	}
	return obstacle;
}

int myMask(cv::Mat img,lineProperty line, vector<lineProperty> linesChosed )
{
	int lineType = 8;
	cv::Mat mask=cv::Mat::zeros(img.rows, img.cols, CV_8UC1);

	int bx,by,tx,ty;
	float k;
	cv::Point points[1][4];

	int height=30;
	int i=line.weight;

	bx=linesChosed[i].top.x; 
	by=img.rows;
	k=linesChosed[i].k;
	points[0][0] = cv::Point(bx,by);
	points[0][1] = cv::Point(int(-height/k+bx+0.5), 0);

//	cout<<bx<<","<<-height/0.5<<img.cols<<endl;

	bx=linesChosed[i-1].top.x; 
	k=linesChosed[i-1].k;
	points[0][3] =cv::Point(bx,by);
	points[0][2] = cv::Point(int(-height/k+bx+0.5), 0);
//	cout<<bx<<","<<int(-height/k+bx+0.5)<<endl;

	const cv::Point* ppt[1] = { points[0] };
	int npt[] = { 4 };

	cv::fillPoly( mask, ppt, npt, 1, cv::Scalar(255), lineType );
	mask=mask & img;

	int obst=detectObstacle(mask, points[0][1].x-points[0][2].x-5);
	return obst;
 }


#endif
