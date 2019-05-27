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
#include <mutex>
#include <unistd.h>
#include "wiringPi.h"


using namespace std;

atomic<bool> stopBit{ 1 };
atomic<int> lineInTheLeft{ 1 };	//	1 = no obstacle 
atomic<int> lineInTheRight{ 1 };
atomic<int> signBit{ SIGN_NOT_DEFINED };
atomic<int> crosswalk{ 0 };	// no crasswalk

atomic<bool> notAcceptStop{ false };
mutex mtx;

void changeBit(atomic<bool> &bit)
{
	lock_guard<std::mutex> lg(mtx);
	bit = !bit;
	sleep(10);
	bit = !bit;
}


int main() {
	Ultrasonic * ultrasonic = new Ultrasonic();
	LaneTracerCam * laneTracerCam = new LaneTracerCam();
	DetectSign2 * ds = new DetectSign2();
	IR_Tracer tracer;
	//MAGU magu;
	Motor motor;
	IR ir;
	int withPwm = 0; // means go with max_speed

	if (wiringPiSetup() == -1) {
		cout << "Setup wiringPi failed !" << endl;
		return 1;
	}

	motor.initDCMotorPwm();
	tracer.setUp();
	ir.setUp();

	MAGU magu;

	thread th(&Ultrasonic::getDistance, ultrasonic, std::ref(stopBit));
	//thread trace(&LaneTracerCam::trace, laneTracerCam, std::ref(lineInTheLeft), std::ref(lineInTheRight), std::ref(crosswalk), std::ref(magu));

	thread DS(&DetectSign2::detect, ds, std::ref(magu), std::ref(signBit), true);
	

	while (true)
	{

		int controlRight = ir.RightIr() & tracer.rightIrTracer() & lineInTheRight & stopBit;
		int controlLeft = ir.LeftIr() & tracer.leftIrTracer() & lineInTheLeft & stopBit;

		switch (signBit)
		{
			case SIGN_STOP:
				if (!notAcceptStop)	// will accept first stop then skips signal for 10 seconds
				{
					motor.controlPwm(0, 0, 0, 0, 0);
					signBit = SIGN_NOT_DEFINED;
					withPwm = 0;	// after stop move fast
					cout << "STOP" << endl;
					thread ch(changeBit, std::ref(notAcceptStop));
					ch.detach();
					sleep(5);
				}
				break;
			case SIGN_LEFT:
				cout << "Left" << endl;
				motor.controlPwm(1, 0, 1, 0, 0);
				sleep(1);
				motor.controlPwm(0, 1, 1, 0, withPwm);	// go left
				delay(800);
				signBit = SIGN_NOT_DEFINED;
				break;
			case SIGN_RIGHT:
				cout << "Right" << endl;
				motor.controlPwm(1, 0, 1, 0, 0);
				sleep(1);
				motor.controlPwm(1, 0, 0, 1, withPwm);	// go right
				delay(800);
				signBit = SIGN_NOT_DEFINED;
				break;
			case SIGN_PEDESTRIAN:
				withPwm = 1; // slow down motor
				cout << "Pedestrian" << endl;
				signBit = SIGN_NOT_DEFINED;
				crosswalk = 1;
				break;
			case SIGN_PARKING:
				cout << "Parking" << endl;
				motor.controlPwm(1, 0, 1, 0, withPwm);
				signBit = SIGN_NOT_DEFINED;
				delay(700);
				break;
			case NO_ROAD:
				cout << "No road" << endl;
				motor.controlPwm(1, 0, 0, 1, 0);	// right
				sleep(1);
				motor.controlPwm(1, 0, 1, 0, 0);	// forward
				delay(800);
				motor.controlPwm(0, 1, 1, 0, 0);	// left
				sleep(1);
				motor.controlPwm(1, 0, 1, 0, 0);	// forward
				sleep(1);
				motor.controlPwm(0, 1, 1, 0, 0);	// left
				sleep(1);
				motor.controlPwm(1, 0, 1, 0, 0);	// forward
				delay(800);
				motor.controlPwm(1, 0, 0, 1, 0);	// right
				sleep(1);
				signBit = SIGN_NOT_DEFINED;
				break;
			case TRAFIC_CONTROL_RED:
				motor.controlPwm(0, 0, 0, 0, 0);
				withPwm = 0; // go fast again
				//signBit = SIGN_NOT_DEFINED;
				break;
			case SIGN_FORWARD:
				motor.controlPwm(1, 0, 1, 0, 0);	// forward
				delay(100);
				//signBit = SIGN_NOT_DEFINED;
				break;
			case SIGN_CIRCLE:
			case SIGN_NOT_DEFINED:
			default:
				motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), withPwm);
		}
	}

	th.join();
	//trace.join();
	DS.join();
	return 0;
}