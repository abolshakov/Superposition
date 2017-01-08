#include "WorldObject.h"

WorldObject::WorldObject(Vector2f position, Vector2u size, string name, string type)
{
	this->position = position;
	this->size = size;
	this->name = name;
	this->type = type;
}

