#include "MushroomsOnStone.h"
#include "Helper.h"

MushroomsOnStone::MushroomsOnStone(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	animationSpeed = 0.0004f;
	animationLength = 12;
	this->typeOfObject = typeOfObject;
	radius = 200;
	randomSizeIncrease = Vector2i(50, 50);
	toSaveName = "spawn";
	setType(typeOfObject);
}

void MushroomsOnStone::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2f(400 + rand() % randomSizeIncrease.x, 400 + rand() % randomSizeIncrease.x);
}

Vector2i MushroomsOnStone::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.4));
}

void MushroomsOnStone::initPedestal()
{
	focus1 = Vector2i(textureBox.left + int(textureBox.width / 4.4), int(position.y));
	focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 4.4), int(position.y));
	ellipseSize = int((focus2.x - focus1.x) * 1.15);
}

std::string MushroomsOnStone::getSpriteName(long long elapsedTime)
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
	return "terrainObjects/mushroomsOnStone/t2_" + std::to_string(currentSprite) + ".png";
}