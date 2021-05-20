#ifndef Chars_h
#define Chars_h
#include <LiquidCrystal_I2C.h>

void load_chars(LiquidCrystal_I2C *lcd)
{
	byte UNDERLINE[] = {B00000, B11111, B11111, B00000, B00000, B00000, B00000, B00000};
	byte ARROWUP[] = {B00000, B00100, B01110, B11111, B00100, B00100, B00100, B00000};
	byte ARROWDOWN[] = {B00000, B00100, B00100, B00100, B11111, B01110, B00100, B00000};
	byte SWITCHSCENE[] = {B00100, B01110, B11111, B00100, B00100, B11111, B01110, B00100};

	lcd->createChar(0, UNDERLINE);
	lcd->createChar(1, ARROWUP);
	lcd->createChar(2, ARROWDOWN);
	lcd->createChar(3, SWITCHSCENE);
}

#endif
