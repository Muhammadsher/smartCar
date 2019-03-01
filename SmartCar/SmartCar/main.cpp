#include "Ultrasonic.h"
#include <iostream>

using namespace std;

int main() {
	Ultrasonic uSensor;

	uSensor.setUp();

	uSensor.start();

	return 0;
}