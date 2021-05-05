#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "Joystick.h"
#include "Clock.h"
#include "Chars.h"
#include "Scene.h"
#include "SceneSwitcher.h"

#define VRxPin A1
#define VRyPin A0
#define SWPin 5

Joystick joystick;
LiquidCrystal_I2C lcd(0x27, 16, 2);

Clock time1;
Clock time2;
Clock time3;

SceneSwitcher sceneSwitcher(15);

Scene scene;
Scene scene2;
Scene *scenes[2];

uint8_t s;

void setup()
{
	Serial.begin(9600); // DEBUG

	joystick.attach(VRxPin, VRyPin, SWPin);

	// Reset lcd
	lcd.clear(); // No idea if clear and init are both needed
	lcd.init();

	// Load custom chars to lcd
	load_chars(&lcd);

	// Create Fields
	time1.init(&lcd, 0, 0, byte(1));
	time2.init(&lcd, 0, 7, byte(2));

	time3.init(&lcd, 0, 0, 'X');

	// Add fields of alarms to scenes
	scene.add(&time1.hours);
	scene.add(&time1.minutes);

	scene.add(&time2.hours);
	scene.add(&time2.minutes);
	
	scene.add(&sceneSwitcher);

	scene2.add(&time3.hours);
	scene2.add(&time3.minutes);

	scene2.add(&sceneSwitcher);
	
	// Add scene object to scenes array
	scenes[0] = &scene;
	scenes[1] = &scene2;

	// Setup the scene (redraw, underline)
	s = 0;
	scenes[s]->setup();

	lcd.backlight();
}

void loop()
{
	// Update joystick values
	joystick.loop();

	if (joystick.isPressed())
	{
		Serial.print("Clear scene: "); // DEBUG
		Serial.println(s, DEC);		   // DEBUG

		scenes[s]->clear();

		s = !s; // Toggle s
		scenes[s]->setup();

		Serial.print("Redraw scene: "); // DEBUG
		Serial.println(s, DEC);			// DEBUG
		Serial.println("");				// DEBUG

		return;
	}

	// Hand events over to the active scene
	scenes[s]->action(joystick.getX(false), joystick.getY(true), joystick.isPressed());
}
