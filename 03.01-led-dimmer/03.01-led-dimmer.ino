#define LEDPIN 11
//#define DEBUG

const int interval = 5884;

unsigned long currentTime = 0;
#ifdef DEBUG
unsigned long startTime;
#endif

void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	while (!Serial);
	#endif

	pinMode(LEDPIN, OUTPUT);

	#ifdef DEBUG
	Serial.println("Setup complete");
	#endif
}

void loop()
{
	#ifdef DEBUG
	Serial.println("Start loop");
	startTime = micros();
	#endif

	for (unsigned int i = 0; i < 255;)
	{
		if (micros() < (currentTime + interval))
			continue;
			currentTime += interval;
			analogWrite(LEDPIN, ++i);
	}
	#ifdef DEBUG
	Serial.println("Full brightness");
	#endif

	for (unsigned int i = 255; i > 0;)
	{
		if (micros() < currentTime + interval)
			continue;

			currentTime += interval;
			analogWrite(LEDPIN, --i);
	}

	#ifdef DEBUG
	currentTime = micros();
	Serial.print("Off\nLoop took ");
	Serial.print(currentTime - startTime, DEC);
	Serial.println(" microseconds.");
	#endif
}