#pragma once
#include "StaticObject.h"

class TerrainObject : public StaticObject
{
public:
	TerrainObject(std::string name, FloatRect boundingBox, IntRect textureBox);
};

