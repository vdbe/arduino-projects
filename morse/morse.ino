// # Defines
#define UNIT 250
#define DEBUG

#define SHORT UNIT
#define LONG (UNIT*3)
#define WAIT UNIT
#define PAUSE (UNIT*7)
#define END (UNIT*14)

// # Functions
void setup(void);
void loop(void);
void stringToMorse(const char*);
unsigned char getIndex(const char *c);
void lightOff(unsigned short delayTime);
void dot(void);
void dash(void);

// # Global variables
//                   {  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9', }
const unsigned char morse[] = { 0x42, 0x81, 0x85, 0x61, 0x20, 0x84, 0x63, 0x80, 0x40, 0x8e, 0x65, 0x82, 0x43, 0x41, 0x67, 0x86, 0x8b, 0x62, 0x60, 0x21, 0x64, 0x88, 0x42, 0x89, 0x8d, 0x83, 0xbf, 0xbe, 0xbc, 0xb8, 0xb0, 0xa0, 0xa1, 0xa3, 0xa7, 0xaf, };
void (*fptr[2])(void) = { dot, dash };  // Function pointer for dot or dash

#ifndef DEBUG
// Message to be morsed
const char *SENTENCE = (const char*)"SOS";
#endif

void setup(void) {
	#ifdef DEBUG
	// Setup Serial
	Serial.begin(9600);
	while (!Serial);

	Serial.println("Morse code");
	#endif

	pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void)
{
	#ifdef DEBUG
	String str;
	int len;

	// Get string from serial
	str = Serial.readString();
	// Remove leading and trailing spaces
	str.trim();

	len = str.length() + 1;

	// Copy the string to a char array
	char SENTENCE[len];
	str.toCharArray(SENTENCE, len);

	Serial.print("\n\nSentence: ");
	Serial.println(SENTENCE);
	#endif

	stringToMorse(SENTENCE);
	lightOff(END);
}

void stringToMorse(const char* word)
{
	unsigned char c; // Stores the morse byte
	unsigned char wordLen, morseLen;  // Length variables
	wordLen = strlen(word);

	// Loop over each char in word
	for (unsigned char i = 0; i < wordLen; ++i, ++word)
	{
		// If space lights off for PAUSE and go to the next char
		if (*word == ' ')
		{
			#ifdef DEBUG
			Serial.println();
			#endif

			lightOff(PAUSE);
			continue;
		}

		// Get the morse byte associated with the char 
		c = morse[getIndex(word)];

		// Get the first 3 bits wich represents the sum of all dashes and dots
		morseLen = c >> 5 & 0x7;

		#ifdef DEBUG
		Serial.print(*word);
		Serial.print(" [");
		Serial.print(getIndex(word), DEC);
		Serial.print("] -> 0x");
		Serial.print(c, HEX);
		Serial.print(": ");
		Serial.print(morseLen, BIN);
		Serial.print(" ");
		#endif

		// Loop over the last <morselen> bytes in reversed order 
		for (unsigned char i = 0; i < morseLen; ++i)
		{
			#ifdef DEBUG
			Serial.print(c >> i & 0x1, BIN);
			#endif

			// Call the correct function and wait
			(*fptr[c >> i & 0x1])();  // (c >> i & 0x1) ? dash() : dot() 
			lightOff(WAIT);
		}

		#ifdef DEBUG
		Serial.println();
		#endif
	}
}

unsigned char getIndex(const char* c)
{
	if (*c >= 'A' && *c <= 'Z')
		return *c - 'A';

	if (*c >= 'a' && *c <= 'z')
		return *c - 'a';

	return *c - 22;
}

void lightOff(unsigned short delayTime)
{
	digitalWrite(LED_BUILTIN, LOW);
	delay(delayTime);
}

void dot(void)
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(SHORT);
}

void dash(void)
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(LONG);
}
