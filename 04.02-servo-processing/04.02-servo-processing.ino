#include <Servo.h>
#define SERVO 10

Servo servo1;

int inputAngle;

void setup()
{
	Serial.begin(9600);

	servo1.attach(SERVO);
}

void loop()
{
	if (Serial.available() > 1)
	{
		inputAngle = Serial.parseInt();
		Serial.read();	// Remove '\n' from serial buffer

		servo1.write(inputAngle);
	}	
}