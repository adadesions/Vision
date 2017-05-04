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

namespace adaVision{
	cv::Mat saltPepperNoise( cv::Mat input, int resolution )
	{
		double prob = 0;
		int res = resolution;
		cv::Mat noiseImg = input.clone();
		for(int i=0; i<noiseImg.rows; i += res)
		{
			for(int j=0; j<noiseImg.cols; j += res)
			{
				prob = (rand()%11)*0.1;
				if(prob < 0.1)
					noiseImg.at<uchar>(i,j) = 0;
				else if(prob > 0.9)
					noiseImg.at<uchar>(i,j) = 255;
			}
		}
		return noiseImg;
	}

	int calSobel( cv::Mat input, int cx, int cy, char direction )
	{
		//Sobel
		int xMask[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
		int yMask[3][3] = {{1, 2, 1}, {0 ,0 ,0}, {-1, -2, -1}};

		//Scharr
		int xSchMask[3][3] = {{-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3}};
		int ySchMask[3][3] = {{-3, -10, -3}, {0, 0, 0}, {3, 10, 3}};

		int ksize = 1;
		int convolute = 0;
		for(int i=-ksize; i<=ksize; i++)
		{
			for(int j=-ksize; j<=ksize; j++)
			{
				int m, n, mask, rows, cols;
				m = ksize+i;
				n = ksize+j;
				cols = cx+i;
				rows = cy+j;
				mask = 0;
				if( cols < 0 || rows < 0)
					continue;
				if( direction == 'x')
				{
					mask = xMask[m][n];
				}
				else
				{
					mask = yMask[m][n];
				}
				convolute += (int)input.at<uchar>(rows, cols) * mask;
			}
		}
		return abs( convolute );
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
		}

		return result;
	}
}
