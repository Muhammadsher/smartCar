
#ifndef Magu_h
#define Magu_h

#include <raspicam/raspicam_cv.h>
#include "opencv2/opencv.hpp"
#include <unistd.h>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;
using namespace cv;

class MAGU {
public:
	MAGU();
	~MAGU();
	Mat getImage();
private:
	mutex mtxCamera;
	Mat frame;
	VideoCapture cap;
	raspicam::RaspiCam_Cv camera;
};

#endif
