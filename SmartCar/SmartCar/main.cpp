#include "IR.h"
#include "IR_Tracer.h"
#include "Motor.h"
#include "LaneTracerCam.h"
#include "DetectSign.h"
#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <atomic>

#include "opencv2/opencv.hpp"

using namespace std;

#define TRIG_PIN 28
#define ECHO_PIN 29

using namespace std;
atomic<bool> stopBit{ TRUE };

void setUpUltrasonic() {
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
}

void getDistance() {

	while (1) {
		int start_time = 0, end_time = 0;
		float distance = 0;

		digitalWrite(TRIG_PIN, LOW);
		delay(500);
		digitalWrite(TRIG_PIN, HIGH);
		delayMicroseconds(10);
		digitalWrite(TRIG_PIN, LOW);

		while (digitalRead(ECHO_PIN) == 0);
		start_time = micros();

		while (digitalRead(ECHO_PIN) == 1);
		end_time = micros();

		distance = (end_time - start_time) / 29. / 2.;

		stopBit = (distance > 35);
	}
}

int main() {
	Motor motor;
	IR_Tracer tracer;
	IR ir;
	DS detectSign;
	LaneTracerCam * laneTracerCam = new LaneTracerCam();
	bool stopRecording = 0;

	if (wiringPiSetup() == -1) {
		cout << "Setup wiringPi failed !" << endl;
		return 1;
	}

	//motor.initDCMotor();
	motor.initDCMotorPwm();
	tracer.setUp();
	ir.setUp();

	setUpUltrasonic();
	thread th(getDistance);
	thread trace(&LaneTracerCam::trace, laneTracerCam, motor, stopRecording);

	while (1)
	{
		int controlRight = ir.RightIr() & tracer.rightIrTracer() & stopBit;
		int controlLeft = ir.LeftIr() & tracer.leftIrTracer() & stopBit;

		//motor.control(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft));
		motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), 0);

		//laneTracerCam.trace(motor);

		char c = cv::waitKey(0);
		if ('q' == c)
			stopRecording = 1;

	}

	return 0;
}