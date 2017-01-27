#include "DynamicObject.h"

DynamicObject::DynamicObject(std::string objectName, Vector2f centerPosition) : WorldObject(objectName, centerPosition)
{
	speed = 0;
	direction = STAND;	
}

void DynamicObject::handleInput()
{
}




