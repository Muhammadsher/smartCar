
#ifndef IR_Tracer_h
#define IR_Tracer_h
#include <wiringPi.h>


class IR_Tracer {
#define TrackSensorPinRight    11
#define TrackSensorPinLeft    10
public:
	IR_Tracer() {

	}

	int setUp();
	int rightIrTracer();
	int leftIrTracer();
};

#endif
