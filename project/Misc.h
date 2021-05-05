
#ifndef Misc_h
#define Misc_h

union LcdLocation
{
	struct
	{
		byte row : 1;
		byte column : 4;
		byte size : 3;
	};
	byte raw;
};

#endif