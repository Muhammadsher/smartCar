#include "LaneTracerCam.h"


void LaneTracerCam::trace(atomic<int> &left, atomic<int> &right, MAGU &magu) {
	//traceBit = false;
	//std::lock_guard<std::mutex> lock(mtx);
	Mat frame, whiteLane, yellowLane, LinesImg, HSV_Img;
	//frame = f.clone();
	//raspicam::RaspiCam_Cv camera;
	//camera.open();

	//sleep(3);

	while (true)
	{
		//camera.grab();
		//camera.retrieve(frame);

		frame = magu.getImage();

		cout << "Lane tracer got image" << endl;

		frame = frame(Rect(0, frame.rows / 2, frame.cols, frame.rows / 2));


		//resize image
		//resize(frame, frame, Size(640, 480));


		//here we define our region of interest
		//box(x, y, b, c); //100, 295, 400, 185
		Rect box(frame.cols / 4, frame.rows / 2, frame.cols / 2, frame.rows / 2); //this mean the first corner is
											//(x,y)=(100,295)
											// and the second corner is
											//(x + b, y+c )= (100 +400,295+185)
		Mat ROI = frame(box);

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

		if (lines.size() == 0)
		{
			left = right = 1;
		}

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];  // we have 4 elements p1=x1,y1  p2= x2,y2
			Scalar greenColor = Scalar(0, 250, 30);  // B=0 G=250 R=30
			line(ROI, Point(l[0], l[1]), Point(l[2], l[3]), greenColor, 3, CV_AA);

			double slope = (l[3] - l[1]) / (double)(l[2] - l[0]);

			if (slope <= 0)
			{
				//cout << "Line in the Left" << endl;
				//m.controlPwm(1, 0, 0, 1, 0);
				right = !(left = 0);
			}
			else
			{
				//cout << "Line in the Right" << endl;
				//m.controlPwm(0, 1, 1, 0, 0);
				left = !(right = 0);
			}

		}
		
		rectangle(frame, box, Scalar(0, 0, 255), 2);

		namedWindow("frame");
		imshow("frame", frame);
		if (waitKey(20) == 27)
		{
			cout << "Lane Tracing Finished" << endl;
			//traceBit = true;
			break;
		}
		
		 
		namedWindow("WhiteLane");
		//moveWindow("WhiteLane", 950, 280);
		imshow("WhiteLane", whiteLane);
		//  */
		
		namedWindow("YellowLane");
		//moveWindow("YellowLane", 950, 30);
		imshow("YellowLane", yellowLane);
		waitKey(1);
		// */

	}
		//traceBit = true;
		//mtx.unlock();

}