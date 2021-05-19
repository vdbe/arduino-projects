#include "project.h"

void setup()
{
	#if DEBUG==1
	Serial.begin(9600);
	#endif

	joystick.attach(VRXPIN, VRYPIN, SWPIN);

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
	
	setup_scenes();

	// Some extra setup stuff
	lcd.backlight();
	lastActionTime = 0;

	if (!rtc.begin())
	{
		#if DEBUG==1
		Serial.println("[ERROR] Couldn't find RTC");
		exit(1);
		#endif
	}
	if (!rtc.isrunning())
	{
		#if DEBUG==1
		Serial.println("[i] RTC is NOT running!");
		#endif
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(__DATE__, __TIME__));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
}

void loop()
{
	static bool backLight = true;
	
	currentMillis = millis();

	#if DEBUG==1
	static uint16_t cycles = 0;
	static unsigned long lastStatTime = CURRENTMILLIS;

	#if PARTCHECK==1
	static unsigned long timeTotalAlarm = 0, timeTotalJoystickLoop = 0, timeTotalJoystickState = 0, timeTotalAction = 0, timeTotalRet = 0;
	unsigned long timeCheckAlarm, timeCheckJoystickLoop, timeCheckJoystickState, timeCheckAction, timeCheckRet;
	#endif

	if (CURRENTMILLIS > (lastStatTime + (REPORTEVERY * 1000))) {
		Serial.print("[STATS] ");
		Serial.print(cycles, DEC);
		Serial.print(" cycles\t");
		Serial.print(cycles / REPORTEVERY, DEC);
		Serial.print(" cycles/sec\n");

		#if PARTCHECK==1
		unsigned long int timeTotal;
		timeTotal = (timeTotalAlarm / 10) + (timeTotalJoystickLoop / 10) + (timeTotalJoystickState / 10) + (timeTotalAction / 10) + (timeTotalRet / 10);

		Serial.print("[PART] alarm:\t\t");
		Serial.print(((float)timeTotalAlarm / timeTotal) * 10, DEC);
		Serial.print("%\t");
		Serial.print(timeTotalAlarm, DEC);

		Serial.print("ms\n[PART] JS loop:\t\t");
		Serial.print(((float)timeTotalJoystickLoop / timeTotal) * 10, DEC);
		Serial.print("%\t");
		Serial.print(timeTotalJoystickLoop, DEC);

		Serial.print("ms\n[PART] JS state:\t");
		Serial.print(((float)timeTotalJoystickState / timeTotal) * 10, DEC);
		Serial.print("%\t");
		Serial.print(timeTotalJoystickState, DEC);

		Serial.print("ms\n[PART] action:\t\t");
		Serial.print(((float)timeTotalAction / timeTotal) * 10, DEC);
		Serial.print("%\t");
		Serial.print(timeTotalAction, DEC);

		Serial.print("ms\n[PART] ret:\t\t");
		Serial.print(((float)timeTotalRet / timeTotal) * 10, DEC);
		Serial.print("%\t");
		Serial.print(timeTotalRet, DEC);
		Serial.println("ms\n");
	
		timeTotalAlarm = timeTotalJoystickLoop = timeTotalJoystickState = timeTotalAction = timeTotalRet = 0;
		#endif
		cycles = 0;
		currentMillis = millis();
		lastStatTime = CURRENTMILLIS;
	}

	cycles++;
	
	#if PARTCHECK==1
	currentMillis = millis();
	#endif

	#endif
	
	checkAlarms();

	#if DEBUG==1 && PARTCHECK==1
	timeCheckAlarm = millis();
	#endif

	// Update joystick values
	joystick.loop();

	#if DEBUG==1 && PARTCHECK==1
	timeCheckJoystickLoop = millis();
	#endif

	// Check if somthing happend
	if (!joystick.getState().raw)
	{
		if (backLight && (CURRENTMILLIS - lastActionTime) > SLEEPTIME * 1000)
		{
			backLight = false;
			lcd.noBacklight();
		}

		#if DEBUG==1 && PARTCHECK==1
		timeCheckJoystickState = timeCheckAction = timeCheckRet = millis();
		#endif
	} else {
		#if DEBUG==1 && PARTCHECK==1
		timeCheckJoystickState = millis();
		#endif

		if (!backLight)
		{
			backLight = true;
			lcd.backlight();
		}

		lastActionTime = CURRENTMILLIS;

		uint8_t ret = scenes[sceneIdx]->action(joystick.getX(false), joystick.getY(true), joystick.isPressed());

		#if DEBUG==1 && PARTCHECK==1
		timeCheckAction = millis();
		#endif

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

	#if DEBUG==1 && PARTCHECK==1
	timeCheckRet = millis();
	
	timeTotalAlarm += timeCheckAlarm - currentMillis;
	timeTotalJoystickLoop += timeCheckJoystickLoop - timeCheckAlarm;
	timeTotalJoystickState += timeCheckJoystickState - timeCheckJoystickLoop;
	timeTotalAction += timeCheckAction - timeCheckJoystickState;
	timeTotalRet += timeCheckRet - timeCheckAction;
	#endif
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
