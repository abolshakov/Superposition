#include "Grass.h"
#include "Helper.h"

Grass::Grass(std::string objectName, Vector2f centerPosition, int typeOfObject) : StaticObject(objectName, centerPosition)
{
	varietyOfTypes = 16;
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
	if (this->typeOfObject == 0)
	{
		this->conditionalSizeUnits = Vector2f(1500, 1200);
	}
	else
	{
		this->conditionalSizeUnits = Vector2f(500, 400);
		this->zCoords = 1;
	}
}

Vector2i Grass::calculateTextureOffset()
{	
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 2));
}

std::string Grass::getSpriteName(long long elapsedTime)
{
	return "terrainObjects/grass/grass" + std::to_string(typeOfObject) + ".png";
}