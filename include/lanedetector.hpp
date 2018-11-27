/** MIT License

 Copyright (c) 2018 Sarthak Mahajan

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

/**
 *@copyright Copyright (c) 2018 Sarthak Mahajan
 *@file lanedetector.h
 *@author Sarthak Mahajan
 *@brief Header file for the Lanedetector class.
 *@brief All methods and variables have been declared here
 *@brief and defined in lanedetector.cpp
 */

#ifndef INCLUDE_LANEDETECTOR_HPP_
#define INCLUDE_LANEDETECTOR_HPP_

#include <iostream>
#include <vector>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "../include/preProcess.hpp"

class lanedetector : public preProcess {
 private:
  bool leftLaneF;  // flag for whether left lane is detected or not
  bool rightLaneF;  // flag for whether right lane is detected or not
  double leftSlope;  // stores value of left slope
  double rightSlope;  // stores value of right slope
  cv::Point leftBias;  // stores value of left bias
  cv::Point rightBias;  // stores value of left bias
  double vanPtThresh;  // stores value for vanishing point threshold
  double imgCenter;  // stores value for center of the image

 public:
  lanedetector();
  lanedetector(bool, bool, double, double, cv::Point, cv::Point, double,
               double);
  cv::Mat detectEdges(const cv::Mat);
  cv::Mat extractROI(const cv::Mat, const cv::Rect);
  cv::Mat createMask(const cv::Mat);
  cv::Mat perspectiveTransform(const cv::Mat);
  std::vector<cv::Vec4i> detectLanes(const cv::Mat);
  cv::Mat drawLines(const cv::Mat, std::vector<cv::Vec4i>);
  cv::Mat drawLines(const cv::Mat, std::vector<cv::Vec4i>,
                    std::vector<cv::Vec4i>);
  int drawPolygon(const cv::Mat, std::vector<cv::Point>, std::string);
  std::vector<std::vector<cv::Vec4i>> sortLanes(std::vector<cv::Vec4i>,
                                                const cv::Mat);
  std::vector<cv::Point> computeFitLine(std::vector<std::vector<cv::Vec4i>>,
                                        const cv::Mat);
  std::string predictTurn();
};

#endif  // INCLUDE_LANEDETECTOR_HPP_
