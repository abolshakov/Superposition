#include "DynamicObject.h"

DynamicObject::DynamicObject(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize) : WorldObject(name, boundingBox, textureOffset, textureBoxSize)
{
	speed = 0;
	direction = STAND;
}

void DynamicObject::handleInput()
{
}




