#include "DetectSign.h"

DetectSign2::DetectSign2() {
	// not implemented
}

void DetectSign2::detect(MAGU &magu, std::atomic<int> &res, bool debug) {
	//std::lock_guard<std::mutex> lock(mtx);

	while (true)
	{
		src = magu.getImage();
		cout << "Detect got image" << endl;

		if (src.empty()) return;
		cv::Mat hsvi, blue_hui, red_hui;

		std::vector<MatRect> res_rect;

		cv::Mat _src = src(cv::Rect(0, 0, src.cols, src.rows / 2));

		cv::medianBlur(_src, _src, 3);

		cvtColor(_src, hsvi, cv::COLOR_BGR2HSV);

		detectRed(hsvi, red_hui, res_rect);

		detectBlue(hsvi, blue_hui, res_rect);

		ident(red_hui, blue_hui, res_rect);

		std::sort(res_rect.begin(), res_rect.end(), std::greater<MatRect>());

		if (debug) {
			for (int i = 0; i < res_rect.size(); i++) {
				std::cout << i << ") " <<
					"SIGN: " << readable(res_rect[i].Result) << "; " <<
					"isRed: " << res_rect[i].isRed << "; " <<
					"isOctal: " << res_rect[i].isOctal << "; " <<
					"isTriangle: " << res_rect[i].isTriangle << "; " <<
					"isBlue: " << res_rect[i].isBlue << "; " <<
					"isCircle: " << res_rect[i].isCircle << "; " <<
					"isRect: " << res_rect[i].isRect << "; " <<
					"Rect: " << res_rect[i].bounds << "; " <<
					"Area: " << res_rect[i].bounds.area() << "; " << std::endl;

				if (res_rect[i].Result != SIGN_NOT_DEFINED) {
					cv::rectangle(_src, res_rect[i].bounds, cv::Scalar(0, 0, 255), 2);
					cv::putText(_src, std::to_string(i) + ") " + readable(res_rect[i].Result), cv::Point(res_rect[i].bounds.x, res_rect[i].bounds.y), cv::FONT_HERSHEY_PLAIN, 2.0, CV_RGB(255, 0, 255), 2);
				}
			}

			resize(_src, _src, cv::Size(0, 0), 0.5, 0.5);
			imshow("SRC", _src);
			cv::waitKey(1);
		}
		if (res_rect.size() > 0) {
			res = res_rect[0].Result;
		}
		else {
			res = SIGN_NOT_DEFINED;
		}
	}
}

std::string DetectSign2::readable(int value) {
	switch (value)
	{
	case SIGN_CIRCLE:
		return "Circle";
	case SIGN_LEFT:
		return "Left";
	case SIGN_RIGHT:
		return "Right";
	case SIGN_FORWARD:
		return "Forward";
	case SIGN_STOP:
		return "Stop";
	case SIGN_PEDESTRIAN:
		return "Pedistrian";
	case SIGN_PARKING:
		return "Parking";
	default:
		return "NOT DEFINED";
	}
}

void DetectSign2::ident(cv::Mat &rhi, cv::Mat &bhi, std::vector<MatRect> &mr) {
	for (int i = 0; i < mr.size(); i++) {
		if (mr[i].isRed && mr[i].isOctal) {
			// Octal
			cv::Rect r = mr[i].bounds;
			r.y += r.height / 3;
			r.height = r.height / 3;

			threshold(rhi(r), rhi(r), 200, 0, 3);

			int x = cv::countNonZero(rhi(r));
			int y = r.width * r.height;
			double dif = (double)x / y;

			if (dif < 0.60 && dif > 0.48) {
				mr[i].Result = SIGN_STOP;
			}
		}
		else if (mr[i].isRed && mr[i].isTriangle) {
			// Triangle
			cv::Rect r = mr[i].bounds;
			r.x += r.width / 3;
			r.y += r.height / 3;
			r.height = r.height / 3;
			r.width = r.width / 3;

			int x = cv::countNonZero(rhi(r));
			int y = r.width * r.height;
			double dif = (double)x / y;

			if (dif < 0.20) {
				mr[i].Result = SIGN_PEDESTRIAN;
			}
		}
		else if (mr[i].isBlue && mr[i].isRect) {
			// Rectangle
			int x = cv::countNonZero(bhi(mr[i].bounds));
			int y = mr[i].bounds.width * mr[i].bounds.height;
			double dif = (double)x / y;

			if (dif > 0.75 && dif < 0.90) {
				mr[i].Result = SIGN_PARKING;
			}
		}
		else if (mr[i].isBlue && mr[i].isCircle) {
			// Circle
			threshold(bhi(mr[i].bounds), bhi(mr[i].bounds), 200, 0, 3);

			double circle = (double)cv::countNonZero(bhi(mr[i].bounds)) / mr[i].bounds.area();

			if (circle > 0.30 && circle < 0.50) {
				mr[i].Result = SIGN_CIRCLE;
			}
			else {
				cv::Rect left(mr[i].bounds.x + 5, mr[i].bounds.y + (mr[i].bounds.height / 3), (mr[i].bounds.width / 5) - 5, mr[i].bounds.height / 3),
					right(mr[i].bounds.x + (mr[i].bounds.width * 4 / 5), mr[i].bounds.y + mr[i].bounds.height / 3, (mr[i].bounds.width / 5) - 5, mr[i].bounds.height / 3),
					bottom(mr[i].bounds.x + (mr[i].bounds.width / 3), mr[i].bounds.y + (mr[i].bounds.width * 4 / 5), mr[i].bounds.width / 3, (mr[i].bounds.height / 5) - 5);

				int __res__ = SIGN_LEFT;
				int right_c = cv::countNonZero(bhi(left));
				int left_c = cv::countNonZero(bhi(right));
				int forward_c = cv::countNonZero(bhi(bottom));

				int min = left_c;
				if (right_c < min) {
					min = right_c; __res__ = SIGN_RIGHT;
				}
				if (forward_c < min) {
					__res__ = SIGN_FORWARD;
				}
				mr[i].Result = __res__;
			}
		}
	}
}

