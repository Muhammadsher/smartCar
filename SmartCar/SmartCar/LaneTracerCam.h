#ifndef LaneTracerCam_h
#define LaneTracerCam_h

#include "Motor.h"
#include <atomic>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <thread>
#include "opencv2/opencv.hpp"
#include <raspicam/raspicam_cv.h>
#include <unistd.h>

using namespace std;
using namespace cv;

class LaneTracerCam {

public:
	LaneTracerCam() {

	}

	void trace(Motor, std::atomic<int> &, std::atomic<int> &);

};

#endif