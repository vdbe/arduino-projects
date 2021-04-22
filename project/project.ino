#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Joystick.h"
#include "Clock.h"
#include "Chars.h"

#define VRxPin A1
#define VRyPin A0
#define SWPin 5

Joystick joystick;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int num = 5, newNum;
bool mode = true;

//Counter counter;
Clock time1;
Clock time2;

Counter *fields[4];
int8_t fieldIdx;
JoystickAction joystickAction;

int b = 0;

void setup()
{
	Serial.begin(9600);

	joystick.attach(VRxPin, VRyPin, SWPin);

	lcd.clear();
	lcd.init();
	
	load_chars(&lcd);

	time1.init(&lcd, 0, 0, byte(1));

	time2.init(&lcd, 0, 7, byte(2));

	fields[0] = &time1.hours;
	fields[1] = &time1.minutes;

	fields[2] = &time2.hours;
	fields[3] = &time2.minutes;

	fieldIdx = 0;

	time1.redraw();
	time2.redraw();
	lcd.backlight();

	fields[fieldIdx]->underline(true);
}

void loop()
{
	joystick.loop();

	joystickAction = joystick.getAction();

	fields[fieldIdx]->action(joystick.getY(mode), false);

	int8_t b = joystick.getX(false);
	if (b)
	{
		// Not Needed since the MINDRAWTIME is commented out in Counter.draw
		//fields[fieldIdx]->draw(true);

		fields[fieldIdx]->underline(false);
		fieldIdx = (fieldIdx + b) % 4;
		if (fieldIdx < 0)
		{
			fieldIdx = 3;
		}
		fields[fieldIdx]->underline(true);
	}
	
}
