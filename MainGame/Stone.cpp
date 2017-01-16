#include "Stone.h"

Stone::Stone(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize) : TerrainObject(name, boundingBox, textureOffset, textureBoxSize)
{
	strength = 100;
}

const std::string Stone::getSpriteName(long long elapsedTime)
{
	return "stone.png";
}


