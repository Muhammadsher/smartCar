#include "Ultrasonic.h"
#include <iostream>

using namespace std;

int main() {
	Ultrasonic uSensor;

	uSensor.setUp();

	while (1)
	{
		uSensor.start();
	}

	return 0;
}