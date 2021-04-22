#ifndef Chars_h
#define Chars_h
#include <LiquidCrystal_I2C.h>

byte ARROWUP[] = { B00000, B00100, B01110, B11111, B00100, B00100, B00100, B00000 };
byte ARROWDOWN[] = { B00000, B00100, B00100, B00100, B11111, B01110, B00100, B00000 };

void load_arrows(LiquidCrystal_I2C *lcd) {
	lcd->createChar(1, ARROWUP);
	lcd->createChar(2, ARROWDOWN);
}

#endif