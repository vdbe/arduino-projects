/*
 * Digital input
 * Input ingesteld op pin 2 Led op pin 13
 * door: vdbe
 */

#define LED 13
#define BUTTON 2

unsigned short ledState = LOW;
unsigned short lastButtonState;
unsigned short currentButtonState;

void setup()
{
	pinMode(BUTTON, INPUT);
	pinMode(LED, OUTPUT);

	currentButtonState = digitalRead(BUTTON);
}

void loop()
{
	lastButtonState = currentButtonState;
	currentButtonState = digitalRead(BUTTON);

	if(lastButtonState == HIGH && currentButtonState == LOW)
	{
		ledState = !ledState;
		digitalWrite(LED, ledState);
	}
}
