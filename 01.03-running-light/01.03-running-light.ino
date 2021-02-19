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

void setup(void);
void loop(void);
void zetOpLeds(unsigned char);

const unsigned char ledPins[] = { LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7 };
const unsigned char sequence[] = { 0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000 };

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

	currentState = 0b00000000;

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}

void zetOpLeds(unsigned char waarde)
{
	#ifdef DEBUG
	Serial.print("\t-> Toggle: ");
	#endif
	for (unsigned char i = 0; i < 8; ++i)
	{
		if((currentState ^ waarde) >> i & 0x1)
		{
			#ifdef DEBUG
			Serial.print(ledPins[i]);
			Serial.print("(");
			Serial.print(!(currentState >> i & 0x1));
			Serial.print("), ");
			#endif
			digitalWrite(ledPins[i], !(currentState >> i & 0x1));
		}
	}
	#ifdef DEBUG
	Serial.println("");
	#endif

	currentState = waarde;
}

void loop()
{
	#ifdef DEBUG
	Serial.println("Start loop");
	#endif
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
