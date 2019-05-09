#include "Ultrasonic.h"

int Ultrasonic::setUp() {

	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);

	return 0;
}

void Ultrasonic::getDistance(atomic<bool> &stopBit)
{
	setUp();

	while (1) {
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

		stopBit = (distance > 35);
	}
}
