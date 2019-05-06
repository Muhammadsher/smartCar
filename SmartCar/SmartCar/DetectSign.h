#ifndef DetectSign_H
#define DetectSign_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include "opencv2/opencv.hpp" 
#include <cmath>

class DS {
#define SIGN_NOT_DEFINED 0
#define SIGN_CIRCLE 1
#define SIGN_LEFT 2
#define SIGN_RIGHT 3
#define SIGN_FORWARD 4
#define SIGN_STOP 5
#define SIGN_PEDESTRIAN 6
#define SIGN_PARKING 7
public:
	DS();
	int detect(cv::Mat &);
	char * readable(int);
	void Erosion(cv::Mat &, int, int);
	void Dilation(cv::Mat &, int, int);
	int trafficControl(cv::Mat &);

private:
	cv::CascadeClassifier cascade_circle, cascade_left, cascade_right, cascade_forward, cascade_stop, cascade_parking, cascade_pedestrian;
	std::vector<cv::Rect> res_cas_circle, res_cas_left, res_cas_right, res_cas_forward, res_cas_stop, res_cas_parking, res_cas_pedestrian;

	double angle(cv::Point, cv::Point, cv::Point);
	cv::Mat CropImage(cv::Mat &, const std::vector<cv::Point> &);
	int recog(cv::Mat &, size_t);
};

#endif