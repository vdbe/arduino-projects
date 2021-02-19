/*
 * Programma met de 3 kleuren led
 * Aangesloten op 13, 12 en 11
 * door: evdb
*/
#define DEBUG

#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED 11

#define DELAY 2000

void setup(void);
void loop(void);

const unsigned char ledPins[] = {RED_LED, GREEN_LED, BLUE_LED};
const unsigned char sequence[] = {0x1, 0x3, 0x2, 0x6, 0x4, 0x5, 0x7};
//const unsigned char sequence[] = {0x1, 0x1, 0x2, 0x2, 0x4, 0x4, 0x0};

unsigned char currentState;

#ifdef DEBUG
const char* colors[] = {"Rood", "Geel", "Groen", "Cyaan", "Blauw", "Magneta", "Wit"};
//const char* colors[] = {"Rood", "Rood", "Groen", "Groen", "Blauw", "Blauw", "Uit"};
#endif

void setup() {
	#ifdef DEBUG
	// Setup Serial
	Serial.begin(9600);
	while (!Serial);
	Serial.println("01.02-blinky-rgb");
	#endif

	for (unsigned char i = 0; i < 3; i++)
		pinMode(ledPins[i], OUTPUT);

	currentState = 0x0;

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}

void loop() {
	#ifdef DEBUG
	Serial.println("Start loop");
	#endif
	for (unsigned char i = 0; i < 7; ++i)
	{
		#ifdef DEBUG
		Serial.print(colors[i]);
		Serial.print("(");
		Serial.print(i, DEC);
		Serial.print(") : ");
		Serial.print(currentState, BIN);
		Serial.print(" ^ ");
		Serial.print(sequence[i], BIN);
		Serial.print(" = ");
		Serial.print(currentState ^ sequence[i], BIN);
		Serial.print("\t-> Toggle: ");
		#endif

		// Toggle the lights that need to be turned on/off,
		// this is a xor between the current and wanted state
		for (unsigned char j = 0; j < 3; j++)
		{

			if((currentState ^ sequence[i]) >> j & 0x1)
			{
				#ifdef DEBUG
				Serial.print(ledPins[j]);
				Serial.print("(");
				Serial.print(!digitalRead(ledPins[j]));
				Serial.print("), ");
				#endif
				digitalWrite(ledPins[j], !(currentState >> j & 0x1));
			}
		}
		#ifdef DEBUG
		Serial.println("");
		#endif
		currentState = sequence[i];
		delay(DELAY);
	}

	#ifdef DEBUG
	Serial.println("End loop");
	#endif
}