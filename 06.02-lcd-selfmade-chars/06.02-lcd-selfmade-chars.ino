#include <LiquidCrystal_I2C.h>
#include "chars.h"

#define CHAR 2
#define DEBUG

// No idea how you get to 0x27 but it works
LiquidCrystal_I2C lcd(0x27, 16, 2);

signed char barWrittenUntil;

void setup(void);
void loop(void);
void initLoadingBar(void);
void loadingBar(int);
void fillRest(signed char);
void printThumbsup(unsigned char);

void setup(void)
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	lcd.init();

	#if CHAR == 0
	lcd.createChar(0, smiley);
	lcd.write(byte(0));
	#elif CHAR == 1
	for (unsigned char i = 0; i < 6; i++)
		lcd.createChar(i, thumbsup[i]);
	printThumbsup(6);
	#elif CHAR == 2
	for (unsigned char i = 0; i < 7; i++)
		lcd.createChar(i, loadingbar[i]);
	initLoadingBar();
	#endif

	lcd.backlight();

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}

void loop(void)
{
	#if CHAR == 2
	#ifdef DEBUG
	if (Serial.available() > 1) {
		int progress = Serial.readStringUntil('\n').toInt();

		loadingBar(progress);
	}
	#else
	for (unsigned char i = 0; i <= 100; i++)
	{
		lcd.setCursor(0, 0);
		lcd.print("loading ");
		lcd.print(i, DEC);
		lcd.print("%  ");

		loadingBar(i);
		delay(100);
	}

	#endif
	#else
	exit(0);
	#endif
}

void initLoadingBar()
{
	barWrittenUntil = 16;
	lcd.setCursor(0, 0);
	lcd.print("loading ");
}

void loadingBar(int progress)
{
	signed char p = map(progress, 0, 100, 0, 32);
	signed char writtenUntilNew = p / 2;

	lcd.setCursor(8, 0);
	lcd.print(progress, DEC);
	lcd.print("%  ");

	lcd.setCursor(0, 1);

	if (p > 1)
	{
		lcd.write(byte(1));
		// TODO: Dont overwrite full blocks
		for (signed char i = 1; i < writtenUntilNew && i < 15; i++)
		{
			lcd.write(byte(4));
		}
		if (p > 30)
		{
			lcd.write(byte(6));
		}
		else if (writtenUntilNew * 2 != p)
		{
			writtenUntilNew++;
			lcd.write(byte(3));
		}
	}


	#ifdef DEBUG
	Serial.print(progress, DEC);
	Serial.print(" => ");
	Serial.print(p, DEC);
	Serial.print(" => ");
	Serial.println(writtenUntilNew, DEC);
	#endif

	fillRest(writtenUntilNew);
	barWrittenUntil = writtenUntilNew;
}

void fillRest(signed char writtenUntilNew)
{
	while (writtenUntilNew < barWrittenUntil)
	{
		switch (++writtenUntilNew)
		{
			case 16:
				lcd.write(byte(5));
				break;
			case 1:
				lcd.write(byte(0));
				break;
			default:
				lcd.write(byte(2));
				break;
		}
	}
}

void printThumbsup(unsigned char leftCol)
{
	lcd.setCursor(leftCol, 0);
	for (unsigned char i = 0; i < 3; i++)
		lcd.write(byte(i));

	lcd.setCursor(leftCol, 1);
	for (unsigned char i = 3; i < 6; i++)
		lcd.write(byte(i));
}