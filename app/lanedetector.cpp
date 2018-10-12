#include "../include/lanedetector.h"

lanedetector::lanedetector() {

}

cv::Mat lanedetector::undistortImage(cv::Mat inpImg, cv::Mat cameraMatrix,
                                     cv::Mat distCoeff) {

  cv::Mat outImg;
  cv::undistort(inpImg, outImg, cameraMatrix, distCoeff);
  return outImg;
}

cv::Mat lanedetector::preprocessImage(cv::Mat inpImg) {
  cv::Mat outImg;
  cv::GaussianBlur(inpImg, outImg, cv::Size(3, 3), 0, 0);
  return outImg;
}

cv::Mat lanedetector::grayImage(cv::Mat inpImg) {
  cv::Mat outImg;
  cv::cvtColor(inpImg, outImg, cv::COLOR_BGR2GRAY);
  return outImg;
}

cv::Mat lanedetector::detectEdges(cv::Mat inpImg) {
  cv::Mat outImg;
  Canny(inpImg, outImg, 50, 150, 3);
  return outImg;
}
