#pragma once
#include "WorldObject.h"

class StaticObject : public WorldObject
{
public:
	StaticObject(std::string objectName, Vector2f centerPosition);
};

