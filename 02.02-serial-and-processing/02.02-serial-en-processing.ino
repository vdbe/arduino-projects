#include <ezButton.h>

#define BUTTON1 3
#define BUTTON2 2

#define DEBOUNCE 50
#define DEFAULT_DELAY 50

ezButton button1(BUTTON1);
ezButton button2(BUTTON2);

void setup()
{
	//Stel hier de pinnen in
	button1.setDebounceTime(DEBOUNCE);
	button2.setDebounceTime(DEBOUNCE);

	//Stel hier de Baud rate in
	Serial.begin(9600);

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}

void loop() {
	button1.loop();
	button2.loop();

	unsigned char choice = button1.isPressed() | button2.isPressed() << 1;

	if (choice)
	{
		#ifdef DEBUG
		Serial.print("Choice: ");
		Serial.println(choice, DEC);
		#else
		Serial.write('0' + choice);
		delay(DEFAULT_DELAY * choice);
		#endif
	}

	delay(DEFAULT_DELAY);
}