#pragma once
#include "WorldObject.h"

class StaticObject : public WorldObject
{
public:
	StaticObject(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize);
};

