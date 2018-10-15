# Advanced Lane Detection using C++
[![Build Status](https://travis-ci.org/smahajan07/advanced-lane-detection.svg?branch=master)](https://travis-ci.org/smahajan07/advanced-lane-detection)
[![Coverage Status](https://coveralls.io/repos/github/smahajan07/advanced-lane-detection/badge.svg?branch=master)](https://coveralls.io/github/smahajan07/advanced-lane-detection?branch=master)
---

## Work In Progress

To track progress please see the [Google Spread Sheet](https://docs.google.com/spreadsheets/d/1pAFY7_X_TjFaCiD-pRBMCMxe55UK4Ttg2dLJemnxAUg/edit?usp=sharing)

## Overview

Lane detection is a critical component of the perception module for an autonomous vehicle. The objective of this module is to design and develop a lane detection algorithm which is robust and optimized. Autonomous driving is a complicated task and an interconnected web of modules running in tandem. Lane detection can further aid in motion planning and navigation and hence should provide realible data. 

For this project we will focus on the computer vision aspect of lane detection. A video feed of a car driving on a higway will be used, credits to dataset available for Udacity's Self Driving Car Nano Degree. Primary objective would be to detect lanes and then predict the turn based on the vaninshing point location.

The source code is in modern C++ and please raise an issue or request a PR if you wish to add features, recommend changes or report a bug. The build system used is Cmake in order to facilitate cross platform builds. The project also uses the OpenCV library as a dependency, so make sure it is installed before running the project and if not, please follow the provided instructions to do so.

## Installation

* Build and run demo and unit tests (Only if OpenCV is installed, else follow the steps below to install OpenCV)

```
git clone git@github.com:smahajan07/advanced-lane-detection.git
cd <path to repository>
mkdir build
cd build
cmake ..
make
./app/shell-app
```

* Steps to install OpenCV, if not installed already

```
sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
git clone https://github.com/Itseez/opencv.git
cd opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..
make -j4
sudo make install
sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig
cd ../../
```

## Doxygen Documentation

Follow the steps to generate doxygen documentation in HTML and LaTEX:

```
cd <path to repository>
mkdir docs
cd docs
doxygen - <config file name>
```

To view the documents, open the file in your browser.

## SIP (Solo Iterative Process)

A solo iterative process is characterized by a single programmer working alone.
SIP is current;y being used to manage this project. A product backlog, iteration backlog and work log and a defect log are being maintained and the progress can be tracked by the following spreadsheet. 

[Google Spread Sheet](https://docs.google.com/spreadsheets/d/1pAFY7_X_TjFaCiD-pRBMCMxe55UK4Ttg2dLJemnxAUg/edit?usp=sharing)

The project consists of two iterations, please find the details in the spreadsheet.

## License

MIT License

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
  