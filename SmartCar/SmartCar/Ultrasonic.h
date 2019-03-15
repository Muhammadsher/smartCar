
#ifndef Ultrasonic_h
#define Ultrasonic_h

class Ultrasonic {

public:
	Ultrasonic() {

	}

	int setUp();
	bool start();

private:
	int getDistance();
};

#endif
