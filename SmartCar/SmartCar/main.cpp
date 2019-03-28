#include "Ultrasonic.h"
#include "IR.h"
#include "Motor.h"
#include <iostream>

using namespace std;

int main() {
	Ultrasonic uSensor;
	Motor motor;
	IR ir;

	uSensor.setUp();
	motor.initDCMotor();
	ir.setUp();


	while (1)
	{
		if (uSensor.start()) {
			motor.goForward();
		}
		
		if(ir.RightIr() == 0)
		{
			motor.goLeft();
		}
		
		if (ir.LeftIr() == 0)
		{
			motor.goRight();
		}

		if (!uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 0)
		{
			motor.stopDCMotor();
		}

	}

	return 0;
}