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
#include "opencv2/opencv.hpp" 

using namespace std;
using namespace cv;

const int thata_max = 360;
const int sigma_max = 30;
const int lambd_max = 180;
const int kernel_max = 500;

int thata_slider, sigma_slider, lambd_slider, kernel_slider;
double thata, sigma, lambd, kernel_value;

Mat img, gaborKernel, dst;

void on_trackbar(int, void*)
{
	thata = (double)thata_slider;
	sigma = (double)sigma_slider;
	lambd = (double)-lambd_slider;
	kernel_value = (double)kernel_slider;

	int ksize = kernel_value, ktype = CV_64F;
	double psi = CV_PI*0.5;
	Mat gaborKernel = getGaborKernel(Size(ksize, ksize), sigma, thata, lambd, 0, psi, ktype);
	filter2D(img, dst, img.depth(), gaborKernel);
	imshow("Kernel", gaborKernel);
	imshow("Gabor Result", dst);
}

int main(int argc, char *argv[])
{
	img = imread("./data/alan.jpg");
	if(!img.data)
		cout << "Image not found" << endl;	
	dst = cv::Mat(img.size(), img.type());

	//init slide value
	thata_slider = 60;
	sigma_slider = 10;
	lambd_slider = 10;
	kernel_slider = 10;

	//named window
	namedWindow("Gabor Result", 1);

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
