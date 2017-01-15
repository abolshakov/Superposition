#include "Stone.h"

Stone::Stone(std::string name, FloatRect boundingBox, IntRect textureBox) : TerrainObject(name, boundingBox, textureBox)
{
}

const std::string Stone::getSpriteName(long long elapsedTime)
{
	return "stone.png";
}


