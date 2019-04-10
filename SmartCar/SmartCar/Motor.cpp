#include "Motor.h"
#include <iostream>
#include <cstdio>
#include <wiringPi.h>
#include <softPwm.h>

using namespace std;

#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

#define MOTOR_INIT digitalWrite(1, HIGH); digitalWrite(4,HIGH); digitalWrite(5, HIGH); digitalWrite(6,HIGH)
#define MOTOR_GO_FORWARD digitalWrite(1,HIGH); digitalWrite(4,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_BACK digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT digitalWrite(1,HIGH);digitalWrite(4,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_LEFT digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_STOP digitalWrite(1, LOW); digitalWrite(4,LOW); digitalWrite(5, LOW); digitalWrite(6,LOW)

void Motor::initDCMotor() {

	pinMode(IN1_PIN, OUTPUT);
	pinMode(IN2_PIN, OUTPUT);
	pinMode(IN3_PIN, OUTPUT);
	pinMode(IN4_PIN, OUTPUT);

	digitalWrite(IN1_PIN, HIGH);
	digitalWrite(IN2_PIN, HIGH);
	digitalWrite(IN3_PIN, HIGH);
	digitalWrite(IN4_PIN, HIGH);

	/*softPwmCreate(IN1_PIN, MIN_SPEED, MAX_SPEED);
	softPwmCreate(IN2_PIN, MIN_SPEED, MAX_SPEED);
	softPwmCreate(IN3_PIN, MIN_SPEED, MAX_SPEED);
	softPwmCreate(IN4_PIN, MIN_SPEED, MAX_SPEED);*/
}

int Motor::checkSpeed(int speed) {
	if (speed > MAX_SPEED)
		speed = MAX_SPEED;
	if (speed < 10)
		speed = 20;
	return speed;
}

void Motor::control(int p1, int p4, int p5, int p6)
{
	digitalWrite(IN1_PIN, p1);
	digitalWrite(IN2_PIN, p4);
	digitalWrite(IN3_PIN, p5);
	digitalWrite(IN4_PIN, p6);
}

void Motor::goForward(int speed) {
	speed = checkSpeed(speed);

	softPwmWrite(IN1_PIN, speed);
	softPwmWrite(IN2_PIN, MIN_SPEED);
	softPwmWrite(IN3_PIN, speed);
	softPwmWrite(IN4_PIN, MIN_SPEED);

	cout << "Forward\n";
}

void Motor::goBackward(int speed) {
	speed = checkSpeed(speed);

	softPwmWrite(IN1_PIN, LOW);
	softPwmWrite(IN2_PIN, speed);
	softPwmWrite(IN3_PIN, LOW);
	softPwmWrite(IN4_PIN, speed);

	cout << "Back\n";
}

void Motor::goLeft(int speed) {
	speed = checkSpeed(speed);

	softPwmWrite(IN1_PIN, LOW);
	softPwmWrite(IN2_PIN, speed);
	softPwmWrite(IN3_PIN, speed);
	softPwmWrite(IN4_PIN, LOW);

	cout << "Left\n";
}

void Motor::goRight(int speed) {
	speed = checkSpeed(speed);

	softPwmWrite(IN1_PIN, speed);
	softPwmWrite(IN2_PIN, LOW);
	softPwmWrite(IN3_PIN, LOW);
	softPwmWrite(IN4_PIN, speed);

	cout << "Right\n";
}

void Motor::stopDCMotor() {
	digitalWrite(IN1_PIN, LOW);
	digitalWrite(IN2_PIN, LOW);
	digitalWrite(IN3_PIN, LOW);
	digitalWrite(IN4_PIN, LOW);
	cout << "Stop\n";
}

