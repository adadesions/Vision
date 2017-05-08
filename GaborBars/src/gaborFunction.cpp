/*
 * =====================================================================================
 *
 *       Filename:  gaborFunction.cpp
 *
 *    Description:  gabor main function
 *
 *        Version:  1.0
 *        Created:  04/11/2017 02:39:23
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

const int thata_max = 360;
const int sigma_max = 100;
const int lambd_max = 180;
const int kernel_max = 500;

int thata_slider, sigma_slider, lambd_slider, kernel_slider;
double thata, sigma, lambd, kernel_value;

Mat img, gaborKernel, dst;

void on_trackbar(int, void*)
{
	thata = (double)thata_slider;
	sigma = (double)sigma_slider;
	lambd = (double)lambd_slider;
	kernel_value = (double)kernel_slider;

	int ksize = kernel_value, ktype = CV_64F;
	double psi = CV_PI*0.5;
	if(ksize <= 0)
		ksize = 1;
	Mat gaborKernel = getGaborKernel(Size(ksize, ksize), sigma, thata, lambd, 1, psi, ktype);
	filter2D(img, dst, img.depth(), gaborKernel);
	Mat marked;
	cvtColor(dst.clone(), marked, COLOR_RGB2GRAY);
	vector<Point> pointStore;

	for(int row = 0; row<marked.rows; row++)
	{
		for(int col = 0; col<marked.cols; col++)
		{
			int intensity = (int)marked.at<uchar>(row, col);
			if(intensity >= 255)
			{
				Point center(col, row);
//				circle(dst, center,	1, Scalar(0,0,255), 0);
				pointStore.push_back(center);
			}
		}
	}
	imshow("Kernel", gaborKernel);

// axis
	line(dst, Point(dst.cols/2, 0), Point(dst.cols/2, dst.rows), Scalar(0, 255, 0), 3);
	line(dst, Point(0, dst.rows/2), Point(dst.cols, dst.rows/2), Scalar(0, 255, 0), 3);
	line(dst, Point(1, 1), Point(dst.cols, dst.rows), Scalar(0, 255, 0), 3);
	line(dst, Point(dst.cols, 0), Point(0, dst.rows), Scalar(0, 255, 0), 3);
	imshow("Gabor Result", dst);
	cout << "nPoint = " << pointStore.size() << endl;
}

int main(int argc, char *argv[])
{
	img = imread("./data/brain2.jpg", 1);
	if(!img.data)
		cout << "Image not found" << endl;
// Region of interest
	Rect roi;
	roi.x = img.cols/13;
	roi.y = img.rows/13;
	roi.width = img.cols - roi.x*2;
	roi.height = img.rows - roi.y*2;
	img = img(roi);


// destination Mat
	dst = cv::Mat(img.size(), img.type());

	//init slide value
	thata_slider = 180;
	sigma_slider = 2;
	lambd_slider = 10;
	kernel_slider = 11;

	//named window
	namedWindow("Gabor Result", 0);

	//Create trackbar
	char namedThata[50], namedSigma[50], namedLambd[50], namedKernel[50];
	sprintf(namedThata, "Thata");
	sprintf(namedSigma, "Sigma");
	sprintf(namedLambd, "Lambd");
	sprintf(namedKernel, "Kernel Size");
	createTrackbar(namedThata, "Gabor Result", &thata_slider, thata_max, on_trackbar);
	createTrackbar(namedSigma, "Gabor Result", &sigma_slider, sigma_max, on_trackbar);
	createTrackbar(namedLambd, "Gabor Result", &lambd_slider, lambd_max, on_trackbar);
	createTrackbar(namedKernel, "Gabor Result", &kernel_slider, kernel_max, on_trackbar);

	//Move window
	moveWindow("Gabor Result", 0, 0);

	//show trackbar
	on_trackbar(thata_slider, 0);
	on_trackbar(sigma_slider, 0);
	on_trackbar(lambd_slider, 0);
	on_trackbar(kernel_slider, 0);

	waitKey(0);

	return 0;
}
