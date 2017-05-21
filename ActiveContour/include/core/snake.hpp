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
      double calExternalEnergy( cv::Mat &gMap, cv::Point pt );
      double calInternalEnergy( cv::Rect ROI );
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
    int magnitude = (int)gMap.at<uchar>(pt.y, pt.x);
    return std::exp( -magnitude );    
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
    std::cout << "gMap" << std::endl;
    std::cout << gMap << std::endl;
    imshow("gMap", gMap);
    for(int row = 0; row < gMap.rows; row++){
      for(int col = 0; col < gMap.cols; col++){
        double Eext = calExternalEnergy( gMap, cv::Point( row, col));
        energyMap.at<double>(col, row) = Eext;
      }
    }
    return energyMap;
  }

  cv::Mat Snake::getGradient( void ){
    return this->gradImg;
  }

}

#endif
