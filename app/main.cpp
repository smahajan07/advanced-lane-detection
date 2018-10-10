#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main()
{
  std::cout<<"Trying to import opencv"<<std::endl;
  Mat img;
  // Provide path for test image
  std::string imgPath("image001.png");
  // read test image
  img = imread(imgPath, IMREAD_COLOR);
  // create a window to display image that is read
  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", img);
  // wait for key press and then close window
  waitKey(0);
  return 0;
}
