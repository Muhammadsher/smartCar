#include "IR.h"
#include "IR_Tracer.h"
#include "Motor.h"
//#include "LaneTracerCam.h"
#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <atomic>

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
	//LaneTracerCam laneTracerCam;

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

	while (1)
	{
		int controlRight = ir.RightIr() & tracer.rightIrTracer() & stopBit;
		int controlLeft = ir.LeftIr() & tracer.leftIrTracer() & stopBit;

		//motor.control(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft));
		motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), 0);

		//laneTracerCam.trace(motor);

	}

	return 0;
}