#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"

enum Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };

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