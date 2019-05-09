#ifndef Ultrasonic_h
#define Ultrasonic_h
#include <atomic>
#include "Ultrasonic.h"
#include <iostream>
#include <wiringPi.h>

using namespace std;

class Ultrasonic {
#define TRIG_PIN 28
#define ECHO_PIN 29

public:
	Ultrasonic() {

	}
	void getDistance(std::atomic<bool> &);

private:
	int setUp();
};

#endif
