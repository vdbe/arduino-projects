#define COIL0 8
#define COIL1 9
#define COIL2 10
#define COIL3 11

#define DELAY 10
#define DEBUG

#include "stepperMotor.h"

unsigned char coilPins1[] = { COIL0, COIL1, COIL2, COIL3 };
const unsigned char sequence1[] = { 0b0001, 0b0010, 0b0100, 0b1000 };

unsigned char currentState;

StepperMotor sm;
void setup()
{
	#ifdef DEBUG
	// Setup Serial
	Serial.begin(9600);
	#endif
	sm.attach(coilPins1);

	//currentState = 0b0000;
	sm.setFullStep(false);
	sm.setSteps(-500);

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}



void loop()
{
	//sm.setSteps(-4096);
	delay(9999);
}

