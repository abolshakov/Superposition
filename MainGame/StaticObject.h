#pragma once
#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "WorldObject.h"

class StaticObject : public WorldObject
{
public:
	StaticObject(std::string objectName, Vector2f centerPosition);
	virtual ~StaticObject();
	int getSpriteNumber() override { return currentSprite[0]; }	
	virtual void setType(int newType) = 0;
};

#endif