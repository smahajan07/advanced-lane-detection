#include "../include/lanedetector.h"

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
              0.8, cvScalar(0, 255, 0), 1, CV_AA);
//  cv::line(outImg, finalPoly[0], finalPoly[1], cv::Scalar(71, 99, 255), 7,
//           CV_AA);
//  cv::line(outImg, finalPoly[2], finalPoly[3], cv::Scalar(71, 99, 255), 7,
//           CV_AA);
  cv::namedWindow("Output", CV_WINDOW_AUTOSIZE);
  cv::imshow("Output", inpImg);

  return 0;
}
