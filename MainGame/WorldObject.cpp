#include "WorldObject.h"

WorldObject::WorldObject(Vector2f position, Vector2u size, std::string name)
{
	this->position = position;
	this->size = size;
	this->name = name;
}

WorldObject::~WorldObject()
{
}

