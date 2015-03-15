#include <opencv/highgui.h>
#include <iostream>
#include <opencv/cv.h>
#include "linefinder.h"
#include <fstream>
#include <sstream>
#include "cmdline.h"
#include "test.h"
#include "linefit.h"

using namespace std;
using namespace cv;

vector<lineProperty> linesBefore;

void lineDetect(Mat ROI, Mat drawImage)
{
	//use canny the detect contours
	Mat image,result;
	Canny(ROI,image,125,310);
//	cv::namedWindow("Canny");
//	cv::imshow("Canny",image);
	//use probabilistic Hough to detect lines
	LineFinder finder;
	finder.setLineLengthAndGap(15,20);
	finder.setMinVote(10);
	vector<cv::Vec4i>lines=finder.findLines(image);	
	
	cout<<"before="<<" "<<linesBefore.size()<<" ";
	vector<lineProperty> linesChosed=chooseLines(lines);
	linesBefore=mergeLines(linesChosed,linesBefore);
	
	cout<<"chosed="<<linesChosed.size()<<endl;
	drawDetectedLines(drawImage,linesBefore);
	printLine(linesBefore);
	
	lineProperty centerLine=findDirection(linesBefore);
	drawDirection(drawImage, centerLine);
	cv::imwrite("1.bmp",drawImage);	
}

Mat gray(Mat& imageROI)
{
	Mat ROI(imageROI.rows,imageROI.cols,CV_8U,Scalar(0));
	for(int i=0;i<imageROI.rows ;i++)
		for(int j=0;j<imageROI.cols;j++)
		{
			ROI.at<uchar>(i,j)=imageROI.at<Vec3b>(i,j)[0];
		}
	return ROI;
}

void ProcessImage(string fileName)
{
	cv::Mat image=cv::imread(fileName);
	Mat grayImage=gray(image);
	//region of interest
	cv::Mat ROI=grayImage(cv::Rect(42,250,546,95));
//	drawDirection(image);
//	cv::namedWindow("ROI");
//	cv::imshow("ROI",image);
	
	lineDetect(ROI, image);
//	cout<<linesBefore.size()<<endl;
	cv::namedWindow("HoughP");
	cv::imshow("HoughP",image);
	cv::waitKey(0);
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
      	  ProcessImage(imageFile);
		//	cout<<imageFile<<'\n';
      	}
	}else{
		ProcessImage(args_info.input_opt_arg);
	}
	return 1;
}
