#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"

enum Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };

class DynamicObject : public WorldObject
{
protected:
	Vector2f focus1, focus2;
	int ellipseSize;
public:
	float speed;
	Direction direction;

	DynamicObject(std::string objectName, Vector2f centerPosition);

	virtual void handleInput();
	
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	
	int getEllipseSize() const { return ellipseSize; }
};

#endif