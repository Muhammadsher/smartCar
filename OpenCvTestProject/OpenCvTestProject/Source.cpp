//#include "opencv2/opencv.hpp"
//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//using namespace cv;
//int main(int, char**)
//{
//	VideoCapture cap(0); // open the default camera
//	if (!cap.isOpened())
//		return -1;
//	namedWindow("Video", 1);
//	while (1)
//	{
//		Mat frame;
//		cap >> frame;
//		imshow("Video", frame);
//		// Press 'c' to escape
//		if (waitKey(30) == 'c') break;
//	}
//	return 0;
//}
//////////////////////////////////////////////////////////
//CODE - 5: Morphological_Image Processing
// Fill all the To-Do sections
/////////////////////////////////////////////////////////

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat image;
	//TO DO: image = imread(xxxxxxx) // Read the file using imshow in GRAYSCALE IMAGE MODE

	//image = imread("C:\\Users\\Mukhammadsher\\Desktop\\images\\traffic_sign(8).png", IMREAD_GRAYSCALE);

	VideoCapture cap(0); 
	// open the default camera
		if (!cap.isOpened())
			return -1;
		namedWindow("Video", 1);
		cap >> image;
	imshow("Original", image);


	// TO Do: Apply the Adaptive thresholding to the image
	// USE ADAPTIVE THRESHOLDING SYNTAX

	//adaptiveThreshold(image, image, 125, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 12);

	Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
	Mat eroded, dilated, opened, closed;
	erode(image, eroded, mask, cv::Point(-1, -1), 3);
	imshow("Eroded", eroded);
	dilate(image, dilated, mask, cv::Point(-1, -1), 3);
	imshow("Dilated", dilated);
	morphologyEx(image, opened, cv::MorphTypes::MORPH_OPEN, mask);
	imshow("open", opened);
	morphologyEx(image, closed, cv::MorphTypes::MORPH_CLOSE, mask);
	imshow("close", closed);
	// TO Do: Now generate the opening and closing images using the just dilation and erosion operations without using MORPH_OPEN or MORPH_CLOSE
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

