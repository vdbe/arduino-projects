#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Joystick.h"
#include "Counter.h"

#define VRxPin A1
#define VRyPin A0
#define SWPin 5

Joystick joystick;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int num = 5, newNum;
bool mode = true;

Counter counter;

void setup()
{
	Serial.begin(9600);
	joystick.attach(VRxPin, VRyPin, SWPin);
	lcd.clear();
	lcd.init();
	counter.init(&lcd, 0, 0, 60, 50);
	counter.draw();
	lcd.backlight();
}

void loop()
{
	joystick.loop();

	mode ^= joystick.isPressed();
	
	counter.update(joystick.getX(mode), true);
}
