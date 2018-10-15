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
 *@file lanedetector.cpp
 *@author Sarthak Mahajan
 *@brief All methods for class lanedetector are defined here.
 *@brief Primary objective of an object of this class is to
 *@brief detect lanes in a given frame and return a frame
 *@brief with an augmented lane overlay for better visualization.
 */
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../include/lanedetector.h"

// Constructor
/**
 *@brief Initializes the private members of the class
 *@param None
 *@return None
 */

lanedetector::lanedetector() {
  leftLaneF = false;
  rightLaneF = false;
  leftSlope = 0;
  rightSlope = 0;
  leftBias = cv::Point(0, 0);
  rightBias = cv::Point(0, 0);
  imgCenter = 0;
  vanPtThresh = 12;
}

// Undistortion
/**
 *@brief Due to radial distortion, straight lines appear as curved.
 *@brief This function undistorts the input image provided the intrinsic
 *@brief camera paramters. OpenCV provides an inbuilt function to perform
 *@brief this operation.
 *@param inpImg is the input image that needs to be un distorted
 *@param cameraMatrix is an intrinsic camera parameter with focal length
 *@param and optical center values
 *@param distCoeff are the distortion coefficients and are required to
 *@param undistort the image
 *@return undistorted image
 */
cv::Mat lanedetector::undistortImage(cv::Mat inpImg, cv::Mat cameraMatrix,
                                     cv::Mat distCoeff) {
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
cv::Mat lanedetector::preprocessImage(cv::Mat inpImg) {
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
cv::Mat lanedetector::grayImage(cv::Mat inpImg) {
  cv::Mat outImg;
  cv::cvtColor(inpImg, outImg, cv::COLOR_BGR2GRAY);

  return outImg;
}

// Detect Edges
/**
 *@brief Detect edges in an gray scale image. Since the ultimate objective
 *@brief is to detect lanes, it is easier and efficient to compute straight
 *@brief lines from edges extracted from an image
 *@param inpImg is a grayscale image
 *@return image with extracted edges
 */
cv::Mat lanedetector::detectEdges(cv::Mat inpImg) {
  cv::Mat outImg;
  Canny(inpImg, outImg, 50, 100, 3);
  // adding dilation to see if results improve
  // cv::dilate(outImg, outImg, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);

  return outImg;
}

// Extract Region of Interest
/**
 *@brief extract a region of interest from an image
 *@brief similar to cropping a region of interest from an image
 *@param inpImg is the input image from which we wish to extract a ROI
 *@param rectRoi is an object of the type cv::Rect and is essentially just
 *@param a set of four points which we will use as vertices for extracting
 *@param the region of interest
 *@return an image which has been extracted from the input image
 */
cv::Mat lanedetector::extractROI(cv::Mat inpImg, cv::Rect rectRoi) {
  cv::Mat outImg;
  outImg = inpImg(rectRoi);

  return outImg;
}

// Create Mask
/**
 *@brief Mask the image so that only a certain portion of the image has active
 *@brief contents, the rest are set to inactive region, say set to zero. This
 *@brief is similar to what extractROI is doing except that we retain the dimensions
 *@brief of the original image and only the region of interest is active.
 *@param inpImg is the input image that we need to mask to bring out only a ROI
 *@return masked image with dimensions same as input image
 */
cv::Mat lanedetector::createMask(cv::Mat inpImg) {
  cv::Mat outImg;
  cv::Mat mask = cv::Mat::zeros(inpImg.size(), inpImg.type());
  cv::Point vertices[4] = { cv::Point(210, 650), cv::Point(550, 450), cv::Point(
      717, 450), cv::Point(1280, 650) };
  cv::fillConvexPoly(mask, vertices, 4, cv::Scalar(255, 0, 0));
  cv::bitwise_and(inpImg, mask, outImg);

  return outImg;
}

// Perspective Transform
/**
 *@brief Warping an image to get a top view or bird's eye view. To improve
 *@brief accuracy of detecting straight lines we can project the ROI with
 *@brief lanes in the bird's eye view.
 *@param inpImg is the input image which needs to projected in top view
 *@return warped image (in the top view)
 */
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

// Detect Lanes
/**
 *@brief detects straight lines in a given image
 *@param inpImg is the image from which we need to detect straight lines
 *@return a vector of a four dimensional data structure which holds the
 *@return x and y coordinates of the start and end point of a line
 */
std::vector<cv::Vec4i> lanedetector::detectLanes(cv::Mat inpImg) {
  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(inpImg, lines, 1, CV_PI / 180, 20, 20, 30);

  return lines;
}

// Draw Lines
/**
 *@brief Used for debugging. Draws lines on top of an image
 *@param inpImg is the input image on top of which lines will be drawn
 *@param lines is a vector of four dimensional data structure and contains
 *@param x and y coordinates of start and end points depicting a line
 *@return input image with overlayed lines
 */
cv::Mat lanedetector::drawLines(cv::Mat inpImg, std::vector<cv::Vec4i> lines) {
  cv::Mat outImg = inpImg;
  for (auto l : lines) {
    cv::line(outImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
             cv::Scalar(0, 0, 255), 3, CV_AA);
  }

  return outImg;
}
// Draw Lines
/**
 *@brief Used for debugging. Draws lines on top of an image
 *@param inpImg is the input image on top of which lines will be drawn
 *@param leftLines is a vector of four dimensional data structure and contains
 *@param x and y coordinates of start and end points depicting a line
 *@param rightLines is a vector of four dimensional data structure and contains
 *@param x and y coordinates of start and end points depicting a line
 *@return input image with overlayed lines
 */
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

// Sort Lanes
/**
 *@brief Selects only valid lines from all detected lines and sorts them
 *@brief into potentially left lane or right lane based on their slope
 *@param lines is a vector of four dimensional data structure and contains
 *@param x and y coordinates of start and end points depicting a line
 *@param inpImg is the input image from which these lines have been calculated
 *@return a two dimensional vector of a four dimensional data structure and contains
 *@return x and y coordinates of start and end points depicting a line
 */
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
      rightLaneF = true;
    } else if (slopes[idx] < 0 && end.x < imgCenter && start.x < imgCenter) {
      leftLanes.push_back(validLines[idx]);
      leftLaneF = true;
    }
    ++idx;
  }

  output[0] = rightLanes;
  output[1] = leftLanes;

  return output;
}

