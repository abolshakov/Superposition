#include "DynamicObject.h"

DynamicObject::DynamicObject(Vector2f position, Vector2u size, std::string name, std::string type) : WorldObject(position, size, name, type)
{
	focus1.x = position.x + size.x / 10;
	focus1.y = position.y + 9 * size.y / 10;
	focus2.x = position.x + 9 * size.x / 10;
	focus2.y = position.y + 9 * size.y / 10;
	elipseSize = size.x;
	speed = 0;
	sqrtspeed = 0;
	direction = STAND;
}



