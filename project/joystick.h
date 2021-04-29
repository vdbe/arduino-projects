#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h>

#include "Button.h"

// [0, TRIGGERBUFFER] and [1023 - TRIGGERBUFFER, 1023] will be read as joystick movement
#define TRIGGERBUFFER 100 // TODO: Find a better name

// Raw state of the joystick
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

// Actions of the joytstick
union JoystickAction
{
	byte raw;
	struct
	{ // Not set in the first cycle (by design)
		byte north : 1;
		byte south : 1;
		byte east : 1;
		byte west : 1;
		byte padding : 4;
	} repeat; // One direction has been active for 2 cycles (with repeat delay)

	struct
	{
		byte padding : 4;
		byte north : 1;
		byte south : 1;
		byte east : 1;
		byte west : 1;
	} hold; // Only set when previous state was neutral (with repeat delay)
};

class Joystick
{
public:
	Joystick(void);
	Joystick(uint8_t VRxPin, uint8_t VRyPin, uint8_t SWPin);

	void attach(uint8_t, uint8_t, uint8_t);

	// Returns `this->joystickState`
	JoystickState getState(void);

	bool loop(void);

	// Returns -1, 0 or 1
	int8_t getX(bool);
	int8_t getY(bool);

	// Returns `this->joystickAction`
	JoystickAction getAction(void);
	// Returns `this->button.ispressed()`
	bool isPressed(void);

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

Joystick::Joystick(void){
	// Do nothing
};

Joystick::Joystick(uint8_t VRxPin, uint8_t VRyPin, uint8_t SWPin)
{
	this->attach(VRxPin, VRyPin, SWPin);
}

void Joystick::attach(uint8_t VRxPin, uint8_t VRyPin, uint8_t SWPin)
{
	// TODO: Add setter for all harcoded values
	pinMode(SWPin, INPUT_PULLUP);

	this->SWPin = SWPin;
	this->VRxPin = VRxPin;
	this->VRyPin = VRyPin;

	this->button.attach(this->SWPin);
	this->button.setDebounceTime(50);

	// Values chosen by testing
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
	read = analogRead(this->VRyPin); // [0; 1023]

	// Get the direction based of the last bit/MSB
	// read - 512; [-512; 511]
	// >> 15; 1 or 0 -> 1: [-512, -1], 0: [0, 511]
	dir = (read - 512) >> 15;

	// Values chosen by testing
	if (!(read >= TRIGGERBUFFER && read <= 1023 - TRIGGERBUFFER))
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

	if (!(read >= TRIGGERBUFFER && read <= 1023 - TRIGGERBUFFER))
	{
		this->joystickState.west = dir;
		this->joystickState.east = !dir;
	}
	else
	{
		this->joystickState.east = this->joystickState.west = false;
	}

	// Set neutral time
	if (!(this->joystickState.raw & 0b1111))
	{
		this->lastNeutralTime = millis();
		this->repeatDelay = this->defaultRepeatDelay;
	}

	this->button.loop();

	this->lastLoopTime = millis();

	// Prevent recalculating each time getX/Y is called
	this->update_action();

	// Return 1 if anything chaged in the raw states
	return this->joystickState.raw != this->lastJoystickState.raw;
}

int8_t Joystick::getY(bool hold)
{
	uint8_t input = this->joystickAction.repeat.padding & 0b11;

	// The repeat action does not trigger in the first cycle (by design)
	if (hold)
	{
		input |= this->joystickAction.hold.padding & 0b11;
	}

	if (input)
	{
		return input == 1 ? 1 : -1;
	}

	return 0;
}

int8_t Joystick::getX(bool hold)
{
	uint8_t input = (this->joystickAction.repeat.padding >> 2) & 0b11;

	// The repeat action does not trigger in the first cycle (by design)
	if (hold)
	{
		input |= (this->joystickAction.hold.padding >> 2) & 0b11;
	}

	if (input)
	{
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
	if ((this->lastLoopTime - this->lastNeutralTime) > this->repeatDelay)
	{
		// Update Repeat delay
		if (this->repeatDelay > this->minRepeatDelay)
		{
			this->repeatDelay -= (this->repeatDelay - this->minRepeatDelay) >> 2;
		}

		this->lastNeutralTime = this->lastLoopTime;
		action = raw;
	}

	// Single step
	if (raw && !(this->lastJoystickState.raw & 0b1111))
	{
		action |= (raw << 4);
	}

	this->joystickAction.raw = action;
}
#endif