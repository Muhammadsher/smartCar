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
#include <unistd.h>
#include "wiringPi.h"


using namespace std;

atomic<bool> stopBit{ 1 };
atomic<int> lineInTheLeft{ 1 };	//	1 = no obstacle 
atomic<int> lineInTheRight{ 1 };
atomic<int> signBit{ SIGN_NOT_DEFINED };
atomic<bool> tmpStopBit{ false };


void tmpSignTest()
{
	while (true)
	{
		/*signBit = SIGN_STOP;
		sleep(10);*/
		if (getchar() == 32 || getchar() == ' ')
		{
			tmpStopBit = !tmpStopBit;
		}
	}
}


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
	

	//thread sign(tmpSignTest);
	
	//thread DS(&DetectSign2::detect, ds, std::ref(magu), std::ref(signBit), false);
	
	//magu.captuerVideo();


	while (true)
	{

		int controlRight = ir.RightIr() & tracer.rightIrTracer() & lineInTheRight & stopBit;
		int controlLeft = ir.LeftIr() & tracer.leftIrTracer() & lineInTheLeft & stopBit;

		if (!tmpStopBit)
		{
			motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), 0);
		}



		/*switch (signBit)
		{
			case SIGN_STOP:
				motor.controlPwm(0, 0, 0, 0, 0);
				signBit = SIGN_NOT_DEFINED;
				sleep(5);
				break;
			case SIGN_LEFT:
				break;
			case SIGN_RIGHT:
				break;
			case SIGN_PEDESTRIAN:
				// eanbling with pwm, goes slower
				// to do when to go faster again
				motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), 1);
				break;
			case SIGN_PARKING:
				break;
			case SIGN_CIRCLE:
			case SIGN_FORWARD:
			case SIGN_NOT_DEFINED:
			default:
				motor.controlPwm(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft), 0);
				break;
		}*/
	}

	th.join();
	//trace.join();
	//DS.join();
	return 0;
}