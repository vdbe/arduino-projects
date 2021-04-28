#ifndef Field_h
#define Field_h

#include <Arduino.h>

class Field {
	public:
	virtual void draw(bool);
	virtual void redraw(void);
	virtual void action(int8_t, bool);
	virtual void underline(bool);
	private:
};

#endif