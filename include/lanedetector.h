#ifndef LANEDETECTOR_H
#define LANEDETECTOR_H

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class lanedetector {
 private:

 public:
  lanedetector();
  cv::Mat undistortImage(cv::Mat, cv::Mat, cv::Mat);
  cv::Mat preprocessImage(cv::Mat);
  cv::Mat grayImage(cv::Mat);
  cv::Mat detectEdges(cv::Mat);

};

#endif
