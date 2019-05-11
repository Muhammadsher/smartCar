#ifndef LaneTracerCam_h
#define LaneTracerCam_h

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <atomic>
#include <unistd.h>
#include <mutex>
#include "Magu.h"

using namespace std;
using namespace cv;

class LaneTracerCam {

public:
	LaneTracerCam() {

	}

	void trace(std::atomic<int> &, std::atomic<int> &, MAGU &);
private:
	mutex mtx;
};

#endif