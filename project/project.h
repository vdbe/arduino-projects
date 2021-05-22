#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include <wire.h>
#include <RTClib.h>

// Software config
// time is in seconds
#define SLEEPTIME 10
// Max: 60
// TODO: Need a better name for this
#define CHECKALARMEVERY 1

// Hardware config
// Joystick
#define VRXPIN A1
#define VRYPIN A0
#define SWPIN 5

// StepperMotor
#define STEPPERDELAY 1
#define STEPPERSTEPS 1024
#define COIL0 8
#define COIL1 9
#define COIL2 10
#define COIL3 11

// Debug config
#define DEBUG 0
#define REPORTEVERY 10
//  0 - Nothing
//  1 - void loop()
//  2 - bool Joystick::loop()
// >2 - Only void setup();
#define PARTCHECK 1


#if DEBUG==0
#undef PARTCHECK
#endif

#if PARTCHECK > 1
bool report = false;
#endif

unsigned long currentMillis;
#ifdef CURRENTMILLIS
#undef CURRENTMILLIS
#endif
#define CURRENTMILLIS currentMillis

#ifndef CURRENTMILLIS
#define CURRENTMILLIS millis()
#endif

#include "Joystick.h"
#include "Clock.h"
#include "Chars.h"
#include "Scene.h"
#include "SceneSwitcher.h"
#include "StepperMotor.h"

#ifdef CURRENTMILLIS
#undef CURRENTMILLIS
#endif
#define CURRENTMILLIS currentMillis

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

unsigned long lastActionTime;

void checkAlarms(void);
bool compareTime(uint8_t, uint8_t, uint8_t, uint8_t, bool *);
void loop(void);
void moveStepperMotor(int32_t steps);
void setup(void);
void setup_scenes(void);
void setRtcScreenClock(void);
void setRtcTime(void);
void updateIdx(int8_t, bool);

void setup_scenes()
{
	// Create Fields
	onTime.init(&lcd, 0, 0, byte(1));
	offTime.init(&lcd, 0, 7, byte(2));

	rtcTime.init(&lcd, 0, 0, 'T');

	// Setup Scene objects
	scene.init(5);
	rtcScene.init(3);

	// Add fields to scenes
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
}
