#include <ezButton.h>

//#define DEBUG

#define LED 11
#define BUTTON 5
#define LDR A0

#define DEBOUNCE 50
#define HERTZ 500

#define MINANALOGEVALUE 0
#define MAXANALOGEVALUE 1023

unsigned char ledMode;
unsigned char change;
unsigned short ldrVolt;
unsigned char dutycycle;
unsigned long currentTime;
unsigned short interval = 1000 / HERTZ + 0.5;

#ifdef DEBUG
unsigned short counter;
#endif

ezButton button(BUTTON);

void setup(void);
void loop(void);
unsigned char readButton(ezButton);


void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	ledMode = 1;

	pinMode(LED, OUTPUT);
	button.setDebounceTime(DEBOUNCE);

	#ifdef DEBUG
	Serial.println("Setup complete");
	Serial.print("Interval: ");
	Serial.print(interval, DEC);
	#endif
}

void loop()
{
	button.loop();

	change = readButton(button);
	ledMode ^= change;

	if(ledMode)
	{
		if (millis() < (currentTime + interval))
			return;
		currentTime += interval;

		ldrVolt = analogRead(LDR);
		dutycycle = map(ldrVolt, MINANALOGEVALUE, MAXANALOGEVALUE, 0, 255);
		analogWrite(LED, dutycycle);

		#ifdef DEBUG
		if(counter++ == 100)
		{
			Serial.print("LDR: ");
			Serial.print(ldrVolt, DEC);
			Serial.print(", dutycycle: ");
			Serial.println(dutycycle, DEC);
			counter = 0;
		}
		#endif
	}
	else if (change)
	{
		digitalWrite(LED, HIGH);

		#ifdef DEBUG
		Serial.print("PWM, dutycycle: ");
		Serial.println(255, DEC);
		#endif
	}
}

unsigned char readButton(ezButton b)
{
	return b.isPressed();
}
