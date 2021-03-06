/*
 * =====================================================================================
 *
 *       Filename:  relaxationLabelling.cpp
 *
 *    Description:  Implementing Relaxation Labelling
 *
 *        Version:  1.0
 *        Created:  05/28/2017 01:41:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ada Kaminkure (AdaCode), ada@adacode.io
 *        Company:  ADACODE.IO
 *
 * =====================================================================================
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <openAda.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
	// Mat src = imread("./data/sampleGeometry2.jpg", 1);
	Mat src = imread("./data/coins.jpg", 1);
	// Mat src = imread("./data/alan.jpg", 1);
	resize(src, src, Size(500, 500));
	Probability prob = Probability( src );
	prob.sample();

	namedWindow("Src", WINDOW_NORMAL);
	moveWindow("Src", 0, 0);
	imshow("Src", src);

	waitKey(0);
	return 0;
}
