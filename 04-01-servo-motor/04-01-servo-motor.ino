#include <Servo.h>
#include <ezButton.h>

//#define DEBUG

#define BUTTON 2
#define SERVO 10

#define ANGLE 180
#define STEPSIZE 1	// WARNING: currentAngle is unsigned going below zero will jump to 255 (or lower) there is no check for this!!!

Servo servo1;
ezButton button(BUTTON);

unsigned char state;
unsigned char currentAngle;
unsigned char targetAngle;

void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	servo1.attach(SERVO);

	state, currentAngle, targetAngle = 0;

	servo1.write(state*ANGLE);

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}

void loop()
{
	button.loop();

	if (button.isPressed())
	{
		state = !state;
		if (currentAngle == 0 || currentAngle == targetAngle)
		{
			// Servo reached target angle

			targetAngle = state*ANGLE;

			#ifdef DEBUG
			Serial.print("Target angle ");
			Serial.println(targetAngle, DEC);
			#endif
		}
		else
		{
			// Servo has not reached target angle and needs to be paused

			targetAngle = currentAngle;

			#ifdef DEBUG
			Serial.print("Pausing at angle ");
			Serial.println(currentAngle, DEC);
			#endif
		}
	}

	if(targetAngle != currentAngle)
	{
		// Servo needs to be moved

		/*
		if(targetAngle < currentAngle)
			currentAngle--;
		else
			currentAngle++;
		*/
		currentAngle += targetAngle < currentAngle ? -STEPSIZE : STEPSIZE;

		servo1.write(currentAngle);

		#ifdef DEBUG
		//Serial.print("Current angle ");
		//Serial.println(currentAngle, DEC);
		delay(10);
		#endif
	}
}