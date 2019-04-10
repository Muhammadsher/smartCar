#include "IR_Tracer.h"
#include <iostream>
#include <wiringPi.h>

#define TrackSensorPinRight    11
#define TrackSensorPinLeft    10

using namespace std;

int IR_Tracer::setUp() {

	pinMode(TrackSensorPinRight, INPUT);
	pinMode(TrackSensorPinLeft, INPUT);

	return 0;
}

int IR_Tracer::rightIrTracer() {
	return digitalRead(TrackSensorPinRight);
}

int IR_Tracer::leftIrTracer() {
	return digitalRead(TrackSensorPinLeft);
}

