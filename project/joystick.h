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

union JoystickAction
{
	byte raw;
	struct
	{
		byte north : 1;
		byte south : 1;
		byte east : 1;
		byte west : 1;
		byte padding: 4;
	} repeat;
	struct
	{
		byte padding: 4;
		byte north : 1;
		byte south : 1;
		byte east : 1;
		byte west : 1;
	} hold;
};




class Joystick
{
public:
	Joystick(void);
	Joystick(uint8_t VRxPin, uint8_t VRyPin, uint8_t SWPin);
	void attach(uint8_t, uint8_t, uint8_t);
	JoystickState getState(void);
	bool isPressed(void);
	bool loop(void);
	int8_t getX(bool);
	int8_t getY(bool);
	JoystickAction getAction(void);

private:
	Button button;
	uint8_t VRxPin, VRyPin, SWPin, debounceDelay;
	JoystickState lastJoystickState, joystickState;
	uint32_t lastNeutralTime, lastLoopTime;
	uint16_t repeatDelay, minRepeatDelay, defaultRepeatDelay;
	JoystickAction joystickAction;

	void update_action();
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
	
	this->joystickState.raw = 0;
}

JoystickState Joystick::getState(void)
{
	return this->joystickState;
}

bool Joystick::isPressed(void)
{
	return this->button.isPressed();
}

bool Joystick::loop()
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

	this->lastLoopTime = millis();

	this->update_action();
	
	return this->joystickState.raw != this->lastJoystickState.raw;
}

int8_t Joystick::getY(bool hold)
{
	uint8_t input = this->joystickAction.repeat.padding & 0b11;
	
	if (hold) {
		input |= this->joystickAction.hold.padding & 0b11;
	}

	if (input) {
		return input == 1 ? 1 : -1;
	}
	
	return 0;
}

int8_t Joystick::getX(bool hold)
{
	uint8_t input = (this->joystickAction.repeat.padding >> 2) & 0b11;
	
	if (hold){
		input |= (this->joystickAction.hold.padding >> 2) & 0b11;
	}

	if (input) {
		return input == 1 ? 1 : -1;
	}
	
	return 0;
}

JoystickAction Joystick::getAction(void)
{
	return this->joystickAction;
}

// Private Functions

void Joystick::update_action()
{
	byte action = 0;
	uint8_t raw = this->joystickState.raw & 0b1111;
	
	// Repeat
	if((this->lastLoopTime - this->lastNeutralTime) > this->repeatDelay) {
		// Update Repeat delay
		if (this->repeatDelay > this->minRepeatDelay) {
			this->repeatDelay -= (this->repeatDelay - this->minRepeatDelay) >> 2;
		}

		this->lastNeutralTime = this->lastLoopTime;
		action = raw;
	}

	// Single step
	if (raw && !(this->lastJoystickState.raw & 0b1111)) {
		action |= (raw << 4);
	}
	
	this->joystickAction.raw = action;
}
#endif