// Compute Fit Line
/**
 *@brief performing basic regression to fit a line for left lane and
 *@brief right lane from all the sorted lines output from sortLanes function
 *@param validLines is a two dimensional vector of a four dimensional data structure
 *@param and contains x and y coordinates of start and end points depicting a line
 *@param inpImg is the input images on which the lines were computed
 *@return a vector of four points representing the final two lines for left
 *@return and right respectively
 */

std::vector<cv::Point> lanedetector::computeFitLine(
    std::vector<std::vector<cv::Vec4i>> validLines, cv::Mat inpImg) {
  // First collect points on right and left lanes
  // Then use cv::fitLine to compute a best fit line for these points
  // then we can simply extrapolate these points acc to our need
  std::vector<cv::Point> output(4);
  cv::Point startLeft, endLeft;
  cv::Point startRight, endRight;
  cv::Vec4d rightLane;
  cv::Vec4d leftLane;
  std::vector<cv::Point> rightPts;
  std::vector<cv::Point> leftPts;
  imgCenter = (inpImg.cols) / 2;
  // If right lines are being detected, fit a line using all the init and final points of the lines
  if (rightLaneF == true) {
    for (auto i : validLines[0]) {
      startRight = cv::Point(i[0], i[1]);
      endRight = cv::Point(i[2], i[3]);

      rightPts.push_back(startRight);
      rightPts.push_back(endRight);
    }

    if (rightPts.size() > 0) {
      // The right line is formed here
      cv::fitLine(rightPts, rightLane, CV_DIST_L2, 0, 0.01, 0.01);
      rightSlope = rightLane[1] / rightLane[0];
      rightBias = cv::Point(rightLane[2], rightLane[3]);
    }
  }

  // If left lines are being detected, fit a line using all the init and final points of the lines
  if (leftLaneF == true) {
    for (auto j : validLines[1]) {
      startLeft = cv::Point(j[0], j[1]);
      endLeft = cv::Point(j[2], j[3]);

      leftPts.push_back(startLeft);
      leftPts.push_back(endLeft);
    }

    if (leftPts.size() > 0) {
      // The left line is formed here
      cv::fitLine(leftPts, leftLane, CV_DIST_L2, 0, 0.01, 0.01);
      leftSlope = leftLane[1] / leftLane[0];
      leftBias = cv::Point(leftLane[2], leftLane[3]);
    }
  }

  // One the slope and offset points have been obtained, apply the line equation to obtain the line points
  int startY = inpImg.rows;
  int endY = 470;

  double startRightX = ((startY - rightBias.y) / rightSlope) + rightBias.x;
  double endRightX = ((endY - rightBias.y) / rightSlope) + rightBias.x;

  double startLeftX = ((startY - leftBias.y) / leftSlope) + leftBias.x;
  double endLeftX = ((endY - leftBias.y) / leftSlope) + leftBias.x;

  output[0] = cv::Point(startRightX, startY);
  output[1] = cv::Point(endRightX, endY);
  output[2] = cv::Point(startLeftX, startY);
  output[3] = cv::Point(endLeftX, endY);

  return output;
}

