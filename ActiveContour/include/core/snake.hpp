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
      double curEnergy;
      double prevEnergy;

      double calMeanPairs( std::vector<cv::Point> ctrlPoints );
      double continuity( cv::Point curPt,  cv::Point nextPt, double meanPairs, double alpha );
      double curvature( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double meanPairs, double beta );
      double calInternalEnergy( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double alpha, double beta, double meanPairs );
      double calExternalEnergy( cv::Mat &gMap, cv::Point pt );
      cv::Mat energy( cv::Point ctrlPoint, cv::Size boundary );
      cv::Point minEnergy( cv::Mat &energyMap );
      void updateCtrlPoints( std::vector<cv::Point> newCtrlPt );
      void calSnake();
      std::vector<cv::Point> sortCtrlPoints( std::vector<cv::Point> ctrlPoints );

    public:
      Snake( cv::Mat &src, std::vector<cv::Point> ctrlPoints, cv::Size boundary );
      ~Snake( void ) {};
      void snaking();
      cv::Mat getGradient( void );
  };

  // Constructor
  Snake::Snake( cv::Mat &src, std::vector<cv::Point> ctrlPoints, cv::Size boundary){
    Gradient grad = Gradient( src );
    this->srcImg = src;
    this->gradImg = grad.gradImg;
    this->ctrlPoints = ctrlPoints; //sortCtrlPoints( ctrlPoints );
    this->boundary = boundary;
    this->curEnergy = 0;
    this->prevEnergy = 0;
  }

  // Private Method
  std::vector<cv::Point> Snake::sortCtrlPoints( std::vector<cv::Point> ctrlPoints ){
      std::vector<cv::Point> sortedStore;
      for(int i = ctrlPoints.size(); i < 0; i--){
        int min = 3000000;
        int index = 0;
        cv::Point minPoint;
        for(int j = 0; j < ctrlPoints.size(); j++){
          int d = std::pow(ctrlPoints[j].x, 2) + std::pow(ctrlPoints[j].y, 2);
          if( d < min){
            min = d;
            minPoint = ctrlPoints[j];
            index = j;
          }
          std::cout << " d : " << d << std::endl;
        }
        sortedStore.push_back( minPoint );
        ctrlPoints.erase( ctrlPoints.begin() + index );
      }
      return sortedStore;
  }

  double Snake::calExternalEnergy( cv::Mat &gMap, cv::Point pt ){
    int magnitude = (int)gMap.at<uchar>(pt.x, pt.y);
    return -magnitude;
  }

  double Snake::calInternalEnergy( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double alpha, double beta, double meanPairs ){
    double cont = continuity( curPt, nextPt, meanPairs, alpha);
    double curve = curvature( curPt, prevPt, nextPt, meanPairs, beta );
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
    return alpha * (double)(dx + dy)/meanPairs;
  }

  double Snake::curvature( cv::Point curPt,  cv::Point prevPt, cv::Point nextPt, double meanPairs, double beta ){
    double dx2 = std::pow( prevPt.x - 2*curPt.x + nextPt.x, 2 );
    double dy2 = std::pow( prevPt.y - 2*curPt.y + nextPt.y, 2 );
    return beta * ( dx2 + dy2 );
  }

  cv::Point Snake::minEnergy( cv::Mat &energyMap ){
    double minimum = 1000;
    cv::Point minPoint;

    for(int row = 0; row < energyMap.rows; row++){
      for(int col = 0; col < energyMap.cols; col++){
        double local = energyMap.at<double>(col, row);
        if( local < minimum ){
          minimum = local;
          minPoint =  cv::Point(col, row);
        }
      }
    }
    this->curEnergy += minimum;
    return minPoint;
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
        double alpha = 0.1, beta = 0.5;
        double Eint = calInternalEnergy( curPt, prevPt, nextPt,  alpha, beta, meanPairs);
        double Eext = calExternalEnergy( gMap, cv::Point( col, row ) );
        energyMap.at<double>(row, col) = Eint + Eext;
      }
    }
    return energyMap;
  }

  void Snake::calSnake() {
    //Init Energy Map
    for(int i = 0; i < ctrlPoints.size(); i++){
      cv::Mat energyMap = this->energy( ctrlPoints[i], this->boundary );
      this->energyMapStore.push_back( energyMap );
    }
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
    this->energyMapStore.clear();

    cv::Mat snaking = this->srcImg.clone();
    for(int i = 0; i < this->ctrlPoints.size(); i++){
      cv::circle(snaking, this->ctrlPoints[i], 4, cv::Scalar(255,255,255), -1);
      cv::line(snaking, ctrlPoints[i], i+1 >= ctrlPoints.size() ? ctrlPoints[0] : ctrlPoints[i+1], cv::Scalar(255,255,255), 1);
    }
    cv::imshow("Snaking", snaking);
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
    for(int i = 0; i < 1000; i++){
  		this->calSnake();
      //debug
      double curEnergy = std::abs(this->curEnergy);
      double prevEnergy = std::abs(this->prevEnergy);
      std::cout << "i : " << i << std::endl;
      std::cout << "curEnergy : " << curEnergy << std::endl;
      std::cout << "prevEnergy : " << prevEnergy << std::endl;
      if( curEnergy - prevEnergy < 0.01 && i > 0 ){
        std::cout<< "Break" << std::endl;
        break;
      }
      this->prevEnergy = this->curEnergy;
      this->curEnergy = 0;
  		cv::waitKey(500);
  	}
  }

}

#endif
