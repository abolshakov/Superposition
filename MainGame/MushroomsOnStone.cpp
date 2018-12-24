#include "MushroomsOnStone.h"
#include "Helper.h"

MushroomsOnStone::MushroomsOnStone(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	animationSpeed = 0.0004f;
	animationLength = 12;
	this->typeOfObject = typeOfObject;
	radius = 200;
	randomSizeIncrease = Vector2i (50, 50);
	toSaveName = "spawn";
	setType(typeOfObject);
}

void MushroomsOnStone::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (400 + rand() % randomSizeIncrease.x, 400 + rand() % randomSizeIncrease.x);
}

Vector2i MushroomsOnStone::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width)*getScaleRatio().x);
	textureBox.height = int(float(textureBox.height)*getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.4));
}

void MushroomsOnStone::initPedestal()
{
	focus1 = Vector2f (textureBox.left + int(textureBox.width / 4.4), int(position.y));
	focus2 = Vector2f (textureBox.left + textureBox.width - int(textureBox.width / 4.4), int(position.y));
	ellipseSize = float((focus2.x - focus1.x) * 1.15);
}

Vector2f MushroomsOnStone::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int MushroomsOnStone::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
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
	return "Game/worldSprites/terrainObjects/mushroomsOnStone/t2_" + std::to_string(currentSprite) + ".png";
}