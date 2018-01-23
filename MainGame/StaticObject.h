#pragma once
#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "WorldObject.h"

class StaticObject : public WorldObject
{
public:
	int getSpriteNumber() override { return currentSprite; }
	StaticObject(std::string objectName, Vector2f centerPosition);
	virtual void setType(int newType) = 0;
};

#endif