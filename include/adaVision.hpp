/*
 * =====================================================================================
 *
 *       Filename:  adaVision.hpp
 *
 *    Description:  Computer Vision library implemented by Ada
 *
 *        Version:  1.0
 *        Created:  04/04/2017 13:37:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ada Kaminkure (AdaCode), ada@adacode.io
 *        Company:  ADACODE.IO
 *
 * =====================================================================================
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "opencv2/opencv.hpp"

void helloAda()
{
	std::cout << "Hello, Ada" << std::endl;
}

cv::Mat saltPepperNoise(cv::Mat input, int resolution)
{
	double prob = 0;
	int res = resolution;
	cv::Mat noise = input.clone();
	for(int i=0; i<noise.rows; i += res)
	{
		for(int j=0; j<noise.cols; j += res)
		{
			prob = (rand()%11)*0.1;
			if(prob < 0.1)
				noise.at<uchar>(i,j) = 0;
			else if(prob > 0.9)
				noise.at<uchar>(i,j) = 255;
		}
	}
	return noise;
}

int calSobel( cv::Mat input, int cx, int cy, char direction)
{
	//Sobel
	int xMask[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
	int yMask[3][3] = {{1, 2, 1}, {0 ,0 ,0}, {-1, -2, -1}};
	//Scharr
	int xSchMask[3][3] = {{-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3}};
	int ySchMask[3][3] = {{-3, -10, -3}, {0, 0, 0}, {3, 10, 3}};
	int ksize = 1;
	int convoluted = 0;
	for(int i=-ksize; i<=ksize; i++)
	{
		for(int j=-ksize; j<=ksize; j++)
		{
			int m,n,x,y,mask;
			m = ksize+i;
			n = ksize+j;
			x = cx+i;
			y = cy+j;
			mask = 0;
			if( x < 0 || y < 0)
				continue;
			if( direction == 'x')
			{
				mask = xMask[m][n];
			}
			else
			{
				mask = yMask[m][n];
			}
			convoluted += (int)input.at<uchar>(y,x)*mask;
		}
	}
	return abs( convoluted );
}

cv::Mat sobel( cv::Mat input )
{
	int rows, cols, type;
	rows = input.rows;
	cols = input.cols;
	type = input.type();
	cv::Mat sobelx(rows, cols, type);
	cv::Mat sobely(rows, cols, type);
	cv::Mat sobelxy(rows, cols, type);
	cv::Mat sobelAngle(rows, cols, type);
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			uchar gx = (uchar)calSobel(input, i, j, 'x');
			uchar gy = (uchar)calSobel(input, i, j, 'y');
			uchar G = (uchar)std::sqrt(gx*gx + gy*gy);
			double theta = std::atan((double)gy/(double)gx)*180/M_PI;

			sobelx.at<uchar>(j,i) = gx; 
			sobely.at<uchar>(j,i) = gy;
			sobelxy.at<uchar>(j,i) = G;
			sobelAngle.at<uchar>(j,i) = (uchar)theta;
		}	
	}
	cv::imshow("Gx", sobelx);
	cv::imshow("Gy", sobely);
	cv::imshow("Angle", sobelAngle);

	//3 Channel
	cv::Mat img_rgb(sobelAngle.size(), CV_8UC3);
	cv::cvtColor(sobelAngle, img_rgb, CV_GRAY2RGB);
	cv::Vec3b blue(255,0,0);
	cv::Vec3b green(0,255,0);
	cv::Vec3b red(0,0,255);
	for(int i=0; i<img_rgb.rows; i++)
	{
		for(int j=0; j<img_rgb.cols; j++)
		{
			int intensity = img_rgb.at<cv::Vec3b>(j,i)[0];
			if(intensity >=0 && intensity < 30)
				img_rgb.at<cv::Vec3b>(j,i) = blue;
			else if(intensity >=31 && intensity < 60)
				img_rgb.at<cv::Vec3b>(j,i) = green;
			else if(intensity >=61 && intensity < 90)
				img_rgb.at<cv::Vec3b>(j,i) = red;
				
		}
	}
	cv::imshow("RGB Angle", img_rgb);
	return sobelxy;
}	

int calLocalAverage(cv::Mat input, int cx, int cy, int kernelSize)
{
	int ks = (kernelSize-1)/2;
	int sum = 0;
	for(int i=-ks; i<=ks; i++)
	{
		for(int j=-ks; j<=ks; j++)
		{
			int n = cx+i;
			int m = cy+j;
			sum += (int)input.at<uchar>(m,n);
		}
	}
	return sum/pow(kernelSize,2);
}

cv::Mat localAverage(cv::Mat input, int kernelSize)
{
	int rows = input.rows;
	int cols = input.cols;
	int meanValue = 0;
	cv::Mat result(input.size(), input.type());
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			meanValue = calLocalAverage(input, i, j, kernelSize);
			result.at<uchar>(j, i) = (uchar)meanValue;
		}
	}	
	return result;
}


int calMedianFilter(cv::Mat input, int cx, int cy, int kernelSize)
{
	int ks = (kernelSize-1)/2;
	int index_median = (kernelSize*kernelSize)/2;
	std::vector<int> data;
	for(int i=-ks; i<=ks; i++)
	{
		for(int j=-ks; j<=ks; j++)
		{
			int m,n;
			n = cx+i;
			m = cy+j;
			data.push_back((int)input.at<uchar>(m,n));
		}
	}
	sort(data.begin(), data.end());
	return data[index_median];
}

cv::Mat medianFilter(cv::Mat input, int kernelSize)
{
	int rows = input.rows;
	int cols = input.cols;
	cv::Mat result = input.clone();
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			result.at<uchar>(j,i) = calMedianFilter(input, i, j, kernelSize);
		}
		cv::imshow("median step", result);
		cv::waitKey(1);
	}

	return result;
}



