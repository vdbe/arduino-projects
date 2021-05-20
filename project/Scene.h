#ifndef Scene_h
#define Scene_h

#include <Arduino.h>

#include "Field.h"

class Scene
{
public:
	int16_t idx;
	callback_function pre_setup_callback;
	callback_function post_clear_callback;

	Scene(void);
	// Handle action
	uint8_t action(int8_t, int8_t, bool);
	// Add a new field to the scene
	void add(Field *);
	// Remove all chars from the lcd
	void clear(void);
	// Draw the scene with idx ´this->idx´
	void draw(bool);
	// Redraw every field in the scene
	void redraw(void);
	// Setup the Scene (redrawing, setting idx to 0)
	void setup(void);
	// Update idx of the current field absolute or relative
	void updateIdx(int8_t, bool);
	// Underline or remove underline from field
	void underline(bool);

private:
	// TODO: Use Vec
	Field *fields[15];
	uint8_t length;
};

Scene::Scene(void)
{
	this->length = 0;
	this->idx = 0;

	this->pre_setup_callback = this->post_clear_callback = NULL;
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

void Scene::add(Field *field)
{
	// TODO: Add length check
	this->fields[this->length] = field;
	this->length++;
}

void Scene::clear()
{
	this->fields[this->idx]->underline(false);
	for (uint8_t i = 0; i < this->length; i++)
	{
		this->fields[i]->clear();
	}

	if(this->post_clear_callback != NULL)
	{
		this->post_clear_callback();
	}
}

void Scene::draw(bool force)
{
	this->fields[this->idx]->draw(force);
}

void Scene::redraw()
{
	for (uint8_t i = 0; i < this->length; i++)
	{
		this->fields[i]->redraw();
	}
}

void Scene::setup(void)
{
	if(this->pre_setup_callback != NULL)
	{
		this->pre_setup_callback();
	}
	this->redraw();
	this->underline(true);
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
	this->fields[this->idx]->underline(underline);
}

#endif
