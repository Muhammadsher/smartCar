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
		
		motor.allInOne(rightBit, (leftBit & ~rightBit), leftBit, (rightBit & ~leftBit));

		//if (!uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 0) // obstacle everywhere 000 - 0
		//{
		//	motor.stopDCMotor();
		//	break;
		//}
		//else if (!uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 1) // no obstacle in left side 001 - 1
		//{
		//	motor.goLeft();
		//}
		//else if (!uSensor.start() && ir.RightIr() == 1 && ir.LeftIr() == 0) // no obstacle in right side 010 - 2
		//{
		//	motor.goRight();
		//}
		//else if (uSensor.start() && ir.RightIr() == 1 && ir.LeftIr() == 1) // no obstacle in right and left side 011 - 3
		//{
		//	motor.goForward();
		//}
		//else if (uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 0) // obstacle in right and left side 100 - 4
		//{
		//	motor.stopDCMotor();
		//}		
		//else if (uSensor.start() && ir.RightIr() == 0 && ir.LeftIr() == 1) // no obstacle in left side 101 - 5
		//{
		//	motor.goLeft();
		//}
		//else if (uSensor.start() && ir.RightIr() == 1 && ir.LeftIr() == 0) // no obstacle in right side 110 - 6
		//{
		//	motor.goRight();
		//}
		//else if (uSensor.start() && ir.RightIr() == 1 && ir.LeftIr() == 1) // no obstacle 111 - 7
		//{
		//	motor.goForward();
		//}

	}

	return 0;
}