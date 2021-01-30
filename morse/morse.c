#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#define UNIT 250

#define SHORT UNIT
#define LONG (UNIT*3)
#define WAIT UNIT
#define PAUSE (UNIT*7)
#define END (UNIT*14)

void dot();
void dash();
void stringToMorse(char*);
int getIndex(char*);


//                   {  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9', }
const char morse[] = { 0x42, 0x81, 0x85, 0x61, 0x20, 0x84, 0x63, 0x80, 0x40, 0x8e, 0x65, 0x82, 0x43, 0x41, 0x67, 0x86, 0x8b, 0x62, 0x60, 0x21, 0x64, 0x88, 0x42, 0x89, 0x8d, 0x83, 0xbf, 0xbe, 0xbc, 0xb8, 0xb0, 0xa0, 0xa1, 0xa3, 0xa7, 0xaf, };


int main(void)
{
	stringToMorse("?");

	return 0;
}

void stringToMorse(char* word)
{
	void (*fptr[2])(void) = { dot, dash };  // Function pointer for dot or dash
	int wordLen, morseLen;  // Length variables
	char c; // Stores the morse byte

	wordLen = strlen(word);

	printf("| ");
	// Loop over each char in word
	for (int i = 0; i < wordLen; ++i, ++word)
	{
		// If space lights off for PAUSE and go to the next char
		if (*word == ' ')
		{
			//lightOff(PAUSE);
			printf("\t");
			continue;
		}

		// Get the morse byte associated with the char
		c = morse[getIndex(word)];
		printf("%x\n", c);

		// Get the first 3 bits wich represents the sum of all dashes and dots
		morseLen = c >> 5 & 0x7;

		// Loop over the last <morselen> bytes in reversed order
		for (int i = 0; i < morseLen; ++i)
		{
			// Call the correct function and wait
			(*fptr[c >> i & 0x1])();  // (c >> i & 0x1) ? dash : dot
			//lightOff(WAIT);
			printf(" ");
		}
		printf("| ");
	}
}

int getIndex(char* c)
{
	if (*c >= 'A' && *c <= 'Z')
		return *c - 'A';

	if (*c >= 'a' && *c <= 'Z')
		return *c - 'a';

	return *c - 22;
}

void dot()
{
	printf(".");
}

void dash()
{
	printf("-");
}
