#include "DetectSign.h"
using namespace cv;
using namespace std;

DS::DS() {
	cascade_circle.load("./HAAR/circle_cascade.xml");
	cascade_forward.load("./HAAR/forward_cascade.xml");
	cascade_left.load("./HAAR/left_cascade.xml");
	cascade_right.load("./HAAR/right_cascade.xml");
	cascade_stop.load("./HAAR/stop_cascade.xml");
	cascade_parking.load("./HAAR/parking_cascade.xml");
	cascade_pedestrian.load("./HAAR/becareful_cascade.xml");
}

double DS::angle(Point pt1, Point pt2, Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

Mat DS::CropImage(Mat &img, const vector<Point> &vec) {
	Rect boundRect = cv::boundingRect(cv::Mat(vec));
	cv::Mat	cropImage = img(boundRect);
	return cropImage;
}

int DS::recog(Mat &cropImage, size_t vec) {
	if (vec == 3) {
		cascade_pedestrian.detectMultiScale(cropImage, res_cas_pedestrian, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));
		if (res_cas_pedestrian.size() > 0)
			return SIGN_PEDESTRIAN;
	}
	else if (vec == 4) {
		cascade_parking.detectMultiScale(cropImage, res_cas_parking, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));
		if (res_cas_parking.size() > 0)
			return SIGN_PARKING;
	}
	else if (vec == 8) {

		cascade_stop.detectMultiScale(cropImage, res_cas_stop, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));
		if (res_cas_stop.size() > 0)
			return SIGN_STOP;

		cascade_circle.detectMultiScale(cropImage, res_cas_circle, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));
		if (res_cas_circle.size() > 0)
			return SIGN_CIRCLE;

		cascade_right.detectMultiScale(cropImage, res_cas_right, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));
		if (res_cas_right.size() > 0)
			return SIGN_RIGHT;

		cascade_left.detectMultiScale(cropImage, res_cas_left, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));
		if (res_cas_left.size() > 0)
			return SIGN_LEFT;

		cascade_forward.detectMultiScale(cropImage, res_cas_forward, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));
		if (res_cas_forward.size() > 0)
			return SIGN_FORWARD;

	}

	return SIGN_NOT_DEFINED;
}

char * DS::readable(int res) {
	switch (res)
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

/**  @function Erosion  */
void DS::Erosion(Mat &src, int erosion_elem, int erosion_size)
{
	int erosion_type;
	if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
	else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	erode(src, src, element);
}

/** @function Dilation */
void DS::Dilation(Mat &src, int dilation_elem, int dilation_size)
{
	int dilation_type;
	if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
	else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(dilation_type,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	dilate(src, src, element);
}

int DS::detect(Mat &src) {
	if (src.empty()) return SIGN_NOT_DEFINED;

	Rect boundRect;

	Mat dst, hsv_image, lower_red_hue_range, upper_red_hue_range, blue_hue_image, hue_image, cr;

	vector<vector<Point> > contours;

	int res;

	double aprx;

	// Convert RGB to HSV
	cvtColor(src, hsv_image, COLOR_BGR2HSV);

	// Get red shapes
	inRange(hsv_image, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(hsv_image, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);

	// Get blue shapes
	inRange(hsv_image, Scalar(103, 90, 50), Scalar(130, 255, 255), blue_hue_image);

	// Combine three images
	addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, hue_image);
	addWeighted(hue_image, 1.0, blue_hue_image, 1.0, 0.0, hue_image);

	Dilation(hue_image, 1, 5);
	//Erosion(hue_image, 0, 0);

	// Find contours
	findContours(hue_image.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> approx(contours.size());

	dst = src.clone();

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx[i], arcLength(Mat(contours[i]), true)*0.025, true);
		cr = CropImage(src, contours[i]);
		aprx = (double)cr.cols / cr.rows;

		if (aprx < 0.7 || aprx > 1.3 || (cr.cols * cr.rows) < 8000)
			continue;

		if (approx[i].size() == 3 || approx[i].size() == 4)
		{
			res = recog(cr, approx[i].size());
			if (res == SIGN_NOT_DEFINED) continue;
			return res;
		}
		else {
			res = recog(cr, 8);

			if (res == SIGN_LEFT || res == SIGN_RIGHT) {
				int left_nonzero = countNonZero(hue_image(Rect(0, hue_image.rows / 3, hue_image.cols / 5, hue_image.rows / 3)));
				int right_nonzero = countNonZero(hue_image(Rect(4 * hue_image.cols / 5, hue_image.rows / 3, hue_image.cols / 5, hue_image.rows / 3)));

				rectangle(hue_image, Rect(0, hue_image.rows / 3, hue_image.cols / 5, hue_image.rows / 3), Scalar(0, 0, 0));
				rectangle(hue_image, Rect(4 * hue_image.cols / 5, hue_image.rows / 3, hue_image.cols / 5, hue_image.rows / 3), Scalar(0, 0, 0));
				
				if (left_nonzero > right_nonzero)
					return SIGN_LEFT;
				else
					return SIGN_RIGHT;
			}

			if (res == SIGN_NOT_DEFINED) continue;
			return res;
		}
	}
	return SIGN_NOT_DEFINED;
}

int DS::trafficControl(Mat &src) {
	if (src.empty()) return SIGN_NOT_DEFINED;

	Mat hi, hsvi;
	vector<vector<Point> > contours;

	cvtColor(src, hsvi, COLOR_BGR2HSV);

	inRange(hsvi, Scalar(0, 0, 255), Scalar(255, 200, 255), hi);

	Dilation(hi, 1, 2);
	Erosion(hi, 1, 1);
	Erosion(hi, 1, 1);

	findContours(hi, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	return contours.size() < 3 && contours.size() > 0;
}