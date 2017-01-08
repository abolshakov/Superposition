#include "StaticObject.h"

StaticObject::StaticObject(Vector2f position, Vector2u size, string name, string type) : WorldObject(position, size, name, type)
{
	focus1.x = position.x + size.x / 10;
	focus1.y = position.y + 9 * size.y / 10;
	focus2.x = position.x + 9 * size.x / 10;
	focus2.y = position.y + 9 * size.y / 10;
	elipseSize = size.x;
}


