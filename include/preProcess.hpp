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
 *@file preProcess.hpp
 *@author Sarthak Mahajan
 *@brief Header file for the pre processing class.
 *@brief All methods and variables have been declared here
 *@brief and defined in preProcess.cpp
 */

#ifndef INCLUDE_PREPROCESS_HPP_
#define INCLUDE_PREPROCESS_HPP_

#include <iostream>
#include <vector>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

class preProcess {
 private:

 public:
  cv::Mat undistortImage(const cv::Mat, const cv::Mat, const cv::Mat);
  cv::Mat preprocessImage(const cv::Mat);
  cv::Mat grayImage(const cv::Mat);
};

#endif  // INCLUDE_PREPROCESS_HPP_
