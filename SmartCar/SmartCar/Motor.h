
#ifndef Motor_h
#define Motor_h

#define MAX_SPEED 50
#define MIN_SPEED 0

class Motor {

public:
	Motor() {

	}

	void initDCMotor();
	void allInOne(int p1, int p4, int p5, int p6);
	void goForward(int);
	void goBackward(int);
	void goLeft(int);
	void goRight(int);
	int checkSpeed(int);
	void stopDCMotor();

private:

};

#endif