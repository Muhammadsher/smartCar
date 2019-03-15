
#ifndef Motor_h
#define Motor_h

class Motor {

public:
	Motor() {

	}

	void initDCMotor();
	void goForward();
	void goBackward();
	void goLeft();
	void goRight();
	void stopDCMotor();

private:

};

#endif