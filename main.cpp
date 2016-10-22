#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
using namespace cv;

double distance (double top, double bot)
{
	double width = top - bot;
	double distance = (480*0.1)/width;
	return distance;
}

double azimuth(double wdth, double pnt1, double pnt2, double dst)
{
	double ang;	
	double width = wdth/2;
	double avg = (pnt1 + pnt2)/2;
	double change = avg-width;
	ang = atan(width/dst) * 180.0 / M_PI;
	return ang;
}

int main()
{
	std::string fileLoc = "rectangle-five-feet.jpg";
	Mat img = imread(fileLoc);
	imshow("OG", img);
	waitKey(0);
	
	//CV conversion
	Mat imsv;
	cvtColor(img,imsv, CV_BGR2HSV);
	//imshow("HSV", imsv);

	//splitting
	vector<Mat> channels;
	split(imsv, channels);
	
	//threshing	
	Mat imgh = channels.at(0).clone();
	cv::Mat Lower, Upper, Result;
    cv::threshold(imgh, Lower, 71, 255, CV_THRESH_BINARY);
    cv::threshold(imgh, Upper, 95, 255, CV_THRESH_BINARY_INV);
    Result = Lower & Upper;
	imshow("th", Result);
	waitKey(0);

	//canny
	Mat cann;
	Canny(Result, cann, 100, 200);
	imshow("Cannied", cann);
	waitKey(0);

	//contours
	std::vector<vector<Point> > cnt;
	findContours(cann, cnt, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	//for loop
	int Max, Min;
	for(int j = 0; j < cnt.size(); j++)
	{
		vector<cv::Point> poly;
		cv::approxPolyDP(cnt[j], poly, (cv::arcLength(cv::Mat(cnt.at(j)), true) * 0.02), true);
        	
		if(poly.size() == 4 && cv::contourArea(poly) > 1350)
		{			
			int max = 0, min = 5000;
			for(int i = 0; i < poly.size(); i++)
			{
				if(poly[i].x > max)
				{
					max = poly[i].x;
				}
				if(poly[i].x < min)
				{
					min = poly[i].x;
				}
				Max = max;
				Min = min;
			}
		}
	}

	double dist, az;
	dist = distance(Max, Min);
	az = azimuth(img.size().width, Max, Min, dist);
	std::cout << "distance " << dist << "m" << std::endl;
	std::cout << "azimuth " << az << " degrees" << std::endl;

	/*Mat draw = Mat::zeros(img.rows, img.cols, CV_8UC3);
	Scalar col = Scalar(255,255,255);
	drawContours(draw,contours, -1, col);
	imshow("Cont", draw);
	waitKey(0);*/

}
