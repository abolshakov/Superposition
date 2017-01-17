#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"

enum Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };

class DynamicObject : public WorldObject
{
public:
	float speed;
	Direction direction;
	int ellipseSize;

	DynamicObject(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize);
	virtual void handleInput();
};

#endif