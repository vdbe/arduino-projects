#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h>

#include "Button.h"

union JoystickState
{
	struct
	{
		byte north : 1;
		byte south : 1;
		byte east : 1;
		byte west : 1;
		byte buttonState : 1;

		byte repeat : 1; // Not used
		byte pressed : 1;
	};
	byte raw;
};

class Joystick
{
public:
	Joystick(void);
	Joystick(uint8_t VRxPin, uint8_t VRyPin, uint8_t SWPin);
	void attach(uint8_t, uint8_t, uint8_t);
	JoystickState getState(void);
	bool isPressed(void);
	void loop(void);
	int8_t getX(bool);
	int8_t getY(bool);

private:
	Button button;
	uint8_t VRxPin, VRyPin, SWPin, debounceDelay;
	JoystickState lastJoystickState, joystickState;
	uint32_t lastNeutralTime;
	uint16_t repeatDelay, minRepeatDelay, defaultRepeatDelay;

	int8_t getDir(bool, bool, bool);
};

// Public funtions

Joystick::Joystick(void)
{
	// Do nothing
};

Joystick::Joystick(uint8_t VRxPin, uint8_t VRyPin, uint8_t SWPin)
{
	this->attach(VRxPin, VRyPin, SWPin);
}

void Joystick::attach(uint8_t VRxPin, uint8_t VRyPin, uint8_t SWPin)
{
	pinMode(SWPin, INPUT_PULLUP);

	this->SWPin = SWPin;
	this->VRxPin = VRxPin;
	this->VRyPin = VRyPin;

	this->button.attach(this->SWPin);
	this->button.setDebounceTime(50);

	this->defaultRepeatDelay = this->repeatDelay = 500;
	this->minRepeatDelay = 200;
}

JoystickState Joystick::getState(void)
{
	return this->joystickState;
}

bool Joystick::isPressed(void)
{
	return this->button.isPressed();
}

void Joystick::loop()
{
	int read;
	bool dir;
	this->lastJoystickState = this->joystickState;

	// TODO: Compress to function called once per axis
	// Set directions
	read = analogRead(this->VRyPin);
	dir = (read - 512) >> 15;

	if (!(read >= 100 && read <= 923))
	{
		this->joystickState.north = dir;
		this->joystickState.south = !dir;
	}
	else
	{
		this->joystickState.north = this->joystickState.south = false;
	}

	read = analogRead(this->VRxPin);
	dir = (read - 512) >> 15;

	if (!(read >= 100 && read <= 923))
	{
		this->joystickState.west = dir;
		this->joystickState.east = !dir;
	}
	else
	{
		this->joystickState.east = this->joystickState.west = false;
	}

	// Set neutral time
	// TODO: possibly place logic in Joystick::getDir
	if (!(this->joystickState.raw & 0b1111))
	{
		this->lastNeutralTime = millis();
		this->repeatDelay = this->defaultRepeatDelay;
	}

	this->button.loop();
}

int8_t Joystick::getX(bool hold)
{
	return this->getDir(joystickState.north, joystickState.south, hold);
}

int8_t Joystick::getY(bool hold)
{
	return this->getDir(joystickState.east, joystickState.west, hold);
}

// Private Functions

int8_t Joystick::getDir(bool positive, bool negative, bool hold)
{
	uint8_t raw = joystickState.raw & 0b1111;

	uint32_t time;

	if (hold && (time = millis()) && raw && (time - this->lastNeutralTime) > this->repeatDelay)
	{
		if (this->repeatDelay > this->minRepeatDelay)
		{
			this->repeatDelay -= (this->repeatDelay - this->minRepeatDelay) >> 2;
		}

		this->lastNeutralTime = time;
	}
	else if (!(raw && !(lastJoystickState.raw & 0b1111)))
	{
		return 0;
	}

	if (positive)
	{
		return 1;
	}
	else if (negative) // NOTE: Check not needed
	{
		return -1;
	}
}
#endif