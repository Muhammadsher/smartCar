#ifndef DetectSign2_H
#define DetectSign2_H

#include "opencv2/opencv.hpp"
#include "Magu.h"
#include <atomic>
#include <mutex>

class DetectSign2 {
#define SIGN_NOT_DEFINED 0
#define SIGN_CIRCLE 1
#define SIGN_LEFT 2
#define SIGN_RIGHT 3
#define SIGN_FORWARD 4
#define SIGN_STOP 5
#define SIGN_PEDESTRIAN 6
#define SIGN_PARKING 7

public:
	DetectSign2();
	void detect(MAGU &, std::atomic<int> &, bool debug = false);
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
	std::mutex mtx;
	cv::Mat src;
	void ident(cv::Mat &, cv::Mat &, std::vector<MatRect> &);
	void detectRed(cv::Mat &, cv::Mat &, std::vector<MatRect> &);
	void detectBlue(cv::Mat &, cv::Mat &, std::vector<MatRect> &);
};

#endif