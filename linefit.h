#if !defined LINEFIT
#define LINEFIT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
//#define PI 3.1415926

using namespace std;

struct lineProperty
{	
	cv::Point pt1;
	cv::Point pt2;
	float k;
	int weight;
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
			//when k<0,pt1 is the buttom point
			lp.pt1=pt1;
			lp.pt2=pt2;
			lp.k=k;
			linesChosed.push_back(lp);
			++it2;
		}
	}
	return linesChosed;
}

void printCenter(lineProperty line)
{
	cout<<"("<<line.pt1.x<<","<<line.pt1.y<<")"<<" ";
	cout<<"("<<line.pt2.x<<","<<line.pt2.y<<")"<<" ";
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
			if(abs(linesChosed[i].k-linesChosed[j].k)<0.2)
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
			if(abs(linesChosed[j].k-linesBefore[i].k)<0.2)
			{	break;	}
			if(j==size-1) //if all chosed have been examinated 
			{
				linesBefore[i].weight--;
				if(linesBefore[i].weight>0)
					linesChosed.push_back(linesBefore[i]);
			}
			
		}
	}
	return linesChosed;
}

lineProperty findDirection(vector<lineProperty> lines)
{
	int left=0,right=546,temp;
	int centerX=280,centerY=95;
	int il=0,ir=0,i;
	int size=lines.size();
	//find the right and left line
	for(i=0;i<size;i++)
	{
		if(lines[i].k>0) //right,buttom is pt2
		{
			temp=lines[i].pt2.x;
			if(temp<right && temp>centerX)
			{
				right=temp;
				ir=i;
			}
		}
		else //left,buttom is pt1
		{
			temp=lines[i].pt1.x;
			if(temp>left && temp<centerX)
			{
				left=temp;
				il=i;
			}
		}
	}
	//calculate the direction
	float xl,xr;
	int xButtom,xTop;
	xl=(centerY-lines[il].pt2.y)/lines[il].k+lines[il].pt2.x;
	xr=(centerY-lines[ir].pt2.y)/lines[ir].k+lines[ir].pt2.x;
	xButtom=int((xl+xr)/2+0.5);

	xl=(1-lines[il].pt2.y)/lines[il].k+lines[il].pt2.x;
	xr=(1-lines[ir].pt2.y)/lines[ir].k+lines[ir].pt2.x;
	xTop=int((xl+xr)/2+0.5);
	
	cv::Point pt1(xButtom,centerY);
	cv::Point pt2(xTop,1);
	lineProperty centerLine;
	centerLine.pt1=pt1;
	centerLine.pt2=pt2;

//	printCenter(lines[il]);
//	printCenter(lines[ir]);
	return centerLine;
}

#endif
