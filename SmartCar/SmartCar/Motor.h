#ifndef Motor_h
#define Motor_h

#define MAX_SPEED 60
#define MIN_SPEED 5		// MIN_SPEED = 6 * MIN_SPEED;

class Motor {

public:
	Motor() {

	}

	void initDCMotor();
	void initDCMotorPwm();
	void control(int p1, int p4, int p5, int p6);
	void controlPwm(int p1, int p4, int p5, int p6, int withPwm);
	void goForward(int);
	void goBackward(int);
	void goLeft(int);
	void goRight(int);
	void stopDCMotor();

private:

};

#endif