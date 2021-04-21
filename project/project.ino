#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Joystick.h"
#include "Clock.h"

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

Counter * fields[4];
int8_t fieldIdx;


void setup()
{
	Serial.begin(9600);
	joystick.attach(VRxPin, VRyPin, SWPin);
	lcd.clear();
	lcd.init();
	time1.init(&lcd, 0, 0);

	time2.init(&lcd, 0, 7);
	lcd.backlight();

	fields[0] = &time1.hours;
	fields[1] = &time1.minutes;
	
	
	fields[2] = &time2.hours;
	fields[3] = &time2.minutes;

	fieldIdx = 0;
	fields[fieldIdx]->underline();
}

void loop()
{
	joystick.loop();

	fields[fieldIdx]->update(joystick.getX(mode), true);

	int8_t b = joystick.getY(false);
	if(b) {
		fields[fieldIdx]->underline();
		fieldIdx = (fieldIdx + b) % 4;
		if(fieldIdx < 0) {
			fieldIdx = 3;
		}
		fields[fieldIdx]->underline();
	}

}
