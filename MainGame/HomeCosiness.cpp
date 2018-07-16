#include "HomeCosiness.h"
#include "Helper.h"

HomeCosiness::HomeCosiness(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	intersectsRadius = 100;
	isBackground = true;
	toSaveName = "homeCosiness";
	zCoords = 2;
	setType(typeOfObject);
}

void HomeCosiness::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(500, 170);
}

Vector2i HomeCosiness::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void HomeCosiness::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(position.x - textureBox.width / 3, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 3, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.2);
	}
}

std::string HomeCosiness::getSpriteName(long long elapsedTime)
{
	return "terrainObjects/homeCosiness/homeCosiness" + std::to_string(typeOfObject) + ".png";
}