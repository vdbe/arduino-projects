#define DEBUG
#define _5v

#define READPIN A0

#define INTERVAL 250

#define MINANALOGEVALUE 0
#define MINVOLTAGE 0

#ifdef _5v
#define MAXANALOGEVALUE 1023
#define MAXVOLTAGE 5
#else
#define MAXANALOGEVALUE 677
#define MAXVOLTAGE 3.3
#endif

unsigned long currentTime;
unsigned val;

void setup(void);
void loop(void);
void loop(void);
float floatMap(float, float, float, float, float);

void setup()
{
	Serial.begin(9600);

	val = currentTime = 0;

	#ifdef DEBUG
	Serial.println("Setup complete");
	Serial.print("Max voltage: ");
	Serial.print(MAXVOLTAGE, DEC);
	#endif
}

void loop()
{
	if (millis() < (currentTime + INTERVAL))
			return;
	currentTime += INTERVAL;

	val = analogRead(READPIN);

	Serial.print("Analog: ");
	Serial.print(val, DEC);
	Serial.print(", Voltage: ");
	Serial.println(floatMap(val, MINANALOGEVALUE, MAXANALOGEVALUE, MINVOLTAGE, MAXVOLTAGE), DEC);
}

// https://www.arduino.cc/reference/en/language/functions/math/map/
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}