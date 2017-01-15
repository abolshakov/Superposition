#include "DynamicObject.h"

DynamicObject::DynamicObject(std::string name, FloatRect boundingBox, IntRect textureBox) : WorldObject(name, boundingBox, textureBox)
{
	speed = 0;
	direction = STAND;
}

void DynamicObject::handleInput()
{
}




