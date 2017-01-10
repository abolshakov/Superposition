#include "Stone.h"

Stone::Stone(Vector2f position, Vector2u size, std::string name) : TerrainObject(position, size, name)
{
	focus1.x = position.x + size.x / 10;
	focus1.y = position.y + 9 * size.y / 10;
	focus2.x = position.x + 9 * size.x / 10;
	focus2.y = position.y + 9 * size.y / 10;
	ellipseSize = size.x;
}

std::string Stone::getSpriteName(long long elapsedTime)
{
	return "stone.png";
}


