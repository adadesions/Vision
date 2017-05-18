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
#include "openADA.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace ada;

int main(int argc, char* argv[])
{
	Ada c1 = Ada(100, 90);
	Factor c2 = Factor(300, 200);
	c1.getHp();
	c2.getHp();

	Mat src = imread("./data/brain1.jpg", 0);
	Mat dst(src.size(), src.type());
	int data[] = {10, 100, 1000, 10000};
	int *pix;

	pix = data;

	for(;;){
		if( !*pix )
			break;
		cout << *pix << " ";
		pix++;
	}
	return 0;
}
