#ifndef Counter_h
#define Counter_h

#define MINDRAWTIME 100

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Field.h"

union CounterLocation
{
	struct
	{
		byte row : 1;
		byte column : 4;
		byte size : 3;
	};
	byte raw;
};

class Counter : public Field
{
public:
	Counter(void);
	Counter(LiquidCrystal_I2C *, uint8_t, uint8_t, int16_t, int16_t, char leadingChar);
	void action(int8_t, bool);
	void draw(bool);
	void redraw(void);
	void clear(void);
	void init(LiquidCrystal_I2C *, uint8_t, uint8_t, int16_t, int16_t, char leadingChar);
	void update(int8_t, bool);
	void underline(bool);

	void save(void);
	void reset(void);

private:
	CounterLocation counterLocation;
	int16_t max;
	int16_t value, saved_value;
	LiquidCrystal_I2C *lcd;
	uint8_t getCounterSize(int16_t);
	uint8_t llen;
	bool underlined, needDraw;
	char leadingChar;

	uint32_t lastDrawn;
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

void Counter::init(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column, int16_t max, int16_t value, char leadingChar)
{
	this->lcd = lcd;
	this->leadingChar = leadingChar;
	this->counterLocation.row = row;
	this->counterLocation.column = column + (leadingChar == 0 ? 0 : 1);
	this->counterLocation.size = this->getCounterSize(max);
	this->max = max;
	this->value = value;
	this->llen = 0;
	this->underlined = false;
}

void Counter::action(int8_t change, bool click)
{
	if (click)
	{
		this->reset();
	}
	else
	{
		this->update(change, true);
	}
}

void Counter::save(void)
{
	this->saved_value = this->value;
}

void Counter::reset(void)
{
	this->value = this->saved_value;
	this->draw(true);
}

void Counter::draw(bool _force)
{
	/* // Not usefull since JoyStick minRepeatDelay 200ms is
	if(!force && (millis() - this->lastDrawn) < MINDRAWTIME) {
		return;
	}
	this->lastDrawn = millis();
	*/

	uint8_t len;
	len = this->getCounterSize(this->value); // Prbly not worth calculating every time

	// Set cursor to start of counter
	this->lcd->setCursor(this->counterLocation.column, this->counterLocation.row);

	// Pad left with zeros
	// TODO: Find a way to write all '0' at once instead of a loop
	for (uint8_t i = len; i < this->counterLocation.size; i++)
	{
		this->lcd->print("0");
	}

	// Write the value
	this->lcd->print(this->value);

	//this->llen = len
}

void Counter::redraw()
{
	this->lcd->setCursor(this->counterLocation.column + (this->leadingChar == 0 ? 0 : -1),
						 this->counterLocation.row);
	this->underlined = !this->underlined;

	if (this->leadingChar)
	{
		this->lcd->print(this->leadingChar);
	}

	this->draw(true);

	this->underline(!this->underlined);
}

void Counter::clear()
{
	Serial.println("\tCounter::clear()"); // DEBUG
	uint8_t start = this->counterLocation.column + (this->leadingChar == 0 ? 0 : -1);
	uint8_t l = start + counterLocation.size;

	this->underline(false);

	this->lcd->setCursor(start, this->counterLocation.row);

	// TODO: Find a way to write all '0' at once instead of a loop
	for (uint8_t i = 0; i < l; i++)
	{
		this->lcd->print(" ");
	}
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

	this->lcd->setCursor(this->counterLocation.column, this->counterLocation.row + 1);

	for (uint8_t i = 0; i < this->counterLocation.size; i++)
	{
		lcd->print(ch);
	}

	this->underlined = underline;
}

// Private functions
uint8_t Counter::getCounterSize(int16_t max)
{
	// TODO: Check what is faster this or log
	for (uint8_t i = 1; i < 8; i++)
	{
		// NOTE: pow takes up 6% of program storage space
		// TODO: write smaller pow if possible
		if (max < (pow(10, i)))
		{
			return i;
		}
	}

	// Should never be reached
	return -1;
}

#endif
