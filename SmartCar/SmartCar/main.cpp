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

		if (!uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 0) // obstacle everywhere
		{
			motor.stopDCMotor();
			break;
		}
		else if (!uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 1) // no obstacle in left side
		{
			motor.goLeft();
		}
		else if (!uSensor.start() && ir.RightIr() == 1 && ir.LeftIr() == 0) // no obstacle in right side
		{
			motor.goRight();
		}
		else if (uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 0) // no obstacle in left side
		{
			motor.goForward();
		}

	}

	return 0;
}