#include "HareTrap.h"
#include "Helper.h"

HareTrap::HareTrap(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	isBackground = true;
	toSaveName = "homeCosiness";
	zCoords = 10;
	setType(typeOfObject);
}

void HareTrap::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(300, 300);
}

Vector2i HareTrap::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void HareTrap::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(position.x - textureBox.width / 3, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 3, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.2);
	}
}

std::string HareTrap::getSpriteName(long long elapsedTime)
{
	return "Game/worldSprites/terrainObjects/hareTrap/hareTrap" + std::to_string(typeOfObject) + ".png";
}