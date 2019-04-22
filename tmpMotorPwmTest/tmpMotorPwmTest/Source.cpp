#include <iostream>

using namespace std;

void controlPwm(int p1, int p2, int p3, int p4, int pwm)
{
	int speed = pwm == 0 ? 100 : 5;
	int sMp = speed * pwm;
	cout << "B1: " << (p1 + (sMp * p1)) * speed << "\n";
	cout << "B2: " << (p2 + (sMp * p2)) * speed << "\n";
	cout << "B3: " << (p3 + (sMp * p3)) * speed << "\n";
	cout << "B4: " << (p4 + (sMp * p4)) * speed << "\n";
}

int main()
{
	controlPwm(1, 0, 1, 0, 1);
	return 0;
}