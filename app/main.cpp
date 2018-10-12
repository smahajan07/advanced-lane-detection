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
  std::string imgPath("image001.png");
  // read test image
  orgImg = imread(imgPath, IMREAD_COLOR);
  // create a window to display image that is read
  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", orgImg);
  // wait for key press and then close window
  waitKey(0);
  // actions
  Mat undImg, ppImg, grayImg, edgeImg;
  // call undistort func
  undImg = obj.undistortImage(orgImg, cameraMatrix, distCoeff);
  // call gray Image func
  grayImg = obj.grayImage(undImg);
  // call detect edges
  edgeImg = obj.detectEdges(grayImg);
  namedWindow("Edges", WINDOW_AUTOSIZE);
  imshow("Edges", edgeImg);
  // wait for key press and then close window
  waitKey(0);

  return 0;
}
