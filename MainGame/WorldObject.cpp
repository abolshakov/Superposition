#include "WorldObject.h"

WorldObject::WorldObject(Vector2f position, Vector2u size, std::string name, std::string type)
{
	this->position = position;
	this->size = size;
	this->name = name;
	this->type = type;
}

WorldObject::~WorldObject()
{
}

