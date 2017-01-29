#include "Stone.h"

Stone::Stone(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	strength = 0;
}

Vector2i Stone::calculateTextureOffset()
{
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.5));
}

void Stone::initPedestal()
{
	ellipseSize = int(textureBox.width * 1.125);
	focus1 = Vector2i(textureBox.left + textureBox.width / 25, int(position.y));
	focus2 = Vector2i(textureBox.left + textureBox.width - textureBox.width / 25, int(position.y));
}

std::string Stone::getSpriteName(long long elapsedTime)
{
	return "stone.png";
}


