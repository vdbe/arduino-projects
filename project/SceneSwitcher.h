#ifndef SceneSwitcher_h
#define SceneSwitcher_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Misc.h"
#include "Field.h"

class SceneSwitcher : public Field
{
public:
	SceneSwitcher(LiquidCrystal_I2C *, uint8_t);
	// Handle action (up/down, click)
	uint8_t action(int8_t, bool);
	// Clear the lcd
	void clear(void);
	// Update screen
	void draw(bool){};
	// Redraw everything of this field
	void redraw(void);
	// Reset field to last save
	void reset(void){};
	// Save Field
	void save(void){};
	// Underline or remove underline from field
	void underline(bool);

private:
	bool underlined;
	char symbol;
	uint8_t column;
	LiquidCrystal_I2C *lcd;
	LcdLocation location;
};

SceneSwitcher::SceneSwitcher(LiquidCrystal_I2C *lcd, uint8_t column)
{
	this->underlined = false;
	this->symbol = byte(3);
	this->location.size = 1;
	this->location.row = 0;
	this->location.column = column;
	this->lcd = lcd;
}

uint8_t SceneSwitcher::action(int8_t change, bool _click)
{
	switch (change)
	{
	case -1:
		return 1;
		break;

	case 1:
		return 2;
		break;

	default:
		return 0;
		break;
	}
}

void SceneSwitcher::clear()
{
	this->lcd->setCursor(this->location.column, this->location.row);

	this->lcd->print(" ");

	this->underline(false);
}

void SceneSwitcher::redraw()
{
	this->lcd->setCursor(this->location.column, this->location.row);

	this->lcd->print(this->symbol);
}

void SceneSwitcher::underline(bool underline)
{
	if (underline == this->underlined)
	{
		// Already in correct state
		return;
	}

	char ch = underline ? byte(0) : ' ';

	this->lcd->setCursor(this->location.column, this->location.row + 1);

	for (uint8_t i = 0; i < this->location.size; i++)
	{
		this->lcd->print(ch);
	}

	this->underlined = underline;
}

#endif
