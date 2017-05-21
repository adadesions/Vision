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
#include <vector>
#include <iterator>
#include <opencv2/opencv.hpp>
#include <openAda.hpp>

using namespace std;
using namespace cv;

Mat src;
Mat painted;
vector<Point> ctrlPoints;

static void onMouseClick(int event, int x, int y, int, void*){
	if( event != EVENT_LBUTTONDOWN || ctrlPoints.size() >= 9){
		if( ctrlPoints.size() >= 9 ){
			cout << ctrlPoints << endl;
		}
		return;
	}

	Mat target = painted;
	circle( target, Point(x, y), 2, Scalar(0, 0, 255), -1);
	ctrlPoints.push_back(Point(x, y));
	imshow("Src", target);
}

int main(int argc, char *argv[]){
	src = imread("./data/brain1.jpg", 1);
	resize(src, src, Size(400, 440));
	painted = src.clone();

	namedWindow("Src", WINDOW_AUTOSIZE);
	moveWindow("Src",0, 0);
	setMouseCallback("Src", onMouseClick, 0);
	// circle(src, Point(180, 27), 2, Scalar(0, 0, 255), -1);
	imshow("Src", src);

	//Testing ctrl points
	Point tcp[] = {
		Point(180, 27),
		Point(82, 72),
		Point(28, 174),
		Point(39, 323),
		Point(147, 411),
		Point(322, 362),
		Point(368, 235),
		Point(348, 143),
		Point(286, 50)
	};
	vector<Point> testCrtlPoints( begin(tcp), end(tcp) );

	// Test Snake Object
	Mat graySrc;
	cvtColor(src, graySrc, CV_BGR2GRAY);
	opa::Snake naka = opa::Snake( graySrc );
	Rect roi(Point(0, 0), Size(15, 15));
	Mat energyMap = naka.energy(testCrtlPoints[0], Size(15, 15));
	imshow("Snake Gradient", naka.getGradient());
	imshow("Energy Map", energyMap);
	cout << energyMap << endl;

	waitKey(0);
	return 0;
}
