#include "IR.h"
#include "IR_Tracer.h"
#include "Motor.h"
#include "Ultrasonic.h"
#include "LaneTracerCam.h"
#include "Magu.h"
#include "DetectSign.h"
#include <iostream>
#include <thread>
#include <atomic>
#include "wiringPi.h"


using namespace std;

atomic<bool> stopBit{ 1 };
atomic<int> lineInTheLeft{ 1 };	//	1 = no obstacle 
atomic<int> lineInTheRight{ 1 };
atomic<int> signBit{ SIGN_NOT_DEFINED };


int main() {
	Ultrasonic * ultrasonic = new Ultrasonic();
	LaneTracerCam * laneTracerCam = new LaneTracerCam();
	DetectSign2 * ds = new DetectSign2();
	IR_Tracer tracer;
	MAGU magu;
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
	thread trace(&LaneTracerCam::trace, laneTracerCam, std::ref(lineInTheLeft), std::ref(lineInTheRight), std::ref(magu));
	thread DS(&DetectSign2::detect, ds, std::ref(magu), std::ref(signBit), false);

	while (true)
	{

		int controlRight = ir.RightIr() & tracer.rightIrTracer() & lineInTheRight & stopBit;
		int controlLeft = ir.LeftIr() & tracer.leftIrTracer() & lineInTheLeft & stopBit;
		//cout << ds->readable(signBit) << endl;
		//motor.control(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft));
		motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), 0);

	}

	th.join();
	trace.join();
	DS.join();
	return 0;
}