void DetectSign2::detectRed(cv::Mat &hsvi, cv::Mat &hi, std::vector<MatRect> &out) {
	cv::Scalar upper(0, 110, 61), lower(14, 255, 255);
	MatRect mr;
	cv::Rect _temp;
	double __temp[2] = { 0, 0 };
	std::vector<std::vector<cv::Point>> contours;

	cv::inRange(hsvi, upper, lower, hi);

	cv::GaussianBlur(hi, hi, cv::Size(5, 5), 2, 2);

	cv::findContours(hi, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> approx(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(cv::Mat(contours[i]), approx[i], cv::arcLength(cv::Mat(contours[i]), true)*0.037, true);

		_temp = cv::boundingRect(approx[i]);
		__temp[0] = cv::contourArea(approx[i]);
		__temp[1] = (double)_temp.width / _temp.height;
		if (__temp[0] > 2000 && __temp[0] < 19000 && __temp[1]  > 0.7 && __temp[1]  < 1.3) {
			mr.bounds = _temp;
			mr.isRed = true;
			mr.isOctal = approx[i].size() >= 7 && approx[i].size() <= 9;
			mr.isTriangle = approx[i].size() >= 3 && approx[i].size() <= 4;

			out.push_back(mr);
		}
	}

}

void DetectSign2::detectBlue(cv::Mat &hsvi, cv::Mat &hi, std::vector<MatRect> &out) {
	cv::Scalar upper(67, 54, 77), lower(130, 255, 255);
	MatRect mr;
	cv::Rect _temp;
	double __temp[2] = { 0, 0 };
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec3f> circles;

	cv::inRange(hsvi, upper, lower, hi);

	cv::GaussianBlur(hi, hi, cv::Size(9, 9), 2, 2);

	cv::findContours(hi, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> approx(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(cv::Mat(contours[i]), approx[i], cv::arcLength(cv::Mat(contours[i]), true)*0.037, true);

		_temp = cv::boundingRect(approx[i]);
		__temp[0] = cv::contourArea(approx[i]);
		__temp[1] = (double)_temp.width / _temp.height;
		if (approx[i].size() == 4 && __temp[0] > 2000 && __temp[0] < 11000 && __temp[1]  > 0.7 && __temp[1]  < 1.3) {
			mr.bounds = _temp;
			mr.isBlue = true;
			mr.isRect = true;

			out.push_back(mr);
		}
	}

	cv::HoughCircles(hi, circles, CV_HOUGH_GRADIENT, 1, hi.rows / 8, 100, 20, 40, 70);

	for (size_t current_circle = 0; current_circle < circles.size(); current_circle++) {

		cv::Point center((int)std::round(circles[current_circle][0]), (int)std::round(circles[current_circle][1]));
		int radius = (int)std::round(circles[current_circle][2]);

		mr.bounds = cv::Rect(center.x - radius, center.y - radius, radius * 2, radius * 2);

		if (mr.bounds.x < 0) mr.bounds.x = 0;
		if (mr.bounds.y < 0) mr.bounds.y = 0;
		if (mr.bounds.x + mr.bounds.width > hsvi.cols) mr.bounds.width = hsvi.cols - mr.bounds.x;
		if (mr.bounds.y + mr.bounds.height > hsvi.rows) mr.bounds.height = hsvi.rows - mr.bounds.y;

		mr.isBlue = true;
		mr.isCircle = true;
		mr.isRect = false;

		out.push_back(mr);
	}

}