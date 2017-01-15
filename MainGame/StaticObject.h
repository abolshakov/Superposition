#pragma once
#include "WorldObject.h"

class StaticObject : public WorldObject
{
public:
	StaticObject(std::string name, FloatRect boundingBox, IntRect textureBox);
};

