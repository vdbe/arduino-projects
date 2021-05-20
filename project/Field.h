#ifndef Field_h
#define Field_h

#include <Arduino.h>
#include "Misc.h"

// Parent class for all fields
class Field
{
public:
	// Update screen
	virtual void draw(bool);
	// Redraw everything of this field
	virtual void redraw(void);
	// Clear the lcd
	virtual void clear(void);
	// Handle action (up/down, click)
	virtual uint8_t action(int8_t, bool);
	// Underline or remove underline from field
	virtual void underline(bool); // TODO: Make this not virtual
	// Save Field
	virtual void save(void);
	// Reset field to last save
	virtual void reset(void);

private:
};

#endif
