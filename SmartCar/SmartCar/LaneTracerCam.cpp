#include "LaneTracerCam.h"
#include "DetectSign.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <thread>

#include "opencv2/opencv.hpp"
#include <raspicam/raspicam_cv.h>

using namespace std;
using namespace cv;

//VideoCapture cap("D:\\test.mp4");


void LaneTracerCam::trace(Motor m) {
	/*VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "error";
		exit(-1);
	}*/


	raspicam::RaspiCam_Cv cap;
	cap.open();

	Mat frame, whiteLane, yellowLane, LinesImg, HSV_Img;

	while (1)
	{
		//cap >> frame;

		cap.grab();
		cap.retrieve(frame);

		if (frame.empty())
		{
			cout << "err";
			break;
		}

		//thread detecSign(&DS::detect, frame);
		//detecSign.detach();
		//resize image
		resize(frame, frame, Size(640, 480));


		//here we define our region of interest
		//box(x, y, b, c);
		Rect const box(100, 295, 400, 185); //this mean the first corner is
											//(x,y)=(100,295)
											// and the second corner is
											//(x + b, y+c )= (100 +400,295+185)
		Mat ROI = frame(box);

		//finding the center
		Point center_of_rect = (box.br() + box.tl())*0.5;

		//cout << "center: " << center_of_rect << endl;

		// convert our img to HSV Space
		cvtColor(ROI, HSV_Img, CV_RGB2HSV);

		//white color thresholding
		Scalar whiteMinScalar = Scalar(90, 0, 236);
		Scalar whiteMaxScalar = Scalar(111, 93, 255);
		inRange(HSV_Img, whiteMinScalar, whiteMaxScalar, whiteLane);

		//yellow color thresholding
		Scalar yellowMinScalar = Scalar(81, 119, 200);
		Scalar yellowMaxScalar = Scalar(101, 255, 255);

		inRange(HSV_Img, yellowMinScalar, yellowMaxScalar, yellowLane);



		//combine our two images in one image
		addWeighted(whiteLane, 1.0, yellowLane, 1.0, 0.0, LinesImg);


		// Edge detection using canny detector
		int minCannyThreshold = 190;
		int maxCannyThreshold = 230;
		Canny(LinesImg, LinesImg, minCannyThreshold, maxCannyThreshold, 5, true);

		// Morphological Operation
		Mat k = getStructuringElement(CV_SHAPE_RECT, Size(9, 9)); //MATLAB :k=Ones(9)

		morphologyEx(LinesImg, LinesImg, MORPH_CLOSE, k);

		// now applying hough transform TO DETECT Lines in our image
		vector<Vec4i> lines;
		double rho = 1;
		double theta = CV_PI / 180;
		int threshold = 43;
		double minLinLength = 35;
		double maxLineGap = 210;

		HoughLinesP(LinesImg, lines, rho, theta, threshold, minLinLength, maxLineGap);

		//draw our lines

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];  // we have 4 elements p1=x1,y1  p2= x2,y2
			Scalar greenColor = Scalar(0, 250, 30);  // B=0 G=250 R=30
			line(ROI, Point(l[0], l[1]), Point(l[2], l[3]), greenColor, 3, CV_AA);

			int direction = l[0] + l[2];
			if (direction > 300) {
				cout << "Line in the Right" << endl;
				m.control(0, 1, 1, 0);
			}
			else if (direction < 300) {
				cout << "Line in the Left" << endl;
				m.control(1, 0, 0, 1);
			}
			else if (direction == 300) {
				cout << "On the Line" << endl;
			}

		}
		// /*
		namedWindow("frame");
		moveWindow("frame", 275, 30);
		circle(frame, Point2i(300, 380), 5, Scalar(0, 125, 230), 4, 3);
		imshow("frame", frame);

		// */

		// /*
		namedWindow("WhiteLane");
		moveWindow("WhiteLane", 950, 280);
		imshow("WhiteLane", whiteLane);
		//  */
		//  /*
		namedWindow("YellowLane");
		//moveWindow("YellowLane", 950, 30);
		imshow("YellowLane", yellowLane);
		// */

		if (waitKey(30) == 27)
		{
			cout << "esc";
			break;
		}
	}
}