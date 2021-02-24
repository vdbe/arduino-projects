/*
	02.01 Hello World
	Deze code print over de seriele poort (UART)
	elke seconde "Hello World"
	(vergeet de seriele monitor niet te openen)
 */
#define DELAY 1000

unsigned char count;
char buf[17];

void setup() {
	Serial.begin(115200);
	count = 0;
}

void loop() {
	#ifdef PRINT
	//Serial.print(count++, DEC);
	Serial.print(count++, HEX);
	Serial.println(F(" Hello World"));
	#else
	sprintf(buf, "%d Hello World\n", count++);
	Serial.write(buf, strlen(buf));
	#endif
	delay(DELAY);
}
