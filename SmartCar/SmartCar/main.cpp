#include "IR.h"
#include "IR_Tracer.h"
#include "Motor.h"
#include "Ultrasonic.h"
#include "LaneTracerCam.h"
#include "DetectSign.h"
#include <iostream>
#include <thread>
#include <atomic>
#include "wiringPi.h"


using namespace std;

atomic<bool> stopBit{ 1 };
atomic<int> lineInTheLeft{ 0 };
atomic<int> lineInTheRight{ 0 };


int main() {
	LaneTracerCam * laneTracerCam = new LaneTracerCam();
	Ultrasonic * ultrasonic = new Ultrasonic();
	IR_Tracer tracer;
	DS detectSign;
	Motor motor;
	IR ir;

	if (wiringPiSetup() == -1) {
		cout << "Setup wiringPi failed !" << endl;
		return 1;
	}

	//motor.initDCMotor();
	motor.initDCMotorPwm();
	tracer.setUp();
	ir.setUp();

	thread th(&Ultrasonic::getDistance, ultrasonic, std::ref(stopBit));

	thread trace(&LaneTracerCam::trace, laneTracerCam, motor, std::ref(lineInTheLeft), std::ref(lineInTheRight));

	while (1)
	{
		int controlRight = ir.RightIr() & tracer.rightIrTracer() & stopBit;
		int controlLeft = ir.LeftIr() & tracer.leftIrTracer() & stopBit;

		controlLeft = controlLeft | lineInTheLeft;
		controlRight = controlRight | lineInTheRight;

		//motor.control(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft));
		motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), 0);

	}

	return 0;
}