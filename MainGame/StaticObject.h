#pragma once
#include "WorldObject.h"

class StaticObject : public WorldObject
{
	Vector2f focus1, focus2;
public:
	StaticObject(Vector2f position, Vector2u size, std::string name, std::string type);
	int elipseSize;
};

