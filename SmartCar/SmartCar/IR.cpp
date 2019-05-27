#include "IR.h"

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

