#include "DetectSign.h"

DetectSign2::DetectSign2() {
	
	cascade_obstacle.load("./HAAR/obstacle_cascade.xml");
	cascade_left.load("./HAAR/left_cascade.xml");
	cascade_right.load("./HAAR/right_cascade.xml");
	cascade_stop.load("./HAAR/stop_cascade.xml");
	cascade_parking.load("./HAAR/parking_cascade.xml");
	cascade_pedestrian.load("./HAAR/becareful_cascade.xml");

	Range_red_upper = cv::Scalar(0, 168, 66);
	Range_red_lower = cv::Scalar(16, 255, 255);

	Range_red_upper2 = cv::Scalar(0, 14, 41);
	Range_red_lower2 = cv::Scalar(7, 255, 255);

	Range_blue_upper = cv::Scalar(90, 82, 28);
	Range_blue_lower = cv::Scalar(134, 255, 255);

	Range_blue_upper2 = cv::Scalar(49, 86, 39);
	Range_blue_lower2 = cv::Scalar(150, 214, 255);

	Range_yellow_upper = cv::Scalar(0, 169, 185);
	Range_yellow_lower = cv::Scalar(255, 255, 255);

	Range_white_upper = cv::Scalar(0, 0, 160);
	Range_white_lower = cv::Scalar(51, 165, 255);

	Range_darkgreen_upper = cv::Scalar(0, 0, 0);
	Range_darkgreen_lower = cv::Scalar(255, 255, 40);

	Range_tc_upper = cv::Scalar(0, 0, 118);
	Range_tc_lower = cv::Scalar(7, 255, 255);

	MedianBlurSize = 3;
}

