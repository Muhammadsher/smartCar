/*#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "opencv2/highgui.hpp"
using namespace std;
using namespace cv;

// Function for Face Detection 
void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade, double scale);
string cascadeName, nestedCascadeName;

int main(int argc, const char** argv)
{
	// VideoCapture class for playing video for which faces to be detected 
	VideoCapture capture;
	Mat frame, image;

	// PreDefined trained XML classifiers with facial features 
	CascadeClassifier cascade, nestedCascade;
	double scale = 1;

	// Load classifiers from "opencv/data/haarcascades" directory  
	nestedCascade.load("C:\opencv\sources\data\haarcascades\haarcascade_eye_tree_eyeglasses.xml");

	// Change path before execution  
	cascade.load("C:\opencv\sources\data\haarcascades\haarcascade_frontalcatface.xml");

	// Start Video..1) 0 for WebCam 2) "Path to Video" for a Local Video 
	capture.open(0);
	if (capture.isOpened())
	{
		// Capture frames from video and detect faces 
		cout << "Face Detection Started...." << endl;
		while (1)
		{
			capture >> frame;
			if (frame.empty())
				break;
			Mat frame1 = frame.clone();
			detectAndDraw(frame1, cascade, nestedCascade, scale);
			char c = (char)waitKey(10);

			// Press q to exit from window 
			if (c == 27 || c == 'q' || c == 'Q')
				break;
		}
	}
	else
		cout << "Could not Open Camera";
	return 0;
}

void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale)
{
	vector<Rect> faces, faces2;
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale 
	double fx = 1 / scale;

	// Resize the Grayscale Image  
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	// Detect faces of different sizes using cascade classifier  
	cascade.detectMultiScale(smallImg, faces, 1.1,
		2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Draw circles around the faces 
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = Scalar(255, 0, 0); // Color for Drawing tool 
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
				cvPoint(cvRound((r.x + r.width - 1)*scale),
					cvRound((r.y + r.height - 1)*scale)), color, 3, 8, 0);
		if (nestedCascade.empty())
			continue;
		smallImgROI = smallImg(r);

		// Detection of eyes int the input image 
		nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
			0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Draw circles around eyes 
		for (size_t j = 0; j < nestedObjects.size(); j++)
		{
			Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
			center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
			radius = cvRound((nr.width + nr.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
	}

	// Show Processed Image with detected faces 
	imshow("Face Detection", img);
}*/

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main()
{

	VideoCapture cap("D:\\test.mp4");

	if (!cap.isOpened())
	{
		cout << "error";
		return -1;
	}



	Mat frame, whiteLane, yellowLane, LinesImg, HSV_Img;

	while (1)
	{

		bool b = cap.read(frame);

		if (!b)
		{
			cout << "err";
			cap.open("laneD.mp4");
			cap.read(frame);
			// break;
		}

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
			if (direction > 300)
				cout << "Line in the Right" << endl;
			else if (direction < 300)
				cout << "Line in the Left" << endl;
			else if(direction == 300)
				cout << "On the Line" << endl;

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

	return 0;
}
