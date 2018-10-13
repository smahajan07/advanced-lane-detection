#ifndef LANEDETECTOR_H
#define LANEDETECTOR_H

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class lanedetector {
 private:
  bool leftLaneF;
  bool rightLaneF;
  double leftSlope;
  double rightSlope;
  cv::Point leftBias;
  cv::Point rightBias;
  double vanPtThresh;
  double imgCenter;

 public:
  lanedetector();
  cv::Mat undistortImage(cv::Mat, cv::Mat, cv::Mat);
  cv::Mat preprocessImage(cv::Mat);
  cv::Mat grayImage(cv::Mat);
  cv::Mat detectEdges(cv::Mat);
  cv::Mat extractROI(cv::Mat, cv::Rect);
  cv::Mat createMask(cv::Mat);
  cv::Mat perspectiveTransform(cv::Mat);
  std::vector<cv::Vec4i> detectLanes(cv::Mat);
  cv::Mat drawLines(cv::Mat, std::vector<cv::Vec4i>);
  cv::Mat drawLines(cv::Mat, std::vector<cv::Vec4i>, std::vector<cv::Vec4i>);
  int drawPolygon(cv::Mat, std::vector<cv::Point>, std::string);
  std::vector<std::vector<cv::Vec4i>> sortLanes(std::vector<cv::Vec4i>,
                                                cv::Mat);
  std::vector<cv::Point> computeFitLine(std::vector<std::vector<cv::Vec4i>>,
                                        cv::Mat);
  std::string predictTurn();

};

#endif
