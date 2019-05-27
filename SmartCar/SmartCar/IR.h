
#ifndef IR_h
#define IR_h
#include <wiringPi.h>

class IR {
#define RIGHT 26
#define LEFT 27

public:
	IR() {

	}

	int setUp();
	int RightIr();
	int LeftIr();
};

#endif
