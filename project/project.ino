#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Joystick.h"
#include "Clock.h"
#include "Chars.h"
#include "Scene.h"

#define VRxPin A1
#define VRyPin A0
#define SWPin 5

Joystick joystick;
LiquidCrystal_I2C lcd(0x27, 16, 2);

Clock time1;
Clock time2;

Scene scene;

void setup()
{
	Serial.begin(9600);

	joystick.attach(VRxPin, VRyPin, SWPin);

	lcd.clear();
	lcd.init();
	
	load_chars(&lcd);

	time1.init(&lcd, 0, 0, byte(1));

	time2.init(&lcd, 0, 7, byte(2));
	
	scene.add(&time1.hours);
	scene.add(&time1.minutes);

	scene.add(&time2.hours);
	scene.add(&time2.minutes);
	
	scene.underline(true);
	
	scene.redraw();
	
	lcd.backlight();
}

void loop()
{
	joystick.loop();

	scene.action(joystick.getX(false), joystick.getY(true), false);
}
