#include "../include/lanedetector.h"

lanedetector::lanedetector() {
  leftLane = false;
  rightLane = false;
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
  Canny(inpImg, outImg, 50, 100, 3);
  // adding dilation to see if results improve
//  cv::dilate(outImg, outImg, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
  return outImg;
}

cv::Mat lanedetector::extractROI(cv::Mat inpImg, cv::Rect rectRoi) {
  cv::Mat outImg;
  outImg = inpImg(rectRoi);
  return outImg;
}

cv::Mat lanedetector::createMask(cv::Mat inpImg) {
  cv::Mat outImg;
  cv::Mat mask = cv::Mat::zeros(inpImg.size(), inpImg.type());
  cv::Point vertices[4] = { cv::Point(210, 650), cv::Point(550, 450), cv::Point(
      717, 450), cv::Point(1280, 650) };
  cv::fillConvexPoly(mask, vertices, 4, cv::Scalar(255, 0, 0));
  cv::bitwise_and(inpImg, mask, outImg);
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

cv::Mat lanedetector::drawLines(cv::Mat inpImg, std::vector<cv::Vec4i> lines) {
  cv::Mat outImg = inpImg;
  for (auto l : lines) {
    cv::line(outImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
             cv::Scalar(0, 0, 255), 3, CV_AA);
  }
  return outImg;
}

cv::Mat lanedetector::drawLines(cv::Mat inpImg,
                                std::vector<cv::Vec4i> leftLines,
                                std::vector<cv::Vec4i> rightLines) {
  cv::Mat outImg = inpImg;
  for (auto l : leftLines) {
    cv::line(outImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
             cv::Scalar(0, 0, 255), 3, CV_AA);
  }
  for (auto l : rightLines) {
    cv::line(outImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
             cv::Scalar(0, 0, 255), 3, CV_AA);
  }
  cv::namedWindow("Output", CV_WINDOW_AUTOSIZE);
  cv::imshow("Output", outImg);
  return outImg;
}

std::vector<std::vector<cv::Vec4i>> lanedetector::sortLanes(
    std::vector<cv::Vec4i> lines, cv::Mat inpImg) {
  // first we need to select valid lines based on their slope
  // then segregate into left and right lines respectively
  std::vector<std::vector<cv::Vec4i>> output(2);
  std::vector<cv::Vec4i> validLines, leftLanes, rightLanes;
  // to keep a track of all slopes
  std::vector<double> slopes;
  // setting a threshold for slopes to avoid unwanted lines
  double threshSlope = 0.4;
  // to keep a track of starting and ending points of each line
  cv::Point start, end;
  // find slope of all lines and select valid lines
  for (auto line : lines) {
    // as per docs for lines output from Hough Lines
    start = cv::Point(line[0], line[1]);
    end = cv::Point(line[2], line[3]);

    // calculate slope
    // .x and .y is how you access cv::point, maybe link to doc
    double slope = (static_cast<double>(end.y) - static_cast<double>(start.y))
        / (static_cast<double>(end.x) - static_cast<double>(start.x) + 0.00001);
    // compare with threshold
    if (std::abs(slope) > threshSlope) {
      slopes.push_back(slope);
      validLines.push_back(line);
    }
  }
  // just checking how many valid lines we got
//  std::cout << "Valid lines len " << validLines.size() << std::endl;

  size_t idx = 0;
  // Split the lines into right and left lines
  double imgCenter = static_cast<double>((inpImg.cols / 2));
  while (idx < validLines.size()) {
    start = cv::Point(validLines[idx][0], validLines[idx][1]);
    end = cv::Point(validLines[idx][2], validLines[idx][3]);

    // Condition to classify line as left side or right side
    if (slopes[idx] > 0 && end.x > imgCenter && start.x > imgCenter) {
      rightLanes.push_back(validLines[idx]);
      rightLane = true;
    } else if (slopes[idx] < 0 && end.x < imgCenter && start.x < imgCenter) {
      leftLanes.push_back(validLines[idx]);
      leftLane = true;
    }
    ++idx;
  }

  output[0] = rightLanes;
  output[1] = leftLanes;


  return output;
}
