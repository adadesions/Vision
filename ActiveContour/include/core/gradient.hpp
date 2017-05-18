#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

namespace opa{

  class Gradient {
    private:
      cv::Mat src;
    public:
      Gradient( cv::Mat src );
      ~Gradient( void ){};
      cv::Mat gxImg;
      cv::Mat gyImg;
      cv::Mat gradImg;
      void calGradientXY( void );
      double setGradImg( cv::Point pt, char direction );
      double magnitude( int gx, int gy );
      double getGradxAt( cv::Point pt );
      double getGradyAt( cv::Point pt );
      double getGradAt( cv::Point pt );
  };

  Gradient::Gradient( cv::Mat src ){
    this->src = src.clone();
    this->gxImg = cv::Mat(src.size(), src.depth());
    this->gyImg = cv::Mat(src.size(), src.depth());
    this->gradImg = cv::Mat(src.size(), src.depth());
    Gradient::calGradientXY();
  }

  void Gradient::calGradientXY( void ){
    for(int row = 1; row < src.rows; row++ ){
      for(int col = 1; col < src.cols; col++ ){
        cv::Point curPtr( row, col );
        this->gxImg.at<uchar>(col, row) = (uchar)setGradImg(curPtr, 'x');
        this->gyImg.at<uchar>(col, row) = (uchar)setGradImg(curPtr, 'y');
        int gx = (int)this->gxImg.at<uchar>(col, row);
        int gy = (int)this->gyImg.at<uchar>(col, row);
        this->gradImg.at<uchar>(col, row) = (uchar)magnitude( gx, gy );
      }
    }
    return;
  }

  double Gradient::setGradImg( cv::Point pt, char direction ){
    int xKernel[3][3] = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };
    int yKernel[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
    int vKernel = 0;
    double sum = 0;

    for(int row = -1; row <= 1; row++){
      for(int col = -1; col <= 1; col++){
        int intensity = this->src.at<uchar>(pt.y + col, pt.x + row);
        if( direction == 'x'){
          vKernel = xKernel[row + 1][col + 1];
        }
        else{
          vKernel = yKernel[row + 1][col + 1];
        }

        sum += vKernel*intensity;
      }
    }

    return std::abs( sum ) > 255 ? 255 : std::abs( sum );
  }

  double Gradient::magnitude( int gx, int gy ){
    double mag = std::sqrt( (gx*gx) + (gy*gy) );
    return mag > 255 ? 255 : mag;
  }

  double Gradient::getGradxAt( cv::Point pt ){
    return (int)this->gxImg.at<uchar>(pt.y, pt.x);
  }

  double Gradient::getGradyAt( cv::Point pt ){
    return (int)this->gyImg.at<uchar>(pt.y, pt.x);
  }

  double Gradient::getGradAt( cv::Point pt ){
    return (int)this->gradImg.at<uchar>(pt.y, pt.x);
  }


}
#endif
