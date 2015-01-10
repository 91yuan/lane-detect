#include <opencv/highgui.h>
#include <iostream>
#include <opencv/cv.h>
#include "linefinder.h"
#include <fstream>
#include <sstream>
#include "cmdline.h"

using namespace std;
using namespace cv;

void lineDetect(Mat ROI, Mat drawImage)
{
	//use canny the detect contours
	Mat image;
	Canny(ROI,image,125,350);

	//use probabilistic Hough to detect lines
	LineFinder finder;
	finder.setLineLengthAndGap(100,20);
	finder.setMinVote(80);
	std::vector<cv::Vec4i>lines=finder.findLines(image);
	finder.drawDetectedLines(drawImage);

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
	cv::Mat ROI=grayImage(cv::Rect(50,150,grayImage.cols-100,grayImage.rows-280));

	lineDetect(ROI, image);
	
	cv::namedWindow("New Image");
	cv::imshow("New Image",image);
	cv::waitKey(50);
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
