#if !defined LINEFIT
#define LINEFIT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#define PI 3.1415926

using namespace std;

struct lineProperty
{
	cv::Point pt1;
	cv::Point pt2;
	float k;
};

std::vector<lineProperty> chooseLines(std::vector<cv::Vec4i> lines)
{
	float deltay,deltax;
	std::vector<lineProperty> linesChosed;
	lineProperty lp;	

	std::vector<cv::Vec4i>::iterator it2=lines.begin();
	while(it2!=lines.end())
	{
		cv::Point pt1((*it2)[0],(*it2)[1]);
		cv::Point pt2((*it2)[2],(*it2)[3]);
		deltay=pt1.y-pt2.y;
		deltax=pt1.x-pt2.x;
		//delete the horizontial lines 
		if(abs(deltay)<6)
			it2=lines.erase(it2);
//		else if(deltay==0 )
//			it2=lines.erase(it2);
		else
		{
			lp.pt1=pt1;
			lp.pt2=pt2;
			lp.k=0;
			linesChosed.push_back(lp);
			++it2;
		}
	}
	return linesChosed;
}

#endif
