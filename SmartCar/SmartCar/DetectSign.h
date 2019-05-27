#ifndef DetectSign2_H
#define DetectSign2_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <atomic>
#include "Magu.h"
#include <math.h>
#include <thread>


class DetectSign2 {
#define SIGN_NOT_DEFINED 0
#define SIGN_CIRCLE 1
#define SIGN_LEFT 2
#define SIGN_RIGHT 3
#define SIGN_FORWARD 4
#define SIGN_STOP 5
#define SIGN_PEDESTRIAN 6
#define SIGN_PARKING 7
#define NO_ROAD 8
#define TRAFIC_CONTROL_RED 9


#define M_PI 3.14159265358979323846
public:
	DetectSign2();
	void detect(MAGU &, std::atomic<int> &, bool);
	std::string readable(int);

	struct MatRect
	{
		int Result = SIGN_NOT_DEFINED;

		cv::Rect bounds;

		bool isRed = false;
		bool isOctal = false;
		bool isTriangle = false;

		bool isBlue = false;
		bool isCircle = false;
		bool isRect = false;

		bool operator<(const MatRect& a) const
		{
			return bounds.area() < a.bounds.area();
		}

		bool operator>(const MatRect& a) const
		{
			return bounds.area() > a.bounds.area();
		}
	};

private:
	cv::Mat src;

	cv::CascadeClassifier cascade_obstacle, cascade_left,
		cascade_right, cascade_stop,
		cascade_parking, cascade_pedestrian;

	cv::Scalar Range_red_upper, Range_red_lower,
		Range_red_upper2, Range_red_lower2,
		Range_blue_upper, Range_blue_lower,
		Range_blue_upper2, Range_blue_lower2,
		Range_yellow_upper, Range_yellow_lower,
		Range_darkgreen_upper, Range_darkgreen_lower,
		Range_tc_upper, Range_tc_lower,
		Range_white_upper, Range_white_lower;
	
	int MedianBlurSize;
	std::string print(MatRect &);

	void fix(cv::Rect &, int, int);
	void ident(cv::Mat &, std::vector<cv::Mat> &, std::vector<MatRect> &);
	void detectTC(cv::Mat &, cv::Mat &, cv::Mat &, std::vector<MatRect> &);
	void detectRed(cv::Mat &, cv::Mat &, cv::Mat &, std::vector<MatRect> &);
	void detectBlue(cv::Mat &, cv::Mat &, cv::Mat &, std::vector<MatRect> &);
};

#endif
