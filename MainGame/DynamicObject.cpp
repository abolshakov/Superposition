#include "DynamicObject.h"

DynamicObject::DynamicObject(Vector2f position, Vector2u size, std::string name) : WorldObject(position, size, name)
{
	focus1.x = position.x + size.x / 10;
	focus1.y = position.y + 9 * size.y / 10;
	focus2.x = position.x + 9 * size.x / 10;
	focus2.y = position.y + 9 * size.y / 10;
	ellipseSize = size.x;
	speed = 0;
	direction = STAND;
}

void DynamicObject::handleInput()
{
}




