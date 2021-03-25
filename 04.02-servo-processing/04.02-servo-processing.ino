#include <Servo.h>
#define SERVO 10

Servo servo1;

void setup()
{
	Serial.begin(9600);

	servo1.attach(SERVO);
}

void loop()
{
	if (Serial.available() > 1)
		servo1.write(Serial.readStringUntil('\n').toInt());

}