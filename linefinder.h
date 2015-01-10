#if !defined LINEF
#define LINEF

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define PI 3.1415926

class LineFinder
{
	private:
	cv::Mat img;
	std::vector<cv::Vec4i>lines;

	double deltaRho;
	double deltaTheta;
	int minVote;
	double minLength;
	double maxGap;

	public:
	LineFinder():deltaRho(1),deltaTheta(PI/180),minVote(10),minLength(0.),maxGap(0.){}

	void setAccResolution(double dRho,double dTheta)
	{
		deltaRho=dRho;
		deltaTheta=dTheta;
	}

	void setMinVote(int minv)
	{
		minVote=minv;
	}

	void setLineLengthAndGap(double length,double gap)
	{
		minLength=length;
		maxGap=gap;
	}

	std::vector<cv::Vec4i>findLines(cv::Mat& binary)
	{
		lines.clear();
		cv::HoughLinesP(binary,lines,deltaRho,deltaTheta,minVote,minLength,maxGap);
		return lines;
	}

	void drawDetectedLines(cv::Mat &image, cv::Scalar color=cv::Scalar(255,0,0))
	{
		std::vector<cv::Vec4i>::const_iterator it2=lines.begin();
		while(it2!=lines.end())
		{
			cv::Point pt1((*it2)[0]+50,(*it2)[1]+150);
			cv::Point pt2((*it2)[2]+50,(*it2)[3]+150);
			cv::line(image,pt1,pt2,color);
			++it2;
		}
	}
};

#endif
