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

private:
	CounterLocation counterLocation;
	int16_t max;
	int16_t value;
	LiquidCrystal_I2C *lcd;
	uint8_t getCounterSize(int16_t);
};

// Public funtions

Counter::Counter(void)
{

}

Counter::Counter(LiquidCrystal_I2C *lcd, uint8_t row, uint8_t column, int16_t max, int16_t value)
{
	this->init(lcd, row, column, max, value);
}

void Counter::draw(void)
{
	this->lcd->setCursor(this->counterLocation.column, this->counterLocation.row);
	this->lcd->print(this->value);

	// TODO: Only needed padding
	this->lcd->print(" ");
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
}

#endif