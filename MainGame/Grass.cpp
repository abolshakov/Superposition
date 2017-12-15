#include "Grass.h"
#include "Helper.h"

Grass::Grass(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	this->typeOfObject = typeOfObject;
	conditionalSizeUnits = Vector2f(500, 300);
	isTerrain = true;
	strength = 0;
}

Vector2i Grass::calculateTextureOffset()
{	
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 2));
}

std::string Grass::getSpriteName(long long elapsedTime)
{
	return "grass" + std::to_string(typeOfObject) + ".png";
}