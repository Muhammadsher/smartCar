#include "Ultrasonic.h"
#include "IR.h"
#include "IR_Tracer.h"
#include "Motor.h"
#include <wiringPi.h>
#include <iostream>

using namespace std;

int main() {
	Ultrasonic uSensor;
	Motor motor;
	IR_Tracer tracer;
	IR ir;

	if (wiringPiSetup() == -1) {
		cout << "Setup wiringPi failed !" << endl;
		return 1;
	}

	uSensor.setUp();
	motor.initDCMotor();
	tracer.setUp();
	ir.setUp();


	while (1)
	{

		int IrRightBit = ir.RightIr();
		int IrLeftBit = ir.LeftIr();
		int tracerRight = tracer.rightIrTracer();
		int tracerLeft = tracer.leftIrTracer();


		motor.control(IrRightBit | tracerRight, ((IrLeftBit | tracerLeft) & ~(IrRightBit | tracerRight)), IrLeftBit | tracerLeft,
			((IrRightBit | tracerRight) & ~(IrLeftBit | tracerLeft)));

		/*if (!uSensor.start())
		{
			motor.stopDCMotor();
		}
		else 
		{
		}*/

	}

	return 0;
}