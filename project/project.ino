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

SceneSwitcher sceneSwitcher(&lcd, 15);

Scene scene;
Scene scene2;
Scene *scenes[2];

uint8_t s;

void setup()
{
	Serial.begin(9600); // DEBUG
	Serial.println(""); // DEBUG
	Serial.println(""); // DEBUG
	Serial.println("-------------------------------------"); // DEBUG
	Serial.println(1, DEC); // DEBUG

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

	Serial.println(2, DEC); // DEBUG

	// Add fields of alarms to scenes
	scene.add(&time1.hours);
	scene.add(&time1.minutes);
	
	Serial.println(3, DEC); // DEBUG

	scene.add(&time2.hours);
	scene.add(&time2.minutes);
	
	Serial.println(4, DEC); // DEBUG
	
	scene.add(&sceneSwitcher);

	Serial.println(5, DEC); // DEBUG

	scene2.add(&time3.hours);
	scene2.add(&time3.minutes);

	Serial.println(6, DEC); // DEBUG

	scene2.add(&sceneSwitcher);

	Serial.println(7, DEC); // DEBUG
	
	// Add scene object to scenes array
	scenes[0] = &scene;
	scenes[1] = &scene2;

	Serial.println(8, DEC); // DEBUG

	// Setup the scene (redraw, underline)
	s = 0;
	//scenes[s]->setup();
	scene.setup();

	Serial.println(9, DEC); // DEBUG


	lcd.backlight();
	
	Serial.println(10, DEC); // DEBUG
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
