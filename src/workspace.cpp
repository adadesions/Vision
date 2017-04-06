/*
 * =====================================================================================
 *
 *       Filename:  workspace.cpp
 *
 *    Description:  Workspace and Experimental for my Computer vision code
 *
 *        Version:  1.0
 *        Created:  03/20/2017 17:13:39
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Ada Kaminkure (AdaCode), ada@adacode.io
 *        Company:  ADACODE.IO
 *
 * =====================================================================================
 */

#include <iostream>
#include "opencv2/opencv.hpp"
#include "adaVision.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *agrv[])
{
	Mat img = imread("./data/alan.jpg", 0);
	if( img.data == NULL)
	{
		cout << "Image not found" << endl;
		return -1;
	}
	if( img.rows > 500)
	{
		resize(img, img, Size(400,400), 0, 0, INTER_AREA);
	}
	//Sobel operator
	Mat sobel_img = sobel(img);
	imshow("G", sobel_img);
	
	//Generate Noise Image
	Mat noiseImg = saltPepperNoise(img, 5); 

	//LocalAverage Operation
	Mat reducedMean = localAverage(noiseImg, 5);

	imshow("Noise Image", noiseImg);
	imshow("LocalAverage", reducedMean);
	
	//MedianFilter
	Mat median = medianFilter(noiseImg, 21);
	imshow("Median", median);
	waitKey(0);
	return 0;
}
