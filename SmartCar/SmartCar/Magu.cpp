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
	writer.release();
}

Mat MAGU::getImage() {
	lock_guard<std::mutex> lg(mtxCamera);
	camera.grab();
	camera.retrieve(frame);

	return frame;
}

void MAGU::captuerVideo()
{
	double width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);
	double fps = camera.get(CV_CAP_PROP_FPS);
	double fourcc = camera.get(CV_CAP_PROP_FOURCC);

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", timeinfo);
	std::string str(buffer);

	cout << width << ", " << height << ", " << fps << ", " << CV_FOURCC('M', 'J', 'P', 'G') << endl;

	writer = VideoWriter("./log/wr_"+str+".avi", CV_FOURCC('M','J','P','G'), fps, Size(width, height));

	while (true)
	{
		writer.write(getImage());
	}
}