#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

//#define currentMillis currentMillies	

#ifndef CURRENTMILLIS
#define CURRENTMILLIS millis()
#endif

#include "Joystick.h"
#include "Clock.h"
#include "Chars.h"
#include "Scene.h"
#include "SceneSwitcher.h"

#ifdef CURRENTMILLIS
#undef CURRENTMILLIS
#define CURRENTMILLIS currentMillis
#endif

#define VRxPin A1
#define VRyPin A0
#define SWPin 5

#define SLEEPTIME 10
#define SCENECOUNT 2

Joystick joystick;
LiquidCrystal_I2C lcd(0x27, 16, 2);

Clock onTime;
Clock offTime;
Clock rtcTime;

SceneSwitcher sceneSwitcher(&lcd, 15);

Scene scene;
Scene rtcScene;
Scene *scenes[SCENECOUNT];

int8_t sceneIdx;

unsigned long lastActionTime, currentMillis;

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
	onTime.init(&lcd, 0, 0, byte(1));
	offTime.init(&lcd, 0, 7, byte(2));

	rtcTime.init(&lcd, 0, 0, 'X');

	// Add fields of alarms to scenes
	scene.add(&onTime.hours);
	scene.add(&onTime.minutes);
	scene.add(&offTime.hours);
	scene.add(&offTime.minutes);
	scene.add(&sceneSwitcher);

	rtcScene.add(&rtcTime.hours);
	rtcScene.add(&rtcTime.minutes);
	rtcScene.add(&sceneSwitcher);
	
	// Add callbacks to certain scenes
	rtcScene.pre_setup_callback = setRtcScreenClock;
	rtcScene.post_clear_callback = setRtcTime;

	// Add scene object to scenes array
	scenes[0] = &scene;
	scenes[1] = &rtcScene;

	// Setup the scene (redraw, underline)
	sceneIdx = 0;
	scenes[sceneIdx]->setup();

	// Some extra setup stuff
	lcd.backlight();
	lastActionTime = 0;
}

void loop()
{
	static bool backLight = true;;

	currentMillis = millis();

	checkAlarms();

	// Update joystick values
	joystick.loop();
	
	// Check if somthing happend
	if(!joystick.getState().raw)
	{
		if(backLight && (CURRENTMILLIS - lastActionTime) > SLEEPTIME * 1000) {
			backLight = false;
			lcd.noBacklight();
		}

		return;
	}

	if(!backLight) {
		backLight = true;
		lcd.backlight();
	}
	
	lastActionTime = CURRENTMILLIS;

	uint8_t ret = scenes[sceneIdx]->action(joystick.getX(false), joystick.getY(true), joystick.isPressed());
	
	if (ret) {
		switch (ret)
		{
		case 1:
			updateIdx(-1, true);
			break;
		case 2:
			updateIdx(1, true);
			break;
		
		default:
			break;
		}
	}
}

void updateIdx(int8_t nidx, bool relative)
{
	scenes[sceneIdx]->clear();

	if(relative) {
		sceneIdx += nidx;
	} else {
		sceneIdx = nidx;
	}

	if (sceneIdx < 0) {
		sceneIdx = SCENECOUNT -1;
	} else {
		sceneIdx %= SCENECOUNT;
	}

	scenes[sceneIdx ]->setup();
}


void checkAlarms()
{

	// Alarm1

	// Alarm2
}

void setRtcScreenClock()
{
	rtcTime.set(13, 37);
}

void setRtcTime()
{
	Serial.print("Time: ");
	Serial.print(rtcTime.hours.saved_value, DEC);
	Serial.print(":");
	Serial.print(rtcTime.minutes.saved_value, DEC);
	Serial.println("");
}
