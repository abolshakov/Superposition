#include "MushroomStone.h"
#include "Helper.h"

MushroomStone::MushroomStone(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	animationSpeed = 0.0004f;
	animationLength = 15;
	this->typeOfObject = typeOfObject;
	radius = 200;
	randomSizeIncrease = Vector2i(50, 50);
	toSaveName = "spawn";
	setType(typeOfObject);
}

void MushroomStone::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(800 + rand() % randomSizeIncrease.x, 800 + rand() % randomSizeIncrease.x);
}

Vector2i MushroomStone::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.2));
}

void MushroomStone::initPedestal()
{
	focus1 = Vector2i(textureBox.left + int(textureBox.width / 3.2), int(position.y));
	focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 4.4), int(position.y));
	ellipseSize = int((focus2.x - focus1.x) * 1.09);
}

std::string MushroomStone::getSpriteName(long long elapsedTime)
{
	timeForNewSprite += elapsedTime;

	if (timeForNewSprite >= 40 / animationSpeed)
	{
		timeForNewSprite = 0;

		if (++currentSprite > animationLength)
		{
			currentSprite = 1;
		}
	}
	return "terrainObjects/mushroomStone/t1_" + std::to_string(currentSprite) + ".png";
}