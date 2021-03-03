/*
 * Digital input
 * Input ingesteld op pin 2 Led op pin 13
 * door: vdbe
 */
#define NOINCLUDE
 
#ifndef NOINCLUDE
#include <ezButton.h>
#endif

#define LED 13
#define BUTTON 2

#ifndef NOINLUCDE
#define DEBOUNCE 50
#endif

unsigned short ledState = LOW;
#ifdef NOINCLUDE
unsigned short lastButtonState;
unsigned short currentButtonState;
#else
ezButton button(BUTTON);
#endif

void setup()
{
	#ifdef DEBUG
	// Setup Serial
	Serial.begin(9600);
	while (!Serial);
	Serial.println("01.04-digital-input");
	#endif

	#ifdef NOINCLUDE
	pinMode(BUTTON, INPUT);
	pinMode(LED, OUTPUT);

	currentButtonState = digitalRead(BUTTON);
	#else
	button.setDebounceTime(DEBOUNCE);
	#endif

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}

void loop()
{
	#ifdef NOINCLUDE
	lastButtonState = currentButtonState;
	currentButtonState = digitalRead(BUTTON);

	if(lastButtonState == HIGH && currentButtonState == LOW)
	{
		ledState = !ledState;
		digitalWrite(LED, ledState);

		#ifdef DEBUG
		Serial.print("Button pressed: ");
		Serial.println(ledState);
		#endif
	}
	#else
	button.loop();

	if(button.isPressed())
	{
		ledState = !ledState;
		digitalWrite(LED, ledState);

		#ifdef DEBUG
		Serial.print("Button pressed: ");
		Serial.println(ledState);
		#endif
	}
	#endif
}
