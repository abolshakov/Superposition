#include "Ground.h"
#include "Helper.h"

Ground::Ground(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 5;
	this->typeOfObject = typeOfObject;
	isBackground = true;
	toSaveName = "ground";
	setType(typeOfObject);
	tag = forestTreeTag;
}

void Ground::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(1000, 1000);
	this->zCoords = typeOfObject;
}

Vector2i Ground::calculateTextureOffset()
{
	return Vector2i(0, 0);
}

std::string Ground::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/ground/ground" + std::to_string(typeOfObject) + ".png";
}