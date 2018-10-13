#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
# include "../include/lanedetector.h"
#define NUMFRAMES 500

using namespace cv;

int main()
{
  // TODO: make params into yml and then import
  Mat cameraMatrix, distCoeff;
  cameraMatrix =
      (Mat_<double>(3, 3) << 1.15422732e+03, 0.00000000e+00, 6.71627794e+02, 0.00000000e+00, 1.14818221e+03, 3.86046312e+02, 0.00000000e+00, 0.00000000e+00, 1.00000000e+00);
  distCoeff =
      (Mat_<double>(5, 1) << -2.42565104e-01, -4.77893070e-02, -1.31388084e-03, -8.79107779e-05, 2.20573263e-02);
  // Creating object of lane detector class
  lanedetector obj;
  // create all variables
  Mat orgImg;
  Mat undImg, ppImg, grayImg, edgeImg;
  Mat maskedImg;
  Mat warpedImg;
  std::vector<cv::Vec4i> lines;
  std::vector<std::vector<cv::Vec4i>> validLines;
  std::vector<cv::Point> finalPoly;
  std::string turn;
  int fCount = 0;

  // Provide path for test image, maybe change through argument
  std::string vidPath("project_video.mp4");
  // open video
  cv::VideoCapture cap(vidPath);
  // check if video opened
  if (!cap.isOpened())
    return -1;
  // start video loop
  while (fCount < NUMFRAMES) {
    if (!cap.read(orgImg))
      break;
    // actions
    // call undistort func
    undImg = obj.undistortImage(orgImg, cameraMatrix, distCoeff);
    // call gray Image func
    grayImg = obj.grayImage(undImg);
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
    ++fCount;
    waitKey(15);
  }

  return 0;
}
