#include "Stone.h"

Stone::Stone(std::string objectName, Vector2f ellipseCenterPosition) : TerrainObject(objectName, ellipseCenterPosition)
{
}

Vector2f Stone::calculateTextureOffset()
{
	return Vector2f(textureBox.width / 2, textureBox.height / 1.5);
}

void Stone::initEllipse()
{
	ellipseSize = textureBox.width*1.125;
	focus1 = Vector2f(textureBox.left + textureBox.width / 25, ellipseCenterPosition.y);
	focus2 = Vector2f(textureBox.left + textureBox.width - textureBox.width / 25, ellipseCenterPosition.y);
}

const std::string Stone::getSpriteName(long long elapsedTime)
{
	return "stone.png";
}


