#include <LiquidCrystal_I2C.h>

#define VERSION 2

// No idea how you get to 0x27 but it works
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned char i = 0;

void setup(void);
void loop(void);
void printNum(int, int, char);

void setup()
{
	lcd.init();
	lcd.backlight();

	#if VERSION == 1
	lcd.setCursor(0, 0);
	lcd.print("Hello world!");
	#else
	Serial.begin(9600);
	#endif
}


void loop()
{
	#if VERSION == 1
	lcd.setCursor(0, 1);
	if (i == 10) {
		lcd.printstr("0 ");
		i = 1;
	}
	else {
		lcd.print(i++);
	}

	delay(1000);
	#else
	if (Serial.available() > 5)
	{
		delay(500);
		int number, places;
		char paddingChar;

		number = Serial.readStringUntil(' ').toInt();
		places = Serial.readStringUntil(' ').toInt();
		paddingChar = Serial.read();

		Serial.readStringUntil('\n');

		#ifdef DEBUG
		Serial.print(number, DEC);
		Serial.print(", ");
		Serial.print(places, DEC);
		Serial.print(", ");
		Serial.write(paddingChar);
		Serial.println("");
		#endif

		printNum(number, places, paddingChar);
		#endif
	}
}

void printNum(int number, int places, char paddingChar)
{
	signed char paddingLen = places - (floor(log10(number)) + 1);

	#ifdef DEBUG
	Serial.print("number length: ");
	Serial.println(floor(log10(number)) + 1, DEC);
	Serial.print("paddingLen: ");
	Serial.println(paddingLen, DEC);
	#endif

	lcd.setCursor(0, 0);

	if (paddingLen < 0) {
		lcd.print("ERROR");
		places = 5;
	} else {
		for (unsigned char i = 0; i < paddingLen; i++)
			lcd.print(paddingChar);

		lcd.print(number);
	}

	for (unsigned char j = places; j < i; j++)
		lcd.print(" ");


	i = places;
}