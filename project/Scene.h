#ifndef Scene_h
#define Scene_h

#include <Arduino.h>
#include "Field.h"

class Scene
{
public:
	Scene(void);
	void draw(bool);
	void redraw(void);
	void add(Field*);
	void action(int8_t, int8_t, bool);
	void updateIdx(int8_t, bool);
	void underline(bool);
	void setup(void);

	int16_t idx;

private:
	// TODO: Use Vec
	Field *fields[15];
	uint8_t length;
};

Scene::Scene(void) {
	// Do nothing
}

void Scene::setup(void) {
	this->idx = 0;
	this->length = 0;

	this->underline(true);
	this->redraw();
}

void Scene::add(Field* field)
{
	this->fields[this->length] = field;

	this->length++;
}

void Scene::action(int8_t changeX, int8_t changeY, bool click) {
	if(changeY || click) {
		this->fields[this->idx]->action(changeY, click);
	}
	
	if(changeX) {
		this->updateIdx(changeX, true);
	}
}

void Scene::draw(bool force) {
	this->fields[this->idx]->draw(force);
}

void Scene::redraw()
{
	for (uint8_t i = 0; i < this->length; i++)
	{
		this->fields[i]->redraw();
	}
}

void Scene::updateIdx(int8_t nidx, bool relative)
{
	this->fields[this->idx]->underline(false);

	if (relative) {
		this->idx += nidx;
	} else {
		this->idx = nidx;
	}
	
	this->idx %= this->length;

	if (this->idx < 0)
	{
		this->idx = this->length - 1;
	}

	this->fields[this->idx]->underline(true);
}

void Scene::underline(bool underline) {
	this->fields[this->idx]->underline(underline);
}

#endif