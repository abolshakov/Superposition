#include "Brazier.h"
#include "Helper.h"

Brazier::Brazier(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	toSaveName = "brazier";
	setType(typeOfObject);
}

void Brazier::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(600, 600);
}

Vector2i Brazier::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.4));
}

void Brazier::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(position.x - textureBox.width / 4, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 4, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.17);
	}
}

std::string Brazier::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/brazier/brazier" + std::to_string(typeOfObject) + ".png";
}