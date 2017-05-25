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
      std::vector<cv::Point> ctrlPoints;
      std::vector<cv::Mat> energyMapStore;
      cv::Mat srcImg;
      cv::Mat gradImg;
      cv::Size boundary;
      double meanPoints;

      double calMeanPairs( std::vector<cv::Point> ctrlPoints );
      double continuity( cv::Point curPt,  cv::Point nextPt, double meanPairs, double alpha );
      double curvature( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double beta );
      double calInternalEnergy( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double alpha, double beta, double meanPairs );
      double calExternalEnergy( cv::Mat &gMap, cv::Point pt );
      cv::Mat energy( cv::Point ctrlPoint, cv::Size boundary );
      cv::Point minEnergy( cv::Mat &energyMap );

    public:
      Snake( cv::Mat &src, std::vector<cv::Point> ctrlPoints, cv::Size boundary );
      ~Snake( void ) {};
      cv::Mat getGradient( void );
      void updateCtrlPoints( std::vector<cv::Point> newCtrlPt );
      void snaking();

  };

  // Constructor
  Snake::Snake( cv::Mat &src, std::vector<cv::Point> ctrlPoints, cv::Size boundary){

    Gradient grad = Gradient( src );
    this->srcImg = src;
    this->gradImg = grad.gradImg;
    this->ctrlPoints = ctrlPoints;
    this->boundary = boundary;

    //Init Energy Map
    for(int i = 0; i < ctrlPoints.size(); i++){
      cv::Mat energyMap = this->energy( ctrlPoints[i], this->boundary );
      this->energyMapStore.push_back( energyMap );
    }
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

  double Snake::calMeanPairs(std::vector<cv::Point> ctrlPoints){
    double mean = 0;
    for(int i = 0; i < ctrlPoints.size(); i++){
      cv::Point curPt = ctrlPoints[i];
      cv::Point nextPt = i+1 >= ctrlPoints.size() ? ctrlPoints[0] : ctrlPoints[i+1];
      double sdifx = std::pow( curPt.x - nextPt.x, 2);
      double sdify = std::pow( curPt.y - nextPt.y, 2);
      mean += std::sqrt( sdifx + sdify );
    }

    return mean/ctrlPoints.size();
  }

  double Snake::continuity( cv::Point curPt,  cv::Point nextPt, double meanPairs, double alpha){
    double dx = std::pow( curPt.x - nextPt.x, 2 );
    double dy = std::pow( curPt.y - nextPt.y, 2 );
    return alpha * std::pow((dx + dy)/meanPairs, 2 );
  }

  double Snake::curvature( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double beta ){
    double dx2 = std::pow( prevPt.x - 2*curPt.x + nextPt.x, 2 );
    double dy2 = std::pow( prevPt.y - 2*curPt.y + nextPt.y, 2 );
    return beta * ( dx2 + dy2 );
  }

  cv::Point Snake::minEnergy( cv::Mat &energyMap ){
    double min[3] = {1000, 1000, 1000};
    double minimum = 0;
    std::vector<cv::Point> minPoint;

    for(int row = 0; row < energyMap.rows; row++){
      for(int col = 0; col < energyMap.cols; col++){
        double local = energyMap.at<double>(col, row);
        if( local < min[0] ){
          if( min[1] <= min[0] ){
            min[2] = min[1];
            min[1] = min[0];
          }
          min[0] = local;
          minPoint.push_back( cv::Point(col, row) );
        }
      }
    }
    return minPoint.back();
  }

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

    double meanPairs = calMeanPairs( this->ctrlPoints );
    for(int row = 0; row < gMap.rows; row++){
      for(int col = 0; col < gMap.cols; col++){
        cv::Point curPt, prevPt, nextPt;
        curPt = cv::Point( col, row );
        prevPt = cv::Point( col - 1, row - 1 );
        nextPt = cv::Point( col + 1, row + 1);
        double alpha = 0.5, beta = 0.5;
        double Eint = calInternalEnergy( curPt, prevPt, nextPt,  alpha, beta, meanPairs);
        double Eext = calExternalEnergy( gMap, cv::Point( col, row ) );
        energyMap.at<double>(col, row) = Eint + Eext;
      }
    }
    return energyMap;
  }

  // Public Method
  cv::Mat Snake::getGradient( void ){
    return this->gradImg;
  }

  void Snake::updateCtrlPoints( std::vector<cv::Point> newCtrlPt ){
    this->ctrlPoints.clear();
    this->ctrlPoints = newCtrlPt;
    for(int i = 0; i < this->ctrlPoints.size(); i++){
      std::cout << ctrlPoints[i] << std::endl;
    }
  }

  void Snake::snaking(){
    // Find min energy for move each ctrlPoints
    int scale = (this->boundary.width - 1)/2;
    std::vector<cv::Point> tempCtrl;
    for(int i = 0; i < this->energyMapStore.size(); i++){
      cv::Point minPt = this->minEnergy( energyMapStore[i] );
      int tx = this->ctrlPoints[i].x + minPt.x - scale;
      int ty = this->ctrlPoints[i].y + minPt.y - scale;
      cv::Point tPt = cv::Point( tx, ty );
      tempCtrl.push_back(tPt);
    }
    updateCtrlPoints( tempCtrl );
    cv::Mat snaking = this->srcImg.clone();
    for(int i = 0; i < this->ctrlPoints.size(); i++){
      cv::circle(snaking, this->ctrlPoints[i], 2, cv::Scalar(255,255,255), -1);
    }
    cv::imshow("Snaking", snaking);
  }

}

#endif
