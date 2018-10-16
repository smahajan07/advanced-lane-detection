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
 *@file test.cpp
 *@author Sarthak Mahajan
 *@brief All tests are defined here.
 *@brief Primarily to check image channels and predicted turns.
 */
#include <iostream>
#include <gtest/gtest.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "../include/lanedetector.h"

TEST(channels, testNumOfChannelsReturned) {
  lanedetector testObj;
  cv::Mat testImg, retImg;
  std::string imgPath =
      "/home/sarthak/Downloads/LaneDetection/DataSet/images/image001.png";
  testImg = cv::imread(imgPath, cv::IMREAD_COLOR);
  retImg = testObj.grayImage(testImg);
  EXPECT_EQ(1, retImg.channels());
}

TEST(turns, checkTurnDirectionLeft) {
  lanedetector testObj(true, true, 1.5, -1.5, cv::Point(200, 0),
                       cv::Point(800, 0), 640, 12);
  EXPECT_EQ("LEFT", testObj.predictTurn());
}

TEST(turns, checkTurnDirectionRight) {
  lanedetector testObj(true, true, 1.5, 1.5, cv::Point(200, 0),
                       cv::Point(400, 0), 640, 12);
  EXPECT_EQ("RIGHT", testObj.predictTurn());
}
