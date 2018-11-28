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
 *@file mockpreProcess.hpp
 *@author Sarthak Mahajan
 *@brief Mock class
 */

#ifndef TEST_INCLUDE_MOCKPREPROCESS_HPP_
#define TEST_INCLUDE_MOCKPREPROCESS_HPP_

#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "../include/preProcess.hpp"
#include "gmock/gmock.h"

class mockpreProcess : public preProcess {
 public:
  MOCK_METHOD3(undistortImage, cv::Mat(const cv::Mat inpImg,
          const cv::Mat cameraMatrix,
          const cv::Mat distCoeff));
  MOCK_METHOD1(preprocessImage, cv::Mat(const cv::Mat inpImg));
  MOCK_METHOD1(grayImage, cv::Mat(const cv::Mat inpImg));
  MOCK_METHOD1(performAllOps, int(const std::string));
};

#endif  // TEST_INCLUDE_MOCKPREPROCESS_HPP_
