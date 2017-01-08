#include "TerrainObject.h"

TerrainObject::TerrainObject(Vector2f position, Vector2u size, string name, string type) : StaticObject(position, size, name, type)
{
	focus1.x = position.x + size.x / 10;
	focus1.y = position.y + 9 * size.y / 10;
	focus2.x = position.x + 9 * size.x / 10;
	focus2.y = position.y + 9 * size.y / 10;
	elipseSize = size.x;
}
bool TerrainObject::isTouch(Vector2f position)
{
	return (sqrt((focus1.x - position.x)*(focus1.x - position.x) + (focus1.y - position.y)*(focus1.y - position.y)) + sqrt((focus2.x - position.x)*(focus2.x - position.x) + (focus2.y - position.y)*(focus2.y - position.y)) <= elipseSize);
}


