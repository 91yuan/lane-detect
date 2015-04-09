#include "cv.h"
#include "highgui.h"
#include <vector>
#include "imagePro.h"
#include "car_move.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat frame,ROI;
	vector<cv::Mat>planes;
//	frame=imread("1.jpg");
	VideoCapture cap(1);
	if(!cap.isOpened())
	{
		cout<<"not open"<<endl;
		return 0;
	}
	bool on(true);
	int dir[2]={7,0};
	while(on)
	{
		if(!cap.read(frame))
		{
			cout<<"cap error"<<endl;
			return 0;
		}
//		imshow("org",frame);
//		imwrite("1.jpg",frame);
//		frame=imread("11.jpg");
		cvtColor(frame,ROI,CV_BGR2YCrCb);
		cv::split(ROI,planes);
		ProcessImage(planes[0],planes[1],planes[2], dir);
		cout<<dir[0]<<" "<<dir[1]<<endl;	
		
		merge(planes,ROI);
		cvtColor(ROI,frame,CV_YCrCb2BGR);

		imshow("cap",frame);
		waitKey(0);
//		imwrite("1.jpg",frame);
/*		if(waitKey(33)==27)	
			on=false;
*/	}
	cap.release();
	return 0;
}
