
#ifndef Ultrasonic_h
#define Ultrasonic_h

class Ultrasonic {

public:
	Ultrasonic() {

	}

	int setUp();
	void start();
	
private:
	int getDistance();
};

#endif
