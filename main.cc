#include <opencv/highgui.h>
#include <iostream>
#include <opencv/cv.h>
#include "linefinder.h"
#include <fstream>
#include <sstream>
#include "cmdline.h"
#include "test.h"
#include "linefit.h"
#include "color.h"

using namespace std;
using namespace cv;

vector<lineProperty> linesBefore;

void lineDetect(Mat ROI, Mat drawImage, Mat Cr,Mat Cb)
{
	//use canny the detect contours
	Mat image,result;
	Canny(ROI,image,125,310);
	cv::namedWindow("Canny");
	cv::imshow("Canny",image);
	//use probabilistic Hough to detect lines
	LineFinder finder;
	finder.setLineLengthAndGap(15,20);
	finder.setMinVote(10);
	vector<cv::Vec4i>lines=finder.findLines(image);	
	
//	cout<<"before="<<" "<<linesBefore.size()<<" ";
	vector<lineProperty> linesChosed=chooseLines(lines);
	linesBefore=mergeLines(linesChosed,linesBefore);
	
//	cout<<"chosed="<<linesChosed.size()<<endl;
	drawDetectedLines(drawImage,linesBefore);
	printLine(linesBefore);
	
	lineProperty centerLine=findDirection(linesBefore);
	drawDirection(drawImage, centerLine);
	cout<<"K="<<centerLine.k<<endl;
//	cv::imwrite("1.bmp",drawImage);	
//	drawDetectRegion(drawImage,linesBefore,centerLine);
/*	cv::Mat road=colorDetect(Cr,Cb);
	int obs=myMask(road,centerLine,linesBefore);
	if(obs>2)
		drawDetectRegion(drawImage,linesBefore,centerLine);*/
}

void ProcessImage(string fileName)
{
	cv::Mat image=cv::imread(fileName);
	//region of interest
	cv::Mat ROI;
	vector<cv::Mat>planes;
	cv::cvtColor(image,ROI,CV_BGR2YCrCb);
	cv::split(ROI,planes);
	cv::Mat Cr=planes[1](cv::Rect(0,150,639,90));
	cv::Mat Cb=planes[2](cv::Rect(0,150,639,90));
//	cv::Mat Y =planes[0](cv::Rect(42,250,546,95));
	cv::Mat Y =planes[0](cv::Rect(0,240,639,239));
//	cout<<image.rows<<" "<<image.cols<<endl;
//	cv::imshow("Y",Y);
//	cv::imshow("Cr",Cr);
//	cv::imshow("Cb",Cb);
	
	lineDetect(Y, image,Cr,Cb);

	cv::namedWindow("HoughP");
	cv::imshow("HoughP",image);
	cv::waitKey(0);
}

int main(int argc, char **argv)
{
	gengetopt_args_info args_info;
	if (cmdline_parser (argc, argv, &args_info) != 0)
    exit(1) ;

	if (args_info.type_opt_flag){
		vector<string> images=ReadLines(args_info.input_opt_arg);
		string path="./";
		for (int i=0; i<images.size(); i++)
     	{
      	  string imageFile = path + images[i];
		  cout<<imageFile<<'\n';
      	  ProcessImage(imageFile);
      	}
	}else{
		ProcessImage(args_info.input_opt_arg);
	}
	return 1;
}
