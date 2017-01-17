#include "Stone.h"

Stone::Stone(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize) : TerrainObject(name, boundingBox, textureOffset, textureBoxSize)
{
	strength = 100;
	smoothBorderX = Vector2f(box.width * 3 / 5, box.width * 2 / 5);
	smoothBorderY = Vector2f(box.height * 3 / 2, box.height * 2 / 5);
}

const std::string Stone::getSpriteName(long long elapsedTime)
{
	return "stone.png";
}


