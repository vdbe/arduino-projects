#ifndef Counter_h
#define Counter_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

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

class Counter
{
public:
	Counter(void);
	Counter(LiquidCrystal_I2C *, uint8_t, uint8_t, int16_t, int16_t);
	void draw(void);
	void init(LiquidCrystal_I2C *, uint8_t, uint8_t, int16_t, int16_t);
	void update(int8_t, bool);
	uint8_t underline(void);

private:
	CounterLocation counterLocation;
	int16_t max;
	int16_t value;
	LiquidCrystal_I2C *lcd;
	uint8_t getCounterSize(int16_t);
	uint8_t llen;
	uint8_t underlined;
};

// Public funtions

Counter::Counter(void)
{
	// Do nothing
}

Counter::Counter(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column, int16_t max, int16_t value)
{
	this->init(lcd, row, column, max, value);
}

void Counter::draw(void)
{
	uint8_t len;
	len = this->getCounterSize(this->value);

	// Set cursor to start of counter
	this->lcd->setCursor(this->counterLocation.column, this->counterLocation.row);

	// Pad left with zeros
	for(uint8_t i = len; i < this->counterLocation.size; i++) {
		this->lcd->print("0");
	}

	// Write the value
	this->lcd->print(this->value);

	//this->llen = len;
}

void Counter::update(int8_t value, bool relative)
{
	if(relative && !value)
	{
		return;
	}
	
	this->value = (relative ? this->value + value : value) % this->max;
	if(this->value < 0)
	{
		this->value += this->max;
	}
	this->draw();
}

void Counter::init(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column, int16_t max, int16_t value)
{
	this->lcd = lcd;
	this->counterLocation.row = row;
	this->counterLocation.column = column;
	this->counterLocation.size = this->getCounterSize(max);
	this->max = max;
	this->value = value;
	this->llen = 0;
	this->underlined = 0;
}

uint8_t Counter::underline(void) {
	char ch;

	ch = this->underlined ? ' ' : '_';
	this->lcd->setCursor(this->counterLocation.column, this->counterLocation.row+1);

	for (uint8_t i = 0; i < this->counterLocation.size; i++)
	{
		lcd->print(ch);
	}

	return this->underlined = !this->underlined;
}

// Private functions
uint8_t Counter::getCounterSize(int16_t max)
{
	// TODO: check what is faster this or log
	for (uint8_t i = 1; i < 8; i++)
	{
		if (max < (pow(10, i)))
		{
			return i;
		}
	}
	
	// Should never be reached
	return -1;
}

#endif
