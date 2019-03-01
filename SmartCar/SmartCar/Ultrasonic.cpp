#include "Ultrasonic.h"
#include <iostream>
#include <wiringPi.h>

using namespace std;

int Ultrasonic::setUp() {
	if (wiringpisetup() == -1)
		return 0;
	else {
		pinmode(trig_pin, output);
		pinmode(echo_pin, input);
	}
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

void Ultrasonic::start() {
	int distance = getDistance();

	if (distance <= 20)
	{
		cout << "STOP";
	}
}
