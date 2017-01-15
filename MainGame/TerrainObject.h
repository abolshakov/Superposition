#pragma once
#include "StaticObject.h"

class TerrainObject : public StaticObject
{
public:
	TerrainObject(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize);
};

