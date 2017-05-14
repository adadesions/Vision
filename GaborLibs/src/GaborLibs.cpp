/*
 * =====================================================================================
 *
 *       Filename:  GaborLibs.cpp
 *
 *    Description:  Gabor Library created by Ada
 *
 *        Version:  1.0
 *        Created:  05/14/2017 14:44:31
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

int main(int argc, char* argv[])
{
	Mat src = imread("./data/brain1.jpg", 0);
	imshow("Src", src);
	waitKey(0);
	return 0;
}
