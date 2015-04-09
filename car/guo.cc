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
	VideoCapture cap(-1);
	if(!cap.isOpened())
	{
		cout<<"not open"<<endl;
		return 0;
	}
	bool on(true);
	int dir[2]={7,0};
	init_motor();
	while(on)
	{
		if(!cap.read(frame))
		{
			cout<<"cap error"<<endl;
			return 0;
		}
//		imshow("org",frame);
		cvtColor(frame,ROI,CV_BGR2YCrCb);
		cv::split(ROI,planes);
		ProcessImage(planes[0],planes[1],planes[2], dir);
//		double degree=atan(direction);
		car_speed_set(dir[0], dir[1]);
	
		cout<<dir[0]<<" "<<dir[1]<<endl;		
//		usleep(50000);
	}
	cap.release();
	return 0;
}
