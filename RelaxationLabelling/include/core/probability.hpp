#ifndef PROBABILITY_HPP
#define PROBABILITY_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>


class Probability{
  private:
    cv::Mat srcImg;
    std::vector<int> bin;
    std::vector<int> initBin( int range = 32 );


  public:
    Probability( cv::Mat srcImg );
    ~Probability(){};
    double mean( std::vector<int> bin, int n );
    double variance( std::vector<int> bin, double mean, int n );
    double SD( std::vector<int> bin, double mean, int n );
    double PDF( double x, double mean, double variance );
    void sample();
    std::vector<int> getBin();
};

  Probability::Probability( cv::Mat srcImg ){
    cv::cvtColor(srcImg, this->srcImg, CV_BGR2GRAY);
  }

  //Private method
  std::vector<int> Probability::initBin( int range ){
    if( range < 0 || range > 256 ){
      range = 32;
    }
    int binSize = std::floor(256/range);
    std::vector<int> bin(binSize);

    int rows = this->srcImg.rows;
    int cols = this->srcImg.cols;
    for(int row = 0; row < rows; row++){
      for(int col = 0; col < cols; col++){
        int pixel = (int)this->srcImg.at<uchar>(col, row);
        int index = pixel % binSize;
        bin[index] = bin[index] + 1;
      }
    }
    for (auto i: bin)
      std::cout << i << '\n';

    return bin;
  }

  // Public method
  std::vector<int> Probability::getBin(){
    return this->bin;
  }

  double Probability::mean( std::vector<int> bin, int n ){
    int sum = 0;

    for(int i = 0; i < this->bin.size(); i++){
      sum = sum + ( i + 1 ) + bin[i];
    }
    return (double)sum/n;
  }

   double Probability::variance( std::vector<int> bin, double mean, int n ){
    double sum = 0;
    for( int i = 0; i < bin.size(); i++){
      sum = sum + std::pow( ( i+1 ) - mean, 2 )*bin[i];
    }
    return (double)sum/n;
  }

  double Probability::SD( std::vector<int> bin, double mean, int n ){
    return (double)std::sqrt( variance(bin, mean, n) );
  }

  // double Probability::PDF( double x, double mean, double variance ){
  //   const double PI = 3.14159265359;
  //
  // }

  void Probability::sample(){
    this->bin = initBin( 32 );
    std::vector<int> *bin = &this->bin;
    cv::Mat img = this->srcImg;
    int n = img.rows * img.cols;

    double xbar = mean(*bin, n);
    double var = variance(*bin, xbar, n);
    double sd = SD(*bin, xbar, n);
    std::cout << "mean : " << std::to_string(xbar) << std::endl;
    std::cout << "Variance : " << std::to_string(var) << std::endl;
    std::cout << "SD : " << std::to_string(sd) << std::endl;
    std::cout << "n : " << std::to_string(n) << std::endl;
  }

#endif
