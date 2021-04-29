#ifndef Button_h
#define Button_h

#include <Arduino.h>

class Button
{
public:
	Button(void);
	Button(uint8_t);
	void attach(uint8_t);
	bool getState(void);
	void loop(void);
	bool isPressed(void);
	void setDebounceTime(uint8_t);

private:
	uint8_t buttonPin;

	uint8_t debounceTime;

	bool lastButtonState, buttonState;
	uint32_t lastDebounceTime;
	bool pressed;
};

Button::Button(void)
{
	// Do nothing
}

Button::Button(uint8_t buttonPin)
{
	this->attach(buttonPin);
}

void Button::attach(uint8_t buttonPin)
{
	this->buttonPin = buttonPin;

	// TODO: Describe INPUT_PULLUP
	pinMode(buttonPin, INPUT_PULLUP);

	this->lastButtonState = this->buttonState = digitalRead(buttonPin);
}

bool Button::getState(void)
{
	return this->buttonState;
}

bool Button::isPressed(void)
{
	return this->pressed;
}

void Button::loop(void)
{
	// FROM: https://www.arduino.cc/en/pmwiki.php?n=Tutorial/Debounce
	// read the state of the switch into a local variable:
	bool reading = digitalRead(buttonPin);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != this->lastButtonState)
	{
		// reset the debouncing timer
		this->lastDebounceTime = millis();
	}

	if ((millis() - this->lastDebounceTime) > this->debounceTime)
	{
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != this->buttonState)
		{
			this->buttonState = reading;

			// Trigger on press instead of release (Use == HIGH for trigger on release)
			if (this->buttonState == LOW)
			{
				// save the reading. Next time through the loop, it'll be the lastButtonState:
				this->lastButtonState = reading;
				this->pressed = 1;
				return;
			}
		}
		this->pressed = 0;
	}

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	this->lastButtonState = reading;
}

void Button::setDebounceTime(uint8_t time)
{
	this->debounceTime = time;
}

#endif