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

		int controlRight = IrRightBit | tracerRight;
		int controlLeft = IrLeftBit | tracerLeft;

		motor.control(controlRight, (controlLeft & ~controlRight), controlLeft, (controlRight & ~controlLeft));

		//motor.control(IrRightBit, (IrLeftBit  & ~IrRightBit), IrLeftBit, (IrRightBit & ~IrLeftBit));

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