#ifndef Scene_h
#define Scene_h

#include <Arduino.h>

#include "Field.h"

class Scene
{
public:
	Scene(void);
	// Draw the scene with idx ´this->idx´
	void draw(bool);
	// Redraw every field in the scene
	void redraw(void);
	// Remove all chars from the lcd
	void clear(void);
	// Add a new field to the scene
	void add(Field *);
	// Handle action
	uint8_t action(int8_t, int8_t, bool);
	// Update idx of the current field absolute or relative
	void updateIdx(int8_t, bool);
	// Underline or remove underline from field
	void underline(bool);
	// Setup the Scene (redrawing, setting idx to 0)
	void setup(void);

	int16_t idx;

private:
	// TODO: Use Vec
	Field *fields[15];
	uint8_t length;
};

Scene::Scene(void)
{
	this->length = 0;
	this->idx = 0;
}

void Scene::setup(void)
{
	Serial.println("Scene::setup() -> start");	// DEBUG
	this->underline(true); // Schould just set the underline bool instead of also drawing it, now its drawn by underline() and redraw()
	this->redraw();
	Serial.println("Scene::setup() -> end");	// DEBUG
	delay(100); // DEBUG
}

void Scene::add(Field *field)
{
	// TODO: Add length check
	this->fields[this->length] = field;
	this->length++;
	
	Serial.print("Added field, length: "); // DEBUG
	Serial.println(this->length, DEC);	// DEBUG
}

uint8_t Scene::action(int8_t changeX, int8_t changeY, bool click)
{
	uint8_t ret = 0;
	if (changeY || click)
	{
		ret = this->fields[this->idx]->action(changeY, click);
	}

	// X-axis is used for switching between scenes
	if (changeX)
	{
		this->updateIdx(changeX, true);
	}
	
	return ret;
}

void Scene::draw(bool force)
{
	this->fields[this->idx]->draw(force);
}

void Scene::redraw()
{
	Serial.println("Scene::redraw()");	// DEBUG
	for (uint8_t i = 0; i < this->length; i++)
	{
		Serial.print("\tScene::redraw() field: ");	// DEBUG
		Serial.print(i, DEC);	// DEBUG
		delay(100); // DEBUG
		this->fields[i]->redraw();
		Serial.println("\t SUCCES");	// DEBUG
	}
}

void Scene::clear()
{
	Serial.println("Scene::clear()"); // DEBUG
	for (uint8_t i = 0; i < this->length; i++)
	{
		this->fields[i]->clear();
	}
}

void Scene::updateIdx(int8_t nidx, bool relative)
{
	this->fields[this->idx]->underline(false);
	this->fields[this->idx]->save();

	if (relative)
	{
		this->idx += nidx;
	}
	else
	{
		this->idx = nidx;
	}

	this->idx %= this->length;

	if (this->idx < 0)
	{
		this->idx = this->length - 1;
	}

	this->fields[this->idx]->underline(true);
}

void Scene::underline(bool underline)
{
	Serial.println("Scene::underline()");	// DEBUG
	this->fields[this->idx]->underline(underline);
}

#endif