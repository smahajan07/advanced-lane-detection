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
 *@file preProcess.cpp
 *@author Sarthak Mahajan
 *@brief All methods for class preProcess are defined here.
 *       Helper methods to preprocess image.
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "../include/preProcess.hpp"

// Undistortion
/**
 *@brief Due to radial distortion, straight lines appear as curved.
 *       This function undistorts the input image provided the intrinsic
 *       camera paramters. OpenCV provides an inbuilt function to perform
 *       this operation.
 *@param inpImg is the input image that needs to be un distorted
 *@param cameraMatrix is an intrinsic camera parameter with focal length
 *       and optical center values
 *@param distCoeff are the distortion coefficients and are required to
 *       undistort the image
 *@return undistorted image
 */
cv::Mat preProcess::undistortImage(const cv::Mat inpImg,
                                   const cv::Mat cameraMatrix,
                                   const cv::Mat distCoeff) {
  cv::Mat outImg;
  cv::undistort(inpImg, outImg, cameraMatrix, distCoeff);

  return outImg;
}

// Pre-Process
/**
 *@brief Denoise or smoothen image by applying a Gaussian filter
 *@param inpImg is the input image which needs to be smoothened
 *@return blurred or smooth image after denoising
 */
cv::Mat preProcess::preprocessImage(const cv::Mat inpImg) {
  cv::Mat outImg;
  cv::GaussianBlur(inpImg, outImg, cv::Size(3, 3), 0, 0);

  return outImg;
}

// Gray Image
/**
 *@brief convert a BGR image (three channels) to grayscale (single channel)
 *@param inpImg that needs to be converted to grayscale
 *@return grayscale image
 */
cv::Mat preProcess::grayImage(const cv::Mat inpImg) {
  cv::Mat outImg;
  cv::cvtColor(inpImg, outImg, cv::COLOR_BGR2GRAY);

  return outImg;
}

// Performs all operations
/**
 *@brief It calls the two main preprocessing functions
 *@param imgPath path for input image
 *@return 1 if success, 0 otherwise
 */
int preProcess::performAllOps(const std::string imgPath) {
  cv::Mat testImg = cv::imread(imgPath, cv::IMREAD_COLOR);
  cv::Mat ppImg, grayImg;
  ppImg = preprocessImage(testImg);
  grayImg = grayImage(ppImg);
  if (countNonZero(grayImg) < 1) {
    return 0;
  }
  return 1;
}

preProcess::~preProcess() {
}
