#ifndef Clock_h
#define Clock_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Counter.h"

class Clock
{
public:
	Clock(void);
	void init(LiquidCrystal_I2C *, uint8_t, uint8_t);
	Counter hours, minutes;

private:
};

Clock::Clock(void) {
	// Do nothing
}

void Clock::init(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column)
{
	this->hours.init(lcd, row, column, 24, 0);

	lcd->setCursor(column+2, row);
	lcd->print(":");

	this->minutes.init(lcd, row, column+3, 60, 0);

	this->hours.draw();
	this->minutes.draw();
}

#endif