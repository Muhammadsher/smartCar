#ifndef Motor_h
#define Motor_h

#define MAX_SPEED 40
#define MIN_SPEED 5		// MIN_SPEED = 6 * MIN_SPEED;
#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

#include <iostream>
#include <cstdio>
#include <wiringPi.h>
#include <softPwm.h>

using namespace std;



class Motor {

public:
	Motor();
	~Motor();
	void initDCMotorPwm();
	void controlPwm(int p1, int p4, int p5, int p6, int withPwm);

	//void initDCMotor();
	//void control(int p1, int p4, int p5, int p6);
	//void goForward(int);
	//void goBackward(int);
	//void goLeft(int);
	//void goRight(int);
	//void stopDCMotor();

private:

};

#endif