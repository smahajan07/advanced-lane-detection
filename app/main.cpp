#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
# include "../include/lanedetector.h"

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
  Mat orgImg;
  // Provide path for test image
  std::string vidPath("project_video.mp4");
  // read test image
//  orgImg = imread(imgPath, IMREAD_COLOR);
  // open video
  cv::VideoCapture cap(vidPath);
  // check if video opened
  if (!cap.isOpened())
    return -1;
  // create a window to display image that is read
//  namedWindow("Original", WINDOW_AUTOSIZE);
//  imshow("Original", orgImg);
  // wait for key press and then close window
//  waitKey(0);
  int fCount = 0;
  // start video loop
  while (fCount < 500) {
    if (!cap.read(orgImg))
      break;
    // actions
    Mat undImg, ppImg, grayImg, edgeImg;
    // call undistort func
    undImg = obj.undistortImage(orgImg, cameraMatrix, distCoeff);
    // call gray Image func
    grayImg = obj.grayImage(undImg);
    // call detect edges
    edgeImg = obj.detectEdges(grayImg);
    // call extract ROI
    Mat roiImg;
    int h = orgImg.rows, w = orgImg.cols;
    Rect rectRoi(0.2 * w, h / 2, 0.6 * w, h / 2);
    roiImg = obj.extractROI(edgeImg, rectRoi);
    //  namedWindow("ROI image", WINDOW_AUTOSIZE);
    //  imshow("ROI image", roiImg);
    // wait for key press and then close window
    //  waitKey(0);
    // call create mask
    Mat maskedImg;
    maskedImg = obj.createMask(edgeImg);
    //  namedWindow("Masked image", WINDOW_AUTOSIZE);
    //  imshow("Masked image", maskedImg);
    // wait for key press and then close window
    //  waitKey(0);
    // call perspective transform
    Mat warpedImg;
    warpedImg = obj.perspectiveTransform(edgeImg);
    //  namedWindow("Warped image", WINDOW_AUTOSIZE);
    //  imshow("Warped image", warpedImg);
    // wait for key press and then close window
    //  waitKey(0);
    // call hough lines
    std::vector<cv::Vec4i> lines;
    lines = obj.detectLanes(maskedImg);
    //  std::cout << "Original len " << lines.size() << std::endl;
    // sort lines
    std::vector<std::vector<cv::Vec4i>> validLines;
    validLines = obj.sortLanes(lines, maskedImg);
    // call draw lines
    Mat linesImg;
    linesImg = obj.drawLines(undImg, validLines[0], validLines[1]);
//    namedWindow("Lines image", WINDOW_AUTOSIZE);
//    imshow("Lines image", linesImg);
    // wait for key press and then close window
    //  waitKey(0);
    ++fCount;
    waitKey(15);
  }
  return 0;
}
