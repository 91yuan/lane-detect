#if !defined LINEFINDER
#define LINEFINDER

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define PI 3.1415926

class LineFinder
{
	private:
	cv::Mat img;

	double deltaRho;
	double deltaTheta;
	int minVote;
	double minLength;
	double maxGap;

	public:
	std::vector<cv::Vec4i> lines;

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

	std::vector<cv::Vec4i> findLines(cv::Mat& binary)
	{
		lines.clear();
		cv::HoughLinesP(binary,lines,deltaRho,deltaTheta,minVote,minLength,maxGap);
		return lines;
	}
	
};

#endif