void DetectSign2::detect(MAGU &magu, std::atomic<int> &res, bool debug) {
	while (true)
	{
		src = magu.getImage();
		//cout << "Detect got image" << endl;
		cv::Mat hsvi, blue_hui, red_hui, green_hui, tc_hui;

		std::vector<cv::Mat> huis(4);

		std::vector<MatRect> res_rect;

		//cv::Mat _src = src.clone();
		cv::Mat _src = src(cv::Rect(0, 0, src.cols, src.rows / 2)).clone();

		cv::resize(_src, _src, cv::Size(0, 0), 0.5, 0.5);

		cv::medianBlur(_src, _src, MedianBlurSize);

		cvtColor(_src, hsvi, cv::COLOR_BGR2HSV);

		detectRed(_src, hsvi, huis[0], res_rect);

		detectBlue(_src, hsvi, huis[1], res_rect);

		detectTC(_src, hsvi, huis[3], res_rect);

		ident(_src, huis, res_rect);

		std::sort(res_rect.begin(), res_rect.end(), std::greater<MatRect>());

		std::vector<MatRect>::iterator newIter = std::remove_if(res_rect.begin(), res_rect.end(), [](MatRect x) { return x.Result == 0; });
		res_rect.resize(newIter - res_rect.begin());


		if (debug) {
			for (size_t i = 0; i < res_rect.size(); i++) {
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

			//resize(_src, _src, cv::Size(0, 0), 0.5, 0.5);
			imshow("SRC", _src);
			int c = cv::waitKey(10);
			if (c == 27) {
				res = NO_ROAD;
				continue;
			}
			else if (c == 120) {
				res = SIGN_FORWARD;
				continue;
			}
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
		return "Circle"; // Done
	case SIGN_LEFT:
		return "Left"; // Done
	case SIGN_RIGHT:
		return "Right"; // Done
	case SIGN_FORWARD:
		return "Forward"; // Done
	case SIGN_STOP:
		return "Stop"; // Done
	case SIGN_PEDESTRIAN:
		return "Pedistrian"; // Done
	case SIGN_PARKING:
		return "Parking"; // Done
	case NO_ROAD:
		return "No road";
	case TRAFIC_CONTROL_RED:
		return "Trafic control RED";
	default:
		return "NOT DEFINED";
	}
}

void DetectSign2::ident(cv::Mat &src, std::vector<cv::Mat> &hi, std::vector<MatRect> &mr) {
	std::vector<cv::Rect> res, _res;
	for (size_t i = 0; i < mr.size(); i++) {
		res.clear();
		if (mr[i].isRed && mr[i].isOctal) {
			cascade_stop.detectMultiScale(src(mr[i].bounds), res, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE);

			if (res.size() > 0)
				mr[i].Result = SIGN_STOP;
		}
		else if (mr[i].isRed && mr[i].isTriangle) {
			cascade_pedestrian.detectMultiScale(src(mr[i].bounds), res, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE);

			if (res.size() > 0)
				mr[i].Result = SIGN_PEDESTRIAN;
		}
		else if (mr[i].isBlue && mr[i].isRect) {
			cascade_parking.detectMultiScale(src(mr[i].bounds), res, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE);

			if (res.size() > 0)
				mr[i].Result = SIGN_PARKING;
		}
		else if (mr[i].isBlue && mr[i].isCircle) {

			//threshold(hi[1](mr[i].bounds), hi[1](mr[i].bounds), 200, 0, 3);

			cascade_left.detectMultiScale(src(mr[i].bounds), res, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE);

			cascade_right.detectMultiScale(src(mr[i].bounds), _res, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE);

			if (res.size() == _res.size()) {

				cv::Rect left(mr[i].bounds.x + 5, mr[i].bounds.y + (mr[i].bounds.height / 3), (mr[i].bounds.width / 5) - 5, mr[i].bounds.height / 3),
					right(mr[i].bounds.x + (mr[i].bounds.width * 4 / 5), mr[i].bounds.y + mr[i].bounds.height / 3, (mr[i].bounds.width / 5) - 5, mr[i].bounds.height / 3);

				//imshow("HI[1]", hi[1](mr[i].bounds));

				fix(left, hi[1].cols, hi[1].rows);
				fix(right, hi[1].cols, hi[1].rows);

				int right_c = cv::countNonZero(hi[1](left));
				int left_c = cv::countNonZero(hi[1](right));

				if (left_c < right_c)
				{
					mr[i].Result = SIGN_LEFT;
				}
				else if (left_c > right_c) {
					mr[i].Result = SIGN_RIGHT;
				}
				else
					mr[i].Result = SIGN_NOT_DEFINED;
			}
			else if (res.size() > _res.size()) {
				mr[i].Result = SIGN_LEFT;
			}
			else {
				mr[i].Result = SIGN_RIGHT;
			}
		}
		else if (mr[i].isRed && mr[i].isCircle) {
			mr[i].Result = TRAFIC_CONTROL_RED;
		}
	}
}

std::string DetectSign2::print(MatRect &mr) {
	return "isRed: " + std::to_string(mr.isRed) + "; " +
		"isOctal: " + std::to_string(mr.isOctal) + "; " +
		"isTriangle: " + std::to_string(mr.isTriangle) + "; " +
		"isBlue: " + std::to_string(mr.isBlue) + "; " +
		"isCircle: " + std::to_string(mr.isCircle) + "; " +
		"isRect: " + std::to_string(mr.isRect) + "; " +
		//"Rect: " + std::string(mr.bounds) + "; " +
		"Area: " + std::to_string(mr.bounds.area()) + "; ";
}

void DetectSign2::fix(cv::Rect &r, int cols, int rows) {
	if (r.x < 0) r.x = 0;
	if (r.y < 0) r.y = 0;
	if (r.x + r.width > cols) { r.x = cols; r.width = 0; }
	if (r.y + r.height > rows) { r.y = rows; r.height = 0; }
}

void DetectSign2::detectTC(cv::Mat &src, cv::Mat &hsvi, cv::Mat &hi, std::vector<MatRect> &out) {

	MatRect mr;
	cv::Mat res, gray, blurred, thresh1;
	std::vector<std::vector<cv::Point>> contours;

	cv::inRange(hsvi, Range_tc_upper, Range_tc_lower, hi);

	cv::bitwise_and(src, src, res, hi);

	cv::cvtColor(res, gray, cv::COLOR_BGR2GRAY);

	cv::GaussianBlur(gray, blurred, cv::Size(35, 35), 0);

	cv::threshold(blurred, thresh1, 127, 150, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

	cv::findContours(thresh1, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	cv::Rect rect;
	double area, dif, per, T;
	for (size_t i = 0; i < contours.size(); i++) {
		rect = cv::boundingRect(contours[i]);
		if (rect.width == src.cols && rect.height == src.rows) continue;

		per = cv::arcLength(contours[i], true);
		area = cv::contourArea(contours[i]);
		dif = (double)rect.width / rect.height;
		T = 4 * M_PI * (area / (per*per));

		std::cout << "[TC (" << i << ") ] => T: " << T << "; Area: " << area << "; Rect: " << rect << "; Dif: " << dif << ";" << std::endl;
		if (area > 1700 && area < 3000 && dif > 0.7 && dif < 1.2 && T > 0.80) {

			mr.bounds = rect;
			mr.isRed = true;
			mr.isCircle = true;

			out.push_back(mr);
		}
	}
}

void DetectSign2::detectRed(cv::Mat &src, cv::Mat &hsvi, cv::Mat &hi, std::vector<MatRect> &out) {

	MatRect mr;
	cv::Mat res, gray, blurred, thresh1, _hi;
	std::vector<std::vector<cv::Point>> contours;

	cv::inRange(hsvi, Range_red_upper, Range_red_lower, hi);
	cv::inRange(hsvi, Range_red_upper2, Range_red_lower2, _hi);

	cv::addWeighted(hi, 1.0, _hi, 1.0, 0.0, hi);

	cv::bitwise_and(src, src, res, hi);

	cv::cvtColor(res, gray, cv::COLOR_BGR2GRAY);

	cv::GaussianBlur(gray, blurred, cv::Size(35, 35), 0);

	cv::threshold(blurred, thresh1, 127, 150, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

	cv::findContours(thresh1, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	cv::Rect rect;
	double area, dif;
	std::vector<std::vector<cv::Point>> approx(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		rect = cv::boundingRect(contours[i]);
		if (rect.width == src.cols && rect.height == src.rows) continue;

		area = cv::contourArea(contours[i]);
		dif = (double)rect.width / rect.height;

		std::cout << "[RED (" << i << ") ] => Area: " << area << "; Rect: " << rect << "; Dif: " << dif << ";" << std::endl;

		if (area > 2000 && area < 6500 && dif > 0.7 && dif < 1.2) {
			approxPolyDP(cv::Mat(contours[i]), approx[i], cv::arcLength(cv::Mat(contours[i]), true)*0.03, true);

			mr.bounds = rect;
			mr.isRed = true;
			mr.isOctal = approx[i].size() >= 7 && approx[i].size() <= 9;
			mr.isTriangle = approx[i].size() >= 3 && approx[i].size() <= 4;

			out.push_back(mr);
		}
	}
}

void DetectSign2::detectBlue(cv::Mat &src, cv::Mat &hsvi, cv::Mat &hi, std::vector<MatRect> &out) {

	MatRect mr;
	cv::Mat res, gray, blurred, thresh1, _hi;
	std::vector<std::vector<cv::Point>> contours;

	cv::inRange(hsvi, Range_blue_upper, Range_blue_lower, hi);
	cv::inRange(hsvi, Range_blue_upper2, Range_blue_lower2, _hi);

	cv::addWeighted(hi, 1.0, _hi, 1.0, 0.0, hi);

	cv::bitwise_and(src, src, res, hi);

	cv::cvtColor(res, gray, cv::COLOR_BGR2GRAY);

	cv::GaussianBlur(gray, blurred, cv::Size(35, 35), 0);

	cv::threshold(blurred, thresh1, 127, 150, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

	cv::findContours(thresh1, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	cv::Rect rect;
	double area, dif, per, T;
	std::vector<std::vector<cv::Point>> approx(contours.size());
	std::vector<cv::Vec3f> circles;

	for (size_t i = 0; i < contours.size(); i++) {
		rect = cv::boundingRect(contours[i]);
		if (rect.width == src.cols && rect.height == src.rows) continue;

		per = cv::arcLength(contours[i], true);
		area = cv::contourArea(contours[i]);
		dif = (double)rect.width / rect.height;
		T = 4 * M_PI * (area / (per*per));

		//cv::rectangle(src, rect, cv::Scalar(0, 0, 255), 10);
		//cv::putText(src, "(" + std::to_string(i) + ") ", cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_PLAIN, 2.0, CV_RGB(255, 0, 0), 2);



		std::cout << "[BLUE (" << i << ") ] => T: " << T << "; Area: " << area << "; Rect: " << rect << "; Dif: " << dif << ";" << std::endl;

		if (area > 2000 && area < 10000 && dif > 0.7 && dif < 1.2) {

			//cv::drawContours(src, approx, i, cv::Scalar(0, 255, 0), 10);

			mr.bounds = rect;
			mr.isBlue = true;
			mr.isRect = !(mr.isCircle = (T > 0.80));

			out.push_back(mr);
		}
	}
}