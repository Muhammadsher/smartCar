#include "Ultrasonic.h"
#include <iostream>
#include <wiringPi.h>

#define TRIG_PIN 28
#define ECHO_PIN 29

using namespace std;

int Ultrasonic::setUp() {

	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);

	return 0;
}

int Ultrasonic::getDistance()
{
	int start_time = 0, end_time = 0;
	float distance = 0;

	digitalWrite(TRIG_PIN, LOW);
	delay(500);
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);

	while (digitalRead(ECHO_PIN) == 0);
	start_time = micros();

	while (digitalRead(ECHO_PIN) == 1);
	end_time = micros();

	distance = (end_time - start_time) / 29. / 2.;
	return (int)distance;
}

bool Ultrasonic::start() {
	int distance = getDistance();

	if (distance <= 40)
	{
		return false;
	}
	else 
	{
		return true;
	}
}
