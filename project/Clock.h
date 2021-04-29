#ifndef Clock_h
#define Clock_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Counter.h"

// Just a wrapper arround `Counter`
class Clock
{
public:
	Counter hours, minutes;

	Clock(void);
	void init(LiquidCrystal_I2C *, uint8_t, uint8_t, char leadingChar);
	void redraw(void);

private:
};

Clock::Clock(void)
{
	// Do nothing
}

void Clock::init(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column, char leadingChar)
{
	this->hours.init(lcd, row, column, 24, 0, leadingChar);

	this->minutes.init(lcd, row, column + 2 + (leadingChar != 0), 60, 0, ':');
}

void Clock::redraw(void)
{
	this->hours.redraw();
	this->minutes.redraw();
}

#endif