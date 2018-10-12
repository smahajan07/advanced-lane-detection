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

cv::Mat lanedetector::extractROI(cv::Mat inpImg, cv::Rect rectRoi) {
  cv::Mat outImg;
  outImg = inpImg(rectRoi);
  return outImg;
}

cv::Mat lanedetector::perspectiveTransform(cv::Mat inpImg) {
  cv::Mat outImg;
  cv::Point2f inpQuad[4], outQuad[4];
  cv::Mat lambda(2, 4, CV_32FC1);
  lambda = cv::Mat::zeros(inpImg.rows, inpImg.cols, inpImg.type());
  inpQuad[0] = cv::Point2f(581, 477);
  inpQuad[1] = cv::Point2f(699, 477);
  inpQuad[2] = cv::Point2f(896, 675);
  inpQuad[3] = cv::Point2f(384, 675);

  outQuad[0] = cv::Point2f(384, 0);
  outQuad[1] = cv::Point2f(896, 0);
  outQuad[2] = cv::Point2f(896, 720);
  outQuad[3] = cv::Point2f(384, 720);

  lambda = cv::getPerspectiveTransform(inpQuad, outQuad);
  cv::warpPerspective(inpImg, outImg, lambda, outImg.size());

  return outImg;
}

std::vector<cv::Vec4i> lanedetector::detectLanes(cv::Mat inpImg) {
  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(inpImg, lines, 1, CV_PI / 180, 20, 20, 30);
  return lines;
}
