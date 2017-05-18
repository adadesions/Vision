/*
 * =====================================================================================
 *
 *       Filename:  activeContour.cpp
 *
 *    Description:  a Snake algorithms
 *
 *        Version:  1.0
 *        Created:  05/04/2017 17:08:21
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
	Mat src = imread("./data/brain1.jpg", 0);
	resize(src, src, Size(400, 440));	

	opa::Gradient test = opa::Gradient(src);
	imshow("gx", test.gxImg);
	imshow("gy", test.gyImg);
	imshow("Grad", test.gradImg);

	waitKey(0);
	return 0;
}
