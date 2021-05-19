#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include <wire.h>
#include <RTClib.h>

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

RTC_DS1307 rtc;

bool ALARM1;
bool ALARM2;

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

void setRtcScreenClock(void);
void  setRtcTime(void);

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
	
	ALARM1 = false;
	ALARM2 = false;

	rtcTime.init(&lcd, 0, 0, 'T');

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

	if (!rtc.begin())
	{
		Serial.println("Couldn't find RTC");
		while (1);
	}
	if (!rtc.isrunning())
	{
		Serial.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
}

void loop()
{
	static bool backLight = true;

	currentMillis = millis();

	checkAlarms();

	// Update joystick values
	joystick.loop();

	// Check if somthing happend
	if (!joystick.getState().raw)
	{
		if (backLight && (CURRENTMILLIS - lastActionTime) > SLEEPTIME * 1000)
		{
			backLight = false;
			lcd.noBacklight();
		}

		return;
	}

	if (!backLight)
	{
		backLight = true;
		lcd.backlight();
	}

	lastActionTime = CURRENTMILLIS;

	uint8_t ret = scenes[sceneIdx]->action(joystick.getX(false), joystick.getY(true), joystick.isPressed());

	if (ret)
	{
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

	if (relative)
	{
		sceneIdx += nidx;
	}
	else
	{
		sceneIdx = nidx;
	}

	if (sceneIdx < 0)
	{
		sceneIdx = SCENECOUNT - 1;
	}
	else
	{
		sceneIdx %= SCENECOUNT;
	}

	scenes[sceneIdx]->setup();
}

void checkAlarms()
{
	DateTime now = rtc.now();

	// Alarm1
	if(compareTime(now.hour(), now.minute(), onTime.hours.saved_value, onTime.minutes.saved_value, &ALARM1))
	{
		Serial.println("Turn on");

	}

	// Alarm2
	if(compareTime(now.hour(), now.minute(), offTime.hours.saved_value, offTime.minutes.saved_value, &ALARM2))
	{
		Serial.println("Turn off");
	}
}

bool compareTime(uint8_t hour1, uint8_t minute1, uint8_t hour2, uint8_t minute2, bool *triggerd)
{
	if (minute1 == minute2 && hour1 == hour2)
	{
		if(!*triggerd)
		{
			*triggerd = true;
			return true;
		} else {
			return false;
		}
			
	}

	if (*triggerd) 
	{
		*triggerd = false;
	}
	
	return false;
}

void setRtcScreenClock()
{
	DateTime now = rtc.now();

	rtcTime.set(now.hour(), now.minute());
}

void setRtcTime()
{
	DateTime now = rtc.now();
	
	// NOTE: Maybe I schould set secs to 0
	rtc.adjust(DateTime(now.year(), now.month(), now.day(), rtcTime.hours.saved_value, rtcTime.minutes.saved_value, now.second()));
}
