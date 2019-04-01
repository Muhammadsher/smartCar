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

		int rightBit = ir.RightIr();
		int leftBit = ir.LeftIr();

		if (!uSensor.start())
		{
			motor.stopDCMotor();
		}
		else 
		{
			motor.allInOne(rightBit, (leftBit & ~rightBit), leftBit, (rightBit & ~leftBit));
		}

	}

	return 0;
}