// Predict Turn
/**
 *@brief Predicts turn based on the location of the vanishing point. Vanishing
 *@brief point is computed by finding the intersection of the final left lane
 *@brief and right lane
 *@param None
 *@return direction of predicted turn, "Left", "Straight" or "Right"
 */
std::string lanedetector::predictTurn() {
  std::string turn;
  // vanishing point
  double vanPt;
  vanPt = static_cast<double>(((rightSlope * rightBias.x)
      - (leftSlope * leftBias.x) - rightBias.y + leftBias.y)
      / (rightSlope - leftSlope));
  if (vanPt < imgCenter - vanPtThresh)
    turn = "LEFT";
  else if (vanPt > imgCenter + vanPtThresh)
    turn = "RIGHT";
  else if (vanPt < (imgCenter + vanPtThresh)
      && vanPt > (imgCenter - vanPtThresh))
    turn = "STRAIGHT";

  return turn;
}

// Draw Polygon
/**
 *@brief Augments the detected lane with a colored polygon for better
 *@brief visualization. Also adds text received from the predictTurn function
 *@param inpImg is the input image which will be overlayed with the augmented box
 *@param finalPoly is the set (or vector) of points depicting the final polygon
 *@param computed from the left and right lane
 *@param turn is the output that was extracted from predictTurn function
 *@return None
 */
int lanedetector::drawPolygon(cv::Mat inpImg, std::vector<cv::Point> finalPoly,
                std::string turn) {
  cv::Mat outImg;
  inpImg.copyTo(outImg);
  std::vector<cv::Point> filledPoly;

  filledPoly.push_back(finalPoly[2]);
  filledPoly.push_back(finalPoly[0]);
  filledPoly.push_back(finalPoly[1]);
  filledPoly.push_back(finalPoly[3]);

  cv::fillConvexPoly(outImg, filledPoly, cv::Scalar(71, 99, 255), CV_AA, 0);
  cv::addWeighted(outImg, 0.45, inpImg, 1.0 - 0.45, 0.0, inpImg);
  cv::putText(inpImg, turn, cv::Point(610, 610),
              cv::FONT_HERSHEY_COMPLEX_SMALL,
              0.8, cv::Scalar(0, 255, 0), 1, CV_AA);
//  cv::line(outImg, finalPoly[0], finalPoly[1], cv::Scalar(71, 99, 255), 7,
//           CV_AA);
//  cv::line(outImg, finalPoly[2], finalPoly[3], cv::Scalar(71, 99, 255), 7,
//           CV_AA);
  cv::namedWindow("Output", CV_WINDOW_AUTOSIZE);
  cv::imshow("Output", inpImg);

  return 0;
}
