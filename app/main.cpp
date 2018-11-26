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
 *@file main.cpp
 *@author Sarthak Mahajan
 *@brief main block which reads a video source and detects lanes from it
 *       by creating an object of the lanedetector class and calling the
 *       methods that facilitate this process
 */

#include <iostream>
#include <chrono>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
# include "../include/lanedetector.hpp"
#define NUMFRAMES 500

using namespace std::chrono;

int main() {
  // TODO(smahajan07): make params into yml and then import
  cv::Mat cameraMatrix, distCoeff;
  cameraMatrix =
      (cv::Mat_<double>(3, 3) << 1.15422732e+03, 0.00000000e+00, 6.71627794e+02,
                                 0.00000000e+00, 1.14818221e+03, 3.86046312e+02,
                                 0.00000000e+00, 0.00000000e+00,
                                 1.00000000e+00);
  distCoeff =
      (cv::Mat_<double>(5, 1) << -2.42565104e-01, -4.77893070e-02,
                                 -1.31388084e-03, -8.79107779e-05,
                                  2.20573263e-02);
  // Creating object of lane detector class
  lanedetector obj;
  // create all variables
  cv::Mat orgImg;
  cv::Mat undImg, ppImg, grayImg, edgeImg;
  cv::Mat maskedImg;
  cv::Mat warpedImg;
  std::vector<cv::Vec4i> lines;
  std::vector<std::vector<cv::Vec4i>> validLines;
  std::vector<cv::Point> finalPoly;
  std::string turn;
  int fCount = 0;

  // Provide path for test image, maybe change through argument
  std::string vidPath(
      "/home/sarthak/Downloads/LaneDetection/DataSet/project_video.mp4");
  // open video
  cv::VideoCapture cap(vidPath);
  // check if video opened
  if (!cap.isOpened())
    return -1;
  // start video loop
  while (fCount < NUMFRAMES) {
    if (!cap.read(orgImg))
      break;
    auto start = high_resolution_clock::now();
    // actions
    // call undistort func
    undImg = obj.undistortImage(orgImg, cameraMatrix, distCoeff);
    // call pre process Image func
    ppImg = obj.preprocessImage(undImg);
    // call gray Image func
    grayImg = obj.grayImage(ppImg);
    // call detect edges
    edgeImg = obj.detectEdges(grayImg);
    // call create mask
    maskedImg = obj.createMask(edgeImg);
    // call perspective transform
    warpedImg = obj.perspectiveTransform(edgeImg);
    // call hough lines
    lines = obj.detectLanes(maskedImg);
    // sort lines
    validLines = obj.sortLanes(lines, maskedImg);
    // call compute Fit line
    finalPoly = obj.computeFitLine(validLines, undImg);
    // call predict turn
    turn = obj.predictTurn();
    // call draw polygon
    obj.drawPolygon(undImg, finalPoly, turn);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken by process: " << duration.count()
              << " microseconds" << std::endl;
    ++fCount;
    cv::waitKey(15);
  }

  return 0;
}
