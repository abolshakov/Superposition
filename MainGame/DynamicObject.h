#pragma once
#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "WorldObject.h"

enum Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };

class DynamicObject : public WorldObject
{
protected:
	Vector2f focus1, focus2, ellipseCenterPosition;
	int ellipseSize;
public:
	float speed;
	Direction direction;

	DynamicObject(std::string objectName, Vector2f ellipseCenterPosition);

	virtual void handleInput();
	Vector2f getellipsePosition() const { return Vector2f(ellipseCenterPosition.x, ellipseCenterPosition.y); }
	Vector2f getFocus1() const { return focus1; }
	Vector2f getFocus2() const { return focus2; }
	void setPosition(Vector2f newPosition);
	int getellipseSize() const { return ellipseSize; }
};

#endif