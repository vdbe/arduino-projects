/*
 * Running LED / Looplicht
 * 8 LED's aangesloten op 13 t.e.m. 6
 * door: vdbe
 */

#define LED0 6
#define LED1 7
#define LED2 8
#define LED3 9
#define LED4 10
#define LED5 11
#define LED6 12
#define LED7 13

#define DELAY 100

const unsigned char ledPins[] = { LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7 };
const unsigned char sequence[] = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40 };

unsigned char currentState;

void setup()
{
	#ifdef DEBUG
	// Setup Serial
	Serial.begin(9600);
	while (!Serial);
	Serial.println("01.03-running-light");
	#endif

	for(unsigned char i = 0; i < 8; ++i)
		pinMode(ledPins[i], OUTPUT);

	currentState = 0x0;

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif

}

void zetOpLeds(int waarde)
{

	#ifdef DEBUG
	Serial.print("\t -> Toggle: ");
	#endif
	for (unsigned char i = 0; i < 8; ++i)
	{
		if((currentState ^ waarde) >> i & 0x1)
		{
			#ifdef DEBUG
			Serial.print(ledPins[i]);
			Serial.print("(");
			Serial.print(!digitalRead(ledPins[i]));
			Serial.print("), ");
			#endif
			digitalWrite(ledPins[i], !digitalRead(ledPins[i]));
		}
	}
	#ifdef DEBUG
	Serial.print("\n");
	#endif

	currentState = waarde;
}

void loop()
{
	for (int i = 0; i < 14; ++i)
	{
		#ifdef DEBUG
		Serial.print("Frame: ");
		Serial.print(i);
		#endif
		zetOpLeds(sequence[i]);
		delay(DELAY);
	}
}
