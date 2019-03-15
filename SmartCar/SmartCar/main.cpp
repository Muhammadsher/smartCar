#include "Ultrasonic.h"
#include "Motor.h"
#include <iostream>

using namespace std;

int main() {
	Ultrasonic uSensor;
	Motor motor;

	uSensor.setUp();
	motor.initDCMotor();


	while (1)
	{
		if (uSensor.start()) {
			motor.goForward();
		}
		else
		{
			motor.stopDCMotor();
		}

	}

	return 0;
}