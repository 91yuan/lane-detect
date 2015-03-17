#if !defined LINEFIT
#define LINEFIT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <algorithm>
//#define PI 3.1415926
#define CX 280
#define CY 95

using namespace std;

struct lineProperty
{	
	cv::Point buttom;
	cv::Point top;
	float k;
	int weight;

	bool operator< (const lineProperty &a) const
	{
		return buttom.x<a.buttom.x;
	}
};

vector<lineProperty> chooseLines(vector<cv::Vec4i> lines)
{
	float deltay,deltax,k;
	vector<lineProperty> linesChosed;
	lineProperty lp;
	lp.weight=3;	

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
		else
		{ 
			if(deltax==0 )
				k=0;
			else
				k=deltay/deltax;
			//when k<0,pt1 is the buttom point,on the left
			if((k<0 && pt1.x<CX) || (k>0 && pt2.x>CX))
			{
				lp.buttom=cv::Point(int((CY-pt1.y)/k+pt1.x+0.5),CY);
				lp.top = cv::Point(int((1-pt1.y)/k+ pt1.x+0.5), 1);
//				lp.ex = cv::Point(int((1-pt1.y)/k+ pt1.x+0.5), 1);
				lp.k=k;
				linesChosed.push_back(lp);
			}
			++it2;
		}
	}
	return linesChosed;
}

void printCenter(lineProperty line)
{
	cout<<"("<<line.buttom.x<<","<<line.buttom.y<<")"<<" ";
	cout<<"("<<line.top.x<<","<<line.top.y<<")"<<" ";
	cout<<"k="<<line.k<<endl;
}

vector<lineProperty> mergeLines(vector<lineProperty> linesChosed, vector<lineProperty> linesBefore)
{
	int i,j;
	int size=linesChosed.size();
	vector<lineProperty> lines;
	
	for(i=0;i<size;i++)
	{
		for(j=i+1;j<size;j++)
		{
			if(abs(linesChosed[i].k-linesChosed[j].k)<0.3)
			{
				linesChosed.erase(linesChosed.begin()+j);
				j--;
				size--;
			}
		}
	}
	
	int sizeB=linesBefore.size();
	for(i=0;i<sizeB;i++) //i=before
	{
		for(j=0;j<size;j++) //j=chosed
		{
			//if detect the same line, leave it out
			//its weight is smaller than before
			if(abs(linesChosed[j].k-linesBefore[i].k)<0.3)
			{	break;	}
			if(j==size-1) //if all chosed have been examinated 
			{
				linesBefore[i].weight--;
				if(linesBefore[i].weight>0)
					linesChosed.push_back(linesBefore[i]);
			}
			
		}
	}
	sort(linesChosed.begin(),linesChosed.end());
	return linesChosed;
}

lineProperty findDirection(vector<lineProperty> lines)
{
	int temp;
	int centerX=CX,centerY=CY;
	int il=0,ir=0,i=0;
	int size=lines.size();
	//find the right and left line
	for(i=0;i<size;i++)
	{
		temp=lines[i].buttom.x;
		if(temp>centerX)
		{
			ir=i;
			il=i-1;
			break;
		}
	}
	//calculate the direction
	int xButtom,xTop;
	xButtom=int((lines[il].buttom.x+lines[ir].buttom.x)/2+0.5);
	xTop=int((lines[il].top.x+lines[ir].top.x)/2+0.5);
	
	lineProperty centerLine;
	centerLine.buttom=cv::Point(xButtom,centerY);
	centerLine.top=cv::Point(xTop,1);
	centerLine.weight=ir; //use the weight to store the site of right 
//	printCenter(lines[il]);
//	printCenter(lines[ir]);
	return centerLine;
}

#endif
