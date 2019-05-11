#include "Magu.h"

MAGU::MAGU()
{
	if (!camera.open()) {
		cerr << "Error opening camera" << endl;
	}

	sleep(3);
}

MAGU::~MAGU()
{
	camera.release();
}

Mat MAGU::getImage() {
	lock_guard<std::mutex> lg(mtxCamera);
	camera.grab();
	camera.retrieve(frame);
	return frame;
}

/*void MAGU::start(atomic<int> &ds_result, atomic<int> &left, atomic<int> &right)
{
	Mat frame;
	bool debug = false;
	atomic<bool> runTracr{ true };
	LaneTracerCam * laneTracerCam = new LaneTracerCam();
	DetectSign2 * DS = new DetectSign2();

	/*VideoCapture cap("./test.mp4");

	if (!cap.isOpened())
	{
		cout << "error opening" << endl;
		return;
	}

	//MAGU magu;

	//laneTracerCam->trace(frame, left, right, magu);

	//thread(&LaneTracerCam::trace, laneTracerCam, std::ref(frame), std::ref(left), std::ref(right), magu);

	/*
	while (true)
	{
		camera.grab();
		camera.retrieve(frame);
		imshow("video", frame);
		waitKey(10);
		/*bool b = cap.read(frame);
		imshow("video", frame);
		waitKey(10);

		if (!b)
		{
			cout << "err";
			cap.open("laneD.mp4");
			cap.read(frame);
			break;
		}

		if (runTracr)
		{
			thread(&LaneTracerCam::trace, laneTracerCam, std::ref(frame), std::ref(left), std::ref(right), std::ref(runTracr)).detach();
		}
		//thread(&DetectSign2::detect, DS, std::ref(frame), std::ref(ds_result), debug).detach();
		//cout << DS->readable(ds_result) << endl;
	}
}*/
