#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"

enum Direction { STAND, LEFT, RIGHT, UP, DOWN, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT };

class DynamicObject : public WorldObject
{
	Vector2f focus1, focus2;
public:
	float speed;
	Direction direction;
	Vector2f motion;
	int ellipseSize;

	DynamicObject(Vector2f position, Vector2u size, std::string name);
	virtual void handleInput();
};

#endif