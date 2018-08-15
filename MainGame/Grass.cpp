#include "Grass.h"
#include "Helper.h"

Grass::Grass(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 9;
	this->typeOfObject = typeOfObject;
	isBackground = true;
	toSaveName = "grass";
	setType(typeOfObject);
}

void Grass::setType(int typeOfObject)
{
	if (typeOfObject == -1)
		return;

	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(250, 250);
	this->zCoords = 1;
}

Vector2i Grass::calculateTextureOffset()
{	
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	
	if (typeOfObject == 0)
		return Vector2i(0, 0);	
	
	return Vector2i(textureBox.width / 2, int(textureBox.height / 2));
}

std::string Grass::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/grass/grass" + std::to_string(typeOfObject) + ".png";
}