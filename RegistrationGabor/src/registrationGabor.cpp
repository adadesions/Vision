/*
 * =====================================================================================
 *
 *       Filename:  registrationGabor.cpp
 *
 *    Description:  For my new experimence
 *
 *        Version:  1.0
 *        Created:  05/08/2017 20:42:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ada Kaminkure (AdaCode), ada@adacode.io
 *        Company:  ADACODE.IO
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

vector<Mat> renderGaborMultiThata(Mat &src, int scale)
{
	vector<Mat> store;
	int dim = scale + 1;
	Size ksize(dim, dim);
	double sigma = 10;
	double lambd = 64;
	double gamma = 1;
	double thata = 22.5*(180/3.1459);

	for(int i = 0; i < 8; i++)
	{
		double thataDividedBy8 = thata*i;
		store.push_back(getGaborKernel(ksize, sigma, thataDividedBy8, lambd, gamma, CV_PI*0.5, CV_64F));
	}
	return store;
}
void displayMultiGabor(vector<Mat> input)
{
	for(int i=0; i<input.size(); i++)
	{
		string winName = "DisplayMultiGabor@"+to_string(i);
		Mat img = input[i];
		namedWindow(winName, WINDOW_NORMAL);
		moveWindow(winName, i*img.cols, 0);
		imshow(winName, img);
	}
}

Mat sumMat(Mat &src, Mat &dst)
{
	Mat result = src.clone();
	int cols, rows;
	cols = src.cols;
	rows = src.rows;
	for(int x=0; x<cols; x++)
	{
		for(int y=0; y<rows; y++)
		{
			int intenSrc = src.at<uchar>(x,y);
			int intenDst = dst.at<uchar>(x,y);
			int newInten = intenSrc + intenDst;
			if( newInten > 255)
				newInten = 255;

			result.at<uchar>(x,y) = newInten;
		}
	}

	return result;
}

Mat summationOfGabor(Mat &src, vector<Mat> gaborStore)
{
	Mat summation(src.size(), src.depth());
	for(int i=0 ; i < gaborStore.size(); i++)
	{
		Mat result = src.clone();
		filter2D(src, result, result.depth(), gaborStore[i]);
		summation = sumMat(summation, result);
	}

	return summation;
}

void grayBG(Mat &src, Mat &dst)
{
	int cols, rows;
	cols = src.cols;
	rows = src.rows;
	for(int x=0; x<cols; x++)
	{
		for(int y=0; y<rows; y++)
		{
			int inten = src.at<uchar>(x,y);
			if( inten < 85)
			{
				dst.at<uchar>(x,y) = 128;
			}
			else if( inten >= 85 && inten < 170){
				dst.at<uchar>(x,y) = 0;
			}
			else{
				dst.at<uchar>(x,y) = 255;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	Mat src, dst;
	src = imread("./data/brain2.jpg", 0);
	dst = imread("./data/brain2.jpg", 0);

	vector<Mat> srcGaborStore = renderGaborMultiThata(src, 8);
	// displayMultiGabor(srcGaborStore);
	Mat sumSrc = summationOfGabor(src, srcGaborStore);
	imshow("SumSrc", sumSrc);
	Mat grayImg = sumSrc.clone();
	// grayBG(sumSrc, grayImg);
	imshow("Gray", grayImg);

	waitKey(0);
	return 0;
}
