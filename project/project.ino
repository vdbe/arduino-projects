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
	Serial.begin(9600); // DEBUG

	joystick.attach(VRxPin, VRyPin, SWPin);

	// Reset lcd
	lcd.clear(); // No ide if clear and init are both needed
	lcd.init();

	// Load custom chars to lcd
	load_chars(&lcd);

	// Create both alarms
	time1.init(&lcd, 0, 0, byte(1));
	time2.init(&lcd, 0, 7, byte(2));

	// Add fields of alarms to scene
	scene.add(&time1.hours);
	scene.add(&time1.minutes);

	scene.add(&time2.hours);
	scene.add(&time2.minutes);

	// Underline the current field
	scene.underline(true);

	// Setup the scene (redraw, underline)
	scene.setup();
}

void loop()
{
	// Update joystick values
	joystick.loop();

	// Hand events over to the active scene
	scene.action(joystick.getX(false), joystick.getY(true), false);
}
