#include "LaneTracerCam.h"


void LaneTracerCam::trace(atomic<int> &left, atomic<int> &right, std::atomic<int> &crosswalk, MAGU &magu) {

	Mat frame, whiteLane, yellowLane, LinesImg, yellowLane_, HSV_Img;

	while (true)
	{
		frame = magu.getImage();

		//cout << "Lane tracer got image" << endl;

		frame = frame(Rect(0, frame.rows / 2, frame.cols, frame.rows / 2));

		//here we define our region of interest
		Rect const box(frame.cols / 4, frame.rows / 2, frame.cols / 2, frame.rows / 2);

		Mat ROI = frame(box).clone();

		cv::medianBlur(ROI, ROI, 5);
		// convert our img to HSV Space
		cvtColor(ROI, HSV_Img, COLOR_BGR2HSV);

		//white color
		Scalar whiteMinScalar = Scalar(0, 128, 153);
		Scalar whiteMaxScalar = Scalar(255, 192, 255);
		inRange(HSV_Img, whiteMinScalar, whiteMaxScalar, whiteLane);

		//yellow color
		Scalar yellowMinScalar = Scalar(0, 145, 155);
		Scalar yellowMaxScalar = Scalar(28, 255, 255);

		Scalar yellowMinScalar_ = Scalar(0, 237, 117);
		Scalar yellowMaxScalar_ = Scalar(255, 255, 255);

		inRange(HSV_Img, yellowMinScalar, yellowMaxScalar, yellowLane);

		inRange(HSV_Img, yellowMinScalar_, yellowMaxScalar_, yellowLane_);

		addWeighted(yellowLane, 1.0, yellowLane_, 1.0, 0.0, yellowLane);

		cv::GaussianBlur(yellowLane, yellowLane, cv::Size(5, 5), 2, 2);

		Canny(yellowLane, LinesImg, 190, 230, 5, true);

		cv::GaussianBlur(whiteLane, whiteLane, cv::Size(5, 5), 2, 2);

		Mat k = getStructuringElement(CV_SHAPE_RECT, Size(9, 9));

		morphologyEx(LinesImg, LinesImg, MORPH_CLOSE, k);

		morphologyEx(whiteLane, whiteLane, MORPH_CLOSE, k);

		std::vector<std::vector<cv::Point>> contours;

		findContours(whiteLane, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		if (contours.size() == 4) {
			crosswalk = 1;	// there is a crosswalk
			cout << "Crosswalk" << endl;
		}
		else
		{
			crosswalk = 0;
		}

		vector<Vec4i> lines;
		double rho = 1;
		double theta = CV_PI / 180;
		int threshold = 43;
		double minLinLength = 35;
		double maxLineGap = 210;

		HoughLinesP(LinesImg, lines, rho, theta, threshold, minLinLength, maxLineGap);

		if (lines.size() == 0)
		{
			L[even] = R[even] = 1;
			//right = left = 1;
		}

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];  // we have 4 elements p1=x1,y1  p2= x2,y2
			Scalar greenColor = Scalar(0, 250, 30);  // B=0 G=250 R=30
			line(ROI, Point(l[0], l[1]), Point(l[2], l[3]), greenColor, 3, CV_AA);

			double slope = (l[3] - l[1]) / (double)(l[2] - l[0]);

			if (slope < 0)
			{
				R[even] = !(L[even] = 0);
				//cout << "Line in the Left" << endl;
				//m.controlPwm(1, 0, 0, 1, 0);
				//right = !(left = 0);
			}
			else
			{
				L[even] = !(R[even] = 0);
				//cout << "Line in the Right" << endl;
				//m.controlPwm(0, 1, 1, 0, 0);
				//left = !(right = 0);
			}

		}
		
		if (false)
		{
			namedWindow("WhiteLane");
			imshow("WhiteLane", whiteLane);

			namedWindow("YellowLane");
			imshow("YellowLane", yellowLane);

			namedWindow("frame");
			imshow("frame", frame);

			namedWindow("ROI");
			imshow("ROI", ROI);
			waitKey(1);

		}

		if (even) {
			left = L[0] | L[1];
			right = R[0] | R[1];
		}
		even = !even;
	}
}