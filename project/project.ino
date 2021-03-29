#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Joystick.h"

#define VRxPin A1
#define VRyPin A0
#define SWPin 5

Joystick joystick;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int num = 5, newNum;
bool mode = true;

void setup()
{
	Serial.begin(9600);
	joystick.attach(VRxPin, VRyPin, SWPin);

	lcd.init();
	lcd.backlight();
	lcd.print(num);
}

void loop()
{
	joystick.loop();

	mode ^= joystick.isPressed();

	newNum = (num + joystick.getX(mode));
	if (newNum == num)
		return; // No screen update needed

	lcd.setCursor(0, 0);

	num = newNum % 60;
	if (num < 0)
	{
		num = 59;
	}

	lcd.print(num);
	lcd.print(" ");
}
