#ifndef Counter_h
#define Counter_h

#define MINDRAWTIME 100

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Field.h"
#include "Misc.h"

#ifndef CURRENTMILLIS
#define CURRENTMILLIS millis()
#endif

class Counter : public Field
{
public:
	int16_t value, saved_value;

	Counter(void);
	Counter(LiquidCrystal_I2C *, uint8_t, uint8_t, int16_t, int16_t, char leadingChar);
	uint8_t action(int8_t, bool);
	void clear(void);
	void draw(bool);
	void init(LiquidCrystal_I2C *, uint8_t, uint8_t, int16_t, int16_t, char leadingChar);
	void redraw(void);
	void reset(void);
	void save(void);
	void update(int8_t, bool);
	void underline(bool);

private:
	bool underlined, needDraw;
	char leadingChar;
	uint8_t getCounterSize(int16_t);
	int16_t max;
	uint32_t lastDrawn;
	LcdLocation location;
	LiquidCrystal_I2C *lcd;
};

// Public funtions

Counter::Counter(void)
{
	// Do nothing
}

Counter::Counter(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column, int16_t max, int16_t value, char leadingChar)
{
	this->init(lcd, row, column, max, value, leadingChar);
}

uint8_t Counter::action(int8_t change, bool click)
{
	if (click)
	{
		this->reset();
	}
	else
	{
		this->update(change, true);
	}

	return 0;
}

void Counter::clear()
{
	uint8_t start = this->location.column + (this->leadingChar == 0 ? 0 : -1);
	uint8_t l = start + location.size;

	this->underline(false);

	this->lcd->setCursor(start, this->location.row);

	// TODO: Find a way to write all '0' at once instead of a loop
	for (uint8_t i = 0; i < l; i++)
	{
		this->lcd->print(" ");
	}
}

void Counter::draw(bool _force)
{
	/* // Not usefull since JoyStick minRepeatDelay 200ms is
	if(!force && (CURRENTMILLIS - this->lastDrawn) < MINDRAWTIME) {
		return;
	}
	this->lastDrawn = CURRENTMILLIS;
	*/

	uint8_t len;
	len = this->getCounterSize(this->value); // Prbly not worth calculating every time

	// Set cursor to start of counter
	this->lcd->setCursor(this->location.column, this->location.row);

	// Pad left with zeros
	// TODO: Find a way to write all '0' at once instead of a loop
	for (uint8_t i = len; i < this->location.size; i++)
	{
		this->lcd->print("0");
	}

	// Write the value
	this->lcd->print(this->value);
}

void Counter::init(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column, int16_t max, int16_t value, char leadingChar)
{
	this->lcd = lcd;
	this->leadingChar = leadingChar;
	this->location.row = row;
	this->location.column = column + (leadingChar == 0 ? 0 : 1);
	this->location.size = this->getCounterSize(max);
	this->max = max;
	this->value = value;
}

void Counter::reset(void)
{
	this->value = this->saved_value;
	this->draw(true);
}

void Counter::redraw()
{
	this->lcd->setCursor(this->location.column + (this->leadingChar == 0 ? 0 : -1),
						 this->location.row);

	if (this->leadingChar)
	{
		this->lcd->print(this->leadingChar);
	}

	this->draw(true);
}

void Counter::save(void)
{
	this->saved_value = this->value;
}

void Counter::update(int8_t value, bool relative)
{
	if (!value && relative)
	{
		return;
	}

	this->value = (relative ? this->value + value : value) % this->max;

	// Why does -1 % 4 not equal 3
	if (this->value < 0)
	{
		this->value += this->max;
	}

	this->draw(false);
}

void Counter::underline(bool underline)
{
	if (underline == this->underlined)
	{
		// Already in correct state
		return;
	}

	char ch = underline ? byte(0) : ' ';

	this->lcd->setCursor(this->location.column, this->location.row + 1);

	for (uint8_t i = 0; i <= this->location.size; i++)
	{
		this->lcd->print(ch);
	}

	this->underlined = underline;
}

// Private functions
uint8_t Counter::getCounterSize(int16_t max)
{
	return floor(log10(max));
}

#endif
