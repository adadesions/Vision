#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <core/gradient.hpp>

namespace opa {

  class Snake {
    private:
      std::vector<cv::Point> ctrlPoint;
      cv::Mat gradImg;
      double meanPoints;
      double continuity( cv::Point curPt,  cv::Point nextPt, double meanPairs, double alpha );
      double curvature( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double beta );
      double calExternalEnergy( cv::Mat &gMap, cv::Point pt );
      double calInternalEnergy( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double alpha, double beta, double meanPairs );
    public:
      Snake( cv::Mat &src );
      ~Snake( void ) {};
      cv::Mat energy( cv::Point ctrlPoint, cv::Size boundary );
      cv::Mat getGradient( void );

  };

  Snake::Snake( cv::Mat &src ){
    Gradient grad = Gradient( src );
    this->gradImg = grad.gradImg;
  }

  // Private Method
  double Snake::calExternalEnergy( cv::Mat &gMap, cv::Point pt ){
    int magnitude = (int)gMap.at<uchar>(pt.x, pt.y);
    return (double)std::exp(-magnitude);
  }

  double Snake::calInternalEnergy( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double alpha, double beta, double meanPairs ){
    double cont = continuity( curPt, nextPt, meanPairs, alpha);
    double curve = curvature( curPt, prevPt, nextPt, beta );
    return cont + curve;
  }

  double Snake::continuity( cv::Point curPt,  cv::Point nextPt, double meanPairs, double alpha){
    double dx = std::pow( curPt.x - nextPt.x, 2 );
    double dy = std::pow( curPt.y - nextPt.y, 2 );
    return alpha * std::pow(dx + dy - meanPairs, 2 );
  }

  double Snake::curvature( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double beta ){
    double dx2 = std::pow( prevPt.x - 2*curPt.x + nextPt.x, 2 );
    double dy2 = std::pow( prevPt.y - 2*curPt.y + nextPt.y, 2 );
    return beta * ( dx2 + dy2 );

  }

  // Public Method
  cv::Mat Snake::energy( cv::Point ctrlPoint, cv::Size boundary){
    if( boundary.width%2 == 0 ){
      boundary.width = boundary.width + 1;
    }
    if( boundary.height%2 == 0 ){
      boundary.height = boundary.height + 1;
    }
    int rowScale = (boundary.height - 1)/2;
    int colScale = (boundary.width - 1)/2;
    cv::Point originPoint(ctrlPoint.x - rowScale, ctrlPoint.y - colScale);
    cv::Rect roi = cv::Rect(originPoint, boundary);
    cv::Mat gMap = this->gradImg(roi);
    cv::Mat energyMap(gMap.size(), CV_64F);

//Debug
    std::cout << "gMap" << std::endl;
    std::cout << gMap << std::endl;
//End Debug

//Debug
    cv::namedWindow("gMap", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("gMap", 950, 0);
    cv::imshow("gMap", gMap);
//End Debug    

    for(int row = 0; row < gMap.rows; row++){
      for(int col = 0; col < gMap.cols; col++){
        cv::Point curPt, prevPt, nextPt;
        curPt = cv::Point( col, row );
        prevPt = cv::Point( col - 1, row - 1 );
        nextPt = cv::Point( col + 1, row + 1);
        double alpha = 1, beta = 1;
        double Eint = calInternalEnergy( curPt, prevPt, nextPt,  alpha, beta, 1);
        double Eext = calExternalEnergy( gMap, cv::Point( col, row));
        energyMap.at<double>(col, row) = Eint + Eext;
      }
    }
    return energyMap;
  }

  cv::Mat Snake::getGradient( void ){
    return this->gradImg;
  }

}

#endif
