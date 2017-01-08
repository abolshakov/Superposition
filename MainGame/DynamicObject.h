#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"

class DynamicObject : public WorldObject
{
	Vector2f focus1, focus2;
public:
	DynamicObject(Vector2f position, Vector2u size, string name, string type);
	int elipseSize, speed, sqrtspeed;
	enum directions {LEFT, RIGHT, UP, DOWN, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT, STAND};
	directions direction;
	Vector2f motion;
};

#endif