#include "IR.h"
#include <iostream>
#include <wiringPi.h>

#define RIGHT 26
#define LEFT 27

using namespace std;

int IR::setUp() {
	pinMode(RIGHT, INPUT);
	pinMode(LEFT, INPUT);
	return 0;
}

int IR::RightIr() {
	return digitalRead(RIGHT);
}

int IR::LeftIr() {
	return digitalRead(LEFT);
}

