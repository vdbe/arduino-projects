#define BLUE_LED 11
#define GREEN_LED 12
#define RED_LED 13

const unsigned char ledPins[] = {RED_LED, GREEN_LED, BLUE_LED};

char signalStr[4];
unsigned char pin;

void setup()
{
	//Stel hier de pinnen in
	for (unsigned char i = 0; i < 3; i++)
	{
		pinMode(ledPins[i], OUTPUT);
		digitalWrite(ledPins[i], 0);
	}

	//Stel hier de Baud rate in
	Serial.begin(9600);
}

void loop()
{
	if(Serial.available() < 4)
	{
		delay(50);
		return;
	}

	Serial.readBytesUntil('\n', signalStr, 4);

	switch (signalStr[0])
	{
		case 'r':
			pin = 0;
			break;
		case 'g':
			pin = 1;
			break;
		default:
			pin = 2;
			break;
	}

	digitalWrite(ledPins[pin], signalStr[2] == '1');
}