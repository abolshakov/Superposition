#include "Grass.h"
#include "Helper.h"

Grass::Grass(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	isTerrain = true;
	strength = 0;
}

Vector2i Grass::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 2));
}

Vector2f Grass::getScaleRatio()
{
	return Vector2f(Helper::GetScreenSize().x / (originalTextureBox.width * 3), Helper::GetScreenSize().y / (originalTextureBox.height * 2.5));
}

std::string Grass::getSpriteName(long long elapsedTime)
{
	return "grass" + typeOfImage + ".png";
}